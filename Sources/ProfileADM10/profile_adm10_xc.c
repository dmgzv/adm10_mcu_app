#include "profile_adm10_xc.h"

uint32_t ProfileADM10ConfigODU1XCSetSrcIdx[PROFILE_ADM10_ODU1_XCS] = {
	PROFILE_ADM10_ODU1_XC_1_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_2_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_3_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_4_SETSRC_IDX,
	PROFILE_ADM10_ODU1_XC_5_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_6_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_7_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_8_SETSRC_IDX
};

uint32_t ProfileADM10ConfigODU1XCSetDstIdx[PROFILE_ADM10_ODU1_XCS] = {
	PROFILE_ADM10_ODU1_XC_1_SETDST_IDX, PROFILE_ADM10_ODU1_XC_2_SETDST_IDX, PROFILE_ADM10_ODU1_XC_3_SETDST_IDX, PROFILE_ADM10_ODU1_XC_4_SETDST_IDX,
	PROFILE_ADM10_ODU1_XC_5_SETDST_IDX, PROFILE_ADM10_ODU1_XC_6_SETDST_IDX, PROFILE_ADM10_ODU1_XC_7_SETDST_IDX, PROFILE_ADM10_ODU1_XC_8_SETDST_IDX
};

uint32_t ProfileADM10ConfigODU1XCSetFactoryIdx[PROFILE_ADM10_ODU1_XCS] = {
	PROFILE_ADM10_ODU1_XC_1_SETFACTORY_IDX, PROFILE_ADM10_ODU1_XC_2_SETFACTORY_IDX, PROFILE_ADM10_ODU1_XC_3_SETFACTORY_IDX, PROFILE_ADM10_ODU1_XC_4_SETFACTORY_IDX,
	PROFILE_ADM10_ODU1_XC_5_SETFACTORY_IDX, PROFILE_ADM10_ODU1_XC_6_SETFACTORY_IDX, PROFILE_ADM10_ODU1_XC_7_SETFACTORY_IDX, PROFILE_ADM10_ODU1_XC_8_SETFACTORY_IDX,
};

uint32_t ProfileADM10ConfigODU0XCSetSrcIdx[PROFILE_ADM10_ODU0_XCS] = {
	PROFILE_ADM10_ODU0_XC_1_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_2_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_3_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_4_SETSRC_IDX,
	PROFILE_ADM10_ODU0_XC_5_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_6_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_7_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_8_SETSRC_IDX,
	PROFILE_ADM10_ODU0_XC_9_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_10_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_11_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_12_SETSRC_IDX,
	PROFILE_ADM10_ODU0_XC_13_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_14_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_15_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_16_SETSRC_IDX
};

uint32_t ProfileADM10ConfigODU0XCSetDstIdx[PROFILE_ADM10_ODU0_XCS] = {
	PROFILE_ADM10_ODU0_XC_1_SETDST_IDX, PROFILE_ADM10_ODU0_XC_2_SETDST_IDX, PROFILE_ADM10_ODU0_XC_3_SETDST_IDX, PROFILE_ADM10_ODU0_XC_4_SETDST_IDX,
	PROFILE_ADM10_ODU0_XC_5_SETDST_IDX, PROFILE_ADM10_ODU0_XC_6_SETDST_IDX, PROFILE_ADM10_ODU0_XC_7_SETDST_IDX, PROFILE_ADM10_ODU0_XC_8_SETDST_IDX,
	PROFILE_ADM10_ODU0_XC_9_SETDST_IDX, PROFILE_ADM10_ODU0_XC_10_SETDST_IDX, PROFILE_ADM10_ODU0_XC_11_SETDST_IDX, PROFILE_ADM10_ODU0_XC_12_SETDST_IDX,
	PROFILE_ADM10_ODU0_XC_13_SETDST_IDX, PROFILE_ADM10_ODU0_XC_14_SETDST_IDX, PROFILE_ADM10_ODU0_XC_15_SETDST_IDX, PROFILE_ADM10_ODU0_XC_16_SETDST_IDX
};

uint32_t ProfileADM10ConfigODU0XCSetFactoryIdx[PROFILE_ADM10_ODU0_XCS] = {
	PROFILE_ADM10_ODU0_XC_1_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_2_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_3_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_4_SETFACTORY_IDX,
	PROFILE_ADM10_ODU0_XC_5_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_6_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_7_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_8_SETFACTORY_IDX,
	PROFILE_ADM10_ODU0_XC_9_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_10_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_11_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_12_SETFACTORY_IDX,
	PROFILE_ADM10_ODU0_XC_13_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_14_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_15_SETFACTORY_IDX, PROFILE_ADM10_ODU0_XC_16_SETFACTORY_IDX
};

extern const uint32_t ProfileADM10ConfigClSetDataTypeIdx[PROFILE_ADM10_CLIENT_PORTS];
extern const uint32_t ProfileADM10ConfigLnOPU1SetPayloadIdx[PROFILE_ADM10_LINE_PORTS][4];

static int32_t PROFILE_ADM10_XC_CheckODU1XCSetSrcValue(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t ODU1XCIdx)
{
	uint32_t i;
	uint32_t Src, Dst;
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			if (*(uint32_t*)Value == 0)
			{
				/*Always allow to set XC source to None*/
				return 0;
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_LINE_ODU1(0, 0)) && (*(uint32_t*)Value <= PROFILE_ADM10_XC_LINE_ODU1(1, 3)))
			{
				/*Allow to set XC source to Line 1 nad 2 ODU1 containers only*/
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_CLIENT_ODU1(0)) && (*(uint32_t*)Value <= PROFILE_ADM10_XC_CLIENT_ODU1(15)) && (((*(uint32_t*)Value - PROFILE_ADM10_XC_CLIENT_ODU1(0)) % 2) == 0))
			{
				/*Allow to set XC destination to Client even ODU0 containers only*/
			}
			else
			{
				return -1;
			}
			for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
			{
				/*Check that there is no XC with same ODU container as source*/
				if (i != ODU1XCIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], &Src))
						return -1;
					if (Src == *(uint32_t*)Value)
						return -1;
				}
				/*Check that there is no XC with same ODU container as desctination*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], &Dst))
					return -1;
				if (Dst == *(uint32_t*)Value)
					return -1;	
			}
			break;
		case 3:
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Src))
				return -1;
			if (Src != *(uint32_t*)Value)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

static int32_t PROFILE_ADM10_XC_CheckODU1XCSetDstValue(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t ODU1XCIdx)
{
	uint32_t i;
	uint32_t Src, Dst;
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			if (*(uint32_t*)Value == 0)
			{
				/*Always allow to set XC source to None*/
				return 0;
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_LINE_ODU1(0, 0)) && (*(uint32_t*)Value <= PROFILE_ADM10_XC_LINE_ODU1(1, 3)))
			{
				/*Allow to set XC source to Line 1 nad 2 ODU0 containers only*/
			}
			else
			{
				return -1;
			}
			for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
			{
				/*Check that there is no XC with same ODU container as source*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], &Src))
					return -1;
				if (Src == *(uint32_t*)Value)
					return -1;
				/*Check that there is no XC with same ODU container as desctination*/
				if (i != ODU1XCIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], &Dst))
						return -1;
					if (Dst == *(uint32_t*)Value)
						return -1;
				}					
			}
			break;
		case 3:
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Dst))
				return -1;
			if (Dst != *(uint32_t*)Value)
				return -1;
		default:
			return -1;
	}	
	return 0;
}

static int32_t PROFILE_ADM10_XC_CheckODU0XCSetSrcValue(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t ODU0XCIdx)
{
	uint32_t i;
	uint32_t Src, Dst;
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			if (*(uint32_t*)Value == 0)
			{
				/*Always allow to set XC source to None*/
				return 0;
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_LINE_ODU0(0, 0)) && (*(uint32_t*)Value <= PROFILE_ADM10_XC_LINE_ODU0(1, 7)))
			{
				/*Allow to set XC source to Line 1 nad 2 ODU0 containers only*/
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_CLIENT_ODU0(0)) && (*(uint32_t*)Value <= PROFILE_ADM10_XC_CLIENT_ODU0(15)))
			{
				/*Allow to set XC destination to Client even ODU0 containers only*/
			}
			else
			{
				return -1;
			}
			for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
			{
				/*Check that there is no XC with same ODU container as source*/
				if (i != ODU0XCIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], &Src))
						return -1;
					if (Src == *(uint32_t*)Value)
						return -1;
				}
				/*Check that there is no XC with same ODU container as desctination*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], &Dst))
					return -1;
				if (Dst == *(uint32_t*)Value)
					return -1;	
			}
			break;
		case 3:
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Src))
				return -1;
			if (Src != *(uint32_t*)Value)
				return -1;
			break;
		default:
			return -1;
	}	
	return 0;
}

static int32_t PROFILE_ADM10_XC_CheckODU0XCSetDstValue(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t ODU0XCIdx)
{
	uint32_t i;
	uint32_t Src, Dst;
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			if (*(uint32_t*)Value == 0)
			{
				/*Always allow to set XC source to None*/
				return 0;
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_LINE_ODU0(0, 0)) && (*(uint32_t*)Value <= PROFILE_ADM10_XC_LINE_ODU0(1, 7)))
			{
				/*Allow to set XC source to Line 1 nad 2 ODU0 containers only*/
			}
			else
			{
				return -1;
			}
			for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
			{
				/*Check that there is no XC with same ODU container as source*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], &Src))
					return -1;
				if (Src == *(uint32_t*)Value)
					return -1;
				/*Check that there is no XC with same ODU container as desctination*/
				if (i != ODU0XCIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], &Dst))
						return -1;
					if (Dst == *(uint32_t*)Value)
						return -1;
				}					
			}
			break;
		case 3:
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Dst))
				return -1;
			if (Dst != *(uint32_t*)Value)
				return -1;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_XC_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
	{
		if (Index == ProfileADM10ConfigODU1XCSetSrcIdx[i])
		{
			if (PROFILE_ADM10_XC_CheckODU1XCSetSrcValue(Profile, Table, Index, Value, i))
				return -1;
			return 0;
		}
		else if (Index == ProfileADM10ConfigODU1XCSetDstIdx[i])
		{
			if (PROFILE_ADM10_XC_CheckODU1XCSetDstValue(Profile, Table, Index, Value, i))
				return -1;
			return 0;
		}
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
	{
		if (Index == ProfileADM10ConfigODU0XCSetSrcIdx[i])
		{
			if (PROFILE_ADM10_XC_CheckODU0XCSetSrcValue(Profile, Table, Index, Value, i))
				return -1;
			return 0;
		}
		else if (Index == ProfileADM10ConfigODU0XCSetDstIdx[i])
		{
			if (PROFILE_ADM10_XC_CheckODU0XCSetDstValue(Profile, Table, Index, Value, i))
				return -1;
			return 0;
		}
	}
	
	return 0;
}

const uint32_t ODU1XCSrcFactory[PROFILE_ADM10_ODU1_XCS] = {
	PROFILE_ADM10_XC_CLIENT_ODU1(0), PROFILE_ADM10_XC_CLIENT_ODU1(2), PROFILE_ADM10_XC_CLIENT_ODU1(4), PROFILE_ADM10_XC_CLIENT_ODU1(6),
	PROFILE_ADM10_XC_CLIENT_ODU1(8), PROFILE_ADM10_XC_CLIENT_ODU1(10), PROFILE_ADM10_XC_CLIENT_ODU1(12), PROFILE_ADM10_XC_CLIENT_ODU1(14), 
};

const uint32_t ODU1XCDstFactory[PROFILE_ADM10_ODU1_XCS] = {
	PROFILE_ADM10_XC_LINE_ODU1(0, 0), PROFILE_ADM10_XC_LINE_ODU1(0, 1), PROFILE_ADM10_XC_LINE_ODU1(0, 2), PROFILE_ADM10_XC_LINE_ODU1(0, 3),
	PROFILE_ADM10_XC_LINE_ODU1(1, 0), PROFILE_ADM10_XC_LINE_ODU1(1, 1), PROFILE_ADM10_XC_LINE_ODU1(1, 2), PROFILE_ADM10_XC_LINE_ODU1(1, 3), 
};

const uint32_t ODU0XCSrcFactory[PROFILE_ADM10_ODU0_XCS] = {
	PROFILE_ADM10_XC_CLIENT_ODU0(0), PROFILE_ADM10_XC_CLIENT_ODU0(1), PROFILE_ADM10_XC_CLIENT_ODU0(2), PROFILE_ADM10_XC_CLIENT_ODU0(3),
	PROFILE_ADM10_XC_CLIENT_ODU0(4), PROFILE_ADM10_XC_CLIENT_ODU0(5), PROFILE_ADM10_XC_CLIENT_ODU0(6), PROFILE_ADM10_XC_CLIENT_ODU0(7),
	PROFILE_ADM10_XC_CLIENT_ODU0(8), PROFILE_ADM10_XC_CLIENT_ODU0(9), PROFILE_ADM10_XC_CLIENT_ODU0(10), PROFILE_ADM10_XC_CLIENT_ODU0(11),
	PROFILE_ADM10_XC_CLIENT_ODU0(12), PROFILE_ADM10_XC_CLIENT_ODU0(13), PROFILE_ADM10_XC_CLIENT_ODU0(14), PROFILE_ADM10_XC_CLIENT_ODU0(15), 
};

const uint32_t ODU0XCDstFactory[PROFILE_ADM10_ODU0_XCS] = {
	PROFILE_ADM10_XC_LINE_ODU0(0, 0), PROFILE_ADM10_XC_LINE_ODU0(0, 1), PROFILE_ADM10_XC_LINE_ODU0(0, 2), PROFILE_ADM10_XC_LINE_ODU0(0, 3),
	PROFILE_ADM10_XC_LINE_ODU0(0, 4), PROFILE_ADM10_XC_LINE_ODU0(0, 5), PROFILE_ADM10_XC_LINE_ODU0(0, 6), PROFILE_ADM10_XC_LINE_ODU0(0, 7),
	PROFILE_ADM10_XC_LINE_ODU0(1, 0), PROFILE_ADM10_XC_LINE_ODU0(1, 1), PROFILE_ADM10_XC_LINE_ODU0(1, 2), PROFILE_ADM10_XC_LINE_ODU0(1, 3),
	PROFILE_ADM10_XC_LINE_ODU0(1, 4), PROFILE_ADM10_XC_LINE_ODU0(1, 5), PROFILE_ADM10_XC_LINE_ODU0(1, 6), PROFILE_ADM10_XC_LINE_ODU0(1, 7),
};

static int32_t PROFILE_ADM10_XC_SetModeValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value)
{
	uint32_t i;
	
	switch (*(uint32_t*)Value)
	{
		case 0:
			/*
			for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
			{
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i]);
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i]);
			}
			for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
			{
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i]);
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i]);
			}
			*/
			break;
		case 3:
			for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
			{
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], (uint32_t*)&ODU1XCSrcFactory[i]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], (uint32_t*)&ODU1XCDstFactory[i]);
			}
			for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
			{
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], (uint32_t*)&ODU0XCSrcFactory[i]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], (uint32_t*)&ODU0XCDstFactory[i]);
			}
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t PROFILE_ADM10_XC_SetODU1FactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t ODU1XCIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[ODU1XCIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[ODU1XCIdx]);
			break;
		case 3:
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[ODU1XCIdx], (uint32_t*)&ODU0XCSrcFactory[ODU1XCIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[ODU1XCIdx], (uint32_t*)&ODU0XCDstFactory[ODU1XCIdx]);
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t PROFILE_ADM10_XC_SetODU0FactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t ODU0XCIdx)
{
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case 0:
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[ODU0XCIdx]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[ODU0XCIdx]);
			break;
		case 3:
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[ODU0XCIdx], (uint32_t*)&ODU0XCSrcFactory[ODU0XCIdx]);
			PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[ODU0XCIdx], (uint32_t*)&ODU0XCDstFactory[ODU0XCIdx]);
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_ADM10_XC_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value)
{
	uint32_t i;
	
	if (Index == PROFILE_ADM10_SETMODE_IDX)
	{
		if (PROFILE_ADM10_XC_SetModeValueDependencies(Profile, Table, Index, Value))
			return -1;
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
	{
		if (Index == ProfileADM10ConfigODU1XCSetFactoryIdx[i])
			PROFILE_ADM10_XC_SetODU1FactoryValueDependencies(Profile, Table, Index, Value, i);
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
	{
		if (Index == ProfileADM10ConfigODU0XCSetFactoryIdx[i])
			PROFILE_ADM10_XC_SetODU0FactoryValueDependencies(Profile, Table, Index, Value, i);
	}
	
	return 0;
}

void PROFILE_ADM10_XC_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t i, j;
	uint32_t Mode;
	uint32_t ODU1Mux, ODU0Mux;
	uint32_t Target, TargetLine, TargetODU1Idx, TargetODU0Idx, TargetClient;
	uint32_t Line, Client, ODU1Idx, ODU0Idx;
	uint32_t LnOPU1Payload[PROFILE_ADM10_LINE_PORTS][4];
	uint32_t ClDataType[PROFILE_ADM10_CLIENT_PORTS];
	uint32_t XCODU1Src[PROFILE_ADM10_ODU1_XCS], XCODU1Dst[PROFILE_ADM10_ODU1_XCS];
	uint32_t XCODU0Src[PROFILE_ADM10_ODU0_XCS], XCODU0Dst[PROFILE_ADM10_ODU0_XCS];
	uint32_t __ODU0Mux, __ODU1Mux, __OPU0PayloadType, __OPU1PayloadType;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return;
	
	for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], &XCODU1Src[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], &XCODU1Dst[i]))
			return;
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], &XCODU0Src[i]))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], &XCODU0Dst[i]))
			return;
	}
	
	for (i = 0; i < PROFILE_ADM10_LINE_PORTS; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigLnOPU1SetPayloadIdx[i][j], &LnOPU1Payload[i][j]))
				return;
		}
	}
	
	for (i = 0; i < PROFILE_ADM10_CLIENT_PORTS; i++)
	{
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigClSetDataTypeIdx[i], &ClDataType[i]))
			return;
	}	
	
	if (Mode == 0)
	{
		for (i = 0; i < 8; i++)
		{
			BSP_FPGA_LINE_MUX_ODU12_SET(i, BSP_FPGA_LINE_MUX_ODU12_ODU1(i));
			BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_SET(i, BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_NULL);
		}
		
		for (i = 0; i < 16; i++)
		{
			BSP_FPGA_LINE_MUX_ODU01_SET(i, BSP_FPGA_LINE_MUX_ODU01_ODU0(i));
			BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_SET(i, BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_NULL);
		}
		
		for(Line = 0; Line < 2; Line++)
		{
			for (ODU1Idx = 0; ODU1Idx < 4; ODU1Idx++)
			{
				Target = 0;
				for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
				{
					if (XCODU1Src[i] == PROFILE_ADM10_XC_LINE_ODU1(Line, ODU1Idx))
					{
						Target = XCODU1Dst[i];
						break;
					}
					if (XCODU1Dst[i] == PROFILE_ADM10_XC_LINE_ODU1(Line, ODU1Idx))
					{
						Target = XCODU1Src[i];
						break;
					}
				}
				
				if (LnOPU1Payload[Line][ODU1Idx] == 0 /*Any*/)
				{
					if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU1(0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU1(15)))
					{
						/*Target is client ODU1 container*/
						TargetClient = Target - 1025;
						ODU1Mux = TargetClient / 2;
						
						for (i = 0; i < 8; i++)
						{
							__ODU1Mux = BSP_FPGA_LINE_MUX_ODU12_GET(i) - 8;
							__OPU1PayloadType = BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_GET(i);
							if (__ODU1Mux == ODU1Mux)
								break;
						}
						if (i != ((Line * 4) + ODU1Idx))
						{
							__ODU1Mux = BSP_FPGA_LINE_MUX_ODU12_GET(((Line * 4) + ODU1Idx)) - 8;
							__OPU1PayloadType = BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_GET(ODU1Mux);
							BSP_FPGA_LINE_MUX_ODU12_SET(i, BSP_FPGA_LINE_MUX_ODU12_ODU1(__ODU1Mux));
							BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_SET(__ODU1Mux, __OPU1PayloadType);
						}
						BSP_FPGA_LINE_MUX_ODU12_SET((Line * 4) + ODU1Idx, BSP_FPGA_LINE_MUX_ODU12_ODU1(ODU1Mux));
						if (ClDataType[TargetClient] == 130)
							BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_SET(ODU1Mux, BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_STM16);
					}
				}
			}
		}
		
		for(Line = 0; Line < 2; Line++)
		{
			for (ODU1Idx = 0; ODU1Idx < 4; ODU1Idx++)
			{
				Target = 0;
				for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
				{
					if (XCODU1Src[i] == PROFILE_ADM10_XC_LINE_ODU1(Line, ODU1Idx))
					{
						Target = XCODU1Dst[i];
						break;
					}
					if (XCODU1Dst[i] == PROFILE_ADM10_XC_LINE_ODU1(Line, ODU1Idx))
					{
						Target = XCODU1Src[i];
						break;
					}
				}
				
				if (LnOPU1Payload[Line][ODU1Idx] == 0 /*Any*/)
				{
					if ((Target >= PROFILE_ADM10_XC_LINE_ODU1(0, 0)) && (Target <= PROFILE_ADM10_XC_LINE_ODU1(1, 4)))
					{
						/*Target is line ODU1 container*/
						TargetLine = ((Target - 1) / 4);
						TargetODU1Idx = ((Target - 1) % 4);
						BSP_FPGA_LINE_MUX_ODU12_SET((Line * 4) + ODU1Idx, BSP_FPGA_LINE_MUX_ODU12_LINE(TargetLine, TargetODU1Idx));
					}
				}
			}
		}
		
		for(Line = 0; Line < 2; Line++)
		{
			for (ODU1Idx = 0; ODU1Idx < 4; ODU1Idx++)
			{
				if (LnOPU1Payload[Line][ODU1Idx] == 1 /*2xODU0*/)
				{
					ODU1Mux = BSP_FPGA_LINE_MUX_ODU12_GET((Line * 4) + ODU1Idx) - 8;
					BSP_FPGA_LINE_MUX_ODU12_SET((Line * 4) + ODU1Idx, BSP_FPGA_LINE_MUX_ODU12_ODU1(ODU1Mux));
					BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_SET(ODU1Mux, BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_2xODU0);
					
					for (ODU0Idx = 0; ODU0Idx < 2; ODU0Idx++)
					{
						Target = 0;
						for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
						{
							if (XCODU0Src[i] == PROFILE_ADM10_XC_LINE_ODU0(Line, (ODU1Idx * 2) + ODU0Idx))
							{
								Target = XCODU0Dst[i];
								break;
							}
							if (XCODU0Dst[i] == PROFILE_ADM10_XC_LINE_ODU0(Line, (ODU1Idx * 2) + ODU0Idx))
							{
								Target = XCODU0Src[i];
								break;
							}
						}
						
						if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU0(0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU0(15)))
						{
							/*Target is client ODU0 container*/
							TargetClient = Target - 1025;
							ODU0Mux = TargetClient;
							for (i = 0; i < 16; i++)
							{
								__ODU0Mux = BSP_FPGA_LINE_MUX_ODU01_GET(i) - 0x10;
								if (__ODU0Mux == ODU0Mux)
									break;
							}
							if (i != ((ODU1Mux*2) + ODU0Idx))
							{
								__ODU0Mux = BSP_FPGA_LINE_MUX_ODU01_GET((ODU1Mux*2) + ODU0Idx) - 0x10;
								__OPU0PayloadType = BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_GET(ODU0Mux);
								BSP_FPGA_LINE_MUX_ODU01_SET(i, BSP_FPGA_LINE_MUX_ODU01_ODU0(__ODU0Mux));
								BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_SET(__ODU0Mux, __OPU0PayloadType);
							}
							BSP_FPGA_LINE_MUX_ODU01_SET((ODU1Mux*2) + ODU0Idx, BSP_FPGA_LINE_MUX_ODU01_ODU0(ODU0Mux));
							if (ClDataType[TargetClient] == 257)
								BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_SET(ODU0Mux, BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_1GE);
							else if (ClDataType[TargetClient] == 128)
								BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_SET(ODU0Mux, BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_STM1);
							else if (ClDataType[TargetClient] == 129)
								BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_SET(ODU0Mux, BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_STM4);
						}
					}
					
					for (ODU0Idx = 0; ODU0Idx < 2; ODU0Idx++)
					{
						Target = 0;
						for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
						{
							if (XCODU0Src[i] == PROFILE_ADM10_XC_LINE_ODU0(Line, (ODU1Idx * 2) + ODU0Idx))
							{
								Target = XCODU0Dst[i];
								break;
							}
							if (XCODU0Dst[i] == PROFILE_ADM10_XC_LINE_ODU0(Line, (ODU1Idx * 2) + ODU0Idx))
							{
								Target = XCODU0Src[i];
								break;
							}
						}
						
						if ((Target >= PROFILE_ADM10_XC_LINE_ODU0(0, 0)) && (Target <= PROFILE_ADM10_XC_LINE_ODU0(1, 7)))
						{
							/*Target is line ODU0 container*/
							TargetLine = ((Target - 1) / 8);
							TargetODU0Idx = ((Target - 1) % 8);
							BSP_FPGA_LINE_MUX_ODU01_SET((ODU1Mux*2) + ODU0Idx, BSP_FPGA_LINE_MUX_ODU01_LINE(TargetLine, TargetODU0Idx));
						}
					}
				}
			}
		}

		
		for (Client = 0; Client < PROFILE_ADM10_CLIENT_PORTS; Client++)
		{
			if (ClDataType[Client] == 130)
			{
				Target = 0;
				for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
				{
					if (XCODU1Src[i] == PROFILE_ADM10_XC_CLIENT_ODU1(Client))
					{
						Target = XCODU1Dst[i];
						break;
					}
					if (XCODU1Dst[i] == PROFILE_ADM10_XC_CLIENT_ODU1(Client))
					{
						Target = XCODU1Src[i];
						break;
					}
				}
					
				if ((Target >= PROFILE_ADM10_XC_LINE_ODU1(0, 0)) && (Target <= PROFILE_ADM10_XC_LINE_ODU1(1, 4)))
				{
					TargetLine = ((Target - 1) / 4);
					TargetODU1Idx = ((Target - 1) % 4);
					BSP_FPGA_LINE_DEMUX_CLIENT_ODU1_PG_PRIMARY_ODU1_SET(Client, TargetLine, TargetODU1Idx);
					BSP_FPGA_CLIENT_IFACE_CTRL_SET(Client, BSP_FPGA_CLIENT_IFACE_CTRL_CNTR_EN_MASK);
				}
				else
				{
					BSP_FPGA_CLIENT_IFACE_CTRL_SET(Client, BSP_FPGA_CLIENT_IFACE_CTRL_CNTR_EN_MASK | BSP_FPGA_CLIENT_IFACE_CTRL_AIS_MASK);
				}
			}
			else if ((ClDataType[Client] == 128) || (ClDataType[Client] == 129) || (ClDataType[Client] == 257))
			{
				Target = 0;
				for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
				{
					if (XCODU0Src[i] == PROFILE_ADM10_XC_CLIENT_ODU0(Client))
					{
						Target = XCODU0Dst[i];
						break;
					}
					if (XCODU0Dst[i] == PROFILE_ADM10_XC_CLIENT_ODU0(Client))
					{
						Target = XCODU0Src[i];
						break;
					}
				}					
				if ((Target >= PROFILE_ADM10_XC_LINE_ODU0(0, 0)) && (Target <= PROFILE_ADM10_XC_LINE_ODU0(1, 7)))
				{
					TargetLine = ((Target - 1) / 8);
					TargetODU0Idx = ((Target - 1) % 8);
					BSP_FPGA_LINE_DEMUX_CLIENT_ODU0_PG_PRIMARY_ODU0_SET(Client, TargetLine, TargetODU0Idx);						
					BSP_FPGA_CLIENT_IFACE_CTRL_SET(Client, BSP_FPGA_CLIENT_IFACE_CTRL_CNTR_EN_MASK);
				}
				else
				{
					BSP_FPGA_CLIENT_IFACE_CTRL_SET(Client, BSP_FPGA_CLIENT_IFACE_CTRL_CNTR_EN_MASK | BSP_FPGA_CLIENT_IFACE_CTRL_AIS_MASK);
				}
			}
		}
	}
	else
	{
		/* Cross-connect is autoconfigured in FPGA */
		for (Client = 0; Client < 16; Client++)
		{
			BSP_FPGA_CLIENT_IFACE_CTRL_SET(Client, BSP_FPGA_CLIENT_IFACE_CTRL_CNTR_EN_MASK);
		}
	}
}

void PROFILE_ADM10_XC_UpdateValues(PROFILE_T *Profile)
{
	
}
