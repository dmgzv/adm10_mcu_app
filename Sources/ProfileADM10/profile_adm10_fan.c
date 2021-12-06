#include "profile_adm10_fan.h"
#include "profile_adm10_enums.h"
#include "profile_adm10_bitmsks.h"
#include <string.h>

const uint16_t ProfileADM10DynamicFANSpeedIdx[8] = 
{
	PROFILE_ADM10_FAN1SPEED_IDX, PROFILE_ADM10_FAN2SPEED_IDX, PROFILE_ADM10_FAN3SPEED_IDX, PROFILE_ADM10_FAN4SPEED_IDX,
	PROFILE_ADM10_FAN5SPEED_IDX, PROFILE_ADM10_FAN6SPEED_IDX, PROFILE_ADM10_FAN7SPEED_IDX, PROFILE_ADM10_FAN8SPEED_IDX,
};

int32_t PROFILE_ADM10_FAN_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Restore)
{
	return 0;
}

int32_t PROFILE_ADM10_FAN_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Restore)
{
	return 0;
}

void PROFILE_ADM10_FAN_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t FansSpeed;
	char Tmp[PROFILE_STRING_MAX_LEN];
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETFANSSPEED_IDX, &FansSpeed))
		return;	
	
	switch (FansSpeed)
	{
		case PROFILE_ADM10_SET_FANS_SPEED_ENUM_0:
			SEEPROM_ConfigFansSpeed(&seeprom, 0.0f);
			break;
		case PROFILE_ADM10_SET_FANS_SPEED_ENUM_25:
			SEEPROM_ConfigFansSpeed(&seeprom, 25.0f);
			break;
		case PROFILE_ADM10_SET_FANS_SPEED_ENUM_50:
			SEEPROM_ConfigFansSpeed(&seeprom, 50.0f);
			break;
		case PROFILE_ADM10_SET_FANS_SPEED_ENUM_75:
			SEEPROM_ConfigFansSpeed(&seeprom, 75.0f);
			break;
		case PROFILE_ADM10_SET_FANS_SPEED_ENUM_100:
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
}

static void PROFILE_ADM10_FAN_UpdateInfoAndStaticValues(PROFILE_T *Profile)
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

static void PROFILE_ADM10_FAN_UpdateSpeedValue(PROFILE_T *Profile, uint32_t Idx)
{
	float Value;
	
	Value = SEEPROM_GetFanSpeed(&seeprom, Idx);

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicFANSpeedIdx[Idx], &Value, PROFILE_STATE_IS);
}

void PROFILE_ADM10_FAN_UpdateDynamicValues(PROFILE_T *Profile)
{
	uint32_t i;
	
	for (i = 0; i < 8; i++)
	{
		PROFILE_ADM10_FAN_UpdateSpeedValue(Profile, i);
	}
}

void PROFILE_ADM10_FAN_UpdateValues(PROFILE_T *Profile)
{
	PROFILE_ADM10_FAN_UpdateInfoAndStaticValues(Profile);
	PROFILE_ADM10_FAN_UpdateDynamicValues(Profile);
}
