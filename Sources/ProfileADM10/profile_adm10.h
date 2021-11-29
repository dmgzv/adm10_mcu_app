#ifndef __PROFILE_ADM10_H
#define __PROFILE_ADM10_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm10_idx.h"
#include "profile_adm10_values.h"
#include "profile_adm10_ps.h"
#include "profile_adm10_fan.h"
#include "profile_adm10_line.h"
#include "profile_adm10_client.h"
#include "profile_adm10_xc.h"
#include "profile_adm10_pg.h"

void PROFILE_ADM10_UpdateValues(PROFILE_T *Profile);
int32_t PROFILE_ADM10_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore);
int32_t PROFILE_ADM10_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore);
void PROFILE_ADM10_UpdateConfigs(PROFILE_T *Profile);

#endif /* __PROFILE_ADM10_H */
