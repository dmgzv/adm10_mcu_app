#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include "lwip/api.h"

#define HTTP_REQUEST_HEAD	"HEAD"
#define HTTP_REQUEST_GET	"GET"
#define HTTP_REQUEST_PUT	"PUT"
#define HTTP_REQUEST_POST	"POST"

#define HTTP_RESPONCE_200_OK									"HTTP/1.1 200 OK"
#define HTTP_RESPONCE_400_BAD_REQUEST					"HTTP/1.1 400 Bad Request"
#define HTTP_RESPONCE_404_NOT_FOUND						"HTTP/1.1 404 Not Found"
#define HTTP_RESPONCE_405_METHOD_NOT_ALLOWED	"HTTP/1.1 405 Method Not Allowed"

#define HTTP_CHARSET_UTF_8						"utf-8"
#define HTTP_CHARSET_WINDOWS_1251			"windows-1251"

#define HTTP_CONTENT_TYPE_TEXT_HTML		"text/html"
#define HTTP_CONTENT_TYPE_TEXT_CSS		"text/css"
#define HTTP_CONTENT_TYPE_TEXT_JS			"text/javascript"
#define HTTP_CONTENT_TYPE_TEXT_PLAIN	"text/plain"
#define HTTP_CONTENT_TYPE_APP_JSON		"application/json"
#define HTTP_CONTENT_TYPE_APP_OCTET		"application/octet-stream"
#define HTTP_CONTENT_TYPE_FONT_WOFF2	"font/woff2"

#define HTTP_TRANSFER_ENCODING_CHUNKED	"chunked"

#define HTTP_CONNECTION_CLOSE "close"
#define HTTP_CONNECTION_KEEP_ALIVE "keep-alive"

typedef struct {	
	char *Type;
	char *Url;
	char *ContentType;
	char *Charset;
	uint32_t ContentLength;
	char *Connection;
	uint32_t ConnectionKeepAlive;
} HTTP_REQUEST_HEADER_T;

typedef struct {
	char *Code;
	char *ContentType;
	char *Charset;
	uint32_t ContentLength;
	char *Connection;
	char *TransferEncoding;
	char *Allowed[4];
	uint32_t AllowedLen;
} HTTP_RESPONCE_HEADER_T;

typedef struct {
	char *Url;
	uint32_t UrlSize;
	void (*HeadProcessing)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
	void (*GetProcessing)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
	void (*PutProcessing)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
	void (*PostProcessing)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
} HTTP_URL_T;

int32_t http_server_write_400_error(struct netconn *conn); 
int32_t http_server_write_404_error(struct netconn *conn);
int32_t http_server_write_header(struct netconn *conn, HTTP_RESPONCE_HEADER_T *Header);
int32_t http_server_write_body_start(struct netconn *conn);
int32_t http_server_write_body(struct netconn *conn, char *str, ...);
int32_t http_server_write_body_finish(struct netconn *conn);

void HTTP_SERVER_Init(void);

#endif /* __HTTP_SERVER_H__ */
