#ifndef __PS_H
#define __PS_H

#include "bsp_dev.h"
#include "at24c02c.h"
#include "ip1554.h"

#define PS_DEV_NAME 	"PS"

typedef struct {
	BSP_DEV;
	char Vendor[64];
	char PtNumber[64];
	char HwNumber[64];
	char SrNumber[64];
	uint8_t ChassisAddress;
	uint32_t __UpdateFlag;
	AT24C02C_T at24c02c;
	IP1554_T ip1554;
} PS_T;

int32_t PS_Init(PS_T *Dev);
void PS_Deinit(PS_T *Dev);
int32_t PS_Update(PS_T *Dev);

int32_t PS_ConfigChassisAddress(PS_T *Dev, uint8_t ChassisAddress);

int32_t PS_ConfigVendor(PS_T *Dev, char *Vendor);
int32_t PS_ConfigPtNumber(PS_T *Dev, char *PtNumber);
int32_t PS_ConfigHwNumber(PS_T *Dev, char *HwNumber);
int32_t PS_ConfigSrNumber(PS_T *Dev, char *SrNumber);

int32_t PS_GetVin(PS_T *Dev, float *Vin);
int32_t PS_GetVout(PS_T *Dev, float *Vout);
int32_t PS_GetIout(PS_T *Dev, float *Iout);
int32_t PS_GetTemp(PS_T *Dev, float *Temp, uint32_t Idx);

#endif /*__PS_H*/
