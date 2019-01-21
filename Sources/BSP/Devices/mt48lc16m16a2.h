#ifndef __MT48LC16M16A2_H
#define __MT48LC16M16A2_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "bsp_config.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#define MT48LC16M16A2_ADDR    ((uint32_t)0xD0000000)
#define MT48LC16M16A2_SIZE		((uint32_t)0x02000000)

int32_t MT48LC16M16A2_Init(void);

#endif /* __MT48LC16M16A2_H */
