#include "si5332.h"
#include "log.h"

int32_t SI5332_Init(SI5332_T *Dev, const char *DevId, const I2C_TypeDef *I2C, uint8_t Addr, const SI5332_CFG_T *Config, const uint32_t ConfigSize)
{
	uint32_t i;
	uint8_t Reg;
	uint8_t Data;
	
	if ((!Dev) || (!I2C) || (Addr > 0x6B) || (Addr < 0x6A))
		return -1;
	
	BSP_DEV_INIT(Dev, "SI5332", DevId);
	BSP_DEV_LOCK(Dev);
	
	Dev->I2C = I2C;
	Dev->Addr = Addr << 1;
	
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
	
	LOG_Add(Dev->DevName, Dev->DevId, "Init: OK");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	LOG_Add(Dev->DevName, Dev->DevId, "Init: Fail");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

void SI5332_Deinit(SI5332_T *Dev)
{
	if (!Dev)
		return;
	LOG_Add(Dev->DevName, Dev->DevId, "Deinit");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
}
