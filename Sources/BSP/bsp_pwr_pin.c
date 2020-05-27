#include "bsp_pwr_pin.h"

PCA9536_T pwr_pca9536 =
{
	.DevName = PCA9536_DEV_NAME,
	.DevId = BSP_PWR_PIN_PCA9536_ID,
	.InitContErrThr = 3,
	.UpdateContErrThr = 3,
	.I2C = BSP_PWR_PIN_PCA9536_I2C,
};

int32_t BSP_PWR_PIN_SetState(uint32_t Pin, uint32_t State)
{
	return PCA9536_SetPinOut(&pwr_pca9536, Pin, State);
}

int32_t BSP_PWR_PIN_GetState(uint32_t Pin, uint32_t *State)
{
	if(BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	*(State) = pwr_pca9536.Outs & (1 << Pin) ? 1 : 0;
	return 0;
}

int32_t BSP_PWR_PIN_Init(uint32_t Pin)
{		
	if (PCA9536_SetPinPol(&pwr_pca9536, Pin, 0))
		return -1;
	if (PCA9536_SetPinDir(&pwr_pca9536, Pin, 0))
		return -1;		
	return 0;
}

int32_t BSP_PWR_PIN_CheckConditions(void)
{
	return 0;
}

void BSP_PWR_PIN_Process(void)
{	
	if (BSP_PWR_PIN_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_RESET)
		{
			PCA9536_Deinit(&pwr_pca9536);
		}
	}
	else
	{
		/* External faults absent */
		if(BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		{
			PCA9536_Init(&pwr_pca9536);
		}
	}
}
