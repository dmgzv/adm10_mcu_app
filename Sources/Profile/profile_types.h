#ifndef __PROFILE_TYPES_H
#define __PROFILE_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include "cmsis_os.h"

#define PROFILE_MUTEX_T	osMutexId
#define PROFILE_MUTEX_INIT(Profile) \
do {\
	if (!(*(Profile)->Mutex)) \
		*(Profile)->Mutex= osMutexCreate(NULL); \
} while(0)
#define PROFILE_MUTEX_LOCK(Profile)	osMutexWait(*(Profile)->Mutex, osWaitForever)
#define PROFILE_MUTEX_UNLOCK(Profile) osMutexRelease(*(Profile)->Mutex)

#define PROFILE_TYPE_NONE 		0
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
#define PROFILE_TYPE_STRING16 33
#define PROFILE_TYPE_STRING32 34
#define PROFILE_TYPE_STRING64 35
#define PROFILE_TYPE_ENUM 		1024
#define PROFILE_TYPE_BITMSK 	2048

#define PROFILE_COLOR_NA 				0 
#define PROFILE_COLOR_NO 				1
#define PROFILE_COLOR_INFO 			2
#define PROFILE_COLOR_NORMAL 		3
#define PROFILE_COLOR_WARNING 	4
#define PROFILE_COLOR_MINOR 		5
#define PROFILE_COLOR_MAJOR 		6
#define PROFILE_COLOR_CRITICAL 	7

#define PROFILE_COLOR_TYPE_NORMAL		0
#define PROFILE_COLOR_TYPE_DEGRADE	1
#define PROFILE_COLOR_TYPE_FAILURE	2

#define	PROFILE_ADMINSTATE_OOS	0
#define	PROFILE_ADMINSTATE_MT		1
#define	PROFILE_ADMINSTATE_IS		2

#define PROFILE_ACCESS_NONE				(0)
#define PROFILE_ACCESS_READ				(1 << 0)
#define PROFILE_ACCESS_WRITE			(3 << 0)

#define PROFILE_ACCESS(MN, US, AD, SU)	(((MN) << 0) | ((US) << 2) | ((AD) << 4) | ((SU) << 6))

typedef struct profile_enum_value {
	const char *Name;
	const int32_t Number;
	const int8_t Color;
} PROFILE_ENUM_VALUE_T;

typedef struct profile_enum {
	const PROFILE_ENUM_VALUE_T *Values;
	const uint16_t ValuesCnt;
} PROFILE_ENUM_T;

typedef struct profile_bitmsk_value {
	const char *Name;
	const int32_t Number;
	const int8_t Color;
} PROFILE_BITMSK_VALUE_T;

typedef struct profile_bitmsk {
	const PROFILE_BITMSK_VALUE_T *Values;
	const uint16_t ValuesCnt;
} PROFILE_BITMSK_T;

typedef struct profile_colors {
	uint8_t *Normal;
	uint8_t *Degrade;
	uint8_t *Failure;
} PROFILE_COLORS_T;

typedef struct profile_param PROFILE_PARAM_T;

typedef struct profile_thrs {
	const PROFILE_PARAM_T *FMin;
	const PROFILE_PARAM_T *DMin;
	const PROFILE_PARAM_T *DMax;
	const PROFILE_PARAM_T *FMax;
} PROFILE_THRS_T;

typedef struct profile_constraint {
	bool (*CheckCondition)(void);
	bool (*CheckValue)(void *Value);
	void (*GetValidValue)(void* Value);
} PROFILE_CONSTRAINT_T;

typedef struct profile_admin_state {
	bool (*CheckCondition)(void);
	int32_t (*GetValue)(void);
} PROFILE_ADMIN_STATE_T;

struct profile_param {
	const char *Name;
	const uint8_t Section;
	const uint16_t Index;
	const uint8_t Access;
	const uint16_t	Type;
	void *Value;
	const void *DefaultValue;
	const PROFILE_ENUM_T *Enum;
	const PROFILE_BITMSK_T *Bitmsk;
	const PROFILE_COLORS_T *Colors;
	const PROFILE_THRS_T *Thrs;
	const PROFILE_CONSTRAINT_T *Constraints;
	const uint32_t *ConstraintsCnt;
	const PROFILE_ADMIN_STATE_T *AdminStates;
	const uint32_t *AdminStatesCnt;
};

typedef struct profile_section {
	const uint8_t Index;
	const PROFILE_PARAM_T **Params;
	const uint16_t ParamsCnt;
} PROFILE_SECTION_T;

typedef struct profile_values {
	void *Values;
	const void *DefaultValues;
	const uint32_t ValuesSize;
} PROFILE_VALUES_T;

typedef union profile_value {
	int32_t i32;
	int64_t i64;
	uint32_t u32;
	uint64_t u64;
	float f32;
	char s16[16];
	char s32[32];
	char s64[64];
} PROFILE_VALUE_T;

typedef struct profile {
	PROFILE_MUTEX_T *Mutex;
	const char **Lines;
	const uint32_t* LinesCnt;
	const PROFILE_SECTION_T **Sections;
	const uint16_t SectionsCnt;
	const PROFILE_VALUES_T *State;
	const PROFILE_VALUES_T *Config;
	const PROFILE_VALUES_T *Calib;
} PROFILE_T;

#endif /* __PROFILE_TYPES_H */
