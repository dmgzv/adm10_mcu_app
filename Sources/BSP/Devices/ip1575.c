#include "ip1575.h"

int32_t IP1575_PCA9538_Process(IP1575_T *Dev, uint8_t ChassisAddress)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(&Dev->pca9538) != BSP_DEV_STATE_OK)
	{
		PCA9538_Init(&Dev->pca9538);
	}
	else
	{
		PCA9538_SetBankDir(&Dev->pca9538, PCA9538_BANK_DIR_OUT);
		PCA9538_SetBankOut(&Dev->pca9538, ChassisAddress);
	}
	return 0;
}

int32_t IP1575_Process(IP1575_T *Dev, uint8_t ChassisAddress)
{
	if (!Dev)
		return -1;
	
	IP1575_PCA9538_Process(Dev, ChassisAddress);
	
	return 0;
}

void IP1575_Deinit(IP1575_T *Dev)
{
	if (!Dev)
		return;
	PCA9538_Deinit(&Dev->pca9538);
}
