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

PCAL6524_T pcal6524[BSP_SFP_PIN_PCAL6524_CNT] =
{
	{
		.DevName = PCAL6524_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCAL6524_1_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCAL6524_1_I2C,
		.Addr = BSP_SFP_PIN_PCAL6524_1_I2C_ADDR,
	},
	{
		.DevName = PCAL6524_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCAL6524_2_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCAL6524_2_I2C,
		.Addr = BSP_SFP_PIN_PCAL6524_2_I2C_ADDR,
	},
	{
		.DevName = PCAL6524_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCAL6524_3_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCAL6524_3_I2C,
		.Addr = BSP_SFP_PIN_PCAL6524_3_I2C_ADDR,
	},
};

PCA9536_T pca9536[BSP_SFP_PIN_PCA9536_CNT] =
{
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_1_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_2_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_3_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_4_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_5_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_6_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_7_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_8_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_9_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_10_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_11_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_12_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_13_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_14_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_15_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_16_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_17_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_18_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_19_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
	{
		.DevName = PCA9536_DEV_NAME,
		.DevId = BSP_SFP_PIN_PCA9536_20_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.I2C = BSP_SFP_PIN_PCA9536_I2C,
	},
};

static int32_t BSP_SFP_PIN_CheckConditions(void)
{
	uint32_t i;
	
	if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good))
		return -1;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	for (i = 0; i < BSP_PCA9548A_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&pca9548a[i]) != BSP_DEV_STATE_OK)
			return -1;
	}
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
		for (i = 0; i < BSP_SFP_PIN_PCA9536_CNT; i++)
		{
			if(BSP_DEV_GET_STATE(&pca9536[i]) != BSP_DEV_STATE_RESET)
			{
				PCA9536_Deinit(&pca9536[i]);
			}
		}
	}
	else
	{
		/* External faults absent */
		for (i = 0; i < BSP_SFP_PIN_PCAL6524_CNT; i++)
		{
			if(BSP_DEV_GET_STATE(&pcal6524[i]) != BSP_DEV_STATE_OK)
			{
				PCAL6524_Init(&pcal6524[i]);
			}
		}
		for (i = 0; i < BSP_SFP_PIN_PCA9536_CNT; i++)
		{
			if(BSP_DEV_GET_STATE(&pca9536[i]) != BSP_DEV_STATE_OK)
			{
				if (!BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SFP_CH(i)))
				{
					PCA9536_Init(&pca9536[i]);
					BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO);
				}
			}
			else
			{
				for (i = 0; i < 20; i++)
				{
					BSP_SFP_PIN_InitAlarmLed(i);
					BSP_SFP_PIN_InitTxEnableLed(i);
				}
			}
		}		
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

int32_t BSP_SFP_PIN_InitTxEnableLed(uint32_t SfpNum)
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

int32_t BSP_SFP_PIN_SetTxEnableLed(uint32_t SfpNum, uint32_t Color)
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

int32_t BSP_SFP_PIN_GetTxEnableLed(uint32_t SfpNum, uint32_t *Color)
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

int32_t BSP_SFP_PIN_InitAlarmLed(uint32_t SfpNum)
{
	if (SfpNum > 19)
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SFP_CH(SfpNum)))
		return -1;	
	if (PCA9536_SetPinDir(&pca9536[SfpNum], BSP_SFP_PIN_PCA9536_LED_0_BIT, 0))
		return -1;
	if (PCA9536_SetPinDir(&pca9536[SfpNum], BSP_SFP_PIN_PCA9536_LED_1_BIT, 0))
		return -1;
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_SetAlarmLed(uint32_t SfpNum, uint32_t Color)
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
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SFP_CH(SfpNum)))
		return -1;	
	if (PCA9536_SetBankOut(&pca9536[SfpNum], Out))
		return -1;
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_GetAlarmLed(uint32_t SfpNum, uint32_t *Color)
{	
	if ((SfpNum > 19) || (!Color))
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SFP_CH(SfpNum)))
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

int32_t BSP_SFP_PIN_InitRateSel(uint32_t SfpNum)
{
	if (SfpNum > 19)
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SFP_CH(SfpNum)))
		return -1;	
	if (PCA9536_SetPinDir(&pca9536[SfpNum], BSP_SFP_PIN_PCA9536_RATE_SEL_BIT, 0))
		return -1;
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_SetRateSel(uint32_t SfpNum, uint32_t RateSel)
{	
	if (SfpNum > 19)
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SFP_CH(SfpNum)))
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
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SFP_CH(SfpNum)))
		return -1;
	if (PCA9536_GetBankOut(&pca9536[SfpNum]))
		return -1;
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO))
		return -1;
	*RateSel = (pca9536[SfpNum].Outs >> BSP_SFP_PIN_PCA9536_RATE_SEL_BIT) & 0x01;	
	return 0;
}


int32_t BSP_SFP_PIN_InitModAbs(uint32_t SfpNum)
{
	if (SfpNum > 19)
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SFP_CH(SfpNum)))
		return -1;	
	if (PCA9536_SetPinDir(&pca9536[SfpNum], BSP_SFP_PIN_PCA9536_MOD_ABS_BIT, 1))
		return -1;
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO))
		return -1;
	return 0;
}

int32_t BSP_SFP_PIN_GetModAbs(uint32_t SfpNum, uint32_t *ModAbs)
{
	if ((SfpNum > 19) || (!ModAbs))
		return -1;
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SFP_CH(SfpNum)))
		return -1;
	
	if (PCA9536_GetBankIn(&pca9536[SfpNum]))
		return -1;
	*ModAbs = (pca9536[SfpNum].Ins >> BSP_SFP_PIN_PCA9536_MOD_ABS_BIT) & 0x01;	

	if (BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO))
		return -1;
	
	return 0;
}
