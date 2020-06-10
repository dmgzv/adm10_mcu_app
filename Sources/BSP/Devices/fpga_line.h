#ifndef __FPGA_LINE_H
#define __FPGA_LINE_H

#include "fpga_line_adm_10.h"
#include "fpga_line_agg_2x10_bs.h"

#define FPGA_LINE_TYPE_ADM_10				0
#define FPGA_LINE_TYPE_AGG_2X10_BS	1

typedef union {
	FPGA_LINE_ADM_10_T adm_10;
	FPGA_LINE_AGG_2X10_BS_T agg_2x10_bs;
} FPGA_LINE_INSTANCE_T;

typedef struct {
	BSP_DEV;
	uint32_t Type;
	FPGA_LINE_INSTANCE_T Instance;
} FPGA_LINE_T;

int32_t FPGA_LINE_Init(FPGA_LINE_T *Dev);
int32_t FPGA_LINE_Update(FPGA_LINE_T *Dev);
void FPGA_LINE_Deinit(FPGA_LINE_T *Dev);

#endif /* __FPGA_LINE_H */