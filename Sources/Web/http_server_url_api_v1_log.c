#include "http_server_url_api_v1_log.h"
#include "log.h"

#define HTTP_SERVER_URL_API_V1_LOG_TEMPLATE	"/api/v1/log"

static void HTTP_SERVER_URL_API_V1_LOG_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_LOG = {
	.Template = HTTP_SERVER_URL_API_V1_LOG_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_LOG_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_LOG_Get,
	.Put = NULL,
	.Post = NULL,
};

static void HTTP_SERVER_URL_API_V1_LOG_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i, LinesCnt;
	LOG_LINE_T LogLine;
	float Time;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
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
	
	LinesCnt = LOG_GetLinesCnt();
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "{\r\n\t\"max-records-cnt\" : \"%d\",\r\n\t\"records-cnt\" : \"%d\",\r\n\t\"records\" : [\r\n", LOG_LINES_CNT, LinesCnt);
	for (i = 0; i < LinesCnt; i++)
	{
		LOG_GetLine(i, &LogLine);
		Time = (float)LogLine.Time / osKernelSysTickFrequency;
		__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"time\" : \"%.4f\",\r\n", Time);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"severity\" : \"%d\",\r\n", LogLine.Severity);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"category\" : \"%s\",\r\n", LogLine.Category);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"object\" : \"%s\",\r\n", LogLine.Object);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"object_id\" : \"%s\",\r\n", LogLine.ObjectId);
		__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"description\" : \"%s\"\r\n", LogLine.Description);		
		if (i == (LinesCnt - 1))
			__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
		else
			__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
	}
	__HTTP_SERVER_WriteBody(Conn, "\t]\r\n}\r\n");
	__HTTP_SERVER_FinishBody(Conn);
}
