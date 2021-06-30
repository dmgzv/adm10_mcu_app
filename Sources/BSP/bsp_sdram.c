#include "bsp_sdram.h"

MT48LC16M16A2_T mt48lc16m16a2 =
{
	.DevName = MT48LC16M16A2_DEV_NAME"_"BSP_SDRAM_ID,
	.InitContErrThr = 0,
	.UpdateContErrThr = BSP_DEV_CONT_ERR_THR_INF,
};
