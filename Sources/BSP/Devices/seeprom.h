#ifndef __SEEPROM_H
#define __SEEPROM_H

#include "bsp_dev.h"
#include "at24c02c.h"
#include "ip1562.h"

#define SEEPROM_DEV_NAME	"SEEPROM"
#define SEEPROM_PT_NUMBER	"SEEPROM"

typedef struct {
	BSP_DEV;
	char PtNumber[64];
	char HwNumber[64];
	AT24C02C_T at24c02c;
	IP1562_T ip1562;
} SEEPROM_T;

int32_t SEEPROM_Init(SEEPROM_T *Dev);
void SEEPROM_Deinit(SEEPROM_T *Dev);
int32_t SEEPROM_Update(SEEPROM_T *Dev);

int32_t SEEPROM_WriteInventory(SEEPROM_T *Dev, char *PtNumber, char *HwNumber);

#endif /*__SEEPROM_H*/
