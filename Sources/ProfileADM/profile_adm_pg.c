#include "profile_adm_pg.h"
#include "profile_adm_values.h"

#define PROFILE_ADM_ODU2_PG_CNT	2

uint32_t ProfileADMConfigODU2PGSetTypeIdx[PROFILE_ADM_ODU2_PG_CNT] = {
	PROFILE_ADM_ODU2_PG_1_SETTYPE_IDX, PROFILE_ADM_ODU2_PG_2_SETTYPE_IDX,
};

uint32_t ProfileADMConfigODU2PGSetModeIdx[PROFILE_ADM_ODU2_PG_CNT] = {
	PROFILE_ADM_ODU2_PG_1_SETMODE_IDX, PROFILE_ADM_ODU2_PG_2_SETMODE_IDX,
};

uint32_t ProfileADMConfigODU2PGSetMonModeIdx[PROFILE_ADM_ODU2_PG_CNT] = {
	PROFILE_ADM_ODU2_PG_1_SETMONMODE_IDX, PROFILE_ADM_ODU2_PG_2_SETMONMODE_IDX,
};

uint32_t ProfileADMConfigODU2PGSetRevModeIdx[PROFILE_ADM_ODU2_PG_CNT] = {
	PROFILE_ADM_ODU2_PG_1_SETREVMODE_IDX, PROFILE_ADM_ODU2_PG_2_SETREVMODE_IDX,
};

uint32_t ProfileADMConfigODU2PGSetPrimaryODUkIdx[PROFILE_ADM_ODU2_PG_CNT] = {
	PROFILE_ADM_ODU2_PG_1_SETPRIMARYODUK_IDX, PROFILE_ADM_ODU2_PG_2_SETPRIMARYODUK_IDX,
};

uint32_t ProfileADMConfigODU2PGSetProtectODUkIdx[PROFILE_ADM_ODU2_PG_CNT] = {
	PROFILE_ADM_ODU2_PG_1_SETPROTECTODUK_IDX, PROFILE_ADM_ODU2_PG_2_SETPROTECTODUK_IDX,
};

uint32_t ProfileADMConfigODU2PGSetWTRTmrIdx[PROFILE_ADM_ODU2_PG_CNT] = {
	PROFILE_ADM_ODU2_PG_1_SETWTRTMR_IDX, PROFILE_ADM_ODU2_PG_2_SETWTRTMR_IDX,
};

uint32_t ProfileADMConfigODU2PGSetHOTmrIdx[PROFILE_ADM_ODU2_PG_CNT] = {
	PROFILE_ADM_ODU2_PG_1_SETHOTMR_IDX, PROFILE_ADM_ODU2_PG_2_SETHOTMR_IDX,
};

uint32_t ProfileADMConfigODU2PGSetFactoryIdx[PROFILE_ADM_ODU2_PG_CNT] = {
	PROFILE_ADM_ODU2_PG_1_SETFACTORY_IDX, PROFILE_ADM_ODU2_PG_2_SETFACTORY_IDX,
};

int32_t PROFILE_ADM_PG_CheckConfigODU2PGSetTypeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 type is always Disabled*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS type is always Unidirerctional 1+1*/
			if (*(uint32_t*)Value != 1)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU2PGSetModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 mode is always Auto*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS type is Auto, Lockout or Forced*/
			if ((*(uint32_t*)Value != 0) && (*(uint32_t*)Value != 1) && (*(uint32_t*)Value != 2))
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU2PGSetMonModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != 1)
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != 1)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU2PGSetRevModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 revert mode is always Revertive*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS revert mode is Revertive or Non-Revertive*/
			if ((*(uint32_t*)Value != 0) && (*(uint32_t*)Value != 1))
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU2PGSetPrimaryODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 Primary ODUk is always None*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS for PG 1 Primary ODUk is always Ln_1_ODU2*/
			if ((ODU2PGIdx == 0) && (*(uint32_t*)Value != 1))
				return -1;
			/*In ATP-2x10-BS for PG 2 Primary ODUk is always Ln_3_ODU2*/
			else if ((ODU2PGIdx == 1) && (*(uint32_t*)Value != 3))
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU2PGSetProtectODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 Primary ODUk is always None*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS for PG 1 Primary ODUk is always Ln_2_ODU2*/
			if ((ODU2PGIdx == 0) && (*(uint32_t*)Value != 2))
				return -1;
			/*In ATP-2x10-BS for PG 2 Primary ODUk is always Ln_4_ODU2*/
			else if ((ODU2PGIdx == 1) && (*(uint32_t*)Value != 4))
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU2PGSetWTRTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS WTR timer grid is 100ms*/
			if ((*(uint32_t*)Value % 100))
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU2PGSetHOTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 HO timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS HO timer grid is 100ms*/
			if ((*(uint32_t*)Value % 100))
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM_ODU2_PG_CNT; i++)
	{
		if (ProfileADMConfigODU2PGSetTypeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU2PGSetTypeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU2PGSetModeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU2PGSetModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU2PGSetMonModeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU2PGSetMonModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU2PGSetRevModeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU2PGSetRevModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU2PGSetPrimaryODUkIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU2PGSetPrimaryODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU2PGSetProtectODUkIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU2PGSetProtectODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU2PGSetWTRTmrIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU2PGSetWTRTmrValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU2PGSetHOTmrIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU2PGSetHOTmrValue(Profile, Table, TableIdx, Value, i);
	}
	
	return 0;
}

static const uint32_t ODU2PGSetTypeFactory[PROFILE_ADM_ODU2_PG_CNT] = {0, 0}; /*Disabled, Disabled*/
static const uint32_t ODU2PGSetModeFactory[PROFILE_ADM_ODU2_PG_CNT] = {0, 0}; /*Auto, Auto*/
static const uint32_t ODU2PGSetMonModeFactory[PROFILE_ADM_ODU2_PG_CNT] = {1, 1}; /*SNC/Ne, SNC/Ne*/
static const uint32_t ODU2PGSetRevModeFactory[PROFILE_ADM_ODU2_PG_CNT] = {0, 0}; /*Revertive, Revertive*/
static const uint32_t ODU2PGSetPrimaryODUkFactory[PROFILE_ADM_ODU2_PG_CNT] = {0, 0}; /*None, None*/
static const uint32_t ODU2PGSetProtectODUkFactory[PROFILE_ADM_ODU2_PG_CNT] = {0, 0}; /*None, None*/
static const uint32_t ODU2PGSetWTRTmrFactory[PROFILE_ADM_ODU2_PG_CNT] = {0, 0};
static const uint32_t ODU2PGSetHOTmrFactory[PROFILE_ADM_ODU2_PG_CNT] = {0, 0};

static int32_t PROFILE_ADM_PG_SetFactoryValue(PROFILE_T *Profile, uint32_t Mode, uint32_t ODU2PGIdx)
{
	switch (Mode)
	{
		case 0:
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetTypeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetTypeFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetModeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetModeFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetMonModeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetMonModeFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetRevModeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetRevModeFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetPrimaryODUkIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetPrimaryODUkFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetProtectODUkIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetProtectODUkFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetWTRTmrIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetWTRTmrFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetHOTmrIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetHOTmrFactory[ODU2PGIdx]);
			break;
		case 3:
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetTypeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetModeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetMonModeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetRevModeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetPrimaryODUkIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetProtectODUkIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetWTRTmrIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetHOTmrIdx[ODU2PGIdx]);
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t PROFILE_ADM_PG_SetConfigSetModeValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM_ODU2_PG_CNT; i++)
	{
		PROFILE_ADM_PG_SetFactoryValue(Profile, *(uint32_t*)Value, i);
	}
	return 0;
}

static int32_t PROFILE_ADM_PG_SetConfigODU2PGSetFactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	return PROFILE_ADM_PG_SetFactoryValue(Profile, Mode, ODU2PGIdx);
}

int32_t PROFILE_ADM_PG_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value)
{
	uint32_t i;
	
	if (TableIdx == PROFILE_ADM_SETMODE_IDX)
		return PROFILE_ADM_PG_SetConfigSetModeValueDependencies(Profile, Table, TableIdx, Value);
	
	for (i = 0; i < PROFILE_ADM_ODU2_PG_CNT; i++)
	{
		if (ProfileADMConfigODU2PGSetFactoryIdx[i] == TableIdx)
			return PROFILE_ADM_PG_SetConfigODU2PGSetFactoryValueDependencies(Profile, Table, TableIdx, Value, i);
	}
	return 0;
}

void PROFILE_ADM_PG_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t PGType[PROFILE_ADM_ODU2_PG_CNT];
	uint32_t PGMode[PROFILE_ADM_ODU2_PG_CNT];
	uint32_t PGMonMode[PROFILE_ADM_ODU2_PG_CNT];
	uint32_t PGRevMode[PROFILE_ADM_ODU2_PG_CNT];
	uint32_t PGPrimaryODUk[PROFILE_ADM_ODU2_PG_CNT];
	uint32_t PGProtectODUk[PROFILE_ADM_ODU2_PG_CNT];
	uint32_t PGWTRTmr[PROFILE_ADM_ODU2_PG_CNT];
	uint32_t PGHOTmr[PROFILE_ADM_ODU2_PG_CNT];
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return;
	
	for (i = 0; i < PROFILE_ADM_ODU2_PG_CNT; i++)
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetTypeIdx[i], &PGType[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetModeIdx[i], &PGMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetMonModeIdx[i], &PGMonMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetRevModeIdx[i], &PGRevMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetPrimaryODUkIdx[i], &PGPrimaryODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetProtectODUkIdx[i], &PGProtectODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetWTRTmrIdx[i], &PGWTRTmr[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetHOTmrIdx[i], &PGHOTmr[i]))
			return;
	}
	
	for (i = 0; i < PROFILE_ADM_ODU2_PG_CNT; i++)
	{
		switch (Mode)
		{
			case 0:
				break;
			case 3:
				if (PGMode[i] == 0) /*Auto*/
				{
					if (PGRevMode[i] == 0) /*Revertive*/
						BSP_FPGA_LINE_AGG_MODE_SET(i, BSP_FPGA_LINE_AGG_MODE_AUTO_REVERTIVE);
					else
						BSP_FPGA_LINE_AGG_MODE_SET(i, BSP_FPGA_LINE_AGG_MODE_AUTO_NON_REVERTIVE);
				}
				else if (PGMode[i] == 1) /*Lockout*/
				{
					BSP_FPGA_LINE_AGG_MODE_SET(i, BSP_FPGA_LINE_AGG_MODE_MANUAL_LINE_0);
				}
				else if (PGMode[i] == 2) /*Forced*/
				{
					BSP_FPGA_LINE_AGG_MODE_SET(i, BSP_FPGA_LINE_AGG_MODE_MANUAL_LINE_1);
				}
				BSP_FPGA_LINE_AGG_WTR_SET(i, PGWTRTmr[i] / 100);
				BSP_FPGA_LINE_AGG_HO_SET(i, PGHOTmr[i] / 100);
				break;
		}
	}
}

void PROFILE_ADM_PG_UpdateValues(PROFILE_T *Profile)
{
	
}

