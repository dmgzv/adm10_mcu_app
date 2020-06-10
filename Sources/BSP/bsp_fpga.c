#include "bsp_fpga.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_clk.h"
#include "log.h"

FPGA_IFACE_T fpga_iface =
{
	.DevName = "FPGA_IFACE",
	.DevId = "",
	.InitContErrThr = 10,
	.UpdateContErrThr = 10,
};
FPGA_MAXV_T fpga_maxv =
{
	.DevName = "FPGA_MAXV",
	.DevId = "",
	.InitContErrThr = 10,
	.UpdateContErrThr = 10,
	.BaseAddr = BSP_FPGA_MAXV_BASE_ADDR,
};
FPGA_LINE_ADM_10_T fpga_line_adm_10  =
{
	.DevName = "FPGA_LINE",
	.DevId = "",
	.InitContErrThr = 10,
	.UpdateContErrThr = 10,
	.BaseAddr = BSP_FPGA_LINE_BASE_ADDR,
};

FPGA_LINE_AGG_2X10_BS_T fpga_line_agg_2x10_bs  =
{
	.DevName = "FPGA_LINE",
	.DevId = "",
	.InitContErrThr = 10,
	.UpdateContErrThr = 10,
	.BaseAddr = BSP_FPGA_LINE_BASE_ADDR,
};

FPGA_CLIENT_ADM_10_T fpga_client_adm_10[BSP_FPGA_CLIENT_CNT] =
{
	{
		.DevName = "FPGA_CLIENT_1",
		.DevId = "",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(0),
	},
	{
		.DevName = "FPGA_CLIENT_2",
		.DevId = "",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(1),
	},
};

FPGA_CLIENT_AGG_2X10_BS_T fpga_client_agg_2x10_bs[BSP_FPGA_CLIENT_CNT] =
{
	{
		.DevName = "FPGA_CLIENT_1",
		.DevId = "",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(0),
	},
	{
		.DevName = "FPGA_CLIENT_2",
		.DevId = "",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(1),
	},
};

static int32_t BSP_FPGA_CheckConditions(void)
{
	uint32_t i;
	
	if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good) || (!dim3r3400_load_timeout))
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
	return 0;
}

void BSP_FPGA_Process(void)
{
	uint32_t i;
	
	if (BSP_FPGA_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&fpga_iface) != BSP_DEV_STATE_RESET)
		{
			FPGA_IFACE_Deinit(&fpga_iface);
			FPGA_MAXV_Deinit(&fpga_maxv);
			FPGA_LINE_ADM_10_Deinit(&fpga_line_adm_10);
			FPGA_LINE_AGG_2X10_BS_Deinit(&fpga_line_agg_2x10_bs);
			for (i = 0; i < BSP_FPGA_CLIENT_CNT; i++)
			{
				FPGA_CLIENT_ADM_10_Deinit(&fpga_client_adm_10[i]);
				FPGA_CLIENT_AGG_2X10_BS_Deinit(&fpga_client_agg_2x10_bs[i]);
			}
		}
	}
	else
	{
		/* External faults absent */
		if(BSP_DEV_GET_STATE(&fpga_iface) != BSP_DEV_STATE_OK)
		{
			FPGA_IFACE_Init(&fpga_iface);
		}
		else
		{
			if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
			{
				FPGA_MAXV_Init(&fpga_maxv);
			}
			else
			{
				FPGA_MAXV_Update(&fpga_maxv);
				if (fpga_maxv.Status & FPGA_MAXV_STATUS_LINE_MASK)
				{
					if (fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10)
					{
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
						if (BSP_DEV_GET_STATE(&fpga_line_adm_10)  != BSP_DEV_STATE_OK)
						{
							FPGA_LINE_AGG_2X10_BS_Init(&fpga_line_agg_2x10_bs);
						}
						else
						{
							FPGA_LINE_AGG_2X10_BS_Update(&fpga_line_agg_2x10_bs);
						}
					}
				}
				else
				{
					FPGA_LINE_ADM_10_Deinit(&fpga_line_adm_10);
					FPGA_LINE_AGG_2X10_BS_Deinit(&fpga_line_agg_2x10_bs);
				}
				for (i = 0; i < BSP_FPGA_CLIENT_CNT; i++)
				{
					if (fpga_maxv.Status & FPGA_MAXV_STATUS_CLIENT_MASK(i))
					{
						if (fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10)
						{
							if (BSP_DEV_GET_STATE(&fpga_client_adm_10[i]) != BSP_DEV_STATE_OK)
							{
								FPGA_CLIENT_ADM_10_Init(&fpga_client_adm_10[i]);
							}
							else
							{
								FPGA_CLIENT_ADM_10_Update(&fpga_client_adm_10[i]);
							}
						}
						else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_BS)
						{
							if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[i]) != BSP_DEV_STATE_OK)
							{
								FPGA_CLIENT_AGG_2X10_BS_Init(&fpga_client_agg_2x10_bs[i]);
							}
							else
							{
								FPGA_CLIENT_AGG_2X10_BS_Update(&fpga_client_agg_2x10_bs[i]);
							}
						}
					}
					else
					{
						FPGA_CLIENT_ADM_10_Deinit(&fpga_client_adm_10[i]);
						FPGA_CLIENT_AGG_2X10_BS_Deinit(&fpga_client_agg_2x10_bs[i]);
					}
				}
			}
		}
	}
}
