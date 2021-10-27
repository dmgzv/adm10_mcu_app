#ifndef __PROFILE_TYPES_H
#define __PROFILE_TYPES_H

#include <stdint.h>
#include <stdlib.h>
#include "cmsis_os.h"

#define PROFILE_MUTEX_T	osMutexId
#define PROFILE_STRING_MAX_LEN	48

#define PROFILE_TYPE_BOOL 		0
#define PROFILE_TYPE_INT32 		1
#define PROFILE_TYPE_UINT32 	2
#define PROFILE_TYPE_FLOAT_1 	3
#define PROFILE_TYPE_FLOAT_2 	4
#define PROFILE_TYPE_FLOAT_3 	5
#define PROFILE_TYPE_FLOAT_E 	6
#define PROFILE_TYPE_HEX32 		7
#define PROFILE_TYPE_STRING 	8
#define PROFILE_TYPE_ENUM 		9
#define PROFILE_TYPE_OID 			10
#define PROFILE_TYPE_IP 			11
#define PROFILE_TYPE_CNT 			12
#define PROFILE_TYPE_UINT64 	13
#define PROFILE_TYPE_BITMSK 	14

#define PROFILE_COLOR_NA 				0 //- параметр не может иметь соответствующего типа аварии. Цвет для данного параметра не определен.
#define PROFILE_COLOR_NO 				1 //- Нет цвета в текущем состоянии
#define PROFILE_COLOR_INFO 			2 //- Белый	  (Цвет для параметров ENUM)
#define PROFILE_COLOR_NORMAL 		3 //- Зелёный
#define PROFILE_COLOR_WARNING 	4 //- Голубой
#define PROFILE_COLOR_MINOR 		5 //- Жёлтый
#define PROFILE_COLOR_MAJOR 		6 //- Оранжевый
#define PROFILE_COLOR_CRITICAL 	7 //- Красный

#define PROFILE_COLOR_TYPE_NORMAL	0
#define PROFILE_COLOR_TYPE_DEGRADE	1
#define PROFILE_COLOR_TYPE_FAILURE	2

#define	PROFILE_STATE_OOS 0
#define	PROFILE_STATE_MT	1
#define	PROFILE_STATE_IS	2

#define PROFILE_THR_HMAX				0//	Верхний физический предел
#define PROFILE_THR_HMIN				1//	Нижний физический предел

#define PROFILE_THR_CMAX				0//	Верхний критический предел
#define PROFILE_THR_WMAX				1//	Верхний порог (предупреждение)
#define PROFILE_THR_WMIN				2// Нижний порог (предупреждение)
#define PROFILE_THR_CMIN				3// Нижнее критический предел

#define PROFILE_ACCESS_NONE				(0)
#define PROFILE_ACCESS_READ				(1 << 0)
#define PROFILE_ACCESS_WRITE			(3 << 0)

#define PROFILE_ACCESS(MN, US, AD, SU)	(((MN) << 0) | ((US) << 2) | ((AD) << 4) | ((SU) << 6))

#define PROFILE_TABLE_PARAM	\
	const char *Name;\
	const uint8_t Table;\
	const uint16_t Index;\
	const uint8_t Access;\
	const uint8_t	Type;\
	const void *Value
	
typedef struct {
	PROFILE_TABLE_PARAM;
} PROFILE_TABLE_PARAM_T;

typedef struct {
	PROFILE_TABLE_PARAM;
	const void *Thesholds;
	const uint8_t *State;
} PROFILE_TABLE1_PARAM_T;

typedef struct {
	PROFILE_TABLE_PARAM;
	const uint8_t *DegradeColor;//Цвет предупреждения
	const uint8_t *FailureColor;//Цвет критического значения
	const void *Thesholds;
	const uint8_t *State;
} PROFILE_TABLE2_PARAM_T;

typedef struct {
	PROFILE_TABLE_PARAM;
	const void *Thesholds; 
	const uint8_t *State;
} PROFILE_TABLE3_PARAM_T;

typedef struct {
	PROFILE_TABLE_PARAM;
	const void *Thesholds;
	const uint8_t *State;
} PROFILE_TABLE4_PARAM_T;

typedef struct {
	PROFILE_TABLE_PARAM;
} PROFILE_TABLE5_PARAM_T;

typedef struct {
	PROFILE_TABLE_PARAM;
	const uint8_t *State;
} PROFILE_TABLE8_PARAM_T;

typedef struct {
	PROFILE_TABLE_PARAM;
} PROFILE_TABLE103_PARAM_T;

typedef struct {
	PROFILE_TABLE_PARAM;
	const void *Thesholds;
	const uint8_t *State;
} PROFILE_TABLE104_PARAM_T;

typedef struct {
	const uint32_t Value;
	const uint8_t Color;
} PROFILE_ENUM_T;

typedef struct {
	const uint32_t Value;
	const uint8_t Color;
} PROFILE_BITMSK_T;

typedef struct {
	const uint8_t Index;
	const PROFILE_TABLE_PARAM_T* const *Params;
	const uint32_t ParamsCnt;
} PROFILE_TABLE_T;

//=============================================================================
// список возможных таблиц профиля
//=============================================================================
#define PROFILE_TABLE_1					1	//[1] - Информационные параметры
#define PROFILE_TABLE_2					2	//[2] - Динамические параметры
#define PROFILE_TABLE_3					3	//[3] - Установочные параметры
#define PROFILE_TABLE_4					4	//[4] - Статические параметры (пороги)
#define PROFILE_TABLE_5					5	//[5] - Лампочки передней панели (Все только лампочки и имеют тип данных 4==DC_U32)
#define PROFILE_TABLE_6					6	//[6] - Калибровка устройства	(Используется для калибровки)
#define PROFILE_TABLE_7					7	//[7] - Калибровочные коэффициенты (Выводятся калибровочние коэффиценты, или при необходимости устанавливаются)
#define PROFILE_TABLE_8					8	//[8] - Накопительные динамические параметры
#define PROFILE_TABLE_100				100	//[100] - Параметры отладки встроенного компьютера (только для блока управления)
#define PROFILE_TABLE_101				101	//[101] - Сетевые настройки встроенного компьютера (только для блока управления)
#define PROFILE_TABLE_102				102	//[102] - Настройки встроенного компьютера (только для блока управления)
#define PROFILE_TABLE_103				103	//[103] - Информация и категории трэпов по портам
#define PROFILE_TABLE_104				104
#define PROFILE_TABLES_CNT			13
 
//=============================================================================
// Структура общего описания профиля
//=============================================================================
typedef struct {
	const char *DeviceClass;
	const PROFILE_TABLE_T* const *Tables;
	const uint32_t TablesCnt;
	const char* const *Text; //Указатель на файл профиля
	const uint32_t	*TextSize;	//Количество строк в профиле
	const uint32_t	DynamicValuesSize;	//Количество строк в профиле
	const void 			*StaticDefaultValues; //Указатель на дефолтные значения
	const uint32_t	StaticValuesSize;	//Количество строк в профиле
	const void 			*CalibDefaultValues; //Указатель на дефолтные значения
	const uint32_t	CalibValuesSize;	//Количество строк в профиле
} PROFILE_DESCR_T;

typedef struct {
	osMutexId Mutex;
	void *Dynamic;
	void *Static;
	void *Calib;
}	PROFILE_VALUES_T;

#endif /* __PROFILE_TYPES_H */
