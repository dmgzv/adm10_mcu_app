#ifndef __SC_H
#define __SC_H

#include "bsp_dev.h"
#include "at24c02c.h"
#include "ipgp966.h"
#include "ipgp967.h"

#define SC_DEV_NAME 	"SC"

typedef struct {
	BSP_DEV;
	char Vendor[64];
	char PtNumber[64];
	char HwNumber[64];
	char SrNumber[64];
	uint8_t ChassisAddress;
	uint32_t __UpdateFlag;
	AT24C02C_T at24c02c;
	IPGP966_T ipgp966;
} SC_T;

int32_t SC_Init(SC_T *Dev);
void SC_Deinit(SC_T *Dev);
int32_t SC_Update(SC_T *Dev);

int32_t SC_ConfigChassisAddress(SC_T *Dev, uint8_t ChassisAddress);

int32_t SC_ConfigVendor(SC_T *Dev, char *Vendor);
int32_t SC_ConfigPtNumber(SC_T *Dev, char *PtNumber);
int32_t SC_ConfigHwNumber(SC_T *Dev, char *HwNumber);
int32_t SC_ConfigSrNumber(SC_T *Dev, char *SrNumber);

int32_t SC_GetVin(SC_T *Dev, float *Vin);
int32_t SC_GetVout(SC_T *Dev, float *Vout);
int32_t SC_GetIout(SC_T *Dev, float *Iout);
int32_t SC_GetTemp(SC_T *Dev, float *Temp, uint32_t Idx);

#endif /*__SC_H*/
