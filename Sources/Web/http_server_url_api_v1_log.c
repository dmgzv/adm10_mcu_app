#include "http_server_url_api_v1_log.h"
#include "log.h"
#include "user.h"
#include "base64.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_LOG_TEMPLATE	"/api/v1/log"

static void HTTP_SERVER_URL_API_V1_LOG_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_LOG_Delete(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_LOG = {
	.Template = HTTP_SERVER_URL_API_V1_LOG_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_LOG_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_LOG_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = HTTP_SERVER_URL_API_V1_LOG_Delete,
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
	if (__HTTP_SERVER_StartBody(Conn))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "{\t\"records\" : [\r\n"))
		return;
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
				if (__HTTP_SERVER_WriteBody(Conn, ",\r\n"))
					return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"index\" : \"%u\",\r\n", LogLine.Index))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timestamp\" : \"%llu\",\r\n", LogLine.TimeStamp))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeticks\" : \"%u\",\r\n", LogLine.TimeTicks))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"severity\" : \"%d\",\r\n", LogLine.Severity))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"category\" : \"%s\",\r\n", LogLine.Category))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"subject\" : \"%s\",\r\n", LogLine.Subject))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"operation\" : \"%s\",\r\n", LogLine.Operation))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"description\" : \"%s\"\r\n", LogLine.Description))
				return;
			if (__HTTP_SERVER_WriteBody(Conn, "\t\t}"))
				return;
			__LinesCnt++;
		}
	}
	if (__HTTP_SERVER_WriteBody(Conn, "\r\n\t],\t\"records_cnt\" : \"%d\",\r\n\t\"records_max_cnt\" : \"%d\"\r\n}\r\n", LinesCnt, LOG_LINES_MAX_CNT))
		return;
	if (__HTTP_SERVER_FinishBody(Conn))
		return;
}

static void HTTP_SERVER_URL_API_V1_LOG_Delete(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	if (strcmp(User.Group, USER_GROUP_SUPERUSER))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
	if (LOG_Erase())
		goto error_500;
	
	netbuf_delete(Inbuf);
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "LogDelete", "");
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	return;
	
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "LogDeleteError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
error_500:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "LogDeleteError", "Internal error");
	__HTTP_SERVER_WriteError500(Conn);
	return;
}
