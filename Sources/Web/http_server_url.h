#ifndef __HTTP_SERVER_URL_H__
#define __HTTP_SERVER_URL_H__

#include "http_server.h"

void HTTP_SERVER_URL_Process(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

#endif /*__HTTP_SERVER_URL_H__*/
