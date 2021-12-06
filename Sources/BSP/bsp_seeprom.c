#include "bsp_seeprom.h"

SEEPROM_T seeprom =
{
	.DevName = SEEPROM_DEV_NAME,
	.InitContErrThr = 3,
	.UpdateContErrThr = 3,
	.at24c02c = {
		.DevName = SEEPROM_DEV_NAME"_"AT24C02C_DEV_NAME,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SEEPROM_I2C,
		.Addr = 0xA4,
	},
	.ip1562 = {
		.pca9538 = {
			.DevName = SEEPROM_DEV_NAME"_"PCA9538_DEV_NAME"_D1",
			.InitContErrThr = 3,
			.UpdateContErrThr = 3,
			.I2C = BSP_SEEPROM_I2C,
			.Addr = 0xE0,
		},
		.max31790 = {
			.DevName = SEEPROM_DEV_NAME"_"MAX31790_DEV_NAME"_D4",
			.InitContErrThr = 3,
			.UpdateContErrThr = 3,
			.I2C = BSP_SEEPROM_I2C,
			.Addr = 0x40,
		},
	},
};

int32_t BSP_SEEPROM_CheckConditions(void)
{
	return 0;
}

void BSP_SEEPROM_Process(void)
{	
	if (BSP_SEEPROM_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&seeprom) != BSP_DEV_STATE_RESET)
		{
			SEEPROM_Deinit(&seeprom);
		}
	}
	else
	{
		/* External faults absent */
		if(BSP_DEV_GET_STATE(&seeprom) != BSP_DEV_STATE_OK)
		{
			SEEPROM_Init(&seeprom);
		}
		else
		{
			SEEPROM_Update(&seeprom);
		}
	}
}
