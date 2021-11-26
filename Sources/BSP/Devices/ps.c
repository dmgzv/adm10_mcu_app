#include "ps.h"
#include "tiny-json.h"
#include <string.h>
#include <stdio.h>

#define PS_PT_NUMBER_JSON_KEY	"pt"
#define PS_HW_NUMBER_JSON_KEY	"hw"

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

		jsonProp = json_getProperty( json, "hw" );
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
	}
	else
	{
		LOG_Info((Dev)->DevName, "", "InventoryParseError", "Invalind JSON data");
	}
	
	LOG_Info((Dev)->DevName, "", "InventoryInit", "%s;%s", Dev->HwNumber, Dev->HwNumber);
	
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
	
	if ((!strcmp(Dev->PtNumber, "PS")) && (!strcmp(Dev->PtNumber, "IP1554")))
	{
		IP1554_Process(&Dev->ip1554, Dev->ChassisAddress);
	}
	
	BSP_DEV_UPDATE_OK(Dev);
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

int32_t PS_WriteInventory(PS_T *Dev, char *PtNumber, char *HwNumber)
{
	char Buffer[AT24C02C_SIZE];
	uint32_t BufferLen;
	
	if ((!Dev) || (strlen(PtNumber) >= sizeof(Dev->PtNumber))|| (strlen(HwNumber) >= sizeof(Dev->HwNumber)))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_UPDATE(Dev);
	
	BufferLen = 0;
	Buffer[BufferLen++] = '{';
	BufferLen += sprintf(&Buffer[BufferLen], "\""PS_PT_NUMBER_JSON_KEY"\":\"%s\",", PtNumber);
	BufferLen += sprintf(&Buffer[BufferLen], "\""PS_HW_NUMBER_JSON_KEY"\":\"%s\"", HwNumber);
	Buffer[BufferLen++] = '}';
	Buffer[BufferLen++] = 0;
	
	if (AT24C02C_Write(&Dev->at24c02c, 0, Buffer, BufferLen))
		goto error;
	
	strcpy(Dev->PtNumber, PtNumber);
	strcpy(Dev->HwNumber, HwNumber);
	LOG_Info((Dev)->DevName, "", "InventoryWrite", "%s;%s", HwNumber, HwNumber);
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;

error:
	LOG_Info((Dev)->DevName, "", "InventoryWriteError", "%s;%s", HwNumber, HwNumber);
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}
