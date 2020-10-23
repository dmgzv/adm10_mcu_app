#include "http_server_api_v1_os.h"
#include <string.h>

#define HTTP_SERVER_API_V1_OS_URL_TEMPLATE	"/api/v1/os"

static void HTTP_SERVER_API_V1_OS_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_OS_Url = {
	.Url = HTTP_SERVER_API_V1_OS_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_OS_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = HTTP_SERVER_API_V1_OS_GetProcessing,
	.PutProcessing = NULL,
	.PostProcessing = NULL,
};

static void HTTP_SERVER_API_V1_OS_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	char thread[512];
	char *p, *s;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(Conn, &RspHeader))
		return;
	
	http_server_write_body_start(Conn);
	http_server_write_body(Conn, "{\r\n");
	http_server_write_body(Conn, "\t\"heap\" : \"%d\",\r\n", xPortGetFreeHeapSize());
  /* The list of tasks and their status */
	osThreadList((unsigned char *)(thread));
  http_server_write_body(Conn, "\t\"threads\" : [\r\n");
	s = thread;
	while (*s) {
		http_server_write_body(Conn, "\t\t{\r\n");
		p = strchr(s, '\t');
		*p++ = 0;
		http_server_write_body(Conn, "\t\t\t\"name\" : \"%s\",\r\n", s);
		s = p;
		p = strchr(s, '\t');
		*p++ = 0;
		http_server_write_body(Conn, "\t\t\t\"state\" : \"%s\",\r\n", s);
		s = p;
		p = strchr(s, '\t');
		*p = 0;
		http_server_write_body(Conn, "\t\t\t\"priority\" : \"%s\",\r\n", s);
		s = p + 1;
		p = strchr(s, '\t');
		*p++ = 0;
		http_server_write_body(Conn, "\t\t\t\"stack\" : \"%s\",\r\n", s);
		s = p;
		p = strchr(s, '\r');
		*p++ = 0;
		*p++ = 0;
		http_server_write_body(Conn, "\t\t\t\"number\" : \"%s\"\r\n", s);
		s = p;
		if (*s)
			http_server_write_body(Conn, "\t\t},\r\n");
		else
			http_server_write_body(Conn, "\t\t}\r\n");	
	}
	http_server_write_body(Conn, "\t]\r\n");
	http_server_write_body(Conn, "}\r\n");
	http_server_write_body_finish(Conn);
}
