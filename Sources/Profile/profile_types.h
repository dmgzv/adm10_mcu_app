#ifndef __PROFILE_TYPES_H
#define __PROFILE_TYPES_H

#include <stdint.h>
#include <stdlib.h>
#include "cmsis_os.h"

#define PROFILE_MUTEX_T	osMutexId
#define PROFILE_STRING_MAX_LEN	64

typedef enum {
	PROFILE_TYPE_BOOL = (1 << 0),
	PROFILE_TYPE_INT32 = (1 << 1),
	PROFILE_TYPE_UINT32 = (1 << 2),
	PROFILE_TYPE_FLOAT_1 = (1 << 3),
	PROFILE_TYPE_FLOAT_2 = (1 << 3) | (1 << 28),
	PROFILE_TYPE_FLOAT_3 = (1 << 3) | (2 << 28),
	PROFILE_TYPE_FLOAT_E = (1 << 3) | (7 << 28),
	PROFILE_TYPE_HEX32 = (1 << 4),
	PROFILE_TYPE_STRING = (1 << 5),
	PROFILE_TYPE_ENUM = (1 << 6),
	PROFILE_TYPE_OID = (1 << 7),
	PROFILE_TYPE_IP = (1 << 8),
	PROFILE_TYPE_CNT = (1 << 9),
	PROFILE_TYPE_CNT64 = (1 << 10),
	PROFILE_TYPE_BITMSK = (1 << 11)
} PROFILE_TYPE_T;

typedef enum {
	PROFILE_COLOR_NA = 0, //- параметр не может иметь соответствующего типа аварии. Цвет для данного параметра не определен.
	PROFILE_COLOR_NO = 1,//- Нет цвета в текущем состоянии
	PROFILE_COLOR_INFO = 2,//- Белый	  (Цвет для параметров ENUM)
	PROFILE_COLOR_NORMAL = 3,//- Зелёный
	PROFILE_COLOR_WARNING = 4,//- Голубой
	PROFILE_COLOR_MINOR = 5,//- Жёлтый
	PROFILE_COLOR_MAJOR = 6,//- Оранжевый
	PROFILE_COLOR_CRITICAL = 7,//- Красный
} PROFILE_COLOR_T;

typedef enum {
	PROFILE_STATE_OOS = 0,
	PROFILE_STATE_MT,
	PROFILE_STATE_IS
}PROFILE_STATE_T;

#define PROFILE_THR_CMAX				3//	Верхний критический предел
#define PROFILE_THR_WMAX				2//	Верхний порог (предупреждение)
#define PROFILE_THR_WMIN				1// Нижний порог (предупреждение)
#define PROFILE_THR_CMIN				0// Нижнее критический предел

#define PROFILE_ACCESS_NONE				(0)
#define PROFILE_ACCESS_READ				(1 << 0)
#define PROFILE_ACCESS_WRITE			(3 << 0)

#define PROFILE_ACCESS(SU, AD, US, MN)	(((MN) << 0) | ((US) << 2) | ((AD) << 4) | ((SU) << 6))

typedef struct {
	uint32_t Table;
	uint32_t Index;
	uint32_t Access;
	PROFILE_TYPE_T	Type;
	void *Value;
	PROFILE_COLOR_T *Color;
	PROFILE_COLOR_T *DegradeColor;//Цвет предупреждения
	PROFILE_COLOR_T *FailureColor;//Цвет критического значения
	void *Thesholds; 
	uint8_t *State;
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
	char *DeviceClass;
	const PROFILE_PARAM_T *Tables[ PROFILE_TABLES_CNT ];	//Указатель на таблицу профиля
	const uint32_t	TablesSize[ PROFILE_TABLES_CNT ];	//Количество строк (параметров) для таблицы профиля
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
