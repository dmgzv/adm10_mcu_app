#include "dbe0125v2.h"
#include "sort.h"
#include <string.h>

#define DBE0125V2_ADDR_STATUS_WORD 0x79
#define DBE0125V2_ADDR_VIN 0x88
#define DBE0125V2_ADDR_VOUT 0x8B
#define DBE0125V2_ADDR_IOUT 0x8C
#define DBE0125V2_ADDR_TEMP 0x8D
#define DBE0125V2_ADDR_MFR_ID 0x99
#define DBE0125V2_ADDR_MFR_MODEL 0x9A
#define DBE0125V2_ADDR_MFR_REVISION 0x9B

/*static int32_t DBE0125V2_GetMfr(DBE0125V2_T *Dev)
{
	uint8_t Register;
	
	Register = DBE0125V2_ADDR_MFR_ID;
	if (PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Register, (uint8_t*)Dev->Mfr.Id, sizeof(Dev->Mfr.Id) - 1))
		return -1;
	
	Register = DBE0125V2_ADDR_MFR_MODEL;
	if (PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Register, (uint8_t*)Dev->Mfr.Model, sizeof(Dev->Mfr.Model) - 1))
		return -1;
	
	Register = DBE0125V2_ADDR_MFR_REVISION;
	if (PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Register, (uint8_t*)Dev->Mfr.Revision, sizeof(Dev->Mfr.Revision) - 1))
		return -1;
	
	return 0;
}*/

static int32_t DBE0125V2_GetStatus(DBE0125V2_T *Dev)
{
	uint8_t Data[2];
	uint8_t Register;
	
	Register = DBE0125V2_ADDR_STATUS_WORD;
	if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
		return -1;
	if (!((Data[0] == 0xFF) && (Data[1] == 0xFF)))
	{
		Dev->Status.Word = Data[0] + (Data[1] << 8);
	}
	else
	{
		return -1;
	}
	return 0;
}

static int32_t DBE0125V2_GetRead(DBE0125V2_T *Dev)
{
	uint32_t i;
	uint16_t LinearData;
	float FloatData[3];
	uint8_t Register;
	uint8_t Data[2];

	for (i = 0; i < sizeof(FloatData) / sizeof(FloatData[0]); i++)
	{
		Register = DBE0125V2_ADDR_VIN;
		if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
			return -1;
		if (!((Data[0] == 0xFF) && (Data[1] == 0xFF)))
		{
			LinearData = Data[0] + (Data[1] << 8);
			FloatData[i] = PMBUS_LinearToFloat(LinearData);
		}
		else
		{
			return -1;
		}
	}
	bubble_sort_float(FloatData, sizeof(FloatData) / sizeof(FloatData[0]));
	Dev->Read.Vin =  FloatData[sizeof(FloatData) / sizeof(FloatData[0]) / 2];

	for (i = 0; i < sizeof(FloatData) / sizeof(FloatData[0]); i++)
	{
		Register = DBE0125V2_ADDR_VOUT;
		if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
			return -1;
		if (!((Data[0] == 0xFF) && (Data[1] == 0xFF)))
		{
			LinearData = Data[0] + (Data[1] << 8);
			FloatData[i] = LinearData * 0.00195f; /*LinearData * 2 ^ (-9)*/
		}
		else
		{
			return -1;
		}
	}
	bubble_sort_float(FloatData, sizeof(FloatData) / sizeof(FloatData[0]));
	Dev->Read.Vout =  FloatData[sizeof(FloatData) / sizeof(FloatData[0]) / 2];

	for (i = 0; i < sizeof(FloatData) / sizeof(FloatData[0]); i++)
	{
		Register = DBE0125V2_ADDR_IOUT;
		if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
			return -1;
		if (!((Data[0] == 0xFF) && (Data[1] == 0xFF)))
		{
			LinearData = Data[0] + (Data[1] << 8);
			FloatData[i] = PMBUS_LinearToFloat(LinearData);
		}
		else
		{
			return -1;
		}
	}
	bubble_sort_float(FloatData, sizeof(FloatData) / sizeof(FloatData[0]));
	Dev->Read.Iout =  FloatData[sizeof(FloatData) / sizeof(FloatData[0]) / 2];

	for (i = 0; i < sizeof(FloatData) / sizeof(FloatData[0]); i++)
	{
		Register = DBE0125V2_ADDR_TEMP;
		if (PMBUS_ReadWord(Dev->I2C, Dev->Addr, Register, Data))
			return -1;
		if (!((Data[0] == 0xFF) && (Data[1] == 0xFF)))
		{
			LinearData = Data[0] + (Data[1] << 8);
			FloatData[i] = PMBUS_LinearToFloat(LinearData);
		}
		else
		{
			return -1;
		}
	}
	bubble_sort_float(FloatData, sizeof(FloatData) / sizeof(FloatData[0]));
	Dev->Read.Temp =  FloatData[sizeof(FloatData) / sizeof(FloatData[0]) / 2];

	return 0;
}

int32_t DBE0125V2_Init(DBE0125V2_T *Dev)
{	
	if ((!Dev) || (!Dev->I2C))
		return -1;
	
	BSP_DEV_INIT(Dev);

	//if (DBE0125V2_GetMfr(Dev))
	//	goto error;
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


