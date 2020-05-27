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
SPI_T spi1;
#endif /*BSP_CONFIG_USE_SPI1*/

#ifdef BSP_CONFIG_USE_SPI2
SPI_T spi2;
#endif /*BSP_CONFIG_USE_SPI2*/

#ifdef BSP_CONFIG_USE_SPI3
SPI_T spi3;
#endif /*BSP_CONFIG_USE_SPI3*/

#ifdef BSP_CONFIG_USE_SPI4
SPI_T spi4;
#endif /*BSP_CONFIG_USE_SPI4*/

#ifdef BSP_CONFIG_USE_SPI5
SPI_T spi5;
#endif /*BSP_CONFIG_USE_SPI5*/

#if defined(BSP_CONFIG_USE_SPI1) || defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5)
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId spi_com_mutex_id;
osMutexDef(spi);
osMessageQDef(spi, SPI_MESSAGE_QUEUE_SIZE, uint32_t);
#endif
#endif

static int32_t SPI_GetHandle(const SPI_TypeDef* Instance, SPI_T **spi)
{
#ifdef BSP_CONFIG_USE_SPI1
	if (Instance == SPI1)
	{
		*spi = &spi1;
	}
	else
#endif /*BSP_CONFIG_USE_SPI1*/
#ifdef BSP_CONFIG_USE_SPI2
	if (Instance == SPI2)
	{
		*spi = &spi2;
	}
	else
#endif /*BSP_CONFIG_USE_SPI2*/
#ifdef BSP_CONFIG_USE_SPI3
	if (Instance == SPI3)
	{
		*spi = &spi3;
	}
	else
#endif /*BSP_CONFIG_USE_SPI3*/
#ifdef BSP_CONFIG_USE_SPI4
	if (Instance == SPI4)
	{
		*spi = &spi4;
	}
	else
#endif /*BSP_CONFIG_USE_SPI4*/
#ifdef BSP_CONFIG_USE_SPI5
	if (Instance == SPI5)
	{
		*spi = &spi5;
	}
	else
#endif /*BSP_CONFIG_USE_SPI5*/
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
  HAL_SPI_IRQHandler(&spi1.handle);
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
  HAL_SPI_IRQHandler(&spi2.handle);
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
  HAL_SPI_IRQHandler(&spi3.handle);
}
#endif /*BSP_CONFIG_USE_SPI3*/

#ifdef BSP_CONFIG_USE_SPI4
/**
  * @brief  SPI4 interrupt Handler.  
  * @param  None
  * @retval None  
  */
void SPI4_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&spi4.handle);
}
#endif /*BSP_CONFIG_USE_SPI4*/

#ifdef BSP_CONFIG_USE_SPI5
/**
  * @brief  SPI5 interrupt Handler.  
  * @param  None
  * @retval None  
  */
void SPI5_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&spi5.handle);
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

#ifdef BSP_CONFIG_USE_SPI1
	if (hspi->Instance == SPI1)
	{
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
		HAL_NVIC_SetPriority(SPI1_IRQn, SPI1_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(SPI1_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_SPI1*/
#ifdef BSP_CONFIG_USE_SPI2
	if (hspi->Instance == SPI2)
	{
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
		HAL_NVIC_SetPriority(SPI2_IRQn, SPI2_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(SPI2_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_SPI2*/
#ifdef BSP_CONFIG_USE_SPI3
	if (hspi->Instance == SPI3)
	{
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
		HAL_NVIC_SetPriority(SPI3_IRQn, SPI3_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(SPI3_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_SPI3*/
#ifdef BSP_CONFIG_USE_SPI4
	if (hspi->Instance == SPI4)
	{
		/* Enable GPIO Clocks */
		SPI4_SCK_GPIO_CLK_ENABLE();
		SPI4_MISO_GPIO_CLK_ENABLE();
		SPI4_MOSI_GPIO_CLK_ENABLE();
		/* Enable SPI clock */
		__HAL_RCC_SPI4_CLK_ENABLE(); 		
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStruct.Pin       = SPI4_SCK_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = SPI4_SCK_AF;		
		HAL_GPIO_Init(SPI4_SCK_GPIO_PORT, &GPIO_InitStruct);			
		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI4_MISO_PIN;
		GPIO_InitStruct.Alternate = SPI4_MISO_AF;		
		HAL_GPIO_Init(SPI4_MISO_GPIO_PORT, &GPIO_InitStruct);		
		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI4_MOSI_PIN;
		GPIO_InitStruct.Alternate = SPI4_MOSI_AF;			
		HAL_GPIO_Init(SPI4_MOSI_GPIO_PORT, &GPIO_InitStruct);				
		/* Configure the NVIC for SPI */
		HAL_NVIC_SetPriority(SPI4_IRQn, SPI4_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(SPI4_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_SPI4*/
#ifdef BSP_CONFIG_USE_SPI5
	if (hspi->Instance == SPI5)
	{
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
		HAL_NVIC_SetPriority(SPI5_IRQn, SPI5_IRQ_PRIORITY, 0);
		HAL_NVIC_EnableIRQ(SPI5_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_SPI5*/
	{
	}
}

/**
  * @brief SPI MSP De-Initialization 
  * @param hspi: SPI Handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
#ifdef BSP_CONFIG_USE_SPI1
	if (hspi->Instance == SPI1)
	{
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
	}
	else
#endif /*BSP_CONFIG_USE_SPI1*/
#ifdef BSP_CONFIG_USE_SPI2
	if (hspi->Instance == SPI2)
	{
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
	}
	else
#endif /*BSP_CONFIG_USE_SPI2*/
#ifdef BSP_CONFIG_USE_SPI3
	if (hspi->Instance == SPI3)
	{
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
	}
	else
#endif /*BSP_CONFIG_USE_SPI3*/
#ifdef BSP_CONFIG_USE_SPI4
	if (hspi->Instance == SPI4)
	{
		/* Reset peripherals */
		__HAL_RCC_SPI4_FORCE_RESET();
		__HAL_RCC_SPI4_RELEASE_RESET();
		/* Disable peripherals and GPIO Clocks */
		/* Configure SPI SCK as alternate function  */
		HAL_GPIO_DeInit(SPI4_SCK_GPIO_PORT, SPI4_SCK_PIN);
		/* Configure SPI MISO as alternate function  */
		HAL_GPIO_DeInit(SPI4_MISO_GPIO_PORT, SPI4_MISO_PIN);
		/* Configure SPI MOSI as alternate function  */
		HAL_GPIO_DeInit(SPI4_MOSI_GPIO_PORT, SPI4_MOSI_PIN);		
		/* Disable the NVIC for SPI */
		HAL_NVIC_DisableIRQ(SPI4_IRQn);
	}
	else
#endif /*BSP_CONFIG_USE_SPI4*/
#ifdef BSP_CONFIG_USE_SPI5
	if (hspi->Instance == SPI5)
	{
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
	}
	else
#endif /*BSP_CONFIG_USE_SPI5*/
	{
	}
}

/**
  * @brief  TxRx Transfer completed callback
  * @param  hspi: SPI Handle. 
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	SPI_T *spi;
	
	if (SPI_GetHandle(hspi->Instance, &spi))
		return;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	if (osMessagePut(spi->message_queue_id, SPI_EVENT_COMPLETE, 0) != osOK)
		spi->message_queue_overflow_errors++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Rx Transfer completed callback
  * @param  hspi: SPI Handle. 
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	SPI_T *spi;
	
	if (SPI_GetHandle(hspi->Instance, &spi))
		return;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	if (osMessagePut(spi->message_queue_id, SPI_EVENT_COMPLETE, 0) != osOK)
		spi->message_queue_overflow_errors++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Tx Transfer completed callback
  * @param  hspi: SPI Handle. 
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	SPI_T *spi;
	
	if (SPI_GetHandle(hspi->Instance, &spi))
		return;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	if (osMessagePut(spi->message_queue_id, SPI_EVENT_COMPLETE, 0) != osOK)
		spi->message_queue_overflow_errors++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  Transfer error callback
  * @param  hspi: SPI Handle
  * @retval None
  */
 void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	SPI_T *spi;
	
	if (SPI_GetHandle(hspi->Instance, &spi))
		return;
	
	if (spi->handle.ErrorCode & HAL_SPI_ERROR_MODF)
		spi->mode_fault_errors++;
	if (spi->handle.ErrorCode & HAL_SPI_ERROR_CRC)
		spi->crc_errors++;
	if (spi->handle.ErrorCode & HAL_SPI_ERROR_OVR)
		spi->overrun_errors++;
	if (spi->handle.ErrorCode & HAL_SPI_ERROR_FRE)
		spi->frame_errors++;
	if (spi->handle.ErrorCode & HAL_SPI_ERROR_DMA)
		spi->dma_errors++;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	if (osMessagePut(spi->message_queue_id, SPI_EVENT_ERROR, 0) != osOK)
		spi->message_queue_overflow_errors++;
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

/**
  * @brief  SPI initialization
  * @param  None
  * @retval 0 on succes, otherwise -1;
  */
int32_t SPI_Init(void)
{
	int32_t result;

#if defined(BSP_CONFIG_USE_SPI1) || defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5)
#ifdef BSP_CONFIG_USE_FREERTOS
	if (!spi_com_mutex_id)
		spi_com_mutex_id = osMutexCreate(osMutex(spi));
#endif
#endif	

	result = 0;
#ifdef BSP_CONFIG_USE_SPI1
	spi1.handle.Instance               = SPI1;
	spi1.handle.Init.BaudRatePrescaler = SPI1_BAUDRATEPRESCALER;
	spi1.handle.Init.Direction         = SPI1_DIRECTION;
	spi1.handle.Init.CLKPhase          = SPI1_PHASE;
	spi1.handle.Init.CLKPolarity       = SPI1_POLARITY;
	spi1.handle.Init.CRCCalculation    = SPI1_CRCCALCULATION;
	spi1.handle.Init.CRCPolynomial     = SPI1_CRCPOLYNOMIAL;
	spi1.handle.Init.DataSize          = SPI1_DATASIZE;
	spi1.handle.Init.FirstBit          = SPI1_FIRSTBIT;
	spi1.handle.Init.NSS               = SPI1_NSS;
	spi1.handle.Init.TIMode            = SPI1_TIMODE;
	spi1.handle.Init.Mode              = SPI1_MODE;		
	if(HAL_SPI_Init(&spi1.handle) != HAL_OK)
		result = -1;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//spi1.mutex_id = osMutexCreate(osMutex(spi));
	spi1.mutex_id = spi_com_mutex_id;
	if (!spi1.message_queue_id)
		spi1.message_queue_id = osMessageCreate (osMessageQ(spi), NULL);	
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI1*/
	
#ifdef BSP_CONFIG_USE_SPI2
	spi2.handle.Instance               = SPI2;
	spi2.handle.Init.BaudRatePrescaler = SPI2_BAUDRATEPRESCALER;
	spi2.handle.Init.Direction         = SPI2_DIRECTION;
	spi2.handle.Init.CLKPhase          = SPI2_PHASE;
	spi2.handle.Init.CLKPolarity       = SPI2_POLARITY;
	spi2.handle.Init.CRCCalculation    = SPI2_CRCCALCULATION;
	spi2.handle.Init.CRCPolynomial     = SPI2_CRCPOLYNOMIAL;
	spi2.handle.Init.DataSize          = SPI2_DATASIZE;
	spi2.handle.Init.FirstBit          = SPI2_FIRSTBIT;
	spi2.handle.Init.NSS               = SPI2_NSS;
	spi2.handle.Init.TIMode            = SPI2_TIMODE;
	spi2.handle.Init.Mode              = SPI2_MODE;		
	if(HAL_SPI_Init(&spi2.handle) != HAL_OK)
		result = -1;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//spi2.mutex_id = osMutexCreate(osMutex(spi));
	spi2.mutex_id = spi_com_mutex_id;
	if (!spi2.message_queue_id)
		spi2.message_queue_id = osMessageCreate (osMessageQ(spi), NULL);	
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI2*/

#ifdef BSP_CONFIG_USE_SPI3
	spi3.handle.Instance               = SPI3;
	spi3.handle.Init.BaudRatePrescaler = SPI3_BAUDRATEPRESCALER;
	spi3.handle.Init.Direction         = SPI3_DIRECTION;
	spi3.handle.Init.CLKPhase          = SPI3_PHASE;
	spi3.handle.Init.CLKPolarity       = SPI3_POLARITY;
	spi3.handle.Init.CRCCalculation    = SPI3_CRCCALCULATION;
	spi3.handle.Init.CRCPolynomial     = SPI3_CRCPOLYNOMIAL;
	spi3.handle.Init.DataSize          = SPI3_DATASIZE;
	spi3.handle.Init.FirstBit          = SPI3_FIRSTBIT;
	spi3.handle.Init.NSS               = SPI3_NSS;
	spi3.handle.Init.TIMode            = SPI3_TIMODE;
	spi3.handle.Init.Mode              = SPI3_MODE;		
	if(HAL_SPI_Init(&spi3.handle) != HAL_OK)
		result = -1;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//spi3.mutex_id = osMutexCreate(osMutex(spi));
	spi3.mutex_id = spi_com_mutex_id;
	if (!spi3.message_queue_id)
		spi3.message_queue_id = osMessageCreate (osMessageQ(spi), NULL);	
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI3*/

#ifdef BSP_CONFIG_USE_SPI4
	spi4.handle.Instance               = SPI4;
	spi4.handle.Init.BaudRatePrescaler = SPI4_BAUDRATEPRESCALER;
	spi4.handle.Init.Direction         = SPI4_DIRECTION;
	spi4.handle.Init.CLKPhase          = SPI4_PHASE;
	spi4.handle.Init.CLKPolarity       = SPI4_POLARITY;
	spi4.handle.Init.CRCCalculation    = SPI4_CRCCALCULATION;
	spi4.handle.Init.CRCPolynomial     = SPI4_CRCPOLYNOMIAL;
	spi4.handle.Init.DataSize          = SPI4_DATASIZE;
	spi4.handle.Init.FirstBit          = SPI4_FIRSTBIT;
	spi4.handle.Init.NSS               = SPI4_NSS;
	spi4.handle.Init.TIMode            = SPI4_TIMODE;
	spi4.handle.Init.Mode              = SPI4_MODE;		
	if(HAL_SPI_Init(&spi4.handle) != HAL_OK)
		result = -1;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//spi4.mutex_id = osMutexCreate(osMutex(spi));
	spi4.mutex_id = spi_com_mutex_id;
	if (!spi4.message_queue_id)
		spi4.message_queue_id = osMessageCreate (osMessageQ(spi), NULL);	
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI4*/
	
#ifdef BSP_CONFIG_USE_SPI5
	spi5.handle.Instance               = SPI5;
	spi5.handle.Init.BaudRatePrescaler = SPI5_BAUDRATEPRESCALER;
	spi5.handle.Init.Direction         = SPI5_DIRECTION;
	spi5.handle.Init.CLKPhase          = SPI5_PHASE;
	spi5.handle.Init.CLKPolarity       = SPI5_POLARITY;
	spi5.handle.Init.CRCCalculation    = SPI5_CRCCALCULATION;
	spi5.handle.Init.CRCPolynomial     = SPI5_CRCPOLYNOMIAL;
	spi5.handle.Init.DataSize          = SPI5_DATASIZE;
	spi5.handle.Init.FirstBit          = SPI5_FIRSTBIT;
	spi5.handle.Init.NSS               = SPI5_NSS;
	spi5.handle.Init.TIMode            = SPI5_TIMODE;
	spi5.handle.Init.Mode              = SPI5_MODE;		
	if(HAL_SPI_Init(&spi5.handle) != HAL_OK)
		result = -1;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	//spi5.mutex_id = osMutexCreate(osMutex(spi));
	spi5.mutex_id = spi_com_mutex_id;
	if (!spi5.message_queue_id)
		spi5.message_queue_id = osMessageCreate (osMessageQ(spi), NULL);	
#endif /*BSP_CONFIG_USE_FREERTOS*/
#endif /*BSP_CONFIG_USE_SPI5*/
	
	return result;
}

/**
  * @brief  SPI deinitialization
  * @param  None
  * @retval None
  */
void SPI_Deinit(void)
{	
#ifdef BSP_CONFIG_USE_SPI1
	HAL_SPI_DeInit(&spi1.handle);	
#endif /*BSP_CONFIG_USE_SPI1*/
	
#ifdef BSP_CONFIG_USE_SPI2
	HAL_SPI_DeInit(&spi2.handle);
#endif /*BSP_CONFIG_USE_SPI2*/
	
#ifdef BSP_CONFIG_USE_SPI3
	HAL_SPI_DeInit(&spi3.handle);
#endif /*BSP_CONFIG_USE_SPI3*/
	
#ifdef BSP_CONFIG_USE_SPI4
	HAL_SPI_DeInit(&spi4.handle);
#endif /*BSP_CONFIG_USE_SPI4*/
	
#ifdef BSP_CONFIG_USE_SPI5
	HAL_SPI_DeInit(&spi5.handle);
#endif /*BSP_CONFIG_USE_SPI5*/
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
int32_t SPI_SequentialTransmit(const SPI_TypeDef *Instance, SPI_CS_T *CS, char *TxBuffer1, uint32_t  TxSize1, char *TxBuffer2, uint32_t  TxSize2)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	SPI_T *spi;
	osEvent evt;	
	
	if (SPI_GetHandle(Instance, &spi))
		return -1;

	if( osMutexWait( spi->mutex_id, SPI_TIMEOUT ) != osOK )
	{
		spi->timeout_errors++;
		return -1;
	}

	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit_IT(&spi->handle, (uint8_t*)TxBuffer1, TxSize1) != HAL_OK)
		goto error;

	evt = osMessageGet(spi->message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage) 
		goto timeout;
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;
	
	if(HAL_SPI_Transmit_IT(&spi->handle, (uint8_t*)TxBuffer2, TxSize2) != HAL_OK)
		goto error;

	evt = osMessageGet(spi->message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage) 
		goto timeout;
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;
	
	if (CS)
		CS->SetState(CS->Priv, 1);

	osMutexRelease( spi->mutex_id );	
	return 0;

timeout:
	spi->seq_tx_timeout_errors++;
error:
	HAL_SPI_Abort_IT(&spi->handle);
	HAL_SPI_DeInit(&spi->handle);
	HAL_SPI_Init(&spi->handle);
	if (CS)
		CS->SetState(CS->Priv, 1);
	osMutexRelease( spi->mutex_id );
	return -1;	
#else /*BSP_CONFIG_USE_FREERTOS*/
	SPI_T *spi;
	
	if (SPI_GetHandle(Instance, &spi))
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit(&spi->handle, (uint8_t *)TxBuffer1, TxSize1, SPI_TIMEOUT) != HAL_OK)
		goto error;
	
	if(HAL_SPI_Transmit(&spi->handle, (uint8_t *)TxBuffer2, TxSize2, SPI_TIMEOUT) != HAL_OK)
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
int32_t SPI_SequentialReceive(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *TxBuffer, uint32_t  TxSize, char * RxBuffer, uint32_t  RxSize)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	SPI_T *spi;
	osEvent evt;
	
	if (SPI_GetHandle(Instance, &spi))
		return -1;

	if( osMutexWait( spi->mutex_id, SPI_TIMEOUT ) != osOK )
	{
		spi->timeout_errors++;
		return -1;
	}
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit_IT(&spi->handle, (uint8_t*)TxBuffer, TxSize) != HAL_OK)
		goto error;

	evt = osMessageGet(spi->message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage) 
		goto timeout;
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;
	
	if(HAL_SPI_Receive_IT(&spi->handle, (uint8_t *)RxBuffer, RxSize) != HAL_OK)
		goto error;

	evt = osMessageGet(spi->message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage)
		goto timeout;
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;

	if (CS)
		CS->SetState(CS->Priv, 1);
	
	osMutexRelease( spi->mutex_id );	
	return 0;

timeout:
	spi->seq_rx_timeout_errors++;
error:
	HAL_SPI_Abort_IT(&spi->handle);
	HAL_SPI_DeInit(&spi->handle);
	HAL_SPI_Init(&spi->handle);
	if (CS)
		CS->SetState(CS->Priv, 1);
	osMutexRelease( spi->mutex_id );
	return -1;	
#else /*BSP_CONFIG_USE_FREERTOS*/
	SPI_T *spi;
	
	if (SPI_GetHandle(Instance, &spi))
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit(&spi->handle, (uint8_t*)TxBuffer, TxSize, SPI_TIMEOUT) != HAL_OK)
		goto error;
	
	if(HAL_SPI_Receive(&spi->handle, (uint8_t *)RxBuffer, RxSize, SPI_TIMEOUT) != HAL_OK)
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
int32_t SPI_TransmitReceive(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *TxBuffer, char * RxBuffer, uint32_t  Size)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	SPI_T *spi;
	osEvent evt;	
	
	if (SPI_GetHandle(Instance, &spi))
		return -1;

	if( osMutexWait( spi->mutex_id, SPI_TIMEOUT ) != osOK )
	{
		spi->timeout_errors++;
		return -1;
	}
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_TransmitReceive_IT(&spi->handle, (uint8_t*)TxBuffer, (uint8_t *)RxBuffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(spi->message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage)
		goto timeout;
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;

	if (CS)
		CS->SetState(CS->Priv, 1);
	
	osMutexRelease( spi->mutex_id );	
	return 0;

timeout:
	spi->tx_rx_timeout_errors++;
error:
	HAL_SPI_Abort_IT(&spi->handle);
	HAL_SPI_DeInit(&spi->handle);
	HAL_SPI_Init(&spi->handle);
	if (CS)
		CS->SetState(CS->Priv, 1);
	osMutexRelease( spi->mutex_id );
	return -1;	
#else /*BSP_CONFIG_USE_FREERTOS*/
	SPI_T *spi;
	
	if (SPI_GetHandle(Instance, &spi))
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_TransmitReceive(&spi->handle, (uint8_t*)TxBuffer, (uint8_t *)RxBuffer, Size, SPI_TIMEOUT) != HAL_OK)
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
int32_t SPI_Transmit(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *Buffer, uint32_t  Size)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	SPI_T *spi;
	osEvent evt;	
	
	if (SPI_GetHandle(Instance, &spi))
		return -1;

	if( osMutexWait( spi->mutex_id, SPI_TIMEOUT ) != osOK )
	{
		spi->timeout_errors++;
		return -1;
	}

	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit_IT(&spi->handle, (uint8_t*)Buffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(spi->message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage) 
		goto timeout;
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;
	
	if (CS)
		CS->SetState(CS->Priv, 1);

	osMutexRelease( spi->mutex_id );	
	return 0;

timeout:
	spi->tx_timeout_errors++;
error:
	HAL_SPI_Abort_IT(&spi->handle);
	HAL_SPI_DeInit(&spi->handle);
	HAL_SPI_Init(&spi->handle);
	if (CS)
		CS->SetState(CS->Priv, 1);
	osMutexRelease( spi->mutex_id );
	return -1;	
#else /*BSP_CONFIG_USE_FREERTOS*/
	SPI_T *spi;
	
	if (SPI_GetHandle(Instance, &spi))
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Transmit(&spi->handle, (uint8_t *)Buffer, Size, SPI_TIMEOUT) != HAL_OK)
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
int32_t SPI_Receive(const SPI_TypeDef * Instance, SPI_CS_T *CS, char *Buffer, uint32_t  Size)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	SPI_T *spi;
	osEvent evt;	
	
	if (SPI_GetHandle(Instance, &spi))
		return -1;

	if( osMutexWait( spi->mutex_id, SPI_TIMEOUT ) != osOK )
	{
		spi->timeout_errors++;
		return -1;
	}
	
	if (CS)
		CS->SetState(CS->Priv, 0);

	if(HAL_SPI_Receive_IT(&spi->handle, (uint8_t *)Buffer, Size) != HAL_OK)
		goto error;

	evt = osMessageGet(spi->message_queue_id, SPI_TIMEOUT);
	if (evt.status != osEventMessage) 
		goto timeout;
	
	if (evt.value.v != SPI_EVENT_COMPLETE)
		goto error;
	
	if (CS)
		CS->SetState(CS->Priv, 1);
	
	osMutexRelease( spi->mutex_id );	
	return 0;

timeout:
	spi->rx_timeout_errors++;
error:
	HAL_SPI_Abort_IT(&spi->handle);
	HAL_SPI_DeInit(&spi->handle);
	HAL_SPI_Init(&spi->handle);
	if (CS)
		CS->SetState(CS->Priv, 1);
	osMutexRelease( spi->mutex_id );
	return -1;
#else /*BSP_CONFIG_USE_FREERTOS*/
	SPI_T *spi;
	
	if (SPI_GetHandle(Instance, &spi))
		return -1;
	
	if (CS)
		CS->SetState(CS->Priv, 0);
	
	if(HAL_SPI_Receive(&spi->handle, (uint8_t *)Buffer, Size, SPI_TIMEOUT) != HAL_OK)
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
