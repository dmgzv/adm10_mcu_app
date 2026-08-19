#include "profile.h"
#include "profile_flash.h"
#include "string_fnc.h"
#include "crc.h"
#include <string.h>
#include <stdio.h>
#include "cmsis_os.h"
#include "bootloader.h"
#include "log.h"

#define PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, Param) 							(void*)((uint32_t)((Profile)->Values->Dynamic) + (uint32_t)((Param)->Value))
#define PROFILE_STATIC_PARAM_VALUE_PTR(Profile, Param) 								(void*)((uint32_t)((Profile)->Values->Static) + (uint32_t)((Param)->Value))
#define PROFILE_CALIB_PARAM_VALUE_PTR(Profile, Param) 								(void*)((uint32_t)((Profile)->Values->Calib) + (uint32_t)((Param)->Value))
#define PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, Param)				(void*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->StaticDefaultValues) + (uint32_t)((Param)->Value))
#define PROFILE_CALIB_PARAM_DEFAULT_VALUE_PTR(Profile, Param)					(void*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->CalibDefaultValues) + (uint32_t)((Param)->Value))
#define PROFILE_DYNAMIC_PARAM_THR_PTR(Profile, Param) 								(Param)->Thesholds ? (void*)((uint32_t)((Profile)->Values->Static) + (uint32_t)((Param)->Thesholds)) : NULL
#define PROFILE_CONFIG_PARAM_THR_PTR(Profile, Param) 									(Param)->Thesholds ? (void*)((uint32_t)((Profile)->Values->Static) + (uint32_t)((Param)->Thesholds)) : NULL
#define PROFILE_STATIC_PARAM_THR_PTR(Profile, Param) 									(Param)->Thesholds ? (void*)((uint32_t)((Profile)->Values->Static) + (uint32_t)((Param)->Thesholds)) : NULL
#define PROFILE_DYNAMIC_PARAM_BITMSK_PTR(Profile, Param) 							(Param)->Thesholds
#define PROFILE_DYNAMIC_PARAM_ENUM_PTR(Profile, Param) 								(Param)->Thesholds
#define PROFILE_CONFIG_PARAM_ENUM_PTR(Profile, Param) 								(Param)->Thesholds
#define PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) 							(Param)->Color ? (uint8_t*)((uint32_t)((Profile)->Values->Dynamic) + (uint32_t)((Param)->Color)) : NULL
#define PROFILE_DYNAMIC_PARAM_DEGR_COLOR_PTR(Profile, Param) 					(Param)->DegradeColor ? (uint8_t*)((uint32_t)((Profile)->Values->Static) + (uint32_t)((Param)->DegradeColor)) : NULL
#define PROFILE_DYNAMIC_PARAM_DEFAULT_DEGR_COLOR_PTR(Profile, Param) 	(Param)->DegradeColor ? (uint8_t*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->StaticDefaultValues) + (uint32_t)((Param)->DegradeColor)) : NULL
#define PROFILE_DYNAMIC_PARAM_FAIL_COLOR_PTR(Profile, Param) 					(Param)->FailureColor ? (uint8_t*)((uint32_t)((Profile)->Values->Static) + (uint32_t)((Param)->FailureColor)) : NULL
#define PROFILE_DYNAMIC_PARAM_DEFAULT_FAIL_COLOR_PTR(Profile, Param) 	(Param)->FailureColor ? (uint8_t*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->StaticDefaultValues) + (uint32_t)((Param)->FailureColor)) : NULL
#define PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, Param) 							(Param)->State ? (uint8_t*)((uint32_t)((Profile)->Values->Dynamic) + (uint32_t)((Param)->State)) : NULL

__weak void __PROFILE_GetStringColor(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t *Color, uint8_t *DegradeColor, uint8_t *FailureColor)
{
	*Color = PROFILE_COLOR_NA;
}

__weak int32_t PROFILE_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore)
{
	return 0;
}

__weak int32_t PROFILE_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore)
{
	return 0;
}

static uint32_t PROFILE_GetTableSize(PROFILE_T *Profile, uint8_t Table)
{
	switch (Table)
	{
		case PROFILE_TABLE_1:
			return Profile->Descr[Profile->Idx]->Table1Size;
		case PROFILE_TABLE_2:
			return Profile->Descr[Profile->Idx]->Table2Size;
		case PROFILE_TABLE_3:
			return Profile->Descr[Profile->Idx]->Table3Size;
		case PROFILE_TABLE_4:
			return Profile->Descr[Profile->Idx]->Table4Size;
		case PROFILE_TABLE_5:
			return Profile->Descr[Profile->Idx]->Table5Size;
		case PROFILE_TABLE_8:
			return Profile->Descr[Profile->Idx]->Table8Size;
		case PROFILE_TABLE_103:
			return Profile->Descr[Profile->Idx]->Table103Size;
		default:
			break;
	}
	return 0;
}

int32_t PROFILE_CheckTableIdx(PROFILE_T *Profile, uint8_t Table, uint16_t Idx)
{
	uint32_t TableSize;
	const char *DeviceClass = "";

	if (!Profile)
	{
		LOG_Error("PROFILE", DeviceClass, "", "Profile = null");
		return -1;
	}
	if ((!Profile->Descr) || (!Profile->Descr[Profile->Idx]))
	{
		LOG_Error("PROFILE", DeviceClass, "", "Profile descriptor = null");
		return -1;
	}

	DeviceClass = Profile->Descr[Profile->Idx]->DeviceClass;

	if (Idx == 0)
	{
		LOG_Error("PROFILE", DeviceClass, "", "Idx = 0");
		return -1;
	}
	TableSize = PROFILE_GetTableSize(Profile, Table);
	if (Idx > TableSize)
	{
		LOG_Error("PROFILE", DeviceClass, "", "Table = %d, TableSize = %d, Idx = %d", Table, TableSize, Idx);
		return -1;
	}
	return 0;
}

uint32_t PROFILE_IsCalibParam(PROFILE_T *Profile, uint8_t Table, uint32_t Idx)
{
	switch (Table)
	{
		case PROFILE_TABLE_1:
			if (((Profile->Descr[Profile->Idx]->Table1[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table1[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				return 1;
			break;
		case PROFILE_TABLE_3:
			if (((Profile->Descr[Profile->Idx]->Table3[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table3[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				return 1;
			break;
		case PROFILE_TABLE_4:
			if (((Profile->Descr[Profile->Idx]->Table4[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table4[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				return 1;
			break;
		case PROFILE_TABLE_103:
			if (((Profile->Descr[Profile->Idx]->Table103[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table103[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				return 1;
			break;
		default:
			break;
	}
	return 0;
}

static void PROFILE_InitStates(PROFILE_T *Profile)
{
	uint32_t Idx;
	
	for (Idx = 1; Idx <= PROFILE_GetTableSize(Profile, PROFILE_TABLE_1); Idx++)
		PROFILE_SetState(Profile, PROFILE_TABLE_1, Idx, PROFILE_STATE_IS);
	for (Idx = 1; Idx <= PROFILE_GetTableSize(Profile, PROFILE_TABLE_2); Idx++)
		PROFILE_SetState(Profile, PROFILE_TABLE_2, Idx, PROFILE_STATE_IS);
	for (Idx = 1; Idx <= PROFILE_GetTableSize(Profile, PROFILE_TABLE_3); Idx++)
		PROFILE_SetState(Profile, PROFILE_TABLE_3, Idx, PROFILE_STATE_IS);
	for (Idx = 1; Idx <= PROFILE_GetTableSize(Profile, PROFILE_TABLE_4); Idx++)
		PROFILE_SetState(Profile, PROFILE_TABLE_4, Idx, PROFILE_STATE_IS);
	for (Idx = 1; Idx <= PROFILE_GetTableSize(Profile, PROFILE_TABLE_8); Idx++)
		PROFILE_SetState(Profile, PROFILE_TABLE_8, Idx, PROFILE_STATE_IS);
}

static int32_t PROFILE_InitValues(PROFILE_T *Profile)
{
	osMutexDef(ProfileValues);
	Profile->Values->Mutex = osMutexCreate(osMutex(ProfileValues));
	if ( !Profile->Values->Mutex )
		return -1;
	
	PROFILE_SetDefault(Profile);
	PROFILE_InitStates(Profile);
	
	return 0;
}

static void __PROFILE_GetValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void **Value)
{
	switch (Table)
	{
		case PROFILE_TABLE_1:
			if (((Profile->Descr[Profile->Idx]->Table1[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table1[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				*Value = PROFILE_CALIB_PARAM_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table1[Idx - 1]);
			else
				*Value = PROFILE_STATIC_PARAM_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table1[Idx - 1]);
			break;
		case PROFILE_TABLE_2:
			*Value = PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		case PROFILE_TABLE_3:
			if (((Profile->Descr[Profile->Idx]->Table3[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table3[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				*Value = PROFILE_CALIB_PARAM_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table3[Idx - 1]);
			else
				*Value = PROFILE_STATIC_PARAM_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table3[Idx - 1]);
			break;
		case PROFILE_TABLE_4:
			*Value = PROFILE_STATIC_PARAM_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table4[Idx - 1]);
			break;
		case PROFILE_TABLE_5:
			*Value = PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table5[Idx - 1]);
			break;
		case PROFILE_TABLE_8:
			*Value = PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table8[Idx - 1]);
			break;
		case PROFILE_TABLE_103:
			*Value = PROFILE_STATIC_PARAM_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table103[Idx - 1]);
			break;
		default:
			*Value = NULL;
			break;
	}
}

static void __PROFILE_GetDefaultValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void **DefaultValue)
{
	switch (Table)
	{
		case PROFILE_TABLE_1:
			if (((Profile->Descr[Profile->Idx]->Table1[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table1[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				*DefaultValue = PROFILE_CALIB_PARAM_DEFAULT_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table1[Idx - 1]);
			else
				*DefaultValue = PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table1[Idx - 1]);
			break;
		case PROFILE_TABLE_3:
			if (((Profile->Descr[Profile->Idx]->Table3[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table3[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				*DefaultValue = PROFILE_CALIB_PARAM_DEFAULT_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table3[Idx - 1]);
			else
				*DefaultValue = PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table3[Idx - 1]);
			break;
		case PROFILE_TABLE_4:
			*DefaultValue = PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table4[Idx - 1]);
			break;
		case PROFILE_TABLE_103:
			*DefaultValue = PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table103[Idx - 1]);
			break;
		default:
			*DefaultValue = NULL;
			break;
	}
}

static void __PROFILE_GetType(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t *Type)
{
	switch (Table)
	{
		case PROFILE_TABLE_1:
			*Type = Profile->Descr[Profile->Idx]->Table1[Idx - 1].Type;
			break;
		case PROFILE_TABLE_2:
			*Type = Profile->Descr[Profile->Idx]->Table2[Idx - 1].Type;
			break;
		case PROFILE_TABLE_3:
			*Type = Profile->Descr[Profile->Idx]->Table3[Idx - 1].Type;
			break;
		case PROFILE_TABLE_4:
			*Type = Profile->Descr[Profile->Idx]->Table4[Idx - 1].Type;
			break;
		case PROFILE_TABLE_5:
			*Type = Profile->Descr[Profile->Idx]->Table5[Idx - 1].Type;
			break;
		case PROFILE_TABLE_8:
			*Type = Profile->Descr[Profile->Idx]->Table8[Idx - 1].Type;
			break;
		case PROFILE_TABLE_103:
			*Type = Profile->Descr[Profile->Idx]->Table103[Idx - 1].Type;
			break;
	}
}

static void __PROFILE_GetThr(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void **Thr)
{
	switch (Table)
	{
		case PROFILE_TABLE_2:
			if ((Profile->Descr[Profile->Idx]->Table2[Idx - 1].Type == PROFILE_TYPE_ENUM) || (Profile->Descr[Profile->Idx]->Table2[Idx - 1].Type == PROFILE_TYPE_BITMSK))
				*Thr = NULL;
			else
				*Thr = PROFILE_DYNAMIC_PARAM_THR_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		case PROFILE_TABLE_3:
			if ((Profile->Descr[Profile->Idx]->Table3[Idx - 1].Type == PROFILE_TYPE_ENUM) || (Profile->Descr[Profile->Idx]->Table3[Idx - 1].Type == PROFILE_TYPE_BITMSK))
				*Thr = NULL;
			else
				*Thr = PROFILE_CONFIG_PARAM_THR_PTR(Profile, &Profile->Descr[Profile->Idx]->Table3[Idx - 1]);
			break;
		case PROFILE_TABLE_4:
			*Thr = PROFILE_STATIC_PARAM_THR_PTR(Profile, &Profile->Descr[Profile->Idx]->Table4[Idx - 1]);
			break;
		default:
			*Thr = NULL;
			break;
	}
}

static void __PROFILE_GetEnum(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, const void **Enum)
{
	switch (Table)
	{
		case PROFILE_TABLE_2:
			*Enum = PROFILE_DYNAMIC_PARAM_ENUM_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		case PROFILE_TABLE_3:
			*Enum = PROFILE_CONFIG_PARAM_ENUM_PTR(Profile, &Profile->Descr[Profile->Idx]->Table3[Idx - 1]);
			break;
		default:
			*Enum = NULL;
			break;
	}
}

static void __PROFILE_GetBitmsk(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, const void **Bitmsk)
{
	switch (Table)
	{
		case PROFILE_TABLE_2:
			*Bitmsk = PROFILE_DYNAMIC_PARAM_BITMSK_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		default:
			*Bitmsk = NULL;
			break;
	}
}

static void __PROFILE_GetDegradeColor(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t **DegradeColor)
{
	switch (Table)
	{
		case PROFILE_TABLE_2:
			*DegradeColor = PROFILE_DYNAMIC_PARAM_DEGR_COLOR_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		default:
			*DegradeColor = NULL;
			break;
	}
}

static void __PROFILE_GetFailureColor(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t **FailureColor)
{
	switch (Table)
	{
		case PROFILE_TABLE_2:
			*FailureColor = PROFILE_DYNAMIC_PARAM_FAIL_COLOR_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		default:
			*FailureColor = NULL;
			break;
	}
}

static void __PROFILE_GetDefaultDegradeColor(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t **DefaultDegradeColor)
{
	switch (Table)
	{
		case PROFILE_TABLE_2:
			*DefaultDegradeColor = PROFILE_DYNAMIC_PARAM_DEFAULT_DEGR_COLOR_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		default:
			*DefaultDegradeColor = NULL;
			break;
	}
}

static void __PROFILE_GetDefaultFailureColor(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t **DefaultFailureColor)
{
	switch (Table)
	{
		case PROFILE_TABLE_2:
			*DefaultFailureColor = PROFILE_DYNAMIC_PARAM_DEFAULT_FAIL_COLOR_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		default:
			*DefaultFailureColor = NULL;
			break;
	}
}

static void __PROFILE_GetState(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t **State)
{
	switch (Table)
	{
		case PROFILE_TABLE_1:
			*State = PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table1[Idx - 1]);
			break;
		case PROFILE_TABLE_2:
			*State = PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		case PROFILE_TABLE_3:
			*State = PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table3[Idx - 1]);
			break;
		case PROFILE_TABLE_4:
			*State = PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table4[Idx - 1]);
			break;
		case PROFILE_TABLE_8:
			*State = PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table8[Idx - 1]);
			break;
		default:
			*State = NULL;
			break;
	}
}

int32_t PROFILE_Init(PROFILE_T *Profile, PROFILE_DESCR_T **Descr, PROFILE_VALUES_T *Values)
{	
	Profile->Idx = 0;
	Profile->Descr = Descr;
	Profile->Values = Values;
	Profile->TablesChanges = 0;
	Profile->SetEnable = 0;
	Profile->ValuesLock = 0;
	if (PROFILE_InitValues(Profile))
		return -1;
	return 0;
}


int32_t PROFILE_ValueToString(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, char* Str , uint32_t *Len)
{
	char *p = Str;
	uint8_t Type;
	
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	__PROFILE_GetType(Profile,Table,Idx, &Type);

	if( !Str || !Value || !Len ) 
		return -1;//На всякий случай, указатели должны быть ненулевые
	
	switch( Type )
	{
		//разная печать в зависимости от типа параметра
		case PROFILE_TYPE_BOOL:
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_CNT://счётчик без знака
		case PROFILE_TYPE_BITMSK:
			p = UintToStr(p, *(unsigned int*)Value);
			break;
		case PROFILE_TYPE_ENUM://перечисление Без знака
		case PROFILE_TYPE_INT32:// со знаком
			p = IntToStr(p, *(int*)Value);// печать
			break;
		case PROFILE_TYPE_HEX32:// шестнадцатеричный формат печати
			p = UintToHexStr(p, *(unsigned int*)Value);
			break;
		case PROFILE_TYPE_OID:// строка из чисел, разделённых "."
		case PROFILE_TYPE_STRING:// произвольная строка
			strncpy(p, Value, PROFILE_STRING_MAX_LEN);
			p[PROFILE_STRING_MAX_LEN - 1] = 0;
			p += strlen(p);
			break;
		case PROFILE_TYPE_FLOAT_1:// плавающая точка формата (+/-)ххх.х
			p = FloatToStr( p, *(float*)Value, 1 );
			break;
		case PROFILE_TYPE_FLOAT_2:// плавающая точка формата (+/-)ххх.хx
			p = FloatToStr( p, *(float*)Value, 2 );
			break;
		case PROFILE_TYPE_FLOAT_3:// плавающая точка формата (+/-)ххх.хxx
			p = FloatToStr( p, *(float*)Value, 2 );
			break;
		case PROFILE_TYPE_FLOAT_E:// плавающая точка формата (+/-)x.xxe-xx
			sprintf( p, "%.2e", *(float*)Value );
			p += strlen(Str);
			break;
		case PROFILE_TYPE_IP://массив из 4 байтов, выводится на печать в формате ххх.ххх.ххх.ххх
			p = IPToStr( p, *(unsigned int*)Value);
			break;
		case PROFILE_TYPE_UINT64:
			sprintf( p, "%llu", *(uint64_t*)Value );
			p += strlen(Str);
			break;
		default:
			return -1;		
	}
	*Len = p - Str; // длина напечатанного
	return 0;
}

int32_t PROFILE_StringToValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, char* Str)
{
	uint8_t Type;
	
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	__PROFILE_GetType(Profile,Table,Idx, &Type);
	
	if( !Str || !Value ) 
		return -1;//На всякий случай
	switch( Type )
	{
		case PROFILE_TYPE_BOOL:
		case PROFILE_TYPE_ENUM:
		case PROFILE_TYPE_BITMSK:
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_CNT://счётчик - беззнаковое
			if( !StrToUint( Str, Value, 0 ) )
				return -1;// проверка корректности строкового представления в звпросе и преобразование в число
			break;
		case PROFILE_TYPE_INT32://Со знаком
			if( !StrToInt( Str, Value, 0 ) )
				return -1;// проверка корректности строкового представления в звпросе и преобразование в число
			break;
		case PROFILE_TYPE_HEX32:
			if( !HexStrToUint( Str, Value, 0 ) )
				return -1;// проверка корректности строкового представления в звпросе и преобразование в число
			break;
		case PROFILE_TYPE_STRING:
		case PROFILE_TYPE_OID:
			strncpy(Value, Str, PROFILE_STRING_MAX_LEN);
			((char*)Value)[PROFILE_STRING_MAX_LEN - 1] = 0;
			break;
		case PROFILE_TYPE_FLOAT_1:
		case PROFILE_TYPE_FLOAT_2:
		case PROFILE_TYPE_FLOAT_3:
		case PROFILE_TYPE_FLOAT_E:
			if( !StrToFloat( Str, Value, 0 ) ) 
				return -1;// проверка корректности строкового представления в звпросе и преобразование в число
			break;
		case PROFILE_TYPE_IP:		
			if( !StrToIP( Str, Value ))
				return -1;// проверка корректности строкового представления в звпросе и преобразование в число
			break;
		case PROFILE_TYPE_UINT64:
			*(uint64_t*)Value = strtoull(Str, &Str, 10);
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_CheckThresholdValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	uint32_t i;
	uint8_t Type;
	void *ParamValue, *ParamValueThr;
	
	__PROFILE_GetType(Profile, Table, Idx, &Type);
	__PROFILE_GetValue(Profile, Table, Idx, &ParamValue);
	__PROFILE_GetThr(Profile, Table, Idx, &ParamValueThr);
	
	for( i = 0; i < 4; i++)
	{
		switch (Type)
		{
			case PROFILE_TYPE_UINT32:
			case PROFILE_TYPE_CNT:
			case PROFILE_TYPE_HEX32:
				if((uint32_t*)ParamValue == &((uint32_t*)ParamValueThr)[i])
				{
					if ((i == PROFILE_THR_CMIN) && (*(uint32_t*)Value > ((uint32_t*)ParamValueThr)[PROFILE_THR_WMIN]))
						return -1;
					else if ((i == PROFILE_THR_CMAX) && (*(uint32_t*)Value < ((uint32_t*)ParamValueThr)[PROFILE_THR_WMAX]))
						return -1;
					else if ((i == PROFILE_THR_WMAX) && ((*(uint32_t*)Value > ((uint32_t*)ParamValueThr)[PROFILE_THR_CMAX]) || (*(uint32_t*)Value < ((uint32_t*)ParamValueThr)[PROFILE_THR_WMIN])))
						return -1;
					else if ((i == PROFILE_THR_WMIN) && ((*(uint32_t*)Value > ((uint32_t*)ParamValueThr)[PROFILE_THR_WMAX]) || (*(uint32_t*)Value < ((uint32_t*)ParamValueThr)[PROFILE_THR_CMIN])))
						return -1;
					return 0;
				}
				break;
			case PROFILE_TYPE_INT32:
				if((int32_t*)ParamValue == &((int32_t*)ParamValueThr)[i])
				{
					if ((i == PROFILE_THR_CMIN) && (*(int32_t*)Value > ((int32_t*)ParamValueThr)[PROFILE_THR_WMIN]))
						return -1;
					else if ((i == PROFILE_THR_CMAX) && (*(int32_t*)Value < ((int32_t*)ParamValueThr)[PROFILE_THR_WMAX]))
						return -1;
					else if ((i == PROFILE_THR_WMAX) && ((*(int32_t*)Value > ((int32_t*)ParamValueThr)[PROFILE_THR_CMAX]) || (*(int32_t*)Value < ((int32_t*)ParamValueThr)[PROFILE_THR_WMIN])))
						return -1;
					else if ((i == PROFILE_THR_WMIN) && ((*(int32_t*)Value > ((int32_t*)ParamValueThr)[PROFILE_THR_WMAX]) || (*(int32_t*)Value < ((int32_t*)ParamValueThr)[PROFILE_THR_CMIN])))
						return -1;
					return 0;
				}
				break;
			case PROFILE_TYPE_FLOAT_1:
			case PROFILE_TYPE_FLOAT_2:
			case PROFILE_TYPE_FLOAT_3:
			case PROFILE_TYPE_FLOAT_E:
				if((float*)ParamValue == &((float*)ParamValueThr)[i])
				{
					if ((i == PROFILE_THR_CMIN) && (*(float*)Value > ((float*)ParamValueThr)[PROFILE_THR_WMIN]))
						return -1;
					else if ((i == PROFILE_THR_CMAX) && (*(float*)Value < ((float*)ParamValueThr)[PROFILE_THR_WMAX]))
						return -1;
					else if ((i == PROFILE_THR_WMAX) && ((*(float*)Value > ((float*)ParamValueThr)[PROFILE_THR_CMAX]) || (*(float*)Value < ((float*)ParamValueThr)[PROFILE_THR_WMIN])))
						return -1;
					else if ((i == PROFILE_THR_WMIN) && ((*(float*)Value > ((float*)ParamValueThr)[PROFILE_THR_WMAX]) || (*(float*)Value < ((float*)ParamValueThr)[PROFILE_THR_CMIN])))
						return -1;
					return 0;
				}
				break;
			case PROFILE_TYPE_UINT64:
				if((uint64_t*)ParamValue == &((uint64_t*)ParamValueThr)[i])
				{
					if ((i == PROFILE_THR_CMIN) && (*(uint64_t*)Value > ((uint64_t*)ParamValueThr)[PROFILE_THR_WMIN]))
						return -1;
					else if ((i == PROFILE_THR_CMAX) && (*(uint64_t*)Value < ((uint64_t*)ParamValueThr)[PROFILE_THR_WMAX]))
						return -1;
					else if ((i == PROFILE_THR_WMAX) && ((*(uint64_t*)Value > ((uint64_t*)ParamValueThr)[PROFILE_THR_CMAX]) || (*(uint64_t*)Value < ((uint64_t*)ParamValueThr)[PROFILE_THR_WMIN])))
						return -1;
					else if ((i == PROFILE_THR_WMIN) && ((*(uint64_t*)Value > ((uint64_t*)ParamValueThr)[PROFILE_THR_WMAX]) || (*(uint64_t*)Value < ((uint64_t*)ParamValueThr)[PROFILE_THR_CMIN])))
						return -1;
					return 0;
				}
				break;
			default:
				return -1;			
		}
	}
	return -1;
}

int32_t PROFILE_CheckConfigValueRange(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore)
{
	uint32_t i;
	uint8_t Type;
	void *ParamValue, *ParamValueThr;
	const void *ParamValueEnum;
	
	__PROFILE_GetType(Profile, Table, Idx, &Type);
	__PROFILE_GetValue(Profile, Table, Idx, &ParamValue);
	__PROFILE_GetThr(Profile, Table, Idx, &ParamValueThr);
	__PROFILE_GetEnum(Profile, Table, Idx, &ParamValueEnum);
	
	switch (Type)
	{
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_CNT:
		case PROFILE_TYPE_HEX32:
			if((ParamValueThr) && (!Restore))
			{
				if ((*(uint32_t*)Value) < ((uint32_t*)ParamValueThr)[PROFILE_THR_HMIN])
					return -1;
				else if ((*(uint32_t*)Value) > ((uint32_t*)ParamValueThr)[PROFILE_THR_HMAX])
					return -1;
			}
			break;
		case PROFILE_TYPE_ENUM:
			if(ParamValueEnum)
			{
				for (i = 1; i <= ((PROFILE_ENUM_T*)ParamValueEnum)[0].Value; i++)
				{
					if (*(uint32_t*)Value == ((PROFILE_ENUM_T*)ParamValueEnum)[i].Value)
						return 0;
				}
				return -1;
			}
			break;
		case PROFILE_TYPE_INT32:
			if((ParamValueThr) && (!Restore))
			{
				if ((*(int32_t*)Value) < ((int32_t*)ParamValueThr)[PROFILE_THR_HMIN])
					return -1;
				else if ((*(int32_t*)Value) > ((int32_t*)ParamValueThr)[PROFILE_THR_HMAX])
					return -1;
			}
			break;
		case PROFILE_TYPE_FLOAT_1:
		case PROFILE_TYPE_FLOAT_2:
		case PROFILE_TYPE_FLOAT_3:
		case PROFILE_TYPE_FLOAT_E:
			if((ParamValueThr) && (!Restore))
			{
				if ((*(float*)Value) < ((float*)ParamValueThr)[PROFILE_THR_HMIN])
					return -1;
				else if ((*(float*)Value) > ((float*)ParamValueThr)[PROFILE_THR_HMAX])
					return -1;
			}
			break;
		case PROFILE_TYPE_UINT64:
			if((ParamValueThr) && (!Restore))
			{
				if ((*(uint64_t*)Value) < ((uint64_t*)ParamValueThr)[PROFILE_THR_HMIN])
					return -1;
				else if ((*(uint64_t*)Value) > ((uint64_t*)ParamValueThr)[PROFILE_THR_HMAX])
					return -1;
			}
			break;
		case PROFILE_TYPE_STRING:
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_CheckTypeValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	uint32_t i;
	uint8_t Type;
	char* Str;

	__PROFILE_GetType(Profile, Table, Idx, &Type);

	switch (Type)
	{
		case PROFILE_TYPE_STRING:
			Str = Value;
			for (i = 0; i < PROFILE_STRING_MAX_LEN; i++)
			{
				if (Str[i] == 0)
					break;
				if (Str[i] <= 0x1F)
					return -1;
				if (Str[i] == 0x7F)
					return -1;
				if (Str[i] == '@')
					return -1;
				if (Str[i] == ';')
					return -1;
			}
			if (i == PROFILE_STRING_MAX_LEN)
				return -1;
			break;
		default:
			break;
	}
	return 0;
}

int32_t PROFILE_CheckValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore)
{
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	if (PROFILE_CheckTypeValue(Profile, Table, Idx, Value))
		return -1;
	if ((Table == PROFILE_TABLE_4) && (!Restore))
		return PROFILE_CheckThresholdValue(Profile, Table, Idx, Value);
	if (Table == PROFILE_TABLE_3)
	{
		if (PROFILE_CheckConfigValueRange(Profile, Table, Idx, Value, Restore))
			return -1;
		if (PROFILE_CheckConfigValue(Profile, Table, Idx, Value, Restore))
			return -1;
	}
	return 0;
}

int32_t PROFILE_SetValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore)
{
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	if (Table == PROFILE_TABLE_3)
		return PROFILE_SetConfigValueDependencies(Profile, Table, Idx, Value, Restore);
	return 0;
}

int32_t PROFILE_SetValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *SetValue)
{
	void *Value;
	uint8_t Type;
	uint8_t *State;
	
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	if (Profile->ValuesLock)
		return -1;
	
	__PROFILE_GetValue(Profile, Table, Idx, &Value);
	__PROFILE_GetType(Profile, Table, Idx, &Type);
	__PROFILE_GetState(Profile, Table, Idx, &State);
	
	switch( Type )
	{
		case PROFILE_TYPE_BOOL:
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_CNT:
		case PROFILE_TYPE_BITMSK:
		case PROFILE_TYPE_HEX32:
		case PROFILE_TYPE_IP:
		case PROFILE_TYPE_ENUM:
		case PROFILE_TYPE_INT32:
		case PROFILE_TYPE_FLOAT_1:
		case PROFILE_TYPE_FLOAT_2:
		case PROFILE_TYPE_FLOAT_3:
		case PROFILE_TYPE_FLOAT_E:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, SetValue, 4))
			{
				memcpy(Value, SetValue, 4);
				if ((!State) || (*State != PROFILE_STATE_OOS))
				{
					Profile->TablesChanges |= 1 << Table;
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_UINT64:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, SetValue, sizeof(uint64_t)))
			{
				memcpy(Value, SetValue, sizeof(uint64_t));
				if ((!State) || (*State != PROFILE_STATE_OOS))
				{
					Profile->TablesChanges |= 1 << Table;
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_OID:// строка из чисел, разделённых "."
		case PROFILE_TYPE_STRING:// произвольная строка
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (strncmp((char*)Value, (char*)SetValue, PROFILE_STRING_MAX_LEN))
			{
				strncpy( (char*)Value, (char*)SetValue, PROFILE_STRING_MAX_LEN);
				((char*)Value)[ PROFILE_STRING_MAX_LEN - 1 ] = 0;
				if ((!State) || (*State != PROFILE_STATE_OOS))
				{
					Profile->TablesChanges |= 1 << Table;
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_GetValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *GetValue)
{
	void *Value;
	uint8_t Type;
	uint8_t *State;
	
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	if (Profile->ValuesLock)
		return -1;
	
	__PROFILE_GetValue(Profile, Table, Idx, &Value);
	__PROFILE_GetType(Profile, Table, Idx, &Type);
	__PROFILE_GetState(Profile, Table, Idx, &State);
	
	switch( Type )
	{
		case PROFILE_TYPE_BOOL:
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_CNT:
		case PROFILE_TYPE_BITMSK:
		case PROFILE_TYPE_HEX32:
		case PROFILE_TYPE_IP:
		case PROFILE_TYPE_ENUM:
		case PROFILE_TYPE_INT32:
		case PROFILE_TYPE_FLOAT_1:
		case PROFILE_TYPE_FLOAT_2:
		case PROFILE_TYPE_FLOAT_3:
		case PROFILE_TYPE_FLOAT_E:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			memcpy(GetValue, Value, 4);
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_UINT64:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			memcpy(GetValue, Value, sizeof(uint64_t));
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_OID:// строка из чисел, разделённых "."
		case PROFILE_TYPE_STRING:// произвольная строка
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			strncpy( (char*)GetValue, (char*)Value, PROFILE_STRING_MAX_LEN);
			((char*)GetValue)[ PROFILE_STRING_MAX_LEN - 1 ] = 0;
			osMutexRelease( Profile->Values->Mutex );
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_SetFactoryValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx)
{
	void *DefaultValue;
	
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	if (Profile->ValuesLock)
		return -1;
	
	__PROFILE_GetDefaultValue(Profile, Table, Idx, &DefaultValue);
	
	return PROFILE_SetValue(Profile, Table, Idx, DefaultValue);	
}

int32_t PROFILE_SetAlarmColors(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t SetDegradeColor, uint8_t SetFailureColor)
{
	uint8_t *DegradeColor, *FailureColor;
	
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	if (Profile->ValuesLock)
		return-1;
	
	__PROFILE_GetDegradeColor(Profile, Table, Idx, &DegradeColor);
	__PROFILE_GetFailureColor(Profile, Table, Idx, &FailureColor);
	
	osMutexWait( Profile->Values->Mutex, osWaitForever );
	if ((DegradeColor) && (SetDegradeColor != PROFILE_COLOR_NA) && (SetDegradeColor <= PROFILE_COLOR_CRITICAL))
		*DegradeColor = SetDegradeColor;
	if ((FailureColor) && (SetFailureColor != PROFILE_COLOR_NA) && (SetFailureColor <= PROFILE_COLOR_CRITICAL))
		*FailureColor = SetFailureColor;
	osMutexRelease( Profile->Values->Mutex );
	return 0;
}

int32_t PROFILE_GetAlarmColors(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t *GetDegradeColor, uint8_t *GetFailureColor)
{
	uint8_t *DegradeColor, *FailureColor;
	
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	if (Profile->ValuesLock)
		return -1;
	
	__PROFILE_GetDegradeColor(Profile, Table, Idx, &DegradeColor);
	__PROFILE_GetFailureColor(Profile, Table, Idx, &FailureColor);
	
	osMutexWait( Profile->Values->Mutex, osWaitForever );
	if ((GetDegradeColor) && (DegradeColor))
		*GetDegradeColor = *DegradeColor;
	else if ((GetDegradeColor) && (!DegradeColor))
		*GetDegradeColor = PROFILE_COLOR_NA;
	if ((GetFailureColor) && (FailureColor))
		*GetFailureColor = *FailureColor;
	else if ((GetFailureColor) && (!FailureColor))
		*GetFailureColor = PROFILE_COLOR_NA;
	osMutexRelease( Profile->Values->Mutex );
	return 0;
}

int32_t PROFILE_SetFactoryAlarmColors(PROFILE_T *Profile, uint8_t Table, uint16_t Idx)
{
	uint8_t *DefaultDegradeColor, *DefaultFailureColor;
	
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	if (Profile->ValuesLock)
		return -1;
		
	__PROFILE_GetDefaultDegradeColor(Profile, Table, Idx, &DefaultDegradeColor);
	__PROFILE_GetDefaultFailureColor(Profile, Table, Idx, &DefaultFailureColor);
	
	return PROFILE_SetAlarmColors(Profile, Table, Idx, DefaultDegradeColor ? *DefaultDegradeColor : PROFILE_COLOR_NA, DefaultFailureColor ? *DefaultFailureColor : PROFILE_COLOR_NA);
}

int32_t PROFILE_SetFactoryTable(PROFILE_T *Profile, uint8_t Table)
{
	uint32_t Idx;
	
	if (Profile->ValuesLock)
		return -1;
	
	if ((Table == PROFILE_TABLE_1) || (Table == PROFILE_TABLE_3) || (Table == PROFILE_TABLE_4) || (Table == PROFILE_TABLE_103))
	{
		for (Idx = 1; Idx <= PROFILE_GetTableSize(Profile, Table); Idx++)
		{
			/* Do not set parameters with write access for superuser only */
			if (PROFILE_IsCalibParam(Profile, Table, Idx))
				continue;
			PROFILE_SetFactoryValue(Profile, Table, Idx);
		}
	}
	else if (Table == PROFILE_TABLE_2)
	{
		for (Idx = 1; Idx <= PROFILE_GetTableSize(Profile, Table); Idx++)
		{
			PROFILE_SetFactoryAlarmColors(Profile, Table, Idx);
		}
	}
	return 0;
}

int32_t PROFILE_SetDefault(PROFILE_T *Profile)
{
	if (Profile->ValuesLock)
		return - 1;
	
	osMutexWait( Profile->Values->Mutex, osWaitForever );
	memset(Profile->Values->Dynamic, 0, Profile->Descr[Profile->Idx]->DynamicValuesSize);
	memcpy(Profile->Values->Calib, Profile->Descr[Profile->Idx]->CalibDefaultValues, Profile->Descr[Profile->Idx]->CalibValuesSize);
	memcpy(Profile->Values->Static, Profile->Descr[Profile->Idx]->StaticDefaultValues, Profile->Descr[Profile->Idx]->StaticValuesSize);
	osMutexRelease( Profile->Values->Mutex );
	return 0;
}

int32_t PROFILE_SetFactory(PROFILE_T *Profile)
{
	if (Profile->ValuesLock)
		return - 1;
	
	osMutexWait( Profile->Values->Mutex, osWaitForever );
	memcpy(Profile->Values->Static, Profile->Descr[Profile->Idx]->StaticDefaultValues, Profile->Descr[Profile->Idx]->StaticValuesSize);
	osMutexRelease( Profile->Values->Mutex );
	return 0;
}

int32_t PROFILE_GetColor(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t *Color)
{
	uint32_t i;
	void *ParamValue, *ParamValueThr;
	const void *ParamValueEnum, *ParamValueBitmsk;
	uint8_t Type;
	uint8_t *DegradeColor;
	uint8_t *FailureColor;
	
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	if (Profile->ValuesLock)
		return -1;
	
	__PROFILE_GetType(Profile, Table, Idx, &Type);
	__PROFILE_GetValue(Profile, Table, Idx, &ParamValue);
	__PROFILE_GetThr(Profile, Table, Idx, &ParamValueThr);
	__PROFILE_GetEnum(Profile, Table, Idx, &ParamValueEnum);
	__PROFILE_GetBitmsk(Profile, Table, Idx, &ParamValueBitmsk);
	__PROFILE_GetDegradeColor(Profile, Table, Idx, &DegradeColor);
	__PROFILE_GetFailureColor(Profile, Table, Idx, &FailureColor);

	switch( Type )
	{
		case PROFILE_TYPE_BOOL:
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_HEX32:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if ((!ParamValueThr) || (!DegradeColor) || (!FailureColor))
			{
				*Color = PROFILE_COLOR_NA;
			}
			else
			{
				if ((*(uint32_t*)ParamValue > ((uint32_t*)ParamValueThr)[PROFILE_THR_CMAX]) ||\
					(*(uint32_t*)ParamValue < ((uint32_t*)ParamValueThr)[PROFILE_THR_CMIN]))
					*Color = *FailureColor;
				else if ((*(uint32_t*)ParamValue > ((uint32_t*)ParamValueThr)[PROFILE_THR_WMAX]) ||\
					(*(uint32_t*)ParamValue < ((uint32_t*)ParamValueThr)[PROFILE_THR_WMIN]))
					*Color = *DegradeColor;
				else
					*Color = PROFILE_COLOR_NORMAL;
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_INT32:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if ((!ParamValueThr) || (!DegradeColor) || (!FailureColor))
			{
				*Color = PROFILE_COLOR_NA;
			}
			else
			{
				if ((*(int32_t*)ParamValue > ((int32_t*)ParamValueThr)[PROFILE_THR_CMAX]) ||\
					(*(int32_t*)ParamValue < ((int32_t*)ParamValueThr)[PROFILE_THR_CMIN]))
					*Color = *FailureColor;
				else if ((*(int32_t*)ParamValue > ((int32_t*)ParamValueThr)[PROFILE_THR_WMAX]) ||\
					(*(int32_t*)ParamValue < ((int32_t*)ParamValueThr)[PROFILE_THR_WMIN]))
					*Color = *DegradeColor;
				else
					*Color = PROFILE_COLOR_NORMAL;
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_FLOAT_1:
		case PROFILE_TYPE_FLOAT_2:
		case PROFILE_TYPE_FLOAT_3:
		case PROFILE_TYPE_FLOAT_E:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if ((!ParamValueThr) || (!DegradeColor) || (!FailureColor))
			{
				*Color = PROFILE_COLOR_NA;
			}
			else
			{
				if ((*(float*)ParamValue > ((float*)ParamValueThr)[PROFILE_THR_CMAX]) ||\
					(*(float*)ParamValue < ((float*)ParamValueThr)[PROFILE_THR_CMIN]))
					*Color = *FailureColor;
				else if ((*(float*)ParamValue > ((float*)ParamValueThr)[PROFILE_THR_WMAX]) ||\
					(*(float*)ParamValue < ((float*)ParamValueThr)[PROFILE_THR_WMIN]))
					*Color = *DegradeColor;
				else
					*Color = PROFILE_COLOR_NORMAL;
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_UINT64:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if ((!ParamValueThr) || (!DegradeColor) || (!FailureColor))
			{
				*Color = PROFILE_COLOR_NA;
			}
			else
			{
				if ((*(uint64_t*)ParamValue > ((uint64_t*)ParamValueThr)[PROFILE_THR_CMAX]) ||\
					(*(uint64_t*)ParamValue < ((uint64_t*)ParamValueThr)[PROFILE_THR_CMIN]))
					*Color = *FailureColor;
				else if ((*(uint64_t*)ParamValue > ((uint64_t*)ParamValueThr)[PROFILE_THR_WMAX]) ||\
					(*(uint64_t*)ParamValue < ((uint64_t*)ParamValueThr)[PROFILE_THR_WMIN]))
					*Color = *DegradeColor;
				else
					*Color = PROFILE_COLOR_NORMAL;
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_ENUM:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if ((!ParamValueEnum) || (!DegradeColor) || (!FailureColor))
			{
				*Color = PROFILE_COLOR_NA;
			}
			else
			{
				*Color = PROFILE_COLOR_NORMAL;
				for ( i = 1; i <= ((PROFILE_ENUM_T*)ParamValueEnum)[0].Value; i++)
				{
					if (*((uint32_t*)ParamValue) == ((PROFILE_ENUM_T*)ParamValueEnum)[i].Value)
					{
						if (((PROFILE_ENUM_T*)ParamValueEnum)[i].Color == PROFILE_COLOR_TYPE_DEGRADE)
							*Color = *DegradeColor;
						else if (((PROFILE_ENUM_T*)ParamValueEnum)[i].Color == PROFILE_COLOR_TYPE_FAILURE)
							*Color = *FailureColor;
						break;
					}
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_BITMSK:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if ((!ParamValueBitmsk) || (!DegradeColor) || (!FailureColor))
			{
				*Color = PROFILE_COLOR_NA;
			}
			else
			{
				*Color = PROFILE_COLOR_NORMAL;
				for ( i = 1; i <= ((PROFILE_BITMSK_T*)ParamValueBitmsk)[0].Value; i++)
				{
					if (*((uint32_t*)ParamValue) & ((PROFILE_BITMSK_T*)ParamValueBitmsk)[i].Value)
					{
						if (((PROFILE_ENUM_T*)ParamValueBitmsk)[i].Color == PROFILE_COLOR_TYPE_DEGRADE)
						{
							if (*Color < *DegradeColor)
								*Color = *DegradeColor;
						}
						else if (((PROFILE_ENUM_T*)ParamValueEnum)[i].Color == PROFILE_COLOR_TYPE_FAILURE)
						{
							if (*Color < *FailureColor)
								*Color = *FailureColor;
						}
							
					}
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		default:
			__PROFILE_GetStringColor(Profile, Table, Idx, Color, DegradeColor, FailureColor);
			break;
	}
	return 0;
}

int32_t PROFILE_GetState(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t *GetState)
{
	uint8_t *State;
	
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	if (Profile->ValuesLock)
		return -1;
	
	__PROFILE_GetState(Profile, Table, Idx, &State);
	
	osMutexWait( Profile->Values->Mutex, osWaitForever );
	if (State)
		*GetState = *State;
	else
		*GetState = PROFILE_STATE_IS;
	osMutexRelease( Profile->Values->Mutex );
	return 0;
}

int32_t PROFILE_SetState(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t SetState)
{
	uint8_t *State;
	void *ParamValueThr;
	
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;

	if (Profile->ValuesLock)
		return -1;
	
	__PROFILE_GetState(Profile, Table, Idx, &State);
	__PROFILE_GetThr(Profile, Table, Idx, &ParamValueThr);
	
	osMutexWait( Profile->Values->Mutex, osWaitForever );
	if (State)
	{
		if (*State != SetState)
		{
			*State = SetState;
			Profile->TablesChanges |= 1 << Table;
			if (ParamValueThr)
			{
				Profile->TablesChanges |= 1 << PROFILE_TABLE_4;
			}
		}
	}
	osMutexRelease( Profile->Values->Mutex );
	return 0;
}

int32_t PROFILE_UpdateParam(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint8_t State)
{
	if (PROFILE_CheckTableIdx(Profile, Table, Idx))
		return -1;
	if (PROFILE_CheckTypeValue(Profile, Table, Idx, Value))
		return -1;
	if (PROFILE_SetValue(Profile, Table, Idx, Value))
		return -1;
	if (PROFILE_SetState(Profile, Table, Idx, State))
		return -1;
	return 0;
}
