#include "bsp_sfp_pin.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_sfp_i2c_switch.h"

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

PCAL6524_T pcal6524[BSP_SFP_PIN_PCAL6524_CNT];
//PCA9536_T pca9536[BSP_SFP_PIN_PCA9536_CNT];	

static int32_t BSP_SFP_PIN_CheckConditions(void)
{
	//uint32_t i;
	
	if (BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
		return -1;
	if (BSP_DEV_GET_STATE(&pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	/*
	for (i = 0; i < BSP_PCA9548_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&pca9548a[i]) != BSP_DEV_STATE_OK)
			return -1;
	}
	for (i = 0; i < BSP_PCA9546_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&pca9546a[i]) != BSP_DEV_STATE_OK)
			return -1;
	}
	*/
	return 0;
}

void BSP_SFP_PIN_Process(void)
{
	uint32_t i;
	
	
	if (BSP_SFP_PIN_CheckConditions())
	{
		/* External faults present */
		for (i = 0; i < BSP_SFP_PIN_PCAL6524_CNT; i++)
		{			
			if(BSP_DEV_GET_STATE(&pcal6524[i]) != BSP_DEV_STATE_RESET)
			{
				PCAL6524_Deinit(&pcal6524[i]);
			}
		}
		/*
		for (i = 0; i < BSP_SFP_PIN_PCA9536_CNT; i++)
		{
			if(BSP_DEV_GET_STATE(&pca9536[i]) != BSP_DEV_STATE_RESET)
			{
				PCA9536_Deinit(&pca9536[i]);
			}
		}
		*/
	}
	else
	{
		/* External faults absent */
		for (i = 0; i < BSP_SFP_PIN_PCAL6524_CNT; i++)
		{
			if(BSP_DEV_GET_STATE(&pcal6524[i]) != BSP_DEV_STATE_OK)
			{
				PCAL6524_Init(&pcal6524[i], (I2C_TypeDef*)pcal6524_i2c[i], (uint8_t)pcal6524_addr[i]);
			}
		}
		/*
		for (i = 0; i < BSP_SFP_PIN_PCAL6524_CNT; i++)
		{
			if(BSP_DEV_GET_STATE(&pca9536[i]) != BSP_DEV_STATE_OK)
			{
				if (!BSP_SFP_I2C_SWITCH_SelectChannel(SfpNum))
				{
					PCA9536_Init(&pca9536[i], BSP_SFP_PIN_PCA9536_I2C);
					BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO);
				}
			}
		}
		*/		
	}
}

int32_t BSP_SFP_PIN_InitTxFault(uint32_t SfpNum)
{
	uint32_t bank, pin;
	
	if (SfpNum > 19)
		return -1;
		
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	
	if (PCAL6524_SetPinPol(&pcal6524[1], bank, pin, 0))
		return -1;
	if (PCAL6524_SetPinCfg(&pcal6524[1], bank, pin, 1))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_GetTxFault(uint32_t SfpNum, uint32_t *TxFault)
{
	uint32_t bank, pin;
	
	if ((SfpNum > 19) || (!TxFault))
		return -1;
	
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	
	if (PCAL6524_GetBankIn(&pcal6524[1], bank))
		return -1;	
	*TxFault = (pcal6524[1].Ins[bank] >> pin) & 0x01;	
	return 0;
}

int32_t BSP_SFP_PIN_InitTxDisable(uint32_t SfpNum)
{
	uint32_t bank, pin;
	
	if (SfpNum > 19)
		return -1;
		
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	
	if (PCAL6524_SetPinPol(&pcal6524[2], bank, pin, 0))
		return -1;
	if (PCAL6524_SetPinCfg(&pcal6524[2], bank, pin, 0))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_SetTxDisable(uint32_t SfpNum, uint32_t TxDisable)
{
	uint32_t bank, pin;
	
	if (SfpNum > 19)
		return -1;
		
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	
	if (PCAL6524_SetPinOut(&pcal6524[2], bank, pin, TxDisable))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_GetTxDisable(uint32_t SfpNum, uint32_t *TxDisable)
{
	uint32_t bank, pin;
	
	if (SfpNum > 19)
		return -1;
	
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	
	if (PCAL6524_GetBankOut(&pcal6524[2], bank))
		return -1;
	*TxDisable = (pcal6524[2].Outs[bank] >> pin) & 0x01;	
	return 0;
}

int32_t BSP_SFP_PIN_InitLinkLed(uint32_t SfpNum)
{
	uint32_t bank, pin;
	
	if (SfpNum > 19)
		return -1;
		
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	
	if (PCAL6524_SetPinPol(&pcal6524[0], bank, pin, 0))
		return -1;
	if (PCAL6524_SetPinCfg(&pcal6524[0], bank, pin, 0))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_SetLinkLed(uint32_t SfpNum, uint32_t Color)
{
	uint32_t bank, pin, out;
	
	if (SfpNum > 19)
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
	if (PCAL6524_SetPinOut(&pcal6524[0], bank, pin, out))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_GetLinkLed(uint32_t SfpNum, uint32_t *Color)
{
	uint32_t bank, pin;
	
	if ((SfpNum > 19) || (!Color))
		return -1;
	
	pin = SfpNum % 8;
	bank = SfpNum / 8;
	
	if (PCAL6524_GetBankOut(&pcal6524[0], bank))
		return -1;
	if ((pcal6524[0].Outs[bank] >> pin) & 0x01)
		*Color = BSP_SFP_PIN_LED_OFF;
	else
		*Color = BSP_SFP_PIN_LED_GREEN;
	return 0;
}
/*
int32_t BSP_SFP_PIN_SetLed(uint32_t SfpNum, uint32_t Color)
{
	uint8_t Out;
	
	if (SfpNum > 19)
		return -1;
	
	Out = pca9536[SfpNum].Outs & ~(BSP_SFP_PIN_PCA9536_LED_0_MASK | BSP_SFP_PIN_PCA9536_LED_1_MASK);
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
	if (BSP_SFP_I2C_SWITCH_SelectChannel(SfpNum))
		return -1;	
	if (PCA9536_SetBankOut(&pca9536[SfpNum], Out))
		return -1;
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_GetLed(uint32_t SfpNum, uint32_t *Color)
{	
	if ((SfpNum > 19) || (!Color))
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(SfpNum))
		return -1;
	if (PCA9536_GetBankOut(&pca9536[SfpNum]))
		return -1;
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO))
		return -1;
	switch (pca9536[SfpNum].Outs & (BSP_SFP_PIN_PCA9536_LED_0_MASK | BSP_SFP_PIN_PCA9536_LED_1_MASK)) {
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

int32_t BSP_SFP_PIN_SetRateSel(uint32_t SfpNum, uint32_t RateSel)
{	
	if (SfpNum > 19)
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(SfpNum))
		return -1;	
	if (PCA9536_SetPinOut(&pca9536[SfpNum], BSP_SFP_PIN_PCA9536_RATE_SEL_BIT, RateSel))
		return -1;
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_GetRateSel(uint32_t SfpNum, uint32_t *RateSel)
{	
	if ((SfpNum > 19) || (!RateSel))
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(SfpNum))
		return -1;
	if (PCA9536_GetBankOut(&pca9536[SfpNum]))
		return -1;
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO))
		return -1;
	*RateSel = (pca9536[SfpNum].Outs >> BSP_SFP_PIN_PCA9536_RATE_SEL_BIT) & 0x01;	
	return 0;
}
*/

int32_t BSP_SFP_PIN_InitModAbs(uint32_t SfpNum)
{
	return 0;
}

int32_t BSP_SFP_PIN_GetModAbs(uint32_t SfpNum, uint32_t *ModAbs)
{
	uint8_t Addr;
	char Data;
	
	if ((SfpNum > 19) || (!ModAbs))
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(SfpNum))
		return -1;
	/*
	if (PCA9536_GetBankIn(&pca9536[SfpNum]))
		return -1;
	*ModAbs = (pca9536[SfpNum].Ins >> BSP_SFP_PIN_PCA9536_MOD_ABS_BIT) & 0x01;	
	*/
	Addr = 0;
	Data = 0;
	*ModAbs = 0;
	if (I2C_Sequential_Receive(BSP_SFP_PIN_PCA9536_I2C, 0xA0, &Addr, 1, &Data, 1))
		*ModAbs = 1;
	if (Data != 0x03)
		*ModAbs = 1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO))
		return -1;
	
	return 0;
}
