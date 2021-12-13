#ifndef __IPGP966_H
#define __IPGP966_H

#include "mcp23008.h"

#define IPGP966_VENDOR		"IPG Photonics"
#define IPGP966_PT_NUMBER	"ONH-S0-0C"
#define IPGP966_HW_NUMBER	"IPGP966"

typedef struct {
	MCP23008_T mcp23008;
} IPGP966_T;

int32_t IPGP966_Process(IPGP966_T *Dev, uint8_t ChassisAddress);
void IPGP966_Deinit(IPGP966_T *Dev);

#endif /*__IPGP966_H*/
