#ifndef __BSP_FPGA_H
#define __BSP_FPGA_H

#include "stm32f4xx_hal.h"
#include "bsp_config.h"
#include "bsp_dev.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#define BSP_FPGA_LINE_ADDR	0x68000000
#define BSP_FPGA_CLIENT_1_ADDR	0x68002000
#define BSP_FPGA_CLIENT_2_ADDR	0x68004000
#define BSP_FPGA_MAXV_ADDR	0x68006000

typedef struct {
	BSP_DEV;
	SRAM_HandleTypeDef hsram;
	uint8_t SwVersion;
}BSP_FPGA_T;

int32_t BSP_FPGA_Init(BSP_FPGA_T *Dev);

#endif /* __BSP_FPGA_H */
