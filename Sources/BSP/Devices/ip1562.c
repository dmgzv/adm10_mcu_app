#include "ip1562.h"

int32_t IP1562_Process(IP1562_T *Dev)
{
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(&Dev->pca9538) != BSP_DEV_STATE_OK)
	{
		PCA9538_Init(&Dev->pca9538);
	}
	else
	{
		PCA9538_SetBankDir(&Dev->pca9538, PCA9538_BANK_DIR_IN);
		PCA9538_GetBankIn(&Dev->pca9538);
	}
	
	return 0;
}

void IP1562_Deinit(IP1562_T *Dev)
{
	if (!Dev)
		return;
	PCA9538_Deinit(&Dev->pca9538);
}

uint32_t IP1562_GetPSPresent(IP1562_T *Dev, uint32_t Idx)
{
	uint32_t Present;
	
	Present = 0;
	if (BSP_DEV_GET_STATE(&Dev->pca9538) == BSP_DEV_STATE_OK)
	{
		BSP_DEV_LOCK(&Dev->pca9538);
		if (Dev->pca9538.Ins & IP1562_PCA9538_PS_PRESENT_MASK(Idx))
			Present = 1;
		BSP_DEV_UNLOCK(&Dev->pca9538);
	}
	return Present;
}

uint32_t IP1562_GetSCPresent(IP1562_T *Dev, uint32_t Idx)
{
	uint32_t Present;
	
	Present = 0;
	if (BSP_DEV_GET_STATE(&Dev->pca9538) == BSP_DEV_STATE_OK)
	{
		BSP_DEV_LOCK(&Dev->pca9538);
		if (Dev->pca9538.Ins & IP1562_PCA9538_SC_PRESENT_MASK(Idx))
			Present = 1;
		BSP_DEV_UNLOCK(&Dev->pca9538);
	}
	return Present;
}
