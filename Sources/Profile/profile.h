#ifndef __PROFILE_H
#define __PROFILE_H

/* Includes ------------------------------------------------------------------*/
#include "profile_types.h"
#include "bootloader.h"

typedef struct {
	uint32_t Idx;
	PROFILE_DESCR_T **Descr;
	PROFILE_VALUES_T *Values;
	uint32_t TablesChanges;
	uint32_t ValuesLock;
	uint16_t ValuesCRC16;
} PROFILE_T;

#define PROFILE_IS_CALIB_PARAM(Param) \
	(((Param->Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && \
	((Param->Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))

#define PROFILE_TABLE_CNANGE_MASK(TableIdx) \
	(1 << (((TableIdx) <= PROFILE_TABLE_8) ? (TableIdx) : (((TableIdx) - PROFILE_TABLE_100) + 10)))


int32_t PROFILE_GetTable(PROFILE_T *Profile, uint8_t TableIdx, PROFILE_TABLE_T **Table);
int32_t PROFILE_GetParam(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, PROFILE_TABLE_PARAM_T **Param);
int32_t PROFILE_GetParamValue(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void **Value);
int32_t PROFILE_ParamValueToString(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void *Value, char* Str , uint32_t *Len);
int32_t PROFILE_StringToParamValue(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void *Value, char* Str);
int32_t PROFILE_GetParamDefaultValue(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void **DefaultValue);
int32_t PROFILE_GetParamThrValues(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void **Values);
int32_t PROFILE_GetParamState(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, uint8_t *State);
int32_t PROFILE_SetParamState(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, uint8_t State);
int32_t PROFILE_GetParamAlarmColors(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, uint8_t *Degrade, uint8_t *Failure);
int32_t PROFILE_SetParamAlarmColors(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, uint8_t Degrade, uint8_t Failure);
int32_t PROFILE_GetParamDefaultAlarmColors(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, uint8_t *Degrade, uint8_t *Failure);
int32_t PROFILE_SetParamDefaultAlarmColors(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param);
int32_t PROFILE_GetParamColor(PROFILE_T *Profile, PROFILE_TABLE_PARAM_T *Param, void *Value, uint8_t *Color);

int32_t PROFILE_Init(PROFILE_T *Profile, PROFILE_DESCR_T **Descr, PROFILE_VALUES_T *Values);

int32_t PROFILE_ValueToString(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, char* Str , uint32_t *Len);
int32_t PROFILE_StringToValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, char* Str);

int32_t PROFILE_CheckValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, uint32_t Restore);
int32_t PROFILE_CheckWriteAccess(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t Access);
int32_t PROFILE_SetValueDependencies(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, uint32_t Restore);
int32_t PROFILE_SetValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *SetValue);
int32_t PROFILE_GetValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *GetValue);
int32_t PROFILE_SetFactoryValue(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx);

int32_t PROFILE_SetAlarmColors(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t DegradeColor, uint8_t FailureColor);
int32_t PROFILE_GetAlarmColors(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t *DegradeColor, uint8_t *FailureColor);
int32_t PROFILE_SetFactoryAlarmColors(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx);
	
int32_t PROFILE_SetFactoryTable(PROFILE_T *Profile, uint8_t Table);
int32_t PROFILE_SetFactory(PROFILE_T *Profile);
int32_t PROFILE_SetDefault(PROFILE_T *Profile);

int32_t PROFILE_GetColor(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t *GetColor);
int32_t PROFILE_SetColor(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t SetColor);

int32_t PROFILE_GetState(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t *GetState);
int32_t PROFILE_SetState(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t SetState);

int32_t PROFILE_UpdateParam(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, void *Value, uint8_t State);

#endif /* __PROFILE_H */
