#ifndef __PROFILE_ADM10_SEEPROM_H
#define __PROFILE_ADM10_SEEPROM_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm10_idx.h"
#include "profile_adm10_values.h"

void PROFILE_ADM10_SEEPROM_GetStringColor(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t *Color, uint8_t DegradeColor, uint8_t FailureColor);
int32_t PROFILE_ADM10_SEEPROM_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore);
int32_t PROFILE_ADM10_SEEPROM_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore);
void PROFILE_ADM10_SEEPROM_UpdateConfigs(PROFILE_T *Profile);
void PROFILE_ADM10_SEEPROM_UpdateValues(PROFILE_T *Profile);

#endif /* __PROFILE_ADM10_SEEPROM_H */
