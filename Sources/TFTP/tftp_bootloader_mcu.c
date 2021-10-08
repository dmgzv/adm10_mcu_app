#include "tftp_bootloader_mcu.h"
#include "tftp_client.h"
#include "bootloader.h"
#include "profile_flash.h"
#include "http_server_files.h"
#include <string.h>
#include "log.h"

#define TFTP_BOOTLOADER_MCU_FILE_WEB			0
#define TFTP_BOOTLOADER_MCU_FILE_LANGPACK	1
#define TFTP_BOOTLOADER_MCU_FILE_UPDATE	2
#define TFTP_BOOTLOADER_MCU_FILES_CNT 3

static void TFTP_BOOTLOADER_MCU_ProcessEvent(uint32_t Event);
static int32_t TFTP_BOOTLOADER_MCU_VerifyInfo(char *Data);

extern BOOTLOADER_T Bootloader;
extern PROFILE_FLASH_T ProfileFlash;

TFTP_BOOTLOADER_FILE_T TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILES_CNT];

TFTP_BOOTLOADER_COMPONENT_T TFTP_BOOTLOADER_MCU = 
{
	.MessageBase = TFTP_BOOTLOADER_MCU_MSG_BASE,
	.Files = TFTP_BOOTLOADER_MCU_Files,
	.FilesCnt = sizeof(TFTP_BOOTLOADER_MCU_Files) / sizeof(TFTP_BOOTLOADER_MCU_Files[0]),
	.ProcessEvent = TFTP_BOOTLOADER_MCU_ProcessEvent,
	.VerifyInfo = TFTP_BOOTLOADER_MCU_VerifyInfo,
};

static int32_t TFTP_BOOTLOADER_MCU_VerifyInfo(char *Data)
{
	char *p, *c;
	uint32_t i, len;
	
	LOG_Info("TFTP_BLDR_MCU", "", "InfoVerifyStart", "");
	
	p = strstr(Data, "MCUSwNumber: ");
	if (!p)
	{
		LOG_Error("TFTP_BLDR_MCU", "", "InfoVerifyError", "MCUSwNumber not found");
		goto error;
	}
	p += sizeof("MCUSwNumber: ") - 1;
	c = strstr(p, "\r\n");
	if (!c)
	{
		LOG_Error("TFTP_BLDR_MCU", "", "InfoVerifyError", "MCUSwNumber invalid syntax");
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
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].MaxSize = HTTP_SERVER_FILES_RECORD_BODY_SIZE;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Data = NULL;
	
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
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].MaxSize = PROFILE_FLASH_LANG_RECORD_BODY_SIZE;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Data = NULL;
	
	strcpy(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Name, TFTP_BOOTLOADER_MCU.Dir);
	strcat(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Name, "/MCU/"BSP_PROJECT"__MCU__Update__");
	len = strlen(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Name);
	memcpy(&TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Name[len], p, c - p);
	for (i = 0; i < (c - p); i++)
	{
		if (TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Name[len + i] == '.')
			TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Name[len + i] = '_';
	}
	len += c - p;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Name[len] = 0;
	strcat(TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Name, ".bin");
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Size = 0;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].MaxSize = BSP_FLASH_FW_SIZE;
	TFTP_BOOTLOADER_MCU.Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Data = NULL;
	
	TFTP_BOOTLOADER_MCU.FilesCnt = TFTP_BOOTLOADER_MCU_FILES_CNT;
	
	LOG_Info("TFTP_BLDR_MCU", "", "InfoVerifyComplete", "");
	return 0;
	
error:
	return -1;
}

static int32_t TFTP_BOOTLOADER_MCU_IdleWeb(void)
{
	LOG_Info("TFTP_BLDR_MCU", "", "WebIdleStart", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE;	
	LOG_Info("TFTP_BLDR_MCU", "", "WebIdleComplete", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE_COMPLETE;
	return 0;	
}

static int TFTP_BOOTLOADER_MCU_DownloadWebReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Size += DataSize;
	if (HTTP_SERVER_FILES_WriteData(((Block - 1) * TFTP_DATA_LEN_MAX), Data, DataSize))
		return -1;
	return 0;
}

static int32_t TFTP_BOOTLOADER_MCU_DownloadWeb(void)
{
	int32_t result;
	ip_addr_t ip_addr;
	
	LOG_Info("TFTP_BLDR_MCU", "", "WebDownloadStart", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD;
	IP_ADDR4(&ip_addr, TFTP_BOOTLOADER_MCU.Addr[0], TFTP_BOOTLOADER_MCU.Addr[1], TFTP_BOOTLOADER_MCU.Addr[2], TFTP_BOOTLOADER_MCU.Addr[3]);	
	result = TFTP_CLIENT_DownloadFile(TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Name, &ip_addr, TFTP_BOOTLOADER_MCU_DownloadWebReceiveCallback, NULL);
	if (result == TFTP_CLIENT_TIMEOUT)
	{
		LOG_Error("TFTP_BLDR_MCU", "", "WebDownloadTimeout", "");
		TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT;
		return -1;
	}
	else if (result != TFTP_CLIENT_OK)
	{
		LOG_Error("TFTP_BLDR_MCU", "", "WebDownloadError", "");
		TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_ERROR;
		return -1;
	}
	
	LOG_Info("TFTP_BLDR_MCU", "", "WebDownloadComplete", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD;
	return 0;
}

static int32_t TFTP_BOOTLOADER_MCU_VerifyWeb(void)
{
	LOG_Info("TFTP_BLDR_MCU", "", "WebVerifyStart", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY;	
	LOG_Info("TFTP_BLDR_MCU", "", "WebVerifyComplete", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE;
	return 0;	
}

static int32_t TFTP_BOOTLOADER_MCU_IdleLangpack(void)
{
	LOG_Info("TFTP_BLDR_MCU", "", "LangpackIdleStart", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE;	
	LOG_Info("TFTP_BLDR_MCU", "", "LangpackIdleComplete", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE_COMPLETE;
	return 0;	
}

static int TFTP_BOOTLOADER_MCU_DownloadLangpackReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Size += DataSize;
	if (PROFILE_FLASH_UpdateLangpackData(&ProfileFlash, (Block - 1) * TFTP_DATA_LEN_MAX, Data, DataSize))
		return -1;
	return 0;
}

static int32_t TFTP_BOOTLOADER_MCU_DownloadLangpack(void)
{
	int32_t result;
	ip_addr_t ip_addr;
	
	LOG_Info("TFTP_BLDR_MCU", "", "LangpackDownloadStart", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD;
	IP_ADDR4(&ip_addr, TFTP_BOOTLOADER_MCU.Addr[0], TFTP_BOOTLOADER_MCU.Addr[1], TFTP_BOOTLOADER_MCU.Addr[2], TFTP_BOOTLOADER_MCU.Addr[3]);	
	result = TFTP_CLIENT_DownloadFile(TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Name, &ip_addr, TFTP_BOOTLOADER_MCU_DownloadLangpackReceiveCallback, NULL);
	if (result == TFTP_CLIENT_TIMEOUT)
	{
		LOG_Error("TFTP_BLDR_MCU", "", "LangpackDownloadTimeout", "");
		TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT;
		return -1;
	}
	else if (result != TFTP_CLIENT_OK)
	{
		LOG_Error("TFTP_BLDR_MCU", "", "LangpackDownloadError", "");
		TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_ERROR;
		return -1;
	}
	
	LOG_Info("TFTP_BLDR_MCU", "", "LangpackDownloadComplete", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_COMPLETE;
	return 0;
}

static int32_t TFTP_BOOTLOADER_MCU_VerifyLangpack()
{
	LOG_Info("TFTP_BLDR_MCU", "", "LangpackVerifyStart", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY;	
	LOG_Info("TFTP_BLDR_MCU", "", "LangpackVerifyComplete", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE;
	return 0;
}

static int32_t TFTP_BOOTLOADER_MCU_IdleUpdate(void)
{
	LOG_Info("TFTP_BLDR_MCU", "", "UpdateIdleStart", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE;	
	LOG_Info("TFTP_BLDR_MCU", "", "UpdateIdleComplete", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Status = TFTP_BOOTLOADER_FILE_STATUS_IDLE_COMPLETE;
	return 0;	
}

static int32_t TFTP_BOOTLOADER_MCU_DownloadUpdateReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Size += DataSize;
	if (BOOTLOADER_UpdateNewFwBlock(&Bootloader, Data, DataSize))
		goto error;
	return 0;	
error:
	return -1;
}

static int32_t TFTP_BOOTLOADER_MCU_DownloadUpdate(void)
{
	int32_t result;
	ip_addr_t ip_addr;
	
	LOG_Info("TFTP_BLDR_MCU", "", "UpdateDownloadStart", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD;
	IP_ADDR4(&ip_addr, TFTP_BOOTLOADER_MCU.Addr[0], TFTP_BOOTLOADER_MCU.Addr[1], TFTP_BOOTLOADER_MCU.Addr[2], TFTP_BOOTLOADER_MCU.Addr[3]);
	if (BOOTLOADER_UpdateNewFwStart(&Bootloader))
	{
		LOG_Error("TFTP_BLDR_MCU", "", "UpdateDownloadError", "");
		TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_ERROR;
		return -1;
	}
	result = TFTP_CLIENT_DownloadFile(TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Name, &ip_addr, TFTP_BOOTLOADER_MCU_DownloadUpdateReceiveCallback, NULL);
	if (result == TFTP_CLIENT_TIMEOUT)
	{
		LOG_Error("TFTP_BLDR_MCU", "", "UpdateDownloadTimeout", "");
		TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT;
		return -1;
	}
	else if (result != TFTP_CLIENT_OK)
	{
		LOG_Error("TFTP_BLDR_MCU", "", "UpdateDownloadError", "");
		TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_ERROR;
		return -1;
	}
	
	LOG_Info("TFTP_BLDR_MCU", "", "UpdateDownloadComplete", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_COMPLETE;
	return 0;	
}

static int32_t TFTP_BOOTLOADER_MCU_VerifyUpdate(void)
{
	LOG_Info("TFTP_BLDR_MCU", "", "UpdateVerifyStart", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY;
	if (BOOTLOADER_UpdateNewFwFinish(&Bootloader))
		goto error;
	if (BOOTLOADER_CheckNewFw(&Bootloader))
		goto error;	
	LOG_Info("TFTP_BLDR_MCU", "", "UpdateVerifyComplete", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE;
	return 0;
	
error:
	LOG_Error("TFTP_BLDR_MCU", "", "UpdateVerifyError", "");
	TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_UPDATE].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY_ERROR;
	return -1;
}

static void TFTP_BOOTLOADER_MCU_Idle(void)
{
	LOG_Info("TFTP_BLDR_MCU", "", "IdleStart", "");
	TFTP_BOOTLOADER_MCU.Status = TFTP_BOOTLOADER_STATUS_IDLE;
	if (TFTP_BOOTLOADER_MCU_IdleWeb())
	{
		goto error;
	}
	if (TFTP_BOOTLOADER_MCU_IdleLangpack())
	{
		goto error;
	}
	if (TFTP_BOOTLOADER_MCU_IdleUpdate())
	{
		goto error;
	}
	LOG_Info("TFTP_BLDR_MCU", "", "IdleComplete", "");
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_IDLE_COMPLETE(&TFTP_BOOTLOADER_MCU), 0);
	return;

error:
	LOG_Error("TFTP_BLDR_MCU", "", "IdleError", "");
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_IDLE_ERROR(&TFTP_BOOTLOADER_MCU), 0);
	return;
}

static void TFTP_BOOTLOADER_MCU_Download(void)
{
	LOG_Info("TFTP_BLDR_MCU", "", "DownloadStart", "");
	TFTP_BOOTLOADER_MCU.Status = TFTP_BOOTLOADER_STATUS_IN_PROGRESS;
	if (TFTP_BOOTLOADER_MCU_DownloadWeb())
	{
		goto error;
	}
	if (TFTP_BOOTLOADER_MCU_VerifyWeb())
	{
		goto error;
	}
	if (TFTP_BOOTLOADER_MCU_DownloadLangpack())
	{
		goto error;
	}
	if (TFTP_BOOTLOADER_MCU_VerifyLangpack())
	{
		goto error;
	}
	if (TFTP_BOOTLOADER_MCU_DownloadUpdate())
	{
		goto error;
	}
	if (TFTP_BOOTLOADER_MCU_VerifyUpdate())
	{
		goto error;
	}	
	LOG_Info("TFTP_BLDR_MCU", "", "DownloadComplete", "");
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_DOWNLOAD_COMPLETE(&TFTP_BOOTLOADER_MCU), 0);
	return;

error:
	LOG_Error("TFTP_BLDR_MCU", "", "DownloadError", "");
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_DOWNLOAD_ERROR(&TFTP_BOOTLOADER_MCU), 0);
	return;
}

static void TFTP_BOOTLOADER_MCU_Update(void)
{
	uint32_t LangpackBlocks;
	
	LOG_Info("TFTP_BLDR_MCU", "", "UpdateStart", "");
	if (TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Size % PROFILE_FLASH_LANG_BLOCK_SIZE)
		LangpackBlocks = (TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Size / PROFILE_FLASH_LANG_BLOCK_SIZE) + 1;
	else
		LangpackBlocks = TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_LANGPACK].Size / PROFILE_FLASH_LANG_BLOCK_SIZE;
	PROFILE_FLASH_UpdateLangpackHeader(&ProfileFlash, LangpackBlocks, 1);
	HTTP_SERVER_FILES_WriteDataSize(TFTP_BOOTLOADER_MCU_Files[TFTP_BOOTLOADER_MCU_FILE_WEB].Size);
	BOOTLOADER_UpdateNewFw(&Bootloader);
	LOG_Info("TFTP_BLDR_MCU", "", "UpdateComplete", "");
	TFTP_BOOTLOADER_MCU.Status = TFTP_BOOTLOADER_STATUS_COMPLETE;
	osMessagePut(TFTP_BOOTLOADER_MCU.MessageQId, TFTP_BOOTLOADER_COMPONENT_MSG_UPDATE_COMPLETE(&TFTP_BOOTLOADER_MCU), 0);
}

static void TFTP_BOOTLOADER_MCU_ProcessEvent(uint32_t Event)
{
	if (Event == TFTP_BOOTLOADER_COMPONENT_MSG_IDLE(&TFTP_BOOTLOADER_MCU))
		TFTP_BOOTLOADER_MCU_Idle();
	else if (Event == TFTP_BOOTLOADER_COMPONENT_MSG_DOWNLOAD(&TFTP_BOOTLOADER_MCU))
		TFTP_BOOTLOADER_MCU_Download();
	else if (Event == TFTP_BOOTLOADER_COMPONENT_MSG_UPDATE(&TFTP_BOOTLOADER_MCU))
		TFTP_BOOTLOADER_MCU_Update();
}
