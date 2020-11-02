#ifndef __HTTP_SERVER_H__
#define __HTTP_SERVER_H__

#include "http_types.h"

typedef struct {
	char *Template;
	uint32_t TemplateSize;
	void (*Head)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
	void (*Get)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
	void (*Put)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
	void (*Post)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
	void (*Delete)(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
} HTTP_SERVER_URL_T;

int32_t __HTTP_SERVER_WriteError301(struct netconn *conn, char *Location);
int32_t __HTTP_SERVER_WriteError400(struct netconn *conn); 
int32_t __HTTP_SERVER_WriteError404(struct netconn *conn);
int32_t __HTTP_SERVER_WriteError500(struct netconn *conn);
int32_t __HTTP_SERVER_WriteHeader(struct netconn *conn, HTTP_HTTP_SERVER_Buffer_T *Header);
int32_t __HTTP_SERVER_StartBody(struct netconn *conn);
int32_t __HTTP_SERVER_WriteBody(struct netconn *conn, char *str, ...);
int32_t __HTTP_SERVER_WriteBodyData(struct netconn *conn, char *data, uint32_t datalen);
int32_t __HTTP_SERVER_FinishBody(struct netconn *conn);

void HTTP_SERVER_Init(void);

#endif /* __HTTP_SERVER_H__ */
