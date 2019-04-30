#include "profile_adm.h"

static void PROFILE_ADM_UpdatepId(PROFILE_T *Profile)
{
	PROFILE_SetState(Profile, PROFILE_TABLE_1, PROFILE_ADM_PID_IDX, PROFILE_STATE_IS);	
}

static void PROFILE_ADM_UpdateSrNumber(PROFILE_T *Profile)
{
	PROFILE_SetState(Profile, PROFILE_TABLE_1, PROFILE_ADM_SRNUMBER_IDX, PROFILE_STATE_IS);	
}

static void PROFILE_ADM_UpdateSwNumber(PROFILE_T *Profile)
{
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM_SWNUMBER_IDX, BSP_SW_VERSION, PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
}

static void PROFILE_ADM_UpdateMCUSwNumber(PROFILE_T *Profile)
{
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM_MCUSWNUMBER_IDX, BSP_MCU_SW_VERSION, PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
}

static void PROFILE_ADM_UpdateMaxVFPGASwNumber(PROFILE_T *Profile)
{
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
	{
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM_MAXVFPGASWNUMBER_IDX, "", PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
	}
	else
	{
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM_MAXVFPGASWNUMBER_IDX, fpga_maxv.SwVersion, PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
	}
}

static void PROFILE_ADM_UpdateLineFPGASwNumber(PROFILE_T *Profile)
{
	if (BSP_DEV_GET_STATE(&fpga_line) != BSP_DEV_STATE_OK)
	{
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM_LINEFPGASWNUMBER_IDX, "", PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
	}
	else
	{
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM_LINEFPGASWNUMBER_IDX, fpga_line.SwVersion, PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
	}
}

static void PROFILE_ADM_UpdateClient1FPGASwNumber(PROFILE_T *Profile)
{
	if (BSP_DEV_GET_STATE(&fpga_client[0]) != BSP_DEV_STATE_OK)
	{
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM_CLIENT1FPGASWNUMBER_IDX, "", PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
	}
	else
	{
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM_CLIENT1FPGASWNUMBER_IDX, fpga_client[0].SwVersion, PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
	}
}

static void PROFILE_ADM_UpdateClient2FPGASwNumber(PROFILE_T *Profile)
{
	if (BSP_DEV_GET_STATE(&fpga_client[1]) != BSP_DEV_STATE_OK)
	{
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM_CLIENT2FPGASWNUMBER_IDX, "", PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
	}
	else
	{
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM_CLIENT2FPGASWNUMBER_IDX, fpga_client[1].SwVersion, PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
	}
}

static void PROFILE_ADM_UpdateHwNumber(PROFILE_T *Profile)
{
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM_HWNUMBER_IDX, BSP_HW_VERSION, PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
}


static void PROFILE_ADM_UpdateTCaseValue(PROFILE_T *Profile)
{
	float TCase;

	TCase = 36.7f;	
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM_CASETEMP_IDX, &TCase, PROFILE_COLOR_TYPE_AUTO, PROFILE_STATE_IS);
}

static void PROFILE_ADM_UpdateTimeValue(PROFILE_T *Profile)
{
	uint32_t Time;
	
	Time = osKernelSysTick() / osKernelSysTickFrequency;	
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM_TIME_IDX, &Time, PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
}

static void PROFILE_ADM_UpdatePowerAlarmsValue(PROFILE_T *Profile)
{
	uint32_t Value, i;
	uint8_t ColorType;
	
	Value = 0;
	ColorType = PROFILE_COLOR_TYPE_NORMAL;
	
	if (BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
	{
		Value |= (1 << 0) | (1 << 1);
		ColorType = PROFILE_COLOR_TYPE_FAILURE;
	}
	else
	{
		if (!(dim3r3400_power_good & (1 << 0)))
			Value |= (1 << 0);
		if (!(dim3r3400_power_good & (1 << 1)))
			Value |= (1 << 1);			
	}
	
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
	{
		for (i = 0; i < UCD90120A_RAILS_CNT; i++)
		{
			Value |= (1 << (3 + i));
			ColorType = PROFILE_COLOR_TYPE_FAILURE;
		}
	}
	else
	{
		for (i = 0; i < UCD90120A_RAILS_CNT; i++)
		{
			if (!UCD90120A_RAIL_POWER_GOOD(&ucd90120a, i))
			{
				Value |= (1 << (3 + i));
				ColorType = PROFILE_COLOR_TYPE_FAILURE;
			}
		}
	}
	
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM_POWERALARMS_IDX, &Value, ColorType, PROFILE_STATE_IS);
}

static void PROFILE_ADM_UpdateHwAlarmsValue(PROFILE_T *Profile)
{
	uint32_t Value;
	uint8_t ColorType;
	
	Value = 0;
	ColorType = PROFILE_COLOR_TYPE_NORMAL;
	
	if (BSP_DEV_GET_STATE(&s29gl01gs) != BSP_DEV_STATE_OK)
		Value |= 1 << 0;
	if (BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
		Value |= 1 << 1;
	if (BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK)
		Value |= 1 << 2;
	
	if (Value)
		ColorType = PROFILE_COLOR_TYPE_FAILURE;
	
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM_HWALARMS_IDX, &Value, ColorType, PROFILE_STATE_IS);
}

static void PROFILE_ADM_UpdateBlockStateValue(PROFILE_T *Profile)
{
	uint32_t Value;
	uint8_t ColorType, HwAlarmsColor, PowerAlarmsColor;
	
	Value = 1;
	ColorType = PROFILE_COLOR_TYPE_NORMAL;
	
	PROFILE_GetColor(Profile, PROFILE_TABLE_2, PROFILE_ADM_POWERALARMS_IDX, &HwAlarmsColor);
	PROFILE_GetColor(Profile, PROFILE_TABLE_2, PROFILE_ADM_HWALARMS_IDX, &PowerAlarmsColor);
	
	if (HwAlarmsColor != PROFILE_COLOR_NORMAL)
		Value = 3;
	if (PowerAlarmsColor != PROFILE_COLOR_NORMAL)
		Value = 3;
	
	if (Value == 3)
		ColorType = PROFILE_COLOR_TYPE_FAILURE;

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM_BLOCKSTATE_IDX, &Value, ColorType, PROFILE_STATE_IS);	
}

static void PROFILE_ADM_UpdateBlockColorValue(PROFILE_T *Profile)
{
	uint32_t i;
	uint32_t BlockColor;
	uint8_t Color;
	uint8_t State;
	
	BlockColor = PROFILE_COLOR_NORMAL;
	for (i = 1; i <= Profile->Descr[Profile->Idx]->Table2Size; i++)
	{
		PROFILE_GetColor(Profile, PROFILE_TABLE_2, i, &Color);
		PROFILE_GetState(Profile, PROFILE_TABLE_2, i, &State);
		if ((State == PROFILE_STATE_IS) && (BlockColor < Color) && (i != PROFILE_ADM_BLOCKCOLOR_IDX))
			BlockColor = Color;
	}
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM_BLOCKCOLOR_IDX, &BlockColor, PROFILE_COLOR_TYPE_NO, PROFILE_STATE_IS);
}

static void PROFILE_ADM_UpdateLedsValue(PROFILE_T *Profile)
{
	uint32_t BlockColor;
	uint32_t LedColor;
	
	/*Update Leds*/
	PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM_BLOCKCOLOR_IDX, &BlockColor);	
	if (BlockColor == 7)
	{
		BSP_LED_Set(BSP_LED_TYPE_ALARM, BSP_LED_COLOR_RED);
		LedColor = 2;
		PROFILE_SetValue(Profile, PROFILE_TABLE_5, PROFILE_ADM_ALARMLED_IDX, &LedColor);
	}
	else if ((BlockColor > 3) && (BlockColor < 7))
	{
		BSP_LED_Set(BSP_LED_TYPE_ALARM, BSP_LED_COLOR_YELLOW);
		LedColor = 3;
		PROFILE_SetValue(Profile, PROFILE_TABLE_5, PROFILE_ADM_ALARMLED_IDX, &LedColor);
	}
	else
	{
		BSP_LED_Set(BSP_LED_TYPE_ALARM, BSP_LED_COLOR_NO);
		LedColor = 0;
		PROFILE_SetValue(Profile, PROFILE_TABLE_5, PROFILE_ADM_ALARMLED_IDX, &LedColor);
	}
	
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
	{
		BSP_LED_Set(BSP_LED_TYPE_POWER, BSP_LED_COLOR_NO);
		LedColor = 0;
		PROFILE_SetValue(Profile, PROFILE_TABLE_5, PROFILE_ADM_POWERLED_IDX, &LedColor);
	}
	else
	{		
		BSP_LED_Set(BSP_LED_TYPE_POWER, BSP_LED_COLOR_GREEN);
		LedColor = 1;
		PROFILE_SetValue(Profile, PROFILE_TABLE_5, PROFILE_ADM_POWERLED_IDX, &LedColor);
	}
}

void PROFILE_ADM_UpdateValues(PROFILE_T *Profile)
{
	PROFILE_ADM_UpdatepId(Profile);
	
	PROFILE_ADM_UpdateSrNumber(Profile);
	
	PROFILE_ADM_UpdateSwNumber(Profile);
	
	PROFILE_ADM_UpdateMCUSwNumber(Profile);
	
	PROFILE_ADM_UpdateMaxVFPGASwNumber(Profile);
	
	PROFILE_ADM_UpdateLineFPGASwNumber(Profile);
	
	PROFILE_ADM_UpdateClient1FPGASwNumber(Profile);
	
	PROFILE_ADM_UpdateClient2FPGASwNumber(Profile);
	
	PROFILE_ADM_UpdateHwNumber(Profile);
	
	PROFILE_ADM_UpdateTCaseValue(Profile);

	PROFILE_ADM_UpdateTimeValue(Profile);

	PROFILE_ADM_UpdatePowerAlarmsValue(Profile);

	PROFILE_ADM_UpdateHwAlarmsValue(Profile);

	PROFILE_ADM_UpdateBlockStateValue(Profile);
	
	PROFILE_ADM_LINE_UpdateValues(Profile);
	
	PROFILE_ADM_CLIENT_UpdateValues(Profile);

	PROFILE_ADM_UpdateBlockColorValue(Profile);

	PROFILE_ADM_UpdateLedsValue(Profile);
}

int32_t PROFILE_ADM_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	if (PROFILE_ADM_LINE_CheckConfigValue(Profile, Table, Idx, Value))
		return -1;
	if (PROFILE_ADM_CLIENT_CheckConfigValue(Profile, Table, Idx, Value))
		return -1;
	//if (PROFILE_ADM_XC_CheckConfigValue(Profile, Table, Idx, Value))
	//	return -1;
	return 0;
}


int32_t PROFILE_ADM_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	if (Idx == PROFILE_ADM_SETDEFAULT_IDX)
		PROFILE_SetDefault(Profile);
	
	if (Idx == PROFILE_ADM_SETFACTORY_IDX)
	{
		if (((*(uint32_t*)Value) == 0) || ((*(uint32_t*)Value) == 1))
			PROFILE_SetFactory(Profile, PROFILE_TABLE_1);
		if (((*(uint32_t*)Value) == 0) || ((*(uint32_t*)Value) == 2))
			PROFILE_SetFactory(Profile, PROFILE_TABLE_3);
		if (((*(uint32_t*)Value) == 0) || ((*(uint32_t*)Value) == 3))
			PROFILE_SetFactory(Profile, PROFILE_TABLE_4);
		if (((*(uint32_t*)Value) == 0) || ((*(uint32_t*)Value) == 4))
			PROFILE_SetFactory(Profile, PROFILE_TABLE_2);
		if (((*(uint32_t*)Value) == 0) || ((*(uint32_t*)Value) == 5))
			PROFILE_SetFactory(Profile, PROFILE_TABLE_103);
	}
	
	if (Idx == PROFILE_ADM_RESET_IDX)
		BSP_Reset();
	if (PROFILE_ADM_LINE_SetConfigValueDependencies(Profile, Table, Idx, Value))
		return -1;
	if (PROFILE_ADM_CLIENT_SetConfigValueDependencies(Profile, Table, Idx, Value))
		return -1;
	//if (PROFILE_ADM_XC_SetConfigValueDependencies(Profile, Table, Idx, Value))
	//	return -1;
	return 0;
}

void PROFILE_ADM_UpdateConfigs(PROFILE_T *Profile)
{
	PROFILE_ADM_LINE_UpdateConfigs(Profile);
	PROFILE_ADM_CLIENT_UpdateConfigs(Profile);
	//PROFILE_ADM_XC_UpdateConfigs(Profile);
}
