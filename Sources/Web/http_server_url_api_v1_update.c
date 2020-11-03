#include "http_server_url_api_v1_update.h"
#include "http_server_files.h"
#include "tftp_bootloader.h"
#include "bootloader.h"
#include "bsp_flash.h"
#include "user.h"
#include "base64.h"
#include <string.h>

extern BOOTLOADER_T Bootloader;

#define HTTP_SERVER_URL_API_V1_UPDATE_TEMPLATE	"/api/v1/update"
#define HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_TEMPLATE	"/api/v1/update/mcu/software"
#define HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_TEMPLATE	"/api/v1/update/mcu/web"

static int32_t HTTP_SERVER_URL_API_V1_UPDATE_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

static void HTTP_SERVER_URL_API_V1_UPDATE_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_UPDATE_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_UPDATE = {
	.Template = HTTP_SERVER_URL_API_V1_UPDATE_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_UPDATE_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_UPDATE_Auth,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_UPDATE_Get,
	.Put = NULL,
	.Post = HTTP_SERVER_URL_API_V1_UPDATE_Post,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE = {
	.Template = HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_UPDATE_Auth,
	.Head = NULL,
	.Get = NULL,
	.Put = NULL,
	.Post = HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_Post,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB = {
	.Template = HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_UPDATE_Auth,
	.Head = NULL,
	.Get = NULL,
	.Put = NULL,
	.Post = HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_Post,
	.Delete = NULL,
};

static int32_t HTTP_SERVER_URL_API_V1_UPDATE_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *Base64, *Login, *Password, *c;
	USER_T User;
	char Buffer[34];
	uint32_t Base64Len, i;
	
	if ((!ReqHeader->Authorization) || (ReqHeader->AuthorizationSize < sizeof("Basic ")))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	if (!memcpy(ReqHeader->Authorization, "Basic ", sizeof("Basic ") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	Base64 = &ReqHeader->Authorization[sizeof("Basic ") - 1];
	ReqHeader->Authorization[ReqHeader->AuthorizationSize] = 0;
	Base64Len = strlen(Base64);
	if ((Base64Len % 4) || ((Base64Len * 3 / 4) >= sizeof(Buffer)))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	c = b64tobin(Buffer, Base64);
	if (!c)
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	*c = 0;
	
	for (i = 0; i < sizeof(Buffer); i++)
	{
		if (Buffer[i] == 0)
		{
			netbuf_delete(Inbuf);
			goto error_401;
		}
		else if (Buffer[i] == ':')
		{
			break;
		}
	}
	
	if (i == sizeof(Buffer))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	Buffer[i] = 0;
	
	Login = Buffer;
	Password = &Buffer[i + 1];
	
	if (USER_GetUserByLogin(Login, &User))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	if (strcmp(Password, User.Password))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	return 0;
	
error_401:
	__HTTP_SERVER_WriteError401(Conn);
	return -1;
}

static void HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(struct netconn *Conn, TFTP_BOOTLOADER_FILE_T *File)
{
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"%s\",\r\n", File->Name);
	switch (File->Status)
	{
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE:
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_COMPLETE:
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Idle\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_TIMEOUT:
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_ERROR:
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Error\",\r\n");
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"error\" : \"Internal error\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD:
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_COMPLETE:
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Download\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT:
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Error\",\r\n");
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"error\" : \"Download timeout\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_ERROR:
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Error\",\r\n");
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"error\" : \"Download error\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY:
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Verify\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE:
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Complete\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_TIMEOUT:
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_ERROR:
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Error\",\r\n");
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"error\" : \"Verify error\",\r\n");
			break;
		default:
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Unknown\",\r\n");
			break;
	}
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"bytes\" : \"%d\"\r\n", File->Size);
	__HTTP_SERVER_WriteBody(Conn, "\t\t}");
}

static void HTTP_SERVER_URL_API_V1_UPDATE_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	uint32_t i, j;
	
	netbuf_delete(Inbuf);
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "{\r\n");
	if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_IDLE)
	{
		__HTTP_SERVER_WriteBody(Conn, "\t\"status\" : \"Idle\"\r\n");
	}
	else if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_IN_PROGRESS)
	{
		__HTTP_SERVER_WriteBody(Conn, "\t\"status\" : \"Download\",\r\n");
		__HTTP_SERVER_WriteBody(Conn, "\t\"files\" : [\r\n");
		HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Info);
		if (TFTP_BOOTLOADER.Info.Status == TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE)
		{
			for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
			{
				for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
				{
					__HTTP_SERVER_WriteBody(Conn, ",\r\n");
					HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		__HTTP_SERVER_WriteBody(Conn, "\r\n");
		__HTTP_SERVER_WriteBody(Conn, "\t]\r\n");
	}
	else if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_COMPLETE)
	{
		__HTTP_SERVER_WriteBody(Conn, "\t\"status\" : \"Complete\",\r\n");
		__HTTP_SERVER_WriteBody(Conn, "\t\"files\" : [\r\n");
		HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Info);
		if (TFTP_BOOTLOADER.Info.Status == TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE)
		{
			for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
			{
				for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
				{
					__HTTP_SERVER_WriteBody(Conn, ",\r\n");
					HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		__HTTP_SERVER_WriteBody(Conn, "\r\n");
		__HTTP_SERVER_WriteBody(Conn, "\t]\r\n");
	}
	else
	{
		__HTTP_SERVER_WriteBody(Conn, "\t\"status\" : \"Error\",\r\n");
		__HTTP_SERVER_WriteBody(Conn, "\t\"files\" : [\r\n");
		HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Info);
		if (TFTP_BOOTLOADER.Info.Status == TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE)
		{
			for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
			{
				for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
				{
					__HTTP_SERVER_WriteBody(Conn, ",\r\n");
					HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		__HTTP_SERVER_WriteBody(Conn, "\r\n");
		__HTTP_SERVER_WriteBody(Conn, "\t]\r\n");
	}
	__HTTP_SERVER_WriteBody(Conn, "}\r\n");
	__HTTP_SERVER_FinishBody(Conn);
}

static void HTTP_SERVER_URL_API_V1_UPDATE_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char Buffer[256];
	uint32_t BufferLen;
	err_t recv_err;
	char *data;
	char *p, *_p;
	char folder[64];
	uint16_t error, datalen;
	uint32_t ContentLength;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	char ip[4];
	uint32_t tmp;

	if (ReqHeader->ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error;
	}
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if (ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error;
	}
	
	error = 0;
	BufferLen = 0;
	if (ReqHeader->Body)
	{
		data = ReqHeader->Body;
		datalen = ReqHeader->BodyLen;		
		goto parse_data;
	}
	netbuf_delete(Inbuf);
	
	while(ContentLength) {
		recv_err = netconn_recv(Conn, &Inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(Inbuf, (void**)&data, &datalen);
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
			netbuf_delete(Inbuf);
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
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	return;
	
error:
	__HTTP_SERVER_WriteError400(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	err_t recv_err;
	char *data;
	uint16_t error, datalen;
	uint32_t ContentLength;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;

	if (ReqHeader->ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error;
	}
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if ((ContentLength == 0) || (ContentLength > BSP_FLASH_FW_SIZE))
	{
		netbuf_delete(Inbuf);
		goto error;
	}
	
	BOOTLOADER_UpdateNewFwStart(&Bootloader);
	
	error = 0;
	if (ReqHeader->Body)
	{
		data = ReqHeader->Body;
		datalen = ReqHeader->BodyLen;		
		goto write_data;
	}
	netbuf_delete(Inbuf);
	
	while(ContentLength) {
		recv_err = netconn_recv(Conn, &Inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(Inbuf, (void**)&data, &datalen);
write_data:
			ContentLength -= datalen;
			if (!error)
			{
				if (BOOTLOADER_UpdateNewFwBlock(&Bootloader, data, datalen))
					error = 1;
			}			
			netbuf_delete(Inbuf);
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
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	netconn_close(Conn);
	netconn_delete(Conn);
	osDelay(1000);
	BSP_Reset();
	return;
	
error:
	__HTTP_SERVER_WriteError400(Conn);
}

static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	err_t recv_err;
	char *data;
	uint16_t error, datalen;
	uint32_t dataaddr, ContentLength;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;

	if (ReqHeader->ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error;
	}
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if ((ContentLength == 0) || (ContentLength > HTTP_SERVER_FILES_RECORD_BODY_SIZE))
	{
		netbuf_delete(Inbuf);
		goto error;
	}
	
	dataaddr = 0;
	error = 0;
	if (ReqHeader->Body)
	{
		data = ReqHeader->Body;
		datalen = ReqHeader->BodyLen;		
		goto write_data;
	}
	netbuf_delete(Inbuf);
	
	while(ContentLength) {
		recv_err = netconn_recv(Conn, &Inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(Inbuf, (void**)&data, &datalen);
write_data:
			ContentLength -= datalen;
			if (!error)
			{
				if (HTTP_SERVER_FILES_WriteData(dataaddr, data, datalen))
					error = 1;
			}
			dataaddr += datalen;
			netbuf_delete(Inbuf);
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
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);	
	return;
	
error:
	__HTTP_SERVER_WriteError400(Conn);
}
