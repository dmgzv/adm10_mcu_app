#include "uart.h"

#ifdef BSP_CONFIG_USE_FREERTOS
#define UART_MESSAGE_QUEUE_SIZE (1)
#define UART_EVENT_COMPLETE			(0)
#define UART_EVENT_ERROR				(0xFFFFFFFF)
#define UART_TIMEOUT 						(1000)
#else /*BSP_CONFIG_USE_FREERTOS*/
#define UART_TIMEOUT 						(1000)
#endif /*BSP_CONFIG_USE_FREERTOS*/

#ifdef BSP_CONFIG_USE_UART1
static UART_HandleTypeDef uart1_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId uart1_rx_mutex_id;
static osMutexId uart1_tx_mutex_id;
static osMessageQId uart1_rx_message_queue_id;
static osMessageQId uart1_tx_message_queue_id;
osMutexDef(uart1_rx);
osMutexDef(uart1_tx);
osMessageQDef(uart1_rx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
osMessageQDef(uart1_tx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART2
static UART_HandleTypeDef uart2_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId uart2_rx_mutex_id;
static osMutexId uart2_tx_mutex_id;
static osMessageQId uart2_rx_message_queue_id;
static osMessageQId uart2_tx_message_queue_id;
osMutexDef(uart2_rx);
osMutexDef(uart2_tx);
osMessageQDef(uart2_rx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
osMessageQDef(uart2_tx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART3
#error UART3 support is not present in bsp driver
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART4
static UART_HandleTypeDef uart4_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId uart4_rx_mutex_id;
static osMutexId uart4_tx_mutex_id;
static osMessageQId uart4_rx_message_queue_id;
static osMessageQId uart4_tx_message_queue_id;
osMutexDef(uart4_rx);
osMutexDef(uart4_tx);
osMessageQDef(uart4_rx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
osMessageQDef(uart4_tx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART5
static UART_HandleTypeDef uart5_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId uart5_rx_mutex_id;
static osMutexId uart5_tx_mutex_id;
static osMessageQId uart5_rx_message_queue_id;
static osMessageQId uart5_tx_message_queue_id;
osMutexDef(uart5_rx);
osMutexDef(uart5_tx);
osMessageQDef(uart5_rx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
osMessageQDef(uart5_tx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_UART6
static UART_HandleTypeDef uart6_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId uart6_rx_mutex_id;
static osMutexId uart6_tx_mutex_id;
static osMessageQId uart6_rx_message_queue_id;
static osMessageQId uart6_tx_message_queue_id;
osMutexDef(uart6_rx);
osMutexDef(uart6_tx);
osMessageQDef(uart6_rx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
osMessageQDef(uart6_tx, UART_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/

#ifdef BSP_CONFIG_USE_FREERTOS
static int32_t UART_GetHandle( USART_TypeDef* Instance, UART_HandleTypeDef **uart_handle, \
	osMutexId *uart_rx_mutex_id, osMessageQId *uart_rx_message_queue_id, \
	osMutexId *uart_tx_mutex_id, osMessageQId *uart_tx_message_queue_id)
#else
static int32_t UART_GetHandle( USART_TypeDef* Instance, UART_HandleTypeDef **Handle)
#endif /*BSP_CONFIG_USE_FREERTOS*/
{	
	if (Instance == USART1)
	{
#ifdef BSP_CONFIG_USE_UART1
		*uart_handle = &uart1_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*uart_rx_mutex_id = uart1_rx_mutex_id;
		*uart_rx_message_queue_id = uart1_rx_message_queue_id;
		*uart_tx_mutex_id = uart1_tx_mutex_id;
		*uart_tx_message_queue_id = uart1_tx_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#else
		return -1;
#endif /*BSP_CONFIG_USE_UART1*/
	}
	else if (Instance == USART2)
	{
#ifdef BSP_CONFIG_USE_UART2
		*uart_handle = &uart2_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*uart_rx_mutex_id = uart2_rx_mutex_id;
		*uart_rx_message_queue_id = uart2_rx_message_queue_id;
		*uart_tx_mutex_id = uart2_tx_mutex_id;
		*uart_tx_message_queue_id = uart2_tx_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#else
		return -1;
#endif /*BSP_CONFIG_USE_UART2*/
	}
	else if (Instance == UART4)
	{
#ifdef BSP_CONFIG_USE_UART4
		*uart_handle = &uart4_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*uart_rx_mutex_id = uart4_rx_mutex_id;
		*uart_rx_message_queue_id = uart4_rx_message_queue_id;
		*uart_tx_mutex_id = uart4_tx_mutex_id;
		*uart_tx_message_queue_id = uart4_tx_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#else
		return -1;
#endif /*BSP_CONFIG_USE_UART4*/
	}
	else if (Instance == UART5)
	{
#ifdef BSP_CONFIG_USE_UART5
		*uart_handle = &uart5_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*uart_rx_mutex_id = uart5_rx_mutex_id;
		*uart_rx_message_queue_id = uart5_rx_message_queue_id;
		*uart_tx_mutex_id = uart5_tx_mutex_id;
		*uart_tx_message_queue_id = uart5_tx_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#else
		return -1;
#endif /*BSP_CONFIG_USE_UART5*/
	}
	else if (Instance == USART6)
	{
#ifdef BSP_CONFIG_USE_UART6
		*uart_handle = &uart6_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*uart_rx_mutex_id = uart6_rx_mutex_id;
		*uart_rx_message_queue_id = uart6_rx_message_queue_id;
		*uart_tx_mutex_id = uart6_tx_mutex_id;
		*uart_tx_message_queue_id = uart6_tx_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
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
  HAL_UART_IRQHandler(&uart1_handle);
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
  HAL_UART_IRQHandler(&uart2_handle);
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
  HAL_UART_IRQHandler(&uart4_handle);
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
  HAL_UART_IRQHandler(&uart5_handle);
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
  HAL_UART_IRQHandler(&uart6_handle);
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
		HAL_NVIC_SetPriority(USART1_IRQn, UART1_IRQ_PRIORITY, 1);
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
		HAL_NVIC_SetPriority(USART2_IRQn, UART2_IRQ_PRIORITY, 1);
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
		HAL_NVIC_SetPriority(USART6_IRQn, UART6_IRQ_PRIORITY, 1);
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
		osMessagePut(uart1_rx_message_queue_id, UART_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/
	}
	else if (hUART->Instance == USART2)
	{
#ifdef BSP_CONFIG_USE_UART2
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart2_rx_message_queue_id, UART_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART2*/
	}
	else if (hUART->Instance == UART4)
	{
#ifdef BSP_CONFIG_USE_UART4
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart4_rx_message_queue_id, UART_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART4*/
	}
	else if (hUART->Instance == UART5)
	{
#ifdef BSP_CONFIG_USE_UART5
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart5_rx_message_queue_id, UART_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART5*/
	}
	else if (hUART->Instance == USART6)
	{
#ifdef BSP_CONFIG_USE_UART6
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart6_rx_message_queue_id, UART_EVENT_COMPLETE, 0);
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
		osMessagePut(uart1_tx_message_queue_id, UART_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/
	}
	else 	if (hUART->Instance == USART2)
	{
#ifdef BSP_CONFIG_USE_UART2
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart2_tx_message_queue_id, UART_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART2*/
	}
	else 	if (hUART->Instance == UART4)
	{
#ifdef BSP_CONFIG_USE_UART4
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart4_tx_message_queue_id, UART_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART4*/
	}
	else 	if (hUART->Instance == UART5)
	{
#ifdef BSP_CONFIG_USE_UART5
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart5_tx_message_queue_id, UART_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART5*/
	}
	else 	if (hUART->Instance == USART6)
	{
#ifdef BSP_CONFIG_USE_UART6
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart6_tx_message_queue_id, UART_EVENT_COMPLETE, 0);
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
		osMessagePut(uart1_rx_message_queue_id, UART_EVENT_ERROR, 0);
		//osMessagePut(uart1_tx_message_queue_id, UART_EVENT_ERROR, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/
	}
	else 	if (hUART->Instance == USART2)
	{
#ifdef BSP_CONFIG_USE_UART2
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart2_rx_message_queue_id, UART_EVENT_ERROR, 0);
		//osMessagePut(uart2_tx_message_queue_id, UART_EVENT_ERROR, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART2*/
	}
	else 	if (hUART->Instance == UART4)
	{
#ifdef BSP_CONFIG_USE_UART4
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart4_rx_message_queue_id, UART_EVENT_ERROR, 0);
		//osMessagePut(uart4_tx_message_queue_id, UART_EVENT_ERROR, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART4*/
	}
	else 	if (hUART->Instance == UART5)
	{
#ifdef BSP_CONFIG_USE_UART5
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart5_rx_message_queue_id, UART_EVENT_ERROR, 0);
		//osMessagePut(uart5_tx_message_queue_id, UART_EVENT_ERROR, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART5*/
	}
	else 	if (hUART->Instance == USART6)
	{
#ifdef BSP_CONFIG_USE_UART6
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(uart6_rx_message_queue_id, UART_EVENT_ERROR, 0);
		//osMessagePut(uart6_tx_message_queue_id, UART_EVENT_ERROR, 0);
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
	uart1_handle.Instance          = USART1;  
	uart1_handle.Init.BaudRate     = UART1_BAUDRATE;
	uart1_handle.Init.WordLength   = UART1_WORDLENGTH;
	uart1_handle.Init.StopBits     = UART1_STOPBITS;
	uart1_handle.Init.Parity       = UART1_PARITY;
	uart1_handle.Init.HwFlowCtl    = UART1_HWCONTROL;
	uart1_handle.Init.Mode         = UART1_MODE;
	uart1_handle.Init.OverSampling = UART1_OVERSAMPLING;
		
	if(HAL_UART_Init(&uart1_handle) != HAL_OK)
	{
		result = -1;
	}
#ifdef BSP_CONFIG_USE_FREERTOS
	uart1_rx_mutex_id = osMutexCreate(osMutex(uart1_rx));
	uart1_tx_mutex_id = osMutexCreate(osMutex(uart1_tx));
	uart1_rx_message_queue_id = osMessageCreate (osMessageQ(uart1_rx), NULL);	
	uart1_tx_message_queue_id = osMessageCreate (osMessageQ(uart1_tx), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/
	
#ifdef BSP_CONFIG_USE_UART2
	uart2_handle.Instance          = USART2;  
	uart2_handle.Init.BaudRate     = UART2_BAUDRATE;
	uart2_handle.Init.WordLength   = UART2_WORDLENGTH;
	uart2_handle.Init.StopBits     = UART2_STOPBITS;
	uart2_handle.Init.Parity       = UART2_PARITY;
	uart2_handle.Init.HwFlowCtl    = UART2_HWCONTROL;
	uart2_handle.Init.Mode         = UART2_MODE;
	uart2_handle.Init.OverSampling = UART2_OVERSAMPLING;
		
	if(HAL_UART_Init(&uart2_handle) != HAL_OK)
	{
		result = -1;
	}
#ifdef BSP_CONFIG_USE_FREERTOS
	uart2_rx_mutex_id = osMutexCreate(osMutex(uart2_rx));
	uart2_tx_mutex_id = osMutexCreate(osMutex(uart2_tx));
	uart2_rx_message_queue_id = osMessageCreate (osMessageQ(uart2_rx), NULL);	
	uart2_tx_message_queue_id = osMessageCreate (osMessageQ(uart2_tx), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART2*/

#ifdef BSP_CONFIG_USE_UART4
	uart4_handle.Instance          = UART4;  
	uart4_handle.Init.BaudRate     = UART4_BAUDRATE;
	uart4_handle.Init.WordLength   = UART4_WORDLENGTH;
	uart4_handle.Init.StopBits     = UART4_STOPBITS;
	uart4_handle.Init.Parity       = UART4_PARITY;
	uart4_handle.Init.HwFlowCtl    = UART4_HWCONTROL;
	uart4_handle.Init.Mode         = UART4_MODE;
	uart4_handle.Init.OverSampling = UART4_OVERSAMPLING;
		
	if(HAL_UART_Init(&uart4_handle) != HAL_OK)
	{
		result = -1;
	}
#ifdef BSP_CONFIG_USE_FREERTOS
	uart4_rx_mutex_id = osMutexCreate(osMutex(uart4_rx));
	uart4_tx_mutex_id = osMutexCreate(osMutex(uart4_tx));
	uart4_rx_message_queue_id = osMessageCreate (osMessageQ(uart4_rx), NULL);	
	uart4_tx_message_queue_id = osMessageCreate (osMessageQ(uart4_tx), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART4*/
	
#ifdef BSP_CONFIG_USE_UART5
	uart5_handle.Instance          = UART5;  
	uart5_handle.Init.BaudRate     = UART5_BAUDRATE;
	uart5_handle.Init.WordLength   = UART5_WORDLENGTH;
	uart5_handle.Init.StopBits     = UART5_STOPBITS;
	uart5_handle.Init.Parity       = UART5_PARITY;
	uart5_handle.Init.HwFlowCtl    = UART5_HWCONTROL;
	uart5_handle.Init.Mode         = UART5_MODE;
	uart5_handle.Init.OverSampling = UART5_OVERSAMPLING;
		
	if(HAL_UART_Init(&uart5_handle) != HAL_OK)
	{
		result = -1;
	}
#ifdef BSP_CONFIG_USE_FREERTOS
	uart5_rx_mutex_id = osMutexCreate(osMutex(uart5_rx));
	uart5_tx_mutex_id = osMutexCreate(osMutex(uart5_tx));
	uart5_rx_message_queue_id = osMessageCreate (osMessageQ(uart5_rx), NULL);	
	uart5_tx_message_queue_id = osMessageCreate (osMessageQ(uart5_tx), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART5*/
	
#ifdef BSP_CONFIG_USE_UART6
	uart6_handle.Instance          = USART6;  
	uart6_handle.Init.BaudRate     = UART6_BAUDRATE;
	uart6_handle.Init.WordLength   = UART6_WORDLENGTH;
	uart6_handle.Init.StopBits     = UART6_STOPBITS;
	uart6_handle.Init.Parity       = UART6_PARITY;
	uart6_handle.Init.HwFlowCtl    = UART6_HWCONTROL;
	uart6_handle.Init.Mode         = UART6_MODE;
	uart6_handle.Init.OverSampling = UART6_OVERSAMPLING;
		
	if(HAL_UART_Init(&uart6_handle) != HAL_OK)
	{
		result = -1;
	}
#ifdef BSP_CONFIG_USE_FREERTOS
	uart6_rx_mutex_id = osMutexCreate(osMutex(uart6_rx));
	uart6_tx_mutex_id = osMutexCreate(osMutex(uart6_tx));
	uart6_rx_message_queue_id = osMessageCreate (osMessageQ(uart6_rx), NULL);	
	uart6_tx_message_queue_id = osMessageCreate (osMessageQ(uart6_tx), NULL);
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
	osMutexWait(uart1_rx_mutex_id, UART_TIMEOUT);
	osMutexWait(uart1_tx_mutex_id, UART_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_UART_DeInit(&uart1_handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(uart1_rx_mutex_id);
	osMutexDelete(uart1_rx_mutex_id);
	osMutexRelease(uart1_tx_mutex_id);
	osMutexDelete(uart1_tx_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART1*/
	
#ifdef BSP_CONFIG_USE_UART2
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(uart2_rx_mutex_id, UART_TIMEOUT);
	osMutexWait(uart2_tx_mutex_id, UART_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_UART_DeInit(&uart2_handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(uart2_rx_mutex_id);
	osMutexDelete(uart2_rx_mutex_id);
	osMutexRelease(uart2_tx_mutex_id);
	osMutexDelete(uart2_tx_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART2*/

#ifdef BSP_CONFIG_USE_UART4
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(uart4_rx_mutex_id, UART_TIMEOUT);
	osMutexWait(uart4_tx_mutex_id, UART_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_UART_DeInit(&uart4_handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(uart4_rx_mutex_id);
	osMutexDelete(uart4_rx_mutex_id);
	osMutexRelease(uart4_tx_mutex_id);
	osMutexDelete(uart4_tx_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART4*/

#ifdef BSP_CONFIG_USE_UART5
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(uart5_rx_mutex_id, UART_TIMEOUT);
	osMutexWait(uart5_tx_mutex_id, UART_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_UART_DeInit(&uart5_handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(uart5_rx_mutex_id);
	osMutexDelete(uart5_rx_mutex_id);
	osMutexRelease(uart5_tx_mutex_id);
	osMutexDelete(uart5_tx_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_UART5*/

#ifdef BSP_CONFIG_USE_UART6
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(uart6_rx_mutex_id, UART_TIMEOUT);
	osMutexWait(uart6_tx_mutex_id, UART_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_UART_DeInit(&uart6_handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(uart6_rx_mutex_id);
	osMutexDelete(uart6_rx_mutex_id);
	osMutexRelease(uart6_tx_mutex_id);
	osMutexDelete(uart6_tx_mutex_id);
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
	UART_HandleTypeDef *uart_handle_p;
	osEvent evt;	
	osMutexId uart_rx_mutex_id, uart_tx_mutex_id;
	osMessageQId uart_rx_message_queue_id, uart_tx_message_queue_id;
	
	if (UART_GetHandle(Instance, &uart_handle_p, &uart_rx_mutex_id, &uart_rx_message_queue_id, &uart_tx_mutex_id, &uart_tx_message_queue_id))
		return -1;
	
	if( osMutexWait( uart_tx_mutex_id, UART_TIMEOUT ) != osOK )
		return -1;

	if(HAL_UART_Transmit_IT(uart_handle_p, (uint8_t*)Buffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(uart_tx_message_queue_id, osWaitForever);
	if (evt.status != osEventMessage)
	{
		HAL_UART_AbortTransmit_IT(uart_handle_p);
		goto error;
	}
	
	if (evt.value.v != UART_EVENT_COMPLETE)
		goto error;

	osMutexRelease( uart_tx_mutex_id );	
	return 0;

error:
	osMutexRelease( uart_tx_mutex_id );
	return -1;
#else /*BSP_CONFIG_USE_FREERTOS*/
	UART_HandleTypeDef *uart_handle_p;
	
	if (UART_GetHandle(Instance, &uart_handle_p))
		return -1;
	
	if(HAL_UART_Transmit(uart_handle_p, (uint8_t*)Buffer, Size, UART_TIMEOUT) != HAL_OK)
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
	UART_HandleTypeDef *uart_handle_p;
	osEvent evt;	
	osMutexId uart_rx_mutex_id, uart_tx_mutex_id;
	osMessageQId uart_rx_message_queue_id, uart_tx_message_queue_id;
	
	if (UART_GetHandle(Instance, &uart_handle_p, &uart_rx_mutex_id, &uart_rx_message_queue_id, &uart_tx_mutex_id, &uart_tx_message_queue_id))
		return -1;
	
	if( osMutexWait( uart_rx_mutex_id, UART_TIMEOUT ) != osOK )
		return -1;

	if(HAL_UART_Receive_IT(uart_handle_p, (uint8_t*)Buffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(uart_rx_message_queue_id, UART_TIMEOUT);
	if (evt.status != osEventMessage)
	{
		HAL_UART_AbortReceive_IT(uart_handle_p);
		goto error;
	}
	
	if (evt.value.v != UART_EVENT_COMPLETE)
		goto error;

	osMutexRelease( uart_rx_mutex_id );	
	return 0;

error:
	osMutexRelease( uart_rx_mutex_id );
	return -1;
#else /*BSP_CONFIG_USE_FREERTOS*/
	UART_HandleTypeDef *uart_handle_p;
	
	if (UART_GetHandle(Instance, &uart_handle_p))
		return -1;
	
	if(HAL_UART_Receive(uart_handle_p, (uint8_t*)Buffer, Size, UART_TIMEOUT) != HAL_OK)
		return -1;
	return 0;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}
