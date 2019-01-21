#include "bsp_sfp.h"
#include <string.h>

const I2C_TypeDef *sfp_i2c[BSP_SFP_CNT] = {
	BSP_SFP_1_I2C,
	BSP_SFP_2_I2C,
	BSP_SFP_3_I2C,
	BSP_SFP_4_I2C,
	BSP_SFP_5_I2C,
	BSP_SFP_6_I2C,
	BSP_SFP_7_I2C,
	BSP_SFP_8_I2C,
	BSP_SFP_9_I2C,
	BSP_SFP_10_I2C,
	BSP_SFP_11_I2C,
	BSP_SFP_12_I2C,
	BSP_SFP_13_I2C,
	BSP_SFP_14_I2C,
	BSP_SFP_15_I2C,
	BSP_SFP_16_I2C,
	BSP_SFP_17_I2C,
	BSP_SFP_18_I2C,
	BSP_SFP_19_I2C,
	BSP_SFP_20_I2C
};

uint32_t BSP_SFP_GetModAbs(BSP_SFP_T *Dev)
{
	if (!Dev)
		return 0;
	if (BSP_SFP_PIN_GetModAbs(Dev->pin, Dev->Num, &Dev->ModAbs))
		return 0;
	return Dev->ModAbs;
}

uint32_t BSP_SFP_GetRxLos(BSP_SFP_T *Dev)
{
	if (!Dev)
		return 0;
	//if (BSP_FPGA_GetSFPLOS(Dev->fpga, Dev->Num, &Dev->LOS))
	//	return 0;
	return Dev->LOS;
}

uint32_t BSP_SFP_GetTxFault(BSP_SFP_T *Dev)
{
	if (!Dev)
		return 0;
	if (BSP_SFP_PIN_GetTxFault(Dev->pin, Dev->Num, &Dev->TxFault))
		return 0;
	return Dev->TxFault;
}

uint32_t BSP_SFP_GetTxDisable(BSP_SFP_T *Dev)
{
	if (!Dev)
		return 0;
	if (BSP_SFP_PIN_GetTxDisable(Dev->pin, Dev->Num, &Dev->TxDisable))
		return 0;
	return Dev->TxDisable;
}

int32_t BSP_SFP_SetTxDisable(BSP_SFP_T *Dev, uint32_t TxDisable)
{
	if (!Dev)
		return -1;
	if (BSP_SFP_PIN_SetTxDisable(Dev->pin, Dev->Num, TxDisable))
		return -1;
	Dev->TxDisable = TxDisable;
	return 0;
}

int32_t BSP_SFP_InitDevice(BSP_SFP_T *Dev)
{
	if (BSP_SFP_I2C_SWITCH_SelectChannel(Dev->i2c_switch, Dev->Num))
		return -1;
	if (SFP_Init(&Dev->sfp, (I2C_TypeDef*)sfp_i2c[Dev->Num]))
		return -1;
	return 0;
}

int32_t BSP_SFP_UpdateDevice(BSP_SFP_T *Dev)
{
	if (BSP_SFP_I2C_SWITCH_SelectChannel(Dev->i2c_switch, Dev->Num))
		return -1;
	if (SFP_Update(&Dev->sfp))
		return -1;
	return 0;
} 

uint32_t SFPUpdateStepCounter;

void BSP_SFP_Update(BSP_SFP_T *Dev, BSP_SFP_CFG_T *Cfg)
{
	if (!Dev)
		return;
	
	if (BSP_SFP_PIN_GetModAbs(Dev->pin, Dev->Num, &Dev->ModAbs))
			return;	
	if (BSP_SFP_PIN_GetTxFault(Dev->pin, Dev->Num, &Dev->TxFault))
			return;
	//if (BSP_FPGA_GetSFPLOS(Dev->fpga, Dev->Num, &Dev->LOS))
	//	return;
	if ((Dev->TxDisable) && (!Cfg->TxDisable))
	{
		if (BSP_SFP_PIN_SetTxDisable(Dev->pin, Dev->Num, 0))
			return;
		Dev->TxDisable = 0;
	}
	else if ((!Dev->TxDisable) && (Cfg->TxDisable))
	{
		if (BSP_SFP_PIN_SetTxDisable(Dev->pin, Dev->Num, 1))
			return;
		Dev->TxDisable = 1;
	}
	
	if (Dev->ModAbs)
	{
		if (BSP_SFP_PIN_SetLed(Dev->pin, Dev->Num, BSP_SFP_PIN_LED_RED))
			return;
		if (BSP_DEV_GET_STATE(&Dev->sfp) != BSP_DEV_STATE_RESET)
		{
			SFP_Deinit(&Dev->sfp);
		}
	}
	else
	{
		if (BSP_SFP_PIN_SetLed(Dev->pin, Dev->Num, BSP_SFP_PIN_LED_GREEN))
			return;
		if (BSP_DEV_GET_STATE(&Dev->sfp) != BSP_DEV_STATE_OK)
		{
			if (BSP_SFP_I2C_SWITCH_SelectChannel(Dev->i2c_switch, Dev->Num))
				return;
			SFP_Init(&Dev->sfp, (I2C_TypeDef*)sfp_i2c[Dev->Num]);
		}
		else
		{
			if (BSP_SFP_I2C_SWITCH_SelectChannel(Dev->i2c_switch, Dev->Num))
				return;
			if (SFP_Update(&Dev->sfp))
				return;
		}
	}
}

//int32_t BSP_SFP_Init(BSP_SFP_T *Dev, uint32_t Num, BSP_SFP_PIN_T *pin, BSP_SFP_I2C_SWITCH_T *i2c_switch, BSP_FPGA_T *fpga)
int32_t BSP_SFP_Init(BSP_SFP_T *Dev, uint32_t Num, BSP_SFP_PIN_T *pin, BSP_SFP_I2C_SWITCH_T *i2c_switch)
{
	//if ((!Dev) || (Num > 19) || (!pin) || (!i2c_switch) || (!fpga))
	if ((!Dev) || (Num > 19) || (!pin) || (!i2c_switch))
		return -1;
	
	memset(Dev, 0, sizeof(*Dev));
	
	Dev->Num = Num;
	Dev->pin = pin;
	Dev->i2c_switch = i2c_switch;
	//Dev->fpga = fpga;
	
	if (BSP_SFP_PIN_GetTxFault(Dev->pin, Dev->Num, &Dev->TxFault))
		return -1;
	if (BSP_SFP_PIN_GetModAbs(Dev->pin, Dev->Num, &Dev->ModAbs))
		return -1;
	if (BSP_SFP_PIN_SetTxDisable(Dev->pin, Dev->Num, 1))
		return -1;
	Dev->TxDisable = 1;
	Dev->ModAbs = 1;
	return 0;
}
