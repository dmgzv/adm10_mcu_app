#include "seeprom.h"
#include "tiny-json.h"
#include <string.h>
#include <stdio.h>

#define SEEPROM_VENDOR_JSON_KEY			"vd"
#define SEEPROM_PT_NUMBER_JSON_KEY	"pt"
#define SEEPROM_HW_NUMBER_JSON_KEY	"hw"
#define SEEPROM_SR_NUMBER_JSON_KEY	"sr"

static int32_t __SEEPROM_WriteInventory(SEEPROM_T *Dev, char *Vendor, char *PtNumber, char *HwNumber, char *SrNumber)
{
	uint32_t i;
	char Buffer[AT24C02C_SIZE];
	uint32_t BufferLen;
	uint32_t PagesCnt;
	
	BufferLen = 0;
	Buffer[BufferLen++] = '{';
	BufferLen += sprintf(&Buffer[BufferLen], "\""SEEPROM_VENDOR_JSON_KEY"\":\"%s\",", Vendor);
	BufferLen += sprintf(&Buffer[BufferLen], "\""SEEPROM_PT_NUMBER_JSON_KEY"\":\"%s\",", PtNumber);
	BufferLen += sprintf(&Buffer[BufferLen], "\""SEEPROM_HW_NUMBER_JSON_KEY"\":\"%s\"", HwNumber);
	BufferLen += sprintf(&Buffer[BufferLen], "\""SEEPROM_SR_NUMBER_JSON_KEY"\":\"%s\"", SrNumber);
	Buffer[BufferLen++] = '}';
	Buffer[BufferLen++] = 0;
	PagesCnt = (BufferLen % AT24C02C_PAGE_SIZE) ? ((BufferLen / AT24C02C_PAGE_SIZE) + 1) : (BufferLen / AT24C02C_PAGE_SIZE);
	
	for (i = 0; i < PagesCnt; i++) {
		if (AT24C02C_Write(&Dev->at24c02c, i * AT24C02C_PAGE_SIZE, &Buffer[i * AT24C02C_PAGE_SIZE], AT24C02C_PAGE_SIZE))
			goto error;
	}
	
	strcpy(Dev->Vendor, Vendor);
	strcpy(Dev->PtNumber, PtNumber);
	strcpy(Dev->HwNumber, HwNumber);
	strcpy(Dev->SrNumber, SrNumber);
	LOG_Info((Dev)->DevName, "", "InventoryWrite", "%s;%s;%s;%s", Vendor, PtNumber, HwNumber, SrNumber);
	
	return 0;

error:
	LOG_Info((Dev)->DevName, "", "InventoryWriteError", "%s;%s;%s;%s", Vendor, PtNumber, HwNumber, SrNumber);
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
	
	Dev->Vendor[0] = 0;
	Dev->PtNumber[0] = 0;
	Dev->HwNumber[0] = 0;
	Dev->SrNumber[0] = 0;
	Dev->__UpdateFlag = 0;
	
	json = json_create( Buffer, jsonPool, sizeof (jsonPool) / sizeof (jsonPool[0]) );
	if ( json )
	{
		jsonProp = json_getProperty( json, SEEPROM_VENDOR_JSON_KEY );
		if ((jsonProp) && (JSON_TEXT == json_getType( jsonProp )))
		{
			c = (char*)json_getValue( jsonProp );
			if (strlen(c) < sizeof(Dev->Vendor))
			{
				strcpy(Dev->Vendor, c);
			}
			else
			{
				LOG_Info((Dev)->DevName, "", "InventoryParseError", "Invalid length of Vendor");
			}
		}
		
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
	
	LOG_Info((Dev)->DevName, "", "InventoryInit", "%s;%s;%s;%s", Dev->Vendor, Dev->PtNumber, Dev->HwNumber, Dev->SrNumber);
	
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
		if (__SEEPROM_WriteInventory(Dev, Dev->Vendor, Dev->PtNumber, Dev->HwNumber, Dev->SrNumber))
			goto error;
		Dev->__UpdateFlag = 0;
	}
	
	if ((!strcmp(Dev->Vendor, IP1562_VENDOR)) && (!strcmp(Dev->PtNumber, IP1562_PT_NUMBER)) && (!strcmp(Dev->HwNumber, IP1562_HW_NUMBER)))
	{
		IP1562_Process(&Dev->ip1562);
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return 0;
}

int32_t SEEPROM_ConfigVendor(SEEPROM_T *Dev, char *Vendor)
{
	
	if ((!Dev) || (strlen(Vendor) >= sizeof(Dev->Vendor)))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_LOCK(Dev);
	
	if (strcmp(Dev->Vendor, Vendor))
	{
		strcpy(Dev->Vendor, Vendor);
		Dev->__UpdateFlag = 1;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SEEPROM_ConfigPtNumber(SEEPROM_T *Dev, char *PtNumber)
{
	
	if ((!Dev) || (strlen(PtNumber) >= sizeof(Dev->PtNumber)))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_LOCK(Dev);
	
	if (strcmp(Dev->PtNumber, PtNumber))
	{
		strcpy(Dev->PtNumber, PtNumber);
		Dev->__UpdateFlag = 1;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SEEPROM_ConfigHwNumber(SEEPROM_T *Dev, char *HwNumber)
{
	
	if ((!Dev) || (strlen(HwNumber) >= sizeof(Dev->HwNumber)))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_LOCK(Dev);
	
	if (strcmp(Dev->HwNumber, HwNumber))
	{
		strcpy(Dev->HwNumber, HwNumber);
		Dev->__UpdateFlag = 1;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SEEPROM_ConfigSrNumber(SEEPROM_T *Dev, char *SrNumber)
{
	
	if ((!Dev) || (strlen(SrNumber) >= sizeof(Dev->SrNumber)))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_LOCK(Dev);
	
	if (strcmp(Dev->SrNumber, SrNumber))
	{
		strcpy(Dev->SrNumber, SrNumber);
		Dev->__UpdateFlag = 1;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SEEPROM_ConfigFansSpeed(SEEPROM_T *Dev, float Speed)
{
	if (!Dev)
		return -1;
	if(BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	if ((!strcmp(Dev->Vendor, IP1562_VENDOR)) && (!strcmp(Dev->PtNumber, IP1562_PT_NUMBER)) && (!strcmp(Dev->HwNumber, IP1562_HW_NUMBER)))
	{
		if (IP1562_ConfigFansSpeed(&Dev->ip1562, Speed))
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

float SEEPROM_GetFanSpeed(SEEPROM_T *Dev, uint32_t Idx)
{
	float Speed;
	
	if ((!Dev) || (Idx >= 8))
		return 0;
	if(BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return 0;
	
	Speed = 0;
	BSP_DEV_LOCK(Dev);
	if ((!strcmp(Dev->Vendor, IP1562_VENDOR)) && (!strcmp(Dev->PtNumber, IP1562_PT_NUMBER)) && (!strcmp(Dev->HwNumber, IP1562_HW_NUMBER)))
	{
		Speed = IP1562_GetFanSpeed(&Dev->ip1562, Idx);
	}
	BSP_DEV_UNLOCK(Dev);
	
	return Speed;
}

uint32_t SEEPROM_GetPSPresent(SEEPROM_T *Dev, uint32_t Idx)
{
	uint32_t Present;
	
	if ((!Dev) || (Idx >= 2))
		return 0;
	if(BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return 0;
	
	Present = 0;
	BSP_DEV_LOCK(Dev);
	if ((!strcmp(Dev->Vendor, IP1562_VENDOR)) && (!strcmp(Dev->PtNumber, IP1562_PT_NUMBER)) && (!strcmp(Dev->HwNumber, IP1562_HW_NUMBER)))
	{
		Present = IP1562_GetPSPresent(&Dev->ip1562, Idx);
	}
	BSP_DEV_UNLOCK(Dev);
	
	return Present;
}

uint32_t SEEPROM_GetSCPresent(SEEPROM_T *Dev, uint32_t Idx)
{
	uint32_t Present;
	
	if ((!Dev) || (Idx >= 2))
		return 0;
	if(BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return 0;
	
	Present = 0;
	BSP_DEV_LOCK(Dev);
	if ((!strcmp(Dev->Vendor, IP1562_VENDOR)) && (!strcmp(Dev->PtNumber, IP1562_PT_NUMBER)) && (!strcmp(Dev->HwNumber, IP1562_HW_NUMBER)))
	{
		Present = IP1562_GetSCPresent(&Dev->ip1562, Idx);
	}
	BSP_DEV_UNLOCK(Dev);
	
	return Present;
}

int32_t SEEPROM_ConfigOutState(SEEPROM_T *Dev, uint32_t Idx, uint32_t State)
{
	if (!Dev)
		return -1;
	if(BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	if ((!strcmp(Dev->Vendor, IP1562_VENDOR)) && (!strcmp(Dev->PtNumber, IP1562_PT_NUMBER)) && (!strcmp(Dev->HwNumber, IP1562_HW_NUMBER)))
	{
		if (IP1562_ConfigOutState(&Dev->ip1562, Idx, State))
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

uint32_t SEEPROM_GetOutState(SEEPROM_T *Dev, uint32_t Idx)
{
	uint32_t OutState;
	
	if ((!Dev) || (Idx >= 2))
		return 0;
	if(BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return 0;
	
	OutState = 0;
	BSP_DEV_LOCK(Dev);
	if ((!strcmp(Dev->Vendor, IP1562_VENDOR)) && (!strcmp(Dev->PtNumber, IP1562_PT_NUMBER)) && (!strcmp(Dev->HwNumber, IP1562_HW_NUMBER)))
	{
		OutState = IP1562_GetOutState(&Dev->ip1562, Idx);
	}
	BSP_DEV_UNLOCK(Dev);
	
	return OutState;
}

uint32_t SEEPROM_GetInState(SEEPROM_T *Dev, uint32_t Idx)
{
	uint32_t InState;
	
	if ((!Dev) || (Idx >= 2))
		return 0;
	if(BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return 0;
	
	InState = 0;
	BSP_DEV_LOCK(Dev);
	if ((!strcmp(Dev->Vendor, IP1562_VENDOR)) && (!strcmp(Dev->PtNumber, IP1562_PT_NUMBER)) && (!strcmp(Dev->HwNumber, IP1562_HW_NUMBER)))
	{
		InState = IP1562_GetInState(&Dev->ip1562, Idx);
	}
	BSP_DEV_UNLOCK(Dev);
	
	return InState;
}
