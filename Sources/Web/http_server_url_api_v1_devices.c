#include "http_server_url_api_v1_devices.h"
#include "user.h"
#include "bsp.h"
#include "base64.h"
#include "log.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_DEVICES_TEMPLATE	"/api/v1/devices"

static void HTTP_SERVER_URL_API_V1_DEVICES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_DEVICES = {
	.Template = HTTP_SERVER_URL_API_V1_DEVICES_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_DEVICES_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_DEVICES_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

static void HTTP_SERVER_URL_API_V1_DEVICES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
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
	
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "[\r\n");
	for (i = 0; i < DevicesCnt; i++)
	{
		if (osMutexWait(Devices[i]->DevMutexId, 1) == osOK)
		{
			Locked = "unlocked";
			osMutexRelease(Devices[i]->DevMutexId);
		}
		else
		{
			Locked = "locked";
		}
		__HTTP_SERVER_WriteBody(Conn, "\t{\r\n");
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"name\" : \"%s\",\r\n", Devices[i]->DevName);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"status\" : \"%s\",\r\n", BSP_DEV_GET_STATE_STR(Devices[i]));
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_cnt\" : \"%d\",\r\n", Devices[i]->InitCnt);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_err\" : \"%d\",\r\n", Devices[i]->InitErr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_cont_err\" : \"%d\",\r\n", Devices[i]->InitContErr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"init_cont_err_thr\" : \"%d\",\r\n", Devices[i]->InitContErrThr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_cnt\" : \"%d\",\r\n", Devices[i]->UpdateCnt);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_err\" : \"%d\",\r\n", Devices[i]->UpdateErr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_cont_err\" : \"%d\",\r\n", Devices[i]->UpdateContErr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"update_cont_err_thr\" : \"%d\",\r\n", Devices[i]->UpdateContErrThr);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\"lock\" : \"%s\"\r\n", Locked);
		if (i == (DevicesCnt - 1))
			__HTTP_SERVER_WriteBody(Conn, "\t}\r\n");
		else
			__HTTP_SERVER_WriteBody(Conn, "\t},\r\n");
	}	
	__HTTP_SERVER_WriteBody(Conn, "]\r\n");
	__HTTP_SERVER_FinishBody(Conn);
	return;
	
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "DevicesReadError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}
