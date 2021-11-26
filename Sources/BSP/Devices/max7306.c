#include "max7306.h"
#include "log.h"

static const uint8_t max7306_pins_addr[MAX7306_PINS_CNT] = {0x01, 0x02, 0x03, 0x04};
static const uint8_t max7306_configs_addr[MAX7306_CONFIGS_CNT] = {0x26, 0x27};

int32_t MAX7306_WritePin(MAX7306_T *Dev, uint32_t Idx, uint8_t Pin)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (Pin & (1 << 7))
	{
		if (Pin != Dev->Pins[Idx])
		{
			if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, (uint8_t*)&max7306_pins_addr[Idx], 1, (char*)&Pin, 1))
				goto error;
			Dev->Pins[Idx] = Pin;
		}
	}
	else
	{
		if ((Pin & 0xE4) != (Dev->Pins[Idx] & 0xE4))
		{
			if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, (uint8_t*)&max7306_pins_addr[Idx], 1, (char*)&Pin, 1))
				goto error;
			Dev->Pins[Idx] = Pin;
		}
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t MAX7306_ReadPin(MAX7306_T *Dev, uint32_t Idx)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&max7306_pins_addr[Idx], 1, (char*)&Dev->Pins[Idx], 1))
			goto error;		
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t MAX7306_Init(MAX7306_T *Dev)
{
	uint32_t i;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	for (i = 0; i < MAX7306_PINS_CNT; i++)
	{
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&max7306_pins_addr[i], 1, (char*)&Dev->Pins[i], 1))
			goto error;		
	}
	
	for (i = 0; i < MAX7306_CONFIGS_CNT; i++)
	{
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&max7306_configs_addr[i], 1, (char*)&Dev->Configs[i], 1))
			goto error;		
	}
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void MAX7306_Deinit(MAX7306_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
