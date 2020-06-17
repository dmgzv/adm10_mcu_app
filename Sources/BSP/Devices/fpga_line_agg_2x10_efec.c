#include "fpga_line_agg_2x10_efec.h"
#include "log.h"
#include <string.h>

#define FPGA_LINE_AGG_2X10_EFEC_ADDR(Dev)									(Dev)->BaseAddr

#define FPGA_LINE_AGG_2X10_EFEC_CMD_ADDR(Dev)							(FPGA_LINE_AGG_2X10_EFEC_ADDR(Dev) + 0x08)
#define FPGA_LINE_AGG_2X10_EFEC_CMD_OPT1_ADDR(Dev)				(FPGA_LINE_AGG_2X10_EFEC_ADDR(Dev) + 0x09)
#define FPGA_LINE_AGG_2X10_EFEC_CMD_OPT2_ADDR(Dev)				(FPGA_LINE_AGG_2X10_EFEC_ADDR(Dev) + 0x0A)
#define FPGA_LINE_AGG_2X10_EFEC_CMD_OPT3_ADDR(Dev)				(FPGA_LINE_AGG_2X10_EFEC_ADDR(Dev) + 0x0B)
#define FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev)					(FPGA_LINE_AGG_2X10_EFEC_ADDR(Dev) + 0x0C)
#define FPGA_LINE_AGG_2X10_EFEC_STATUS_OPT_ADDR(Dev)			(FPGA_LINE_AGG_2X10_EFEC_ADDR(Dev) + 0x0D)
#define FPGA_LINE_AGG_2X10_EFEC_LINE_ADDR(Dev, LineIdx) (FPGA_LINE_AGG_2X10_EFEC_ADDR(Dev) + ((LineIdx) * 0x100) + 0x100)

#define FPGA_LINE_AGG_2X10_EFEC_LINE_STATUS_ADDR(Dev, LineIdx)   	(FPGA_LINE_AGG_2X10_EFEC_LINE_ADDR(Dev, LineIdx) + 0x00)

#define FPGA_LINE_AGG_2X10_EFEC_LINE_CTRL_ADDR(Dev, LineIdx)   	(FPGA_LINE_AGG_2X10_EFEC_LINE_ADDR(Dev, LineIdx) + 0x0C)

#define FPGA_LINE_AGG_2X10_EFEC_LINE_PMASTATUS_ADDR(Dev, LineIdx)   (FPGA_LINE_AGG_2X10_EFEC_LINE_ADDR(Dev, LineIdx) + 0x0E)


static void FPGA_LINE_AGG_2X10_EFEC_WriteByte(FPGA_LINE_AGG_2X10_EFEC_T *Dev, uint32_t Addr, uint8_t Data)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteByte: 0x%08x 0x%02x", Addr, Data);
	*((uint8_t*)(Addr)) = Data;
}

static void FPGA_LINE_AGG_2X10_EFEC_WriteCmd(FPGA_LINE_AGG_2X10_EFEC_T *Dev, uint32_t Cmd, uint8_t Opt1, uint8_t Opt2, uint8_t Opt3)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteCmd: 0x%02x 0x%02x 0x%02x 0x%02x", Cmd, Opt1, Opt2, Opt3);
	*((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_CMD_OPT1_ADDR(Dev)) = Opt1;
	*((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_CMD_OPT2_ADDR(Dev)) = Opt2;
	*((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_CMD_OPT3_ADDR(Dev)) = Opt3;
	*((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_CMD_ADDR(Dev)) = Cmd;
}

int32_t FPGA_LINE_AGG_2X10_EFEC_Init(FPGA_LINE_AGG_2X10_EFEC_T *Dev)
{
	uint8_t Status, SwVersion[4];
	uint32_t i;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
	if (Status == 0)
		goto error;
	
	for (i = 0; i < 4; i++)
	{
		SwVersion[i] = *((char*)(FPGA_LINE_AGG_2X10_EFEC_ADDR(Dev) + i));
	}
	sprintf(Dev->SwVersion, "%d.%d.%d.%d", SwVersion[1], SwVersion[0], SwVersion[3], SwVersion[2]);
	
	for (i = 0; i < FPGA_LINE_AGG_2X10_EFEC_LINE_CNT; i++)
	{		
		Dev->Line[i].PMAStatus = Dev->Line[i].__PMAStatus = *((uint8_t*)(FPGA_LINE_AGG_2X10_EFEC_LINE_PMASTATUS_ADDR(Dev, i)));
		Dev->Line[i].Ctrl = Dev->Line[i].__Ctrl = *((uint8_t*)(FPGA_LINE_AGG_2X10_EFEC_LINE_CTRL_ADDR(Dev, i)));
		
		Dev->Line[i].__Status = *((uint16_t*)FPGA_LINE_AGG_2X10_EFEC_LINE_STATUS_ADDR(Dev, i));
	}
	
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "Init: Ok");
	BSP_DEV_INIT_OK(Dev);
	return 0;

error:
	BSP_DEV_INIT_FAIL(Dev);
	return 0;
}

int32_t FPGA_LINE_AGG_2X10_EFEC_Update(FPGA_LINE_AGG_2X10_EFEC_T *Dev)
{
	uint32_t i;
	uint8_t Status;
	uint16_t Temp;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	Temp = *((uint16_t*)(FPGA_LINE_AGG_2X10_EFEC_ADDR(Dev) + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;
	
	for (i = 0; i < FPGA_LINE_AGG_2X10_EFEC_LINE_CNT; i++)
	{
		
		Dev->Line[i].__Status = *((uint16_t*)FPGA_LINE_AGG_2X10_EFEC_LINE_STATUS_ADDR(Dev, i));
		Dev->Line[i].__PMAStatus = *((uint8_t*)(FPGA_LINE_AGG_2X10_EFEC_LINE_PMASTATUS_ADDR(Dev, i)));
		Dev->Line[i].__Ctrl = *((uint8_t*)(FPGA_LINE_AGG_2X10_EFEC_LINE_CTRL_ADDR(Dev, i)));
		
		if ( Dev->Line[i].Ctrl != Dev->Line[i].__Ctrl )
		{
			FPGA_LINE_AGG_2X10_EFEC_WriteByte(Dev, FPGA_LINE_AGG_2X10_EFEC_LINE_CTRL_ADDR(Dev, i), Dev->Line[i].Ctrl);
		}
		
		if ((Dev->Line[i].PMAStatus & FPGA_LINE_AGG_2X10_EFEC_LINE_PMASTATUS_LOOPBACK_MASK) != (Dev->Line[i].__PMAStatus & FPGA_LINE_AGG_2X10_EFEC_LINE_PMASTATUS_LOOPBACK_MASK))
		{
			if (Dev->Line[i].PMAStatus & FPGA_LINE_AGG_2X10_EFEC_LINE_PMASTATUS_LOOPBACK_SERIAL_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x03, 0x00, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Line[i].PMAStatus & FPGA_LINE_AGG_2X10_EFEC_LINE_PMASTATUS_LOOPBACK_POST_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x01, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Line[i].PMAStatus & FPGA_LINE_AGG_2X10_EFEC_LINE_PMASTATUS_LOOPBACK_PRE_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x00, i, 0x01);
				osDelay(5);
			}
			else
			{
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_EFEC_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
			}
		}
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
}

void FPGA_LINE_AGG_2X10_EFEC_Deinit(FPGA_LINE_AGG_2X10_EFEC_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
