#include "http_server_url_api_v1_update.h"
#include "http_server_files.h"
#include "tftp_bootloader.h"
#include "bootloader.h"
#include "bsp_flash.h"
#include "user.h"
#include "base64.h"
#include "log.h"
#include <string.h>

extern BOOTLOADER_T Bootloader;

#define HTTP_SERVER_URL_API_V1_UPDATE_TEMPLATE	"/api/v1/system/update"
#define HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_TEMPLATE	"/api/v1/update/mcu/software"
#define HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_TEMPLATE	"/api/v1/update/mcu/web"

static void HTTP_SERVER_URL_API_V1_UPDATE_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_UPDATE_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_UPDATE = {
	.Template = HTTP_SERVER_URL_API_V1_UPDATE_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_UPDATE_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_UPDATE_Get,
	.Put = NULL,
	.Post = HTTP_SERVER_URL_API_V1_UPDATE_Post,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE = {
	.Template = HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_Get,
	.Put = HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_Put,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB = {
	.Template = HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_Get,
	.Put = HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_Put,
	.Post = NULL,
	.Delete = NULL,
};

static void HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(struct netconn *Conn, TFTP_BOOTLOADER_FILE_T *File)
{
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"%s\",\r\n", File->Name))
		return;
	switch (File->Status)
	{
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE:
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_COMPLETE:
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Idle\",\r\n"))
				return;
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_TIMEOUT:
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_ERROR:
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Error\",\r\n"))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"error\" : \"Internal error\",\r\n"))
				return;
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD:
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_COMPLETE:
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Download\",\r\n"))
				return;
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT:
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Error\",\r\n"))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"error\" : \"Download timeout\",\r\n"))
				return;
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_ERROR:
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Error\",\r\n"))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"error\" : \"Download error\",\r\n"))
				return;
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY:
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Verify\",\r\n"))
				return;
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE:
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Complete\",\r\n"))
				return;
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_TIMEOUT:
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_ERROR:
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Error\",\r\n"))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"error\" : \"Verify error\",\r\n"))
				return;
			break;
		default:
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"status\" : \"Unknown\",\r\n"))
				return;
			break;
	}
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"bytes\" : \"%d\"\r\n", File->Size))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}"))
		return;
}

static void HTTP_SERVER_URL_API_V1_UPDATE_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i, j;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
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
	
	if (__HTTP_SERVER_StartBody(Conn))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "{\r\n"))
		return;
	if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_IDLE)
	{
		if (__HTTP_SERVER_WriteBody(Conn, "\t\"status\" : \"Idle\"\r\n"))
			return;
	}
	else if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_IN_PROGRESS)
	{
		if (__HTTP_SERVER_WriteBody(Conn, "\t\"status\" : \"Download\",\r\n"))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\"files\" : [\r\n"))
			return;
		HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Info);
		if (TFTP_BOOTLOADER.Info.Status == TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE)
		{
			for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
			{
				for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
				{
					if (__HTTP_SERVER_WriteBody(Conn, ",\r\n"))
						return;
					HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		if (__HTTP_SERVER_WriteBody(Conn, "\r\n"))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t]\r\n"))
			return;
	}
	else if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_COMPLETE)
	{
		if (__HTTP_SERVER_WriteBody(Conn, "\t\"status\" : \"Complete\",\r\n"))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\"files\" : [\r\n"))
			return;
		HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Info);
		if (TFTP_BOOTLOADER.Info.Status == TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE)
		{
			for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
			{
				for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
				{
					if (__HTTP_SERVER_WriteBody(Conn, ",\r\n"))
						return;
					HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		if (__HTTP_SERVER_WriteBody(Conn, "\r\n"))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t]\r\n"))
			return;
	}
	else
	{
		if (__HTTP_SERVER_WriteBody(Conn, "\t\"status\" : \"Error\",\r\n"))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\"files\" : [\r\n"))
			return;
		HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Info);
		if (TFTP_BOOTLOADER.Info.Status == TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE)
		{
			for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
			{
				for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
				{
					if (__HTTP_SERVER_WriteBody(Conn, ",\r\n"))
						return;
					HTTP_SERVER_URL_API_V1_UPDATE_WriteFileStatus(Conn, &TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		if (__HTTP_SERVER_WriteBody(Conn, "\r\n"))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t]\r\n"))
			return;
	}
	if (__HTTP_SERVER_WriteBody(Conn, "}\r\n"))
		return;
	if (__HTTP_SERVER_FinishBody(Conn))
		return;
	return;
	
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "SoftwareUpdateReadError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
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
	char ip[4];
	uint32_t tmp;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;

	if (ReqHeader->ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if (ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
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
		goto error_400;
	
	Buffer[BufferLen] = 0;
	
	p = strstr(Buffer, "\"interface\"");
	if (!p)
		goto error_400;
	p += sizeof("\"interface\"") - 1;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != ':')
		goto error_400;
	p++;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != '\"')
		goto error_400;
	p++;
	if (memcmp(p, "tftp\"", sizeof("tftp\"") - 1))
		goto error_400;
	
	p = strstr(Buffer, "\"server_ip\"");
	if (!p)
		goto error_400;
	p += sizeof("\"server_ip\"") - 1;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != ':')
		goto error_400;
	p++;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != '\"')
		goto error_400;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp == 0)  || (tmp > 255) || (*p != '.'))
		goto error_400;
	ip[0] = tmp;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp > 255) || (*p != '.'))
		goto error_400;
	ip[1] = tmp;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp > 255) || (*p != '.'))
		goto error_400;
	ip[2] = tmp;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp == 0) || (tmp > 255) || (*p != '\"'))
		goto error_400;
	ip[3] = tmp;

	p = strstr(Buffer, "\"server_folder\"");
	if (!p)
		goto error_400;
	p += sizeof("\"server_folder\"") - 1;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != ':')
		goto error_400;
	p++;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != '\"')
		goto error_400;
	p++;
	_p = strchr(p, '\"');
	if (!_p)
		goto error_400;
	memcpy(folder, p, _p - p);
	folder[_p - p] = 0;
	
	if (TFTP_BOOTLOADER_Update(folder, ip))
		goto error_500;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "SoftwareUpdateStart", "");
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "SoftwareUpdateStartError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "SoftwareUpdateStartError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
error_500:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "SoftwareUpdateStartError", "Internal error");
	__HTTP_SERVER_WriteError500(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;

	if (strcmp(User.Group, USER_GROUP_SUPERUSER))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
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
	
	if (__HTTP_SERVER_StartBody(Conn))
		return;
	if (__HTTP_SERVER_WriteBodyData(Conn, (char*)BOOTLOADER_FW_ADDR, BOOTLOADER_FW_SIZE))
		return;
	if (__HTTP_SERVER_FinishBody(Conn))
		return;
	return;
	
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "MCUSoftwareReadError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	err_t recv_err;
	char *data;
	uint16_t error, datalen;
	uint32_t ContentLength;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;

	if (ReqHeader->ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if ((ContentLength == 0) || (ContentLength > BSP_FLASH_FW_SIZE))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if (strcmp(User.Group, USER_GROUP_SUPERUSER))
	{
		netbuf_delete(Inbuf);
		goto error_403;
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
		goto error_400;
	
	if(BOOTLOADER_UpdateNewFwFinish(&Bootloader))
		goto error_400;
	if(BOOTLOADER_CheckNewFw(&Bootloader))
		goto error_400;
	if(BOOTLOADER_UpdateNewFw(&Bootloader))
		goto error_400;
	
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
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "MCUSoftwareUpdate", "");
	osDelay(3000);
	BSP_Reset();
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "MCUSoftwareUpdateError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "MCUSoftwareUpdateError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	USER_T User;
	char Data[256];
	uint32_t DataAddr, DataLen, DataSize;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	if (strcmp(User.Group, USER_GROUP_SUPERUSER))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}

	netbuf_delete(Inbuf);
	
	DataAddr = 0;
	if (HTTP_SERVER_FILES_ReadDataSize(&DataSize))
		goto error_500;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	if (__HTTP_SERVER_StartBody(Conn))
		return;
	while (DataSize)
	{
		if (DataSize >= sizeof(Data))
			DataLen = sizeof(Data);
		else
			DataLen = DataSize;
		if (HTTP_SERVER_FILES_ReadData(DataAddr, Data, DataLen))
			return;
		if (__HTTP_SERVER_WriteBodyData(Conn, (char*)Data, DataLen))
			return;
		DataAddr += DataLen;
		DataSize -= DataLen;
	}
	if (__HTTP_SERVER_FinishBody(Conn))
		return;
	return;

error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "MCUWebReadError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
error_500:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "MCUWebReadError", "Internal error");
	__HTTP_SERVER_WriteError500(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	err_t RecvError;
	char *Data;
	uint16_t Error, DataLen;
	uint32_t DataAddr, DataSize, ContentLength;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;

	if (ReqHeader->ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if ((ContentLength == 0) || (ContentLength > HTTP_SERVER_FILES_RECORD_BODY_SIZE))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if (strcmp(User.Group, USER_GROUP_SUPERUSER))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
	DataAddr = 0;
	DataSize = ContentLength;
	Error = 0;
	if (ReqHeader->Body)
	{
		Data = ReqHeader->Body;
		DataLen = ReqHeader->BodyLen;		
		goto write_data;
	}
	netbuf_delete(Inbuf);
	
	while(ContentLength) {
		RecvError = netconn_recv(Conn, &Inbuf);
		if (RecvError == ERR_OK)
		{
			netbuf_data(Inbuf, (void**)&Data, &DataLen);
write_data:
			ContentLength -= DataLen;
			if (!Error)
			{
				if (HTTP_SERVER_FILES_WriteData(DataAddr, Data, DataLen))
					Error = 1;
			}
			DataAddr += DataLen;
			netbuf_delete(Inbuf);
		}
		else
		{
			return;
		}
	}
	if (Error)
		goto error_500;
	
	if (HTTP_SERVER_FILES_WriteDataSize(DataSize))
		goto error_500;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "MCUWebUpdate", "");
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "MCUWebUpdateError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "MCUWebUpdateError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
error_500:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "MCUWebUpdateError", "Internal error");
	__HTTP_SERVER_WriteError500(Conn);
	return;
}
