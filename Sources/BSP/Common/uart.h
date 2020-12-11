#ifndef __UART_H
#define __UART_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "stm32f4xx_hal.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

/* Definition for USART1 */
#define UART1_TX_PIN        GPIO_PIN_9
#define UART1_TX_GPIO_PORT  GPIOA  
#define UART1_TX_AF         GPIO_AF7_USART1
#define UART1_RX_PIN        GPIO_PIN_10
#define UART1_RX_GPIO_PORT  GPIOA 
#define UART1_RX_AF         GPIO_AF7_USART1

#define UART1_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART1_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART1_CLK_ENABLE()      				__HAL_RCC_USART1_CLK_ENABLE()
#define UART1_DMA_CLK_ENABLE()      		__HAL_RCC_DMA2_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define UART1_DMA_IRQ_PRIORITY           (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#define UART1_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#else
#define UART1_DMA_IRQ_PRIORITY           (0)
#define UART1_IRQ_PRIORITY               (0)
#endif

#define UART1_BAUDRATE			(115200)
#define UART1_WORDLENGTH 		UART_WORDLENGTH_8B
#define UART1_STOPBITS 			UART_STOPBITS_1
#define UART1_PARITY 				UART_PARITY_NONE
#define UART1_HWCONTROL 		UART_HWCONTROL_NONE
#define UART1_MODE 					UART_MODE_TX_RX
#define UART1_OVERSAMPLING 	UART_OVERSAMPLING_16

#define UART1_TX_DMA_CHANNEL            DMA_CHANNEL_4
#define UART1_TX_DMA_STREAM             DMA2_Stream7
#define UART1_RX_DMA_CHANNEL            DMA_CHANNEL_4
#define UART1_RX_DMA_STREAM             DMA2_Stream5

#define UART1_DMA_TX_IRQn               DMA2_Stream7_IRQn
#define UART1_DMA_RX_IRQn               DMA2_Stream5_IRQn
#define UART1_DMA_TX_IRQHandler         DMA2_Stream7_IRQHandler
#define UART1_DMA_RX_IRQHandler         DMA2_Stream5_IRQHandler

/* Definition for USART2 */
#define UART2_TX_PIN        GPIO_PIN_5
#define UART2_TX_GPIO_PORT  GPIOD  
#define UART2_TX_AF         GPIO_AF7_USART2
#define UART2_RX_PIN        GPIO_PIN_6
#define UART2_RX_GPIO_PORT  GPIOD 
#define UART2_RX_AF         GPIO_AF7_USART2

#define UART2_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define UART2_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE()
#define UART2_CLK_ENABLE()      				__HAL_RCC_USART2_CLK_ENABLE()
#define UART2_DMA_CLK_ENABLE()      		__HAL_RCC_DMA1_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define UART2_DMA_IRQ_PRIORITY           (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#define UART2_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#else
#define UART2_DMA_IRQ_PRIORITY           (0)
#define UART2_IRQ_PRIORITY               (0)
#endif

#define UART2_BAUDRATE			(115200)
#define UART2_WORDLENGTH 		UART_WORDLENGTH_8B
#define UART2_STOPBITS 			UART_STOPBITS_1
#define UART2_PARITY 				UART_PARITY_NONE
#define UART2_HWCONTROL 		UART_HWCONTROL_NONE
#define UART2_MODE 					UART_MODE_TX_RX
#define UART2_OVERSAMPLING 	UART_OVERSAMPLING_16

#define UART2_TX_DMA_CHANNEL            DMA_CHANNEL_4
#define UART2_TX_DMA_STREAM             DMA1_Stream6
#define UART2_RX_DMA_CHANNEL            DMA_CHANNEL_4
#define UART2_RX_DMA_STREAM             DMA1_Stream5

#define UART2_DMA_TX_IRQn               DMA1_Stream6_IRQn
#define UART2_DMA_RX_IRQn               DMA1_Stream5_IRQn
#define UART2_DMA_TX_IRQHandler         DMA1_Stream6_IRQHandler
#define UART2_DMA_RX_IRQHandler         DMA1_Stream5_IRQHandler

/* Definition for USART3 */
#define UART3_TX_PIN        GPIO_PIN_10
#define UART3_TX_GPIO_PORT  GPIOB  
#define UART3_TX_AF         GPIO_AF7_USART3
#define UART3_RX_PIN        GPIO_PIN_11
#define UART3_RX_GPIO_PORT  GPIOB 
#define UART3_RX_AF         GPIO_AF7_USART3

#define UART3_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define UART3_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 
#define UART3_CLK_ENABLE()      				__HAL_RCC_USART3_CLK_ENABLE()
#define UART3_DMA_CLK_ENABLE()      		__HAL_RCC_DMA1_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define UART3_DMA_IRQ_PRIORITY           (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#define UART3_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#else
#define UART3_DMA_IRQ_PRIORITY           (0)
#define UART3_IRQ_PRIORITY               (0)
#endif

#define UART3_BAUDRATE			(115300)
#define UART3_WORDLENGTH 		UART_WORDLENGTH_8B
#define UART3_STOPBITS 			UART_STOPBITS_1
#define UART3_PARITY 				UART_PARITY_NONE
#define UART3_HWCONTROL 		UART_HWCONTROL_NONE
#define UART3_MODE 					UART_MODE_TX_RX
#define UART3_OVERSAMPLING 	UART_OVERSAMPLING_16

#define UART3_TX_DMA_CHANNEL            DMA_CHANNEL_4
#define UART3_TX_DMA_STREAM             DMA1_Stream3
#define UART3_RX_DMA_CHANNEL            DMA_CHANNEL_4
#define UART3_RX_DMA_STREAM             DMA1_Stream1

#define UART3_DMA_TX_IRQn               DMA1_Stream3_IRQn
#define UART3_DMA_RX_IRQn               DMA1_Stream1_IRQn
#define UART3_DMA_TX_IRQHandler         DMA1_Stream3_IRQHandler
#define UART3_DMA_RX_IRQHandler         DMA1_Stream1_IRQHandler

/* Definition for UART4 */
#define UART4_TX_PIN        GPIO_PIN_10
#define UART4_TX_GPIO_PORT  GPIOC  
#define UART4_TX_AF         GPIO_AF8_UART4
#define UART4_RX_PIN        GPIO_PIN_11
#define UART4_RX_GPIO_PORT  GPIOC 
#define UART4_RX_AF         GPIO_AF8_UART4

#define UART4_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART4_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART4_CLK_ENABLE()      				__HAL_RCC_UART4_CLK_ENABLE()
#define UART4_DMA_CLK_ENABLE()      		__HAL_RCC_DMA1_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define UART4_DMA_IRQ_PRIORITY           (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#define UART4_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#else
#define UART4_DMA_IRQ_PRIORITY           (0)
#define UART4_IRQ_PRIORITY               (0)
#endif

#define UART4_BAUDRATE			(115200)
#define UART4_WORDLENGTH 		UART_WORDLENGTH_8B
#define UART4_STOPBITS 			UART_STOPBITS_1
#define UART4_PARITY 				UART_PARITY_NONE
#define UART4_HWCONTROL 		UART_HWCONTROL_NONE
#define UART4_MODE 					UART_MODE_TX_RX
#define UART4_OVERSAMPLING 	UART_OVERSAMPLING_16

#define UART4_TX_DMA_CHANNEL            DMA_CHANNEL_4
#define UART4_TX_DMA_STREAM             DMA1_Stream4
#define UART4_RX_DMA_CHANNEL            DMA_CHANNEL_4
#define UART4_RX_DMA_STREAM             DMA1_Stream2

#define UART4_DMA_TX_IRQn               DMA1_Stream4_IRQn
#define UART4_DMA_RX_IRQn               DMA1_Stream2_IRQn
#define UART4_DMA_TX_IRQHandler         DMA1_Stream4_IRQHandler
#define UART4_DMA_RX_IRQHandler         DMA1_Stream2_IRQHandler

/* Definition for UART5 */
#define UART5_TX_PIN        GPIO_PIN_12
#define UART5_TX_GPIO_PORT  GPIOC  
#define UART5_TX_AF         GPIO_AF8_UART5
#define UART5_RX_PIN        GPIO_PIN_2
#define UART5_RX_GPIO_PORT  GPIOD 
#define UART5_RX_AF         GPIO_AF8_UART5

#define UART5_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART5_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOD_CLK_ENABLE() 
#define UART5_CLK_ENABLE()      				__HAL_RCC_UART5_CLK_ENABLE()
#define UART5_DMA_CLK_ENABLE()      		__HAL_RCC_DMA1_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define UART5_DMA_IRQ_PRIORITY           (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#define UART5_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#else
#define UART5_IRQ_PRIORITY               (0)
#endif

#define UART5_BAUDRATE			(115200)
#define UART5_WORDLENGTH 		UART_WORDLENGTH_8B
#define UART5_STOPBITS 			UART_STOPBITS_1
#define UART5_PARITY 				UART_PARITY_NONE
#define UART5_HWCONTROL 		UART_HWCONTROL_NONE
#define UART5_MODE 					UART_MODE_TX_RX
#define UART5_OVERSAMPLING 	UART_OVERSAMPLING_16

#define UART5_TX_DMA_CHANNEL            DMA_CHANNEL_4
#define UART5_TX_DMA_STREAM             DMA1_Stream7
#define UART5_RX_DMA_CHANNEL            DMA_CHANNEL_4
#define UART5_RX_DMA_STREAM             DMA1_Stream0

#define UART5_DMA_TX_IRQn               DMA1_Stream7_IRQn
#define UART5_DMA_RX_IRQn               DMA1_Stream0_IRQn
#define UART5_DMA_TX_IRQHandler         DMA1_Stream7_IRQHandler
#define UART5_DMA_RX_IRQHandler         DMA1_Stream0_IRQHandler

/* Definition for UART6 */
#define UART6_TX_PIN        GPIO_PIN_6
#define UART6_TX_GPIO_PORT  GPIOC  
#define UART6_TX_AF         GPIO_AF8_USART6
#define UART6_RX_PIN        GPIO_PIN_7
#define UART6_RX_GPIO_PORT  GPIOC 
#define UART6_RX_AF         GPIO_AF8_USART6

#define UART6_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART6_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()
#define UART6_CLK_ENABLE()      				__HAL_RCC_USART6_CLK_ENABLE()
#define UART6_DMA_CLK_ENABLE()      		__HAL_RCC_DMA2_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define UART6_DMA_IRQ_PRIORITY           (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#define UART6_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 3)
#else
#define UART6_IRQ_PRIORITY               (0)
#endif

#define UART6_BAUDRATE			(115200)
#define UART6_WORDLENGTH 		UART_WORDLENGTH_8B
#define UART6_STOPBITS 			UART_STOPBITS_1
#define UART6_PARITY 				UART_PARITY_NONE
#define UART6_HWCONTROL 		UART_HWCONTROL_NONE
#define UART6_MODE 					UART_MODE_TX_RX
#define UART6_OVERSAMPLING 	UART_OVERSAMPLING_16

#define UART6_TX_DMA_CHANNEL            DMA_CHANNEL_5
#define UART6_TX_DMA_STREAM             DMA2_Stream6
#define UART6_RX_DMA_CHANNEL            DMA_CHANNEL_5
#define UART6_RX_DMA_STREAM             DMA2_Stream1

#define UART6_DMA_TX_IRQn               DMA2_Stream6_IRQn
#define UART6_DMA_RX_IRQn               DMA2_Stream1_IRQn
#define UART6_DMA_TX_IRQHandler         DMA2_Stream6_IRQHandler
#define UART6_DMA_RX_IRQHandler         DMA2_Stream1_IRQHandler

typedef struct {	
	UART_HandleTypeDef handle;
	DMA_HandleTypeDef hdma_tx;
  DMA_HandleTypeDef hdma_rx;
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexId rx_mutex_id;
	osMessageQId rx_message_queue_id;
	uint32_t rx_message_queue_overflow_errors;
	uint8_t *rx_buffer;
	uint32_t rx_buffer_size;
	uint32_t rx_buffer_idx;
	uint8_t *rx_buffer_end;
	
	osMutexId tx_mutex_id;
	osMessageQId tx_message_queue_id;
	uint32_t tx_message_queue_overflow_errors;

	uint32_t timeout_errors;
	uint32_t rx_timeout_errors;
	uint32_t rx_cmd_timeout_errors;
	uint32_t tx_timeout_errors;
#endif /*BSP_CONFIG_USE_FREERTOS*/
	uint32_t overrun_errors;
	uint32_t parity_errors;
	uint32_t noise_errors;
	uint32_t frame_errors;
	uint32_t dma_errors;
} UART_T;

#ifdef BSP_CONFIG_USE_UART1
extern UART_T uart1;
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART2
extern UART_T uart2;
#endif /*BSP_CONFIG_USE_UART2*/

#ifdef BSP_CONFIG_USE_UART3
extern UART_T uart3;
#endif /*BSP_CONFIG_USE_UART3*/

#ifdef BSP_CONFIG_USE_UART4
extern UART_T uart4;
#endif /*BSP_CONFIG_USE_UART4*/

#ifdef BSP_CONFIG_USE_UART5
extern UART_T uart5;
#endif /*BSP_CONFIG_USE_UART5*/

#ifdef BSP_CONFIG_USE_UART6
extern UART_T uart6;
#endif /*BSP_CONFIG_USE_UART6*/

int32_t UART_Init(void);
void UART_Deinit(void);
int32_t UART_Transmit(USART_TypeDef * Instance, char *Buffer, uint32_t  Size);
int32_t UART_Receive(USART_TypeDef * Instance, char *Buffer, uint32_t  Size);
int32_t UART_ReceiveCommand(USART_TypeDef *Instance, char *Buffer, uint32_t Size, char End, uint32_t *Len, uint32_t Timeout);

int32_t UART_DMA_Transmit(USART_TypeDef * Instance, char *Buffer, uint32_t  Size);

#endif /* __UART_H */
