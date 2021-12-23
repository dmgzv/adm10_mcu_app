#include "http_server_url_api_v1_power.h"
#include "bsp.h"
#include "user.h"
#include "base64.h"
#include "log.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_POWER_TEMPLATE	"/api/v1/power"

static void HTTP_SERVER_URL_API_V1_POWER_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_POWER = {
	.Template = HTTP_SERVER_URL_API_V1_POWER_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_POWER_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_POWER_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

static void HTTP_SERVER_URL_API_V1_POWER_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i;
	USER_T User;
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
	if (__HTTP_SERVER_WriteBody(Conn, "[\r\n"))
		return;
	for (i = 0; i < UCD90120A_RAILS_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK)
			break;
		if (__HTTP_SERVER_WriteBody(Conn, "\t{\r\n"))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"config\" : \"%.3f\"\r\n,", ucd90120a.Cfg.Rail[i].VoutCmd))
			return;
		if (__HTTP_SERVER_WriteBody(Conn, "\t\t\"monitor\" : \"%.3f\"\r\n", ucd90120a.Mon.Rail[i].Vout))
			return;
		if (i != (UCD90120A_RAILS_CNT - 1))
		{
			if (__HTTP_SERVER_WriteBody(Conn, "\t},\r\n"))
				return;
		}
		else
		{
			if (__HTTP_SERVER_WriteBody(Conn, "\t}\r\n"))
				return;
		}
	}
	if (__HTTP_SERVER_WriteBody(Conn, "]\r\n"))
		return;
	if (__HTTP_SERVER_FinishBody(Conn))
		return;
	return;
	
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "PowerReadError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}
