#include "fpga_maxv.h"
#include "log.h"
#include <string.h>

#define FPGA_MAXV_BASE_ADDR(Dev) 				((Dev)->BaseAddr)
#define FPGA_MAXV_SW_VERSION_ADDR(Dev)	(FPGA_MAXV_BASE_ADDR(Dev) + 0x00)
#define FPGA_MAXV_STATUS_ADDR(Dev)			(FPGA_MAXV_BASE_ADDR(Dev) + 0x01)
#define FPGA_MAXV_CONTROL_ADDR(Dev)			(FPGA_MAXV_BASE_ADDR(Dev) + 0x02)
#define FPGA_MAXV_POWER_UP_ADDR(Dev)		(FPGA_MAXV_BASE_ADDR(Dev) + 0x03)
#define FPGA_MAXV_PAGE_SELECT_ADDR(Dev)	(FPGA_MAXV_BASE_ADDR(Dev) + 0x04)
#define FPGA_MAXV_FPGA_SELECT_ADDR(Dev)	(FPGA_MAXV_BASE_ADDR(Dev) + 0x05)

static void FPGA_MAXV_WriteByte(FPGA_MAXV_T *Dev, uint32_t Addr, uint8_t Data)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteByte: 0x%08x 0x%02x", Addr, Data);
	*((uint8_t*)(Addr)) = Data;
}

static void FPGA_MAXV_UpdateMode(FPGA_MAXV_T *Dev)
{
	uint8_t LinePageSelect, Client1PageSelect, Client2PageSelect;
	uint32_t Timeout;
	
	if (Dev->Mode != Dev->__Mode)
	{
		if (Dev->Mode == FPGA_MAXV_MODE_AGG_2X10_BS)
		{
			LinePageSelect = 0;
			Client1PageSelect = 1;
			Client2PageSelect = 1;
		}
		else if (Dev->Mode == FPGA_MAXV_MODE_ADM_10)
		{
			LinePageSelect = 2;
			Client1PageSelect = 3;
			Client2PageSelect = 3;
		}
		else if (Dev->Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5)
		{
			LinePageSelect = 0;
			Client1PageSelect = 1;
			Client2PageSelect = 7;
		}
		else
		{
			return;
		}

		/*Upload Line FPGA*/
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_PAGE_SELECT_ADDR(Dev), LinePageSelect);
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_FPGA_SELECT_ADDR(Dev), (1 << 0));
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_CONTROL_ADDR(Dev), (1 << 0));
		Timeout = 100;
		do {
			osDelay(10);
			Dev->Status = *((uint8_t*)FPGA_MAXV_STATUS_ADDR(Dev));
			Timeout--;
			if (!Timeout)
				return;
		} while(!(Dev->Status & (1 << 1)));
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_FPGA_SELECT_ADDR(Dev), 0);
		
		/*Upload Clien 1 FPGA*/
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_PAGE_SELECT_ADDR(Dev), Client1PageSelect);
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_FPGA_SELECT_ADDR(Dev), (1 << 1));
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_CONTROL_ADDR(Dev), (1 << 0));
		Timeout = 100;
		do {
			osDelay(10);
			Dev->Status = *((uint8_t*)FPGA_MAXV_STATUS_ADDR(Dev));
			Timeout--;
			if (!Timeout)
				return;
		} while(!(Dev->Status & (1 << 2)));
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_FPGA_SELECT_ADDR(Dev), 0);
		
		/*Upload Clien 2 FPGA*/
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_PAGE_SELECT_ADDR(Dev), Client2PageSelect);
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_FPGA_SELECT_ADDR(Dev), (1 << 2));
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_CONTROL_ADDR(Dev), (1 << 0));
		Timeout = 100;
		do {
			osDelay(10);
			Dev->Status = *((uint8_t*)FPGA_MAXV_STATUS_ADDR(Dev));
			Timeout--;
			if (!Timeout)
				return;
		} while(!(Dev->Status & (1 << 3)));
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_FPGA_SELECT_ADDR(Dev), 0);
		
		Dev->__Mode = Dev->Mode; 
	}
}

int32_t FPGA_MAXV_Init(FPGA_MAXV_T *Dev)
{
	uint8_t SwVersion;
	uint8_t PowerUpMarker;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	PowerUpMarker = *((uint8_t*)FPGA_MAXV_POWER_UP_ADDR(Dev));
	if (PowerUpMarker != 0xA5)
		goto error;
	
	SwVersion = *((uint8_t*)FPGA_MAXV_SW_VERSION_ADDR(Dev));
	snprintf(Dev->SwVersion, sizeof(Dev->SwVersion), "%d", SwVersion);
	Dev->Status = *((uint8_t*)FPGA_MAXV_STATUS_ADDR(Dev));
	Dev->Mode = Dev->__Mode = FPGA_MAXV_MODE_NONE;
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

int32_t FPGA_MAXV_Update(FPGA_MAXV_T *Dev)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	Dev->Status = *((uint8_t*)FPGA_MAXV_STATUS_ADDR(Dev));
	FPGA_MAXV_UpdateMode(Dev);
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
}

void FPGA_MAXV_Deinit(FPGA_MAXV_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
