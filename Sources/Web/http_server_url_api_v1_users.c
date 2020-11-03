#include "http_server_url_api_v1_users.h"
#include "user.h"
#include "tiny-json.h"
#include "base64.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_USER_TEMPLATE	"/api/v1/users/*"
#define HTTP_SERVER_URL_API_V1_USERS_TEMPLATE	"/api/v1/users"

static int32_t HTTP_SERVER_URL_API_V1_USERS_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

static void HTTP_SERVER_URL_API_V1_USER_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_USER_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_USER_Delete(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_USERS_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_USERS_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_USER = {
	.Template = HTTP_SERVER_URL_API_V1_USER_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_USER_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_USERS_Auth,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_USER_Get,
	.Put = HTTP_SERVER_URL_API_V1_USER_Put,
	.Post = NULL,
	.Delete = HTTP_SERVER_URL_API_V1_USER_Delete,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_USERS = {
	.Template = HTTP_SERVER_URL_API_V1_USERS_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_USERS_TEMPLATE) - 1,
	.Auth = HTTP_SERVER_URL_API_V1_USERS_Auth,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_USERS_Get,
	.Put = NULL,
	.Post = HTTP_SERVER_URL_API_V1_USERS_Post,
	.Delete = NULL,
};

static int32_t HTTP_SERVER_URL_API_V1_USERS_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
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

static void HTTP_SERVER_URL_API_V1_USERS_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i, UsersCnt;
	USER_T User;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
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
	
	UsersCnt = 0;
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "[\r\n");
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (!USER_GetUserByIndex(i, &User))
		{
			if (UsersCnt)
				__HTTP_SERVER_WriteBody(Conn, "\t},\r\n");
			__HTTP_SERVER_WriteBody(Conn, "\t{\r\n");
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"login\" : \"%s\",\r\n", User.Login);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"name\" : \"%s\",\r\n", User.Name);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"tel\" : \"%s\",\r\n", User.Tel);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"email\" : \"%s\",\r\n", User.Email);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"role\" : \"%s\"\r\n", User.Role);
			UsersCnt++;

		}
	}
	if (UsersCnt)
		__HTTP_SERVER_WriteBody(Conn, "\t}\r\n");
	__HTTP_SERVER_WriteBody(Conn, "]\r\n");
	__HTTP_SERVER_FinishBody(Conn);
}

static void HTTP_SERVER_URL_API_V1_USERS_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *Data, *c;
	uint16_t DataLen;
	json_t jsonPool[16];
	json_t const* json;
	json_t const* jsonProp;
	USER_T User;
	uint32_t ContentLength, RecvError;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
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
	
	if (ReqHeader->Body)
	{
		Data = ReqHeader->Body;
		DataLen = ReqHeader->BodyLen;
		goto parse_body;
	}
	netbuf_delete(Inbuf);
	
	RecvError = netconn_recv(Conn, &Inbuf);
	if (RecvError == ERR_OK)
	{
		netbuf_data(Inbuf, (void**)&Data, &DataLen);
parse_body:
		
		json = json_create( Data, jsonPool, sizeof (jsonPool) / sizeof (jsonPool[0]) );
    if ( !json ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }
		
		jsonProp = json_getProperty( json, "login" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Login, c, sizeof(User.Login));
		
		jsonProp = json_getProperty( json, "name" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Name, c, sizeof(User.Name));
		
		jsonProp = json_getProperty( json, "password" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Password, c, sizeof(User.Password));
		
		jsonProp = json_getProperty( json, "tel" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Tel, c, sizeof(User.Tel));
		
		jsonProp = json_getProperty( json, "email" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Email, c, sizeof(User.Email));
		
		jsonProp = json_getProperty( json, "role" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Role, c, sizeof(User.Role));
		
		if (USER_CreateUser(&User))
		{
        netbuf_delete(Inbuf);
				goto error_400;
    }
		
		netbuf_delete(Inbuf);
	}
	else
	{
		goto error_400;
	}
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\"login\" : \"%s\",\r\n", User.Login);
	__HTTP_SERVER_WriteBody(Conn, "\t\"name\" : \"%s\",\r\n", User.Name);
	__HTTP_SERVER_WriteBody(Conn, "\t\"tel\" : \"%s\",\r\n", User.Tel);
	__HTTP_SERVER_WriteBody(Conn, "\t\"email\" : \"%s\",\r\n", User.Email);
	__HTTP_SERVER_WriteBody(Conn, "\t\"role\" : \"%s\"\r\n", User.Role);
	__HTTP_SERVER_WriteBody(Conn, "}\r\n");
	__HTTP_SERVER_FinishBody(Conn);
	
	return;
	
error_400:
	__HTTP_SERVER_WriteError400(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_USER_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *Data, *c;
	uint16_t DataLen;
	json_t jsonPool[16];
	json_t const* json;
	json_t const* jsonProp;
	USER_T User;
	uint32_t ContentLength, RecvError;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	c = &ReqHeader->Url[sizeof("/api/v1/users/") - 1];
	ReqHeader->Url[ReqHeader->UrlSize] = 0;
	if (USER_GetUserByLogin(c, &User))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
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
	
	if (ReqHeader->Body)
	{
		Data = ReqHeader->Body;
		DataLen = ReqHeader->BodyLen;
		goto parse_body;
	}
	netbuf_delete(Inbuf);
	
	RecvError = netconn_recv(Conn, &Inbuf);
	if (RecvError == ERR_OK)
	{
		netbuf_data(Inbuf, (void**)&Data, &DataLen);
parse_body:
		
		json = json_create( Data, jsonPool, sizeof (jsonPool) / sizeof (jsonPool[0]) );
    if ( !json ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }
		
		jsonProp = json_getProperty( json, "login" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Login, c, sizeof(User.Login));
		
		jsonProp = json_getProperty( json, "name" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Name, c, sizeof(User.Name));
		
		jsonProp = json_getProperty( json, "password" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Password, c, sizeof(User.Password));
		
		jsonProp = json_getProperty( json, "tel" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Tel, c, sizeof(User.Tel));
		
		jsonProp = json_getProperty( json, "email" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Email, c, sizeof(User.Email));
		
		jsonProp = json_getProperty( json, "role" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		strncpy(User.Role, c, sizeof(User.Role));
		
		if (USER_UpdateUserByLogin(c, &User))
		{
        netbuf_delete(Inbuf);
				goto error_400;
    }
		
		netbuf_delete(Inbuf);
	}
	else
	{
		goto error_400;
	}
	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = HTTP_TRANSFER_ENCODING_CHUNKED;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\"login\" : \"%s\",\r\n", User.Login);
	__HTTP_SERVER_WriteBody(Conn, "\t\"name\" : \"%s\",\r\n", User.Name);
	__HTTP_SERVER_WriteBody(Conn, "\t\"tel\" : \"%s\",\r\n", User.Tel);
	__HTTP_SERVER_WriteBody(Conn, "\t\"email\" : \"%s\",\r\n", User.Email);
	__HTTP_SERVER_WriteBody(Conn, "\t\"role\" : \"%s\"\r\n", User.Role);
	__HTTP_SERVER_WriteBody(Conn, "}\r\n");
	__HTTP_SERVER_FinishBody(Conn);
	
	return;
	
error_400:
	__HTTP_SERVER_WriteError400(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_USER_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	USER_T User;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	c = &ReqHeader->Url[sizeof("/api/v1/users/") - 1];
	ReqHeader->Url[ReqHeader->UrlSize] = 0;
	if (USER_GetUserByLogin(c, &User))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
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
	
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\"login\" : \"%s\",\r\n", User.Login);
	__HTTP_SERVER_WriteBody(Conn, "\t\"name\" : \"%s\",\r\n", User.Name);
	__HTTP_SERVER_WriteBody(Conn, "\t\"tel\" : \"%s\",\r\n", User.Tel);
	__HTTP_SERVER_WriteBody(Conn, "\t\"email\" : \"%s\",\r\n", User.Email);
	__HTTP_SERVER_WriteBody(Conn, "\t\"role\" : \"%s\"\r\n", User.Role);
	__HTTP_SERVER_WriteBody(Conn, "}\r\n");
	__HTTP_SERVER_FinishBody(Conn);
	
	
error_400:
	__HTTP_SERVER_WriteError400(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_USER_Delete(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	HTTP_HTTP_SERVER_Buffer_T RspHeader;
	
	c = &ReqHeader->Url[sizeof("/api/v1/users/") - 1];
	ReqHeader->Url[ReqHeader->UrlSize] = 0;
	if (USER_DeleteUserByLogin(c))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	netbuf_delete(Inbuf);	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = HTTP_CONTENT_TYPE_APP_JSON;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	
error_400:
	__HTTP_SERVER_WriteError400(Conn);
	return;
}
