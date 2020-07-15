#include "profile_adm10_client.h"
#include "profile_adm10_line.h"
#include "profile_adm10_xc.h"
#include "profile_adm10_pg.h"
#include "profile_adm10_enums.h"
#include "profile_adm10_bitmsks.h"

const uint16_t ProfileADM10DynamicODU2PGStateIdx[PROFILE_ADM10_ODU2_PGS] = {
	PROFILE_ADM10_ODU2_PG_1_STATE_IDX, PROFILE_ADM10_ODU2_PG_2_STATE_IDX,
};

const uint16_t ProfileADM10DynamicODU2PGActiveODUkIdx[PROFILE_ADM10_ODU2_PGS] = {
	PROFILE_ADM10_ODU2_PG_1_ACTIVEODUK_IDX, PROFILE_ADM10_ODU2_PG_2_ACTIVEODUK_IDX,
};

const uint16_t ProfileADM10ConfigODU2PGSetTypeIdx[PROFILE_ADM10_ODU2_PGS] = {
	PROFILE_ADM10_ODU2_PG_1_SETTYPE_IDX, PROFILE_ADM10_ODU2_PG_2_SETTYPE_IDX,
};

const uint16_t ProfileADM10ConfigODU2PGSetModeIdx[PROFILE_ADM10_ODU2_PGS] = {
	PROFILE_ADM10_ODU2_PG_1_SETMODE_IDX, PROFILE_ADM10_ODU2_PG_2_SETMODE_IDX,
};

const uint16_t ProfileADM10ConfigODU2PGSetMonModeIdx[PROFILE_ADM10_ODU2_PGS] = {
	PROFILE_ADM10_ODU2_PG_1_SETMONMODE_IDX, PROFILE_ADM10_ODU2_PG_2_SETMONMODE_IDX,
};

const uint16_t ProfileADM10ConfigODU2PGSetRevModeIdx[PROFILE_ADM10_ODU2_PGS] = {
	PROFILE_ADM10_ODU2_PG_1_SETREVMODE_IDX, PROFILE_ADM10_ODU2_PG_2_SETREVMODE_IDX,
};

const uint16_t ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[PROFILE_ADM10_ODU2_PGS] = {
	PROFILE_ADM10_ODU2_PG_1_SETPRIMARYODUK_IDX, PROFILE_ADM10_ODU2_PG_2_SETPRIMARYODUK_IDX,
};

const uint16_t ProfileADM10ConfigODU2PGSetProtectODUkIdx[PROFILE_ADM10_ODU2_PGS] = {
	PROFILE_ADM10_ODU2_PG_1_SETPROTECTODUK_IDX, PROFILE_ADM10_ODU2_PG_2_SETPROTECTODUK_IDX,
};

const uint16_t ProfileADM10ConfigODU2PGSetWTRTmrIdx[PROFILE_ADM10_ODU2_PGS] = {
	PROFILE_ADM10_ODU2_PG_1_SETWTRTMR_IDX, PROFILE_ADM10_ODU2_PG_2_SETWTRTMR_IDX,
};

const uint16_t ProfileADM10ConfigODU2PGSetHOTmrIdx[PROFILE_ADM10_ODU2_PGS] = {
	PROFILE_ADM10_ODU2_PG_1_SETHOTMR_IDX, PROFILE_ADM10_ODU2_PG_2_SETHOTMR_IDX,
};

const uint16_t ProfileADM10ConfigODU2PGSetFactoryIdx[PROFILE_ADM10_ODU2_PGS] = {
	PROFILE_ADM10_ODU2_PG_1_SETFACTORY_IDX, PROFILE_ADM10_ODU2_PG_2_SETFACTORY_IDX,
};

const uint16_t ProfileADM10DynamicODU1PGStateIdx[PROFILE_ADM10_ODU1_PGS] = {
	PROFILE_ADM10_ODU1_PG_1_STATE_IDX, PROFILE_ADM10_ODU1_PG_2_STATE_IDX, PROFILE_ADM10_ODU1_PG_3_STATE_IDX, PROFILE_ADM10_ODU1_PG_4_STATE_IDX,
};

const uint16_t ProfileADM10DynamicODU1PGActiveODUkIdx[PROFILE_ADM10_ODU1_PGS] = {
	PROFILE_ADM10_ODU1_PG_1_ACTIVEODUK_IDX, PROFILE_ADM10_ODU1_PG_2_ACTIVEODUK_IDX, PROFILE_ADM10_ODU1_PG_3_ACTIVEODUK_IDX, PROFILE_ADM10_ODU1_PG_4_ACTIVEODUK_IDX,
};

const uint16_t ProfileADM10ConfigODU1PGSetTypeIdx[PROFILE_ADM10_ODU1_PGS] = {
	PROFILE_ADM10_ODU1_PG_1_SETTYPE_IDX, PROFILE_ADM10_ODU1_PG_2_SETTYPE_IDX, PROFILE_ADM10_ODU1_PG_3_SETTYPE_IDX, PROFILE_ADM10_ODU1_PG_4_SETTYPE_IDX,
};

const uint16_t ProfileADM10ConfigODU1PGSetModeIdx[PROFILE_ADM10_ODU1_PGS] = {
	PROFILE_ADM10_ODU1_PG_1_SETMODE_IDX, PROFILE_ADM10_ODU1_PG_2_SETMODE_IDX, PROFILE_ADM10_ODU1_PG_3_SETMODE_IDX, PROFILE_ADM10_ODU1_PG_4_SETMODE_IDX,
};

const uint16_t ProfileADM10ConfigODU1PGSetMonModeIdx[PROFILE_ADM10_ODU1_PGS] = {
	PROFILE_ADM10_ODU1_PG_1_SETMONMODE_IDX, PROFILE_ADM10_ODU1_PG_2_SETMONMODE_IDX, PROFILE_ADM10_ODU1_PG_3_SETMONMODE_IDX, PROFILE_ADM10_ODU1_PG_4_SETMONMODE_IDX,
};

const uint16_t ProfileADM10ConfigODU1PGSetRevModeIdx[PROFILE_ADM10_ODU1_PGS] = {
	PROFILE_ADM10_ODU1_PG_1_SETREVMODE_IDX, PROFILE_ADM10_ODU1_PG_2_SETREVMODE_IDX, PROFILE_ADM10_ODU1_PG_3_SETREVMODE_IDX, PROFILE_ADM10_ODU1_PG_4_SETREVMODE_IDX,
};

const uint16_t ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[PROFILE_ADM10_ODU1_PGS] = {
	PROFILE_ADM10_ODU1_PG_1_SETPRIMARYODUK_IDX, PROFILE_ADM10_ODU1_PG_2_SETPRIMARYODUK_IDX, PROFILE_ADM10_ODU1_PG_3_SETPRIMARYODUK_IDX, PROFILE_ADM10_ODU1_PG_4_SETPRIMARYODUK_IDX,
};

const uint16_t ProfileADM10ConfigODU1PGSetProtectODUkIdx[PROFILE_ADM10_ODU1_PGS] = {
	PROFILE_ADM10_ODU1_PG_1_SETPROTECTODUK_IDX, PROFILE_ADM10_ODU1_PG_2_SETPROTECTODUK_IDX, PROFILE_ADM10_ODU1_PG_3_SETPROTECTODUK_IDX, PROFILE_ADM10_ODU1_PG_4_SETPROTECTODUK_IDX,
};

const uint16_t ProfileADM10ConfigODU1PGSetWTRTmrIdx[PROFILE_ADM10_ODU1_PGS] = {
	PROFILE_ADM10_ODU1_PG_1_SETWTRTMR_IDX, PROFILE_ADM10_ODU1_PG_2_SETWTRTMR_IDX, PROFILE_ADM10_ODU1_PG_3_SETWTRTMR_IDX, PROFILE_ADM10_ODU1_PG_4_SETWTRTMR_IDX,
};

const uint16_t ProfileADM10ConfigODU1PGSetHOTmrIdx[PROFILE_ADM10_ODU1_PGS] = {
	PROFILE_ADM10_ODU1_PG_1_SETHOTMR_IDX, PROFILE_ADM10_ODU1_PG_2_SETHOTMR_IDX, PROFILE_ADM10_ODU1_PG_3_SETHOTMR_IDX, PROFILE_ADM10_ODU1_PG_4_SETHOTMR_IDX,
};

const uint16_t ProfileADM10ConfigODU1PGSetFactoryIdx[PROFILE_ADM10_ODU1_PGS] = {
	PROFILE_ADM10_ODU1_PG_1_SETFACTORY_IDX, PROFILE_ADM10_ODU1_PG_2_SETFACTORY_IDX, PROFILE_ADM10_ODU1_PG_3_SETFACTORY_IDX, PROFILE_ADM10_ODU1_PG_4_SETFACTORY_IDX,
};

const uint16_t ProfileADM10DynamicODU0PGStateIdx[PROFILE_ADM10_ODU0_PGS] = {
	PROFILE_ADM10_ODU0_PG_1_STATE_IDX, PROFILE_ADM10_ODU0_PG_2_STATE_IDX, PROFILE_ADM10_ODU0_PG_3_STATE_IDX, PROFILE_ADM10_ODU0_PG_4_STATE_IDX,
	PROFILE_ADM10_ODU0_PG_5_STATE_IDX, PROFILE_ADM10_ODU0_PG_6_STATE_IDX, PROFILE_ADM10_ODU0_PG_7_STATE_IDX, PROFILE_ADM10_ODU0_PG_8_STATE_IDX,
};

const uint16_t ProfileADM10DynamicODU0PGActiveODUkIdx[PROFILE_ADM10_ODU0_PGS] = {
	PROFILE_ADM10_ODU0_PG_1_ACTIVEODUK_IDX, PROFILE_ADM10_ODU0_PG_2_ACTIVEODUK_IDX, PROFILE_ADM10_ODU0_PG_3_ACTIVEODUK_IDX, PROFILE_ADM10_ODU0_PG_4_ACTIVEODUK_IDX,
	PROFILE_ADM10_ODU0_PG_5_ACTIVEODUK_IDX, PROFILE_ADM10_ODU0_PG_6_ACTIVEODUK_IDX, PROFILE_ADM10_ODU0_PG_7_ACTIVEODUK_IDX, PROFILE_ADM10_ODU0_PG_8_ACTIVEODUK_IDX,
};

const uint16_t ProfileADM10ConfigODU0PGSetTypeIdx[PROFILE_ADM10_ODU0_PGS] = {
	PROFILE_ADM10_ODU0_PG_1_SETTYPE_IDX, PROFILE_ADM10_ODU0_PG_2_SETTYPE_IDX, PROFILE_ADM10_ODU0_PG_3_SETTYPE_IDX, PROFILE_ADM10_ODU0_PG_4_SETTYPE_IDX,
	PROFILE_ADM10_ODU0_PG_5_SETTYPE_IDX, PROFILE_ADM10_ODU0_PG_6_SETTYPE_IDX, PROFILE_ADM10_ODU0_PG_7_SETTYPE_IDX, PROFILE_ADM10_ODU0_PG_8_SETTYPE_IDX,
};

const uint16_t ProfileADM10ConfigODU0PGSetModeIdx[PROFILE_ADM10_ODU0_PGS] = {
	PROFILE_ADM10_ODU0_PG_1_SETMODE_IDX, PROFILE_ADM10_ODU0_PG_2_SETMODE_IDX, PROFILE_ADM10_ODU0_PG_3_SETMODE_IDX, PROFILE_ADM10_ODU0_PG_4_SETMODE_IDX,
	PROFILE_ADM10_ODU0_PG_5_SETMODE_IDX, PROFILE_ADM10_ODU0_PG_6_SETMODE_IDX, PROFILE_ADM10_ODU0_PG_7_SETMODE_IDX, PROFILE_ADM10_ODU0_PG_8_SETMODE_IDX,
};

const uint16_t ProfileADM10ConfigODU0PGSetMonModeIdx[PROFILE_ADM10_ODU0_PGS] = {
	PROFILE_ADM10_ODU0_PG_1_SETMONMODE_IDX, PROFILE_ADM10_ODU0_PG_2_SETMONMODE_IDX, PROFILE_ADM10_ODU0_PG_3_SETMONMODE_IDX, PROFILE_ADM10_ODU0_PG_4_SETMONMODE_IDX,
	PROFILE_ADM10_ODU0_PG_5_SETMONMODE_IDX, PROFILE_ADM10_ODU0_PG_6_SETMONMODE_IDX, PROFILE_ADM10_ODU0_PG_7_SETMONMODE_IDX, PROFILE_ADM10_ODU0_PG_8_SETMONMODE_IDX,
};

const uint16_t ProfileADM10ConfigODU0PGSetRevModeIdx[PROFILE_ADM10_ODU0_PGS] = {
	PROFILE_ADM10_ODU0_PG_1_SETREVMODE_IDX, PROFILE_ADM10_ODU0_PG_2_SETREVMODE_IDX, PROFILE_ADM10_ODU0_PG_3_SETREVMODE_IDX, PROFILE_ADM10_ODU0_PG_4_SETREVMODE_IDX,
	PROFILE_ADM10_ODU0_PG_5_SETREVMODE_IDX, PROFILE_ADM10_ODU0_PG_6_SETREVMODE_IDX, PROFILE_ADM10_ODU0_PG_7_SETREVMODE_IDX, PROFILE_ADM10_ODU0_PG_8_SETREVMODE_IDX,
};

const uint16_t ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[PROFILE_ADM10_ODU0_PGS] = {
	PROFILE_ADM10_ODU0_PG_1_SETPRIMARYODUK_IDX, PROFILE_ADM10_ODU0_PG_2_SETPRIMARYODUK_IDX, PROFILE_ADM10_ODU0_PG_3_SETPRIMARYODUK_IDX, PROFILE_ADM10_ODU0_PG_4_SETPRIMARYODUK_IDX,
	PROFILE_ADM10_ODU0_PG_5_SETPRIMARYODUK_IDX, PROFILE_ADM10_ODU0_PG_6_SETPRIMARYODUK_IDX, PROFILE_ADM10_ODU0_PG_7_SETPRIMARYODUK_IDX, PROFILE_ADM10_ODU0_PG_8_SETPRIMARYODUK_IDX,
};

const uint16_t ProfileADM10ConfigODU0PGSetProtectODUkIdx[PROFILE_ADM10_ODU0_PGS] = {
	PROFILE_ADM10_ODU0_PG_1_SETPROTECTODUK_IDX, PROFILE_ADM10_ODU0_PG_2_SETPROTECTODUK_IDX, PROFILE_ADM10_ODU0_PG_3_SETPROTECTODUK_IDX, PROFILE_ADM10_ODU0_PG_4_SETPROTECTODUK_IDX,
	PROFILE_ADM10_ODU0_PG_5_SETPROTECTODUK_IDX, PROFILE_ADM10_ODU0_PG_6_SETPROTECTODUK_IDX, PROFILE_ADM10_ODU0_PG_7_SETPROTECTODUK_IDX, PROFILE_ADM10_ODU0_PG_8_SETPROTECTODUK_IDX,
};

const uint16_t ProfileADM10ConfigODU0PGSetWTRTmrIdx[PROFILE_ADM10_ODU0_PGS] = {
	PROFILE_ADM10_ODU0_PG_1_SETWTRTMR_IDX, PROFILE_ADM10_ODU0_PG_2_SETWTRTMR_IDX, PROFILE_ADM10_ODU0_PG_3_SETWTRTMR_IDX, PROFILE_ADM10_ODU0_PG_4_SETWTRTMR_IDX,
	PROFILE_ADM10_ODU0_PG_5_SETWTRTMR_IDX, PROFILE_ADM10_ODU0_PG_6_SETWTRTMR_IDX, PROFILE_ADM10_ODU0_PG_7_SETWTRTMR_IDX, PROFILE_ADM10_ODU0_PG_8_SETWTRTMR_IDX,
};

const uint16_t ProfileADM10ConfigODU0PGSetHOTmrIdx[PROFILE_ADM10_ODU0_PGS] = {
	PROFILE_ADM10_ODU0_PG_1_SETHOTMR_IDX, PROFILE_ADM10_ODU0_PG_2_SETHOTMR_IDX, PROFILE_ADM10_ODU0_PG_3_SETHOTMR_IDX, PROFILE_ADM10_ODU0_PG_4_SETHOTMR_IDX,
	PROFILE_ADM10_ODU0_PG_5_SETHOTMR_IDX, PROFILE_ADM10_ODU0_PG_6_SETHOTMR_IDX, PROFILE_ADM10_ODU0_PG_7_SETHOTMR_IDX, PROFILE_ADM10_ODU0_PG_8_SETHOTMR_IDX,
};

const uint16_t ProfileADM10ConfigODU0PGSetFactoryIdx[PROFILE_ADM10_ODU0_PGS] = {
	PROFILE_ADM10_ODU0_PG_1_SETFACTORY_IDX, PROFILE_ADM10_ODU0_PG_2_SETFACTORY_IDX, PROFILE_ADM10_ODU0_PG_3_SETFACTORY_IDX, PROFILE_ADM10_ODU0_PG_4_SETFACTORY_IDX,
	PROFILE_ADM10_ODU0_PG_5_SETFACTORY_IDX, PROFILE_ADM10_ODU0_PG_6_SETFACTORY_IDX, PROFILE_ADM10_ODU0_PG_7_SETFACTORY_IDX, PROFILE_ADM10_ODU0_PG_8_SETFACTORY_IDX,
};

int32_t PROFILE_ADM10_PG_CheckConfigODU2PGSetTypeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 ODU2PG 1, 2 type is always Disabled*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_DISABLED)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS type is always Unidirerctional 1+1*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_UNIDIRECTIONAL_1_1)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC type is always Disabled*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_DISABLED)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (ODU2PGIdx < 1)
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 1 type is always Unidirerctional 1+1*/
				if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_UNIDIRECTIONAL_1_1)
					return -1;
			}
			else
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 2 type is always Disabled*/
				if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_DISABLED)
					return -1;
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU2PGSetModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 ODU2PG 1, 2 mode is always Auto*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_AUTO)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS mode is Auto, Lockout, Forced or Manual*/
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC mode is always Auto*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_AUTO)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (ODU2PGIdx < 1)
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 1 mode is always Auto, Lockout, Forced or Manual*/
			}
			else
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 2 mode is always Auto*/
				if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_AUTO)
					return -1;
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU2PGSetMonModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 ODU2PG 1, 2 monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_MON_MODE_ENUM_SNC_NE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_MON_MODE_ENUM_SNC_NE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC monitor mode is always SNCP/Ne*/
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			/*In AGG-10-BS+ADM-2.5 ODU2PG 1, 2 monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_MON_MODE_ENUM_SNC_NE)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU2PGSetRevModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 ODU2PG 1, 2 revert mode is always Revertive*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_REV_MODE_ENUM_REVERTIVE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS revert mode is Revertive or Non-Revertive*/
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC revert mode is always Revertive*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_REV_MODE_ENUM_REVERTIVE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (ODU2PGIdx < 1)
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 1 revert mode is Revertive or Non-Revertive*/
			}
			else
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 2 revert mode is always Revertive*/
				if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_REV_MODE_ENUM_REVERTIVE)
					return -1;
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU2PGSetPrimaryODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 ODU2PG 1, 2 Primary ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PRIMARY_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS for PG 1 Primary ODUk is always Ln_1_ODU2*/
			if ((ODU2PGIdx == 0) && (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PRIMARY_ODUK_ENUM_LN_1_ODU2))
				return -1;
			/*In AGG-2x10-BS for PG 2 Primary ODUk is always Ln_2_ODU2*/
			else if ((ODU2PGIdx == 1) && (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PRIMARY_ODUK_ENUM_LN_2_ODU2))
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC Primary ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PRIMARY_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (ODU2PGIdx < 1)
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 1 Primary ODUk is always Ln_1_ODU2*/
				if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PRIMARY_ODUK_ENUM_LN_1_ODU2)
					return -1;
			}
			else
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 2 Primary ODUk is always None*/
				if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PRIMARY_ODUK_ENUM_NONE)
					return -1;
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU2PGSetProtectODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 ODU2PG 1, 2 Protect ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PROTECT_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS for PG 1 Protect ODUk is always Ln_3_ODU2*/
			if ((ODU2PGIdx == 0) && (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PROTECT_ODUK_ENUM_LN_3_ODU2))
				return -1;
			/*In AGG-2x10-BS for PG 2 Protect ODUk is always Ln_4_ODU2*/
			else if ((ODU2PGIdx == 1) && (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PROTECT_ODUK_ENUM_LN_4_ODU2))
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC Protect ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PROTECT_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (ODU2PGIdx < 1)
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 1 Protect ODUk is always Ln_1_ODU2*/
				if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PROTECT_ODUK_ENUM_LN_3_ODU2)
					return -1;
			}
			else
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 2 Protect ODUk is always None*/
				if (*(uint32_t*)Value != PROFILE_ADM10_ODU2_PG_SET_PROTECT_ODUK_ENUM_NONE)
					return -1;
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU2PGSetWTRTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 ODU2PG 1, 2 WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS WTR timer grid is 100ms*/
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (ODU2PGIdx < 1)
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 1 WTR timer is updatable*/
			}
			else
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 2 WTR timer is always 0*/
				if (*(uint32_t*)Value != 0)
					return -1;
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU2PGSetHOTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 HO timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS HO timer grid is 100ms*/
			if ((*(uint32_t*)Value % 100))
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (ODU2PGIdx < 1)
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 1 HO timer grid is 100ms*/
				if ((*(uint32_t*)Value % 100))
					return -1;
			}
			else
			{
				/*In AGG-10-BS+ADM-2.5 ODU2PG 2 HO timer is always 0*/
				if (*(uint32_t*)Value != 0)
					return -1;
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU1PGSetTypeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 type is Disabled or Unidirerctional 1+1*/
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS type is always Disabled*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_TYPE_ENUM_DISABLED)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC type is always Disabled*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_TYPE_ENUM_DISABLED)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			/*In AGG-10-BS+ADM-2.5 type is Disabled or Unidirerctional 1+1*/
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU1PGSetModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 mode is Auto, Lockout, Forced or Manual*/
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS mode is always Auto*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_AUTO)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC mode is always Auto*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_AUTO)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			/*In AGG-10-BS+ADM-2.5 mode is Auto, Lockout, Forced or Manual*/
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU1PGSetMonModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_MON_MODE_ENUM_SNC_NE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_MON_MODE_ENUM_SNC_NE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_MON_MODE_ENUM_SNC_NE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			/*In AGG-10-BS+ADM-2.5 monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_MON_MODE_ENUM_SNC_NE)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU1PGSetRevModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 revert mode is Revertive or Non-Revertive*/
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS revert mode is always Revertive*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_REV_MODE_ENUM_REVERTIVE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC revert mode is always Revertive*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_REV_MODE_ENUM_REVERTIVE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			/*In AGG-10-BS+ADM-2.5 revert mode is Revertive or Non-Revertive*/
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU1PGSetPrimaryODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t PrimaryODUk, ProtectODUk;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_NONE)
			{
				/*In ADM-10 always allow to set PrimaryODUk to None */
				return 0;
			}
			else if ((*(uint32_t*)Value > PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_LN_1_ODU1_1) &&
					(*(uint32_t*)Value < PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_LN_2_ODU1_4))
			{
				/*In ADM-10 allow to set PrimaryODUk to Line 1, 2 ODU1 1, 2, 3, 4 */
			}
			else
			{
				return -1;
			}
			
			/*In ADM-10 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM10_ODU1_PGS; i++)
			{
				if (i != ODU1PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], &PrimaryODUk))
						return -1;
					if (*(uint32_t*)Value == PrimaryODUk)
						return -1;
				}
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], &ProtectODUk))
					return -1;
				if (*(uint32_t*)Value == ProtectODUk)
					return -1;
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS Primary ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC Primary ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_NONE)
			{
				/*In AGG-10-BS+ADM-2.5 always allow to set PrimaryODUk to None */
				return 0;
			}
			else if ((*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU1) ||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_CL_11_ODU1) ||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_CL_13_ODU1) ||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_CL_15_ODU1))
			{
				/*In AGG-10-BS+ADM-2.5 allow to set PrimaryODUk to Client 9, 11, 13, 15 ODU1 */
			}
			else
			{
				return -1;
			}
			
			/*In AGG-10-BS+ADM-2.5 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM10_ODU1_PGS; i++)
			{
				if (i != ODU1PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], &PrimaryODUk))
						return -1;
					if (*(uint32_t*)Value == PrimaryODUk)
						return -1;
				}
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], &ProtectODUk))
					return -1;
				if (*(uint32_t*)Value == ProtectODUk)
					return -1;
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU1PGSetProtectODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t PrimaryODUk, ProtectODUk;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_NONE)
			{
				/*In ADM-10 always allow to set ProtectODUk to None */
				return 0;
			}
			else if ((*(uint32_t*)Value > PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_LN_1_ODU1_1) &&
					(*(uint32_t*)Value < PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_LN_2_ODU1_4))
			{
				/*In ADM-10 allow to set ProtectODUk to Line 1, 2 ODU1 1, 2, 3, 4 */
			}
			else
			{
				return -1;
			}
			
			/*In ADM-10 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM10_ODU1_PGS; i++)
			{
				
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], &PrimaryODUk))
					return -1;
				if (*(uint32_t*)Value == PrimaryODUk)
					return -1;
				if (i != ODU1PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], &ProtectODUk))
						return -1;
					if (*(uint32_t*)Value == ProtectODUk)
						return -1;
				}
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS Protect ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_PROTECT_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC Protect ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU1_PG_SET_PROTECT_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_NONE)
			{
				/*In AGG-10-BS+ADM-2.5 always allow to set ProtectODUk to None */
				return 0;
			}
			else if ((*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU1) ||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_CL_11_ODU1) ||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_CL_13_ODU1) ||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_CL_15_ODU1))
			{
				/*In AGG-10-BS+ADM-2.5 allow to set ProtectODUk to Client 9, 11, 13, 15 ODU1 */
			}
			else
			{
				return -1;
			}
			
			/*In AGG-10-BS+ADM-2.5 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM10_ODU1_PGS; i++)
			{
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], &PrimaryODUk))
					return -1;
				if (*(uint32_t*)Value == PrimaryODUk)
					return -1;
				if (i != ODU1PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], &ProtectODUk))
						return -1;
					if (*(uint32_t*)Value == ProtectODUk)
						return -1;
				}
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU1PGSetWTRTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU1PGSetHOTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 HO timer grid is 100ms*/
			if ((*(uint32_t*)Value % 100))
				return -1;	
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			/*In AGG-10-BS+ADM-2.5 HO timer grid is 100ms*/
			if ((*(uint32_t*)Value % 100))
				return -1;	
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU0PGSetTypeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 type is Disabled or Unidirerctional 1+1*/
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS type is always Disabled*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_TYPE_ENUM_DISABLED)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-BS type is always Disabled*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_TYPE_ENUM_DISABLED)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			/*In AGG-10-BS+ADM-2.5 type is Disabled or Unidirerctional 1+1*/
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU0PGSetModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 mode is Auto, Lockout, Forced or Manual*/
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS mode is always Auto*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_AUTO)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC mode is always Auto*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_AUTO)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			/*In AGG-10-BS+ADM-2.5 mode is Auto, Lockout, Forced or Manual*/
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU0PGSetMonModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_MON_MODE_ENUM_SNC_NE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_MON_MODE_ENUM_SNC_NE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_MON_MODE_ENUM_SNC_NE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			/*In AGG-10-BS+ADM-2.5 monitor mode is always SNCP/Ne*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_MON_MODE_ENUM_SNC_NE)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU0PGSetRevModeValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 revert mode is Revertive or Non-Revertive*/
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS revert mode is always Revertive*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_REV_MODE_ENUM_REVERTIVE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC revert mode is always Revertive*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_REV_MODE_ENUM_REVERTIVE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			/*In AGG-10-BS+ADM-2.5 revert mode is Revertive or Non-Revertive*/
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU0PGSetPrimaryODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t PrimaryODUk, ProtectODUk;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_NONE)
			{
				/*In ADM-10 always allow to set PrimaryODUk to None */
				return 0;
			}
			else if ((*(uint32_t*)Value > PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_LN_1_ODU0_1) &&
					(*(uint32_t*)Value < PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_LN_2_ODU0_8))
			{
				/*In ADM-10 allow to set PrimaryODUk to Line 1, 2 ODU1 1, 2, ..., 8 */
			}
			else
			{
				return -1;
			}
			
			/*In ADM-10 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM10_ODU0_PGS; i++)
			{
				if (i != ODU0PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i], &PrimaryODUk))
						return -1;
					if (*(uint32_t*)Value == PrimaryODUk)
						return -1;
				}
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[i], &ProtectODUk))
					return -1;
				if (*(uint32_t*)Value == ProtectODUk)
					return -1;
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS Primary ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC Primary ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_NONE)
			{
				/*In AGG-10-BS+ADM-2.5 always allow to set PrimaryODUk to None */
				return 0;
			}
			else if ((*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU0_1) ||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU0_2)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_11_ODU0_1)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_11_ODU0_2)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_13_ODU0_1)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_13_ODU0_2)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_15_ODU0_1)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_15_ODU0_2))
			{
				/*In AGG-10-BS+ADM-2.5 allow to set PrimaryODUk to Client 9, 11, 13, 15 ODU0 1, 2 */
			}
			else
			{
				return -1;
			}
			
			/*In AGG-10-BS+ADM-2.5 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM10_ODU0_PGS; i++)
			{
				if (i != ODU0PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i], &PrimaryODUk))
						return -1;
					if (*(uint32_t*)Value == PrimaryODUk)
						return -1;
				}
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[i], &ProtectODUk))
					return -1;
				if (*(uint32_t*)Value == ProtectODUk)
					return -1;
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU0PGSetProtectODUkValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t PrimaryODUk, ProtectODUk;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_NONE)
			{
				/*In ADM-10 always allow to set ProtectODUk to None */
				return 0;
			}
			else if ((*(uint32_t*)Value > PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_LN_1_ODU0_1) &&
					(*(uint32_t*)Value < PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_LN_2_ODU0_8))
			{
				/*In ADM-10 allow to set ProtectODUk to Line 1, 2 ODU1 1, 2, ..., 8 */
			}
			else
			{
				return -1;
			}
			
			/*In ADM-10 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM10_ODU0_PGS; i++)
			{
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i], &PrimaryODUk))
					return -1;
				if (*(uint32_t*)Value == PrimaryODUk)
					return -1;
				if (i != ODU0PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[i], &ProtectODUk))
						return -1;
					if (*(uint32_t*)Value == ProtectODUk)
						return -1;
				}
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS Protect ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_PROTECT_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC Protect ODUk is always None*/
			if (*(uint32_t*)Value != PROFILE_ADM10_ODU0_PG_SET_PROTECT_ODUK_ENUM_NONE)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_NONE)
			{
				/*In AGG-10-BS+ADM-2.5 always allow to set ProtectODUk to None */
				return 0;
			}
			else if ((*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU0_1) ||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU0_2)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_11_ODU0_1)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_11_ODU0_2)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_13_ODU0_1)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_13_ODU0_2)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_15_ODU0_1)||
					(*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_15_ODU0_2))
			{
				/*In AGG-10-BS+ADM-2.5 allow to set ProtectODUk to Client 9, 11, 13, 15 ODU0 1, 2 */
			}
			else
			{
				return -1;
			}
			
			/*In AGG-10-BS+ADM-2.5 ODUk could be used only once as PrimaryODUk or ProtectODUk*/
			for (i = 0; i < PROFILE_ADM10_ODU0_PGS; i++)
			{
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i], &PrimaryODUk))
					return -1;
				if (*(uint32_t*)Value == PrimaryODUk)
					return -1;
				
				if (i != ODU0PGIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[i], &ProtectODUk))
						return -1;
					if (*(uint32_t*)Value == ProtectODUk)
						return -1;
				}				
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU0PGSetWTRTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigODU0PGSetHOTmrValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			/*In ADM-10 HO timer grid is 100ms*/
			if ((*(uint32_t*)Value % 100))
				return -1;	
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			/*In AGG-2x10-BS WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			/*In AGG-2x10-EFEC WTR timer is always 0*/
			if (*(uint32_t*)Value != 0)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			/*In AGG-10-BS+ADM-2.5 HO timer grid is 100ms*/
			if ((*(uint32_t*)Value % 100))
				return -1;	
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_PG_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM10_ODU2_PGS; i++)
	{
		if (ProfileADM10ConfigODU2PGSetTypeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU2PGSetTypeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU2PGSetModeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU2PGSetModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU2PGSetMonModeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU2PGSetMonModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU2PGSetRevModeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU2PGSetRevModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU2PGSetPrimaryODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU2PGSetProtectODUkIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU2PGSetProtectODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU2PGSetWTRTmrIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU2PGSetWTRTmrValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU2PGSetHOTmrIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU2PGSetHOTmrValue(Profile, Table, TableIdx, Value, i);
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU1_PGS; i++)
	{
		if (ProfileADM10ConfigODU1PGSetTypeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU1PGSetTypeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU1PGSetModeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU1PGSetModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU1PGSetMonModeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU1PGSetMonModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU1PGSetRevModeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU1PGSetRevModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU1PGSetPrimaryODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU1PGSetProtectODUkIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU1PGSetProtectODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU1PGSetWTRTmrIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU1PGSetWTRTmrValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU1PGSetHOTmrIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU1PGSetHOTmrValue(Profile, Table, TableIdx, Value, i);
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU0_PGS; i++)
	{
		if (ProfileADM10ConfigODU0PGSetTypeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU0PGSetTypeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU0PGSetModeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU0PGSetModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU0PGSetMonModeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU0PGSetMonModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU0PGSetRevModeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU0PGSetRevModeValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU0PGSetPrimaryODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU0PGSetProtectODUkIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU0PGSetProtectODUkValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU0PGSetWTRTmrIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU0PGSetWTRTmrValue(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU0PGSetHOTmrIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_CheckConfigODU0PGSetHOTmrValue(Profile, Table, TableIdx, Value, i);
	}
	
	return 0;
}

static const uint32_t ODU2PGSetTypeFactory[PROFILE_ADM10_ODU2_PGS] = {PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_UNIDIRECTIONAL_1_1, PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_UNIDIRECTIONAL_1_1}; /*Unidirectional 1+1, Unidirectional 1+1*/
static const uint32_t ODU2PGSetModeFactory[PROFILE_ADM10_ODU2_PGS] = {PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_AUTO, PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_AUTO}; /*Auto, Auto*/
static const uint32_t ODU2PGSetMonModeFactory[PROFILE_ADM10_ODU2_PGS] = {PROFILE_ADM10_ODU2_PG_SET_MON_MODE_ENUM_SNC_NE, PROFILE_ADM10_ODU2_PG_SET_MON_MODE_ENUM_SNC_NE}; /*SNC/Ne, SNC/Ne*/
static const uint32_t ODU2PGSetRevModeFactory[PROFILE_ADM10_ODU2_PGS] = {PROFILE_ADM10_ODU2_PG_SET_REV_MODE_ENUM_REVERTIVE, PROFILE_ADM10_ODU2_PG_SET_REV_MODE_ENUM_REVERTIVE}; /*Revertive, Revertive*/
static const uint32_t ODU2PGSetPrimaryODUkFactory[PROFILE_ADM10_ODU2_PGS] = {PROFILE_ADM10_ODU2_PG_SET_PRIMARY_ODUK_ENUM_LN_1_ODU2, PROFILE_ADM10_ODU2_PG_SET_PRIMARY_ODUK_ENUM_LN_2_ODU2}; /*Line_1_ODU2, Line_2_ODU2*/
static const uint32_t ODU2PGSetProtectODUkFactory[PROFILE_ADM10_ODU2_PGS] = {PROFILE_ADM10_ODU2_PG_SET_PROTECT_ODUK_ENUM_LN_3_ODU2, PROFILE_ADM10_ODU2_PG_SET_PROTECT_ODUK_ENUM_LN_4_ODU2}; /*Line_3_ODU2, Line_4_ODU2*/
static const uint32_t ODU2PGSetWTRTmrFactory[PROFILE_ADM10_ODU2_PGS] = {0, 0};
static const uint32_t ODU2PGSetHOTmrFactory[PROFILE_ADM10_ODU2_PGS] = {0, 0};

static int32_t PROFILE_ADM10_ODU2PG_SetFactoryValue(PROFILE_T *Profile, uint32_t Mode, uint32_t ODU2PGIdx)
{
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[ODU2PGIdx]);
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:			
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetTypeFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetModeFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetMonModeFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetRevModeFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetPrimaryODUkFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetProtectODUkFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetWTRTmrFactory[ODU2PGIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetHOTmrFactory[ODU2PGIdx]);
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[ODU2PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[ODU2PGIdx]);
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			if (ODU2PGIdx < 1)
			{
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetTypeFactory[ODU2PGIdx]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetModeFactory[ODU2PGIdx]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetMonModeFactory[ODU2PGIdx]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetRevModeFactory[ODU2PGIdx]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetPrimaryODUkFactory[ODU2PGIdx]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetProtectODUkFactory[ODU2PGIdx]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetWTRTmrFactory[ODU2PGIdx]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[ODU2PGIdx], (uint32_t*)&ODU2PGSetHOTmrFactory[ODU2PGIdx]);
			}
			else
			{
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[ODU2PGIdx]);
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[ODU2PGIdx]);
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[ODU2PGIdx]);
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[ODU2PGIdx]);
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[ODU2PGIdx]);
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[ODU2PGIdx]);
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[ODU2PGIdx]);
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[ODU2PGIdx]);
			}
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t PROFILE_ADM10_ODU1PG_SetFactoryValue(PROFILE_T *Profile, uint32_t Mode, uint32_t ODU1PGIdx)
{
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetTypeIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetModeIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetMonModeIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetRevModeIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetWTRTmrIdx[ODU1PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetHOTmrIdx[ODU1PGIdx]);
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t PROFILE_ADM10_ODU0PG_SetFactoryValue(PROFILE_T *Profile, uint32_t Mode, uint32_t ODU0PGIdx)
{
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetTypeIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetModeIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetMonModeIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetRevModeIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetWTRTmrIdx[ODU0PGIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetHOTmrIdx[ODU0PGIdx]);
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t PROFILE_ADM10_PG_SetConfigSetModeValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM10_ODU2_PGS; i++)
	{
		PROFILE_ADM10_ODU2PG_SetFactoryValue(Profile, *(uint32_t*)Value, i);
	}
	for (i = 0; i < PROFILE_ADM10_ODU1_PGS; i++)
	{
		PROFILE_ADM10_ODU1PG_SetFactoryValue(Profile, *(uint32_t*)Value, i);
	}
	for (i = 0; i < PROFILE_ADM10_ODU0_PGS; i++)
	{
		PROFILE_ADM10_ODU0PG_SetFactoryValue(Profile, *(uint32_t*)Value, i);
	}
	return 0;
}

static int32_t PROFILE_ADM10_PG_SetConfigODU2PGSetTypeValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	if (*(uint32_t*)Value == PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_DISABLED)
	{
		return PROFILE_ADM10_ODU2PG_SetFactoryValue(Profile, Mode, ODU2PGIdx);
	}
	return 0;
}

static int32_t PROFILE_ADM10_PG_SetConfigODU1PGSetTypeValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	if (*(uint32_t*)Value == PROFILE_ADM10_ODU1_PG_SET_TYPE_ENUM_DISABLED)
	{
		return PROFILE_ADM10_ODU1PG_SetFactoryValue(Profile, Mode, ODU1PGIdx);
	}
	return 0;
}

static int32_t PROFILE_ADM10_PG_SetConfigODU0PGSetTypeValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	if (*(uint32_t*)Value == PROFILE_ADM10_ODU0_PG_SET_TYPE_ENUM_DISABLED)
	{
		return PROFILE_ADM10_ODU0PG_SetFactoryValue(Profile, Mode, ODU0PGIdx);
	}
	return 0;
}

static int32_t PROFILE_ADM10_PG_SetConfigODU2PGSetFactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU2PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	return PROFILE_ADM10_ODU2PG_SetFactoryValue(Profile, Mode, ODU2PGIdx);
}

static int32_t PROFILE_ADM10_PG_SetConfigODU1PGSetFactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU1PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	return PROFILE_ADM10_ODU1PG_SetFactoryValue(Profile, Mode, ODU1PGIdx);
}

static int32_t PROFILE_ADM10_PG_SetConfigODU0PGSetFactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t ODU0PGIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	return PROFILE_ADM10_ODU0PG_SetFactoryValue(Profile, Mode, ODU0PGIdx);
}

int32_t PROFILE_ADM10_PG_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value)
{
	uint32_t i;
	
	if (TableIdx == PROFILE_ADM10_SETMODE_IDX)
		return PROFILE_ADM10_PG_SetConfigSetModeValueDependencies(Profile, Table, TableIdx, Value);
	
	for (i = 0; i < PROFILE_ADM10_ODU2_PGS; i++)
	{
		if (ProfileADM10ConfigODU2PGSetTypeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_SetConfigODU2PGSetTypeValueDependencies(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU2PGSetFactoryIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_SetConfigODU2PGSetFactoryValueDependencies(Profile, Table, TableIdx, Value, i);
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU1_PGS; i++)
	{
		if (ProfileADM10ConfigODU1PGSetTypeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_SetConfigODU1PGSetTypeValueDependencies(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU1PGSetFactoryIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_SetConfigODU1PGSetFactoryValueDependencies(Profile, Table, TableIdx, Value, i);
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU0_PGS; i++)
	{
		if (ProfileADM10ConfigODU0PGSetTypeIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_SetConfigODU0PGSetTypeValueDependencies(Profile, Table, TableIdx, Value, i);
		else if (ProfileADM10ConfigODU0PGSetFactoryIdx[i] == TableIdx)
			return PROFILE_ADM10_PG_SetConfigODU0PGSetFactoryValueDependencies(Profile, Table, TableIdx, Value, i);
	}
	return 0;
}

static void PROFILE_ADM10_ODU2PG_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t i;
	uint32_t Mode;
	uint32_t ODU2PGType[PROFILE_ADM10_ODU2_PGS];
	uint32_t ODU2PGMode[PROFILE_ADM10_ODU2_PGS];
	uint32_t ODU2PGMonMode[PROFILE_ADM10_ODU2_PGS];
	uint32_t ODU2PGRevMode[PROFILE_ADM10_ODU2_PGS];
	uint32_t ODU2PGPrimaryODUk[PROFILE_ADM10_ODU2_PGS];
	uint32_t ODU2PGProtectODUk[PROFILE_ADM10_ODU2_PGS];
	uint32_t ODU2PGWTRTmr[PROFILE_ADM10_ODU2_PGS];
	uint32_t ODU2PGHOTmr[PROFILE_ADM10_ODU2_PGS];
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return;
	
	for (i = 0; i < PROFILE_ADM10_ODU2_PGS; i++)
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[i], &ODU2PGType[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[i], &ODU2PGMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[i], &ODU2PGMonMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[i], &ODU2PGRevMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[i], &ODU2PGPrimaryODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[i], &ODU2PGProtectODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[i], &ODU2PGWTRTmr[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[i], &ODU2PGHOTmr[i]))
			return;
		
		switch (Mode)
		{
			case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetFactoryIdx[i], PROFILE_STATE_OOS);
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
				if (ODU2PGType[i] == PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_DISABLED)
				{
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetFactoryIdx[i], PROFILE_STATE_OOS);
				}
				else
				{
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetFactoryIdx[i], PROFILE_STATE_IS);
				}
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetFactoryIdx[i], PROFILE_STATE_OOS);
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
				if (i < 1)
				{
					if (ODU2PGType[i] == PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_DISABLED)
					{
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[i], PROFILE_STATE_IS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[i], PROFILE_STATE_OOS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[i], PROFILE_STATE_OOS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[i], PROFILE_STATE_OOS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetFactoryIdx[i], PROFILE_STATE_OOS);
					}
					else
					{
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[i], PROFILE_STATE_IS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[i], PROFILE_STATE_IS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[i], PROFILE_STATE_IS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[i], PROFILE_STATE_IS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[i], PROFILE_STATE_IS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[i], PROFILE_STATE_IS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[i], PROFILE_STATE_IS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[i], PROFILE_STATE_IS);
						PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetFactoryIdx[i], PROFILE_STATE_IS);
					}
				}
				else
				{
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetMonModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetRevModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetFactoryIdx[i], PROFILE_STATE_OOS);
				}
				break;
		}
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU2_PGS; i++)
	{
		switch (Mode)
		{
			case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
				if (ODU2PGMode[i] == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_AUTO) /*Auto*/
				{
					if (ODU2PGRevMode[i] == PROFILE_ADM10_ODU2_PG_SET_REV_MODE_ENUM_REVERTIVE) /*Revertive*/
						FPGA_LINE_AGG_2X10_BS_AGG_MODE_SET(&fpga_line_agg_2x10_bs, i, FPGA_LINE_AGG_2X10_BS_AGG_MODE_AUTO_REVERTIVE);
					else
						FPGA_LINE_AGG_2X10_BS_AGG_MODE_SET(&fpga_line_agg_2x10_bs, i, FPGA_LINE_AGG_2X10_BS_AGG_MODE_AUTO_NON_REVERTIVE);
				}
				else if (ODU2PGMode[i] == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
				{
					FPGA_LINE_AGG_2X10_BS_AGG_MODE_SET(&fpga_line_agg_2x10_bs, i, FPGA_LINE_AGG_2X10_BS_AGG_MODE_MANUAL_LINE_0);
				}
				else if (ODU2PGMode[i] == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_FORCED) /*Forced*/
				{
					FPGA_LINE_AGG_2X10_BS_AGG_MODE_SET(&fpga_line_agg_2x10_bs, i, FPGA_LINE_AGG_2X10_BS_AGG_MODE_MANUAL_LINE_1);
				}
				else if (ODU2PGMode[i] == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
				{
					if ((!((fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & (FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_LOF_MASK | FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_LOM_MASK)) || \
						(fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_AIS_MASK) || \
						((fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_STAT_MASK) == FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_STAT_OCI_MASK) || \
						((fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_STAT_MASK) == FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_STAT_LCK_MASK))) && \
						(!(fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_SD_MASK)) && \
						(!(fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_OPUCSF_MASK)))
					{
						FPGA_LINE_AGG_2X10_BS_AGG_MODE_SET(&fpga_line_agg_2x10_bs, i, FPGA_LINE_AGG_2X10_BS_AGG_MODE_MANUAL_LINE_1);
					}
				}
				FPGA_LINE_AGG_2X10_BS_AGG_WTR_SET(&fpga_line_agg_2x10_bs, i, ODU2PGWTRTmr[i]);
				FPGA_LINE_AGG_2X10_BS_AGG_HO_SET(&fpga_line_agg_2x10_bs, i, ODU2PGHOTmr[i] / 100);
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
				if (i < 1)
				{
					if (ODU2PGMode[i] == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_AUTO) /*Auto*/
					{
						if (ODU2PGRevMode[i] == PROFILE_ADM10_ODU2_PG_SET_REV_MODE_ENUM_REVERTIVE) /*Revertive*/
							FPGA_LINE_AGG_2X10_BS_AGG_MODE_SET(&fpga_line_agg_2x10_bs, i, FPGA_LINE_AGG_2X10_BS_AGG_MODE_AUTO_REVERTIVE);
						else
							FPGA_LINE_AGG_2X10_BS_AGG_MODE_SET(&fpga_line_agg_2x10_bs, i, FPGA_LINE_AGG_2X10_BS_AGG_MODE_AUTO_NON_REVERTIVE);
					}
					else if (ODU2PGMode[i] == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
					{
						FPGA_LINE_AGG_2X10_BS_AGG_MODE_SET(&fpga_line_agg_2x10_bs, i, FPGA_LINE_AGG_2X10_BS_AGG_MODE_MANUAL_LINE_0);
					}
					else if (ODU2PGMode[i] == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_FORCED) /*Forced*/
					{
						FPGA_LINE_AGG_2X10_BS_AGG_MODE_SET(&fpga_line_agg_2x10_bs, i, FPGA_LINE_AGG_2X10_BS_AGG_MODE_MANUAL_LINE_1);
					}
					else if (ODU2PGMode[i] == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
					{
						if ((!((fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & (FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_LOF_MASK | FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_LOM_MASK)) || \
							(fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_AIS_MASK) || \
							((fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_STAT_MASK) == FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_STAT_OCI_MASK) || \
							((fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_STAT_MASK) == FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_STAT_LCK_MASK))) && \
							(!(fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_SD_MASK)) && \
							(!(fpga_line_agg_2x10_bs.Iface[i + 2].__ODU2Alarms & FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_OPUCSF_MASK)))
						{
							FPGA_LINE_AGG_2X10_BS_AGG_MODE_SET(&fpga_line_agg_2x10_bs, i, FPGA_LINE_AGG_2X10_BS_AGG_MODE_MANUAL_LINE_1);
						}
					}
					FPGA_LINE_AGG_2X10_BS_AGG_WTR_SET(&fpga_line_agg_2x10_bs, i, ODU2PGWTRTmr[i]);
					FPGA_LINE_AGG_2X10_BS_AGG_HO_SET(&fpga_line_agg_2x10_bs, i, ODU2PGHOTmr[i] / 100);
				}
				break;
		}
	}
}

static void PROFILE_ADM10_ODU1PG_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t i, j;
	uint32_t Mode;
	uint32_t PrimaryLineIdx, PrimaryODU1Idx, ProtectLineIdx, ProtectODU1Idx;
	uint32_t ODU1FramerOPU1PayloadType, ODU1FramerOPU1Mux;
	uint32_t ODU1XCSrc, ODU1XCDst;
	uint32_t PrimaryODU1Mux;
	uint32_t Target, Client;
	uint32_t ODU1PGType[PROFILE_ADM10_ODU1_PGS];
	uint32_t ODU1PGMode[PROFILE_ADM10_ODU1_PGS];
	uint32_t ODU1PGMonMode[PROFILE_ADM10_ODU1_PGS];
	uint32_t ODU1PGRevMode[PROFILE_ADM10_ODU1_PGS];
	uint32_t ODU1PGPrimaryODUk[PROFILE_ADM10_ODU1_PGS];
	uint32_t ODU1PGProtectODUk[PROFILE_ADM10_ODU1_PGS];
	uint32_t ODU1PGWTRTmr[PROFILE_ADM10_ODU1_PGS];
	uint32_t ODU1PGHOTmr[PROFILE_ADM10_ODU1_PGS];
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return;
	
	for (i = 0; i < PROFILE_ADM10_ODU1_PGS; i++)
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetTypeIdx[i], &ODU1PGType[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetModeIdx[i], &ODU1PGMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetMonModeIdx[i], &ODU1PGMonMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetRevModeIdx[i], &ODU1PGRevMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], &ODU1PGPrimaryODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], &ODU1PGProtectODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetWTRTmrIdx[i], &ODU1PGWTRTmr[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetHOTmrIdx[i], &ODU1PGHOTmr[i]))
			return;
		
		switch (Mode)
		{
			case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
				if (ODU1PGType[i] == PROFILE_ADM10_ODU1_PG_SET_TYPE_ENUM_DISABLED)
				{
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetTypeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetMonModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetRevModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetFactoryIdx[i], PROFILE_STATE_OOS);
				}
				else
				{
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetTypeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetMonModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetRevModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetWTRTmrIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetHOTmrIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetFactoryIdx[i], PROFILE_STATE_IS);
				}
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetTypeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetMonModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetRevModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetFactoryIdx[i], PROFILE_STATE_OOS);
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetTypeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetMonModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetRevModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetFactoryIdx[i], PROFILE_STATE_OOS);
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
				if (ODU1PGType[i] == PROFILE_ADM10_ODU1_PG_SET_TYPE_ENUM_DISABLED)
				{
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetTypeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetMonModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetRevModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetFactoryIdx[i], PROFILE_STATE_OOS);
				}
				else
				{
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetTypeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetMonModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetRevModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetWTRTmrIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetHOTmrIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetFactoryIdx[i], PROFILE_STATE_IS);
				}
				break;
		}
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_CNT; i++)
	{
		FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_SET(&fpga_line_adm_10, i, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_MANUAL_PRIMARY);
	}
	
	for (i = 0 ; i < BSP_FPGA_CLIENT_CNT; i++)
	{
		for (j = 0; j < FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_CNT; j++)
		{
			FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_SET(&fpga_client_adm_2_5[i], j, FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_MANUAL_PRIMARY);
		}
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU1_PGS; i++)
	{
		switch (Mode)
		{
			case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
				if ((ODU1PGType[i] != PROFILE_ADM10_ODU1_PG_SET_TYPE_ENUM_DISABLED) &&
						(ODU1PGPrimaryODUk[i] != PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_NONE) &&
						(ODU1PGProtectODUk[i] != PROFILE_ADM10_ODU1_PG_SET_PROTECT_ODUK_ENUM_NONE))
				{
					
					PrimaryLineIdx = (ODU1PGPrimaryODUk[i] - 1) / 4;
					PrimaryODU1Idx = (ODU1PGPrimaryODUk[i] - 1) % 4;
					ProtectLineIdx = (ODU1PGProtectODUk[i] - 1) / 4;
					ProtectODU1Idx = (ODU1PGProtectODUk[i] - 1) % 4;
					
					PrimaryODU1Mux = FPGA_LINE_ADM_10_MUX_ODU12_GET(&fpga_line_adm_10, (PrimaryLineIdx * 4) + PrimaryODU1Idx);
					
					Target = 0xFFFFFFFF;
					
					for (j = 0; j < PROFILE_ADM10_ODU1_XCS; j++)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[j], &ODU1XCSrc))
							return;
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[j], &ODU1XCDst))
							return;
						if (ODU1XCSrc == PROFILE_ADM10_XC_LINE_ODU1(PrimaryLineIdx, PrimaryODU1Idx))
						{
							Target = ODU1XCDst;
							break;
						}
						if (ODU1XCDst == PROFILE_ADM10_XC_LINE_ODU1(PrimaryLineIdx, PrimaryODU1Idx))
						{
							Target = ODU1XCSrc;
							break;
						}
					}
					
					if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU1(0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU1(15)))
					{
						FPGA_LINE_ADM_10_MUX_ODU12_SET(&fpga_line_adm_10, (ProtectLineIdx * 4) + ProtectODU1Idx, PrimaryODU1Mux);
						
						Client = Target - 1025;
						FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_PROTECT_ODU1_SET(&fpga_line_adm_10, Client, ProtectLineIdx, ProtectODU1Idx);
						
						if (ODU1PGMode[i] == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_AUTO) /*Auto*/
						{
							if (ODU1PGRevMode[i] == PROFILE_ADM10_ODU1_PG_SET_REV_MODE_ENUM_REVERTIVE) /*Revertive*/
								FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_SET(&fpga_line_adm_10, Client, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_AUTO_REVERTIVE);
							else
								FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_SET(&fpga_line_adm_10, Client, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_AUTO_NON_REVERTIVE);
						}
						else if (ODU1PGMode[i] == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
						{
							FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_SET(&fpga_line_adm_10, Client, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_MANUAL_PRIMARY);
						}
						else if (ODU1PGMode[i] == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_FORCED) /*Forced*/
						{
							FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_SET(&fpga_line_adm_10, Client, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_MANUAL_PROTECT);
						}
						else if (ODU1PGMode[i] == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
						{
							if ((!((fpga_line_adm_10.Iface[ProtectLineIdx].__ODU1Alarms[ProtectODU1Idx] & (FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_LOF_MASK | FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_LOM_MASK)) || \
								(fpga_line_adm_10.Iface[ProtectLineIdx].__ODU1Alarms[ProtectODU1Idx] & FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_AIS_MASK) || \
								((fpga_line_adm_10.Iface[ProtectLineIdx].__ODU1Alarms[ProtectODU1Idx] & FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_STAT_MASK) == FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_STAT_OCI_MASK) || \
								((fpga_line_adm_10.Iface[ProtectLineIdx].__ODU1Alarms[ProtectODU1Idx] & FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_STAT_MASK) == FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_STAT_LCK_MASK))) && \
								(!(fpga_line_adm_10.Iface[ProtectLineIdx].__ODU1Alarms[ProtectODU1Idx] & FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_SD_MASK)) && \
								(!(fpga_line_adm_10.Iface[ProtectLineIdx].__ODU1Alarms[ProtectODU1Idx] & FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_OPUCSF_MASK)))
							{
								FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_SET(&fpga_line_adm_10, Client, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_MANUAL_PROTECT);
							}
						}
						FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_WTR_SET(&fpga_line_adm_10, Client, ODU1PGWTRTmr[i]);
						FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_HO_SET(&fpga_line_adm_10, Client, ODU1PGHOTmr[i] / 100);
					}
				}
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
				if ((ODU1PGType[i] != PROFILE_ADM10_ODU1_PG_SET_TYPE_ENUM_DISABLED) &&
						(ODU1PGPrimaryODUk[i] != PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_NONE) &&
						(ODU1PGProtectODUk[i] != PROFILE_ADM10_ODU1_PG_SET_PROTECT_ODUK_ENUM_NONE))
				{
					
					PrimaryLineIdx = (ODU1PGPrimaryODUk[i] - PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU1) / 2;
					ProtectLineIdx = (ODU1PGProtectODUk[i] - PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU1) / 2;
					
					ODU1FramerOPU1PayloadType = FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_GET(&fpga_client_adm_2_5[1], PrimaryLineIdx);
					ODU1FramerOPU1Mux = FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_MUX_GET(&fpga_client_adm_2_5[1], PrimaryLineIdx);
					
					Target = 0xFFFFFFFF;
					
					for (j = 0; j < PROFILE_ADM10_ODU1_XCS; j++)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[j], &ODU1XCSrc))
							return;
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[j], &ODU1XCDst))
							return;
						if (ODU1XCSrc == PROFILE_ADM10_XC_CLIENT_ODU1(8 + PrimaryLineIdx))
						{
							Target = ODU1XCDst;
							break;
						}
						if (ODU1XCDst == PROFILE_ADM10_XC_CLIENT_ODU1(8 + PrimaryLineIdx))
						{
							Target = ODU1XCSrc;
							break;
						}
					}
					
					if ((Target == PROFILE_ADM10_XC_CLIENT_ODU1(9)) ||
							(Target == PROFILE_ADM10_XC_CLIENT_ODU1(11)) || 
							(Target == PROFILE_ADM10_XC_CLIENT_ODU1(13)) || 
							(Target == PROFILE_ADM10_XC_CLIENT_ODU1(15)))
					{
						FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_SET(&fpga_client_adm_2_5[1], ProtectLineIdx, ODU1FramerOPU1PayloadType);
						FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_MUX_SET(&fpga_client_adm_2_5[1], ProtectLineIdx, ODU1FramerOPU1Mux);
						
						Client = (Target - PROFILE_ADM10_XC_CLIENT_ODU1(8)) / 2;
						FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_PROTECT_ODU1_SET(&fpga_client_adm_2_5[1], Client, ProtectLineIdx);
						
						if (ODU1PGMode[i] == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_AUTO) /*Auto*/
						{
							if (ODU1PGRevMode[i] == PROFILE_ADM10_ODU1_PG_SET_REV_MODE_ENUM_REVERTIVE) /*Revertive*/
								FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_AUTO_REVERTIVE);
							else
								FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_AUTO_NON_REVERTIVE);
						}
						else if (ODU1PGMode[i] == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
						{
							FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_MANUAL_PRIMARY);
						}
						else if (ODU1PGMode[i] == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_FORCED) /*Forced*/
						{
							FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_MANUAL_PROTECT);
						}
						else if (ODU1PGMode[i] == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
						{
							if ((!((fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU1Alarms & (FPGA_CLIENT_ADM_2_5_LINE_ODU1_ALARMS_LOF_MASK | FPGA_CLIENT_ADM_2_5_LINE_ODU1_ALARMS_LOM_MASK)) || \
								(fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU1Alarms & FPGA_CLIENT_ADM_2_5_LINE_ODU1_ALARMS_AIS_MASK) || \
								((fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU1Alarms & FPGA_CLIENT_ADM_2_5_LINE_ODU1_ALARMS_STAT_MASK) == FPGA_CLIENT_ADM_2_5_LINE_ODU1_ALARMS_STAT_OCI_MASK) || \
								((fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU1Alarms & FPGA_CLIENT_ADM_2_5_LINE_ODU1_ALARMS_STAT_MASK) == FPGA_CLIENT_ADM_2_5_LINE_ODU1_ALARMS_STAT_LCK_MASK))) && \
								(!(fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU1Alarms & FPGA_CLIENT_ADM_2_5_LINE_ODU1_ALARMS_SD_MASK)) && \
								(!(fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU1Alarms & FPGA_CLIENT_ADM_2_5_LINE_ODU1_ALARMS_OPUCSF_MASK)))
							{
								FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_MANUAL_PROTECT);
							}
						}
						FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_WTR_SET(&fpga_client_adm_2_5[1], Client, ODU1PGWTRTmr[i]);
						FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_HO_SET(&fpga_client_adm_2_5[1], Client, ODU1PGHOTmr[i] / 100);
					}
				}
				break;
		}
	}
}

static void PROFILE_ADM10_ODU0PG_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t i, j;
	uint32_t Mode;
	uint32_t PrimaryLineIdx, PrimaryODU1Idx, PrimaryODU0Idx, ProtectLineIdx, ProtectODU1Idx, ProtectODU0Idx;
	uint32_t ODU1FramerOPU1PayloadType, ODU1FramerODU0Mux;
	uint32_t ODU0XCSrc, ODU0XCDst;
	uint32_t PrimaryODU1Mux, ProtectODU1Mux, PrimaryODU0Mux;
	uint32_t Target, Client;
	uint32_t ODU0PGType[PROFILE_ADM10_ODU0_PGS];
	uint32_t ODU0PGMode[PROFILE_ADM10_ODU0_PGS];
	uint32_t ODU0PGMonMode[PROFILE_ADM10_ODU0_PGS];
	uint32_t ODU0PGRevMode[PROFILE_ADM10_ODU0_PGS];
	uint32_t ODU0PGPrimaryODUk[PROFILE_ADM10_ODU0_PGS];
	uint32_t ODU0PGProtectODUk[PROFILE_ADM10_ODU0_PGS];
	uint32_t ODU0PGWTRTmr[PROFILE_ADM10_ODU0_PGS];
	uint32_t ODU0PGHOTmr[PROFILE_ADM10_ODU0_PGS];
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return;
	
	for (i = 0; i < PROFILE_ADM10_ODU0_PGS; i++)
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetTypeIdx[i], &ODU0PGType[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetModeIdx[i], &ODU0PGMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetMonModeIdx[i], &ODU0PGMonMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetRevModeIdx[i], &ODU0PGRevMode[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i], &ODU0PGPrimaryODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[i], &ODU0PGProtectODUk[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetWTRTmrIdx[i], &ODU0PGWTRTmr[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetHOTmrIdx[i], &ODU0PGHOTmr[i]))
			return;
		
		switch (Mode)
		{
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
			case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
				if (ODU0PGType[i] == PROFILE_ADM10_ODU0_PG_SET_TYPE_ENUM_DISABLED)
				{
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetTypeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetMonModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetRevModeIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetFactoryIdx[i], PROFILE_STATE_OOS);
				}
				else
				{
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetTypeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetMonModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetRevModeIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetWTRTmrIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetHOTmrIdx[i], PROFILE_STATE_IS);
					PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetFactoryIdx[i], PROFILE_STATE_IS);
				}
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetTypeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetMonModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetRevModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetFactoryIdx[i], PROFILE_STATE_OOS);
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetTypeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetMonModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetRevModeIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetWTRTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetHOTmrIdx[i], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetFactoryIdx[i], PROFILE_STATE_OOS);
				break;
		}
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_CNT; i++)
	{
		FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_SET(&fpga_line_adm_10, i, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_MANUAL_PRIMARY);
	}
	
	for (i = 0 ; i < BSP_FPGA_CLIENT_CNT; i++)
	{
		for (j = 0; j < FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_CNT; j++)
		{
			FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_SET(&fpga_client_adm_2_5[i], j, FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_MANUAL_PRIMARY);
		}
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU0_PGS; i++)
	{
		switch (Mode)
		{
			case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
				if ((ODU0PGType[i] != PROFILE_ADM10_ODU0_PG_SET_TYPE_ENUM_DISABLED) &&
						(ODU0PGPrimaryODUk[i] != PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_NONE) &&
						(ODU0PGProtectODUk[i] != PROFILE_ADM10_ODU0_PG_SET_PROTECT_ODUK_ENUM_NONE))
				{
					
					PrimaryLineIdx = (ODU0PGPrimaryODUk[i] - 1) / 8;
					PrimaryODU1Idx = ((ODU0PGPrimaryODUk[i] - 1) % 8) / 2;
					PrimaryODU0Idx = ((ODU0PGPrimaryODUk[i] - 1) % 8) % 2;
					ProtectLineIdx = (ODU0PGProtectODUk[i] - 1) / 8;
					ProtectODU1Idx = ((ODU0PGProtectODUk[i] - 1) % 8) / 2;
					ProtectODU0Idx = ((ODU0PGProtectODUk[i] - 1) % 8) % 2;
					
					PrimaryODU1Mux =FPGA_LINE_ADM_10_MUX_ODU12_GET(&fpga_line_adm_10, (PrimaryLineIdx * 4) + PrimaryODU1Idx);
					ProtectODU1Mux =FPGA_LINE_ADM_10_MUX_ODU12_GET(&fpga_line_adm_10, (ProtectLineIdx * 4) + ProtectODU1Idx);
					
					PrimaryODU0Mux =FPGA_LINE_ADM_10_MUX_ODU01_GET(&fpga_line_adm_10, ((PrimaryODU1Mux - 8) * 2) + PrimaryODU0Idx);					
					
					Target = 0xFFFFFFFF;
					
					for (j = 0; j < PROFILE_ADM10_ODU0_XCS; j++)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[j], &ODU0XCSrc))
							return;
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[j], &ODU0XCDst))
							return;
						if (ODU0XCSrc == PROFILE_ADM10_XC_LINE_ODU0(PrimaryLineIdx, (PrimaryODU1Idx*2) + PrimaryODU0Idx))
						{
							Target = ODU0XCDst;
							break;
						}
						if (ODU0XCDst == PROFILE_ADM10_XC_LINE_ODU0(PrimaryLineIdx, (PrimaryODU1Idx*2) + PrimaryODU0Idx))
						{
							Target = ODU0XCSrc;
							break;
						}
					}
					
					if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU0(0, 0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU0(15, 1)))
					{
						FPGA_LINE_ADM_10_MUX_ODU01_SET(&fpga_line_adm_10, ((ProtectODU1Mux - 8) * 2) + ProtectODU0Idx, PrimaryODU0Mux);
						
						Client = (Target - PROFILE_ADM10_XC_CLIENT_ODU0(0, 0)) / 2;
						FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_PROTECT_ODU0_SET(&fpga_line_adm_10, Client, ProtectLineIdx, (ProtectODU1Idx*2) + ProtectODU0Idx);
						
						if (ODU0PGMode[i] == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_AUTO) /*Auto*/
						{
							if (ODU0PGRevMode[i] == PROFILE_ADM10_ODU0_PG_SET_REV_MODE_ENUM_REVERTIVE) /*Revertive*/
								FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_SET(&fpga_line_adm_10, Client, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_AUTO_REVERTIVE);
							else
								FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_SET(&fpga_line_adm_10, Client, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_AUTO_NON_REVERTIVE);
						}
						else if (ODU0PGMode[i] == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
						{
							FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_SET(&fpga_line_adm_10, Client, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_MANUAL_PRIMARY);
						}
						else if (ODU0PGMode[i] == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_FORCED) /*Forced*/
						{
							FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_SET(&fpga_line_adm_10, Client, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_MANUAL_PROTECT);
						}
						else if (ODU0PGMode[i] == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
						{
							if ((!((fpga_line_adm_10.Iface[ProtectLineIdx].__ODU0Alarms[(ProtectODU1Idx * 2) + ProtectODU0Idx] & (FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_LOF_MASK |FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_LOM_MASK)) || \
								(fpga_line_adm_10.Iface[ProtectLineIdx].__ODU0Alarms[(ProtectODU1Idx * 2) + ProtectODU0Idx] &FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_AIS_MASK) || \
								((fpga_line_adm_10.Iface[ProtectLineIdx].__ODU0Alarms[(ProtectODU1Idx * 2) + ProtectODU0Idx] &FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_STAT_MASK) == FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_STAT_OCI_MASK) || \
								((fpga_line_adm_10.Iface[ProtectLineIdx].__ODU0Alarms[(ProtectODU1Idx * 2) + ProtectODU0Idx] &FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_STAT_MASK) == FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_STAT_LCK_MASK))) && \
								(!(fpga_line_adm_10.Iface[ProtectLineIdx].__ODU0Alarms[(ProtectODU1Idx * 2) + ProtectODU0Idx] &FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_SD_MASK)) && \
								(!(fpga_line_adm_10.Iface[ProtectLineIdx].__ODU0Alarms[(ProtectODU1Idx * 2) + ProtectODU0Idx] &FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_OPUCSF_MASK)))
							{
								FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_SET(&fpga_line_adm_10, Client, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_MANUAL_PROTECT);
							}
						}
						FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_WTR_SET(&fpga_line_adm_10, Client, ODU0PGWTRTmr[i]);
						FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_HO_SET(&fpga_line_adm_10, Client, ODU0PGHOTmr[i] / 100);
					}
				}
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
				if ((ODU0PGType[i] != PROFILE_ADM10_ODU0_PG_SET_TYPE_ENUM_DISABLED) &&
						(ODU0PGPrimaryODUk[i] != PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_NONE) &&
						(ODU0PGProtectODUk[i] != PROFILE_ADM10_ODU0_PG_SET_PROTECT_ODUK_ENUM_NONE))
				{
					PrimaryLineIdx = ((ODU0PGPrimaryODUk[i] - PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU0_1) / 2) / 2;
					PrimaryODU0Idx = (ODU0PGPrimaryODUk[i] - PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU0_1) % 2;
					ProtectLineIdx = ((ODU0PGProtectODUk[i] - PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU0_1) / 2) / 2;
					ProtectODU0Idx = (ODU0PGProtectODUk[i] - PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU0_1) % 2;
					
					ODU1FramerOPU1PayloadType = FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_GET(&fpga_client_adm_2_5[1], PrimaryLineIdx);
					ODU1FramerODU0Mux = FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_GET(&fpga_client_adm_2_5[1], PrimaryLineIdx, PrimaryODU0Idx);
					
					Target = 0xFFFFFFFF;
					
					for (j = 0; j < PROFILE_ADM10_ODU0_XCS; j++)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[j], &ODU0XCSrc))
							return;
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[j], &ODU0XCDst))
							return;
						if (ODU0XCSrc == PROFILE_ADM10_XC_CLIENT_ODU0(8 + (PrimaryLineIdx * 2), PrimaryODU0Idx))
						{
							Target = ODU0XCDst;
							break;
						}
						if (ODU0XCDst == PROFILE_ADM10_XC_CLIENT_ODU0(8 + (PrimaryLineIdx * 2), PrimaryODU0Idx))
						{
							Target = ODU0XCSrc;
							break;
						}
					}
					
					if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU0(15, 1)))
					{
						FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_SET(&fpga_client_adm_2_5[1], ProtectLineIdx, ODU1FramerOPU1PayloadType);
						FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_SET(&fpga_client_adm_2_5[1], ProtectLineIdx, ProtectODU0Idx, ODU1FramerODU0Mux);
						
						Client = ((Target - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) / 2) / 2;
						FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_PROTECT_ODU0_SET(&fpga_client_adm_2_5[1], Client, ProtectLineIdx, ProtectODU0Idx);
						
						if (ODU0PGMode[i] == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_AUTO) /*Auto*/
						{
							if (ODU0PGRevMode[i] == PROFILE_ADM10_ODU0_PG_SET_REV_MODE_ENUM_REVERTIVE) /*Revertive*/
								FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_AUTO_REVERTIVE);
							else
								FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_AUTO_NON_REVERTIVE);
						}
						else if (ODU0PGMode[i] == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
						{
							FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_MANUAL_PRIMARY);
						}
						else if (ODU0PGMode[i] == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_FORCED) /*Forced*/
						{
							FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_MANUAL_PROTECT);
						}
						else if (ODU0PGMode[i] == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
						{
							if ((!((fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU0Alarms[ProtectODU0Idx] & (FPGA_CLIENT_ADM_2_5_LINE_ODU0_ALARMS_LOF_MASK |FPGA_CLIENT_ADM_2_5_LINE_ODU0_ALARMS_LOM_MASK)) || \
								(fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU0Alarms[ProtectODU0Idx] &FPGA_CLIENT_ADM_2_5_LINE_ODU0_ALARMS_AIS_MASK) || \
								((fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU0Alarms[ProtectODU0Idx] &FPGA_CLIENT_ADM_2_5_LINE_ODU0_ALARMS_STAT_MASK) == FPGA_CLIENT_ADM_2_5_LINE_ODU0_ALARMS_STAT_OCI_MASK) || \
								((fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU0Alarms[ProtectODU0Idx] &FPGA_CLIENT_ADM_2_5_LINE_ODU0_ALARMS_STAT_MASK) == FPGA_CLIENT_ADM_2_5_LINE_ODU0_ALARMS_STAT_LCK_MASK))) && \
								(!(fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU0Alarms[ProtectODU0Idx] &FPGA_CLIENT_ADM_2_5_LINE_ODU0_ALARMS_SD_MASK)) && \
								(!(fpga_client_adm_2_5[1].Line[ProtectLineIdx].__ODU0Alarms[ProtectODU0Idx] &FPGA_CLIENT_ADM_2_5_LINE_ODU0_ALARMS_OPUCSF_MASK)))
							{
								FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_MANUAL_PROTECT);
							}
						}
						FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_WTR_SET(&fpga_client_adm_2_5[1], Client, ODU0PGWTRTmr[i]);
						FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_HO_SET(&fpga_client_adm_2_5[1], Client, ODU0PGHOTmr[i] / 100);
					}
				}
				break;
		}
	}
}

void PROFILE_ADM10_PG_UpdateConfigs(PROFILE_T *Profile)
{
	PROFILE_ADM10_ODU2PG_UpdateConfigs(Profile);
	PROFILE_ADM10_ODU1PG_UpdateConfigs(Profile);
	PROFILE_ADM10_ODU0PG_UpdateConfigs(Profile);
}

static void PROFILE_ADM10_ODU2PG_UpdateValues(PROFILE_T *Profile)
{
	uint32_t Mode;
	uint32_t PGType;
	uint32_t PGMode;
	uint32_t PGState;
	uint32_t PGPrimaryODUk;
	uint32_t PGProtectODUk;
	uint32_t PGActiveODUk;
	uint8_t FPGAPGStatus;
	uint32_t i;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return;
	
	for (i = 0; i < PROFILE_ADM10_ODU2_PGS; i++)
	{
		switch (Mode)
		{
			case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
				PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_NORMAL;
				PGActiveODUk = PROFILE_ADM10_ODU2_PG_ACTIVE_ODUK_ENUM_NONE;
				PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGStateIdx[i], &PGState, PROFILE_STATE_OOS);
				PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_OOS);
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[i], &PGType))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[i], &PGMode))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[i], &PGPrimaryODUk))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[i], &PGProtectODUk))
					return;
				if ((PGType == PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_DISABLED) ||
						(PGPrimaryODUk == PROFILE_ADM10_ODU2_PG_SET_PRIMARY_ODUK_ENUM_NONE) ||
						(PGProtectODUk == PROFILE_ADM10_ODU2_PG_SET_PROTECT_ODUK_ENUM_NONE))
				{
					PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_NORMAL;
					PGActiveODUk = PROFILE_ADM10_ODU2_PG_ACTIVE_ODUK_ENUM_NONE;
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGStateIdx[i], &PGState, PROFILE_STATE_OOS);
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_OOS);
				}
				else
				{
					PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_NORMAL;
					PGActiveODUk = PROFILE_ADM10_ODU2_PG_ACTIVE_ODUK_ENUM_NONE;
					
					FPGAPGStatus = fpga_line_agg_2x10_bs.AGG[i].__Status;						
					if (PGMode == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_AUTO) /*Auto*/
					{
						if (FPGAPGStatus & (0x03 << 1)) /*SF*/
							PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_SIGNAL_FAIL;
						else if (FPGAPGStatus & 0x01) /*SD*/
							PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_SIGNAL_DEGRADE;
					}
					else if (PGMode == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
					{
						PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_LOCKOUT;
					}
					else if (PGMode == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_FORCED) /*Forced*/
					{
						PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_FORCED;
					}
					else if (PGMode == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
					{
						PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_MANUAL;
					}
				
					if (FPGAPGStatus & (0x01 << 3))
						PGActiveODUk = PGProtectODUk;
					else
						PGActiveODUk = PGPrimaryODUk;						
				}					
				PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGStateIdx[i], &PGState, PROFILE_STATE_IS);
				PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_IS);
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
				PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_NORMAL;
				PGActiveODUk = PROFILE_ADM10_ODU2_PG_ACTIVE_ODUK_ENUM_NONE;
				PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGStateIdx[i], &PGState, PROFILE_STATE_OOS);
				PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_OOS);
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
				if (i < 1)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetTypeIdx[i], &PGType))
						return;
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetModeIdx[i], &PGMode))
						return;
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[i], &PGPrimaryODUk))
						return;
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU2PGSetProtectODUkIdx[i], &PGProtectODUk))
						return;
					if ((PGType == PROFILE_ADM10_ODU2_PG_SET_TYPE_ENUM_DISABLED) ||
							(PGPrimaryODUk == PROFILE_ADM10_ODU2_PG_SET_PRIMARY_ODUK_ENUM_NONE) ||
							(PGProtectODUk == PROFILE_ADM10_ODU2_PG_SET_PROTECT_ODUK_ENUM_NONE))
					{
						PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_NORMAL;
						PGActiveODUk = PROFILE_ADM10_ODU2_PG_ACTIVE_ODUK_ENUM_NONE;
						PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGStateIdx[i], &PGState, PROFILE_STATE_OOS);
						PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_OOS);
					}
					else
					{
						PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_NORMAL;
						PGActiveODUk = PROFILE_ADM10_ODU2_PG_ACTIVE_ODUK_ENUM_NONE;
						
						FPGAPGStatus = fpga_line_agg_2x10_bs.AGG[i].__Status;						
						if (PGMode == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_AUTO) /*Auto*/
						{
							if (FPGAPGStatus & (0x03 << 1)) /*SF*/
								PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_SIGNAL_FAIL;
							else if (FPGAPGStatus & 0x01) /*SD*/
								PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_SIGNAL_DEGRADE;
						}
						else if (PGMode == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
						{
							PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_LOCKOUT;
						}
						else if (PGMode == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_FORCED) /*Forced*/
						{
							PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_FORCED;
						}
						else if (PGMode == PROFILE_ADM10_ODU2_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
						{
							PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_MANUAL;
						}
					
						if (FPGAPGStatus & (0x01 << 3))
							PGActiveODUk = PGProtectODUk;
						else
							PGActiveODUk = PGPrimaryODUk;						
					}					
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGStateIdx[i], &PGState, PROFILE_STATE_IS);
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_IS);
				}
				else
				{
					PGState = PROFILE_ADM10_ODU2_PG_STATE_ENUM_NORMAL;
					PGActiveODUk = PROFILE_ADM10_ODU2_PG_ACTIVE_ODUK_ENUM_NONE;
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGStateIdx[i], &PGState, PROFILE_STATE_OOS);
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU2PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_OOS);
				}				
				break;
		}
	}
}

static void PROFILE_ADM10_ODU1PG_UpdateValues(PROFILE_T *Profile)
{
	uint32_t Mode;
	uint32_t PGType;
	uint32_t PGMode;
	uint32_t PGState;
	uint32_t PGPrimaryODUk;
	uint32_t PGProtectODUk;
	uint32_t PGActiveODUk;
	uint32_t PrimaryLineIdx, PrimaryODU1Idx;
	uint32_t ODU1XCSrc, ODU1XCDst;
	uint32_t Target, Client;
	uint8_t FPGAPGStatus;
	uint32_t i, j;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return;
	
	for (i = 0; i < PROFILE_ADM10_ODU1_PGS; i++)
	{
		switch (Mode)
		{
			case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetTypeIdx[i], &PGType))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetModeIdx[i], &PGMode))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], &PGPrimaryODUk))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], &PGProtectODUk))
					return;
				if ((PGType == PROFILE_ADM10_ODU1_PG_SET_TYPE_ENUM_DISABLED) ||
						(PGPrimaryODUk == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_NONE) ||
						(PGProtectODUk == PROFILE_ADM10_ODU1_PG_SET_PROTECT_ODUK_ENUM_NONE))
				{
					PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_NORMAL;
					PGActiveODUk = PROFILE_ADM10_ODU1_PG_ACTIVE_ODUK_ENUM_NONE;
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU1PGStateIdx[i], &PGState, PROFILE_STATE_OOS);
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU1PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_OOS);
				}
				else
				{
					PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_NORMAL;
					PGActiveODUk = PROFILE_ADM10_ODU1_PG_ACTIVE_ODUK_ENUM_NONE;
					
					PrimaryLineIdx = (PGPrimaryODUk - 1) / 4;
					PrimaryODU1Idx = (PGPrimaryODUk - 1) % 4;
					
					for (j = 0; j < PROFILE_ADM10_ODU1_XCS; j++)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[j], &ODU1XCSrc))
							return;
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[j], &ODU1XCDst))
							return;
						if (ODU1XCSrc == PROFILE_ADM10_XC_LINE_ODU1(PrimaryLineIdx, PrimaryODU1Idx))
						{
							Target = ODU1XCDst;
							break;
						}
						if (ODU1XCDst == PROFILE_ADM10_XC_LINE_ODU1(PrimaryLineIdx, PrimaryODU1Idx))
						{
							Target = ODU1XCSrc;
							break;
						}
					}
					
					if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU1(0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU1(15)))
					{
						Client = Target - 1025;
						
						FPGAPGStatus = FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_STATUS_GET(&fpga_line_adm_10, Client);						
						if (PGMode == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_AUTO) /*Auto*/
						{
							if (FPGAPGStatus & (0x03 << 1)) /*SF*/
								PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_SIGNAL_FAIL;
							else if (FPGAPGStatus & 0x01) /*SD*/
								PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_SIGNAL_DEGRADE;
						}
						else if (PGMode == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
						{
							PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_LOCKOUT;
						}
						else if (PGMode == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_FORCED) /*Forced*/
						{
							PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_FORCED;
						}
						else if (PGMode == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
						{
							PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_MANUAL;
						}
						
						if (FPGAPGStatus & (0x01 << 3))
							PGActiveODUk = PGProtectODUk;
						else
							PGActiveODUk = PGPrimaryODUk;					
					}					
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU1PGStateIdx[i], &PGState, PROFILE_STATE_IS);
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU1PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_IS);
				}
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
				PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_NORMAL;
				PGActiveODUk = PROFILE_ADM10_ODU1_PG_ACTIVE_ODUK_ENUM_NONE;
				PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU1PGStateIdx[i], &PGState, PROFILE_STATE_OOS);
				PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU1PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_OOS);
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetTypeIdx[i], &PGType))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetModeIdx[i], &PGMode))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[i], &PGPrimaryODUk))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1PGSetProtectODUkIdx[i], &PGProtectODUk))
					return;
				if ((PGType == PROFILE_ADM10_ODU1_PG_SET_TYPE_ENUM_DISABLED) ||
						(PGPrimaryODUk == PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_NONE) ||
						(PGProtectODUk == PROFILE_ADM10_ODU1_PG_SET_PROTECT_ODUK_ENUM_NONE))
				{
					PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_NORMAL;
					PGActiveODUk = PROFILE_ADM10_ODU1_PG_ACTIVE_ODUK_ENUM_NONE;
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU1PGStateIdx[i], &PGState, PROFILE_STATE_OOS);
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU1PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_OOS);
				}
				else
				{
					PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_NORMAL;
					PGActiveODUk = PROFILE_ADM10_ODU1_PG_ACTIVE_ODUK_ENUM_NONE;
					
					PrimaryLineIdx = (PGPrimaryODUk - PROFILE_ADM10_ODU1_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU1) / 2;
					
					Target = 0xFFFFFFFF;
					
					for (j = 0; j < PROFILE_ADM10_ODU1_XCS; j++)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[j], &ODU1XCSrc))
							return;
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[j], &ODU1XCDst))
							return;
						if (ODU1XCSrc == PROFILE_ADM10_XC_CLIENT_ODU1(8 + PrimaryLineIdx))
						{
							Target = ODU1XCDst;
							break;
						}
						if (ODU1XCDst == PROFILE_ADM10_XC_CLIENT_ODU1(8 + PrimaryLineIdx))
						{
							Target = ODU1XCSrc;
							break;
						}
					}
					
					if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU1(8)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU1(15)))
					{
						Client = (Target - PROFILE_ADM10_XC_CLIENT_ODU1(8)) / 2;
						
						FPGAPGStatus = FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_STATUS_GET(&fpga_client_adm_2_5[1], Client);						
						if (PGMode == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_AUTO) /*Auto*/
						{
							if (FPGAPGStatus & (0x03 << 1)) /*SF*/
								PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_SIGNAL_FAIL;
							else if (FPGAPGStatus & 0x01) /*SD*/
								PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_SIGNAL_DEGRADE;
						}
						else if (PGMode == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
						{
							PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_LOCKOUT;
						}
						else if (PGMode == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_FORCED) /*Forced*/
						{
							PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_FORCED;
						}
						else if (PGMode == PROFILE_ADM10_ODU1_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
						{
							PGState = PROFILE_ADM10_ODU1_PG_STATE_ENUM_MANUAL;
						}
						
						if (FPGAPGStatus & (0x01 << 3))
							PGActiveODUk = PGProtectODUk;
						else
							PGActiveODUk = PGPrimaryODUk;					
					}					
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU1PGStateIdx[i], &PGState, PROFILE_STATE_IS);
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU1PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_IS);
				}
				break;
		}
	}	
}

static void PROFILE_ADM10_ODU0PG_UpdateValues(PROFILE_T *Profile)
{
	uint32_t Mode;
	uint32_t PGType;
	uint32_t PGMode;
	uint32_t PGState;
	uint32_t PGPrimaryODUk;
	uint32_t PGProtectODUk;
	uint32_t PGActiveODUk;
	uint32_t PrimaryLineIdx, PrimaryODU1Idx, PrimaryODU0Idx;
	uint32_t ODU0XCSrc, ODU0XCDst;
	uint32_t Target, Client;
	uint8_t FPGAPGStatus;
	uint32_t i, j;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return;
	
	for (i = 0; i < PROFILE_ADM10_ODU0_PGS; i++)
	{
		switch (Mode)
		{
			case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetTypeIdx[i], &PGType))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetModeIdx[i], &PGMode))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i], &PGPrimaryODUk))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[i], &PGProtectODUk))
					return;
				if ((PGType == PROFILE_ADM10_ODU0_PG_SET_TYPE_ENUM_DISABLED) ||
						(PGPrimaryODUk == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_NONE) ||
						(PGProtectODUk == PROFILE_ADM10_ODU0_PG_SET_PROTECT_ODUK_ENUM_NONE))
				{
					PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_NORMAL;
					PGActiveODUk = PROFILE_ADM10_ODU0_PG_ACTIVE_ODUK_ENUM_NONE;
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU0PGStateIdx[i], &PGState, PROFILE_STATE_OOS);
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU0PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_OOS);
				}
				else
				{
					PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_NORMAL;
					PGActiveODUk = PROFILE_ADM10_ODU0_PG_ACTIVE_ODUK_ENUM_NONE;
					
					PrimaryLineIdx = (PGPrimaryODUk - 1) / 8;
					PrimaryODU1Idx = ((PGPrimaryODUk - 1) % 8) / 2;
					PrimaryODU0Idx = ((PGPrimaryODUk - 1) % 8) % 2;
					
					for (j = 0; j < PROFILE_ADM10_ODU0_XCS; j++)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[j], &ODU0XCSrc))
							return;
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[j], &ODU0XCDst))
							return;
						if (ODU0XCSrc == PROFILE_ADM10_XC_LINE_ODU0(PrimaryLineIdx, (PrimaryODU1Idx*2) + PrimaryODU0Idx))
						{
							Target = ODU0XCDst;
							break;
						}
						if (ODU0XCDst == PROFILE_ADM10_XC_LINE_ODU0(PrimaryLineIdx, (PrimaryODU1Idx*2) + PrimaryODU0Idx))
						{
							Target = ODU0XCSrc;
							break;
						}
					}
					
					if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU0(0, 0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU0(15, 1)))
					{
						Client = (Target - 1025) / 2;
						
						FPGAPGStatus = FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_STATUS_GET(&fpga_line_adm_10, Client);						
						if (PGMode == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_AUTO) /*Auto*/
						{
							if (FPGAPGStatus & (0x03 << 1)) /*SF*/
								PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_SIGNAL_FAIL;
							else if (FPGAPGStatus & 0x01) /*SD*/
								PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_SIGNAL_DEGRADE;
						}
						else if (PGMode == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
						{
							PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_LOCKOUT;
						}
						else if (PGMode == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_FORCED) /*Forced*/
						{
							PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_FORCED;
						}
						else if (PGMode == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
						{
							PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_MANUAL;
						}
						
						if (FPGAPGStatus & (0x01 << 3))
							PGActiveODUk = PGProtectODUk;
						else
							PGActiveODUk = PGPrimaryODUk;						
					}					
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU0PGStateIdx[i], &PGState, PROFILE_STATE_IS);
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU0PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_IS);
				}
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS:
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
				PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_NORMAL;
				PGActiveODUk = PROFILE_ADM10_ODU0_PG_ACTIVE_ODUK_ENUM_NONE;
				PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU0PGStateIdx[i], &PGState, PROFILE_STATE_OOS);
				PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU0PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_OOS);
				break;
			case PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5:
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetTypeIdx[i], &PGType))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetModeIdx[i], &PGMode))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[i], &PGPrimaryODUk))
					return;
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0PGSetProtectODUkIdx[i], &PGProtectODUk))
					return;
				if ((PGType == PROFILE_ADM10_ODU0_PG_SET_TYPE_ENUM_DISABLED) ||
						(PGPrimaryODUk == PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_NONE) ||
						(PGProtectODUk == PROFILE_ADM10_ODU0_PG_SET_PROTECT_ODUK_ENUM_NONE))
				{
					PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_NORMAL;
					PGActiveODUk = PROFILE_ADM10_ODU0_PG_ACTIVE_ODUK_ENUM_NONE;
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU0PGStateIdx[i], &PGState, PROFILE_STATE_OOS);
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU0PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_OOS);
				}
				else
				{
					PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_NORMAL;
					PGActiveODUk = PROFILE_ADM10_ODU0_PG_ACTIVE_ODUK_ENUM_NONE;
					
					PrimaryLineIdx = ((PGPrimaryODUk - PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU0_1) / 2) / 2;
					PrimaryODU0Idx = (PGPrimaryODUk - PROFILE_ADM10_ODU0_PG_SET_PRIMARY_ODUK_ENUM_CL_9_ODU0_1) % 2;
					
					Target = 0xFFFFFFFF;
					
					for (j = 0; j < PROFILE_ADM10_ODU0_XCS; j++)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[j], &ODU0XCSrc))
							return;
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[j], &ODU0XCDst))
							return;
						if (ODU0XCSrc == PROFILE_ADM10_XC_CLIENT_ODU0(8 + PrimaryLineIdx, PrimaryODU0Idx))
						{
							Target = ODU0XCDst;
							break;
						}
						if (ODU0XCDst == PROFILE_ADM10_XC_CLIENT_ODU0(8 + PrimaryLineIdx, PrimaryODU0Idx))
						{
							Target = ODU0XCSrc;
							break;
						}
					}
					
					if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU0(15, 0)))
					{
						Client = ((Target - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) / 2) / 2;
						
						FPGAPGStatus = FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_STATUS_GET(&fpga_client_adm_2_5[1], Client);						
						if (PGMode == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_AUTO) /*Auto*/
						{
							if (FPGAPGStatus & (0x03 << 1)) /*SF*/
								PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_SIGNAL_FAIL;
							else if (FPGAPGStatus & 0x01) /*SD*/
								PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_SIGNAL_DEGRADE;
						}
						else if (PGMode == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_LOCKOUT) /*Lockout*/
						{
							PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_LOCKOUT;
						}
						else if (PGMode == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_FORCED) /*Forced*/
						{
							PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_FORCED;
						}
						else if (PGMode == PROFILE_ADM10_ODU0_PG_SET_MODE_ENUM_MANUAL) /*Manual*/
						{
							PGState = PROFILE_ADM10_ODU0_PG_STATE_ENUM_MANUAL;
						}
						
						if (FPGAPGStatus & (0x01 << 3))
							PGActiveODUk = PGProtectODUk;
						else
							PGActiveODUk = PGPrimaryODUk;					
					}					
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU0PGStateIdx[i], &PGState, PROFILE_STATE_IS);
					PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADM10DynamicODU0PGActiveODUkIdx[i], &PGActiveODUk, PROFILE_STATE_IS);
				}
				break;
		}
	}	
}

void PROFILE_ADM10_PG_UpdateValues(PROFILE_T *Profile)
{
	PROFILE_ADM10_ODU2PG_UpdateValues(Profile);
	PROFILE_ADM10_ODU1PG_UpdateValues(Profile);
	PROFILE_ADM10_ODU0PG_UpdateValues(Profile);
}

