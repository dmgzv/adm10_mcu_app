#include "fpga_line_adm_10.h"
#include "log.h"
#include <string.h>
#include <stdio.h>

#define FPGA_LINE_ADM_10_ADDR(Dev)									(Dev)->BaseAddr

#define FPGA_LINE_ADM_10_CMD_ADDR(Dev)							(FPGA_LINE_ADM_10_ADDR(Dev) + 0x08)
#define FPGA_LINE_ADM_10_CMD_OPT1_ADDR(Dev)					(FPGA_LINE_ADM_10_ADDR(Dev) + 0x09)
#define FPGA_LINE_ADM_10_CMD_OPT2_ADDR(Dev)					(FPGA_LINE_ADM_10_ADDR(Dev) + 0x0A)
#define FPGA_LINE_ADM_10_CMD_OPT3_ADDR(Dev)					(FPGA_LINE_ADM_10_ADDR(Dev) + 0x0B)
#define FPGA_LINE_ADM_10_STATUS_ADDR(Dev)						(FPGA_LINE_ADM_10_ADDR(Dev) + 0x0C)
#define FPGA_LINE_ADM_10_STATUS_OPT_ADDR(Dev)				(FPGA_LINE_ADM_10_ADDR(Dev) + 0x0D)
#define FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx)  (FPGA_LINE_ADM_10_ADDR(Dev) + ((IfaceIdx) * 0x100) + 0x100)

#define FPGA_LINE_ADM_10_IFACE_OTU2_ALARMS_ADDR(Dev, IfaceIdx)   		(FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x00)
#define FPGA_LINE_ADM_10_IFACE_OTU2_BIP_ADDR(Dev, IfaceIdx)   			(FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x02)
#define FPGA_LINE_ADM_10_IFACE_OTU2_FEC_CORR_ADDR(Dev, IfaceIdx)  	(FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x04)
#define FPGA_LINE_ADM_10_IFACE_OTU2_FEC_UNCORR_ADDR(Dev, IfaceIdx)	(FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x08)

#define FPGA_LINE_ADM_10_IFACE_CTRL_ADDR(Dev, IfaceIdx)   	(FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x0C)

#define FPGA_LINE_ADM_10_IFACE_PMASTATUS_ADDR(Dev, IfaceIdx)   (FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x0E)

#define FPGA_LINE_ADM_10_IFACE_ODU2_ALARMS_ADDR(Dev, IfaceIdx)  (FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x10)
#define FPGA_LINE_ADM_10_IFACE_ODU2_BIP_ADDR(Dev, IfaceIdx)   	(FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x10 + 0x02)

#define FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_ADDR(Dev, IfaceIdx, ODU1Idx)   (FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x18 + ((ODU1Idx) * 0x08))
#define FPGA_LINE_ADM_10_IFACE_ODU1_BIP_ADDR(Dev, IfaceIdx, ODU1Idx)   (FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x18 + ((ODU1Idx) * 0x08) + 0x02)

#define FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_ADDR(Dev, IfaceIdx, ODU0Idx)   (FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x38 + ((ODU0Idx) * 0x08))
#define FPGA_LINE_ADM_10_IFACE_ODU0_BIP_ADDR(Dev, IfaceIdx, ODU0Idx)   (FPGA_LINE_ADM_10_IFACE_ADDR(Dev, IfaceIdx) + 0x38 + ((ODU0Idx) * 0x08) + 0x02)

#define FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_ADDR(Dev, OPU0Idx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x530 + (OPU0Idx))
#define FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_ADDR(Dev, OPU1Idx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x518 + (OPU1Idx))

#define FPGA_LINE_ADM_10_MUX_ODU01_ADDR(Dev, ODU01Idx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x520 + (ODU01Idx))
#define FPGA_LINE_ADM_10_MUX_ODU12_ADDR(Dev, ODU12Idx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x510 + (ODU12Idx))

#define FPGA_LINE_ADM_10_DEMUX_CLIENT_MODE_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x500 + (ClientIdx))

#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x540 + ((ClientIdx) * 8))
#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_STATUS_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x540 + ((ClientIdx) * 8) + 1)
#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_PRIMARY_ODU1_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x540 + ((ClientIdx) * 8) + 2)
#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_PROTECT_ODU1_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x540 + ((ClientIdx) * 8) + 3)
#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_WTR_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x540 + ((ClientIdx) * 8) + 4)
#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_HO_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x540 + ((ClientIdx) * 8) + 5)

#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x580 + ((ClientIdx) * 8))
#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_STATUS_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x580 + ((ClientIdx) * 8) + 1)
#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_PRIMARY_ODU0_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x580 + ((ClientIdx) * 8) + 2)
#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_PROTECT_ODU0_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x580 + ((ClientIdx) * 8) + 3)
#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_WTR_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x580 + ((ClientIdx) * 8) + 4)
#define FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_HO_ADDR(Dev, ClientIdx)   (FPGA_LINE_ADM_10_ADDR(Dev) + 0x580 + ((ClientIdx) * 8) + 5)


static void FPGA_LINE_ADM_10_WriteByte(FPGA_LINE_ADM_10_T *Dev, uint32_t Addr, uint8_t Data)
{
	LOG_Info(Dev->DevName, "", "ByteWrite", "0x%08x;0x%02x", Addr, Data);
	*((uint8_t*)(Addr)) = Data;
}

static void FPGA_LINE_ADM_10_WriteCmd(FPGA_LINE_ADM_10_T *Dev, uint32_t Cmd, uint8_t Opt1, uint8_t Opt2, uint8_t Opt3)
{
	LOG_Info(Dev->DevName, "", "CmdWrite", "0x%02x;0x%02x;0x%02x;0x%02x", Cmd, Opt1, Opt2, Opt3);
	*((uint8_t*)FPGA_LINE_ADM_10_CMD_OPT1_ADDR(Dev)) = Opt1;
	*((uint8_t*)FPGA_LINE_ADM_10_CMD_OPT2_ADDR(Dev)) = Opt2;
	*((uint8_t*)FPGA_LINE_ADM_10_CMD_OPT3_ADDR(Dev)) = Opt3;
	*((uint8_t*)FPGA_LINE_ADM_10_CMD_ADDR(Dev)) = Cmd;
}

int32_t FPGA_LINE_ADM_10_Init(FPGA_LINE_ADM_10_T *Dev)
{
	uint8_t Status, SwVersion[4];
	uint32_t i, j;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
	if (Status == 0)
		goto error;
	
	for (i = 0; i < 4; i++)
	{
		SwVersion[i] = *((char*)(FPGA_LINE_ADM_10_ADDR(Dev) + i));
	}
	sprintf(Dev->SwVersion, "%d.%d.%d.%d", SwVersion[1], SwVersion[0], SwVersion[3], SwVersion[2]);
	
	for (i = 0; i < FPGA_LINE_ADM_10_IFACE_CNT; i++)
	{		
		Dev->Iface[i].PMAStatus = Dev->Iface[i].__PMAStatus = *((uint8_t*)(FPGA_LINE_ADM_10_IFACE_PMASTATUS_ADDR(Dev, i)));
		Dev->Iface[i].Ctrl = Dev->Iface[i].__Ctrl = *((uint8_t*)(FPGA_LINE_ADM_10_IFACE_CTRL_ADDR(Dev, i)));
		
		Dev->Iface[i].__OTU2Alarms = *((uint16_t*)FPGA_LINE_ADM_10_IFACE_OTU2_ALARMS_ADDR(Dev, i));
		
		Dev->Iface[i].__OTU2BIP = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_OTU2_BIP_ADDR(Dev, i)));
		Dev->Iface[i].OTU2BIP = 0;
		
		Dev->Iface[i].__OTU2FECCorr = *((uint32_t*)(FPGA_LINE_ADM_10_IFACE_OTU2_FEC_CORR_ADDR(Dev, i)));
		Dev->Iface[i].OTU2FECCorr = 0;
		
		Dev->Iface[i].__OTU2FECUncorr = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_OTU2_FEC_UNCORR_ADDR(Dev, i)));
		Dev->Iface[i].OTU2FECUncorr = 0;
		
		Dev->Iface[i].__ODU2Alarms = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU2_ALARMS_ADDR(Dev, i)));	
			
		Dev->Iface[i].__ODU2BIP = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU2_BIP_ADDR(Dev, i)));
		Dev->Iface[i].ODU2BIP = 0;
		
		for (j = 0; j < FPGA_LINE_ADM_10_IFACE_ODU1_CNT; j++)
		{
			Dev->Iface[i].__ODU1Alarms[j] = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_ADDR(Dev, i, j)));
			
			Dev->Iface[i].__ODU1BIP[j] = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU1_BIP_ADDR(Dev, i, j)));
			Dev->Iface[i].ODU1BIP[j] = 0;
		}
		
		for (j = 0; j < FPGA_LINE_ADM_10_IFACE_ODU0_CNT; j++)
		{
			Dev->Iface[i].__ODU0Alarms[j] = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_ADDR(Dev, i, j)));
			
			Dev->Iface[i].__ODU0BIP[j] = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU0_BIP_ADDR(Dev, i, j)));
			Dev->Iface[i].ODU0BIP[j] = 0;
		}
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_MUX_OPU0_CNT; i++)
	{
		Dev->Mux.OPU0PayloadType[i] = *((uint8_t*)(FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_ADDR(Dev, i)));
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_MUX_OPU1_CNT; i++)
	{
		Dev->Mux.OPU1PayloadType[i] = *((uint8_t*)(FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_ADDR(Dev, i)));
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_MUX_ODU01_CNT; i+=2)
	{
		Dev->Mux.ODU01[i] = *((uint8_t*)(FPGA_LINE_ADM_10_MUX_ODU01_ADDR(Dev, i)));
		Dev->Mux.ODU01[i+1] = *((uint8_t*)(FPGA_LINE_ADM_10_MUX_ODU01_ADDR(Dev, i+1)));
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_MUX_ODU12_CNT; i++)
	{
		Dev->Mux.ODU12[i] = *((uint8_t*)(FPGA_LINE_ADM_10_MUX_ODU12_ADDR(Dev, i)));
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_DEMUX_CLIENT_MODE_CNT; i++)
	{
		Dev->Demux.ClientMode[i] = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_MODE_ADDR(Dev, i)));
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_CNT; i++)
	{
		Dev->Demux.ClientODU1PG[i].Mode = Dev->Demux.ClientODU1PG[i].__Mode =*((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_ADDR(Dev, i)));
		
		Dev->Demux.ClientODU1PG[i].__Status = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_STATUS_ADDR(Dev, i)));
		
		Dev->Demux.ClientODU1PG[i].PrimaryODUk = Dev->Demux.ClientODU1PG[i].__PrimaryODUk = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_PRIMARY_ODU1_ADDR(Dev, i)));
		
		Dev->Demux.ClientODU1PG[i].ProtectODUk = Dev->Demux.ClientODU1PG[i].__ProtectODUk = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_PROTECT_ODU1_ADDR(Dev, i)));
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_CNT; i++)
	{
		Dev->Demux.ClientODU0PG[i].Mode = Dev->Demux.ClientODU0PG[i].__Mode = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_ADDR(Dev, i)));
		
		Dev->Demux.ClientODU0PG[i].__Status = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_STATUS_ADDR(Dev, i)));
		
		Dev->Demux.ClientODU0PG[i].PrimaryODUk = Dev->Demux.ClientODU0PG[i].__PrimaryODUk = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_PRIMARY_ODU0_ADDR(Dev, i)));
	
		Dev->Demux.ClientODU0PG[i].ProtectODUk = Dev->Demux.ClientODU0PG[i].__ProtectODUk = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_PROTECT_ODU0_ADDR(Dev, i)));
	}
	
	BSP_DEV_INIT_OK(Dev);
	return 0;

error:
	BSP_DEV_INIT_FAIL(Dev);
	return 0;
}

int32_t FPGA_LINE_ADM_10_Update(FPGA_LINE_ADM_10_T *Dev)
{
	uint32_t i, j;
	uint8_t Status;
	uint16_t Temp, OTU2BIP, OTU2FECUncorr, ODU2BIP, ODU1BIP, ODU0BIP;
	uint32_t OTU2FECCorr;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	Temp = *((uint16_t*)(FPGA_LINE_ADM_10_ADDR(Dev) + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;
	
	for (i = 0; i < FPGA_LINE_ADM_10_IFACE_CNT; i++)
	{
		
		Dev->Iface[i].__OTU2Alarms = *((uint16_t*)FPGA_LINE_ADM_10_IFACE_OTU2_ALARMS_ADDR(Dev, i));
		Dev->Iface[i].__PMAStatus = *((uint8_t*)(FPGA_LINE_ADM_10_IFACE_PMASTATUS_ADDR(Dev, i)));
		Dev->Iface[i].__Ctrl = *((uint8_t*)(FPGA_LINE_ADM_10_IFACE_CTRL_ADDR(Dev, i)));
		
		OTU2BIP = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_OTU2_BIP_ADDR(Dev, i)));
		Dev->Iface[i].OTU2BIP = OTU2BIP - Dev->Iface[i].__OTU2BIP;
		Dev->Iface[i].__OTU2BIP = OTU2BIP;
		
		OTU2FECCorr = *((uint32_t*)(FPGA_LINE_ADM_10_IFACE_OTU2_FEC_CORR_ADDR(Dev, i)));
		Dev->Iface[i].OTU2FECCorr = OTU2FECCorr - Dev->Iface[i].__OTU2FECCorr;
		Dev->Iface[i].__OTU2FECCorr = OTU2FECCorr;
		
		OTU2FECUncorr = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_OTU2_FEC_UNCORR_ADDR(Dev, i)));
		Dev->Iface[i].OTU2FECUncorr = OTU2FECUncorr - Dev->Iface[i].__OTU2FECUncorr;
		Dev->Iface[i].__OTU2FECUncorr = OTU2FECUncorr;
		
		Dev->Iface[i].__ODU2Alarms = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU2_ALARMS_ADDR(Dev, i)));
		
		ODU2BIP = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU2_BIP_ADDR(Dev, i)));
		Dev->Iface[i].ODU2BIP = ODU2BIP - Dev->Iface[i].__ODU2BIP;
		Dev->Iface[i].__ODU2BIP = ODU2BIP;
		
		for (j = 0; j < FPGA_LINE_ADM_10_IFACE_ODU1_CNT; j++)
		{
			Dev->Iface[i].__ODU1Alarms[j] = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU1_ALARMS_ADDR(Dev, i, j)));
			
			ODU1BIP = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU1_BIP_ADDR(Dev, i, j)));
			Dev->Iface[i].ODU1BIP[j] = ODU1BIP - Dev->Iface[i].__ODU1BIP[j];
			Dev->Iface[i].__ODU1BIP[j] = ODU1BIP;
		}
		
		for (j = 0; j < FPGA_LINE_ADM_10_IFACE_ODU0_CNT; j++)
		{
			Dev->Iface[i].__ODU0Alarms[j] = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU0_ALARMS_ADDR(Dev, i, j)));
			
			ODU0BIP = *((uint16_t*)(FPGA_LINE_ADM_10_IFACE_ODU0_BIP_ADDR(Dev, i, j)));
			Dev->Iface[i].ODU0BIP[j] = ODU0BIP - Dev->Iface[i].__ODU0BIP[j];
			Dev->Iface[i].__ODU0BIP[j] = ODU0BIP;
		}
		
		if ( Dev->Iface[i].Ctrl != Dev->Iface[i].__Ctrl )
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_IFACE_CTRL_ADDR(Dev, i), Dev->Iface[i].Ctrl);
		}
		
		if ((Dev->Iface[i].PMAStatus & FPGA_LINE_ADM_10_IFACE_PMASTATUS_LOOPBACK_MASK) != (Dev->Iface[i].__PMAStatus & FPGA_LINE_ADM_10_IFACE_PMASTATUS_LOOPBACK_MASK))
		{
			if (Dev->Iface[i].PMAStatus & FPGA_LINE_ADM_10_IFACE_PMASTATUS_LOOPBACK_SERIAL_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x03, 0x00, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Iface[i].PMAStatus & FPGA_LINE_ADM_10_IFACE_PMASTATUS_LOOPBACK_POST_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x02, 0x01, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Iface[i].PMAStatus & FPGA_LINE_ADM_10_IFACE_PMASTATUS_LOOPBACK_PRE_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x02, 0x00, i, 0x01);
				osDelay(5);
			}
			else
			{
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_ADM_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_ADM_10_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
			}
		}
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_MUX_OPU0_CNT; i++)
	{
		Dev->Mux.__OPU0PayloadType[i] = *((uint8_t*)(FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_ADDR(Dev, i)));
		
		if (Dev->Mux.__OPU0PayloadType[i] != Dev->Mux.OPU0PayloadType[i])
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_MUX_OPU0_PAYLOAD_TYPE_ADDR(Dev, i), Dev->Mux.OPU0PayloadType[i]);
		}
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_MUX_OPU1_CNT; i++)
	{
		Dev->Mux.__OPU1PayloadType[i] = *((uint8_t*)(FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_ADDR(Dev, i)));
		if (Dev->Mux.__OPU1PayloadType[i] != Dev->Mux.OPU1PayloadType[i])
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_MUX_OPU1_PAYLOAD_TYPE_ADDR(Dev, i), Dev->Mux.OPU1PayloadType[i]);
		}
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_MUX_ODU01_CNT; i+=2)
	{
		Dev->Mux.__ODU01[i] = *((uint8_t*)(FPGA_LINE_ADM_10_MUX_ODU01_ADDR(Dev, i)));
		Dev->Mux.__ODU01[i+1] = *((uint8_t*)(FPGA_LINE_ADM_10_MUX_ODU01_ADDR(Dev, i+1)));
		if ((Dev->Mux.__ODU01[i] != Dev->Mux.ODU01[i]) || (Dev->Mux.__ODU01[i+1] != Dev->Mux.ODU01[i+1])) 
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_MUX_ODU01_ADDR(Dev, i), Dev->Mux.ODU01[i]);
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_MUX_ODU01_ADDR(Dev, i+1), Dev->Mux.ODU01[i+1]);
		}
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_MUX_ODU12_CNT; i++)
	{
		Dev->Mux.__ODU12[i] = *((uint8_t*)(FPGA_LINE_ADM_10_MUX_ODU12_ADDR(Dev, i)));
		if (Dev->Mux.__ODU12[i] != Dev->Mux.ODU12[i])
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_MUX_ODU12_ADDR(Dev, i), Dev->Mux.ODU12[i]);
		}
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_DEMUX_CLIENT_MODE_CNT; i++)
	{
		Dev->Demux.__ClientMode[i] = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_MODE_ADDR(Dev, i)));
		if (Dev->Demux.__ClientMode[i] != Dev->Demux.ClientMode[i])
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_DEMUX_CLIENT_MODE_ADDR(Dev, i), Dev->Demux.ClientMode[i]);
		}
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_CNT; i++)
	{
		Dev->Demux.ClientODU1PG[i].__Mode = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_ADDR(Dev, i)));
		if (Dev->Demux.ClientODU1PG[i].__Mode != Dev->Demux.ClientODU1PG[i].Mode)
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_MODE_ADDR(Dev, i), Dev->Demux.ClientODU1PG[i].Mode);
		}
		
		Dev->Demux.ClientODU1PG[i].__Status = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_STATUS_ADDR(Dev, i)));
		
		Dev->Demux.ClientODU1PG[i].__PrimaryODUk = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_PRIMARY_ODU1_ADDR(Dev, i)));
		if (Dev->Demux.ClientODU1PG[i].__PrimaryODUk != Dev->Demux.ClientODU1PG[i].PrimaryODUk)
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_PRIMARY_ODU1_ADDR(Dev, i), Dev->Demux.ClientODU1PG[i].PrimaryODUk);
		}
		
		Dev->Demux.ClientODU1PG[i].__ProtectODUk = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_PROTECT_ODU1_ADDR(Dev, i)));
		if (Dev->Demux.ClientODU1PG[i].__ProtectODUk != Dev->Demux.ClientODU1PG[i].ProtectODUk)
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_PROTECT_ODU1_ADDR(Dev, i), Dev->Demux.ClientODU1PG[i].ProtectODUk);
		}
		
		Dev->Demux.ClientODU1PG[i].__WTRTmr = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_WTR_ADDR(Dev, i)));
		if (Dev->Demux.ClientODU1PG[i].__WTRTmr != Dev->Demux.ClientODU1PG[i].WTRTmr)
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_WTR_ADDR(Dev, i), Dev->Demux.ClientODU1PG[i].WTRTmr);
		}
		
		Dev->Demux.ClientODU1PG[i].__HOTmr = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_HO_ADDR(Dev, i)));
		if (Dev->Demux.ClientODU1PG[i].__HOTmr != Dev->Demux.ClientODU1PG[i].HOTmr)
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU1_PG_HO_ADDR(Dev, i), Dev->Demux.ClientODU1PG[i].HOTmr);
		}
	}
	
	for (i = 0; i < FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_CNT; i++)
	{
		Dev->Demux.ClientODU0PG[i].__Mode = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_ADDR(Dev, i)));
		if (Dev->Demux.ClientODU0PG[i].__Mode != Dev->Demux.ClientODU0PG[i].Mode)
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_MODE_ADDR(Dev, i), Dev->Demux.ClientODU0PG[i].Mode);
		}
		
		Dev->Demux.ClientODU0PG[i].__Status = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_STATUS_ADDR(Dev, i)));
		
		Dev->Demux.ClientODU0PG[i].__PrimaryODUk = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_PRIMARY_ODU0_ADDR(Dev, i)));
		if (Dev->Demux.ClientODU0PG[i].__PrimaryODUk != Dev->Demux.ClientODU0PG[i].PrimaryODUk)
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_PRIMARY_ODU0_ADDR(Dev, i), Dev->Demux.ClientODU0PG[i].PrimaryODUk);
		}
		
		Dev->Demux.ClientODU0PG[i].__ProtectODUk = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_PROTECT_ODU0_ADDR(Dev, i)));
		if (Dev->Demux.ClientODU0PG[i].__ProtectODUk != Dev->Demux.ClientODU0PG[i].ProtectODUk)
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_PROTECT_ODU0_ADDR(Dev, i), Dev->Demux.ClientODU0PG[i].ProtectODUk);
		}
		
		Dev->Demux.ClientODU0PG[i].__WTRTmr = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_WTR_ADDR(Dev, i)));
		if (Dev->Demux.ClientODU0PG[i].__WTRTmr != Dev->Demux.ClientODU0PG[i].WTRTmr)
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_WTR_ADDR(Dev, i), Dev->Demux.ClientODU0PG[i].WTRTmr);
		}
		
		Dev->Demux.ClientODU0PG[i].__HOTmr = *((uint8_t*)(FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_HO_ADDR(Dev, i)));
		if (Dev->Demux.ClientODU0PG[i].__HOTmr != Dev->Demux.ClientODU0PG[i].HOTmr)
		{
			FPGA_LINE_ADM_10_WriteByte(Dev, FPGA_LINE_ADM_10_DEMUX_CLIENT_ODU0_PG_HO_ADDR(Dev, i), Dev->Demux.ClientODU0PG[i].HOTmr);
		}
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
}

void FPGA_LINE_ADM_10_Deinit(FPGA_LINE_ADM_10_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
