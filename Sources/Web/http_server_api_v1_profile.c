#include "http_server_api_v1_profile.h"
#include "profile.h"
#include "profile_flash.h"
#include <string.h>

extern const PROFILE_T *Profile;
extern PROFILE_FLASH_T ProfileFlash;

#define HTTP_SERVER_API_V1_PROFILE_URL_TEMPLATE	"/api/v1/profile"
#define HTTP_SERVER_API_V1_PROFILE_LANGPACK_URL_TEMPLATE	"/api/v1/profile/langpack"
#define HTTP_SERVER_API_V1_PROFILE_CONFIG_URL_TEMPLATE	"/api/v1/profile/config"
#define HTTP_SERVER_API_V1_PROFILE_SECTION_VALUES_URL_TEMPLATE	"/api/v1/profile/section/*/values"
#define HTTP_SERVER_API_V1_PROFILE_SECTION_COLORS_URL_TEMPLATE	"/api/v1/profile/section/*/colors"
#define HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_VALUE_URL_TEMPLATE	"/api/v1/profile/section/*/param/*/value"
#define HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_COLORS_URL_TEMPLATE	"/api/v1/profile/section/*/param/*/colors"

static void HTTP_SERVER_API_V1_PROFILE_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_PROFILE_LANGPACK_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_PROFILE_LANGPACK_PutProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_PROFILE_CONFIG_HeadProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_PROFILE_CONFIG_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_PROFILE_CONFIG_PutProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_PROFILE_SECTION_VALUES_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_PROFILE_SECTION_COLORS_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_VALUE_PutProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);
static void HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_COLORS_PutProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_PROFILE_Url = {
	.Url = HTTP_SERVER_API_V1_PROFILE_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_PROFILE_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = HTTP_SERVER_API_V1_PROFILE_GetProcessing,
	.PutProcessing = NULL,
	.PostProcessing = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_PROFILE_LANGPACK_Url = {
	.Url = HTTP_SERVER_API_V1_PROFILE_LANGPACK_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_PROFILE_LANGPACK_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = HTTP_SERVER_API_V1_PROFILE_LANGPACK_GetProcessing,
	.PutProcessing = HTTP_SERVER_API_V1_PROFILE_LANGPACK_PutProcessing,
	.PostProcessing = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_PROFILE_CONFIG_Url = {
	.Url = HTTP_SERVER_API_V1_PROFILE_CONFIG_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_PROFILE_CONFIG_URL_TEMPLATE) - 1,
	.HeadProcessing = HTTP_SERVER_API_V1_PROFILE_CONFIG_HeadProcessing,
	.GetProcessing = HTTP_SERVER_API_V1_PROFILE_CONFIG_GetProcessing,
	.PutProcessing = HTTP_SERVER_API_V1_PROFILE_CONFIG_PutProcessing,
	.PostProcessing = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_PROFILE_SECTION_VALUES_Url = {
	.Url = HTTP_SERVER_API_V1_PROFILE_SECTION_VALUES_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_PROFILE_SECTION_VALUES_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = HTTP_SERVER_API_V1_PROFILE_SECTION_VALUES_GetProcessing,
	.PutProcessing = NULL,
	.PostProcessing = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_PROFILE_SECTION_COLORS_Url = {
	.Url = HTTP_SERVER_API_V1_PROFILE_SECTION_COLORS_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_PROFILE_SECTION_COLORS_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = HTTP_SERVER_API_V1_PROFILE_SECTION_COLORS_GetProcessing,
	.PutProcessing = NULL,
	.PostProcessing = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_VALUE_Url = {
	.Url = HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_VALUE_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_VALUE_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = NULL,
	.PutProcessing = HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_VALUE_PutProcessing,
	.PostProcessing = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_COLORS_Url = {
	.Url = HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_COLORS_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_COLORS_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = NULL,
	.PutProcessing = HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_COLORS_PutProcessing,
	.PostProcessing = NULL,
};

static void HTTP_SERVER_API_V1_PROFILE_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	uint32_t i;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(Conn, &RspHeader))
		return;
	
	http_server_write_body_start(Conn);
	for (i = 0; i < *Profile->LinesCnt; i++)
	{
		http_server_write_body_static(Conn, (char*)Profile->Lines[i]);
	}
	http_server_write_body_finish(Conn);
}

static void HTTP_SERVER_API_V1_PROFILE_LANGPACK_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	uint32_t i;
	char LangpackBlock[PROFILE_FLASH_LANG_BLOCK_SIZE + 1];
	uint32_t LangpackBlocks, LangpackType;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (PROFILE_FLASH_LoadLangpackHeader(&ProfileFlash, &LangpackBlocks, &LangpackType))
		goto error_500;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(Conn, &RspHeader))
		return;
	
	http_server_write_body_start(Conn);
	for (i = 0; i < LangpackBlocks; i++)
	{
		if (PROFILE_FLASH_LoadLangpackData(&ProfileFlash, i * PROFILE_FLASH_LANG_BLOCK_SIZE, LangpackBlock, PROFILE_FLASH_LANG_BLOCK_SIZE))
			return;
		LangpackBlock[PROFILE_FLASH_LANG_BLOCK_SIZE] = 0;
		http_server_write_body(Conn, LangpackBlock);
	}
	http_server_write_body_finish(Conn);
	return;

error_500:
	http_server_write_500_error(Conn);
}

static void HTTP_SERVER_API_V1_PROFILE_LANGPACK_PutProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	struct netbuf *inbuf;
	err_t recv_err;
	char *data;
	uint16_t datalen;
	uint32_t LangpackAddr, LangpackBlocks;
	uint32_t ContentLength;
	HTTP_RESPONCE_HEADER_T RspHeader;

	ContentLength = ReqHeader->ContentLength;
	if ((ContentLength > PROFILE_FLASH_LANG_RECORD_BODY_SIZE) || (ContentLength == 0))
		goto error_400;
	
	LangpackAddr = 0;
	LangpackBlocks = (ContentLength % PROFILE_FLASH_LANG_BLOCK_SIZE) ? ((ContentLength / PROFILE_FLASH_LANG_BLOCK_SIZE) + 1) : (ContentLength / PROFILE_FLASH_LANG_BLOCK_SIZE);
	inbuf = NULL;
	if ((Body) && (BodyLen))
	{
		data = Body;
		datalen = BodyLen;
		goto write_data;
	}
	
	while(ContentLength) {
		recv_err = netconn_recv(Conn, &inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&data, &datalen);
write_data:
			ContentLength -= datalen;
			if (PROFILE_FLASH_SaveLangpackData(&ProfileFlash, LangpackAddr, data, datalen))
			{
				netbuf_delete(inbuf);
				goto error_500;
			}
			LangpackAddr += datalen;
			netbuf_delete(inbuf);
			
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
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	http_server_write_header(Conn, &RspHeader);
	return;
	
error_400:
	http_server_write_400_error(Conn);
	return;
error_500:
	http_server_write_500_error(Conn);
	return;
}

static void HTTP_SERVER_API_V1_PROFILE_CONFIG_HeadProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	char Buffer[256];
	char ValueString[sizeof(PROFILE_VALUE_T)];
	uint32_t ContentLength;
	uint8_t NColor, DColor, FColor;
	uint32_t i, j, Len;
	PROFILE_VALUE_T Value;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
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
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = ContentLength;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(Conn, &RspHeader))
		return;
}

static void HTTP_SERVER_API_V1_PROFILE_CONFIG_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	uint8_t NColor, DColor, FColor;
	uint32_t i, j, Len;
	PROFILE_VALUE_T Value;
	char ValueString[64];
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(Conn, &RspHeader))
		return;
	
	http_server_write_body_start(Conn);	
	http_server_write_body(Conn, "#"BSP_PT_NUMBER";"BSP_HW_VERSION";"BSP_SW_VERSION"\r\n");
	for (i = 0; i < Profile->SectionsCnt; i++)
	{
		for (j = 0; j < Profile->Sections[i]->ParamsCnt; j++)
		{
			if (!PROFILE_IsConfigParam(Profile, Profile->Sections[i]->Params[j]))
			{
				__PROFILE_GetParamValue(Profile, Profile->Sections[i]->Params[j], &Value);
				PROFILE_ParamValueToString(Profile->Sections[i]->Params[j]->Type, &Value, ValueString, &Len);
				http_server_write_body(Conn, "%d;%s;0;%s\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, ValueString);
			}
			if (Profile->Sections[i]->Params[j]->Colors)
			{
				__PROFILE_GetParamColors(Profile, Profile->Sections[i]->Params[j], &NColor, &DColor, &FColor);
				http_server_write_body(Conn, "%d;%s;1;%d\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, NColor);
				http_server_write_body(Conn, "%d;%s;2;%d\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, DColor);
				http_server_write_body(Conn, "%d;%s;3;%d\r\n", Profile->Sections[i]->Params[j]->Section, Profile->Sections[i]->Params[j]->Name, FColor);
			}
		}
	}
	http_server_write_body_finish(Conn);	
}

static void HTTP_SERVER_API_V1_PROFILE_CONFIG_PutProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	struct netbuf *inbuf;
	char *p, *Section, *Name, *Value, *Type, *data;
	char Buffer[256];
	uint32_t BufferLen;
	uint8_t Color;
	uint16_t datalen;
	uint32_t ContentLength, Error, SectionIdx, NameLen;
	PROFILE_PARAM_T *Param;
	PROFILE_VALUE_T ParamValue;
	HTTP_RESPONCE_HEADER_T RspHeader;
	err_t recv_err;
	
	if (ReqHeader->ContentLength == 0)
		goto error_400;
	ContentLength = ReqHeader->ContentLength;
	BufferLen = 0;
	Error = 0;
	PROFILE_MUTEX_LOCK(Profile);
	if ((Body) && (BodyLen))
	{
		inbuf = NULL;
		data = Body;
		datalen = BodyLen;
		goto parse_body;
	}
	while(ContentLength)
	{
		recv_err = netconn_recv(Conn, &inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&data, &datalen);
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
			netbuf_delete(inbuf);
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
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	http_server_write_header(Conn, &RspHeader);
	return;

error_500:
	PROFILE_FLASH_LoadConfigs(&ProfileFlash);
	http_server_write_500_error(Conn);
	return;
		
error_400:
	http_server_write_400_error(Conn);
	return;
}

static void HTTP_SERVER_API_V1_PROFILE_SECTION_VALUES_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	char *c;
	uint32_t section_idx;
	uint32_t i, j, Len;
	PROFILE_VALUE_T Value;
	char ValueStr[sizeof(PROFILE_VALUE_T)];
	uint8_t Color;
	int32_t AdminState;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	c = &ReqHeader->Url[sizeof("/api/v1/profile/sections/") - 1];
	section_idx = strtoul(c, &c, 10);
	if (memcmp(c, "/values", sizeof("/values") - 1))
		goto error_400;

	for (i = 0; i < Profile->SectionsCnt; i++)
	{
		if (Profile->Sections[i]->Index == section_idx)
		{
			RspHeader.Code = HTTP_RESPONCE_200_OK;
			RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
			RspHeader.Charset = HTTP_CHARSET_UTF_8;
			RspHeader.ContentLength = 0;
			if (ReqHeader->ConnectionKeepAlive)
				RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
			else
				RspHeader.Connection = HTTP_CONNECTION_CLOSE;
			RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
			RspHeader.AllowedLen = 0;
			if (http_server_write_header(Conn, &RspHeader))
				return;
			
			http_server_write_body_start(Conn);
			PROFILE_MUTEX_LOCK(Profile);
			for (j = 0; j < Profile->Sections[i]->ParamsCnt; j++)
			{
				__PROFILE_GetParamValue(Profile, Profile->Sections[i]->Params[j], &Value);
				__PROFILE_GetParamColor(Profile, Profile->Sections[i]->Params[j], &Color);
				__PROFILE_GetParamAdminState(Profile, Profile->Sections[i]->Params[j], &AdminState);
				PROFILE_ParamValueToString(Profile->Sections[i]->Params[j]->Type, &Value, ValueStr, &Len);
				http_server_write_body(Conn, "%s@%d", ValueStr, AdminState);
				if (Color != PROFILE_COLOR_NA)
					http_server_write_body(Conn, "@%u", Color);
				if (j != Profile->Sections[i]->ParamsCnt - 1)
					http_server_write_body(Conn, ";");
			}
			PROFILE_MUTEX_UNLOCK(Profile);
			http_server_write_body_finish(Conn);
			return;
		}
	}
	
error_404:
	http_server_write_404_error(Conn);
	return;
error_400:
	http_server_write_400_error(Conn);
	return;
}

static void HTTP_SERVER_API_V1_PROFILE_SECTION_COLORS_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	char *c;
	uint32_t section_idx;
	uint32_t i, j;
	uint8_t Normal, Degrade, Failure;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	c = &ReqHeader->Url[sizeof("/api/v1/profile/sections/") - 1];
	section_idx = strtoul(c, &c, 10);
	if (memcmp(c, "/colors", sizeof("/colors") - 1))
		goto error_400;
	
	for (i = 0; i < Profile->SectionsCnt; i++)
	{
		if (Profile->Sections[i]->Index == section_idx)
		{
			RspHeader.Code = HTTP_RESPONCE_200_OK;
			RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
			RspHeader.Charset = HTTP_CHARSET_UTF_8;
			RspHeader.ContentLength = 0;
			if (ReqHeader->ConnectionKeepAlive)
				RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
			else
				RspHeader.Connection = HTTP_CONNECTION_CLOSE;
			RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
			RspHeader.AllowedLen = 0;
			if (http_server_write_header(Conn, &RspHeader))
				return;
			
			http_server_write_body_start(Conn);
			PROFILE_MUTEX_LOCK(Profile);
			for (j = 0; j < Profile->Sections[i]->ParamsCnt; j++)
			{
				if (!__PROFILE_GetParamColors(Profile, Profile->Sections[i]->Params[j], &Normal, &Degrade, &Failure))
				{
					http_server_write_body(Conn, "%d@%d@%d", Normal, Degrade, Failure);
				}
				if (j != Profile->Sections[i]->ParamsCnt - 1)
					http_server_write_body(Conn, ";");
			}
			PROFILE_MUTEX_UNLOCK(Profile);
			http_server_write_body_finish(Conn);
			return;
		}
	}

error_404:
	http_server_write_404_error(Conn);
	return;
error_400:
	http_server_write_400_error(Conn);
	return;
}

static void HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_VALUE_PutProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	char *c;
	uint32_t section_idx, param_idx, error;
	struct netbuf *inbuf;
	err_t recv_err;
	char *data;
	uint16_t datalen;
	HTTP_RESPONCE_HEADER_T RspHeader;
	PROFILE_PARAM_T *Param;
	PROFILE_VALUE_T Value;

	c = &ReqHeader->Url[sizeof("/api/v1/profile/sections/") - 1];
	section_idx = strtoul(c, &c, 10);
	if (memcmp(c, "/param/", sizeof("/param/") - 1))
		goto error_400;
	c += sizeof("/param/") - 1;
	param_idx = strtoul(c, &c, 10);
	if (memcmp(c, "/value", sizeof("/value") - 1))
		goto error_400;
	
	if (ReqHeader->ContentLength == 0)
		goto error_400;
	
	error = 0;
	if ((Body) && (BodyLen))
	{
		inbuf = NULL;
		data = Body;
		datalen = BodyLen;
		goto write_param;
	}
	
	recv_err = netconn_recv(Conn, &inbuf);
	if (recv_err == ERR_OK)
	{
		netbuf_data(inbuf, (void**)&data, &datalen);
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
		netbuf_delete(inbuf);
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
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	http_server_write_header(Conn, &RspHeader);
	return;

error_400:
	http_server_write_400_error(Conn);
	return;
error_404:
	http_server_write_404_error(Conn);
	return;
}

static void HTTP_SERVER_API_V1_PROFILE_SECTION_PARAM_COLORS_PutProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
	uint32_t section_idx, param_idx, error;
	struct netbuf *inbuf;
	err_t recv_err;
	char *data;
	uint16_t datalen;
	HTTP_RESPONCE_HEADER_T RspHeader;
	PROFILE_PARAM_T *Param;
	uint8_t Normal, Degrade, Failure;

	if (ReqHeader->ContentLength == 0)
		goto error_400;
	
	error = 0;
	if ((Body) && (BodyLen))
	{
		inbuf = NULL;
		data = Body;
		datalen = BodyLen;
		goto write_param;
	}
	recv_err = netconn_recv(Conn, &inbuf);
	if (recv_err == ERR_OK)
	{
		netbuf_data(inbuf, (void**)&data, &datalen);
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
		netbuf_delete(inbuf);
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
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	http_server_write_header(Conn, &RspHeader);
	return;

error_400:
	http_server_write_400_error(Conn);
	return;
error_404:
	http_server_write_404_error(Conn);
	return;
}
