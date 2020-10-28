#include "http_server_url_api_v1_os.h"
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
};

static void HTTP_SERVER_URL_API_V1_OS_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char thread[512];
	char *p, *s;
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
	__HTTP_SERVER_WriteBody(Conn, "{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\"heap\" : \"%d\",\r\n", xPortGetFreeHeapSize());
  /* The list of tasks and their status */
	osThreadList((unsigned char *)(thread));
  __HTTP_SERVER_WriteBody(Conn, "\t\"threads\" : [\r\n");
	s = thread;
	while (*s) {
		__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
		p = strchr(s, '\t');
		*p++ = 0;
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"%s\",\r\n", s);
		s = p;
		p = strchr(s, '\t');
		*p++ = 0;
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"state\" : \"%s\",\r\n", s);
		s = p;
		p = strchr(s, '\t');
		*p = 0;
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"priority\" : \"%s\",\r\n", s);
		s = p + 1;
		p = strchr(s, '\t');
		*p++ = 0;
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"stack\" : \"%s\",\r\n", s);
		s = p;
		p = strchr(s, '\r');
		*p++ = 0;
		*p++ = 0;
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"number\" : \"%s\"\r\n", s);
		s = p;
		if (*s)
			__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
		else
			__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");	
	}
	__HTTP_SERVER_WriteBody(Conn, "\t]\r\n");
	__HTTP_SERVER_WriteBody(Conn, "}\r\n");
	__HTTP_SERVER_FinishBody(Conn);
}
