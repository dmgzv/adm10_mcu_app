#include "bsp_clk.h"
#include "bsp_clk_cfg.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"

static const uint8_t bsp_clk_si5332_i2c_addr[BSP_CLK_SI5332_CNT] = {BSP_CLK_SI5332_1_I2C_ADDR, BSP_CLK_SI5332_2_I2C_ADDR};
static const I2C_TypeDef* bsp_clk_si5332_i2c[BSP_CLK_SI5332_CNT] = {BSP_CLK_SI5332_1_I2C, BSP_CLK_SI5332_2_I2C};

SI5332_T si5332[BSP_CLK_SI5332_CNT];

static int32_t BSP_CLK_CheckConditions(void)
{
	if (BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
		return -1;
	if (BSP_DEV_GET_STATE(&pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	return 0;
}

void BSP_CLK_Process(void)
{
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
			if(BSP_DEV_GET_STATE(&si5332[i]) != BSP_DEV_STATE_OK)
			{
				SI5332_Init(&si5332[i], (I2C_TypeDef*) bsp_clk_si5332_i2c[i], bsp_clk_si5332_i2c_addr[i], (SI5332_CFG_T*)bsp_clk_cfg, sizeof(bsp_clk_cfg) / sizeof(bsp_clk_cfg[0]));
			}
		}
	}
}
