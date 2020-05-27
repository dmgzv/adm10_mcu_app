#include "si5332.h"
#include "log.h"

int32_t SI5332_Init(SI5332_T *Dev, const SI5332_CFG_T *Config, const uint32_t ConfigSize)
{
	uint32_t i;
	uint8_t Reg;
	uint8_t Data;
	
	if ((!Dev) || (!Dev->I2C) || (Dev->Addr > (0x6B << 1)) || (Dev->Addr < (0x6A << 1)))
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Reg = 0x0D;
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Reg, 1, (char*)&Data, 1))
		goto error;	
	
	Reg = 0x06;
	Data = 0x01;
	if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Reg, 1, (char*)&Data, 1))
		goto error;
	
	for (i = 0; i < ConfigSize; i++)
	{
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, (uint8_t*)&Config[i].Addr, 1, (char*)&Config[i].Data, 1))
			goto error;
	}

	Reg = 0x06;
	Data = 0x02;
	if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Reg, 1, (char*)&Data, 1))
		goto error;
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void SI5332_Deinit(SI5332_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
