#include "profile.h"
#include <string.h>

#define PROFILE_TYPE_INT32 		2
#define PROFILE_TYPE_INT64 		3
#define PROFILE_TYPE_UINT32 	4
#define PROFILE_TYPE_HEX32 		5
#define PROFILE_TYPE_UINT64 	6
#define PROFILE_TYPE_HEX64 		7
#define PROFILE_TYPE_FLOAT_1 	9
#define PROFILE_TYPE_FLOAT_2 	10
#define PROFILE_TYPE_FLOAT_3 	11
#define PROFILE_TYPE_FLOAT_4 	12
#define PROFILE_TYPE_FLOAT_E1	17
#define PROFILE_TYPE_FLOAT_E2	18
#define PROFILE_TYPE_FLOAT_E3	19
#define PROFILE_TYPE_FLOAT_E4	20

static int32_t __PROFILE_CheckParamValueLimits(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, void *Value)
{
	uint32_t i;
	
	if (Param->Type == PROFILE_TYPE_NONE)
	{
		return 0;
	}
	else if ((Param->Type == PROFILE_TYPE_INT32) ||
		(Param->Type == PROFILE_TYPE_INT64) ||
		(Param->Type == PROFILE_TYPE_UINT32) ||
		(Param->Type == PROFILE_TYPE_HEX32) ||
		(Param->Type == PROFILE_TYPE_UINT64) ||
		(Param->Type == PROFILE_TYPE_HEX64) ||
		(Param->Type == PROFILE_TYPE_FLOAT_1) ||
		(Param->Type == PROFILE_TYPE_FLOAT_2) ||
		(Param->Type == PROFILE_TYPE_FLOAT_3) ||
		(Param->Type == PROFILE_TYPE_FLOAT_4) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E1) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E2) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E3) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E4))
	{
		return 0;
	}
	else if (Param->Type == PROFILE_TYPE_STRING16)
	{
		if (strlen(Value) < 16)
			return 0;
	}
	else if (Param->Type == PROFILE_TYPE_STRING32)
	{
		if (strlen(Value) < 32)
			return 0;
	}
	else if (Param->Type == PROFILE_TYPE_STRING64)
	{
		if (strlen(Value) < 64)
			return 0;
	}
	else if ((Param->Type > PROFILE_TYPE_ENUM) && (Param->Type < PROFILE_TYPE_BITMSK))
	{
		for (i = 0; i < Param->Enum->ValuesCnt; i++)
		{
			if (*(int32_t*)Value == Param->Enum->Values[i].Number)
			{
				return 0;
			}
		}
		return -1;
	}
	else if ((Param->Type > PROFILE_TYPE_BITMSK) && (Param->Type < (PROFILE_TYPE_BITMSK+1024)))
	{
		return 0;
	}
	return -1;
}

static int32_t __PROFILE_CheckParamValueConstraint(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, void *Value)
{
	uint32_t i;
	
	if (Param->Constraints)
	{
		for (i = 0; i < *Param->ConstraintsCnt; i++)
		{
			if (Param->Constraints[i].CheckCondition())
			{
				if (Param->Constraints[i].CheckValue(Value))
				{
					return 0;
				}
				else
				{
					return -1;
				}
			}
		}
	}
	return 0;
}

static int32_t __PROFILE_WriteParamValue(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, void *Value)
{
	if (Param->Type == PROFILE_TYPE_NONE)
	{
		return 0;
	}
	else if ((Param->Type == PROFILE_TYPE_INT32) ||
		(Param->Type == PROFILE_TYPE_UINT32) ||
		(Param->Type == PROFILE_TYPE_HEX32) ||
		(Param->Type == PROFILE_TYPE_FLOAT_1) ||
		(Param->Type == PROFILE_TYPE_FLOAT_2) ||
		(Param->Type == PROFILE_TYPE_FLOAT_3) ||
		(Param->Type == PROFILE_TYPE_FLOAT_4) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E1) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E2) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E3) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E4) || 
		((Param->Type > PROFILE_TYPE_ENUM) && (Param->Type < (PROFILE_TYPE_ENUM + 1024))) ||
		((Param->Type > PROFILE_TYPE_BITMSK) && (Param->Type < (PROFILE_TYPE_BITMSK+1024))))
	{
		memcpy((void*)Param->Value, Value, 4);
		return 0;
	}
	else if ((Param->Type == PROFILE_TYPE_INT64) ||
		(Param->Type == PROFILE_TYPE_UINT64) ||
		(Param->Type == PROFILE_TYPE_HEX64))
	{
		memcpy((void*)Param->Value, Value, 8);
		return 0;
	}
	else if ((Param->Type == PROFILE_TYPE_STRING16) ||
		(Param->Type == PROFILE_TYPE_STRING32) ||
		(Param->Type == PROFILE_TYPE_STRING64))
	{
		strcpy((void*)Param->Value, Value);
		return 0;
	}
	
	return -1;
}

static int32_t __PROFILE_WriteParamValidValue(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param)
{
	uint32_t i;
	
	if (Param->Constraints)
	{
		for (i = 0; i < *Param->ConstraintsCnt; i++)
		{
			if (Param->Constraints[i].CheckCondition())
			{
				Param->Constraints[i].GetValidValue(Param->Value);
			}
		}
	}
	return -1;
}

static int32_t __PROFILE_ReadParamValue(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, void *Value)
{
	if (Param->Type == PROFILE_TYPE_NONE)
	{
		return 0;
	}
	else if ((Param->Type == PROFILE_TYPE_INT32) ||
		(Param->Type == PROFILE_TYPE_UINT32) ||
		(Param->Type == PROFILE_TYPE_HEX32) ||
		(Param->Type == PROFILE_TYPE_FLOAT_1) ||
		(Param->Type == PROFILE_TYPE_FLOAT_2) ||
		(Param->Type == PROFILE_TYPE_FLOAT_3) ||
		(Param->Type == PROFILE_TYPE_FLOAT_4) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E1) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E2) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E3) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E4) || 
		((Param->Type > PROFILE_TYPE_ENUM) && (Param->Type < (PROFILE_TYPE_ENUM + 1024))) ||
		((Param->Type > PROFILE_TYPE_BITMSK) && (Param->Type < (PROFILE_TYPE_BITMSK+1024))))
	{
		memcpy(Value, Param->Value, 4);
		return 0;
	}
	else if ((Param->Type == PROFILE_TYPE_INT64) ||
		(Param->Type == PROFILE_TYPE_UINT64) ||
		(Param->Type == PROFILE_TYPE_HEX64))
	{
		memcpy(Value, Param->Value, 8);
		return 0;
	}
	else if ((Param->Type == PROFILE_TYPE_STRING16) ||
		(Param->Type == PROFILE_TYPE_STRING32) ||
		(Param->Type == PROFILE_TYPE_STRING64))
	{
		strcpy(Value, Param->Value);
		return 0;
	}
	
	return -1;
}

int32_t PROFILE_StringToParamValue(uint16_t Type, PROFILE_VALUE_T *Value, char *String, uint32_t Len)
{	
	char *c;
	
	if (Type == PROFILE_TYPE_NONE)
	{
		
	}
	else if ((Type == PROFILE_TYPE_INT32) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->i32 = strtol(Value->s64, &c, 10);
	}
	else if ((Type == PROFILE_TYPE_INT64) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->i64 = strtoll(Value->s64, &c, 10);
	}
	else if ((Type == PROFILE_TYPE_UINT32) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->u32 = strtoul(Value->s64, &c, 10);
	}
	else if ((Type == PROFILE_TYPE_HEX32) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->u32 = strtoul(Value->s64, &c, 16);
	}
	else if ((Type == PROFILE_TYPE_UINT64) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->u64 = strtoul(Value->s64, &c, 10);
	}
	else if ((Type == PROFILE_TYPE_HEX64) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->u64 = strtoul(Value->s64, &c, 16);
	}
	else if ((Type == PROFILE_TYPE_FLOAT_1) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->f32 = strtof(Value->s64, &c);
	}
	else if ((Type == PROFILE_TYPE_FLOAT_2) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->f32 = strtof(Value->s64, &c);
	}
	else if ((Type == PROFILE_TYPE_FLOAT_3) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->f32 = strtof(Value->s64, &c);
	}
	else if ((Type == PROFILE_TYPE_FLOAT_4) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->f32 = strtof(Value->s64, &c);
	}
	else if ((Type == PROFILE_TYPE_FLOAT_E1) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->f32 = strtof(Value->s64, &c);
	}
	else if ((Type == PROFILE_TYPE_FLOAT_E2) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->f32 = strtof(Value->s64, &c);
	}
	else if ((Type == PROFILE_TYPE_FLOAT_E3) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->f32 = strtof(Value->s64, &c);
	}
	else if ((Type == PROFILE_TYPE_FLOAT_E4) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->f32 = strtof(Value->s64, &c);
	}
	else if (((Type > PROFILE_TYPE_ENUM) && (Type < (PROFILE_TYPE_ENUM + 1024))) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->i32 = strtol(Value->s64, &c, 10);
	}
	else if (((Type > PROFILE_TYPE_BITMSK) && (Type < (PROFILE_TYPE_BITMSK+1024))) && (Len <= 63))
	{
		memcpy(Value, String, Len);
		Value->s64[Len] = 0;
		Value->u32 = strtoul(Value->s64, &c, 10);
	}
	else if ((Type == PROFILE_TYPE_STRING16) && (Len <= 15))
	{
		memcpy(Value->s16, String, Len);
		Value->s16[Len] = 0;
	}
	else if ((Type == PROFILE_TYPE_STRING32) && (Len <= 31))
	{
		memcpy(Value->s32, String, Len);
		Value->s32[Len] = 0;
	}
	else if ((Type == PROFILE_TYPE_STRING64) && (Len <= 63))
	{
		memcpy(Value->s64, String, Len);
		Value->s64[Len] = 0;
	}
	else
	{
		return -1;
	}
	return 0;
}

int32_t PROFILE_ParamValueToString(uint16_t Type, PROFILE_VALUE_T *Value, char *String, uint32_t *Len)
{	
	if (Type == PROFILE_TYPE_NONE)
	{
		*Len = 0;
	}
	else if (Type == PROFILE_TYPE_INT32)
	{
		*Len = sprintf(String, "%d", Value->i32);
	}
	else if (Type == PROFILE_TYPE_INT64)
	{
		*Len = sprintf(String, "%lld", Value->i64);
	}
	else if (Type == PROFILE_TYPE_UINT32)
	{
		*Len = sprintf(String, "%u", Value->u32);
	}
	else if (Type == PROFILE_TYPE_HEX32)
	{
		*Len = sprintf(String, "%X", Value->u32);
	}
	else if (Type == PROFILE_TYPE_UINT64)
	{
		*Len = sprintf(String, "%llu", Value->u64);
	}
	else if (Type == PROFILE_TYPE_HEX64)
	{
		*Len = sprintf(String, "%llX", Value->u64);
	}
	else if (Type == PROFILE_TYPE_FLOAT_1)
	{
		*Len = sprintf(String, "%.lf", Value->f32);
	}
	else if (Type == PROFILE_TYPE_FLOAT_2)
	{
		*Len = sprintf(String, "%.2f", Value->f32);
	}
	else if (Type == PROFILE_TYPE_FLOAT_3)
	{
		*Len = sprintf(String, "%.3f", Value->f32);
	}
	else if (Type == PROFILE_TYPE_FLOAT_4)
	{
		*Len = sprintf(String, "%.4f", Value->f32);
	}
	else if (Type == PROFILE_TYPE_FLOAT_E1)
	{
		*Len = sprintf(String, "%.le", Value->f32);
	}
	else if (Type == PROFILE_TYPE_FLOAT_E2)
	{
		*Len = sprintf(String, "%.2e", Value->f32);
	}
	else if (Type == PROFILE_TYPE_FLOAT_E3)
	{
		*Len = sprintf(String, "%.3e", Value->f32);
	}
	else if (Type == PROFILE_TYPE_FLOAT_E4)
	{
		*Len = sprintf(String, "%.4e", Value->f32);
	}
	else if ((Type > PROFILE_TYPE_ENUM) && (Type < (PROFILE_TYPE_ENUM + 1024)))
	{
		*Len = sprintf(String, "%d", Value->i32);
	}
	else if ((Type > PROFILE_TYPE_BITMSK) && (Type < (PROFILE_TYPE_BITMSK+1024)))
	{
		*Len = sprintf(String, "%u", Value->u32);
	}
	else if (Type == PROFILE_TYPE_STRING16)
	{
		*Len = sprintf(String, "%s", Value->s16);
	}
	else if (Type == PROFILE_TYPE_STRING32)
	{
		*Len = sprintf(String, "%s", Value->s32);
	}
	else if (Type == PROFILE_TYPE_STRING64)
	{
		*Len = sprintf(String, "%s", Value->s64);
	}
	else
	{
		return -1;
	}
	return 0;
}

int32_t PROFILE_GetParamValueSize(uint16_t Type, uint8_t *ValueSize)
{
	if (Type == PROFILE_TYPE_NONE)
	{
		*ValueSize = 0;
	}
	else if (Type == PROFILE_TYPE_INT32)
	{
		*ValueSize = sizeof(int32_t);
	}
	else if (Type == PROFILE_TYPE_INT64)
	{
		*ValueSize = sizeof(int64_t);
	}
	else if (Type == PROFILE_TYPE_UINT32)
	{
		*ValueSize = sizeof(uint32_t);
	}
	else if (Type == PROFILE_TYPE_HEX32)
	{
		*ValueSize = sizeof(uint32_t);
	}
	else if (Type == PROFILE_TYPE_UINT64)
	{
		*ValueSize = sizeof(uint64_t);
	}
	else if (Type == PROFILE_TYPE_HEX64)
	{
		*ValueSize = sizeof(uint64_t);
	}
	else if (Type == PROFILE_TYPE_FLOAT_1)
	{
		*ValueSize = sizeof(float);
	}
	else if (Type == PROFILE_TYPE_FLOAT_2)
	{
		*ValueSize = sizeof(float);
	}
	else if (Type == PROFILE_TYPE_FLOAT_3)
	{
		*ValueSize = sizeof(float);
	}
	else if (Type == PROFILE_TYPE_FLOAT_4)
	{
		*ValueSize = sizeof(float);
	}
	else if (Type == PROFILE_TYPE_FLOAT_E1)
	{
		*ValueSize = sizeof(float);
	}
	else if (Type == PROFILE_TYPE_FLOAT_E2)
	{
		*ValueSize = sizeof(float);
	}
	else if (Type == PROFILE_TYPE_FLOAT_E3)
	{
		*ValueSize = sizeof(float);
	}
	else if (Type == PROFILE_TYPE_FLOAT_E4)
	{
		*ValueSize = sizeof(float);
	}
	else if ((Type > PROFILE_TYPE_ENUM) && (Type < (PROFILE_TYPE_ENUM + 1024)))
	{
		*ValueSize = sizeof(int32_t);
	}
	else if ((Type > PROFILE_TYPE_BITMSK) && (Type < (PROFILE_TYPE_BITMSK+1024)))
	{
		*ValueSize = sizeof(uint32_t);
	}
	else if (Type == PROFILE_TYPE_STRING16)
	{
		*ValueSize = 16;
	}
	else if (Type == PROFILE_TYPE_STRING32)
	{
		*ValueSize = 32;
	}
	else if (Type == PROFILE_TYPE_STRING64)
	{
		*ValueSize = 64;
	}
	else
	{
		return -1;
	}
	return 0;
}

int32_t PROFILE_GetParamByIndex(const PROFILE_T *Profile, uint8_t SectionIndex, uint32_t ParamIndex, PROFILE_PARAM_T** Param)
{
	uint32_t i;
	
	for (i = 0; i < Profile->SectionsCnt; i++)
	{
		if (Profile->Sections[i]->Index == SectionIndex)
		{
			if ((ParamIndex >= 1) && (ParamIndex <= Profile->Sections[i]->ParamsCnt))
			{
				*Param = (PROFILE_PARAM_T*)Profile->Sections[i]->Params[ParamIndex - 1];
				return 0;
			}
			break;
		}
	}
	return NULL;
}

int32_t __PROFILE_SetParamValue(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, PROFILE_VALUE_T *Value)
{
	uint32_t i, j;
	
	if (__PROFILE_CheckParamValueLimits(Profile, Param, Value))
		return -1;
	if (__PROFILE_CheckParamValueConstraint(Profile, Param, Value))
		return -1;
	
	if (Param->SetCallback)
	{
		if (!Param->SetCallback((PROFILE_T*)Profile, (PROFILE_PARAM_T*)Param, Value))
			return -1;
	}
	
	if (__PROFILE_WriteParamValue(Profile, Param, Value))
		return -1;
	
	for (i = 0; i < Profile->SectionsCnt; i++)
	{
		if (Profile->Sections[i]->Index == 3)
		{
config_check:
			for (j = 0; j < Profile->Sections[i]->ParamsCnt; j++)
			{
				if ((__PROFILE_CheckParamValueLimits(Profile, Profile->Sections[i]->Params[j], Profile->Sections[i]->Params[j]->Value) || 
					__PROFILE_CheckParamValueConstraint(Profile, Profile->Sections[i]->Params[j], Profile->Sections[i]->Params[j]->Value)))
				{
					__PROFILE_WriteParamValidValue(Profile, Profile->Sections[i]->Params[j]);
					goto config_check;
				}
			}
		}
	}
	return 0;
}

int32_t __PROFILE_GetParamValue(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, PROFILE_VALUE_T *Value)
{
	if (__PROFILE_ReadParamValue(Profile, Param, Value))
		return -1;
	return 0;
}

int32_t __PROFILE_GetParamAdminState(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, int32_t *AdminState)
{
	uint32_t i;
	
	for (i = 0; i < *Param->AdminStatesCnt; i++)
	{	
		if (Param->AdminStates[i].CheckCondition())
		{
			*AdminState = Param->AdminStates[i].GetValue();
			return 0;
		}
	}
	*AdminState = 0;
	return 0;
}

int32_t __PROFILE_GetParamColor(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, uint8_t *Color)
{
	uint32_t i;
	uint8_t ColorType;
	
	if (!Param->Colors)
		goto color_na;
	
	if (Param->Type == PROFILE_TYPE_INT32)
	{
		if (!Param->Thrs)
			goto color_na;
		
		if ((*((int32_t*)Param->Value) > *((int32_t*)Param->Thrs->FMax->Value)) || 
			(*((int32_t*)Param->Value) < *((int32_t*)Param->Thrs->FMin->Value)))
		{
			*Color = *Param->Colors->Failure;
			return 0;
		}
		else if ((*((int32_t*)Param->Value) > *((int32_t*)Param->Thrs->DMax->Value)) || 
			(*((int32_t*)Param->Value) < *((int32_t*)Param->Thrs->DMin->Value)))
		{
			*Color = *Param->Colors->Degrade;
			return 0;
		}
		else
		{
			*Color = *Param->Colors->Normal;
			return 0;
		}
	}
	else if (Param->Type == PROFILE_TYPE_INT64)
	{
		if (!Param->Thrs)
			goto color_na;
		
		if ((*((int64_t*)Param->Value) > *((int64_t*)Param->Thrs->FMax->Value)) || 
			(*((int64_t*)Param->Value) < *((int64_t*)Param->Thrs->FMin->Value)))
		{
			*Color = *Param->Colors->Failure;
			return 0;
		}
		else if ((*((int64_t*)Param->Value) > *((int64_t*)Param->Thrs->DMax->Value)) || 
			(*((int64_t*)Param->Value) < *((int64_t*)Param->Thrs->DMin->Value)))
		{
			*Color = *Param->Colors->Degrade;
			return 0;
		}
		else
		{
			*Color = *Param->Colors->Normal;
			return 0;
		}
	}
	else if ((Param->Type == PROFILE_TYPE_UINT32) ||
		(Param->Type == PROFILE_TYPE_HEX32))
	{
		if (!Param->Thrs)
			goto color_na;
		
		if ((*((uint32_t*)Param->Value) > *((uint32_t*)Param->Thrs->FMax->Value)) || 
			(*((uint32_t*)Param->Value) < *((uint32_t*)Param->Thrs->FMin->Value)))
		{
			*Color = *Param->Colors->Failure;
			return 0;
		}
		else if ((*((uint32_t*)Param->Value) > *((uint32_t*)Param->Thrs->DMax->Value)) || 
			(*((uint32_t*)Param->Value) < *((uint32_t*)Param->Thrs->DMin->Value)))
		{
			*Color = *Param->Colors->Degrade;
			return 0;
		}
		else
		{
			*Color = *Param->Colors->Normal;
			return 0;
		}
	}
	else if ((Param->Type == PROFILE_TYPE_UINT64) ||
		(Param->Type == PROFILE_TYPE_HEX64))
	{
		if (!Param->Thrs)
			goto color_na;
		
		if ((*((uint64_t*)Param->Value) > *((uint64_t*)Param->Thrs->FMax->Value)) || 
			(*((uint64_t*)Param->Value) < *((uint64_t*)Param->Thrs->FMin->Value)))
		{
			*Color = *Param->Colors->Failure;
			return 0;
		}
		else if ((*((uint64_t*)Param->Value) > *((uint64_t*)Param->Thrs->DMax->Value)) || 
			(*((uint64_t*)Param->Value) < *((uint64_t*)Param->Thrs->DMin->Value)))
		{
			*Color = *Param->Colors->Degrade;
			return 0;
		}
		else
		{
			*Color = *Param->Colors->Normal;
			return 0;
		}
	}
	else if ((Param->Type == PROFILE_TYPE_FLOAT_1) ||
		(Param->Type == PROFILE_TYPE_FLOAT_2) ||
		(Param->Type == PROFILE_TYPE_FLOAT_3) ||
		(Param->Type == PROFILE_TYPE_FLOAT_4) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E1) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E2) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E3) ||
		(Param->Type == PROFILE_TYPE_FLOAT_E4))
	{
		if (!Param->Thrs)
			goto color_na;
		
		if ((*((float*)Param->Value) > *((float*)Param->Thrs->FMax->Value)) || 
			(*((float*)Param->Value) < *((float*)Param->Thrs->FMin->Value)))
		{
			*Color = *Param->Colors->Failure;
			return 0;
		}
		else if ((*((float*)Param->Value) > *((float*)Param->Thrs->DMax->Value)) || 
			(*((float*)Param->Value) < *((float*)Param->Thrs->DMin->Value)))
		{
			*Color = *Param->Colors->Degrade;
			return 0;
		}
		else
		{
			*Color = *Param->Colors->Normal;
			return 0;
		}
	}
	else if ((Param->Type == PROFILE_TYPE_STRING16) ||
		(Param->Type == PROFILE_TYPE_STRING32) ||
		(Param->Type == PROFILE_TYPE_STRING64))
	{
		goto color_na;
	}
	else if ((Param->Type > PROFILE_TYPE_ENUM) && (Param->Type < (PROFILE_TYPE_ENUM + 1024)))
	{
		for (i = 0; i < Param->Enum->ValuesCnt; i++)
		{
			if (*((int32_t*)Param->Value) == Param->Enum->Values[i].Number)
			{
				if (Param->Enum->Values[i].Color == PROFILE_COLOR_TYPE_NORMAL)
				{
					*Color = *Param->Colors->Normal;
					return 0;
				}
				else if (Param->Enum->Values[i].Color == PROFILE_COLOR_TYPE_DEGRADE)
				{
					*Color = *Param->Colors->Degrade;
					return 0;
				}
				else if (Param->Enum->Values[i].Color == PROFILE_COLOR_TYPE_FAILURE)
				{
					*Color = *Param->Colors->Failure;
					return 0;
				}
				goto color_na;
			}
		}
		goto color_na;
	}
	else if ((Param->Type > PROFILE_TYPE_BITMSK) && (Param->Type < (PROFILE_TYPE_BITMSK+1024)))
	{
		if (*((uint32_t*)Param->Value) == 0)
		{
			if (Param->Bitmsk->Values[0].Color == PROFILE_COLOR_TYPE_NORMAL)
			{
				*Color = *Param->Colors->Normal;
				return 0;
			}
			else if (Param->Bitmsk->Values[0].Color == PROFILE_COLOR_TYPE_DEGRADE)
			{
				*Color = *Param->Colors->Degrade;
				return 0;
			}
			else if (Param->Bitmsk->Values[0].Color == PROFILE_COLOR_TYPE_FAILURE)
			{
				*Color = *Param->Colors->Failure;
				return 0;
			}
		}
		
		ColorType = PROFILE_COLOR_TYPE_NORMAL;
		
		for (i = 1; i < Param->Bitmsk->ValuesCnt; i++)
		{
			if (*((uint32_t*)Param->Value) & (1 << Param->Bitmsk->Values[i].Number))
			{
				if ((Param->Bitmsk->Values[i].Color == PROFILE_COLOR_TYPE_DEGRADE) &&
					(ColorType == PROFILE_COLOR_TYPE_NORMAL))
				{
					ColorType = PROFILE_COLOR_TYPE_DEGRADE;
				}
				else if ((Param->Bitmsk->Values[i].Color == PROFILE_COLOR_TYPE_FAILURE) &&
					(ColorType != PROFILE_COLOR_TYPE_FAILURE))
				{
					ColorType = PROFILE_COLOR_TYPE_FAILURE;
				}
			}
		}
		
		if (ColorType == PROFILE_COLOR_TYPE_NORMAL)
		{
			*Color = *Param->Colors->Normal;
			return 0;
		}
		else if (ColorType == PROFILE_COLOR_TYPE_DEGRADE)
		{
			*Color = *Param->Colors->Degrade;
			return 0;
		}
		else if (ColorType == PROFILE_COLOR_TYPE_FAILURE)
		{
			*Color = *Param->Colors->Failure;
			return 0;
		}
	}
	
color_na:
	*Color = PROFILE_COLOR_NA;
	return 0;
}

int32_t __PROFILE_SetParamColors(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, uint8_t Normal, uint8_t Degrade, uint8_t Failure)
{
	if (!Param->Colors)
		return -1;
	
	*Param->Colors->Normal = Normal;
	*Param->Colors->Degrade = Degrade;
	*Param->Colors->Failure = Failure;
	return 0;	
}

int32_t __PROFILE_GetParamColors(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, uint8_t *Normal, uint8_t *Degrade, uint8_t *Failure)
{
	if (!Param->Colors)
		return -1;
	
	*Normal = *Param->Colors->Normal;
	*Degrade = *Param->Colors->Degrade;
	*Failure = *Param->Colors->Failure;
	return 0;	
}

int32_t PROFILE_IsCalibParam(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param)
{
	if ((!Param->SetCallback) && ((Param->Access == 192) || (Param->Access == 208) || (Param->Access == 212) || (Param->Access == 213)))
		return 0;
	return -1;
}
int32_t PROFILE_IsConfigParam(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param)
{
	if ((!Param->SetCallback) && ((Param->Access == 240) || (Param->Access == 244) || (Param->Access == 245) || (Param->Access == 252) || (Param->Access == 253) || (Param->Access == 255)))
		return 0;
	return -1;
}

void PROFILE_Init(const PROFILE_T *Profile)
{
	PROFILE_MUTEX_INIT(Profile);
	PROFILE_MUTEX_LOCK(Profile);	
	memcpy(Profile->State->Values, Profile->State->DefaultValues, Profile->State->ValuesSize);
	memcpy(Profile->Config->Values, Profile->Config->DefaultValues, Profile->Config->ValuesSize);
	memcpy(Profile->Calib->Values, Profile->Calib->DefaultValues, Profile->Calib->ValuesSize);
	PROFILE_MUTEX_UNLOCK(Profile);
}

