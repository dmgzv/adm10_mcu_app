#ifndef __RTC_H
#define __RTC_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "stm32f4xx_hal.h"
#include <time.h>
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#define RCC_RTCCLKSOURCE RCC_RTCCLKSOURCE_HSE_DIV25

int32_t RTC_Init(void);
void RTC_Deinit(void);

int32_t RTC_GetTime(RTC_TimeTypeDef *Time);
int32_t RTC_GetDate(RTC_DateTypeDef *Date);
int32_t RTC_SetTime(RTC_TimeTypeDef *Time);
int32_t RTC_SetDate(RTC_DateTypeDef *Date);

#endif /* __RTC_H */
