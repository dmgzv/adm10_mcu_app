#include "http_server_url_api_v1_interfaces.h"
#include "bsp.h"
#include "user.h"
#include "base64.h"
#include "log.h"
#include <string.h>

#define HTTP_SERVER_URL_API_V1_INTERFACES_TEMPLATE	"/api/v1/interfaces"

static void HTTP_SERVER_URL_API_V1_INTERFACES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader);

const HTTP_SERVER_URL_T HTTP_SERVER_URL_API_V1_INTERFACES = {
	.Template = HTTP_SERVER_URL_API_V1_INTERFACES_TEMPLATE,
	.TemplateSize = sizeof(HTTP_SERVER_URL_API_V1_INTERFACES_TEMPLATE) - 1,
	.Head = NULL,
	.Get = HTTP_SERVER_URL_API_V1_INTERFACES_Get,
	.Put = NULL,
	.Post = NULL,
	.Delete = NULL,
};

static void HTTP_SERVER_URL_API_V1_INTERFACES_Get(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader)
{
	USER_T User;
	HTTP_RESPONCE_HEADER_T RspHeader;
	
	if (__HTTP_SERVER_Authenticate(Conn, Inbuf, ReqHeader, &User))
		return;
	
	if (strcmp(User.Group, USER_GROUP_SUPERUSER))
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
	
	if (__HTTP_SERVER_StartBody(Conn))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\"i2c\" : [\r\n"))
		return;
#ifdef BSP_CONFIG_USE_I2C1
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"I2C1\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c1.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"berr\" : \"%d\",\r\n", i2c1.berr_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c1.arlo_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"af\" : \"%d\",\r\n", i2c1.af_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", i2c1.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c1.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c1.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c1.seq_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c1.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c1.seq_tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c1.message_queue_overflow_errors))
		return;
#if (defined(BSP_CONFIG_USE_I2C2) || defined(BSP_CONFIG_USE_I2C3))
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
		return;
#else
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
#endif
#ifdef BSP_CONFIG_USE_I2C2
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"I2C2\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c2.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"berr\" : \"%d\",\r\n", i2c2.berr_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c2.arlo_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"af\" : \"%d\",\r\n", i2c2.af_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", i2c2.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c2.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c2.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c2.seq_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c2.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c2.seq_tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c2.message_queue_overflow_errors))
		return;
#if defined(BSP_CONFIG_USE_I2C3)
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
		return;
#else
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
#endif
#ifdef BSP_CONFIG_USE_I2C3
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"I2C3\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c3.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"berr\" : \"%d\",\r\n", i2c3.berr_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c3.arlo_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"af\" : \"%d\",\r\n", i2c3.af_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", i2c3.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c3.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c3.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c3.seq_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c3.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c3.seq_tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c3.message_queue_overflow_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
	if (__HTTP_SERVER_WriteBody(Conn, "\t],\r\n"))
		return;
	
	if (__HTTP_SERVER_WriteBody(Conn, "\t\"spi\" : [\r\n"))
		return;
#ifdef BSP_CONFIG_USE_SPI1
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"SPI1\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi1.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi1.mode_fault_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi1.crc_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi1.frame_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi1.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi1.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi1.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi1.seq_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi1.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi1.seq_tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi1.tx_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi1.message_queue_overflow_errors))
		return;
#if (defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
		return;
#else
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI2
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"SPI2\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi2.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi2.mode_fault_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi2.crc_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi2.frame_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi2.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi2.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi2.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi2.seq_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi2.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi2.seq_tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi2.tx_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi2.message_queue_overflow_errors))
		return;
#if (defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
		return;
#else
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI3
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"SPI3\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi3.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi3.mode_fault_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi3.crc_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi3.frame_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi3.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi3.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi3.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi3.seq_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi3.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi3.seq_tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi3.tx_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi3.message_queue_overflow_errors))
		return;
#if (defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
		return;
#else
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI4
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"SPI4\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi4.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi4.mode_fault_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi4.crc_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi4.frame_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi4.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi4.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi4.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi4.seq_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi4.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi4.seq_tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi4.tx_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi4.message_queue_overflow_errors))
		return;
#if defined(BSP_CONFIG_USE_SPI5)
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
		return;
#else
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI5
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"SPI5\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi5.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi5.mode_fault_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi5.crc_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi5.frame_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi5.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi5.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi5.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi5.seq_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi5.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi5.seq_tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi5.tx_rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi5.message_queue_overflow_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
	if (__HTTP_SERVER_WriteBody(Conn, "\t],\r\n"))
		return;
	
	if (__HTTP_SERVER_WriteBody(Conn, "\t\"uart\" : [\r\n"))
		return;
#ifdef BSP_CONFIG_USE_UART1
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART1\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart1.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart1.parity_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart1.noise_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart1.frame_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart1.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart1.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart1.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart1.rx_cmd_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart1.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart1.rx_message_queue_overflow_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart1.tx_message_queue_overflow_errors))
		return;
#if (defined(BSP_CONFIG_USE_UART2) || defined(BSP_CONFIG_USE_UART3) || defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
		return;
#else
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
#endif
#ifdef BSP_CONFIG_USE_UART2
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART2\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart2.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart2.parity_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart2.noise_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart2.frame_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart2.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart2.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart2.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart2.rx_cmd_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart2.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart2.rx_message_queue_overflow_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart2.tx_message_queue_overflow_errors))
		return;
#if (defined(BSP_CONFIG_USE_UART3) || defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
		return;
#else
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
#endif
#ifdef BSP_CONFIG_USE_UART3
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART3\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart3.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart3.parity_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart3.noise_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart3.frame_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart3.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart3.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart3.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart3.rx_cmd_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart3.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart3.rx_message_queue_overflow_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart3.tx_message_queue_overflow_errors))
		return;
#if (defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
		return;
#else
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
#endif
#ifdef BSP_CONFIG_USE_UART4
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART4\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart4.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart4.parity_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart4.noise_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart4.frame_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart4.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart4.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart4.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart4.rx_cmd_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart4.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart4.rx_message_queue_overflow_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart4.tx_message_queue_overflow_errors))
		return;
#if (defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
		return;
#else
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
#endif
#ifdef BSP_CONFIG_USE_UART5
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART5\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart5.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart5.parity_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart5.noise_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart5.frame_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart5.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart5.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart5.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart5.rx_cmd_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart5.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart5.rx_message_queue_overflow_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart5.tx_message_queue_overflow_errors))
		return;
#if defined(BSP_CONFIG_USE_UART6)
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t},\r\n"))
		return;
#else
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
#endif
#ifdef BSP_CONFIG_USE_UART6
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t{\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"name\" : \"UART6\",\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart6.overrun_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart6.parity_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart6.noise_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart6.frame_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart6.dma_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart6.timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart6.rx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart6.rx_cmd_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart6.tx_timeout_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart6.rx_message_queue_overflow_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart6.tx_message_queue_overflow_errors))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "\t\t}\r\n"))
		return;
#endif
	if (__HTTP_SERVER_WriteBody(Conn, "\t]\r\n"))
		return;
	if (__HTTP_SERVER_WriteBody(Conn, "}\r\n"))
		return;
	if (__HTTP_SERVER_FinishBody(Conn))
		return;
	return;
	
error_403:
	__LOG_Add("HTTP_SERVER", LOG_ACCESS_ADMIN, LOG_SEVERITY_INFO, User.Login, "InterfacesReadError", "Access denied");
	__HTTP_SERVER_WriteError403(Conn);
	return;
}
