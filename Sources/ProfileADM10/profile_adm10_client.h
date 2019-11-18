#ifndef __PROFILE_ADM10_CLIENT_H
#define __PROFILE_ADM10_CLIENT_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm10_idx.h"

int32_t PROFILE_ADM10_CLIENT_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
int32_t PROFILE_ADM10_CLIENT_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
void PROFILE_ADM10_CLIENT_UpdateConfigs(PROFILE_T *Profile);
void PROFILE_ADM10_CLIENT_UpdateValues(PROFILE_T *Profile);

#endif /* __PROFILE_ADM10_CLIENT_H */
