#include "http_server_url.h"
#include "http_server_api_v1_log.h"
#include <string.h>

const HTTP_SERVER_URL_T *HTTP_SERVER_URL_List[] = {
	&HTTP_SERVER_API_V1_LOG_Url,
};
const uint32_t HTTP_SERVER_URL_ListSize = sizeof(HTTP_SERVER_URL_List) / sizeof(HTTP_SERVER_URL_List[0]);

int32_t HTTP_SERVER_URL_CompareToTemplate(char *Url, uint32_t UrlSize, char *UrlTemplate, uint32_t UrlTemplateSize)
{
	uint32_t UrlIdx, UrlTemplateIdx;
	
	UrlIdx = 0;
	UrlTemplateIdx = 0;
	while ((UrlIdx < UrlSize) && (UrlTemplateIdx < UrlTemplateSize))
	{
		if (UrlTemplate[UrlTemplateIdx] == '*')
		{
			while(1)
			{
				if (((Url[UrlIdx] >= '0') && (Url[UrlIdx] <= '9')) || \
						((Url[UrlIdx] >= 'a') && (Url[UrlIdx] <= 'z')) || \
						((Url[UrlIdx] >= 'A') && (Url[UrlIdx] <= 'Z')) ||\
						(Url[UrlIdx] == '.') || \
						(Url[UrlIdx] == '_') || \
						(Url[UrlIdx] == '-') || \
						(Url[UrlIdx] == '~'))
					UrlIdx++;
				else
					break;
			}
			UrlTemplateIdx++;
		}
		else if (Url[UrlIdx] != UrlTemplate[UrlTemplateIdx])
		{
			return -1;
		}
		UrlIdx++;
		UrlTemplateIdx++;
	}
	if ((UrlIdx != UrlSize) || (UrlTemplateIdx != UrlTemplateSize))
		return -1;
	return 0;
}

int32_t HTTP_SERVER_URL_Process(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	uint32_t i;
	char **Allowed;
	HTTP_RESPONCE_HEADER_T RspHeader;

	for (i = 0; i < HTTP_SERVER_URL_ListSize; i++)
	{
		if (!HTTP_SERVER_URL_CompareToTemplate(ReqHeader->Url, ReqHeader->UrlSize, HTTP_SERVER_URL_List[i]->Url, HTTP_SERVER_URL_List[i]->UrlSize))
		{
			if ((!memcmp(ReqHeader->Type, HTTP_REQUEST_HEAD" ", sizeof(HTTP_REQUEST_HEAD))) && (HTTP_SERVER_URL_List[i]->HeadProcessing))
			{
				HTTP_SERVER_URL_List[i]->GetProcessing(Conn, Inbuf, ReqHeader, Body, BodyLen);
			}
			else if ((!memcmp(ReqHeader->Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET))) && (HTTP_SERVER_URL_List[i]->GetProcessing))
			{
				HTTP_SERVER_URL_List[i]->GetProcessing(Conn, Inbuf, ReqHeader, Body, BodyLen);
			}
			else if ((!memcmp(ReqHeader->Type, HTTP_REQUEST_PUT" ", sizeof(HTTP_REQUEST_PUT))) && (HTTP_SERVER_URL_List[i]->PutProcessing))
			{
				HTTP_SERVER_URL_List[i]->PutProcessing(Conn, Inbuf, ReqHeader, Body, BodyLen);
			}
			else if ((!memcmp(ReqHeader->Type, HTTP_REQUEST_POST" ", sizeof(HTTP_REQUEST_POST))) && (HTTP_SERVER_URL_List[i]->PostProcessing))
			{
				HTTP_SERVER_URL_List[i]->PutProcessing(Conn, Inbuf, ReqHeader, Body, BodyLen);
			}
			else
			{
				netbuf_delete(Inbuf);
				RspHeader.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
				RspHeader.ContentType = NULL;
				RspHeader.Charset = NULL;
				RspHeader.ContentLength = 0;
				RspHeader.Connection = HTTP_CONNECTION_CLOSE;
				RspHeader.TransferEncoding = NULL;
				Allowed = RspHeader.Allowed;
				if (HTTP_SERVER_URL_List[i]->HeadProcessing)
					*Allowed++ = HTTP_REQUEST_HEAD;
				if (HTTP_SERVER_URL_List[i]->GetProcessing)
					*Allowed++ = HTTP_REQUEST_GET;
				if (HTTP_SERVER_URL_List[i]->PutProcessing)
					*Allowed++ = HTTP_REQUEST_PUT;
				if (HTTP_SERVER_URL_List[i]->PostProcessing)
					*Allowed++ = HTTP_REQUEST_POST;
				RspHeader.AllowedLen = Allowed - RspHeader.Allowed;
				http_server_write_header(Conn, &RspHeader);
			}
			return 0;
		}
	}
	return -1;
}
