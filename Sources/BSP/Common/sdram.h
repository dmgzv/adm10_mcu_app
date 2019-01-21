#ifndef __SDRAM_H
#define __SDRAM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "bsp_config.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

int32_t SDRAM_Init(void);

#endif /* __SDRAM_H */
