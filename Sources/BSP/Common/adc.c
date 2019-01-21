#include "adc.h"

ADC_HandleTypeDef    AdcHandle;
__IO uint16_t uhADCxConvertedValue = 0;
/**
  * @brief  This function handles ADC interrupt request.
  * @param  None
  * @retval None
  */
void ADC_IRQHandler(void)
{
  HAL_ADC_IRQHandler(&AdcHandle);
}
/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Get the converted value of regular channel */
  uhADCxConvertedValue = HAL_ADC_GetValue(hadc);
}

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
	/* ADC1 Periph clock enable */
	__HAL_RCC_ADC1_CLK_ENABLE();
	/* Enable GPIO clock ****************************************/
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/
	/* ADC Channel GPIO pin configuration */
	GPIO_InitStruct.Pin = ADC_CHANNEL_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(ADC_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

	/*##-3- Configure the NVIC #################################################*/
	/* NVIC configuration for ADC interrupt */
	HAL_NVIC_SetPriority(ADC_IRQn, ADC_IRQ_PRIORITY, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);
}

/**
  * @brief ADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
		
	/*##-1- Reset peripherals ##################################################*/
	__HAL_RCC_ADC_FORCE_RESET();
	__HAL_RCC_ADC_RELEASE_RESET();

	/*##-2- Disable peripherals and GPIO Clocks ################################*/
	/* De-initialize the ADC Channel GPIO pin */
	HAL_GPIO_DeInit(ADC_CHANNEL_GPIO_PORT, ADC_CHANNEL_GPIO_PIN);
}

int32_t ADC_Init(void)
{	
#ifdef BSP_CONFIG_USE_ADC
	ADC_ChannelConfTypeDef sConfig;
	
	/*##-1- Configure the ADC peripheral #######################################*/
  AdcHandle.Instance          					= ADC1;  
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode disabled to have only 1 conversion at each conversion trig */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion   = 0;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;        /* Conversion start trigged at each external event */
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.NbrOfConversion       = 1;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  AdcHandle.Init.EOCSelection          = DISABLE;

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
		return -1;
	
  /*##-2- Configure ADC regular channel ######################################*/
  sConfig.Channel      = ADC_CHANNEL;
  sConfig.Rank         = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  sConfig.Offset       = 0;
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
		return -1;

  /*##-3- Start the conversion process #######################################*/
  if (HAL_ADC_Start_IT(&AdcHandle) != HAL_OK)
		return -1;
#endif
	return 0;
}

void ADC_Deinit(void)
{
#ifdef BSP_CONFIG_USE_ADC
	HAL_ADC_Stop_IT(&AdcHandle);
	HAL_ADC_DeInit(&AdcHandle);
#endif
}

float ADC_GetVoltage(void)
{
	return ((3.3 * uhADCxConvertedValue) / 0xFFF);
}
