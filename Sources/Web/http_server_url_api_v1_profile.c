#include "http_server_url_api_v1_profile.h"
#include "profile.h"
#include "profile_flash.h"
#include "user.h"
#include "base64.h"
#include <string.h>

extern const PROFILE_T *Profile;
extern PROFILE_FLASH_T ProfileFlash;

#define HTTP_SERVER_URL_API_V1_PROFILE_TEMPLATE	"/api/v1/profile"
#define HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_TEMPLATE	"/api/v1/profile/langpack"
#define HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_TEMPLATE	"/api/v1/profile/config"
#define HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES_TEMPLATE	"/api/v1/profile/sections/*/values"
#define HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS_TEMPLATE	"/api/v1/profile/sections/*/colors"
#define HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE_TEMPLATE	"/api/v1/profile/sections/*/params/*/value"
#define HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS_TEMPLATE	"/api/v1/profile/sections/*/params/*/colors"

static int32_t HTTP_SERVER_URL_API_V1_PROFILE_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

static void HTTP_SERVER_URL_API_V1_PROFILE_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Head(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_PROFILE_Auth,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_PROFILE_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_PROFILE_Auth,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_Get,
	.Put = HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_Put,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_CONFIG = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_PROFILE_Auth,
	.Head = HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Head,
	.Get = HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Get,
	.Put = HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Put,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_PROFILE_Auth,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_PROFILE_Auth,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_PROFILE_Auth,
	.Head = NULL,
	.Get = NULL,
	.Put = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE_Put,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_PROFILE_Auth,
	.Head = NULL,
	.Get = NULL,
	.Put = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS_Put,
	.Post = NULL,
	.Delete = NULL,
};

static int32_t HTTP_SERVER_URL_API_V1_PROFILE_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *Base64, *Login, *Password, *c;
	USER_T User;
	char Buffer[34];
	uint32_t Base64Len, i;
	
	if ((!ReqHeader->Authorization) || (ReqHeader->AuthorizationSize < sizeof("Basic ")))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	if (!memcpy(ReqHeader->Authorization, "Basic ", sizeof("Basic ") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	Base64 = &ReqHeader->Authorization[sizeof("Basic ") - 1];
	ReqHeader->Authorization[ReqHeader->AuthorizationSize] = 0;
	Base64Len = strlen(Base64);
	if ((Base64Len % 4) || ((Base64Len * 3 / 4) >= sizeof(Buffer)))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	c = b64tobin(Buffer, Base64);
	if (!c)
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	*c = 0;
	
	for (i = 0; i < sizeof(Buffer); i++)
	{
		if (Buffer[i] == 0)
		{
			netbuf_delete(Inbuf);
			goto error_401;
		}
		else if (Buffer[i] == ':')
		{
			break;
		}
	}
	
	if (i == sizeof(Buffer))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	Buffer[i] = 0;
	
	Login = Buffer;
	Password = &Buffer[i + 1];
	
	if (USER_GetUserByLogin(Login, &User))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	if (strcmp(Password, User.Password))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	return 0;
	
error_401:
	__HTTP_SERVER_WriteError401(Conn);
	return -1;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	netbuf_delete(Inbuf);
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	__HTTP_SERVER_StartBody(Conn);
	for (i = 0; i < *Profile->LinesCnt; i++)
	{
		__HTTP_SERVER_WriteBodyData(Conn, (char*)Profile->Lines[i], strlen(Profile->Lines[i]));
		__HTTP_SERVER_WriteBody(Conn, "\r\n");
	}
	__HTTP_SERVER_FinishBody(Conn);
}

static void HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i;
	char LangpackBlock[PROFILE_FLASH_LANG_BLOCK_SIZE + 1];
	uint32_t LangpackBlocks, LangpackType;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	netbuf_delete(Inbuf);
	
	if (PROFILE_FLASH_LoadLangpackHeader(&ProfileFlash, &LangpackBlocks, &LangpackType))
		goto error_500;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	__HTTP_SERVER_StartBody(Conn);
	for (i = 0; i < LangpackBlocks; i++)
	{
		if (PROFILE_FLASH_LoadLangpackData(&ProfileFlash, i * PROFILE_FLASH_LANG_BLOCK_SIZE, LangpackBlock, PROFILE_FLASH_LANG_BLOCK_SIZE))
			return;
		LangpackBlock[PROFILE_FLASH_LANG_BLOCK_SIZE] = 0;
		__HTTP_SERVER_WriteBody(Conn, LangpackBlock);
	}
	__HTTP_SERVER_FinishBody(Conn);
	return;

error_500:
	__HTTP_SERVER_WriteError500(Conn);
}

static void HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	err_t recv_err;
	char *data;
	uint16_t datalen;
	uint32_t LangpackAddr, LangpackBlocks;
	uint32_t ContentLength;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;

	if (!ReqHeader->ContentLength)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if ((ContentLength == 0) || (ContentLength > PROFILE_FLASH_LANG_RECORD_BODY_SIZE))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	LangpackAddr = 0;
	LangpackBlocks = (ContentLength % PROFILE_FLASH_LANG_BLOCK_SIZE) ? ((ContentLength / PROFILE_FLASH_LANG_BLOCK_SIZE) + 1) : (ContentLength / PROFILE_FLASH_LANG_BLOCK_SIZE);
	if (ReqHeader->Body)
	{
		data = ReqHeader->Body;
		datalen = ReqHeader->BodyLen;
		goto write_data;
	}
	netbuf_delete(Inbuf);
	
	while(ContentLength) {
		recv_err = netconn_recv(Conn, &Inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(Inbuf, (void**)&data, &datalen);
write_data:
			ContentLength -= datalen;
			if (PROFILE_FLASH_SaveLangpackData(&ProfileFlash, LangpackAddr, data, datalen))
			{
				netbuf_delete(Inbuf);
				goto error_500;
			}
			LangpackAddr += datalen;
			netbuf_delete(Inbuf);
			
		}
		else
		{
			return;
		}
	}
	
	if (PROFILE_FLASH_SaveLangpackHeader(&ProfileFlash, LangpackBlocks, 1))
		goto error_500;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	return;
	
error_400:
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_500:
	__HTTP_SERVER_WriteError500(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Head(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char Buffer[128];
	char ValueString[sizeof(PROFILE_VALUE_T)];
	uint32_t ContentLength;
	uint8_t NColor, DColor, FColor;
	uint32_t i, j, Len;
	PROFILE_VALUE_T Value;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	ContentLength = 0;
	ContentLength += sprintf(Buffer, "#"BSP_PT_NUMBER";"BSP_HW_VERSION";"BSP_SW_VERSION"\r\n");
	for (i = 0; i < Profile->SectionsCnt; i++)
	{
		for (j = 0; j < Profile->Sections[i]->ParamsCnt; j++)
		{
			if (!PROFILE_IsConfigParam(Profile, Profile->Sections[i]->Params[j]))
			{
				__PROFILE_GetParamValue(Profile, Profile->Sections[i]->Params[j], &Value);
				PROFILE_ParamValueToString(Profile->Sections[i]->Params[j]->Type, &Value, ValueString, &Len);
				ContentLength += sprintf(Buffer, "%d;%s;0;%s\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, ValueString);
			}
			if (Profile->Sections[i]->Params[j]->Colors)
			{
				__PROFILE_GetParamColors(Profile, Profile->Sections[i]->Params[j], &NColor, &DColor, &FColor);
				ContentLength += sprintf(Buffer, "%d;%s;1;%d\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, NColor);
				ContentLength += sprintf(Buffer, "%d;%s;2;%d\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, DColor);
				ContentLength += sprintf(Buffer, "%d;%s;3;%d\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, FColor);
			}
		}
	}
	
	netbuf_delete(Inbuf);
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = ContentLength;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint8_t NColor, DColor, FColor;
	uint32_t i, j, Len;
	PROFILE_VALUE_T Value;
	char ValueString[64];
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	netbuf_delete(Inbuf);
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	__HTTP_SERVER_StartBody(Conn);	
	__HTTP_SERVER_WriteBody(Conn, "#"BSP_PT_NUMBER";"BSP_HW_VERSION";"BSP_SW_VERSION"\r\n");
	for (i = 0; i < Profile->SectionsCnt; i++)
	{
		for (j = 0; j < Profile->Sections[i]->ParamsCnt; j++)
		{
			if (!PROFILE_IsConfigParam(Profile, Profile->Sections[i]->Params[j]))
			{
				__PROFILE_GetParamValue(Profile, Profile->Sections[i]->Params[j], &Value);
				PROFILE_ParamValueToString(Profile->Sections[i]->Params[j]->Type, &Value, ValueString, &Len);
				__HTTP_SERVER_WriteBody(Conn, "%d;%s;0;%s\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, ValueString);
			}
			if (Profile->Sections[i]->Params[j]->Colors)
			{
				__PROFILE_GetParamColors(Profile, Profile->Sections[i]->Params[j], &NColor, &DColor, &FColor);
				__HTTP_SERVER_WriteBody(Conn, "%d;%s;1;%d\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, NColor);
				__HTTP_SERVER_WriteBody(Conn, "%d;%s;2;%d\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, DColor);
				__HTTP_SERVER_WriteBody(Conn, "%d;%s;3;%d\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, FColor);
			}
		}
	}
	__HTTP_SERVER_FinishBody(Conn);	
}

static void HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *p, *Section, *Name, *Value, *Type, *data;
	char Buffer[256];
	uint32_t BufferLen;
	uint8_t Color;
	uint16_t datalen;
	uint32_t ContentLength, Error, SectionIdx, NameLen;
	PROFILE_PARAM_T *Param;
	PROFILE_VALUE_T ParamValue;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	err_t recv_err;
	
	if (!ReqHeader->ContentLength)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if (ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	BufferLen = 0;
	Error = 0;
	PROFILE_MUTEX_LOCK(Profile);
	if (ReqHeader->Body)
	{
		data = ReqHeader->Body;
		datalen = ReqHeader->BodyLen;
		goto parse_body;
	}
	netbuf_delete(Inbuf);
	
	while(ContentLength)
	{
		recv_err = netconn_recv(Conn, &Inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(Inbuf, (void**)&data, &datalen);
parse_body:
			ContentLength -= datalen;
			for (p = data; (p < (data + datalen)) && (!Error); p++)
			{
				if (*p == '\n')
				{
					if (Buffer[BufferLen - 1] == '\r')
					{
						Section = Buffer;
						if (*Section == '#')
						{
							if (memcmp(Section, "#"BSP_PT_NUMBER";"BSP_HW_VERSION";"BSP_SW_VERSION"\r", sizeof("#"BSP_PT_NUMBER";"BSP_HW_VERSION";"BSP_SW_VERSION"\r") - 1))
							{
								Error = 1;
							}
						}
						else
						{
							SectionIdx = strtoul(Section, &Name, 10);
							if ((Name == Section) || (Name == NULL) || (*Name != ';'))
							{
								Error = 1;
							}
							else
							{
								Name++;
								NameLen = 0;
								for (Type = Name; Type < &Buffer[BufferLen]; Type++)
								{
									if (*Type == ';')
									{
										NameLen = Type - Name;
										break;
									}
								}
								if ((Type == Name) || (*Type != ';'))
								{
									Error = 1;
								}
								else
								{
									Type++;
									Value = Type + 1;
									if (*Value != ';')
									{
										Error = 1;
									}
									else
									{
										Value++;
										if (*Type == '0')
										{
											if (PROFILE_GetParamByName(Profile, SectionIdx, Name, NameLen, &Param))
												Error = 1;
											if ((Error) || (PROFILE_IsConfigParam(Profile,Param)))
												Error = 1;
											if ((Error) || (PROFILE_StringToParamValue(Param->Type, &ParamValue, Value, &Buffer[BufferLen] - Value)))
												Error = 1;
											if ((Error) || (__PROFILE_SetParamValue(Profile, Param, &ParamValue)))
												Error = 1;
										}
										else if (*Type == '1')
										{
											if (PROFILE_GetParamByName(Profile, SectionIdx, Name, NameLen, &Param))
												Error = 1;
											if ((Error) || (!Param->Colors))
												Error = 1;
											else
												Color = strtoul(Value, NULL, 10);
											if ((Error) || (__PROFILE_SetParamColors(Profile, Param, Color, *Param->Colors->Degrade, *Param->Colors->Failure)))
												Error = 1;
										}
										else if (*Type == '2')
										{
											if (PROFILE_GetParamByName(Profile, SectionIdx, Name, NameLen, &Param))
												Error = 1;
											if ((Error) || (!Param->Colors))
												Error = 1;
											else
												Color = strtoul(Value, NULL, 10);
											if ((Error) | (__PROFILE_SetParamColors(Profile, Param, *Param->Colors->Normal, Color, *Param->Colors->Failure)))
												Error = 1;
										}
										else if (*Type == '3')
										{
											if (PROFILE_GetParamByName(Profile, SectionIdx, Name, NameLen, &Param))
												Error = 1;
											if ((Error) || (!Param->Colors))
												Error = 1;
											else
												Color = strtoul(Value, NULL, 10);
											if ((Error) || (__PROFILE_SetParamColors(Profile, Param, *Param->Colors->Normal, *Param->Colors->Degrade, Color)))
												Error = 1;
										}
									}
								}
							}
						}
					}
					else
					{
						Error = 1;
					}
					BufferLen = 0;
				}
				else
				{
					Buffer[BufferLen++] = *p;
					if (BufferLen >= sizeof(Buffer))
						Error = 1;
				}
			}
			netbuf_delete(Inbuf);
		}
		else
		{
			goto error_500;
		}
	}	
	PROFILE_MUTEX_UNLOCK(Profile);
	if (Error)
		goto error_500;
	
	PROFILE_FLASH_SaveConfigs(&ProfileFlash);
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	return;

error_500:
	PROFILE_FLASH_LoadConfigs(&ProfileFlash);
	__HTTP_SERVER_WriteError500(Conn);
	return;
		
error_400:
	__HTTP_SERVER_WriteError400(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	uint32_t section_idx;
	uint32_t i, j, Len;
	PROFILE_VALUE_T Value;
	char ValueStr[sizeof(PROFILE_VALUE_T)];
	uint8_t Color;
	int32_t AdminState;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	c = &ReqHeader->Url[sizeof("/api/v1/profile/sections/") - 1];
	section_idx = strtoul(c, &c, 10);
	if (memcmp(c, "/values", sizeof("/values") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}

	netbuf_delete(Inbuf);
	
	for (i = 0; i < Profile->SectionsCnt; i++)
	{
		if (Profile->Sections[i]->Index == section_idx)
		{
			RspHeader.Code = HTTP_RESPONCE_200_OK;
			RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
			RspHeader.Charset = HTTP_CHARSET_UTF_8;
			RspHeader.ContentLength = 0;
			RspHeader.Connection = HTTP_CONNECTION_CLOSE;
			RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
			RspHeader.AllowedLen = 0;
			if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
				return;
			
			__HTTP_SERVER_StartBody(Conn);
			PROFILE_MUTEX_LOCK(Profile);
			for (j = 0; j < Profile->Sections[i]->ParamsCnt; j++)
			{
				__PROFILE_GetParamValue(Profile, Profile->Sections[i]->Params[j], &Value);
				__PROFILE_GetParamColor(Profile, Profile->Sections[i]->Params[j], &Color);
				__PROFILE_GetParamAdminState(Profile, Profile->Sections[i]->Params[j], &AdminState);
				PROFILE_ParamValueToString(Profile->Sections[i]->Params[j]->Type, &Value, ValueStr, &Len);
				__HTTP_SERVER_WriteBody(Conn, "%s@%d", ValueStr, AdminState);
				if (Color != PROFILE_COLOR_NA)
					__HTTP_SERVER_WriteBody(Conn, "@%u", Color);
				if (j != Profile->Sections[i]->ParamsCnt - 1)
					__HTTP_SERVER_WriteBody(Conn, ";");
			}
			PROFILE_MUTEX_UNLOCK(Profile);
			__HTTP_SERVER_FinishBody(Conn);
			return;
		}
	}
	
	__HTTP_SERVER_WriteError404(Conn);
	return;
error_400:
	__HTTP_SERVER_WriteError400(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	uint32_t section_idx;
	uint32_t i, j;
	uint8_t Normal, Degrade, Failure;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	c = &ReqHeader->Url[sizeof("/api/v1/profile/sections/") - 1];
	section_idx = strtoul(c, &c, 10);
	if (memcmp(c, "/colors", sizeof("/colors") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}

	netbuf_delete(Inbuf);
	
	for (i = 0; i < Profile->SectionsCnt; i++)
	{
		if (Profile->Sections[i]->Index == section_idx)
		{
			RspHeader.Code = HTTP_RESPONCE_200_OK;
			RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
			RspHeader.Charset = HTTP_CHARSET_UTF_8;
			RspHeader.ContentLength = 0;
			RspHeader.Connection = HTTP_CONNECTION_CLOSE;
			RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
			RspHeader.AllowedLen = 0;
			if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
				return;
			
			__HTTP_SERVER_StartBody(Conn);
			PROFILE_MUTEX_LOCK(Profile);
			for (j = 0; j < Profile->Sections[i]->ParamsCnt; j++)
			{
				if (!__PROFILE_GetParamColors(Profile, Profile->Sections[i]->Params[j], &Normal, &Degrade, &Failure))
				{
					__HTTP_SERVER_WriteBody(Conn, "%d@%d@%d", Normal, Degrade, Failure);
				}
				if (j != Profile->Sections[i]->ParamsCnt - 1)
					__HTTP_SERVER_WriteBody(Conn, ";");
			}
			PROFILE_MUTEX_UNLOCK(Profile);
			__HTTP_SERVER_FinishBody(Conn);
			return;
		}
	}

	__HTTP_SERVER_WriteError404(Conn);
	return;
error_400:
	__HTTP_SERVER_WriteError400(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	uint32_t section_idx, param_idx, error, ContentLength;
	err_t recv_err;
	char *data;
	uint16_t datalen;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	PROFILE_PARAM_T *Param;
	PROFILE_VALUE_T Value;

	c = &ReqHeader->Url[sizeof("/api/v1/profile/sections/") - 1];
	section_idx = strtoul(c, &c, 10);
	if (memcmp(c, "/params/", sizeof("/params/") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	c += sizeof("/params/") - 1;
	param_idx = strtoul(c, &c, 10);
	if (memcmp(c, "/value", sizeof("/value") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if (ReqHeader->ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if (ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	error = 0;
	if (ReqHeader->Body)
	{
		data = ReqHeader->Body;
		datalen = ReqHeader->BodyLen;
		goto write_param;
	}
	netbuf_delete(Inbuf);
	
	recv_err = netconn_recv(Conn, &Inbuf);
	if (recv_err == ERR_OK)
	{
		netbuf_data(Inbuf, (void**)&data, &datalen);
write_param:
		if (PROFILE_GetParamByIndex(Profile, section_idx, param_idx, &Param))
			error = 404;
		if ((!error) && (PROFILE_StringToParamValue(Param->Type, &Value, data, datalen)))
			error = 400;
		PROFILE_MUTEX_LOCK(Profile);
		if ((!error) && (__PROFILE_SetParamValue(Profile, Param, &Value)))
		{
			PROFILE_MUTEX_UNLOCK(Profile);
			error = 400;
		}
		PROFILE_MUTEX_UNLOCK(Profile);
		netbuf_delete(Inbuf);
	}
	else
	{
		goto error_400;
	}

	if (!error)
		PROFILE_FLASH_SaveParamValue(&ProfileFlash, Param);
	else if (error == 404)
		goto error_404;
	else
		goto error_400;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	return;

error_400:
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_404:
	__HTTP_SERVER_WriteError404(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	uint32_t section_idx, param_idx, error, ContentLength;
	err_t recv_err;
	char *data;
	uint16_t datalen;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	PROFILE_PARAM_T *Param;
	uint8_t Normal, Degrade, Failure;

	c = &ReqHeader->Url[sizeof("/api/v1/profile/sections/") - 1];
	section_idx = strtoul(c, &c, 10);
	if (memcmp(c, "/params/", sizeof("/params/") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	c += sizeof("/params/") - 1;
	param_idx = strtoul(c, &c, 10);
	if (memcmp(c, "/colors", sizeof("/colors") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if (ReqHeader->ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if (ContentLength == 0)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	error = 0;
	if (ReqHeader->Body)
	{
		data = ReqHeader->Body;
		datalen = ReqHeader->BodyLen;
		goto write_param;
	}
	netbuf_delete(Inbuf);
	
	recv_err = netconn_recv(Conn, &Inbuf);
	if (recv_err == ERR_OK)
	{
		netbuf_data(Inbuf, (void**)&data, &datalen);
write_param:
		if (PROFILE_GetParamByIndex(Profile, section_idx, param_idx, &Param))
			error = 404;
		if (!error)
		{
			Normal = strtoul(data, &data, 10);
			if (*data != ';')
			{
				error = 400;
			}
			else
			{
				data++;
				Degrade = strtoul(data, &data, 10);
				if (*data != ';')
				{
					error = 400;
				}
				else
				{
					data++;
					Failure = strtoul(data, &data, 10);
				}
			}
		}
		PROFILE_MUTEX_LOCK(Profile);
		if ((!error) && (__PROFILE_SetParamColors(Profile, Param, Normal, Degrade, Failure)))
		{
			PROFILE_MUTEX_UNLOCK(Profile);
			error = 400;
		}
		PROFILE_MUTEX_UNLOCK(Profile);
		netbuf_delete(Inbuf);
	}
	else
	{
		goto error_400;
	}
	
	if (!error)
		PROFILE_FLASH_SaveParamColors(&ProfileFlash, Param);
	else if (error == 404)
		goto error_404;
	else
		goto error_400;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	return;

error_400:
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_404:
	__HTTP_SERVER_WriteError404(Conn);
	return;
}
