#include "http_server_api_v1_modules.h"
#include "bsp.h"

#define HTTP_SERVER_API_V1_MODULES_URL_TEMPLATE	"/api/v1/modules"

static void HTTP_SERVER_API_V1_MODULES_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_MODULES_Url = {
	.Url = HTTP_SERVER_API_V1_MODULES_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_MODULES_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = HTTP_SERVER_API_V1_MODULES_GetProcessing,
	.PutProcessing = NULL,
	.PostProcessing = NULL,
};

static void HTTP_SERVER_API_V1_MODULES_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	uint32_t i;
	char *Locked;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	netbuf_delete(Inbuf);
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(Conn, &RspHeader))
		return;
	
	http_server_write_body_start(Conn);
	http_server_write_body(Conn, "[\r\n");
	for (i = 0; i < ModulesCnt; i++)
	{
		if (osMutexWait(Modules[i]->DevMutexId, 1) == osOK)
		{
			Locked = "unlocked";
			osMutexRelease(Modules[i]->DevMutexId);
		}
		else
		{
			Locked = "locked";
		}
		http_server_write_body(Conn, "\t{\r\n");
		http_server_write_body(Conn, "\t\t\"name\" : \"%s\",\r\n", Modules[i]->DevName);
		http_server_write_body(Conn, "\t\t\"id\" : \"%s\",\r\n", Modules[i]->DevId);
		http_server_write_body(Conn, "\t\t\"status\" : \"%s\",\r\n", BSP_DEV_GET_STATE_STR(Modules[i]));
		http_server_write_body(Conn, "\t\t\"init_cnt\" : \"%d\",\r\n", Modules[i]->InitCnt);
		http_server_write_body(Conn, "\t\t\"init_err\" : \"%d\",\r\n", Modules[i]->InitErr);
		http_server_write_body(Conn, "\t\t\"init_cont_err\" : \"%d\",\r\n", Modules[i]->InitContErr);
		http_server_write_body(Conn, "\t\t\"init_cont_err_thr\" : \"%d\",\r\n", Modules[i]->InitContErrThr);
		http_server_write_body(Conn, "\t\t\"update_cnt\" : \"%d\",\r\n", Modules[i]->UpdateCnt);
		http_server_write_body(Conn, "\t\t\"update_err\" : \"%d\",\r\n", Modules[i]->UpdateErr);
		http_server_write_body(Conn, "\t\t\"update_cont_err\" : \"%d\",\r\n", Modules[i]->UpdateContErr);
		http_server_write_body(Conn, "\t\t\"update_cont_err_thr\" : \"%d\",\r\n", Modules[i]->UpdateContErrThr);
		http_server_write_body(Conn, "\t\t\"lock\" : \"%s\"\r\n", Locked);
		if (i == (ModulesCnt - 1))
			http_server_write_body(Conn, "\t}\r\n");
		else
			http_server_write_body(Conn, "\t},\r\n");
	}	
	http_server_write_body(Conn, "]\r\n");
	http_server_write_body_finish(Conn);
}
