#ifndef __PROFILE_ADM_XC_H
#define __PROFILE_ADM_XC_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm_idx.h"

#define PROFILE_ADM_ODU1_XC_CNT	8
#define PROFILE_ADM_ODU0_XC_CNT	16

int32_t PROFILE_ADM_XC_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
int32_t PROFILE_ADM_XC_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
void PROFILE_ADM_XC_UpdateConfigs(PROFILE_T *Profile);
void PROFILE_ADM_XC_UpdateValues(PROFILE_T *Profile);

#endif /* __PROFILE_ADM_XC_H */
