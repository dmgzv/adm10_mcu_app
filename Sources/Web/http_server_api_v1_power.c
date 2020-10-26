#include "http_server_api_v1_power.h"
#include "bsp.h"

#define HTTP_SERVER_API_V1_POWER_URL_TEMPLATE	"/api/v1/power"

static void HTTP_SERVER_API_V1_POWER_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_POWER_Url = {
	.Url = HTTP_SERVER_API_V1_POWER_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_POWER_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = HTTP_SERVER_API_V1_POWER_GetProcessing,
	.PutProcessing = NULL,
	.PostProcessing = NULL,
};

static void HTTP_SERVER_API_V1_POWER_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	uint32_t i;
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
	for (i = 0; i < UCD90120A_RAILS_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK)
			break;
		http_server_write_body(Conn, "\t{\r\n");
		http_server_write_body(Conn, "\t\t\"config\" : \"%.3f\"\r\n,", ucd90120a.Cfg.Rail[i].VoutCmd);
		http_server_write_body(Conn, "\t\t\"monitor\" : \"%.3f\"\r\n", ucd90120a.Mon.Rail[i].Vout);
		if (i != (UCD90120A_RAILS_CNT - 1))
			http_server_write_body(Conn, "\t},\r\n");
		else
			http_server_write_body(Conn, "\t}\r\n");
	}
	http_server_write_body(Conn, "]\r\n");
	http_server_write_body_finish(Conn);
}
