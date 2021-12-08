#ifndef __PROFILE_ADM10_H
#define __PROFILE_ADM10_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm10_idx.h"
#include "profile_adm10_values.h"
#include "profile_adm10_sc.h"
#include "profile_adm10_ps.h"
#include "profile_adm10_seeprom.h"
#include "profile_adm10_line.h"
#include "profile_adm10_client.h"
#include "profile_adm10_xc.h"
#include "profile_adm10_pg.h"

void PROFILE_ADM10_GetStringColor(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t *Color, uint8_t DegradeColor, uint8_t FailureColor);
void PROFILE_ADM10_UpdateValues(PROFILE_T *Profile);
int32_t PROFILE_ADM10_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore);
int32_t PROFILE_ADM10_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore);
void PROFILE_ADM10_UpdateConfigs(PROFILE_T *Profile);

#endif /* __PROFILE_ADM10_H */
