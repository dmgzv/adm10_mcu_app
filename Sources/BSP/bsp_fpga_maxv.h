#ifndef __BSP_FPGA_MAXV_H
#define __BSP_FPGA_MAXV_H

#include "fpga_maxv.h"

#define BSP_FPGA_MAXV_BASE_ADDR	0x68006000

extern FPGA_MAXV_T fpga_maxv;

void BSP_FPGA_MAXV_Process(void);

#endif /* __BSP_FPGA_MAXV_H */
