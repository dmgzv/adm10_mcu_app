#include "http_server.h"
#include "http_server_url.h"
#include "http_server_files.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HTTP_SERVER_THREAD_STACK_SIZE  ( 1024 )
#define HTTP_SERVER_THREAD_PRIO    ( osPriorityAboveNormal )
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static char RESPONCE_HEADER[512];
static uint32_t RESPONCE_HEADER_LEN;
static char RESPONCE_BODY[4096];
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
	char *p1, *p2;
	
	memset(Header, 0, sizeof(HTTP_REQUEST_HEADER_T));
	
	p2 = p1 = data;
	
	if (!memcmp(p1, HTTP_REQUEST_HEAD, sizeof(HTTP_REQUEST_HEAD) - 1))
	{
		Header->Type = p2;
		Header->TypeSize = sizeof(HTTP_REQUEST_HEAD) - 1;
		p1 = p1 + sizeof(HTTP_REQUEST_HEAD) - 1;
	}
	else if (!memcmp(p1, HTTP_REQUEST_GET, sizeof(HTTP_REQUEST_GET) - 1))
	{
		Header->Type = p2;
		Header->TypeSize = sizeof(HTTP_REQUEST_GET) - 1;
		p1 = p1 + sizeof(HTTP_REQUEST_GET) - 1;
	}
	else if (!memcmp(p1, HTTP_REQUEST_PUT, sizeof(HTTP_REQUEST_PUT) - 1))
	{
		Header->Type = p2;
		Header->TypeSize = sizeof(HTTP_REQUEST_PUT) - 1;
		p1 = p1 + sizeof(HTTP_REQUEST_PUT) - 1;
	}
	else if (!memcmp(p1, HTTP_REQUEST_POST, sizeof(HTTP_REQUEST_POST) - 1))
	{
		Header->Type = p2;
		Header->TypeSize = sizeof(HTTP_REQUEST_POST) - 1;
		p1 = p1 + sizeof(HTTP_REQUEST_POST) - 1;
	}
	else
	{
		return -1;
	}
	
	if (((p1 - data) > datalen) || (*p1 != ' '))
		return -1;
	
	p2 = ++p1;	
	if (((p1 - data) > datalen) || (*p1 != '/'))
		return -1;
	while (((p1 - data) <= datalen) && \
				(((*p1 >= '0') && (*p1 <= '9')) || \
				((*p1 >= 'a') && (*p1 <= 'z')) || \
				((*p1 >= 'A') && (*p1 <= 'Z')) ||\
				(*p1 == '/') || (*p1 == '.') || \
				(*p1 == '_') || (*p1 == '-') || \
				(*p1 == '~')))
	{
			p1++;
	}	
	if ((((p1 - data) <= datalen)) && ((*p1 == '?') || (*p1 == ' ')))
	{
		Header->Url = p2;
		Header->UrlSize = p1 - p2;
	}
	else
	{
		return -1;
	}
	
	if ((((p1 - data) <= datalen)) && (*p1 == '?'))
	{
		p2 = ++p1;		
		while (((p1 - data) <= datalen) && \
					(((*p1 >= '0') && (*p1 <= '9')) || \
					((*p1 >= 'a') && (*p1 <= 'z')) || \
					((*p1 >= 'A') && (*p1 <= 'Z')) ||\
					(*p1 == '/') || (*p1 == '.') || \
					(*p1 == '_') || (*p1 == '-') || \
					(*p1 == '~') || (*p1 == '=') || \
					(*p1 == '&') || (*p1 == '+')))
		{
				p1++;
		}
		if (((p1 - data) <= datalen) && (*p1 == ' '))
		{
			Header->Query = p2;
			Header->QuerySize = p1 - p2;
		}
		else
		{
			return -1;
		}
	}
	
	while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
	{
		if ((p1 - data) < datalen)
			p1++;
		else
			return -1;
	}
	p1 = p1 + 2;
	
	while ((p1 - data) < datalen)
	{
		if (!memcmp(p1, "\r\n", sizeof("\r\n") - 1))
		{
			p2 = p1 = p1 + sizeof("\r\n") - 1;
			if ((p1 - data) < datalen)
			{
				*body = p2;
				*bodylen = data + datalen - p2;
			}
			else
			{
				*body = NULL;
				*bodylen = 0;
			}
			return 0;
		}
		else if (!memcmp(p1, "Connection: ", sizeof("Connection: ") - 1))
		{
			p2 = p1 = p1 + sizeof("Connection: ") - 1;
			while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
			{
				if ((p1 - data) < datalen)
					p1++;
				else
					return -1;
			}
			Header->Connection = p2;
			Header->ConnectionSize = p1 - p2;
			p1 = p1 + 2;
		}
		else if (!memcmp(p1, "connection: ", sizeof("connection: ") - 1))
		{
			p2 = p1 = p1 + sizeof("connection: ") - 1;
			while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
			{
				if ((p1 - data) < datalen)
					p1++;
				else
					return -1;
			}
			Header->Connection = p2;
			Header->ConnectionSize = p1 - p2;
			p1 = p1 + 2;
		}
		else if (!memcmp(p1, "Content-Type: ", sizeof("Content-Type: ") - 1))
		{
			p2 = p1 = p1 + sizeof("Content-Type: ") - 1;
			while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
			{
				if ((p1 - data) < datalen)
					p1++;
				else
					return -1;
			}
			Header->ContentType = p2;
			Header->ContentTypeSize = p1 - p2;
			p1 = p1 + 2;
		}
		else if (!memcmp(p1, "content-type: ", sizeof("content-type: ") - 1))
		{
			p2 = p1 = p1 + sizeof("content-type: ") - 1;
			while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
			{
				if ((p1 - data) < datalen)
					p1++;
				else
					return -1;
			}
			Header->ContentType = p2;
			Header->ContentTypeSize = p1 - p2;
			p1 = p1 + 2;
		}
		else if (!memcmp(p1, "Content-Length: ", sizeof("Content-Length: ") - 1))
		{
			p2 = p1 = p1 + sizeof("Content-Length: ") - 1;
			while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
			{
				if ((p1 - data) < datalen)
					p1++;
				else
					return -1;
			}
			Header->ContentLength = p2;
			Header->ContentLengthSize = p1 - p2;
			p1 = p1 + 2;
		}
		else if (!memcmp(p1, "content-length: ", sizeof("content-length: ") - 1))
		{
			p2 = p1 = p1 + sizeof("content-length: ") - 1;
			while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
			{
				if ((p1 - data) < datalen)
					p1++;
				else
					return -1;
			}
			Header->ContentLength = p2;
			Header->ContentLengthSize = p1 - p2;
			p1 = p1 + 2;
		}
		else
		{
			while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
			{
				if ((p1 - data) < datalen)
					p1++;
				else
					return -1;
			}
			p1 = p1 + 2;
		}
	}
	return -1;
}

int32_t http_server_write_400_error(struct netconn *conn)
{
	if (http_server_write(conn, HTTP_RESPONCE_400_BAD_REQUEST"\r\n\r\n", sizeof(HTTP_RESPONCE_400_BAD_REQUEST"\r\n\r\n") - 1, NETCONN_NOCOPY))
		return -1;
	return 0;
}
 
int32_t http_server_write_404_error(struct netconn *conn)
{
	if (http_server_write(conn, HTTP_RESPONCE_404_NOT_FOUND"\r\n\r\n", sizeof(HTTP_RESPONCE_404_NOT_FOUND"\r\n\r\n") - 1, NETCONN_NOCOPY))
		return -1;
	return 0;
}

int32_t http_server_write_500_error(struct netconn *conn)
{
	if (http_server_write(conn, HTTP_RESPONCE_500_INTERNAL_SERVER_ERROR"\r\n\r\n", sizeof(HTTP_RESPONCE_500_INTERNAL_SERVER_ERROR"\r\n\r\n") - 1, NETCONN_NOCOPY))
		return -1;
	return 0;
}

int32_t http_server_write_header(struct netconn *conn, HTTP_RESPONCE_HEADER_T *Header)
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

int32_t http_server_write_body_start(struct netconn *conn)
{
	RESPONCE_BODY_LEN = 0;
	return 0;
}

int32_t http_server_write_body(struct netconn *conn, char *str, ...)
{
	char chunk_size[16];
	
	va_list va;
	va_start(va, str);
	RESPONCE_BODY_LEN += vsnprintf(&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY), str, va);
	va_end(va);
	if (RESPONCE_BODY_LEN >= 1024)
	{
		snprintf (chunk_size, sizeof(chunk_size), "%X\r\n", RESPONCE_BODY_LEN);
		if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY | NETCONN_MORE))
			return -1;	
		RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
		RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
		if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
			return -1;
		RESPONCE_BODY_LEN = 0;
	}
  return 0;
}

int32_t http_server_write_body_data(struct netconn *conn, char *data, uint32_t datalen)
{
	char chunk_size[16];
	uint32_t datashift;
	
	while (datalen)
	{
		if (datalen + RESPONCE_BODY_LEN >= 1024)
		{
			datashift = 1024 - RESPONCE_BODY_LEN;
			memcpy(&RESPONCE_BODY[RESPONCE_BODY_LEN], data, datashift);
			RESPONCE_BODY_LEN = 1024;
			snprintf(chunk_size, sizeof(chunk_size), "%X\r\n", RESPONCE_BODY_LEN);
			if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY | NETCONN_MORE))
				return -1;	
			RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
			RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
			if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
				return -1;
			data += datashift;
			datalen -= datashift;
			RESPONCE_BODY_LEN = 0;
		}
		else
		{
			memcpy(&RESPONCE_BODY[RESPONCE_BODY_LEN], data, datalen);
			RESPONCE_BODY_LEN += datalen;
			data += datalen;
			datalen = 0;
		}
	}
	return 0;
}

int32_t http_server_write_body_finish(struct netconn *conn)
{
	char chunk_size[16];
	
	if (RESPONCE_BODY_LEN)
	{
		snprintf (chunk_size, sizeof(chunk_size), "%X\r\n", RESPONCE_BODY_LEN);
		if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY | NETCONN_MORE))
			return -1;	
		RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\r';
		RESPONCE_BODY[RESPONCE_BODY_LEN++] = '\n';
		if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
			return -1;
		RESPONCE_BODY_LEN = 0;
	}	
	strncpy(chunk_size, "0\r\n\r\n", sizeof(chunk_size));
	if (http_server_write(conn, chunk_size, strlen(chunk_size), NETCONN_COPY))
		return - 1;
	return 0;
}


static void http_server_serve_get_file(struct netconn *conn, struct netbuf *inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char *body, uint16_t bodylen)
{	
	uint32_t data_addr, data_size, data_type;
	char *name;
	uint32_t name_size;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	name = ReqHeader->Url + 1;
	name_size = ReqHeader->UrlSize - 1;
	
	if (HTTP_SERVER_FILES_FindByName(name, name_size, &data_addr, &data_size, &data_type))
		goto error_404;

	if (strncmp(ReqHeader->Type, HTTP_REQUEST_GET" ", sizeof(HTTP_REQUEST_GET)) != 0)	
		goto error_405;
	
	if (data_type == HTTP_SERVER_FILES_DATA_TYPE_HTML)
	{
		netbuf_delete(inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_HTML;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = data_size;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (http_server_write_header(conn, &RspHeader))
			return;
	}
	else if (data_type == HTTP_SERVER_FILES_DATA_TYPE_JS)
	{
		netbuf_delete(inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_JS;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = data_size;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (http_server_write_header(conn, &RspHeader))
			return;
	}
	else if (data_type == HTTP_SERVER_FILES_DATA_TYPE_CSS)
	{
		netbuf_delete(inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_TEXT_CSS;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = data_size;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (http_server_write_header(conn, &RspHeader))
			return;
	}
	else if (data_type == HTTP_SERVER_FILES_DATA_TYPE_WOFF2)
	{
		netbuf_delete(inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_FONT_WOFF2;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = data_size;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (http_server_write_header(conn, &RspHeader))
			return;
	}
	else if (data_type == HTTP_SERVER_FILES_DATA_TYPE_JSON)
	{
		netbuf_delete(inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
		RspHeader.Charset = HTTP_CHARSET_UTF_8;
		RspHeader.ContentLength = data_size;
		RspHeader.Connection = HTTP_CONNECTION_CLOSE;
		RspHeader.TransferEncoding = NULL;
		RspHeader.AllowedLen = 0;
		if (http_server_write_header(conn, &RspHeader))
			return;
	}
	else
	{
		netbuf_delete(inbuf);	
		RspHeader.Code = HTTP_RESPONCE_200_OK;
		RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_OCTET;
		RspHeader.Charset = NULL;
		RspHeader.ContentLength = data_size;
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
	netbuf_delete(inbuf);	
	http_server_write_404_error(conn);
	return;
error_405:
	netbuf_delete(inbuf);	
	RspHeader.Code = HTTP_RESPONCE_405_METHOD_NOT_ALLOWED;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.Allowed[0] = HTTP_REQUEST_GET;
	RspHeader.AllowedLen = 1;
	http_server_write_header(conn, &RspHeader);
	return;
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
	HTTP_REQUEST_HEADER_T req_header;

	recv_err = netconn_recv(conn, &inbuf);
	if (recv_err == ERR_OK)
	{
		netbuf_data(inbuf, (void**)&data, &datalen);
		if (http_server_parse_request(data, datalen, &req_header, &body, &bodylen))
		{
			netbuf_delete(inbuf);
			http_server_write_400_error(conn);
			return;
		}
		
		if (!HTTP_SERVER_URL_Process(conn, inbuf, &req_header, body, bodylen))
			return;
		http_server_serve_get_file(conn, inbuf, &req_header, body, bodylen);
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
