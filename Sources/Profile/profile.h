#ifndef __PROFILE_H
#define __PROFILE_H

/* Includes ------------------------------------------------------------------*/
#include "profile_types.h"
#include "profile_flash.h"
#include "bootloader.h"

typedef struct {
	uint32_t Idx;
	PROFILE_DESCR_T **Descr;
	PROFILE_VALUES_T Values;
	uint32_t TablesChanges;
	uint32_t SetEnable;
	PROFILE_FLASH_T ProfileFlash;
	BOOTLOADER_T Bootloader;
} PROFILE_T;

int32_t PROFILE_Init(PROFILE_T *Profile, PROFILE_DESCR_T **Descr, void *Static, uint32_t StaticSize, void *Dynamic, uint32_t DynamicSize, BSP_FLASH_T *FlashInstance);

int32_t PROFILE_ValueToString(PROFILE_TYPE_T Type, void *Value, char* Str , uint32_t *Len);
int32_t PROFILE_StringToValue(PROFILE_TYPE_T Type, void *Value, char* Str);

int32_t PROFILE_CheckValue(PROFILE_T *Profile, PROFILE_PARAM_T *Param, void *Value);
int32_t PROFILE_SetValueDependencies(PROFILE_T *Profile, PROFILE_PARAM_T *Param, void *Value);
int32_t PROFILE_SetValue(PROFILE_T *Profile, PROFILE_PARAM_T *Param, void *Value);
int32_t PROFILE_GetValue(PROFILE_T *Profile, PROFILE_PARAM_T *Param, void *Value);
int32_t PROFILE_SetFactoryValue(PROFILE_T *Profile, PROFILE_PARAM_T * Param);

void PROFILE_SetAlarmColors(PROFILE_T *Profile, PROFILE_PARAM_T *Param, PROFILE_COLOR_T DegradeColor, PROFILE_COLOR_T FailureColor);
void PROFILE_GetAlarmColors(PROFILE_T *Profile, PROFILE_PARAM_T *Param, PROFILE_COLOR_T *DegradeColor, PROFILE_COLOR_T *FailureColor);
void PROFILE_SetFactoryAlarmColors(PROFILE_T *Profile, PROFILE_PARAM_T * Param);
	
void PROFILE_SetFactory(PROFILE_T *Profile);
void PROFILE_SetDefault(PROFILE_T *Profile);

void PROFILE_GetColor(PROFILE_T *Profile, PROFILE_PARAM_T *Param, PROFILE_COLOR_T *Color);
void PROFILE_SetColor(PROFILE_T *Profile, PROFILE_PARAM_T *Param, PROFILE_COLOR_T Color);
void PROFILE_SetColorToThresholds(PROFILE_T *Profile, PROFILE_PARAM_T * Param);
void PROFILE_SetColorToDegrade(PROFILE_T *Profile, PROFILE_PARAM_T *Param);
void PROFILE_SetColorToFailure(PROFILE_T *Profile, PROFILE_PARAM_T *Param);

void PROFILE_GetState(PROFILE_T *Profile, PROFILE_PARAM_T *Param, uint8_t *State);
void PROFILE_SetState(PROFILE_T *Profile, PROFILE_PARAM_T *Param, uint8_t State);

#endif /* __PROFILE_H */
