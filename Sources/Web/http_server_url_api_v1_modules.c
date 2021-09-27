#include "http_server_url_api_v1_modules.h"
#include "bsp.h"
#include "user.h"
#include "base64.h"
#include "log.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_MODULES_TEMPLATE	"/api/v1/modules"

static void HTTP_SERVER_URL_API_V1_MODULES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_MODULES = {
	.Template = HTTP_SERVER_URL_API_V1_MODULES_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_MODULES_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_MODULES_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

static void HTTP_SERVER_URL_API_V1_MODULES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i;
	USER_T User;
	char *Locked;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	if (strcmp(User.Group, USER_GROUP_SUPERUSER))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
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
	
	if (__HTTP_SERVER_StartBody(Conn))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "[\r\n"))
		return;
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
		if (__HTTP_SERVER_WriteBody(Conn, "\t{\r\n"))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"name\" : \"%s\",\r\n", Modules[i]->DevName))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"status\" : \"%s\",\r\n", BSP_DEV_GET_STATE_STR(Modules[i])))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_cnt\" : \"%d\",\r\n", Modules[i]->InitCnt))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_err\" : \"%d\",\r\n", Modules[i]->InitErr))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_cont_err\" : \"%d\",\r\n", Modules[i]->InitContErr))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_cont_err_thr\" : \"%d\",\r\n", Modules[i]->InitContErrThr))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_cnt\" : \"%d\",\r\n", Modules[i]->UpdateCnt))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_err\" : \"%d\",\r\n", Modules[i]->UpdateErr))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_cont_err\" : \"%d\",\r\n", Modules[i]->UpdateContErr))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_cont_err_thr\" : \"%d\",\r\n", Modules[i]->UpdateContErrThr))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"lock\" : \"%s\"\r\n", Locked))
			return;
		if (i == (ModulesCnt - 1))
		{
			if (__HTTP_SERVER_WriteBody(Conn, "\t}\r\n"))
				return;
		}
		else
		{
			if (__HTTP_SERVER_WriteBody(Conn, "\t},\r\n"))
				return;
		}
	}	
	if (__HTTP_SERVER_WriteBody(Conn, "]\r\n"))
		return;
	if (__HTTP_SERVER_FinishBody(Conn))
		return;
	return;
	
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ModulesReadError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}
