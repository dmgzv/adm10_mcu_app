#include "http_server.h"
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define HTTP_SERVER_THREAD_STACK_SIZE  ( 512 )
#define HTTP_SERVER_THREAD_PRIO    ( osPriorityAboveNormal )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static const char http_200_ok[] = \
	"HTTP/1.1 200 OK\r\n"\
	"\r\n";
 
static const char http_html_hdr[] = \
	"HTTP/1.1 200 OK\r\n"\
	"Content-type: text/html\r\n"\
	"\r\n";
 
static const char http_css_hdr[] = \
	"HTTP/1.1 200 OK\r\n"\
	"Content-type: text/css\r\n"\
	"\r\n";
 
static const char http_font_woff2_hdr[] = \
	"HTTP/1.1 200 OK\r\n"\
	"Content-type: font/woff2\r\n"\
	"\r\n";
 
static const char http_js_hdr[] = \
	"HTTP/1.1 200 OK\r\n"\
	"Content-type: text/javascript\r\n"\
	"\r\n";
 
static const char http_plain_hdr[] = \
	"HTTP/1.1 200 OK\r\n"\
	"Content-type: text/plain\r\n"\
	"\r\n";
 
static const char http_json_hdr[] = \
	"HTTP/1.1 200 OK\r\n"\
	"Content-type: application/json\r\n"\
	"\r\n";
 
static const char http_404_err[] = \
	"HTTP/1.1 404 Not Found\r\n"\
	"\r\n";
 
static const char http_400_err[] = \
	"HTTP/1.1 400 Bad Request\r\n"\
	"\r\n";

extern const char index_html[];
extern const unsigned int index_html_size;
extern const char css_bootstrap_min_css[];
extern const unsigned int css_bootstrap_min_css_size;
extern const char css_style_css[];
extern const unsigned int css_style_css_size;
/*extern const char css_font_awesome_min_css[];
extern const unsigned int css_font_awesome_min_css_size;
extern const char fonts_fontawesome_webfont_woff2[];
extern const unsigned int fonts_fontawesome_webfont_woff2_size;*/
extern const char js_script_js[];
extern const unsigned int js_script_js_size;
extern const char js_jquery_3_4_1_slim_min_js[];
extern const unsigned int js_jquery_3_4_1_slim_min_js_size;
extern const char js_popper_min_js[];
extern const unsigned int js_popper_min_js_size;
extern const char js_bootstrap_min_js[];
extern const unsigned int js_bootstrap_min_js_size;

extern PROFILE_T Profile;
extern BOOTLOADER_T Bootloader;
extern PROFILE_PROTOCOL_T ProfileProtocol;

static char REQUEST_BODY[PROFILE_PROTOCOL_CMD_MAX_SIZE];
static uint32_t REQUEST_BODY_LEN;
static char RESPONCE_BODY[PROFILE_PROTOCOL_RSP_MAX_SIZE];
static uint32_t RESPONCE_BODY_LEN;

static int32_t http_server_write(struct netconn *conn, const void *dataptr, size_t size, u8_t apiflags)
{
	err_t write_err;
	while (1)
	{
		if(conn->state == NETCONN_NONE) // check the state
		{
			write_err = netconn_write(conn, dataptr, size, apiflags);
			if (write_err != ERR_OK)
				return -1;
			osDelay(1);
			break;
		}
		osDelay(1);
	}
	return 0;
}

static void http_server_get_body(char *data, uint32_t datalen, char **body, uint16_t *bodylen)
{
	uint32_t i;

	for (i = 0; i < (datalen - 4); i++)
	{
		if ((data[i] == '\r') && (data[i + 1] == '\n') && (data[i + 2] == '\r') && (data[i + 3] == '\n'))
		{
			*body = &data[i + 4];
			*bodylen = datalen - (i + 4);
			return;
		}
	}
	*body = NULL;
	*bodylen = 0;
}

static void http_server_serve_400_error(struct netconn *conn)
{
	if (http_server_write(conn, http_400_err, sizeof(http_400_err) - 1, NETCONN_NOCOPY))
		return;
}
 
static void http_server_serve_404_error(struct netconn *conn)
{
	if (http_server_write(conn, http_404_err, sizeof(http_404_err) - 1, NETCONN_NOCOPY))
		return;
}

static void http_server_serve_get_index_html(struct netconn *conn)
{
	if (http_server_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, index_html, index_html_size, NETCONN_NOCOPY))
		return;
}
 
static void http_server_serve_get_css_bootstrap_min_css(struct netconn *conn)
{
	if (http_server_write(conn, http_css_hdr, sizeof(http_css_hdr) - 1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, css_bootstrap_min_css, css_bootstrap_min_css_size, NETCONN_NOCOPY))
		return;
}

static void http_server_serve_get_css_style_css(struct netconn *conn)
{
	if (http_server_write(conn, http_css_hdr, sizeof(http_css_hdr) - 1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, css_style_css, css_style_css_size, NETCONN_NOCOPY))
		return;
}
/*
static void http_server_serve_get_css_font_awesome_min_css(struct netconn *conn)
{
	if (http_server_write(conn, http_css_hdr, sizeof(http_css_hdr) - 1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, css_font_awesome_min_css, css_font_awesome_min_css_size, NETCONN_NOCOPY))
		return;
}
 
static void http_server_serve_get_fonts_fontawesome_webfont_woff2(struct netconn *conn)
{
	if (http_server_write(conn, http_font_woff2_hdr, sizeof(http_font_woff2_hdr) - 1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, fonts_fontawesome_webfont_woff2, fonts_fontawesome_webfont_woff2_size, NETCONN_NOCOPY))
		return;
}
*/
static void http_server_serve_get_js_jquery_3_4_1_slim_min_js(struct netconn *conn)
{
	if (http_server_write(conn, http_js_hdr, sizeof(http_js_hdr) - 1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, js_jquery_3_4_1_slim_min_js, js_jquery_3_4_1_slim_min_js_size, NETCONN_NOCOPY))
		return;
}
 
static void http_server_serve_get_js_popper_min_js(struct netconn *conn)
{
	if (http_server_write(conn, http_js_hdr, sizeof(http_js_hdr) - 1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, js_popper_min_js, js_popper_min_js_size, NETCONN_NOCOPY))
		return;
}
 
static void http_server_serve_get_js_bootstrap_min_js(struct netconn *conn)
{
	if (http_server_write(conn, http_js_hdr, sizeof(http_js_hdr) - 1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, js_bootstrap_min_js, js_bootstrap_min_js_size, NETCONN_NOCOPY))
		return;
}
 
static void http_server_serve_get_js_script_js(struct netconn *conn)
{
	if (http_server_write(conn, http_js_hdr, sizeof(http_js_hdr) - 1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, js_script_js, js_script_js_size, NETCONN_NOCOPY))
		return;
}

/**
  * @brief  Send profile. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_profile(struct netconn *conn)
{
	uint32_t i;
	uint32_t len;
  /* Send the dynamically generated page */
	if (http_server_write(conn, http_plain_hdr, sizeof(http_plain_hdr)-1, NETCONN_NOCOPY))
		return;
	
	RESPONCE_BODY_LEN = 0;
	
	for (i = 0; i < *Profile.Descr[Profile.Idx]->TextSize; i++)
	{
		len = strlen(Profile.Descr[Profile.Idx]->Text[i]) + 1;
		
		memcpy(&RESPONCE_BODY[RESPONCE_BODY_LEN], Profile.Descr[Profile.Idx]->Text[i], len);
		RESPONCE_BODY_LEN += len;
		if (RESPONCE_BODY_LEN > 1024)
		{
			if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_MORE | NETCONN_COPY))
				return;
			RESPONCE_BODY_LEN = 0;
			osDelay(1);
		}
	}
	if (RESPONCE_BODY_LEN)
	{
		if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_MORE | NETCONN_COPY))
			return;
	}
	RESPONCE_BODY_LEN = 0;
}

/**
  * @brief  Create and send profile section values. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_profile_section_values(struct netconn *conn, uint32_t section_idx)
{
	char *body;
	uint32_t i, body_len;
	uint32_t complete;
	uint32_t header_len;
	
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
	body_len = RESPONCE_BODY_LEN - header_len - 1;
	
	/*Send responce*/
	if (http_server_write(conn, http_plain_hdr, sizeof(http_plain_hdr)-1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, body, body_len, NETCONN_COPY))
		return;
	
	return;

error:
	if (http_server_write(conn, http_400_err, sizeof(http_400_err)-1, NETCONN_NOCOPY))
		return;
}

/**
  * @brief  Create and send profile section values. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_profile_section_colors(struct netconn *conn, uint32_t SectionId)
{
	char *body;
	uint32_t i, body_len;
	uint32_t complete;
	uint32_t header_len;
	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_GET_COLORS;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d;", SectionId);
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
	body_len = RESPONCE_BODY_LEN - header_len - 1;
	
	/*Send responce*/
	if (http_server_write(conn, http_plain_hdr, sizeof(http_plain_hdr)-1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, body, body_len, NETCONN_COPY))
		return;
	
	return;

error:
	if (http_server_write(conn, http_400_err, sizeof(http_400_err)-1, NETCONN_NOCOPY))
		return;
}

/**
  * @brief  Set profile section parameter value. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
static void http_server_serve_put_api_v1_profile_section_param_value(struct netconn *conn, uint32_t SectionId, uint32_t ParamId, char *body, uint16_t body_len)
{
	uint32_t i;
	uint32_t complete;
	uint32_t header_len;

	if ((!body) || (!body_len))
	{
		http_server_serve_400_error(conn);
		return;
	}	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_SET_VALUE_2;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d;%d;", SectionId, ParamId);
	header_len = REQUEST_BODY_LEN - 1;
	memcpy(&REQUEST_BODY[REQUEST_BODY_LEN], body, body_len);
	REQUEST_BODY_LEN += body_len;
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
	
	for (i = 0; i < body_len; i++)
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
	
	body = &RESPONCE_BODY[header_len + 1];
	body_len = RESPONCE_BODY_LEN - header_len - 1;
	
	/*Send responce*/
	if (http_server_write(conn, http_plain_hdr, sizeof(http_plain_hdr)-1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, body, body_len, NETCONN_COPY))
		return;
	
	return;

error:
	if (http_server_write(conn, http_400_err, sizeof(http_400_err)-1, NETCONN_NOCOPY))
		return;
}

/**
  * @brief  Set profile section parameter value. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
static void http_server_serve_put_api_v1_profile_section_param_color(struct netconn *conn, uint32_t SectionId, uint32_t ParamId, char *body, uint16_t body_len)
{
	uint32_t i;
	uint32_t complete;
	uint32_t header_len;

	if ((!body) || (!body_len))
	{
		http_server_serve_400_error(conn);
		return;
	}
	
	/*Prepare request*/
	REQUEST_BODY[0] = SlotAddress + 0x80;
	REQUEST_BODY[1] = PROFILE_PROTOCOL_CMD_SET_COLOR;
	REQUEST_BODY_LEN = 2;
	REQUEST_BODY_LEN += sprintf(&REQUEST_BODY[REQUEST_BODY_LEN], "%d;%d;", SectionId, ParamId);
	header_len = REQUEST_BODY_LEN - 1;
	memcpy(&REQUEST_BODY[REQUEST_BODY_LEN], body, body_len);
	REQUEST_BODY_LEN += body_len;
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
	
	body = &RESPONCE_BODY[header_len + 1];
	body_len = RESPONCE_BODY_LEN - header_len - 1;
	
	/*Send responce*/
	if (http_server_write(conn, http_plain_hdr, sizeof(http_plain_hdr)-1, NETCONN_NOCOPY))
		return;
	if (http_server_write(conn, body, body_len, NETCONN_COPY))
		return;
	
	return;

error:
	if (http_server_write(conn, http_400_err, sizeof(http_400_err)-1, NETCONN_NOCOPY))
		return;
}

/**
  * @brief  Create and send system data. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_system_os(struct netconn *conn)
{
	char *thread = &RESPONCE_BODY[sizeof(RESPONCE_BODY) - 512];
	char *p, *s;
	snprintf (RESPONCE_BODY, sizeof(RESPONCE_BODY), "{\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN,	"\t\"heap\" : \"%d\",\r\n", xPortGetFreeHeapSize());
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);	
  /* The list of tasks and their status */
	osThreadList((unsigned char *)(thread));
  snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"threads\" : [\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	while (*thread) {
		snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n");
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
		s = thread;
		p = strchr(s, '\t');
		*p++ = 0;
		snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"name\" : \"%s\",\r\n", s);
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
		s = p;
		p = strchr(s, '\t');
		*p++ = 0;
		snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"state\" : \"%s\",\r\n", s);
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
		s = p;
		p = strchr(s, '\t');
		*p = 0;
		snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"priority\" : \"%s\",\r\n", s);
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
		s = p + 1;
		p = strchr(s, '\t');
		*p++ = 0;
		snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"stack\" : \"%s\",\r\n", s);
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
		s = p;
		p = strchr(s, '\r');
		*p++ = 0;
		*p++ = 0;
		snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t\"number\" : \"%s\"\r\n", s);
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
		thread = p;
		if (*thread)
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t},\r\n");
		else
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t}\r\n");	
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	}
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t]\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "}\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);	

  /* Send the dynamically generated page */
	netconn_write(conn, http_json_hdr, sizeof(http_json_hdr)-1, NETCONN_NOCOPY);
  netconn_write(conn, RESPONCE_BODY, strlen(RESPONCE_BODY), NETCONN_COPY);
}

/**
  * @brief  Create and send system data. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
void http_server_serve_get_api_v1_system_hardware(struct netconn *conn)
{
	uint32_t i;
	snprintf (RESPONCE_BODY, sizeof(RESPONCE_BODY), "{\r\n\t\"power\" : [\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	for (i = 0; i < UCD90120A_RAILS_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK)
			break;
		if (i != (UCD90120A_RAILS_CNT - 1))
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n\t\t\t\"config\" : \"%.3f\",\r\n\t\t\t\"monitor\" : \"%.3f\"\r\n\t\t},\r\n", ucd90120a.Cfg.Rail[i].VoutCmd, ucd90120a.Mon.Rail[i].Vout);
		else
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n\t\t\t\"config\" : \"%.3f\",\r\n\t\t\t\"monitor\" : \"%.3f\"\r\n\t\t}\r\n", ucd90120a.Cfg.Rail[i].VoutCmd, ucd90120a.Mon.Rail[i].Vout);
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	}
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t],\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\"ic\" : {\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : {\r\n\t\t\t\"id\" : \"%s\",\r\n\t\t\t\"status\" : \"%s\"\r\n\t\t},\r\n", at45db641e.DevName, at45db641e.DevId, BSP_DEV_GET_STATE_STR(&at45db641e));
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : {\r\n\t\t\t\"id\" : \"%s\",\r\n\t\t\t\"status\" : \"%s\"\r\n\t\t},\r\n", dim3r3400.DevName, dim3r3400.DevId, BSP_DEV_GET_STATE_STR(&dim3r3400));
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : {\r\n\t\t\t\"id\" : \"%s\",\r\n\t\t\t\"status\" : \"%s\"\r\n\t\t},\r\n", ucd90120a.DevName, ucd90120a.DevId, BSP_DEV_GET_STATE_STR(&ucd90120a));
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : {\r\n\t\t\t\"id\" : \"%s\",\r\n\t\t\t\"status\" : \"%s\"\r\n\t\t},\r\n", pwr_pca9536.DevName, pwr_pca9536.DevId, BSP_DEV_GET_STATE_STR(&pwr_pca9536));
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : {\r\n\t\t\t\"id\" : \"%s\",\r\n\t\t\t\"status\" : \"%s\"\r\n\t\t},\r\n", ksz8895.DevName, ksz8895.DevId, BSP_DEV_GET_STATE_STR(&ksz8895));
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	
	/*
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : [\r\n", gdt080a[0].DevName);
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	for (i = 0; i < BSP_FPGA_PWR_GDT080A_CNT; i++)
	{
		if (i != (BSP_FPGA_PWR_GDT080A_CNT - 1))
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t},\r\n", gdt080a[i].DevId, BSP_DEV_GET_STATE_STR(&gdt080a[i]));
		else
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t}\r\n", gdt080a[i].DevId, BSP_DEV_GET_STATE_STR(&gdt080a[i]));
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	}
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t],\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	*/
	
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : [\r\n", si5332[0].DevName);
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	for (i = 0; i < BSP_CLK_SI5332_CNT; i++)
	{
		if (i != (BSP_CLK_SI5332_CNT - 1))
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t},\r\n", si5332[i].DevId, BSP_DEV_GET_STATE_STR(&si5332[i]));
		else
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t}\r\n", si5332[i].DevId, BSP_DEV_GET_STATE_STR(&si5332[i]));
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	}
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t],\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : {\r\n\t\t\t\"id\" : \"%s\",\r\n\t\t\t\"status\" : \"%s\"\r\n\t\t},\r\n", fpga_maxv.DevName, fpga_maxv.DevId, BSP_DEV_GET_STATE_STR(&fpga_maxv));
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : {\r\n\t\t\t\"id\" : \"%s\",\r\n\t\t\t\"status\" : \"%s\"\r\n\t\t},\r\n", fpga_line.DevName, fpga_line.DevId, BSP_DEV_GET_STATE_STR(&fpga_line));
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : [\r\n", fpga_client[0].DevName);
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	for (i = 0; i < BSP_FPGA_CLIENT_CNT; i++)
	{
		if (i != (BSP_FPGA_CLIENT_CNT - 1))
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t},\r\n", fpga_client[i].DevId, BSP_DEV_GET_STATE_STR(&fpga_client[i]));
		else
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t}\r\n", fpga_client[i].DevId, BSP_DEV_GET_STATE_STR(&fpga_client[i]));
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	}
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t],\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : [\r\n", pca9548a[0].DevName);
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	for (i = 0; i < BSP_PCA9548_CNT; i++)
	{
		if (i != (BSP_PCA9548_CNT - 1))
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t},\r\n", pca9548a[i].DevId, BSP_DEV_GET_STATE_STR(&pca9548a[i]));
		else
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t}\r\n", pca9548a[i].DevId, BSP_DEV_GET_STATE_STR(&pca9548a[i]));
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	}
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t],\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : [\r\n", pcal6524[0].DevName);
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	for (i = 0; i < BSP_SFP_PIN_PCAL6524_CNT; i++)
	{
		if (i != (BSP_SFP_PIN_PCAL6524_CNT - 1))
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t},\r\n", pcal6524[i].DevId, BSP_DEV_GET_STATE_STR(&pcal6524[i]));
		else
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t}\r\n", pcal6524[i].DevId, BSP_DEV_GET_STATE_STR(&pcal6524[i]));
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	}
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t],\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : [\r\n", pca9536[0].DevName);
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	for (i = 0; i < BSP_SFP_PIN_PCA9536_CNT; i++)
	{
		if (i != (BSP_SFP_PIN_PCA9536_CNT - 1))
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t},\r\n", pca9536[i].DevId, BSP_DEV_GET_STATE_STR(&pca9536[i]));
		else
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t}\r\n", pca9536[i].DevId, BSP_DEV_GET_STATE_STR(&pca9536[i]));
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	}
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t],\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\"%s\" : [\r\n", sfp[0].DevName);
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	for (i = 0; i < BSP_SFP_CNT; i++)
	{
		if (i != (BSP_SFP_CNT - 1))
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t},\r\n", sfp[i].DevId, BSP_DEV_GET_STATE_STR(&sfp[i]));
		else
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t\t {\r\n\t\t\t\t\"id\" : \"%s\",\r\n\t\t\t\t\"status\" : \"%s\"\r\n\t\t\t}\r\n", sfp[i].DevId, BSP_DEV_GET_STATE_STR(&sfp[i]));
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	}
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t]\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t}\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "}\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);

  /* Send the dynamically generated page */
	netconn_write(conn, http_json_hdr, sizeof(http_json_hdr)-1, NETCONN_NOCOPY);
  netconn_write(conn, RESPONCE_BODY, strlen(RESPONCE_BODY), NETCONN_COPY);
}

/**
  * @brief  Create and send a log data.  
  * @param  conn pointer on connection structure 
  * @retval None
  */
static void http_server_serve_get_api_v1_log(struct netconn *conn)
{
	uint32_t i, LinesCnt, LinesIdx, LineTime;
	float Time;
	const char *Line;
	const char *Name;
	const char *Id;
  /* Send the dynamically generated page */
	if (http_server_write(conn, http_json_hdr, sizeof(http_json_hdr)-1, NETCONN_NOCOPY))
		 return;
	LinesCnt = LOG_GetLinesCnt();
	snprintf (RESPONCE_BODY, sizeof(RESPONCE_BODY), "{\r\n\t\"max-records-cnt\" : \"%d\",\r\n\t\"records-cnt\" : \"%d\",\r\n\t\"records\" : [\r\n", LOG_LINES_CNT, LinesCnt);
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	if (LinesCnt > LOG_LINES_CNT)
	{
		LinesIdx = LinesCnt % LOG_LINES_CNT;
		LinesCnt = LOG_LINES_CNT;
	}
	else
	{
		LinesIdx = 0;
	}
	for (i = 0; i < LinesCnt; i++)
	{
		LOG_GetLine((i + LinesIdx) % LinesCnt, &LineTime, &Name, &Id, &Line);
		Time = (float)LineTime / osKernelSysTickFrequency;
		if (i == (LinesCnt - 1))
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n\t\t\t\"time\" : \"%.4f\",\r\n\t\t\t\"name\" : \"%s\",\r\n\t\t\t\"id\" : \"%s\",\r\n\t\t\t\"data\" : \"%s\"\r\n\t\t}\r\n", Time, Name, Id, Line);
		else
			snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t\t{\r\n\t\t\t\"time\" : \"%.4f\",\r\n\t\t\t\"name\" : \"%s\",\r\n\t\t\t\"id\" : \"%s\",\r\n\t\t\t\"data\" : \"%s\"\r\n\t\t},\r\n", Time, Name, Id, Line);
		RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
		if (RESPONCE_BODY_LEN > 1024)
		{
			if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY | NETCONN_MORE))
				return;
			RESPONCE_BODY_LEN = 0;
			osDelay(1);
		}
	}
	snprintf (&RESPONCE_BODY[RESPONCE_BODY_LEN], sizeof(RESPONCE_BODY) - RESPONCE_BODY_LEN, "\t]\r\n}\r\n");
	RESPONCE_BODY_LEN = strlen(RESPONCE_BODY);
	if (http_server_write(conn, RESPONCE_BODY, RESPONCE_BODY_LEN, NETCONN_COPY))
		return;
	RESPONCE_BODY_LEN = 0;
}

/**
  * @brief  Create and send a log data.  
  * @param  conn pointer on connection structure 
  * @retval None
  */
static void http_server_serve_post_api_v1_system_update_software(struct netconn *conn, char *data, uint16_t datalen)
{
	struct netbuf *inbuf;
	err_t recv_err;
	char *p;
	uint32_t ContentLength;
	
	p = strstr(data, "Content-Length: ");
	if (!p)
		goto error;
	
	p += sizeof("Content-Length: ") - 1;
	ContentLength = strtoul(p, &p, 10);
	BOOTLOADER_UpdateNewFwStart(&Bootloader);
	while(ContentLength) {
		recv_err = netconn_recv(conn, &inbuf);
		if (recv_err == ERR_OK)
		{
			netbuf_data(inbuf, (void**)&data, &datalen);
			if (BOOTLOADER_UpdateNewFwBlock(&Bootloader, data, datalen))
				goto error;
			ContentLength -= datalen;
			netbuf_delete(inbuf);
		}
		else
		{
			goto error;
		}
	}
	if(BOOTLOADER_UpdateNewFwComplete(&Bootloader))
		goto error;
	
	http_server_write(conn, http_200_ok, sizeof(http_200_ok)-1, NETCONN_NOCOPY);
	netconn_close(conn);
	netconn_delete(conn);
	osDelay(100);
	BSP_Reset();
	
	return;
	
error:
	if (http_server_write(conn, http_400_err, sizeof(http_400_err)-1, NETCONN_NOCOPY))
		return;
}

/**
  * @brief serve tcp connection  
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
	char *c;
  
	/* Read the data from the port, blocking if nothing yet there. 
	   We assume the request (the part we care about) is in one netbuf */
	recv_err = netconn_recv(conn, &inbuf);
	
	if (recv_err == ERR_OK)
	{
		netbuf_data(inbuf, (void**)&data, &datalen);

		/* Is this an HTTP GET command? (only check the first 5 chars, since
		   there are other formats for GET, and we're keeping it very simple )*/
		if ((datalen >=5) && (strncmp(data, "GET /", sizeof("GET /") - 1) == 0))
		{
			if (strncmp(data, "GET /index.html", sizeof("GET /index.html") - 1) == 0)
			{
				http_server_serve_get_index_html(conn);
			}
			else if (strncmp(data, "GET /css/bootstrap.min.css", sizeof("GET /css/bootstrap.min.css") - 1) == 0)
			{
				http_server_serve_get_css_bootstrap_min_css(conn);
			}
			else if (strncmp(data, "GET /css/style.css", sizeof("GET /css/style.css") - 1) == 0)
			{
				http_server_serve_get_css_style_css(conn);
			}
			/*
			else if (strncmp(data, "GET /css/font-awesome.min.css", sizeof("GET /css/font-awesome.min.css") - 1) == 0)
			{
				http_server_serve_get_css_font_awesome_min_css(conn);
			}
			else if (strncmp(data, "GET /fonts/fontawesome-webfont.woff2", sizeof("GET /fonts/fontawesome-webfont.woff2") - 1) == 0)
			{
				http_server_serve_get_fonts_fontawesome_webfont_woff2(conn);
			}
			*/
			else if (strncmp(data, "GET /js/jquery-3.4.1.slim.min.js", sizeof("GET /js/jquery-3.4.1.slim.min.js") - 1) == 0)
			{
				http_server_serve_get_js_jquery_3_4_1_slim_min_js(conn);
			}
			else if (strncmp(data, "GET /js/popper.min.js", sizeof("GET /js/popper.min.js") - 1) == 0)
			{
				http_server_serve_get_js_popper_min_js(conn);
			}
			else if (strncmp(data, "GET /js/bootstrap.min.js", sizeof("GET /js/bootstrap.min.js") - 1) == 0)
			{
				http_server_serve_get_js_bootstrap_min_js(conn);
			}
			else if (strncmp(data, "GET /js/script.js", sizeof("GET /js/script.js") - 1) == 0)
			{
				http_server_serve_get_js_script_js(conn);
			}
			else if (strncmp(data, "GET /api/v1/log", sizeof("GET /api/v1/log") - 1) == 0)
			{
				http_server_serve_get_api_v1_log(conn);
			}
			else if (strncmp(data, "GET /api/v1/system/hardware", sizeof("GET /api/v1/system/hardware") - 1) == 0)
			{
				http_server_serve_get_api_v1_system_hardware(conn);
			}
			else if (strncmp(data, "GET /api/v1/system/os", sizeof("GET /api/v1/system/os") - 1) == 0)
			{
				http_server_serve_get_api_v1_system_os(conn);
			}
			else if (strncmp(data, "GET /api/v1/profile/sections/", sizeof("GET /api/v1/profile/sections/") - 1) == 0)
			{
				section_idx = strtoul(&data[sizeof("GET /api/v1/profile/sections/") - 1], &c, 10);
				if (strncmp(c, "/values", sizeof("/values") - 1) == 0)
					http_server_serve_get_api_v1_profile_section_values(conn, section_idx);
				else if (strncmp(c, "/colors", sizeof("/colors") - 1) == 0)
					http_server_serve_get_api_v1_profile_section_colors(conn, section_idx);
				else
					http_server_serve_404_error(conn);
			}
			else if (strncmp(data, "GET /api/v1/profile", sizeof("GET /api/v1/profile") - 1) == 0)
			{
				http_server_serve_get_api_v1_profile(conn);
			}
			else
			{
				http_server_serve_404_error(conn);
			}
		}      
		else if (strncmp(data, "PUT /", sizeof("PUT /") - 1) == 0)
		{
			if (strncmp(data, "PUT /api/v1/profile/sections/", sizeof("PUT /api/v1/profile/sections/") - 1) == 0)
			{
				section_idx = strtoul(&data[sizeof("GET /api/v1/profile/sections/") - 1], &c, 10);
				if (strncmp(c, "/params/", sizeof("/params/") - 1) == 0)
				{
					param_idx = strtoul(&c[sizeof("/params/") - 1], &c, 10);
					if (strncmp(c, "/value", sizeof("/value") - 1) == 0)
					{
						http_server_get_body(data, datalen, &body, &bodylen);
						http_server_serve_put_api_v1_profile_section_param_value(conn, section_idx, param_idx, body, bodylen); 
					}
					else if (strncmp(c, "/colors", sizeof("/colors") - 1) == 0)
					{
						http_server_get_body(data, datalen, &body, &bodylen);
						http_server_serve_put_api_v1_profile_section_param_color(conn, section_idx, param_idx, body, bodylen); 
					}
					else
					{
						http_server_serve_404_error(conn);
					}
				}
				else
				{
					http_server_serve_404_error(conn);
				}				           
			}
			else
			{
				http_server_serve_404_error(conn);
			}
		}
		else if (strncmp(data, "POST /", sizeof("POST /") - 1) == 0)
		{
			if (strncmp(data, "POST /api/v1/system/update/software", sizeof("POST /api/v1/system/update/software") - 1) == 0)
			{
				http_server_serve_post_api_v1_system_update_software(conn, data, datalen);
			}
			else
			{
				http_server_serve_404_error(conn);
			}
		}
		else
		{
				http_server_serve_404_error(conn);
		}
		netbuf_delete(inbuf);
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
