#ifndef __BSP_FPGA_H
#define __BSP_FPGA_H

#include "fpga_iface.h"
#include "fpga_maxv.h"
#include "fpga_line_adm_10.h"
#include "fpga_line_agg_2x10_bs.h"
#include "fpga_client_adm_10.h"
#include "fpga_client_agg_2x10_bs.h"
#include "fpga_client_adm_2_5.h"

#define BSP_FPGA_CLIENT_CNT	2

#define BSP_FPGA_MAXV_BASE_ADDR	0x68006000
#define BSP_FPGA_LINE_BASE_ADDR	0x68000000
#define BSP_FPGA_CLIENT_BASE_ADDR(Idx)	(0x68002000 + ((Idx) * 0x2000))

extern FPGA_IFACE_T fpga_iface;
extern FPGA_MAXV_T fpga_maxv;
extern FPGA_LINE_ADM_10_T fpga_line_adm_10;
extern FPGA_LINE_AGG_2X10_BS_T fpga_line_agg_2x10_bs;
extern FPGA_CLIENT_ADM_10_T fpga_client_adm_10[BSP_FPGA_CLIENT_CNT];
extern FPGA_CLIENT_AGG_2X10_BS_T fpga_client_agg_2x10_bs[BSP_FPGA_CLIENT_CNT];
extern FPGA_CLIENT_ADM_2_5_T fpga_client_adm_2_5[BSP_FPGA_CLIENT_CNT];

void BSP_FPGA_Process(void);

#endif /* __BSP_FPGA_H */
