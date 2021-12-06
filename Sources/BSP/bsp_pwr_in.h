#ifndef __BSP_PWR_IN_H
#define __BSP_PWR_IN_H

//#define BSP_PWR_IN_DIM3R3400_ENABLE
//#define BSP_PWR_IN_POWER_RESET_TIMEOUT 60

#ifdef BSP_PWR_IN_DIM3R3400_ENABLE

#include "dim3r3400.h"

#define BSP_PWR_IN_DIM3R3400_ID				"A5"
#define BSP_PWR_IN_DIM3R3400_I2C			I2C3
#define BSP_PWR_IN_DIM3R3400_I2C_ADDR	(0x2B << 1)

#define BSP_PWR_IN_MIN_OFF	36.0f
#define BSP_PWR_IN_MAX_OFF	74.0f

#define BSP_PWR_IN_MIN_ON		37.0f
#define BSP_PWR_IN_MAX_ON		73.0f

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_PWR_IN
#endif

extern DIM3R3400_T dim3r3400;
extern uint32_t dim3r3400_power_good;
#ifdef BSP_PWR_IN_POWER_RESET_TIMEOUT
extern uint32_t dim3r3400_power_reset_timeout;
#endif
#endif

void BSP_PWR_IN_Process(void);

#endif /* __BSP_PWR_IN_H */
