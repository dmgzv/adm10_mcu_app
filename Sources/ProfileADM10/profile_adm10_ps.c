#include "profile_adm10_ps.h"
#include "profile_adm10_pg.h"
#include "profile_adm10_enums.h"
#include "profile_adm10_bitmsks.h"
#include "als.h"
#include "perf.h"
#include <string.h>

const uint16_t ProfileADM10DynamicPSBlockStateIdx[2] = 
{
	PROFILE_ADM10_PS1BLOCKSTATE_IDX, PROFILE_ADM10_PS2BLOCKSTATE_IDX,
};

const uint16_t ProfileADM10DynamicPSInVIdx[2] = 
{
	PROFILE_ADM10_PS1INV_IDX, PROFILE_ADM10_PS2INV_IDX,
};

const uint16_t ProfileADM10DynamicPSOutVIdx[2] = 
{
	PROFILE_ADM10_PS1OUTV_IDX, PROFILE_ADM10_PS2OUTV_IDX,
};

const uint16_t ProfileADM10DynamicPSOutIIdx[2] = 
{
	PROFILE_ADM10_PS1OUTI_IDX, PROFILE_ADM10_PS2OUTI_IDX,
};

const uint16_t ProfileADM10DynamicPSTempIdx[2] = 
{
	PROFILE_ADM10_PS1TEMP_IDX, PROFILE_ADM10_PS2TEMP_IDX,
};

const uint16_t ProfileADM10InfoPSVendorIdx[2] = 
{
	PROFILE_ADM10_PS1VENDOR_IDX, PROFILE_ADM10_PS2VENDOR_IDX,
};

const uint16_t ProfileADM10InfoPSPtNumberIdx[2] = 
{
	PROFILE_ADM10_PS1PTNUMBER_IDX, PROFILE_ADM10_PS2PTNUMBER_IDX,
};

const uint16_t ProfileADM10InfoPSSrNumberIdx[2] = 
{
	PROFILE_ADM10_PS1SRNUMBER_IDX, PROFILE_ADM10_PS2SRNUMBER_IDX,
};

const uint16_t ProfileADM10InfoPSHwNumberIdx[2] = 
{
	PROFILE_ADM10_PS1HWNUMBER_IDX, PROFILE_ADM10_PS2HWNUMBER_IDX,
};

const uint16_t ProfileADM10StaticPSInVIdx[2][4] = 
{
	{PROFILE_ADM10_PS1INVCMIN_IDX, PROFILE_ADM10_PS1INVWMIN_IDX, PROFILE_ADM10_PS1INVWMAX_IDX, PROFILE_ADM10_PS1INVCMAX_IDX},
	{PROFILE_ADM10_PS2INVCMIN_IDX, PROFILE_ADM10_PS2INVWMIN_IDX, PROFILE_ADM10_PS2INVWMAX_IDX, PROFILE_ADM10_PS2INVCMAX_IDX},
};

const uint16_t ProfileADM10StaticPSTempIdx[2][4] = 
{
	{PROFILE_ADM10_PS1TEMPCMIN_IDX, PROFILE_ADM10_PS1TEMPWMIN_IDX, PROFILE_ADM10_PS1TEMPWMAX_IDX, PROFILE_ADM10_PS1TEMPCMAX_IDX},
	{PROFILE_ADM10_PS2TEMPCMIN_IDX, PROFILE_ADM10_PS2TEMPWMIN_IDX, PROFILE_ADM10_PS2TEMPWMAX_IDX, PROFILE_ADM10_PS2TEMPCMAX_IDX},
};

int32_t PROFILE_ADM10_PS_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Restore)
{
	return 0;
}

int32_t PROFILE_ADM10_PS_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Restore)
{
	return 0;
}

void PROFILE_ADM10_PS_UpdateConfigs(PROFILE_T *Profile)
{
	
}

static void PROFILE_ADM10_PS_UpdateInfoAndStaticValues(PROFILE_T *Profile, uint32_t Idx)
{
	PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADM10InfoPSVendorIdx[Idx], PROFILE_STATE_IS);
	PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADM10InfoPSSrNumberIdx[Idx], PROFILE_STATE_IS);
	PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADM10InfoPSPtNumberIdx[Idx], PROFILE_STATE_IS);
	PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADM10InfoPSHwNumberIdx[Idx], PROFILE_STATE_IS);
}

static void PROFILE_ADM10_PS_UpdateStateValue(PROFILE_T *Profile, uint32_t Idx)
{
	uint32_t Value = PROFILE_ADM10_PS_BLOCK_STATE_ENUM_OK;

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicPSBlockStateIdx[Idx], &Value, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_PS_UpdateInVValue(PROFILE_T *Profile, uint32_t Idx)
{
	float Value = 48.0f;

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicPSInVIdx[Idx], &Value, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_PS_UpdateOutVValue(PROFILE_T *Profile, uint32_t Idx)
{
	float Value = 12.0f;

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicPSOutVIdx[Idx], &Value, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_PS_UpdateOutIValue(PROFILE_T *Profile, uint32_t Idx)
{
	float Value = 10.0f;

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicPSOutIIdx[Idx], &Value, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_PS_UpdateTempValue(PROFILE_T *Profile, uint32_t Idx)
{
	float Value = 36.6f;

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicPSTempIdx[Idx], &Value, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_PS_UpdateDynamicValues(PROFILE_T *Profile, uint32_t Idx)
{
	PROFILE_ADM10_PS_UpdateStateValue(Profile, Idx);
	PROFILE_ADM10_PS_UpdateInVValue(Profile, Idx);
	PROFILE_ADM10_PS_UpdateOutVValue(Profile, Idx);
	PROFILE_ADM10_PS_UpdateOutIValue(Profile, Idx);
	PROFILE_ADM10_PS_UpdateTempValue(Profile, Idx);
}

void PROFILE_ADM10_PS_UpdateValues(PROFILE_T *Profile)
{
	uint32_t i;
	
	for (i = 0; i < 2; i++)
	{
		PROFILE_ADM10_PS_UpdateInfoAndStaticValues(Profile, i);
		PROFILE_ADM10_PS_UpdateDynamicValues(Profile, i);
	}
}
