#ifndef __IP1562_H
#define __IP1562_H

#include "pca9538.h"
#include "max31790.h"

#define IP1562_VENDOR			"IPG"
#define IP1562_PT_NUMBER	"SEEPROM"
#define IP1562_HW_NUMBER	"IP1562"

#define IP1562_PCA9538_PS_PRESENT_MASK(Idx)	((Idx) ? (1 << 1) : (1 << 0))
#define IP1562_PCA9538_SC_PRESENT_MASK(Idx)	((Idx) ? (1 << 2) : (1 << 3))
#define IP1562_PCA9538_MB_PRESENT_MASK			(1 << 4)

typedef struct {
	PCA9538_T pca9538;
	MAX31790_T max31790;
} IP1562_T;

int32_t IP1562_Process(IP1562_T *Dev);
void IP1562_Deinit(IP1562_T *Dev);

int32_t IP1562_ConfigFansSpeed(IP1562_T *Dev, float Speed);
float IP1562_GetFanSpeed(IP1562_T *Dev, uint32_t Idx);

uint32_t IP1562_GetPSPresent(IP1562_T *Dev, uint32_t Idx);
uint32_t IP1562_GetSCPresent(IP1562_T *Dev, uint32_t Idx);

#endif /*__IP1562_H*/
