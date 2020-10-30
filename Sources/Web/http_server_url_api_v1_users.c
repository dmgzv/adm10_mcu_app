#include "http_server_url_api_v1_users.h"
#include "user.h"
#include "tiny-json.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_USERS_TEMPLATE	"/api/v1/users"

static void HTTP_SERVER_URL_API_V1_USERS_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_USERS_Post(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_USERS = {
	.Template = HTTP_SERVER_URL_API_V1_USERS_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_USERS_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_USERS_Get,
	.Put = NULL,
	.Post = HTTP_SERVER_URL_API_V1_USERS_Post,
};

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
	
	UsersCnt = USER_GetUsersCnt();
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "[\r\n");
	for (i = 0; i < UsersCnt; i++)
	{
		if (!USER_GetUser(i, &User))
		{
			__HTTP_SERVER_WriteBody(Conn, "\t{\r\n");
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"login\" : \"%s\",\r\n", User.Login);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"name\" : \"%s\",\r\n", User.Name);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"tel\" : \"+%s\",\r\n", User.Tel);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"email\" : \"%s\",\r\n", User.Email);
			__HTTP_SERVER_WriteBody(Conn, "\t\t\"role\" : \"%s\"\r\n", User.Role);	
			if (i == (UsersCnt - 1))
				__HTTP_SERVER_WriteBody(Conn, "\t}\r\n");
			else
				__HTTP_SERVER_WriteBody(Conn, "\t},\r\n");
		}
	}
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
	__HTTP_SERVER_WriteBody(Conn, "\t\"tel\" : \"+%s\",\r\n", User.Tel);
	__HTTP_SERVER_WriteBody(Conn, "\t\"email\" : \"%s\",\r\n", User.Email);
	__HTTP_SERVER_WriteBody(Conn, "\t\"role\" : \"%s\"\r\n", User.Role);
	__HTTP_SERVER_WriteBody(Conn, "}\r\n");
	__HTTP_SERVER_FinishBody(Conn);
	
	return;
	
error_400:
	__HTTP_SERVER_WriteError400(Conn);
	return;
}
