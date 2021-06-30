#include "bsp_pwr_seq.h"
#include "bsp_pwr_pin.h"

static int32_t BSP_PWR_SEQ_InitPin(void *Priv, uint32_t Pin);
static int32_t BSP_PWR_SEQ_SetPinState(void *Priv, uint32_t Pin, uint32_t State);

const BSP_PIN_CTRL_T ucd90120a_pin_ctrl = {
	BSP_PWR_SEQ_InitPin,
	BSP_PWR_SEQ_SetPinState,
	NULL,
	NULL
};

UCD90120A_T ucd90120a = {
	.DevName = UCD90120A_DEV_NAME"_"BSP_PWR_SEQ_UCD90120A_ID,
	.InitContErrThr = 3,
	.UpdateContErrThr = 3,
	.I2C = BSP_PWR_SEQ_UCD90120A_I2C,
	.Addr = BSP_PWR_SEQ_UCD90120A_I2C_ADDR,
	.PinCtrl = &ucd90120a_pin_ctrl,
};

static int32_t BSP_PWR_SEQ_InitPin(void *Priv, uint32_t Pin)
{
	switch (Pin)
	{
		case UCD90120A_PIN_RESET:
			if (BSP_PWR_PIN_Init(BSP_PWR_PIN_UCD_RST))
				return -1;
			break;
		case UCD90120A_PIN_CONTROL:
			if (BSP_PWR_PIN_Init(BSP_PWR_PIN_UCD_CTL))
				return -1;
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t BSP_PWR_SEQ_SetPinState(void *Priv, uint32_t Pin, uint32_t State)
{
	switch (Pin)
	{
		case UCD90120A_PIN_RESET:
			if (BSP_PWR_PIN_SetState(BSP_PWR_PIN_UCD_RST, State))
				return -1;
			break;
		case UCD90120A_PIN_CONTROL:
			if (BSP_PWR_PIN_SetState(BSP_PWR_PIN_UCD_CTL, State))
				return -1;
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t BSP_PWR_SEQ_CheckConditions(void)
{
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	return 0;
}

void BSP_PWR_SEQ_Process(void)
{	
	if (BSP_PWR_SEQ_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_RESET)
		{
			UCD90120A_Deinit(&ucd90120a);
		}
	}
	else
	{
		/* External faults absent */
		if(BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK)
		{
			UCD90120A_Init(&ucd90120a, BSP_UCD90120A_CFG_Table, BSP_UCD90120A_CFG_TableSize);
		}
		else
		{
			UCD90120A_Update(&ucd90120a);
		}		
	}
}
