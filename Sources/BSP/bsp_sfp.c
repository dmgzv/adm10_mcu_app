#include "bsp_sfp.h"
#include <string.h>
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_sfp_pin.h"
#include "bsp_sfp_i2c_switch.h"
//#include "bsp_fpga.h"

static int32_t __BSP_SFP_ConfigSwitch(void *Priv);
static int32_t __BSP_SFP_ResetSwitch(void *Priv);

static int32_t __BSP_SFP_InitPin(void *Priv, uint32_t Pin);
static int32_t __BSP_SFP_SetPinState(void *Priv, uint32_t Pin, uint32_t State);
static int32_t __BSP_SFP_GetPinState(void *Priv, uint32_t Pin, uint32_t *State);


const BSP_PIN_CTRL_T bsp_sfp_pin_ctrl[BSP_SFP_CNT] = {
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)0	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)1	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)2	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)3	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)4	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)5	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)6	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)7	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)8	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)9	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)10	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)11	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)12	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)13	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)14	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)15	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)16	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)17	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)18	},
	{	__BSP_SFP_InitPin,	__BSP_SFP_SetPinState,	__BSP_SFP_GetPinState, (void*)19	},
};

const BSP_I2C_CTRL_T bsp_sfp_i2c_ctrl[BSP_SFP_CNT] = {
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)0	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)1	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)2	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)3	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)4	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)5	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)6	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)7	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)8	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)9	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)10	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)11	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)12	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)13	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)14	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)15	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)16	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)17	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)18	},
	{	BSP_SFP_I2C,	__BSP_SFP_ConfigSwitch,	__BSP_SFP_ResetSwitch,	(void*)19	},
};

SFP_T sfp[BSP_SFP_CNT] =
{
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_1_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[0],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[0],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_2_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[1],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[1],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_3_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[2],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[2],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_4_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[3],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[3],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_5_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[4],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[4],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_6_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[5],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[5],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_7_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[6],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[6],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_8_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[7],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[7],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_9_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[8],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[8],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_10_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[9],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[9],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_11_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[10],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[10],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_12_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[11],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[11],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_13_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[12],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[12],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_14_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[13],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[13],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_15_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[14],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[14],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_16_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[15],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[15],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_17_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[16],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[16],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_18_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[17],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[17],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_19_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[18],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[18],
	},
	{
		.DevName = SFP_DEV_NAME"_"BSP_SFP_20_ID,
		.InitContErrThr = 3,
		.UpdateContErrThr = 3,
		.PinCtrl = &bsp_sfp_pin_ctrl[19],
		.I2CCtrl = &bsp_sfp_i2c_ctrl[19],
	},
};

static int32_t __BSP_SFP_ConfigSwitch(void *Priv)
{
	uint32_t Idx = (uint32_t)Priv;
	
	return BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_SFP_CH(Idx));
}

static int32_t __BSP_SFP_ResetSwitch(void *Priv)
{	
	return BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_CH_NO);
}

static int32_t __BSP_SFP_InitPin(void *Priv, uint32_t Pin)
{
	uint32_t Idx = (uint32_t)Priv;
	
	switch (Pin)
	{
		case SFP_PIN_MOD_ABS:
			if (BSP_SFP_PIN_InitModAbs(Idx))
				return -1;
			break;
		case SFP_PIN_TX_DISABLE:
			if (BSP_SFP_PIN_InitTxDisable(Idx))
				return -1;
			break;
		case SFP_PIN_RATE_SEL:
			if (BSP_SFP_PIN_InitRateSel(Idx))
				return -1;
			break;
		case SFP_PIN_TX_FAULT:
			if (BSP_SFP_PIN_InitTxFault(Idx))
				return -1;
			break;
		case SFP_PIN_RX_LOS:
			break;
	}
	return 0;	
}

static int32_t __BSP_SFP_SetPinState(void *Priv, uint32_t Pin, uint32_t State)
{
	uint32_t Idx = (uint32_t)Priv;
	
	switch (Pin)
	{
		case SFP_PIN_MOD_ABS:
			break;
		case SFP_PIN_TX_DISABLE:
			if (BSP_SFP_PIN_SetTxDisable(Idx, State))
				return -1;
			break;
		case SFP_PIN_RATE_SEL:
			if (BSP_SFP_PIN_SetRateSel(Idx, State))
				return -1;
			break;
		case SFP_PIN_TX_FAULT:
			break;
		case SFP_PIN_RX_LOS:
			break;
	}
	return 0;	
}

static int32_t __BSP_SFP_GetPinState(void *Priv, uint32_t Pin, uint32_t *State)
{
	uint32_t Idx = (uint32_t)Priv;
	
	switch (Pin)
	{
		case SFP_PIN_MOD_ABS:
			if (BSP_SFP_PIN_GetModAbs(Idx, State))
				return -1;
			break;
		case SFP_PIN_TX_DISABLE:
			if (BSP_SFP_PIN_GetTxDisable(Idx, State))
				return -1;
			break;
		case SFP_PIN_RATE_SEL:
			if (BSP_SFP_PIN_GetRateSel(Idx, State))
				return -1;
			break;
		case SFP_PIN_TX_FAULT:
			if (BSP_SFP_PIN_GetTxFault(Idx, State))
				return -1;
			break;
		case SFP_PIN_RX_LOS:
			//if (BSP_FPGA_GetSFPLOS(Idx, State))
			//	return -1;
			break;
	}
	return 0;	
}

static int32_t BSP_SFP_CheckConditions(uint32_t Idx)
{
	uint32_t i;
	
	//if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good))
	//	return -1;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	//if (BSP_DEV_GET_STATE(&fpga) != BSP_DEV_STATE_OK)
	//	return -1;
	for (i = 0; i < BSP_PCA9548A_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&pca9548a[i]) != BSP_DEV_STATE_OK)
			return -1;
	}
	for (i = 0; i < BSP_SFP_PIN_PCAL6524_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&pcal6524[i]) != BSP_DEV_STATE_OK)
			return -1;
	}
	
	return 0;
}

void BSP_SFP_Process(uint32_t Idx)
{	
	if (BSP_SFP_CheckConditions(Idx))
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&sfp[Idx]) != BSP_DEV_STATE_RESET)
		{
			SFP_Deinit(&sfp[Idx]);
		}
	}
	else
	{
		/* External faults absent */
		if(BSP_DEV_GET_STATE(&sfp[Idx]) != BSP_DEV_STATE_OK)
		{
			SFP_Init(&sfp[Idx]);
		}
		else
		{
			SFP_Update(&sfp[Idx]);
		}
	}
}
