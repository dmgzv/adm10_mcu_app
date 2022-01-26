#ifndef __IP1575_H
#define __IP1575_H

#include "pca9538.h"

#define IP1575_VENDOR		"IPG Photonics"
#define IP1575_PT_NUMBER	"ONH-O0-0C"
#define IP1575_HW_NUMBER	"IP1575"

typedef struct {
	PCA9538_T pca9538;
} IP1575_T;

int32_t IP1575_Process(IP1575_T *Dev, uint8_t ChassisAddress);
void IP1575_Deinit(IP1575_T *Dev);

#endif /*__IP1575_H*/
