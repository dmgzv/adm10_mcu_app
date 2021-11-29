#include "ps.h"
#include "tiny-json.h"
#include <string.h>
#include <stdio.h>

#define PS_PT_NUMBER_JSON_KEY	"pt"
#define PS_HW_NUMBER_JSON_KEY	"hw"
#define PS_SR_NUMBER_JSON_KEY	"sr"

static int32_t __PS_WriteInventory(PS_T *Dev, char *PtNumber, char *HwNumber, char *SrNumber)
{
	char Buffer[AT24C02C_SIZE];
	uint32_t BufferLen;
	
	BufferLen = 0;
	Buffer[BufferLen++] = '{';
	BufferLen += sprintf(&Buffer[BufferLen], "\""PS_PT_NUMBER_JSON_KEY"\":\"%s\",", PtNumber);
	BufferLen += sprintf(&Buffer[BufferLen], "\""PS_HW_NUMBER_JSON_KEY"\":\"%s\"", HwNumber);
	BufferLen += sprintf(&Buffer[BufferLen], "\""PS_SR_NUMBER_JSON_KEY"\":\"%s\"", SrNumber);
	Buffer[BufferLen++] = '}';
	Buffer[BufferLen++] = 0;
	
	if (AT24C02C_Write(&Dev->at24c02c, 0, Buffer, BufferLen))
		goto error;
	
	strcpy(Dev->PtNumber, PtNumber);
	strcpy(Dev->HwNumber, HwNumber);
	strcpy(Dev->SrNumber, SrNumber);
	LOG_Info((Dev)->DevName, "", "InventoryWrite", "%s;%s;%s", HwNumber, HwNumber, SrNumber);
	
	return 0;

error:
	LOG_Info((Dev)->DevName, "", "InventoryWriteError", "%s;%s;%s", HwNumber, HwNumber, SrNumber);
	return -1;
}

int32_t PS_Init(PS_T *Dev)
{
	json_t jsonPool[16];
	json_t const* json;
	json_t const* jsonProp;
	char Buffer[AT24C02C_SIZE];
	char *c;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	if (AT24C02C_Init(&Dev->at24c02c))
		goto error;
	
	if (AT24C02C_Read(&Dev->at24c02c, 0, Buffer, sizeof(Buffer)))
		goto error;
	
	Dev->PtNumber[0] = 0;
	Dev->HwNumber[0] = 0;
	Dev->SrNumber[0] = 0;
	Dev->__UpdateFlag = 0;
	
	json = json_create( Buffer, jsonPool, sizeof (jsonPool) / sizeof (jsonPool[0]) );
	if ( json )
	{
		jsonProp = json_getProperty( json, PS_PT_NUMBER_JSON_KEY );
		if ((jsonProp) && (JSON_TEXT == json_getType( jsonProp )))
		{
			c = (char*)json_getValue( jsonProp );
			if (strlen(c) < sizeof(Dev->PtNumber))
			{
				strcpy(Dev->PtNumber, c);
			}
			else
			{
				LOG_Info((Dev)->DevName, "", "InventoryParseError", "Invalid length of PtNumber");
			}
		}

		jsonProp = json_getProperty( json, PS_HW_NUMBER_JSON_KEY );
		if ((jsonProp) && (JSON_TEXT == json_getType( jsonProp )))
		{
			c = (char*)json_getValue( jsonProp );
			if (strlen(c) < sizeof(Dev->HwNumber))
			{
				strcpy(Dev->HwNumber, c);
			}
			else
			{
				LOG_Info((Dev)->DevName, "", "InventoryParseError", "Invalid length of HwNumber");
			}
		}
		
		jsonProp = json_getProperty( json, PS_SR_NUMBER_JSON_KEY );
		if ((jsonProp) && (JSON_TEXT == json_getType( jsonProp )))
		{
			c = (char*)json_getValue( jsonProp );
			if (strlen(c) < sizeof(Dev->SrNumber))
			{
				strcpy(Dev->SrNumber, c);
			}
			else
			{
				LOG_Info((Dev)->DevName, "", "InventoryParseError", "Invalid length of SrNumber");
			}
		}
	}
	else
	{
		LOG_Info((Dev)->DevName, "", "InventoryParseError", "Invalind JSON data");
	}
	
	LOG_Info((Dev)->DevName, "", "InventoryInit", "%s;%s;%s", Dev->HwNumber, Dev->HwNumber, Dev->SrNumber);
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void PS_Deinit(PS_T *Dev)
{
	if (!Dev)
		return;
	IP1554_Deinit(&Dev->ip1554);
	AT24C02C_Deinit(&Dev->at24c02c);
	BSP_DEV_DEINIT(Dev);
}

int32_t PS_Update(PS_T *Dev)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->__UpdateFlag)
	{
		if (__PS_WriteInventory(Dev, Dev->PtNumber, Dev->HwNumber, Dev->SrNumber))
			goto error;
		Dev->__UpdateFlag = 0;
	}
	
	if ((!strcmp(Dev->PtNumber, PS_PT_NUMBER)) && (!strcmp(Dev->PtNumber, IP1554_HW_NUMBER)))
	{
		IP1554_Process(&Dev->ip1554, Dev->ChassisAddress);
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return 0;
}

int32_t PS_ConfigChassisAddress(PS_T *Dev, uint8_t ChassisAddress)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_LOCK(Dev);
	
	Dev->ChassisAddress = ChassisAddress;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t PS_ConfigInventory(PS_T *Dev, char *PtNumber, char *HwNumber, char *SrNumber)
{
	
	if ((!Dev) || (strlen(PtNumber) >= sizeof(Dev->PtNumber)) || (strlen(HwNumber) >= sizeof(Dev->HwNumber)) || (strlen(SrNumber) >= sizeof(Dev->SrNumber)))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_LOCK(Dev);
	
	if (strcmp(Dev->PtNumber, PtNumber))
	{
		strcpy(Dev->PtNumber, PtNumber);
		Dev->__UpdateFlag = 1;
	}
	if (strcmp(Dev->HwNumber, HwNumber))
	{
		strcpy(Dev->HwNumber, HwNumber);
		Dev->__UpdateFlag = 1;
	}
	if (strcmp(Dev->SrNumber, SrNumber))
		
	{
		strcpy(Dev->SrNumber, SrNumber);
		Dev->__UpdateFlag = 1;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t PS_GetVin(PS_T *Dev, float *Vin)
{
	if ((!Dev) || (!Vin))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_LOCK(Dev);
	
	*Vin = 0.0f;
	if ((!strcmp(Dev->PtNumber, PS_PT_NUMBER)) && (!strcmp(Dev->PtNumber, IP1554_HW_NUMBER)))
	{
		if (IP1554_GetVin(&Dev->ip1554, Vin))
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PS_GetVout(PS_T *Dev, float *Vout)
{
	if ((!Dev) || (!Vout))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_LOCK(Dev);
	
	*Vout = 0.0f;
	if ((!strcmp(Dev->PtNumber, PS_PT_NUMBER)) && (!strcmp(Dev->PtNumber, IP1554_HW_NUMBER)))
	{
		if (IP1554_GetVout(&Dev->ip1554, Vout))
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PS_GetIout(PS_T *Dev, float *Iout)
{
	if ((!Dev) || (!Iout))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_LOCK(Dev);
	
	*Iout = 0.0f;
	if ((!strcmp(Dev->PtNumber, PS_PT_NUMBER)) && (!strcmp(Dev->PtNumber, IP1554_HW_NUMBER)))
	{
		if (IP1554_GetIout(&Dev->ip1554, Iout))
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PS_GetTemp(PS_T *Dev, float *Temp, uint32_t Idx)
{
	if ((!Dev) || (!Temp))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_LOCK(Dev);
	
	*Temp = 0.0f;
	if ((!strcmp(Dev->PtNumber, PS_PT_NUMBER)) && (!strcmp(Dev->PtNumber, IP1554_HW_NUMBER)))
	{
		if (IP1554_GetTemp(&Dev->ip1554, Temp, Idx))
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}
