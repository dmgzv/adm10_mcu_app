#ifndef __PROFILE_ADM_H
#define __PROFILE_ADM_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm_idx.h"
#include "profile_adm_values.h"
#include "profile_adm_line.h"
#include "profile_adm_client.h"
#include "profile_adm_xc.h"
#include "profile_adm_pg.h"

void PROFILE_ADM_UpdateValues(PROFILE_T *Profile);
int32_t PROFILE_ADM_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
int32_t PROFILE_ADM_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
void PROFILE_ADM_UpdateConfigs(PROFILE_T *Profile);

#endif /* __PROFILE_ADM_H */
