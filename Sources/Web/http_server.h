#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include "http_types.h"

typedef struct {
	char *Url;
	uint32_t UrlSize;
	void (*HeadProcessing)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
	void (*GetProcessing)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
	void (*PutProcessing)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
	void (*PostProcessing)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
} HTTP_SERVER_URL_T;

int32_t http_server_write_400_error(struct netconn *conn); 
int32_t http_server_write_404_error(struct netconn *conn);
int32_t http_server_write_500_error(struct netconn *conn);
int32_t http_server_write_header(struct netconn *conn, HTTP_RESPONCE_HEADER_T *Header);
int32_t http_server_write_body_start(struct netconn *conn);
int32_t http_server_write_body(struct netconn *conn, char *str, ...);
int32_t http_server_write_body_data(struct netconn *conn, char *data, uint32_t datalen);
int32_t http_server_write_body_finish(struct netconn *conn);

void HTTP_SERVER_Init(void);

#endif /* __HTTP_SERVER_H__ */
