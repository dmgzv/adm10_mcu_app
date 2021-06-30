#ifndef __HTTP_SERVER_URL_API_H__
#define __HTTP_SERVER_URL_API_H__

#include "http_server.h"

int32_t HTTP_SERVER_URL_API_Process(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

#endif /*__HTTP_SERVER_URL_API_H__*/
