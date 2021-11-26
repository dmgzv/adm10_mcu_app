#include "ip1554.h"

int32_t IP1554_Process(IP1554_T *Dev, uint8_t ChassisAddress)
{
	uint32_t i;
	uint8_t Pin;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(&Dev->max7306) != BSP_DEV_STATE_OK)
	{
		MAX7306_Init(&Dev->max7306);
	}
	else
	{
		for (i = 0; i < MAX7306_PINS_CNT; i++)
		{
			if (ChassisAddress & (1 << 0))
				Pin = 0x7F;
			else
				Pin = 0x40;
			MAX7306_WritePin(&Dev->max7306, i, Pin);
		}
	}
	return 0;
}

void IP1554_Deinit(IP1554_T *Dev)
{
	if (!Dev)
		return;
	MAX7306_Deinit(&Dev->max7306);
}
