#include "ip1554.h"

int32_t IP1554_DBE0125V2_Process(IP1554_T *Dev)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(&Dev->dbe0125v2) != BSP_DEV_STATE_OK)
	{
		DBE0125V2_Init(&Dev->dbe0125v2);
	}
	else
	{
		DBE0125V2_Update(&Dev->dbe0125v2);
	}
	return 0;
}

int32_t IP1554_MAX7306_Process(IP1554_T *Dev, uint8_t ChassisAddress)
{
	uint32_t i;
	uint8_t Pin;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(&Dev->max7306) != BSP_DEV_STATE_OK)
	{
		MAX7306_Init(&Dev->max7306);
	}
	else
	{
		for (i = 0; i < MAX7306_PINS_CNT; i++)
		{
			if (ChassisAddress & (1 << i))
				Pin = 0x7F;
			else
				Pin = 0x40;
			MAX7306_WritePin(&Dev->max7306, i, Pin);
		}
	}
	return 0;
}

int32_t IP1554_Process(IP1554_T *Dev, uint8_t ChassisAddress)
{
	if (!Dev)
		return -1;
	
	IP1554_MAX7306_Process(Dev, ChassisAddress);
	IP1554_DBE0125V2_Process(Dev);
	
	return 0;
}

void IP1554_Deinit(IP1554_T *Dev)
{
	if (!Dev)
		return;
	MAX7306_Deinit(&Dev->max7306);
	DBE0125V2_Deinit(&Dev->dbe0125v2);
}

int32_t IP1554_GetVin(IP1554_T *Dev, float *Vin)
{
	if ((!Dev) || (!Vin))
		return -1;
	if (BSP_DEV_GET_STATE(&Dev->dbe0125v2) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(&Dev->dbe0125v2);
	*Vin = Dev->dbe0125v2.Read.Vin;
	BSP_DEV_UNLOCK(&Dev->dbe0125v2);
	
	return 0;
}

int32_t IP1554_GetVout(IP1554_T *Dev, float *Vout)
{
	if ((!Dev) || (!Vout))
		return -1;
	if (BSP_DEV_GET_STATE(&Dev->dbe0125v2) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(&Dev->dbe0125v2);
	*Vout = Dev->dbe0125v2.Read.Vout;
	BSP_DEV_UNLOCK(&Dev->dbe0125v2);
	
	return 0;
}

int32_t IP1554_GetIout(IP1554_T *Dev, float *Iout)
{
	if ((!Dev) || (!Iout))
		return -1;
	if (BSP_DEV_GET_STATE(&Dev->dbe0125v2) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(&Dev->dbe0125v2);
	*Iout = Dev->dbe0125v2.Read.Iout;
	BSP_DEV_UNLOCK(&Dev->dbe0125v2);
	
	return 0;
}

int32_t IP1554_GetTemp(IP1554_T *Dev, float *Temp)
{
	if ((!Dev) || (!Temp))
		return -1;
	if (BSP_DEV_GET_STATE(&Dev->dbe0125v2) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(&Dev->dbe0125v2);
	*Temp = Dev->dbe0125v2.Read.Temp;
	BSP_DEV_UNLOCK(&Dev->dbe0125v2);
	
	return 0;
}
