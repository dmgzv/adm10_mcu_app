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

extern uint32_t ProfileADM10DynamicODU2PGStateIdx[];
extern uint32_t ProfileADM10DynamicODU2PGActiveODUkIdx[];
extern uint32_t ProfileADM10ConfigODU2PGSetTypeIdx[];
extern uint32_t ProfileADM10ConfigODU2PGSetModeIdx[];
extern uint32_t ProfileADM10ConfigODU2PGSetMonModeIdx[];
extern uint32_t ProfileADM10ConfigODU2PGSetRevModeIdx[];
extern uint32_t ProfileADM10ConfigODU2PGSetPrimaryODUkIdx[];
extern uint32_t ProfileADM10ConfigODU2PGSetProtectODUkIdx[];
extern uint32_t ProfileADM10ConfigODU2PGSetWTRTmrIdx[];
extern uint32_t ProfileADM10ConfigODU2PGSetHOTmrIdx[];
extern uint32_t ProfileADM10ConfigODU2PGSetFactoryIdx[];
extern uint32_t ProfileADM10DynamicODU1PGStateIdx[];
extern uint32_t ProfileADM10DynamicODU1PGActiveODUkIdx[];
extern uint32_t ProfileADM10ConfigODU1PGSetTypeIdx[];
extern uint32_t ProfileADM10ConfigODU1PGSetModeIdx[];
extern uint32_t ProfileADM10ConfigODU1PGSetMonModeIdx[];
extern uint32_t ProfileADM10ConfigODU1PGSetRevModeIdx[];
extern uint32_t ProfileADM10ConfigODU1PGSetPrimaryODUkIdx[];
extern uint32_t ProfileADM10ConfigODU1PGSetProtectODUkIdx[];
extern uint32_t ProfileADM10ConfigODU1PGSetWTRTmrIdx[];
extern uint32_t ProfileADM10ConfigODU1PGSetHOTmrIdx[];
extern uint32_t ProfileADM10ConfigODU1PGSetFactoryIdx[];
extern uint32_t ProfileADM10DynamicODU0PGStateIdx[];
extern uint32_t ProfileADM10DynamicODU0PGActiveODUkIdx[];
extern uint32_t ProfileADM10ConfigODU0PGSetTypeIdx[];
extern uint32_t ProfileADM10ConfigODU0PGSetModeIdx[];
extern uint32_t ProfileADM10ConfigODU0PGSetMonModeIdx[];
extern uint32_t ProfileADM10ConfigODU0PGSetRevModeIdx[];
extern uint32_t ProfileADM10ConfigODU0PGSetPrimaryODUkIdx[];
extern uint32_t ProfileADM10ConfigODU0PGSetProtectODUkIdx[];
extern uint32_t ProfileADM10ConfigODU0PGSetWTRTmrIdx[];
extern uint32_t ProfileADM10ConfigODU0PGSetHOTmrIdx[];
extern uint32_t ProfileADM10ConfigODU0PGSetFactoryIdx[];

#endif /* __PROFILE_ADM10_PG_H */
