#include "http_server_api_v1_log.h"
#include "log.h"

#define HTTP_SERVER_API_V1_LOG_URL_TEMPLATE	"/api/v1/log"

static void HTTP_SERVER_API_V1_LOG_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_LOG_Url = {
	.Url = HTTP_SERVER_API_V1_LOG_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_LOG_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = HTTP_SERVER_API_V1_LOG_GetProcessing,
	.PutProcessing = NULL,
	.PostProcessing = NULL,
};

static void HTTP_SERVER_API_V1_LOG_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	uint32_t i, LinesCnt;
	LOG_LINE_T LogLine;
	float Time;
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
	
	LinesCnt = LOG_GetLinesCnt();
	http_server_write_body_start(Conn);
	http_server_write_body(Conn, "{\r\n\t\"max-records-cnt\" : \"%d\",\r\n\t\"records-cnt\" : \"%d\",\r\n\t\"records\" : [\r\n", LOG_LINES_CNT, LinesCnt);
	for (i = 0; i < LinesCnt; i++)
	{
		LOG_GetLine(i, &LogLine);
		Time = (float)LogLine.Time / osKernelSysTickFrequency;
		http_server_write_body(Conn, "\t\t{\r\n");
		http_server_write_body(Conn, "\t\t\t\"time\" : \"%.4f\",\r\n", Time);
		http_server_write_body(Conn, "\t\t\t\"severity\" : \"%d\",\r\n", LogLine.Severity);
		http_server_write_body(Conn, "\t\t\t\"category\" : \"%s\",\r\n", LogLine.Category);
		http_server_write_body(Conn, "\t\t\t\"object\" : \"%s\",\r\n", LogLine.Object);
		http_server_write_body(Conn, "\t\t\t\"object_id\" : \"%s\",\r\n", LogLine.ObjectId);
		http_server_write_body(Conn, "\t\t\t\"description\" : \"%s\"\r\n", LogLine.Description);		
		if (i == (LinesCnt - 1))
			http_server_write_body(Conn, "\t\t}\r\n");
		else
			http_server_write_body(Conn, "\t\t},\r\n");
	}
	http_server_write_body(Conn, "\t]\r\n}\r\n");
	http_server_write_body_finish(Conn);
}
