#include "http_server_api_v1_update.h"
#include "http_server_files.h"
#include "tftp_bootloader.h"
#include "bootloader.h"
#include "bsp_flash.h"
#include <string.h>

extern BOOTLOADER_T Bootloader;

#define HTTP_SERVER_API_V1_UPDATE_URL_TEMPLATE	"/api/v1/update"
#define HTTP_SERVER_API_V1_UPDATE_MCU_SOFTWARE_URL_TEMPLATE	"/api/v1/update/mcu/software"
#define HTTP_SERVER_API_V1_UPDATE_MCU_WEB_URL_TEMPLATE	"/api/v1/update/mcu/web"

static void HTTP_SERVER_API_V1_UPDATE_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_UPDATE_PostProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_UPDATE_MCU_SOFTWARE_PostProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_UPDATE_MCU_WEB_PostProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_UPDATE_Url = {
	.Url = HTTP_SERVER_API_V1_UPDATE_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_UPDATE_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = HTTP_SERVER_API_V1_UPDATE_GetProcessing,
	.PutProcessing = NULL,
	.PostProcessing = HTTP_SERVER_API_V1_UPDATE_PostProcessing,
};

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_UPDATE_MCU_SOFTWARE_Url = {
	.Url = HTTP_SERVER_API_V1_UPDATE_MCU_SOFTWARE_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_UPDATE_MCU_SOFTWARE_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = NULL,
	.PutProcessing = NULL,
	.PostProcessing = HTTP_SERVER_API_V1_UPDATE_MCU_SOFTWARE_PostProcessing,
};

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_UPDATE_MCU_WEB_Url = {
	.Url = HTTP_SERVER_API_V1_UPDATE_MCU_WEB_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_UPDATE_MCU_WEB_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = NULL,
	.PutProcessing = NULL,
	.PostProcessing = HTTP_SERVER_API_V1_UPDATE_MCU_WEB_PostProcessing,
};

static void HTTP_SERVER_API_V1_UPDATE_WriteFileStatus(struct netconn *Conn, TFTP_BOOTLOADER_FILE_T *File)
{
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"%s\",\r\n", File->Name);
	switch (File->Status)
	{
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE:
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_COMPLETE:
			http_server_write_body(Conn, "\t\t\t\"status\" : \"Idle\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_TIMEOUT:
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_ERROR:
			http_server_write_body(Conn, "\t\t\t\"status\" : \"Error\",\r\n");
			http_server_write_body(Conn, "\t\t\t\"error\" : \"Internal error\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD:
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_COMPLETE:
			http_server_write_body(Conn, "\t\t\t\"status\" : \"Download\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT:
			http_server_write_body(Conn, "\t\t\t\"status\" : \"Error\",\r\n");
			http_server_write_body(Conn, "\t\t\t\"error\" : \"Download timeout\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_ERROR:
			http_server_write_body(Conn, "\t\t\t\"status\" : \"Error\",\r\n");
			http_server_write_body(Conn, "\t\t\t\"error\" : \"Download error\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY:
			http_server_write_body(Conn, "\t\t\t\"status\" : \"Verify\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE:
			http_server_write_body(Conn, "\t\t\t\"status\" : \"Complete\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_TIMEOUT:
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_ERROR:
			http_server_write_body(Conn, "\t\t\t\"status\" : \"Error\",\r\n");
			http_server_write_body(Conn, "\t\t\t\"error\" : \"Verify error\",\r\n");
			break;
		default:
			http_server_write_body(Conn, "\t\t\t\"status\" : \"Unknown\",\r\n");
			break;
	}
	http_server_write_body(Conn, "\t\t\t\"bytes\" : \"%d\"\r\n", File->Size);
	http_server_write_body(Conn, "\t\t}");
}

static void HTTP_SERVER_API_V1_UPDATE_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	HTTP_RESPONCE_HEADER_T RspHeader;
	uint32_t i, j;
	
	netbuf_delete(Inbuf);
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(Conn, &RspHeader))
		return;
	
	http_server_write_body_start(Conn);
	http_server_write_body(Conn, "{\r\n");
	if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_IDLE)
	{
		http_server_write_body(Conn, "\t\"status\" : \"Idle\"\r\n");
	}
	else if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_IN_PROGRESS)
	{
		http_server_write_body(Conn, "\t\"status\" : \"Download\",\r\n");
		http_server_write_body(Conn, "\t\"files\" : [\r\n");
		HTTP_SERVER_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Info);
		if (TFTP_BOOTLOADER.Info.Status == TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE)
		{
			for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
			{
				for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
				{
					http_server_write_body(Conn, ",\r\n");
					HTTP_SERVER_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		http_server_write_body(Conn, "\r\n");
		http_server_write_body(Conn, "\t]\r\n");
	}
	else if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_COMPLETE)
	{
		http_server_write_body(Conn, "\t\"status\" : \"Complete\",\r\n");
		http_server_write_body(Conn, "\t\"files\" : [\r\n");
		HTTP_SERVER_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Info);
		if (TFTP_BOOTLOADER.Info.Status == TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE)
		{
			for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
			{
				for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
				{
					http_server_write_body(Conn, ",\r\n");
					HTTP_SERVER_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		http_server_write_body(Conn, "\r\n");
		http_server_write_body(Conn, "\t]\r\n");
	}
	else
	{
		http_server_write_body(Conn, "\t\"status\" : \"Error\",\r\n");
		http_server_write_body(Conn, "\t\"files\" : [\r\n");
		HTTP_SERVER_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Info);
		if (TFTP_BOOTLOADER.Info.Status == TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE)
		{
			for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
			{
				for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
				{
					http_server_write_body(Conn, ",\r\n");
					HTTP_SERVER_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		http_server_write_body(Conn, "\r\n");
		http_server_write_body(Conn, "\t]\r\n");
	}
	http_server_write_body(Conn, "}\r\n");
	http_server_write_body_finish(Conn);
}

static void HTTP_SERVER_API_V1_UPDATE_PostProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	char Buffer[256];
	uint32_t BufferLen;
	struct netbuf *inbuf;
	err_t recv_err;
	char *data;
	char *p, *_p;
	char folder[64];
	uint16_t error, datalen;
	uint32_t ContentLength;
	HTTP_RESPONCE_HEADER_T RspHeader;
	char ip[4];
	uint32_t tmp;

	if (ReqHeader->ContentLength == 0)
		goto error;
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if (ContentLength == 0)
		goto error;
	
	error = 0;
	BufferLen = 0;
	if ((Body) && (BodyLen))
	{
		inbuf = Inbuf;
		data = Body;
		datalen = BodyLen;		
		goto parse_data;
	}
	netbuf_delete(Inbuf);
	
	while(ContentLength) {
		recv_err = netconn_recv(Conn, &inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&data, &datalen);
parse_data:
			ContentLength -= datalen;
			if ((!error) && ((BufferLen + datalen) < (sizeof(Buffer) - 1)))
			{
				memcpy(&Buffer[BufferLen], data, datalen);
				BufferLen += datalen;
			}
			else
			{
				error = 1;
			}
			netbuf_delete(inbuf);
		}
		else
		{
			return;
		}
	}
	if (error)
		goto error;
	
	Buffer[BufferLen] = 0;
	
	p = strstr(Buffer, "\"interface\"");
	if (!p)
		goto error;
	p += sizeof("\"interface\"") - 1;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != ':')
		goto error;
	p++;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != '\"')
		goto error;
	p++;
	if (memcmp(p, "tftp\"", sizeof("tftp\"") - 1))
		goto error;
	
	p = strstr(Buffer, "\"server_ip\"");
	if (!p)
		goto error;
	p += sizeof("\"server_ip\"") - 1;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != ':')
		goto error;
	p++;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != '\"')
		goto error;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp == 0)  || (tmp > 255) || (*p != '.'))
		goto error;
	ip[0] = tmp;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp > 255) || (*p != '.'))
		goto error;
	ip[1] = tmp;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp > 255) || (*p != '.'))
		goto error;
	ip[2] = tmp;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp == 0) || (tmp > 255) || (*p != '\"'))
		goto error;
	ip[3] = tmp;

	p = strstr(Buffer, "\"server_folder\"");
	if (!p)
		goto error;
	p += sizeof("\"server_folder\"") - 1;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != ':')
		goto error;
	p++;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != '\"')
		goto error;
	p++;
	_p = strchr(p, '\"');
	if (!_p)
		goto error;
	memcpy(folder, p, _p - p);
	folder[_p - p] = 0;
	
	if (TFTP_BOOTLOADER_Update(folder, ip))
		goto error;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	http_server_write_header(Conn, &RspHeader);
	return;
	
error:
	http_server_write_400_error(Conn);
	return;
}

static void HTTP_SERVER_API_V1_UPDATE_MCU_SOFTWARE_PostProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	struct netbuf *inbuf;
	err_t recv_err;
	char *data;
	uint16_t error, datalen;
	uint32_t ContentLength;
	HTTP_RESPONCE_HEADER_T RspHeader;

	if (ReqHeader->ContentLength == 0)
		goto error;
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if ((ContentLength == 0) || (ContentLength > BSP_FLASH_FW_SIZE))
		goto error;
	
	BOOTLOADER_UpdateNewFwStart(&Bootloader);
	
	error = 0;
	if ((Body) && (BodyLen))
	{
		inbuf = Inbuf;
		data = Body;
		datalen = BodyLen;		
		goto write_data;
	}
	netbuf_delete(Inbuf);
	
	while(ContentLength) {
		recv_err = netconn_recv(Conn, &inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&data, &datalen);
write_data:
			ContentLength -= datalen;
			if (!error)
			{
				if (BOOTLOADER_UpdateNewFwBlock(&Bootloader, data, datalen))
					error = 1;
			}			
			netbuf_delete(inbuf);
		}
		else
		{
			return;
		}
	}
	if (error)
		goto error;
	
	if(BOOTLOADER_UpdateNewFwFinish(&Bootloader))
		goto error;
	if(BOOTLOADER_CheckNewFw(&Bootloader))
		goto error;
	if(BOOTLOADER_UpdateNewFw(&Bootloader))
		goto error;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	http_server_write_header(Conn, &RspHeader);
	netconn_close(Conn);
	netconn_delete(Conn);
	osDelay(1000);
	BSP_Reset();
	return;
	
error:
	http_server_write_400_error(Conn);
}

static void HTTP_SERVER_API_V1_UPDATE_MCU_WEB_PostProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	struct netbuf *inbuf;
	err_t recv_err;
	char *data;
	uint16_t error, datalen;
	uint32_t dataaddr, ContentLength;
	HTTP_RESPONCE_HEADER_T RspHeader;

	if (ReqHeader->ContentLength == 0)
		goto error;
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if ((ContentLength == 0) || (ContentLength > HTTP_SERVER_FILES_RECORD_BODY_SIZE))
		goto error;
	
	dataaddr = 0;
	error = 0;
	if ((Body) && (BodyLen))
	{
		inbuf = Inbuf;
		data = Body;
		datalen = BodyLen;		
		goto write_data;
	}
	netbuf_delete(Inbuf);
	
	while(ContentLength) {
		recv_err = netconn_recv(Conn, &inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&data, &datalen);
write_data:
			ContentLength -= datalen;
			if (!error)
			{
				if (HTTP_SERVER_FILES_WriteData(dataaddr, data, datalen))
					error = 1;
			}
			dataaddr += datalen;
			netbuf_delete(inbuf);
		}
		else
		{
			return;
		}
	}
	if (error)
		goto error;
	
	if (HTTP_SERVER_FILES_WriteDataComplete())
		goto error;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	http_server_write_header(Conn, &RspHeader);	
	return;
	
error:
	http_server_write_400_error(Conn);
}
