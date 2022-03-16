#include "ip1562.h"

int32_t IP1562_PCA9536_Process(IP1562_T *Dev)
{
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(&Dev->pca9536) != BSP_DEV_STATE_OK)
	{
		PCA9536_Init(&Dev->pca9536);
	}
	else
	{
		PCA9536_SetBankDir(&Dev->pca9536, 0x03);
		PCA9536_GetBankIn(&Dev->pca9536);
		Dev->PinsState &= ~0x03;
		Dev->PinsState |= Dev->pca9536.Ins & 0x03;
		PCA9536_SetBankOut(&Dev->pca9536, Dev->PinsState & (0x03 << 2));
	}
	
	return 0;
}

int32_t IP1562_PCA9538_Process(IP1562_T *Dev)
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

int32_t IP1562_MAX31790_Process(IP1562_T *Dev)
{
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(&Dev->max31790) != BSP_DEV_STATE_OK)
	{
		MAX31790_Init(&Dev->max31790);
	}
	else
	{
		MAX31790_Update(&Dev->max31790);
	}
	
	return 0;
}

int32_t IP1562_Process(IP1562_T *Dev)
{
	if (!Dev)
		return -1;
	
	IP1562_PCA9536_Process(Dev);
	IP1562_PCA9538_Process(Dev);
	IP1562_MAX31790_Process(Dev);
	
	return 0;
}

void IP1562_Deinit(IP1562_T *Dev)
{
	if (!Dev)
		return;
	PCA9538_Deinit(&Dev->pca9538);
	MAX31790_Deinit(&Dev->max31790);
}

int32_t IP1562_ConfigFansSpeed(IP1562_T *Dev, float Speed)
{
	if (!Dev)
		return -1;
	
	if (MAX31790_ConfigPWMOUT(&Dev->max31790, 0, MAX31790_PWMOUT_MODE_PWM, (Speed * 511) / 100, 0))
		return -1;
	if (MAX31790_ConfigPWMOUT(&Dev->max31790, 1, MAX31790_PWMOUT_MODE_TACH, 0, 0))
		return -1;
	if (MAX31790_ConfigPWMOUT(&Dev->max31790, 2, MAX31790_PWMOUT_MODE_PWM, (Speed * 511) / 100, 0))
		return -1;
	if (MAX31790_ConfigPWMOUT(&Dev->max31790, 3, MAX31790_PWMOUT_MODE_PWM, (Speed * 511) / 100, 0))
		return -1;
	if (MAX31790_ConfigPWMOUT(&Dev->max31790, 4, MAX31790_PWMOUT_MODE_TACH, 0, 0))
		return -1;
	if (MAX31790_ConfigPWMOUT(&Dev->max31790, 5, MAX31790_PWMOUT_MODE_PWM, (Speed * 511) / 100, 0))
		return -1;
	
	return 0;
}

float IP1562_GetFanSpeed(IP1562_T *Dev, uint32_t Idx)
{
	float Speed;
	
	if ((!Dev) || (Idx >= 8))
		return 0;
	
	if (Idx == 6)
		Idx = 7;
	else if (Idx == 7)
		Idx = 10;

	Speed = 0;
	if (BSP_DEV_GET_STATE(&Dev->max31790) == BSP_DEV_STATE_OK)
	{
		BSP_DEV_LOCK(&Dev->max31790);
		if (Dev->max31790.TachCount[Idx] < 0xFFE0)
		{
			Speed = 8192.0f * 60.0f * 4.0f * 100.0f / (2.0f * (Dev->max31790.TachCount[Idx] >> 5) * 25000.0f);
		}
		BSP_DEV_UNLOCK(&Dev->max31790);
	}
	return Speed;
}

uint32_t IP1562_GetPSPresent(IP1562_T *Dev, uint32_t Idx)
{
	uint32_t Present;
	
	if ((!Dev) || (Idx >= 2))
		return 0;
	
	Present = 0;
	if (BSP_DEV_GET_STATE(&Dev->pca9538) == BSP_DEV_STATE_OK)
	{
		BSP_DEV_LOCK(&Dev->pca9538);
		if ((Dev->pca9538.Ins & IP1562_PCA9538_PS_PRESENT_MASK(Idx)) == 0)
			Present = 1;
		BSP_DEV_UNLOCK(&Dev->pca9538);
	}
	return Present;
}

uint32_t IP1562_GetSCPresent(IP1562_T *Dev, uint32_t Idx)
{
	uint32_t Present;
	
	if ((!Dev) || (Idx >= 2))
		return 0;
	
	Present = 0;
	if (BSP_DEV_GET_STATE(&Dev->pca9538) == BSP_DEV_STATE_OK)
	{
		BSP_DEV_LOCK(&Dev->pca9538);
		if ((Dev->pca9538.Ins & IP1562_PCA9538_SC_PRESENT_MASK(Idx)) == 0)
			Present = 1;
		BSP_DEV_UNLOCK(&Dev->pca9538);
	}
	return Present;
}

int32_t IP1562_ConfigOutState(IP1562_T *Dev, uint32_t Idx, uint32_t State)
{
	if (!Dev)
		return -1;
	
	if (!State)
		Dev->PinsState &= ~(1 << (Idx + 2));
	else
		Dev->PinsState |= (1 << (Idx + 2));
	
	return 0;
}
uint32_t IP1562_GetOutState(IP1562_T *Dev, uint32_t Idx)
{
	return ((Dev->PinsState >> (Idx + 2)) & 0x01);
}
uint32_t IP1562_GetInState(IP1562_T *Dev, uint32_t Idx)
{
	return ((Dev->PinsState >> Idx) & 0x01);
}
