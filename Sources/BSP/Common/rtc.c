#include "rtc.h"

#if (RCC_RTCCLKSOURCE == RCC_RTCCLKSOURCE_LSE)
/*
	LSE is 32.768 kHz RTC clock is 1Hz.
	((RTC_ASYNCH_PREDIV + 1) * (RTC_SYNCH_PREDIV + 1)) = 32768
	*/
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */

#if (((RTC_ASYNCH_PREDIV + 1) * (RTC_SYNCH_PREDIV + 1)) != 32768)
#error	Incorrect RTC ASYNCH and SYNCH dividers
#endif

#elif (RCC_RTCCLKSOURCE == RCC_RTCCLKSOURCE_LSI)
/*
	LSI is 32.000 kHz RTC clock is 1Hz.
	((RTC_ASYNCH_PREDIV + 1) * (RTC_SYNCH_PREDIV + 1)) = 32000
	*/
#define RTC_ASYNCH_PREDIV  0x7F
#define RTC_SYNCH_PREDIV   0x00F9

#if (((RTC_ASYNCH_PREDIV + 1) * (RTC_SYNCH_PREDIV + 1)) != 32000)
#error	Incorrect RTC ASYNCH and SYNCH dividers
#endif

#else

#define RTC_ASYNCH_PREDIV  99   /* HSE as RTC clock */
#define RTC_SYNCH_PREDIV   9999 /* HSE as RTC clock */

#if (HSE_VALUE % ((RCC_RTCCLKSOURCE >> 16) & 0x1F))
#error	Incorrect RTC HSE divider
#endif

#if (((RTC_ASYNCH_PREDIV + 1) * (RTC_SYNCH_PREDIV + 1)) != (HSE_VALUE / ((RCC_RTCCLKSOURCE >> 16) & 0x1F)))
#error	Incorrect RTC ASYNCH and SYNCH dividers
#endif

#endif

RTC_HandleTypeDef RtcHandle; 

static int32_t RTC_InitDateTime(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2014 */
  sdatestructure.Year = 0x14;
  sdatestructure.Month = RTC_MONTH_FEBRUARY;
  sdatestructure.Date = 0x18;
  sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    return -1;
  } 
  
  /*##-2- Configure the Time #################################################*/
  /* Set Time: 02:00:00 */
  stimestructure.Hours = 0x02;
  stimestructure.Minutes = 0x00;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    return -1;
  }
  
  /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
  HAL_RTCEx_BKUPWrite(&RtcHandle,RTC_BKP_DR0,0x32F2);
	
	return 0;
}

int32_t RTC_Init(void)
{
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

#if (RCC_RTCCLKSOURCE == RCC_RTCCLKSOURCE_LSE)
	RCC_OscInitTypeDef        RCC_OscInitStruct;
	/* Enable LSE Oscillator */
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
#elif (RCC_RTCCLKSOURCE == RCC_RTCCLKSOURCE_LSI)
	RCC_OscInitTypeDef        RCC_OscInitStruct;
	/* Enable LSI Oscillator */
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
#endif
	
	/* Configure RTC clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
	
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

  /* Configure RTC prescaler and RTC data registers */
  RtcHandle.Instance = RTC; 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  __HAL_RTC_RESET_HANDLE_STATE(&RtcHandle);
  if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
    return -1;
	
	if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR0) != 0x32F2)
	{
		if (RTC_InitDateTime())
			return -1;
	}
	return 0;
}

void RTC_Deinit(void)
{
	HAL_RTC_DeInit(&RtcHandle);
	__HAL_RCC_RTC_DISABLE();
}

int32_t RTC_GetTime(RTC_TimeTypeDef *Time)
{
	if (HAL_RTC_GetTime(&RtcHandle, Time, RTC_FORMAT_BIN) != HAL_OK)
		return -1;
	return 0;
}

int32_t RTC_GetDate(RTC_DateTypeDef *Date)
{
	if (HAL_RTC_GetDate(&RtcHandle, Date, RTC_FORMAT_BIN) != HAL_OK)
		return -1;
	return 0;
}

int32_t RTC_SetTime(RTC_TimeTypeDef *Time)
{
	if (HAL_RTC_SetTime(&RtcHandle, Time, RTC_FORMAT_BIN) != HAL_OK)
		return -1;
	return 0;
}

int32_t RTC_SetDate(RTC_DateTypeDef *Date)
{
	if (HAL_RTC_SetDate(&RtcHandle, Date, RTC_FORMAT_BIN) != HAL_OK)
		return -1;
	return 0;
}
