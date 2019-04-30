#include "max6639.h"

static int32_t MAX6639_ReadDevId(MAX6639_T *Dev)
{
	uint8_t Register;
	uint8_t DevId, ManufId;
	
	Register = 0x3D;
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Register, sizeof(Register), (char*)&DevId, sizeof(DevId)))
		return -1;
	
	Register = 0x3E;
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Register, sizeof(Register), (char*)&ManufId, sizeof(ManufId)))
		return -1;
	
	if ((DevId != 0x58) || (ManufId != 0x4D))
		return -1;
	Dev->DevId = DevId;	
	Dev->ManufId = ManufId;	
	return 0;
}

int32_t MAX6639_ReadChTemp(MAX6639_T *Dev, MAX6639_CH_T Channel)
{
	uint8_t Register;
	uint8_t Temp;
	
	if ((!Dev) || ((Channel != MAX6639_CH_1) && (Channel != MAX6639_CH_2)))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (Channel == MAX6639_CH_1)
		Register = 0x00;
	else
		Register = 0x01;
	
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Register, sizeof(Register), (char*)&Temp, sizeof(Temp)))
		goto error;
	
	Dev->Temp[Channel] = Temp;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t MAX6639_Init(MAX6639_T *Dev, I2C_TypeDef *I2C, uint8_t Addr)
{
	if ((!Dev) || (!I2C) || ((Addr != 0x5E) && (Addr != 0x5C) && (Addr != 0x58)))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->I2C = I2C;
	Dev->Addr = Addr;
	
	if (MAX6639_ReadDevId(Dev))
		goto error;
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

void MAX6639_Deinit(MAX6639_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
