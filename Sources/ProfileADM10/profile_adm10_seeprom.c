#include "profile_adm10_seeprom.h"
#include "profile_adm10_enums.h"
#include "profile_adm10_bitmsks.h"
#include <string.h>

const uint16_t ProfileADM10ConfigSetFanStateIdx[8] = 
{
	PROFILE_ADM10_SETFAN1STATE_IDX, PROFILE_ADM10_SETFAN2STATE_IDX, PROFILE_ADM10_SETFAN3STATE_IDX, PROFILE_ADM10_SETFAN4STATE_IDX,
	PROFILE_ADM10_SETFAN5STATE_IDX, PROFILE_ADM10_SETFAN6STATE_IDX, PROFILE_ADM10_SETFAN7STATE_IDX, PROFILE_ADM10_SETFAN8STATE_IDX,
};

const uint16_t ProfileADM10ConfigSetInFuncIdx[2] = 
{
	PROFILE_ADM10_SETIN1FUNC_IDX, PROFILE_ADM10_SETIN2FUNC_IDX,
};

const uint16_t ProfileADM10ConfigSetInOpenDescrIdx[2] = 
{
	PROFILE_ADM10_SETIN1OPENDESCR_IDX, PROFILE_ADM10_SETIN2OPENDESCR_IDX,
};

const uint16_t ProfileADM10ConfigSetInCloseDescrIdx[2] = 
{
	PROFILE_ADM10_SETIN1CLOSEDESCR_IDX, PROFILE_ADM10_SETIN2CLOSEDESCR_IDX,
};

const uint16_t ProfileADM10ConfigSetOutFuncIdx[2] = 
{
	PROFILE_ADM10_SETOUT1FUNC_IDX, PROFILE_ADM10_SETOUT2FUNC_IDX,
};

const uint16_t ProfileADM10DynamicInIdx[2] = 
{
	PROFILE_ADM10_IN1_IDX, PROFILE_ADM10_IN2_IDX,
};

const uint16_t ProfileADM10DynamicOutIdx[2] = 
{
	PROFILE_ADM10_OUT1_IDX, PROFILE_ADM10_OUT2_IDX,
};

const uint16_t ProfileADM10DynamicFanSpeedIdx[8] = 
{
	PROFILE_ADM10_FAN1SPEED_IDX, PROFILE_ADM10_FAN2SPEED_IDX, PROFILE_ADM10_FAN3SPEED_IDX, PROFILE_ADM10_FAN4SPEED_IDX,
	PROFILE_ADM10_FAN5SPEED_IDX, PROFILE_ADM10_FAN6SPEED_IDX, PROFILE_ADM10_FAN7SPEED_IDX, PROFILE_ADM10_FAN8SPEED_IDX,
};

void PROFILE_ADM10_SEEPROM_GetStringColor(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t *Color, uint8_t DegradeColor, uint8_t FailureColor)
{
	uint32_t i;
	uint32_t InFunc;
	
	for (i = 0; i < 2; i++)
	{
		if ((TableIdx == PROFILE_TABLE_2) && (ParamIdx == ProfileADM10DynamicInIdx[i]))
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigSetInFuncIdx[i], &InFunc))
				return;
			
			if (InFunc == PROFILE_ADM10_SET_IN_FUNC_ENUM_INFO)
			{
				*Color = PROFILE_COLOR_INFO;
			}
			else if (InFunc == PROFILE_ADM10_SET_IN_FUNC_ENUM_DEGRADEONCLOSE)
			{
				if (SEEPROM_GetInState(&seeprom, i))
				{
					*Color = DegradeColor;
				}
				else
				{
					*Color = PROFILE_COLOR_NORMAL;
				}
			}
			else if (InFunc == PROFILE_ADM10_SET_IN_FUNC_ENUM_DEGRADEONOPEN)
			{
				if (SEEPROM_GetInState(&seeprom, i))
				{
					*Color = PROFILE_COLOR_NORMAL;
				}
				else
				{
					*Color = DegradeColor;
				}
			}
			else if (InFunc == PROFILE_ADM10_SET_IN_FUNC_ENUM_FAILUREONCLOSE)
			{
				if (SEEPROM_GetInState(&seeprom, i))
				{
					*Color = FailureColor;
				}
				else
				{
					*Color = PROFILE_COLOR_NORMAL;
				}
			}
			else if (InFunc == PROFILE_ADM10_SET_IN_FUNC_ENUM_FAILUREONOPEN)
			{
				if (SEEPROM_GetInState(&seeprom, i))
				{
					*Color = PROFILE_COLOR_NORMAL;
				}
				else
				{
					*Color = FailureColor;
				}
			}
			break;
		}
	}
}

int32_t PROFILE_ADM10_SEEPROM_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Restore)
{
	return 0;
}

int32_t PROFILE_ADM10_SEEPROM_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Restore)
{
	return 0;
}

void PROFILE_ADM10_SEEPROM_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t i;
	uint32_t FansSpeed;
	uint32_t OutFunc;
	uint32_t BlockColor, ChassisColor, StackColor;
	char Tmp[PROFILE_STRING_MAX_LEN];
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETFANSSPEED_IDX, &FansSpeed))
		return;	
	
	switch (FansSpeed)
	{
		case PROFILE_ADM10_SET_FANS_SPEED_ENUM_MIN:
			SEEPROM_ConfigFansSpeed(&seeprom, 0.0f);
			break;
		case PROFILE_ADM10_SET_FANS_SPEED_ENUM_LOW:
			SEEPROM_ConfigFansSpeed(&seeprom, 25.0f);
			break;
		case PROFILE_ADM10_SET_FANS_SPEED_ENUM_MID:
			SEEPROM_ConfigFansSpeed(&seeprom, 50.0f);
			break;
		case PROFILE_ADM10_SET_FANS_SPEED_ENUM_HIGH:
			SEEPROM_ConfigFansSpeed(&seeprom, 75.0f);
			break;
		case PROFILE_ADM10_SET_FANS_SPEED_ENUM_MAX:
			SEEPROM_ConfigFansSpeed(&seeprom, 100.0f);
			break;
	}

	if (PROFILE_GetValue(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMVENDOR_IDX, Tmp))
		return;
	SEEPROM_ConfigVendor(&seeprom, Tmp);
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMPTNUMBER_IDX, Tmp))
		return;
	SEEPROM_ConfigPtNumber(&seeprom, Tmp);
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMHWNUMBER_IDX, Tmp))
		return;
	SEEPROM_ConfigHwNumber(&seeprom, Tmp);
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMSRNUMBER_IDX, Tmp))
		return;
	SEEPROM_ConfigSrNumber(&seeprom, Tmp);
	
	for (i = 0; i < 2; i++)
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigSetOutFuncIdx[i], &OutFunc))
			return;
		
		if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_OPEN)
		{
			SEEPROM_ConfigOutState(&seeprom, i, 0);
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_CLOSE)
		{
			SEEPROM_ConfigOutState(&seeprom, i, 1);
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_OPENONBLOCKDEGRADE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_BLOCKCOLOR_IDX, &BlockColor))
				return;
			if (BlockColor > 3)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_CLOSEONBLOCKDEGRADE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_BLOCKCOLOR_IDX, &BlockColor))
				return;
			if (BlockColor > 3)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_OPENONBLOCKFAILURE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_BLOCKCOLOR_IDX, &BlockColor))
				return;
			if (BlockColor > 6)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_CLOSEONBLOCKFAILURE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_BLOCKCOLOR_IDX, &BlockColor))
				return;
			if (BlockColor > 6)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_OPENONCHASSISDEGRADE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CHASSISCOLOR_IDX, &ChassisColor))
				return;
			if (ChassisColor > 3)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_CLOSEONCHASSISDEGRADE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CHASSISCOLOR_IDX, &ChassisColor))
				return;
			if (ChassisColor > 3)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_OPENONCHASSISFAILURE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CHASSISCOLOR_IDX, &ChassisColor))
				return;
			if (ChassisColor > 6)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_CLOSEONCHASSISFAILURE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CHASSISCOLOR_IDX, &ChassisColor))
				return;
			if (ChassisColor > 6)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_OPENONSTACKDEGRADE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_STACKCOLOR_IDX, &StackColor))
				return;
			if (StackColor > 3)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_CLOSEONSTACKDEGRADE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_STACKCOLOR_IDX, &StackColor))
				return;
			if (StackColor > 3)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_OPENONSTACKFAILURE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_STACKCOLOR_IDX, &StackColor))
				return;
			if (StackColor > 6)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_CLOSEONSTACKFAILURE)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_STACKCOLOR_IDX, &StackColor))
				return;
			if (StackColor > 6)
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_OPENONIN1OPEN)
		{
			if (SEEPROM_GetInState(&seeprom, 0))
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_OPENONIN1CLOSE)
		{
			if (SEEPROM_GetInState(&seeprom, 0))
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_OPENONIN2OPEN)
		{
			if (SEEPROM_GetInState(&seeprom, 1))
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
		}
		else if (OutFunc == PROFILE_ADM10_SET_OUT_FUNC_ENUM_OPENONIN2CLOSE)
		{
			if (SEEPROM_GetInState(&seeprom, 1))
			{
				SEEPROM_ConfigOutState(&seeprom, i, 0);
			}
			else
			{
				SEEPROM_ConfigOutState(&seeprom, i, 1);
			}
		}
	}
}

static void PROFILE_ADM10_SEEPROM_UpdateInfoAndStaticValues(PROFILE_T *Profile)
{
	if (BSP_DEV_GET_STATE(&seeprom) == BSP_DEV_STATE_OK)
	{
		BSP_DEV_LOCK(&seeprom);
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMVENDOR_IDX, seeprom.Vendor, PROFILE_STATE_IS);
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMPTNUMBER_IDX, seeprom.PtNumber, PROFILE_STATE_IS);
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMHWNUMBER_IDX, seeprom.HwNumber, PROFILE_STATE_IS);
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMSRNUMBER_IDX, seeprom.SrNumber, PROFILE_STATE_IS);
		BSP_DEV_UNLOCK(&seeprom);
		return;
	}
	
	PROFILE_SetState(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMVENDOR_IDX, PROFILE_STATE_OOS);
	PROFILE_SetState(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMPTNUMBER_IDX, PROFILE_STATE_OOS);
	PROFILE_SetState(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMHWNUMBER_IDX, PROFILE_STATE_OOS);
	PROFILE_SetState(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SEEPROMSRNUMBER_IDX, PROFILE_STATE_OOS);
}

static void PROFILE_ADM10_SEEPROM_UpdateFanSpeedValue(PROFILE_T *Profile, uint32_t Idx)
{
	uint32_t FanState;
	float Value;
	uint8_t State;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigSetFanStateIdx[Idx], &FanState))
		return;	
	
	Value = SEEPROM_GetFanSpeed(&seeprom, Idx);

	if (FanState == PROFILE_ADM10_SET_FAN_STATE_ENUM_IS)
		State = PROFILE_STATE_IS;
	else if (FanState == PROFILE_ADM10_SET_FAN_STATE_ENUM_MT)
		State = PROFILE_STATE_MT;
	else
		State = PROFILE_STATE_OOS;
	
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicFanSpeedIdx[Idx], &Value, State);
}

static void PROFILE_ADM10_SEEPROM_UpdateInValue(PROFILE_T *Profile, uint32_t Idx)
{
	char Tmp[PROFILE_STRING_MAX_LEN];
	
	if (SEEPROM_GetInState(&seeprom, Idx))
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigSetInCloseDescrIdx[Idx], Tmp))
			return;	
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicInIdx[Idx], Tmp, PROFILE_STATE_IS);
	}
	else
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigSetInOpenDescrIdx[Idx], Tmp))
			return;	
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicInIdx[Idx], Tmp, PROFILE_STATE_IS);
	}
}

static void PROFILE_ADM10_SEEPROM_UpdateOutValue(PROFILE_T *Profile, uint32_t Idx)
{
	uint32_t Value;
	
	if (SEEPROM_GetOutState(&seeprom, Idx))
	{
		Value = 1;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicOutIdx[Idx], &Value, PROFILE_STATE_IS);
	}
	else
	{
		Value = 0;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicOutIdx[Idx], &Value, PROFILE_STATE_IS);
	}
}

void PROFILE_ADM10_SEEPROM_UpdateDynamicValues(PROFILE_T *Profile)
{
	uint32_t i;
	
	for (i = 0; i < 8; i++)
	{
		PROFILE_ADM10_SEEPROM_UpdateFanSpeedValue(Profile, i);
	}
	for (i = 0; i < 2; i++)
	{
		PROFILE_ADM10_SEEPROM_UpdateInValue(Profile, i);
		PROFILE_ADM10_SEEPROM_UpdateOutValue(Profile, i);
	}
}

void PROFILE_ADM10_SEEPROM_UpdateValues(PROFILE_T *Profile)
{
	PROFILE_ADM10_SEEPROM_UpdateInfoAndStaticValues(Profile);
	PROFILE_ADM10_SEEPROM_UpdateDynamicValues(Profile);
}
