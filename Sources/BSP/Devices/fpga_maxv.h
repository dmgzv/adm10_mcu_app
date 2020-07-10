#ifndef __FPGA_MAXV_H
#define __FPGA_MAXV_H

#include "stm32f4xx_hal.h"
#include "bsp_config.h"
#include "bsp_dev.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#define FPGA_MAXV_MODE_NONE								0
#define FPGA_MAXV_MODE_AGG_2X10_BS				1
#define FPGA_MAXV_MODE_ADM_10							2
#define FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5	4
#define FPGA_MAXV_MODE_SET(Dev, __Mode) (Dev)->Mode = __Mode;

#define FPGA_MAXV_STATUS_LINE_MASK				(1 << 1)
#define FPGA_MAXV_STATUS_CLIENT_MASK(Idx)	(1 << ((Idx) + 2))

typedef struct {
	BSP_DEV;
	const uint32_t BaseAddr;
	char SwVersion[9];
	uint8_t Status;
	uint32_t Mode;
	uint32_t __Mode;
} FPGA_MAXV_T;

int32_t FPGA_MAXV_Init(FPGA_MAXV_T *Dev);
int32_t FPGA_MAXV_Update(FPGA_MAXV_T *Dev);
void FPGA_MAXV_Deinit(FPGA_MAXV_T *Dev);

#endif /* __FPGA_MAXV_H */
