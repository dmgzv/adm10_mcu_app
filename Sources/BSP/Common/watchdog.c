#include "watchdog.h"

#ifdef BSP_CONFIG_USE_WATCHDOG

static IWDG_HandleTypeDef IWDG_Handle;

void __WATCHDOG_Init(uint32_t Time)
{
	IWDG_Handle.Instance = IWDG;
	if (Time < 4096)
	{
		IWDG_Handle.Init.Prescaler = IWDG_PRESCALER_32;
		IWDG_Handle.Init.Reload = Time;
	}
	else if (Time < 8192)
	{
		IWDG_Handle.Init.Prescaler = IWDG_PRESCALER_64;
		IWDG_Handle.Init.Reload = Time / 2;
	}
	else if (Time < 16384)
	{
		IWDG_Handle.Init.Prescaler = IWDG_PRESCALER_128;
		IWDG_Handle.Init.Reload = Time / 4;
	}
	else if (Time < 32768)
	{
		IWDG_Handle.Init.Prescaler = IWDG_PRESCALER_256;
		IWDG_Handle.Init.Reload = Time / 8;
	}
	else
	{
		IWDG_Handle.Init.Prescaler = IWDG_PRESCALER_256;
		IWDG_Handle.Init.Reload = 0xFFF;
	}
	HAL_IWDG_Init(&IWDG_Handle);
}
void __WATCHDOG_Refresh(void)
{
	HAL_IWDG_Refresh(&IWDG_Handle);
}

#endif
