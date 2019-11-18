#ifndef __PROFILE_H
#define __PROFILE_H

/* Includes ------------------------------------------------------------------*/
#include "profile_types.h"
#include "profile_flash.h"
#include "bootloader.h"

#define PROFILE_COLOR_TYPE_AUTO	0
#define PROFILE_COLOR_TYPE_NO	1
#define PROFILE_COLOR_TYPE_NORMAL	2
#define PROFILE_COLOR_TYPE_DEGRADE	3
#define PROFILE_COLOR_TYPE_FAILURE	4

typedef struct {
	uint32_t Idx;
	PROFILE_DESCR_T **Descr;
	PROFILE_VALUES_T Values;
	uint32_t TablesChanges;
	uint32_t SetEnable;
	uint32_t ValuesLock;
	uint16_t ValuesCRC16;
	PROFILE_FLASH_T ProfileFlash;
	BOOTLOADER_T Bootloader;
} PROFILE_T;

int32_t PROFILE_Init(PROFILE_T *Profile, PROFILE_DESCR_T **Descr, void *Static, uint32_t StaticSize, void *Dynamic, uint32_t DynamicSize, void *Calib, uint32_t CalibSize);

int32_t PROFILE_ValueToString(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, char* Str , uint32_t *Len);
int32_t PROFILE_StringToValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, char* Str);

int32_t PROFILE_CheckValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
int32_t PROFILE_SetValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
int32_t PROFILE_SetValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *SetValue);
int32_t PROFILE_GetValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *GetValue);
void PROFILE_SaveValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx);
int32_t PROFILE_SetFactoryValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx);

int32_t PROFILE_SetAlarmColors(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t DegradeColor, uint8_t FailureColor);
int32_t PROFILE_GetAlarmColors(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t *DegradeColor, uint8_t *FailureColor);
int32_t PROFILE_SetFactoryAlarmColors(PROFILE_T *Profile, uint8_t Table, uint16_t Idx);
	
int32_t PROFILE_SetFactoryTable(PROFILE_T *Profile, uint8_t Table);
int32_t PROFILE_SetFactory(PROFILE_T *Profile);
int32_t PROFILE_SetDefault(PROFILE_T *Profile);

int32_t PROFILE_GetColor(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t *GetColor);
int32_t PROFILE_SetColor(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t SetColor);
int32_t PROFILE_SetColorToThresholds(PROFILE_T *Profile, uint8_t Table, uint16_t Idx);
int32_t PROFILE_SetColorToDegrade(PROFILE_T *Profile, uint8_t Table, uint16_t Idx);
int32_t PROFILE_SetColorToFailure(PROFILE_T *Profile, uint8_t Table, uint16_t Idx);

int32_t PROFILE_GetState(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t *GetState);
int32_t PROFILE_SetState(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, uint8_t SetState);

int32_t PROFILE_UpdateParam(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint8_t ColorType, uint8_t State);

#endif /* __PROFILE_H */
