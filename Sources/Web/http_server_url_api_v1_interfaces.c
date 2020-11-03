#include "http_server_url_api_v1_interfaces.h"
#include "bsp.h"
#include "user.h"
#include "base64.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_INTERFACES_TEMPLATE	"/api/v1/interfaces"

static int32_t HTTP_SERVER_URL_API_V1_INTERFACES_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);
static void HTTP_SERVER_URL_API_V1_INTERFACES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_INTERFACES = {
	.Template = HTTP_SERVER_URL_API_V1_INTERFACES_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_INTERFACES_TEMPLATE) - 1,
		.Auth = HTTP_SERVER_URL_API_V1_INTERFACES_Auth,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_INTERFACES_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

static int32_t HTTP_SERVER_URL_API_V1_INTERFACES_Auth(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
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

static void HTTP_SERVER_URL_API_V1_INTERFACES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
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
	
	__HTTP_SERVER_StartBody(Conn);
	__HTTP_SERVER_WriteBody(Conn, "\t\"i2c\" : [\r\n");
#ifdef BSP_CONFIG_USE_I2C1
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"I2C1\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c1.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"berr\" : \"%d\",\r\n", i2c1.berr_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c1.arlo_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"af\" : \"%d\",\r\n", i2c1.af_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", i2c1.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c1.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c1.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c1.seq_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c1.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c1.seq_tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c1.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_I2C2) || defined(BSP_CONFIG_USE_I2C3))
	__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_I2C2
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"I2C2\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c2.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"berr\" : \"%d\",\r\n", i2c2.berr_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c2.arlo_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"af\" : \"%d\",\r\n", i2c2.af_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", i2c2.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c2.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c2.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c2.seq_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c2.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c2.seq_tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c2.message_queue_overflow_errors);
#if defined(BSP_CONFIG_USE_I2C3)
	__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_I2C3
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"I2C3\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c3.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"berr\" : \"%d\",\r\n", i2c3.berr_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c3.arlo_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"af\" : \"%d\",\r\n", i2c3.af_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", i2c3.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c3.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c3.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c3.seq_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c3.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c3.seq_tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c3.message_queue_overflow_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
	__HTTP_SERVER_WriteBody(Conn, "\t],\r\n");
	
	__HTTP_SERVER_WriteBody(Conn, "\t\"spi\" : [\r\n");
#ifdef BSP_CONFIG_USE_SPI1
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"SPI1\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi1.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi1.mode_fault_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi1.crc_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi1.frame_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi1.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi1.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi1.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi1.seq_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi1.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi1.seq_tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi1.tx_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi1.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI2
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"SPI2\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi2.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi2.mode_fault_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi2.crc_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi2.frame_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi2.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi2.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi2.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi2.seq_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi2.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi2.seq_tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi2.tx_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi2.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI3
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"SPI3\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi3.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi3.mode_fault_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi3.crc_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi3.frame_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi3.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi3.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi3.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi3.seq_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi3.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi3.seq_tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi3.tx_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi3.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI4
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"SPI4\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi4.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi4.mode_fault_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi4.crc_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi4.frame_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi4.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi4.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi4.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi4.seq_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi4.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi4.seq_tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi4.tx_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi4.message_queue_overflow_errors);
#if defined(BSP_CONFIG_USE_SPI5)
	__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI5
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"SPI5\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi5.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi5.mode_fault_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi5.crc_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi5.frame_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi5.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi5.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi5.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi5.seq_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi5.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi5.seq_tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi5.tx_rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi5.message_queue_overflow_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
	__HTTP_SERVER_WriteBody(Conn, "\t],\r\n");
	
	__HTTP_SERVER_WriteBody(Conn, "\t\"uart\" : [\r\n");
#ifdef BSP_CONFIG_USE_UART1
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART1\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart1.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart1.parity_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart1.noise_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart1.frame_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart1.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart1.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart1.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart1.rx_cmd_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart1.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart1.rx_message_queue_overflow_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart1.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART2) || defined(BSP_CONFIG_USE_UART3) || defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART2
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART2\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart2.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart2.parity_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart2.noise_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart2.frame_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart2.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart2.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart2.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart2.rx_cmd_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart2.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart2.rx_message_queue_overflow_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart2.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART3) || defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART3
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART3\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart3.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart3.parity_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart3.noise_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart3.frame_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart3.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart3.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart3.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart3.rx_cmd_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart3.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart3.rx_message_queue_overflow_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart3.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART4
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART4\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart4.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart4.parity_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart4.noise_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart4.frame_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart4.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart4.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart4.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart4.rx_cmd_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart4.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart4.rx_message_queue_overflow_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart4.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART5
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART5\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart5.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart5.parity_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart5.noise_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart5.frame_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart5.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart5.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart5.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart5.rx_cmd_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart5.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart5.rx_message_queue_overflow_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart5.tx_message_queue_overflow_errors);
#if defined(BSP_CONFIG_USE_UART6)
	__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n");
#else
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART6
	__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART6\",\r\n");
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart6.overrun_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart6.parity_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart6.noise_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart6.frame_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart6.dma_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart6.timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart6.rx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart6.rx_cmd_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart6.tx_timeout_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart6.rx_message_queue_overflow_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart6.tx_message_queue_overflow_errors);
	__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n");
#endif
	__HTTP_SERVER_WriteBody(Conn, "\t]\r\n");
	__HTTP_SERVER_WriteBody(Conn, "}\r\n");
	__HTTP_SERVER_FinishBody(Conn);
}
