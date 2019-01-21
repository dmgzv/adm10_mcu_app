#ifndef __NOR_H
#define __NOR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "bsp_config.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

int32_t NOR_Init(void);

#endif /* __NOR_H */
