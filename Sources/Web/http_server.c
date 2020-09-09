#include "http_server.h"
#include "http_server_files.h"
#include "string.h"
#include "cmsis_os.h"
#include "profile.h"
#include "profile_protocol.h"
#include "profile_types.h"
#include "bootloader.h"
#include "bsp.h"
#include "crc.h"
#include "log.h"
#include <stdio.h>
#include "tftp_bootloader.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct {	
	char *Type;
	char *Url;
	char *ContentType;
	char *Charset;
	uint32_t ContentLength;
	char *Connection;
	uint32_t ConnectionKeepAlive;
} HTTP_REQUEST_HEADER_T;

typedef struct {
	char *Code;
	char *ContentType;
	char *Charset;
	uint32_t ContentLength;
	char *Connection;
	char *TransferEncoding;
	char *Allowed[4];
	uint32_t AllowedLen;
} HTTP_RESPONCE_HEADER_T;

/* Private define ------------------------------------------------------------*/
#define HTTP_SERVER_THREAD_STACK_SIZE  ( 512 )
#define HTTP_SERVER_THREAD_PRIO    ( osPriorityAboveNormal )

#define HTTP_REQUEST_HEAD	"HEAD"
#define HTTP_REQUEST_GET	"GET"
#define HTTP_REQUEST_PUT	"PUT"
#define HTTP_REQUEST_POST	"POST"

#define HTTP_RESPONCE_200_OK									"HTTP/1.1 200 OK"
#define HTTP_RESPONCE_400_BAD_REQUEST					"HTTP/1.1 400 Bad Request"
#define HTTP_RESPONCE_404_NOT_FOUND						"HTTP/1.1 404 Not Found"
#define HTTP_RESPONCE_405_METHOD_NOT_ALLOWED	"HTTP/1.1 405 Method Not Allowed"


#define HTTP_CHARSET_UTF_8						"utf-8"
#define HTTP_CHARSET_WINDOWS_1251			"windows-1251"

#define HTTP_CONTENT_TYPE_TEXT_HTML		"text/html"
#define HTTP_CONTENT_TYPE_TEXT_CSS		"text/css"
#define HTTP_CONTENT_TYPE_TEXT_JS			"text/javascript"
#define HTTP_CONTENT_TYPE_TEXT_PLAIN	"text/plain"
#define HTTP_CONTENT_TYPE_APP_JSON		"application/json"
#define HTTP_CONTENT_TYPE_APP_OCTET		"application/octet-stream"
#define HTTP_CONTENT_TYPE_FONT_WOFF2	"font/woff2"

#define HTTP_TRANSFER_ENCODING_CHUNKED	"chunked"

#define HTTP_CONNECTION_CLOSE "close"
#define HTTP_CONNECTION_KEEP_ALIVE "keep-alive"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern PROFILE_T Profile;
extern BOOTLOADER_T Bootloader;
extern PROFILE_PROTOCOL_T ProfileProtocol;
extern PROFILE_FLASH_T ProfileFlash;

static char REQUEST_BODY[PROFILE_PROTOCOL_CMD_MAX_SIZE];
static uint32_t REQUEST_BODY_LEN;
static char RESPONCE_HEADER[512];
static uint32_t RESPONCE_HEADER_LEN;
static char RESPONCE_BODY[PROFILE_PROTOCOL_RSP_MAX_SIZE];
static uint32_t RESPONCE_BODY_LEN;

static int32_t http_server_write(struct netconn *conn, const void *dataptr, size_t size, u8_t apiflags)
{
	err_t write_err;
	while (1)
	{
		write_err = netconn_write(conn, dataptr, size, apiflags);
		if (write_err == ERR_OK)
			break;
		else if (write_err == ERR_INPROGRESS)
			osDelay(1);
		else
			return -1;
	}
	return 0;
}

static int32_t http_server_parse_request(char *data, uint32_t datalen, HTTP_REQUEST_HEADER_T *Header, char **body, uint16_t *bodylen)
{
	char *p;
	
	Header->Type = data;
	
	if (!memcmp(Header->Type, HTTP_REQUEST_HEAD" ", sizeof(HTTP_REQUEST_HEAD)))
		Header->Url = data + sizeof(HTTP_REQUEST_HEAD);
	else if (!memcmp(Header->Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)))
		Header->Url = data + sizeof(HTTP_REQUEST_GET);
	else if (!memcmp(Header->Type, HTTP_REQUEST_PUT" ", sizeof(HTTP_REQUEST_PUT)))
		Header->Url = data + sizeof(HTTP_REQUEST_PUT);
	else if (!memcmp(Header->Type, HTTP_REQUEST_POST" ", sizeof(HTTP_REQUEST_POST)))
		Header->Url = data + sizeof(HTTP_REQUEST_POST);
	else
		return -1;
	
	Header->Connection = strstr(data, "Connection: ");
	if (Header->Connection)
	{
		Header->Connection += sizeof("Connection: ") - 1;
		/*
		if (strncmp(Header->Connection, HTTP_CONNECTION_KEEP_ALIVE, sizeof(HTTP_CONNECTION_KEEP_ALIVE) - 1) == 0)
			Header->ConnectionKeepAlive = 1;
		else
		*/
			Header->ConnectionKeepAlive = 0;
	}
	else
	{
		Header->ConnectionKeepAlive = 0;
	}
	
	Header->ContentType = strstr(data, "Content-Type: ");
	if (Header->ContentType)
		Header->ContentType += sizeof("Content-Type: ") - 1;
	
	Header->Charset = strstr(data, "charset=");
	if (Header->Charset)
		Header->Charset += sizeof("charset=") - 1;
	
	p = strstr(data, "Content-Length: ");
	if (p)
		Header->ContentLength = strtoul(p + sizeof("Content-Length: ") - 1, &p, 10);
	else
		Header->ContentLength = 0;
	
	*body = strstr(data, "\r\n\r\n");
	if (body)
	{
		*body += sizeof("\r\n\r\n") - 1;
		*bodylen = data + datalen - *body;
	}
	else
	{
		bodylen = 0;
	}
	return 0;
}

static void http_server_write_400_error(struct netconn *conn)
{
	if (http_server_write(conn, HTTP_RESPONCE_400_BAD_REQUEST"\r\n\r\n", sizeof(HTTP_RESPONCE_400_BAD_REQUEST"\r\n\r\n") - 1, NETCONN_NOCOPY))
		return;
}
 
static void http_server_write_404_error(struct netconn *conn)
{
	if (http_server_write(conn, HTTP_RESPONCE_404_NOT_FOUND"\r\n\r\n", sizeof(HTTP_RESPONCE_404_NOT_FOUND"\r\n\r\n") - 1, NETCONN_NOCOPY))
		return;
}

static int32_t http_server_write_header(struct netconn *conn, HTTP_RESPONCE_HEADER_T *Header)
{
	uint32_t i;
	
	RESPONCE_HEADER_LEN = 0;
	
	RESPONCE_HEADER_LEN = sprintf(&RESPONCE_HEADER[RESPONCE_HEADER_LEN], "%s\r\n", Header->Code);
	if (Header->Connection)
		RESPONCE_HEADER_LEN += sprintf(&RESPONCE_HEADER[RESPONCE_HEADER_LEN], "Connection: %s\r\n", Header->Connection);
	if (Header->ContentType)
	{
		if (Header->Charset)
			RESPONCE_HEADER_LEN += sprintf(&RESPONCE_HEADER[RESPONCE_HEADER_LEN], "Content-Type: %s; charset=%s\r\n", Header->ContentType, Header->Charset);
		else
			RESPONCE_HEADER_LEN += sprintf(&RESPONCE_HEADER[RESPONCE_HEADER_LEN], "Content-Type: %s\r\n", Header->ContentType);
	}
	if (Header->ContentLength)
		RESPONCE_HEADER_LEN += sprintf(&RESPONCE_HEADER[RESPONCE_HEADER_LEN], "Content-Length: %d\r\n", Header->ContentLength);
	if (Header->TransferEncoding)
		RESPONCE_HEADER_LEN += sprintf(&RESPONCE_HEADER[RESPONCE_HEADER_LEN], "Transfer-Encoding: %s\r\n", Header->TransferEncoding);
	
	if (Header->AllowedLen)
	{
		RESPONCE_HEADER_LEN += sprintf(&RESPONCE_HEADER[RESPONCE_HEADER_LEN], "Allow: ");
		for (i = 0; i < Header->AllowedLen; i++)
		{
			RESPONCE_HEADER_LEN += sprintf(&RESPONCE_HEADER[RESPONCE_HEADER_LEN], "%s", Header->Allowed[i]);
			if (i == Header->AllowedLen - 1)
			{
				memcpy(&RESPONCE_HEADER[RESPONCE_HEADER_LEN], "\r\n", sizeof("\r\n"));
				RESPONCE_HEADER_LEN += sizeof("\r\n") - 1;
			}
			else
			{
				memcpy(&RESPONCE_HEADER[RESPONCE_HEADER_LEN], ", ", sizeof(", "));
				RESPONCE_HEADER_LEN += sizeof(", ") - 1;
			}
		}
	}
	
	memcpy(&RESPONCE_HEADER[RESPONCE_HEADER_LEN], "\r\n", sizeof("\r\n"));
	RESPONCE_HEADER_LEN += sizeof("\r\n") - 1;

	return http_server_write(conn, RESPONCE_HEADER, RESPONCE_HEADER_LEN, NETCONN_COPY);
}

static void http_server_serve_get_file(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{	
	uint32_t data_addr, data_size, data_type;
	char *name, *name_end;
	uint32_t name_size, size;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	name = ReqHeader->Url + 1;
	name_end = strchr(name, ' ');
	name_size = name_end - name;
	
	if (HTTP_SERVER_FILES_FindByName(name, name_size, &data_addr, &data_size, &data_type))
		goto error_404;

	if (strncmp(ReqHeader->Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) != 0)
		goto error_405;	
			
	if (data_type == HTTP_SERVER_FILES_DATA_TYPE_HTML)
	{
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_HTML;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = size;
		if (ReqHeader->ConnectionKeepAlive)
			RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
		else
			RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (http_server_write_header(conn, &RspHeader))
			return;
	}
	else if (data_type == HTTP_SERVER_FILES_DATA_TYPE_JS)
	{
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_JS;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = size;
		if (ReqHeader->ConnectionKeepAlive)
			RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
		else
			RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (http_server_write_header(conn, &RspHeader))
			return;
	}
	else if (data_type == HTTP_SERVER_FILES_DATA_TYPE_CSS)
	{
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_CSS;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = size;
		if (ReqHeader->ConnectionKeepAlive)
			RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
		else
			RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (http_server_write_header(conn, &RspHeader))
			return;
	}
	else if (data_type == HTTP_SERVER_FILES_DATA_TYPE_WOFF2)
	{
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_FONT_WOFF2;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = size;
		if (ReqHeader->ConnectionKeepAlive)
			RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
		else
			RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (http_server_write_header(conn, &RspHeader))
			return;
	}
	else if (data_type == HTTP_SERVER_FILES_DATA_TYPE_JSON)
	{
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = size;
		if (ReqHeader->ConnectionKeepAlive)
			RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
		else
			RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (http_server_write_header(conn, &RspHeader))
			return;
	}
	else
	{
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_OCTET;
		RspHeader.Charset = NULL;
		RspHeader.ContentLength = size;
		if (ReqHeader->ConnectionKeepAlive)
			RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
		else
			RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (http_server_write_header(conn, &RspHeader))
			return;
	}
	
	while(data_size)
	{
		if (data_size > 1024)
			RESPONCE_BODY_LEN = 1024;
		else
			RESPONCE_BODY_LEN = data_size;
		if (HTTP_SERVER_FILES_ReadData(data_addr, RESPONCE_BODY, RESPONCE_BODY_LEN))
			return;
		if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
			return;
		data_addr += RESPONCE_BODY_LEN;
		data_size -= RESPONCE_BODY_LEN;
	}
	return;			

error_404:
	http_server_write_404_error(conn);
	return;
error_405:
	RspHeader.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.Allowed[0] = HTTP_REQUEST_GET;
	RspHeader.AllowedLen = 1;
	http_server_write_header(conn, &RspHeader);
	return;
}

/**
  * @brief  Get profile langpack. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_profile_langpack(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	uint32_t i;
	uint32_t LangpackBlocks, LangpackType;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (PROFILE_FLASH_LoadLangpackHeader(&ProfileFlash, &LangpackBlocks, &LangpackType))
		goto error;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_OCTET;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = LangpackBlocks * PROFILE_FLASH_LANG_BLOCK_SIZE;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(conn, &RspHeader))
		return;
	
	RESPONCE_BODY_LEN = 0;
	
	for (i = 0; i < LangpackBlocks; i++)
	{
		if (PROFILE_FLASH_LoadLangpackData(&ProfileFlash, i * PROFILE_FLASH_LANG_BLOCK_SIZE, &RESPONCE_BODY[RESPONCE_BODY_LEN], PROFILE_FLASH_LANG_BLOCK_SIZE))
			goto error;
		RESPONCE_BODY_LEN += PROFILE_FLASH_LANG_BLOCK_SIZE;
		if (RESPONCE_BODY_LEN >= 1024)
		{
			if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_MORE | NETCONN_COPY))
				return;
			RESPONCE_BODY_LEN = 0;
		}
	}
	if (RESPONCE_BODY_LEN)
	{
		if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_MORE | NETCONN_COPY))
			return;
		RESPONCE_BODY_LEN = 0;
	}
	RESPONCE_BODY_LEN = 0;
	
	return;

error:
	http_server_write_404_error(conn);
}

/**
  * @brief  Put profile langpack. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_put_api_v1_profile_langpack(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
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
		goto error;
	
	LangpackAddr = 0;
	LangpackBlocks = (ContentLength % PROFILE_FLASH_LANG_BLOCK_SIZE) ? ((ContentLength / PROFILE_FLASH_LANG_BLOCK_SIZE) + 1) : (ContentLength / PROFILE_FLASH_LANG_BLOCK_SIZE);
	inbuf = NULL;
	if ((body) && (bodylen))
	{
		data = body;
		datalen = bodylen;
		goto write_data;
	}
	
	while(ContentLength) {
		recv_err = netconn_recv(conn, &inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&data, &datalen);
write_data:
			ContentLength -= datalen;
			if (PROFILE_FLASH_SaveLangpackData(&ProfileFlash, LangpackAddr, data, datalen))
				goto error;
			LangpackAddr += datalen;
			netbuf_delete(inbuf);
		}
		else
		{
			return;
		}
	}
	
	if (PROFILE_FLASH_SaveLangpackHeader(&ProfileFlash, LangpackBlocks, 1))
		goto error;
	
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
	http_server_write_header(conn, &RspHeader);
	return;
	
error:
	http_server_write_400_error(conn);
}

/**
  * @brief  Head profile config. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_head_api_v1_profile_config(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	char *p;
	uint32_t complete;
	uint32_t header_len;
	uint32_t blocks_cnt;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_CONFIG;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%s;", PROFILE_PROTOCOL_CONFIG_STARTREAD);
	header_len = REQUEST_BODY_LEN - 1;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
	
	/*Parse request and prepare responce*/
	RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);
	
	if (memcmp(REQUEST_BODY, RESPONCE_BODY, 2))
		goto error;
	
	if (memcmp("OK;", &RESPONCE_BODY[2], sizeof("OK;") - 1))
		goto error;
	
	if (memcmp(&REQUEST_BODY[2], &RESPONCE_BODY[2 + sizeof("OK;") - 1], header_len - 2))
		goto error;
	
	header_len += sizeof("OK;") - 1;
	
	blocks_cnt = strtoul(&RESPONCE_BODY[header_len + 1], &p, 10);
	
	if (blocks_cnt == 0)
		goto error;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = blocks_cnt * 512;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	http_server_write_header(conn, &RspHeader);
	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_CONFIG;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%s;", PROFILE_PROTOCOL_CONFIG_FINISHREAD);
	header_len = REQUEST_BODY_LEN - 1;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
	
	/*Parse request and prepare responce*/
	RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);	
	return;

error:
	http_server_write_400_error(conn);
}

/**
  * @brief  Get profile config. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_profile_config(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	char *p;
	uint32_t i;
	uint32_t complete;
	uint32_t header_len;
	uint32_t blocks_cnt;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_CONFIG;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%s;", PROFILE_PROTOCOL_CONFIG_STARTREAD);
	header_len = REQUEST_BODY_LEN - 1;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
	
	/*Parse request and prepare responce*/
	RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);
	
	if (memcmp(REQUEST_BODY, RESPONCE_BODY, 2))
		goto error;
	
	if (memcmp("OK;", &RESPONCE_BODY[2], sizeof("OK;") - 1))
		goto error;
	
	if (memcmp(&REQUEST_BODY[2], &RESPONCE_BODY[2 + sizeof("OK;") - 1], header_len - 2))
		goto error;
	
	header_len += sizeof("OK;") - 1;
	
	blocks_cnt = strtoul(&RESPONCE_BODY[header_len + 1], &p, 10);
	
	if (blocks_cnt == 0)
		goto error;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = blocks_cnt * 512;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(conn, &RspHeader))
		return;
	
	for (i = 0; i < blocks_cnt; i++)
	{
		/*Prepare request*/
		REQUEST_BODY[0] = SlotAddress + 0x80;
		REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_CONFIG;
		REQUEST_BODY_LEN = 2;
		REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%s;%d;", PROFILE_PROTOCOL_CONFIG_READBLOCK, i);
		header_len = REQUEST_BODY_LEN - 1;
		REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
	
		/*Parse request and prepare responce*/
		RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);
		
		/*
		if (memcmp(REQUEST_BODY, RESPONCE_BODY, 2))
			goto error;
		
		if (memcmp("OK;", &RESPONCE_BODY[2], sizeof("OK;") - 1))
			goto error;
		
		if (memcmp(&REQUEST_BODY[2], &RESPONCE_BODY[2 + sizeof("OK;") - 1], header_len - 2))
			goto error;
		*/
		
		header_len += sizeof("OK;") - 1;
		
		while (RESPONCE_BODY[RESPONCE_BODY_LEN] != ';')
		{
			RESPONCE_BODY[RESPONCE_BODY_LEN] = 0;
			RESPONCE_BODY_LEN--;
		}
		RESPONCE_BODY[RESPONCE_BODY_LEN] = 0;
		
		body = &RESPONCE_BODY[header_len + 1];
		bodylen = RESPONCE_BODY_LEN - header_len - 1;
		
		if (http_server_write(conn, body, bodylen, NETCONN_COPY | NETCONN_MORE))
			break;
	}
	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_CONFIG;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%s;", PROFILE_PROTOCOL_CONFIG_FINISHREAD);
	header_len = REQUEST_BODY_LEN - 1;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
	
	/*Parse request and prepare responce*/
	RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);	
	return;

error:
	http_server_write_400_error(conn);
}

/**
  * @brief  Put profile config. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_put_api_v1_profile_config(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	struct netbuf *inbuf;
	err_t recv_err;
	char *data;
	uint16_t datalen;
	uint32_t error, datashift, block_idx, header_len, complete;
	uint32_t ContentLength;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	ContentLength = ReqHeader->ContentLength;
	
	if ((ContentLength % 512) || (ContentLength == 0))
		goto error;
	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_CONFIG;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%s;", PROFILE_PROTOCOL_CONFIG_STARTWRITE);
	header_len = REQUEST_BODY_LEN - 1;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
	
	/*Parse request and prepare responce*/
	RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);
	
	if (memcmp(REQUEST_BODY, RESPONCE_BODY, 2))
		goto error;
	
	if (memcmp("OK;", &RESPONCE_BODY[2], sizeof("OK;") - 1))
		goto error;
	
	if (memcmp(&REQUEST_BODY[2], &RESPONCE_BODY[2 + sizeof("OK;") - 1], header_len - 2))
		goto error;
	
	header_len += sizeof("OK;") - 1;
	
	error = 0;
	datashift = 0;
	block_idx = 0;
	inbuf = NULL;
	if ((body) && (bodylen))
	{
		data = body;
		datalen = bodylen;		
		goto write_data;
	}
	
	while(ContentLength) {
		recv_err = netconn_recv(conn, &inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&data, &datalen);
write_data:
			ContentLength -= datalen;
			while((datalen) && (!error))
			{
				if (datashift != 0)
				{
					if ((datashift + datalen) >= 512)
					{						
						/*Continue preparing request*/
						memcpy(&REQUEST_BODY[REQUEST_BODY_LEN  + datashift], data, 512 - datashift);
						REQUEST_BODY_LEN += 512;
						REQUEST_BODY[REQUEST_BODY_LEN] = ';';
						REQUEST_BODY_LEN++;
						REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
					
						/*Parse request and prepare responce*/
						RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);
						
						if (memcmp(REQUEST_BODY, RESPONCE_BODY, 2))
						{
							error = 1;
							break;
						}
						
						if (memcmp("OK;", &RESPONCE_BODY[2], sizeof("OK;") - 1))
						{
							error = 1;
							break;
						}
						
						if (memcmp(&REQUEST_BODY[2], &RESPONCE_BODY[2 + sizeof("OK;") - 1], header_len - 2))
						{
							error = 1;
							break;
						}
						
						header_len += sizeof("OK;") - 1;
						
						block_idx++;
						datalen -= 512 - datashift;
						data += 512 - datashift;
						datashift = 0;
					}
					else
					{
						/*Continue preparing request*/
						memcpy(&REQUEST_BODY[REQUEST_BODY_LEN  + datashift], data, datalen);
						datashift += datalen;
						data += datalen;
						datalen = 0;
					}
				}
				else
				{
					if (datalen >= 512)
					{
						/*Prepare request*/
						REQUEST_BODY[0] = SlotAddress + 0x80;
						REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_CONFIG;
						REQUEST_BODY_LEN = 2;
						REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%s;%d;", PROFILE_PROTOCOL_CONFIG_WRITEBLOCK, block_idx);
						header_len = REQUEST_BODY_LEN - 1;
						memcpy(&REQUEST_BODY[REQUEST_BODY_LEN], data, 512);
						REQUEST_BODY_LEN += 512;
						REQUEST_BODY[REQUEST_BODY_LEN] = ';';
						REQUEST_BODY_LEN++;
						REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
					
						/*Parse request and prepare responce*/
						RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);
						
						if (memcmp(REQUEST_BODY, RESPONCE_BODY, 2))
						{
							error = 1;
							break;
						}
						
						if (memcmp("OK;", &RESPONCE_BODY[2], sizeof("OK;") - 1))
						{
							error = 1;
							break;
						}
						
						if (memcmp(&REQUEST_BODY[2], &RESPONCE_BODY[2 + sizeof("OK;") - 1], header_len - 2))
						{
							error = 1;
							break;
						}
						
						header_len += sizeof("OK;") - 1;
						
						block_idx++;
						data += 512;
						datalen -= 512;						
					}
					else
					{
						/*Start preparing request*/
						REQUEST_BODY[0] = SlotAddress + 0x80;
						REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_CONFIG;
						REQUEST_BODY_LEN = 2;
						REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%s;%d;", PROFILE_PROTOCOL_CONFIG_WRITEBLOCK, block_idx);
						header_len = REQUEST_BODY_LEN - 1;
						memcpy(&REQUEST_BODY[REQUEST_BODY_LEN], data, datalen);
						datashift = datalen;
						data += datalen;
						datalen = 0;
					}
				}
			}
			netbuf_delete(inbuf);
		}
		else
		{
			break;
		}
	}
	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_CONFIG;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%s;", PROFILE_PROTOCOL_CONFIG_FINISHWRITE);
	header_len = REQUEST_BODY_LEN - 1;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
	
	/*Parse request and prepare responce*/
	RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);	
	
	if (memcmp(REQUEST_BODY, RESPONCE_BODY, 2))
		goto error;
	
	if (memcmp("OK;", &RESPONCE_BODY[2], sizeof("OK;") - 1))
		goto error;
	
	if (memcmp(&REQUEST_BODY[2], &RESPONCE_BODY[2 + sizeof("OK;") - 1], header_len - 2))
		goto error;
	
	header_len += sizeof("OK;") - 1;
	
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
	http_server_write_header(conn, &RspHeader);
	return;
	
error:
	http_server_write_400_error(conn);
}

/**
  * @brief  Send profile line. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_profile_line(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen, uint32_t line_idx)
{
	HTTP_RESPONCE_HEADER_T RspHeader;
	if (line_idx >= *Profile.Descr[Profile.Idx]->TextSize)
		goto error;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_WINDOWS_1251;
	RspHeader.ContentLength = strlen(Profile.Descr[Profile.Idx]->Text[line_idx]) + 2;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(conn, &RspHeader))
		return;
	
	RESPONCE_BODY_LEN = strlen(Profile.Descr[Profile.Idx]->Text[line_idx]);
	
	memcpy(RESPONCE_BODY, Profile.Descr[Profile.Idx]->Text[line_idx], RESPONCE_BODY_LEN);
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';

	if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY))
		return;
	RESPONCE_BODY_LEN = 0;
	
	return;
	
error:
	http_server_write_404_error(conn);
}

/**
  * @brief  Send profile. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_profile(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	uint32_t i;
	uint32_t len;
	HTTP_RESPONCE_HEADER_T RspHeader;
  
	len = 0;
	for (i = 0; i < *Profile.Descr[Profile.Idx]->TextSize; i++)
	{
		len += strlen(Profile.Descr[Profile.Idx]->Text[i]) + 2;
	}
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_PLAIN;
	RspHeader.Charset = HTTP_CHARSET_WINDOWS_1251;
	RspHeader.ContentLength = len;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(conn, &RspHeader))
		return;
	
	RESPONCE_BODY_LEN = 0;	
	for (i = 0; i < *Profile.Descr[Profile.Idx]->TextSize; i++)
	{
		len = strlen(Profile.Descr[Profile.Idx]->Text[i]);
		
		memcpy(&RESPONCE_BODY[RESPONCE_BODY_LEN], Profile.Descr[Profile.Idx]->Text[i], len);
		RESPONCE_BODY_LEN += len;
		RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
		RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
		if (RESPONCE_BODY_LEN > 1024)
		{
			if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
				return;
			RESPONCE_BODY_LEN = 0;
			
		}
	}
	if (RESPONCE_BODY_LEN)
	{
		if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
			return;
	}
}

/**
  * @brief  Create and send profile section values. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_profile_section_values(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen, uint32_t section_idx)
{
	uint32_t i;
	uint32_t complete;
	uint32_t header_len;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_GET_VALUES_2;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d;", section_idx);
	header_len = REQUEST_BODY_LEN - 1;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
	
	/*Parse request and prepare responce*/
	RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);
	
	if (RESPONCE_BODY_LEN < REQUEST_BODY_LEN)
		goto error;
	
	for (i = 0; i < header_len; i++)
	{
		if (REQUEST_BODY[i] != RESPONCE_BODY[i])
			goto error;
	}
	
	if ((RESPONCE_BODY[header_len + 1] == 'N') && (RESPONCE_BODY[header_len + 2] == 'A'))
		goto error;
	
	while (RESPONCE_BODY[RESPONCE_BODY_LEN] != ';')
	{
		RESPONCE_BODY[RESPONCE_BODY_LEN] = 0;
		RESPONCE_BODY_LEN--;
	}
	RESPONCE_BODY[RESPONCE_BODY_LEN] = 0;
	
	body = &RESPONCE_BODY[header_len + 1];
	bodylen = RESPONCE_BODY_LEN - header_len - 1;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_OCTET;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = bodylen;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(conn, &RspHeader))
		return;
	if (http_server_write(conn, body, bodylen, NETCONN_COPY))
		return;
	
	return;

error:
	http_server_write_400_error(conn);
}

/**
  * @brief  Create and send profile section values. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_profile_section_colors(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen, uint32_t section_idx)
{
	uint32_t i;
	uint32_t complete;
	uint32_t header_len;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_GET_COLORS;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d;", section_idx);
	header_len = REQUEST_BODY_LEN - 1;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
	
	/*Parse request and prepare responce*/
	RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);
	
	if (RESPONCE_BODY_LEN < REQUEST_BODY_LEN)
		goto error;
	
	for (i = 0; i < header_len; i++)
	{
		if (REQUEST_BODY[i] != RESPONCE_BODY[i])
			goto error;
	}
	
	if ((RESPONCE_BODY[header_len + 1] == 'N') && (RESPONCE_BODY[header_len + 2] == 'A'))
		goto error;
	
	while (RESPONCE_BODY[RESPONCE_BODY_LEN] != ';')
	{
		RESPONCE_BODY[RESPONCE_BODY_LEN] = 0;
		RESPONCE_BODY_LEN--;
	}
	RESPONCE_BODY[RESPONCE_BODY_LEN] = 0;
	
	body = &RESPONCE_BODY[header_len + 1];
	bodylen = RESPONCE_BODY_LEN - header_len - 1;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_OCTET;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = bodylen;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(conn, &RspHeader))
		return;
	if (http_server_write(conn, body, bodylen, NETCONN_COPY))
		return;
	
	return;

error:
	http_server_write_400_error(conn);
}

/**
  * @brief  Set profile section parameter value. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
static void http_server_serve_put_api_v1_profile_section_param_value(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen, uint32_t section_idx, uint32_t param_idx)
{
	uint32_t i;
	uint32_t complete;
	uint32_t header_len;
	HTTP_RESPONCE_HEADER_T RspHeader;

	if ((!body) || (!bodylen))
	{
		http_server_write_400_error(conn);
		return;
	}	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_SET_VALUE_2;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d;%d;", section_idx, param_idx);
	header_len = REQUEST_BODY_LEN - 1;
	memcpy(&REQUEST_BODY[REQUEST_BODY_LEN], body, bodylen);
	REQUEST_BODY_LEN += bodylen;
	REQUEST_BODY[REQUEST_BODY_LEN++] = ';';
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
	
	/*Parse request and prepare responce*/
	Profile.SetEnable = 1;
	RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);
	Profile.SetEnable = 0;
	
	if (RESPONCE_BODY_LEN < REQUEST_BODY_LEN)
		goto error;
	
	for (i = 0; i < header_len; i++)
	{
		if (REQUEST_BODY[i] != RESPONCE_BODY[i])
			goto error;
	}
	
	if ((RESPONCE_BODY[header_len + 1] == 'N') && (RESPONCE_BODY[header_len + 2] == 'A'))
		goto error;
	
	for (i = 0; i < bodylen; i++)
	{
		if (REQUEST_BODY[header_len + 1 + i] != RESPONCE_BODY[header_len + 1 + i])
			goto error;
	}
		
	while (RESPONCE_BODY[RESPONCE_BODY_LEN] != ';')
	{
		RESPONCE_BODY[RESPONCE_BODY_LEN] = 0;
		RESPONCE_BODY_LEN--;
	}
	RESPONCE_BODY[RESPONCE_BODY_LEN] = 0;
	
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
	http_server_write_header(conn, &RspHeader);
	return;

error:
	http_server_write_400_error(conn);
}

/**
  * @brief  Set profile section parameter colors. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
static void http_server_serve_put_api_v1_profile_section_param_color(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen, uint32_t section_idx, uint32_t param_idx)
{
	uint32_t i;
	uint32_t complete;
	uint32_t header_len;
	HTTP_RESPONCE_HEADER_T RspHeader;

	if ((!body) || (!bodylen))
	{
		http_server_write_400_error(conn);
		return;
	}
	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_SET_COLOR;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d;%d;", section_idx, param_idx);
	header_len = REQUEST_BODY_LEN - 1;
	memcpy(&REQUEST_BODY[REQUEST_BODY_LEN], body, bodylen);
	REQUEST_BODY_LEN += bodylen;
	REQUEST_BODY[REQUEST_BODY_LEN++] = ';';
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d", Calc_CRC16_ASCII(REQUEST_BODY, REQUEST_BODY_LEN));
	
	/*Parse request and prepare responce*/
	Profile.SetEnable = 1;
	RESPONCE_BODY_LEN = PROFILE_PROTOCOL_Command(&ProfileProtocol, REQUEST_BODY, RESPONCE_BODY, &complete);
	Profile.SetEnable = 0;
	
	if (RESPONCE_BODY_LEN < REQUEST_BODY_LEN)
		goto error;
	
	for (i = 0; i < header_len; i++)
	{
		if (REQUEST_BODY[i] != RESPONCE_BODY[i])
			goto error;
	}
	
	if ((RESPONCE_BODY[header_len + 1] == 'N') && (RESPONCE_BODY[header_len + 2] == 'A'))
		goto error;
	
	while (RESPONCE_BODY[RESPONCE_BODY_LEN] != ';')
	{
		RESPONCE_BODY[RESPONCE_BODY_LEN] = 0;
		RESPONCE_BODY_LEN--;
	}
	RESPONCE_BODY[RESPONCE_BODY_LEN] = 0;
	
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
	http_server_write_header(conn, &RspHeader);	
	return;

error:
	http_server_write_400_error(conn);
}

/**
  * @brief  Create and send system data. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_system_os(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	char *thread = &RESPONCE_BODY[sizeof(RESPONCE_BODY) - 512];
	char *p, *s;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	RESPONCE_BODY_LEN = snprintf (RESPONCE_BODY, sizeof(RESPONCE_BODY), "{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN,	"\t\"heap\" : \"%d\",\r\n", xPortGetFreeHeapSize());
  /* The list of tasks and their status */
	osThreadList((unsigned char *)(thread));
  RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"threads\" : [\r\n");
	while (*thread) {
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
		s = thread;
		p = strchr(s, '\t');
		*p++ = 0;
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"%s\",\r\n", s);
		s = p;
		p = strchr(s, '\t');
		*p++ = 0;
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"state\" : \"%s\",\r\n", s);
		s = p;
		p = strchr(s, '\t');
		*p = 0;
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"priority\" : \"%s\",\r\n", s);
		s = p + 1;
		p = strchr(s, '\t');
		*p++ = 0;
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"stack\" : \"%s\",\r\n", s);
		s = p;
		p = strchr(s, '\r');
		*p++ = 0;
		*p++ = 0;
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"number\" : \"%s\"\r\n", s);
		thread = p;
		if (*thread)
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
		else
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");	
	}
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t]\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "}\r\n");	

	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = RESPONCE_BODY_LEN;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(conn, &RspHeader))
		return;
	
	if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY))
		return;
}

/**
  * @brief  Create and send system data. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_system_hardware(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	uint32_t i;
	char chunk_size[16];
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(conn, &RspHeader))
		return;
	
	RESPONCE_BODY_LEN = snprintf (RESPONCE_BODY, sizeof(RESPONCE_BODY), "{\r\n\t\"power\" : [\r\n");
	for (i = 0; i < UCD90120A_RAILS_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK)
			break;
		if (i != (UCD90120A_RAILS_CNT - 1))
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n\t\t\t\"config\" : \"%.3f\",\r\n\t\t\t\"monitor\" : \"%.3f\"\r\n\t\t},\r\n", ucd90120a.Cfg.Rail[i].VoutCmd, ucd90120a.Mon.Rail[i].Vout);
		else
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n\t\t\t\"config\" : \"%.3f\",\r\n\t\t\t\"monitor\" : \"%.3f\"\r\n\t\t}\r\n", ucd90120a.Cfg.Rail[i].VoutCmd, ucd90120a.Mon.Rail[i].Vout);
		if (RESPONCE_BODY_LEN > 1024)
		{
			snprintf (chunk_size, sizeof(chunk_size), "%X\r\n", RESPONCE_BODY_LEN);
			if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY | NETCONN_MORE))
				return;
			RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
			RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
			if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
				return;
			RESPONCE_BODY_LEN = 0;
		}
	}
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t],\r\n");	
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"devices\" : [\r\n");
	for (i = 0; i < (DevicesCnt + ModulesCnt); i++)
	{
		if (i < DevicesCnt)
		{
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"%s\",\r\n", Devices[i]->DevName);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"id\" : \"%s\",\r\n", Devices[i]->DevId);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"status\" : \"%s\",\r\n", BSP_DEV_GET_STATE_STR(Devices[i]));
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"init_cnt\" : \"%d\",\r\n", Devices[i]->InitCnt);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"init_err\" : \"%d\",\r\n", Devices[i]->InitErr);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"init_cont_err\" : \"%d\",\r\n", Devices[i]->InitContErr);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"init_cont_err_thr\" : \"%d\",\r\n", Devices[i]->InitContErrThr);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"update_cnt\" : \"%d\",\r\n", Devices[i]->UpdateCnt);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"update_err\" : \"%d\",\r\n", Devices[i]->UpdateErr);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"update_cont_err\" : \"%d\",\r\n", Devices[i]->UpdateContErr);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"update_cont_err_thr\" : \"%d\"\r\n", Devices[i]->UpdateContErrThr);
		}
		else
		{
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"%s\",\r\n", Modules[i - DevicesCnt]->DevName);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"id\" : \"%s\",\r\n", Modules[i - DevicesCnt]->DevId);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"status\" : \"%s\",\r\n", BSP_DEV_GET_STATE_STR(Modules[i - DevicesCnt]));
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"init_cnt\" : \"%d\",\r\n", Modules[i - DevicesCnt]->InitCnt);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"init_err\" : \"%d\",\r\n", Modules[i - DevicesCnt]->InitErr);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"init_cont_err\" : \"%d\",\r\n", Modules[i - DevicesCnt]->InitContErr);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"init_cont_err_thr\" : \"%d\",\r\n", Modules[i - DevicesCnt]->InitContErrThr);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"update_cnt\" : \"%d\",\r\n", Modules[i - DevicesCnt]->UpdateCnt);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"update_err\" : \"%d\",\r\n", Modules[i - DevicesCnt]->UpdateErr);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"update_cont_err\" : \"%d\",\r\n", Modules[i - DevicesCnt]->UpdateContErr);
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"update_cont_err_thr\" : \"%d\"\r\n", Modules[i - DevicesCnt]->UpdateContErrThr);
		}
		if (i == (DevicesCnt + ModulesCnt - 1))
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
		else
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
		if (RESPONCE_BODY_LEN > 1024)
		{
			snprintf (chunk_size, sizeof(chunk_size), "%X\r\n", RESPONCE_BODY_LEN);
			if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY | NETCONN_MORE))
				return;
			RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
			RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
			if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
				return;
			RESPONCE_BODY_LEN = 0;
		}
	}	
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t]\r\n");	
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "}\r\n");
	
	snprintf (chunk_size, sizeof(chunk_size), "%X\r\n", RESPONCE_BODY_LEN);
	if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY | NETCONN_MORE))
		return;	
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
	if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
		return;
	RESPONCE_BODY_LEN = 0;
	
	strncpy(chunk_size, "0\r\n\r\n", sizeof(chunk_size));
	if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY))
		return;
}

/**
  * @brief  Create and send system data. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_system_interfaces(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	char chunk_size[16];
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(conn, &RspHeader))
		return;
	
	RESPONCE_BODY_LEN = snprintf (RESPONCE_BODY, sizeof(RESPONCE_BODY), "{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"i2c\" : [\r\n");
#ifdef BSP_CONFIG_USE_I2C1
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"I2C1\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c1.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"berr\" : \"%d\",\r\n", i2c1.berr_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c1.arlo_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"af\" : \"%d\",\r\n", i2c1.af_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", i2c1.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c1.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c1.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c1.seq_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c1.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c1.seq_tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c1.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_I2C2) || defined(BSP_CONFIG_USE_I2C3))
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
#else
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_I2C2
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"I2C2\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c2.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"berr\" : \"%d\",\r\n", i2c2.berr_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c2.arlo_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"af\" : \"%d\",\r\n", i2c2.af_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", i2c2.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c2.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c2.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c2.seq_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c2.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c2.seq_tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c2.message_queue_overflow_errors);
#if defined(BSP_CONFIG_USE_I2C3)
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
#else
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_I2C3
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"I2C3\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c3.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"berr\" : \"%d\",\r\n", i2c3.berr_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c3.arlo_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"af\" : \"%d\",\r\n", i2c3.af_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", i2c3.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c3.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c3.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c3.seq_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c3.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c3.seq_tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c3.message_queue_overflow_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t],\r\n");
	
	snprintf (chunk_size, sizeof(chunk_size), "%X\r\n", RESPONCE_BODY_LEN);
	if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY | NETCONN_MORE))
		return;
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
	if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
		return;
	RESPONCE_BODY_LEN = 0;
	
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"spi\" : [\r\n");
#ifdef BSP_CONFIG_USE_SPI1
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"SPI1\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", spi1.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi1.mode_fault_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"crc\" : \"%d\",\r\n", spi1.crc_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"frame\" : \"%d\",\r\n", spi1.frame_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", spi1.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", spi1.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi1.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi1.seq_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi1.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi1.seq_tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi1.tx_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi1.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
#else
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI2
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"SPI2\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", spi2.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi2.mode_fault_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"crc\" : \"%d\",\r\n", spi2.crc_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"frame\" : \"%d\",\r\n", spi2.frame_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", spi2.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", spi2.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi2.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi2.seq_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi2.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi2.seq_tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi2.tx_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi2.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
#else
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI3
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"SPI3\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", spi3.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi3.mode_fault_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"crc\" : \"%d\",\r\n", spi3.crc_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"frame\" : \"%d\",\r\n", spi3.frame_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", spi3.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", spi3.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi3.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi3.seq_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi3.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi3.seq_tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi3.tx_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi3.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
#else
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI4
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"SPI4\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", spi4.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi4.mode_fault_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"crc\" : \"%d\",\r\n", spi4.crc_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"frame\" : \"%d\",\r\n", spi4.frame_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", spi4.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", spi4.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi4.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi4.seq_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi4.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi4.seq_tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi4.tx_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi4.message_queue_overflow_errors);
#if defined(BSP_CONFIG_USE_SPI5)
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
#else
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI5
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"SPI5\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", spi5.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi5.mode_fault_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"crc\" : \"%d\",\r\n", spi5.crc_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"frame\" : \"%d\",\r\n", spi5.frame_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", spi5.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", spi5.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi5.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi5.seq_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi5.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi5.seq_tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi5.tx_rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi5.message_queue_overflow_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t],\r\n");
	
	snprintf (chunk_size, sizeof(chunk_size), "%X\r\n", RESPONCE_BODY_LEN);
	if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY | NETCONN_MORE))
		return;
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
	if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
		return;
	RESPONCE_BODY_LEN = 0;
	
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"uart\" : [\r\n");
#ifdef BSP_CONFIG_USE_UART1
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"UART1\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", uart1.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"parity\" : \"%d\",\r\n", uart1.parity_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"noise\" : \"%d\",\r\n", uart1.noise_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"frame\" : \"%d\",\r\n", uart1.frame_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", uart1.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", uart1.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart1.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart1.rx_cmd_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart1.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart1.rx_message_queue_overflow_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart1.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART2) || defined(BSP_CONFIG_USE_UART3) || defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
#else
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART2
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"UART2\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", uart2.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"parity\" : \"%d\",\r\n", uart2.parity_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"noise\" : \"%d\",\r\n", uart2.noise_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"frame\" : \"%d\",\r\n", uart2.frame_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", uart2.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", uart2.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart2.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart2.rx_cmd_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart2.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart2.rx_message_queue_overflow_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart2.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART3) || defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
#else
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART3
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"UART3\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", uart3.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"parity\" : \"%d\",\r\n", uart3.parity_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"noise\" : \"%d\",\r\n", uart3.noise_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"frame\" : \"%d\",\r\n", uart3.frame_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", uart3.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", uart3.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart3.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart3.rx_cmd_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart3.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart3.rx_message_queue_overflow_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart3.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
#else
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART4
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"UART4\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", uart4.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"parity\" : \"%d\",\r\n", uart4.parity_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"noise\" : \"%d\",\r\n", uart4.noise_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"frame\" : \"%d\",\r\n", uart4.frame_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", uart4.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", uart4.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart4.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart4.rx_cmd_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart4.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart4.rx_message_queue_overflow_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart4.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
#else
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART5
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"UART5\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", uart5.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"parity\" : \"%d\",\r\n", uart5.parity_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"noise\" : \"%d\",\r\n", uart5.noise_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"frame\" : \"%d\",\r\n", uart5.frame_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", uart5.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", uart5.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart5.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart5.rx_cmd_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart5.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart5.rx_message_queue_overflow_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart5.tx_message_queue_overflow_errors);
#if defined(BSP_CONFIG_USE_UART6)
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
#else
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART6
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"UART6\",\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"overrun\" : \"%d\",\r\n", uart6.overrun_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"parity\" : \"%d\",\r\n", uart6.parity_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"noise\" : \"%d\",\r\n", uart6.noise_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"frame\" : \"%d\",\r\n", uart6.frame_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"dma\" : \"%d\",\r\n", uart6.dma_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"timeout\" : \"%d\",\r\n", uart6.timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart6.rx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart6.rx_cmd_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart6.tx_timeout_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart6.rx_message_queue_overflow_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart6.tx_message_queue_overflow_errors);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
#endif
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t]\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "}\r\n");
	
	snprintf (chunk_size, sizeof(chunk_size), "%X\r\n", RESPONCE_BODY_LEN);
	if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY | NETCONN_MORE))
		return;	
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
	if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
		return;
	RESPONCE_BODY_LEN = 0;
	
	strncpy(chunk_size, "0\r\n\r\n", sizeof(chunk_size));
	if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY))
		return;
}

/**
  * @brief  Get log data.  
  * @param  conn pointer on connection structure 
  * @retval None
  */
static void http_server_serve_get_api_v1_log(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	uint32_t i, LinesCnt;
	LOG_LINE_T LogLine;
	float Time;
	char chunk_size[16];
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(conn, &RspHeader))
		return;
	
	LinesCnt = LOG_GetLinesCnt();
	RESPONCE_BODY_LEN += snprintf (RESPONCE_BODY, sizeof(RESPONCE_BODY), "{\r\n\t\"max-records-cnt\" : \"%d\",\r\n\t\"records-cnt\" : \"%d\",\r\n\t\"records\" : [\r\n", LOG_LINES_CNT, LinesCnt);
	for (i = 0; i < LinesCnt; i++)
	{
		LOG_GetLine(i, &LogLine);
		Time = (float)LogLine.Time / osKernelSysTickFrequency;
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"time\" : \"%.4f\",\r\n", Time);
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"severity\" : \"%d\",\r\n", LogLine.Severity);
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"category\" : \"%s\",\r\n", LogLine.Category);
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"object\" : \"%s\",\r\n", LogLine.Object);
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"object_id\" : \"%s\",\r\n", LogLine.ObjectId);
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"description\" : \"%s\"\r\n", LogLine.Description);		
		if (i == (LinesCnt - 1))
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");
		else
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
		if (RESPONCE_BODY_LEN > 1024)
		{
			snprintf (chunk_size, sizeof(chunk_size), "%X\r\n", RESPONCE_BODY_LEN);
			if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY | NETCONN_MORE))
				return;
			
			RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
			RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
			if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
				return;
			RESPONCE_BODY_LEN = 0;
			
		}
	}
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t]\r\n}\r\n");
	snprintf (chunk_size, sizeof(chunk_size), "%X\r\n", RESPONCE_BODY_LEN);
	if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY | NETCONN_MORE))
		return;
	
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
	RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
	if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
		return;
	
	strncpy(chunk_size, "0\r\n\r\n", sizeof(chunk_size));
	if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY))
		return;
}

/**
  * @brief  Post MCU Software.  
  * @param  conn pointer on connection structure 
  * @retval None
  */
static void http_server_serve_post_api_v1_system_update_mcu_software(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	struct netbuf *inbuf;
	err_t recv_err;
	char *data;
	uint16_t error, datalen;
	uint32_t ContentLength;
	HTTP_RESPONCE_HEADER_T RspHeader;

	ContentLength = ReqHeader->ContentLength;
	
	if ((ContentLength == 0) || (ContentLength > BSP_FLASH_FW_SIZE))
		goto error;
	
	BOOTLOADER_UpdateNewFwStart(&Bootloader);
	
	inbuf = NULL;
	error = 0;
	if ((body) && (bodylen))
	{
		data = body;
		datalen = bodylen;		
		goto write_data;
	}
	
	while(ContentLength) {
		recv_err = netconn_recv(conn, &inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&data, &datalen);
write_data:
			ContentLength -= datalen;
			if (!error)
			{
				if (BOOTLOADER_UpdateNewFwBlock(&Bootloader, data, datalen))
					error = 1;
			}			
			netbuf_delete(inbuf);
		}
		else
		{
			return;
		}
	}
	if (error)
		goto error;
	
	if(BOOTLOADER_UpdateNewFwFinish(&Bootloader))
		goto error;
	if(BOOTLOADER_CheckNewFw(&Bootloader))
		goto error;
	if(BOOTLOADER_UpdateNewFw(&Bootloader))
		goto error;
	
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
	http_server_write_header(conn, &RspHeader);
	netconn_close(conn);
	netconn_delete(conn);
	osDelay(1000);
	BSP_Reset();
	return;
	
error:
	http_server_write_400_error(conn);
}

/**
  * @brief  Post MCU Web.  
  * @param  conn pointer on connection structure 
  * @retval None
  */
static void http_server_serve_post_api_v1_system_update_mcu_web(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	struct netbuf *inbuf;
	err_t recv_err;
	char *data;
	uint16_t error, datalen;
	uint32_t dataaddr, ContentLength;
	HTTP_RESPONCE_HEADER_T RspHeader;

	ContentLength = ReqHeader->ContentLength;
	
	if ((ContentLength == 0) || (ContentLength > HTTP_SERVER_FILES_RECORD_BODY_SIZE))
		goto error;
	
	dataaddr = 0;
	inbuf = NULL;
	error = 0;
	if ((body) && (bodylen))
	{
		data = body;
		datalen = bodylen;		
		goto write_data;
	}
	
	while(ContentLength) {
		recv_err = netconn_recv(conn, &inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&data, &datalen);
write_data:
			ContentLength -= datalen;
			if (!error)
			{
				if (HTTP_SERVER_FILES_WriteData(dataaddr, data, datalen))
					error = 1;
			}
			dataaddr += datalen;
			netbuf_delete(inbuf);
		}
		else
		{
			return;
		}
	}
	if (error)
		goto error;
	
	if (HTTP_SERVER_FILES_WriteDataComplete())
		goto error;
	
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
	http_server_write_header(conn, &RspHeader);	
	return;
	
error:
	http_server_write_400_error(conn);
}

/**
  * @brief  Get System Update.  
  * @param  conn pointer on connection structure 
  * @retval None
  */

static void http_server_serve_get_api_v1_system_update_add_file_to_response(TFTP_BOOTLOADER_FILE_T *File)
{
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"%s\",\r\n", File->Name);
	switch (File->Status)
	{
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE:
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_COMPLETE:
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"status\" : \"Idle\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_TIMEOUT:
		case TFTP_BOOTLOADER_FILE_STATUS_IDLE_ERROR:
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"status\" : \"Error\",\r\n");
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"error\" : \"Internal error\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD:
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_COMPLETE:
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"status\" : \"Download\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT:
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"status\" : \"Error\",\r\n");
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"error\" : \"Download timeout\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_ERROR:
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"status\" : \"Error\",\r\n");
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"error\" : \"Download error\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY:
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"status\" : \"Verify\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_COMPLETE:
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"status\" : \"Complete\",\r\n");
			break;
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_TIMEOUT:
		case TFTP_BOOTLOADER_FILE_STATUS_VERIFY_ERROR:
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"status\" : \"Error\",\r\n");
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"error\" : \"Verify error\",\r\n");
			break;
		default:
			RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"status\" : \"Unknown\",\r\n");
			break;
	}
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"bytes\" : \"%d\"\r\n", File->Size);
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}");
}

static void http_server_serve_get_api_v1_system_update(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	HTTP_RESPONCE_HEADER_T RspHeader;
	uint32_t i, j;
	
	RESPONCE_BODY_LEN = 0;
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "{\r\n");
	if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_IDLE)
	{
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"status\" : \"Idle\"\r\n");
	}
	else if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_IN_PROGRESS)
	{
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"status\" : \"Download\",\r\n");
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"files\" : [\r\n");
		http_server_serve_get_api_v1_system_update_add_file_to_response(&TFTP_BOOTLOADER.Info);
		for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
		{
			
			for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
			{
				if (TFTP_BOOTLOADER.Components[i]->Files[j].Name[0] != 0)
				{
					RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, ",\r\n");
					http_server_serve_get_api_v1_system_update_add_file_to_response(&TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\r\n");
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t]\r\n");
	}
	else if (TFTP_BOOTLOADER.Status == TFTP_BOOTLOADER_STATUS_COMPLETE)
	{
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"status\" : \"Complete\",\r\n");
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"files\" : [\r\n");
		http_server_serve_get_api_v1_system_update_add_file_to_response(&TFTP_BOOTLOADER.Info);
		for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
		{
			for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
			{
				if (TFTP_BOOTLOADER.Components[i]->Files[j].Name[0] != 0)
				{
					RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, ",\r\n");
					http_server_serve_get_api_v1_system_update_add_file_to_response(&TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\r\n");
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t]\r\n");
	}
	else
	{
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"status\" : \"Error\",\r\n");
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"files\" : [\r\n");
		http_server_serve_get_api_v1_system_update_add_file_to_response(&TFTP_BOOTLOADER.Info);
		for (i = 0; i < TFTP_BOOTLOADER.ComponentsCnt; i++)
		{
			for (j = 0; j < TFTP_BOOTLOADER.Components[i]->FilesCnt; j++)
			{
				if (TFTP_BOOTLOADER.Components[i]->Files[j].Name[0] != 0)
				{
					RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, ",\r\n");
					http_server_serve_get_api_v1_system_update_add_file_to_response(&TFTP_BOOTLOADER.Components[i]->Files[j]);
				}
			}
		}
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\r\n");
		RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t]\r\n");
	}
	RESPONCE_BODY_LEN += snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "}\r\n");
	
  RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = RESPONCE_BODY_LEN;
	if (ReqHeader->ConnectionKeepAlive)
		RspHeader.Connection = HTTP_CONNECTION_KEEP_ALIVE;
	else
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (http_server_write_header(conn, &RspHeader))
		return;
	if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY))
		return;
}

/**
  * @brief  Post System Update.  
  * @param  conn pointer on connection structure 
  * @retval None
  */
static void http_server_serve_post_api_v1_system_update(struct netconn *conn, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{
	struct netbuf *inbuf;
	err_t recv_err;
	char *data;
	char *p, *_p;
	char folder[64];
	uint16_t error, datalen;
	uint32_t ContentLength;
	HTTP_RESPONCE_HEADER_T RspHeader;
	char ip[4];
	uint32_t tmp;

	ContentLength = ReqHeader->ContentLength;
	
	if (ContentLength == 0)
		goto error;
	
	inbuf = NULL;
	error = 0;
	REQUEST_BODY_LEN = 0;
	if ((body) && (bodylen))
	{
		data = body;
		datalen = bodylen;		
		goto parse_data;
	}
	
	while(ContentLength) {
		recv_err = netconn_recv(conn, &inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&data, &datalen);
parse_data:
			ContentLength -= datalen;
			if ((!error) && ((REQUEST_BODY_LEN + datalen) < (sizeof(REQUEST_BODY) - 1)))
			{
				memcpy(&REQUEST_BODY[REQUEST_BODY_LEN], data, datalen);
				REQUEST_BODY_LEN += datalen;
			}
			else
			{
				error = 1;
			}
			netbuf_delete(inbuf);
		}
		else
		{
			return;
		}
	}
	if (error)
		goto error;
	
	REQUEST_BODY[REQUEST_BODY_LEN] = 0;
	
	p = strstr(REQUEST_BODY, "\"interface\"");
	if (!p)
		goto error;
	p += sizeof("\"interface\"") - 1;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != ':')
		goto error;
	p++;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != '\"')
		goto error;
	p++;
	if (memcmp(p, "tftp\"", sizeof("tftp\"") - 1))
		goto error;
	
	p = strstr(REQUEST_BODY, "\"server_ip\"");
	if (!p)
		goto error;
	p += sizeof("\"server_ip\"") - 1;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != ':')
		goto error;
	p++;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != '\"')
		goto error;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp == 0)  || (tmp > 255) || (*p != '.'))
		goto error;
	ip[0] = tmp;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp > 255) || (*p != '.'))
		goto error;
	ip[1] = tmp;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp > 255) || (*p != '.'))
		goto error;
	ip[2] = tmp;
	p++;
	tmp = strtoul(p, &p, 10);
	if ((tmp == 0) || (tmp > 255) || (*p != '\"'))
		goto error;
	ip[3] = tmp;

	p = strstr(REQUEST_BODY, "\"server_folder\"");
	if (!p)
		goto error;
	p += sizeof("\"server_folder\"") - 1;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != ':')
		goto error;
	p++;
	while ((*p == ' ') || (*p == '\t') || (*p == '\r') || (*p == '\n'))
		p++;
	if (*p != '\"')
		goto error;
	p++;
	_p = strchr(p, '\"');
	if (!_p)
		goto error;
	memcpy(folder, p, _p - p);
	folder[_p - p] = 0;
	
	TFTP_BOOTLOADER_Update(folder, ip);
	
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
	http_server_write_header(conn, &RspHeader);
	return;
	
error:
	http_server_write_400_error(conn);
}

/**
  * @brief serve http connection  
  * @param conn: pointer on connection structure 
  * @retval None
  */
static void http_server_serve(struct netconn *conn) 
{
  struct netbuf *inbuf;
  err_t recv_err;
  char* data;
  u16_t datalen;
	char* body;
  u16_t bodylen;
	u32_t section_idx;
	u32_t param_idx;
	u32_t line_idx;
	char *c;
	HTTP_REQUEST_HEADER_T req_header;
	HTTP_RESPONCE_HEADER_T rsp_header;

receive:
	recv_err = netconn_recv(conn, &inbuf);
	if (recv_err == ERR_OK)
	{
		netbuf_data(inbuf, (void**)&data, &datalen);
		if (http_server_parse_request(data, datalen, &req_header, &body, &bodylen))
		{
			http_server_write_400_error(conn);
			netbuf_delete(inbuf);
			return;
		}
		
		if (strncmp(req_header.Url, "/api/v1/log ", sizeof("/api/v1/log ") - 1) == 0)
		{
			if (strncmp(req_header.Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) == 0)
			{
				http_server_serve_get_api_v1_log(conn, &req_header, body, bodylen);
			}
			else
			{
				rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
				rsp_header.ContentType = NULL;
				rsp_header.Charset = NULL;
				rsp_header.ContentLength = 0;
				if (req_header.ConnectionKeepAlive)
					rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
				else
					rsp_header.Connection = HTTP_CONNECTION_CLOSE;
				rsp_header.TransferEncoding = NULL;
				rsp_header.Allowed[0] = HTTP_REQUEST_GET;
				rsp_header.AllowedLen = 1;
				http_server_write_header(conn, &rsp_header);
			}
		}
		else if (strncmp(req_header.Url, "/api/v1/system/hardware ", sizeof("/api/v1/system/hardware ") - 1) == 0)
		{
			if (strncmp(req_header.Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) == 0)
			{
				http_server_serve_get_api_v1_system_hardware(conn, &req_header, body, bodylen);
			}
			else
			{
				rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
				rsp_header.ContentType = NULL;
				rsp_header.Charset = NULL;
				rsp_header.ContentLength = 0;
				if (req_header.ConnectionKeepAlive)
					rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
				else
					rsp_header.Connection = HTTP_CONNECTION_CLOSE;
				rsp_header.TransferEncoding = NULL;
				rsp_header.Allowed[0] = HTTP_REQUEST_GET;
				rsp_header.AllowedLen = 1;
				http_server_write_header(conn, &rsp_header);
			}
		}
		else if (strncmp(req_header.Url, "/api/v1/system/os ", sizeof("/api/v1/system/os ") - 1) == 0)
		{
			if (strncmp(req_header.Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) == 0)
			{
				http_server_serve_get_api_v1_system_os(conn, &req_header, body, bodylen);
			}
			else
			{
				rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
				rsp_header.ContentType = NULL;
				rsp_header.Charset = NULL;
				rsp_header.ContentLength = 0;
				if (req_header.ConnectionKeepAlive)
					rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
				else
					rsp_header.Connection = HTTP_CONNECTION_CLOSE;
				rsp_header.TransferEncoding = NULL;
				rsp_header.Allowed[0] = HTTP_REQUEST_GET;
				rsp_header.AllowedLen = 1;
				http_server_write_header(conn, &rsp_header);
			}
		}
		else if (strncmp(req_header.Url, "/api/v1/system/interfaces ", sizeof("/api/v1/system/interfaces ") - 1) == 0)
		{
			if (strncmp(req_header.Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) == 0)
			{
				http_server_serve_get_api_v1_system_interfaces(conn, &req_header, body, bodylen);
			}
			else
			{
				rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
				rsp_header.ContentType = NULL;
				rsp_header.Charset = NULL;
				rsp_header.ContentLength = 0;
				if (req_header.ConnectionKeepAlive)
					rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
				else
					rsp_header.Connection = HTTP_CONNECTION_CLOSE;
				rsp_header.TransferEncoding = NULL;
				rsp_header.Allowed[0] = HTTP_REQUEST_GET;
				rsp_header.AllowedLen = 1;
				http_server_write_header(conn, &rsp_header);
			}
		}
		else if (strncmp(req_header.Url, "/api/v1/profile ", sizeof("/api/v1/profile ") - 1) == 0)
		{
			if (strncmp(req_header.Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) == 0)
			{
				http_server_serve_get_api_v1_profile(conn, &req_header, body, bodylen);
			}
			else
			{
				rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
				rsp_header.ContentType = NULL;
				rsp_header.Charset = NULL;
				rsp_header.ContentLength = 0;
				if (req_header.ConnectionKeepAlive)
					rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
				else
					rsp_header.Connection = HTTP_CONNECTION_CLOSE;
				rsp_header.TransferEncoding = NULL;
				rsp_header.Allowed[0] = HTTP_REQUEST_GET;
				rsp_header.AllowedLen = 1;
				http_server_write_header(conn, &rsp_header);
			}
		}
		else if (strncmp(req_header.Url, "/api/v1/profile?line=", sizeof("/api/v1/profile?line=") - 1) == 0)
		{
			line_idx = strtoul(&req_header.Url[sizeof("/api/v1/profile?line=") - 1], &c, 10);
			if (line_idx >= *Profile.Descr[Profile.Idx]->TextSize)
			{
				http_server_write_404_error(conn);
			}
			else
			{
				if (strncmp(req_header.Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) == 0)
				{
					http_server_serve_get_api_v1_profile_line(conn, &req_header, body, bodylen, line_idx);
				}
				else
				{
					rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
					rsp_header.ContentType = NULL;
					rsp_header.Charset = NULL;
					rsp_header.ContentLength = 0;
					if (req_header.ConnectionKeepAlive)
						rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
					else
						rsp_header.Connection = HTTP_CONNECTION_CLOSE;
					rsp_header.TransferEncoding = NULL;
					rsp_header.Allowed[0] = HTTP_REQUEST_GET;
					rsp_header.AllowedLen = 1;
					http_server_write_header(conn, &rsp_header);
				}
			}
		}
		else if (strncmp(req_header.Url, "/api/v1/profile/sections/", sizeof("/api/v1/profile/sections/") - 1) == 0)
		{
			section_idx = strtoul(&req_header.Url[sizeof("/api/v1/profile/sections/") - 1], &c, 10);
			if (((section_idx != 1) &&
					(section_idx != 2) &&
					(section_idx != 3) &&
					(section_idx != 4) &&
					(section_idx != 5) &&
					(section_idx != 8) &&
					(section_idx != 103)) ||
					((section_idx == 1) && (Profile.Descr[Profile.Idx]->Table1Size == 0)) ||
					((section_idx == 2) && (Profile.Descr[Profile.Idx]->Table2Size == 0)) ||
					((section_idx == 3) && (Profile.Descr[Profile.Idx]->Table3Size == 0)) ||
					((section_idx == 4) && (Profile.Descr[Profile.Idx]->Table4Size == 0)) ||
					((section_idx == 5) && (Profile.Descr[Profile.Idx]->Table5Size == 0)) ||
					((section_idx == 8) && (Profile.Descr[Profile.Idx]->Table8Size == 0)) ||
					((section_idx == 103) && (Profile.Descr[Profile.Idx]->Table103Size == 0)))
			{
				http_server_write_404_error(conn);
			}
			else
			{
				if (strncmp(c, "/params/", sizeof("/params/") - 1) == 0)
				{
					param_idx = strtoul(&c[sizeof("/params/") - 1], &c, 10);
					if (((section_idx == 1) && (param_idx >= Profile.Descr[Profile.Idx]->Table1Size)) ||
							((section_idx == 2) && (param_idx >= Profile.Descr[Profile.Idx]->Table2Size)) ||
							((section_idx == 3) && (param_idx >= Profile.Descr[Profile.Idx]->Table3Size)) ||
							((section_idx == 4) && (param_idx >= Profile.Descr[Profile.Idx]->Table4Size)) ||
							((section_idx == 5) && (param_idx >= Profile.Descr[Profile.Idx]->Table5Size)) ||
							((section_idx == 8) && (param_idx >= Profile.Descr[Profile.Idx]->Table8Size)) ||
							((section_idx == 103) && (param_idx >= Profile.Descr[Profile.Idx]->Table103Size)))
					{
						http_server_write_404_error(conn);
					}
					else
					{
						if (strncmp(c, "/value ", sizeof("/value ") - 1) == 0)
						{
							if (strncmp(req_header.Type, HTTP_REQUEST_PUT" ", sizeof(HTTP_REQUEST_PUT)) == 0)
							{
								http_server_serve_put_api_v1_profile_section_param_value(conn, &req_header, body, bodylen, section_idx, param_idx); 
							}
							else
							{
								rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
								rsp_header.ContentType = NULL;
								rsp_header.Charset = NULL;
								rsp_header.ContentLength = 0;
								if (req_header.ConnectionKeepAlive)
									rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
								else
									rsp_header.Connection = HTTP_CONNECTION_CLOSE;
								rsp_header.TransferEncoding = NULL;
								rsp_header.Allowed[0] = HTTP_REQUEST_PUT;
								rsp_header.AllowedLen = 1;
								http_server_write_header(conn, &rsp_header);
							}
						}
						else if (strncmp(c, "/colors ", sizeof("/colors ") - 1) == 0)
						{
							if (strncmp(req_header.Type, HTTP_REQUEST_PUT" ", sizeof(HTTP_REQUEST_PUT)) == 0)
							{
								http_server_serve_put_api_v1_profile_section_param_color(conn, &req_header, body, bodylen, section_idx, param_idx); 
							}
							else
							{
								rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
								rsp_header.ContentType = NULL;
								rsp_header.Charset = NULL;
								rsp_header.ContentLength = 0;
								if (req_header.ConnectionKeepAlive)
									rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
								else
									rsp_header.Connection = HTTP_CONNECTION_CLOSE;
								rsp_header.TransferEncoding = NULL;
								rsp_header.Allowed[0] = HTTP_REQUEST_PUT;
								rsp_header.AllowedLen = 1;
								http_server_write_header(conn, &rsp_header);
							}
						}
						else
						{
							http_server_write_404_error(conn);
						}
					}
				}
				else if (strncmp(c, "/values ", sizeof("/values ") - 1) == 0)
				{
					if (strncmp(req_header.Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) == 0)
					{
						http_server_serve_get_api_v1_profile_section_values(conn, &req_header, body, bodylen, section_idx);
					}
					else
					{
						rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
						rsp_header.ContentType = NULL;
						rsp_header.Charset = NULL;
						rsp_header.ContentLength = 0;
						if (req_header.ConnectionKeepAlive)
							rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
						else
							rsp_header.Connection = HTTP_CONNECTION_CLOSE;
						rsp_header.TransferEncoding = NULL;
						rsp_header.Allowed[0] = HTTP_REQUEST_GET;
						rsp_header.AllowedLen = 1;
						http_server_write_header(conn, &rsp_header);
					}
				}
				else if ((strncmp(c, "/colors ", sizeof("/colors ") - 1) == 0) && (section_idx == 2))
				{
					if (strncmp(req_header.Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) == 0)
					{
						http_server_serve_get_api_v1_profile_section_colors(conn, &req_header, body, bodylen, section_idx);
					}
					else
					{
						rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
						rsp_header.ContentType = NULL;
						rsp_header.Charset = NULL;
						rsp_header.ContentLength = 0;
						if (req_header.ConnectionKeepAlive)
							rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
						else
							rsp_header.Connection = HTTP_CONNECTION_CLOSE;
						rsp_header.TransferEncoding = NULL;
						rsp_header.Allowed[0] = HTTP_REQUEST_GET;
						rsp_header.AllowedLen = 1;
						http_server_write_header(conn, &rsp_header);
					}
				}
				else
				{
					http_server_write_404_error(conn);
				}
			}
		}
		else if (strncmp(req_header.Url, "/api/v1/profile/config ", sizeof("/api/v1/profile/config ") - 1) == 0)
		{
			if (strncmp(req_header.Type, HTTP_REQUEST_HEAD" ", sizeof(HTTP_REQUEST_HEAD)) == 0)
			{
				http_server_serve_head_api_v1_profile_config(conn, &req_header, body, bodylen);
			}
			else if (strncmp(req_header.Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) == 0)
			{
				http_server_serve_get_api_v1_profile_config(conn, &req_header, body, bodylen);
			}
			else if (strncmp(req_header.Type, HTTP_REQUEST_PUT" ", sizeof(HTTP_REQUEST_PUT)) == 0)
			{
				http_server_serve_put_api_v1_profile_config(conn, &req_header, body, bodylen);
			}
			else
			{
				rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
				rsp_header.ContentType = NULL;
				rsp_header.Charset = NULL;
				rsp_header.ContentLength = 0;
				if (req_header.ConnectionKeepAlive)
					rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
				else
					rsp_header.Connection = HTTP_CONNECTION_CLOSE;
				rsp_header.TransferEncoding = NULL;
				rsp_header.Allowed[0] = HTTP_REQUEST_HEAD;
				rsp_header.Allowed[1] = HTTP_REQUEST_GET;
				rsp_header.Allowed[2] = HTTP_REQUEST_PUT;
				rsp_header.AllowedLen = 3;
				http_server_write_header(conn, &rsp_header);
			}
		}
		else if (strncmp(req_header.Url, "/api/v1/profile/langpack ", sizeof("/api/v1/profile/langpack ") - 1) == 0)
		{
			if (strncmp(req_header.Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) == 0)
			{
				http_server_serve_get_api_v1_profile_langpack(conn, &req_header, body, bodylen);
			}
			else if (strncmp(req_header.Type, HTTP_REQUEST_PUT" ", sizeof(HTTP_REQUEST_PUT)) == 0)
			{
				http_server_serve_put_api_v1_profile_langpack(conn, &req_header, body, bodylen);
			}
			else
			{
				rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
				rsp_header.ContentType = NULL;
				rsp_header.Charset = NULL;
				rsp_header.ContentLength = 0;
				if (req_header.ConnectionKeepAlive)
					rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
				else
					rsp_header.Connection = HTTP_CONNECTION_CLOSE;
				rsp_header.TransferEncoding = NULL;
				rsp_header.Allowed[0] = HTTP_REQUEST_GET;
				rsp_header.Allowed[1] = HTTP_REQUEST_PUT;
				rsp_header.AllowedLen = 2;
				http_server_write_header(conn, &rsp_header);
			}
		}
		else if (strncmp(req_header.Url, "/api/v1/system/update/mcu/software ", sizeof("/api/v1/system/update/mcu/software ") - 1) == 0)
		{
			if (strncmp(req_header.Type, HTTP_REQUEST_POST" ", sizeof(HTTP_REQUEST_POST)) == 0)
			{
				http_server_serve_post_api_v1_system_update_mcu_software(conn, &req_header, body, bodylen);
			}
			else
			{
				rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
				rsp_header.ContentType = NULL;
				rsp_header.Charset = NULL;
				rsp_header.ContentLength = 0;
				if (req_header.ConnectionKeepAlive)
					rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
				else
					rsp_header.Connection = HTTP_CONNECTION_CLOSE;
				rsp_header.TransferEncoding = NULL;
				rsp_header.Allowed[0] = HTTP_REQUEST_POST;
				rsp_header.AllowedLen = 1;
				http_server_write_header(conn, &rsp_header);
			}
		}
		else if (strncmp(req_header.Url, "/api/v1/system/update/mcu/web ", sizeof("/api/v1/system/update/mcu/web ") - 1) == 0)
		{
			if (strncmp(req_header.Type, HTTP_REQUEST_POST" ", sizeof(HTTP_REQUEST_POST)) == 0)
			{
				http_server_serve_post_api_v1_system_update_mcu_web(conn, &req_header, body, bodylen);
			}
			else
			{
				rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
				rsp_header.ContentType = NULL;
				rsp_header.Charset = NULL;
				rsp_header.ContentLength = 0;
				if (req_header.ConnectionKeepAlive)
					rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
				else
					rsp_header.Connection = HTTP_CONNECTION_CLOSE;
				rsp_header.TransferEncoding = NULL;
				rsp_header.Allowed[0] = HTTP_REQUEST_POST;
				rsp_header.AllowedLen = 1;
				http_server_write_header(conn, &rsp_header);
			}
		}
		else if (strncmp(req_header.Url, "/api/v1/system/update ", sizeof("/api/v1/system/update ") - 1) == 0)
		{
			if (strncmp(req_header.Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) == 0)
			{
				http_server_serve_get_api_v1_system_update(conn, &req_header, body, bodylen);
			}
			else if (strncmp(req_header.Type, HTTP_REQUEST_POST" ", sizeof(HTTP_REQUEST_POST)) == 0)
			{
				http_server_serve_post_api_v1_system_update(conn, &req_header, body, bodylen);
			}
			else
			{
				rsp_header.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
				rsp_header.ContentType = NULL;
				rsp_header.Charset = NULL;
				rsp_header.ContentLength = 0;
				if (req_header.ConnectionKeepAlive)
					rsp_header.Connection = HTTP_CONNECTION_KEEP_ALIVE;
				else
					rsp_header.Connection = HTTP_CONNECTION_CLOSE;
				rsp_header.TransferEncoding = NULL;
				rsp_header.Allowed[0] = HTTP_REQUEST_GET;
				rsp_header.Allowed[1] = HTTP_REQUEST_POST;
				rsp_header.AllowedLen = 2;
				http_server_write_header(conn, &rsp_header);
			}
		}
		else 
		{
			http_server_serve_get_file(conn, &req_header, body, bodylen);
		}
		netbuf_delete(inbuf);
		if (req_header.ConnectionKeepAlive)
			goto receive;
	}
}

/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void HTTP_SERVER_Thread(void *arg)
{ 
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
    /* Bind to port 80 (HTTP) with default IP address */
    err = netconn_bind(conn, NULL, 80);
    
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
  
      while(1) 
      {
        /* Accept any icoming connection */
        accept_err = netconn_accept(conn, &newconn);
        if(accept_err == ERR_OK)
        {
					/* Serve incoming connection */
					http_server_serve(newconn);

					/* Close the connection*/
					netconn_close(newconn);

					/* delete connection */
					netconn_delete(newconn);    
        }
      }
    }
  }
}
 
void HTTP_SERVER_Init(void)
{
   sys_thread_new("HTTP", HTTP_SERVER_Thread, NULL, HTTP_SERVER_THREAD_STACK_SIZE, HTTP_SERVER_THREAD_PRIO);
}
