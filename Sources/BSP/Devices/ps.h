#ifndef __PS_H
#define __PS_H

#include "bsp_dev.h"
#include "at24c02c.h"
#include "ip1554.h"

#define PS_DEV_NAME 	"PS"
#define PS_PT_NUMBER	"PS"

typedef struct {
	BSP_DEV;
	char PtNumber[64];
	char HwNumber[64];
	uint8_t ChassisAddress;
	AT24C02C_T at24c02c;
	IP1554_T ip1554;
} PS_T;

int32_t PS_Init(PS_T *Dev);
void PS_Deinit(PS_T *Dev);
int32_t PS_Update(PS_T *Dev);

int32_t PS_ConfigChassisAddress(PS_T *Dev, uint8_t ChassisAddress);
int32_t PS_WriteInventory(PS_T *Dev, char *PtNumber, char *HwNumber);

#endif /*__PS_H*/
