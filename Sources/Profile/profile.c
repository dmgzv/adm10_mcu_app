#include "profile.h"
#include "profile_flash.h"
#include "string_fnc.h"
#include "crc.h"
#include <string.h>
#include "cmsis_os.h"
#include "bootloader.h"

#define PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, Param) 			(void*)((uint32_t)((Profile)->Values.Dynamic) + (uint32_t)((Param)->Value))
#define PROFILE_STATIC_PARAM_VALUE_PTR(Profile, Param) 				(void*)((uint32_t)((Profile)->Values.Static) + (uint32_t)((Param)->Value))
#define PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, Param) (void*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->DefaultValues) + (uint32_t)((Param)->Value))
#define PROFILE_DYNAMIC_PARAM_THR_PTR(Profile, Param) 				(void*)((uint32_t)((Profile)->Values.Static) + (uint32_t)((Param)->Thesholds))
#define PROFILE_STATIC_PARAM_THR_PTR(Profile, Param) 					(void*)((uint32_t)((Profile)->Values.Static) + (uint32_t)((Param)->Thesholds))
#define PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) 			(PROFILE_COLOR_T*)((uint32_t)((Profile)->Values.Dynamic) + (uint32_t)((Param)->Color))
#define PROFILE_DYNAMIC_PARAM_DEGR_COLOR_PTR(Profile, Param) 	(PROFILE_COLOR_T*)((uint32_t)((Profile)->Values.Static) + (uint32_t)((Param)->DegradeColor))
#define PROFILE_DYNAMIC_PARAM_DEFAULT_DEGR_COLOR_PTR(Profile, Param) 	(PROFILE_COLOR_T*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->DefaultValues) + (uint32_t)((Param)->DegradeColor))
#define PROFILE_DYNAMIC_PARAM_FAIL_COLOR_PTR(Profile, Param) 	(PROFILE_COLOR_T*)((uint32_t)((Profile)->Values.Static) + (uint32_t)((Param)->FailureColor))
#define PROFILE_DYNAMIC_PARAM_DEFAULT_FAIL_COLOR_PTR(Profile, Param) 	(PROFILE_COLOR_T*)((uint32_t)((Profile)->Descr[(Profile)->Idx]->DefaultValues) + (uint32_t)((Param)->FailureColor))
#define PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, Param) 			(uint8_t*)((uint32_t)((Profile)->Values.Dynamic) + (uint32_t)((Param)->State))

__weak int32_t PROFILE_CheckConfigValue(PROFILE_T *Profile, PROFILE_PARAM_T	*Param, void *Value)
{
	return 0;
}

__weak int32_t PROFILE_SetConfigValueDependencies(PROFILE_T *Profile, PROFILE_PARAM_T	*Param, void *Value)
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

int32_t PROFILE_Init(PROFILE_T *Profile, PROFILE_DESCR_T **Descr, void *Static, uint32_t StaticSize, void *Dynamic, uint32_t DynamicSize, BSP_FLASH_T *FlashInstance)
{	
	Profile->Idx = 0;
	Profile->Descr = Descr;
	Profile->TablesChanges = 0;
	Profile->SetEnable = 0;
	if (PROFILE_InitValues(&Profile->Values, Static, StaticSize, Dynamic, DynamicSize))
		return -1;
	
	BOOTLOADER_Init(&Profile->Bootloader, FlashInstance);
	if( PROFILE_FLASH_Init(&Profile->ProfileFlash, "Profile Flash", FlashInstance, Profile->Values.Static, Profile->Values.StaticSize, Profile->Values.Mutex))
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


int32_t PROFILE_ValueToString(PROFILE_TYPE_T Type, void *Value, char* Str , uint32_t *Len)
{
	char *p = Str;

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

int32_t PROFILE_StringToValue(PROFILE_TYPE_T Type, void *Value, char* Str)
{

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

int32_t PROFILE_CheckThresholdValue(PROFILE_T *Profile, PROFILE_PARAM_T * Param, void *Value)
{
	uint32_t i;
	void *ParamValue, *ParamValueThr;
	
	ParamValue = PROFILE_STATIC_PARAM_VALUE_PTR(Profile, Param);
	ParamValueThr = PROFILE_STATIC_PARAM_THR_PTR(Profile, Param);
	
	for( i = 0; i < 6; i++)
	{
		switch (Param->Type)
		{
			case PROFILE_TYPE_BOOL:
			case PROFILE_TYPE_UINT32:
			case PROFILE_TYPE_CNT:
			case PROFILE_TYPE_BITMSK:
			case PROFILE_TYPE_HEX32:
				if((uint32_t*)ParamValue == &((uint32_t*)ParamValueThr)[i])
				{
					if ((i == PROFILE_THR_CMIN) && (*(uint32_t*)Value > ((uint32_t*)ParamValueThr)[i + 1]))
						return -1;
					else if ((i == PROFILE_THR_CMAX) && (*(uint32_t*)Value < ((uint32_t*)ParamValueThr)[i - 1]))
						return -1;
					else if (((i == PROFILE_THR_WMAX) || (i == PROFILE_THR_WMIN)) && ((*(uint32_t*)Value > ((uint32_t*)ParamValueThr)[i + 1]) || (*(uint32_t*)Value < ((uint32_t*)ParamValueThr)[i - 1])))
						return -1;			
					return 0;
				}
				break;
			case PROFILE_TYPE_ENUM:
			case PROFILE_TYPE_INT32:
				if((int32_t*)ParamValue == &((int32_t*)ParamValueThr)[i])
				{
					if ((i == PROFILE_THR_CMIN) && (*(int32_t*)Value > ((int32_t*)ParamValueThr)[i + 1]))
						return -1;
					else if ((i == PROFILE_THR_CMAX) && (*(int32_t*)Value < ((int32_t*)ParamValueThr)[i - 1]))
						return -1;
					else if (((i == PROFILE_THR_WMAX) || (i == PROFILE_THR_WMIN)) && ((*(int32_t*)Value > ((int32_t*)ParamValueThr)[i + 1]) || (*(int32_t*)Value < ((int32_t*)ParamValueThr)[i - 1])))
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
					if ((i == PROFILE_THR_CMIN) && (*(float*)Value > ((float*)ParamValueThr)[i + 1]))
						return -1;
					else if ((i == PROFILE_THR_CMAX) && (*(float*)Value < ((float*)ParamValueThr)[i - 1]))
						return -1;
					else if (((i == PROFILE_THR_WMAX) || (i == PROFILE_THR_WMIN)) && ((*(float*)Value > ((float*)ParamValueThr)[i + 1]) || (*(float*)Value < ((float*)ParamValueThr)[i - 1])))
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

int32_t PROFILE_CheckValue(PROFILE_T *Profile, PROFILE_PARAM_T * Param, void *Value)
{
	if (Param->Table == PROFILE_TABLE_4)
		return PROFILE_CheckThresholdValue(Profile, Param, Value);
	if (Param->Table == PROFILE_TABLE_3)
		return PROFILE_CheckConfigValue(Profile, Param, Value);
	return 0;
}

int32_t PROFILE_SetValueDependencies(PROFILE_T *Profile, PROFILE_PARAM_T * Param, void *Value)
{
	if (Param->Table == PROFILE_TABLE_3)
		return PROFILE_SetConfigValueDependencies(Profile, Param, Value);
	return 0;
}

int32_t PROFILE_SetValue(PROFILE_T *Profile, PROFILE_PARAM_T * Param, void *Value)
{
	void *ParamValue;
	
	if ((Param->Table == PROFILE_TABLE_2) || (Param->Table == PROFILE_TABLE_5)  || (Param->Table == PROFILE_TABLE_8))
		ParamValue = PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, Param);
	else
		ParamValue = PROFILE_STATIC_PARAM_VALUE_PTR(Profile, Param);
	
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
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if (memcmp(ParamValue, Value, 4))
			{
				memcpy(ParamValue, Value, 4);
				if ((!Param->State) || (*PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, Param) != PROFILE_STATE_OOS))
					Profile->TablesChanges |= 1 << Param->Table;
			}
			osMutexRelease( Profile->Values.Mutex );
			break;
		case PROFILE_TYPE_OID:// строка из чисел, разделённых "."
		case PROFILE_TYPE_STRING:// произвольная строка
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if (strncmp((char*)ParamValue, (char*)Value, PROFILE_STRING_MAX_LEN))
			{
				strncpy( (char*)ParamValue, (char*)Value, PROFILE_STRING_MAX_LEN);
				((char*)ParamValue)[ PROFILE_STRING_MAX_LEN - 1 ] = 0;
				if ((!Param->State) || (*PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, Param) != PROFILE_STATE_OOS))
					Profile->TablesChanges |= 1 << Param->Table;
			}
			osMutexRelease( Profile->Values.Mutex );
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_GetValue(PROFILE_T *Profile, PROFILE_PARAM_T * Param, void *Value)
{
	void *ParamValue;
	
	if ((Param->Table == PROFILE_TABLE_2) || (Param->Table == PROFILE_TABLE_5)  || (Param->Table == PROFILE_TABLE_8))
		ParamValue = PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, Param);
	else
		ParamValue = PROFILE_STATIC_PARAM_VALUE_PTR(Profile, Param);
	
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
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			memcpy(Value, ParamValue, 4);
			osMutexRelease( Profile->Values.Mutex );
			break;
		case PROFILE_TYPE_OID:// строка из чисел, разделённых "."
		case PROFILE_TYPE_STRING:// произвольная строка
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			strncpy( (char*)Value, (char*)ParamValue, PROFILE_STRING_MAX_LEN);
			((char*)Value)[ PROFILE_STRING_MAX_LEN - 1 ] = 0;
			osMutexRelease( Profile->Values.Mutex );
			break;
		default:
			return -1;
	}
	return 0;
}

int32_t PROFILE_SetFactoryValue(PROFILE_T *Profile, PROFILE_PARAM_T * Param)
{
	void *ParamValue;
	void *ParamDefaultValue;
	
	if ((Param->Table == PROFILE_TABLE_2) || (Param->Table == PROFILE_TABLE_5)  || (Param->Table == PROFILE_TABLE_8))
		return -1;
	
	ParamValue = PROFILE_STATIC_PARAM_VALUE_PTR(Profile, Param);
	ParamDefaultValue = PROFILE_STATIC_PARAM_DEFAULT_VALUE_PTR(Profile, Param);
	
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
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if (memcmp(ParamValue, ParamDefaultValue, 4))
			{
				memcpy(ParamValue, ParamDefaultValue, 4);
				if ((!Param->State) || (*PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, Param) != PROFILE_STATE_OOS))
					Profile->TablesChanges |= 1 << Param->Table;
			}
			osMutexRelease( Profile->Values.Mutex );
			break;
		case PROFILE_TYPE_OID:// строка из чисел, разделённых "."
		case PROFILE_TYPE_STRING:// произвольная строка
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if (strncmp((char*)ParamValue, (char*)ParamDefaultValue, PROFILE_STRING_MAX_LEN))
			{
				strncpy( (char*)ParamValue, (char*)ParamDefaultValue, PROFILE_STRING_MAX_LEN);
				((char*)ParamValue)[ PROFILE_STRING_MAX_LEN - 1 ] = 0;
				if ((!Param->State) || (*PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, Param) != PROFILE_STATE_OOS))
					Profile->TablesChanges |= 1 << Param->Table;
			}
			osMutexRelease( Profile->Values.Mutex );
			break;
		default:
			return -1;
	}
	return 0;
}

void PROFILE_SetAlarmColors(PROFILE_T *Profile, PROFILE_PARAM_T * Param, PROFILE_COLOR_T DegradeColor, PROFILE_COLOR_T FailureColor)
{
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if (Param->DegradeColor && DegradeColor != PROFILE_COLOR_NA && DegradeColor <= PROFILE_COLOR_CRITICAL)
		*PROFILE_DYNAMIC_PARAM_DEGR_COLOR_PTR(Profile, Param) = DegradeColor;
	if (Param->FailureColor && FailureColor != PROFILE_COLOR_NA && FailureColor <= PROFILE_COLOR_CRITICAL)
		*PROFILE_DYNAMIC_PARAM_FAIL_COLOR_PTR(Profile, Param) = FailureColor;
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_GetAlarmColors(PROFILE_T *Profile, PROFILE_PARAM_T * Param, PROFILE_COLOR_T *DegradeColor, PROFILE_COLOR_T *FailureColor)
{
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if (Param->DegradeColor)
		*DegradeColor = *PROFILE_DYNAMIC_PARAM_DEGR_COLOR_PTR(Profile, Param);
	else
		*DegradeColor = PROFILE_COLOR_NA;
	if (Param->FailureColor)
		*FailureColor = *PROFILE_DYNAMIC_PARAM_FAIL_COLOR_PTR(Profile, Param);
	else
		*FailureColor = PROFILE_COLOR_NA;
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_SetFactoryAlarmColors(PROFILE_T *Profile, PROFILE_PARAM_T * Param)
{
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if (Param->DegradeColor)
		*PROFILE_DYNAMIC_PARAM_DEGR_COLOR_PTR(Profile, Param) = *PROFILE_DYNAMIC_PARAM_DEFAULT_DEGR_COLOR_PTR(Profile, Param);
	if (Param->FailureColor)
		*PROFILE_DYNAMIC_PARAM_FAIL_COLOR_PTR(Profile, Param) = *PROFILE_DYNAMIC_PARAM_DEFAULT_FAIL_COLOR_PTR(Profile, Param);
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_SetFactory(PROFILE_T *Profile)
{
	PROFILE_PARAM_T * Param;
	uint32_t Table, Idx;
	
	for (Table = 0; Table < PROFILE_TABLES_CNT; Table++)
	{
		if ((Table == PROFILE_TABLE_2) || (Table == PROFILE_TABLE_5)  || (Table == PROFILE_TABLE_8))
			continue;
		Param = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[Table];
		for (Idx = 0; Idx < Profile->Descr[Profile->Idx]->TablesSize[Table]; Idx++)
		{
			/* Do not set parameters with write access for superuser only */
			if (((Param[Idx].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((Param[Idx].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				continue;
			PROFILE_SetFactoryValue(Profile, &Param[Idx]);
		}
	}
	
	Param = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	for (Idx = 0; Idx < Profile->Descr[Profile->Idx]->TablesSize[PROFILE_TABLE_2]; Idx++)
	{
		PROFILE_SetFactoryAlarmColors(Profile, &Param[Idx]);
	}
}

void PROFILE_SetDefault(PROFILE_T *Profile)
{
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	memcpy(Profile->Values.Static, Profile->Descr[Profile->Idx]->DefaultValues, Profile->Values.StaticSize);
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_GetColor(PROFILE_T *Profile, PROFILE_PARAM_T *Param, PROFILE_COLOR_T *Color)
{
	if (!Param->Color)
	{
		*Color = PROFILE_COLOR_NA;
		return;
	}
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	*Color = *PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param);
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_SetColor(PROFILE_T *Profile, PROFILE_PARAM_T * Param, PROFILE_COLOR_T Color)
{
	if (!Param->Color)
		return;
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = Color;
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_SetColorToThresholds(PROFILE_T *Profile, PROFILE_PARAM_T * Param)
{
	void *ParamValue, *ParamValueThr;
	
	if (!Param->Color || !Param->DegradeColor  || !Param->FailureColor  || !Param->Thesholds)
		return;
	
	ParamValue = PROFILE_DYNAMIC_PARAM_VALUE_PTR(Profile, Param);
	ParamValueThr = PROFILE_DYNAMIC_PARAM_THR_PTR(Profile, Param);
	
	switch( Param->Type )
	{
		case PROFILE_TYPE_BOOL:
		case PROFILE_TYPE_UINT32:
		case PROFILE_TYPE_HEX32:
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if ((*(uint32_t*)ParamValue > ((uint32_t*)ParamValueThr)[PROFILE_THR_CMAX]) ||\
				(*(uint32_t*)ParamValue < ((uint32_t*)ParamValueThr)[PROFILE_THR_CMIN]))
				*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = *PROFILE_DYNAMIC_PARAM_FAIL_COLOR_PTR(Profile, Param);
			else if ((*(uint32_t*)ParamValue > ((uint32_t*)ParamValueThr)[PROFILE_THR_WMAX]) ||\
				(*(uint32_t*)ParamValue < ((uint32_t*)ParamValueThr)[PROFILE_THR_WMIN]))
				*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = *PROFILE_DYNAMIC_PARAM_DEGR_COLOR_PTR(Profile, Param);
			else
				*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = PROFILE_COLOR_NORMAL;	
			osMutexRelease( Profile->Values.Mutex );
			break;
		case PROFILE_TYPE_INT32:
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if ((*(int32_t*)ParamValue > ((int32_t*)ParamValueThr)[PROFILE_THR_CMAX]) ||\
				(*(int32_t*)ParamValue < ((int32_t*)ParamValueThr)[PROFILE_THR_CMIN]))
				*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = *PROFILE_DYNAMIC_PARAM_FAIL_COLOR_PTR(Profile, Param);
			else if ((*(int32_t*)ParamValue > ((int32_t*)ParamValueThr)[PROFILE_THR_WMAX]) ||\
				(*(int32_t*)ParamValue < ((int32_t*)ParamValueThr)[PROFILE_THR_WMIN]))
				*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = *PROFILE_DYNAMIC_PARAM_DEGR_COLOR_PTR(Profile, Param);
			else
				*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = PROFILE_COLOR_NORMAL;		
			osMutexRelease( Profile->Values.Mutex );
			break;
		case PROFILE_TYPE_FLOAT_1:
		case PROFILE_TYPE_FLOAT_2:
		case PROFILE_TYPE_FLOAT_3:
		case PROFILE_TYPE_FLOAT_E:
			osMutexWait( Profile->Values.Mutex, osWaitForever );
			if ((*(float*)ParamValue > ((float*)ParamValueThr)[PROFILE_THR_CMAX]) ||\
				(*(float*)ParamValue < ((float*)ParamValueThr)[PROFILE_THR_CMIN]))
				*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = *PROFILE_DYNAMIC_PARAM_FAIL_COLOR_PTR(Profile, Param);
			else if ((*(float*)ParamValue > ((float*)ParamValueThr)[PROFILE_THR_WMAX]) ||\
				(*(float*)ParamValue < ((float*)ParamValueThr)[PROFILE_THR_WMIN]))
				*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = *PROFILE_DYNAMIC_PARAM_DEGR_COLOR_PTR(Profile, Param);
			else
				*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = PROFILE_COLOR_NORMAL;		
			osMutexRelease( Profile->Values.Mutex );
			break;
		default:
			return;
	}
}

void PROFILE_SetColorToDegrade(PROFILE_T *Profile, PROFILE_PARAM_T * Param)
{
	if (!Param->Color || !Param->DegradeColor)
		return;
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = *PROFILE_DYNAMIC_PARAM_DEGR_COLOR_PTR(Profile, Param);
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_SetColorToFailure(PROFILE_T *Profile, PROFILE_PARAM_T * Param)
{
	if (!Param->Color || !Param->FailureColor)
			return;
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	*PROFILE_DYNAMIC_PARAM_COLOR_PTR(Profile, Param) = *PROFILE_DYNAMIC_PARAM_FAIL_COLOR_PTR(Profile, Param);
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_GetState(PROFILE_T *Profile, PROFILE_PARAM_T * Param, uint8_t *State)
{
	if (!Param->State)
	{
		*State = PROFILE_STATE_IS;
		return;
	}
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	*State = *PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, Param);
	osMutexRelease( Profile->Values.Mutex );
}

void PROFILE_SetState(PROFILE_T *Profile, PROFILE_PARAM_T *Param, uint8_t State)
{
	if (!Param->State)
		return;
	osMutexWait( Profile->Values.Mutex, osWaitForever );
	if (*PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, Param) != State)
	{
		*PROFILE_DYNAMIC_PARAM_STATE_PTR(Profile, Param) = State;
		Profile->TablesChanges |= 1 << Param->Table;
	}
	osMutexRelease( Profile->Values.Mutex );
}
