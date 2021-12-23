#include "http_server_url_api_v1_os.h"
#include "user.h"
#include "base64.h"
#include "log.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_OS_TEMPLATE	"/api/v1/os"

static void HTTP_SERVER_URL_API_V1_OS_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_OS = {
	.Template = HTTP_SERVER_URL_API_V1_OS_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_OS_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_OS_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

static void HTTP_SERVER_URL_API_V1_OS_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char thread[512];
	USER_T User;
	char *p, *s;
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
	if (__HTTP_SERVER_WriteBody(Conn, "{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\"heap\" : \"%d\",\r\n", xPortGetFreeHeapSize()))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\"min_heap\" : \"%d\",\r\n", xPortGetMinimumEverFreeHeapSize()))
		return;
	/* The list of tasks and their status */
	osThreadList((unsigned char *)(thread));
	if (__HTTP_SERVER_WriteBody(Conn, "\t\"threads\" : [\r\n"))
		return;
	s = thread;
	while (*s) {
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
			return;
		p = strchr(s, '\t');
		*p++ = 0;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"%s\",\r\n", s))
			return;
		s = p;
		p = strchr(s, '\t');
		*p++ = 0;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"state\" : \"%s\",\r\n", s))
			return;
		s = p;
		p = strchr(s, '\t');
		*p = 0;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"priority\" : \"%s\",\r\n", s))
			return;
		s = p + 1;
		p = strchr(s, '\t');
		*p++ = 0;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"stack\" : \"%s\",\r\n", s))
			return;
		s = p;
		p = strchr(s, '\r');
		*p++ = 0;
		*p++ = 0;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"number\" : \"%s\"\r\n", s))
			return;
		s = p;
		if (*s)
		{
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
				return;
		}
		else
		{
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
				return;
		}
	}
	if (__HTTP_SERVER_WriteBody(Conn, "\t]\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "}\r\n"))
		return;
	if (__HTTP_SERVER_FinishBody(Conn))
		return;
	return;
	
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "OSReadError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}
