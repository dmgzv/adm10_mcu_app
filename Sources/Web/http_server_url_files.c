#include "http_server_url_files.h"
#include "http_server.h"
#include "http_server_files.h"
#include <string.h>

static void HTTP_SERVER_URL_FILES_Head(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, uint32_t DataAddr, uint32_t DataSize, uint32_t DataType)
{
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	if (DataType == HTTP_SERVER_FILES_DATA_TYPE_HTML)
	{
		netbuf_delete(Inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_HTML;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = DataSize;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
	}
	else if (DataType == HTTP_SERVER_FILES_DATA_TYPE_JS)
	{
		netbuf_delete(Inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_JS;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = DataSize;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
	}
	else if (DataType == HTTP_SERVER_FILES_DATA_TYPE_CSS)
	{
		netbuf_delete(Inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_CSS;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = DataSize;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
	}
	else if (DataType == HTTP_SERVER_FILES_DATA_TYPE_WOFF2)
	{
		netbuf_delete(Inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_FONT_WOFF2;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = DataSize;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
	}
	else if (DataType == HTTP_SERVER_FILES_DATA_TYPE_JSON)
	{
		netbuf_delete(Inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = DataSize;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
	}
	else
	{
		netbuf_delete(Inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_OCTET;
		RspHeader.Charset = NULL;
		RspHeader.ContentLength = DataSize;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
	}
}

static void HTTP_SERVER_URL_FILES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, uint32_t DataAddr, uint32_t DataSize, uint32_t DataType)
{
	char Buffer[256];
	uint32_t Size;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	if (DataType == HTTP_SERVER_FILES_DATA_TYPE_HTML)
	{
		netbuf_delete(Inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_HTML;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = 0;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
	}
	else if (DataType == HTTP_SERVER_FILES_DATA_TYPE_JS)
	{
		netbuf_delete(Inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_JS;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = 0;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
	}
	else if (DataType == HTTP_SERVER_FILES_DATA_TYPE_CSS)
	{
		netbuf_delete(Inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_CSS;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = 0;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
	}
	else if (DataType == HTTP_SERVER_FILES_DATA_TYPE_WOFF2)
	{
		netbuf_delete(Inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_FONT_WOFF2;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = 0;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
	}
	else if (DataType == HTTP_SERVER_FILES_DATA_TYPE_JSON)
	{
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
	}
	else
	{
		netbuf_delete(Inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_OCTET;
		RspHeader.Charset = NULL;
		RspHeader.ContentLength = 0;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
		RspHeader.AllowedLen = 0;
		if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
			return;
	}
	
	__HTTP_SERVER_StartBody(Conn);
	while(DataSize)
	{
		if (DataSize > sizeof(Buffer))
			Size = sizeof(Buffer);
		else
			Size = DataSize;
		if (HTTP_SERVER_FILES_ReadData(DataAddr, Buffer, Size))
			return;
		if (__HTTP_SERVER_WriteBodyData(Conn, Buffer, Size))
			return;
		DataAddr += Size;
		DataSize -= Size;
	}
	__HTTP_SERVER_FinishBody(Conn);
}

int32_t HTTP_SERVER_URL_FILES_Process(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t DataAddr, DataSize, DataType;
	char *Name;
	uint32_t NameSize;
	char **Allowed;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	Name = ReqHeader->Url + 1;
	NameSize = ReqHeader->UrlSize - 1;
	
	if (HTTP_SERVER_FILES_FindByName(Name, NameSize, &DataAddr, &DataSize, &DataType))
		return -1;
	
	if (!memcmp(ReqHeader->Type, HTTP_REQUEST_HEAD" ", sizeof(HTTP_REQUEST_HEAD)))
	{
		HTTP_SERVER_URL_FILES_Head(Conn, Inbuf, ReqHeader, DataAddr, DataSize, DataType);
	}
	else if (!memcmp(ReqHeader->Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)))
	{
		HTTP_SERVER_URL_FILES_Get(Conn, Inbuf, ReqHeader, DataAddr, DataSize, DataType);
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
		*Allowed++ = HTTP_REQUEST_HEAD;
		*Allowed++ = HTTP_REQUEST_GET;
		RspHeader.AllowedLen = Allowed - RspHeader.Allowed;
		__HTTP_SERVER_WriteHeader(Conn, &RspHeader);
	}
	return 0;	
}
