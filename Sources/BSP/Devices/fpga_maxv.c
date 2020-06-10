#include "fpga_maxv.h"
#include "log.h"
#include <string.h>

#define FPGA_MAXV_BASE_ADDR(Dev) 				((Dev)->BaseAddr)
#define FPGA_MAXV_SW_VERSION_ADDR(Dev)	(FPGA_MAXV_BASE_ADDR(Dev) + 0x00)
#define FPGA_MAXV_STATUS_ADDR(Dev)			(FPGA_MAXV_BASE_ADDR(Dev) + 0x01)
#define FPGA_MAXV_MODE_ADDR(Dev)				(FPGA_MAXV_BASE_ADDR(Dev) + 0x02)
#define FPGA_MAXV_POWER_UP_ADDR(Dev)		(FPGA_MAXV_BASE_ADDR(Dev) + 0x03)

static void FPGA_MAXV_WriteByte(FPGA_MAXV_T *Dev, uint32_t Addr, uint8_t Data)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteByte: 0x%08x 0x%02x", Addr, Data);
	*((uint8_t*)(Addr)) = Data;
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
	Dev->Mode = ((*((uint8_t*)FPGA_MAXV_MODE_ADDR(Dev))) >> 7) & 0x01;
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

int32_t FPGA_MAXV_Update(FPGA_MAXV_T *Dev)
{
	uint32_t Mode;
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	Dev->Status = *((uint8_t*)FPGA_MAXV_STATUS_ADDR(Dev));
	Mode = ((*((uint8_t*)FPGA_MAXV_MODE_ADDR(Dev))) >> 7) & 0x01;
	if (Dev->Mode != Mode)
	{
		FPGA_MAXV_WriteByte(Dev, FPGA_MAXV_STATUS_ADDR(Dev), ((Dev->Mode & 0x01) << 7) | (1 << 0));
		Dev->Status = 0;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
}

void FPGA_MAXV_Deinit(FPGA_MAXV_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
