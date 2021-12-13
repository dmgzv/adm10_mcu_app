#ifndef __MCP23008_H
#define __MCP23008_H

#include "bsp_dev.h"
#include "i2c.h"

#define MCP23008_DEV_NAME "MCP23008"

typedef struct {
	BSP_DEV;
	const I2C_TypeDef *I2C;
	const uint8_t Addr;
	uint8_t GPIO;
	uint8_t IODIR;
	uint8_t IPOL;
} MCP23008_T;

int32_t MCP23008_GetIODIR(MCP23008_T *Dev);
int32_t MCP23008_SetIODIR(MCP23008_T *Dev, uint8_t IODIR);
int32_t MCP23008_GetGPIO(MCP23008_T *Dev);
int32_t MCP23008_SetGPIO(MCP23008_T *Dev, uint8_t GPIO);
int32_t MCP23008_GetIPOL(MCP23008_T *Dev);
int32_t MCP23008_SetIPOL(MCP23008_T *Dev, uint8_t IPOL);

int32_t MCP23008_Init(MCP23008_T *Dev);
void MCP23008_Deinit(MCP23008_T *Dev);

#endif /* __MCP23008_H */
