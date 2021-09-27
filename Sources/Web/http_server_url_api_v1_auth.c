#include "http_server_url_api_v1_auth.h"
#include "user.h"
#include "tiny-json.h"
#include <string.h>
#include "log.h"

#define HTTP_SERVER_URL_API_V1_AUTH_TEMPLATE	"/api/v1/auth"

static void HTTP_SERVER_URL_API_V1_AUTH_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_AUTH = {
	.Template = HTTP_SERVER_URL_API_V1_AUTH_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_AUTH_TEMPLATE) - 1,
	.Head = NULL,
	.Get = NULL,
	.Put = NULL,
	.Post = HTTP_SERVER_URL_API_V1_AUTH_Post,
	.Delete = NULL,
};

static void HTTP_SERVER_URL_API_V1_AUTH_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	char *Data;
	uint16_t DataLen;
	char *Login, *Password;
	json_t mem[4];
	json_t const* json;
	json_t const* jsonProp;
	USER_T User;
	char PassHash[USER_PASSHASH_SIZE];
	uint32_t ContentLength, RecvError;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
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
	
	Login = NULL;
	Password = NULL;
	
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
		
		json = json_create( Data, mem, sizeof mem / sizeof *mem );
    if ( !json ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }
		
		jsonProp = json_getProperty( json, "login" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }
		
		Login = (char*)json_getValue( jsonProp );
		
		jsonProp = json_getProperty( json, "password" );
    if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) ) {
        netbuf_delete(Inbuf);
				goto error_400;
    }
		
		Password = (char*)json_getValue( jsonProp );
		
		if (USER_GetUserByLogin(Login, &User))
		{
			netbuf_delete(Inbuf);
			goto error_400;
		}
		
		if (USER_MD5CRYPT_GenPassHash(Password, User.PassHash, PassHash))
			goto error_400;
		
		if (strcmp(PassHash, User.PassHash))
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
	
	if (__HTTP_SERVER_StartBody(Conn))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\"login\" : \"%s\",\r\n", User.Login))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\"name\" : \"%s\",\r\n", User.Name))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\"tel\" : \"%s\",\r\n", User.Tel))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\"email\" : \"%s\",\r\n", User.Email))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\"group\" : \"%s\"\r\n", User.Group))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "}\r\n"))
		return;
	if (__HTTP_SERVER_FinishBody(Conn))
		return;
	
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "Login", "");
	return;
	
error_400:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, "", "LoginError", "Invalid request");
	__HTTP_SERVER_WriteError400(Conn);
	return;
}
