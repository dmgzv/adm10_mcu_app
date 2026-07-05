#include "tftp_bootloader.h"
#include "tftp_bootloader_mcu.h"
#include "tftp_bootloader_fpga.h"
#include "tftp_client.h"
#include "bootloader.h"
#include "profile_flash.h"
#include <string.h>
#include "log.h"

#define TFTP_BOOTLOADER_MSG_UPDATE				0

#define TFTP_BOOTLOADER_QUEUE_SIZE 10

/* Private function prototypes -----------------------------------------------*/
static void TFTP_BOOTLOADER_Thread(void const *argument);
/* System Thread definition */
osThreadDef(TFTP_BOOTLOADER, TFTP_BOOTLOADER_Thread, osPriorityNormal, 0, 512);
osMessageQDef(TFTP_BOOTLOADER, TFTP_BOOTLOADER_QUEUE_SIZE, uint32_t);
osMutexDef(TFTP_BOOTLOADER);

char TFTP_BOOTLOADER_InfoData[512];

TFTP_BOOTLOADER_COMPONENT_T *TFTP_BOOTLOADER_COMPONENTS[] = 
{
	&TFTP_BOOTLOADER_MCU,
};

TFTP_BOOTLOADER_T TFTP_BOOTLOADER = 
{
	.Info = {
		.Data = TFTP_BOOTLOADER_InfoData,
		.MaxSize = sizeof(TFTP_BOOTLOADER_InfoData),
	},
	.Components = TFTP_BOOTLOADER_COMPONENTS,
	.ComponentsCnt = sizeof(TFTP_BOOTLOADER_COMPONENTS) / sizeof(TFTP_BOOTLOADER_COMPONENTS[0]) 
};

extern BOOTLOADER_T Bootloader;
extern PROFILE_FLASH_T ProfileFlash;

static int32_t TFTP_BOOTLOADER_DownloadInfoReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{	
	if ((((Block - 1) * TFTP_DATA_LEN_MAX) + DataSize) < TFTP_BOOTLOADER.Info.MaxSize)
	{
		memcpy(&TFTP_BOOTLOADER.Info.Data[(Block - 1) * TFTP_DATA_LEN_MAX], Data, DataSize);
		TFTP_BOOTLOADER.Info.Data[((Block - 1) * TFTP_DATA_LEN_MAX) + DataSize] = 0;
		TFTP_BOOTLOADER.Info.Size += DataSize;
		return 0;
	}
	return -1;
}

static int32_t TFTP_BOOTLOADER_DownloadInfo(void)
{
	int32_t result;
	ip_addr_t ip_addr;
	
	LOG_Info("TFTP_BOOTLOADER", "", "", "Download Info");
	TFTP_BOOTLOADER.Info.Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD;
	IP_ADDR4(&ip_addr, TFTP_BOOTLOADER.Addr[0], TFTP_BOOTLOADER.Addr[1], TFTP_BOOTLOADER.Addr[2], TFTP_BOOTLOADER.Addr[3]);	
	result = TFTP_CLIENT_DownloadFile(TFTP_BOOTLOADER.Info.Name, &ip_addr, TFTP_BOOTLOADER_DownloadInfoReceiveCallback, NULL);
	if (result == TFTP_CLIENT_TIMEOUT)
	{
		LOG_Error("TFTP_BOOTLOADER", "", "", "Download Info: Timeout Error");
		TFTP_BOOTLOADER.Info.Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT;
		return -1;
	}
	else if (result != TFTP_CLIENT_OK)
	{
		LOG_Error("TFTP_BOOTLOADER", "", "", "Download Info: Size Error");
		TFTP_BOOTLOADER.Info.Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_ERROR;
		return -1;
	}
	
	LOG_Info("TFTP_BOOTLOADER", "", "", "Download Info: Complete");
	TFTP_BOOTLOADER.Info.Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_COMPLETE;
	return 0;
}

static int32_t TFTP_BOOTLOADER_VerifyInfo(void)
{
	uint32_t i;
	char *p;
	
	LOG_Info("TFTP_BOOTLOADER", "", "", "Verify Info");
	TFTP_BOOTLOADER.Info.Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY;
	
	p = strstr(TFTP_BOOTLOADER.Info.Data, "pId: ");
	if (!p)
	{
		LOG_Error("TFTP_BOOTLOADER", "", "", "pId: Not found");
		goto error;
	}
	p += sizeof("pId: ") - 1;
	if (memcmp(p, BSP_PT_NUMBER"\r\n", sizeof(BSP_PT_NUMBER"\r\n") - 1))
	{
		LOG_Error("TFTP_BOOTLOADER", "", "", "pId: Invalid value");
		goto error;
	}
	
	p = strstr(TFTP_BOOTLOADER.Info.Data, "HwNumber: ");
	if (!p)
	{
		LOG_Error("TFTP_BOOTLOADER", "", "", "HwNumber: Not found");
		goto error;
	}
	p += sizeof("HwNumber: ") - 1;
	if (memcmp(p, BSP_HW_VERSION"\r\n", sizeof(BSP_HW_VERSION"\r\n") - 1))
	{
		LOG_Error("TFTP_BOOTLOADER", "", "", "HwNumber: Invalid value");
		goto error;
	}

	for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
	{
		if (TFTP_BOOTLOADER.Components[i]->VerifyInfo(TFTP_BOOTLOADER.Info.Data))
			goto error;
	}
	
	LOG_Info("TFTP_BOOTLOADER", "", "", "Verify Info: Complete");
	TFTP_BOOTLOADER.Info.Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE;
	return 0;

error:
	LOG_Error("TFTP_BOOTLOADER", "", "", "Verify Info: Error");
	TFTP_BOOTLOADER.Info.Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY_ERROR;
	return -1;
}


static void TFTP_BOOTLOADER_Thread(void const *argument)
{
	uint32_t i, j;
	osEvent Event;
	uint32_t IdleCompleteCntr = 0;
	uint32_t DownloadCompleteCntr = 0;
	uint32_t UpdateCompleteCntr = 0;
	
	while(1)
  {
event_wait:
		Event = osMessageGet(TFTP_BOOTLOADER.MessageQId, osWaitForever);
		if (Event.status != osEventMessage)
			goto error;
		if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_ERROR)
			goto event_wait;
		
		if (Event.value.v == TFTP_BOOTLOADER_MSG_UPDATE)
		{
			if (TFTP_BOOTLOADER_DownloadInfo())
				goto error;
			if (TFTP_BOOTLOADER_VerifyInfo())
				goto error;
			
			IdleCompleteCntr = 0;
			for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
			{
				osMessagePut(TFTP_BOOTLOADER.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_IDLE(TFTP_BOOTLOADER_COMPONENTS[i]), 0);
			}
			goto event_wait;
		}
		
		for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
		{
			if (Event.value.v == TFTP_BOOTLOADER_COMPONENT_MSG_IDLE(TFTP_BOOTLOADER_COMPONENTS[i]))
			{
				TFTP_BOOTLOADER.Components[i]->ProcessEvent(Event.value.v);
				goto event_wait;
			}
			else if (Event.value.v == TFTP_BOOTLOADER_COMPONENT_MSG_IDLE_COMPLETE(TFTP_BOOTLOADER_COMPONENTS[i]))
			{
				IdleCompleteCntr++;
				if (IdleCompleteCntr == TFTP_BOOTLOADER.ComponentsCnt)
				{
					DownloadCompleteCntr = 0;
					LOG_Info("TFTP_BOOTLOADER", "", "", "Idle: Complete");
					for (j = 0; j < TFTP_BOOTLOADER.ComponentsCnt; j++)
					{
						osMessagePut(TFTP_BOOTLOADER.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_DOWNLOAD(TFTP_BOOTLOADER_COMPONENTS[j]), 0);
					}
				}
				goto event_wait;
			}
			else if (Event.value.v == TFTP_BOOTLOADER_COMPONENT_MSG_IDLE_ERROR(TFTP_BOOTLOADER_COMPONENTS[i]))
			{
				goto error;
			}
			else if (Event.value.v == TFTP_BOOTLOADER_COMPONENT_MSG_DOWNLOAD(TFTP_BOOTLOADER_COMPONENTS[i]))
			{
				TFTP_BOOTLOADER.Components[i]->ProcessEvent(Event.value.v);
				goto event_wait;
			}
			else if (Event.value.v == TFTP_BOOTLOADER_COMPONENT_MSG_DOWNLOAD_COMPLETE(TFTP_BOOTLOADER_COMPONENTS[i]))
			{
				DownloadCompleteCntr++;
				if (DownloadCompleteCntr == TFTP_BOOTLOADER.ComponentsCnt)
				{
					UpdateCompleteCntr = 0;
					LOG_Info("TFTP_BOOTLOADER", "", "", "Download: Complete");
					for (j = 0; j < TFTP_BOOTLOADER.ComponentsCnt; j++)
					{
						osMessagePut(TFTP_BOOTLOADER.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_UPDATE(TFTP_BOOTLOADER_COMPONENTS[j]), 0);
					}
				}
				goto event_wait;
			}
			else if (Event.value.v == TFTP_BOOTLOADER_COMPONENT_MSG_DOWNLOAD_ERROR(TFTP_BOOTLOADER_COMPONENTS[i]))
			{
				goto error;
			}
			else if (Event.value.v == TFTP_BOOTLOADER_COMPONENT_MSG_UPDATE(TFTP_BOOTLOADER_COMPONENTS[i]))
			{
				TFTP_BOOTLOADER.Components[i]->ProcessEvent(Event.value.v);
				goto event_wait;
			}
			else if (Event.value.v == TFTP_BOOTLOADER_COMPONENT_MSG_UPDATE_COMPLETE(TFTP_BOOTLOADER_COMPONENTS[i]))
			{
				UpdateCompleteCntr++;
				if (UpdateCompleteCntr == TFTP_BOOTLOADER.ComponentsCnt)
				{
					UpdateCompleteCntr = 0;
					LOG_Info("TFTP_BOOTLOADER", "", "", "Update: Complete");
					TFTP_BOOTLOADER.Status = TFTP_BOOTLOADER_STATUS_COMPLETE;
					osDelay(3000);
					BSP_Reset();
				}
				goto event_wait;
			}
		}
		
		LOG_Error("TFTP_BOOTLOADER", "", "", "Update: Unknown Event");
error:
		LOG_Error("TFTP_BOOTLOADER", "", "", "Update: Terminated");
		TFTP_BOOTLOADER.Status = TFTP_BOOTLOADER_STATUS_ERROR;
		goto event_wait;
	}
}

int32_t TFTP_BOOTLOADER_Init(void)
{
	if (!TFTP_BOOTLOADER.MutexId)
		TFTP_BOOTLOADER.MutexId = osMutexCreate (osMutex(TFTP_BOOTLOADER));
	if (!TFTP_BOOTLOADER.MessageQId)
		TFTP_BOOTLOADER.MessageQId = osMessageCreate (osMessageQ(TFTP_BOOTLOADER), NULL);
	if (!TFTP_BOOTLOADER.ThreadId)
		TFTP_BOOTLOADER.ThreadId = osThreadCreate (osThread(TFTP_BOOTLOADER), NULL);
	if ((!TFTP_BOOTLOADER.MessageQId) || (!TFTP_BOOTLOADER.ThreadId) || (!TFTP_BOOTLOADER.MutexId))
		return -1;
	return 0;
}

int32_t TFTP_BOOTLOADER_Update(char* Dir, char *Addr)
{
	uint32_t i, j;
	
	if ((!TFTP_BOOTLOADER.MessageQId) || (!TFTP_BOOTLOADER.ThreadId) || (!TFTP_BOOTLOADER.MutexId)  || (!Dir) || (!Addr))
		return -1;
	
	osMutexWait(TFTP_BOOTLOADER.MutexId, osWaitForever);
	
	if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_IN_PROGRESS)
		goto error;
	
	memcpy(TFTP_BOOTLOADER.Addr, Addr, sizeof(TFTP_BOOTLOADER.Addr));
	LOG_Info("TFTP_BOOTLOADER", "", "", "TFTP Addr: %d.%d.%d.%d", TFTP_BOOTLOADER.Addr[0], TFTP_BOOTLOADER.Addr[1], TFTP_BOOTLOADER.Addr[2], TFTP_BOOTLOADER.Addr[3]);	
	strncpy(TFTP_BOOTLOADER.Dir, Dir, sizeof(TFTP_BOOTLOADER.Dir) - 1);
	TFTP_BOOTLOADER.Dir[sizeof(TFTP_BOOTLOADER.Dir) - 1] = 0;
	LOG_Info("TFTP_BOOTLOADER", "", "", "TFTP Direrctory: %s", TFTP_BOOTLOADER.Dir);
	
	strcpy(TFTP_BOOTLOADER.Info.Name, TFTP_BOOTLOADER.Dir);
	strcat(TFTP_BOOTLOADER.Info.Name, "/info.yaml");
	TFTP_BOOTLOADER.Info.Size = 0;
	TFTP_BOOTLOADER.Info.Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD;
	LOG_Info("TFTP_BOOTLOADER", "", "", "Info: %s", TFTP_BOOTLOADER.Info.Name);
	
	for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
	{
		TFTP_BOOTLOADER.Components[i]->Status = TFTP_BOOTLOADER_STATUS_IDLE;
		TFTP_BOOTLOADER.Components[i]->Addr = TFTP_BOOTLOADER.Addr;
		TFTP_BOOTLOADER.Components[i]->Dir = TFTP_BOOTLOADER.Dir;
		TFTP_BOOTLOADER.Components[i]->Version[0] = 0;
		TFTP_BOOTLOADER.Components[i]->MessageQId = TFTP_BOOTLOADER.MessageQId;
		for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
		{
			TFTP_BOOTLOADER.Components[i]->Files[j].Name[0] = 0;
			TFTP_BOOTLOADER.Components[i]->Files[j].Size = 0;
			TFTP_BOOTLOADER.Components[i]->Files[j].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE;
		}
	}
	
	TFTP_BOOTLOADER.Status = TFTP_BOOTLOADER_STATUS_IN_PROGRESS;
		
	osMessagePut(TFTP_BOOTLOADER.MessageQId, TFTP_BOOTLOADER_MSG_UPDATE, 0);
	
	osMutexRelease(TFTP_BOOTLOADER.MutexId);	
	return 0;
	
error:
	osMutexRelease(TFTP_BOOTLOADER.MutexId);	
	return -1;
}
