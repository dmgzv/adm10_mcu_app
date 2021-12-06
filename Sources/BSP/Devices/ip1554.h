#ifndef __IP1554_H
#define __IP1554_H

#include "max7306.h"
#include "dbe0125v2.h"

#define IP1554_VENDOR			"IPG"
#define IP1554_PT_NUMBER	"ONH011C400"
#define IP1554_HW_NUMBER	"IP1554"

typedef struct {
	MAX7306_T max7306;
	DBE0125V2_T dbe0125v2;
} IP1554_T;

int32_t IP1554_Process(IP1554_T *Dev, uint8_t ChassisAddress);
void IP1554_Deinit(IP1554_T *Dev);

int32_t IP1554_GetVin(IP1554_T *Dev, float *Vin);
int32_t IP1554_GetVout(IP1554_T *Dev, float *Vout);
int32_t IP1554_GetIout(IP1554_T *Dev, float *Iout);
int32_t IP1554_GetTemp(IP1554_T *Dev, float *Temp, uint32_t Idx);

#endif /*__IP1554_H*/
