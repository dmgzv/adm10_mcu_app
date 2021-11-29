#include "dbe0125v2.h"
#include <string.h>

#define DBE0125V2_ADDR_STATUS_WORD 0x79
#define DBE0125V2_ADDR_VIN 0x88
#define DBE0125V2_ADDR_VOUT 0x8B
#define DBE0125V2_ADDR_IOUT 0x8C
#define DBE0125V2_ADDR_TEMP(Idx) ((Idx) ? 0x8E : 0x8D)
#define DBE0125V2_ADDR_MFR_ID 0x99
#define DBE0125V2_ADDR_MFR_MODEL 0x9A
#define DBE0125V2_ADDR_MFR_REVISION 0x9B

static int32_t DBE0125V2_GetMfr(DBE0125V2_T *Dev)
{
	uint8_t Data[64];
	uint8_t Register;
	
	Register = DBE0125V2_ADDR_MFR_ID;
	if (PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Register, Data, 1))
		return -1;
	if (Data[0] >= 64)
		return -1;
	Register = DBE0125V2_ADDR_MFR_ID;
	if (PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Register, Data, Data[0] + 1))
		return -1;
	memcpy(Dev->Mfr.Id, &Data[1], Data[0]);
	Dev->Mfr.Id[Data[0]] = 0;
	
	Register = DBE0125V2_ADDR_MFR_MODEL;
	if (PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Register, Data, 1))
		return -1;
	if (Data[0] >= 64)
		return -1;
	Register = DBE0125V2_ADDR_MFR_MODEL;
	if (PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Register, Data, Data[0] + 1))
		return -1;
	memcpy(Dev->Mfr.Model, &Data[1], Data[0]);
	Dev->Mfr.Model[Data[0]] = 0;
	
	Register = DBE0125V2_ADDR_MFR_REVISION;
	if (PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Register, Data, 1))
		return -1;
	if (Data[0] >= 64)
		return -1;
	Register = DBE0125V2_ADDR_MFR_REVISION;
	if (PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Register, Data, Data[0] + 1))
		return -1;
	memcpy(Dev->Mfr.Revision, &Data[1], Data[0]);
	Dev->Mfr.Revision[Data[0]] = 0;
	
	return 0;
}

static int32_t DBE0125V2_GetStatus(DBE0125V2_T *Dev)
{
	uint8_t Data[2];
	uint8_t Register;
	
	Register = DBE0125V2_ADDR_STATUS_WORD;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	Dev->Status.Word = Data[0] + (Data[1] << 8);
	return 0;
}

static int32_t DBE0125V2_GetRead(DBE0125V2_T *Dev)
{
	uint32_t i;
	uint16_t LinearData;
	uint8_t Register;
	uint8_t Data[2];

	Register = DBE0125V2_ADDR_VIN;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	LinearData = Data[0] + (Data[1] << 8);
	Dev->Read.Vin = PMBUS_LinearToFloat(LinearData);

	Register = DBE0125V2_ADDR_VOUT;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	LinearData = Data[0] + (Data[1] << 8);
	Dev->Read.Vout = PMBUS_LinearToFloat(LinearData);

	Register = DBE0125V2_ADDR_IOUT;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	LinearData = Data[0] + (Data[1] << 8);
	Dev->Read.Iout = PMBUS_LinearToFloat(LinearData);

	for (i = 0; i < 2; i++)
	{
		Register = DBE0125V2_ADDR_TEMP(i);
		if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
			return -1;
		LinearData = Data[0] + (Data[1] << 8);
		Dev->Read.Temp[i] = PMBUS_LinearToFloat(LinearData);
	}
	return 0;
}

int32_t DBE0125V2_Init(DBE0125V2_T *Dev)
{	
	if ((!Dev) || (!Dev->I2C))
		return -1;
	
	BSP_DEV_INIT(Dev);

	if (DBE0125V2_GetMfr(Dev))
		goto error;
	if (DBE0125V2_GetStatus(Dev))
		goto error;
	if (DBE0125V2_GetRead(Dev))
		goto error;
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void DBE0125V2_Deinit(DBE0125V2_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}

int32_t DBE0125V2_Update(DBE0125V2_T *Dev)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (DBE0125V2_GetStatus(Dev))
		goto error;
	if (DBE0125V2_GetRead(Dev))
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}


