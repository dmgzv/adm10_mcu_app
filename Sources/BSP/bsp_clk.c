#include "bsp_clk.h"
#include "bsp_clk_1_6p0_cfg.h"
#include "bsp_clk_1_6p0_efec_cfg.h"
#include "bsp_clk_2_6p0_cfg.h"
#include "bsp_clk_1_6p5_cfg.h"
#include "bsp_clk_1_6p5_efec_cfg.h"
#include "bsp_clk_2_6p5_cfg.h"
#include "bsp_clk_1_7p0_cfg.h"
#include "bsp_clk_1_7p0_efec_cfg.h"
#include "bsp_clk_2_7p0_cfg.h"
#include "bsp_clk_1_7p5_cfg.h"
#include "bsp_clk_1_7p5_efec_cfg.h"
#include "bsp_clk_2_7p5_cfg.h"
#include "bsp_clk_1_8p0_cfg.h"
#include "bsp_clk_1_8p0_efec_cfg.h"
#include "bsp_clk_2_8p0_cfg.h"
#include "bsp_clk_1_8p5_cfg.h"
#include "bsp_clk_1_8p5_efec_cfg.h"
#include "bsp_clk_2_8p5_cfg.h"
#include "bsp_clk_1_9p0_cfg.h"
#include "bsp_clk_1_9p0_efec_cfg.h"
#include "bsp_clk_2_9p0_cfg.h"
#include "bsp_clk_1_9p5_cfg.h"
#include "bsp_clk_1_9p5_efec_cfg.h"
#include "bsp_clk_2_9p5_cfg.h"
#include "bsp_clk_1_10p0_cfg.h"
#include "bsp_clk_1_10p0_efec_cfg.h"
#include "bsp_clk_2_10p0_cfg.h"
#include "bsp_clk_1_10p5_cfg.h"
#include "bsp_clk_1_10p5_efec_cfg.h"
#include "bsp_clk_2_10p5_cfg.h"
#include "bsp_clk_1_11p0_cfg.h"
#include "bsp_clk_1_11p0_efec_cfg.h"
#include "bsp_clk_2_11p0_cfg.h"
#include "bsp_clk_1_11p5_cfg.h"
#include "bsp_clk_1_11p5_efec_cfg.h"
#include "bsp_clk_2_11p5_cfg.h"
#include "bsp_clk_1_12p0_cfg.h"
#include "bsp_clk_1_12p0_efec_cfg.h"
#include "bsp_clk_2_12p0_cfg.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_sfp_i2c_switch.h"

static const SI5332_CFG_T* bsp_clk_si5332_cfg[BSP_CLK_SI5332_CNT][BSP_CLK_SI5332_CFG_CNT] = {
	{
		bsp_clk_1_6p0_cfg,
		bsp_clk_1_6p5_cfg,
		bsp_clk_1_7p0_cfg,
		bsp_clk_1_7p5_cfg,
		bsp_clk_1_8p0_cfg,
		bsp_clk_1_8p5_cfg,
		bsp_clk_1_9p0_cfg,
		bsp_clk_1_9p5_cfg,
		bsp_clk_1_10p0_cfg,
		bsp_clk_1_10p5_cfg,
		bsp_clk_1_11p0_cfg,
		bsp_clk_1_11p5_cfg,
		bsp_clk_1_12p0_cfg,
	},
	{
		bsp_clk_2_6p0_cfg,
		bsp_clk_2_6p5_cfg,
		bsp_clk_2_7p0_cfg,
		bsp_clk_2_7p5_cfg,
		bsp_clk_2_8p0_cfg,
		bsp_clk_2_8p5_cfg,
		bsp_clk_2_9p0_cfg,
		bsp_clk_2_9p5_cfg,
		bsp_clk_2_10p0_cfg,
		bsp_clk_2_10p5_cfg,
		bsp_clk_2_11p0_cfg,
		bsp_clk_2_11p5_cfg,
		bsp_clk_2_12p0_cfg,
	},
};
static const uint32_t bsp_clk_si5332_cfg_size[BSP_CLK_SI5332_CNT][BSP_CLK_SI5332_CFG_CNT] = {
	{
		sizeof(bsp_clk_1_6p0_cfg) / sizeof(bsp_clk_1_6p0_cfg[0]),
		sizeof(bsp_clk_1_6p5_cfg) / sizeof(bsp_clk_1_6p5_cfg[0]),
		sizeof(bsp_clk_1_7p0_cfg) / sizeof(bsp_clk_1_7p0_cfg[0]),
		sizeof(bsp_clk_1_7p5_cfg) / sizeof(bsp_clk_1_7p5_cfg[0]),
		sizeof(bsp_clk_1_8p0_cfg) / sizeof(bsp_clk_1_8p0_cfg[0]),
		sizeof(bsp_clk_1_8p5_cfg) / sizeof(bsp_clk_1_8p5_cfg[0]),
		sizeof(bsp_clk_1_9p0_cfg) / sizeof(bsp_clk_1_9p0_cfg[0]),
		sizeof(bsp_clk_1_9p5_cfg) / sizeof(bsp_clk_1_9p5_cfg[0]),
		sizeof(bsp_clk_1_10p0_cfg) / sizeof(bsp_clk_1_10p0_cfg[0]),
		sizeof(bsp_clk_1_10p5_cfg) / sizeof(bsp_clk_1_10p5_cfg[0]),
		sizeof(bsp_clk_1_11p0_cfg) / sizeof(bsp_clk_1_11p0_cfg[0]),
		sizeof(bsp_clk_1_11p5_cfg) / sizeof(bsp_clk_1_11p5_cfg[0]),
		sizeof(bsp_clk_1_12p0_cfg) / sizeof(bsp_clk_1_12p0_cfg[0]),
	},
	{	
		sizeof(bsp_clk_2_6p0_cfg) / sizeof(bsp_clk_2_6p0_cfg[0]),
		sizeof(bsp_clk_2_6p5_cfg) / sizeof(bsp_clk_2_6p5_cfg[0]),
		sizeof(bsp_clk_2_7p0_cfg) / sizeof(bsp_clk_2_7p0_cfg[0]),
		sizeof(bsp_clk_2_7p5_cfg) / sizeof(bsp_clk_2_7p5_cfg[0]),
		sizeof(bsp_clk_2_8p0_cfg) / sizeof(bsp_clk_2_8p0_cfg[0]),
		sizeof(bsp_clk_2_8p5_cfg) / sizeof(bsp_clk_2_8p5_cfg[0]),
		sizeof(bsp_clk_2_9p0_cfg) / sizeof(bsp_clk_2_9p0_cfg[0]),
		sizeof(bsp_clk_2_9p5_cfg) / sizeof(bsp_clk_2_9p5_cfg[0]),
		sizeof(bsp_clk_2_10p0_cfg) / sizeof(bsp_clk_2_10p0_cfg[0]),
		sizeof(bsp_clk_2_10p5_cfg) / sizeof(bsp_clk_2_10p5_cfg[0]),
		sizeof(bsp_clk_2_11p0_cfg) / sizeof(bsp_clk_2_11p0_cfg[0]),
		sizeof(bsp_clk_2_11p5_cfg) / sizeof(bsp_clk_2_11p5_cfg[0]),
		sizeof(bsp_clk_2_12p0_cfg) / sizeof(bsp_clk_2_12p0_cfg[0]),
	},
};

static const SI5332_CFG_T* bsp_clk_si5332_efec_cfg[BSP_CLK_SI5332_CNT][BSP_CLK_SI5332_CFG_CNT] = {
	{
		bsp_clk_1_6p0_efec_cfg,
		bsp_clk_1_6p5_efec_cfg,
		bsp_clk_1_7p0_efec_cfg,
		bsp_clk_1_7p5_efec_cfg,
		bsp_clk_1_8p0_efec_cfg,
		bsp_clk_1_8p5_efec_cfg,
		bsp_clk_1_9p0_efec_cfg,
		bsp_clk_1_9p5_efec_cfg,
		bsp_clk_1_10p0_efec_cfg,
		bsp_clk_1_10p5_efec_cfg,
		bsp_clk_1_11p0_efec_cfg,
		bsp_clk_1_11p5_efec_cfg,
		bsp_clk_1_12p0_efec_cfg,
	},
	{
		bsp_clk_2_6p0_cfg,
		bsp_clk_2_6p5_cfg,
		bsp_clk_2_7p0_cfg,
		bsp_clk_2_7p5_cfg,
		bsp_clk_2_8p0_cfg,
		bsp_clk_2_8p5_cfg,
		bsp_clk_2_9p0_cfg,
		bsp_clk_2_9p5_cfg,
		bsp_clk_2_10p0_cfg,
		bsp_clk_2_10p5_cfg,
		bsp_clk_2_11p0_cfg,
		bsp_clk_2_11p5_cfg,
		bsp_clk_2_12p0_cfg,
	},
};
static const uint32_t bsp_clk_si5332_cfg_efec_size[BSP_CLK_SI5332_CNT][BSP_CLK_SI5332_CFG_CNT] = {
	{
		sizeof(bsp_clk_1_6p0_efec_cfg) / sizeof(bsp_clk_1_6p0_efec_cfg[0]),
		sizeof(bsp_clk_1_6p5_efec_cfg) / sizeof(bsp_clk_1_6p5_efec_cfg[0]),
		sizeof(bsp_clk_1_7p0_efec_cfg) / sizeof(bsp_clk_1_7p0_efec_cfg[0]),
		sizeof(bsp_clk_1_7p5_efec_cfg) / sizeof(bsp_clk_1_7p5_efec_cfg[0]),
		sizeof(bsp_clk_1_8p0_efec_cfg) / sizeof(bsp_clk_1_8p0_efec_cfg[0]),
		sizeof(bsp_clk_1_8p5_efec_cfg) / sizeof(bsp_clk_1_8p5_efec_cfg[0]),
		sizeof(bsp_clk_1_9p0_efec_cfg) / sizeof(bsp_clk_1_9p0_efec_cfg[0]),
		sizeof(bsp_clk_1_9p5_efec_cfg) / sizeof(bsp_clk_1_9p5_efec_cfg[0]),
		sizeof(bsp_clk_1_10p0_efec_cfg) / sizeof(bsp_clk_1_10p0_efec_cfg[0]),
		sizeof(bsp_clk_1_10p5_efec_cfg) / sizeof(bsp_clk_1_10p5_efec_cfg[0]),
		sizeof(bsp_clk_1_11p0_efec_cfg) / sizeof(bsp_clk_1_11p0_efec_cfg[0]),
		sizeof(bsp_clk_1_11p5_efec_cfg) / sizeof(bsp_clk_1_11p5_efec_cfg[0]),
		sizeof(bsp_clk_1_12p0_efec_cfg) / sizeof(bsp_clk_1_12p0_efec_cfg[0]),
	},
	{	
		sizeof(bsp_clk_2_6p0_cfg) / sizeof(bsp_clk_2_6p0_cfg[0]),
		sizeof(bsp_clk_2_6p5_cfg) / sizeof(bsp_clk_2_6p5_cfg[0]),
		sizeof(bsp_clk_2_7p0_cfg) / sizeof(bsp_clk_2_7p0_cfg[0]),
		sizeof(bsp_clk_2_7p5_cfg) / sizeof(bsp_clk_2_7p5_cfg[0]),
		sizeof(bsp_clk_2_8p0_cfg) / sizeof(bsp_clk_2_8p0_cfg[0]),
		sizeof(bsp_clk_2_8p5_cfg) / sizeof(bsp_clk_2_8p5_cfg[0]),
		sizeof(bsp_clk_2_9p0_cfg) / sizeof(bsp_clk_2_9p0_cfg[0]),
		sizeof(bsp_clk_2_9p5_cfg) / sizeof(bsp_clk_2_9p5_cfg[0]),
		sizeof(bsp_clk_2_10p0_cfg) / sizeof(bsp_clk_2_10p0_cfg[0]),
		sizeof(bsp_clk_2_10p5_cfg) / sizeof(bsp_clk_2_10p5_cfg[0]),
		sizeof(bsp_clk_2_11p0_cfg) / sizeof(bsp_clk_2_11p0_cfg[0]),
		sizeof(bsp_clk_2_11p5_cfg) / sizeof(bsp_clk_2_11p5_cfg[0]),
		sizeof(bsp_clk_2_12p0_cfg) / sizeof(bsp_clk_2_12p0_cfg[0]),
	},
};

SI5332_T si5332[BSP_CLK_SI5332_CNT] =
{
	{
		.DevName = SI5332_DEV_NAME"_"BSP_CLK_SI5332_1_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_CLK_SI5332_1_I2C,
		.Addr = BSP_CLK_SI5332_1_I2C_ADDR,
	},
	{
		.DevName = SI5332_DEV_NAME"_"BSP_CLK_SI5332_2_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_CLK_SI5332_2_I2C,
		.Addr = BSP_CLK_SI5332_2_I2C_ADDR,
	},
};

uint32_t si5332_mode[BSP_CLK_SI5332_CNT] = {BSP_CLK_SI5332_MODE_RSFEC, BSP_CLK_SI5332_MODE_RSFEC};
static uint32_t __si5332_mode[BSP_CLK_SI5332_CNT];
uint32_t si5332_cfg[BSP_CLK_SI5332_CNT] = {BSP_CLK_SI5332_1_DEF_CFG, BSP_CLK_SI5332_2_DEF_CFG};
static uint32_t __si5332_cfg[BSP_CLK_SI5332_CNT];

static int32_t BSP_CLK_CheckConditions(void)
{
	uint32_t i;
	
	if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good))
		return -1;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	for (i = 0; i < BSP_PCA9548A_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&pca9548a[i]) != BSP_DEV_STATE_OK)
			return -1;
	}
	return 0;
}

void BSP_CLK_Process(void)
{
	SI5332_CFG_T *cfg;
	uint32_t cfg_size;
	uint32_t i;
	if (BSP_CLK_CheckConditions())
	{
		/* External faults present */
		for (i = 0; i < BSP_CLK_SI5332_CNT; i++)
		{
			if(BSP_DEV_GET_STATE(&si5332[i]) != BSP_DEV_STATE_RESET)
			{
				SI5332_Deinit(&si5332[i]);
			}
		}
	}
	else
	{
		/* External faults absent */
		for (i = 0; i < BSP_CLK_SI5332_CNT; i++)
		{
			if ((BSP_DEV_GET_STATE(&si5332[i]) != BSP_DEV_STATE_OK) || (si5332_mode[i] != __si5332_mode[i]) || (si5332_cfg[i] != __si5332_cfg[i]))
			{
				switch (si5332_cfg[i])
				{
					case BSP_CLK_SI5332_6P0_CFG:
					case BSP_CLK_SI5332_6P5_CFG:
					case BSP_CLK_SI5332_7P0_CFG:
					case BSP_CLK_SI5332_7P5_CFG:
					case BSP_CLK_SI5332_8P0_CFG:
					case BSP_CLK_SI5332_8P5_CFG:
					case BSP_CLK_SI5332_9P0_CFG:
					case BSP_CLK_SI5332_9P5_CFG:
					case BSP_CLK_SI5332_10P0_CFG:
					case BSP_CLK_SI5332_10P5_CFG:
					case BSP_CLK_SI5332_11P0_CFG:
					case BSP_CLK_SI5332_11P5_CFG:
					case BSP_CLK_SI5332_12P0_CFG:
						if (si5332_mode[i] == BSP_CLK_SI5332_MODE_RSFEC)
						{
							cfg = (SI5332_CFG_T*)bsp_clk_si5332_cfg[i][si5332_cfg[i]];
							cfg_size = bsp_clk_si5332_cfg_size[i][si5332_cfg[i]];
						}
						else if (si5332_mode[i] == BSP_CLK_SI5332_MODE_EFEC_I_4)
						{
							cfg = (SI5332_CFG_T*)bsp_clk_si5332_efec_cfg[i][si5332_cfg[i]];
							cfg_size = bsp_clk_si5332_cfg_efec_size[i][si5332_cfg[i]];
						}
						else
						{
							break;
						}
						if (!BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SI5332_CH(i)))
						{
							if (SI5332_Init(&si5332[i], cfg, cfg_size) == 0)
							{
								__si5332_cfg[i] = si5332_cfg[i];
								__si5332_mode[i] = si5332_mode[i];
							}
							BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO);
						}
						break;
					default:
						SI5332_Deinit(&si5332[i]);
						break;
				}
			}
			else
			{
				if (!BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SI5332_CH(i)))
				{
					SI5332_Update(&si5332[i]);
					BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO);
				}
			}
		}
	}
}
