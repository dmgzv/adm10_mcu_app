#include "http_server_url_api_v1_devices.h"
#include "user.h"
#include "bsp.h"
#include "base64.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_DEVICES_TEMPLATE	"/api/v1/devices"

static int32_t HTTP_SERVER_URL_API_V1_DEVICES_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_DEVICES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_DEVICES = {
	.Template = HTTP_SERVER_URL_API_V1_DEVICES_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_DEVICES_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_DEVICES_Auth,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_DEVICES_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

static int32_t HTTP_SERVER_URL_API_V1_DEVICES_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
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

static void HTTP_SERVER_URL_API_V1_DEVICES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i;
	char *Locked;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
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
	__HTTP_SERVER_WriteBody(Conn, "[\r\n");
	for (i = 0; i < DevicesCnt; i++)
	{
		if (osMutexWait(Devices[i]->DevMutexId, 1) == osOK)
		{
			Locked = "unlocked";
			osMutexRelease(Devices[i]->DevMutexId);
		}
		else
		{
			Locked = "locked";
		}
		__HTTP_SERVER_WriteBody(Conn, "\t{\r\n");
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"name\" : \"%s\",\r\n", Devices[i]->DevName);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"id\" : \"%s\",\r\n", Devices[i]->DevId);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"status\" : \"%s\",\r\n", BSP_DEV_GET_STATE_STR(Devices[i]));
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_cnt\" : \"%d\",\r\n", Devices[i]->InitCnt);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_err\" : \"%d\",\r\n", Devices[i]->InitErr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_cont_err\" : \"%d\",\r\n", Devices[i]->InitContErr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_cont_err_thr\" : \"%d\",\r\n", Devices[i]->InitContErrThr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_cnt\" : \"%d\",\r\n", Devices[i]->UpdateCnt);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_err\" : \"%d\",\r\n", Devices[i]->UpdateErr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_cont_err\" : \"%d\",\r\n", Devices[i]->UpdateContErr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_cont_err_thr\" : \"%d\",\r\n", Devices[i]->UpdateContErrThr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"lock\" : \"%s\"\r\n", Locked);
		if (i == (DevicesCnt - 1))
			__HTTP_SERVER_WriteBody(Conn, "\t}\r\n");
		else
			__HTTP_SERVER_WriteBody(Conn, "\t},\r\n");
	}	
	__HTTP_SERVER_WriteBody(Conn, "]\r\n");
	__HTTP_SERVER_FinishBody(Conn);
}
