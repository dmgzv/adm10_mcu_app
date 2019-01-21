#include "bsp_clk.h"
#include "bsp_clk_cfg.h"

static const uint8_t bsp_clk_si5332_i2c_addr[BSP_CLK_SI5332_CNT] = {BSP_CLK_SI5332_1_I2C_ADDR, BSP_CLK_SI5332_2_I2C_ADDR};
static const I2C_TypeDef* bsp_clk_si5332_i2c[BSP_CLK_SI5332_CNT] = {BSP_CLK_SI5332_1_I2C, BSP_CLK_SI5332_2_I2C};
int32_t BSP_CLK_Init(BSP_CLK_T *Dev)
{
	uint32_t i;
	int32_t result = 0;
	
	if (!Dev)
		return -1;
	
	for (i = 0; i < BSP_CLK_SI5332_CNT; i++)
	{
		if (SI5332_Init(&Dev->si5332[i], (I2C_TypeDef*) bsp_clk_si5332_i2c[i], bsp_clk_si5332_i2c_addr[i]))
		{
			result = -1;
			continue;
		}
		
		if (SI5332_Config(&Dev->si5332[i], (SI5332_CFG_T*)bsp_clk_cfg, sizeof(bsp_clk_cfg) / sizeof(bsp_clk_cfg[0])))
		{
			result = -1;
			continue;
		}
	}
		
	return result;
}
