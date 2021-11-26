#ifndef __BSP_PS_H
#define __BSP_PS_H

#include "ps.h"

#define BSP_PS_CNT	2
#define BSP_PS_I2C	I2C3

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_PS
#endif

extern PS_T ps[BSP_PS_CNT];
	
void BSP_PS_Process(void);

#endif /* __BSP_PS_H */
