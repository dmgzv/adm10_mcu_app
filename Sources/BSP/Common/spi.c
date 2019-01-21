#include "spi.h"

#ifdef BSP_CONFIG_USE_FREERTOS
#define SPI_MESSAGE_QUEUE_SIZE 	(1)
#define SPI_EVENT_COMPLETE			(0)
#define SPI_EVENT_ERROR					(0xFFFFFFFF)
#define SPI_TIMEOUT 						(1000)
#else
#define SPI_TIMEOUT 						(1000)
#endif /*BSP_CONFIG_USE_FREERTOS*/

#ifdef BSP_CONFIG_USE_SPI1
static SPI_HandleTypeDef spi1_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
//static osMutexId spi1_mutex_id;
//osMutexDef(spi1);
static osMessageQId spi1_message_queue_id;
osMessageQDef(spi1, SPI_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI1*/

#ifdef BSP_CONFIG_USE_SPI2
static SPI_HandleTypeDef spi2_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
//static osMutexId spi2_mutex_id;
//osMutexDef(spi2);
static osMessageQId spi2_message_queue_id;
osMessageQDef(spi2, SPI_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI2*/

#ifdef BSP_CONFIG_USE_SPI3
static SPI_HandleTypeDef spi3_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
//static osMutexId spi3_mutex_id;
//osMutexDef(spi3);
static osMessageQId spi3_message_queue_id;
osMessageQDef(spi3, SPI_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI3*/

#ifdef BSP_CONFIG_USE_SPI5
static SPI_HandleTypeDef spi5_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
//static osMutexId spi5_mutex_id;
//osMutexDef(spi5);
static osMessageQId spi5_message_queue_id;
osMessageQDef(spi5, SPI_MESSAGE_QUEUE_SIZE, uint32_t);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI5*/

#if defined(BSP_CONFIG_USE_SPI1) || defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI5)
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId spi_com_mutex_id;
osMutexDef(spi);
#endif
#endif

#ifdef BSP_CONFIG_USE_FREERTOS
static int32_t SPI_GetHandle( SPI_TypeDef* Instance, SPI_HandleTypeDef **spi_handle, osMutexId *spi_mutex_id, osMessageQId *spi_message_queue_id)
#else
static int32_t SPI_GetHandle( SPI_TypeDef* Instance, SPI_HandleTypeDef **Handle)
#endif /*BSP_CONFIG_USE_FREERTOS*/
{	
	if (Instance == SPI1)
	{
#ifdef BSP_CONFIG_USE_SPI1
		*spi_handle = &spi1_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*spi_mutex_id = spi_com_mutex_id;
		*spi_message_queue_id = spi1_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#else
		return -1;
#endif /*BSP_CONFIG_USE_SPI1*/
	}
	else if (Instance == SPI2)
	{
#ifdef BSP_CONFIG_USE_SPI2
		*spi_handle = &spi2_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*spi_mutex_id = spi_com_mutex_id;
		*spi_message_queue_id = spi2_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#else
		return -1;
#endif /*BSP_CONFIG_USE_SPI2*/
	}
	else if (Instance == SPI3)
	{
#ifdef BSP_CONFIG_USE_SPI3
		*spi_handle = &spi3_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*spi_mutex_id = spi_com_mutex_id;
		*spi_message_queue_id = spi3_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#else
		return -1;
#endif /*BSP_CONFIG_USE_SPI3*/
	}
	else if (Instance == SPI5)
	{
#ifdef BSP_CONFIG_USE_SPI5
		*spi_handle = &spi5_handle;
#ifdef BSP_CONFIG_USE_FREERTOS
		*spi_mutex_id = spi_com_mutex_id;
		*spi_message_queue_id = spi5_message_queue_id;
#endif /*BSP_CONFIG_USE_FREERTOS*/
#else
		return -1;
#endif /*BSP_CONFIG_USE_SPI5*/
	}
	else
	{
		return -1;
	}
	return 0;
}

#ifdef BSP_CONFIG_USE_SPI1
/**
  * @brief  SPI1 interrupt Handler.  
  * @param  None
  * @retval None  
  */
void SPI1_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&spi1_handle);
}
#endif /*BSP_CONFIG_USE_SPI1*/

#ifdef BSP_CONFIG_USE_SPI2
/**
  * @brief  SPI3 interrupt Handler.  
  * @param  None
  * @retval None  
  */
void SPI2_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&spi2_handle);
}
#endif /*BSP_CONFIG_USE_SPI2*/

#ifdef BSP_CONFIG_USE_SPI3
/**
  * @brief  SPI3 interrupt Handler.  
  * @param  None
  * @retval None  
  */
void SPI3_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&spi3_handle);
}
#endif /*BSP_CONFIG_USE_SPI3*/

#ifdef BSP_CONFIG_USE_SPI5
/**
  * @brief  SPI5 interrupt Handler.  
  * @param  None
  * @retval None  
  */
void SPI5_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&spi5_handle);
}
#endif /*BSP_CONFIG_USE_SPI5*/

/**
  * @brief SPI MSP Initialization 
  * @param hspi: SPI Handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
	if (hspi->Instance == SPI1)
	{
#ifdef BSP_CONFIG_USE_SPI1
		/* Enable GPIO Clocks */
		SPI1_SCK_GPIO_CLK_ENABLE();
		SPI1_MISO_GPIO_CLK_ENABLE();
		SPI1_MOSI_GPIO_CLK_ENABLE();
		/* Enable SPI clock */
		__HAL_RCC_SPI1_CLK_ENABLE(); 		
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStruct.Pin       = SPI1_SCK_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = SPI1_SCK_AF;		
		HAL_GPIO_Init(SPI1_SCK_GPIO_PORT, &GPIO_InitStruct);			
		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI1_MISO_PIN;
		GPIO_InitStruct.Alternate = SPI1_MISO_AF;		
		HAL_GPIO_Init(SPI1_MISO_GPIO_PORT, &GPIO_InitStruct);		
		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI1_MOSI_PIN;
		GPIO_InitStruct.Alternate = SPI1_MOSI_AF;			
		HAL_GPIO_Init(SPI1_MOSI_GPIO_PORT, &GPIO_InitStruct);				
		/* Configure the NVIC for SPI */
		HAL_NVIC_SetPriority(SPI1_IRQn, SPI1_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(SPI1_IRQn);
#endif /*BSP_CONFIG_USE_SPI1*/
	}
	else if (hspi->Instance == SPI2)
	{
#ifdef BSP_CONFIG_USE_SPI2
		/* Enable GPIO Clocks */
		SPI2_SCK_GPIO_CLK_ENABLE();
		SPI2_MISO_GPIO_CLK_ENABLE();
		SPI2_MOSI_GPIO_CLK_ENABLE();
		/* Enable SPI clock */
		__HAL_RCC_SPI2_CLK_ENABLE(); 		
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStruct.Pin       = SPI2_SCK_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = SPI2_SCK_AF;		
		HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);			
		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI2_MISO_PIN;
		GPIO_InitStruct.Alternate = SPI2_MISO_AF;		
		HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &GPIO_InitStruct);		
		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI2_MOSI_PIN;
		GPIO_InitStruct.Alternate = SPI2_MOSI_AF;			
		HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &GPIO_InitStruct);				
		/* Configure the NVIC for SPI */
		HAL_NVIC_SetPriority(SPI2_IRQn, SPI2_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(SPI2_IRQn);
#endif /*BSP_CONFIG_USE_SPI2*/
	}
	else if (hspi->Instance == SPI3)
	{
#ifdef BSP_CONFIG_USE_SPI3
		/* Enable GPIO Clocks */
		SPI3_SCK_GPIO_CLK_ENABLE();
		SPI3_MISO_GPIO_CLK_ENABLE();
		SPI3_MOSI_GPIO_CLK_ENABLE();
		/* Enable SPI clock */
		__HAL_RCC_SPI3_CLK_ENABLE(); 		
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStruct.Pin       = SPI3_SCK_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = SPI3_SCK_AF;		
		HAL_GPIO_Init(SPI3_SCK_GPIO_PORT, &GPIO_InitStruct);			
		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI3_MISO_PIN;
		GPIO_InitStruct.Alternate = SPI3_MISO_AF;		
		HAL_GPIO_Init(SPI3_MISO_GPIO_PORT, &GPIO_InitStruct);		
		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI3_MOSI_PIN;
		GPIO_InitStruct.Alternate = SPI3_MOSI_AF;			
		HAL_GPIO_Init(SPI3_MOSI_GPIO_PORT, &GPIO_InitStruct);				
		/* Configure the NVIC for SPI */
		HAL_NVIC_SetPriority(SPI3_IRQn, SPI3_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(SPI3_IRQn);
#endif /*BSP_CONFIG_USE_SPI3*/
	}
	else if (hspi->Instance == SPI5)
	{
#ifdef BSP_CONFIG_USE_SPI5
		/* Enable GPIO Clocks */
		SPI5_SCK_GPIO_CLK_ENABLE();
		SPI5_MISO_GPIO_CLK_ENABLE();
		SPI5_MOSI_GPIO_CLK_ENABLE();
		/* Enable SPI clock */
		__HAL_RCC_SPI5_CLK_ENABLE(); 		
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStruct.Pin       = SPI5_SCK_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = SPI5_SCK_AF;		
		HAL_GPIO_Init(SPI5_SCK_GPIO_PORT, &GPIO_InitStruct);			
		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI5_MISO_PIN;
		GPIO_InitStruct.Alternate = SPI5_MISO_AF;		
		HAL_GPIO_Init(SPI5_MISO_GPIO_PORT, &GPIO_InitStruct);		
		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI5_MOSI_PIN;
		GPIO_InitStruct.Alternate = SPI5_MOSI_AF;			
		HAL_GPIO_Init(SPI5_MOSI_GPIO_PORT, &GPIO_InitStruct);				
		/* Configure the NVIC for SPI */
		HAL_NVIC_SetPriority(SPI5_IRQn, SPI5_IRQ_PRIORITY, 1);
		HAL_NVIC_EnableIRQ(SPI5_IRQn);
#endif /*BSP_CONFIG_USE_SPI5*/
	}
}

/**
  * @brief SPI MSP De-Initialization 
  * @param hspi: SPI Handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
#ifdef BSP_CONFIG_USE_SPI1
		/* Reset peripherals */
		__HAL_RCC_SPI1_FORCE_RESET();
		__HAL_RCC_SPI1_RELEASE_RESET();
		/* Disable peripherals and GPIO Clocks */
		/* Configure SPI SCK as alternate function  */
		HAL_GPIO_DeInit(SPI1_SCK_GPIO_PORT, SPI1_SCK_PIN);
		/* Configure SPI MISO as alternate function  */
		HAL_GPIO_DeInit(SPI1_MISO_GPIO_PORT, SPI1_MISO_PIN);
		/* Configure SPI MOSI as alternate function  */
		HAL_GPIO_DeInit(SPI1_MOSI_GPIO_PORT, SPI1_MOSI_PIN);		
		/* Disable the NVIC for SPI */
		HAL_NVIC_DisableIRQ(SPI1_IRQn);
#endif /*BSP_CONFIG_USE_SPI1*/
	}
	else if (hspi->Instance == SPI2)
	{
#ifdef BSP_CONFIG_USE_SPI2
		/* Reset peripherals */
		__HAL_RCC_SPI2_FORCE_RESET();
		__HAL_RCC_SPI2_RELEASE_RESET();
		/* Disable peripherals and GPIO Clocks */
		/* Configure SPI SCK as alternate function  */
		HAL_GPIO_DeInit(SPI2_SCK_GPIO_PORT, SPI2_SCK_PIN);
		/* Configure SPI MISO as alternate function  */
		HAL_GPIO_DeInit(SPI2_MISO_GPIO_PORT, SPI2_MISO_PIN);
		/* Configure SPI MOSI as alternate function  */
		HAL_GPIO_DeInit(SPI2_MOSI_GPIO_PORT, SPI2_MOSI_PIN);		
		/* Disable the NVIC for SPI */
		HAL_NVIC_DisableIRQ(SPI2_IRQn);
#endif /*BSP_CONFIG_USE_SPI2*/
	}
	else if (hspi->Instance == SPI3)
	{
#ifdef BSP_CONFIG_USE_SPI3
		/* Reset peripherals */
		__HAL_RCC_SPI3_FORCE_RESET();
		__HAL_RCC_SPI3_RELEASE_RESET();
		/* Disable peripherals and GPIO Clocks */
		/* Configure SPI SCK as alternate function  */
		HAL_GPIO_DeInit(SPI3_SCK_GPIO_PORT, SPI3_SCK_PIN);
		/* Configure SPI MISO as alternate function  */
		HAL_GPIO_DeInit(SPI3_MISO_GPIO_PORT, SPI3_MISO_PIN);
		/* Configure SPI MOSI as alternate function  */
		HAL_GPIO_DeInit(SPI3_MOSI_GPIO_PORT, SPI3_MOSI_PIN);		
		/* Disable the NVIC for SPI */
		HAL_NVIC_DisableIRQ(SPI3_IRQn);
#endif /*BSP_CONFIG_USE_SPI3*/
	}
	else if (hspi->Instance == SPI5)
	{
#ifdef BSP_CONFIG_USE_SPI5
		/* Reset peripherals */
		__HAL_RCC_SPI5_FORCE_RESET();
		__HAL_RCC_SPI5_RELEASE_RESET();
		/* Disable peripherals and GPIO Clocks */
		/* Configure SPI SCK as alternate function  */
		HAL_GPIO_DeInit(SPI5_SCK_GPIO_PORT, SPI5_SCK_PIN);
		/* Configure SPI MISO as alternate function  */
		HAL_GPIO_DeInit(SPI5_MISO_GPIO_PORT, SPI5_MISO_PIN);
		/* Configure SPI MOSI as alternate function  */
		HAL_GPIO_DeInit(SPI5_MOSI_GPIO_PORT, SPI5_MOSI_PIN);		
		/* Disable the NVIC for SPI */
		HAL_NVIC_DisableIRQ(SPI5_IRQn);
#endif /*BSP_CONFIG_USE_SPI5*/
	}
}

/**
  * @brief  TxRx Transfer completed callback
  * @param  hspi: SPI Handle. 
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
#ifdef BSP_CONFIG_USE_SPI1
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi1_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI1*/
	}
	else if (hspi->Instance == SPI2)
	{
#ifdef BSP_CONFIG_USE_SPI2
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi2_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI2*/
	}
	else if (hspi->Instance == SPI3)
	{
#ifdef BSP_CONFIG_USE_SPI3
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi3_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI3*/
	}
	else if (hspi->Instance == SPI5)
	{
#ifdef BSP_CONFIG_USE_SPI5
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi5_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI5*/
	}
}

/**
  * @brief  Rx Transfer completed callback
  * @param  hspi: SPI Handle. 
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
#ifdef BSP_CONFIG_USE_SPI1
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi1_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI1*/
	}
	else if (hspi->Instance == SPI2)
	{
#ifdef BSP_CONFIG_USE_SPI2
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi2_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI2*/
	}
	else if (hspi->Instance == SPI3)
	{
#ifdef BSP_CONFIG_USE_SPI3
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi3_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI3*/
	}
	else if (hspi->Instance == SPI5)
	{
#ifdef BSP_CONFIG_USE_SPI5
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi5_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI5*/
	}
}

/**
  * @brief  Tx Transfer completed callback
  * @param  hspi: SPI Handle. 
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
#ifdef BSP_CONFIG_USE_SPI1
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi1_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI1*/
	}
	else if (hspi->Instance == SPI2)
	{
#ifdef BSP_CONFIG_USE_SPI2
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi2_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI2*/
	}
	else if (hspi->Instance == SPI3)
	{
#ifdef BSP_CONFIG_USE_SPI3
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi3_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI3*/
	}
	else if (hspi->Instance == SPI5)
	{
#ifdef BSP_CONFIG_USE_SPI5
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi5_message_queue_id, SPI_EVENT_COMPLETE, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI5*/
	}
}

/**
  * @brief  Transfer error callback
  * @param  hspi: SPI Handle
  * @retval None
  */
 void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
#ifdef BSP_CONFIG_USE_SPI1
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi1_message_queue_id, SPI_EVENT_ERROR, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI1*/
	}
	else if (hspi->Instance == SPI2)
	{
#ifdef BSP_CONFIG_USE_SPI2
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi2_message_queue_id, SPI_EVENT_ERROR, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI2*/
	}
	else if (hspi->Instance == SPI3)
	{
#ifdef BSP_CONFIG_USE_SPI3
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi3_message_queue_id, SPI_EVENT_ERROR, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI3*/
	}
	else if (hspi->Instance == SPI5)
	{
#ifdef BSP_CONFIG_USE_SPI5
#ifdef BSP_CONFIG_USE_FREERTOS
		osMessagePut(spi5_message_queue_id, SPI_EVENT_ERROR, 0);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI5*/
	}
}

/**
  * @brief  SPI initialization
  * @param  None
  * @retval 0 on succes, otherwise -1;
  */
int32_t SPI_Init(void)
{
	int32_t result;
	
	result = 0;
#ifdef BSP_CONFIG_USE_SPI1
	spi1_handle.Instance               = SPI1;
	spi1_handle.Init.BaudRatePrescaler = SPI1_BAUDRATEPRESCALER;
	spi1_handle.Init.Direction         = SPI1_DIRECTION;
	spi1_handle.Init.CLKPhase          = SPI1_PHASE;
	spi1_handle.Init.CLKPolarity       = SPI1_POLARITY;
	spi1_handle.Init.CRCCalculation    = SPI1_CRCCALCULATION;
	spi1_handle.Init.CRCPolynomial     = SPI1_CRCPOLYNOMIAL;
	spi1_handle.Init.DataSize          = SPI1_DATASIZE;
	spi1_handle.Init.FirstBit          = SPI1_FIRSTBIT;
	spi1_handle.Init.NSS               = SPI1_NSS;
	spi1_handle.Init.TIMode            = SPI1_TIMODE;
	spi1_handle.Init.Mode              = SPI1_MODE;		
	if(HAL_SPI_Init(&spi1_handle) != HAL_OK)
		result = -1;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//spi1_mutex_id = osMutexCreate(osMutex(spi1));
	spi1_message_queue_id = osMessageCreate (osMessageQ(spi1), NULL);	
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI1*/
	
#ifdef BSP_CONFIG_USE_SPI2
	spi2_handle.Instance               = SPI2;
	spi2_handle.Init.BaudRatePrescaler = SPI2_BAUDRATEPRESCALER;
	spi2_handle.Init.Direction         = SPI2_DIRECTION;
	spi2_handle.Init.CLKPhase          = SPI2_PHASE;
	spi2_handle.Init.CLKPolarity       = SPI2_POLARITY;
	spi2_handle.Init.CRCCalculation    = SPI2_CRCCALCULATION;
	spi2_handle.Init.CRCPolynomial     = SPI2_CRCPOLYNOMIAL;
	spi2_handle.Init.DataSize          = SPI2_DATASIZE;
	spi2_handle.Init.FirstBit          = SPI2_FIRSTBIT;
	spi2_handle.Init.NSS               = SPI2_NSS;
	spi2_handle.Init.TIMode            = SPI2_TIMODE;
	spi2_handle.Init.Mode              = SPI2_MODE;		
	if(HAL_SPI_Init(&spi2_handle) != HAL_OK)
		result = -1;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//spi2_mutex_id = osMutexCreate(osMutex(spi2));
	spi2_message_queue_id = osMessageCreate (osMessageQ(spi2), NULL);	
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI2*/

#ifdef BSP_CONFIG_USE_SPI3
	spi3_handle.Instance               = SPI3;
	spi3_handle.Init.BaudRatePrescaler = SPI3_BAUDRATEPRESCALER;
	spi3_handle.Init.Direction         = SPI3_DIRECTION;
	spi3_handle.Init.CLKPhase          = SPI3_PHASE;
	spi3_handle.Init.CLKPolarity       = SPI3_POLARITY;
	spi3_handle.Init.CRCCalculation    = SPI3_CRCCALCULATION;
	spi3_handle.Init.CRCPolynomial     = SPI3_CRCPOLYNOMIAL;
	spi3_handle.Init.DataSize          = SPI3_DATASIZE;
	spi3_handle.Init.FirstBit          = SPI3_FIRSTBIT;
	spi3_handle.Init.NSS               = SPI3_NSS;
	spi3_handle.Init.TIMode            = SPI3_TIMODE;
	spi3_handle.Init.Mode              = SPI3_MODE;		
	if(HAL_SPI_Init(&spi3_handle) != HAL_OK)
		result = -1;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//spi3_mutex_id = osMutexCreate(osMutex(spi3));
	spi3_message_queue_id = osMessageCreate (osMessageQ(spi3), NULL);	
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI3*/
	
#ifdef BSP_CONFIG_USE_SPI5
	spi5_handle.Instance               = SPI5;
	spi5_handle.Init.BaudRatePrescaler = SPI5_BAUDRATEPRESCALER;
	spi5_handle.Init.Direction         = SPI5_DIRECTION;
	spi5_handle.Init.CLKPhase          = SPI5_PHASE;
	spi5_handle.Init.CLKPolarity       = SPI5_POLARITY;
	spi5_handle.Init.CRCCalculation    = SPI5_CRCCALCULATION;
	spi5_handle.Init.CRCPolynomial     = SPI5_CRCPOLYNOMIAL;
	spi5_handle.Init.DataSize          = SPI5_DATASIZE;
	spi5_handle.Init.FirstBit          = SPI5_FIRSTBIT;
	spi5_handle.Init.NSS               = SPI5_NSS;
	spi5_handle.Init.TIMode            = SPI5_TIMODE;
	spi5_handle.Init.Mode              = SPI5_MODE;		
	if(HAL_SPI_Init(&spi5_handle) != HAL_OK)
		result = -1;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//spi5_mutex_id = osMutexCreate(osMutex(spi5));
	spi5_message_queue_id = osMessageCreate (osMessageQ(spi5), NULL);	
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI5*/
	
#if defined(BSP_CONFIG_USE_SPI1) || defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI5)
#ifdef BSP_CONFIG_USE_FREERTOS
	spi_com_mutex_id = osMutexCreate(osMutex(spi));
#endif
#endif
	
	return result;
}

/**
  * @brief  SPI deinitialization
  * @param  None
  * @retval None
  */
void SPI_Deinit(void)
{
	
#if defined(BSP_CONFIG_USE_SPI1) || defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI5)
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait(spi_com_mutex_id, SPI_TIMEOUT);
#endif
#endif
	
#ifdef BSP_CONFIG_USE_SPI1
#ifdef BSP_CONFIG_USE_FREERTOS
	//osMutexWait(spi1_mutex_id, SPI_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_SPI_DeInit(&spi1_handle);	
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//osMutexRelease(spi1_mutex_id);
	//osMutexDelete(spi1_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI1*/
	
#ifdef BSP_CONFIG_USE_SPI2
#ifdef BSP_CONFIG_USE_FREERTOS
	//osMutexWait(spi2_mutex_id, SPI_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_SPI_DeInit(&spi2_handle);
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//osMutexRelease(spi2_mutex_id);
	//osMutexDelete(spi2_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI2*/
	
#ifdef BSP_CONFIG_USE_SPI3
#ifdef BSP_CONFIG_USE_FREERTOS
	//osMutexWait(spi3_mutex_id, SPI_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_SPI_DeInit(&spi3_handle);
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//osMutexRelease(spi3_mutex_id);
	//osMutexDelete(spi3_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI3*/

#ifdef BSP_CONFIG_USE_SPI5
#ifdef BSP_CONFIG_USE_FREERTOS
	//osMutexWait(spi5_mutex_id, SPI_TIMEOUT);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	
	HAL_SPI_DeInit(&spi5_handle);
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//osMutexRelease(spi5_mutex_id);
	//osMutexDelete(spi5_mutex_id);
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI5*/

#if defined(BSP_CONFIG_USE_SPI1) || defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI5)
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease(spi_com_mutex_id);
	osMutexDelete(spi_com_mutex_id);
#endif
#endif

}

/**
  * @brief  Sequential transmit an amount of data in blocking mode with Interrupt.
  * @param  Instance: SPI instance
  * @param  CS: CS pin management instance
  * @param  TxBuffer1: pointer to buffer with data to transmit first
  * @param  TxSize1: size of data to transmit first
  * @param  TxBuffer2: pointer to buffer with data to transmit last
  * @param  TxSize2: size of data to transmit last
  * @retval 0 on succes, otherwise -1;
  */
int32_t SPI_SequentialTransmit(SPI_TypeDef * Instance, SPI_CS_T *CS, char *TxBuffer1, uint32_t  TxSize1, char *TxBuffer2, uint32_t  TxSize2)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	SPI_HandleTypeDef *spi_handle_p;
	osEvent evt;	
	osMutexId spi_mutex_id;
	osMessageQId spi_message_queue_id;
	
	if (SPI_GetHandle(Instance, &spi_handle_p, &spi_mutex_id, &spi_message_queue_id))
		return -1;

	if( osMutexWait( spi_mutex_id, SPI_TIMEOUT ) != osOK )
		return -1;

	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit_IT(spi_handle_p, (uint8_t*)TxBuffer1, TxSize1) != HAL_OK)
		goto error;

	evt = osMessageGet(spi_message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage) 
	{
		HAL_SPI_Abort_IT(spi_handle_p);
		goto error;
	}
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;
	
	if(HAL_SPI_Transmit_IT(spi_handle_p, (uint8_t*)TxBuffer2, TxSize2) != HAL_OK)
		goto error;

	evt = osMessageGet(spi_message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage) 
	{
		HAL_SPI_Abort_IT(spi_handle_p);
		goto error;
	}
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;
	
	if (CS)
		CS->SetState(CS->Priv, 1);

	osMutexRelease( spi_mutex_id );	
	return 0;

error:
	HAL_SPI_DeInit(spi_handle_p);
	HAL_SPI_Init(spi_handle_p);
	if (CS)
		CS->SetState(CS->Priv, 1);
	osMutexRelease( spi_mutex_id );
	return -1;	
#else /*BSP_CONFIG_USE_FREERTOS*/
	SPI_HandleTypeDef *spi_handle_p;
	
	if (SPI_GetHandle(Instance, &spi_handle_p))
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit(spi_handle_p, (uint8_t *)TxBuffer1, TxSize1, SPI_TIMEOUT) != HAL_OK)
		goto error;
	
	if(HAL_SPI_Transmit(spi_handle_p, (uint8_t *)TxBuffer2, TxSize2, SPI_TIMEOUT) != HAL_OK)
		goto error;
	
	if (CS)
		CS->SetState(CS->Priv, 1);
	return 0;	
	
error:
	if (CS)
		CS->SetState(CS->Priv, 1);
	return -1;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Sequential transmit and then receive an amount of data in blocking mode with Interrupt.
  * @param  Instance: SPI instance
  * @param  CS: CS pin management instance
  * @param  TxBuffer: pointer to buffer with data to transmit
  * @param  RxBuffer: pointer to buffer for received data
  * @param  TxSize: size of data to transmit
  * @param  RxSize: size of data to receive
  * @retval 0 on succes, otherwise -1;
  */
int32_t SPI_SequentialReceive(SPI_TypeDef * Instance, SPI_CS_T *CS, char *TxBuffer, uint32_t  TxSize, char * RxBuffer, uint32_t  RxSize)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	SPI_HandleTypeDef *spi_handle_p;
	osEvent evt;	
	osMutexId spi_mutex_id;
	osMessageQId spi_message_queue_id;
	
	if (SPI_GetHandle(Instance, &spi_handle_p, &spi_mutex_id, &spi_message_queue_id))
		return -1;

	if( osMutexWait( spi_mutex_id, SPI_TIMEOUT ) != osOK )
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit_IT(spi_handle_p, (uint8_t*)TxBuffer, TxSize) != HAL_OK)
		goto error;

	evt = osMessageGet(spi_message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage) 
	{
		HAL_SPI_Abort_IT(spi_handle_p);
		goto error;
	}
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;
	
	if(HAL_SPI_Receive_IT(spi_handle_p, (uint8_t *)RxBuffer, RxSize) != HAL_OK)
		goto error;

	evt = osMessageGet(spi_message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage)
	{
		HAL_SPI_Abort_IT(spi_handle_p);
		goto error;
	}
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;

	if (CS)
		CS->SetState(CS->Priv, 1);
	
	osMutexRelease( spi_mutex_id );	
	return 0;

error:
	HAL_SPI_DeInit(spi_handle_p);
	HAL_SPI_Init(spi_handle_p);
	if (CS)
		CS->SetState(CS->Priv, 1);
	osMutexRelease( spi_mutex_id );
	return -1;	
#else /*BSP_CONFIG_USE_FREERTOS*/
	SPI_HandleTypeDef *spi_handle_p;
	
	if (SPI_GetHandle(Instance, &spi_handle_p))
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit(spi_handle_p, (uint8_t*)TxBuffer, TxSize, SPI_TIMEOUT) != HAL_OK)
		goto error;
	
	if(HAL_SPI_Receive(spi_handle_p, (uint8_t *)RxBuffer, RxSize, SPI_TIMEOUT) != HAL_OK)
		goto error;
	
	if (CS)
		CS->SetState(CS->Priv, 1);
	return 0;	
	
error:
	if (CS)
		CS->SetState(CS->Priv, 1);
	return -1;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Transmit and receive an amount of data in blocking mode with Interrupt.
  * @param  Instance: SPI instance
  * @param  CS: CS pin management instance
  * @param  TxBuffer: pointer to buffer with data to transmit
  * @param  RxBuffer: pointer to buffer for received data
  * @param  Size: size of data to transmit and receive
  * @retval 0 on succes, otherwise -1;
  */
int32_t SPI_TransmitReceive(SPI_TypeDef * Instance, SPI_CS_T *CS, char *TxBuffer, char * RxBuffer, uint32_t  Size)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	SPI_HandleTypeDef *spi_handle_p;
	osEvent evt;	
	osMutexId spi_mutex_id;
	osMessageQId spi_message_queue_id;
	
	if (SPI_GetHandle(Instance, &spi_handle_p, &spi_mutex_id, &spi_message_queue_id))
		return -1;

	if( osMutexWait( spi_mutex_id, SPI_TIMEOUT ) != osOK )
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_TransmitReceive_IT(spi_handle_p, (uint8_t*)TxBuffer, (uint8_t *)RxBuffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(spi_message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage)
	{
		HAL_SPI_Abort_IT(spi_handle_p);
		goto error;
	}
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;

	if (CS)
		CS->SetState(CS->Priv, 1);
	
	osMutexRelease( spi_mutex_id );	
	return 0;

error:
	HAL_SPI_DeInit(spi_handle_p);
	HAL_SPI_Init(spi_handle_p);
	if (CS)
		CS->SetState(CS->Priv, 1);
	osMutexRelease( spi_mutex_id );
	return -1;	
#else /*BSP_CONFIG_USE_FREERTOS*/
	SPI_HandleTypeDef *spi_handle_p;
	
	if (SPI_GetHandle(Instance, &spi_handle_p))
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_TransmitReceive(spi_handle_p, (uint8_t*)TxBuffer, (uint8_t *)RxBuffer, Size, SPI_TIMEOUT) != HAL_OK)
		goto error;
	
	if (CS)
		CS->SetState(CS->Priv, 1);
	return 0;	
	
error:
	if (CS)
		CS->SetState(CS->Priv, 1);
	return -1;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Transmit an amount of data in blocking mode with Interrupt.
  * @param  Instance: SPI instance
  * @param  CS: CS pin management instance
  * @param  Buffer: pointer to buffer with data to transmit
  * @param  Size: size of data to transmit
  * @retval 0 on succes, otherwise -1;
  */
int32_t SPI_Transmit(SPI_TypeDef * Instance, SPI_CS_T *CS, char *Buffer, uint32_t  Size)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	SPI_HandleTypeDef *spi_handle_p;
	osEvent evt;	
	osMutexId spi_mutex_id;
	osMessageQId spi_message_queue_id;
	
	if (SPI_GetHandle(Instance, &spi_handle_p, &spi_mutex_id, &spi_message_queue_id))
		return -1;

	if( osMutexWait( spi_mutex_id, SPI_TIMEOUT ) != osOK )
		return -1;

	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit_IT(spi_handle_p, (uint8_t*)Buffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(spi_message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage) 
	{
		HAL_SPI_Abort_IT(spi_handle_p);
		goto error;
	}
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;
	
	if (CS)
		CS->SetState(CS->Priv, 1);

	osMutexRelease( spi_mutex_id );	
	return 0;

error:
	HAL_SPI_DeInit(spi_handle_p);
	HAL_SPI_Init(spi_handle_p);
	if (CS)
		CS->SetState(CS->Priv, 1);
	osMutexRelease( spi_mutex_id );
	return -1;	
#else /*BSP_CONFIG_USE_FREERTOS*/
	SPI_HandleTypeDef *spi_handle_p;
	
	if (SPI_GetHandle(Instance, &spi_handle_p))
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit(spi_handle_p, (uint8_t *)Buffer, Size, SPI_TIMEOUT) != HAL_OK)
		goto error;
	
	if (CS)
		CS->SetState(CS->Priv, 1);
	return 0;	
	
error:
	if (CS)
		CS->SetState(CS->Priv, 1);
	return -1;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Receive an amount of data in blocking mode with Interrupt.
  * @param  Instance: SPI instance
  * @param  CS: CS pin management instance
  * @param  Buffer: pointer to buffer for received data
  * @param  Size: size of data to receive
  * @retval 0 on succes, otherwise -1;
  */
int32_t SPI_Receive(SPI_TypeDef * Instance, SPI_CS_T *CS, char *Buffer, uint32_t  Size)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	SPI_HandleTypeDef *spi_handle_p;
	osEvent evt;	
	osMutexId spi_mutex_id;
	osMessageQId spi_message_queue_id;
	
	if (SPI_GetHandle(Instance, &spi_handle_p, &spi_mutex_id, &spi_message_queue_id))
		return -1;

	if( osMutexWait( spi_mutex_id, SPI_TIMEOUT ) != osOK )
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);

	if(HAL_SPI_Receive_IT(spi_handle_p, (uint8_t *)Buffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(spi_message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage) 
	{
		HAL_SPI_Abort_IT(spi_handle_p);
		goto error;
	}
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;
	
	if (CS)
		CS->SetState(CS->Priv, 1);
	
	osMutexRelease( spi_mutex_id );	
	return 0;

error:
	HAL_SPI_DeInit(spi_handle_p);
	HAL_SPI_Init(spi_handle_p);
	if (CS)
		CS->SetState(CS->Priv, 1);
	osMutexRelease( spi_mutex_id );
	return -1;
#else /*BSP_CONFIG_USE_FREERTOS*/
	SPI_HandleTypeDef *spi_handle_p;
	
	if (SPI_GetHandle(Instance, &spi_handle_p))
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Receive(spi_handle_p, (uint8_t *)Buffer, Size, SPI_TIMEOUT) != HAL_OK)
		goto error;
	
	if (CS)
		CS->SetState(CS->Priv, 1);
	return 0;	
	
error:
	if (CS)
		CS->SetState(CS->Priv, 1);
	return -1;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}
