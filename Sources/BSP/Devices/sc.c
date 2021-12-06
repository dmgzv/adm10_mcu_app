#include "sc.h"
#include "tiny-json.h"
#include <string.h>
#include <stdio.h>

#define SC_VENDOR_JSON_KEY	"vd"
#define SC_PT_NUMBER_JSON_KEY	"pt"
#define SC_HW_NUMBER_JSON_KEY	"hw"
#define SC_SR_NUMBER_JSON_KEY	"sr"

static int32_t __SC_WriteInventory(SC_T *Dev, char *Vendor, char *PtNumber, char *HwNumber, char *SrNumber)
{
	uint32_t i;
	char Buffer[AT24C02C_SIZE];
	uint32_t BufferLen;
	uint32_t PagesCnt;
	
	BufferLen = 0;
	Buffer[BufferLen++] = '{';
	BufferLen += sprintf(&Buffer[BufferLen], "\""SC_VENDOR_JSON_KEY"\":\"%s\",", Vendor);
	BufferLen += sprintf(&Buffer[BufferLen], "\""SC_PT_NUMBER_JSON_KEY"\":\"%s\",", PtNumber);
	BufferLen += sprintf(&Buffer[BufferLen], "\""SC_HW_NUMBER_JSON_KEY"\":\"%s\"", HwNumber);
	BufferLen += sprintf(&Buffer[BufferLen], "\""SC_SR_NUMBER_JSON_KEY"\":\"%s\"", SrNumber);
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

int32_t SC_Init(SC_T *Dev)
{
	json_t jsonPool[16];
	json_t const* json;
	json_t const* jsonProp;
	char Buffer[AT24C02C_SIZE];
	char *c;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Dev->Vendor[0] = 0;
	Dev->PtNumber[0] = 0;
	Dev->HwNumber[0] = 0;
	Dev->SrNumber[0] = 0;
	Dev->__UpdateFlag = 0;
	
	if (AT24C02C_Init(&Dev->at24c02c))
		goto ipgp967_card;
	
	if (AT24C02C_Read(&Dev->at24c02c, 0, Buffer, sizeof(Buffer)))
		goto error;
	
	json = json_create( Buffer, jsonPool, sizeof (jsonPool) / sizeof (jsonPool[0]) );
	if ( json )
	{
		jsonProp = json_getProperty( json, SC_VENDOR_JSON_KEY );
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
		
		jsonProp = json_getProperty( json, SC_PT_NUMBER_JSON_KEY );
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

		jsonProp = json_getProperty( json, SC_HW_NUMBER_JSON_KEY );
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
		
		jsonProp = json_getProperty( json, SC_SR_NUMBER_JSON_KEY );
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
	
ipgp967_card:
	strcpy(Dev->Vendor, IPGP967_VENDOR);
	strcpy(Dev->PtNumber, IPGP967_PT_NUMBER);
	strcpy(Dev->HwNumber, IPGP967_HW_NUMBER);
	strcpy(Dev->SrNumber, "");
	
	LOG_Info((Dev)->DevName, "", "InventoryInit", "%s;%s;%s;%s", Dev->Vendor, Dev->PtNumber, Dev->HwNumber, Dev->SrNumber);
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void SC_Deinit(SC_T *Dev)
{
	if (!Dev)
		return;
	AT24C02C_Deinit(&Dev->at24c02c);
	BSP_DEV_DEINIT(Dev);
}

int32_t SC_Update(SC_T *Dev)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->__UpdateFlag)
	{
		if (__SC_WriteInventory(Dev, Dev->Vendor, Dev->PtNumber, Dev->HwNumber, Dev->SrNumber))
			goto error;
		Dev->__UpdateFlag = 0;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return 0;
}

int32_t SC_ConfigChassisAddress(SC_T *Dev, uint8_t ChassisAddress)
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

int32_t SC_ConfigVendor(SC_T *Dev, char *Vendor)
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

int32_t SC_ConfigPtNumber(SC_T *Dev, char *PtNumber)
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

int32_t SC_ConfigHwNumber(SC_T *Dev, char *HwNumber)
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

int32_t SC_ConfigSrNumber(SC_T *Dev, char *SrNumber)
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
