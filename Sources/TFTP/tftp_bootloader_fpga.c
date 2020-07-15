#include "tftp_bootloader_fpga.h"
#include "tftp_client.h"
#include "bootloader.h"
#include "profile_flash.h"
#include <string.h>
#include "log.h"

#define TFTP_BOOTLOADER_FPGA_FILE_IMAGE	0
#define TFTP_BOOTLOADER_FPGA_FILES_CNT 	1

static void TFTP_BOOTLOADER_FPGA_ProcessEvent(uint32_t Event);
static int32_t TFTP_BOOTLOADER_FPGA_VerifyInfo(char *Data);

TFTP_BOOTLOADER_FILE_T TFTP_BOOTLOADER_FPGA_Files[TFTP_BOOTLOADER_FPGA_FILES_CNT];

TFTP_BOOTLOADER_COMPONENT_T TFTP_BOOTLOADER_FPGA = 
{
	.Files = TFTP_BOOTLOADER_FPGA_Files,
	.FilesCnt = sizeof(TFTP_BOOTLOADER_FPGA_Files) / sizeof(TFTP_BOOTLOADER_FPGA_Files[0]),
	.ProcessEvent = TFTP_BOOTLOADER_FPGA_ProcessEvent,
	.VerifyInfo = TFTP_BOOTLOADER_FPGA_VerifyInfo,
};

static int32_t TFTP_BOOTLOADER_FPGA_VerifyInfo(char *Data)
{
	char *p, *c;
	uint32_t i, len;
	
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
	
	strcpy(TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Name, TFTP_BOOTLOADER_FPGA.Dir);
	strcat(TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Name, "/FPGA/"BSP_PROJECT"__FPGA__Image__");
	len = strlen(TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Name);
	memcpy(&TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Name[len], p, c - p);
	for (i = 0; i < (c - p); i++)
	{
		if (TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Name[len + i] == '.')
			TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Name[len + i] = '_';
	}
	len += c - p;
	TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Name[len] = 0;
	strcat(TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Name, ".bin");
	TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Size = 0;
	TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD;
	TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].MaxSize = 0x4000000;
	TFTP_BOOTLOADER_FPGA.Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Data = NULL;
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Image: %s", TFTP_BOOTLOADER_FPGA_Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Name);
	
	TFTP_BOOTLOADER_FPGA.FilesCnt = TFTP_BOOTLOADER_FPGA_FILES_CNT;	
	return 0;
	
error:
	return -1;
}

static int TFTP_BOOTLOADER_FPGA_DownloadImageReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{	
	TFTP_BOOTLOADER_FPGA_Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Size += DataSize;
	/*todo: Write data to FPGA flash*/
	return 0;
}

static void TFTP_BOOTLOADER_FPGA_CompleteImage(uint32_t NextEvent)
{	
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Complete Image");
	TFTP_BOOTLOADER_FPGA_Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Status = TFTP_BOOTLOADER_FILE_STATUS_COMPLETE;
	osMessagePut(TFTP_BOOTLOADER_FPGA.MessageQId, NextEvent, 0);
}

static void TFTP_BOOTLOADER_FPGA_VerifyImage(uint32_t NextEvent)
{
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Verify Image");
	TFTP_BOOTLOADER_FPGA_Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Status = TFTP_BOOTLOADER_FILE_STATUS_VERIFY;	
	TFTP_BOOTLOADER_FPGA_CompleteImage(NextEvent);
}

static void TFTP_BOOTLOADER_FPGA_DownloadImage(uint32_t NextEvent)
{
	int32_t result;
	ip_addr_t ip_addr;
	
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Download Image");
	IP_ADDR4(&ip_addr, TFTP_BOOTLOADER_FPGA.Addr[0], TFTP_BOOTLOADER_FPGA.Addr[1], TFTP_BOOTLOADER_FPGA.Addr[2], TFTP_BOOTLOADER_FPGA.Addr[3]);	
	result = TFTP_CLIENT_DownloadFile(TFTP_BOOTLOADER_FPGA_Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Name, &ip_addr, TFTP_BOOTLOADER_FPGA_DownloadImageReceiveCallback, NULL);
	if (result == TFTP_CLIENT_OK)
	{
		TFTP_BOOTLOADER_FPGA_VerifyImage(NextEvent);
	}
	else if (result == TFTP_CLIENT_TIMEOUT)
	{
		LOG_Error("TFTP_BOOTLOADER", "FPGA", "", "Download Image: Timeout Error");
		TFTP_BOOTLOADER_FPGA_Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT_ERROR;
		osMessagePut(TFTP_BOOTLOADER_FPGA.MessageQId, TFTP_BOOTLOADER_FPGA_MSG_DOWNLOAD_ERROR, 0);
	}
	else
	{
		LOG_Error("TFTP_BOOTLOADER", "FPGA", "", "Download Image: Size Error");
		TFTP_BOOTLOADER_FPGA_Files[TFTP_BOOTLOADER_FPGA_FILE_IMAGE].Status = TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_SIZE_ERROR;
		osMessagePut(TFTP_BOOTLOADER_FPGA.MessageQId, TFTP_BOOTLOADER_FPGA_MSG_DOWNLOAD_ERROR, 0);
	}
}

static void TFTP_BOOTLOADER_FPGA_Download(uint32_t NextEvent)
{
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Download");
	osMessagePut(TFTP_BOOTLOADER_FPGA.MessageQId, NextEvent, 0);
	return;	
}

static void TFTP_BOOTLOADER_FPGA_Update(void)
{
	LOG_Info("TFTP_BOOTLOADER", "FPGA", "", "Update");
	/*todo: Update FPGA*/
	TFTP_BOOTLOADER_FPGA.Status = TFTP_BOOTLOADER_STATUS_COMPLETE;
	osMessagePut(TFTP_BOOTLOADER_FPGA.MessageQId, TFTP_BOOTLOADER_FPGA_MSG_UPDATE_COMPLETE, 0);
}

static void TFTP_BOOTLOADER_FPGA_ProcessEvent(uint32_t Event)
{
	switch (Event)
	{
		case TFTP_BOOTLOADER_FPGA_MSG_DOWNLOAD:
			TFTP_BOOTLOADER_FPGA_Download(TFTP_BOOTLOADER_FPGA_MSG_IMAGE_DOWNLOAD);
			break;
		case TFTP_BOOTLOADER_FPGA_MSG_IMAGE_DOWNLOAD:
			TFTP_BOOTLOADER_FPGA_DownloadImage(TFTP_BOOTLOADER_FPGA_MSG_DOWNLOAD_COMPLETE);
			break;
		case TFTP_BOOTLOADER_FPGA_MSG_UPDATE:
			TFTP_BOOTLOADER_FPGA_Update();
			break;
	}
}
