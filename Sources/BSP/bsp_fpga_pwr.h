#ifndef __BSP_FPGA_PWR_H
#define __BSP_FPGA_PWR_H

#include "gdt080a.h"
#include "bsp_pwr_pin.h"

#define BSP_FPGA_PWR_GDT080A_0V9		0
#define BSP_FPGA_PWR_GDT080A_0V95	1
#define BSP_FPGA_PWR_GDT080A_CNT		2
	
#define BSP_FPGA_PWR_GDT080A_0V9_I2C				I2C3
#define BSP_FPGA_PWR_GDT080A_0V9_I2C_ADDR	42
#define BSP_FPGA_PWR_GDT080A_0V9_RTIM			4.0f /* 4 kOhm */

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_FPGA_PWR
#endif

#define BSP_FPGA_PWR_GDT080A_0V95_I2C				I2C3
#define BSP_FPGA_PWR_GDT080A_0V95_I2C_ADDR	34
#define BSP_FPGA_PWR_GDT080A_0V95_RTIM			3.4f /* 3.4 kOhm */
	
#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_FPGA_PWR
#endif

extern GDT080A_T gdt080a[BSP_FPGA_PWR_GDT080A_CNT];
	
void BSP_FPGA_PWR_Process(void);
int32_t BSP_FPGA_PWR_SetVout(uint32_t Idx, float Vout);
#endif /* __BSP_FPGA_PWR_H */
