#include "bsp_fpga_line.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_clk.h"
#include "bsp_fpga_maxv.h"
#include "bsp_fpga_flash.h"
#include "bsp_fpga_fw.h"
#include "log.h"

FPGA_LINE_ADM_10_T fpga_line_adm_10  =
{
	.DevName = "FPGA_LINE_ADM_10",
	.DevId = "",
	.InitContErrThr = 10,
	.UpdateContErrThr = 10,
	.BaseAddr = BSP_FPGA_LINE_BASE_ADDR,
};

FPGA_LINE_AGG_2X10_BS_T fpga_line_agg_2x10_bs  =
{
	.DevName = "FPGA_LINE_AGG_2X10_BS",
	.DevId = "",
	.InitContErrThr = 10,
	.UpdateContErrThr = 10,
	.BaseAddr = BSP_FPGA_LINE_BASE_ADDR,
};

FPGA_LINE_AGG_2X10_EFEC_T fpga_line_agg_2x10_efec  =
{
	.DevName = "FPGA_LINE_AGG_2X10_EFEC",
	.DevId = "",
	.InitContErrThr = 10,
	.UpdateContErrThr = 10,
	.BaseAddr = BSP_FPGA_LINE_BASE_ADDR,
};

static int32_t BSP_FPGA_LINE_CheckConditions(void)
{
	uint32_t i;
	
	if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good))
		return -1;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	for (i = 0; i < BSP_CLK_SI5332_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&si5332[i]) != BSP_DEV_STATE_OK)
			return -1;
	}
	if (BSP_DEV_GET_STATE(&s29gl01gs) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK) || (!(fpga_maxv.Status & FPGA_MAXV_STATUS_LINE_MASK)))
		return -1;
	if (BSP_DEV_GET_STATE(&fpga_fw) != BSP_DEV_STATE_OK)
		return -1;
	return 0;
}

void BSP_FPGA_LINE_Process(void)
{
	if (BSP_FPGA_LINE_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&fpga_line_adm_10) != BSP_DEV_STATE_RESET)
		{
			FPGA_LINE_ADM_10_Deinit(&fpga_line_adm_10);
		}
		if(BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs) != BSP_DEV_STATE_RESET)
		{
			FPGA_LINE_AGG_2X10_BS_Deinit(&fpga_line_agg_2x10_bs);
		}
		if(BSP_DEV_GET_STATE(&fpga_line_agg_2x10_efec) != BSP_DEV_STATE_RESET)
		{
			FPGA_LINE_AGG_2X10_EFEC_Deinit(&fpga_line_agg_2x10_efec);
		}
	}
	else
	{
		/* External faults absent */
		if (fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10)
		{
			if(BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs) != BSP_DEV_STATE_RESET)
			{
				FPGA_LINE_AGG_2X10_BS_Deinit(&fpga_line_agg_2x10_bs);
			}
			if(BSP_DEV_GET_STATE(&fpga_line_agg_2x10_efec) != BSP_DEV_STATE_RESET)
			{
				FPGA_LINE_AGG_2X10_EFEC_Deinit(&fpga_line_agg_2x10_efec);
			}
			if (BSP_DEV_GET_STATE(&fpga_line_adm_10)  != BSP_DEV_STATE_OK)
			{
				FPGA_LINE_ADM_10_Init(&fpga_line_adm_10);
			}
			else
			{
				FPGA_LINE_ADM_10_Update(&fpga_line_adm_10);
			}
		}
		else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_BS)
		{
			if(BSP_DEV_GET_STATE(&fpga_line_adm_10) != BSP_DEV_STATE_RESET)
			{
				FPGA_LINE_ADM_10_Deinit(&fpga_line_adm_10);
			}
			if(BSP_DEV_GET_STATE(&fpga_line_agg_2x10_efec) != BSP_DEV_STATE_RESET)
			{
				FPGA_LINE_AGG_2X10_EFEC_Deinit(&fpga_line_agg_2x10_efec);
			}
			if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs)  != BSP_DEV_STATE_OK)
			{
				FPGA_LINE_AGG_2X10_BS_Init(&fpga_line_agg_2x10_bs);
			}
			else
			{
				FPGA_LINE_AGG_2X10_BS_Update(&fpga_line_agg_2x10_bs);
			}
		}
		else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_EFEC)
		{
			if(BSP_DEV_GET_STATE(&fpga_line_adm_10) != BSP_DEV_STATE_RESET)
			{
				FPGA_LINE_ADM_10_Deinit(&fpga_line_adm_10);
			}
			if(BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs) != BSP_DEV_STATE_RESET)
			{
				FPGA_LINE_AGG_2X10_BS_Deinit(&fpga_line_agg_2x10_bs);
			}
			if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_efec)  != BSP_DEV_STATE_OK)
			{
				FPGA_LINE_AGG_2X10_EFEC_Init(&fpga_line_agg_2x10_efec);
			}
			else
			{
				FPGA_LINE_AGG_2X10_EFEC_Update(&fpga_line_agg_2x10_efec);
			}
		}
		else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5)
		{
			if(BSP_DEV_GET_STATE(&fpga_line_adm_10) != BSP_DEV_STATE_RESET)
			{
				FPGA_LINE_ADM_10_Deinit(&fpga_line_adm_10);
			}
			if(BSP_DEV_GET_STATE(&fpga_line_agg_2x10_efec) != BSP_DEV_STATE_RESET)
			{
				FPGA_LINE_AGG_2X10_EFEC_Deinit(&fpga_line_agg_2x10_efec);
			}
			if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs)  != BSP_DEV_STATE_OK)
			{
				FPGA_LINE_AGG_2X10_BS_Init(&fpga_line_agg_2x10_bs);
			}
			else
			{
				FPGA_LINE_AGG_2X10_BS_Update(&fpga_line_agg_2x10_bs);
			}
		}
		else
		{
			if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs)  != BSP_DEV_STATE_RESET)
			{
				FPGA_LINE_AGG_2X10_BS_Deinit(&fpga_line_agg_2x10_bs);
			}
			if(BSP_DEV_GET_STATE(&fpga_line_adm_10) != BSP_DEV_STATE_RESET)
			{
				FPGA_LINE_ADM_10_Deinit(&fpga_line_adm_10);
			}
			if(BSP_DEV_GET_STATE(&fpga_line_agg_2x10_efec) != BSP_DEV_STATE_RESET)
			{
				FPGA_LINE_AGG_2X10_EFEC_Deinit(&fpga_line_agg_2x10_efec);
			}
		}
	}
}
