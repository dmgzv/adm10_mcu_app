#ifndef __HTTP_TYPES_H__
#define __HTTP_TYPES_H__

#include "lwip/api.h"

#define HTTP_REQUEST_HEAD	"HEAD"
#define HTTP_REQUEST_GET	"GET"
#define HTTP_REQUEST_PUT	"PUT"
#define HTTP_REQUEST_POST	"POST"
#define HTTP_REQUEST_DELETE	"DELETE"

#define HTTP_RESPONCE_200_OK										"HTTP/1.1 200 OK"
#define HTTP_RESPONCE_301_MOVED_PERMANENTLY			"HTTP/1.1 301 Moved Permanently"
#define HTTP_RESPONCE_400_BAD_REQUEST						"HTTP/1.1 400 Bad Request"
#define HTTP_RESPONCE_401_UNAUTHORIZED					"HTTP/1.1 401 Unauthorized"
#define HTTP_RESPONCE_404_NOT_FOUND							"HTTP/1.1 404 Not Found"
#define HTTP_RESPONCE_405_METHOD_NOT_ALLOWED		"HTTP/1.1 405 Method Not Allowed"
#define HTTP_RESPONCE_500_INTERNAL_SERVER_ERROR	"HTTP/1.1 500 Internal Server Error"

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
	uint32_t TypeSize;
	char *Url;
	uint32_t UrlSize;
	char *Query;
	uint32_t QuerySize;
	char *ContentType;
	uint32_t ContentTypeSize;
	char *ContentLength;
	uint32_t ContentLengthSize;
	char *Connection;
	uint32_t ConnectionSize;
	char *Authorization;
	uint32_t AuthorizationSize;
	char *Body;
	uint32_t BodyLen;
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
} HTTP_HTTP_SERVER_Buffer_T;

#endif /*__HTTP_TYPES_H__*/
