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

typedef struct {	
	UART_HandleTypeDef handle;
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexId rx_mutex_id;	
	osMessageQId rx_message_queue_id;
	uint32_t rx_message_queue_full;
	uint8_t *rx_buffer;
	uint32_t rx_buffer_size;
	uint32_t rx_buffer_idx;
	uint8_t *rx_buffer_end;
	
	osMutexId tx_mutex_id;
	osMessageQId tx_message_queue_id;
	uint32_t tx_message_queue_full;

	uint32_t error;
#endif /*BSP_CONFIG_USE_FREERTOS*/	
} UART_T;

#ifdef BSP_CONFIG_USE_UART1
static UART_T uart1;
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART2
static UART_T uart2;
#endif /*BSP_CONFIG_USE_UART2*/

#ifdef BSP_CONFIG_USE_UART3
#error UART3 support is not present in bsp driver
#endif /*BSP_CONFIG_USE_UART3*/

#ifdef BSP_CONFIG_USE_UART4
static UART_T uart4;
#endif /*BSP_CONFIG_USE_UART4*/

#ifdef BSP_CONFIG_USE_UART5
static UART_T uart5;
#endif /*BSP_CONFIG_USE_UART5*/

#ifdef BSP_CONFIG_USE_UART6
static UART_T uart6;
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

#ifdef BSP_CONFIG_USE_UART1
/**
  * @brief  This function Handles UART1 interrupt request.  
  * @param  None
  * @retval None    
  */
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart1.handle);
}
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART2
/**
  * @brief  This function Handles UART2 interrupt request.  
  * @param  None
  * @retval None   
  */
void USART2_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart2.handle);
}
#endif /*BSP_CONFIG_USE_UART2*/

#ifdef BSP_CONFIG_USE_UART4
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
  * @brief  This function Handles UART5 interrupt request.  
  * @param  None
  * @retval None   
  */
void UART5_IRQHandler(void)
{
  HAL_UART_IRQHandler(&uart5.handle);
}
#endif /*BSP_CONFIG_USE_UART5*/

#ifdef BSP_CONFIG_USE_UART6
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
 
	if (huart->Instance == USART1)
	{	
#ifdef BSP_CONFIG_USE_UART1
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		UART1_TX_GPIO_CLK_ENABLE();
		UART1_RX_GPIO_CLK_ENABLE();
		/* Enable USART1 clock */
		__HAL_RCC_USART1_CLK_ENABLE(); 		
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
		/*##-3- Configure the NVIC for UART ########################################*/
		/* NVIC for USART1 */
		HAL_NVIC_SetPriority(USART1_IRQn, UART1_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
#endif /*BSP_CONFIG_USE_UART1*/
	}
	else if (huart->Instance == USART2)
	{	
#ifdef BSP_CONFIG_USE_UART2
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		UART2_TX_GPIO_CLK_ENABLE();
		UART2_RX_GPIO_CLK_ENABLE();
		/* Enable USART2 clock */
		__HAL_RCC_USART2_CLK_ENABLE(); 		
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
		/*##-3- Configure the NVIC for UART ########################################*/
		/* NVIC for USART2 */
		HAL_NVIC_SetPriority(USART2_IRQn, UART2_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
#endif /*BSP_CONFIG_USE_UART2*/
	}
	else if (huart->Instance == UART4)
	{	
#ifdef BSP_CONFIG_USE_UART4
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		UART4_TX_GPIO_CLK_ENABLE();
		UART4_RX_GPIO_CLK_ENABLE();
		/* Enable UART4 clock */
		__HAL_RCC_UART4_CLK_ENABLE(); 		
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
		/*##-3- Configure the NVIC for UART ########################################*/
		/* NVIC for UART4 */
		HAL_NVIC_SetPriority(UART4_IRQn, UART4_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(UART4_IRQn);
#endif /*BSP_CONFIG_USE_UART4*/
	}
	else if (huart->Instance == UART5)
	{	
#ifdef BSP_CONFIG_USE_UART5
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		UART5_TX_GPIO_CLK_ENABLE();
		UART5_RX_GPIO_CLK_ENABLE();
		/* Enable UART5 clock */
		__HAL_RCC_UART5_CLK_ENABLE(); 		
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
		/*##-3- Configure the NVIC for UART ########################################*/
		/* NVIC for UART5 */
		HAL_NVIC_SetPriority(UART5_IRQn, UART5_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(UART5_IRQn);
#endif /*BSP_CONFIG_USE_UART5*/
	}
	else if (huart->Instance == USART6)
	{	
#ifdef BSP_CONFIG_USE_UART6
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* Enable GPIO TX/RX clock */
		UART6_TX_GPIO_CLK_ENABLE();
		UART6_RX_GPIO_CLK_ENABLE();
		/* Enable USART6 clock */
		__HAL_RCC_USART6_CLK_ENABLE(); 		
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
		/*##-3- Configure the NVIC for UART ########################################*/
		/* NVIC for USART6 */
		HAL_NVIC_SetPriority(USART6_IRQn, UART6_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
#endif /*BSP_CONFIG_USE_UART6*/
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
	if (huart->Instance == USART1)
	{
#ifdef BSP_CONFIG_USE_UART1
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
#endif /*BSP_CONFIG_USE_UART1*/
	}
	else if (huart->Instance == USART2)
	{
#ifdef BSP_CONFIG_USE_UART2
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
#endif /*BSP_CONFIG_USE_UART2*/
	}
	else if (huart->Instance == UART4)
	{
#ifdef BSP_CONFIG_USE_UART4
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
#endif /*BSP_CONFIG_USE_UART4*/
	}
	else if (huart->Instance == UART5)
	{
#ifdef BSP_CONFIG_USE_UART5
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
#endif /*BSP_CONFIG_USE_UART5*/
	}
	else if (huart->Instance == USART6)
	{
#ifdef BSP_CONFIG_USE_UART6
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
#endif /*BSP_CONFIG_USE_UART6*/
	}
}

/**
  * @brief  Rx Transfer completed callback
  * @param  hUART: UART Handle. 
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *hUART)
{	
	if (hUART->Instance == USART1)
	{
#ifdef BSP_CONFIG_USE_UART1
#ifdef BSP_CONFIG_USE_FREERTOS
		if (uart1.rx_buffer_end)
		{
			if (uart1.rx_buffer[uart1.rx_buffer_idx] == *uart1.rx_buffer_end)
			{
				if (osMessagePut(uart1.rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
					uart1.rx_message_queue_full++;
			}
			else if (uart1.rx_buffer_idx == (uart1.rx_buffer_size - 1))
			{
				if (osMessagePut(uart1.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart1.rx_message_queue_full++;
			}
			else if (uart1.handle.ErrorCode != HAL_UART_ERROR_NONE)
			{
				if (osMessagePut(uart1.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart1.rx_message_queue_full++;
			}
			else
			{
				HAL_UART_Receive_IT(&uart1.handle, (uint8_t*)&uart1.rx_buffer[++uart1.rx_buffer_idx], 1);
			}
		}
		else
		{
			if (uart1.rx_buffer_idx == (uart1.rx_buffer_size - 1))
			{
				if (osMessagePut(uart1.rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
					uart1.rx_message_queue_full++;
			}
			else if (uart1.handle.ErrorCode != HAL_UART_ERROR_NONE)
			{
				if (osMessagePut(uart1.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart1.rx_message_queue_full++;
			}
			else
			{
				HAL_UART_Receive_IT(&uart1.handle, (uint8_t*)&uart1.rx_buffer[++uart1.rx_buffer_idx], 1);
			}
		}
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/
	}
	else if (hUART->Instance == USART2)
	{
#ifdef BSP_CONFIG_USE_UART2
#ifdef BSP_CONFIG_USE_FREERTOS
		if (uart2.rx_buffer_end)
		{
			if (uart2.rx_buffer[uart2.rx_buffer_idx] == *uart2.rx_buffer_end)
			{
				if (osMessagePut(uart2.rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
					uart2.rx_message_queue_full++;
			}
			else if (uart2.rx_buffer_idx == (uart2.rx_buffer_size - 1))
			{
				if (osMessagePut(uart2.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart2.rx_message_queue_full++;
			}
			else if (uart2.handle.ErrorCode != HAL_UART_ERROR_NONE)
			{
				if (osMessagePut(uart2.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart2.rx_message_queue_full++;
			}
			else
			{
				HAL_UART_Receive_IT(&uart2.handle, (uint8_t*)&uart2.rx_buffer[++uart2.rx_buffer_idx], 1);
			}
		}
		else
		{
			if (uart2.rx_buffer_idx == (uart2.rx_buffer_size - 1))
			{
				if (osMessagePut(uart2.rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
					uart2.rx_message_queue_full++;
			}
			else if (uart2.handle.ErrorCode != HAL_UART_ERROR_NONE)
			{
				if (osMessagePut(uart2.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart2.rx_message_queue_full++;
			}
			else
			{
				HAL_UART_Receive_IT(&uart2.handle, (uint8_t*)&uart2.rx_buffer[++uart2.rx_buffer_idx], 1);
			}
		}
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART2*/
	}
	else if (hUART->Instance == UART4)
	{
#ifdef BSP_CONFIG_USE_UART4
#ifdef BSP_CONFIG_USE_FREERTOS
		if (uart4.rx_buffer_end)
		{
			if (uart4.rx_buffer[uart4.rx_buffer_idx] == *uart4.rx_buffer_end)
			{
				if (osMessagePut(uart4.rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
					uart4.rx_message_queue_full++;
			}
			else if (uart4.rx_buffer_idx == (uart4.rx_buffer_size - 1))
			{
				if (osMessagePut(uart4.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart4.rx_message_queue_full++;
			}
			else if (uart4.handle.ErrorCode != HAL_UART_ERROR_NONE)
			{
				if (osMessagePut(uart4.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart4.rx_message_queue_full++;
			}
			else
			{
				HAL_UART_Receive_IT(&uart4.handle, (uint8_t*)&uart4.rx_buffer[++uart4.rx_buffer_idx], 1);
			}
		}
		else
		{
			if (uart4.rx_buffer_idx == (uart4.rx_buffer_size - 1))
			{
				if (osMessagePut(uart4.rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
					uart4.rx_message_queue_full++;
			}
			else if (uart4.handle.ErrorCode != HAL_UART_ERROR_NONE)
			{
				if (osMessagePut(uart4.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart4.rx_message_queue_full++;
			}
			else
			{
				HAL_UART_Receive_IT(&uart4.handle, (uint8_t*)&uart4.rx_buffer[++uart4.rx_buffer_idx], 1);
			}
		}
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART4*/
	}
	else if (hUART->Instance == UART5)
	{
#ifdef BSP_CONFIG_USE_UART5
#ifdef BSP_CONFIG_USE_FREERTOS
		if (uart5.rx_buffer_end)
		{
			if (uart5.rx_buffer[uart5.rx_buffer_idx] == *uart5.rx_buffer_end)
			{
				if (osMessagePut(uart5.rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
					uart5.rx_message_queue_full++;
			}
			else if (uart5.rx_buffer_idx == (uart5.rx_buffer_size - 1))
			{
				if (osMessagePut(uart5.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart5.rx_message_queue_full++;
			}
			else if (uart5.handle.ErrorCode != HAL_UART_ERROR_NONE)
			{
				if (osMessagePut(uart5.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart5.rx_message_queue_full++;
			}
			else
			{
				HAL_UART_Receive_IT(&uart5.handle, (uint8_t*)&uart5.rx_buffer[++uart5.rx_buffer_idx], 1);
			}
		}
		else
		{
			if (uart5.rx_buffer_idx == (uart5.rx_buffer_size - 1))
			{
				if (osMessagePut(uart5.rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
					uart5.rx_message_queue_full++;
			}
			else if (uart5.handle.ErrorCode != HAL_UART_ERROR_NONE)
			{
				if (osMessagePut(uart5.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart5.rx_message_queue_full++;
			}
			else
			{
				HAL_UART_Receive_IT(&uart5.handle, (uint8_t*)&uart5.rx_buffer[++uart5.rx_buffer_idx], 1);
			}
		}
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART5*/
	}
	else if (hUART->Instance == USART6)
	{
#ifdef BSP_CONFIG_USE_UART6
#ifdef BSP_CONFIG_USE_FREERTOS
		if (uart6.rx_buffer_end)
		{
			if (uart6.rx_buffer[uart6.rx_buffer_idx] == *uart6.rx_buffer_end)
			{
				if (osMessagePut(uart6.rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
					uart6.rx_message_queue_full++;
			}
			else if (uart6.rx_buffer_idx == (uart6.rx_buffer_size - 1))
			{
				if (osMessagePut(uart6.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart6.rx_message_queue_full++;
			}
			else if (uart6.handle.ErrorCode != HAL_UART_ERROR_NONE)
			{
				if (osMessagePut(uart6.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart6.rx_message_queue_full++;
			}
			else
			{
				HAL_UART_Receive_IT(&uart6.handle, (uint8_t*)&uart6.rx_buffer[++uart6.rx_buffer_idx], 1);
			}
		}
		else
		{
			if (uart6.rx_buffer_idx == (uart6.rx_buffer_size - 1))
			{
				if (osMessagePut(uart6.rx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
					uart6.rx_message_queue_full++;
			}
			else if (uart6.handle.ErrorCode != HAL_UART_ERROR_NONE)
			{
				if (osMessagePut(uart6.rx_message_queue_id, UART_EVENT_ERROR, 0) != osOK)
					uart6.rx_message_queue_full++;
			}
			else
			{
				HAL_UART_Receive_IT(&uart6.handle, (uint8_t*)&uart6.rx_buffer[++uart6.rx_buffer_idx], 1);
			}
		}
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART6*/
	}
}

/**
  * @brief  Tx Transfer completed callback
  * @param  hUART: UART Handle. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *hUART)
{
	if (hUART->Instance == USART1)
	{
#ifdef BSP_CONFIG_USE_UART1
#ifdef BSP_CONFIG_USE_FREERTOS
		if (osMessagePut(uart1.tx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
			uart1.tx_message_queue_full++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/
	}
	else 	if (hUART->Instance == USART2)
	{
#ifdef BSP_CONFIG_USE_UART2
#ifdef BSP_CONFIG_USE_FREERTOS
		if (osMessagePut(uart2.tx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
			uart2.tx_message_queue_full++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART2*/
	}
	else 	if (hUART->Instance == UART4)
	{
#ifdef BSP_CONFIG_USE_UART4
#ifdef BSP_CONFIG_USE_FREERTOS
		if (osMessagePut(uart4.tx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
			uart4.tx_message_queue_full++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART4*/
	}
	else 	if (hUART->Instance == UART5)
	{
#ifdef BSP_CONFIG_USE_UART5
#ifdef BSP_CONFIG_USE_FREERTOS
		if (osMessagePut(uart5.tx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
			uart5.tx_message_queue_full++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART5*/
	}
	else 	if (hUART->Instance == USART6)
	{
#ifdef BSP_CONFIG_USE_UART6
#ifdef BSP_CONFIG_USE_FREERTOS
		if (osMessagePut(uart6.tx_message_queue_id, UART_EVENT_COMPLETE, 0) != osOK)
			uart6.tx_message_queue_full++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART6*/
	}
}

/**
  * @brief  Transfer error callback
  * @param  hUART: UART Handle
  * @retval None
  */
 void HAL_UART_ErrorCallback(UART_HandleTypeDef *hUART)
{
	if (hUART->Instance == USART1)
	{
#ifdef BSP_CONFIG_USE_UART1
#ifdef BSP_CONFIG_USE_FREERTOS
		uart1.error++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/
	}
	else 	if (hUART->Instance == USART2)
	{
#ifdef BSP_CONFIG_USE_UART2
#ifdef BSP_CONFIG_USE_FREERTOS
		uart2.error++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART2*/
	}
	else 	if (hUART->Instance == UART4)
	{
#ifdef BSP_CONFIG_USE_UART4
#ifdef BSP_CONFIG_USE_FREERTOS
		uart4.error++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART4*/
	}
	else 	if (hUART->Instance == UART5)
	{
#ifdef BSP_CONFIG_USE_UART5
#ifdef BSP_CONFIG_USE_FREERTOS
		uart5.error++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART5*/
	}
	else 	if (hUART->Instance == USART6)
	{
#ifdef BSP_CONFIG_USE_UART6
#ifdef BSP_CONFIG_USE_FREERTOS
		uart6.error++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART6*/
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
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(uart1.rx_mutex_id, osWaitForever);
	osMutexWait(uart1.tx_mutex_id, osWaitForever);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_UART_DeInit(&uart1.handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(uart1.rx_mutex_id);
	osMutexDelete(uart1.rx_mutex_id);
	osMutexRelease(uart1.tx_mutex_id);
	osMutexDelete(uart1.tx_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/
	
#ifdef BSP_CONFIG_USE_UART2
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(uart2.rx_mutex_id, osWaitForever);
	osMutexWait(uart2.tx_mutex_id, osWaitForever);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_UART_DeInit(&uart2.handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(uart2.rx_mutex_id);
	osMutexDelete(uart2.rx_mutex_id);
	osMutexRelease(uart2.tx_mutex_id);
	osMutexDelete(uart2.tx_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART2*/

#ifdef BSP_CONFIG_USE_UART4
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(uart4.rx_mutex_id, osWaitForever);
	osMutexWait(uart4.tx_mutex_id, osWaitForever);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_UART_DeInit(&uart4.handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(uart4.rx_mutex_id);
	osMutexDelete(uart4.rx_mutex_id);
	osMutexRelease(uart4.tx_mutex_id);
	osMutexDelete(uart4.tx_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART4*/

#ifdef BSP_CONFIG_USE_UART5
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(uart5.rx_mutex_id, osWaitForever);
	osMutexWait(uart5.tx_mutex_id, osWaitForever);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_UART_DeInit(&uart5.handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(uart5.rx_mutex_id);
	osMutexDelete(uart5.rx_mutex_id);
	osMutexRelease(uart5.tx_mutex_id);
	osMutexDelete(uart5.tx_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART5*/

#ifdef BSP_CONFIG_USE_UART6
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(uart6.rx_mutex_id, osWaitForever);
	osMutexWait(uart6.tx_mutex_id, osWaitForever);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_UART_DeInit(&uart6.handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(uart6.rx_mutex_id);
	osMutexDelete(uart6.rx_mutex_id);
	osMutexRelease(uart6.tx_mutex_id);
	osMutexDelete(uart6.tx_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
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
		return -1;
	
	if(HAL_UART_Transmit_IT(&uart->handle, (uint8_t*)Buffer, Size) != HAL_OK)
		goto error;
	
	evt = osMessageGet(uart->tx_message_queue_id, UART_TIMEOUT);
	if (evt.status != osEventMessage)
		goto abort;
	
	if (evt.value.v != UART_EVENT_COMPLETE)
		goto abort;
	
	osMutexRelease( uart->tx_mutex_id );	
	return 0;
	
abort:
	HAL_UART_AbortTransmit_IT(&uart->handle);
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
		return -1;
	
	uart->rx_buffer = (uint8_t*)Buffer;
	uart->rx_buffer_size = Size;
	uart->rx_buffer_idx = 0;
	uart->rx_buffer_end = NULL;

	if(HAL_UART_Receive_IT(&uart->handle, uart->rx_buffer, 1) != HAL_OK)
		goto error;

	evt = osMessageGet(uart->rx_message_queue_id, UART_TIMEOUT);
	if (evt.status != osEventMessage)
		goto abort;
	
	if (evt.value.v != UART_EVENT_COMPLETE)
		goto abort;

	osMutexRelease( uart->rx_mutex_id );	
	return 0;
	
abort:
	HAL_UART_AbortReceive_IT(&uart->handle);
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
	
	if( osMutexWait( uart->rx_mutex_id, Timeout ) != osOK )
		return -1;		
	
	uart->rx_buffer = (uint8_t*)Buffer;
	uart->rx_buffer_size = Size;
	uart->rx_buffer_idx = 0;
	uart->rx_buffer_end = (uint8_t*)&End;
	
	if(HAL_UART_Receive_IT(&uart->handle, uart->rx_buffer, 1) != HAL_OK)
		goto error;
	
	evt = osMessageGet(uart->rx_message_queue_id, Timeout);
	if (evt.status != osEventMessage)
		goto abort;
	
	if (evt.value.v != UART_EVENT_COMPLETE)
		goto abort;
	
	*Len = uart->rx_buffer_idx + 1;
	
	osMutexRelease( uart->rx_mutex_id );	
	return 0;

abort:
	HAL_UART_AbortReceive_IT(&uart->handle);
error:
	osMutexRelease( uart->rx_mutex_id );
	return -1;
#else /*BSP_CONFIG_USE_FREERTOS*/
	return -1;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}
