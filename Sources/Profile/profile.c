#include "profile.h"
#include "profile_flash.h"
#include "string_fnc.h"
#include "crc.h"
#include <string.h>
#include "cmsis_os.h"
#include "bootloader.h"

#define PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, Param) 							(void*)((uint32_t)((Profile)->Values->Dynamic) + (uint32_t)((Param)->Value))
#define PROFILE_STATIC_PARAM_VALUE_PTR(Profile, Param) 								(void*)((uint32_t)((Profile)->Values->Static) + (uint32_t)((Param)->Value))
#define PROFILE_CALIB_PARAM_VALUE_PTR(Profile, Param) 								(void*)((uint32_t)((Profile)->Values->Calib) + (uint32_t)((Param)->Value))
#define PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, Param)				(void*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->StaticDefaultValues) + (uint32_t)((Param)->Value))
#define PROFILE_CALIB_PARAM_DEFAULT_VALUE_PTR(Profile, Param)					(void*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->CalibDefaultValues) + (uint32_t)((Param)->Value))
#define PROFILE_PARAM_THR_PTR(Profile, Param) 												(Param)->Thesholds ? (void*)((uint32_t)((Profile)->Values->Static) + (uint32_t)((Param)->Thesholds)) : NULL
#define PROFILE_PARAM_BITMSK_PTR(Profile, Param) 											(void*)((Param)->Thesholds)
#define PROFILE_PARAM_ENUM_PTR(Profile, Param) 												(void*)((Param)->Thesholds)
#define PROFILE_PARAM_COLOR_PTR(Profile, Param) 											(Param)->Color ? (uint8_t*)((uint32_t)((Profile)->Values->Dynamic) + (uint32_t)((Param)->Color)) : NULL
#define PROFILE_PARAM_DEGR_COLOR_PTR(Profile, Param) 									(Param)->DegradeColor ? (uint8_t*)((uint32_t)((Profile)->Values->Static) + (uint32_t)((Param)->DegradeColor)) : NULL
#define PROFILE_PARAM_DEFAULT_DEGR_COLOR_PTR(Profile, Param) 					(Param)->DegradeColor ? (uint8_t*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->StaticDefaultValues) + (uint32_t)((Param)->DegradeColor)) : NULL
#define PROFILE_PARAM_FAIL_COLOR_PTR(Profile, Param) 									(Param)->FailureColor ? (uint8_t*)((uint32_t)((Profile)->Values->Static) + (uint32_t)((Param)->FailureColor)) : NULL
#define PROFILE_PARAM_DEFAULT_FAIL_COLOR_PTR(Profile, Param) 					(Param)->FailureColor ? (uint8_t*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->StaticDefaultValues) + (uint32_t)((Param)->FailureColor)) : NULL
#define PROFILE_PARAM_STATE_PTR(Profile, Param) 											(Param)->State ? (uint8_t*)((uint32_t)((Profile)->Values->Dynamic) + (uint32_t)((Param)->State)) : NULL

static uint8_t __PROFILE_GetLogAccess(PROFILE_TABLE_PARAM_T *Param)
{
	if (Param->Access & PROFILE_ACCESS(PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE))
		return LOG_ACCESS_MONITOR;
	if (Param->Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE))
		return LOG_ACCESS_USER;
	if (Param->Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE))
		return LOG_ACCESS_ADMIN;
	return LOG_ACCESS_SU;
}

__weak void __PROFILE_GetStringColor(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t *Color, uint8_t DegradeColor, uint8_t FailureColor)
{
	*Color = PROFILE_COLOR_NA;
}

#ifndef __PROFILE_GetStringColor

#endif

__weak int32_t PROFILE_CheckConfigValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, uint32_t Restore)
{
	return 0;
}

__weak int32_t PROFILE_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, uint32_t Restore)
{
	return 0;
}

int32_t PROFILE_GetTable(PROFILE_T *Profile, uint8_t TableIdx, PROFILE_TABLE_T **Table)
{
	uint32_t i;
	
	for (i = 0; i < Profile->Descr[Profile->Idx]->TablesCnt; i++)
	{
		if (Profile->Descr[Profile->Idx]->Tables[i]->Index == TableIdx)
		{
			*Table = (PROFILE_TABLE_T *)Profile->Descr[Profile->Idx]->Tables[i];
			return 0;
		}
	}
	
	return -1;
}

int32_t PROFILE_GetParam(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, PROFILE_TABLE_PARAM_T **Param)
{
	uint32_t i;
	
	for (i = 0; i < Profile->Descr[Profile->Idx]->TablesCnt; i++)
	{
		if (Profile->Descr[Profile->Idx]->Tables[i]->Index == TableIdx)
		{
			*Param = (PROFILE_TABLE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[i]->Params[ParamIdx - 1];
			return 0;
		}
	}
	return -1;
}

static void PROFILE_InitStates(PROFILE_T *Profile)
{
	uint32_t i, j;
	
	for (i = 0; i < Profile->Descr[Profile->Idx]->TablesCnt; i++)
	{
		for (j = 0; j < Profile->Descr[Profile->Idx]->Tables[i]->ParamsCnt; j++)
		{
			PROFILE_SetState(Profile, Profile->Descr[Profile->Idx]->Tables[i]->Index, Profile->Descr[Profile->Idx]->Tables[i]->Params[j]->Index, PROFILE_STATE_IS);
		}
	}
}

static int32_t PROFILE_InitValues(PROFILE_T *Profile)
{
	osMutexDef(ProfileValues);
	Profile->Values->Mutex = osMutexCreate(osMutex(ProfileValues));
	if ( !Profile->Values->Mutex )
		return -1;
	
	PROFILE_InitStates(Profile);
	PROFILE_SetDefault(Profile);
	
	return 0;
}

int32_t PROFILE_GetParamValue(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void **Value)
{
	switch (Param->Table)
	{
		case PROFILE_TABLE_1:
		case PROFILE_TABLE_3:
		case PROFILE_TABLE_4:
		case PROFILE_TABLE_6:
		case PROFILE_TABLE_7:
		case PROFILE_TABLE_103:
		case PROFILE_TABLE_104:
			if (PROFILE_IS_CALIB_PARAM(Param))
				*Value = PROFILE_CALIB_PARAM_VALUE_PTR(Profile, Param);
			else
				*Value = PROFILE_STATIC_PARAM_VALUE_PTR(Profile, Param);
			break;
		case PROFILE_TABLE_2:
		case PROFILE_TABLE_5:
		case PROFILE_TABLE_8:
			*Value = PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, Param);
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_SetParamValue(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void *NewValue)
{
	void *Value;
	uint8_t State;
	uint8_t Color, NewColor;
	
	if (Profile->ValuesLock)
		return -1;
	
	if (PROFILE_GetParamValue(Profile, Param, &Value))
		return -1;
	if (PROFILE_GetParamState(Profile, Param, &State))
		return -1;
	if (PROFILE_GetParamColor(Profile, Param, Value, &Color))
		return -1;
	if (PROFILE_GetParamColor(Profile, Param, NewValue, &NewColor))
		return -1;
	
	switch( Param->Type )
	{
		case PROFILE_TYPE_BOOL:
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_CNT:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, NewValue, 4))
			{
				memcpy(Value, NewValue, 4);
				if (Param->Table == PROFILE_TABLE_2)
				{
					if (NewColor != Color)
					{
						__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%u", Param->Name, *(uint32_t*)Value);
					}
				}
				else if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%u", Param->Name, *(uint32_t*)Value);
				}
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_HEX32:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, NewValue, 4))
			{
				memcpy(Value, NewValue, 4);
				if (Param->Table == PROFILE_TABLE_2)
				{
					if (NewColor != Color)
					{
						__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%x", Param->Name, *(uint32_t*)Value);
					}
				}
				else if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%x", Param->Name, *(uint32_t*)Value);
				}
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_INT32:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, NewValue, 4))
			{
				memcpy(Value, NewValue, 4);
				if (Param->Table == PROFILE_TABLE_2)
				{
					if (NewColor != Color)
					{
						__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%d", Param->Name, *(int32_t*)Value);
					}
				}
				else if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%d", Param->Name, *(int32_t*)Value);
				}
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_FLOAT_1:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, NewValue, 4))
			{
				memcpy(Value, NewValue, 4);
				if (Param->Table == PROFILE_TABLE_2)
				{
					if (NewColor != Color)
					{
						__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%.1f", Param->Name, *(float*)Value);
					}
				}
				else if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%.1f", Param->Name, *(float*)Value);
				}
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_FLOAT_2:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, NewValue, 4))
			{
				memcpy(Value, NewValue, 4);
				if (Param->Table == PROFILE_TABLE_2)
				{
					if (NewColor != Color)
					{
						__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%.2f", Param->Name, *(float*)Value);
					}
				}
				else if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%.2f", Param->Name, *(float*)Value);
				}
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_FLOAT_3:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, NewValue, 4))
			{
				memcpy(Value, NewValue, 4);
				if (Param->Table == PROFILE_TABLE_2)
				{
					if (NewColor != Color)
					{
						__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%.3f", Param->Name, *(float*)Value);
					}
				}
				else if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%.3f", Param->Name, *(float*)Value);
				}
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_FLOAT_E:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, NewValue, 4))
			{
				memcpy(Value, NewValue, 4);
				if (Param->Table == PROFILE_TABLE_2)
				{
					if (NewColor != Color)
					{
						__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%.3e", Param->Name, *(float*)Value);
					}
				}
				else if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%.3e", Param->Name, *(float*)Value);
				}
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_BITMSK:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, NewValue, 4))
			{
				memcpy(Value, NewValue, 4);
				if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%u", Param->Name, *(uint32_t*)Value);
				}
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_IP:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, NewValue, 4))
			{
				memcpy(Value, NewValue, 4);
				if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%u", Param->Name, *(uint32_t*)Value);
				}
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_ENUM:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, NewValue, 4))
			{
				memcpy(Value, NewValue, 4);
				if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%d", Param->Name, *(int32_t*)Value);
				}
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_UINT64:
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (memcmp(Value, NewValue, sizeof(uint64_t)))
			{
				memcpy(Value, NewValue, sizeof(uint64_t));
				if (Param->Table == PROFILE_TABLE_2)
				{
					if (NewColor != Color)
					{
						__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%ull", Param->Name, *(uint64_t*)Value);
					}
				}
				else if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%ull", Param->Name, *(uint64_t*)Value);
				}
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_OID:// строка из чисел, разделённых "."
		case PROFILE_TYPE_STRING:// произвольная строка
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if (strncmp((char*)Value, (char*)NewValue, PROFILE_STRING_MAX_LEN))
			{
				strncpy( (char*)Value, (char*)NewValue, PROFILE_STRING_MAX_LEN);
				if (Param->Table == PROFILE_TABLE_2)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%s", Param->Name, (char*)Value);
				}
				else if (Param->Table != PROFILE_TABLE_8)
				{
					__LOG_Add("PROFILE", __PROFILE_GetLogAccess(Param), NewColor, "", "ParamValueUpdate", "%s;%s", Param->Name, (char*)Value);
				}
				((char*)Value)[ PROFILE_STRING_MAX_LEN - 1 ] = 0;
				if (State != PROFILE_STATE_OOS)
				{
					Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_GetParamDefaultValue(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void **DefaultValue)
{	
	switch (Param->Table)
	{
		case PROFILE_TABLE_1:
		case PROFILE_TABLE_3:
		case PROFILE_TABLE_4:
		case PROFILE_TABLE_6:
		case PROFILE_TABLE_7:
		case PROFILE_TABLE_103:
		case PROFILE_TABLE_104:
			if (PROFILE_IS_CALIB_PARAM(Param))
				*DefaultValue = PROFILE_CALIB_PARAM_DEFAULT_VALUE_PTR(Profile, Param);
			else
				*DefaultValue = PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, Param);
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_SetParamDefaultValue(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void **DefaultValue)
{
	switch (Param->Table)
	{
		case PROFILE_TABLE_1:
		case PROFILE_TABLE_3:
		case PROFILE_TABLE_4:
		case PROFILE_TABLE_6:
		case PROFILE_TABLE_7:
		case PROFILE_TABLE_103:
		case PROFILE_TABLE_104:
			if (PROFILE_IS_CALIB_PARAM(Param))
				*DefaultValue = PROFILE_CALIB_PARAM_DEFAULT_VALUE_PTR(Profile, Param);
			else
				*DefaultValue = PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, Param);
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_ParamValueToString(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void *Value, char* Str , uint32_t *Len)
{
	char *p = Str;

	if( !Str || !Value || !Len ) 
		return -1;
	
	switch( Param->Type )
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

int32_t PROFILE_StringToParamValue(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void *Value, char* Str)
{
	if( !Str || !Value ) 
		return -1;
	
	switch( Param->Type )
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

int32_t PROFILE_GetParamThrValues(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void **Values)
{
	switch (Param->Table)
	{
		case PROFILE_TABLE_1:
			if (Param->Type == PROFILE_TYPE_ENUM)
				*Values = PROFILE_PARAM_ENUM_PTR(Profile, (PROFILE_TABLE1_PARAM_T*)Param);
			else if (Param->Type == PROFILE_TYPE_BITMSK)
				*Values = PROFILE_PARAM_BITMSK_PTR(Profile, (PROFILE_TABLE1_PARAM_T*)Param);
			else
				*Values = PROFILE_PARAM_THR_PTR(Profile, (PROFILE_TABLE1_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_2:
			if (Param->Type == PROFILE_TYPE_ENUM)
				*Values = PROFILE_PARAM_ENUM_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			else if (Param->Type == PROFILE_TYPE_BITMSK)
				*Values = PROFILE_PARAM_BITMSK_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			else
				*Values = PROFILE_PARAM_THR_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_3:
			if (Param->Type == PROFILE_TYPE_ENUM)
				*Values = PROFILE_PARAM_ENUM_PTR(Profile, (PROFILE_TABLE3_PARAM_T*)Param);
			else if (Param->Type == PROFILE_TYPE_BITMSK)
				*Values = PROFILE_PARAM_BITMSK_PTR(Profile, (PROFILE_TABLE3_PARAM_T*)Param);
			else
				*Values = PROFILE_PARAM_THR_PTR(Profile, (PROFILE_TABLE3_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_4:
			*Values = PROFILE_PARAM_THR_PTR(Profile, (PROFILE_TABLE4_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_104:
			if (Param->Type == PROFILE_TYPE_ENUM)
				*Values = PROFILE_PARAM_ENUM_PTR(Profile, (PROFILE_TABLE104_PARAM_T*)Param);
			else if (Param->Type == PROFILE_TYPE_BITMSK)
				*Values = PROFILE_PARAM_BITMSK_PTR(Profile, (PROFILE_TABLE104_PARAM_T*)Param);
			else
				*Values = PROFILE_PARAM_THR_PTR(Profile, (PROFILE_TABLE104_PARAM_T*)Param);
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_GetParamState(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, uint8_t *State)
{
	uint8_t *StatePtr;
	
	switch (Param->Table)
	{
		case PROFILE_TABLE_1:
			StatePtr = PROFILE_PARAM_STATE_PTR(Profile, (PROFILE_TABLE1_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_2:
			StatePtr = PROFILE_PARAM_STATE_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_3:
			StatePtr = PROFILE_PARAM_STATE_PTR(Profile, (PROFILE_TABLE3_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_4:
			StatePtr = PROFILE_PARAM_STATE_PTR(Profile, (PROFILE_TABLE4_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_5:
			StatePtr = NULL;
			break;
		case PROFILE_TABLE_6:
			StatePtr = NULL;
			break;
		case PROFILE_TABLE_7:
			StatePtr = NULL;
			break;
		case PROFILE_TABLE_8:
			StatePtr = PROFILE_PARAM_STATE_PTR(Profile, (PROFILE_TABLE8_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_103:
			StatePtr = NULL;
			break;
		case PROFILE_TABLE_104:
			StatePtr = NULL;
			break;
		default:
			return -1;
	}
	
	if (StatePtr) 
	{
		osMutexWait( Profile->Values->Mutex, osWaitForever );
		*State = *StatePtr;
		osMutexRelease( Profile->Values->Mutex );
	}
	else
	{
		*State = PROFILE_STATE_IS;
	}
	
	return 0;
}

int32_t PROFILE_SetParamState(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, uint8_t State)
{
	uint8_t *StatePtr;
	void *ParamThrValues;
	
	switch (Param->Table)
	{
		case PROFILE_TABLE_1:
			StatePtr = PROFILE_PARAM_STATE_PTR(Profile, (PROFILE_TABLE1_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_2:
			StatePtr = PROFILE_PARAM_STATE_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_3:
			StatePtr = PROFILE_PARAM_STATE_PTR(Profile, (PROFILE_TABLE3_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_4:
			StatePtr = PROFILE_PARAM_STATE_PTR(Profile, (PROFILE_TABLE4_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_5:
			StatePtr = NULL;
			break;
		case PROFILE_TABLE_6:
			StatePtr = NULL;
			break;
		case PROFILE_TABLE_7:
			StatePtr = NULL;
			break;
		case PROFILE_TABLE_8:
			StatePtr = PROFILE_PARAM_STATE_PTR(Profile, (PROFILE_TABLE8_PARAM_T*)Param);
			break;
		case PROFILE_TABLE_103:
			StatePtr = NULL;
			break;
		case PROFILE_TABLE_104:
			StatePtr = NULL;
			break;
		default:
			return -1;
	}
	
	if (StatePtr) 
	{		
		osMutexWait( Profile->Values->Mutex, osWaitForever );
		if (*StatePtr != State)
		{
			*StatePtr = State;
			Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Param->Table);
			if (!PROFILE_GetParamThrValues(Profile, Param, &ParamThrValues))
			{
				Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(PROFILE_TABLE_4);
			}
		}
		osMutexRelease( Profile->Values->Mutex );
	}
	else
	{
		return -1;
	}
	
	return 0;
}

int32_t PROFILE_GetParamAlarmColors(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, uint8_t *Degrade, uint8_t *Failure)
{
	uint8_t *DegradePtr;
	uint8_t *FailurePtr;
	
	switch (Param->Table)
	{
		case PROFILE_TABLE_2:
			DegradePtr = PROFILE_PARAM_DEGR_COLOR_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			FailurePtr = PROFILE_PARAM_FAIL_COLOR_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			break;
		default:
			return -1;
	}
	if ((DegradePtr) && (FailurePtr))
	{
		osMutexWait( Profile->Values->Mutex, osWaitForever );
		*Degrade = *DegradePtr;
		*Failure = *FailurePtr;
		osMutexRelease( Profile->Values->Mutex );
	}
	else
	{
		*Degrade = PROFILE_COLOR_NA;
		*Failure = PROFILE_COLOR_NA;
	}
	return 0;
}

int32_t PROFILE_SetParamAlarmColors(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, uint8_t Degrade, uint8_t Failure)
{
	uint8_t *DegradePtr;
	uint8_t *FailurePtr;
	switch (Param->Table)
	{
		case PROFILE_TABLE_2:
			DegradePtr = PROFILE_PARAM_DEGR_COLOR_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			FailurePtr = PROFILE_PARAM_FAIL_COLOR_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			break;
		default:
			return -1;
	}
	if ((DegradePtr) && (FailurePtr))
	{
		osMutexWait( Profile->Values->Mutex, osWaitForever );
		*DegradePtr = Degrade;
		*FailurePtr = Failure;
		osMutexRelease( Profile->Values->Mutex );
	}
	else
	{
		return -1;
	}
	return 0;
}

int32_t PROFILE_GetParamDefaultAlarmColors(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, uint8_t *Degrade, uint8_t *Failure)
{
	uint8_t *DefaultDegradePtr;
	uint8_t *DefaultFailurePtr;
	switch (Param->Table)
	{
		case PROFILE_TABLE_2:
			DefaultDegradePtr = PROFILE_PARAM_DEFAULT_DEGR_COLOR_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			DefaultFailurePtr = PROFILE_PARAM_DEFAULT_FAIL_COLOR_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			break;
		default:
			return -1;
	}
	
	if ((DefaultDegradePtr) && (DefaultFailurePtr))
	{
		osMutexWait( Profile->Values->Mutex, osWaitForever );
		*Degrade = *DefaultDegradePtr;
		*Failure = *DefaultFailurePtr;
		osMutexRelease( Profile->Values->Mutex );
	}
	else
	{
		return -1;
	}
	return 0;
}

int32_t PROFILE_SetParamDefaultAlarmColors(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param)
{
	uint8_t *DegradePtr, *DefaultDegradePtr;
	uint8_t *FailurePtr, *DefaultFailurePtr;
	switch (Param->Table)
	{
		case PROFILE_TABLE_2:
			DegradePtr = PROFILE_PARAM_DEGR_COLOR_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			FailurePtr = PROFILE_PARAM_FAIL_COLOR_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			DefaultDegradePtr = PROFILE_PARAM_DEFAULT_DEGR_COLOR_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			DefaultFailurePtr = PROFILE_PARAM_DEFAULT_FAIL_COLOR_PTR(Profile, (PROFILE_TABLE2_PARAM_T*)Param);
			break;
		default:
			return -1;
	}
	
	if ((DegradePtr) && (FailurePtr) && (DefaultDegradePtr) && (DefaultFailurePtr))
	{
		osMutexWait( Profile->Values->Mutex, osWaitForever );
		*DegradePtr = *DefaultDegradePtr;
		*FailurePtr = *DefaultFailurePtr;
		osMutexRelease( Profile->Values->Mutex );
	}
	else
	{
		return -1;
	}
	return 0;
}

int32_t PROFILE_GetParamColor(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void *Value, uint8_t *Color)
{
	uint32_t i;
	void *ThrValues;
	uint8_t DegradeColor, FailureColor;
	
	*Color = PROFILE_COLOR_NA;
	
	if (PROFILE_GetParamAlarmColors(Profile, Param, &DegradeColor, &FailureColor))
		return 0;

	switch( Param->Type )
	{
		case PROFILE_TYPE_BOOL:
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_HEX32:
			if (PROFILE_GetParamThrValues(Profile, Param, &ThrValues))
				return 0;
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if ((*(uint32_t*)Value > ((uint32_t*)ThrValues)[PROFILE_THR_CMAX]) ||\
				(*(uint32_t*)Value < ((uint32_t*)ThrValues)[PROFILE_THR_CMIN]))
				*Color = FailureColor;
			else if ((*(uint32_t*)Value > ((uint32_t*)ThrValues)[PROFILE_THR_WMAX]) ||\
				(*(uint32_t*)Value < ((uint32_t*)ThrValues)[PROFILE_THR_WMIN]))
				*Color = DegradeColor;
			else
				*Color = PROFILE_COLOR_NORMAL;
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_INT32:
			if (PROFILE_GetParamThrValues(Profile, Param, &ThrValues))
				return 0;
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if ((*(int32_t*)Value > ((int32_t*)ThrValues)[PROFILE_THR_CMAX]) ||\
				(*(int32_t*)Value < ((int32_t*)ThrValues)[PROFILE_THR_CMIN]))
				*Color = FailureColor;
			else if ((*(int32_t*)Value > ((int32_t*)ThrValues)[PROFILE_THR_WMAX]) ||\
				(*(int32_t*)Value < ((int32_t*)ThrValues)[PROFILE_THR_WMIN]))
				*Color = DegradeColor;
			else
				*Color = PROFILE_COLOR_NORMAL;
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_FLOAT_1:
		case PROFILE_TYPE_FLOAT_2:
		case PROFILE_TYPE_FLOAT_3:
		case PROFILE_TYPE_FLOAT_E:
			if (PROFILE_GetParamThrValues(Profile, Param, &ThrValues))
				return 0;
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if ((*(float*)Value > ((float*)ThrValues)[PROFILE_THR_CMAX]) ||\
				(*(float*)Value < ((float*)ThrValues)[PROFILE_THR_CMIN]))
				*Color = FailureColor;
			else if ((*(float*)Value > ((float*)ThrValues)[PROFILE_THR_WMAX]) ||\
				(*(float*)Value < ((float*)ThrValues)[PROFILE_THR_WMIN]))
				*Color = DegradeColor;
			else
				*Color = PROFILE_COLOR_NORMAL;
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_UINT64:
			if (PROFILE_GetParamThrValues(Profile, Param, &ThrValues))
				return 0;
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			if ((*(uint64_t*)Value > ((uint64_t*)ThrValues)[PROFILE_THR_CMAX]) ||\
				(*(uint64_t*)Value < ((uint64_t*)ThrValues)[PROFILE_THR_CMIN]))
				*Color = FailureColor;
			else if ((*(uint64_t*)Value > ((uint64_t*)ThrValues)[PROFILE_THR_WMAX]) ||\
				(*(uint64_t*)Value < ((uint64_t*)ThrValues)[PROFILE_THR_WMIN]))
				*Color = DegradeColor;
			else
				*Color = PROFILE_COLOR_NORMAL;
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_ENUM:
			if (PROFILE_GetParamThrValues(Profile, Param, &ThrValues))
				return 0;
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			*Color = PROFILE_COLOR_NORMAL;
			for ( i = 1; i <= ((PROFILE_ENUM_T*)ThrValues)[0].Value; i++)
			{
				if (*((uint32_t*)Value) == ((PROFILE_ENUM_T*)ThrValues)[i].Value)
				{
					if (((PROFILE_ENUM_T*)ThrValues)[i].Color == PROFILE_COLOR_TYPE_DEGRADE)
						*Color = DegradeColor;
					else if (((PROFILE_ENUM_T*)ThrValues)[i].Color == PROFILE_COLOR_TYPE_FAILURE)
						*Color = FailureColor;
					break;
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		case PROFILE_TYPE_BITMSK:
			if (PROFILE_GetParamThrValues(Profile, Param, &ThrValues))
				return 0;
			osMutexWait( Profile->Values->Mutex, osWaitForever );
			*Color = PROFILE_COLOR_NORMAL;
			for ( i = 1; i <= ((PROFILE_BITMSK_T*)ThrValues)[0].Value; i++)
			{
				if (*((uint32_t*)Value) & ((PROFILE_BITMSK_T*)ThrValues)[i].Value)
				{
					if (((PROFILE_ENUM_T*)ThrValues)[i].Color == PROFILE_COLOR_TYPE_DEGRADE)
					{
						if (*Color < DegradeColor)
							*Color = DegradeColor;
					}
					else if (((PROFILE_ENUM_T*)ThrValues)[i].Color == PROFILE_COLOR_TYPE_FAILURE)
					{
						if (*Color < FailureColor)
							*Color = FailureColor;
					}
						
				}
			}
			osMutexRelease( Profile->Values->Mutex );
			break;
		default:
			__PROFILE_GetStringColor(Profile, Param->Table, Param->Index, Color, DegradeColor, FailureColor);
			break;
	}
	return 0;
}

int32_t PROFILE_Init(PROFILE_T *Profile, PROFILE_DESCR_T **Descr, PROFILE_VALUES_T *Values)
{	
	Profile->Idx = 0;
	Profile->Descr = Descr;
	Profile->Values = Values;
	Profile->TablesChanges = 0;
	Profile->ValuesLock = 0;
	if (PROFILE_InitValues(Profile))
		return -1;
	return 0;
}

int32_t PROFILE_ValueToString(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, char* Str , uint32_t *Len)
{
	PROFILE_TABLE_PARAM_T *Param;
	char *p = Str;

	if( !Str || !Value || !Len ) 
		return -1;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	
	switch( Param->Type )
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

int32_t PROFILE_StringToValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, char* Str)
{
	PROFILE_TABLE_PARAM_T *Param;
	if( !Str || !Value ) 
		return -1;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	
	switch( Param->Type )
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
			if (strlen(Value) >= PROFILE_STRING_MAX_LEN)
				return -1;
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

int32_t PROFILE_CheckThresholdValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, uint32_t Restore)
{
	uint32_t i;
	PROFILE_TABLE_PARAM_T *Param;
	void *ParamValue, *ParamThrValues;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (PROFILE_GetParamValue(Profile, Param, &ParamValue))
		return -1;
	if (PROFILE_GetParamThrValues(Profile, Param, &ParamThrValues))
		return -1;
	
	for( i = 0; i < 4; i++)
	{
		switch (Param->Type)
		{
			case PROFILE_TYPE_UINT32:
			case PROFILE_TYPE_CNT:
			case PROFILE_TYPE_HEX32:
				if(((uint32_t*)ParamValue == &((uint32_t*)ParamThrValues)[i]) && (!Restore))
				{
					if ((i == PROFILE_THR_CMIN) && (*(uint32_t*)Value > ((uint32_t*)ParamThrValues)[PROFILE_THR_WMIN]))
						return -1;
					else if ((i == PROFILE_THR_CMAX) && (*(uint32_t*)Value < ((uint32_t*)ParamThrValues)[PROFILE_THR_WMAX]))
						return -1;
					else if ((i == PROFILE_THR_WMAX) && ((*(uint32_t*)Value > ((uint32_t*)ParamThrValues)[PROFILE_THR_CMAX]) || (*(uint32_t*)Value < ((uint32_t*)ParamThrValues)[PROFILE_THR_WMIN])))
						return -1;
					else if ((i == PROFILE_THR_WMIN) && ((*(uint32_t*)Value > ((uint32_t*)ParamThrValues)[PROFILE_THR_WMAX]) || (*(uint32_t*)Value < ((uint32_t*)ParamThrValues)[PROFILE_THR_CMIN])))
						return -1;
					return 0;
				}
				break;
			case PROFILE_TYPE_INT32:
				if(((int32_t*)ParamValue == &((int32_t*)ParamThrValues)[i]) && (!Restore))
				{
					if ((i == PROFILE_THR_CMIN) && (*(int32_t*)Value > ((int32_t*)ParamThrValues)[PROFILE_THR_WMIN]))
						return -1;
					else if ((i == PROFILE_THR_CMAX) && (*(int32_t*)Value < ((int32_t*)ParamThrValues)[PROFILE_THR_WMAX]))
						return -1;
					else if ((i == PROFILE_THR_WMAX) && ((*(int32_t*)Value > ((int32_t*)ParamThrValues)[PROFILE_THR_CMAX]) || (*(int32_t*)Value < ((int32_t*)ParamThrValues)[PROFILE_THR_WMIN])))
						return -1;
					else if ((i == PROFILE_THR_WMIN) && ((*(int32_t*)Value > ((int32_t*)ParamThrValues)[PROFILE_THR_WMAX]) || (*(int32_t*)Value < ((int32_t*)ParamThrValues)[PROFILE_THR_CMIN])))
						return -1;
					return 0;
				}
				break;
			case PROFILE_TYPE_FLOAT_1:
			case PROFILE_TYPE_FLOAT_2:
			case PROFILE_TYPE_FLOAT_3:
			case PROFILE_TYPE_FLOAT_E:
				if(((float*)ParamValue == &((float*)ParamThrValues)[i]) && (!Restore))
				{
					if ((i == PROFILE_THR_CMIN) && (*(float*)Value > ((float*)ParamThrValues)[PROFILE_THR_WMIN]))
						return -1;
					else if ((i == PROFILE_THR_CMAX) && (*(float*)Value < ((float*)ParamThrValues)[PROFILE_THR_WMAX]))
						return -1;
					else if ((i == PROFILE_THR_WMAX) && ((*(float*)Value > ((float*)ParamThrValues)[PROFILE_THR_CMAX]) || (*(float*)Value < ((float*)ParamThrValues)[PROFILE_THR_WMIN])))
						return -1;
					else if ((i == PROFILE_THR_WMIN) && ((*(float*)Value > ((float*)ParamThrValues)[PROFILE_THR_WMAX]) || (*(float*)Value < ((float*)ParamThrValues)[PROFILE_THR_CMIN])))
						return -1;
					return 0;
				}
				break;
			case PROFILE_TYPE_UINT64:
				if(((uint64_t*)ParamValue == &((uint64_t*)ParamThrValues)[i]) && (!Restore))
				{
					if ((i == PROFILE_THR_CMIN) && (*(uint64_t*)Value > ((uint64_t*)ParamThrValues)[PROFILE_THR_WMIN]))
						return -1;
					else if ((i == PROFILE_THR_CMAX) && (*(uint64_t*)Value < ((uint64_t*)ParamThrValues)[PROFILE_THR_WMAX]))
						return -1;
					else if ((i == PROFILE_THR_WMAX) && ((*(uint64_t*)Value > ((uint64_t*)ParamThrValues)[PROFILE_THR_CMAX]) || (*(uint64_t*)Value < ((uint64_t*)ParamThrValues)[PROFILE_THR_WMIN])))
						return -1;
					else if ((i == PROFILE_THR_WMIN) && ((*(uint64_t*)Value > ((uint64_t*)ParamThrValues)[PROFILE_THR_WMAX]) || (*(uint64_t*)Value < ((uint64_t*)ParamThrValues)[PROFILE_THR_CMIN])))
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

int32_t PROFILE_CheckConfigValueRange(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, uint32_t Restore)
{
	uint32_t i;
	PROFILE_TABLE_PARAM_T *Param;
	void *ParamThrValues;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (PROFILE_GetParamThrValues(Profile, Param, &ParamThrValues))
		return -1;
	
	switch (Param->Type)
	{
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_CNT:
		case PROFILE_TYPE_HEX32:
			if((ParamThrValues) && (!Restore))
			{
				if ((*(uint32_t*)Value) < ((uint32_t*)ParamThrValues)[PROFILE_THR_HMIN])
					return -1;
				else if ((*(uint32_t*)Value) > ((uint32_t*)ParamThrValues)[PROFILE_THR_HMAX])
					return -1;
			}
			break;
		case PROFILE_TYPE_ENUM:
			if(ParamThrValues)
			{
				for (i = 1; i <= ((PROFILE_ENUM_T*)ParamThrValues)[0].Value; i++)
				{
					if (*(uint32_t*)Value == ((PROFILE_ENUM_T*)ParamThrValues)[i].Value)
						return 0;
				}
				return -1;
			}
			break;
		case PROFILE_TYPE_INT32:
			if((ParamThrValues) && (!Restore))
			{
				if ((*(int32_t*)Value) < ((int32_t*)ParamThrValues)[PROFILE_THR_HMIN])
					return -1;
				else if ((*(int32_t*)Value) > ((int32_t*)ParamThrValues)[PROFILE_THR_HMAX])
					return -1;
			}
			break;
		case PROFILE_TYPE_FLOAT_1:
		case PROFILE_TYPE_FLOAT_2:
		case PROFILE_TYPE_FLOAT_3:
		case PROFILE_TYPE_FLOAT_E:
			if((ParamThrValues) && (!Restore))
			{
				if ((*(float*)Value) < ((float*)ParamThrValues)[PROFILE_THR_HMIN])
					return -1;
				else if ((*(float*)Value) > ((float*)ParamThrValues)[PROFILE_THR_HMAX])
					return -1;
			}
			break;
		case PROFILE_TYPE_UINT64:
			if((ParamThrValues) && (!Restore))
			{
				if ((*(uint64_t*)Value) < ((uint64_t*)ParamThrValues)[PROFILE_THR_HMIN])
					return -1;
				else if ((*(uint64_t*)Value) > ((uint64_t*)ParamThrValues)[PROFILE_THR_HMAX])
					return -1;
			}
			break;
		case PROFILE_TYPE_STRING:
			for (i = 0; i < strlen((char*)Value); i++)
			{
				if (((char*)Value)[i] == ';')
					return -1;
			}
			break;
	}
	return 0;
}

int32_t PROFILE_CheckValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, uint32_t Restore)
{
	
	if (TableIdx == PROFILE_TABLE_1)
	{
		if (PROFILE_CheckConfigValueRange(Profile, TableIdx, ParamIdx, Value, Restore))
			return -1;
	}
	else if (TableIdx == PROFILE_TABLE_3)
	{
		if (PROFILE_CheckConfigValueRange(Profile, TableIdx, ParamIdx, Value, Restore))
			return -1;
		if (PROFILE_CheckConfigValue(Profile, TableIdx, ParamIdx, Value, Restore))
			return -1;
	}
	else if (TableIdx == PROFILE_TABLE_4)
	{
		if (PROFILE_CheckThresholdValue(Profile, TableIdx, ParamIdx, Value, Restore))
			return -1;
	}
	else if (TableIdx == PROFILE_TABLE_104)
	{
		if (PROFILE_CheckConfigValueRange(Profile, TableIdx, ParamIdx, Value, Restore))
			return -1;
		if (PROFILE_CheckConfigValue(Profile, TableIdx, ParamIdx, Value, Restore))
			return -1;
	}
	return 0;
}

int32_t PROFILE_CheckWriteAccess(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t Access)
{
	PROFILE_TABLE_PARAM_T *Param;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (
		(
			(Access == PROFILE_ACCESS(PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE))
			&& ((Param->Access & PROFILE_ACCESS(PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE))
		)
		|| (
			(Access == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE))
			&& ((Param->Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE))
		)
		|| (
			(Access == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE))
			&& ((Param->Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE))
		)
		|| (
			(Access == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE))
			&& ((Param->Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE))
		)
	)
	{
		return 0;
	}
	return -1;
}

int32_t PROFILE_SetValueDependencies(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, uint32_t Restore)
{
	if (TableIdx == PROFILE_TABLE_3)
		return PROFILE_SetConfigValueDependencies(Profile, TableIdx, ParamIdx, Value, Restore);
	return 0;
}

int32_t PROFILE_GetValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *GetValue)
{
	PROFILE_TABLE_PARAM_T *Param;
	void *Value;
	
	if (Profile->ValuesLock)
		return -1;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (PROFILE_GetParamValue(Profile, Param, &Value))
		return -1;
	
	switch( Param->Type )
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

int32_t PROFILE_SetValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value)
{
	PROFILE_TABLE_PARAM_T *Param;
	
	if (Profile->ValuesLock)
		return -1;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	
	return PROFILE_SetParamValue(Profile, Param, Value);	
}

int32_t PROFILE_SetFactoryValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx)
{
	PROFILE_TABLE_PARAM_T *Param;
	void *DefaultValue;
	
	if (Profile->ValuesLock)
		return -1;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (PROFILE_GetParamDefaultValue(Profile, Param, &DefaultValue))
		return -1;
	
	if (PROFILE_IS_CALIB_PARAM(Param))
		return -1;
	
	return PROFILE_SetParamValue(Profile, Param, DefaultValue);	
}

int32_t PROFILE_SetAlarmColors(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t SetDegradeColor, uint8_t SetFailureColor)
{
	PROFILE_TABLE_PARAM_T *Param;
	
	if (Profile->ValuesLock)
		return-1;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (PROFILE_SetParamAlarmColors(Profile, Param, SetDegradeColor, SetFailureColor))
		return -1;
	return 0;
}

int32_t PROFILE_GetAlarmColors(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t *GetDegradeColor, uint8_t *GetFailureColor)
{
	PROFILE_TABLE_PARAM_T *Param;
	
	if (Profile->ValuesLock)
		return -1;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (PROFILE_GetParamAlarmColors(Profile, Param, GetDegradeColor, GetFailureColor))
		return -1;
	return 0;
}

int32_t PROFILE_SetFactoryAlarmColors(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx)
{
	PROFILE_TABLE_PARAM_T *Param;
	
	if (Profile->ValuesLock)
		return -1;
		
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (PROFILE_SetParamDefaultAlarmColors(Profile, Param))
		return -1;
	return 0;
}

int32_t PROFILE_SetFactoryTable(PROFILE_T *Profile, uint8_t TableIdx)
{
	uint32_t ParamIdx;
	PROFILE_TABLE_T *Table;
	
	if (Profile->ValuesLock)
		return -1;
	
	if (PROFILE_GetTable(Profile, TableIdx, &Table))
		return -1;
	
	if ((TableIdx == PROFILE_TABLE_1) || (TableIdx == PROFILE_TABLE_3) || (TableIdx == PROFILE_TABLE_4) || (TableIdx == PROFILE_TABLE_103) || (TableIdx == PROFILE_TABLE_104))
	{
		for (ParamIdx = 1; ParamIdx <= Table->ParamsCnt; ParamIdx++)
		{
			PROFILE_SetFactoryValue(Profile, TableIdx, ParamIdx);
		}
	}
	else if (TableIdx == PROFILE_TABLE_2)
	{
		for (ParamIdx = 1; ParamIdx <= Table->ParamsCnt; ParamIdx++)
		{
			PROFILE_SetFactoryAlarmColors(Profile, TableIdx, ParamIdx);
		}
	}
	return 0;
}

int32_t PROFILE_SetDefault(PROFILE_T *Profile)
{
	if (Profile->ValuesLock)
		return - 1;
	
	osMutexWait( Profile->Values->Mutex, osWaitForever );
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

int32_t PROFILE_GetColor(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t *Color)
{
	PROFILE_TABLE_PARAM_T *Param;
	void *Value;
	
	if (Profile->ValuesLock)
		return -1;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (PROFILE_GetParamValue(Profile, Param, &Value))
		return -1;
	if (PROFILE_GetParamColor(Profile, Param, Value, Color))
		return -1;
	return 0;
}

int32_t PROFILE_GetState(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t *GetState)
{
	PROFILE_TABLE_PARAM_T *Param;
	
	if (Profile->ValuesLock)
		return -1;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (PROFILE_GetParamState(Profile, Param, GetState))
		return -1;
	return 0;
}

int32_t PROFILE_SetState(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t SetState)
{
	PROFILE_TABLE_PARAM_T *Param;
	
	if (Profile->ValuesLock)
		return -1;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (PROFILE_SetParamState(Profile, Param, SetState))
		return -1;
	return 0;
}

int32_t PROFILE_UpdateParam(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, uint8_t State)
{
	PROFILE_TABLE_PARAM_T *Param;
	
	if (Profile->ValuesLock)
		return -1;
	
	if (PROFILE_GetParam(Profile, TableIdx, ParamIdx, &Param))
		return -1;
	if (PROFILE_SetParamValue(Profile, Param, Value))
		return -1;
	if (PROFILE_SetParamState(Profile, Param, State))
		return -1;
	return 0;
}
