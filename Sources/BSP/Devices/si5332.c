#include "si5332.h"


int32_t SI5332_Init(SI5332_T *Dev, I2C_TypeDef *I2C, uint8_t Addr)
{
	uint8_t Reg;
	uint8_t Data;
	
	if ((!Dev) || (!I2C) || (Addr > 0x6B) || (Addr < 0x6A))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->I2C = I2C;
	Dev->Addr = Addr << 1;
	
	Reg = 0x0D;
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Reg, 1, (char*)&Data, 1))
		goto error;	
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t SI5332_Config(SI5332_T *Dev, SI5332_CFG_T *Config, uint32_t ConfigSize)
{
	uint32_t i;
	uint8_t Addr;
	uint8_t Data;
	
	if ((!Dev) || (!Config))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Addr = 0x06;
	Data = 0x01;
	if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Data, 1))
		goto error;
	
	for (i = 0; i < ConfigSize; i++)
	{
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Config[i].Addr, 1, (char*)&Config[i].Data, 1))
			goto error;
	}

	Addr = 0x06;
	Data = 0x02;
	if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Data, 1))
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}
