#ifndef __BSP_FPGA_FW_H
#define __BSP_FPGA_FW_H

#include "fpga_maxv.h"

typedef struct {
	BSP_DEV;
	uint32_t Status;
	char Buffer[512];
	uint32_t Addr;
} FPGA_FW_T;

extern FPGA_FW_T fpga_fw;

void BSP_FPGA_FW_Process(void);

#endif /* __BSP_FPGA_FW_H */
