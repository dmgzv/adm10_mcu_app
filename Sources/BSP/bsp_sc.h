#ifndef __BSP_SC_H
#define __BSP_SC_H

#include "sc.h"

#define BSP_SC_CNT	2
#define BSP_SC_I2C	I2C3

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_SC
#endif

extern SC_T sc[BSP_SC_CNT];
	
void BSP_SC_Process(void);

#endif /* __BSP_SC_H */
