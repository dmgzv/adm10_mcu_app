#ifndef __PROFILE_ADM_H
#define __PROFILE_ADM_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm_idx.h"
#include "profile_adm_values.h"
#include "profile_adm_line.h"
#include "profile_adm_client.h"

void PROFILE_ADM_UpdateValues(PROFILE_T *Profile, BSP_DEVICES_T *Devices);
int32_t PROFILE_ADM_CheckConfigValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value);
int32_t PROFILE_ADM_SetConfigValueDependencies(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value);
void PROFILE_ADM_UpdateConfigs(PROFILE_T *Profile, BSP_DEVICES_T *Devices, BSP_DEVICES_CFG_T *DevicesCfg);
void PROFILE_ADM_InitStates(PROFILE_T *Profile);

#endif /* __PROFILE_ADM_H */
