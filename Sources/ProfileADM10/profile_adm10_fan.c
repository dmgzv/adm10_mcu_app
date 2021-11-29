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
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_104, PROFILE_ADM10_SETFANSSPEED_IDX, &FansSpeed))
		return;	
	
	switch (FansSpeed)
	{
		case PROFILE_ADM10_SET_FANS_SPEED_ENUM_0:
			SEEPROM_ConfigFansSpeed(&seeprom, 0.0f);
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
}

static void PROFILE_ADM10_FAN_UpdateSpeedValue(PROFILE_T *Profile, uint32_t Idx)
{
	float Value;
	
	Value = SEEPROM_GetFanSpeed(&seeprom, Idx);

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicFANSpeedIdx[Idx], &Value, PROFILE_STATE_IS);
}

void PROFILE_ADM10_FAN_UpdateValues(PROFILE_T *Profile)
{
	uint32_t i;
	
	for (i = 0; i < 8; i++)
	{
		PROFILE_ADM10_FAN_UpdateSpeedValue(Profile, i);
	}
}
