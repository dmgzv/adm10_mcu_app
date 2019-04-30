#include "i2c.h"

#ifdef BSP_CONFIG_USE_FREERTOS
	#define I2C_MESSAGE_QUEUE_SIZE	(1)
	#define I2C_EVENT_COMPLETE		(0)
	#define I2C_EVENT_ERROR			(0xFFFFFFFF)
	#define I2C_TIMEOUT 			(1000)/*(osWaitForever)*/
#else
	#define I2C_TIMEOUT 			(1000)
#endif /*BSP_CONFIG_USE_FREERTOS*/

#ifdef BSP_CONFIG_USE_I2C1
static I2C_HandleTypeDef i2c1_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId i2c1_mutex_id;
osMutexDef(i2c1);
static osMessageQId i2c1_message_queue_id;
osMessageQDef(i2c1, I2C_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C1*/

#ifdef BSP_CONFIG_USE_I2C2
static I2C_HandleTypeDef i2c2_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId i2c2_mutex_id;
osMutexDef(i2c2);
static osMessageQId i2c2_message_queue_id;
osMessageQDef(i2c2, I2C_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C2*/

#ifdef BSP_CONFIG_USE_I2C3
static I2C_HandleTypeDef i2c3_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId i2c3_mutex_id;
osMutexDef(i2c3);
static osMessageQId i2c3_message_queue_id;
osMessageQDef(i2c3, I2C_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C3*/

#ifdef BSP_CONFIG_USE_FREERTOS
static int32_t I2C_GetHandle( I2C_TypeDef* Instance, I2C_HandleTypeDef **i2c_handle, osMutexId *i2c_mutex_id, osMessageQId *i2c_message_queue_id)
#else
static int32_t I2C_GetHandle( I2C_TypeDef* Instance, I2C_HandleTypeDef **Handle)
#endif /*BSP_CONFIG_USE_FREERTOS*/
{	
	if (Instance == I2C1)
	{
#ifdef BSP_CONFIG_USE_I2C1
		*i2c_handle = &i2c1_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*i2c_mutex_id = i2c1_mutex_id;
		*i2c_message_queue_id = i2c1_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#else
		return -1;
#endif /*BSP_CONFIG_USE_I2C1*/
	}
	else if (Instance == I2C2)
	{
#ifdef BSP_CONFIG_USE_I2C2
		*i2c_handle = &i2c2_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*i2c_mutex_id = i2c2_mutex_id;
		*i2c_message_queue_id = i2c2_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#else
		return -1;
#endif /*BSP_CONFIG_USE_I2C2*/
	}
	else if (Instance == I2C3)
	{
#ifdef BSP_CONFIG_USE_I2C3
		*i2c_handle = &i2c3_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*i2c_mutex_id = i2c3_mutex_id;
		*i2c_message_queue_id = i2c3_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#else
		return -1;
#endif /*BSP_CONFIG_USE_I2C3*/
	}
	else
	{
		return -1;
	}
	return 0;
}

#ifdef BSP_CONFIG_USE_I2C1
/**
  * @brief  This function Handles I2C1 event interrupt request.  
  * @param  None
  * @retval None   
  */
void I2C1_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&i2c1_handle);
}
/**
  * @brief  This function Handles I2C1 error interrupt request.
  * @param  None
  * @retval None
  */
void I2C1_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&i2c1_handle);
}
#endif /*BSP_CONFIG_USE_I2C1*/

#ifdef BSP_CONFIG_USE_I2C2
/**
  * @brief  This function Handles I2C2 event interrupt request.  
  * @param  None
  * @retval None   
  */
void I2C2_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&i2c2_handle);
}
/**
  * @brief  This function Handles I2C2 error interrupt request.
  * @param  None
  * @retval None
  */
void I2C2_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&i2c2_handle);
}
#endif /*BSP_CONFIG_USE_I2C2*/

#ifdef BSP_CONFIG_USE_I2C3
/**
  * @brief  This function Handles I2C3 event interrupt request.  
  * @param  None
  * @retval None   
  */
void I2C3_EV_IRQHandler(void)
{
  HAL_I2C_EV_IRQHandler(&i2c3_handle);
}
/**
  * @brief  This function Handles I2C3 error interrupt request.
  * @param  None
  * @retval None
  */
void I2C3_ER_IRQHandler(void)
{
  HAL_I2C_ER_IRQHandler(&i2c3_handle);
}
#endif /*BSP_CONFIG_USE_I2C3*/

/**
  * @brief I2C MSP Initialization 
  *        This function configures the hardware resources: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param hi2c: I2C Handle pointer
  * @retval None
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{  
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	if (hi2c->Instance == I2C1)
	{
#ifdef BSP_CONFIG_USE_I2C1
		/*##-1- Enable GPIO Clocks #################################################*/
		/* Enable GPIO TX/RX clock */
		I2C1_SCL_GPIO_CLK_ENABLE();
		I2C1_SDA_GPIO_CLK_ENABLE();  
		/*##-2- Configure peripheral GPIO ##########################################*/  
		/* I2C TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = I2C1_SCL_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = I2C1_SCL_AF;
		HAL_GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStruct);    
		/* I2C RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = I2C1_SDA_PIN;
		GPIO_InitStruct.Alternate = I2C1_SDA_AF;
		HAL_GPIO_Init(I2C1_SDA_GPIO_PORT, &GPIO_InitStruct);
		/*##-3- Enable I2C peripherals Clock #######################################*/
		/* Enable I2C1 clock */
		__HAL_RCC_I2C1_CLK_ENABLE();      
		/*##-4- Configure the NVIC for I2C #########################################*/   
		/* NVIC for I2C1 */
		HAL_NVIC_SetPriority(I2C1_ER_IRQn, I2C1_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
		HAL_NVIC_SetPriority(I2C1_EV_IRQn, I2C1_IRQ_PRIORITY + 1, 0);
		HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
#endif /*BSP_CONFIG_USE_I2C1*/
	}
	else if (hi2c->Instance == I2C2)
	{
#ifdef BSP_CONFIG_USE_I2C2
		/*##-1- Enable GPIO Clocks #################################################*/
		/* Enable GPIO TX/RX clock */
		I2C2_SCL_GPIO_CLK_ENABLE();
		I2C2_SDA_GPIO_CLK_ENABLE();  
		/*##-2- Configure peripheral GPIO ##########################################*/  
		/* I2C TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = I2C2_SCL_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = I2C2_SCL_AF;
		HAL_GPIO_Init(I2C2_SCL_GPIO_PORT, &GPIO_InitStruct);    
		/* I2C RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = I2C2_SDA_PIN;
		GPIO_InitStruct.Alternate = I2C2_SDA_AF;
		HAL_GPIO_Init(I2C2_SDA_GPIO_PORT, &GPIO_InitStruct);
		/*##-3- Enable I2C peripherals Clock #######################################*/
		/* Enable I2C2 clock */
		__HAL_RCC_I2C2_CLK_ENABLE();      
		/*##-4- Configure the NVIC for I2C #########################################*/   
		/* NVIC for I2C2 */
		HAL_NVIC_SetPriority(I2C2_ER_IRQn, I2C2_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
		HAL_NVIC_SetPriority(I2C2_EV_IRQn, I2C2_IRQ_PRIORITY + 1, 0);
		HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
#endif /*BSP_CONFIG_USE_I2C2*/
	}
	else if (hi2c->Instance == I2C3)
	{
#ifdef BSP_CONFIG_USE_I2C3
		/*##-1- Enable GPIO Clocks #################################################*/
		/* Enable GPIO TX/RX clock */
		I2C3_SCL_GPIO_CLK_ENABLE();
		I2C3_SDA_GPIO_CLK_ENABLE();  
		/*##-2- Configure peripheral GPIO ##########################################*/  
		/* I2C TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = I2C3_SCL_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = I2C3_SCL_AF;
		HAL_GPIO_Init(I2C3_SCL_GPIO_PORT, &GPIO_InitStruct);    
		/* I2C RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = I2C3_SDA_PIN;
		GPIO_InitStruct.Alternate = I2C3_SDA_AF;
		HAL_GPIO_Init(I2C3_SDA_GPIO_PORT, &GPIO_InitStruct);
		/*##-3- Enable I2C peripherals Clock #######################################*/
		/* Enable I2C3 clock */
		__HAL_RCC_I2C3_CLK_ENABLE();      
		/*##-4- Configure the NVIC for I2C #########################################*/   
		/* NVIC for I2C3 */
		HAL_NVIC_SetPriority(I2C3_ER_IRQn, I2C3_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
		HAL_NVIC_SetPriority(I2C3_EV_IRQn, I2C3_IRQ_PRIORITY + 1, 0);
		HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
#endif /*BSP_CONFIG_USE_I2C3*/
	}
}

/**
  * @brief I2C MSP De-Initialization 
  *        This function frees the hardware resources:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hi2c: I2C Handle pointer
  * @retval None
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == I2C1)
	{
#ifdef BSP_CONFIG_USE_I2C1
		/*##-1- Reset peripherals ##################################################*/
		__HAL_RCC_I2C1_FORCE_RESET();
		__HAL_RCC_I2C1_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks ################################*/
		/* Configure I2C Tx as alternate function  */
		HAL_GPIO_DeInit(I2C1_SCL_GPIO_PORT, I2C1_SCL_PIN);
		/* Configure I2C Rx as alternate function  */
		HAL_GPIO_DeInit(I2C1_SDA_GPIO_PORT, I2C1_SDA_PIN);
		/*##-3- Disable the NVIC for I2C ###########################################*/
		HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
		HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
#endif /*BSP_CONFIG_USE_I2C1*/
	}
	else if (hi2c->Instance == I2C2)
	{
#ifdef BSP_CONFIG_USE_I2C2
		/*##-1- Reset peripherals ##################################################*/
		__HAL_RCC_I2C2_FORCE_RESET();
		__HAL_RCC_I2C2_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks ################################*/
		/* Configure I2C Tx as alternate function  */
		HAL_GPIO_DeInit(I2C2_SCL_GPIO_PORT, I2C2_SCL_PIN);
		/* Configure I2C Rx as alternate function  */
		HAL_GPIO_DeInit(I2C2_SDA_GPIO_PORT, I2C2_SDA_PIN);
		/*##-3- Disable the NVIC for I2C ###########################################*/
		HAL_NVIC_DisableIRQ(I2C2_ER_IRQn);
		HAL_NVIC_DisableIRQ(I2C2_EV_IRQn);
#endif /*BSP_CONFIG_USE_I2C2*/
	}
	else if (hi2c->Instance == I2C3)
	{
#ifdef BSP_CONFIG_USE_I2C3
		/*##-1- Reset peripherals ##################################################*/
		__HAL_RCC_I2C3_FORCE_RESET();
		__HAL_RCC_I2C3_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks ################################*/
		/* Configure I2C Tx as alternate function  */
		HAL_GPIO_DeInit(I2C3_SCL_GPIO_PORT, I2C3_SCL_PIN);
		/* Configure I2C Rx as alternate function  */
		HAL_GPIO_DeInit(I2C3_SDA_GPIO_PORT, I2C3_SDA_PIN);
		/*##-3- Disable the NVIC for I2C ###########################################*/
		HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
		HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
#endif /*BSP_CONFIG_USE_I2C3*/
	}
}

/**
  * @brief  Tx Transfer completed callback.
  * @param  hi2c: I2C handle 
  * @note   Report end of IT Tx transfer
  * @retval None
  */

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == I2C1)
	{
#ifdef BSP_CONFIG_USE_I2C1
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(i2c1_message_queue_id, I2C_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C1*/
	}
	else if (hi2c->Instance == I2C2)
	{
#ifdef BSP_CONFIG_USE_I2C2
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(i2c2_message_queue_id, I2C_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C2*/
	}
	else if (hi2c->Instance == I2C3)
	{
#ifdef BSP_CONFIG_USE_I2C3
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(i2c3_message_queue_id, I2C_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C3*/
	}
}


/**
  * @brief  Rx Transfer completed callback.
  * @param  hi2c: I2C handle
  * @note   Report end of IT Rx transfer
  * @retval None
  */

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == I2C1)
	{
#ifdef BSP_CONFIG_USE_I2C1
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(i2c1_message_queue_id, I2C_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C1*/
	}
	else if (hi2c->Instance == I2C2)
	{
#ifdef BSP_CONFIG_USE_I2C2
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(i2c2_message_queue_id, I2C_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C2*/
	}
	else if (hi2c->Instance == I2C3)
	{
#ifdef BSP_CONFIG_USE_I2C3
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(i2c3_message_queue_id, I2C_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C3*/
	}
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == I2C1)
	{
#ifdef BSP_CONFIG_USE_I2C1
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(i2c1_message_queue_id, I2C_EVENT_ERROR, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C1*/
	}
	else if (hi2c->Instance == I2C2)
	{
#ifdef BSP_CONFIG_USE_I2C2
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(i2c2_message_queue_id, I2C_EVENT_ERROR, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C2*/
	}
	else if (hi2c->Instance == I2C3)
	{
#ifdef BSP_CONFIG_USE_I2C3
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(i2c3_message_queue_id, I2C_EVENT_ERROR, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C3*/
	}
}
/**
  * @brief  I2C initialization
  * @param  Instance: I2C instance
  * @retval 0 on succes, otherwise -1;
  */

int32_t I2C_Init(void)
{	
	int32_t result;
	
	result = 0;
#ifdef BSP_CONFIG_USE_I2C1
#ifdef BSP_CONFIG_USE_FREERTOS
	if (!i2c1_mutex_id)
		i2c1_mutex_id = osMutexCreate(osMutex(i2c1));
	if (!i2c1_message_queue_id)
		i2c1_message_queue_id = osMessageCreate (osMessageQ(i2c1), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	i2c1_handle.Instance             = I2C1;  
	i2c1_handle.Init.AddressingMode  = I2C1_ADDRESSINGMODE;
	i2c1_handle.Init.ClockSpeed      = I2C1_CLOCKSPEED;
	i2c1_handle.Init.DualAddressMode = I2C1_DUALADDRESS;
	i2c1_handle.Init.DutyCycle       = I2C1_DUTYCYCLE;
	i2c1_handle.Init.GeneralCallMode = I2C1_GENERALCALL;
	i2c1_handle.Init.NoStretchMode   = I2C1_NOSTRETCH;
	i2c1_handle.Init.OwnAddress1     = I2C1_ADDRESS;
	i2c1_handle.Init.OwnAddress2     = 0;

	if(HAL_I2C_Init(&i2c1_handle) != HAL_OK)
	{
		result = -1;
	}
#endif /*BSP_CONFIG_USE_I2C1*/

#ifdef BSP_CONFIG_USE_I2C2
#ifdef BSP_CONFIG_USE_FREERTOS
	if (!i2c2_mutex_id)
		i2c2_mutex_id = osMutexCreate(osMutex(i2c2));
	if (!i2c2_message_queue_id)
		i2c2_message_queue_id = osMessageCreate (osMessageQ(i2c2), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	i2c2_handle.Instance             = I2C2;  
	i2c2_handle.Init.AddressingMode  = I2C2_ADDRESSINGMODE;
	i2c2_handle.Init.ClockSpeed      = I2C2_CLOCKSPEED;
	i2c2_handle.Init.DualAddressMode = I2C2_DUALADDRESS;
	i2c2_handle.Init.DutyCycle       = I2C2_DUTYCYCLE;
	i2c2_handle.Init.GeneralCallMode = I2C2_GENERALCALL;
	i2c2_handle.Init.NoStretchMode   = I2C2_NOSTRETCH;
	i2c2_handle.Init.OwnAddress1     = I2C2_ADDRESS;
	i2c2_handle.Init.OwnAddress2     = 0;

	if(HAL_I2C_Init(&i2c2_handle) != HAL_OK)
	{
		result = -1;
	}
#endif /*BSP_CONFIG_USE_I2C2*/

#ifdef BSP_CONFIG_USE_I2C3
#ifdef BSP_CONFIG_USE_FREERTOS
	if (!i2c3_mutex_id)
		i2c3_mutex_id = osMutexCreate(osMutex(i2c3));
	if (!i2c3_message_queue_id)
		i2c3_message_queue_id = osMessageCreate (osMessageQ(i2c3), NULL);
#endif /*BSP_CONFIG_USE_FREERTOS*/

	i2c3_handle.Instance             = I2C3;  
	i2c3_handle.Init.AddressingMode  = I2C3_ADDRESSINGMODE;
	i2c3_handle.Init.ClockSpeed      = I2C3_CLOCKSPEED;
	i2c3_handle.Init.DualAddressMode = I2C3_DUALADDRESS;
	i2c3_handle.Init.DutyCycle       = I2C3_DUTYCYCLE;
	i2c3_handle.Init.GeneralCallMode = I2C3_GENERALCALL;
	i2c3_handle.Init.NoStretchMode   = I2C3_NOSTRETCH;
	i2c3_handle.Init.OwnAddress1     = I2C3_ADDRESS;
	i2c3_handle.Init.OwnAddress2     = 0;

	if(HAL_I2C_Init(&i2c3_handle) != HAL_OK)
	{
		result = -1;
	}
#endif /*BSP_CONFIG_USE_I2C3*/
	return result;
}

void I2C_Deinit(void)
{
#ifdef BSP_CONFIG_USE_I2C1
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(i2c1_mutex_id, I2C_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_I2C_DeInit(&i2c1_handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(i2c1_mutex_id);
	osMutexDelete(i2c1_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C1*/
	
#ifdef BSP_CONFIG_USE_I2C2
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(i2c2_mutex_id, I2C_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_I2C_DeInit(&i2c2_handle);
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(i2c2_mutex_id);
	osMutexDelete(i2c2_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C2*/
	
#ifdef BSP_CONFIG_USE_I2C3
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(i2c3_mutex_id, I2C_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_I2C_DeInit(&i2c3_handle);
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(i2c3_mutex_id);
	osMutexDelete(i2c3_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_I2C3*/

}

/**
  * @brief  Transmit in master mode an amount of data in blocking mode with Interrupt
  * @param  Instance: I2C instance
  * @param  DevAddress: Target device address. The device 7 bits address value
  *         in datasheet must be shift at right before call interface
  * @param  Buffer: pointer to buffer with data to transmit
  * @param  Size: Amount of data to be sent
  * @retval 0 on succes, otherwise -1
  */

int32_t I2C_Transmit(I2C_TypeDef * Instance, uint16_t DevAddress, char *Buffer, uint16_t Size)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	I2C_HandleTypeDef *i2c_handle_p;
	osEvent evt;	
	osMutexId i2c_mutex_id;
	osMessageQId i2c_message_queue_id;
	
	if (I2C_GetHandle(Instance, &i2c_handle_p, &i2c_mutex_id, &i2c_message_queue_id))
		return -1;

	if( osMutexWait( i2c_mutex_id, I2C_TIMEOUT ) != osOK )
		return -1;

	if(HAL_I2C_Master_Transmit_IT(i2c_handle_p, (uint16_t)DevAddress, (uint8_t*)Buffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(i2c_message_queue_id, I2C_TIMEOUT);
	if (evt.status != osEventMessage) 
		goto error;
	
	if (evt.value.v != I2C_EVENT_COMPLETE)
		goto error;

	osMutexRelease( i2c_mutex_id );	
	return 0;

error:
	HAL_I2C_Master_Abort_IT(i2c_handle_p, DevAddress);
	HAL_I2C_DeInit(i2c_handle_p);
	HAL_I2C_Init(i2c_handle_p);
	osMutexRelease( i2c_mutex_id );
	return -1;
	
#else /*BSP_CONFIG_USE_FREERTOS*/
	I2C_HandleTypeDef *i2c_handle_p;
	
	if (I2C_GetHandle(Instance, &i2c_handle_p))
		return -1;
	
	if(HAL_I2C_Master_Transmit(i2c_handle_p, (uint16_t)DevAddress, (uint8_t*)Buffer, Size, I2C_TIMEOUT) != HAL_OK)
		return -1;
	return 0;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Receive in master mode an amount of data in blocking mode with Interrupt
  * @param  Instance: I2C instance
  * @param  DevAddress: Target device address. The device 7 bits address value
  *         in datasheet must be shift at right before call interface
  * @param  Buffer: pointer to buffer with data to transmit
  * @param  Size: Amount of data to receive
  * @retval 0 on succes, otherwise -1
  */

int32_t I2C_Receive(I2C_TypeDef * Instance, uint16_t DevAddress, char *Buffer, uint16_t Size)

{
#ifdef BSP_CONFIG_USE_FREERTOS
	I2C_HandleTypeDef *i2c_handle_p;
	osEvent evt;	
	osMutexId i2c_mutex_id;
	osMessageQId i2c_message_queue_id;
	
	if (I2C_GetHandle(Instance, &i2c_handle_p, &i2c_mutex_id, &i2c_message_queue_id))
		return -1;
	
	if( osMutexWait( i2c_mutex_id, I2C_TIMEOUT ) != osOK )
		return -1;

	if(HAL_I2C_Master_Receive_IT(i2c_handle_p, DevAddress, (uint8_t*)Buffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(i2c_message_queue_id, I2C_TIMEOUT);
	if (evt.status != osEventMessage) 
		goto error;
	
	if (evt.value.v != I2C_EVENT_COMPLETE)
		goto error;

	osMutexRelease( i2c_mutex_id );	
	return 0;

error:
	HAL_I2C_Master_Abort_IT(i2c_handle_p, DevAddress);
	HAL_I2C_DeInit(i2c_handle_p);
	HAL_I2C_Init(i2c_handle_p);
	osMutexRelease( i2c_mutex_id );
	return -1;
	
#else /*BSP_CONFIG_USE_FREERTOS*/
	I2C_HandleTypeDef *i2c_handle_p;
	
	if (I2C_GetHandle(Instance, &i2c_handle_p))
		return -1;
	
	if(HAL_I2C_Master_Receive(i2c_handle_p, DevAddress, (uint8_t*)Buffer, Size, I2C_TIMEOUT) != HAL_OK)
		return -1;
	return 0;	
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Transmit in sequential mode
  * @param  Instance: I2C instance
  * @param  DevAddress: Target device address. The device 7 bits address value
  *         in datasheet must be shift at right before call interface
  * @param  Address: Target device register address.
  * @param  AddressSize: Target device register address size. 
  * @param  Buffer: Pointer to buffer with data for transmit
  * @param  BufferSize: Amount of data to transmit
  * @retval 0 on succes, otherwise -1
  */

int32_t I2C_Sequential_Transmit(I2C_TypeDef * Instance, uint16_t DevAddress, uint8_t *Address, uint16_t AddressSize, char *Buffer, uint16_t BufferSize)
{
	
#ifdef BSP_CONFIG_USE_FREERTOS
	I2C_HandleTypeDef *i2c_handle_p;
	osEvent evt;	
	osMutexId i2c_mutex_id;
	osMessageQId i2c_message_queue_id;
	
	if (I2C_GetHandle(Instance, &i2c_handle_p, &i2c_mutex_id, &i2c_message_queue_id))
		return -1;
	
	if( osMutexWait( i2c_mutex_id, I2C_TIMEOUT ) != osOK )
		return -1;

	if (HAL_I2C_Master_Sequential_Transmit_IT(i2c_handle_p, DevAddress, Address, AddressSize, I2C_FIRST_FRAME) != HAL_OK)
		goto error;
		
	evt = osMessageGet(i2c_message_queue_id, I2C_TIMEOUT);
	if (evt.status != osEventMessage) 
		goto error;
		
	if (evt.value.v != I2C_EVENT_COMPLETE)
		goto error;


	if (HAL_I2C_Master_Sequential_Transmit_IT(i2c_handle_p, DevAddress, (uint8_t*)Buffer, BufferSize, I2C_LAST_FRAME) != HAL_OK)
		goto error;

	evt = osMessageGet(i2c_message_queue_id, I2C_TIMEOUT);
	if (evt.status != osEventMessage) 
		goto error;
	
	if (evt.value.v != I2C_EVENT_COMPLETE)
		goto error;

	osMutexRelease( i2c_mutex_id );	
	return 0;

error:
	HAL_I2C_Master_Abort_IT(i2c_handle_p, DevAddress);
	HAL_I2C_DeInit(i2c_handle_p);
	HAL_I2C_Init(i2c_handle_p);
	osMutexRelease( i2c_mutex_id );
	return -1;
	
#else /*BSP_CONFIG_USE_FREERTOS*/
	I2C_HandleTypeDef *i2c_handle_p;
	
	if (I2C_GetHandle(Instance, &i2c_handle_p))
		return -1;
	
	if (HAL_I2C_Master_Sequential_Transmit_IT(i2c_handle_p, DevAddress, Address, AddressSize, I2C_FIRST_FRAME) != HAL_OK)
		return -1;
	
	while (HAL_I2C_GetState(i2c_handle_p) != HAL_I2C_STATE_READY);
	
	if (HAL_I2C_Master_Sequential_Transmit_IT(i2c_handle_p, DevAddress, (uint8_t*)Buffer, BufferSize, I2C_LAST_FRAME) != HAL_OK)
		return -1;
		
	while (HAL_I2C_GetState(i2c_handle_p) != HAL_I2C_STATE_READY);
	
	return 0;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Receive in sequential mode
  * @param  Instance: I2C instance
  * @param  DevAddress: Target device address. The device 7 bits address value
  *         in datasheet must be shift at right before call interface
  * @param  Address: Target device register address.
  * @param  AddressSize: Target device register address size. 
  * @param  Buffer: Pointer to buffer for data receive
  * @param  BufferSize: Amount of data to receive
  * @retval 0 on succes, otherwise -1
  */

int32_t I2C_Sequential_Receive(I2C_TypeDef * Instance, uint16_t DevAddress, uint8_t *Address, uint16_t AddressSize, char *Buffer, uint16_t BufferSize)
{
	
#ifdef BSP_CONFIG_USE_FREERTOS
	I2C_HandleTypeDef *i2c_handle_p;
	osEvent evt;	
	osMutexId i2c_mutex_id;
	osMessageQId i2c_message_queue_id;
	
	if (I2C_GetHandle(Instance, &i2c_handle_p, &i2c_mutex_id, &i2c_message_queue_id))
		return -1;
	
	if( osMutexWait( i2c_mutex_id, I2C_TIMEOUT ) != osOK )
		return -1;

	if (HAL_I2C_Master_Sequential_Transmit_IT(i2c_handle_p, DevAddress, Address, AddressSize, I2C_FIRST_FRAME) != HAL_OK)
		goto error;
		
	evt = osMessageGet(i2c_message_queue_id, I2C_TIMEOUT);
	if (evt.status != osEventMessage)
		goto error;
		
	if (evt.value.v != I2C_EVENT_COMPLETE)
		goto error;

	if (HAL_I2C_Master_Sequential_Receive_IT(i2c_handle_p, DevAddress, (uint8_t*)Buffer, BufferSize, I2C_LAST_FRAME) != HAL_OK)
		goto error;
	
	evt = osMessageGet(i2c_message_queue_id, I2C_TIMEOUT);
	if (evt.status != osEventMessage) 
		goto error;
	
	if (evt.value.v != I2C_EVENT_COMPLETE)
		goto error;

	osMutexRelease( i2c_mutex_id );	
	return 0;

error:
	HAL_I2C_Master_Abort_IT(i2c_handle_p, DevAddress);
	HAL_I2C_DeInit(i2c_handle_p);
	HAL_I2C_Init(i2c_handle_p);
	osMutexRelease( i2c_mutex_id );
	return -1;
	
#else /*BSP_CONFIG_USE_FREERTOS*/
	I2C_HandleTypeDef *i2c_handle_p;
	
	if (I2C_GetHandle(Instance, &i2c_handle_p))
		return -1;

	if (HAL_I2C_Master_Sequential_Transmit_IT(i2c_handle_p, DevAddress, Address, AddressSize, I2C_FIRST_FRAME) != HAL_OK)
		return -1;
		
	while (HAL_I2C_GetState(i2c_handle_p) != HAL_I2C_STATE_READY);
	
	if (HAL_I2C_Master_Sequential_Receive(i2c_handle_p, DevAddress, (uint8_t*)Buffer, BufferSize, I2C_LAST_FRAME) != HAL_OK)
		return -1;
		
	while (HAL_I2C_GetState(i2c_handle_p) != HAL_I2C_STATE_READY);
	
	return 0;	
#endif /*BSP_CONFIG_USE_FREERTOS*/
}
