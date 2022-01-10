#include "bsp_ps.h"
#include "bsp_seeprom.h"
#include "string.h"

PS_T ps[BSP_PS_CNT] =
{
	{
		.DevName = PS_DEV_NAME"_1",
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.at24c02c = {
			.DevName = PS_DEV_NAME"_1""_"AT24C02C_DEV_NAME,
			.InitContErrThr = 3,
			.UpdateContErrThr = 3,
			.I2C = BSP_PS_I2C,
			.Addr = 0xA0,
		},
		.ip1554 = {
			.max7306 = {
				.DevName = PS_DEV_NAME"_1""_"MAX7306_DEV_NAME"_D2",
				.InitContErrThr = 3,
				.UpdateContErrThr = 3,
				.I2C = BSP_PS_I2C,
				.Addr = 0x98,
			},
			.dbe0125v2 = {
				.DevName = PS_DEV_NAME"_1""_"DBE0125V2_DEV_NAME"_A5",
				.InitContErrThr = 3,
				.UpdateContErrThr = 3,
				.I2C = BSP_PS_I2C,
				.Addr = 27 << 1,
			},
		},
	},
	{
		.DevName = PS_DEV_NAME"_2",
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.at24c02c = {
			.DevName = PS_DEV_NAME"_2""_"AT24C02C_DEV_NAME,
			.InitContErrThr = 3,
			.UpdateContErrThr = 3,
			.I2C = BSP_PS_I2C,
			.Addr = 0xA2,
		},
		.ip1554 = {
			.max7306 = {
				.DevName = PS_DEV_NAME"_2""_"MAX7306_DEV_NAME"_D2",
				.InitContErrThr = 3,
				.UpdateContErrThr = 3,
				.I2C = BSP_PS_I2C,
				.Addr = 0x9A,
			},
			.dbe0125v2 = {
				.DevName = PS_DEV_NAME"_2""_"DBE0125V2_DEV_NAME"_A5",
				.InitContErrThr = 3,
				.UpdateContErrThr = 3,
				.I2C = BSP_PS_I2C,
				.Addr = 28 << 1,
			},
		},
	},
};

int32_t BSP_PS_CheckConditions(uint32_t i)
{
	if(!SEEPROM_GetPSPresent(&seeprom, i))
		return -1;
	return 0;
}

void BSP_PS_Process(void)
{
	uint32_t i;
	
	for (i = 0; i < BSP_PS_CNT; i++)
	{
		if (BSP_PS_CheckConditions(i))
		{
			/* External faults present */
			if(BSP_DEV_GET_STATE(&ps[i]) != BSP_DEV_STATE_RESET)
			{
				PS_Deinit(&ps[i]);
			}
		}
		else
		{
			/* External faults absent */
			if(BSP_DEV_GET_STATE(&ps[i]) != BSP_DEV_STATE_OK)
			{
				PS_Init(&ps[i]);
			}
			else
			{
				PS_Update(&ps[i]);
			}
		}
	}
}
