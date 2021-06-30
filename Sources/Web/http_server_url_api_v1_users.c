#include "http_server_url_api_v1_users.h"
#include "user.h"
#include "tiny-json.h"
#include "base64.h"
#include "log.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_USER_TEMPLATE	"/api/v1/users/*"
#define HTTP_SERVER_URL_API_V1_USERS_TEMPLATE	"/api/v1/users"

static void HTTP_SERVER_URL_API_V1_USER_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_USER_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_USER_Delete(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_USERS_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_USERS_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_USER = {
	.Template = HTTP_SERVER_URL_API_V1_USER_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_USER_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_USER_Get,
	.Put = HTTP_SERVER_URL_API_V1_USER_Put,
	.Post = NULL,
	.Delete = HTTP_SERVER_URL_API_V1_USER_Delete,
};

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_USERS = {
	.Template = HTTP_SERVER_URL_API_V1_USERS_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_USERS_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_USERS_Get,
	.Put = NULL,
	.Post = HTTP_SERVER_URL_API_V1_USERS_Post,
	.Delete = NULL,
};

static void HTTP_SERVER_URL_API_V1_USERS_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	uint32_t i, UsersCnt;
	USER_T User;
	char AuthUserLogin[USER_LOGIN_SIZE];
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	strcpy(AuthUserLogin, User.Login);
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
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
	
	UsersCnt = 0;
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "[\r\n");
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (!USER_GetUserByIndex(i, &User) && (User.Login[0]))
		{
			if (UsersCnt)
				__HTTP_SERVER_WriteBody(Conn, "\t},\r\n");
			__HTTP_SERVER_WriteBody(Conn, "\t{\r\n");
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"login\" : \"%s\",\r\n", User.Login);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"name\" : \"%s\",\r\n", User.Name);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"tel\" : \"%s\",\r\n", User.Tel);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"email\" : \"%s\",\r\n", User.Email);
#ifndef HTTP_SERVER_HTTP_SERVER
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"group\" : \"%s\",\r\n", User.Group);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"passhash\" : \"%s\",\r\n", User.PassHash);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"snmphash\" : \"%s\"\r\n", User.SnmpHash);
#else
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"group\" : \"%s\"\r\n", User.Group);
#endif
			UsersCnt++;
		}
	}
	if (UsersCnt)
		__HTTP_SERVER_WriteBody(Conn, "\t}\r\n");
	__HTTP_SERVER_WriteBody(Conn, "]\r\n");
	__HTTP_SERVER_FinishBody(Conn);
	return;
	
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UsersReadError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_USERS_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *Data, *c;
	uint16_t DataLen;
	uint32_t len;
	json_t jsonPool[16];
	json_t const* json;
	json_t const* jsonProp;
	uint32_t ContentLength, RecvError;
	USER_T User;
	char AuthUserLogin[USER_LOGIN_SIZE];
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	strcpy(AuthUserLogin, User.Login);
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
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
	
	memset(&User, 0, sizeof(User));
	
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
    if ( !json )
		{
       netbuf_delete(Inbuf);
			goto error_400;
    }
		
		jsonProp = json_getProperty( json, "login" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		{
       netbuf_delete(Inbuf);
			goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		len = strlen(c);
		if (len >= sizeof(User.Login))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		strcpy(User.Login, c);
		
		jsonProp = json_getProperty( json, "name" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		{
			netbuf_delete(Inbuf);
			goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		len = strlen(c);
		if (len >= sizeof(User.Name))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		strcpy(User.Name, c);
		
		jsonProp = json_getProperty( json, "password" );
		if (jsonProp)
		{
			if ( JSON_TEXT != json_getType( jsonProp ) )
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}		
			c = (char*)json_getValue( jsonProp );
			len = strlen(c);
			if ((len < 8) || (len > 128))
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}
			if (USER_MD5CRYPT_GenPassHash(c, "bpVng.ZK", User.PassHash))
			{
				netbuf_delete(Inbuf);
				goto error_500;
			}
			if (USER_SNMPCRYPT_GenPassHash(c, User.SnmpHash))
			{
				netbuf_delete(Inbuf);
				goto error_500;
			}
		}
		
		jsonProp = json_getProperty( json, "passhash" );
		if (jsonProp)
		{
			if ( JSON_TEXT != json_getType( jsonProp ) )
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}		
			c = (char*)json_getValue( jsonProp );
			len = strlen(c);
			if (len != (sizeof(User.PassHash) - 1))
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}
			memcpy(User.PassHash, c, sizeof(User.PassHash));
		}
		
		jsonProp = json_getProperty( json, "snmphash" );
		if (jsonProp)
		{
			if ( JSON_TEXT != json_getType( jsonProp ) )
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}		
			c = (char*)json_getValue( jsonProp );
			len = strlen(c);
			if (len != (sizeof(User.SnmpHash) - 1))
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}
			memcpy(User.SnmpHash, c, sizeof(User.SnmpHash));
		}
		
		jsonProp = json_getProperty( json, "tel" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		{
			netbuf_delete(Inbuf);
			goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		len = strlen(c);
		if (len >= sizeof(User.Tel))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		strcpy(User.Tel, c);
		
		jsonProp = json_getProperty( json, "email" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		{
			netbuf_delete(Inbuf);
			goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		len = strlen(c);
		if (len >= sizeof(User.Email))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		strcpy(User.Email, c);
		
		jsonProp = json_getProperty( json, "group" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		{
			netbuf_delete(Inbuf);
			goto error_400;
    }		
		c = (char*)json_getValue( jsonProp );
		len = strlen(c);
		if (len >= sizeof(User.Group))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		strcpy(User.Group, c);
		
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
	RspHeader.ContentType = NULL;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserCreate", "%s", User.Login);
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserCreateError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserCreateError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
error_500:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserCreateError", "Internal error");
	__HTTP_SERVER_WriteError500(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_USER_Put(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *Data, *c, *Login;
	uint16_t DataLen;
	uint32_t len;
	json_t jsonPool[16];
	json_t const* json;
	json_t const* jsonProp;
	USER_T User;
	char AuthUserLogin[USER_LOGIN_SIZE];
	uint32_t ContentLength, RecvError;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	strcpy(AuthUserLogin, User.Login);
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
	Login = &ReqHeader->Url[sizeof("/api/v1/users/") - 1];
	ReqHeader->Url[ReqHeader->UrlSize] = 0;
	if (USER_GetUserByLogin(Login, &User))
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
    if ( !json )
		{
			netbuf_delete(Inbuf);
			goto error_400;
    }
		
		jsonProp = json_getProperty( json, "login" );
    if ( jsonProp )
		{
			if (JSON_TEXT != json_getType( jsonProp ) )
			{
        netbuf_delete(Inbuf);
				goto error_400;
			}
			c = (char*)json_getValue( jsonProp );
			len = strlen(c);
			if (len >= sizeof(User.Login))
			{
        netbuf_delete(Inbuf);
				goto error_400;
			}
			strcpy(User.Login, c);
		}
		
		jsonProp = json_getProperty( json, "name" );
    if ( jsonProp )
		{
			if ( JSON_TEXT != json_getType( jsonProp ) )
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}		
			c = (char*)json_getValue( jsonProp );
			len = strlen(c);
			if (len >= sizeof(User.Name))
			{
        netbuf_delete(Inbuf);
				goto error_400;
			}
			strcpy(User.Name, c);
		}
		
		jsonProp = json_getProperty( json, "password" );
    if ( jsonProp )
		{
			if( JSON_TEXT != json_getType( jsonProp ) )
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}		
			c = (char*)json_getValue( jsonProp );
			len = strlen(c);
			if ((len < 8) || (len > 128))
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}
			if (USER_MD5CRYPT_GenPassHash(c, "bpVng.ZK", User.PassHash))
			{
				netbuf_delete(Inbuf);
				goto error_500;
			}
			if (USER_SNMPCRYPT_GenPassHash(c, User.SnmpHash))
			{
				netbuf_delete(Inbuf);
				goto error_500;
			}
		}
		
		jsonProp = json_getProperty( json, "tel" );
    if ( jsonProp )
		{
			if ( JSON_TEXT != json_getType( jsonProp ) )
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}		
			c = (char*)json_getValue( jsonProp );
			len = strlen(c);
			if (len >= sizeof(User.Tel))
			{
        netbuf_delete(Inbuf);
				goto error_400;
			}
			strcpy(User.Tel, c);
		}
		
		jsonProp = json_getProperty( json, "email" );
    if ( jsonProp )
		{
			if ( JSON_TEXT != json_getType( jsonProp ) )
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}		
			c = (char*)json_getValue( jsonProp );
			len = strlen(c);
			if (len >= sizeof(User.Email))
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}
			strcpy(User.Email, c);
		}
		
		jsonProp = json_getProperty( json, "group" );
    if ( jsonProp )
		{
			if ( JSON_TEXT != json_getType( jsonProp ) )
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}		
			c = (char*)json_getValue( jsonProp );
			len = strlen(c);
			if (len >= sizeof(User.Group))
			{
				netbuf_delete(Inbuf);
				goto error_400;
			}
			strcpy(User.Group, c);
		}
		
		if (USER_UpdateUserByLogin(Login, &User))
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
	RspHeader.ContentType = NULL;
	RspHeader.Charset = HTTP_CHARSET_UTF_8;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserUpdate", "%s", User.Login);
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserUpdateError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserUpdateError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
error_500:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserUpdateError", "Internal error");
	__HTTP_SERVER_WriteError500(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_USER_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	USER_T User;
	char AuthUserLogin[USER_LOGIN_SIZE];
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	strcpy(AuthUserLogin, User.Login);
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
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
#ifndef HTTP_SERVER_HTTP_SERVER
	__HTTP_SERVER_WriteBody(Conn, "\t\"group\" : \"%s\",\r\n", User.Group);
	__HTTP_SERVER_WriteBody(Conn, "\t\"passhash\" : \"%s\",\r\n", User.PassHash);
	__HTTP_SERVER_WriteBody(Conn, "\t\"snmphash\" : \"%s\"\r\n", User.SnmpHash);
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\"group\" : \"%s\"\r\n", User.Group);
#endif
	__HTTP_SERVER_WriteBody(Conn, "}\r\n");
	__HTTP_SERVER_FinishBody(Conn);
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserGetError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserGetError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}

static void HTTP_SERVER_URL_API_V1_USER_Delete(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *c;
	USER_T User;
	char AuthUserLogin[USER_LOGIN_SIZE];
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	strcpy(AuthUserLogin, User.Login);
	
	if ((strcmp(User.Group, USER_GROUP_ADMIN)) && (strcmp(User.Group, USER_GROUP_SUPERUSER)))
	{
		netbuf_delete(Inbuf);
		goto error_403;
	}
	
	c = &ReqHeader->Url[sizeof("/api/v1/users/") - 1];
	ReqHeader->Url[ReqHeader->UrlSize] = 0;
	if (USER_DeleteUserByLogin(c))
	{
		netbuf_delete(Inbuf);
		goto error_400;
	}
	
	netbuf_delete(Inbuf);	
	RspHeader.Code = HTTP_RESPONCE_200_OK;
	RspHeader.ContentType = NULL;
	RspHeader.Charset = NULL;
	RspHeader.ContentLength = 0;
	RspHeader.Connection = HTTP_CONNECTION_CLOSE;
	RspHeader.TransferEncoding = NULL;
	RspHeader.AllowedLen = 0;
	if (__HTTP_SERVER_WriteHeader(Conn, &RspHeader))
		return;
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserDelete", "%s", c);
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserCreateError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, AuthUserLogin, "UserCreateError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}
