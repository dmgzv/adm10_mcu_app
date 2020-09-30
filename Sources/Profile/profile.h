#ifndef __PROFILE_H
#define __PROFILE_H

/* Includes ------------------------------------------------------------------*/
#include "profile_types.h"
#include "bootloader.h"

/* This functions are not thread safe*/

int32_t __PROFILE_ParamValueToString(uint16_t Type, PROFILE_VALUE_T *Value, char *String, uint32_t *Len);
int32_t __PROFILE_StringToParamValue(uint16_t Type, PROFILE_VALUE_T *Value, char *String, uint32_t Len);

int32_t __PROFILE_GetParamByIndex(const PROFILE_T *Profile, uint8_t SectionIndex, uint32_t ParamIndex, PROFILE_PARAM_T** Param);
int32_t __PROFILE_SetParamValue(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, PROFILE_VALUE_T *Value);
int32_t __PROFILE_GetParamValue(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, PROFILE_VALUE_T *Value);
int32_t __PROFILE_GetParamAdminState(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, int32_t *AdminState);
int32_t __PROFILE_GetParamColor(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, uint8_t *Color);
int32_t __PROFILE_SetParamColors(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, uint8_t Normal, uint8_t Degrade, uint8_t Failure);
int32_t __PROFILE_GetParamColors(const PROFILE_T *Profile, const PROFILE_PARAM_T *Param, uint8_t *Normal, uint8_t *Degrade, uint8_t *Failure);
void PROFILE_Init(const PROFILE_T *Profile);

#endif /* __PROFILE_H */
