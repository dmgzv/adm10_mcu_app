#include "seeprom.h"
#include "tiny-json.h"
#include <string.h>
#include <stdio.h>

#define SEEPROM_PT_NUMBER_JSON_KEY	"pt"
#define SEEPROM_HW_NUMBER_JSON_KEY	"hw"
#define SEEPROM_SR_NUMBER_JSON_KEY	"sr"

static int32_t __SEEPROM_WriteInventory(SEEPROM_T *Dev, char *PtNumber, char *HwNumber, char *SrNumber)
{
	char Buffer[AT24C02C_SIZE];
	uint32_t BufferLen;
	
	BufferLen = 0;
	Buffer[BufferLen++] = '{';
	BufferLen += sprintf(&Buffer[BufferLen], "\""SEEPROM_PT_NUMBER_JSON_KEY"\":\"%s\",", PtNumber);
	BufferLen += sprintf(&Buffer[BufferLen], "\""SEEPROM_HW_NUMBER_JSON_KEY"\":\"%s\"", HwNumber);
	BufferLen += sprintf(&Buffer[BufferLen], "\""SEEPROM_SR_NUMBER_JSON_KEY"\":\"%s\"", SrNumber);
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

int32_t SEEPROM_Init(SEEPROM_T *Dev)
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
		jsonProp = json_getProperty( json, SEEPROM_PT_NUMBER_JSON_KEY );
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

		jsonProp = json_getProperty( json, SEEPROM_HW_NUMBER_JSON_KEY );
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
		
		jsonProp = json_getProperty( json, SEEPROM_SR_NUMBER_JSON_KEY );
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

void SEEPROM_Deinit(SEEPROM_T *Dev)
{
	if (!Dev)
		return;
	IP1562_Deinit(&Dev->ip1562);
	AT24C02C_Deinit(&Dev->at24c02c);
	BSP_DEV_DEINIT(Dev);
}

int32_t SEEPROM_Update(SEEPROM_T *Dev)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->__UpdateFlag)
	{
		if (__SEEPROM_WriteInventory(Dev, Dev->PtNumber, Dev->HwNumber, Dev->SrNumber))
			goto error;
		Dev->__UpdateFlag = 0;
	}
	
	if ((!strcmp(Dev->PtNumber, SEEPROM_PT_NUMBER)) && (!strcmp(Dev->HwNumber, IP1562_HW_NUMBER)))
	{
		IP1562_Process(&Dev->ip1562);
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return 0;
}

int32_t SEEPROM_ConfigInventory(SEEPROM_T *Dev, char *PtNumber, char *HwNumber, char *SrNumber)
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

uint32_t SEEPROM_GetPSPresent(SEEPROM_T *Dev, uint32_t Idx)
{
	uint32_t Present;
	
	if (!Dev)
		return 0;
	if(BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return 0;
	
	Present = 0;
	BSP_DEV_LOCK(Dev);
	if ((!strcmp(Dev->PtNumber, SEEPROM_PT_NUMBER)) && (!strcmp(Dev->HwNumber, IP1562_HW_NUMBER)))
	{
		Present = IP1562_GetPSPresent(&Dev->ip1562, Idx);
	}
	BSP_DEV_UNLOCK(Dev);
	
	return Present;
}

uint32_t SEEPROM_GetSCPresent(SEEPROM_T *Dev, uint32_t Idx)
{
	uint32_t Present;
	
	if (!Dev)
		return 0;
	if(BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return 0;
	
	Present = 0;
	BSP_DEV_LOCK(Dev);
	if ((!strcmp(Dev->PtNumber, SEEPROM_PT_NUMBER)) && (!strcmp(Dev->HwNumber, IP1562_HW_NUMBER)))
	{
		Present = IP1562_GetSCPresent(&Dev->ip1562, Idx);
	}
	BSP_DEV_UNLOCK(Dev);
	
	return Present;
}
