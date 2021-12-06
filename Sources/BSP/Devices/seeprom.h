#ifndef __SEEPROM_H
#define __SEEPROM_H

#include "bsp_dev.h"
#include "at24c02c.h"
#include "ip1562.h"

#define SEEPROM_DEV_NAME	"SEEPROM"

typedef struct {
	BSP_DEV;
	char Vendor[64];
	char PtNumber[64];
	char HwNumber[64];
	char SrNumber[64];
	uint32_t __UpdateFlag;
	AT24C02C_T at24c02c;
	IP1562_T ip1562;
} SEEPROM_T;

int32_t SEEPROM_Init(SEEPROM_T *Dev);
void SEEPROM_Deinit(SEEPROM_T *Dev);
int32_t SEEPROM_Update(SEEPROM_T *Dev);

int32_t SEEPROM_ConfigVendor(SEEPROM_T *Dev, char *Vendor);
int32_t SEEPROM_ConfigPtNumber(SEEPROM_T *Dev, char *PtNumber);
int32_t SEEPROM_ConfigHwNumber(SEEPROM_T *Dev, char *HwNumber);
int32_t SEEPROM_ConfigSrNumber(SEEPROM_T *Dev, char *SrNumber);

int32_t SEEPROM_ConfigFansSpeed(SEEPROM_T *Dev, float Speed);
float SEEPROM_GetFanSpeed(SEEPROM_T *Dev, uint32_t Idx);

uint32_t SEEPROM_GetPSPresent(SEEPROM_T *Dev, uint32_t Idx);
uint32_t SEEPROM_GetSCPresent(SEEPROM_T *Dev, uint32_t Idx);

#endif /*__SEEPROM_H*/
