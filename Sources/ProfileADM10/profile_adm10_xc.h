#ifndef __PROFILE_ADM10_XC_H
#define __PROFILE_ADM10_XC_H

#include "bsp.h"
#include "profile.h"
#include "profile_adm10_idx.h"
#include "profile_adm10_values.h"

#define PROFILE_ADM10_XC_LINE_ODU1(Line, ODU1Idx) (1 + ((Line) * 4) + (ODU1Idx))
#define PROFILE_ADM10_XC_CLIENT_ODU1(Client) 			(1025 + (Client))
#define PROFILE_ADM10_XC_LINE_ODU0(Line, ODU0Idx) (1 + ((Line) * 8) + (ODU0Idx))
#define PROFILE_ADM10_XC_CLIENT_ODU0(Client, ODU0Idx) 			(1025 + ((Client) * 2) + (ODU0Idx))

int32_t PROFILE_ADM10_XC_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
int32_t PROFILE_ADM10_XC_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value);
void PROFILE_ADM10_XC_UpdateConfigs(PROFILE_T *Profile);
void PROFILE_ADM10_XC_UpdateValues(PROFILE_T *Profile);

extern const uint16_t ProfileADM10ConfigODU1XCSetSrcIdx[PROFILE_ADM10_ODU1_XCS];
extern const uint16_t ProfileADM10ConfigODU1XCSetDstIdx[PROFILE_ADM10_ODU1_XCS];
extern const uint16_t ProfileADM10ConfigODU0XCSetSrcIdx[PROFILE_ADM10_ODU0_XCS];
extern const uint16_t ProfileADM10ConfigODU0XCSetDstIdx[PROFILE_ADM10_ODU0_XCS];

#endif /* __PROFILE_ADM10_XC_H */
