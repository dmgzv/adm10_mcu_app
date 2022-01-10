#include "bsp_sc.h"
#include "bsp_seeprom.h"
#include "string.h"

SC_T sc[BSP_SC_CNT] =
{
	{
		.DevName = SC_DEV_NAME"_1",
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.at24c02c = {
			.DevName = SC_DEV_NAME"_1""_"AT24C02C_DEV_NAME,
			.InitContErrThr = 3,
			.UpdateContErrThr = 3,
			.I2C = BSP_SC_I2C,
			.Addr = 0xAE,
		},
	},
	{
		.DevName = SC_DEV_NAME"_2",
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.at24c02c = {
			.DevName = SC_DEV_NAME"_2""_"AT24C02C_DEV_NAME,
			.InitContErrThr = 3,
			.UpdateContErrThr = 3,
			.I2C = BSP_SC_I2C,
			.Addr = 0xAA,
		},
	},
};

int32_t BSP_SC_CheckConditions(uint32_t i)
{
	if(!SEEPROM_GetSCPresent(&seeprom, i))
		return -1;
	return 0;
}

void BSP_SC_Process(void)
{
	uint32_t i;
	
	for (i = 0; i < BSP_SC_CNT; i++)
	{
		if (BSP_SC_CheckConditions(i))
		{
			/* External faults present */
			if(BSP_DEV_GET_STATE(&sc[i]) != BSP_DEV_STATE_RESET)
			{
				SC_Deinit(&sc[i]);
			}
		}
		else
		{
			/* External faults absent */
			if(BSP_DEV_GET_STATE(&sc[i]) != BSP_DEV_STATE_OK)
			{
				SC_Init(&sc[i]);
			}
			else
			{
				SC_Update(&sc[i]);
			}
		}
	}
}
