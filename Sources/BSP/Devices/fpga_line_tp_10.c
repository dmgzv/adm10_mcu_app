#include "fpga_line_tp_10.h"
#include "log.h"
#include <string.h>

#define FPGA_LINE_TP_10_ADDR(Dev)									(Dev)->BaseAddr

#define FPGA_LINE_TP_10_CMD_ADDR(Dev)							(FPGA_LINE_TP_10_ADDR(Dev) + 0x08)
#define FPGA_LINE_TP_10_CMD_OPT1_ADDR(Dev)				(FPGA_LINE_TP_10_ADDR(Dev) + 0x09)
#define FPGA_LINE_TP_10_CMD_OPT2_ADDR(Dev)				(FPGA_LINE_TP_10_ADDR(Dev) + 0x0A)
#define FPGA_LINE_TP_10_CMD_OPT3_ADDR(Dev)				(FPGA_LINE_TP_10_ADDR(Dev) + 0x0B)
#define FPGA_LINE_TP_10_STATUS_ADDR(Dev)					(FPGA_LINE_TP_10_ADDR(Dev) + 0x0C)
#define FPGA_LINE_TP_10_STATUS_OPT_ADDR(Dev)			(FPGA_LINE_TP_10_ADDR(Dev) + 0x0D)

#define FPGA_LINE_TP_10_LINE_ADDR(Dev, LineIdx) (FPGA_LINE_TP_10_ADDR(Dev) + ((LineIdx) * 0x20) + 0x100)

#define FPGA_LINE_TP_10_LINE_OTU2_ALARMS_ADDR(Dev, LineIdx)   	(FPGA_LINE_TP_10_LINE_ADDR(Dev, LineIdx) + 0x00)
#define FPGA_LINE_TP_10_LINE_OTU2_BIP_ADDR(Dev, LineIdx)   			(FPGA_LINE_TP_10_LINE_ADDR(Dev, LineIdx) + 0x02)
#define FPGA_LINE_TP_10_LINE_OTU2_FEC_CORR_ADDR(Dev, LineIdx)  	(FPGA_LINE_TP_10_LINE_ADDR(Dev, LineIdx) + 0x04)
#define FPGA_LINE_TP_10_LINE_OTU2_FEC_UNCORR_ADDR(Dev, LineIdx)	(FPGA_LINE_TP_10_LINE_ADDR(Dev, LineIdx) + 0x08)

#define FPGA_LINE_TP_10_LINE_CTRL_ADDR(Dev, LineIdx)   	(FPGA_LINE_TP_10_LINE_ADDR(Dev, LineIdx) + 0x0C)

#define FPGA_LINE_TP_10_LINE_PMASTATUS_ADDR(Dev, LineIdx)   (FPGA_LINE_TP_10_LINE_ADDR(Dev, LineIdx) + 0x0E)

#define FPGA_LINE_TP_10_LINE_ODU2_ALARMS_ADDR(Dev, LineIdx)  (FPGA_LINE_TP_10_LINE_ADDR(Dev, LineIdx) + 0x10)
#define FPGA_LINE_TP_10_LINE_ODU2_BIP_ADDR(Dev, LineIdx)   	(FPGA_LINE_TP_10_LINE_ADDR(Dev, LineIdx) + 0x10 + 0x02)

#define FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) (FPGA_LINE_TP_10_ADDR(Dev) + ((ClientIdx) * 0x20) + 0x140)

#define FPGA_LINE_TP_10_CLIENT_MODE_ADDR(Dev, ClientIdx)   			(FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x00)
#define FPGA_LINE_TP_10_CLIENT_PMASTATUS_ADDR(Dev, ClientIdx)   (FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x01)
#define FPGA_LINE_TP_10_CLIENT_PCSSTATUS_ADDR(Dev, ClientIdx)   (FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x02)
#define FPGA_LINE_TP_10_CLIENT_CTRL_ADDR(Dev, ClientIdx)   			(FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x03)

#define FPGA_LINE_TP_10_CLIENT_SDH_RX_STATUS_ADDR(Dev, ClientIdx)	(FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x04)
#define FPGA_LINE_TP_10_CLIENT_SDH_BIP8_ADDR(Dev, ClientIdx)			(FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x06)
#define FPGA_LINE_TP_10_CLIENT_SDH_BIP24_ADDR(Dev, ClientIdx)			(FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x08)

#define FPGA_LINE_TP_10_CLIENT_ETH_TX_PKTS_ADDR(Dev, ClientIdx)	(FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x0C)
#define FPGA_LINE_TP_10_CLIENT_ETH_RX_PKTS_ADDR(Dev, ClientIdx)	(FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x10)
#define FPGA_LINE_TP_10_CLIENT_ETH_TX_OCTS_ADDR(Dev, ClientIdx)	(FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x14)
#define FPGA_LINE_TP_10_CLIENT_ETH_RX_OCTS_ADDR(Dev, ClientIdx)	(FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x18)
#define FPGA_LINE_TP_10_CLIENT_ETH_TX_ERRS_ADDR(Dev, ClientIdx)	(FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x1C)
#define FPGA_LINE_TP_10_CLIENT_ETH_RX_ERRS_ADDR(Dev, ClientIdx)	(FPGA_LINE_TP_10_CLIENT_ADDR(Dev, ClientIdx) + 0x1E)

static void FPGA_LINE_TP_10_WriteByte(FPGA_LINE_TP_10_T *Dev, uint32_t Addr, uint8_t Data)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteByte: 0x%08x 0x%02x", Addr, Data);
	*((uint8_t*)(Addr)) = Data;
}

static void FPGA_LINE_TP_10_WriteCmd(FPGA_LINE_TP_10_T *Dev, uint32_t Cmd, uint8_t Opt1, uint8_t Opt2, uint8_t Opt3)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteCmd: 0x%02x 0x%02x 0x%02x 0x%02x", Cmd, Opt1, Opt2, Opt3);
	*((uint8_t*)FPGA_LINE_TP_10_CMD_OPT1_ADDR(Dev)) = Opt1;
	*((uint8_t*)FPGA_LINE_TP_10_CMD_OPT2_ADDR(Dev)) = Opt2;
	*((uint8_t*)FPGA_LINE_TP_10_CMD_OPT3_ADDR(Dev)) = Opt3;
	*((uint8_t*)FPGA_LINE_TP_10_CMD_ADDR(Dev)) = Cmd;
}

int32_t FPGA_LINE_TP_10_Init(FPGA_LINE_TP_10_T *Dev)
{
	uint8_t Status, SwVersion[4];
	uint32_t i;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
	if (Status == 0)
		goto error;
	
	for (i = 0; i < 4; i++)
	{
		SwVersion[i] = *((char*)(FPGA_LINE_TP_10_ADDR(Dev) + i));
	}
	sprintf(Dev->SwVersion, "%d.%d.%d.%d", SwVersion[1], SwVersion[0], SwVersion[3], SwVersion[2]);
	
	for (i = 0; i < FPGA_LINE_TP_10_LINE_CNT; i++)
	{		
		Dev->Line[i].PMAStatus = Dev->Line[i].__PMAStatus = *((uint8_t*)(FPGA_LINE_TP_10_LINE_PMASTATUS_ADDR(Dev, i)));
		Dev->Line[i].Ctrl = Dev->Line[i].__Ctrl = *((uint8_t*)(FPGA_LINE_TP_10_LINE_CTRL_ADDR(Dev, i)));
		
		Dev->Line[i].OTU2.__Alarms = *((uint16_t*)FPGA_LINE_TP_10_LINE_OTU2_ALARMS_ADDR(Dev, i));
		
		Dev->Line[i].OTU2.__BIP = *((uint16_t*)(FPGA_LINE_TP_10_LINE_OTU2_BIP_ADDR(Dev, i)));
		Dev->Line[i].OTU2.BIP = 0;
		
		Dev->Line[i].OTU2.__FECCorr = *((uint32_t*)(FPGA_LINE_TP_10_LINE_OTU2_FEC_CORR_ADDR(Dev, i)));
		Dev->Line[i].OTU2.FECCorr = 0;
		
		Dev->Line[i].OTU2.__FECUncorr = *((uint16_t*)(FPGA_LINE_TP_10_LINE_OTU2_FEC_UNCORR_ADDR(Dev, i)));
		Dev->Line[i].OTU2.FECUncorr = 0;
		
		Dev->Line[i].ODU2.__Alarms = *((uint16_t*)(FPGA_LINE_TP_10_LINE_ODU2_ALARMS_ADDR(Dev, i)));	
			
		Dev->Line[i].ODU2.__BIP = *((uint16_t*)(FPGA_LINE_TP_10_LINE_ODU2_BIP_ADDR(Dev, i)));
		Dev->Line[i].ODU2.BIP = 0;
	}
	
	for (i = 0; i < FPGA_LINE_TP_10_LINE_CNT; i++)
	{	

		Dev->Client[i].Mode = Dev->Client[i].__Mode = *((uint8_t*)(FPGA_LINE_TP_10_CLIENT_MODE_ADDR(Dev, i)));
		Dev->Client[i].PMAStatus = Dev->Client[i].__PMAStatus = *((uint8_t*)(FPGA_LINE_TP_10_CLIENT_PMASTATUS_ADDR(Dev, i)));
		Dev->Client[i].__PCSStatus = *((uint8_t*)(FPGA_LINE_TP_10_CLIENT_PCSSTATUS_ADDR(Dev, i)));
		Dev->Client[i].Ctrl = Dev->Client[i].__Ctrl = *((uint8_t*)(FPGA_LINE_TP_10_CLIENT_CTRL_ADDR(Dev, i)));
		
		Dev->Client[i].SDH.__RxStatus = *((uint16_t*)FPGA_LINE_TP_10_CLIENT_SDH_RX_STATUS_ADDR(Dev, i));
		
		Dev->Client[i].SDH.__BIP8 = *((uint16_t*)(FPGA_LINE_TP_10_CLIENT_SDH_BIP8_ADDR(Dev, i)));
		Dev->Client[i].SDH.BIP8 = 0;
		
		Dev->Client[i].SDH.__BIP24 = *((uint16_t*)(FPGA_LINE_TP_10_CLIENT_SDH_BIP24_ADDR(Dev, i)));
		Dev->Client[i].SDH.BIP24 = 0;
		
		Dev->Client[i].Eth.__TxPkts = *((uint32_t*)(FPGA_LINE_TP_10_CLIENT_ETH_TX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].Eth.TxPkts = 0;
		
		Dev->Client[i].Eth.__RxPkts = *((uint32_t*)(FPGA_LINE_TP_10_CLIENT_ETH_RX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].Eth.RxPkts = 0;
		
		Dev->Client[i].Eth.__TxOcts = *((uint32_t*)(FPGA_LINE_TP_10_CLIENT_ETH_TX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].Eth.TxOcts = 0;
		
		Dev->Client[i].Eth.__RxOcts = *((uint32_t*)(FPGA_LINE_TP_10_CLIENT_ETH_RX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].Eth.RxOcts = 0;
		
		Dev->Client[i].Eth.__TxErrs = *((uint16_t*)(FPGA_LINE_TP_10_CLIENT_ETH_TX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].Eth.TxErrs = 0;
		
		Dev->Client[i].Eth.__RxErrs = *((uint16_t*)(FPGA_LINE_TP_10_CLIENT_ETH_RX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].Eth.RxErrs = 0;
	}
	
	BSP_DEV_INIT_OK(Dev);
	return 0;

error:
	BSP_DEV_INIT_FAIL(Dev);
	return 0;
}

int32_t FPGA_LINE_TP_10_Update(FPGA_LINE_TP_10_T *Dev)
{
	uint32_t i;
	uint8_t Status;
	uint16_t Temp;
	uint16_t OTU2BIP, OTU2FECUncorr, ODU2BIP;
	uint32_t OTU2FECCorr;
	uint16_t SDHBIP8, SDHBIP24;
	uint16_t EthRxErrs, EthTxErrs;
	uint32_t EthRxPkts, EthRxOcts, EthTxPkts, EthTxOcts;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	Temp = *((uint16_t*)(FPGA_LINE_TP_10_ADDR(Dev) + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;
	
	for (i = 0; i < FPGA_LINE_TP_10_LINE_CNT; i++)
	{
		
		Dev->Line[i].OTU2.__Alarms = *((uint16_t*)FPGA_LINE_TP_10_LINE_OTU2_ALARMS_ADDR(Dev, i));
		Dev->Line[i].__PMAStatus = *((uint8_t*)(FPGA_LINE_TP_10_LINE_PMASTATUS_ADDR(Dev, i)));
		Dev->Line[i].__Ctrl = *((uint8_t*)(FPGA_LINE_TP_10_LINE_CTRL_ADDR(Dev, i)));
		
		OTU2BIP = *((uint16_t*)(FPGA_LINE_TP_10_LINE_OTU2_BIP_ADDR(Dev, i)));
		Dev->Line[i].OTU2.BIP = OTU2BIP - Dev->Line[i].OTU2.__BIP;
		Dev->Line[i].OTU2.__BIP = OTU2BIP;
		
		OTU2FECCorr = *((uint32_t*)(FPGA_LINE_TP_10_LINE_OTU2_FEC_CORR_ADDR(Dev, i)));
		Dev->Line[i].OTU2.FECCorr = OTU2FECCorr - Dev->Line[i].OTU2.__FECCorr;
		Dev->Line[i].OTU2.__FECCorr = OTU2FECCorr;
		
		OTU2FECUncorr = *((uint16_t*)(FPGA_LINE_TP_10_LINE_OTU2_FEC_UNCORR_ADDR(Dev, i)));
		Dev->Line[i].OTU2.FECUncorr = OTU2FECUncorr - Dev->Line[i].OTU2.__FECUncorr;
		Dev->Line[i].OTU2.__FECUncorr = OTU2FECUncorr;
		
		Dev->Line[i].ODU2.__Alarms = *((uint16_t*)(FPGA_LINE_TP_10_LINE_ODU2_ALARMS_ADDR(Dev, i)));
		
		ODU2BIP = *((uint16_t*)(FPGA_LINE_TP_10_LINE_ODU2_BIP_ADDR(Dev, i)));
		Dev->Line[i].ODU2.BIP = ODU2BIP - Dev->Line[i].ODU2.__BIP;
		Dev->Line[i].ODU2.__BIP = ODU2BIP;
		
		if ( Dev->Line[i].Ctrl != Dev->Line[i].__Ctrl )
		{
			FPGA_LINE_TP_10_WriteByte(Dev, FPGA_LINE_TP_10_LINE_CTRL_ADDR(Dev, i), Dev->Line[i].Ctrl);
		}
		
		if ((Dev->Line[i].PMAStatus & FPGA_LINE_TP_10_LINE_PMASTATUS_LOOPBACK_MASK) != (Dev->Line[i].__PMAStatus & FPGA_LINE_TP_10_LINE_PMASTATUS_LOOPBACK_MASK))
		{
			if (Dev->Line[i].PMAStatus & FPGA_LINE_TP_10_LINE_PMASTATUS_LOOPBACK_SERIAL_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x00, i + 2, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x01, i + 2, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x03, 0x00, i + 2, 0x01);
				osDelay(5);
			}
			else if (Dev->Line[i].PMAStatus & FPGA_LINE_TP_10_LINE_PMASTATUS_LOOPBACK_POST_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x03, 0x00, i + 2, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x00, i + 2, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x01, i + 2, 0x01);
				osDelay(5);
			}
			else if (Dev->Line[i].PMAStatus & FPGA_LINE_TP_10_LINE_PMASTATUS_LOOPBACK_PRE_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x03, 0x00, i + 2, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x01, i + 2, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x00, i + 2, 0x01);
				osDelay(5);
			}
			else
			{
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x00, i + 2, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x01, i + 2, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x03, 0x00, i + 2, 0x00);
				osDelay(5);
			}
		}
	}
	
	for (i = 0; i < FPGA_LINE_TP_10_CLIENT_CNT; i++)
	{
		Dev->Client[i].__Mode = *((uint8_t*)(FPGA_LINE_TP_10_CLIENT_MODE_ADDR(Dev, i)));
		Dev->Client[i].__PMAStatus = *((uint8_t*)(FPGA_LINE_TP_10_CLIENT_PMASTATUS_ADDR(Dev, i)));
		Dev->Client[i].__PCSStatus = *((uint8_t*)(FPGA_LINE_TP_10_CLIENT_PCSSTATUS_ADDR(Dev, i)));
		Dev->Client[i].__Ctrl = *((uint8_t*)(FPGA_LINE_TP_10_CLIENT_CTRL_ADDR(Dev, i)));
		
		Dev->Client[i].SDH.__RxStatus = *((uint16_t*)(FPGA_LINE_TP_10_CLIENT_SDH_RX_STATUS_ADDR(Dev, i)));
		
		SDHBIP8 = *((uint16_t*)(FPGA_LINE_TP_10_CLIENT_SDH_BIP8_ADDR(Dev, i)));
		Dev->Client[i].SDH.BIP8 = SDHBIP8 - Dev->Client[i].SDH.__BIP8;
		Dev->Client[i].SDH.__BIP8 = SDHBIP8;
		
		SDHBIP24 = *((uint16_t*)(FPGA_LINE_TP_10_CLIENT_SDH_BIP24_ADDR(Dev, i)));
		Dev->Client[i].SDH.BIP24 = SDHBIP24 - Dev->Client[i].SDH.__BIP24;
		Dev->Client[i].SDH.__BIP24 = SDHBIP24;
		
		EthTxPkts = *((uint32_t*)(FPGA_LINE_TP_10_CLIENT_ETH_TX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].Eth.TxPkts = EthTxPkts - Dev->Client[i].Eth.__TxPkts;
		Dev->Client[i].Eth.__TxPkts = EthTxPkts;
		
		EthRxPkts = *((uint32_t*)(FPGA_LINE_TP_10_CLIENT_ETH_RX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].Eth.RxPkts = EthRxPkts - Dev->Client[i].Eth.__RxPkts;
		Dev->Client[i].Eth.__RxPkts = EthRxPkts;
		
		EthTxOcts = *((uint32_t*)(FPGA_LINE_TP_10_CLIENT_ETH_TX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].Eth.TxOcts = EthTxOcts - Dev->Client[i].Eth.__TxOcts;
		Dev->Client[i].Eth.__TxOcts = EthTxOcts;
		
		EthRxOcts = *((uint32_t*)(FPGA_LINE_TP_10_CLIENT_ETH_RX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].Eth.RxOcts = EthRxOcts - Dev->Client[i].Eth.__RxOcts;
		Dev->Client[i].Eth.__RxOcts = EthRxOcts;
		
		EthTxErrs = *((uint16_t*)(FPGA_LINE_TP_10_CLIENT_ETH_TX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].Eth.TxErrs = EthTxErrs - Dev->Client[i].Eth.__TxErrs;
		Dev->Client[i].Eth.__TxErrs = EthTxErrs;
		
		EthRxErrs = *((uint16_t*)(FPGA_LINE_TP_10_CLIENT_ETH_RX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].Eth.RxErrs = EthRxErrs - Dev->Client[i].Eth.__RxErrs;
		Dev->Client[i].Eth.__RxErrs = EthRxErrs;
		
		if (Dev->Client[i].__Ctrl !=  Dev->Client[i].Ctrl)
		{
			FPGA_LINE_TP_10_WriteByte(Dev, FPGA_LINE_TP_10_CLIENT_CTRL_ADDR(Dev, i), Dev->Client[i].Ctrl);
		}
		
		if (Dev->Client[i].__Mode !=  Dev->Client[i].Mode)
		{
			Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
			if (Status == 0)
				continue;
			FPGA_LINE_TP_10_WriteCmd(Dev, 0x01, 0x00, i, Dev->Client[i].Mode);
			osDelay(5);
		}
		
		if ((Dev->Client[i].PMAStatus & FPGA_LINE_TP_10_CLIENT_PMASTATUS_LOOPBACK_MASK) != (Dev->Client[i].__PMAStatus & FPGA_LINE_TP_10_CLIENT_PMASTATUS_LOOPBACK_MASK))
		{
			if (Dev->Client[i].PMAStatus & FPGA_LINE_TP_10_CLIENT_PMASTATUS_LOOPBACK_SERIAL_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x03, 0x00, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Client[i].PMAStatus & FPGA_LINE_TP_10_CLIENT_PMASTATUS_LOOPBACK_POST_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x01, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Client[i].PMAStatus & FPGA_LINE_TP_10_CLIENT_PMASTATUS_LOOPBACK_PRE_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x00, i, 0x01);
				osDelay(5);
			}
			else
			{
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_LINE_TP_10_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_LINE_TP_10_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
			}
		}
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
}

void FPGA_LINE_TP_10_Deinit(FPGA_LINE_TP_10_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
