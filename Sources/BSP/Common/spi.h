#ifndef __SPI_H
#define __SPI_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "stm32f4xx_hal.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

/* Definition for SPI1 Pins */
#define SPI1_SCK_PIN                     GPIO_PIN_5
#define SPI1_SCK_GPIO_PORT               GPIOA
#define SPI1_SCK_AF                      GPIO_AF5_SPI1
#define SPI1_MISO_PIN                    GPIO_PIN_6
#define SPI1_MISO_GPIO_PORT              GPIOA
#define SPI1_MISO_AF                     GPIO_AF5_SPI1
#define SPI1_MOSI_PIN                    GPIO_PIN_5
#define SPI1_MOSI_GPIO_PORT              GPIOB
#define SPI1_MOSI_AF                     GPIO_AF5_SPI1

#define SPI1_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPI1_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE() 
#define SPI1_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define SPI1_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#else
#define SPI1_IRQ_PRIORITY               0
#endif

#define SPI1_BAUDRATEPRESCALER					SPI_BAUDRATEPRESCALER_64
#define SPI1_DIRECTION									SPI_DIRECTION_2LINES
#define SPI1_PHASE											SPI_PHASE_2EDGE
#define SPI1_POLARITY										SPI_POLARITY_HIGH
#define SPI1_CRCCALCULATION							SPI_CRCCALCULATION_DISABLE
#define SPI1_CRCPOLYNOMIAL							(7)
#define SPI1_DATASIZE										SPI_DATASIZE_8BIT
#define SPI1_FIRSTBIT										SPI_FIRSTBIT_MSB
#define SPI1_NSS												SPI_NSS_SOFT
#define SPI1_TIMODE											SPI_TIMODE_DISABLE
#define	SPI1_MODE												SPI_MODE_MASTER

/* Definition for SPI2 Pins */
#define SPI2_SCK_PIN                     GPIO_PIN_13
#define SPI2_SCK_GPIO_PORT               GPIOB
#define SPI2_SCK_AF                      GPIO_AF5_SPI2
#define SPI2_MISO_PIN                    GPIO_PIN_14
#define SPI2_MISO_GPIO_PORT              GPIOB
#define SPI2_MISO_AF                     GPIO_AF5_SPI2
#define SPI2_MOSI_PIN                    GPIO_PIN_15
#define SPI2_MOSI_GPIO_PORT              GPIOB
#define SPI2_MOSI_AF                     GPIO_AF5_SPI2

#define SPI2_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define SPI2_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE() 
#define SPI2_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define SPI2_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#else
#define SPI2_IRQ_PRIORITY               0
#endif

#define SPI2_BAUDRATEPRESCALER					SPI_BAUDRATEPRESCALER_32
#define SPI2_DIRECTION									SPI_DIRECTION_2LINES
#define SPI2_PHASE											SPI_PHASE_2EDGE
#define SPI2_POLARITY										SPI_POLARITY_HIGH
#define SPI2_CRCCALCULATION							SPI_CRCCALCULATION_DISABLE
#define SPI2_CRCPOLYNOMIAL							(7)
#define SPI2_DATASIZE										SPI_DATASIZE_8BIT
#define SPI2_FIRSTBIT										SPI_FIRSTBIT_MSB
#define SPI2_NSS												SPI_NSS_SOFT
#define SPI2_TIMODE											SPI_TIMODE_DISABLE
#define	SPI2_MODE												SPI_MODE_MASTER

/* Definition for SPI3 Pins */
#define SPI3_SCK_PIN                     GPIO_PIN_10
#define SPI3_SCK_GPIO_PORT               GPIOC
#define SPI3_SCK_AF                      GPIO_AF6_SPI3
#define SPI3_MISO_PIN                    GPIO_PIN_11
#define SPI3_MISO_GPIO_PORT              GPIOC
#define SPI3_MISO_AF                     GPIO_AF6_SPI3
#define SPI3_MOSI_PIN                    GPIO_PIN_12
#define SPI3_MOSI_GPIO_PORT              GPIOC
#define SPI3_MOSI_AF                     GPIO_AF6_SPI3

#define SPI3_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPI3_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE() 
#define SPI3_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define SPI3_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#else
#define SPI3_IRQ_PRIORITY               0
#endif

#define SPI3_BAUDRATEPRESCALER					SPI_BAUDRATEPRESCALER_32
#define SPI3_DIRECTION									SPI_DIRECTION_2LINES
#define SPI3_PHASE											SPI_PHASE_2EDGE
#define SPI3_POLARITY										SPI_POLARITY_HIGH
#define SPI3_CRCCALCULATION							SPI_CRCCALCULATION_DISABLE
#define SPI3_CRCPOLYNOMIAL							(7)
#define SPI3_DATASIZE										SPI_DATASIZE_8BIT
#define SPI3_FIRSTBIT										SPI_FIRSTBIT_MSB
#define SPI3_NSS												SPI_NSS_SOFT
#define SPI3_TIMODE											SPI_TIMODE_DISABLE
#define	SPI3_MODE												SPI_MODE_MASTER

/* Definition for SPI4 Pins */
#define SPI4_SCK_PIN                     GPIO_PIN_10
#define SPI4_SCK_GPIO_PORT               GPIOC
#define SPI4_SCK_AF                      GPIO_AF6_SPI4
#define SPI4_MISO_PIN                    GPIO_PIN_11
#define SPI4_MISO_GPIO_PORT              GPIOC
#define SPI4_MISO_AF                     GPIO_AF6_SPI4
#define SPI4_MOSI_PIN                    GPIO_PIN_12
#define SPI4_MOSI_GPIO_PORT              GPIOC
#define SPI4_MOSI_AF                     GPIO_AF6_SPI4

#define SPI4_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define SPI4_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE() 
#define SPI4_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOC_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define SPI4_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#else
#define SPI4_IRQ_PRIORITY               0
#endif

#define SPI4_BAUDRATEPRESCALER					SPI_BAUDRATEPRESCALER_42
#define SPI4_DIRECTION									SPI_DIRECTION_2LINES
#define SPI4_PHASE											SPI_PHASE_2EDGE
#define SPI4_POLARITY										SPI_POLARITY_HIGH
#define SPI4_CRCCALCULATION							SPI_CRCCALCULATION_DISABLE
#define SPI4_CRCPOLYNOMIAL							(7)
#define SPI4_DATASIZE										SPI_DATASIZE_8BIT
#define SPI4_FIRSTBIT										SPI_FIRSTBIT_MSB
#define SPI4_NSS												SPI_NSS_SOFT
#define SPI4_TIMODE											SPI_TIMODE_DISABLE
#define	SPI4_MODE												SPI_MODE_MASTER

/* Definition for SPI5 Pins */
#define SPI5_SCK_PIN                     GPIO_PIN_7
#define SPI5_SCK_GPIO_PORT               GPIOF
#define SPI5_SCK_AF                      GPIO_AF5_SPI5
#define SPI5_MISO_PIN                    GPIO_PIN_8
#define SPI5_MISO_GPIO_PORT              GPIOF
#define SPI5_MISO_AF                     GPIO_AF5_SPI5
#define SPI5_MOSI_PIN                    GPIO_PIN_9
#define SPI5_MOSI_GPIO_PORT              GPIOF
#define SPI5_MOSI_AF                     GPIO_AF5_SPI5

#define SPI5_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()
#define SPI5_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE() 
#define SPI5_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOF_CLK_ENABLE()
#define SPI5_DMA_CLK_ENABLE()            __HAL_RCC_DMA2_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define SPI5_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#define SPI5_DMA_TX_IRQ_PRIORITY        (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#define SPI5_DMA_RX_IRQ_PRIORITY        (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#else
#define SPI5_IRQ_PRIORITY               0
#define SPI5_DMA_TX_IRQ_PRIORITY        0
#define SPI5_DMA_RX_IRQ_PRIORITY        0
#endif

#define SPI5_BAUDRATEPRESCALER					SPI_BAUDRATEPRESCALER_128
#define SPI5_DIRECTION									SPI_DIRECTION_2LINES
#define SPI5_PHASE											SPI_PHASE_2EDGE
#define SPI5_POLARITY										SPI_POLARITY_HIGH
#define SPI5_CRCCALCULATION							SPI_CRCCALCULATION_DISABLE
#define SPI5_CRCPOLYNOMIAL							(7)
#define SPI5_DATASIZE										SPI_DATASIZE_8BIT
#define SPI5_FIRSTBIT										SPI_FIRSTBIT_MSB
#define SPI5_NSS												SPI_NSS_SOFT
#define SPI5_TIMODE											SPI_TIMODE_DISABLE
#define	SPI5_MODE												SPI_MODE_MASTER

/* Definition for SPI5's DMA */
#define SPI5_TX_DMA_CHANNEL              DMA_CHANNEL_2
#define SPI5_TX_DMA_STREAM               DMA2_Stream4
#define SPI5_RX_DMA_CHANNEL              DMA_CHANNEL_2
#define SPI5_RX_DMA_STREAM               DMA2_Stream3

/* Definition for SPI5's NVIC */
#define SPI5_DMA_TX_IRQn                 DMA2_Stream4_IRQn
#define SPI5_DMA_RX_IRQn                 DMA2_Stream3_IRQn
#define SPI5_DMA_TX_IRQHandler           DMA2_Stream4_IRQHandler
#define SPI5_DMA_RX_IRQHandler           DMA2_Stream3_IRQHandler

typedef struct {
	void (*SetState)(void *Priv, uint32_t State);
	void *Priv;
} SPI_CS_T;

typedef struct {
	SPI_HandleTypeDef handle;
	DMA_HandleTypeDef hdmatx;
  DMA_HandleTypeDef hdmarx;
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexId mutex_id;
	osMessageQId message_queue_id;
	uint32_t message_queue_overflow_errors;
	uint32_t timeout_errors;
	uint32_t rx_timeout_errors;
	uint32_t tx_timeout_errors;
	uint32_t seq_rx_timeout_errors;
	uint32_t seq_tx_timeout_errors;
	uint32_t tx_rx_timeout_errors;
#endif /*BSP_CONFIG_USE_FREERTOS*/
	uint32_t mode_fault_errors;
	uint32_t crc_errors;
	uint32_t frame_errors;
	uint32_t overrun_errors;
	uint32_t dma_errors;
} SPI_T;

#ifdef BSP_CONFIG_USE_SPI1
extern SPI_T spi1;
#endif /*BSP_CONFIG_USE_SPI1*/

#ifdef BSP_CONFIG_USE_SPI2
extern SPI_T spi2;
#endif /*BSP_CONFIG_USE_SPI2*/

#ifdef BSP_CONFIG_USE_SPI3
extern SPI_T spi3;
#endif /*BSP_CONFIG_USE_SPI3*/

#ifdef BSP_CONFIG_USE_SPI4
extern SPI_T spi4;
#endif /*BSP_CONFIG_USE_SPI4*/

#ifdef BSP_CONFIG_USE_SPI5
extern SPI_T spi5;
#endif /*BSP_CONFIG_USE_SPI5*/

int32_t SPI_Init(void);
void SPI_Deinit(void);

int32_t SPI_SequentialTransmit(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *TxBuffer1, uint32_t  Size1, char *TxBuffer2, uint32_t  TxSize2);
int32_t SPI_SequentialReceive(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *TxBuffer, uint32_t  TxSize, char *RxBuffer, uint32_t  RxSize);
int32_t SPI_TransmitReceive(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *TxBuffer, char * RxBuffer, uint32_t  Size);
int32_t SPI_Transmit(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *Buffer, uint32_t  Size);
int32_t SPI_Receive(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *Buffer, uint32_t  Size);

int32_t SPI_DMA_SequentialTransmit(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *TxBuffer1, uint32_t  Size1, char *TxBuffer2, uint32_t  TxSize2);
int32_t SPI_DMA_SequentialReceive(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *TxBuffer, uint32_t  TxSize, char *RxBuffer, uint32_t  RxSize);
int32_t SPI_DMA_TransmitReceive(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *TxBuffer, char * RxBuffer, uint32_t  Size);
int32_t SPI_DMA_Transmit(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *Buffer, uint32_t  Size);
int32_t SPI_DMA_Receive(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *Buffer, uint32_t  Size);

#endif /* __SPI_H */
