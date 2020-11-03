#include "http_server_url_api.h"
#include "http_server_url_api_v1_auth.h"
#include "http_server_url_api_v1_log.h"
#include "http_server_url_api_v1_devices.h"
#include "http_server_url_api_v1_modules.h"
#include "http_server_url_api_v1_interfaces.h"
#include "http_server_url_api_v1_power.h"
#include "http_server_url_api_v1_os.h"
#include "http_server_url_api_v1_profile.h"
#include "http_server_url_api_v1_update.h"
#include "http_server_url_api_v1_users.h"
#include <string.h>

const HTTP_SERVER_URL_T *HTTP_SERVER_URL_API[] = {
	&HTTP_SERVER_URL_API_V1_AUTH,
	&HTTP_SERVER_URL_API_V1_USER,
	&HTTP_SERVER_URL_API_V1_USERS,
	&HTTP_SERVER_URL_API_V1_LOG,
	&HTTP_SERVER_URL_API_V1_DEVICES,
	&HTTP_SERVER_URL_API_V1_MODULES,
	&HTTP_SERVER_URL_API_V1_INTERFACES,
	&HTTP_SERVER_URL_API_V1_POWER,
	&HTTP_SERVER_URL_API_V1_OS,
	&HTTP_SERVER_URL_API_V1_PROFILE,
	&HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK,
	&HTTP_SERVER_URL_API_V1_PROFILE_CONFIG,
	&HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES,
	&HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS,
	&HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE,
	&HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS,
	&HTTP_SERVER_URL_API_V1_UPDATE,
	&HTTP_SERVER_URL_API_V1_UPDATE_MCU_SOFTWARE,
	&HTTP_SERVER_URL_API_V1_UPDATE_MCU_WEB,
};
const uint32_t HTTP_SERVER_URL_APISize = sizeof(HTTP_SERVER_URL_API) / sizeof(HTTP_SERVER_URL_API[0]);

int32_t HTTP_SERVER_URL_API_CompareToTemplate(char *Url, uint32_t UrlSize, char *UrlTemplate, uint32_t UrlTemplateSize)
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
		else
		{
			UrlIdx++;
			UrlTemplateIdx++;
		}
	}
	if ((UrlIdx != UrlSize) || (UrlTemplateIdx != UrlTemplateSize))
		return -1;
	return 0;
}

int32_t HTTP_SERVER_URL_API_Process(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i;
	char **Allowed;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;

	for (i = 0; i < HTTP_SERVER_URL_APISize; i++)
	{
		if (!HTTP_SERVER_URL_API_CompareToTemplate(ReqHeader->Url, ReqHeader->UrlSize, HTTP_SERVER_URL_API[i]->Template, HTTP_SERVER_URL_API[i]->TemplateSize))
		{
			if ((!memcmp(ReqHeader->Type, HTTP_REQUEST_HEAD" ", sizeof(HTTP_REQUEST_HEAD))) && (HTTP_SERVER_URL_API[i]->Head))
			{
				if (HTTP_SERVER_URL_API[i]->Auth)
				{
					if (!HTTP_SERVER_URL_API[i]->Auth(Conn, Inbuf, ReqHeader))
						HTTP_SERVER_URL_API[i]->Head(Conn, Inbuf, ReqHeader);
				}
				else
				{
					HTTP_SERVER_URL_API[i]->Head(Conn, Inbuf, ReqHeader);
				}
			}
			else if ((!memcmp(ReqHeader->Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET))) && (HTTP_SERVER_URL_API[i]->Get))
			{
				if (HTTP_SERVER_URL_API[i]->Auth)
				{
					if (!HTTP_SERVER_URL_API[i]->Auth(Conn, Inbuf, ReqHeader))
						HTTP_SERVER_URL_API[i]->Get(Conn, Inbuf, ReqHeader);
				}
				else
				{
					HTTP_SERVER_URL_API[i]->Get(Conn, Inbuf, ReqHeader);
				}
			}
			else if ((!memcmp(ReqHeader->Type, HTTP_REQUEST_PUT" ", sizeof(HTTP_REQUEST_PUT))) && (HTTP_SERVER_URL_API[i]->Put))
			{
				if (HTTP_SERVER_URL_API[i]->Auth)
				{
					if (!HTTP_SERVER_URL_API[i]->Auth(Conn, Inbuf, ReqHeader))
						HTTP_SERVER_URL_API[i]->Put(Conn, Inbuf, ReqHeader);
				}
				else
				{
					HTTP_SERVER_URL_API[i]->Put(Conn, Inbuf, ReqHeader);
				}
			}
			else if ((!memcmp(ReqHeader->Type, HTTP_REQUEST_POST" ", sizeof(HTTP_REQUEST_POST))) && (HTTP_SERVER_URL_API[i]->Post))
			{
				if (HTTP_SERVER_URL_API[i]->Auth)
				{
					if (!HTTP_SERVER_URL_API[i]->Auth(Conn, Inbuf, ReqHeader))
						HTTP_SERVER_URL_API[i]->Post(Conn, Inbuf, ReqHeader);
				}
				else
				{
					HTTP_SERVER_URL_API[i]->Post(Conn, Inbuf, ReqHeader);
				}
			}
			else if ((!memcmp(ReqHeader->Type, HTTP_REQUEST_DELETE" ", sizeof(HTTP_REQUEST_DELETE))) && (HTTP_SERVER_URL_API[i]->Delete))
			{
				if (HTTP_SERVER_URL_API[i]->Auth)
				{
					if (!HTTP_SERVER_URL_API[i]->Auth(Conn, Inbuf, ReqHeader))
						HTTP_SERVER_URL_API[i]->Delete(Conn, Inbuf, ReqHeader);
				}
				else
				{
					HTTP_SERVER_URL_API[i]->Delete(Conn, Inbuf, ReqHeader);
				}
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
				if (HTTP_SERVER_URL_API[i]->Head)
					*Allowed++ = HTTP_REQUEST_HEAD;
				if (HTTP_SERVER_URL_API[i]->Get)
					*Allowed++ = HTTP_REQUEST_GET;
				if (HTTP_SERVER_URL_API[i]->Put)
					*Allowed++ = HTTP_REQUEST_PUT;
				if (HTTP_SERVER_URL_API[i]->Post)
					*Allowed++ = HTTP_REQUEST_POST;
				if (HTTP_SERVER_URL_API[i]->Delete)
					*Allowed++ = HTTP_REQUEST_DELETE;
				RspHeader.AllowedLen = Allowed - RspHeader.Allowed;
				__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
			}
			return 0;
		}
	}
	return -1;
}
