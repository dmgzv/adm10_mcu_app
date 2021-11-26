#include "at24c02c.h"

int32_t AT24C02C_Write(AT24C02C_T *Dev, uint8_t Addr, char *Buffer, uint32_t Size)
{
	if ((!Dev) || (!Buffer))
		return -1;
	if ((Addr + Size) > AT24C02C_SIZE)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_UPDATE(Dev);
	
	if (!I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Addr, sizeof(Addr), Buffer, Size))
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;

error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t AT24C02C_Read(AT24C02C_T *Dev, uint8_t Addr, char *Buffer, uint32_t Size)
{
	if ((!Dev) || (!Buffer))
		return -1;
	if ((Addr + Size) > AT24C02C_SIZE)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_UPDATE(Dev);
	
	if (!I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, sizeof(Addr), Buffer, Size))
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t AT24C02C_Init(AT24C02C_T *Dev)
{
	uint8_t Addr;
	char Buffer[8];
	
	if ((!Dev) || (!Dev->I2C) || ((Dev->Addr != 0xA0) && (Dev->Addr != 0xA8)))
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Addr = 0;
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr , &Addr, sizeof(Addr), Buffer, sizeof(Buffer)))
		goto error;
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void AT24C02C_Deinit(AT24C02C_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
