#include "profile.h"
#include "profile_flash.h"
#include "string_fnc.h"
#include "crc.h"
#include <string.h>
#include "cmsis_os.h"
#include "bootloader.h"

#define PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, Param) 							(void*)((uint32_t)((Profile)->Values.Dynamic) + (uint32_t)((Param)->Value))
#define PROFILE_STATIC_PARAM_VALUE_PTR(Profile, Param) 								(void*)((uint32_t)((Profile)->Values.Static) + (uint32_t)((Param)->Value))
#define PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, Param)				(void*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->DefaultValues) + (uint32_t)((Param)->Value))
#define PROFILE_DYNAMIC_PARAM_THR_PTR(Profile, Param) 								(void*)((uint32_t)((Profile)->Values.Static) + (uint32_t)((Param)->Thesholds))
#define PROFILE_STATIC_PARAM_THR_PTR(Profile, Param) 									(void*)((uint32_t)((Profile)->Values.Static) + (uint32_t)((Param)->Thesholds))
#define PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) 							(Param)->Color ? (uint8_t*)((uint32_t)((Profile)->Values.Dynamic) + (uint32_t)((Param)->Color)) : NULL
#define PROFILE_DYNAMIC_PARAM_DEGR_COLOR_PTR(Profile, Param) 					(Param)->DegradeColor ? (uint8_t*)((uint32_t)((Profile)->Values.Static) + (uint32_t)((Param)->DegradeColor)) : NULL
#define PROFILE_DYNAMIC_PARAM_DEFAULT_DEGR_COLOR_PTR(Profile, Param) 	(Param)->DegradeColor ? (uint8_t*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->DefaultValues) + (uint32_t)((Param)->DegradeColor)) : NULL
#define PROFILE_DYNAMIC_PARAM_FAIL_COLOR_PTR(Profile, Param) 					(Param)->FailureColor ? (uint8_t*)((uint32_t)((Profile)->Values.Static) + (uint32_t)((Param)->FailureColor)) : NULL
#define PROFILE_DYNAMIC_PARAM_DEFAULT_FAIL_COLOR_PTR(Profile, Param) 	(Param)->FailureColor ? (uint8_t*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->DefaultValues) + (uint32_t)((Param)->FailureColor)) : NULL
#define PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, Param) 							(Param)->State ? (uint8_t*)((uint32_t)((Profile)->Values.Dynamic) + (uint32_t)((Param)->State)) : NULL

__weak int32_t PROFILE_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	return 0;
}

__weak int32_t PROFILE_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	return 0;
}

static int32_t PROFILE_InitValues(PROFILE_VALUES_T *Values, void *Static, uint32_t StaticSize, void *Dynamic, uint32_t DynamicSize)
{
	osMutexDef(ProfileValues);
	Values->Dynamic = Dynamic;
	Values->DynamicSize = DynamicSize;
	Values->Static = Static;
	Values->StaticSize = StaticSize;
	Values->Mutex = osMutexCreate(osMutex(ProfileValues));
	if ( !Values->Mutex )
		return -1;
	return 0;
}

static void __PROFILE_GetValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void **Value)
{
	switch (Table)
	{
		case PROFILE_TABLE_1:
			*Value = PROFILE_STATIC_PARAM_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table1[Idx - 1]);
			break;
		case PROFILE_TABLE_2:
			*Value = PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		case PROFILE_TABLE_3:
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
			*DefaultValue = PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table1[Idx - 1]);
			break;
		case PROFILE_TABLE_3:
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

static void __PROFILE_GetColor(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t **Color)
{
	switch (Table)
	{
		case PROFILE_TABLE_2:
			*Color = PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		default:
			*Color = NULL;
			break;
	}
}

static void __PROFILE_GetThr(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void **Thr)
{
	switch (Table)
	{
		case PROFILE_TABLE_2:
			*Thr = PROFILE_DYNAMIC_PARAM_THR_PTR(Profile, &Profile->Descr[Profile->Idx]->Table2[Idx - 1]);
			break;
		case PROFILE_TABLE_4:
			*Thr = PROFILE_STATIC_PARAM_THR_PTR(Profile, &Profile->Descr[Profile->Idx]->Table4[Idx - 1]);
			break;
		default:
			*Thr = NULL;
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
		case PROFILE_TABLE_8:
			*State = PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, &Profile->Descr[Profile->Idx]->Table8[Idx - 1]);
			break;
		default:
			*State = NULL;
			break;
	}
}

int32_t PROFILE_Init(PROFILE_T *Profile, PROFILE_DESCR_T **Descr, void *Static, uint32_t StaticSize, void *Dynamic, uint32_t DynamicSize)
{	
	Profile->Idx = 0;
	Profile->Descr = Descr;
	Profile->TablesChanges = 0;
	Profile->SetEnable = 0;
	if (PROFILE_InitValues(&Profile->Values, Static, StaticSize, Dynamic, DynamicSize))
		return -1;
	
	BOOTLOADER_Init(&Profile->Bootloader);
	if( PROFILE_FLASH_Init(&Profile->ProfileFlash, "Profile Flash", Profile->Values.Static, Profile->Values.StaticSize, Profile->Values.Mutex))
		goto error;
	
	if (PROFILE_FLASH_ProfileIdxLoad(&Profile->ProfileFlash, &Profile->Idx))
	{
		Profile->Idx = 0;
		PROFILE_FLASH_ProfileIdxSave(&Profile->ProfileFlash, Profile->Idx);
	}
	if (PROFILE_FLASH_ParamsLoad(&Profile->ProfileFlash))
		goto error;
	
	return 0;
	
error:
	PROFILE_SetDefault(Profile);
	PROFILE_FLASH_ParamsSave(&Profile->ProfileFlash);
	return 0;
}


int32_t PROFILE_ValueToString(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, char* Str , uint32_t *Len)
{
	char *p = Str;
	uint8_t Type;
	
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
		default:
			return -1;		
	}
	*Len = p - Str; // длина напечатанного
	return 0;
}

int32_t PROFILE_StringToValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, char* Str)
{
	uint8_t Type;
	
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
	
	for( i = 0; i < 6; i++)
	{
		switch (Type)
		{
			case PROFILE_TYPE_BOOL:
			case PROFILE_TYPE_UINT32:
			case PROFILE_TYPE_CNT:
			case PROFILE_TYPE_BITMSK:
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
			case PROFILE_TYPE_ENUM:
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
			default:
				return -1;			
		}
	}
	return -1;
}

int32_t PROFILE_CheckValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	if (Table == PROFILE_TABLE_4)
		return PROFILE_CheckThresholdValue(Profile, Table, Idx, Value);
	if (Table == PROFILE_TABLE_3)
		return PROFILE_CheckConfigValue(Profile, Table, Idx, Value);
	return 0;
}

int32_t PROFILE_SetValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	if (Table == PROFILE_TABLE_3)
		return PROFILE_SetConfigValueDependencies(Profile, Table, Idx, Value);
	return 0;
}

int32_t PROFILE_SetValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *SetValue)
{
	void *Value;
	uint8_t Type;
	uint8_t *State;
	
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
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if (memcmp(Value, SetValue, 4))
			{
				memcpy(Value, SetValue, 4);
				if ((!State) || (*State != PROFILE_STATE_OOS))
				{
					Profile->TablesChanges |= 1 << Table;
				}
			}
			osMutexRelease( Profile->Values.Mutex );
			break;
		case PROFILE_TYPE_OID:// строка из чисел, разделённых "."
		case PROFILE_TYPE_STRING:// произвольная строка
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if (strncmp((char*)Value, (char*)SetValue, PROFILE_STRING_MAX_LEN))
			{
				strncpy( (char*)Value, (char*)SetValue, PROFILE_STRING_MAX_LEN);
				((char*)Value)[ PROFILE_STRING_MAX_LEN - 1 ] = 0;
				if ((!State) || (*State != PROFILE_STATE_OOS))
				{
					Profile->TablesChanges |= 1 << Table;
				}
			}
			osMutexRelease( Profile->Values.Mutex );
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
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			memcpy(GetValue, Value, 4);
			osMutexRelease( Profile->Values.Mutex );
			break;
		case PROFILE_TYPE_OID:// строка из чисел, разделённых "."
		case PROFILE_TYPE_STRING:// произвольная строка
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			strncpy( (char*)GetValue, (char*)Value, PROFILE_STRING_MAX_LEN);
			((char*)GetValue)[ PROFILE_STRING_MAX_LEN - 1 ] = 0;
			osMutexRelease( Profile->Values.Mutex );
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_SetFactoryValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx)
{
	void *DefaultValue;
	
	__PROFILE_GetDefaultValue(Profile, Table, Idx, &DefaultValue);
	
	return PROFILE_SetValue(Profile, Table, Idx, DefaultValue);	
}

void PROFILE_SetAlarmColors(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t SetDegradeColor, uint8_t SetFailureColor)
{
	uint8_t *DegradeColor, *FailureColor;
	
	__PROFILE_GetDegradeColor(Profile, Table, Idx, &DegradeColor);
	__PROFILE_GetFailureColor(Profile, Table, Idx, &FailureColor);
	
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if ((DegradeColor) && (SetDegradeColor != PROFILE_COLOR_NA) && (SetDegradeColor <= PROFILE_COLOR_CRITICAL))
		*DegradeColor = SetDegradeColor;
	if ((FailureColor) && (SetFailureColor != PROFILE_COLOR_NA) && (SetFailureColor <= PROFILE_COLOR_CRITICAL))
		*FailureColor = SetFailureColor;
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_GetAlarmColors(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t *GetDegradeColor, uint8_t *GetFailureColor)
{
	uint8_t *DegradeColor, *FailureColor;
	
	__PROFILE_GetDegradeColor(Profile, Table, Idx, &DegradeColor);
	__PROFILE_GetFailureColor(Profile, Table, Idx, &FailureColor);
	
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if ((GetDegradeColor) && (DegradeColor))
		*GetDegradeColor = *DegradeColor;
	else if ((GetDegradeColor) && (!DegradeColor))
		*GetDegradeColor = PROFILE_COLOR_NA;
	if ((GetFailureColor) && (FailureColor))
		*GetFailureColor = *FailureColor;
	else if ((GetFailureColor) && (!FailureColor))
		*GetFailureColor = PROFILE_COLOR_NA;
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_SetFactoryAlarmColors(PROFILE_T *Profile, uint8_t Table, uint16_t Idx)
{
	uint8_t *DefaultDegradeColor, *DefaultFailureColor;
		
	__PROFILE_GetDefaultDegradeColor(Profile, Table, Idx, &DefaultDegradeColor);
	__PROFILE_GetDefaultFailureColor(Profile, Table, Idx, &DefaultFailureColor);
	
	PROFILE_SetAlarmColors(Profile, Table, Idx, DefaultDegradeColor ? *DefaultDegradeColor : PROFILE_COLOR_NA, DefaultFailureColor ? *DefaultFailureColor : PROFILE_COLOR_NA);
}

void PROFILE_SetFactory(PROFILE_T *Profile, uint8_t Table)
{
	uint32_t Idx;
	
	if (Table == PROFILE_TABLE_1)
	{
		for (Idx = 1; Idx <= Profile->Descr[Profile->Idx]->Table1Size; Idx++)
		{
			/* Do not set parameters with write access for superuser only */
			if (((Profile->Descr[Profile->Idx]->Table1[Idx].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table1[Idx].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				continue;
			PROFILE_SetFactoryValue(Profile, Table, Idx);
		}
	}
	else if (Table == PROFILE_TABLE_2)
	{
		for (Idx = 1; Idx <= Profile->Descr[Profile->Idx]->Table2Size; Idx++)
		{
			PROFILE_SetFactoryAlarmColors(Profile, Table, Idx);
		}
	}
	else if (Table == PROFILE_TABLE_3)
	{
		for (Idx = 1; Idx <= Profile->Descr[Profile->Idx]->Table3Size; Idx++)
		{
			/* Do not set parameters with write access for superuser only */
			if (((Profile->Descr[Profile->Idx]->Table3[Idx].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table3[Idx].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				continue;
			PROFILE_SetFactoryValue(Profile, Table, Idx);
		}
	}
	else if (Table == PROFILE_TABLE_4)
	{
		for (Idx = 1; Idx <= Profile->Descr[Profile->Idx]->Table4Size; Idx++)
		{
			/* Do not set parameters with write access for superuser only */
			if (((Profile->Descr[Profile->Idx]->Table4[Idx].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table4[Idx].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				continue;
			PROFILE_SetFactoryValue(Profile, Table, Idx);
		}
	}
	else if (Table == PROFILE_TABLE_103)
	{
		for (Idx = 1; Idx <= Profile->Descr[Profile->Idx]->Table103Size; Idx++)
		{
			/* Do not set parameters with write access for superuser only */
			if (((Profile->Descr[Profile->Idx]->Table103[Idx].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Profile->Descr[Profile->Idx]->Table103[Idx].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				continue;
			PROFILE_SetFactoryValue(Profile, Table, Idx);
		}
	}
}

void PROFILE_SetDefault(PROFILE_T *Profile)
{
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	memcpy(Profile->Values.Static, Profile->Descr[Profile->Idx]->DefaultValues, Profile->Values.StaticSize);
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_GetColor(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t *GetColor)
{
	uint8_t *Color;
	
	__PROFILE_GetColor(Profile, Table, Idx, &Color);
	
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if (Color)
		*GetColor = *Color;
	else
		*GetColor = PROFILE_COLOR_NA;
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_SetColor(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t SetColor)
{
	uint8_t *Color;
	
	__PROFILE_GetColor(Profile, Table, Idx, &Color);
	
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if (Color)
		*Color = SetColor;
	osMutexRelease( Profile->Values.Mutex );;
}

void PROFILE_SetColorToThresholds(PROFILE_T *Profile, uint8_t Table, uint16_t Idx)
{
	void *ParamValue, *ParamValueThr;
	uint8_t Type;
	uint8_t *Color;
	uint8_t *DegradeColor;
	uint8_t *FailureColor;
	
	__PROFILE_GetType(Profile, Table, Idx, &Type);
	__PROFILE_GetValue(Profile, Table, Idx, &ParamValue);
	__PROFILE_GetThr(Profile, Table, Idx, &ParamValueThr);
	__PROFILE_GetColor(Profile, Table, Idx, &Color);
	__PROFILE_GetDegradeColor(Profile, Table, Idx, &DegradeColor);
	__PROFILE_GetFailureColor(Profile, Table, Idx, &FailureColor);
	
	if ((!ParamValue) || (!ParamValueThr) || (!Color) || (!DegradeColor) || (!FailureColor))
		return;
	
	switch( Type )
	{
		case PROFILE_TYPE_BOOL:
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_HEX32:
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if ((*(uint32_t*)ParamValue > ((uint32_t*)ParamValueThr)[PROFILE_THR_CMAX]) ||\
				(*(uint32_t*)ParamValue < ((uint32_t*)ParamValueThr)[PROFILE_THR_CMIN]))
				*Color = *FailureColor;
			else if ((*(uint32_t*)ParamValue > ((uint32_t*)ParamValueThr)[PROFILE_THR_WMAX]) ||\
				(*(uint32_t*)ParamValue < ((uint32_t*)ParamValueThr)[PROFILE_THR_WMIN]))
				*Color = *DegradeColor;
			else
				*Color = PROFILE_COLOR_NORMAL;	
			osMutexRelease( Profile->Values.Mutex );
			break;
		case PROFILE_TYPE_INT32:
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if ((*(int32_t*)ParamValue > ((int32_t*)ParamValueThr)[PROFILE_THR_CMAX]) ||\
				(*(int32_t*)ParamValue < ((int32_t*)ParamValueThr)[PROFILE_THR_CMIN]))
				*Color = *FailureColor;
			else if ((*(int32_t*)ParamValue > ((int32_t*)ParamValueThr)[PROFILE_THR_WMAX]) ||\
				(*(int32_t*)ParamValue < ((int32_t*)ParamValueThr)[PROFILE_THR_WMIN]))
				*Color = *DegradeColor;
			else
				*Color = PROFILE_COLOR_NORMAL;		
			osMutexRelease( Profile->Values.Mutex );
			break;
		case PROFILE_TYPE_FLOAT_1:
		case PROFILE_TYPE_FLOAT_2:
		case PROFILE_TYPE_FLOAT_3:
		case PROFILE_TYPE_FLOAT_E:
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if ((*(float*)ParamValue > ((float*)ParamValueThr)[PROFILE_THR_CMAX]) ||\
				(*(float*)ParamValue < ((float*)ParamValueThr)[PROFILE_THR_CMIN]))
				*Color = *FailureColor;
			else if ((*(float*)ParamValue > ((float*)ParamValueThr)[PROFILE_THR_WMAX]) ||\
				(*(float*)ParamValue < ((float*)ParamValueThr)[PROFILE_THR_WMIN]))
				*Color = *DegradeColor;
			else
				*Color = PROFILE_COLOR_NORMAL;		
			osMutexRelease( Profile->Values.Mutex );
			break;
		default:
			return;
	}
}

void PROFILE_SetColorToDegrade(PROFILE_T *Profile, uint8_t Table, uint16_t Idx)
{
	uint8_t *Color;
	uint8_t *DegradeColor;
	
	__PROFILE_GetColor(Profile, Table, Idx, &Color);
	__PROFILE_GetDegradeColor(Profile, Table, Idx, &DegradeColor);
	
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if ((Color) && (DegradeColor))
		*Color = *DegradeColor;
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_SetColorToFailure(PROFILE_T *Profile, uint8_t Table, uint16_t Idx)
{
	uint8_t *Color;
	uint8_t *FailureColor;
	
	__PROFILE_GetColor(Profile, Table, Idx, &Color);
	__PROFILE_GetFailureColor(Profile, Table, Idx, &FailureColor);
	
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if ((Color) && (FailureColor))
		*Color = *FailureColor;
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_GetState(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t *GetState)
{
	uint8_t *State;
	
	__PROFILE_GetState(Profile, Table, Idx, &State);
	
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if (State)
		*GetState = *State;
	else
		*GetState = PROFILE_STATE_IS;
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_SetState(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t SetState)
{
	uint8_t *State;
	
	__PROFILE_GetState(Profile, Table, Idx, &State);
	
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if (State)
	{
		*State = SetState;
		Profile->TablesChanges |= 1 << Table;
	}
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_UpdateParam(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint8_t ColorType, uint8_t State)
{
	void *ParamValue, *ParamThr;
	uint8_t ParamType;
	uint8_t *ParamState;
	uint8_t *ParamColor;
	uint8_t *ParamDegradeColor;
	uint8_t *ParamFailureColor;
	
	__PROFILE_GetType(Profile, Table, Idx, &ParamType);
	__PROFILE_GetValue(Profile, Table, Idx, &ParamValue);
	__PROFILE_GetState(Profile, Table, Idx, &ParamState);
	__PROFILE_GetThr(Profile, Table, Idx, &ParamThr);
	__PROFILE_GetColor(Profile, Table, Idx, &ParamColor);
	__PROFILE_GetDegradeColor(Profile, Table, Idx, &ParamDegradeColor);
	__PROFILE_GetFailureColor(Profile, Table, Idx, &ParamFailureColor);
	
	if (!ParamValue)
		return;
	
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	switch( ParamType )
	{
		case PROFILE_TYPE_BOOL:
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_HEX32:
		case PROFILE_TYPE_ENUM:
		case PROFILE_TYPE_CNT:
		case PROFILE_TYPE_BITMSK:
			if (ParamState)
				*ParamState = State;
			if (ParamValue)
			{
				memcpy(ParamValue, Value, 4);
				if ((!ParamState) || (*ParamState != PROFILE_STATE_OOS))
				{
					Profile->TablesChanges |= 1 << Table;
				}
			}
			if ((ColorType == PROFILE_COLOR_TYPE_AUTO) && (ParamThr) && (ParamColor) && (ParamDegradeColor) && (ParamFailureColor))
			{
				if ((*(uint32_t*)ParamValue > ((uint32_t*)ParamThr)[PROFILE_THR_CMAX]) ||\
					(*(uint32_t*)ParamValue < ((uint32_t*)ParamThr)[PROFILE_THR_CMIN]))
					*ParamColor = *ParamFailureColor;
				else if ((*(uint32_t*)ParamValue > ((uint32_t*)ParamThr)[PROFILE_THR_WMAX]) ||\
					(*(uint32_t*)ParamValue < ((uint32_t*)ParamThr)[PROFILE_THR_WMIN]))
					*ParamColor = *ParamDegradeColor;
				else
					*ParamColor = PROFILE_COLOR_NORMAL;	
			}
			else if ((ColorType == PROFILE_COLOR_TYPE_DEGRADE) && (ParamColor) && (ParamDegradeColor))
			{
				*ParamColor = *ParamDegradeColor;
			}
			else if ((ColorType == PROFILE_COLOR_TYPE_FAILURE) && (ParamColor) && (ParamFailureColor))
			{
				*ParamColor = *ParamFailureColor;
			}
			else if ((ColorType == PROFILE_COLOR_TYPE_NORMAL) && (ParamColor))
			{
				*ParamColor = PROFILE_COLOR_NORMAL;	
			}
			break;
		case PROFILE_TYPE_INT32:
			if (ParamState)
				*ParamState = State;
			if (ParamValue)
			{
				memcpy(ParamValue, Value, 4);
				if ((!ParamState) || (*ParamState != PROFILE_STATE_OOS))
				{
					Profile->TablesChanges |= 1 << Table;
				}
			}
			if ((ColorType == PROFILE_COLOR_TYPE_AUTO) && (ParamThr) && (ParamColor) && (ParamDegradeColor) && (ParamFailureColor))
			{
				if ((*(int32_t*)ParamValue > ((int32_t*)ParamThr)[PROFILE_THR_CMAX]) ||\
					(*(int32_t*)ParamValue < ((int32_t*)ParamThr)[PROFILE_THR_CMIN]))
					*ParamColor = *ParamFailureColor;
				else if ((*(int32_t*)ParamValue > ((int32_t*)ParamThr)[PROFILE_THR_WMAX]) ||\
					(*(int32_t*)ParamValue < ((int32_t*)ParamThr)[PROFILE_THR_WMIN]))
					*ParamColor = *ParamDegradeColor;
				else
					*ParamColor = PROFILE_COLOR_NORMAL;		
			}
			else if ((ColorType == PROFILE_COLOR_TYPE_DEGRADE) && (ParamColor) && (ParamDegradeColor))
			{
				*ParamColor = *ParamDegradeColor;
			}
			else if ((ColorType == PROFILE_COLOR_TYPE_FAILURE) && (ParamColor) && (ParamFailureColor))
			{
				*ParamColor = *ParamFailureColor;
			}
			else if ((ColorType == PROFILE_COLOR_TYPE_NORMAL) && (ParamColor))
			{
				*ParamColor = PROFILE_COLOR_NORMAL;	
			}
			break;
		case PROFILE_TYPE_FLOAT_1:
		case PROFILE_TYPE_FLOAT_2:
		case PROFILE_TYPE_FLOAT_3:
		case PROFILE_TYPE_FLOAT_E:
			if (ParamState)
				*ParamState = State;
			if (ParamValue)
			{
				memcpy(ParamValue, Value, 4);
				if ((!ParamState) || (*ParamState != PROFILE_STATE_OOS))
				{
					Profile->TablesChanges |= 1 << Table;
				}
			}
			if ((ColorType == PROFILE_COLOR_TYPE_AUTO) && (ParamThr) && (ParamColor) && (ParamDegradeColor) && (ParamFailureColor))
			{
				if ((*(float*)ParamValue > ((float*)ParamThr)[PROFILE_THR_CMAX]) ||\
					(*(float*)ParamValue < ((float*)ParamThr)[PROFILE_THR_CMIN]))
					*ParamColor = *ParamFailureColor;
				else if ((*(float*)ParamValue > ((float*)ParamThr)[PROFILE_THR_WMAX]) ||\
					(*(float*)ParamValue < ((float*)ParamThr)[PROFILE_THR_WMIN]))
					*ParamColor = *ParamDegradeColor;
				else
					*ParamColor = PROFILE_COLOR_NORMAL;
			}
			else if ((ColorType == PROFILE_COLOR_TYPE_DEGRADE) && (ParamColor) && (ParamDegradeColor))
			{
				*ParamColor = *ParamDegradeColor;
			}
			else if ((ColorType == PROFILE_COLOR_TYPE_FAILURE) && (ParamColor) && (ParamFailureColor))
			{
				*ParamColor = *ParamFailureColor;
			}
			else if ((ColorType == PROFILE_COLOR_TYPE_NORMAL) && (ParamColor))
			{
				*ParamColor = PROFILE_COLOR_NORMAL;	
			}
			break;
		case PROFILE_TYPE_OID:
		case PROFILE_TYPE_STRING:
			if (ParamState)
				*ParamState = State;
			if (ParamValue)
			{
				strncpy( (char*)ParamValue, (char*)Value, PROFILE_STRING_MAX_LEN);
				((char*)ParamValue)[ PROFILE_STRING_MAX_LEN - 1 ] = 0;
				if ((!ParamState) || (*ParamState != PROFILE_STATE_OOS))
				{
					Profile->TablesChanges |= 1 << Table;
				}
			}
			if ((ColorType == PROFILE_COLOR_TYPE_DEGRADE) && (ParamColor) && (ParamDegradeColor))
			{
				*ParamColor = *ParamDegradeColor;
			}
			else if ((ColorType == PROFILE_COLOR_TYPE_FAILURE) && (ParamColor) && (ParamFailureColor))
			{
				*ParamColor = *ParamFailureColor;
			}
			else if ((ColorType == PROFILE_COLOR_TYPE_NORMAL) && (ParamColor))
			{
				*ParamColor = PROFILE_COLOR_NORMAL;	
			}
			break;
		default:
			break;
	}
	osMutexRelease( Profile->Values.Mutex );
	
}
