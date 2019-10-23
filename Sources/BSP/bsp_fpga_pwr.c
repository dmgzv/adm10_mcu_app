#include "bsp_fpga_pwr.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_sfp_i2c_switch.h"

const I2C_TypeDef *bsp_fpga_pwr_gdt080a_i2c[BSP_FPGA_PWR_GDT080A_CNT] = {BSP_FPGA_PWR_GDT080A_0V9_I2C, BSP_FPGA_PWR_GDT080A_0V95_I2C};
const uint8_t bsp_fpga_pwr_gdt080a_i2c_addr[BSP_FPGA_PWR_GDT080A_CNT] = {BSP_FPGA_PWR_GDT080A_0V9_I2C_ADDR, BSP_FPGA_PWR_GDT080A_0V95_I2C_ADDR};
const float bsp_fpga_pwr_gdt080a_vout[BSP_FPGA_PWR_GDT080A_CNT] = {GDT080A_RTRIM_TO_VOUT(BSP_FPGA_PWR_GDT080A_0V9_RTIM), GDT080A_RTRIM_TO_VOUT(BSP_FPGA_PWR_GDT080A_0V95_RTIM)};

GDT080A_T gdt080a[BSP_FPGA_PWR_GDT080A_CNT];

static int32_t BSP_FPGA_PWR_CheckConditions(void)
{
	uint32_t i;
	
	if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good))
		return -1;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	for (i = 0; i < BSP_PCA9548_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&pca9548a[i]) != BSP_DEV_STATE_OK)
			return -1;
	}
	return 0;
}

void BSP_FPGA_PWR_Process(void)
{	
	uint32_t i;
	if (BSP_FPGA_PWR_CheckConditions())
	{
		/* External faults present */
		for (i = 0; i < BSP_FPGA_PWR_GDT080A_CNT; i++)
		{
			if(BSP_DEV_GET_STATE(&gdt080a[i]) != BSP_DEV_STATE_RESET)
			{
				GDT080A_Deinit(&gdt080a[i]);
			}
		}
	}
	else
	{
		/* External faults absent */
		for (i = 0; i < BSP_FPGA_PWR_GDT080A_CNT; i++)
		{
			if(BSP_DEV_GET_STATE(&gdt080a[i]) != BSP_DEV_STATE_OK)
			{
				if (!BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_GDT_CH(i)))
				{
					GDT080A_Init(&gdt080a[i], (I2C_TypeDef *)bsp_fpga_pwr_gdt080a_i2c[i], bsp_fpga_pwr_gdt080a_i2c_addr[i], bsp_fpga_pwr_gdt080a_vout[i]);
					BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO);
				}
			}
			else
			{
				if (!BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_GDT_CH(i)))
				{
					GDT080A_Update(&gdt080a[i]);
					BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO);
				}
			}
		}			
	}
}

int32_t BSP_FPGA_PWR_SetVout(uint32_t Idx, float Vout)
{
	int32_t result = -1;
	if (!BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_GDT_CH(Idx)))
	{
		result = GDT080A_SetVout(&gdt080a[Idx], Vout);
		BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO);
	}
	return result;
}
