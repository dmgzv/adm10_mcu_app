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
