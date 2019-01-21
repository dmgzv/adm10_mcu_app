#include "gdt080a.h"

#define GDT080A_ADDR_VOUT_SCALE_MONITOR 0x2A
#define GDT080A_ADDR_VOUT_TRIM 0x22
#define GDT080A_ADDR_VOUT_MARGIN_HIGH	0x25
#define GDT080A_ADDR_VOUT_MARGIN_LOW	0x26
#define GDT080A_ADDR_STATUS_BYTE 0x78
#define GDT080A_ADDR_STATUS_WORD 0x79
#define GDT080A_ADDR_VIN 0x88
#define GDT080A_ADDR_VOUT 0x8B
#define GDT080A_ADDR_IOUT 0x8C
#define GDT080A_ADDR_TEMP_1 0x8D
#define GDT080A_ADDR_TEMP_2 0x8E
#define GDT080A_ADDR_MFR_SPECIFIC_00 0xD0

static int32_t GDT080A_GetInfo(GDT080A_T *Dev)
{
	uint8_t Data[2];
	uint8_t Register;
	
	Register = GDT080A_ADDR_MFR_SPECIFIC_00;
	if (PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Register, Data, 3))
		return -1;
	Dev->Info.Id = Data[0] >> 2;
	Dev->Info.Revision = Data[1] >> 3;
	return 0;
}

static int32_t GDT080A_GetStatus(GDT080A_T *Dev)
{
	uint8_t Data[2];
	uint8_t Register;
	
	Register = GDT080A_ADDR_STATUS_WORD;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	Dev->Status = Data[0] + (Data[1] << 8);
	return 0;
}

static int32_t GDT080A_GetVin(GDT080A_T *Dev)
{
	uint16_t LinearData;
	uint8_t Register;
	uint8_t Data[2];

	Register = GDT080A_ADDR_VIN;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	LinearData = Data[0] + (Data[1] << 8);
	Dev->Mon.Vin = PMBUS_LinearToFloat(LinearData);
	return 0;
}

static int32_t GDT080A_GetVout(GDT080A_T *Dev)
{
	uint16_t LinearData;
	uint8_t Register;
	uint8_t Data[2];

	Register = GDT080A_ADDR_VOUT;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	LinearData = Data[0] + (Data[1] << 8);
	/* 
		Correct value of VOUT_SCALE_MONITOR should be set, to get scaled Vout. 
		But GDT080A becomes unstable if correct VOUT_SCALE_MONITOR is set.
		GDT080A works fine if VOUT_SCALE_MONITOR = 1 is set. 
		Scale Vout manualy.
	*/
	Dev->Mon.Vout = (LinearData / 8192.0 ) / Dev->Cfg.Vout.Scale;
	return 0;
}

static int32_t GDT080A_GetIout(GDT080A_T *Dev)
{
	uint16_t LinearData;
	uint8_t Register;
	uint8_t Data[2];

	Register = GDT080A_ADDR_IOUT;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	LinearData = Data[0] + (Data[1] << 8);
	Dev->Mon.Iout = PMBUS_LinearToFloat(LinearData);
	return 0;
}

static int32_t GDT080A_GetTemp(GDT080A_T *Dev)
{
	uint16_t LinearData;
	uint8_t Register;
	uint8_t Data[2];

	Register = GDT080A_ADDR_TEMP_1;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	LinearData = Data[0] + (Data[1] << 8);
	Dev->Mon.Temp1 = PMBUS_LinearToFloat(LinearData);
	
	Register = GDT080A_ADDR_TEMP_2;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	LinearData = Data[0] + (Data[1] << 8);
	Dev->Mon.Temp2 = PMBUS_LinearToFloat(LinearData);
	return 0;
}

static int32_t GDT080A_GetVoutMargins(GDT080A_T *Dev)
{
	uint16_t LinearData;
	uint8_t Register;
	uint8_t Data[2];

	Register = GDT080A_ADDR_VOUT_MARGIN_HIGH;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	LinearData = Data[0] + (Data[1] << 8);
	Dev->Cfg.Vout.MarginHigh = (LinearData / 8192.0) / Dev->Cfg.Vout.Scale;
	
	Register = GDT080A_ADDR_VOUT_MARGIN_LOW;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	LinearData = Data[0] + (Data[1] << 8);
	Dev->Cfg.Vout.MarginLow = (LinearData / 8192.0) / Dev->Cfg.Vout.Scale;
	return 0;
}

static int32_t GDT080A_GetVoutScale(GDT080A_T *Dev)
{
	uint16_t LinearData;
	uint8_t Register;
	uint8_t Data[2];

	Register = GDT080A_ADDR_VOUT_SCALE_MONITOR;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	LinearData = Data[0] + (Data[1] << 8);
	Dev->Cfg.Vout.Scale = PMBUS_LinearToFloat(LinearData);
	return 0;
}

static int32_t GDT080A_GetVoutTrim(GDT080A_T *Dev)
{
	uint8_t Register;
	uint8_t Data[2];
	
	Register = GDT080A_ADDR_VOUT_TRIM;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	Dev->Cfg.Vout.Trim = (Data[0] + (Data[1] << 8)) / 8192.0;
	return 0;
}

static int32_t GDT080A_SetVoutScale(GDT080A_T *Dev, float Vout)
{
	int16_t LinearData;
	uint8_t Register;
	uint8_t Data[2];
	
	/*
		GDT080A becomes unstable if correct VOUT_SCALE_MONITOR is set.
		GDT080A works fine if VOUT_SCALE_MONITOR = 1.
	*/
	LinearData = PMBUS_FloatToLinear( 1.0 /*Vout / GDT080A_VREF*/);
	Data[0] = LinearData & 0xFF;
	Data[1] = (LinearData >> 8) & 0xFF;
	Register = GDT080A_ADDR_VOUT_SCALE_MONITOR;
	if (PMBUS_WriteWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	Dev->Cfg.Vout.Scale = GDT080A_VREF / Vout;
	Dev->Cfg.Vout.Value = (GDT080A_VREF + Dev->Cfg.Vout.Trim) / Dev->Cfg.Vout.Scale;
	return 0;
}

static int32_t GDT080A_SetVoutTrim(GDT080A_T *Dev, float Trim)
{
	int16_t LinearData;
	uint8_t Register;
	uint8_t Data[2];
	
	LinearData = Trim * 8192.0f;
	Data[0] = LinearData & 0xFF;
	Data[1] = (LinearData >> 8) & 0xFF;
	Register = GDT080A_ADDR_VOUT_TRIM;
	if (PMBUS_WriteWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	Dev->Cfg.Vout.Trim = Trim;
	Dev->Cfg.Vout.Value = (GDT080A_VREF + Dev->Cfg.Vout.Trim) /  Dev->Cfg.Vout.Scale;
	return 0;
}

int32_t GDT080A_SetVout(GDT080A_T *Dev, float Vout)
{
	float Trim;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Trim = Vout * Dev->Cfg.Vout.Scale - GDT080A_VREF;
	if (GDT080A_SetVoutTrim(Dev, Trim))
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t GDT080A_Init(GDT080A_T *Dev, I2C_TypeDef *I2C, uint8_t Addr, float Vout)
{	
	if ((!Dev) || (!I2C))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->I2C = I2C;
	Dev->Addr = Addr << 1;

	if (GDT080A_GetInfo(Dev))
		goto error;
	if (GDT080A_GetStatus(Dev))
		goto error;
	if (GDT080A_GetVoutScale(Dev))
		goto error;
	if (GDT080A_SetVoutScale(Dev, Vout))
		goto error;
	if (GDT080A_GetVoutTrim(Dev))
		goto error;
	if (GDT080A_SetVoutTrim(Dev, 0))
		goto error;
	if (GDT080A_GetVoutMargins(Dev))
		goto error;
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t GDT080A_Update(GDT080A_T *Dev)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (GDT080A_GetStatus(Dev))
		goto error;
	if (GDT080A_GetVin(Dev))
		goto error;
	if (GDT080A_GetVout(Dev))
		goto error;
	if (GDT080A_GetIout(Dev))
		return -1;
	if (GDT080A_GetTemp(Dev))
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}


