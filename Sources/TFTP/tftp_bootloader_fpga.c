#include "tftp_bootloader_fpga.h"
#include "tftp_client.h"
#include "bootloader.h"
#include "profile_flash.h"
#include <string.h>
#include "log.h"

#define TFTP_BOOTLOADER_FPGA_FILE_IMAGE_MAX_SIZE	0x2000000

#define TFTP_BOOTLOADER_FPGA_FILE_IMAGE_1	0
#define TFTP_BOOTLOADER_FPGA_FILE_IMAGE_2	1
#define TFTP_BOOTLOADER_FPGA_FILES_CNT 	2

static void TFTP_BOOTLOADER_FPGA_ProcessEvent(uint32_t Event);
static int32_t TFTP_BOOTLOADER_FPGA_VerifyInfo(char *Data);

TFTP_BOOTLOADER_FILE_T TFTP_BOOTLOADER_FPGA_Files[TFTP_BOOTLOADER_FPGA_FILES_CNT];

TFTP_BOOTLOADER_COMPONENT_T TFTP_BOOTLOADER_FPGA = 
{
	.MessageBase = TFTP_BOOTLOADER_FPGA_MSG_BASE,
	.Files = TFTP_BOOTLOADER_FPGA_Files,
	.FilesCnt = sizeof(TFTP_BOOTLOADER_FPGA_Files) / sizeof(TFTP_BOOTLOADER_FPGA_Files[0]),
	.ProcessEvent = TFTP_BOOTLOADER_FPGA_ProcessEvent,
	.VerifyInfo = TFTP_BOOTLOADER_FPGA_VerifyInfo,
};

static int32_t TFTP_BOOTLOADER_FPGA_VerifyInfo(char *Data)
{
	char *p, *c;
	uint32_t i, j, len;
	
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Verify info");
	
	p = strstr(Data, "FPGASwNumber: ");
	if (!p)
	{
		LOG_Error("TFTP_BOOTLOADER", "FPGA", "", "FPGASwNumber: Not found");
		goto error;
	}
	p += sizeof("FPGASwNumber: ") - 1;
	c = strstr(p, "\r\n");
	if (!c)
	{
		LOG_Error("TFTP_BOOTLOADER", "FPGA", "", "FPGASwNumber: Invalid syntax");
		goto error;
	}
	memcpy(&TFTP_BOOTLOADER_FPGA.Version, p, c - p);
	TFTP_BOOTLOADER_FPGA.Version[c - p] = 0;
	
	for (i = 0; i < TFTP_BOOTLOADER_FPGA_FILES_CNT; i++)
	{
		strcpy(TFTP_BOOTLOADER_FPGA.Files[i].Name, TFTP_BOOTLOADER_FPGA.Dir);
		strcat(TFTP_BOOTLOADER_FPGA.Files[i].Name, "/FPGA/"BSP_PROJECT"__FPGA__Update_");
		len = strlen(TFTP_BOOTLOADER_FPGA.Files[i].Name);
		TFTP_BOOTLOADER_FPGA.Files[i].Name[len++] = 0x30 + i + 1;
		TFTP_BOOTLOADER_FPGA.Files[i].Name[len++] = '_';
		TFTP_BOOTLOADER_FPGA.Files[i].Name[len++] = '_';
		memcpy(&TFTP_BOOTLOADER_FPGA.Files[i].Name[len], p, c - p);
		for (j = 0; j < (c - p); j++)
		{
			if (TFTP_BOOTLOADER_FPGA.Files[i].Name[len + j] == '.')
				TFTP_BOOTLOADER_FPGA.Files[i].Name[len + j] = '_';
		}
		len += c - p;
		TFTP_BOOTLOADER_FPGA.Files[i].Name[len] = 0;
		strcat(TFTP_BOOTLOADER_FPGA.Files[i].Name, ".bin");
		TFTP_BOOTLOADER_FPGA.Files[i].Size = 0;
		TFTP_BOOTLOADER_FPGA.Files[i].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE;
		TFTP_BOOTLOADER_FPGA.Files[i].MaxSize = TFTP_BOOTLOADER_FPGA_FILE_IMAGE_MAX_SIZE;
		TFTP_BOOTLOADER_FPGA.Files[i].Data = NULL;
		LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Update %d: %s", i + 1, TFTP_BOOTLOADER_FPGA_Files[i].Name);
	}
	
	TFTP_BOOTLOADER_FPGA.FilesCnt = TFTP_BOOTLOADER_FPGA_FILES_CNT;	
	return 0;
	
error:
	return -1;
}

static int32_t TFTP_BOOTLOADER_FPGA_IdleUpdate(uint32_t Idx)
{
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Idle Update %d", Idx);
	TFTP_BOOTLOADER_FPGA_Files[Idx].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE;
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Idle Update %d: Complete", Idx);
	TFTP_BOOTLOADER_FPGA_Files[Idx].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE_COMPLETE;
	return 0;
}

static int TFTP_BOOTLOADER_FPGA_DownloadUpdateReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{
	uint32_t Idx;
	Idx = (uint32_t)Priv;
	if (Idx >= TFTP_BOOTLOADER_FPGA_FILES_CNT)
		return -1;
	if (DataSize)
	{
		TFTP_BOOTLOADER_FPGA_Files[Idx].Size += DataSize;
		if (BSP_FPGA_FLASH_WriteSection(BSP_FLASH_FPGA_UPDATE_SECTION, (TFTP_BOOTLOADER_FPGA_FILE_IMAGE_MAX_SIZE * Idx) + ((Block - 1) * TFTP_DATA_LEN_MAX), Data, DataSize))
			return -1;
	}
	return 0;
}

static int32_t TFTP_BOOTLOADER_FPGA_DownloadUpdate(uint32_t Idx)
{
	int32_t result;
	ip_addr_t ip_addr;
	
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Download Update %d", Idx + 1);
	IP_ADDR4(&ip_addr, TFTP_BOOTLOADER_FPGA.Addr[0], TFTP_BOOTLOADER_FPGA.Addr[1], TFTP_BOOTLOADER_FPGA.Addr[2], TFTP_BOOTLOADER_FPGA.Addr[3]);
	result = TFTP_CLIENT_DownloadFile(TFTP_BOOTLOADER_FPGA_Files[Idx].Name, &ip_addr, TFTP_BOOTLOADER_FPGA_DownloadUpdateReceiveCallback, (void*)Idx);
	if (result == TFTP_CLIENT_TIMEOUT)
	{
		LOG_Error("TFTP_BOOTLOADER", "FPGA", "", "Download Update %d: Timeout", Idx);
		TFTP_BOOTLOADER_FPGA_Files[Idx].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT;
		return -1;
	}
	else if (result != TFTP_CLIENT_OK)
	{
		LOG_Error("TFTP_BOOTLOADER", "FPGA", "", "Download Update %d: Error", Idx);
		TFTP_BOOTLOADER_FPGA_Files[Idx].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_ERROR;
		return -1;
	}	
	
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Download Update %d: Complete", Idx);
	TFTP_BOOTLOADER_FPGA_Files[Idx].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_COMPLETE;
	return 0;
}

static int32_t TFTP_BOOTLOADER_FPGA_VerifyUpdate(uint32_t Idx)
{
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Verify Update %d", Idx + 1);
	TFTP_BOOTLOADER_FPGA_Files[Idx].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY;	
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Verify Update %d: Complete", Idx + 1);
	TFTP_BOOTLOADER_FPGA_Files[Idx].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE;
	return 0;
}

static void TFTP_BOOTLOADER_FPGA_Idle(void)
{
	uint32_t i;
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Idle");
	TFTP_BOOTLOADER_FPGA.Status = TFTP_BOOTLOADER_STATUS_IDLE;
	for (i = 0; i < TFTP_BOOTLOADER_FPGA_FILES_CNT; i++)
	{
		if (TFTP_BOOTLOADER_FPGA_IdleUpdate(i))
		{
			goto error;
		}
	}
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Idle: Complete");
	osMessagePut(TFTP_BOOTLOADER_FPGA.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_IDLE_COMPLETE(&TFTP_BOOTLOADER_FPGA), 0);
	return;

error:
	LOG_Error("TFTP_BOOTLOADER", "FPGA", "", "Idle: Error");
	TFTP_BOOTLOADER_FPGA.Status = TFTP_BOOTLOADER_STATUS_ERROR;
	osMessagePut(TFTP_BOOTLOADER_FPGA.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_IDLE_ERROR(&TFTP_BOOTLOADER_FPGA), 0);
	return;
}

static void TFTP_BOOTLOADER_FPGA_Download(void)
{
	uint32_t i;
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Download");
	TFTP_BOOTLOADER_FPGA.Status = TFTP_BOOTLOADER_STATUS_IN_PROGRESS;
	for (i = 0; i < TFTP_BOOTLOADER_FPGA_FILES_CNT; i++)
	{
		if (TFTP_BOOTLOADER_FPGA_DownloadUpdate(i))
		{
			goto error;
		}
		if (TFTP_BOOTLOADER_FPGA_VerifyUpdate(i))
		{
			goto error;
		}
	}
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Download: Complete");
	osMessagePut(TFTP_BOOTLOADER_FPGA.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_DOWNLOAD_COMPLETE(&TFTP_BOOTLOADER_FPGA), 0);
	return;

error:
	LOG_Error("TFTP_BOOTLOADER", "FPGA", "", "Download: Error");
	TFTP_BOOTLOADER_FPGA.Status = TFTP_BOOTLOADER_STATUS_ERROR;
	osMessagePut(TFTP_BOOTLOADER_FPGA.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_DOWNLOAD_ERROR(&TFTP_BOOTLOADER_FPGA), 0);
	return;
}

static void TFTP_BOOTLOADER_FPGA_Update(void)
{
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Update");
	BSP_FPGA_FLASH_EraseSection(BSP_FLASH_FPGA_BOOT_SECTION);
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Update: Complete");
	TFTP_BOOTLOADER_FPGA.Status = TFTP_BOOTLOADER_STATUS_COMPLETE;
	osMessagePut(TFTP_BOOTLOADER_FPGA.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_UPDATE_COMPLETE(&TFTP_BOOTLOADER_FPGA), 0);
}

static void TFTP_BOOTLOADER_FPGA_ProcessEvent(uint32_t Event)
{
	if (Event == TFTP_BOOTLOADER_COMPONENT_MSG_IDLE(&TFTP_BOOTLOADER_FPGA))
		TFTP_BOOTLOADER_FPGA_Idle();
	else if (Event == TFTP_BOOTLOADER_COMPONENT_MSG_DOWNLOAD(&TFTP_BOOTLOADER_FPGA))
		TFTP_BOOTLOADER_FPGA_Download();
	else if (Event == TFTP_BOOTLOADER_COMPONENT_MSG_UPDATE(&TFTP_BOOTLOADER_FPGA))
		TFTP_BOOTLOADER_FPGA_Update();
}
