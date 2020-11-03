#include "http_server_url_api_v1_log.h"
#include "log.h"
#include "user.h"
#include "base64.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_LOG_TEMPLATE	"/api/v1/log"

static int32_t HTTP_SERVER_URL_API_V1_LOG_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_LOG_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_LOG = {
	.Template = HTTP_SERVER_URL_API_V1_LOG_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_LOG_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_LOG_Auth,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_LOG_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

static int32_t HTTP_SERVER_URL_API_V1_LOG_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
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

static void HTTP_SERVER_URL_API_V1_LOG_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i, LinesCnt;
	LOG_LINE_T LogLine;
	float Time;
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
	
	LinesCnt = LOG_GetLinesCnt();
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "{\r\n\t\"max-records-cnt\" : \"%d\",\r\n\t\"records-cnt\" : \"%d\",\r\n\t\"records\" : [\r\n", LOG_LINES_CNT, LinesCnt);
	for (i = 0; i < LinesCnt; i++)
	{
		LOG_GetLine(i, &LogLine);
		Time = (float)LogLine.Time / osKernelSysTickFrequency;
		__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"time\" : \"%.4f\",\r\n", Time);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"severity\" : \"%d\",\r\n", LogLine.Severity);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"category\" : \"%s\",\r\n", LogLine.Category);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"object\" : \"%s\",\r\n", LogLine.Object);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"object_id\" : \"%s\",\r\n", LogLine.ObjectId);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"description\" : \"%s\"\r\n", LogLine.Description);		
		if (i == (LinesCnt - 1))
			__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
		else
			__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
	}
	__HTTP_SERVER_WriteBody(Conn, "\t]\r\n}\r\n");
	__HTTP_SERVER_FinishBody(Conn);
}
