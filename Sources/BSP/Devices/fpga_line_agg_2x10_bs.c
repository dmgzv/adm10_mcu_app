#include "fpga_line_agg_2x10_bs.h"
#include "log.h"
#include <string.h>
#include <stdio.h>

#define FPGA_LINE_AGG_2X10_BS_ADDR(Dev)									(Dev)->BaseAddr

#define FPGA_LINE_AGG_2X10_BS_CMD_ADDR(Dev)							(FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + 0x08)
#define FPGA_LINE_AGG_2X10_BS_CMD_OPT1_ADDR(Dev)				(FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + 0x09)
#define FPGA_LINE_AGG_2X10_BS_CMD_OPT2_ADDR(Dev)				(FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + 0x0A)
#define FPGA_LINE_AGG_2X10_BS_CMD_OPT3_ADDR(Dev)				(FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + 0x0B)
#define FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev)					(FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + 0x0C)
#define FPGA_LINE_AGG_2X10_BS_STATUS_OPT_ADDR(Dev)			(FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + 0x0D)
#define FPGA_LINE_AGG_2X10_BS_IFACE_ADDR(Dev, IfaceIdx) (FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + ((IfaceIdx) * 0x100) + 0x100)

#define FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_ALARMS_ADDR(Dev, IfaceIdx)   	(FPGA_LINE_AGG_2X10_BS_IFACE_ADDR(Dev, IfaceIdx) + 0x00)
#define FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_BIP_ADDR(Dev, IfaceIdx)   			(FPGA_LINE_AGG_2X10_BS_IFACE_ADDR(Dev, IfaceIdx) + 0x02)
#define FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_FEC_CORR_ADDR(Dev, IfaceIdx)  	(FPGA_LINE_AGG_2X10_BS_IFACE_ADDR(Dev, IfaceIdx) + 0x04)
#define FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_FEC_UNCORR_ADDR(Dev, IfaceIdx)	(FPGA_LINE_AGG_2X10_BS_IFACE_ADDR(Dev, IfaceIdx) + 0x08)

#define FPGA_LINE_AGG_2X10_BS_IFACE_CTRL_ADDR(Dev, IfaceIdx)   	(FPGA_LINE_AGG_2X10_BS_IFACE_ADDR(Dev, IfaceIdx) + 0x0C)

#define FPGA_LINE_AGG_2X10_BS_IFACE_PMASTATUS_ADDR(Dev, IfaceIdx)   (FPGA_LINE_AGG_2X10_BS_IFACE_ADDR(Dev, IfaceIdx) + 0x0E)

#define FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_ADDR(Dev, IfaceIdx)  (FPGA_LINE_AGG_2X10_BS_IFACE_ADDR(Dev, IfaceIdx) + 0x10)
#define FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_BIP_ADDR(Dev, IfaceIdx)   	(FPGA_LINE_AGG_2X10_BS_IFACE_ADDR(Dev, IfaceIdx) + 0x10 + 0x02)

#define FPGA_LINE_AGG_2X10_BS_AGG_MODE_ADDR(Dev, AGGIdx) (FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + 0x500 + (AGGIdx)*(0x10))

#define FPGA_LINE_AGG_2X10_BS_AGG_STATUS_ADDR(Dev, AGGIdx) (FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + 0x501 + (AGGIdx)*(0x10))

#define FPGA_LINE_AGG_2X10_BS_AGG_WTR_ADDR(Dev, AGGIdx) (FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + 0x502 + (AGGIdx)*(0x10))
#define FPGA_LINE_AGG_2X10_BS_AGG_HO_ADDR(Dev, AGGIdx) (FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + 0x503 + (AGGIdx)*(0x10))


static void FPGA_LINE_AGG_2X10_BS_WriteByte(FPGA_LINE_AGG_2X10_BS_T *Dev, uint32_t Addr, uint8_t Data)
{
	LOG_Info(Dev->DevName, "", "ByteWrite", "0x%08x;0x%02x", Addr, Data);
	*((uint8_t*)(Addr)) = Data;
}

static void FPGA_LINE_AGG_2X10_BS_WriteCmd(FPGA_LINE_AGG_2X10_BS_T *Dev, uint32_t Cmd, uint8_t Opt1, uint8_t Opt2, uint8_t Opt3)
{
	LOG_Info(Dev->DevName, "", "CmdWrite", "0x%02x;0x%02x;0x%02x;0x%02x", Cmd, Opt1, Opt2, Opt3);
	*((uint8_t*)FPGA_LINE_AGG_2X10_BS_CMD_OPT1_ADDR(Dev)) = Opt1;
	*((uint8_t*)FPGA_LINE_AGG_2X10_BS_CMD_OPT2_ADDR(Dev)) = Opt2;
	*((uint8_t*)FPGA_LINE_AGG_2X10_BS_CMD_OPT3_ADDR(Dev)) = Opt3;
	*((uint8_t*)FPGA_LINE_AGG_2X10_BS_CMD_ADDR(Dev)) = Cmd;
}

int32_t FPGA_LINE_AGG_2X10_BS_Init(FPGA_LINE_AGG_2X10_BS_T *Dev)
{
	uint8_t Status, SwVersion[4];
	uint32_t i;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
	if (Status == 0)
		goto error;
	
	for (i = 0; i < 4; i++)
	{
		SwVersion[i] = *((char*)(FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + i));
	}
	sprintf(Dev->SwVersion, "%d.%d.%d.%d", SwVersion[1], SwVersion[0], SwVersion[3], SwVersion[2]);
	
	for (i = 0; i < FPGA_LINE_AGG_2X10_BS_IFACE_CNT; i++)
	{		
		Dev->Iface[i].PMAStatus = Dev->Iface[i].__PMAStatus = *((uint8_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_PMASTATUS_ADDR(Dev, i)));
		Dev->Iface[i].Ctrl = Dev->Iface[i].__Ctrl = *((uint8_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_CTRL_ADDR(Dev, i)));
		
		Dev->Iface[i].__OTU2Alarms = *((uint16_t*)FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_ALARMS_ADDR(Dev, i));
		
		Dev->Iface[i].__OTU2BIP = *((uint16_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_BIP_ADDR(Dev, i)));
		Dev->Iface[i].OTU2BIP = 0;
		
		Dev->Iface[i].__OTU2FECCorr = *((uint32_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_FEC_CORR_ADDR(Dev, i)));
		Dev->Iface[i].OTU2FECCorr = 0;
		
		Dev->Iface[i].__OTU2FECUncorr = *((uint16_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_FEC_UNCORR_ADDR(Dev, i)));
		Dev->Iface[i].OTU2FECUncorr = 0;
		
		Dev->Iface[i].__ODU2Alarms = *((uint16_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_ADDR(Dev, i)));	
			
		Dev->Iface[i].__ODU2BIP = *((uint16_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_BIP_ADDR(Dev, i)));
		Dev->Iface[i].ODU2BIP = 0;
	}
	
	for (i = 0; i < 2; i++)
	{
		Dev->AGG[i].Mode = Dev->AGG[i].__Mode = *(uint8_t*)(FPGA_LINE_AGG_2X10_BS_AGG_MODE_ADDR(Dev, i));
		Dev->AGG[i].__Status = *(uint8_t*)(FPGA_LINE_AGG_2X10_BS_AGG_STATUS_ADDR(Dev, i));
		Dev->AGG[i].WTR = Dev->AGG[i].__WTR = *(uint8_t*)(FPGA_LINE_AGG_2X10_BS_AGG_WTR_ADDR(Dev, i));
		Dev->AGG[i].HO = Dev->AGG[i].__HO = *(uint8_t*)(FPGA_LINE_AGG_2X10_BS_AGG_HO_ADDR(Dev, i));
	}
	
	BSP_DEV_INIT_OK(Dev);
	return 0;

error:
	BSP_DEV_INIT_FAIL(Dev);
	return 0;
}

int32_t FPGA_LINE_AGG_2X10_BS_Update(FPGA_LINE_AGG_2X10_BS_T *Dev)
{
	uint32_t i;
	uint8_t Status;
	uint16_t Temp, OTU2BIP, OTU2FECUncorr, ODU2BIP;
	uint32_t OTU2FECCorr;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	Temp = *((uint16_t*)(FPGA_LINE_AGG_2X10_BS_ADDR(Dev) + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;
	
	for (i = 0; i < FPGA_LINE_AGG_2X10_BS_IFACE_CNT; i++)
	{
		
		Dev->Iface[i].__OTU2Alarms = *((uint16_t*)FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_ALARMS_ADDR(Dev, i));
		Dev->Iface[i].__PMAStatus = *((uint8_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_PMASTATUS_ADDR(Dev, i)));
		Dev->Iface[i].__Ctrl = *((uint8_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_CTRL_ADDR(Dev, i)));
		
		OTU2BIP = *((uint16_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_BIP_ADDR(Dev, i)));
		Dev->Iface[i].OTU2BIP = OTU2BIP - Dev->Iface[i].__OTU2BIP;
		Dev->Iface[i].__OTU2BIP = OTU2BIP;
		
		OTU2FECCorr = *((uint32_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_FEC_CORR_ADDR(Dev, i)));
		Dev->Iface[i].OTU2FECCorr = OTU2FECCorr - Dev->Iface[i].__OTU2FECCorr;
		Dev->Iface[i].__OTU2FECCorr = OTU2FECCorr;
		
		OTU2FECUncorr = *((uint16_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_OTU2_FEC_UNCORR_ADDR(Dev, i)));
		Dev->Iface[i].OTU2FECUncorr = OTU2FECUncorr - Dev->Iface[i].__OTU2FECUncorr;
		Dev->Iface[i].__OTU2FECUncorr = OTU2FECUncorr;
		
		Dev->Iface[i].__ODU2Alarms = *((uint16_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_ALARMS_ADDR(Dev, i)));
		
		ODU2BIP = *((uint16_t*)(FPGA_LINE_AGG_2X10_BS_IFACE_ODU2_BIP_ADDR(Dev, i)));
		Dev->Iface[i].ODU2BIP = ODU2BIP - Dev->Iface[i].__ODU2BIP;
		Dev->Iface[i].__ODU2BIP = ODU2BIP;
		
		if ( Dev->Iface[i].Ctrl != Dev->Iface[i].__Ctrl )
		{
			FPGA_LINE_AGG_2X10_BS_WriteByte(Dev, FPGA_LINE_AGG_2X10_BS_IFACE_CTRL_ADDR(Dev, i), Dev->Iface[i].Ctrl);
		}
		
		if ((Dev->Iface[i].PMAStatus & FPGA_LINE_AGG_2X10_BS_IFACE_PMASTATUS_LOOPBACK_MASK) != (Dev->Iface[i].__PMAStatus & FPGA_LINE_AGG_2X10_BS_IFACE_PMASTATUS_LOOPBACK_MASK))
		{
			if (Dev->Iface[i].PMAStatus & FPGA_LINE_AGG_2X10_BS_IFACE_PMASTATUS_LOOPBACK_SERIAL_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x03, 0x00, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Iface[i].PMAStatus & FPGA_LINE_AGG_2X10_BS_IFACE_PMASTATUS_LOOPBACK_POST_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x01, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Iface[i].PMAStatus & FPGA_LINE_AGG_2X10_BS_IFACE_PMASTATUS_LOOPBACK_PRE_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x00, i, 0x01);
				osDelay(5);
			}
			else
			{
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_AGG_2X10_BS_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
			}
		}
	}
	
	for (i = 0; i < 2; i++)
	{
		Dev->AGG[i].__Mode = *(uint8_t*)(FPGA_LINE_AGG_2X10_BS_AGG_MODE_ADDR(Dev, i));
		Dev->AGG[i].__Status = *(uint8_t*)(FPGA_LINE_AGG_2X10_BS_AGG_STATUS_ADDR(Dev, i));
		Dev->AGG[i].__WTR = *(uint8_t*)(FPGA_LINE_AGG_2X10_BS_AGG_WTR_ADDR(Dev, i));
		Dev->AGG[i].__HO = *(uint8_t*)(FPGA_LINE_AGG_2X10_BS_AGG_HO_ADDR(Dev, i));
		
		if (Dev->AGG[i].__Mode != Dev->AGG[i].Mode)
		{
			FPGA_LINE_AGG_2X10_BS_WriteByte(Dev, FPGA_LINE_AGG_2X10_BS_AGG_MODE_ADDR(Dev, i), Dev->AGG[i].Mode);
		}
		
		if (Dev->AGG[i].__WTR != Dev->AGG[i].WTR)
		{
			FPGA_LINE_AGG_2X10_BS_WriteByte(Dev, FPGA_LINE_AGG_2X10_BS_AGG_WTR_ADDR(Dev, i), Dev->AGG[i].WTR);
		}
		
		if (Dev->AGG[i].__HO != Dev->AGG[i].HO)
		{
			FPGA_LINE_AGG_2X10_BS_WriteByte(Dev, FPGA_LINE_AGG_2X10_BS_AGG_HO_ADDR(Dev, i), Dev->AGG[i].HO);
		}
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
}

void FPGA_LINE_AGG_2X10_BS_Deinit(FPGA_LINE_AGG_2X10_BS_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
