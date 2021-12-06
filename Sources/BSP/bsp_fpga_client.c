#include "bsp_fpga_client.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_clk.h"
#include "bsp_fpga_maxv.h"
#include "bsp_fpga_flash.h"
#include "bsp_fpga_fw.h"
#include "log.h"

FPGA_CLIENT_ADM_10_T fpga_client_adm_10[BSP_FPGA_CLIENT_CNT] =
{
	{
		.DevName = "FPGA_CLIENT_1_ADM_10",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(0),
	},
	{
		.DevName = "FPGA_CLIENT_2_ADM_10",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(1),
	},
};

FPGA_CLIENT_AGG_2X10_BS_T fpga_client_agg_2x10_bs[BSP_FPGA_CLIENT_CNT] =
{
	{
		.DevName = "FPGA_CLIENT_1_AGG_2X10_BS",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(0),
	},
	{
		.DevName = "FPGA_CLIENT_2_AGG_2X10_BS",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(1),
	},
};

FPGA_CLIENT_AGG_2X10_EFEC_T fpga_client_agg_2x10_efec[BSP_FPGA_CLIENT_CNT] =
{
	{
		.DevName = "FPGA_CLIENT_1_AGG_2X10_EFEC",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(0),
	},
	{
		.DevName = "FPGA_CLIENT_1_AGG_2X10_EFEC",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(1),
	},
};

FPGA_CLIENT_ADM_2_5_T fpga_client_adm_2_5[BSP_FPGA_CLIENT_CNT] =
{
	{
		.DevName = "FPGA_CLIENT_1_ADM_2_5",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(0),
	},
	{
		.DevName = "FPGA_CLIENT_2_ADM_2_5",
		.InitContErrThr = 10,
		.UpdateContErrThr = 10,
		.BaseAddr = BSP_FPGA_CLIENT_BASE_ADDR(1),
	},
};

static int32_t BSP_FPGA_CLIENT_CheckConditions(uint32_t Idx)
{
	uint32_t i;

#ifdef BSP_PWR_IN_DIM3R3400_ENABLE 
	if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good))
		return -1;
#endif
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	for (i = 0; i < BSP_CLK_SI5332_CNT; i++)
	{
		if ((BSP_DEV_GET_STATE(&si5332[i]) != BSP_DEV_STATE_OK) || (si5332[i].Status != SI5332_STATUS_ACTIVE))
			return -1;
	}
	if (BSP_DEV_GET_STATE(&s29gl01gs) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK) || (!(fpga_maxv.Status & FPGA_MAXV_STATUS_CLIENT_MASK(Idx))))
		return -1;
	if (BSP_DEV_GET_STATE(&fpga_fw) != BSP_DEV_STATE_OK)
		return -1;
	return 0;
}

void BSP_FPGA_CLIENT_Process(void)
{
	uint32_t i;
	
	for (i = 0; i < BSP_FPGA_CLIENT_CNT; i++)
	{
		if (BSP_FPGA_CLIENT_CheckConditions(i))
		{
			/* External faults present */
			if(BSP_DEV_GET_STATE(&fpga_client_adm_10[i]) != BSP_DEV_STATE_RESET)
			{
				FPGA_CLIENT_ADM_10_Deinit(&fpga_client_adm_10[i]);
			}
			if(BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[i]) != BSP_DEV_STATE_RESET)
			{
				FPGA_CLIENT_AGG_2X10_BS_Deinit(&fpga_client_agg_2x10_bs[i]);
			}
			if(BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[i]) != BSP_DEV_STATE_RESET)
			{
				FPGA_CLIENT_AGG_2X10_EFEC_Deinit(&fpga_client_agg_2x10_efec[i]);
			}
			if(BSP_DEV_GET_STATE(&fpga_client_adm_2_5[i]) != BSP_DEV_STATE_RESET)
			{
				FPGA_CLIENT_ADM_2_5_Deinit(&fpga_client_adm_2_5[i]);
			}
		}
		else
		{
			/* External faults absent */
			if (fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10)
			{
				if(BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_AGG_2X10_BS_Deinit(&fpga_client_agg_2x10_bs[i]);
				}
				if(BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_AGG_2X10_EFEC_Deinit(&fpga_client_agg_2x10_efec[i]);
				}
				if(BSP_DEV_GET_STATE(&fpga_client_adm_2_5[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_ADM_2_5_Deinit(&fpga_client_adm_2_5[i]);
				}
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
				if(BSP_DEV_GET_STATE(&fpga_client_adm_10[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_ADM_10_Deinit(&fpga_client_adm_10[i]);
				}
				if(BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_AGG_2X10_EFEC_Deinit(&fpga_client_agg_2x10_efec[i]);
				}
				if(BSP_DEV_GET_STATE(&fpga_client_adm_2_5[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_ADM_2_5_Deinit(&fpga_client_adm_2_5[i]);
				}
				if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[i]) != BSP_DEV_STATE_OK)
				{
					FPGA_CLIENT_AGG_2X10_BS_Init(&fpga_client_agg_2x10_bs[i]);
				}
				else
				{
					FPGA_CLIENT_AGG_2X10_BS_Update(&fpga_client_agg_2x10_bs[i]);
				}
			}
			else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_EFEC)
			{
				if(BSP_DEV_GET_STATE(&fpga_client_adm_10[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_ADM_10_Deinit(&fpga_client_adm_10[i]);
				}
				if(BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_AGG_2X10_BS_Deinit(&fpga_client_agg_2x10_bs[i]);
				}
				if(BSP_DEV_GET_STATE(&fpga_client_adm_2_5[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_ADM_2_5_Deinit(&fpga_client_adm_2_5[i]);
				}
				if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[i]) != BSP_DEV_STATE_OK)
				{
					FPGA_CLIENT_AGG_2X10_EFEC_Init(&fpga_client_agg_2x10_efec[i]);
				}
				else
				{
					FPGA_CLIENT_AGG_2X10_EFEC_Update(&fpga_client_agg_2x10_efec[i]);
				}
			}
			else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5)
			{
				if (i == 0)
				{
					if(BSP_DEV_GET_STATE(&fpga_client_adm_10[i]) != BSP_DEV_STATE_RESET)
					{
						FPGA_CLIENT_ADM_10_Deinit(&fpga_client_adm_10[i]);
					}
					if(BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[i]) != BSP_DEV_STATE_RESET)
					{
						FPGA_CLIENT_AGG_2X10_EFEC_Deinit(&fpga_client_agg_2x10_efec[i]);
					}
					if(BSP_DEV_GET_STATE(&fpga_client_adm_2_5[i]) != BSP_DEV_STATE_RESET)
					{
						FPGA_CLIENT_ADM_2_5_Deinit(&fpga_client_adm_2_5[i]);
					}
					if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[i]) != BSP_DEV_STATE_OK)
					{
						FPGA_CLIENT_AGG_2X10_BS_Init(&fpga_client_agg_2x10_bs[i]);
					}
					else
					{
						FPGA_CLIENT_AGG_2X10_BS_Update(&fpga_client_agg_2x10_bs[i]);
					}
				}
				else
				{
					if(BSP_DEV_GET_STATE(&fpga_client_adm_10[i]) != BSP_DEV_STATE_RESET)
					{
						FPGA_CLIENT_ADM_10_Deinit(&fpga_client_adm_10[i]);
					}
					if(BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[i]) != BSP_DEV_STATE_RESET)
					{
						FPGA_CLIENT_AGG_2X10_BS_Deinit(&fpga_client_agg_2x10_bs[i]);
					}
					if(BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[i]) != BSP_DEV_STATE_RESET)
					{
						FPGA_CLIENT_AGG_2X10_EFEC_Deinit(&fpga_client_agg_2x10_efec[i]);
					}
					if (BSP_DEV_GET_STATE(&fpga_client_adm_2_5[i]) != BSP_DEV_STATE_OK)
					{
						FPGA_CLIENT_ADM_2_5_Init(&fpga_client_adm_2_5[i]);
					}
					else
					{
						FPGA_CLIENT_ADM_2_5_Update(&fpga_client_adm_2_5[i]);
					}
				}
			}
			else
			{
				if(BSP_DEV_GET_STATE(&fpga_client_adm_10[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_ADM_10_Deinit(&fpga_client_adm_10[i]);
				}
				if(BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_AGG_2X10_BS_Deinit(&fpga_client_agg_2x10_bs[i]);
				}
				if(BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_AGG_2X10_EFEC_Deinit(&fpga_client_agg_2x10_efec[i]);
				}
				if(BSP_DEV_GET_STATE(&fpga_client_adm_2_5[i]) != BSP_DEV_STATE_RESET)
				{
					FPGA_CLIENT_ADM_2_5_Deinit(&fpga_client_adm_2_5[i]);
				}
			}
		}
	}
}
