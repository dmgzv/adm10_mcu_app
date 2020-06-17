#include "fpga_client_agg_2x10_efec.h"

#define FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev)		((Dev)->BaseAddr)

#define FPGA_CLIENT_AGG_2X10_EFEC_CMD_ADDR(Dev)				(FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x08)
#define FPGA_CLIENT_AGG_2X10_EFEC_CMD_OPT1_ADDR(Dev)		(FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x09)
#define FPGA_CLIENT_AGG_2X10_EFEC_CMD_OPT2_ADDR(Dev)		(FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x0A)
#define FPGA_CLIENT_AGG_2X10_EFEC_CMD_OPT3_ADDR(Dev)		(FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x0B)
#define FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev)			(FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x0C)
#define FPGA_CLIENT_AGG_2X10_EFEC_STATUS_OPT_ADDR(Dev)	(FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x0D)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_TRANS_ADDR(Dev, ClientIdx)  (FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x10 + ((ClientIdx) * 0x04))
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x40 + ((ClientIdx) * 0x20))
#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_ADDR(Dev)   (FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x280)
#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU2_ADDR(Dev)   (FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x310)
#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU1_ADDR(Dev, ODU1Idx)   (FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x200 + ((ODU1Idx) * 0x08))
#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU0_ADDR(Dev, ODU0Idx)   (FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x220 + ((ODU0Idx) * 0x08))

#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_MODE_ADDR(Dev, ClientIdx)				(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_TRANS_ADDR(Dev, ClientIdx) + 0x00)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_PMASTATUS_ADDR(Dev, ClientIdx)	(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_TRANS_ADDR(Dev, ClientIdx) + 0x01)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_PCSSTATUS_ADDR(Dev, ClientIdx)	(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_TRANS_ADDR(Dev, ClientIdx) + 0x02)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_CDRSTATUS_ADDR(Dev, ClientIdx)	(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_TRANS_ADDR(Dev, ClientIdx) + 0x02)

#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_CTRL_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x00)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STATUS_ADDR(Dev, ClientIdx) (FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x01)

#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_SDH_RX_STATUS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x02)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_SDH_BIP8_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x04)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_SDH_BIP24_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x06)

#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_TX_PKTS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x08)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_RX_PKTS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x0C)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_TX_OCTS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x10)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_RX_OCTS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x14)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_TX_ERRS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x18)
#define FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_RX_ERRS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x1A)

#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_ALARMS_ADDR(Dev) 			(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_ADDR(Dev) + 0x00)
#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_BIP_ADDR(Dev)   			(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_ADDR(Dev) + 0x02)
#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_FEC_CORR_ADDR(Dev)  	(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_ADDR(Dev) + 0x04)
#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_FEC_UNCORR_ADDR(Dev)  (FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_ADDR(Dev) + 0x08)
#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_CTRL_ADDR(Dev)   			(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_ADDR(Dev) + 0x0C)

#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU2_ALARMS_ADDR(Dev) 	(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU2_ADDR(Dev) + 0x00)
#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU2_BIP_ADDR(Dev)   	(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU2_ADDR(Dev) + 0x02)

#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU1_ALARMS_ADDR(Dev, ODU1Idx) (FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU1_ADDR(Dev, ODU1Idx) + 0x00)
#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU1_BIP_ADDR(Dev, ODU1Idx)   (FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU1_ADDR(Dev, ODU1Idx) + 0x02)

#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU0_ALARMS_ADDR(Dev, ODU0Idx) (FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU0_ADDR(Dev, ODU0Idx) + 0x00)
#define FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU0_BIP_ADDR(Dev, ODU0Idx)   (FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU0_ADDR(Dev, ODU0Idx) + 0x02)

static void FPGA_CLIENT_AGG_2X10_EFEC_WriteByte(FPGA_CLIENT_AGG_2X10_EFEC_T *Dev, uint32_t Addr, uint8_t Data)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteByte: 0x%08x 0x%02x", Addr, Data);
	*((uint8_t*)(Addr)) = Data;
}

static void FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(FPGA_CLIENT_AGG_2X10_EFEC_T *Dev, uint32_t Cmd, uint8_t Opt1, uint8_t Opt2, uint8_t Opt3)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteCmd: 0x%02x 0x%02x 0x%02x 0x%02x", Cmd, Opt1, Opt2, Opt3);
	*((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_CMD_OPT1_ADDR(Dev)) = Opt1;
	*((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_CMD_OPT2_ADDR(Dev)) = Opt2;
	*((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_CMD_OPT3_ADDR(Dev)) = Opt3;
	*((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_CMD_ADDR(Dev)) = Cmd;
}

int32_t FPGA_CLIENT_AGG_2X10_EFEC_Init(FPGA_CLIENT_AGG_2X10_EFEC_T *Dev)
{
	uint32_t i;
	uint8_t Status, SwVersion[4];
	uint16_t Temp;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
	if (Status == 0)
		goto error;
	
	for (i = 0; i < 4; i++)
	{
		SwVersion[i] = *((char*)(FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + i));
	}
	sprintf(Dev->SwVersion, "%d.%d.%d.%d", SwVersion[1], SwVersion[0], SwVersion[3], SwVersion[2]);
	
	Temp = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;
	
	Dev->Line.OTU2Ctrl = Dev->Line.__OTU2Ctrl = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_CTRL_ADDR(Dev)));
	
	Dev->Line.__OTU2Alarms = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_ALARMS_ADDR(Dev)));			
	Dev->Line.__OTU2BIP =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_BIP_ADDR(Dev)));
	Dev->Line.OTU2BIP = 0;
	Dev->Line.__OTU2FECCorr =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_FEC_CORR_ADDR(Dev)));
	Dev->Line.OTU2FECCorr = 0;
	Dev->Line.__OTU2FECUncorr =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_FEC_UNCORR_ADDR(Dev)));
	Dev->Line.OTU2FECUncorr = 0;
	
	Dev->Line.__ODU2Alarms = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU2_ALARMS_ADDR(Dev)));			
	Dev->Line.__ODU2BIP =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU2_BIP_ADDR(Dev)));
	Dev->Line.ODU2BIP = 0;
	
	for (i = 0; i < FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU1_CNT; i++)
	{
		Dev->Line.__ODU1Alarms[i] = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU1_ALARMS_ADDR(Dev, i)));			
		Dev->Line.__ODU1BIP[i] =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU1_BIP_ADDR(Dev, i)));
		Dev->Line.ODU1BIP[i] = 0;
	}
	
	for (i = 0; i < FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU0_CNT; i++)
	{
		Dev->Line.__ODU0Alarms[i] = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU0_ALARMS_ADDR(Dev, i)));			
		Dev->Line.__ODU0BIP[i] =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU0_BIP_ADDR(Dev, i)));
		Dev->Line.ODU0BIP[i] = 0;
	}
	
	for (i = 0; i < FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_CNT; i++)
	{
		Dev->Client[i].Mode = Dev->Client[i].__Mode = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_MODE_ADDR(Dev, i)));
		Dev->Client[i].PMAStatus = Dev->Client[i].__PMAStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_PMASTATUS_ADDR(Dev, i)));
		Dev->Client[i].__PCSStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_PCSSTATUS_ADDR(Dev, i)));
		Dev->Client[i].__CDRStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_CDRSTATUS_ADDR(Dev, i)));
		Dev->Client[i].Ctrl = Dev->Client[i].__Ctrl = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_CTRL_ADDR(Dev, i)));
		
		Dev->Client[i].__SDHRxStatus = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_SDH_RX_STATUS_ADDR(Dev, i)));
		
		Dev->Client[i].__SDHBIP8 = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_SDH_BIP8_ADDR(Dev, i)));
		Dev->Client[i].SDHBIP8 = 0;
		
		Dev->Client[i].__SDHBIP24 = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_SDH_BIP24_ADDR(Dev, i)));
		Dev->Client[i].SDHBIP24 = 0;
		
		Dev->Client[i].__EthTxPkts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_TX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].EthTxPkts = 0;
		
		Dev->Client[i].__EthRxPkts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_RX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].EthRxPkts = 0;
		
		Dev->Client[i].__EthTxOcts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_TX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].EthTxOcts = 0;
		
		Dev->Client[i].__EthRxOcts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_RX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].EthRxOcts = 0;
		
		Dev->Client[i].__EthTxErrs = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_TX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].EthTxErrs = 0;
		
		Dev->Client[i].__EthRxErrs = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_RX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].EthRxErrs = 0;
	}
	
	BSP_DEV_INIT_OK(Dev);
	return 0;

error:
	BSP_DEV_INIT_FAIL(Dev);
	return 0;
}

int32_t FPGA_CLIENT_AGG_2X10_EFEC_Update(FPGA_CLIENT_AGG_2X10_EFEC_T *Dev)
{
	uint32_t i;
	uint8_t Status;
	uint16_t Temp, SDHBIP8, SDHBIP24, EthRxErrs, EthTxErrs;
	uint16_t OTU2BIP, ODU2BIP, ODU1BIP, ODU0BIP;
	uint32_t OTU2FECCorr, OTU2FECUncorr, EthRxPkts, EthRxOcts, EthTxPkts, EthTxOcts;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	Temp = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_ADDR(Dev) + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;

	Dev->Line.__OTU2Ctrl = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_CTRL_ADDR(Dev)));
	
	Dev->Line.__OTU2Alarms = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_ALARMS_ADDR(Dev)));			
	OTU2BIP =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_BIP_ADDR(Dev)));
	Dev->Line.OTU2BIP =  OTU2BIP - Dev->Line.__OTU2BIP;
	Dev->Line.__OTU2BIP = OTU2BIP;
	OTU2FECCorr =  *((uint32_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_FEC_CORR_ADDR(Dev)));
	Dev->Line.OTU2FECCorr = OTU2FECCorr - Dev->Line.__OTU2FECCorr;
	Dev->Line.__OTU2FECCorr = OTU2FECCorr;
	OTU2FECUncorr =  *((uint32_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_FEC_UNCORR_ADDR(Dev)));
	Dev->Line.OTU2FECUncorr = OTU2FECUncorr - Dev->Line.__OTU2FECUncorr;
	Dev->Line.__OTU2FECUncorr = OTU2FECUncorr;
	
	Dev->Line.__ODU2Alarms = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU2_ALARMS_ADDR(Dev)));
	ODU2BIP =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU2_BIP_ADDR(Dev)));
	Dev->Line.ODU2BIP =  ODU2BIP - Dev->Line.__ODU2BIP;
	Dev->Line.__ODU2BIP = ODU2BIP;
	
	for (i = 0; i < FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU1_CNT; i++)
	{
		Dev->Line.__ODU1Alarms[i] = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU1_ALARMS_ADDR(Dev, i)));			
		ODU1BIP =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU1_BIP_ADDR(Dev, i)));
		Dev->Line.ODU1BIP[i] = ODU1BIP - Dev->Line.__ODU1BIP[i];
		Dev->Line.__ODU1BIP[i] = ODU1BIP;
	}
	
	for (i = 0; i < FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU0_CNT; i++)
	{
		Dev->Line.__ODU0Alarms[i] = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU0_ALARMS_ADDR(Dev, i)));			
		ODU0BIP =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_LINE_ODU0_BIP_ADDR(Dev, i)));
		Dev->Line.ODU0BIP[i] = ODU0BIP - Dev->Line.__ODU0BIP[i];
		Dev->Line.__ODU0BIP[i] = ODU0BIP;
	}
	
	if (Dev->Line.__OTU2Ctrl != Dev->Line.OTU2Ctrl)
	{
		FPGA_CLIENT_AGG_2X10_EFEC_WriteByte(Dev, FPGA_CLIENT_AGG_2X10_EFEC_LINE_OTU2_CTRL_ADDR(Dev), Dev->Line.OTU2Ctrl);
	}
	
	for (i = 0; i < FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_CNT; i++)
	{
		Dev->Client[i].__Mode = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_MODE_ADDR(Dev, i)));
		Dev->Client[i].__PMAStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_PMASTATUS_ADDR(Dev, i)));
		Dev->Client[i].__PCSStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_PCSSTATUS_ADDR(Dev, i)));
		Dev->Client[i].__CDRStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_CDRSTATUS_ADDR(Dev, i)));
		Dev->Client[i].__Ctrl = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_CTRL_ADDR(Dev, i)));
		
		Dev->Client[i].__SDHRxStatus = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_SDH_RX_STATUS_ADDR(Dev, i)));
		
		SDHBIP8 = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_SDH_BIP8_ADDR(Dev, i)));
		Dev->Client[i].SDHBIP8 = SDHBIP8 - Dev->Client[i].__SDHBIP8;
		Dev->Client[i].__SDHBIP8 = SDHBIP8;
		
		SDHBIP24 = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_SDH_BIP24_ADDR(Dev, i)));
		Dev->Client[i].SDHBIP24 = SDHBIP24 - Dev->Client[i].__SDHBIP24;
		Dev->Client[i].__SDHBIP24 = SDHBIP24;
		
		EthTxPkts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_TX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].EthTxPkts = EthTxPkts - Dev->Client[i].__EthTxPkts;
		Dev->Client[i].__EthTxPkts = EthTxPkts;
		
		EthRxPkts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_RX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].EthRxPkts = EthRxPkts - Dev->Client[i].__EthRxPkts;
		Dev->Client[i].__EthRxPkts = EthRxPkts;
		
		EthTxOcts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_TX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].EthTxOcts = EthTxOcts - Dev->Client[i].__EthTxOcts;
		Dev->Client[i].__EthTxOcts = EthTxOcts;
		
		EthRxOcts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_RX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].EthRxOcts = EthRxOcts - Dev->Client[i].__EthRxOcts;
		Dev->Client[i].__EthRxOcts = EthRxOcts;
		
		EthTxErrs = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_TX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].EthTxErrs = EthTxErrs - Dev->Client[i].__EthTxErrs;
		Dev->Client[i].__EthTxErrs = EthTxErrs;
		
		EthRxErrs = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_ETH_RX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].EthRxErrs = EthRxErrs - Dev->Client[i].__EthRxErrs;
		Dev->Client[i].__EthRxErrs = EthRxErrs;
				
		if (Dev->Client[i].__Ctrl !=  Dev->Client[i].Ctrl)
		{
			FPGA_CLIENT_AGG_2X10_EFEC_WriteByte(Dev, FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_CTRL_ADDR(Dev, i), Dev->Client[i].Ctrl);
		}
		
		if (Dev->Client[i].__Mode !=  Dev->Client[i].Mode)
		{
			Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
			if (Status == 0)
				continue;
			FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x01, 0x00, i, Dev->Client[i].Mode);
			osDelay(5);
		}
		
		if ((Dev->Client[i].PMAStatus & FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_PMASTATUS_LOOPBACK_MASK) != (Dev->Client[i].__PMAStatus & FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_PMASTATUS_LOOPBACK_MASK))
		{
			if (Dev->Client[i].PMAStatus & FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_PMASTATUS_LOOPBACK_SERIAL_MASK)
			{
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x03, 0x00, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Client[i].PMAStatus & FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_PMASTATUS_LOOPBACK_POST_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x01, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Client[i].PMAStatus & FPGA_CLIENT_AGG_2X10_EFEC_CLIENT_PMASTATUS_LOOPBACK_PRE_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x00, i, 0x01);
				osDelay(5);
			}
			else
			{
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_EFEC_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_EFEC_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
			}
		}
	}

	BSP_DEV_UPDATE_OK(Dev);
	return 0;
}

void FPGA_CLIENT_AGG_2X10_EFEC_Deinit(FPGA_CLIENT_AGG_2X10_EFEC_T *Dev)
{
	if (!Dev)
		return;
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "Deinit");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
}
