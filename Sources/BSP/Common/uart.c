#include "uart.h"

#ifdef BSP_CONFIG_USE_FREERTOS
#define UART_MESSAGE_QUEUE_SIZE (1)
#define UART_EVENT_COMPLETE			(0)
#define UART_EVENT_ERROR				(0xFFFFFFFF)
#define UART_TIMEOUT 						(1000)
#else /*BSP_CONFIG_USE_FREERTOS*/
#define UART_TIMEOUT 						(1000)
#endif /*BSP_CONFIG_USE_FREERTOS*/

#ifdef BSP_CONFIG_USE_FREERTOS
osMutexDef(uart_rx);
osMutexDef(uart_tx);
osMessageQDef(uart_rx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
osMessageQDef(uart_tx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/

#ifdef BSP_CONFIG_USE_UART1
UART_T uart1;
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART2
UART_T uart2;
#endif /*BSP_CONFIG_USE_UART2*/

#ifdef BSP_CONFIG_USE_UART3
UART_T uart3;
#endif /*BSP_CONFIG_USE_UART3*/

#ifdef BSP_CONFIG_USE_UART4
UART_T uart4;
#endif /*BSP_CONFIG_USE_UART4*/

#ifdef BSP_CONFIG_USE_UART5
UART_T uart5;
#endif /*BSP_CONFIG_USE_UART5*/

#ifdef BSP_CONFIG_USE_UART6
UART_T uart6;
#endif /*BSP_CONFIG_USE_UART6*/

static int32_t UART_GetHandle( USART_TypeDef* Instance, UART_T **uart)
{
	if (Instance == USART1)
	{
#ifdef BSP_CONFIG_USE_UART1
		*uart = &uart1;
#else
		return -1;
#endif /*BSP_CONFIG_USE_UART1*/
	}
	else if (Instance == USART2)
	{
#ifdef BSP_CONFIG_USE_UART2
		*uart = &uart2;
#else
		return -1;
#endif /*BSP_CONFIG_USE_UART2*/
	}
	else if (Instance == USART3)
	{
#ifdef BSP_CONFIG_USE_UART3
		*uart = &uart3;
#else
		return -1;
#endif /*BSP_CONFIG_USE_UART2*/
	}
	else if (Instance == UART4)
	{
#ifdef BSP_CONFIG_USE_UART4
		*uart = &uart4;
#else
		return -1;
#endif /*BSP_CONFIG_USE_UART4*/
	}
	else if (Instance == UART5)
	{
#ifdef BSP_CONFIG_USE_UART5
		*uart = &uart5;
#else
		return -1;
#endif /*BSP_CONFIG_USE_UART5*/
	}
	else if (Instance == USART6)
	{
#ifdef BSP_CONFIG_USE_UART6
		*uart = &uart6;
#else
		return -1;
#endif /*BSP_CONFIG_USE_UART6*/
	}
	else
	{
		return -1;
	}
	return 0;
}
#if 0
static void //UART_IDLE_IRQHandler(UART_T *uart)
{
	if(RESET != __HAL_UART_GET_FLAG(&uart->handle, UART_FLAG_IDLE))
	{
		__HAL_UART_CLEAR_IDLEFLAG(&uart->handle);
		__HAL_UART_DISABLE_IT(&uart->handle, UART_IT_IDLE);
		//Stop this DMA transmission
		HAL_UART_DMAStop(&uart->handle);
		
		if (osMessagePut(uart->rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
			uart->rx_message_queue_overflow_errors++;
	}
}
#endif

#ifdef BSP_CONFIG_USE_UART1
/**
  * @brief  This function handles DMA RX interrupt request.
  * @param  None
  * @retval None
  */
void UART1_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart1.handle.hdmarx);
}

/**
  * @brief  This function handles DMA TX interrupt request.
  * @param  None
  * @retval None
  */
void UART1_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart1.handle.hdmatx);
}
/**
  * @brief  This function Handles UART1 interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart1.handle);
	
	//UART_IDLE_IRQHandler(&uart1);
}
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART2
/**
  * @brief  This function handles DMA RX interrupt request.
  * @param  None
  * @retval None
  */
void UART2_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart2.handle.hdmarx);
}

/**
  * @brief  This function handles DMA TX interrupt request.
  * @param  None
  * @retval None
  */
void UART2_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart2.handle.hdmatx);
}
/**
  * @brief  This function Handles UART2 interrupt request.
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart2.handle);
	
	//UART_IDLE_IRQHandler(&uart2);
}
#endif /*BSP_CONFIG_USE_UART2*/

#ifdef BSP_CONFIG_USE_UART3
/**
  * @brief  This function handles DMA RX interrupt request.
  * @param  None
  * @retval None
  */
void UART3_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart3.handle.hdmarx);
}

/**
  * @brief  This function handles DMA TX interrupt request.
  * @param  None
  * @retval None
  */
void UART3_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart3.handle.hdmatx);
}
/**
  * @brief  This function Handles UART3 interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart3.handle);
	
	//UART_IDLE_IRQHandler(&uart3);
}
#endif /*BSP_CONFIG_USE_UART3*/

#ifdef BSP_CONFIG_USE_UART4
/**
  * @brief  This function handles DMA RX interrupt request.
  * @param  None
  * @retval None
  */
void UART4_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart4.handle.hdmarx);
}

/**
  * @brief  This function handles DMA TX interrupt request.
  * @param  None
  * @retval None
  */
void UART4_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart4.handle.hdmatx);
}
/**
  * @brief  This function Handles UART4 interrupt request.
  * @param  None
  * @retval None
  */
void UART4_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart4.handle);
}
#endif /*BSP_CONFIG_USE_UART4*/

#ifdef BSP_CONFIG_USE_UART5
/**
  * @brief  This function handles DMA RX interrupt request.
  * @param  None
  * @retval None
  */
void UART5_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart5.handle.hdmarx);
}

/**
  * @brief  This function handles DMA TX interrupt request.
  * @param  None
  * @retval None
  */
void UART5_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart5.handle.hdmatx);
}
/**
  * @brief  This function Handles UART5 interrupt request.
  * @param  None
  * @retval None
  */
void USART5_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart5.handle);
	
	//UART_IDLE_IRQHandler(&uart5);
}
#endif /*BSP_CONFIG_USE_UART5*/

#ifdef BSP_CONFIG_USE_UART6
/**
  * @brief  This function handles DMA RX interrupt request.
  * @param  None
  * @retval None
  */
void UART6_DMA_RX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart6.handle.hdmarx);
}

/**
  * @brief  This function handles DMA TX interrupt request.
  * @param  None
  * @retval None
  */
void UART6_DMA_TX_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uart6.handle.hdmatx);
	
	//UART_IDLE_IRQHandler(&uart6);
}
/**
  * @brief  This function Handles UART6 interrupt request.
  * @param  None
  * @retval None
  */
void USART6_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart6.handle);
}
#endif /*BSP_CONFIG_USE_UART6*/

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART Handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  GPIO_InitTypeDef  GPIO_InitStruct;

#ifdef BSP_CONFIG_USE_UART1
	if (huart->Instance == USART1)
	{
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		UART1_TX_GPIO_CLK_ENABLE();
		UART1_RX_GPIO_CLK_ENABLE();
		/* Enable UART clock */
		UART1_CLK_ENABLE();
		/* Enable UART DMA clock */
		UART1_DMA_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* UART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = UART1_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = UART1_TX_AF;
		HAL_GPIO_Init(UART1_TX_GPIO_PORT, &GPIO_InitStruct);
		/* UART RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = UART1_RX_PIN;
		GPIO_InitStruct.Alternate = UART1_RX_AF;
		HAL_GPIO_Init(UART1_RX_GPIO_PORT, &GPIO_InitStruct);

		/*##-3- Configure the DMA streams ##########################################*/
		/* Configure the DMA handler for Transmission process */
		uart1.hdma_tx.Instance                 = UART1_TX_DMA_STREAM;

		uart1.hdma_tx.Init.Channel             = UART1_TX_DMA_CHANNEL;
		uart1.hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		uart1.hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart1.hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
		uart1.hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart1.hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart1.hdma_tx.Init.Mode                = DMA_NORMAL;
		uart1.hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
		uart1.hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart1.hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart1.hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
		uart1.hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart1.hdma_tx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmatx, uart1.hdma_tx);

		/* Configure the DMA handler for Transmission process */
		uart1.hdma_rx.Instance                 = UART1_RX_DMA_STREAM;

		uart1.hdma_rx.Init.Channel             = UART1_RX_DMA_CHANNEL;
		uart1.hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		uart1.hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart1.hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
		uart1.hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart1.hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart1.hdma_rx.Init.Mode                = DMA_NORMAL;
		uart1.hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		uart1.hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart1.hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart1.hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
		uart1.hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart1.hdma_rx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmarx, uart1.hdma_rx);

		/*##-4- Configure the NVIC for UART ########################################*/
		/* NVIC for USART1 */
		HAL_NVIC_SetPriority(USART1_IRQn, UART1_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);

		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
		HAL_NVIC_SetPriority(UART1_DMA_TX_IRQn, UART1_DMA_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(UART1_DMA_TX_IRQn);

		/* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
		HAL_NVIC_SetPriority(UART1_DMA_RX_IRQn, UART1_DMA_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(UART1_DMA_RX_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART1*/
#ifdef BSP_CONFIG_USE_UART2
	if (huart->Instance == USART2)
	{
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		UART2_TX_GPIO_CLK_ENABLE();
		UART2_RX_GPIO_CLK_ENABLE();
		/* Enable UART clock */
		UART2_CLK_ENABLE();
		/* Enable UART DMA clock */
		UART2_DMA_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* UART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = UART2_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = UART2_TX_AF;
		HAL_GPIO_Init(UART2_TX_GPIO_PORT, &GPIO_InitStruct);
		/* UART RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = UART2_RX_PIN;
		GPIO_InitStruct.Alternate = UART2_RX_AF;
		HAL_GPIO_Init(UART2_RX_GPIO_PORT, &GPIO_InitStruct);

		/*##-3- Configure the DMA streams ##########################################*/
		/* Configure the DMA handler for Transmission process */
		uart2.hdma_tx.Instance                 = UART2_TX_DMA_STREAM;

		uart2.hdma_tx.Init.Channel             = UART2_TX_DMA_CHANNEL;
		uart2.hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		uart2.hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart2.hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
		uart2.hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart2.hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart2.hdma_tx.Init.Mode                = DMA_NORMAL;
		uart2.hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
		uart2.hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart2.hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart2.hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
		uart2.hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart2.hdma_tx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmatx, uart2.hdma_tx);

		/* Configure the DMA handler for Transmission process */
		uart2.hdma_rx.Instance                 = UART2_RX_DMA_STREAM;

		uart2.hdma_rx.Init.Channel             = UART2_RX_DMA_CHANNEL;
		uart2.hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		uart2.hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart2.hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
		uart2.hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart2.hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart2.hdma_rx.Init.Mode                = DMA_NORMAL;
		uart2.hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		uart2.hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart2.hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart2.hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
		uart2.hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart2.hdma_rx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmarx, uart2.hdma_rx);

		/*##-4- Configure the NVIC for UART ########################################*/
		/* NVIC for USART2 */
		HAL_NVIC_SetPriority(USART2_IRQn, UART2_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);

		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
		HAL_NVIC_SetPriority(UART2_DMA_TX_IRQn, UART2_DMA_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(UART2_DMA_TX_IRQn);

		/* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
		HAL_NVIC_SetPriority(UART2_DMA_RX_IRQn, UART2_DMA_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(UART2_DMA_RX_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART2*/
#ifdef BSP_CONFIG_USE_UART3
	if (huart->Instance == USART3)
	{
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		UART3_TX_GPIO_CLK_ENABLE();
		UART3_RX_GPIO_CLK_ENABLE();
		/* Enable UART clock */
		UART3_CLK_ENABLE();
		/* Enable UART DMA clock */
		UART3_DMA_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* UART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = UART3_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = UART3_TX_AF;
		HAL_GPIO_Init(UART3_TX_GPIO_PORT, &GPIO_InitStruct);
		/* UART RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = UART3_RX_PIN;
		GPIO_InitStruct.Alternate = UART3_RX_AF;
		HAL_GPIO_Init(UART3_RX_GPIO_PORT, &GPIO_InitStruct);

		/*##-3- Configure the DMA streams ##########################################*/
		/* Configure the DMA handler for Transmission process */
		uart3.hdma_tx.Instance                 = UART3_TX_DMA_STREAM;

		uart3.hdma_tx.Init.Channel             = UART3_TX_DMA_CHANNEL;
		uart3.hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		uart3.hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart3.hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
		uart3.hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart3.hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart3.hdma_tx.Init.Mode                = DMA_NORMAL;
		uart3.hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
		uart3.hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart3.hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart3.hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
		uart3.hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart3.hdma_tx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmatx, uart3.hdma_tx);

		/* Configure the DMA handler for Transmission process */
		uart3.hdma_rx.Instance                 = UART3_RX_DMA_STREAM;

		uart3.hdma_rx.Init.Channel             = UART3_RX_DMA_CHANNEL;
		uart3.hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		uart3.hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart3.hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
		uart3.hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart3.hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart3.hdma_rx.Init.Mode                = DMA_NORMAL;
		uart3.hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		uart3.hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart3.hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart3.hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
		uart3.hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart3.hdma_rx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmarx, uart3.hdma_rx);

		/*##-4- Configure the NVIC for UART ########################################*/
		/* NVIC for USART3 */
		HAL_NVIC_SetPriority(USART3_IRQn, UART3_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);

		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
		HAL_NVIC_SetPriority(UART3_DMA_TX_IRQn, UART3_DMA_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(UART3_DMA_TX_IRQn);

		/* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
		HAL_NVIC_SetPriority(UART3_DMA_RX_IRQn, UART3_DMA_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(UART3_DMA_RX_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART3*/
#ifdef BSP_CONFIG_USE_UART4
	if (huart->Instance == UART4)
	{
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		UART4_TX_GPIO_CLK_ENABLE();
		UART4_RX_GPIO_CLK_ENABLE();
		/* Enable UART clock */
		UART4_CLK_ENABLE();
		/* Enable UART DMA clock */
		UART4_DMA_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* UART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = UART4_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = UART4_TX_AF;
		HAL_GPIO_Init(UART4_TX_GPIO_PORT, &GPIO_InitStruct);
		/* UART RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = UART4_RX_PIN;
		GPIO_InitStruct.Alternate = UART4_RX_AF;
		HAL_GPIO_Init(UART4_RX_GPIO_PORT, &GPIO_InitStruct);

		/*##-3- Configure the DMA streams ##########################################*/
		/* Configure the DMA handler for Transmission process */
		uart4.hdma_tx.Instance                 = UART4_TX_DMA_STREAM;

		uart4.hdma_tx.Init.Channel             = UART4_TX_DMA_CHANNEL;
		uart4.hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		uart4.hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart4.hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
		uart4.hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart4.hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart4.hdma_tx.Init.Mode                = DMA_NORMAL;
		uart4.hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
		uart4.hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart4.hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart4.hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
		uart4.hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart4.hdma_tx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmatx, uart4.hdma_tx);

		/* Configure the DMA handler for Transmission process */
		uart4.hdma_rx.Instance                 = UART4_RX_DMA_STREAM;

		uart4.hdma_rx.Init.Channel             = UART4_RX_DMA_CHANNEL;
		uart4.hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		uart4.hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart4.hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
		uart4.hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart4.hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart4.hdma_rx.Init.Mode                = DMA_NORMAL;
		uart4.hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		uart4.hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart4.hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart4.hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
		uart4.hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart4.hdma_rx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmarx, uart4.hdma_rx);

		/*##-4- Configure the NVIC for UART ########################################*/
		/* NVIC for USART4 */
		HAL_NVIC_SetPriority(UART4_IRQn, UART4_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(UART4_IRQn);

		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
		HAL_NVIC_SetPriority(UART4_DMA_TX_IRQn, UART4_DMA_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(UART4_DMA_TX_IRQn);

		/* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
		HAL_NVIC_SetPriority(UART4_DMA_RX_IRQn, UART4_DMA_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(UART4_DMA_RX_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART4*/
#ifdef BSP_CONFIG_USE_UART5
	if (huart->Instance == UART5)
	{
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		UART5_TX_GPIO_CLK_ENABLE();
		UART5_RX_GPIO_CLK_ENABLE();
		/* Enable UART clock */
		UART5_CLK_ENABLE();
		/* Enable UART DMA clock */
		UART5_DMA_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* UART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = UART5_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = UART5_TX_AF;
		HAL_GPIO_Init(UART5_TX_GPIO_PORT, &GPIO_InitStruct);
		/* UART RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = UART5_RX_PIN;
		GPIO_InitStruct.Alternate = UART5_RX_AF;
		HAL_GPIO_Init(UART5_RX_GPIO_PORT, &GPIO_InitStruct);

		/*##-3- Configure the DMA streams ##########################################*/
		/* Configure the DMA handler for Transmission process */
		uart5.hdma_tx.Instance                 = UART5_TX_DMA_STREAM;

		uart5.hdma_tx.Init.Channel             = UART5_TX_DMA_CHANNEL;
		uart5.hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		uart5.hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart5.hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
		uart5.hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart5.hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart5.hdma_tx.Init.Mode                = DMA_NORMAL;
		uart5.hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
		uart5.hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart5.hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart5.hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
		uart5.hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart5.hdma_tx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmatx, uart5.hdma_tx);

		/* Configure the DMA handler for Transmission process */
		uart5.hdma_rx.Instance                 = UART5_RX_DMA_STREAM;

		uart5.hdma_rx.Init.Channel             = UART5_RX_DMA_CHANNEL;
		uart5.hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		uart5.hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart5.hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
		uart5.hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart5.hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart5.hdma_rx.Init.Mode                = DMA_NORMAL;
		uart5.hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		uart5.hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart5.hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart5.hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
		uart5.hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart5.hdma_rx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmarx, uart5.hdma_rx);

		/*##-4- Configure the NVIC for UART ########################################*/
		/* NVIC for USART5 */
		HAL_NVIC_SetPriority(UART5_IRQn, UART5_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(UART5_IRQn);

		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
		HAL_NVIC_SetPriority(UART5_DMA_TX_IRQn, UART5_DMA_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(UART5_DMA_TX_IRQn);

		/* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
		HAL_NVIC_SetPriority(UART5_DMA_RX_IRQn, UART5_DMA_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(UART5_DMA_RX_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART5*/
#ifdef BSP_CONFIG_USE_UART6
	if (huart->Instance == USART6)
	{
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		UART6_TX_GPIO_CLK_ENABLE();
		UART6_RX_GPIO_CLK_ENABLE();
		/* Enable UART clock */
		UART6_CLK_ENABLE();
		/* Enable UART DMA clock */
		UART6_DMA_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* UART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = UART6_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = UART6_TX_AF;
		HAL_GPIO_Init(UART6_TX_GPIO_PORT, &GPIO_InitStruct);
		/* UART RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = UART6_RX_PIN;
		GPIO_InitStruct.Alternate = UART6_RX_AF;
		HAL_GPIO_Init(UART6_RX_GPIO_PORT, &GPIO_InitStruct);

		/*##-3- Configure the DMA streams ##########################################*/
		/* Configure the DMA handler for Transmission process */
		uart6.hdma_tx.Instance                 = UART6_TX_DMA_STREAM;

		uart6.hdma_tx.Init.Channel             = UART6_TX_DMA_CHANNEL;
		uart6.hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		uart6.hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart6.hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
		uart6.hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart6.hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart6.hdma_tx.Init.Mode                = DMA_NORMAL;
		uart6.hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
		uart6.hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart6.hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart6.hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
		uart6.hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart6.hdma_tx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmatx, uart6.hdma_tx);

		/* Configure the DMA handler for Transmission process */
		uart6.hdma_rx.Instance                 = UART6_RX_DMA_STREAM;

		uart6.hdma_rx.Init.Channel             = UART6_RX_DMA_CHANNEL;
		uart6.hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		uart6.hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		uart6.hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
		uart6.hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		uart6.hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		uart6.hdma_rx.Init.Mode                = DMA_NORMAL;
		uart6.hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		uart6.hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		uart6.hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		uart6.hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
		uart6.hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;

		HAL_DMA_Init(&uart6.hdma_rx);

		/* Associate the initialized DMA handle to the the UART handle */
		__HAL_LINKDMA(huart, hdmarx, uart6.hdma_rx);

		/*##-4- Configure the NVIC for UART ########################################*/
		/* NVIC for USART6 */
		HAL_NVIC_SetPriority(USART6_IRQn, UART6_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(USART6_IRQn);

		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
		HAL_NVIC_SetPriority(UART6_DMA_TX_IRQn, UART6_DMA_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(UART6_DMA_TX_IRQn);

		/* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
		HAL_NVIC_SetPriority(UART6_DMA_RX_IRQn, UART6_DMA_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(UART6_DMA_RX_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART6*/
	{
	}
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART Handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
#ifdef BSP_CONFIG_USE_UART1
	if (huart->Instance == USART1)
	{

		/*##-1- Reset peripherals ##################################################*/
		__HAL_RCC_USART1_FORCE_RESET();
		__HAL_RCC_USART1_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks #################################*/
		/* Configure UART Tx as alternate function  */
		HAL_GPIO_DeInit(UART1_TX_GPIO_PORT, UART1_TX_PIN);
		/* Configure UART Rx as alternate function  */
		HAL_GPIO_DeInit(UART1_RX_GPIO_PORT, UART1_RX_PIN);
		/*##-3- Disable the NVIC for UART ##########################################*/
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART1*/
#ifdef BSP_CONFIG_USE_UART2
	if (huart->Instance == USART2)
	{
		/*##-1- Reset peripherals ##################################################*/
		__HAL_RCC_USART2_FORCE_RESET();
		__HAL_RCC_USART2_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks #################################*/
		/* Configure UART Tx as alternate function  */
		HAL_GPIO_DeInit(UART2_TX_GPIO_PORT, UART2_TX_PIN);
		/* Configure UART Rx as alternate function  */
		HAL_GPIO_DeInit(UART2_RX_GPIO_PORT, UART2_RX_PIN);
		/*##-3- Disable the NVIC for UART ##########################################*/
		HAL_NVIC_DisableIRQ(USART2_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART2*/
#ifdef BSP_CONFIG_USE_UART3
	if (huart->Instance == USART3)
	{
		/*##-1- Reset peripherals ##################################################*/
		__HAL_RCC_USART3_FORCE_RESET();
		__HAL_RCC_USART3_RELEASE_RESET();
		/*##-3- Disable peripherals and GPIO Clocks #################################*/
		/* Configure UART Tx as alternate function  */
		HAL_GPIO_DeInit(UART3_TX_GPIO_PORT, UART3_TX_PIN);
		/* Configure UART Rx as alternate function  */
		HAL_GPIO_DeInit(UART3_RX_GPIO_PORT, UART3_RX_PIN);
		/*##-3- Disable the NVIC for UART ##########################################*/
		HAL_NVIC_DisableIRQ(USART3_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART3*/
#ifdef BSP_CONFIG_USE_UART4
	if (huart->Instance == UART4)
	{
		/*##-1- Reset peripherals ##################################################*/
		__HAL_RCC_UART4_FORCE_RESET();
		__HAL_RCC_UART4_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks #################################*/
		/* Configure UART Tx as alternate function  */
		HAL_GPIO_DeInit(UART4_TX_GPIO_PORT, UART4_TX_PIN);
		/* Configure UART Rx as alternate function  */
		HAL_GPIO_DeInit(UART4_RX_GPIO_PORT, UART4_RX_PIN);
		/*##-3- Disable the NVIC for UART ##########################################*/
		HAL_NVIC_DisableIRQ(UART4_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART4*/
#ifdef BSP_CONFIG_USE_UART5
	if (huart->Instance == UART5)
	{
		/*##-1- Reset peripherals ##################################################*/
		__HAL_RCC_UART5_FORCE_RESET();
		__HAL_RCC_UART5_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks #################################*/
		/* Configure UART Tx as alternate function  */
		HAL_GPIO_DeInit(UART5_TX_GPIO_PORT, UART5_TX_PIN);
		/* Configure UART Rx as alternate function  */
		HAL_GPIO_DeInit(UART5_RX_GPIO_PORT, UART5_RX_PIN);
		/*##-3- Disable the NVIC for UART ##########################################*/
		HAL_NVIC_DisableIRQ(UART5_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART5*/
#ifdef BSP_CONFIG_USE_UART6
	if (huart->Instance == USART6)
	{
		/*##-1- Reset peripherals ##################################################*/
		__HAL_RCC_USART6_FORCE_RESET();
		__HAL_RCC_USART6_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks #################################*/
		/* Configure UART Tx as alternate function  */
		HAL_GPIO_DeInit(UART6_TX_GPIO_PORT, UART6_TX_PIN);
		/* Configure UART Rx as alternate function  */
		HAL_GPIO_DeInit(UART6_RX_GPIO_PORT, UART6_RX_PIN);
		/*##-3- Disable the NVIC for UART ##########################################*/
		HAL_NVIC_DisableIRQ(USART6_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_UART6*/
	{
	}
}

/**
  * @brief  Rx Transfer completed callback
  * @param  hUART: UART Handle.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *hUART)
{
	UART_T *uart;

	if (UART_GetHandle(hUART->Instance, &uart))
		return;

	if (uart->rx_buffer_end)
	{
		if (uart->rx_buffer[uart->rx_buffer_idx] == *uart->rx_buffer_end)
		{
			if (osMessagePut(uart->rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
				uart->rx_message_queue_overflow_errors++;
		}
		else if (uart->rx_buffer_idx == (uart->rx_buffer_size - 1))
		{
			if (osMessagePut(uart->rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
				uart->rx_message_queue_overflow_errors++;
		}
		else if (uart->handle.ErrorCode != HAL_UART_ERROR_NONE)
		{
			if (osMessagePut(uart->rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
				uart->rx_message_queue_overflow_errors++;
		}
		else
		{
			HAL_UART_Receive_IT(&uart->handle, (uint8_t*)&uart->rx_buffer[++uart->rx_buffer_idx], 1);
		}
	}
	else
	{
		if (uart->rx_buffer_idx == (uart->rx_buffer_size - 1))
		{
			if (osMessagePut(uart->rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
				uart->rx_message_queue_overflow_errors++;
		}
		else if (uart->handle.ErrorCode != HAL_UART_ERROR_NONE)
		{
			if (osMessagePut(uart->rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
				uart->rx_message_queue_overflow_errors++;
		}
		else
		{
			HAL_UART_Receive_IT(&uart->handle, (uint8_t*)&uart->rx_buffer[++uart->rx_buffer_idx], 1);
		}
	}
}

/**
  * @brief  Tx Transfer completed callback
  * @param  hUART: UART Handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *hUART)
{
	UART_T *uart;

	if (UART_GetHandle(hUART->Instance, &uart))
		return;

	if (osMessagePut(uart->tx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
		uart->tx_message_queue_overflow_errors++;
}

/**
  * @brief  Transfer error callback
  * @param  hUART: UART Handle
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *hUART)
{
	UART_T *uart;

	if (UART_GetHandle(hUART->Instance, &uart))
		return;

	if (uart->handle.ErrorCode & HAL_UART_ERROR_PE)
		uart->parity_errors++;
	if (uart->handle.ErrorCode & HAL_UART_ERROR_NE)
		uart->noise_errors++;
	if (uart->handle.ErrorCode & HAL_UART_ERROR_FE)
		uart->frame_errors++;
	if (uart->handle.ErrorCode & HAL_UART_ERROR_ORE)
		uart->overrun_errors++;
	if (uart->handle.ErrorCode & HAL_UART_ERROR_DMA)
		uart->dma_errors++;

	if (uart->handle.ErrorCode & (HAL_UART_ERROR_DMA | HAL_UART_ERROR_ORE))
	{
		if (osMessagePut(uart->rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
			uart->rx_message_queue_overflow_errors++;
	}
}

/**
  * @brief  UART initialization
  * @param  Instance: UART instance
  * @retval 0 on succes, otherwise -1;
  */

int32_t UART_Init(void)
{
	int32_t result;

	result = 0;
#ifdef BSP_CONFIG_USE_UART1
	uart1.handle.Instance          = USART1;
	uart1.handle.Init.BaudRate     = UART1_BAUDRATE;
	uart1.handle.Init.WordLength   = UART1_WORDLENGTH;
	uart1.handle.Init.StopBits     = UART1_STOPBITS;
	uart1.handle.Init.Parity       = UART1_PARITY;
	uart1.handle.Init.HwFlowCtl    = UART1_HWCONTROL;
	uart1.handle.Init.Mode         = UART1_MODE;
	uart1.handle.Init.OverSampling = UART1_OVERSAMPLING;

	if(HAL_UART_Init(&uart1.handle) != HAL_OK)
	{
		result = -1;
	}
#ifdef BSP_CONFIG_USE_FREERTOS
	if (!uart1.rx_mutex_id)
		uart1.rx_mutex_id = osMutexCreate(osMutex(uart_rx));
	if (!uart1.tx_mutex_id)
		uart1.tx_mutex_id = osMutexCreate(osMutex(uart_tx));
	if (!uart1.rx_message_queue_id)
		uart1.rx_message_queue_id = osMessageCreate (osMessageQ(uart_rx), NULL);
	if (!uart1.tx_message_queue_id)
		uart1.tx_message_queue_id = osMessageCreate (osMessageQ(uart_tx), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART2
	uart2.handle.Instance          = USART2;
	uart2.handle.Init.BaudRate     = UART2_BAUDRATE;
	uart2.handle.Init.WordLength   = UART2_WORDLENGTH;
	uart2.handle.Init.StopBits     = UART2_STOPBITS;
	uart2.handle.Init.Parity       = UART2_PARITY;
	uart2.handle.Init.HwFlowCtl    = UART2_HWCONTROL;
	uart2.handle.Init.Mode         = UART2_MODE;
	uart2.handle.Init.OverSampling = UART2_OVERSAMPLING;

	if(HAL_UART_Init(&uart2.handle) != HAL_OK)
	{
		result = -1;
	}
#ifdef BSP_CONFIG_USE_FREERTOS
	if (!uart2.rx_mutex_id)
		uart2.rx_mutex_id = osMutexCreate(osMutex(uart_rx));
	if (!uart2.tx_mutex_id)
		uart2.tx_mutex_id = osMutexCreate(osMutex(uart_tx));
	if (!uart2.rx_message_queue_id)
		uart2.rx_message_queue_id = osMessageCreate (osMessageQ(uart_rx), NULL);
	if (!uart2.tx_message_queue_id)
		uart2.tx_message_queue_id = osMessageCreate (osMessageQ(uart_tx), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART2*/

#ifdef BSP_CONFIG_USE_UART3
	uart3.handle.Instance          = USART3;
	uart3.handle.Init.BaudRate     = UART3_BAUDRATE;
	uart3.handle.Init.WordLength   = UART3_WORDLENGTH;
	uart3.handle.Init.StopBits     = UART3_STOPBITS;
	uart3.handle.Init.Parity       = UART3_PARITY;
	uart3.handle.Init.HwFlowCtl    = UART3_HWCONTROL;
	uart3.handle.Init.Mode         = UART3_MODE;
	uart3.handle.Init.OverSampling = UART3_OVERSAMPLING;

	if(HAL_UART_Init(&uart3.handle) != HAL_OK)
	{
		result = -1;
	}
#ifdef BSP_CONFIG_USE_FREERTOS
	if (!uart3.rx_mutex_id)
		uart3.rx_mutex_id = osMutexCreate(osMutex(uart_rx));
	if (!uart3.tx_mutex_id)
		uart3.tx_mutex_id = osMutexCreate(osMutex(uart_tx));
	if (!uart3.rx_message_queue_id)
		uart3.rx_message_queue_id = osMessageCreate (osMessageQ(uart_rx), NULL);
	if (!uart3.tx_message_queue_id)
		uart3.tx_message_queue_id = osMessageCreate (osMessageQ(uart_tx), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART3*/

#ifdef BSP_CONFIG_USE_UART4
	uart4.handle.Instance          = UART4;
	uart4.handle.Init.BaudRate     = UART4_BAUDRATE;
	uart4.handle.Init.WordLength   = UART4_WORDLENGTH;
	uart4.handle.Init.StopBits     = UART4_STOPBITS;
	uart4.handle.Init.Parity       = UART4_PARITY;
	uart4.handle.Init.HwFlowCtl    = UART4_HWCONTROL;
	uart4.handle.Init.Mode         = UART4_MODE;
	uart4.handle.Init.OverSampling = UART4_OVERSAMPLING;

	if(HAL_UART_Init(&uart4.handle) != HAL_OK)
	{
		result = -1;
	}
#ifdef BSP_CONFIG_USE_FREERTOS
	if (!uart4.rx_mutex_id)
		uart4.rx_mutex_id = osMutexCreate(osMutex(uart_rx));
	if (!uart4.tx_mutex_id)
		uart4.tx_mutex_id = osMutexCreate(osMutex(uart_tx));
	if (!uart4.rx_message_queue_id)
		uart4.rx_message_queue_id = osMessageCreate (osMessageQ(uart_rx), NULL);
	if (!uart4.tx_message_queue_id)
		uart4.tx_message_queue_id = osMessageCreate (osMessageQ(uart_tx), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART4*/

#ifdef BSP_CONFIG_USE_UART5
	uart5.handle.Instance          = UART5;
	uart5.handle.Init.BaudRate     = UART5_BAUDRATE;
	uart5.handle.Init.WordLength   = UART5_WORDLENGTH;
	uart5.handle.Init.StopBits     = UART5_STOPBITS;
	uart5.handle.Init.Parity       = UART5_PARITY;
	uart5.handle.Init.HwFlowCtl    = UART5_HWCONTROL;
	uart5.handle.Init.Mode         = UART5_MODE;
	uart5.handle.Init.OverSampling = UART5_OVERSAMPLING;

	if(HAL_UART_Init(&uart5.handle) != HAL_OK)
	{
		result = -1;
	}
#ifdef BSP_CONFIG_USE_FREERTOS
	if (!uart5.rx_mutex_id)
		uart5.rx_mutex_id = osMutexCreate(osMutex(uart_rx));
	if (!uart5.tx_mutex_id)
		uart5.tx_mutex_id = osMutexCreate(osMutex(uart_tx));
	if (!uart5.rx_message_queue_id)
		uart5.rx_message_queue_id = osMessageCreate (osMessageQ(uart_rx), NULL);
	if (!uart5.tx_message_queue_id)
		uart5.tx_message_queue_id = osMessageCreate (osMessageQ(uart_tx), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART5*/

#ifdef BSP_CONFIG_USE_UART6
	uart6.handle.Instance          = USART6;
	uart6.handle.Init.BaudRate     = UART6_BAUDRATE;
	uart6.handle.Init.WordLength   = UART6_WORDLENGTH;
	uart6.handle.Init.StopBits     = UART6_STOPBITS;
	uart6.handle.Init.Parity       = UART6_PARITY;
	uart6.handle.Init.HwFlowCtl    = UART6_HWCONTROL;
	uart6.handle.Init.Mode         = UART6_MODE;
	uart6.handle.Init.OverSampling = UART6_OVERSAMPLING;

	if(HAL_UART_Init(&uart6.handle) != HAL_OK)
	{
		result = -1;
	}
#ifdef BSP_CONFIG_USE_FREERTOS
	if (!uart6.rx_mutex_id)
		uart6.rx_mutex_id = osMutexCreate(osMutex(uart_rx));
	if (!uart6.tx_mutex_id)
		uart6.tx_mutex_id = osMutexCreate(osMutex(uart_tx));
	if (!uart6.rx_message_queue_id)
		uart6.rx_message_queue_id = osMessageCreate (osMessageQ(uart_rx), NULL);
	if (!uart6.tx_message_queue_id)
		uart6.tx_message_queue_id = osMessageCreate (osMessageQ(uart_tx), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART6*/
	return result;
}

/**
  * @brief  UART initialization
  * @param  Instance: UART instance
  * @retval 0 on succes, otherwise -1;
  */
void UART_Deinit(void)
{
#ifdef BSP_CONFIG_USE_UART1
	HAL_UART_DeInit(&uart1.handle);
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART2
	HAL_UART_DeInit(&uart2.handle);
#endif /*BSP_CONFIG_USE_UART2*/

#ifdef BSP_CONFIG_USE_UART3
	HAL_UART_DeInit(&uart3.handle);
#endif /*BSP_CONFIG_USE_UART3*/

#ifdef BSP_CONFIG_USE_UART4
	HAL_UART_DeInit(&uart4.handle);
#endif /*BSP_CONFIG_USE_UART4*/

#ifdef BSP_CONFIG_USE_UART5
	HAL_UART_DeInit(&uart5.handle);
#endif /*BSP_CONFIG_USE_UART5*/

#ifdef BSP_CONFIG_USE_UART6
	HAL_UART_DeInit(&uart6.handle);
#endif /*BSP_CONFIG_USE_UART6*/
}

/**
  * @brief  Transmit an amount of data in blocking mode with Interrupt.
  * @param  Instance: UART instance
  * @param  Buffer: pointer to buffer with data to transmit
  * @param  Size: size of data to transmit
  * @retval 0 on succes, otherwise -1;
  */
int32_t UART_Transmit(USART_TypeDef * Instance, char *Buffer, uint32_t  Size)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	UART_T *uart;
	osEvent evt;

	if (UART_GetHandle(Instance, &uart))
		return -1;

	if( osMutexWait( uart->tx_mutex_id, UART_TIMEOUT ) != osOK )
	{
		uart->timeout_errors++;
		return -1;
	}

	if(HAL_UART_Transmit_IT(&uart->handle, (uint8_t*)Buffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(uart->tx_message_queue_id, UART_TIMEOUT);
	if (evt.status != osEventMessage)
		goto timeout;

	if (evt.value.v != UART_EVENT_COMPLETE)
		goto abort;

	osMutexRelease( uart->tx_mutex_id );
	return 0;

timeout:
	uart->tx_timeout_errors++;
abort:
	HAL_UART_AbortTransmit(&uart->handle);
error:
	osMutexRelease( uart->tx_mutex_id );
	return -1;
#else /*BSP_CONFIG_USE_FREERTOS*/
	UART_T *uart;

	if (UART_GetHandle(Instance, &uart))
		return -1;

	if(HAL_UART_Transmit(&uart->handle, (uint8_t*)Buffer, Size, UART_TIMEOUT) != HAL_OK)
		return -1;
	return 0;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Receive an amount of data in blocking mode with Interrupt.
  * @param  Instance: UART instance
  * @param  Buffer: pointer to buffer for received data
  * @param  Size: size of data to receive
  * @retval 0 on succes, otherwise -1;
  */
int32_t UART_Receive(USART_TypeDef * Instance, char *Buffer, uint32_t  Size)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	UART_T *uart;
	osEvent evt;

	if (UART_GetHandle(Instance, &uart))
		return -1;

	if( osMutexWait( uart->rx_mutex_id, UART_TIMEOUT ) != osOK )
	{
		uart->timeout_errors++;
		return -1;
	}

	uart->rx_buffer = (uint8_t*)Buffer;
	uart->rx_buffer_size = Size;
	uart->rx_buffer_idx = 0;
	uart->rx_buffer_end = NULL;

	if(HAL_UART_Receive_IT(&uart->handle, uart->rx_buffer, 1) != HAL_OK)
		goto error;

	evt = osMessageGet(uart->rx_message_queue_id, UART_TIMEOUT);
	if (evt.status != osEventMessage)
		goto timeout;

	if (evt.value.v != UART_EVENT_COMPLETE)
		goto abort;

	osMutexRelease( uart->rx_mutex_id );
	return 0;

timeout:
	uart->rx_timeout_errors++;
abort:
	HAL_UART_AbortReceive(&uart->handle);
error:
	osMutexRelease( uart->rx_mutex_id );
	return -1;
#else /*BSP_CONFIG_USE_FREERTOS*/
	UART_T *uart;

	if (UART_GetHandle(Instance, &uart))
		return -1;

	if(HAL_UART_Receive(&uart->handle, (uint8_t*)Buffer, Size, UART_TIMEOUT) != HAL_OK)
		return -1;
	return 0;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Receive an amount of data in blocking mode with Interrupt.
  * @param  Instance: UART instance
  * @param  Buffer: pointer to buffer for received data
  * @param  Size: size of data to receive
  * @retval 0 on succes, otherwise -1;
  */
int32_t UART_ReceiveCommand(USART_TypeDef *Instance, char *Buffer, uint32_t Size, char End, uint32_t *Len, uint32_t Timeout)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	UART_T *uart;
	osEvent evt;

	if (UART_GetHandle(Instance, &uart))
		return -1;

	if( osMutexWait( uart->rx_mutex_id, Timeout) != osOK )
	{
		uart->timeout_errors++;
		return -1;
	}

	uart->rx_buffer = (uint8_t*)Buffer;
	uart->rx_buffer_size = Size;
	uart->rx_buffer_idx = 0;
	uart->rx_buffer_end = (uint8_t*)&End;

	if(HAL_UART_Receive_IT(&uart->handle, uart->rx_buffer, 1) != HAL_OK)
		goto error;

	evt = osMessageGet(uart->rx_message_queue_id, Timeout);
	if (evt.status != osEventMessage)
		goto timeout;

	if (evt.value.v != UART_EVENT_COMPLETE)
		goto abort;

	*Len = uart->rx_buffer_idx + 1;

	osMutexRelease( uart->rx_mutex_id );
	return 0;

timeout:
	uart->rx_cmd_timeout_errors++;
abort:
	HAL_UART_AbortReceive(&uart->handle);
error:
	osMutexRelease( uart->rx_mutex_id );
	return -1;
#else /*BSP_CONFIG_USE_FREERTOS*/
	return -1;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Transmit an amount of data in blocking mode with DMA.
  * @param  Instance: UART instance
  * @param  Buffer: pointer to buffer with data to transmit
  * @param  Size: size of data to transmit
  * @retval 0 on succes, otherwise -1;
  */
int32_t UART_DMA_Transmit(USART_TypeDef * Instance, char *Buffer, uint32_t  Size)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	UART_T *uart;
	osEvent evt;

	if (UART_GetHandle(Instance, &uart))
		return -1;

	if( osMutexWait( uart->tx_mutex_id, UART_TIMEOUT ) != osOK )
	{
		uart->timeout_errors++;
		return -1;
	}

	if(HAL_UART_Transmit_DMA(&uart->handle, (uint8_t*)Buffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(uart->tx_message_queue_id, UART_TIMEOUT);
	if (evt.status != osEventMessage)
		goto timeout;

	if (evt.value.v != UART_EVENT_COMPLETE)
		goto abort;

	osMutexRelease( uart->tx_mutex_id );
	return 0;

timeout:
	uart->tx_timeout_errors++;
abort:
	HAL_UART_AbortTransmit(&uart->handle);
error:
	osMutexRelease( uart->tx_mutex_id );
	return -1;
#else /*BSP_CONFIG_USE_FREERTOS*/
	UART_T *uart;

	if (UART_GetHandle(Instance, &uart))
		return -1;

	if(HAL_UART_Transmit_DMA(&uart->handle, (uint8_t*)Buffer, Size, UART_TIMEOUT) != HAL_OK)
		return -1;
	return 0;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

#if 0
/**
  * @brief  Receive an amount of data in blocking mode with Interrupt.
  * @param  Instance: UART instance
  * @param  Buffer: pointer to buffer for received data
  * @param  Size: size of data to receive
  * @retval 0 on succes, otherwise -1;
  */
int32_t UART_DMA_ReceiveCommand(USART_TypeDef *Instance, char *Buffer, uint32_t Size, char End, uint32_t *Len, uint32_t Timeout)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	UART_T *uart;
	osEvent evt;
	uint32_t i;

	if (UART_GetHandle(Instance, &uart))
		return -1;

	if( osMutexWait( uart->rx_mutex_id, Timeout) != osOK )
	{
		uart->timeout_errors++;
		return -1;
	}

	uart->rx_buffer = (uint8_t*)Buffer;
	uart->rx_buffer_size = Size;
	uart->rx_buffer_idx = 0;
	uart->rx_buffer_end = (uint8_t*)&End;
	
	__HAL_UART_ENABLE_IT(&uart->handle, UART_IT_IDLE);
	if(HAL_UART_Receive_DMA(&uart->handle, uart->rx_buffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(uart->rx_message_queue_id, Timeout);
	if (evt.status != osEventMessage)
		goto timeout;

	if (evt.value.v == UART_EVENT_ERROR)
		goto abort;
	
	*Len = 0;	
	for (i = 0; i < Size; i++)
	{
		if (Buffer[i] == End)
		{
			*Len = i + 1;
		}
	}
	
	if (*Len == 0)
		goto error;

	osMutexRelease( uart->rx_mutex_id );
	return 0;

timeout:
	uart->rx_cmd_timeout_errors++;
abort:
	HAL_UART_AbortReceive(&uart->handle);
error:
	osMutexRelease( uart->rx_mutex_id );
	return -1;
#else /*BSP_CONFIG_USE_FREERTOS*/
	return -1;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}
#endif
