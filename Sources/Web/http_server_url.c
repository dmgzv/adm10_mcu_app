#include "http_server_url_api.h"
#include "http_server_url_files.h"
#include <string.h>

typedef struct {
	char *Url;
	uint32_t UrlSize;
	char *RedirUrl;
	uint32_t RedirUrlSize;
} HTTP_SERVER_URL_REDIRECT_T;

static const HTTP_SERVER_URL_REDIRECT_T HTTP_SERVER_URL_REDIRECT_Table[] = {
	{
		.Url = "/",
		.UrlSize = sizeof("/") - 1,
		.RedirUrl = "/index.html",
		.RedirUrlSize = sizeof("/index.html") - 1,
	},
};

static const uint32_t HTTP_SERVER_URL_REDIRECT_TableSize = sizeof(HTTP_SERVER_URL_REDIRECT_Table) / sizeof(HTTP_SERVER_URL_REDIRECT_Table[0]);


static int32_t HTTP_SERVER_URL_Redirect(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i;
	
	for (i = 0; i < HTTP_SERVER_URL_REDIRECT_TableSize; i++)
	{
		if ((ReqHeader->UrlSize == HTTP_SERVER_URL_REDIRECT_Table[i].UrlSize) && \
			(!memcmp(HTTP_SERVER_URL_REDIRECT_Table[i].Url, ReqHeader->Url, HTTP_SERVER_URL_REDIRECT_Table[i].UrlSize)))
		{
			ReqHeader->Url = (char*)HTTP_SERVER_URL_REDIRECT_Table[i].RedirUrl;
			ReqHeader->UrlSize = HTTP_SERVER_URL_REDIRECT_Table[i].RedirUrlSize;
			return -1;
		}
		else if ((ReqHeader->UrlSize == HTTP_SERVER_URL_REDIRECT_Table[i].RedirUrlSize) && \
			(!memcmp(HTTP_SERVER_URL_REDIRECT_Table[i].RedirUrl, ReqHeader->Url, HTTP_SERVER_URL_REDIRECT_Table[i].RedirUrlSize)))
		{
			netbuf_delete(Inbuf);
			__HTTP_SERVER_WriteError301(Conn, HTTP_SERVER_URL_REDIRECT_Table[i].Url);
			return 0;
		}
	}
	return -1;
}

void HTTP_SERVER_URL_Process(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	
	if (!HTTP_SERVER_URL_Redirect(Conn, Inbuf, ReqHeader))
		return;	
	if (!HTTP_SERVER_URL_API_Process(Conn, Inbuf, ReqHeader))
		return;
	if (!HTTP_SERVER_URL_FILES_Process(Conn, Inbuf, ReqHeader))
		return;
	
	netbuf_delete(Inbuf);
	__HTTP_SERVER_WriteError404(Conn);
}
