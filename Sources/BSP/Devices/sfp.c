#include "sfp.h"
#include <math.h>

static int32_t SFP_ReadVendor(SFP_T *Dev)
{
	uint8_t Addr;	
	
	Addr = 20;
	if (I2C_Sequential_Receive(Dev->I2C,0xA0, &Addr, 1, Dev->Vendor, 16))
		return -1;	
	Dev->Vendor[16] = 0;
	return 0;
}

static int32_t SFP_ReadPartNumber(SFP_T *Dev)
{
	uint8_t Addr;	
	
	Addr = 40;
	if (I2C_Sequential_Receive(Dev->I2C,0xA0, &Addr, 1, Dev->PartNumber, 16))
		return -1;	
	Dev->PartNumber[16] = 0;
	return 0;
}

static int32_t SFP_ReadSerialNumber(SFP_T *Dev)
{
	uint8_t Addr;	
	
	Addr = 68;
	if (I2C_Sequential_Receive(Dev->I2C,0xA0, &Addr, 1, Dev->SerialNumber, 16))
		return -1;	
	Dev->SerialNumber[16] = 0;
	return 0;
}

static int32_t SFP_ReadTempThrs(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[4][2];
	int16_t val;
	
	Addr = 00;
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[0][0], 2))
		return -1;
	Addr = 02;
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[1][0], 2))
		return -1;
	Addr = 04;
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[2][0], 2))
		return -1;
	Addr = 06;	
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[3][0], 2))
		return -1;	
	
	val = (tmp[0][0] << 8) | tmp[0][1];
	Dev->TempThrs[SFP_THR_CMAX] = val / 256.0;
	val = (tmp[1][0] << 8) | tmp[1][1];
	Dev->TempThrs[SFP_THR_CMIN] = val / 256.0;
	val = (tmp[2][0] << 8) | tmp[2][1];
	Dev->TempThrs[SFP_THR_WMAX] = val / 256.0;
	val = (tmp[3][0] << 8) | tmp[3][1];
	Dev->TempThrs[SFP_THR_WMIN] = val / 256.0;
	
	return 0;
}

static int32_t SFP_ReadRxPwrThrs(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[4][2];
	uint16_t val;
	
	Addr = 32;
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[0][0], 2))
		return -1;
	Addr = 34;
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[1][0], 2))
		return -1;
	Addr = 36;
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[2][0], 2))
		return -1;
	Addr = 38;	
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[3][0], 2))
		return -1;
	
	val = (tmp[0][0] << 8) | tmp[0][1];
	Dev->RxPwrThrs[SFP_THR_CMAX] = 10 * log10(val * 0.0001);
	val = (tmp[1][0] << 8) | tmp[1][1];
	Dev->RxPwrThrs[SFP_THR_CMIN] = 10 * log10(val * 0.0001);
	val = (tmp[2][0] << 8) | tmp[2][1];
	Dev->RxPwrThrs[SFP_THR_WMAX] = 10 * log10(val * 0.0001);
	val = (tmp[3][0] << 8) | tmp[3][1];
	Dev->RxPwrThrs[SFP_THR_WMIN] = 10 * log10(val * 0.0001);
	return 0;
}

static int32_t SFP_ReadTxPwrThrs(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[4][2];
	uint16_t val;
	
	Addr = 24;
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[0][0], 2))
		return -1;
	Addr = 26;
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[1][0], 2))
		return -1;
	Addr = 28;
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[2][0], 2))
		return -1;
	Addr = 30;	
	if (I2C_Sequential_Receive(Dev->I2C,0xA2, &Addr, 1, (char*)&tmp[3][0], 2))
		return -1;	
	
	val = (tmp[0][0] << 8) | tmp[0][1];
	Dev->TxPwrThrs[SFP_THR_CMAX] = 10 * log10(val * 0.0001);
	val = (tmp[1][0] << 8) | tmp[1][1];
	Dev->TxPwrThrs[SFP_THR_CMIN] = 10 * log10(val * 0.0001);
	val = (tmp[2][0] << 8) | tmp[2][1];
	Dev->TxPwrThrs[SFP_THR_WMAX] = 10 * log10(val * 0.0001);
	val = (tmp[3][0] << 8) | tmp[3][1];
	Dev->TxPwrThrs[SFP_THR_WMIN] = 10 * log10(val * 0.0001);
	return 0;
}

static int32_t SFP_ReadTemp(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[2];
	int16_t val;
	
	Addr = 96;
	if (I2C_Sequential_Receive(Dev->I2C, 0xA2, &Addr, 1, (char*)&tmp[0], 2))
		return -1;
	
	val = (tmp[0] << 8) | tmp[1];
	Dev->Temp = val / 256.0;
	return 0;
}

static int32_t SFP_ReadRxPwr(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[2];
	uint16_t val;
	
	Addr = 104;
	if (I2C_Sequential_Receive(Dev->I2C, 0xA2, &Addr, 1, (char*)&tmp[0], 2))
		return -1;
	
	val = (tmp[0] << 8) | tmp[1];
	if (val == 0)
		Dev->RxPwr = -40;
	else
		Dev->RxPwr = 10 * log10(val * 0.0001);
	return 0;
}

static int32_t SFP_ReadTxPwr(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[2];
	uint16_t val;
	
	Addr = 102;
	if (I2C_Sequential_Receive(Dev->I2C, 0xA2, &Addr, 1, (char*)&tmp[0], 2))
		return -1;

	val = (tmp[0] << 8) | tmp[1];
	if (val == 0)
		Dev->TxPwr = -40;
	else
		Dev->TxPwr = 10 * log10(val * 0.0001);
	return 0;
}

int32_t SFP_Update(SFP_T *Dev)
{
	if ((!Dev) || (!Dev->I2C))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (SFP_ReadTemp(Dev))
		goto error;
	if (SFP_ReadRxPwr(Dev))
		goto error;
	if (SFP_ReadTxPwr(Dev))
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t SFP_Init(SFP_T *Dev, I2C_TypeDef *I2C)
{
	
	if ((!Dev) || (!I2C))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->I2C = I2C;
	
	if (SFP_ReadVendor(Dev))
		goto error;
	if (SFP_ReadPartNumber(Dev))
		goto error;
	if (SFP_ReadSerialNumber(Dev))
		goto error;
	if (SFP_ReadTempThrs(Dev))
		goto error;
	if (SFP_ReadRxPwrThrs(Dev))
		goto error;
	if (SFP_ReadTxPwrThrs(Dev))
		goto error;
	if (SFP_ReadTemp(Dev))
		goto error;
	if (SFP_ReadRxPwr(Dev))
		goto error;
	if (SFP_ReadTxPwr(Dev))
		goto error;
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}
void SFP_Deinit(SFP_T *Dev)
{
	if (!Dev)
		return;
	Dev->I2C = 0;
	BSP_DEV_DEINIT(Dev);
}
