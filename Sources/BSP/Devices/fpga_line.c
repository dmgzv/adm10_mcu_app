#include "fpga_line.h"


int32_t FPGA_LINE_Init(FPGA_LINE_T *Dev)
{
	
	switch (Dev->Type)
	{
		case FPGA_LINE_TYPE_ADM_10:
			if (FPGA_LINE_ADM_10_Update(&Dev->Instance.adm_10))
				goto error;
			break;
		case FPGA_LINE_TYPE_AGG_2X10_BS:
			if (FPGA_LINE_AGG_2X10_BS_Update(&Dev->Instance.agg_2x10_bs))
				goto error;
			break;
		default:
			goto error;
	}
	
}
int32_t FPGA_LINE_Update(FPGA_LINE_T *Dev)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	switch (Dev->Type)
	{
		case FPGA_LINE_TYPE_ADM_10:
			if (FPGA_LINE_ADM_10_Update(&Dev->Instance.adm_10))
				goto error;
			break;
		case FPGA_LINE_TYPE_AGG_2X10_BS:
			if (FPGA_LINE_AGG_2X10_BS_Update(&Dev->Instance.agg_2x10_bs))
				goto error;
			break;
		default:
			goto error;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;

error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return 0;	
}
void FPGA_LINE_Deinit(FPGA_LINE_T *Dev)
{
	
}