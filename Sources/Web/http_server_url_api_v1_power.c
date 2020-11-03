#include "http_server_url_api_v1_power.h"
#include "bsp.h"
#include "user.h"
#include "base64.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_POWER_TEMPLATE	"/api/v1/power"

static int32_t HTTP_SERVER_URL_API_V1_POWER_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_POWER_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_POWER = {
	.Template = HTTP_SERVER_URL_API_V1_POWER_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_POWER_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_POWER_Auth,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_POWER_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

static int32_t HTTP_SERVER_URL_API_V1_POWER_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
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

static void HTTP_SERVER_URL_API_V1_POWER_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i;
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
	for (i = 0; i < UCD90120A_RAILS_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK)
			break;
		__HTTP_SERVER_WriteBody(Conn, "\t{\r\n");
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"config\" : \"%.3f\"\r\n,", ucd90120a.Cfg.Rail[i].VoutCmd);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"monitor\" : \"%.3f\"\r\n", ucd90120a.Mon.Rail[i].Vout);
		if (i != (UCD90120A_RAILS_CNT - 1))
			__HTTP_SERVER_WriteBody(Conn, "\t},\r\n");
		else
			__HTTP_SERVER_WriteBody(Conn, "\t}\r\n");
	}
	__HTTP_SERVER_WriteBody(Conn, "]\r\n");
	__HTTP_SERVER_FinishBody(Conn);
}
