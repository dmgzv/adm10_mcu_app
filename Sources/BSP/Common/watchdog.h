#ifndef __WATCHDOG_H
#define __WATCHDOG_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "stm32f4xx_hal.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#ifdef BSP_CONFIG_USE_WATCHDOG

void __WATCHDOG_Init(uint32_t Time);
void __WATCHDOG_Refresh(void);

#define WATCHDOG_Init(Time) __WATCHDOG_Init(Time)
#define WATCHDOG_Refresh(Time) __WATCHDOG_Refresh()

#else

#define WATCHDOG_Init(Time)
#define WATCHDOG_Refresh(Time)

#endif



#endif /*__WATCHDOG_H*/
