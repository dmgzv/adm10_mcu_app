#ifndef __PROFILE_ADM10_PG_H
#define __PROFILE_ADM10_PG_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm10_idx.h"

#define PROFILE_ADM10_ODU1_XC_CNT	8
#define PROFILE_ADM10_ODU0_XC_CNT	16

int32_t PROFILE_ADM10_PG_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
int32_t PROFILE_ADM10_PG_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
void PROFILE_ADM10_PG_UpdateConfigs(PROFILE_T *Profile);
void PROFILE_ADM10_PG_UpdateValues(PROFILE_T *Profile);

#endif /* __PROFILE_ADM10_PG_H */
