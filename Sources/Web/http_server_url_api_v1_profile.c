#include "http_server_url_api_v1_profile.h"
#include "profile.h"
#include "profile_flash.h"
#include "string_fnc.h"
#include "user.h"
#include "base64.h"
#include "crc.h"
#include "log.h"
#include <string.h>

extern PROFILE_T Profile;
extern BOOTLOADER_T Bootloader;
extern PROFILE_FLASH_T ProfileFlash;

#define HTTP_SERVER_URL_API_V1_PROFILE_TEMPLATE	"/api/v1/profile"
#define HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_TEMPLATE	"/api/v1/profile/langpack"
#define HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_TEMPLATE	"/api/v1/profile/config"
#define HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES_TEMPLATE	"/api/v1/profile/sections/*/values"
#define HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS_TEMPLATE	"/api/v1/profile/sections/*/colors"
#define HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE_TEMPLATE	"/api/v1/profile/sections/*/params/*/value"
#define HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS_TEMPLATE	"/api/v1/profile/sections/*/params/*/colors"

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
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_PROFILE_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_Get,
	.Put = HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_Put,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_CONFIG = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_TEMPLATE) - 1,
	.Head = HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Head,
	.Get = HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Get,
	.Put = HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Put,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE_TEMPLATE) - 1,
	.Head = NULL,
	.Get = NULL,
	.Put = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE_Put,
	.Post = NULL,
	.Delete = NULL,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS = {
	.Template = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS_TEMPLATE) - 1,
	.Head = NULL,
	.Get = NULL,
	.Put = HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS_Put,
	.Post = NULL,
	.Delete = NULL,
};

static void HTTP_SERVER_URL_API_V1_PROFILE_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i;
	char *c;
	uint32_t line;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if ((ReqHeader->Query) && (ReqHeader->QuerySize))
	{
		if (memcmp(ReqHeader->Query, "line=", sizeof("line=") - 1))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		line = strtoul(&ReqHeader->Query[sizeof("line=") - 1], &c, 10);
		if ((c == &ReqHeader->Query[sizeof("line=") - 1]) || (line >= *Profile.Descr[Profile.Idx]->TextSize))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		
		netbuf_delete(Inbuf);
		
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
		RspHeader.Charset = HTTP_CHARSET_WINDOWS_1251;
		RspHeader.ContentLength = 0;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
		
		__HTTP_SERVER_StartBody(Conn);
		__HTTP_SERVER_WriteBodyData(Conn, (char*)Profile.Descr[Profile.Idx]->Text[line], strlen(Profile.Descr[Profile.Idx]->Text[line]));
		__HTTP_SERVER_WriteBody(Conn, "\r\n");
		__HTTP_SERVER_FinishBody(Conn);
	}
	else
	{
		if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
			return;
	
		netbuf_delete(Inbuf);
		
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
		RspHeader.Charset = HTTP_CHARSET_WINDOWS_1251;
		RspHeader.ContentLength = 0;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
		
		__HTTP_SERVER_StartBody(Conn);
		for (i = 0; i < *Profile.Descr[Profile.Idx]->TextSize; i++)
		{
			__HTTP_SERVER_WriteBodyData(Conn, (char*)Profile.Descr[Profile.Idx]->Text[i], strlen(Profile.Descr[Profile.Idx]->Text[i]));
			__HTTP_SERVER_WriteBody(Conn, "\r\n");
		}
		__HTTP_SERVER_FinishBody(Conn);
	}
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ProfileReadError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i;
	char LangpackBlock[PROFILE_FLASH_LANG_BLOCK_SIZE + 1];
	uint32_t LangpackBlocks, LangpackType;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	netbuf_delete(Inbuf);
	
	if (PROFILE_FLASH_ReadLangpackHeader(&ProfileFlash, &LangpackBlocks, &LangpackType))
		goto error_500;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_WINDOWS_1251;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	__HTTP_SERVER_StartBody(Conn);
	for (i = 0; i < LangpackBlocks; i++)
	{
		if (PROFILE_FLASH_ReadLangpackData(&ProfileFlash, i * PROFILE_FLASH_LANG_BLOCK_SIZE, LangpackBlock, PROFILE_FLASH_LANG_BLOCK_SIZE))
			return;
		LangpackBlock[PROFILE_FLASH_LANG_BLOCK_SIZE] = 0;
		__HTTP_SERVER_WriteBody(Conn, LangpackBlock);
	}
	__HTTP_SERVER_FinishBody(Conn);
	return;

error_500:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "LangpackReadError", "Internal error");
	__HTTP_SERVER_WriteError500(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_LANGPACK_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	err_t recv_err;
	char *data;
	uint16_t datalen;
	uint32_t LangpackAddr, LangpackBlocks;
	uint32_t ContentLength;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;

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
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
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
			if (PROFILE_FLASH_UpdateLangpackData(&ProfileFlash, LangpackAddr, data, datalen))
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
	
	if (PROFILE_FLASH_UpdateLangpackHeader(&ProfileFlash, LangpackBlocks, 1))
		goto error_500;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "LangpackUpdate", "");
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "LangpackUpdateError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "LangpackUpdateError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
error_500:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "LangpackUpdateError", "Internal error");
	__HTTP_SERVER_WriteError500(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Head(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t ContentLength;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
	ContentLength = 512;
	if (Profile.Descr[Profile.Idx]->StaticValuesSize % 256)
		ContentLength += 2 * ((Profile.Descr[Profile.Idx]->StaticValuesSize / 256) + 1) * 256;
	else
		ContentLength += 2 * Profile.Descr[Profile.Idx]->StaticValuesSize;
	
	netbuf_delete(Inbuf);
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_WINDOWS_1251;
	RspHeader.ContentLength = ContentLength;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	return;
	
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ConfigReadSizeError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i;
	uint32_t DataSize;
	char HexStr[8];
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
	netbuf_delete(Inbuf);
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_WINDOWS_1251;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	__HTTP_SERVER_StartBody(Conn);
	
	for (i = 0; i < sizeof(BSP_PROJECT); i++)
	{
		ByteToHexStr(HexStr, BSP_PROJECT[i]);
		__HTTP_SERVER_WriteBodyData(Conn, HexStr, 2);
	}
	
	for (i = 0; i < sizeof(BSP_HW_VERSION); i++)
	{
		ByteToHexStr(HexStr, BSP_HW_VERSION[i]);
		__HTTP_SERVER_WriteBodyData(Conn, HexStr, 2);
	}
	
	for (i = 0; i < sizeof(BSP_SW_VERSION); i++)
	{
		ByteToHexStr(HexStr, BSP_SW_VERSION[i]);
		__HTTP_SERVER_WriteBodyData(Conn, HexStr, 2);
	}
	
	osMutexWait( Profile.Values->Mutex, osWaitForever );
	Profile.ValuesCRC16 = Calc_CRC16( Profile.Values->Static, Profile.Descr[Profile.Idx]->StaticValuesSize, 0);
	WordToHexStr(HexStr, Profile.ValuesCRC16);
	__HTTP_SERVER_WriteBodyData(Conn, HexStr, 4);
	
	i = 2 * (sizeof(BSP_PROJECT) + sizeof(BSP_HW_VERSION) + sizeof(BSP_SW_VERSION)) + 4;
	for (; i < 512; i++)
		__HTTP_SERVER_WriteBodyData(Conn, "0", 1);
	
	if (Profile.Descr[Profile.Idx]->StaticValuesSize % 256)
		DataSize = ((Profile.Descr[Profile.Idx]->StaticValuesSize / 256) + 1) * 256;
	else
		DataSize = Profile.Descr[Profile.Idx]->StaticValuesSize;
	for (i = 0; i < DataSize; i++)
	{
		if (i < Profile.Descr[Profile.Idx]->StaticValuesSize)
		{
			ByteToHexStr(HexStr, ((char*)Profile.Values->Static)[i]);
			__HTTP_SERVER_WriteBodyData(Conn, HexStr, 2);
		}
		else
		{
			__HTTP_SERVER_WriteBodyData(Conn, "00", 2);
		}
	}
	osMutexRelease( Profile.Values->Mutex );
	
	__HTTP_SERVER_FinishBody(Conn);
	return;
	
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ConfigReadError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_CONFIG_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *data, *p;
	uint16_t datalen;
	uint32_t ConfigSize;
	uint32_t ContentLength;
	uint32_t DataIdx;
	char Buffer[8];
	uint8_t Byte;
	uint32_t Error;
	uint16_t CalcCrc16, ReadCrc16;

	err_t recv_err;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	if (!ReqHeader->ContentLength)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
	ConfigSize = 512;
	if (Profile.Descr[Profile.Idx]->StaticValuesSize % 256)
		ConfigSize += 2 * ((Profile.Descr[Profile.Idx]->StaticValuesSize / 256) + 1) * 256;
	else
		ConfigSize += 2 * Profile.Descr[Profile.Idx]->StaticValuesSize;
	
	ContentLength = strtoul(ReqHeader->ContentLength, NULL, 10);
	if (ContentLength != ConfigSize)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
	DataIdx = 0;
	Error = 0;
	CalcCrc16 = 0;
	ReadCrc16 = 0;
	
	osMutexWait( Profile.Values->Mutex, osWaitForever );
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
			p = data;
			ContentLength -= datalen;
			
			if (Error)
				continue;
			
			while (p < (data + datalen))
			{
				if (DataIdx < (2 * sizeof(BSP_PROJECT)))
				{
					Buffer[DataIdx % 2] = *p++;
					if ((DataIdx % 2) == 1)
					{
						HexStr2ToByte(Buffer, &Byte);
						if (Byte != BSP_PROJECT[DataIdx / 2])
						{
							Error = 500;
							break;
						}
					}
					DataIdx++;
				}
				else if ((DataIdx >= (2 * sizeof(BSP_PROJECT))) && (DataIdx < (2 * (sizeof(BSP_PROJECT) + sizeof(BSP_HW_VERSION)))))
				{
					Buffer[DataIdx % 2] = *p++;
					if ((DataIdx % 2) == 1)
					{
						HexStr2ToByte(Buffer, &Byte);
						if (Byte != BSP_HW_VERSION[(DataIdx - (2 * (sizeof(BSP_PROJECT)))) / 2])
						{
							Error = 500;
							break;
						}
					}
					DataIdx++;
				}
				else if ((DataIdx >= (2 * (sizeof(BSP_PROJECT) + sizeof(BSP_HW_VERSION)))) && (DataIdx < (2 * (sizeof(BSP_PROJECT) + sizeof(BSP_HW_VERSION) + sizeof(BSP_SW_VERSION)))))
				{
					Buffer[DataIdx % 2] = *p++;
					if ((DataIdx % 2) == 1)
					{
						HexStr2ToByte(Buffer, &Byte);
						if (Byte != BSP_SW_VERSION[(DataIdx - (2 * (sizeof(BSP_PROJECT) + sizeof(BSP_HW_VERSION)))) / 2])
						{
							Error = 500;
							break;
						}
					}
					DataIdx++;
				}
				else if ((DataIdx >= (2 * (sizeof(BSP_PROJECT) + sizeof(BSP_HW_VERSION) + sizeof(BSP_SW_VERSION)))) && (DataIdx < ((2 * (sizeof(BSP_PROJECT) + sizeof(BSP_HW_VERSION) + sizeof(BSP_SW_VERSION))) + 4)))
				{
					Buffer[DataIdx % 4] = *p++;
					if ((DataIdx % 4) == 3)
					{
						HexStr4ToWord(Buffer, &ReadCrc16);
					}
					DataIdx++;
				}
				else if ((DataIdx >= 512) && (DataIdx < (512 + (2 * Profile.Descr[Profile.Idx]->StaticValuesSize))))
				{
					Buffer[DataIdx % 2] = *p++;
					if ((DataIdx % 2) == 1)
					{
						HexStr2ToByte(Buffer, &Byte);
						((char*)Profile.Values->Static)[(DataIdx - 512) / 2] = Byte;
						CalcCrc16 = Calc_CRC16((char*)&Byte, 1, CalcCrc16);
					}
					DataIdx++;
				}
				else
				{
					p++;
					DataIdx++;
				}
			}
			netbuf_delete(Inbuf);
		}
		else
		{
			Error = 500;
		}
	}
	osMutexRelease( Profile.Values->Mutex );
	
	if (Error == 400)
	{
		PROFILE_FLASH_InitConfigs(&ProfileFlash);
		goto error_400;
	}
	
	if (Error == 500)
	{
		PROFILE_FLASH_InitConfigs(&ProfileFlash);
		goto error_500;
	}
	
	if (CalcCrc16 != ReadCrc16)
	{
		PROFILE_FLASH_InitConfigs(&ProfileFlash);
		goto error_500;
	}
	
	PROFILE_FLASH_UpdateConfigs(&ProfileFlash);
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ConfigUpdate", "");
	return;
		
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ConfigUpdateError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ConfigUpdateError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
error_500:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ConfigUpdateError", "Internal error");
	__HTTP_SERVER_WriteError500(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_VALUES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	uint32_t i;
	uint32_t TableIdx;
	PROFILE_TABLE_T *Table;
	char Value[PROFILE_STRING_MAX_LEN];
	char ValueStr[PROFILE_STRING_MAX_LEN + 4];
	uint32_t ValueStrLen;
	uint8_t Color;
	uint8_t State;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	c = &ReqHeader->Url[sizeof("/api/v1/profile/sections/") - 1];
	TableIdx = strtoul(c, &c, 10);
	if (memcmp(c, "/values", sizeof("/values") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if (PROFILE_GetTable(&Profile, TableIdx, &Table))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	netbuf_delete(Inbuf);
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_WINDOWS_1251;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	__HTTP_SERVER_StartBody(Conn);
	for(i = 1 ; i <= Table->ParamsCnt; i++ )
	{
		if (PROFILE_GetState(&Profile, TableIdx, i, &State))
			return;
		if (State != PROFILE_STATE_OOS)
		{
			if (PROFILE_GetValue(&Profile, TableIdx, i, Value))
				return;
			if (PROFILE_ValueToString(&Profile, TableIdx, i, Value, ValueStr , &ValueStrLen))
				return;
			if (TableIdx == PROFILE_TABLE_2)
			{
				if (PROFILE_GetColor(&Profile, TableIdx, i, &Color))
					return;
				if ((State != PROFILE_STATE_IS) && (Color != PROFILE_COLOR_NA))
					Color = PROFILE_COLOR_NO;
				ValueStrLen += sprintf(&ValueStr[ValueStrLen], "@%d", Color);
			}
		}
		else
		{
			ValueStr[0] = 0x01;
			ValueStr[1] = 0x00;
			ValueStrLen = 1;
		}
		__HTTP_SERVER_WriteBody(Conn, ValueStr);
		if (i != Table->ParamsCnt)
			__HTTP_SERVER_WriteBody(Conn, ";");
	}
	__HTTP_SERVER_FinishBody(Conn);
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "SectionValuesReadError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_COLORS_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	uint32_t i;
	uint32_t TableIdx;
	PROFILE_TABLE_T *Table;
	uint8_t ColorDegrade, ColorFailure;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	c = &ReqHeader->Url[sizeof("/api/v1/profile/sections/") - 1];
	TableIdx = strtoul(c, &c, 10);
	if (memcmp(c, "/colors", sizeof("/colors") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if ((TableIdx != PROFILE_TABLE_2) || (PROFILE_GetTable(&Profile, TableIdx, &Table)))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}

	netbuf_delete(Inbuf);
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_WINDOWS_1251;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	__HTTP_SERVER_StartBody(Conn);
	for(i = 1 ; i <= Table->ParamsCnt; i++ )
	{
		if (PROFILE_GetAlarmColors(&Profile, TableIdx, i, &ColorDegrade, &ColorFailure))
			return;
		__HTTP_SERVER_WriteBody(Conn, "%d@%d", ColorDegrade, ColorFailure);
		if (i != Table->ParamsCnt)
			__HTTP_SERVER_WriteBody(Conn, ";");
	}
	__HTTP_SERVER_FinishBody(Conn);
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "SectionColorsReadError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_VALUE_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	uint32_t ContentLength;
	uint32_t TableIdx, ParamIdx;
	PROFILE_TABLE_T *Table;
	PROFILE_TABLE_PARAM_T *Param;
	err_t recv_err;
	char *data;
	uint16_t datalen;
	uint8_t Access;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	char Value[PROFILE_STRING_MAX_LEN];
	char ValueStr[PROFILE_STRING_MAX_LEN];
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;

	c = &ReqHeader->Url[sizeof("/api/v1/profile/sections/") - 1];
	TableIdx = strtoul(c, &c, 10);
	if (memcmp(c, "/params/", sizeof("/params/") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	c += sizeof("/params/") - 1;
	ParamIdx = strtoul(c, &c, 10);
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
	
	if (((TableIdx != PROFILE_TABLE_1) &&
		(TableIdx != PROFILE_TABLE_3) &&
		(TableIdx != PROFILE_TABLE_4) &&
		(TableIdx != PROFILE_TABLE_4) &&
		(TableIdx != PROFILE_TABLE_4) &&
		(TableIdx != PROFILE_TABLE_103) &&
		(TableIdx != PROFILE_TABLE_104)) ||
		(PROFILE_GetTable(&Profile, TableIdx, &Table)))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if (ParamIdx > Table->ParamsCnt)
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if (!strcmp(User.Group, USER_GROUP_MONITOR))
	{
		Access = PROFILE_ACCESS(PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE);
	}
	else if (!strcmp(User.Group, USER_GROUP_USER))
	{
		Access = PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE);
	}
	else if (!strcmp(User.Group, USER_GROUP_ADMIN))
	{
		Access = PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE);
	}
	else if (!strcmp(User.Group, USER_GROUP_SUPERUSER))
	{
		Access = PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE);
	}
	else
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
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
		memcpy(ValueStr, data, datalen);
		ValueStr[datalen] = 0;
		if (PROFILE_GetParam(&Profile, TableIdx, ParamIdx, &Param))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		if (PROFILE_StringToValue(&Profile, TableIdx, ParamIdx, Value, ValueStr))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		if (PROFILE_CheckValue(&Profile, TableIdx, ParamIdx, Value, 0))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		if (PROFILE_CheckWriteAccess(&Profile, TableIdx, ParamIdx, Access))
		{
			netbuf_delete(Inbuf);
			goto error_403;
		}
		if (PROFILE_SetValue(&Profile, TableIdx, ParamIdx, Value))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		PROFILE_SetValueDependencies(&Profile, TableIdx, ParamIdx, Value, 0);
		PROFILE_FLASH_UpdateValue(&ProfileFlash, TableIdx, ParamIdx);
		netbuf_delete(Inbuf);
	}
	else
	{
		goto error_400;
	}
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ParamValueUpdate", "%s;%s",Param->Name, ValueStr);
	return;

error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ParamValueUpdateError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ParamValueUpdateError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_PROFILE_SECTION_PARAM_COLORS_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	uint32_t ContentLength;
	uint32_t TableIdx, ParamIdx;
	PROFILE_TABLE_T *Table;
	PROFILE_TABLE_PARAM_T *Param;
	err_t recv_err;
	char *data;
	uint16_t datalen;
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	char ValueStr[PROFILE_STRING_MAX_LEN];
	uint8_t ColorDegrade, ColorFailure;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;

	c = &ReqHeader->Url[sizeof("/api/v1/profile/sections/") - 1];
	TableIdx = strtoul(c, &c, 10);
	if (memcmp(c, "/params/", sizeof("/params/") - 1))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	c += sizeof("/params/") - 1;
	ParamIdx = strtoul(c, &c, 10);
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
	
	if ((TableIdx != PROFILE_TABLE_2) || (PROFILE_GetTable(&Profile, TableIdx, &Table)))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if ((!ParamIdx) || (ParamIdx > Table->ParamsCnt))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	if ((!strcmp(User.Group, USER_GROUP_ADMIN)) && (!strcmp(User.Group, USER_GROUP_SUPERUSER)) )
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
	if (ReqHeader->Body)
	{
		data = ReqHeader->Body;
		datalen = ReqHeader->BodyLen;
		goto write_param;
	}
	
	recv_err = netconn_recv(Conn, &Inbuf);
	if (recv_err == ERR_OK)
	{
		netbuf_data(Inbuf, (void**)&data, &datalen);
write_param:
		memcpy(ValueStr, data, datalen);
		ValueStr[datalen] = 0;
		ColorDegrade = strtoul(ValueStr, &c, 10);
		if ((c == ValueStr) || (*c != ';'))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		c++;
		ColorFailure = strtoul(c, &c, 10);
		if (*c != 0)
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		if (PROFILE_GetParam(&Profile, TableIdx, ParamIdx, &Param))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		if (PROFILE_SetAlarmColors(&Profile, TableIdx, ParamIdx, ColorDegrade, ColorFailure))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		PROFILE_FLASH_UpdateValue(&ProfileFlash, TableIdx, ParamIdx);
		netbuf_delete(Inbuf);
	}
	else
	{
		goto error_400;
	}
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "ParamColorsUpdate", "%s;%d;%d",Param->Name, ColorDegrade, ColorFailure);
	return;

error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "UserCreateError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "UserCreateError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}
