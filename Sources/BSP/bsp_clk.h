#ifndef __BSP_CLK_H
#define __BSP_CLK_H

#include "si5332.h"

#define BSP_CLK_SI5332_CNT	2

#define BSP_CLK_SI5332_1_I2C			I2C3
#define BSP_CLK_SI5332_1_I2C_ADDR	0x6A

#define BSP_CLK_SI5332_2_I2C			I2C3
#define BSP_CLK_SI5332_2_I2C_ADDR	0x6B

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_CLK
#endif

extern SI5332_T si5332[BSP_CLK_SI5332_CNT];

void BSP_CLK_Process(void);

#endif /* __BSP_CLK_H */
