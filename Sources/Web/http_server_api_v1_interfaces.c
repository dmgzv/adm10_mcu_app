#include "http_server_api_v1_interfaces.h"
#include "bsp.h"

#define HTTP_SERVER_API_V1_INTERFACES_URL_TEMPLATE	"/api/v1/interfaces"

static void HTTP_SERVER_API_V1_INTERFACES_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen);

const HTTP_SERVER_URL_T HTTP_SERVER_API_V1_INTERFACES_Url = {
	.Url = HTTP_SERVER_API_V1_INTERFACES_URL_TEMPLATE,
	.UrlSize = sizeof(HTTP_SERVER_API_V1_INTERFACES_URL_TEMPLATE) - 1,
	.HeadProcessing = NULL,
	.GetProcessing = HTTP_SERVER_API_V1_INTERFACES_GetProcessing,
	.PutProcessing = NULL,
	.PostProcessing = NULL,
};

static void HTTP_SERVER_API_V1_INTERFACES_GetProcessing(struct netconn *Conn, struct netbuf *Inbuf, HTTP_REQUEST_HEADER_T *ReqHeader, char* Body, uint16_t BodyLen)
{
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
	if (http_server_write_header(Conn, &RspHeader))
		return;
	
	http_server_write_body_start(Conn);
	http_server_write_body(Conn, "\t\"i2c\" : [\r\n");
#ifdef BSP_CONFIG_USE_I2C1
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"I2C1\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c1.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"berr\" : \"%d\",\r\n", i2c1.berr_errors);
	http_server_write_body(Conn, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c1.arlo_errors);
	http_server_write_body(Conn, "\t\t\t\"af\" : \"%d\",\r\n", i2c1.af_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", i2c1.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c1.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c1.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c1.seq_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c1.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c1.seq_tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c1.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_I2C2) || defined(BSP_CONFIG_USE_I2C3))
	http_server_write_body(Conn, "\t\t},\r\n");
#else
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_I2C2
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"I2C2\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c2.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"berr\" : \"%d\",\r\n", i2c2.berr_errors);
	http_server_write_body(Conn, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c2.arlo_errors);
	http_server_write_body(Conn, "\t\t\t\"af\" : \"%d\",\r\n", i2c2.af_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", i2c2.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c2.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c2.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c2.seq_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c2.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c2.seq_tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c2.message_queue_overflow_errors);
#if defined(BSP_CONFIG_USE_I2C3)
	http_server_write_body(Conn, "\t\t},\r\n");
#else
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_I2C3
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"I2C3\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", i2c3.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"berr\" : \"%d\",\r\n", i2c3.berr_errors);
	http_server_write_body(Conn, "\t\t\t\"arlo\" : \"%d\",\r\n", i2c3.arlo_errors);
	http_server_write_body(Conn, "\t\t\t\"af\" : \"%d\",\r\n", i2c3.af_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", i2c3.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", i2c3.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", i2c3.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", i2c3.seq_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", i2c3.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", i2c3.seq_tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", i2c3.message_queue_overflow_errors);
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
	http_server_write_body(Conn, "\t],\r\n");
	
	http_server_write_body(Conn, "\t\"spi\" : [\r\n");
#ifdef BSP_CONFIG_USE_SPI1
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"SPI1\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi1.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi1.mode_fault_errors);
	http_server_write_body(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi1.crc_errors);
	http_server_write_body(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi1.frame_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi1.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi1.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi1.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi1.seq_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi1.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi1.seq_tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi1.tx_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi1.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	http_server_write_body(Conn, "\t\t},\r\n");
#else
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI2
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"SPI2\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi2.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi2.mode_fault_errors);
	http_server_write_body(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi2.crc_errors);
	http_server_write_body(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi2.frame_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi2.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi2.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi2.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi2.seq_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi2.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi2.seq_tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi2.tx_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi2.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	http_server_write_body(Conn, "\t\t},\r\n");
#else
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI3
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"SPI3\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi3.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi3.mode_fault_errors);
	http_server_write_body(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi3.crc_errors);
	http_server_write_body(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi3.frame_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi3.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi3.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi3.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi3.seq_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi3.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi3.seq_tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi3.tx_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi3.message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5))
	http_server_write_body(Conn, "\t\t},\r\n");
#else
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI4
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"SPI4\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi4.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi4.mode_fault_errors);
	http_server_write_body(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi4.crc_errors);
	http_server_write_body(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi4.frame_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi4.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi4.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi4.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi4.seq_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi4.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi4.seq_tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi4.tx_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi4.message_queue_overflow_errors);
#if defined(BSP_CONFIG_USE_SPI5)
	http_server_write_body(Conn, "\t\t},\r\n");
#else
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_SPI5
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"SPI5\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", spi5.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"mode_fault\" : \"%d\",\r\n", spi5.mode_fault_errors);
	http_server_write_body(Conn, "\t\t\t\"crc\" : \"%d\",\r\n", spi5.crc_errors);
	http_server_write_body(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", spi5.frame_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", spi5.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", spi5.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", spi5.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_rx_timeout\" : \"%d\",\r\n", spi5.seq_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", spi5.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"seq_tx_timeout\" : \"%d\",\r\n", spi5.seq_tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_rx_timeout\" : \"%d\",\r\n", spi5.tx_rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"queue_overflow\" : \"%d\"\r\n", spi5.message_queue_overflow_errors);
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
	http_server_write_body(Conn, "\t],\r\n");
	
	http_server_write_body(Conn, "\t\"uart\" : [\r\n");
#ifdef BSP_CONFIG_USE_UART1
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"UART1\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart1.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart1.parity_errors);
	http_server_write_body(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart1.noise_errors);
	http_server_write_body(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart1.frame_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart1.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart1.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart1.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart1.rx_cmd_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart1.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart1.rx_message_queue_overflow_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart1.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART2) || defined(BSP_CONFIG_USE_UART3) || defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	http_server_write_body(Conn, "\t\t},\r\n");
#else
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART2
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"UART2\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart2.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart2.parity_errors);
	http_server_write_body(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart2.noise_errors);
	http_server_write_body(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart2.frame_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart2.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart2.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart2.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart2.rx_cmd_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart2.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart2.rx_message_queue_overflow_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart2.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART3) || defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	http_server_write_body(Conn, "\t\t},\r\n");
#else
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART3
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"UART3\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart3.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart3.parity_errors);
	http_server_write_body(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart3.noise_errors);
	http_server_write_body(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart3.frame_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart3.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart3.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart3.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart3.rx_cmd_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart3.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart3.rx_message_queue_overflow_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart3.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	http_server_write_body(Conn, "\t\t},\r\n");
#else
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART4
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"UART4\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart4.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart4.parity_errors);
	http_server_write_body(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart4.noise_errors);
	http_server_write_body(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart4.frame_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart4.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart4.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart4.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart4.rx_cmd_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart4.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart4.rx_message_queue_overflow_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart4.tx_message_queue_overflow_errors);
#if (defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6))
	http_server_write_body(Conn, "\t\t},\r\n");
#else
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART5
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"UART5\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart5.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart5.parity_errors);
	http_server_write_body(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart5.noise_errors);
	http_server_write_body(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart5.frame_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart5.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart5.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart5.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart5.rx_cmd_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart5.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart5.rx_message_queue_overflow_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart5.tx_message_queue_overflow_errors);
#if defined(BSP_CONFIG_USE_UART6)
	http_server_write_body(Conn, "\t\t},\r\n");
#else
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
#endif
#ifdef BSP_CONFIG_USE_UART6
	http_server_write_body(Conn, "\t\t{\r\n");
	http_server_write_body(Conn, "\t\t\t\"name\" : \"UART6\",\r\n");
	http_server_write_body(Conn, "\t\t\t\"overrun\" : \"%d\",\r\n", uart6.overrun_errors);
	http_server_write_body(Conn, "\t\t\t\"parity\" : \"%d\",\r\n", uart6.parity_errors);
	http_server_write_body(Conn, "\t\t\t\"noise\" : \"%d\",\r\n", uart6.noise_errors);
	http_server_write_body(Conn, "\t\t\t\"frame\" : \"%d\",\r\n", uart6.frame_errors);
	http_server_write_body(Conn, "\t\t\t\"dma\" : \"%d\",\r\n", uart6.dma_errors);
	http_server_write_body(Conn, "\t\t\t\"timeout\" : \"%d\",\r\n", uart6.timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_timeout\" : \"%d\",\r\n", uart6.rx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_cmd_timeout\" : \"%d\",\r\n", uart6.rx_cmd_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_timeout\" : \"%d\",\r\n", uart6.tx_timeout_errors);
	http_server_write_body(Conn, "\t\t\t\"rx_queue_overflow\" : \"%d\",\r\n", uart6.rx_message_queue_overflow_errors);
	http_server_write_body(Conn, "\t\t\t\"tx_queue_overflow\" : \"%d\"\r\n", uart6.tx_message_queue_overflow_errors);
	http_server_write_body(Conn, "\t\t}\r\n");
#endif
	http_server_write_body(Conn, "\t]\r\n");
	http_server_write_body(Conn, "}\r\n");
	http_server_write_body_finish(Conn);
}
