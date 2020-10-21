#ifndef __PROFILE_H
#define __PROFILE_H

/* Includes ------------------------------------------------------------------*/
#include "profile_types.h"
#include "bootloader.h"




int32_t PROFILE_ParamValueToString(uint16_t Type, PROFILE_VALUE_T *Value, char *String, uint32_t *Len);
int32_t PROFILE_StringToParamValue(uint16_t Type, PROFILE_VALUE_T *Value, char *String, uint32_t Len);
int32_t PROFILE_GetParamValueSize(uint16_t Type, uint8_t *ValueSize);

int32_t PROFILE_GetParamByIndex(const PROFILE_T *Profile, uint8_t SectionIndex, uint32_t ParamIndex, PROFILE_PARAM_T** Param);
int32_t PROFILE_GetParamByName(const PROFILE_T *Profile, uint8_t SectionIndex, char* Name, uint32_t NameLen, PROFILE_PARAM_T** Param);

/* ==== These functions are not thread safe. Use external profile mutex ===============*/
int32_t __PROFILE_SetParamValue(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, PROFILE_VALUE_T *Value);
int32_t __PROFILE_GetParamValue(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, PROFILE_VALUE_T *Value);
int32_t __PROFILE_GetParamAdminState(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, int32_t *AdminState);
int32_t __PROFILE_GetParamColor(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, uint8_t *Color);
int32_t __PROFILE_SetParamColors(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, uint8_t Normal, uint8_t Degrade, uint8_t Failure);
int32_t __PROFILE_GetParamColors(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, uint8_t *Normal, uint8_t *Degrade, uint8_t *Failure);
/*=====================================================================================*/

int32_t PROFILE_IsCalibParam(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param);
int32_t PROFILE_IsConfigParam(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param);

void PROFILE_Init(const PROFILE_T *Profile);

#endif /* __PROFILE_H */
