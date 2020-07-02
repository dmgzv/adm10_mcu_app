#ifndef __FPGA_IFACE_H
#define __FPGA_IFACE_H

#include "stm32f4xx_hal.h"
#include "bsp_config.h"
#include "bsp_dev.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

typedef struct {
	BSP_DEV;
	SRAM_HandleTypeDef hsram;
} FPGA_IFACE_T;

int32_t FPGA_IFACE_Init(FPGA_IFACE_T *Dev);
void FPGA_IFACE_Deinit(FPGA_IFACE_T *Dev);

#endif /* __FPGA_IFACE_H */
