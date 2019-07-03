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

#define	PROFILE_STATE_OOS 0
#define	PROFILE_STATE_MT	1
#define	PROFILE_STATE_IS	2

#define PROFILE_THR_CMAX				0//	Верхний критический предел
#define PROFILE_THR_WMAX				1//	Верхний порог (предупреждение)
#define PROFILE_THR_WMIN				2// Нижний порог (предупреждение)
#define PROFILE_THR_CMIN				3// Нижнее критический предел

#define PROFILE_ACCESS_NONE				(0)
#define PROFILE_ACCESS_READ				(1 << 0)
#define PROFILE_ACCESS_WRITE			(3 << 0)

#define PROFILE_ACCESS(SU, AD, US, MN)	(((MN) << 0) | ((US) << 2) | ((AD) << 4) | ((SU) << 6))

typedef struct {
	uint8_t Table;
	uint16_t Index;
	uint8_t Access;
	uint8_t	Type;
	void *Value;
	uint8_t *State;
} PROFILE_TABLE1_PARAM_T;

typedef struct {
	uint8_t Table;
	uint16_t Index;
	uint8_t Access;
	uint8_t	Type;
	void *Value;
	uint8_t *Color;
	uint8_t *DegradeColor;//Цвет предупреждения
	uint8_t *FailureColor;//Цвет критического значения
	void *Thesholds; 
	uint8_t *State;
} PROFILE_TABLE2_PARAM_T;

typedef struct {
	uint8_t Table;
	uint16_t Index;
	uint8_t Access;
	uint8_t	Type;
	void *Value;
} PROFILE_TABLE3_PARAM_T;

typedef struct {
	uint8_t Table;
	uint16_t Index;
	uint8_t Access;
	uint8_t	Type;
	void *Value;
	void *Thesholds; 
} PROFILE_TABLE4_PARAM_T;

typedef struct {
	uint8_t Table;
	uint16_t Index;
	uint8_t Access;
	uint8_t	Type;
	void *Value;
} PROFILE_TABLE5_PARAM_T;

typedef struct {
	uint8_t Table;
	uint16_t Index;
	uint8_t Access;
	uint8_t	Type;
	void *Value;
	uint8_t *State;
} PROFILE_TABLE8_PARAM_T;

typedef struct {
	uint8_t Table;
	uint16_t Index;
	uint8_t Access;
	uint8_t	Type;
	void *Value;
} PROFILE_TABLE103_PARAM_T;

typedef struct {
	uint8_t Table;
	uint16_t Index;
	uint8_t Access;
	uint8_t	Type;
	void *Value;
} PROFILE_PARAM_T;

//=============================================================================
// список возможных таблиц профиля
//=============================================================================
#define PROFILE_TABLE_1					0	//[1] - Информационные параметры
#define PROFILE_TABLE_2					1	//[2] - Динамические параметры
#define PROFILE_TABLE_3					2	//[3] - Установочные параметры
#define PROFILE_TABLE_4					3	//[4] - Статические параметры (пороги)
#define PROFILE_TABLE_5					4	//[5] - Лампочки передней панели (Все только лампочки и имеют тип данных 4==DC_U32)
#define PROFILE_TABLE_6					5	//[6] - Калибровка устройства	(Используется для калибровки)
#define PROFILE_TABLE_7					6	//[7] - Калибровочные коэффициенты (Выводятся калибровочние коэффиценты, или при необходимости устанавливаются)
#define PROFILE_TABLE_8					7	//[8] - Накопительные динамические параметры
#define PROFILE_TABLE_100				8	//[100] - Параметры отладки встроенного компьютера (только для блока управления)
#define PROFILE_TABLE_101				9	//[101] - Сетевые настройки встроенного компьютера (только для блока управления)
#define PROFILE_TABLE_102				10	//[102] - Настройки встроенного компьютера (только для блока управления)
#define PROFILE_TABLE_103				11	//[103] - Информация и категории трэпов по портам
#define PROFILE_TABLES_CNT			12
 
//=============================================================================
// Структура общего описания профиля
//=============================================================================
typedef struct {
	const char *DeviceClass;
	const PROFILE_TABLE1_PARAM_T *Table1;	//Указатель на таблицу профиля
	const uint32_t Table1Size;	//Количество строк (параметров) для таблицы профиля
	const PROFILE_TABLE2_PARAM_T *Table2;	//Указатель на таблицу профиля
	const uint32_t Table2Size;	//Количество строк (параметров) для таблицы профиля
	const PROFILE_TABLE3_PARAM_T *Table3;	//Указатель на таблицу профиля
	const uint32_t Table3Size;	//Количество строк (параметров) для таблицы профиля
	const PROFILE_TABLE4_PARAM_T *Table4;	//Указатель на таблицу профиля
	const uint32_t Table4Size;	//Количество строк (параметров) для таблицы профиля
	const PROFILE_TABLE5_PARAM_T *Table5;	//Указатель на таблицу профиля
	const uint32_t Table5Size;	//Количество строк (параметров) для таблицы профиля
	const PROFILE_TABLE8_PARAM_T *Table8;	//Указатель на таблицу профиля
	const uint32_t Table8Size;	//Количество строк (параметров) для таблицы профиля
	const PROFILE_TABLE103_PARAM_T *Table103;	//Указатель на таблицу профиля
	const uint32_t Table103Size;	//Количество строк (параметров) для таблицы профиля
	const char 			**Text; //Указатель на файл профиля
	const uint32_t	*TextSize;	//Количество строк в профиле
	const void 			*DefaultValues; //Указатель на дефолтные значения
} PROFILE_DESCR_T;

typedef struct {
	osMutexId Mutex;
	void *Dynamic;
	uint32_t DynamicSize;
	void *Static;
	uint32_t StaticSize;
}	PROFILE_VALUES_T;

#endif /* __PROFILE_TYPES_H */
