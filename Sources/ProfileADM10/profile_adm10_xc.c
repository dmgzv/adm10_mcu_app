#include "profile_adm10_client.h"
#include "profile_adm10_line.h"
#include "profile_adm10_xc.h"
#include "profile_adm10_pg.h"
#include "profile_adm10_enums.h"
#include "profile_adm10_bitmsks.h"

const uint16_t ProfileADM10ConfigODU1XCSetSrcIdx[PROFILE_ADM10_ODU1_XCS] = {
	PROFILE_ADM10_ODU1_XC_1_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_2_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_3_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_4_SETSRC_IDX,
	PROFILE_ADM10_ODU1_XC_5_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_6_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_7_SETSRC_IDX, PROFILE_ADM10_ODU1_XC_8_SETSRC_IDX
};

const uint16_t ProfileADM10ConfigODU1XCSetDstIdx[PROFILE_ADM10_ODU1_XCS] = {
	PROFILE_ADM10_ODU1_XC_1_SETDST_IDX, PROFILE_ADM10_ODU1_XC_2_SETDST_IDX, PROFILE_ADM10_ODU1_XC_3_SETDST_IDX, PROFILE_ADM10_ODU1_XC_4_SETDST_IDX,
	PROFILE_ADM10_ODU1_XC_5_SETDST_IDX, PROFILE_ADM10_ODU1_XC_6_SETDST_IDX, PROFILE_ADM10_ODU1_XC_7_SETDST_IDX, PROFILE_ADM10_ODU1_XC_8_SETDST_IDX
};


const uint16_t ProfileADM10ConfigODU0XCSetSrcIdx[PROFILE_ADM10_ODU0_XCS] = {
	PROFILE_ADM10_ODU0_XC_1_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_2_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_3_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_4_SETSRC_IDX,
	PROFILE_ADM10_ODU0_XC_5_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_6_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_7_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_8_SETSRC_IDX,
	PROFILE_ADM10_ODU0_XC_9_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_10_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_11_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_12_SETSRC_IDX,
	PROFILE_ADM10_ODU0_XC_13_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_14_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_15_SETSRC_IDX, PROFILE_ADM10_ODU0_XC_16_SETSRC_IDX
};

const uint16_t ProfileADM10ConfigODU0XCSetDstIdx[PROFILE_ADM10_ODU0_XCS] = {
	PROFILE_ADM10_ODU0_XC_1_SETDST_IDX, PROFILE_ADM10_ODU0_XC_2_SETDST_IDX, PROFILE_ADM10_ODU0_XC_3_SETDST_IDX, PROFILE_ADM10_ODU0_XC_4_SETDST_IDX,
	PROFILE_ADM10_ODU0_XC_5_SETDST_IDX, PROFILE_ADM10_ODU0_XC_6_SETDST_IDX, PROFILE_ADM10_ODU0_XC_7_SETDST_IDX, PROFILE_ADM10_ODU0_XC_8_SETDST_IDX,
	PROFILE_ADM10_ODU0_XC_9_SETDST_IDX, PROFILE_ADM10_ODU0_XC_10_SETDST_IDX, PROFILE_ADM10_ODU0_XC_11_SETDST_IDX, PROFILE_ADM10_ODU0_XC_12_SETDST_IDX,
	PROFILE_ADM10_ODU0_XC_13_SETDST_IDX, PROFILE_ADM10_ODU0_XC_14_SETDST_IDX, PROFILE_ADM10_ODU0_XC_15_SETDST_IDX, PROFILE_ADM10_ODU0_XC_16_SETDST_IDX
};

static int32_t PROFILE_ADM10_XC_CheckODU1XCSetSrcValue(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t ODU1XCIdx, uint32_t Restore)
{
	uint32_t i;
	uint32_t Src, Dst;
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU1_XC_SET_SRC_ENUM_NONE)
			{
				/*Always allow to set ODU1XC source to None*/
				return 0;
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_LINE_ODU1(0, 0)) &&
				(*(uint32_t*)Value <= PROFILE_ADM10_XC_LINE_ODU1(1, 3)))
			{
				/*Allow to set ODU1XC source to Line 1, 2 ODU1 1, 2, 3, 4 containers only*/
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_CLIENT_ODU1(0)) &&
				(*(uint32_t*)Value <= PROFILE_ADM10_XC_CLIENT_ODU1(15)) &&
				(((*(uint32_t*)Value - PROFILE_ADM10_XC_CLIENT_ODU1(0)) % 2) == 0))
			{
				/*Allow to set ODU1XC source to Client 1, 3, 5, ..., 15 ODU1 containers only*/
			}
			else
			{
				return -1;
			}
			for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
			{
				/*Check that there is no ODU1XC with same ODU1 container as source*/
				if (i != ODU1XCIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], &Src))
						return -1;
					if (Src == *(uint32_t*)Value)
						return -1;
				}
				/*Check that there is no ODU1XC with same ODU1 container as destination*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], &Dst))
					return -1;
				if (Dst == *(uint32_t*)Value)
					return -1;	
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10BS:
			/*ODU1XC source values are fixed*/
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Src))
				return -1;
			if (Src != *(uint32_t*)Value)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Src))
				return -1;
			if (Src != *(uint32_t*)Value)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10BS_ADM_2_5:
			if (Index < 4)
			{
				/*First 4 ODU1XC source values are fixed*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Src))
					return -1;
				if (Src != *(uint32_t*)Value)
					return -1;
			}
			else
			{
				if (*(uint32_t*)Value == PROFILE_ADM10_ODU1_XC_SET_SRC_ENUM_NONE)
				{
					/*Always allow to set ODU1XC source to None*/
					return 0;
				}
				else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_CLIENT_ODU1(8)) &&
					(*(uint32_t*)Value <= PROFILE_ADM10_XC_CLIENT_ODU1(15)) &&
					(((*(uint32_t*)Value - PROFILE_ADM10_XC_CLIENT_ODU1(0)) % 2) != 0))
				{
					/*Allow to set ODU1XC source to Client 9, 11, 13, 15 ODU1 containers only*/
				}
				else
				{
					return -1;
				}
				
				for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
				{
					/*Check that there is no ODU1XC with same ODU1 container as source*/
					if (i != ODU1XCIdx)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], &Src))
							return -1;
						if (Src == *(uint32_t*)Value)
							return -1;
					}
					/*Check that there is no ODU1XC with same ODU1 container as desctination*/
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], &Dst))
						return -1;
					if (Dst == *(uint32_t*)Value)
						return -1;	
				}
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

static int32_t PROFILE_ADM10_XC_CheckODU1XCSetDstValue(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t ODU1XCIdx, uint32_t Restore)
{
	uint32_t i;
	uint32_t Src, Dst;
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU1_XC_SET_DST_ENUM_NONE)
			{
				/*Always allow to set ODU1XC destination to None*/
				return 0;
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_LINE_ODU1(0, 0)) &&
				(*(uint32_t*)Value <= PROFILE_ADM10_XC_LINE_ODU1(1, 3)))
			{
				/*Allow to set ODU1XC destination to Line 1, 2 ODU1 1, 2, 3, 4 containers only*/
			}
			else
			{
				return -1;
			}
			for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
			{
				/*Check that there is no ODU1XC with same ODU1 container as source*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], &Src))
					return -1;
				if (Src == *(uint32_t*)Value)
					return -1;
				/*Check that there is no ODU1XC with same ODU1 container as desctination*/
				if (i != ODU1XCIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], &Dst))
						return -1;
					if (Dst == *(uint32_t*)Value)
						return -1;
				}					
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10BS:
			/*ODU1XC destination values are fixed*/
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Dst))
				return -1;
			if (Dst != *(uint32_t*)Value)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Dst))
				return -1;
			if (Dst != *(uint32_t*)Value)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10BS_ADM_2_5:
			if (Index < 4)
			{
				/*First 4 ODU1XC destination values are fixed*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Dst))
					return -1;
				if (Dst != *(uint32_t*)Value)
					return -1;
			}
			else
			{
				if (*(uint32_t*)Value == PROFILE_ADM10_ODU1_XC_SET_DST_ENUM_NONE)
				{
					/*Always allow to set ODU1XC destination to None*/
					return 0;
				}
				else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_CLIENT_ODU1(8)) &&
					(*(uint32_t*)Value <= PROFILE_ADM10_XC_CLIENT_ODU1(15)) &&
					(((*(uint32_t*)Value - PROFILE_ADM10_XC_CLIENT_ODU1(0)) % 2) == 0))
				{
					/*Allow to set ODU1XC destination to Client 10, 12, 14, 16 ODU1 containers only*/
				}
				else
				{
					return -1;
				}
				
				for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
				{
					/*Check that there is no ODU1XC with same ODU1 container as source*/
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], &Src))
						return -1;
					if (Src == *(uint32_t*)Value)
						return -1;
					/*Check that there is no ODU1XC with same ODU1 container as desctination*/
					if (i != ODU1XCIdx)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], &Dst))
							return -1;
						if (Dst == *(uint32_t*)Value)
							return -1;
					}					
				}
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

static int32_t PROFILE_ADM10_XC_CheckODU0XCSetSrcValue(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t ODU0XCIdx, uint32_t Restore)
{
	uint32_t i;
	uint32_t Src, Dst;
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU0_XC_SET_SRC_ENUM_NONE)
			{
				/*Always allow to set ODU0XC source to None*/
				return 0;
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_LINE_ODU0(0, 0)) &&
				(*(uint32_t*)Value <= PROFILE_ADM10_XC_LINE_ODU0(1, 7)))
			{
				/*Allow to set ODU0XC source to Line 1, 2 ODU0 1, ..., 8 containers only*/
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_CLIENT_ODU0(0, 0)) &&
				(*(uint32_t*)Value <= PROFILE_ADM10_XC_CLIENT_ODU0(15, 1)) &&
				(((*(uint32_t*)Value - PROFILE_ADM10_XC_CLIENT_ODU0(0, 0)) % 2) == 0))
			{
				/*Allow to set ODU0XC source to Client 1, 2, ..., 16 ODU0 1 containers only*/
			}
			else
			{
				return -1;
			}
			for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
			{
				/*Check that there is no ODU0XC with same ODU0 container as source*/
				if (i != ODU0XCIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], &Src))
						return -1;
					if (Src == *(uint32_t*)Value)
						return -1;
				}
				/*Check that there is no ODU0XC with same ODU0 container as destination*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], &Dst))
					return -1;
				if (Dst == *(uint32_t*)Value)
					return -1;	
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10BS:
			/*ODU0XC source is fixed*/
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Src))
				return -1;
			if (Src != *(uint32_t*)Value)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Src))
				return -1;
			if (Src != *(uint32_t*)Value)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10BS_ADM_2_5:
			if (Index < 8)
			{
				/*ODU0XC source is fixed*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Src))
					return -1;
				if (Src != *(uint32_t*)Value)
					return -1;
			}
			else
			{
				if (*(uint32_t*)Value == PROFILE_ADM10_ODU0_XC_SET_SRC_ENUM_NONE)
				{
					/*Always allow to set XC source to None*/
					return 0;
				}
				else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) &&
						(*(uint32_t*)Value <= PROFILE_ADM10_XC_CLIENT_ODU0(15, 1)) &&
						(((((*(uint32_t*)Value - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) / 2) % 2) == 0) ||
						(((((*(uint32_t*)Value - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) / 2) % 2) != 0) &&
						(((*(uint32_t*)Value - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) % 2) == 0))))
				{
					/*Allow to set ODU0XC source to Client 10, 12, 14, 16 ODU0 1 containers only*/
				}
				else
				{
					return -1;
				}
				for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
				{
					/*Check that there is no ODU0XC with same ODU0 container as source*/
					if (i != ODU0XCIdx)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], &Src))
							return -1;
						if (Src == *(uint32_t*)Value)
							return -1;
					}
					/*Check that there is no ODU0XC with same ODU0 container as destination*/
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], &Dst))
						return -1;
					if (Dst == *(uint32_t*)Value)
						return -1;	
				}
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

static int32_t PROFILE_ADM10_XC_CheckODU0XCSetDstValue(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t ODU0XCIdx, uint32_t Restore)
{
	uint32_t i;
	uint32_t Src, Dst;
	uint32_t Mode;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			if (*(uint32_t*)Value == PROFILE_ADM10_ODU0_XC_SET_DST_ENUM_NONE)
			{
				/*Always allow to set ODU0XC destination to None*/
				return 0;
			}
			else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_LINE_ODU0(0, 0)) &&
				(*(uint32_t*)Value <= PROFILE_ADM10_XC_LINE_ODU0(1, 7)))
			{
				/*Allow to set ODU0XC destination to Line 1, 2 ODU0 1, ..., 8 containers only*/
			}
			else
			{
				return -1;
			}
			for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
			{
				/*Check that there is no ODU0XC with same ODU0 container as source*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], &Src))
					return -1;
				if (Src == *(uint32_t*)Value)
					return -1;
				/*Check that there is no ODU0XC with same ODU0 container as destination*/
				if (i != ODU0XCIdx)
				{
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], &Dst))
						return -1;
					if (Dst == *(uint32_t*)Value)
						return -1;
				}					
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10BS:
			/*ODU0XC destination is fixed*/
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Dst))
				return -1;
			if (Dst != *(uint32_t*)Value)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Dst))
				return -1;
			if (Dst != *(uint32_t*)Value)
				return -1;
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10BS_ADM_2_5:
			if (Index < 8)
			{
				/*ODU0XC destination is fixed*/
				if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, Index, &Dst))
					return -1;
				if (Dst != *(uint32_t*)Value)
					return -1;
			}
			else
			{
				if (*(uint32_t*)Value == PROFILE_ADM10_ODU0_XC_SET_DST_ENUM_NONE)
				{
					/*Always allow to set ODU0XC destination to None*/
					return 0;
				}
				else if ((*(uint32_t*)Value >= PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) &&
					(*(uint32_t*)Value <= PROFILE_ADM10_XC_CLIENT_ODU0(15, 1)) &&
					((((*(uint32_t*)Value - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) / 2) % 2) == 0))
				{
					/*Allow to set ODU0XC destination to Client 9, 11, 13, 15 ODU0 1, 2 containers only*/
				}
				else
				{
					return -1;
				}
				for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
				{
					/*Check that there is no ODU0XC with same ODU0 container as source*/
					if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], &Src))
						return -1;
					if (Src == *(uint32_t*)Value)
						return -1;
					/*Check that there is no ODU0XC with same ODU0 container as destination*/
					if (i != ODU0XCIdx)
					{
						if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], &Dst))
							return -1;
						if (Dst == *(uint32_t*)Value)
							return -1;
					}					
				}
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t PROFILE_ADM10_XC_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t Restore)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
	{
		if (Index == ProfileADM10ConfigODU1XCSetSrcIdx[i])
		{
			if (PROFILE_ADM10_XC_CheckODU1XCSetSrcValue(Profile, Table, Index, Value, i, Restore))
				return -1;
			return 0;
		}
		else if (Index == ProfileADM10ConfigODU1XCSetDstIdx[i])
		{
			if (PROFILE_ADM10_XC_CheckODU1XCSetDstValue(Profile, Table, Index, Value, i, Restore))
				return -1;
			return 0;
		}
	}
	
	for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
	{
		if (Index == ProfileADM10ConfigODU0XCSetSrcIdx[i])
		{
			if (PROFILE_ADM10_XC_CheckODU0XCSetSrcValue(Profile, Table, Index, Value, i, Restore))
				return -1;
			return 0;
		}
		else if (Index == ProfileADM10ConfigODU0XCSetDstIdx[i])
		{
			if (PROFILE_ADM10_XC_CheckODU0XCSetDstValue(Profile, Table, Index, Value, i, Restore))
				return -1;
			return 0;
		}
	}
	
	return 0;
}

static const uint32_t ODU1XCSrcFactory[PROFILE_ADM10_ODU1_XCS] = {
	PROFILE_ADM10_XC_CLIENT_ODU1(0), PROFILE_ADM10_XC_CLIENT_ODU1(2), PROFILE_ADM10_XC_CLIENT_ODU1(4), PROFILE_ADM10_XC_CLIENT_ODU1(6),
	PROFILE_ADM10_XC_CLIENT_ODU1(8), PROFILE_ADM10_XC_CLIENT_ODU1(10), PROFILE_ADM10_XC_CLIENT_ODU1(12), PROFILE_ADM10_XC_CLIENT_ODU1(14), 
};

static const uint32_t ODU1XCDstFactory[PROFILE_ADM10_ODU1_XCS] = {
	PROFILE_ADM10_XC_LINE_ODU1(0, 0), PROFILE_ADM10_XC_LINE_ODU1(0, 1), PROFILE_ADM10_XC_LINE_ODU1(0, 2), PROFILE_ADM10_XC_LINE_ODU1(0, 3),
	PROFILE_ADM10_XC_LINE_ODU1(1, 0), PROFILE_ADM10_XC_LINE_ODU1(1, 1), PROFILE_ADM10_XC_LINE_ODU1(1, 2), PROFILE_ADM10_XC_LINE_ODU1(1, 3), 
};

static const uint32_t ODU0XCSrcFactory[PROFILE_ADM10_ODU0_XCS] = {
	PROFILE_ADM10_XC_CLIENT_ODU0(0, 0), PROFILE_ADM10_XC_CLIENT_ODU0(1, 0), PROFILE_ADM10_XC_CLIENT_ODU0(2, 0), PROFILE_ADM10_XC_CLIENT_ODU0(3, 0),
	PROFILE_ADM10_XC_CLIENT_ODU0(4, 0), PROFILE_ADM10_XC_CLIENT_ODU0(5, 0), PROFILE_ADM10_XC_CLIENT_ODU0(6, 0), PROFILE_ADM10_XC_CLIENT_ODU0(7, 0),
	PROFILE_ADM10_XC_CLIENT_ODU0(8, 0), PROFILE_ADM10_XC_CLIENT_ODU0(9, 0), PROFILE_ADM10_XC_CLIENT_ODU0(10, 0), PROFILE_ADM10_XC_CLIENT_ODU0(11, 0),
	PROFILE_ADM10_XC_CLIENT_ODU0(12, 0), PROFILE_ADM10_XC_CLIENT_ODU0(13, 0), PROFILE_ADM10_XC_CLIENT_ODU0(14, 0), PROFILE_ADM10_XC_CLIENT_ODU0(15, 0), 
};

static const uint32_t ODU0XCDstFactory[PROFILE_ADM10_ODU0_XCS] = {
	PROFILE_ADM10_XC_LINE_ODU0(0, 0), PROFILE_ADM10_XC_LINE_ODU0(0, 1), PROFILE_ADM10_XC_LINE_ODU0(0, 2), PROFILE_ADM10_XC_LINE_ODU0(0, 3),
	PROFILE_ADM10_XC_LINE_ODU0(0, 4), PROFILE_ADM10_XC_LINE_ODU0(0, 5), PROFILE_ADM10_XC_LINE_ODU0(0, 6), PROFILE_ADM10_XC_LINE_ODU0(0, 7),
	PROFILE_ADM10_XC_LINE_ODU0(1, 0), PROFILE_ADM10_XC_LINE_ODU0(1, 1), PROFILE_ADM10_XC_LINE_ODU0(1, 2), PROFILE_ADM10_XC_LINE_ODU0(1, 3),
	PROFILE_ADM10_XC_LINE_ODU0(1, 4), PROFILE_ADM10_XC_LINE_ODU0(1, 5), PROFILE_ADM10_XC_LINE_ODU0(1, 6), PROFILE_ADM10_XC_LINE_ODU0(1, 7),
};

static int32_t PROFILE_ADM10_XC_SetModeValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t Restore)
{
	uint32_t i;
	
	switch (*(uint32_t*)Value)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
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
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10BS:
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
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
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
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10BS_ADM_2_5:			
			for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
			{
				if (i < 4)
				{
					PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], (uint32_t*)&ODU1XCSrcFactory[i]);
					PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], (uint32_t*)&ODU1XCDstFactory[i]);
				}
				else
				{
					PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i]);
					PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i]);
				}
			}
			for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
			{
				if (i < 8)
				{
					PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], (uint32_t*)&ODU0XCSrcFactory[i]);
					PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], (uint32_t*)&ODU0XCDstFactory[i]);
				}
				else
				{
					PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i]);
					PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i]);
				}
			}
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t PROFILE_ADM10_XC_SetODU1FactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t Restore)
{
	uint32_t i, Mode;
	
	if (Restore)
		return 0;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
			{
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i]);
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i]);
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10BS:
			for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
			{
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], (uint32_t*)&ODU1XCSrcFactory[i]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], (uint32_t*)&ODU1XCDstFactory[i]);
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
			{
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], (uint32_t*)&ODU1XCSrcFactory[i]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], (uint32_t*)&ODU1XCDstFactory[i]);
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10BS_ADM_2_5:			
			for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
			{
				if (i < 4)
				{
					PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i], (uint32_t*)&ODU1XCSrcFactory[i]);
					PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i], (uint32_t*)&ODU1XCDstFactory[i]);
				}
				else
				{
					PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i]);
					PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i]);
				}
			}
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t PROFILE_ADM10_XC_SetODU0FactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t Restore)
{
	uint32_t i, Mode;
	
	if (Restore)
		return 0;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return -1;
	
	switch (Mode)
	{
		case PROFILE_ADM10_SET_MODE_ENUM_ADM_10:
			for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
			{
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i]);
				PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i]);
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10BS:
			for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
			{
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], (uint32_t*)&ODU0XCSrcFactory[i]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], (uint32_t*)&ODU0XCDstFactory[i]);
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC:
			for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
			{
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], (uint32_t*)&ODU0XCSrcFactory[i]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], (uint32_t*)&ODU0XCDstFactory[i]);
			}
			break;
		case PROFILE_ADM10_SET_MODE_ENUM_AGG_10BS_ADM_2_5:			
			for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
			{
				if (i < 8)
				{
					PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetSrcIdx[i], (uint32_t*)&ODU0XCSrcFactory[i]);
					PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU0XCSetDstIdx[i], (uint32_t*)&ODU0XCDstFactory[i]);
				}
				else
				{
					PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetSrcIdx[i]);
					PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigODU1XCSetDstIdx[i]);
				}
			}
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_ADM10_XC_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Index, void *Value, uint32_t Restore)
{	
	if (Index == PROFILE_ADM10_SETMODE_IDX)
	{
		if (PROFILE_ADM10_XC_SetModeValueDependencies(Profile, Table, Index, Value, Restore))
			return -1;
	}
	if (Index == PROFILE_ADM10_ODU1_XC_SETFACTORY_IDX)
	{
		if (PROFILE_ADM10_XC_SetODU1FactoryValueDependencies(Profile, Table, Index, Value, Restore))
			return -1;
	}	
	if (Index == PROFILE_ADM10_ODU0_XC_SETFACTORY_IDX)
	{
		if (PROFILE_ADM10_XC_SetODU0FactoryValueDependencies(Profile, Table, Index, Value, Restore))
			return -1;
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
	uint32_t LnOPU1Payload[PROFILE_ADM10_LINE_PORTS][PROFILE_ADM10_LINE_PORT_ODU1S];
	uint32_t ClDataType[PROFILE_ADM10_CLIENT_PORTS];
	uint32_t ClOPU1Payload[PROFILE_ADM10_CLIENT_PORTS];
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
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADM10ConfigClOPU1SetPayloadIdx[i], &ClOPU1Payload[i]))
			return;
	}	
	
	if (Mode == PROFILE_ADM10_SET_MODE_ENUM_ADM_10)
	{
		for (i = 0; i < 8; i++)
		{
			FPGA_LINE_ADM_10_MUX_ODU12_SET(&fpga_line_adm_10, i, FPGA_LINE_ADM_10_MUX_ODU12_ODU1(i));
			FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_SET(&fpga_line_adm_10, i, FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_NULL);
		}
		
		for (i = 0; i < 16; i++)
		{
			FPGA_LINE_ADM_10_MUX_ODU01_SET(&fpga_line_adm_10, i, FPGA_LINE_ADM_10_MUX_ODU01_ODU0(i));
			FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_SET(&fpga_line_adm_10, i, FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_NULL);
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
							__ODU1Mux = FPGA_LINE_ADM_10_MUX_ODU12_GET(&fpga_line_adm_10, i) - 8;
							__OPU1PayloadType = FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_GET(&fpga_line_adm_10, i);
							if (__ODU1Mux == ODU1Mux)
								break;
						}
						if (i != ((Line * 4) + ODU1Idx))
						{
							__ODU1Mux = FPGA_LINE_ADM_10_MUX_ODU12_GET(&fpga_line_adm_10, ((Line * 4) + ODU1Idx)) - 8;
							__OPU1PayloadType = FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_GET(&fpga_line_adm_10, ODU1Mux);
							FPGA_LINE_ADM_10_MUX_ODU12_SET(&fpga_line_adm_10, i, FPGA_LINE_ADM_10_MUX_ODU12_ODU1(__ODU1Mux));
							FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_SET(&fpga_line_adm_10, __ODU1Mux, __OPU1PayloadType);
						}
						FPGA_LINE_ADM_10_MUX_ODU12_SET(&fpga_line_adm_10, (Line * 4) + ODU1Idx, FPGA_LINE_ADM_10_MUX_ODU12_ODU1(ODU1Mux));
						if (ClDataType[TargetClient] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_STM16)
							FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_SET(&fpga_line_adm_10, ODU1Mux, FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_STM16);
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
						FPGA_LINE_ADM_10_MUX_ODU12_SET(&fpga_line_adm_10, (Line * 4) + ODU1Idx, FPGA_LINE_ADM_10_MUX_ODU12_LINE(TargetLine, TargetODU1Idx));
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
					ODU1Mux = FPGA_LINE_ADM_10_MUX_ODU12_GET(&fpga_line_adm_10, (Line * 4) + ODU1Idx) - 8;
					FPGA_LINE_ADM_10_MUX_ODU12_SET(&fpga_line_adm_10, (Line * 4) + ODU1Idx, FPGA_LINE_ADM_10_MUX_ODU12_ODU1(ODU1Mux));
					FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_SET(&fpga_line_adm_10, ODU1Mux, FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_2xODU0);
					
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
						
						if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU0(0, 0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU0(15, 1)))
						{
							/*Target is client ODU0 container*/
							TargetClient = (Target - 1025) / 2;
							ODU0Mux = TargetClient;
							for (i = 0; i < 16; i++)
							{
								__ODU0Mux = FPGA_LINE_ADM_10_MUX_ODU01_GET(&fpga_line_adm_10, i) - 0x10;
								if (__ODU0Mux == ODU0Mux)
									break;
							}
							if (i != ((ODU1Mux*2) + ODU0Idx))
							{
								__ODU0Mux = FPGA_LINE_ADM_10_MUX_ODU01_GET(&fpga_line_adm_10, (ODU1Mux*2) + ODU0Idx) - 0x10;
								__OPU0PayloadType = FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_GET(&fpga_line_adm_10, ODU0Mux);
								FPGA_LINE_ADM_10_MUX_ODU01_SET(&fpga_line_adm_10, i, FPGA_LINE_ADM_10_MUX_ODU01_ODU0(__ODU0Mux));
								FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_SET(&fpga_line_adm_10, __ODU0Mux, __OPU0PayloadType);
							}
							FPGA_LINE_ADM_10_MUX_ODU01_SET(&fpga_line_adm_10, (ODU1Mux*2) + ODU0Idx, FPGA_LINE_ADM_10_MUX_ODU01_ODU0(ODU0Mux));
							if (ClDataType[TargetClient] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_1GE)
								FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_SET(&fpga_line_adm_10, ODU0Mux, FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_1GE);
							else if (ClDataType[TargetClient] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_STM1)
								FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_SET(&fpga_line_adm_10, ODU0Mux, FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_STM1);
							else if (ClDataType[TargetClient] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_STM4)
								FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_SET(&fpga_line_adm_10, ODU0Mux, FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_STM4);
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
							FPGA_LINE_ADM_10_MUX_ODU01_SET(&fpga_line_adm_10, (ODU1Mux*2) + ODU0Idx, FPGA_LINE_ADM_10_MUX_ODU01_LINE(TargetLine, TargetODU0Idx));
						}
					}
				}
			}
		}

		for (Client = 0; Client < PROFILE_ADM10_CLIENT_PORTS; Client++)
		{
			if (ClDataType[Client] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_STM16)
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
					FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_PRIMARY_ODU1_SET(&fpga_line_adm_10, Client, TargetLine, TargetODU1Idx);
					FPGA_CLIENT_ADM_10_IFACE_CTRL_SET(&fpga_client_adm_10[Client / 8], Client % 8, FPGA_CLIENT_ADM_10_IFACE_CTRL_EN_MASK);
				}
				else
				{
					FPGA_CLIENT_ADM_10_IFACE_CTRL_SET(&fpga_client_adm_10[Client / 8], Client % 8, FPGA_CLIENT_ADM_10_IFACE_CTRL_EN_MASK | FPGA_CLIENT_ADM_10_IFACE_CTRL_AIS_MASK);
				}
			}
			else if ((ClDataType[Client] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_STM1) || \
				(ClDataType[Client] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_STM4) || \
				(ClDataType[Client] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_1GE))
			{
				Target = 0;
				for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
				{
					if (XCODU0Src[i] == PROFILE_ADM10_XC_CLIENT_ODU0(Client, 0))
					{
						Target = XCODU0Dst[i];
						break;
					}
					if (XCODU0Dst[i] == PROFILE_ADM10_XC_CLIENT_ODU0(Client, 0))
					{
						Target = XCODU0Src[i];
						break;
					}
				}					
				if ((Target >= PROFILE_ADM10_XC_LINE_ODU0(0, 0)) && (Target <= PROFILE_ADM10_XC_LINE_ODU0(1, 7)))
				{
					TargetLine = ((Target - 1) / 8);
					TargetODU0Idx = ((Target - 1) % 8);
					FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_PRIMARY_ODU0_SET(&fpga_line_adm_10, Client, TargetLine, TargetODU0Idx);						
					FPGA_CLIENT_ADM_10_IFACE_CTRL_SET(&fpga_client_adm_10[Client / 8], Client % 8, FPGA_CLIENT_ADM_10_IFACE_CTRL_EN_MASK);
				}
				else
				{
					FPGA_CLIENT_ADM_10_IFACE_CTRL_SET(&fpga_client_adm_10[Client / 8], Client % 8, FPGA_CLIENT_ADM_10_IFACE_CTRL_EN_MASK | FPGA_CLIENT_ADM_10_IFACE_CTRL_AIS_MASK);
				}
			}
		}
	}
	else if (Mode == PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10BS)
	{
		/* Cross-connect is autoconfigured in FPGA */
		for (Client = 0; Client < 16; Client++)
		{
			FPGA_CLIENT_AGG_2X10_BS_IFACE_CTRL_SET(&fpga_client_agg_2x10_bs[Client / 8], Client % 8, FPGA_CLIENT_AGG_2X10_BS_IFACE_CTRL_EN_MASK);
		}
	}
	else if (Mode == PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC)
	{
		/* Cross-connect is autoconfigured in FPGA */
		for (Client = 0; Client < 16; Client++)
		{
			FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_CTRL_SET(&fpga_client_agg_2x10_efec[Client / 8], Client % 8, FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_CTRL_EN_MASK);
		}
	}
	else if (Mode == PROFILE_ADM10_SET_MODE_ENUM_AGG_10BS_ADM_2_5)
	{
		/* Cross-connect is autoconfigured in FPGA */
		for (Client = 0; Client < 16; Client++)
		{
			FPGA_CLIENT_AGG_2X10_BS_IFACE_CTRL_SET(&fpga_client_agg_2x10_bs[Client / 8], Client % 8, FPGA_CLIENT_AGG_2X10_BS_IFACE_CTRL_EN_MASK);
		}
		
		/*Cross-connects configuration for ADM-2.5*/		
		for (i = 0; i < FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_CNT; i++)
		{
			FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_SET(&fpga_client_adm_2_5[1], i, FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_STM16);
			FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_MUX_SET(&fpga_client_adm_2_5[1], i, FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_MUX_CLIENT(i));
			for (j = 0; j < 2; j++)
			{
				FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_SET(&fpga_client_adm_2_5[1], i, j, FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_ODU0_LINE(i, j));
			}
		}
		
		for (i = 0; i < FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_CNT; i++)
		{
			FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_SET(&fpga_client_adm_2_5[1], i, FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_STM1);
		}
		
		for (Line = 0; Line < FPGA_CLIENT_ADM_2_5_LINE_CNT; Line++)
		{
			if (ClOPU1Payload[8 + (Line * 2)] == PROFILE_ADM10_CL_OPU1_SET_PAYLOAD_ENUM_ANY)
			{
				Target = 0;
				for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
				{
					if (XCODU1Src[i] == PROFILE_ADM10_XC_CLIENT_ODU1(8 + (Line * 2)))
					{
						Target = XCODU1Dst[i];
						break;
					}
					if (XCODU1Dst[i] == PROFILE_ADM10_XC_CLIENT_ODU1(8 + (Line * 2)))
					{
						Target = XCODU1Src[i];
						break;
					}
				}
				
				if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU1(8)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU1(15)))
				{
					TargetClient = ((Target - PROFILE_ADM10_XC_CLIENT_ODU1(8)) / 2);
					FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_SET(&fpga_client_adm_2_5[1], Line, FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_STM16);
					FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_MUX_SET(&fpga_client_adm_2_5[1], Line, FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_MUX_CLIENT(TargetClient));
				}
			}
			else if (ClOPU1Payload[8 + (Line * 2)] == PROFILE_ADM10_CL_OPU1_SET_PAYLOAD_ENUM_AMP_2XODU0)
			{
				FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_SET(&fpga_client_adm_2_5[1], Line, FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_2XODU0);
				for (ODU0Idx = 0; ODU0Idx < 2; ODU0Idx++)
				{
					Target = 0;
					for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
					{
						if (XCODU0Src[i] == PROFILE_ADM10_XC_CLIENT_ODU0(8 + (Line * 2), ODU0Idx))
						{
							Target = XCODU0Dst[i];
							break;
						}
						if (XCODU0Dst[i] == PROFILE_ADM10_XC_CLIENT_ODU0(8 + (Line * 2), ODU0Idx))
						{
							Target = XCODU0Src[i];
							break;
						}
					}
					
					if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU0(15, 0)) && ((((Target - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) / 2) % 2) == 0))
					{
						TargetLine = (((Target - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) / 2) / 2);
						TargetODU0Idx = ((Target - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) % 2);						
						FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_SET(&fpga_client_adm_2_5[1], Line, ODU0Idx, FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_ODU0_LINE(TargetLine, TargetODU0Idx));
					}
					else if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU0(15, 0)) && ((((Target - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) / 2) % 2) != 0))
					{
						TargetClient = (((Target - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) / 2) / 2);
						FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_SET(&fpga_client_adm_2_5[1], Line, ODU0Idx, FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_ODU0_FRAMER(TargetClient));
						if (ClDataType[(Target - PROFILE_ADM10_XC_CLIENT_ODU0(0, 0)) / 2] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_1GE)
							FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_SET(&fpga_client_adm_2_5[1], TargetClient, FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_1GE);
						else if (ClDataType[(Target - PROFILE_ADM10_XC_CLIENT_ODU0(0, 0)) / 2] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_STM1)
							FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_SET(&fpga_client_adm_2_5[1], TargetClient, FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_STM1);
						else if (ClDataType[(Target - PROFILE_ADM10_XC_CLIENT_ODU0(0, 0)) / 2] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_STM4)
							FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_SET(&fpga_client_adm_2_5[1], TargetClient, FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_STM4);
					}
				}
			}
		}
		
		for (Client = 0; Client < FPGA_CLIENT_ADM_2_5_CLIENT_CNT; Client++)
		{
			if (ClDataType[8 + (Client * 2) + 1] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_STM16)
			{
				Target = 0;
				for (i = 0; i < PROFILE_ADM10_ODU1_XCS; i++)
				{
					if (XCODU1Src[i] == PROFILE_ADM10_XC_CLIENT_ODU1(8 + (Client * 2) + 1))
					{
						Target = XCODU1Dst[i];
						break;
					}
					if (XCODU1Dst[i] == PROFILE_ADM10_XC_CLIENT_ODU1(8 + (Client * 2) + 1))
					{
						Target = XCODU1Src[i];
						break;
					}
				}
					
				if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU1(8)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU1(15)))
				{
					TargetLine = ((Target - PROFILE_ADM10_XC_CLIENT_ODU1(8)) / 2);
					FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_PRIMARY_ODU1_SET(&fpga_client_adm_2_5[1], Client, TargetLine);
					FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_EN_MASK);
				}
				else
				{
					FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_EN_MASK | FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_AIS_MASK);
				}
			}
			else if ((ClDataType[8 + (Client * 2) + 1] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_STM1) || \
				(ClDataType[8 + (Client * 2) + 1] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_STM4) || \
				(ClDataType[8 + (Client * 2) + 1] == PROFILE_ADM10_CL_SET_DATA_TYPE_ENUM_1GE))
			{
				Target = 0;
				for (i = 0; i < PROFILE_ADM10_ODU0_XCS; i++)
				{
					if (XCODU0Src[i] == PROFILE_ADM10_XC_CLIENT_ODU0(8 + (Client * 2) + 1, 0))
					{
						Target = XCODU0Dst[i];
						break;
					}
					if (XCODU0Dst[i] == PROFILE_ADM10_XC_CLIENT_ODU0(8 + (Client * 2) + 1, 0))
					{
						Target = XCODU0Src[i];
						break;
					}
				}					
				if ((Target >= PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) && (Target <= PROFILE_ADM10_XC_CLIENT_ODU0(15, 1)))
				{
					TargetLine = (((Target - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) / 2) / 2);
					TargetODU0Idx = ((Target - PROFILE_ADM10_XC_CLIENT_ODU0(8, 0)) % 2);
					FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_PRIMARY_ODU0_SET(&fpga_client_adm_2_5[1], Client, TargetLine, TargetODU0Idx);				
					FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_EN_MASK);
				}
				else
				{
					FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_SET(&fpga_client_adm_2_5[1], Client, FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_EN_MASK | FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_AIS_MASK);
				}
			}
		}
		
	}
}

void PROFILE_ADM10_XC_UpdateValues(PROFILE_T *Profile)
{
	
}
