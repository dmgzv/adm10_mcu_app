#ifndef __PROFILE_ADM10_PS_H
#define __PROFILE_ADM10_PS_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm10_idx.h"
#include "profile_adm10_values.h"

int32_t PROFILE_ADM10_PS_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore);
int32_t PROFILE_ADM10_PS_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore);
void PROFILE_ADM10_PS_UpdateConfigs(PROFILE_T *Profile);
void PROFILE_ADM10_PS_UpdateValues(PROFILE_T *Profile);

#endif /* __PROFILE_ADM10_PS_H */
