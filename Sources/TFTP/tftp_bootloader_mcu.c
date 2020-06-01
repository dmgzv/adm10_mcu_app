#include "tftp_bootloader_mcu.h"
#include "tftp_client.h"
#include "bootloader.h"
#include "profile_flash.h"
#include <string.h>
#include "log.h"

#define TFTP_BOOTLOADER_MCU_FILE_WEB			0
#define TFTP_BOOTLOADER_MCU_FILE_LANGPACK	1
#define TFTP_BOOTLOADER_MCU_FILE_FIRMWARE	2
#define TFTP_BOOTLOADER_MCU_FILES_CNT 3

static void TFTP_BOOTLOADER_MCU_ProcessEvent(uint32_t Event);
static int32_t TFTP_BOOTLOADER_MCU_VerifyInfo(void);

extern BOOTLOADER_T Bootloader;
extern PROFILE_FLASH_T ProfileFlash;

TFTP_BOOTLOADER_FILE_T TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILES_CNT];

TFTP_BOOTLOADER_COMPONENT_T TFTP_BOOTLOADER_MCU = 
{
	.Files = TFTP_BOOTLOADER_MCU_Files,
	.FilesCnt = sizeof(TFTP_BOOTLOADER_MCU_Files) / sizeof(TFTP_BOOTLOADER_MCU_Files[0]),
	.ProcessEvent = TFTP_BOOTLOADER_MCU_ProcessEvent,
	.VerifyInfo = TFTP_BOOTLOADER_MCU_VerifyInfo,
};

static int32_t TFTP_BOOTLOADER_MCU_VerifyInfo(void)
{
	char *p, *c;
	uint32_t i, len;
	
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Verify info");
	
	p = strstr(TFTP_BOOTLOADER_MCU.Info->Data, "MCUSwNumber: ");
	if (!p)
	{
		LOG_Error("TFTP_BOOTLOADER", "MCU", "", "MCUSwNumber: Not found");
		goto error;
	}
	p += sizeof("MCUSwNumber: ") - 1;
	c = strstr(p, "\r\n");
	if (!c)
	{
		LOG_Error("TFTP_BOOTLOADER", "MCU", "", "MCUSwNumber: Invalid syntax");
		goto error;
	}
	memcpy(&TFTP_BOOTLOADER_MCU.Version, p, c - p);
	TFTP_BOOTLOADER_MCU.Version[c - p] = 0;
	
	strcpy(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Name, TFTP_BOOTLOADER_MCU.Dir);
	strcat(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Name, "/MCU/"BSP_PROJECT"__MCU__Web__");
	len = strlen(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Name);
	memcpy(&TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Name[len], p, c - p);
	for (i = 0; i < (c - p); i++)
	{
		if (TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Name[len + i] == '.')
			TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Name[len + i] = '_';
	}
	len += c - p;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Name[len] = 0;
	strcat(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Name, ".bin");
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Size = 0;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].MaxSize = BSP_FLASH_WEB_SIZE;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Data = NULL;
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Web: %s", TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Name);
	
	strcpy(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Name, TFTP_BOOTLOADER_MCU.Dir);
	strcat(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Name, "/MCU/"BSP_PROJECT"__MCU__Langpack__");
	len = strlen(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Name);
	memcpy(&TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Name[len], p, c - p);
	for (i = 0; i < (c - p); i++)
	{
		if (TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Name[len + i] == '.')
			TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Name[len + i] = '_';
	}
	len += c - p;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Name[len] = 0;
	strcat(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Name, ".lng");
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Size = 0;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].MaxSize = BSP_FLASH_LANG_SIZE;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Data = NULL;
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Langpack: %s", TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Name);
	
	strcpy(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Name, TFTP_BOOTLOADER_MCU.Dir);
	strcat(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Name, "/MCU/"BSP_PROJECT"__MCU__Release__");
	len = strlen(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Name);
	memcpy(&TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Name[len], p, c - p);
	for (i = 0; i < (c - p); i++)
	{
		if (TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Name[len + i] == '.')
			TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Name[len + i] = '_';
	}
	len += c - p;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Name[len] = 0;
	strcat(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Name, ".bin");
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Size = 0;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].MaxSize = BSP_FLASH_FW_SIZE;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Data = NULL;
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Firmware: %s", TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Name);
	
	TFTP_BOOTLOADER_MCU.FilesCnt = TFTP_BOOTLOADER_MCU_FILES_CNT;	
	return 0;
	
error:
	return -1;
}

static void TFTP_BOOTLOADER_MCU_DownloadWebTimeoutCallback(void *Priv)
{
	LOG_Error("TFTP_BOOTLOADER", "MCU", "", "Download Web: Timeout");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT_ERROR;
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_MCU_MSG_DOWNLOAD_ERROR, 0);
}

static int TFTP_BOOTLOADER_MCU_DownloadWebReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{
	uint32_t Addr;
	uint32_t BlockSize;
	uint32_t Size;
	uint32_t NextEvent = (uint32_t)Priv;
	
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Size += DataSize;	
	if ((((Block - 1) * TFTP_DATA_LEN_MAX) + DataSize) < BSP_FLASH_WEB_SIZE)
	{
		Addr = BSP_FLASH_WEB_ADDR + ((Block - 1) * TFTP_DATA_LEN_MAX);
		BlockSize = DataSize;	
		while (DataSize)
		{
			if (DataSize > BSP_FLASH_PAGE_SIZE)
				Size = BSP_FLASH_PAGE_SIZE;
			else
				Size = DataSize;
			if (BSP_FLASH_WaitReady())
				goto error;
			if (BSP_FLASH_ProgramPage(Addr, Data, Size))
				goto error;
			Addr += Size;
			Data += Size;
			DataSize -= Size;
		}
		
		if (BlockSize != TFTP_DATA_LEN_MAX)
		{
			LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Download Web: %d bytes", TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Size);
			TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY;
			osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, NextEvent, 0);
		}
		return 0;
	}
error:
	LOG_Error("TFTP_BOOTLOADER", "MCU", "", "Download Web: Invalid file size");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_SIZE_ERROR;
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_MCU_MSG_DOWNLOAD_ERROR, 0);
	return -1;
}

static void TFTP_BOOTLOADER_MCU_DownloadWeb(uint32_t NextEvent)
{
	ip_addr_t ip_addr;
	
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Download Web");
	IP_ADDR4(&ip_addr, TFTP_BOOTLOADER_MCU.Addr[0], TFTP_BOOTLOADER_MCU.Addr[1], TFTP_BOOTLOADER_MCU.Addr[2], TFTP_BOOTLOADER_MCU.Addr[3]);	
	TFTP_CLIENT_DownloadFile(TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Name, &ip_addr, TFTP_BOOTLOADER_MCU_DownloadWebReceiveCallback, TFTP_BOOTLOADER_MCU_DownloadWebTimeoutCallback, (void*)NextEvent);
}

static void TFTP_BOOTLOADER_MCU_VerifyWeb(uint32_t NextEvent)
{
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Verify Web");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_COMPLETE;
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, NextEvent, 0);	
}

static void TFTP_BOOTLOADER_MCU_DownloadLangpackTimeoutCallback(void *Priv)
{
	LOG_Error("TFTP_BOOTLOADER", "MCU", "", "Download Langpack: Timeout");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT_ERROR;
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_MCU_MSG_DOWNLOAD_ERROR, 0);
}

static int TFTP_BOOTLOADER_MCU_DownloadLangpackReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{
	uint32_t DataAddr;
	uint32_t TotalDataSize;
	uint32_t NextEvent = (uint32_t)Priv;
	
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Size += DataSize;
	if ((((Block - 1) * TFTP_DATA_LEN_MAX) + DataSize) < BSP_FLASH_LANG_SIZE)
	{
		DataAddr = (Block - 1) * TFTP_DATA_LEN_MAX;	
		if (PROFILE_FLASH_SaveLangpackData(&ProfileFlash, DataAddr, Data, DataSize))
			goto error;
		if (DataSize != TFTP_DATA_LEN_MAX)
		{
			LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Download Langpack: %d bytes", TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Size);
			TotalDataSize = DataSize + ((Block - 1) * TFTP_DATA_LEN_MAX);
			if (PROFILE_FLASH_SaveLangpackHeader(&ProfileFlash, TotalDataSize))
				goto error;
			TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY;
			osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, NextEvent, 0);
		}
		return 0;
	}
error:
	LOG_Error("TFTP_BOOTLOADER", "MCU", "", "Download Langpack: Invalid file size");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_SIZE_ERROR;
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_MCU_MSG_DOWNLOAD_ERROR, 0);
	return -1;
}

static void TFTP_BOOTLOADER_MCU_DownloadLangpack(uint32_t NextEvent)
{
	ip_addr_t ip_addr;
	
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Download Langpack");
	IP_ADDR4(&ip_addr, TFTP_BOOTLOADER_MCU.Addr[0], TFTP_BOOTLOADER_MCU.Addr[1], TFTP_BOOTLOADER_MCU.Addr[2], TFTP_BOOTLOADER_MCU.Addr[3]);	
	TFTP_CLIENT_DownloadFile(TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Name, &ip_addr, TFTP_BOOTLOADER_MCU_DownloadLangpackReceiveCallback, TFTP_BOOTLOADER_MCU_DownloadLangpackTimeoutCallback, (void*)NextEvent);
}

static void TFTP_BOOTLOADER_MCU_VerifyLangpack(uint32_t NextEvent)
{
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Verify Langpack");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_COMPLETE;
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, NextEvent, 0);	
}

static void TFTP_BOOTLOADER_MCU_DownloadFirmwareTimeoutCallback(void *Priv)
{
	LOG_Error("TFTP_BOOTLOADER", "MCU", "", "Download Firmware: Timeout");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT_ERROR;
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_MCU_MSG_DOWNLOAD_ERROR, 0);
}

static int32_t TFTP_BOOTLOADER_MCU_DownloadFirmwareReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{
	uint32_t NextEvent = (uint32_t)Priv;
	
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Size += DataSize;
	if (BOOTLOADER_UpdateNewFwBlock(&Bootloader, Data, DataSize))
		goto error;
	if (DataSize != TFTP_DATA_LEN_MAX)
	{
		LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Download Firmware: %d bytes", TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Size);
		TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY;
		osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, NextEvent, 0);
	}
	return 0;
	
	error:
	LOG_Error("TFTP_BOOTLOADER", "MCU", "", "Download Firmware: Invalid file size");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_SIZE_ERROR;
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_MCU_MSG_DOWNLOAD_ERROR, 0);
	return -1;
}

static void TFTP_BOOTLOADER_MCU_DownloadFirmware(uint32_t NextEvent)
{
	ip_addr_t ip_addr;
	
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Download Firmware");
	IP_ADDR4(&ip_addr, TFTP_BOOTLOADER_MCU.Addr[0], TFTP_BOOTLOADER_MCU.Addr[1], TFTP_BOOTLOADER_MCU.Addr[2], TFTP_BOOTLOADER_MCU.Addr[3]);
	BOOTLOADER_UpdateNewFwStart(&Bootloader);
	TFTP_CLIENT_DownloadFile(TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Name, &ip_addr, TFTP_BOOTLOADER_MCU_DownloadFirmwareReceiveCallback, TFTP_BOOTLOADER_MCU_DownloadFirmwareTimeoutCallback, (void*)NextEvent);
}

static void TFTP_BOOTLOADER_MCU_VerifyFirmware(uint32_t NextEvent)
{
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Verify Firmware");
	if (BOOTLOADER_UpdateNewFwFinish(&Bootloader))
		goto error;
	if (BOOTLOADER_CheckNewFw(&Bootloader))
		goto error;
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Status = TFTP_BOOTLOADER_FILE_STATUS_COMPLETE;
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, NextEvent, 0);
	return;
error:
	LOG_Error("TFTP_BOOTLOADER", "MCU", "", "Verify Firmware: Error");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_FIRMWARE].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY_ERROR;
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_MCU_MSG_DOWNLOAD_ERROR, 0);
}

static void TFTP_BOOTLOADER_MCU_Download(uint32_t NextEvent)
{
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Download");
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, NextEvent, 0);
	return;	
}

static void TFTP_BOOTLOADER_MCU_Update(void)
{
	LOG_Info("TFTP_BOOTLOADER", "MCU", "", "Update");
	BOOTLOADER_UpdateNewFw(&Bootloader);
	BSP_Reset();
}

static void TFTP_BOOTLOADER_MCU_ProcessEvent(uint32_t Event)
{
	switch (Event)
	{
		case TFTP_BOOTLOADER_MCU_MSG_DOWNLOAD:
			TFTP_BOOTLOADER_MCU_Download(TFTP_BOOTLOADER_MCU_MSG_WEB_DOWNLOAD);
			break;
		case TFTP_BOOTLOADER_MCU_MSG_WEB_DOWNLOAD:
			TFTP_BOOTLOADER_MCU_DownloadWeb(TFTP_BOOTLOADER_MCU_MSG_WEB_VERIFY);
			break;
		case TFTP_BOOTLOADER_MCU_MSG_WEB_VERIFY:
			TFTP_BOOTLOADER_MCU_VerifyWeb(TFTP_BOOTLOADER_MCU_MSG_LANGPACK_DOWNLOAD);
			break;
		case TFTP_BOOTLOADER_MCU_MSG_LANGPACK_DOWNLOAD:
			TFTP_BOOTLOADER_MCU_DownloadLangpack(TFTP_BOOTLOADER_MCU_MSG_LANGPACK_VERIFY);
			break;
		case TFTP_BOOTLOADER_MCU_MSG_LANGPACK_VERIFY:
			TFTP_BOOTLOADER_MCU_VerifyLangpack(TFTP_BOOTLOADER_MCU_MSG_FIRMWARE_DOWNLOAD);
			break;
		case TFTP_BOOTLOADER_MCU_MSG_FIRMWARE_DOWNLOAD:
			TFTP_BOOTLOADER_MCU_DownloadFirmware(TFTP_BOOTLOADER_MCU_MSG_FIRMWARE_VERIFY);
			break;
		case TFTP_BOOTLOADER_MCU_MSG_FIRMWARE_VERIFY:
			TFTP_BOOTLOADER_MCU_VerifyFirmware(TFTP_BOOTLOADER_MCU_MSG_DOWNLOAD_COMPLETE);
			break;
		case TFTP_BOOTLOADER_MCU_MSG_UPDATE:
			TFTP_BOOTLOADER_MCU_Update();
			break;
	}
}
