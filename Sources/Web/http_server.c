#include "http_server.h"
#include "http_server_url.h"
#include "base64.h"
#include "log.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HTTP_SERVER_THREAD_STACK_SIZE  ( 6*128 )
#define HTTP_SERVER_THREAD_PRIO    ( osPriorityAboveNormal )
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static char HTTP_SERVER_Buffer[4096];
static uint32_t HTTP_SERVER_BufferLen;

static int32_t HTTP_SERVER_Write(struct netconn *Conn, const void *Data, size_t Size, u8_t Flags)
{
	err_t WriteError;
	while (1)
	{
		WriteError = netconn_write(Conn, Data, Size, Flags);
		if (WriteError == ERR_OK)
		{
			break;
		}
		else if (WriteError == ERR_INPROGRESS)
		{
			osDelay(1);
		}
		else
		{
			LOG_Error("HTTP", "SERVER", "", "Write error: %d", WriteError);
			return -1;
		}
	}
	return 0;
}

static int32_t HTTP_SERVER_ParseRequest(char *Data, uint32_t DataLen, HTTP_REQUEST_HEADER_T *Header)
{
	char *p1, *p2;
	
	memset(Header, 0, sizeof(HTTP_REQUEST_HEADER_T));
	
	p2 = p1 = Data;
	
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
	else if (!memcmp(p1, HTTP_REQUEST_DELETE, sizeof(HTTP_REQUEST_DELETE) - 1))
	{
		Header->Type = p2;
		Header->TypeSize = sizeof(HTTP_REQUEST_DELETE) - 1;
		p1 = p1 + sizeof(HTTP_REQUEST_DELETE) - 1;
	}
	else
	{
		return -1;
	}
	
	if (((p1 - Data) > DataLen) || (*p1 != ' '))
		return -1;
	
	p2 = ++p1;	
	if (((p1 - Data) > DataLen) || (*p1 != '/'))
		return -1;
	while (((p1 - Data) <= DataLen) && \
				(((*p1 >= '0') && (*p1 <= '9')) || \
				((*p1 >= 'a') && (*p1 <= 'z')) || \
				((*p1 >= 'A') && (*p1 <= 'Z')) ||\
				(*p1 == '/') || (*p1 == '.') || \
				(*p1 == '_') || (*p1 == '-') || \
				(*p1 == '~')))
	{
			p1++;
	}	
	if ((((p1 - Data) <= DataLen)) && ((*p1 == '?') || (*p1 == ' ')))
	{
		Header->Url = p2;
		Header->UrlSize = p1 - p2;
	}
	else
	{
		return -1;
	}
	
	if ((((p1 - Data) <= DataLen)) && (*p1 == '?'))
	{
		p2 = ++p1;		
		while (((p1 - Data) <= DataLen) && \
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
		if (((p1 - Data) <= DataLen) && (*p1 == ' '))
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
		if ((p1 - Data) < DataLen)
			p1++;
		else
			return -1;
	}
	p1 = p1 + 2;
	
	while ((p1 - Data) < DataLen)
	{
		if (!memcmp(p1, "\r\n", sizeof("\r\n") - 1))
		{
			p2 = p1 = p1 + sizeof("\r\n") - 1;
			if ((p1 - Data) < DataLen)
			{
				Header->Body = p2;
				Header->BodyLen = Data + DataLen - p2;
			}
			else
			{
				Header->Body = NULL;
				Header->BodyLen = 0;
			}
			return 0;
		}
		else if (!memcmp(p1, "Connection: ", sizeof("Connection: ") - 1))
		{
			p2 = p1 = p1 + sizeof("Connection: ") - 1;
			while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
			{
				if ((p1 - Data) < DataLen)
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
				if ((p1 - Data) < DataLen)
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
				if ((p1 - Data) < DataLen)
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
				if ((p1 - Data) < DataLen)
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
				if ((p1 - Data) < DataLen)
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
				if ((p1 - Data) < DataLen)
					p1++;
				else
					return -1;
			}
			Header->ContentLength = p2;
			Header->ContentLengthSize = p1 - p2;
			p1 = p1 + 2;
		}
		else if (!memcmp(p1, "Authorization: ", sizeof("Authorization: ") - 1))
		{
			p2 = p1 = p1 + sizeof("Authorization: ") - 1;
			while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
			{
				if ((p1 - Data) < DataLen)
					p1++;
				else
					return -1;
			}
			Header->Authorization = p2;
			Header->AuthorizationSize = p1 - p2;
			p1 = p1 + 2;
		}
		else if (!memcmp(p1, "authorization: ", sizeof("authorization: ") - 1))
		{
			p2 = p1 = p1 + sizeof("authorization: ") - 1;
			while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
			{
				if ((p1 - Data) < DataLen)
					p1++;
				else
					return -1;
			}
			Header->Authorization = p2;
			Header->AuthorizationSize = p1 - p2;
			p1 = p1 + 2;
		}
		else
		{
			while ((*p1 != '\r') || (*(p1 + 1) != '\n'))
			{
				if ((p1 - Data) < DataLen)
					p1++;
				else
					return -1;
			}
			p1 = p1 + 2;
		}
	}
	return -1;
}

int32_t __HTTP_SERVER_Authenticate(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, USER_T *User)
{
	char *Base64, *Login, *Password, *c;
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
	
	if (USER_GetUserByLogin(Login, User))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	if (strcmp(Password, User->Password))
	{
		netbuf_delete(Inbuf);
		goto error_401;
	}
	
	return 0;
	
error_401:
	__HTTP_SERVER_WriteError401(Conn);
	return -1;
}

int32_t __HTTP_SERVER_WriteError301(struct netconn *Conn, char *Location)
{
	HTTP_SERVER_BufferLen = sprintf(HTTP_SERVER_Buffer, HTTP_RESPONCE_301_MOVED_PERMANENTLY"\r\nLocation: %s\r\n\r\n", Location);	
	if (HTTP_SERVER_Write(Conn, HTTP_SERVER_Buffer, HTTP_SERVER_BufferLen, NETCONN_COPY))
		return -1;
	return 0;
}

int32_t __HTTP_SERVER_WriteError400(struct netconn *Conn)
{
	if (HTTP_SERVER_Write(Conn, HTTP_RESPONCE_400_BAD_REQUEST"\r\n\r\n", sizeof(HTTP_RESPONCE_400_BAD_REQUEST"\r\n\r\n") - 1, NETCONN_NOCOPY))
		return -1;
	return 0;
}

int32_t __HTTP_SERVER_WriteError401(struct netconn *Conn)
{
	if (HTTP_SERVER_Write(Conn, HTTP_RESPONCE_401_UNAUTHORIZED"\r\nWWW-Authenticate: Basic realm=\"Access to device data\"\r\n\r\n", sizeof(HTTP_RESPONCE_401_UNAUTHORIZED"\r\nWWW-Authenticate: Basic realm=\"Access to device data\"\r\n\r\n") - 1, NETCONN_NOCOPY))
		return -1;
	return 0;
}

int32_t __HTTP_SERVER_WriteError403(struct netconn *Conn)
{
	if (HTTP_SERVER_Write(Conn, HTTP_RESPONCE_403_FORBIDDEN"\r\n\r\n", sizeof(HTTP_RESPONCE_403_FORBIDDEN"\r\n\r\n") - 1, NETCONN_NOCOPY))
		return -1;
	return 0;
}
 
int32_t __HTTP_SERVER_WriteError404(struct netconn *Conn)
{
	if (HTTP_SERVER_Write(Conn, HTTP_RESPONCE_404_NOT_FOUND"\r\n\r\n", sizeof(HTTP_RESPONCE_404_NOT_FOUND"\r\n\r\n") - 1, NETCONN_NOCOPY))
		return -1;
	return 0;
}

int32_t __HTTP_SERVER_WriteError500(struct netconn *Conn)
{
	if (HTTP_SERVER_Write(Conn, HTTP_RESPONCE_500_INTERNAL_SERVER_ERROR"\r\n\r\n", sizeof(HTTP_RESPONCE_500_INTERNAL_SERVER_ERROR"\r\n\r\n") - 1, NETCONN_NOCOPY))
		return -1;
	return 0;
}

int32_t __HTTP_SERVER_WriteHeader(struct netconn *Conn, HTTP_RESPONCE_HEADER_T *Header)
{
	uint32_t i;
	
	HTTP_SERVER_BufferLen = 0;
	
	HTTP_SERVER_BufferLen = sprintf(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], "%s\r\n", Header->Code);
	if (Header->Connection)
		HTTP_SERVER_BufferLen += sprintf(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], "Connection: %s\r\n", Header->Connection);
	
	if (Header->ContentType)
	{
		if (Header->Charset)
			HTTP_SERVER_BufferLen += sprintf(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], "Content-Type: %s; charset=%s\r\n", Header->ContentType, Header->Charset);
		else
			HTTP_SERVER_BufferLen += sprintf(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], "Content-Type: %s\r\n", Header->ContentType);
	}
	
	if (Header->ContentLength)
		HTTP_SERVER_BufferLen += sprintf(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], "Content-Length: %d\r\n", Header->ContentLength);
	
	if (Header->TransferEncoding)
		HTTP_SERVER_BufferLen += sprintf(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], "Transfer-Encoding: %s\r\n", Header->TransferEncoding);
	
	if (Header->AllowedLen)
	{
		HTTP_SERVER_BufferLen += sprintf(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], "Allow: ");
		for (i = 0; i < Header->AllowedLen; i++)
		{
			HTTP_SERVER_BufferLen += sprintf(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], "%s", Header->Allowed[i]);
			if (i == Header->AllowedLen - 1)
			{
				memcpy(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], "\r\n", sizeof("\r\n"));
				HTTP_SERVER_BufferLen += sizeof("\r\n") - 1;
			}
			else
			{
				memcpy(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], ", ", sizeof(", "));
				HTTP_SERVER_BufferLen += sizeof(", ") - 1;
			}
		}
	}
	
	memcpy(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], "\r\n", sizeof("\r\n"));
	HTTP_SERVER_BufferLen += sizeof("\r\n") - 1;

	return HTTP_SERVER_Write(Conn, HTTP_SERVER_Buffer, HTTP_SERVER_BufferLen, NETCONN_COPY);
}

int32_t __HTTP_SERVER_StartBody(struct netconn *Conn)
{
	HTTP_SERVER_BufferLen = 0;
	return 0;
}

int32_t __HTTP_SERVER_WriteBody(struct netconn *Conn, char *str, ...)
{
	char ChunkSize[16];
	
	va_list va;
	va_start(va, str);
	HTTP_SERVER_BufferLen += vsnprintf(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], sizeof(HTTP_SERVER_Buffer), str, va);
	va_end(va);
	if (HTTP_SERVER_BufferLen >= 1024)
	{
		snprintf (ChunkSize, sizeof(ChunkSize), "%X\r\n", HTTP_SERVER_BufferLen);
		if (HTTP_SERVER_Write(Conn, ChunkSize, strlen(ChunkSize), NETCONN_COPY | NETCONN_MORE))
			return -1;	
		HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen++] = '\r';
		HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen++] = '\n';
		if (HTTP_SERVER_Write(Conn, HTTP_SERVER_Buffer, HTTP_SERVER_BufferLen, NETCONN_COPY | NETCONN_MORE))
			return -1;
		HTTP_SERVER_BufferLen = 0;
	}
  return 0;
}

int32_t __HTTP_SERVER_WriteBodyData(struct netconn *Conn, char *Data, uint32_t DataLen)
{
	char ChunkSize[16];
	uint32_t DataShift;
	
	while (DataLen)
	{
		if (DataLen + HTTP_SERVER_BufferLen >= 1024)
		{
			DataShift = 1024 - HTTP_SERVER_BufferLen;
			memcpy(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], Data, DataShift);
			HTTP_SERVER_BufferLen = 1024;
			snprintf(ChunkSize, sizeof(ChunkSize), "%X\r\n", HTTP_SERVER_BufferLen);
			if (HTTP_SERVER_Write(Conn, ChunkSize, strlen(ChunkSize), NETCONN_COPY | NETCONN_MORE))
				return -1;	
			HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen++] = '\r';
			HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen++] = '\n';
			if (HTTP_SERVER_Write(Conn, HTTP_SERVER_Buffer, HTTP_SERVER_BufferLen, NETCONN_COPY | NETCONN_MORE))
				return -1;
			Data += DataShift;
			DataLen -= DataShift;
			HTTP_SERVER_BufferLen = 0;
		}
		else
		{
			memcpy(&HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen], Data, DataLen);
			HTTP_SERVER_BufferLen += DataLen;
			Data += DataLen;
			DataLen = 0;
		}
	}
	return 0;
}

int32_t __HTTP_SERVER_FinishBody(struct netconn *Conn)
{
	char ChunkSize[16];
	
	if (HTTP_SERVER_BufferLen)
	{
		snprintf (ChunkSize, sizeof(ChunkSize), "%X\r\n", HTTP_SERVER_BufferLen);
		if (HTTP_SERVER_Write(Conn, ChunkSize, strlen(ChunkSize), NETCONN_COPY | NETCONN_MORE))
			return -1;	
		HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen++] = '\r';
		HTTP_SERVER_Buffer[HTTP_SERVER_BufferLen++] = '\n';
		if (HTTP_SERVER_Write(Conn, HTTP_SERVER_Buffer, HTTP_SERVER_BufferLen, NETCONN_COPY | NETCONN_MORE))
			return -1;
		HTTP_SERVER_BufferLen = 0;
	}	
	strncpy(ChunkSize, "0\r\n\r\n", sizeof(ChunkSize));
	if (HTTP_SERVER_Write(Conn, ChunkSize, strlen(ChunkSize), NETCONN_COPY))
		return - 1;
	return 0;
}

/**
  * @brief serve http Connection  
  * @param Conn: pointer on Connection structure 
  * @retval None
  */
static void HTTP_SERVER_ServeConnection(struct netconn *Conn) 
{
  struct netbuf *Inbuf;
  err_t RecvError;
  char* Data;
  u16_t DataLen;
	HTTP_REQUEST_HEADER_T ReqHeader;

	RecvError = netconn_recv(Conn, &Inbuf);
	if (RecvError == ERR_OK)
	{
		netbuf_data(Inbuf, (void**)&Data, &DataLen);
		if (HTTP_SERVER_ParseRequest(Data, DataLen, &ReqHeader))
		{
			LOG_Error("HTTP", "SERVER", "", "Request header parse error");
			netbuf_delete(Inbuf);
			__HTTP_SERVER_WriteError400(Conn);
			return;
		}
		
		HTTP_SERVER_URL_Process(Conn, Inbuf, &ReqHeader);
	}
	else
	{
		LOG_Error("HTTP", "SERVER", "", "Netconn recv error: %d", RecvError);
	}
}

/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void HTTP_SERVER_Thread(void *arg)
{ 
  struct netconn *Conn, *NewConn;
  err_t Error;
  
  /* Create a new TCP Connection handle */
  Conn = netconn_new(NETCONN_TCP);
  
  if (Conn!= NULL)
  {
    /* Bind to port 80 (HTTP) of any IP address */
    Error = netconn_bind(Conn, NULL, 80);
    
    if (Error == ERR_OK)
    {
      /* Put the Connection into LISTEN state */
      netconn_listen(Conn);
  
      while(1) 
      {
        /* Accept any icoming Connection */
        Error = netconn_accept(Conn, &NewConn);
        if(Error == ERR_OK)
        {
					/* Serve incoming Connection */
					HTTP_SERVER_ServeConnection(NewConn);

					/* Close the Connection*/
					Error = netconn_close(NewConn);
					if(Error != ERR_OK)
						LOG_Error("HTTP", "SERVER", "", "Netconn close error: %d", Error);

					/* delete Connection */
					Error = netconn_delete(NewConn);
					if(Error != ERR_OK)
						LOG_Error("HTTP", "SERVER", "", "Netconn delete error: %d", Error);					
        }
				else
				{
					LOG_Error("HTTP", "SERVER", "", "Netconn accept error: %d", Error);
				}
      }
    }
  }
}
 
void HTTP_SERVER_Init(void)
{
   if (!sys_thread_new("HTTP", HTTP_SERVER_Thread, NULL, HTTP_SERVER_THREAD_STACK_SIZE, HTTP_SERVER_THREAD_PRIO))
		 LOG_Error("HTTP", "SERVER", "", "Init error");
}
