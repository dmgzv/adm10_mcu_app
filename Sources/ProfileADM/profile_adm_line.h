#ifndef __PROFILE_ADM_LINE_H
#define __PROFILE_ADM_LINE_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm_idx.h"

int32_t PROFILE_ADM_LINE_CheckConfigValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value);
int32_t PROFILE_ADM_LINE_SetConfigValueDependencies(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value);
void PROFILE_ADM_LINE_UpdateConfigs(PROFILE_T *Profile, BSP_DEVICES_T *Devices, BSP_DEVICES_CFG_T *DevicesCfg);
void PROFILE_ADM_LINE_UpdateValues(PROFILE_T *Profile, BSP_DEVICES_T *Devices);

#endif /* __PROFILE_ADM_LINE_H */
