#ifndef __PROFILE_ADM_LINE_H
#define __PROFILE_ADM_LINE_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm_idx.h"

int32_t PROFILE_ADM_LINE_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
int32_t PROFILE_ADM_LINE_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
void PROFILE_ADM_LINE_UpdateConfigs(PROFILE_T *Profile);
void PROFILE_ADM_LINE_UpdateValues(PROFILE_T *Profile);

#endif /* __PROFILE_ADM_LINE_H */
