#include "profile_adm_pg.h"
#include "profile_adm_values.h"
#include "profile_adm_xc.h"

uint32_t ProfileADMConfigODU2PGSetTypeIdx[PROFILE_ADM_ODU2_PGS] = {
	PROFILE_ADM_ODU2_PG_1_SETTYPE_IDX, PROFILE_ADM_ODU2_PG_2_SETTYPE_IDX,
};

uint32_t ProfileADMConfigODU2PGSetModeIdx[PROFILE_ADM_ODU2_PGS] = {
	PROFILE_ADM_ODU2_PG_1_SETMODE_IDX, PROFILE_ADM_ODU2_PG_2_SETMODE_IDX,
};

uint32_t ProfileADMConfigODU2PGSetMonModeIdx[PROFILE_ADM_ODU2_PGS] = {
	PROFILE_ADM_ODU2_PG_1_SETMONMODE_IDX, PROFILE_ADM_ODU2_PG_2_SETMONMODE_IDX,
};

uint32_t ProfileADMConfigODU2PGSetRevModeIdx[PROFILE_ADM_ODU2_PGS] = {
	PROFILE_ADM_ODU2_PG_1_SETREVMODE_IDX, PROFILE_ADM_ODU2_PG_2_SETREVMODE_IDX,
};

uint32_t ProfileADMConfigODU2PGSetPrimaryODUkIdx[PROFILE_ADM_ODU2_PGS] = {
	PROFILE_ADM_ODU2_PG_1_SETPRIMARYODUK_IDX, PROFILE_ADM_ODU2_PG_2_SETPRIMARYODUK_IDX,
};

uint32_t ProfileADMConfigODU2PGSetProtectODUkIdx[PROFILE_ADM_ODU2_PGS] = {
	PROFILE_ADM_ODU2_PG_1_SETPROTECTODUK_IDX, PROFILE_ADM_ODU2_PG_2_SETPROTECTODUK_IDX,
};

uint32_t ProfileADMConfigODU2PGSetWTRTmrIdx[PROFILE_ADM_ODU2_PGS] = {
	PROFILE_ADM_ODU2_PG_1_SETWTRTMR_IDX, PROFILE_ADM_ODU2_PG_2_SETWTRTMR_IDX,
};

uint32_t ProfileADMConfigODU2PGSetHOTmrIdx[PROFILE_ADM_ODU2_PGS] = {
	PROFILE_ADM_ODU2_PG_1_SETHOTMR_IDX, PROFILE_ADM_ODU2_PG_2_SETHOTMR_IDX,
};

uint32_t ProfileADMConfigODU2PGSetFactoryIdx[PROFILE_ADM_ODU2_PGS] = {
	PROFILE_ADM_ODU2_PG_1_SETFACTORY_IDX, PROFILE_ADM_ODU2_PG_2_SETFACTORY_IDX,
};

uint32_t ProfileADMConfigODU1PGSetTypeIdx[PROFILE_ADM_ODU1_PGS] = {
	PROFILE_ADM_ODU1_PG_1_SETTYPE_IDX, PROFILE_ADM_ODU1_PG_2_SETTYPE_IDX, PROFILE_ADM_ODU1_PG_3_SETTYPE_IDX, PROFILE_ADM_ODU1_PG_4_SETTYPE_IDX,
};

uint32_t ProfileADMConfigODU1PGSetModeIdx[PROFILE_ADM_ODU1_PGS] = {
	PROFILE_ADM_ODU1_PG_1_SETMODE_IDX, PROFILE_ADM_ODU1_PG_2_SETMODE_IDX, PROFILE_ADM_ODU1_PG_3_SETMODE_IDX, PROFILE_ADM_ODU1_PG_4_SETMODE_IDX,
};

uint32_t ProfileADMConfigODU1PGSetMonModeIdx[PROFILE_ADM_ODU1_PGS] = {
	PROFILE_ADM_ODU1_PG_1_SETMONMODE_IDX, PROFILE_ADM_ODU1_PG_2_SETMONMODE_IDX, PROFILE_ADM_ODU1_PG_3_SETMONMODE_IDX, PROFILE_ADM_ODU1_PG_4_SETMONMODE_IDX,
};

uint32_t ProfileADMConfigODU1PGSetRevModeIdx[PROFILE_ADM_ODU1_PGS] = {
	PROFILE_ADM_ODU1_PG_1_SETREVMODE_IDX, PROFILE_ADM_ODU1_PG_2_SETREVMODE_IDX, PROFILE_ADM_ODU1_PG_3_SETREVMODE_IDX, PROFILE_ADM_ODU1_PG_4_SETREVMODE_IDX,
};

uint32_t ProfileADMConfigODU1PGSetPrimaryODUkIdx[PROFILE_ADM_ODU1_PGS] = {
	PROFILE_ADM_ODU1_PG_1_SETPRIMARYODUK_IDX, PROFILE_ADM_ODU1_PG_2_SETPRIMARYODUK_IDX, PROFILE_ADM_ODU1_PG_3_SETPRIMARYODUK_IDX, PROFILE_ADM_ODU1_PG_4_SETPRIMARYODUK_IDX,
};

uint32_t ProfileADMConfigODU1PGSetProtectODUkIdx[PROFILE_ADM_ODU1_PGS] = {
	PROFILE_ADM_ODU1_PG_1_SETPROTECTODUK_IDX, PROFILE_ADM_ODU1_PG_2_SETPROTECTODUK_IDX, PROFILE_ADM_ODU1_PG_3_SETPROTECTODUK_IDX, PROFILE_ADM_ODU1_PG_4_SETPROTECTODUK_IDX,
};

uint32_t ProfileADMConfigODU1PGSetWTRTmrIdx[PROFILE_ADM_ODU1_PGS] = {
	PROFILE_ADM_ODU1_PG_1_SETWTRTMR_IDX, PROFILE_ADM_ODU1_PG_2_SETWTRTMR_IDX, PROFILE_ADM_ODU1_PG_3_SETWTRTMR_IDX, PROFILE_ADM_ODU1_PG_4_SETWTRTMR_IDX,
};

uint32_t ProfileADMConfigODU1PGSetHOTmrIdx[PROFILE_ADM_ODU1_PGS] = {
	PROFILE_ADM_ODU1_PG_1_SETHOTMR_IDX, PROFILE_ADM_ODU1_PG_2_SETHOTMR_IDX, PROFILE_ADM_ODU1_PG_3_SETHOTMR_IDX, PROFILE_ADM_ODU1_PG_4_SETHOTMR_IDX,
};

uint32_t ProfileADMConfigODU1PGSetFactoryIdx[PROFILE_ADM_ODU1_PGS] = {
	PROFILE_ADM_ODU1_PG_1_SETFACTORY_IDX, PROFILE_ADM_ODU1_PG_2_SETFACTORY_IDX, PROFILE_ADM_ODU1_PG_3_SETFACTORY_IDX, PROFILE_ADM_ODU1_PG_4_SETFACTORY_IDX,
};

uint32_t ProfileADMConfigODU0PGSetTypeIdx[PROFILE_ADM_ODU0_PGS] = {
	PROFILE_ADM_ODU0_PG_1_SETTYPE_IDX, PROFILE_ADM_ODU0_PG_2_SETTYPE_IDX, PROFILE_ADM_ODU0_PG_3_SETTYPE_IDX, PROFILE_ADM_ODU0_PG_4_SETTYPE_IDX,
	PROFILE_ADM_ODU0_PG_5_SETTYPE_IDX, PROFILE_ADM_ODU0_PG_6_SETTYPE_IDX, PROFILE_ADM_ODU0_PG_7_SETTYPE_IDX, PROFILE_ADM_ODU0_PG_8_SETTYPE_IDX,
};

uint32_t ProfileADMConfigODU0PGSetModeIdx[PROFILE_ADM_ODU0_PGS] = {
	PROFILE_ADM_ODU0_PG_1_SETMODE_IDX, PROFILE_ADM_ODU0_PG_2_SETMODE_IDX, PROFILE_ADM_ODU0_PG_3_SETMODE_IDX, PROFILE_ADM_ODU0_PG_4_SETMODE_IDX,
	PROFILE_ADM_ODU0_PG_5_SETMODE_IDX, PROFILE_ADM_ODU0_PG_6_SETMODE_IDX, PROFILE_ADM_ODU0_PG_7_SETMODE_IDX, PROFILE_ADM_ODU0_PG_8_SETMODE_IDX,
};

uint32_t ProfileADMConfigODU0PGSetMonModeIdx[PROFILE_ADM_ODU0_PGS] = {
	PROFILE_ADM_ODU0_PG_1_SETMONMODE_IDX, PROFILE_ADM_ODU0_PG_2_SETMONMODE_IDX, PROFILE_ADM_ODU0_PG_3_SETMONMODE_IDX, PROFILE_ADM_ODU0_PG_4_SETMONMODE_IDX,
	PROFILE_ADM_ODU0_PG_5_SETMONMODE_IDX, PROFILE_ADM_ODU0_PG_6_SETMONMODE_IDX, PROFILE_ADM_ODU0_PG_7_SETMONMODE_IDX, PROFILE_ADM_ODU0_PG_8_SETMONMODE_IDX,
};

uint32_t ProfileADMConfigODU0PGSetRevModeIdx[PROFILE_ADM_ODU0_PGS] = {
	PROFILE_ADM_ODU0_PG_1_SETREVMODE_IDX, PROFILE_ADM_ODU0_PG_2_SETREVMODE_IDX, PROFILE_ADM_ODU0_PG_3_SETREVMODE_IDX, PROFILE_ADM_ODU0_PG_4_SETREVMODE_IDX,
	PROFILE_ADM_ODU0_PG_5_SETREVMODE_IDX, PROFILE_ADM_ODU0_PG_6_SETREVMODE_IDX, PROFILE_ADM_ODU0_PG_7_SETREVMODE_IDX, PROFILE_ADM_ODU0_PG_8_SETREVMODE_IDX,
};

uint32_t ProfileADMConfigODU0PGSetPrimaryODUkIdx[PROFILE_ADM_ODU0_PGS] = {
	PROFILE_ADM_ODU0_PG_1_SETPRIMARYODUK_IDX, PROFILE_ADM_ODU0_PG_2_SETPRIMARYODUK_IDX, PROFILE_ADM_ODU0_PG_3_SETPRIMARYODUK_IDX, PROFILE_ADM_ODU0_PG_4_SETPRIMARYODUK_IDX,
	PROFILE_ADM_ODU0_PG_5_SETPRIMARYODUK_IDX, PROFILE_ADM_ODU0_PG_6_SETPRIMARYODUK_IDX, PROFILE_ADM_ODU0_PG_7_SETPRIMARYODUK_IDX, PROFILE_ADM_ODU0_PG_8_SETPRIMARYODUK_IDX,
};

uint32_t ProfileADMConfigODU0PGSetProtectODUkIdx[PROFILE_ADM_ODU0_PGS] = {
	PROFILE_ADM_ODU0_PG_1_SETPROTECTODUK_IDX, PROFILE_ADM_ODU0_PG_2_SETPROTECTODUK_IDX, PROFILE_ADM_ODU0_PG_3_SETPROTECTODUK_IDX, PROFILE_ADM_ODU0_PG_4_SETPROTECTODUK_IDX,
	PROFILE_ADM_ODU0_PG_5_SETPROTECTODUK_IDX, PROFILE_ADM_ODU0_PG_6_SETPROTECTODUK_IDX, PROFILE_ADM_ODU0_PG_7_SETPROTECTODUK_IDX, PROFILE_ADM_ODU0_PG_8_SETPROTECTODUK_IDX,
};

uint32_t ProfileADMConfigODU0PGSetWTRTmrIdx[PROFILE_ADM_ODU0_PGS] = {
	PROFILE_ADM_ODU0_PG_1_SETWTRTMR_IDX, PROFILE_ADM_ODU0_PG_2_SETWTRTMR_IDX, PROFILE_ADM_ODU0_PG_3_SETWTRTMR_IDX, PROFILE_ADM_ODU0_PG_4_SETWTRTMR_IDX,
	PROFILE_ADM_ODU0_PG_5_SETWTRTMR_IDX, PROFILE_ADM_ODU0_PG_6_SETWTRTMR_IDX, PROFILE_ADM_ODU0_PG_7_SETWTRTMR_IDX, PROFILE_ADM_ODU0_PG_8_SETWTRTMR_IDX,
};

uint32_t ProfileADMConfigODU0PGSetHOTmrIdx[PROFILE_ADM_ODU0_PGS] = {
	PROFILE_ADM_ODU0_PG_1_SETHOTMR_IDX, PROFILE_ADM_ODU0_PG_2_SETHOTMR_IDX, PROFILE_ADM_ODU0_PG_3_SETHOTMR_IDX, PROFILE_ADM_ODU0_PG_4_SETHOTMR_IDX,
	PROFILE_ADM_ODU0_PG_5_SETHOTMR_IDX, PROFILE_ADM_ODU0_PG_6_SETHOTMR_IDX, PROFILE_ADM_ODU0_PG_7_SETHOTMR_IDX, PROFILE_ADM_ODU0_PG_8_SETHOTMR_IDX,
};

uint32_t ProfileADMConfigODU0PGSetFactoryIdx[PROFILE_ADM_ODU0_PGS] = {
	PROFILE_ADM_ODU0_PG_1_SETFACTORY_IDX, PROFILE_ADM_ODU0_PG_2_SETFACTORY_IDX, PROFILE_ADM_ODU0_PG_3_SETFACTORY_IDX, PROFILE_ADM_ODU0_PG_4_SETFACTORY_IDX,
	PROFILE_ADM_ODU0_PG_5_SETFACTORY_IDX, PROFILE_ADM_ODU0_PG_6_SETFACTORY_IDX, PROFILE_ADM_ODU0_PG_7_SETFACTORY_IDX, PROFILE_ADM_ODU0_PG_8_SETFACTORY_IDX,
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

int32_t PROFILE_ADM_PG_CheckConfigODU1PGSetTypeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 type is Disabled or Unidirerctional 1+1*/
			if ((*(uint32_t*)Value != 0) && (*(uint32_t*)Value != 1))
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS type is always Disabled*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU1PGSetModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 mode is Auto, Lockout or Forced*/
			if ((*(uint32_t*)Value != 0) && (*(uint32_t*)Value != 1) && (*(uint32_t*)Value != 2))
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS type is always Auto*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU1PGSetMonModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
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

int32_t PROFILE_ADM_PG_CheckConfigODU1PGSetRevModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 revert mode is Revertive or Non-Revertive*/
			if ((*(uint32_t*)Value != 0) && (*(uint32_t*)Value != 1))
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS revert mode is always Revertive*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU1PGSetPrimaryODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t PrimaryODUk, ProtectODUk;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			if (*(uint32_t*)Value == 0)
				return 0;
			/*In ADM10 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM_ODU1_PGS; i++)
			{
				if (i != ODU1PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetPrimaryODUkIdx[i], &PrimaryODUk))
						return -1;
					if (*(uint32_t*)Value == PrimaryODUk)
						return -1;
				}
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetProtectODUkIdx[i], &ProtectODUk))
					return -1;
				if (*(uint32_t*)Value == ProtectODUk)
					return -1;
			}
			break;
		case 3:
			/*In ATP-2x10-BS Primary ODUk is always None*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU1PGSetProtectODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t PrimaryODUk, ProtectODUk;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			if (*(uint32_t*)Value == 0)
				return 0;
			/*In ADM10 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM_ODU1_PGS; i++)
			{				
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetPrimaryODUkIdx[i], &PrimaryODUk))
					return -1;
				if (*(uint32_t*)Value == PrimaryODUk)
					return -1;
				if (i != ODU1PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetProtectODUkIdx[i], &ProtectODUk))
						return -1;
					if (*(uint32_t*)Value == ProtectODUk)
						return -1;
				}
			}
			break;
		case 3:
			/*In ATP-2x10-BS Protect ODUk is always None*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU1PGSetWTRTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			break;
		case 3:
			/*In ATP-2x10-BS WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU1PGSetHOTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM-10 HO timer grid is 100ms*/
			if ((*(uint32_t*)Value % 100))
				return -1;	
			break;
		case 3:
			/*In ATP-2x10-BS WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU0PGSetTypeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 type is Disabled or Unidirerctional 1+1*/
			if ((*(uint32_t*)Value != 0) && (*(uint32_t*)Value != 1))
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS type is always Disabled*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU0PGSetModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 mode is Auto, Lockout or Forced*/
			if ((*(uint32_t*)Value != 0) && (*(uint32_t*)Value != 1) && (*(uint32_t*)Value != 2))
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS type is always Auto*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU0PGSetMonModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
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

int32_t PROFILE_ADM_PG_CheckConfigODU0PGSetRevModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM10 revert mode is Revertive or Non-Revertive*/
			if ((*(uint32_t*)Value != 0) && (*(uint32_t*)Value != 1))
				return -1;
			break;
		case 3:
			/*In ATP-2x10-BS revert mode is always Revertive*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU0PGSetPrimaryODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t PrimaryODUk, ProtectODUk;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			if (*(uint32_t*)Value == 0)
				return 0;
			/*In ADM10 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM_ODU0_PGS; i++)
			{
				if (i != ODU0PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetPrimaryODUkIdx[i], &PrimaryODUk))
						return -1;
					if (*(uint32_t*)Value == PrimaryODUk)
						return -1;
				}
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetProtectODUkIdx[i], &ProtectODUk))
					return -1;
				if (*(uint32_t*)Value == ProtectODUk)
					return -1;
			}
			break;
		case 3:
			/*In ATP-2x10-BS Primary ODUk is always None*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU0PGSetProtectODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t PrimaryODUk, ProtectODUk;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			if (*(uint32_t*)Value == 0)
				return 0;
			/*In ADM10 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM_ODU0_PGS; i++)
			{
				
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetPrimaryODUkIdx[i], &PrimaryODUk))
					return -1;
				if (*(uint32_t*)Value == PrimaryODUk)
					return -1;
				if (i != ODU0PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetProtectODUkIdx[i], &ProtectODUk))
						return -1;
					if (*(uint32_t*)Value == ProtectODUk)
						return -1;
				}
			}
			break;
		case 3:
			/*In ATP-2x10-BS Protect ODUk is always None*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU0PGSetWTRTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			break;
		case 3:
			/*In ATP-2x10-BS WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM_PG_CheckConfigODU0PGSetHOTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			/*In ADM-10 HO timer grid is 100ms*/
			if ((*(uint32_t*)Value % 100))
				return -1;	
			break;
		case 3:
			/*In ATP-2x10-BS WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
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
	
	for (i = 0; i < PROFILE_ADM_ODU2_PGS; i++)
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
	
	for (i = 0; i < PROFILE_ADM_ODU1_PGS; i++)
	{
		if (ProfileADMConfigODU1PGSetTypeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU1PGSetTypeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU1PGSetModeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU1PGSetModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU1PGSetMonModeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU1PGSetMonModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU1PGSetRevModeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU1PGSetRevModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU1PGSetPrimaryODUkIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU1PGSetPrimaryODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU1PGSetProtectODUkIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU1PGSetProtectODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU1PGSetWTRTmrIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU1PGSetWTRTmrValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU1PGSetHOTmrIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU1PGSetHOTmrValue(Profile, Table, TableIdx, Value, i);
	}
	
	for (i = 0; i < PROFILE_ADM_ODU0_PGS; i++)
	{
		if (ProfileADMConfigODU0PGSetTypeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU0PGSetTypeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU0PGSetModeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU0PGSetModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU0PGSetMonModeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU0PGSetMonModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU0PGSetRevModeIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU0PGSetRevModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU0PGSetPrimaryODUkIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU0PGSetPrimaryODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU0PGSetProtectODUkIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU0PGSetProtectODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU0PGSetWTRTmrIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU0PGSetWTRTmrValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADMConfigODU0PGSetHOTmrIdx[i] == TableIdx)
			return PROFILE_ADM_PG_CheckConfigODU0PGSetHOTmrValue(Profile, Table, TableIdx, Value, i);
	}
	
	return 0;
}

static const uint32_t ODU2PGSetTypeFactory[PROFILE_ADM_ODU2_PGS] = {0, 0}; /*Disabled, Disabled*/
static const uint32_t ODU2PGSetModeFactory[PROFILE_ADM_ODU2_PGS] = {0, 0}; /*Auto, Auto*/
static const uint32_t ODU2PGSetMonModeFactory[PROFILE_ADM_ODU2_PGS] = {1, 1}; /*SNC/Ne, SNC/Ne*/
static const uint32_t ODU2PGSetRevModeFactory[PROFILE_ADM_ODU2_PGS] = {0, 0}; /*Revertive, Revertive*/
static const uint32_t ODU2PGSetPrimaryODUkFactory[PROFILE_ADM_ODU2_PGS] = {0, 0}; /*None, None*/
static const uint32_t ODU2PGSetProtectODUkFactory[PROFILE_ADM_ODU2_PGS] = {0, 0}; /*None, None*/
static const uint32_t ODU2PGSetWTRTmrFactory[PROFILE_ADM_ODU2_PGS] = {0, 0};
static const uint32_t ODU2PGSetHOTmrFactory[PROFILE_ADM_ODU2_PGS] = {0, 0};

static int32_t PROFILE_ADM_ODU2PG_SetFactoryValue(PROFILE_T *Profile, uint32_t Mode, uint32_t ODU2PGIdx)
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

static int32_t PROFILE_ADM_ODU1PG_SetFactoryValue(PROFILE_T *Profile, uint32_t Mode, uint32_t ODU1PGIdx)
{
	switch (Mode)
	{
		case 0:
		case 3:
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetTypeIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetModeIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetMonModeIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetRevModeIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetPrimaryODUkIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetProtectODUkIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetWTRTmrIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetHOTmrIdx[ODU1PGIdx]);
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t PROFILE_ADM_ODU0PG_SetFactoryValue(PROFILE_T *Profile, uint32_t Mode, uint32_t ODU0PGIdx)
{
	switch (Mode)
	{
		case 0:
		case 3:
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetTypeIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetModeIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetMonModeIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetRevModeIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetPrimaryODUkIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetProtectODUkIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetWTRTmrIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetHOTmrIdx[ODU0PGIdx]);
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t PROFILE_ADM_PG_SetConfigSetModeValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM_ODU2_PGS; i++)
	{
		PROFILE_ADM_ODU2PG_SetFactoryValue(Profile, *(uint32_t*)Value, i);
	}
	for (i = 0; i < PROFILE_ADM_ODU1_PGS; i++)
	{
		PROFILE_ADM_ODU1PG_SetFactoryValue(Profile, *(uint32_t*)Value, i);
	}
	for (i = 0; i < PROFILE_ADM_ODU0_PGS; i++)
	{
		PROFILE_ADM_ODU0PG_SetFactoryValue(Profile, *(uint32_t*)Value, i);
	}
	return 0;
}

static int32_t PROFILE_ADM_PG_SetConfigODU2PGSetFactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	return PROFILE_ADM_ODU2PG_SetFactoryValue(Profile, Mode, ODU2PGIdx);
}

static int32_t PROFILE_ADM_PG_SetConfigODU1PGSetFactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	return PROFILE_ADM_ODU1PG_SetFactoryValue(Profile, Mode, ODU1PGIdx);
}

static int32_t PROFILE_ADM_PG_SetConfigODU0PGSetFactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return -1;
	
	return PROFILE_ADM_ODU0PG_SetFactoryValue(Profile, Mode, ODU0PGIdx);
}

int32_t PROFILE_ADM_PG_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value)
{
	uint32_t i;
	
	if (TableIdx == PROFILE_ADM_SETMODE_IDX)
		return PROFILE_ADM_PG_SetConfigSetModeValueDependencies(Profile, Table, TableIdx, Value);
	
	for (i = 0; i < PROFILE_ADM_ODU2_PGS; i++)
	{
		if (ProfileADMConfigODU2PGSetFactoryIdx[i] == TableIdx)
			return PROFILE_ADM_PG_SetConfigODU2PGSetFactoryValueDependencies(Profile, Table, TableIdx, Value, i);
	}
	
	for (i = 0; i < PROFILE_ADM_ODU1_PGS; i++)
	{
		if (ProfileADMConfigODU1PGSetFactoryIdx[i] == TableIdx)
			return PROFILE_ADM_PG_SetConfigODU1PGSetFactoryValueDependencies(Profile, Table, TableIdx, Value, i);
	}
	
	for (i = 0; i < PROFILE_ADM_ODU0_PGS; i++)
	{
		if (ProfileADMConfigODU0PGSetFactoryIdx[i] == TableIdx)
			return PROFILE_ADM_PG_SetConfigODU0PGSetFactoryValueDependencies(Profile, Table, TableIdx, Value, i);
	}
	return 0;
}

static void PROFILE_ADM_ODU2PG_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t ODU2PGType[PROFILE_ADM_ODU2_PGS];
	uint32_t ODU2PGMode[PROFILE_ADM_ODU2_PGS];
	uint32_t ODU2PGMonMode[PROFILE_ADM_ODU2_PGS];
	uint32_t ODU2PGRevMode[PROFILE_ADM_ODU2_PGS];
	uint32_t ODU2PGPrimaryODUk[PROFILE_ADM_ODU2_PGS];
	uint32_t ODU2PGProtectODUk[PROFILE_ADM_ODU2_PGS];
	uint32_t ODU2PGWTRTmr[PROFILE_ADM_ODU2_PGS];
	uint32_t ODU2PGHOTmr[PROFILE_ADM_ODU2_PGS];
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return;
	
	for (i = 0; i < PROFILE_ADM_ODU2_PGS; i++)
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetTypeIdx[i], &ODU2PGType[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetModeIdx[i], &ODU2PGMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetMonModeIdx[i], &ODU2PGMonMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetRevModeIdx[i], &ODU2PGRevMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetPrimaryODUkIdx[i], &ODU2PGPrimaryODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetProtectODUkIdx[i], &ODU2PGProtectODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetWTRTmrIdx[i], &ODU2PGWTRTmr[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU2PGSetHOTmrIdx[i], &ODU2PGHOTmr[i]))
			return;
	}
	
	for (i = 0; i < PROFILE_ADM_ODU2_PGS; i++)
	{
		switch (Mode)
		{
			case 0:
				break;
			case 3:
				if (ODU2PGMode[i] == 0) /*Auto*/
				{
					if (ODU2PGRevMode[i] == 0) /*Revertive*/
						BSP_FPGA_LINE_AGG_MODE_SET(i, BSP_FPGA_LINE_AGG_MODE_AUTO_REVERTIVE);
					else
						BSP_FPGA_LINE_AGG_MODE_SET(i, BSP_FPGA_LINE_AGG_MODE_AUTO_NON_REVERTIVE);
				}
				else if (ODU2PGMode[i] == 1) /*Lockout*/
				{
					BSP_FPGA_LINE_AGG_MODE_SET(i, BSP_FPGA_LINE_AGG_MODE_MANUAL_LINE_0);
				}
				else if (ODU2PGMode[i] == 2) /*Forced*/
				{
					BSP_FPGA_LINE_AGG_MODE_SET(i, BSP_FPGA_LINE_AGG_MODE_MANUAL_LINE_1);
				}
				BSP_FPGA_LINE_AGG_WTR_SET(i, ODU2PGWTRTmr[i] / 100);
				BSP_FPGA_LINE_AGG_HO_SET(i, ODU2PGHOTmr[i] / 100);
				break;
		}
	}
}

static void PROFILE_ADM_ODU1PG_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t i, j;
	uint32_t Mode;
	uint32_t PrimaryLineIdx, PrimaryODU1Idx, ProtectLineIdx, ProtectODU1Idx;
	uint32_t ODU1XCSrc, ODU1XCDst;
	uint32_t PrimaryODU1Mux;
	uint32_t Target, Client;
	uint32_t ODU1PGType[PROFILE_ADM_ODU1_PGS];
	uint32_t ODU1PGMode[PROFILE_ADM_ODU1_PGS];
	uint32_t ODU1PGMonMode[PROFILE_ADM_ODU1_PGS];
	uint32_t ODU1PGRevMode[PROFILE_ADM_ODU1_PGS];
	uint32_t ODU1PGPrimaryODUk[PROFILE_ADM_ODU1_PGS];
	uint32_t ODU1PGProtectODUk[PROFILE_ADM_ODU1_PGS];
	uint32_t ODU1PGWTRTmr[PROFILE_ADM_ODU1_PGS];
	uint32_t ODU1PGHOTmr[PROFILE_ADM_ODU1_PGS];
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return;
	
	for (i = 0; i < PROFILE_ADM_ODU1_PGS; i++)
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetTypeIdx[i], &ODU1PGType[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetModeIdx[i], &ODU1PGMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetMonModeIdx[i], &ODU1PGMonMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetRevModeIdx[i], &ODU1PGRevMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetPrimaryODUkIdx[i], &ODU1PGPrimaryODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetProtectODUkIdx[i], &ODU1PGProtectODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetWTRTmrIdx[i], &ODU1PGWTRTmr[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1PGSetHOTmrIdx[i], &ODU1PGHOTmr[i]))
			return;
	}
	
	for (i = 0; i < 8; i++)
	{
		BSP_FPGA_LINE_DEMUX_CLIENT_ODU1_PG_MODE_SET(i, 0x00);
	}
	
	for (i = 0; i < PROFILE_ADM_ODU1_PGS; i++)
	{
		switch (Mode)
		{
			case 0:
				if (ODU1PGType[i] != 0)
				{
					if ((ODU1PGPrimaryODUk[i] == 0) || (ODU1PGProtectODUk[i] == 0))
						continue;
					
					PrimaryLineIdx = (ODU1PGPrimaryODUk[i] - 1) / 4;
					PrimaryODU1Idx = (ODU1PGPrimaryODUk[i] - 1) % 4;
					ProtectLineIdx = (ODU1PGProtectODUk[i] - 1) / 4;
					ProtectODU1Idx = (ODU1PGProtectODUk[i] - 1) % 4;
					
					PrimaryODU1Mux = BSP_FPGA_LINE_MUX_ODU12_GET((PrimaryLineIdx * 4) + PrimaryODU1Idx);
					BSP_FPGA_LINE_MUX_ODU12_SET((ProtectLineIdx * 4) + ProtectODU1Idx, PrimaryODU1Mux);
					
					for (j = 0; j < PROFILE_ADM_ODU1_XC_CNT; j++)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1XCSetSrcIdx[j], &ODU1XCSrc))
							return;
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU1XCSetDstIdx[j], &ODU1XCDst))
							return;
						if (ODU1XCSrc == PROFILE_ADM_XC_LINE_ODU1(PrimaryLineIdx, PrimaryODU1Idx))
						{
							Target = ODU1XCDst;
							break;
						}
						if (ODU1XCDst == PROFILE_ADM_XC_LINE_ODU1(PrimaryLineIdx, PrimaryODU1Idx))
						{
							Target = ODU1XCSrc;
							break;
						}
					}
					
					if ((Target >= PROFILE_ADM_XC_CLIENT_ODU1(0)) && (Target <= PROFILE_ADM_XC_CLIENT_ODU1(15)))
					{
						Client = Target - 1025;
						BSP_FPGA_LINE_DEMUX_CLIENT_ODU1_PG_PROTECT_ODU1_SET(Client, ProtectLineIdx, ProtectODU1Idx);
						
						if (ODU1PGMode[i] == 0) /*Auto*/
						{
							if (ODU1PGRevMode[i] == 0) /*Revertive*/
								BSP_FPGA_LINE_DEMUX_CLIENT_ODU1_PG_MODE_SET(Client, 0x06);
							else
								BSP_FPGA_LINE_DEMUX_CLIENT_ODU1_PG_MODE_SET(Client, 0x04);
						}
						else if (ODU1PGMode[i] == 1) /*Lockout*/
						{
							BSP_FPGA_LINE_DEMUX_CLIENT_ODU1_PG_MODE_SET(Client, 0x00);
						}
						else if (ODU1PGMode[i] == 2) /*Forced*/
						{
							BSP_FPGA_LINE_DEMUX_CLIENT_ODU1_PG_MODE_SET(Client, 0x01);
						}
						BSP_FPGA_LINE_DEMUX_CLIENT_ODU1_PG_WTR_SET(Client, ODU1PGWTRTmr[i]);
						BSP_FPGA_LINE_DEMUX_CLIENT_ODU1_PG_HO_SET(Client, ODU1PGHOTmr[i]);
					}
				}
				break;
			case 3:
				break;
		}
	}
}

static void PROFILE_ADM_ODU0PG_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t i, j;
	uint32_t Mode;
	uint32_t PrimaryLineIdx, PrimaryODU1Idx, PrimaryODU0Idx, ProtectLineIdx, ProtectODU1Idx, ProtectODU0Idx;
	uint32_t ODU0XCSrc, ODU0XCDst;
	uint32_t PrimaryODU1Mux, ProtectODU1Mux, PrimaryODU0Mux;
	uint32_t Target, Client;
	uint32_t ODU0PGType[PROFILE_ADM_ODU0_PGS];
	uint32_t ODU0PGMode[PROFILE_ADM_ODU0_PGS];
	uint32_t ODU0PGMonMode[PROFILE_ADM_ODU0_PGS];
	uint32_t ODU0PGRevMode[PROFILE_ADM_ODU0_PGS];
	uint32_t ODU0PGPrimaryODUk[PROFILE_ADM_ODU0_PGS];
	uint32_t ODU0PGProtectODUk[PROFILE_ADM_ODU0_PGS];
	uint32_t ODU0PGWTRTmr[PROFILE_ADM_ODU0_PGS];
	uint32_t ODU0PGHOTmr[PROFILE_ADM_ODU0_PGS];
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM_SETMODE_IDX, &Mode))
		return;
	
	for (i = 0; i < PROFILE_ADM_ODU0_PGS; i++)
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetTypeIdx[i], &ODU0PGType[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetModeIdx[i], &ODU0PGMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetMonModeIdx[i], &ODU0PGMonMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetRevModeIdx[i], &ODU0PGRevMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetPrimaryODUkIdx[i], &ODU0PGPrimaryODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetProtectODUkIdx[i], &ODU0PGProtectODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetWTRTmrIdx[i], &ODU0PGWTRTmr[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0PGSetHOTmrIdx[i], &ODU0PGHOTmr[i]))
			return;
	}
	
	for (i = 0; i < BSP_FPGA_LINE_DEMUX_CLIENT_ODU0_PG_CNT; i++)
	{
		BSP_FPGA_LINE_DEMUX_CLIENT_ODU0_PG_MODE_SET(i, 0x00);
	}
	
	for (i = 0; i < PROFILE_ADM_ODU0_PGS; i++)
	{
		switch (Mode)
		{
			case 0:
				if (ODU0PGType[i] != 0)
				{
					if ((ODU0PGPrimaryODUk[i] == 0) || (ODU0PGProtectODUk[i] == 0))
						continue;
					PrimaryLineIdx = (ODU0PGPrimaryODUk[i] - 1) / 8;
					PrimaryODU1Idx = ((ODU0PGPrimaryODUk[i] - 1) % 8) / 2;
					PrimaryODU0Idx = ((ODU0PGPrimaryODUk[i] - 1) % 8) % 2;
					ProtectLineIdx = (ODU0PGProtectODUk[i] - 1) / 8;
					ProtectODU1Idx = ((ODU0PGProtectODUk[i] - 1) % 8) / 2;
					ProtectODU0Idx = ((ODU0PGProtectODUk[i] - 1) % 8) % 2;
					
					PrimaryODU1Mux = BSP_FPGA_LINE_MUX_ODU12_GET((PrimaryLineIdx * 4) + PrimaryODU1Idx);
					ProtectODU1Mux = BSP_FPGA_LINE_MUX_ODU12_GET((ProtectLineIdx * 4) + ProtectODU1Idx);
					
					PrimaryODU0Mux = BSP_FPGA_LINE_MUX_ODU01_GET(((PrimaryODU1Mux - 8) * 2) + PrimaryODU0Idx);					
					BSP_FPGA_LINE_MUX_ODU01_SET(((ProtectODU1Mux - 8) * 2) + ProtectODU0Idx, PrimaryODU0Mux);
					
					for (j = 0; j < PROFILE_ADM_ODU0_XC_CNT; j++)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0XCSetSrcIdx[j], &ODU0XCSrc))
							return;
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigODU0XCSetDstIdx[j], &ODU0XCDst))
							return;
						if (ODU0XCSrc == PROFILE_ADM_XC_LINE_ODU0(PrimaryLineIdx, (PrimaryODU1Idx*2) + PrimaryODU0Idx))
						{
							Target = ODU0XCDst;
							break;
						}
						if (ODU0XCDst == PROFILE_ADM_XC_LINE_ODU0(PrimaryLineIdx, (PrimaryODU1Idx*2) + PrimaryODU0Idx))
						{
							Target = ODU0XCSrc;
							break;
						}
					}
					
					if ((Target >= PROFILE_ADM_XC_CLIENT_ODU0(0)) && (Target <= PROFILE_ADM_XC_CLIENT_ODU0(15)))
					{
						Client = Target - 1025;
						BSP_FPGA_LINE_DEMUX_CLIENT_ODU0_PG_PROTECT_ODU0_SET(Client, ProtectLineIdx, (ProtectODU1Idx*2) + ProtectODU0Idx);
						
						if (ODU0PGMode[i] == 0) /*Auto*/
						{
							if (ODU0PGRevMode[i] == 0) /*Revertive*/
								BSP_FPGA_LINE_DEMUX_CLIENT_ODU0_PG_MODE_SET(Client, 0x06);
							else
								BSP_FPGA_LINE_DEMUX_CLIENT_ODU0_PG_MODE_SET(Client, 0x04);
						}
						else if (ODU0PGMode[i] == 1) /*Lockout*/
						{
							BSP_FPGA_LINE_DEMUX_CLIENT_ODU0_PG_MODE_SET(Client, 0x00);
						}
						else if (ODU0PGMode[i] == 2) /*Forced*/
						{
							BSP_FPGA_LINE_DEMUX_CLIENT_ODU0_PG_MODE_SET(Client, 0x01);
						}
						BSP_FPGA_LINE_DEMUX_CLIENT_ODU0_PG_WTR_SET(Client, ODU0PGWTRTmr[i]);
						BSP_FPGA_LINE_DEMUX_CLIENT_ODU0_PG_HO_SET(Client, ODU0PGHOTmr[i]);
					}
				}
				break;
			case 3:
				break;
		}
	}
}

void PROFILE_ADM_PG_UpdateConfigs(PROFILE_T *Profile)
{
	PROFILE_ADM_ODU2PG_UpdateConfigs(Profile);
	PROFILE_ADM_ODU1PG_UpdateConfigs(Profile);
	PROFILE_ADM_ODU0PG_UpdateConfigs(Profile);
}

void PROFILE_ADM_PG_UpdateValues(PROFILE_T *Profile)
{
	
}

