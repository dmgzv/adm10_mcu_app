#include "ipgp966.h"

int32_t IPGP966_MCP23008_Process(IPGP966_T *Dev, uint8_t ChassisAddress)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(&Dev->mcp23008) != BSP_DEV_STATE_OK)
	{
		MCP23008_Init(&Dev->mcp23008);
	}
	else
	{
		MCP23008_SetIODIR(&Dev->mcp23008, 0x00);
		MCP23008_SetGPIO(&Dev->mcp23008, ChassisAddress);
	}
	return 0;
}

int32_t IPGP966_Process(IPGP966_T *Dev, uint8_t ChassisAddress)
{
	if (!Dev)
		return -1;
	
	IPGP966_MCP23008_Process(Dev, ChassisAddress);
	
	return 0;
}

void IPGP966_Deinit(IPGP966_T *Dev)
{
	if (!Dev)
		return;
	MCP23008_Deinit(&Dev->mcp23008);
}
