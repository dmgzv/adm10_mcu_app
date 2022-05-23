#ifndef __BSP_FPGA_LINE_H
#define __BSP_FPGA_LINE_H

#include "fpga_line_adm_10.h"
#include "fpga_line_agg_2x10_bs.h"
#include "fpga_line_agg_2x10_efec.h"
#include "fpga_line_tp_10.h"

#define BSP_FPGA_LINE_BASE_ADDR	0x68000000

extern FPGA_LINE_ADM_10_T fpga_line_adm_10;
extern FPGA_LINE_AGG_2X10_BS_T fpga_line_agg_2x10_bs;
extern FPGA_LINE_AGG_2X10_EFEC_T fpga_line_agg_2x10_efec;
extern FPGA_LINE_TP_10_T fpga_line_tp_10;

void BSP_FPGA_LINE_Process(void);

#endif /* __BSP_FPGA_LINE_H */
