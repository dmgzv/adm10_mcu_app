#include "http_server_url_api_v1_log.h"
#include "log.h"
#include "user.h"
#include "base64.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_LOG_TEMPLATE	"/api/v1/log"

static void HTTP_SERVER_URL_API_V1_LOG_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_LOG = {
	.Template = HTTP_SERVER_URL_API_V1_LOG_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_LOG_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_LOG_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

static void HTTP_SERVER_URL_API_V1_LOG_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i, LinesCnt, __LinesCnt;
	LOG_LINE_T LogLine;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
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
	
	__LinesCnt = 0;
	LinesCnt = LOG_GetLinesCnt();
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "{\t\"records\" : [\r\n");
	for (i = 0; i < LinesCnt; i++)
	{
		LOG_GetLine(i, &LogLine);
		if (
			((!strcmp(User.Group, USER_GROUP_MONITOR)) && (LogLine.Access == LOG_ACCESS_MONITOR))
			|| ((!strcmp(User.Group, USER_GROUP_USER)) && ((LogLine.Access == LOG_ACCESS_MONITOR) || (LogLine.Access == LOG_ACCESS_USER)))
			|| ((!strcmp(User.Group, USER_GROUP_ADMIN)) && ((LogLine.Access == LOG_ACCESS_MONITOR) || (LogLine.Access == LOG_ACCESS_USER) || (LogLine.Access == LOG_ACCESS_ADMIN)))
			|| (!strcmp(User.Group, USER_GROUP_SUPERUSER))
		)
		{
			if (__LinesCnt != 0)
				__HTTP_SERVER_WriteBody(Conn, ",\r\n");
			__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"index\" : \"%u\",\r\n", LogLine.Index);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timestamp\" : \"%llu\",\r\n", LogLine.TimeStamp);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeticks\" : \"%u\",\r\n", LogLine.TimeTicks);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"severity\" : \"%d\",\r\n", LogLine.Severity);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"category\" : \"%s\",\r\n", LogLine.Category);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"subject\" : \"%s\",\r\n", LogLine.Subject);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"operation\" : \"%s\",\r\n", LogLine.Operation);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"description\" : \"%s\"\r\n", LogLine.Description);		
			__HTTP_SERVER_WriteBody(Conn, "\t\t}");
			__LinesCnt++;
		}
	}
	__HTTP_SERVER_WriteBody(Conn, "\r\n\t],\t\"records_cnt\" : \"%d\",\r\n\t\"records_max_cnt\" : \"%d\"\r\n}\r\n", LinesCnt, LOG_LINES_MAX_CNT);
	__HTTP_SERVER_FinishBody(Conn);
}
