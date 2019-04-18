#include "dim3r3400.h"

#define DIM3R3400_REG_STATUS	0x1E
#define DIM3R3400_REG_VOUT		0x1F
#define DIM3R3400_REG_IOUT		0x21
#define DIM3R3400_REG_VIN_1		0x22
#define DIM3R3400_REG_VIN_2		0x23
#define DIM3R3400_REG_TEMP		0x28

static int32_t DIM3R3400_ReadReg(DIM3R3400_T *Dev, uint8_t Reg, uint8_t *Data)
{
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Reg, sizeof(Reg), (char*)Data, sizeof(*Data)))
		return -1;
	return 0;
}

static int32_t __DIM3R3400_Update(DIM3R3400_T *Dev)
{
	uint8_t Data;

	if (DIM3R3400_ReadReg(Dev, DIM3R3400_REG_STATUS, &Data))
		return -1;
	Dev->Status = Data;

	if (DIM3R3400_ReadReg(Dev, DIM3R3400_REG_VOUT, &Data))
		return -1;
	Dev->Vout = Data * 0.398;

	if (DIM3R3400_ReadReg(Dev, DIM3R3400_REG_IOUT, &Data))
		return -1;
	Dev->Iout = Data * 0.094;

	if (DIM3R3400_ReadReg(Dev, DIM3R3400_REG_VIN_1, &Data))
		return -1;
	Dev->Vin[0] = Data * 0.325;

	if (DIM3R3400_ReadReg(Dev, DIM3R3400_REG_VIN_2, &Data))
		return -1;
	Dev->Vin[1] = Data * 0.325;

	if (DIM3R3400_ReadReg(Dev, DIM3R3400_REG_TEMP, &Data))
		return -1;
	Dev->Temp = Data;
	
	return 0;
}

int32_t DIM3R3400_Init(DIM3R3400_T *Dev, I2C_TypeDef *I2C, uint8_t Addr)
{
	if ((!Dev) || (!I2C) || (Addr < 0x28) || (Addr > 0x2F))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->I2C = I2C;
	Dev->Addr = Addr << 1;
	
	if (__DIM3R3400_Update(Dev))
		goto error;	
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t DIM3R3400_Update(DIM3R3400_T *Dev)
{
	if ((!Dev))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__DIM3R3400_Update(Dev))
		goto error;	

	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}
