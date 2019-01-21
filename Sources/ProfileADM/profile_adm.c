#include "profile_adm.h"

static void PROFILE_ADM_UpdateTCaseValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices)
{
	float TCase;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	
	//TCase = Devices->Temp.Temp[0];
	TCase = 36.7f;
	PROFILE_SetValue(Profile, &DynamicParams[PROFILE_ADM_CASETEMP_IDX - 1], &TCase);
	PROFILE_SetColorToThresholds(Profile, &DynamicParams[PROFILE_ADM_CASETEMP_IDX - 1]);
}

static void PROFILE_ADM_UpdateTimeValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices)
{
	uint32_t Time;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	
	Time = osKernelSysTick() / osKernelSysTickFrequency;	
	PROFILE_SetValue(Profile, &DynamicParams[PROFILE_ADM_TIME_IDX - 1], &Time);	
}

static void PROFILE_ADM_UpdateBlockStateValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices)
{
	uint32_t i, Value;
	PROFILE_COLOR_T Color;
	PROFILE_COLOR_T DegradeColor;
	PROFILE_COLOR_T FailureColor;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*Param = &DynamicParams[PROFILE_ADM_BLOCKSTATE_IDX - 1];
	
	PROFILE_GetAlarmColors(Profile, Param, &DegradeColor, &FailureColor);
	
	Value = 1;
	
	if (BSP_DEV_GET_STATE(&Devices->PowerPin.pca9536) == BSP_DEV_STATE_FAIL)
		Value = 3;
	if (BSP_DEV_GET_STATE(&Devices->Power.ucd90120a) == BSP_DEV_STATE_FAIL)
		Value = 3;
	if (BSP_DEV_GET_STATE(&Devices->Power.gdt080a[0]) == BSP_DEV_STATE_FAIL)
		Value = 3;
	if (BSP_DEV_GET_STATE(&Devices->Power.gdt080a[1]) == BSP_DEV_STATE_FAIL)
		Value = 3;
	if (BSP_DEV_GET_STATE(&Devices->Power.dim3r3400) == BSP_DEV_STATE_FAIL)
		Value = 3;
	for (i = 0; i < BSP_CLK_SI5332_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&Devices->Clk.si5332[i]) == BSP_DEV_STATE_FAIL)
			Value = 3;
	}
	for (i = 0; i < BSP_SFP_I2C_SWITCH_PCA9548_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&Devices->SfpSwitch.pca9548a[i]) == BSP_DEV_STATE_FAIL)
			Value = 3;
	}
	for (i = 0; i < BSP_SFP_I2C_SWITCH_PCA9546_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&Devices->SfpSwitch.pca9546a[i]) == BSP_DEV_STATE_FAIL)
			Value = 3;
	}
	for (i = 0; i < BSP_SFP_PIN_PCA9536_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&Devices->SfpPin.pca9536[i]) == BSP_DEV_STATE_FAIL)
			Value = 3;
	}
	for (i = 0; i < BSP_SFP_PIN_PCAL6524_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&Devices->SfpPin.pcal6524[i]) == BSP_DEV_STATE_FAIL)
			Value = 3;
	}
	
	if (Value == 3)
		Color = FailureColor;
	else
		Color = PROFILE_COLOR_NORMAL;
	PROFILE_SetValue(Profile, Param, &Value);
	PROFILE_SetColor(Profile, Param, Color);	
}

static void PROFILE_ADM_UpdateBlockColorValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices)
{
	uint32_t i;
	uint32_t BlockColor;
	PROFILE_COLOR_T Color;
	uint8_t State;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	
	BlockColor = 3;
	for (i = 0; i < Profile->Descr[Profile->Idx]->TablesSize[PROFILE_TABLE_2]; i++)
	{
		if (DynamicParams[i].Color)
		{
			PROFILE_GetColor(Profile, &DynamicParams[i], &Color);
			PROFILE_GetState(Profile, &DynamicParams[i], &State);
			if ((State == PROFILE_STATE_IS) && (BlockColor < Color))
				BlockColor = Color;
		}
	}
	PROFILE_SetValue(Profile, &DynamicParams[PROFILE_ADM_BLOCKCOLOR_IDX - 1], &BlockColor);	
}

static void PROFILE_ADM_UpdateLedsValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices)
{
	uint32_t BlockColor;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*LedParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_5];
	
	/*Update Leds*/
	PROFILE_GetValue(Profile, &DynamicParams[PROFILE_ADM_BLOCKCOLOR_IDX - 1], &BlockColor);	
	if (BlockColor == 7)
		BSP_LED_Set(&Devices->AlarmLed, BSP_LED_COLOR_RED);
	else if ((BlockColor > 3) && (BlockColor < 7))
		BSP_LED_Set(&Devices->AlarmLed, BSP_LED_COLOR_YELLOW);
	else
		BSP_LED_Set(&Devices->AlarmLed, BSP_LED_COLOR_NO);	
	PROFILE_SetValue(Profile, &LedParams[PROFILE_ADM_POWERLED_IDX - 1], &Devices->PowerLed.Color);
	PROFILE_SetValue(Profile, &LedParams[PROFILE_ADM_ALARMLED_IDX - 1], &Devices->AlarmLed.Color);
}

void PROFILE_ADM_UpdateValues(PROFILE_T *Profile, BSP_DEVICES_T *Devices)
{		
	PROFILE_ADM_LINE_UpdateValues(Profile, Devices);
	
	PROFILE_ADM_CLIENT_UpdateValues(Profile, Devices);
	/*Update TCase*/
	PROFILE_ADM_UpdateTCaseValue(Profile, Devices);
	/*Update time*/
	PROFILE_ADM_UpdateTimeValue(Profile, Devices);
	/*Update BlockState*/
	PROFILE_ADM_UpdateBlockStateValue(Profile, Devices);
	/*Update BlockColor*/
	PROFILE_ADM_UpdateBlockColorValue(Profile, Devices);
	/*Update Leds*/
	PROFILE_ADM_UpdateLedsValue(Profile, Devices);
}

int32_t PROFILE_ADM_CheckConfigValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value)
{
	if (PROFILE_ADM_LINE_CheckConfigValue(Profile, Devices, Param, Value))
		return -1;
	if (PROFILE_ADM_CLIENT_CheckConfigValue(Profile, Devices, Param, Value))
		return -1;
	return 0;
}

int32_t PROFILE_ADM_SetConfigValueDependencies(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value)
{
	if (Param->Index == PROFILE_ADM_SETDEFAULT_IDX)
		PROFILE_SetDefault(Profile);
	
	if (Param->Index == PROFILE_ADM_SETFACTORY_IDX)
		PROFILE_SetFactory(Profile);
	
	if (Param->Index == PROFILE_ADM_RESET_IDX)
		BSP_Reset();
	
	if (PROFILE_ADM_LINE_SetConfigValueDependencies(Profile, Devices, Param, Value))
		return -1;
	if (PROFILE_ADM_CLIENT_SetConfigValueDependencies(Profile, Devices, Param, Value))
		return -1;
	return 0;
}

void PROFILE_ADM_UpdateConfigs(PROFILE_T *Profile, BSP_DEVICES_T *Devices, BSP_DEVICES_CFG_T *DevicesCfg)
{	
	PROFILE_ADM_LINE_UpdateConfigs(Profile, Devices, DevicesCfg);
	PROFILE_ADM_CLIENT_UpdateConfigs(Profile, Devices, DevicesCfg);
}

void PROFILE_ADM_InitStates(PROFILE_T *Profile)
{
	PROFILE_PARAM_T	*InfoParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_1];
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	
	PROFILE_SetState(Profile, &InfoParams[PROFILE_ADM_PID_IDX - 1], PROFILE_STATE_IS);	
	PROFILE_SetState(Profile, &InfoParams[PROFILE_ADM_SRNUMBER_IDX - 1], PROFILE_STATE_IS);
	
	PROFILE_SetValue(Profile, &InfoParams[PROFILE_ADM_SWNUMBER_IDX - 1], BSP_SW_VERSION);
	PROFILE_SetState(Profile, &InfoParams[PROFILE_ADM_SWNUMBER_IDX - 1], PROFILE_STATE_IS);
	PROFILE_SetValue(Profile, &InfoParams[PROFILE_ADM_HWNUMBER_IDX - 1], BSP_HW_VERSION);
	PROFILE_SetState(Profile, &InfoParams[PROFILE_ADM_HWNUMBER_IDX - 1], PROFILE_STATE_IS);
	
	PROFILE_SetState(Profile, &DynamicParams[PROFILE_ADM_TIME_IDX - 1], PROFILE_STATE_IS);
	PROFILE_SetState(Profile, &DynamicParams[PROFILE_ADM_BLOCKCOLOR_IDX - 1], PROFILE_STATE_IS);	
	PROFILE_SetState(Profile, &DynamicParams[PROFILE_ADM_BLOCKSTATE_IDX - 1], PROFILE_STATE_IS);
	PROFILE_SetState(Profile, &DynamicParams[PROFILE_ADM_CASETEMP_IDX - 1], PROFILE_STATE_IS);	
}
