#include "bsp_pwr_pin.h"

int32_t BSP_PWR_PIN_SetState(BSP_PWR_PIN_T *Dev, uint8_t PinMask, uint8_t State)
{
	uint8_t CurState, NewState;
	
	if (!Dev)
		return -1;
	
	CurState = Dev->pca9536.Outs;
	
	if (State)
		NewState = CurState | PinMask;
	else
		NewState = CurState & (~PinMask);
	
	if (NewState != CurState )
	{
		if (PCA9536_SetBankOut(&Dev->pca9536, NewState))
			return -1;
	}
	
	return 0;
}

int32_t BSP_PWR_PIN_GetState(BSP_PWR_PIN_T *Dev, uint8_t PinMask, uint8_t *State)
{
	uint8_t CurState;
	
	if (!Dev)
		return -1;

	CurState = Dev->pca9536.Outs;
	
	if (CurState & PinMask)
		*State = 1;
	else
		*State = 0;

	return 0;
}

int32_t BSP_PWR_PIN_Init(BSP_PWR_PIN_T *Dev)
{
	
	if (!Dev)
		return -1;
		
	if (PCA9536_Init(&Dev->pca9536, BSP_PWR_PIN_PCA9536_I2C))
			return -1;	
	if (PCA9536_SetBankPol(&Dev->pca9536, BSP_PWR_PIN_PCA9536_POL_DEFAULT ))
		return -1;
	if (PCA9536_SetBankOut(&Dev->pca9536, BSP_PWR_PIN_PCA9536_OUT_DEFAULT))
		return -1;
	if (PCA9536_SetBankDir(&Dev->pca9536, BSP_PWR_PIN_PCA9536_DIR_DEFAULT))
		return -1;
		
	return 0;
}
