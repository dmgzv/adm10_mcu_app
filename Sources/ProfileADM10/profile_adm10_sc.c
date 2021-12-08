#include "profile_adm10_ps.h"
#include "profile_adm10_enums.h"
#include "profile_adm10_bitmsks.h"
#include <string.h>

/*
const uint16_t ProfileADM10DynamicSCBlockStateIdx[2] = 
{
	PROFILE_ADM10_SC1BLOCKSTATE_IDX, PROFILE_ADM10_SC2BLOCKSTATE_IDX,
};

const uint16_t ProfileADM10DynamicSCHwAlarmsIdx[2] = 
{
	PROFILE_ADM10_SC1HWALARMS_IDX, PROFILE_ADM10_SC2HWALARMS_IDX,
};
*/

const uint16_t ProfileADM10InfoSCVendorIdx[2] = 
{
	PROFILE_ADM10_SC1VENDOR_IDX, PROFILE_ADM10_SC2VENDOR_IDX,
};

const uint16_t ProfileADM10InfoSCPtNumberIdx[2] = 
{
	PROFILE_ADM10_SC1PTNUMBER_IDX, PROFILE_ADM10_SC2PTNUMBER_IDX,
};

const uint16_t ProfileADM10InfoSCSrNumberIdx[2] = 
{
	PROFILE_ADM10_SC1SRNUMBER_IDX, PROFILE_ADM10_SC2SRNUMBER_IDX,
};

const uint16_t ProfileADM10InfoSCHwNumberIdx[2] = 
{
	PROFILE_ADM10_SC1HWNUMBER_IDX, PROFILE_ADM10_SC2HWNUMBER_IDX,
};

int32_t PROFILE_ADM10_SC_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Restore)
{
	return 0;
}

int32_t PROFILE_ADM10_SC_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Restore)
{
	return 0;
}

void PROFILE_ADM10_SC_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t i;
	uint32_t StackChassisIdx;
	char Tmp[PROFILE_STRING_MAX_LEN];
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_104, PROFILE_ADM10_STACKCHASSISIDX_IDX, &StackChassisIdx))
		return;
	for (i = 0; i < 2; i++)
	{
		SC_ConfigChassisAddress(&sc[i], StackChassisIdx);

		if (PROFILE_GetValue(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCVendorIdx[i], Tmp))
			return;
		SC_ConfigVendor(&sc[i], Tmp);
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCPtNumberIdx[i], Tmp))
			return;
		SC_ConfigPtNumber(&sc[i], Tmp);
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCHwNumberIdx[i], Tmp))
			return;
		SC_ConfigHwNumber(&sc[i], Tmp);
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCSrNumberIdx[i], Tmp))
			return;
		SC_ConfigSrNumber(&sc[i], Tmp);
	}
}

static void PROFILE_ADM10_SC_UpdateInfoAndStaticValues(PROFILE_T *Profile, uint32_t Idx)
{
	if (SEEPROM_GetSCPresent(&seeprom, Idx))
	{
		if (BSP_DEV_GET_STATE(&sc[Idx]) == BSP_DEV_STATE_OK)
		{
			BSP_DEV_LOCK(&sc[Idx]);
			PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCVendorIdx[Idx], sc[Idx].Vendor, PROFILE_STATE_IS);
			PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCPtNumberIdx[Idx], sc[Idx].PtNumber, PROFILE_STATE_IS);
			PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCHwNumberIdx[Idx], sc[Idx].HwNumber, PROFILE_STATE_IS);
			PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCSrNumberIdx[Idx], sc[Idx].SrNumber, PROFILE_STATE_IS);
			BSP_DEV_UNLOCK(&sc[Idx]);
			return;
		}
	}
	
	PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCVendorIdx[Idx], PROFILE_STATE_OOS);
	PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCSrNumberIdx[Idx], PROFILE_STATE_OOS);
	PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCPtNumberIdx[Idx], PROFILE_STATE_OOS);
	PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADM10InfoSCHwNumberIdx[Idx], PROFILE_STATE_OOS);
}
/*
static void PROFILE_ADM10_SC_UpdateBlockStateValue(PROFILE_T *Profile, uint32_t Idx)
{
	uint32_t Value;
	
	Value = PROFILE_ADM10_SC_BLOCK_STATE_ENUM_ABSENT;
	
	if (SEEPROM_GetSCPresent(&seeprom, Idx))
	{
		Value = PROFILE_ADM10_SC_BLOCK_STATE_ENUM_FAIL;
		
		if (BSP_DEV_GET_STATE(&sc[Idx]) == BSP_DEV_STATE_RESET)
			Value = PROFILE_ADM10_SC_BLOCK_STATE_ENUM_ABSENT;
		else if (BSP_DEV_GET_STATE(&sc[Idx]) == BSP_DEV_STATE_INIT)
			Value = PROFILE_ADM10_SC_BLOCK_STATE_ENUM_NOT_READY;
		else if (BSP_DEV_GET_STATE(&sc[Idx]) == BSP_DEV_STATE_OK)
		{
			if ((!strcmp(sc[Idx].Vendor, IPGP967_VENDOR)) && (!strcmp(sc[Idx].PtNumber, IPGP967_PT_NUMBER)) && (!strcmp(sc[Idx].HwNumber, IPGP967_HW_NUMBER)))
			{
				Value = PROFILE_ADM10_SC_BLOCK_STATE_ENUM_OK;
			}
		}
	}

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicSCBlockStateIdx[Idx], &Value, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_SC_UpdateHwAlarmsValue(PROFILE_T *Profile, uint32_t Idx)
{
	uint32_t Value;
	
	if (SEEPROM_GetSCPresent(&seeprom, Idx))
	{
		Value = 0;
		
		if (BSP_DEV_GET_STATE(&sc[Idx].at24c02c) != BSP_DEV_STATE_OK)
			Value |= PROFILE_ADM10_SC_HW_ALARMS_BITMSK_AT24C02C;
		
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicSCHwAlarmsIdx[Idx], &Value, PROFILE_STATE_IS);
		return;
	}
	
	PROFILE_SetState(Profile, PROFILE_TABLE_2, ProfileADM10DynamicSCHwAlarmsIdx[Idx], PROFILE_STATE_OOS);
}
*/
static void PROFILE_ADM10_SC_UpdateDynamicValues(PROFILE_T *Profile, uint32_t Idx)
{
	//PROFILE_ADM10_SC_UpdateBlockStateValue(Profile, Idx);
	//PROFILE_ADM10_SC_UpdateHwAlarmsValue(Profile, Idx);
}

void PROFILE_ADM10_SC_UpdateValues(PROFILE_T *Profile)
{
	uint32_t i;
	
	for (i = 0; i < 2; i++)
	{
		PROFILE_ADM10_SC_UpdateInfoAndStaticValues(Profile, i);
		PROFILE_ADM10_SC_UpdateDynamicValues(Profile, i);
	}
}
