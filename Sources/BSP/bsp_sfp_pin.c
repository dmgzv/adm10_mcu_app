#include "bsp_sfp_pin.h"

#define BSP_SFP_PIN_PCA9536_LED_0_BIT		 0
#define BSP_SFP_PIN_PCA9536_LED_1_BIT		 1
#define BSP_SFP_PIN_PCA9536_MOD_ABS_BIT	 2
#define BSP_SFP_PIN_PCA9536_RATE_SEL_BIT 3

#define BSP_SFP_PIN_PCA9536_LED_0_MASK		(1 << BSP_SFP_PIN_PCA9536_LED_0_BIT)
#define BSP_SFP_PIN_PCA9536_LED_1_MASK		(1 << BSP_SFP_PIN_PCA9536_LED_1_BIT)
#define BSP_SFP_PIN_PCA9536_MOD_ABS_MASK	(1 << BSP_SFP_PIN_PCA9536_MOD_ABS_BIT)
#define BSP_SFP_PIN_PCA9536_RATE_SEL_MASK	(1 << BSP_SFP_PIN_PCA9536_RATE_SEL_BIT)
	
#define BSP_SFP_PIN_PCA9536_POL_DEFAULT		( 0x0000 ) // All pins are not inverted
#define BSP_SFP_PIN_PCA9536_DIR_DEFAULT		( BSP_SFP_PIN_PCA9536_MOD_ABS_MASK ) // Only MOD_ABS pin is input
#define BSP_SFP_PIN_PCA9536_OUT_DEFAULT		( BSP_SFP_PIN_PCA9536_LED_0_MASK | BSP_SFP_PIN_PCA9536_LED_1_MASK | BSP_SFP_PIN_PCA9536_RATE_SEL_MASK )

static const I2C_TypeDef *pcal6524_i2c[BSP_SFP_PIN_PCAL6524_CNT] =
{
	BSP_SFP_PIN_PCAL6524_1_I2C,
	BSP_SFP_PIN_PCAL6524_2_I2C,
	BSP_SFP_PIN_PCAL6524_3_I2C
};
static const uint8_t pcal6524_addr[BSP_SFP_PIN_PCAL6524_CNT] =
{
	BSP_SFP_PIN_PCAL6524_1_I2C_ADDR,
	BSP_SFP_PIN_PCAL6524_2_I2C_ADDR,
	BSP_SFP_PIN_PCAL6524_3_I2C_ADDR
};
static const uint8_t pcal6524_bank_cfg[BSP_SFP_PIN_PCAL6524_CNT][PCAL6524_BANKS_CNT] = 
{
	{0x00, 0x00, 0x00}, /* All pins are outputs */ 
	{0xFF, 0xFF, 0xFF}, /* All pins are inputs */
	{0x00, 0x00, 0x00} /* All pins are outputs */ 
};
static const uint8_t pcal6524_bank_out[BSP_SFP_PIN_PCAL6524_CNT][PCAL6524_BANKS_CNT] = 
{
	{0xFF, 0xFF, 0xFF}, /* All pins are asserted */ 
	{0xFF, 0xFF, 0xFF}, /* All pins are asserted */
	{0xFF, 0xFF, 0xFF}  /* All pins are asserted */
};
static const uint8_t pcal6524_bank_pol[BSP_SFP_PIN_PCAL6524_CNT][PCAL6524_BANKS_CNT] = 
{
	{0x00, 0x00, 0x00}, /* All pins are not inverted */
	{0x00, 0x00, 0x00}, /* All pins are not inverted */ 
	{0x00, 0x00, 0x00}  /* All pins are not inverted */
};

int32_t BSP_SFP_PIN_Init(BSP_SFP_PIN_T *Dev, BSP_SFP_I2C_SWITCH_T *I2CSwitch)
{
	uint32_t i, bank;
	int32_t result = 0;
	
	if (!Dev)
		return -1;
	
	Dev->I2CSwitch = I2CSwitch;
	
	for (i = 0; i < BSP_SFP_PIN_PCAL6524_CNT; i++)
	{
		if (PCAL6524_Init(&Dev->pcal6524[i], (I2C_TypeDef*)pcal6524_i2c[i], (uint8_t)pcal6524_addr[i]))
		{
			result = -1;
			continue;
		}
		for (bank = 0; bank < PCAL6524_BANKS_CNT; bank++)
		{
			if (PCAL6524_SetBankPol(&Dev->pcal6524[i], bank, (uint8_t)pcal6524_bank_pol[i][bank]))
			{
				result = -1;
				continue;
			}
			if (PCAL6524_SetBankCfg(&Dev->pcal6524[i], bank, (uint8_t)pcal6524_bank_cfg[i][bank]))
			{
				result = -1;
				continue;
			}
			if (PCAL6524_SetBankOut(&Dev->pcal6524[i], bank, (uint8_t)pcal6524_bank_out[i][bank]))
			{
				result = -1;
				continue;
			}
			if (PCAL6524_GetBankIn(&Dev->pcal6524[i], bank))
			{
				result = -1;
				continue;
			}
		}
	}
	
	for (i = 0; i < BSP_SFP_PIN_PCA9536_CNT; i++)
	{
		if (BSP_SFP_I2C_SWITCH_SelectChannel(Dev->I2CSwitch, i))
		{
			result = -1;
			continue;
		}
		if (PCA9536_Init(&Dev->pca9536[i], BSP_SFP_PIN_PCA9536_I2C))
		{
			result = -1;
			continue;
		}
		if (PCA9536_SetBankPol(&Dev->pca9536[i],BSP_SFP_PIN_PCA9536_POL_DEFAULT ))
		{
			result = -1;
			continue;
		}
		if (PCA9536_SetBankOut(&Dev->pca9536[i], BSP_SFP_PIN_PCA9536_OUT_DEFAULT))
		{
			result = -1;
			continue;
		}
		if (PCA9536_SetBankDir(&Dev->pca9536[i], BSP_SFP_PIN_PCA9536_DIR_DEFAULT))
		{
			result = -1;
			continue;
		}
	}	
	return result;	
}

int32_t BSP_SFP_PIN_SetTxDisable(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t TxDisable)
{
	uint32_t bank, pin;
	
	if ((!Dev) || (SfpNum > 19))
		return -1;
		
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	
	if (PCAL6524_SetPinOut(&Dev->pcal6524[2], bank, pin, TxDisable))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_GetTxDisable(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *TxDisable)
{
	uint32_t bank, pin;
	
	if ((!Dev) || (SfpNum > 19))
		return -1;
	
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	
	if (PCAL6524_GetBankOut(&Dev->pcal6524[2], bank))
		return -1;
	*TxDisable = (Dev->pcal6524[2].Outs[bank] >> pin) & 0x01;	
	return 0;
}

int32_t BSP_SFP_PIN_SetLinkLed(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t Color)
{
	uint32_t bank, pin, out;
	
	if ((!Dev) || (SfpNum > 19))
		return -1;
		
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	switch (Color)
	{
		case BSP_SFP_PIN_LED_OFF:
			out = 1;
			break;
		case BSP_SFP_PIN_LED_GREEN:
			out = 0;
			break;
		default:
			return -1;
	}
	if (PCAL6524_SetPinOut(&Dev->pcal6524[0], bank, pin, out))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_GetLinkLed(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *Color)
{
	uint32_t bank, pin;
	
	if ((!Dev) || (SfpNum > 19) || (!Color))
		return -1;
	
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	
	if (PCAL6524_GetBankOut(&Dev->pcal6524[0], bank))
		return -1;
	if ((Dev->pcal6524[0].Outs[bank] >> pin) & 0x01)
		*Color = BSP_SFP_PIN_LED_OFF;
	else
		*Color = BSP_SFP_PIN_LED_GREEN;
	return 0;
}

int32_t BSP_SFP_PIN_SetLed(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t Color)
{
	uint8_t Out;
	
	if ((!Dev) || (SfpNum > 19))
		return -1;
	
	Out = Dev->pca9536[SfpNum].Outs & ~(BSP_SFP_PIN_PCA9536_LED_0_MASK | BSP_SFP_PIN_PCA9536_LED_1_MASK);
	switch (Color) {
		case BSP_SFP_PIN_LED_OFF:
			Out |= BSP_SFP_PIN_PCA9536_LED_0_MASK | BSP_SFP_PIN_PCA9536_LED_1_MASK;
			break;
		case BSP_SFP_PIN_LED_GREEN:
			Out |= BSP_SFP_PIN_PCA9536_LED_1_MASK;
			break;
		case BSP_SFP_PIN_LED_RED:
			Out |= BSP_SFP_PIN_PCA9536_LED_0_MASK ;
			break;
		case BSP_SFP_PIN_LED_YELLOW:
			break;
		default:
			return -1;
	}
	if (BSP_SFP_I2C_SWITCH_SelectChannel(Dev->I2CSwitch, SfpNum))
		return -1;	
	if (PCA9536_SetBankOut(&Dev->pca9536[SfpNum], Out))
		return -1;
	
	return 0;
}

int32_t BSP_SFP_PIN_GetLed(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *Color)
{	
	if ((!Dev) || (SfpNum > 19) || (!Color))
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(Dev->I2CSwitch, SfpNum))
		return -1;
	if (PCA9536_GetBankOut(&Dev->pca9536[SfpNum]))
		return -1;
	switch (Dev->pca9536[SfpNum].Outs & (BSP_SFP_PIN_PCA9536_LED_0_MASK | BSP_SFP_PIN_PCA9536_LED_1_MASK)) {
		case (BSP_SFP_PIN_PCA9536_LED_0_MASK | BSP_SFP_PIN_PCA9536_LED_1_MASK):
			*Color = BSP_SFP_PIN_LED_OFF;
			break;
		case BSP_SFP_PIN_PCA9536_LED_1_MASK:
			*Color = BSP_SFP_PIN_LED_GREEN;
			break;
		case BSP_SFP_PIN_PCA9536_LED_0_MASK:
			*Color = BSP_SFP_PIN_LED_RED ;
			break;
		case 0:
			*Color = BSP_SFP_PIN_LED_YELLOW;
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t BSP_SFP_PIN_SetRateSel(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t RateSel)
{	
	if ((!Dev) || (SfpNum > 19))
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(Dev->I2CSwitch, SfpNum))
		return -1;	
	if (PCA9536_SetPinOut(&Dev->pca9536[SfpNum], BSP_SFP_PIN_PCA9536_RATE_SEL_BIT, RateSel))
		return -1;	
	return 0;
}

int32_t BSP_SFP_PIN_GetRateSel(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *RateSel)
{	
	if ((!Dev) || (SfpNum > 19) || (!RateSel))
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(Dev->I2CSwitch, SfpNum))
		return -1;
	if (PCA9536_GetBankOut(&Dev->pca9536[SfpNum]))
		return -1;
	*RateSel = (Dev->pca9536[SfpNum].Outs >> BSP_SFP_PIN_PCA9536_RATE_SEL_BIT) & 0x01;	
	return 0;
}

int32_t BSP_SFP_PIN_GetModAbs(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *ModAbs)
{	
	if ((!Dev) || (SfpNum > 19) || (!ModAbs))
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(Dev->I2CSwitch, SfpNum))
		return -1;
	if (PCA9536_GetBankIn(&Dev->pca9536[SfpNum]))
		return -1;	
	*ModAbs = (Dev->pca9536[SfpNum].Ins >> BSP_SFP_PIN_PCA9536_MOD_ABS_BIT) & 0x01;	
	return 0;
}

int32_t BSP_SFP_PIN_GetTxFault(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *TxFault)
{
	uint32_t bank, pin;
	
	if ((!Dev) || (SfpNum > 19) || (!TxFault))
		return -1;
	
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	
	if (PCAL6524_GetBankIn(&Dev->pcal6524[1], bank))
		return -1;	
	*TxFault = (Dev->pcal6524[1].Ins[bank] >> pin) & 0x01;	
	return 0;
}
