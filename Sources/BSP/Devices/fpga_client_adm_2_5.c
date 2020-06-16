#include "fpga_client_adm_2_5.h"

#define FPGA_CLIENT_ADM_2_5_ADDR(Dev)		((Dev)->BaseAddr)

#define FPGA_CLIENT_ADM_2_5_CMD_ADDR(Dev)				(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x08)
#define FPGA_CLIENT_ADM_2_5_CMD_OPT1_ADDR(Dev)		(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x09)
#define FPGA_CLIENT_ADM_2_5_CMD_OPT2_ADDR(Dev)		(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x0A)
#define FPGA_CLIENT_ADM_2_5_CMD_OPT3_ADDR(Dev)		(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x0B)
#define FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev)			(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x0C)
#define FPGA_CLIENT_ADM_2_5_STATUS_OPT_ADDR(Dev)	(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x0D)

#define FPGA_CLIENT_ADM_2_5_LINE_OTU1_ADDR(Dev, LineIdx)  						(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + ((LineIdx) * 0x40) + 0x100)

#define FPGA_CLIENT_ADM_2_5_LINE_OTU1_ALARMS_ADDR(Dev, LineIdx)   		(FPGA_CLIENT_ADM_2_5_LINE_OTU1_ADDR(Dev, LineIdx) + 0x00)
#define FPGA_CLIENT_ADM_2_5_LINE_OTU1_BIP_ADDR(Dev, LineIdx)   			(FPGA_CLIENT_ADM_2_5_LINE_OTU1_ADDR(Dev, LineIdx) + 0x02)
#define FPGA_CLIENT_ADM_2_5_LINE_OTU1_FEC_CORR_ADDR(Dev, LineIdx)  	(FPGA_CLIENT_ADM_2_5_LINE_OTU1_ADDR(Dev, LineIdx) + 0x04)
#define FPGA_CLIENT_ADM_2_5_LINE_OTU1_FEC_UNCORR_ADDR(Dev, LineIdx)	(FPGA_CLIENT_ADM_2_5_LINE_OTU1_ADDR(Dev, LineIdx) + 0x08)
#define FPGA_CLIENT_ADM_2_5_LINE_OTU1_CTRL_ADDR(Dev, LineIdx)   			(FPGA_CLIENT_ADM_2_5_LINE_OTU1_ADDR(Dev, LineIdx) + 0x0C)

#define FPGA_CLIENT_ADM_2_5_LINE_TRANS_ADDR(Dev, LineIdx)  					(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + ((LineIdx) * 0x40) + 0x10E)

#define FPGA_CLIENT_ADM_2_5_LINE_TRANS_PMASTATUS_ADDR(Dev, LineIdx)  (FPGA_CLIENT_ADM_2_5_LINE_TRANS_ADDR(Dev, LineIdx) + 0x00)

#define FPGA_CLIENT_ADM_2_5_LINE_ODU1_ADDR(Dev, LineIdx)  				(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + ((LineIdx) * 0x40) + 0x110)

#define FPGA_CLIENT_ADM_2_5_LINE_ODU1_ALARMS_ADDR(Dev, LineIdx)  (FPGA_CLIENT_ADM_2_5_LINE_ODU1_ADDR(Dev, LineIdx) + 0x00)
#define FPGA_CLIENT_ADM_2_5_LINE_ODU1_BIP_ADDR(Dev, LineIdx)   	(FPGA_CLIENT_ADM_2_5_LINE_ODU1_ADDR(Dev, LineIdx) + 0x02)

#define FPGA_CLIENT_ADM_2_5_LINE_ODU0_ADDR(Dev, LineIdx, ODU0Idx) 				(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + ((LineIdx) * 0x40) + 0x118 + ((ODU0Idx) * 0x08))

#define FPGA_CLIENT_ADM_2_5_LINE_ODU0_ALARMS_ADDR(Dev, LineIdx, ODU0Idx) (FPGA_CLIENT_ADM_2_5_LINE_ODU0_ADDR(Dev, LineIdx, ODU0Idx) + 0x00)
#define FPGA_CLIENT_ADM_2_5_LINE_ODU0_BIP_ADDR(Dev, LineIdx, ODU0Idx)   	(FPGA_CLIENT_ADM_2_5_LINE_ODU0_ADDR(Dev, LineIdx, ODU0Idx) + 0x02)

#define FPGA_CLIENT_ADM_2_5_CLIENT_TRANS_ADDR(Dev, ClientIdx)  (FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x260 + ((ClientIdx) * 0x04))

#define FPGA_CLIENT_ADM_2_5_CLIENT_MODE_ADDR(Dev, ClientIdx)				(FPGA_CLIENT_ADM_2_5_CLIENT_TRANS_ADDR(Dev, ClientIdx) + 0x00)
#define FPGA_CLIENT_ADM_2_5_CLIENT_PMASTATUS_ADDR(Dev, ClientIdx)	(FPGA_CLIENT_ADM_2_5_CLIENT_TRANS_ADDR(Dev, ClientIdx) + 0x01)
#define FPGA_CLIENT_ADM_2_5_CLIENT_PCSSTATUS_ADDR(Dev, ClientIdx)	(FPGA_CLIENT_ADM_2_5_CLIENT_TRANS_ADDR(Dev, ClientIdx) + 0x02)
#define FPGA_CLIENT_ADM_2_5_CLIENT_CDRSTATUS_ADDR(Dev, ClientIdx)	(FPGA_CLIENT_ADM_2_5_CLIENT_TRANS_ADDR(Dev, ClientIdx) + 0x02)

#define FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x280 + ((ClientIdx) * 0x20))

#define FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x00)
#define FPGA_CLIENT_ADM_2_5_CLIENT_STATUS_ADDR(Dev, ClientIdx) (FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x01)

#define FPGA_CLIENT_ADM_2_5_CLIENT_SDH_RX_STATUS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x02)
#define FPGA_CLIENT_ADM_2_5_CLIENT_SDH_BIP8_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x04)
#define FPGA_CLIENT_ADM_2_5_CLIENT_SDH_BIP24_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x06)

#define FPGA_CLIENT_ADM_2_5_CLIENT_ETH_TX_PKTS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x08)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ETH_RX_PKTS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x0C)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ETH_TX_OCTS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x10)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ETH_RX_OCTS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x14)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ETH_TX_ERRS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x18)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ETH_RX_ERRS_ADDR(Dev, ClientIdx)   (FPGA_CLIENT_ADM_2_5_CLIENT_STAT_ADDR(Dev, ClientIdx) + 0x1A)

#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_ADDR(Dev, ClientIdx)								(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x240 + ((ClientIdx) * 0x08))
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_MODE_ADDR(Dev, ClientIdx)					(FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_ADDR(Dev, ClientIdx) + 0x00)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_STATUS_ADDR(Dev, ClientIdx)				(FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_ADDR(Dev, ClientIdx) + 0x01)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_PRIMARY_ODUk_ADDR(Dev, ClientIdx)	(FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_ADDR(Dev, ClientIdx) + 0x02)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_PROTECT_ODUk_ADDR(Dev, ClientIdx)	(FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_ADDR(Dev, ClientIdx) + 0x03)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_WTR_ADDR(Dev, ClientIdx)						(FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_ADDR(Dev, ClientIdx) + 0x04)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_HO_ADDR(Dev, ClientIdx)						(FPGA_CLIENT_ADM_2_5_CLIENT_ODU0_PG_ADDR(Dev, ClientIdx) + 0x05)

#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_ADDR(Dev, ClientIdx)								(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x220 + ((ClientIdx) * 0x08))
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_ADDR(Dev, ClientIdx)					(FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_ADDR(Dev, ClientIdx) + 0x00)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_STATUS_ADDR(Dev, ClientIdx)				(FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_ADDR(Dev, ClientIdx) + 0x01)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_PRIMARY_ODUk_ADDR(Dev, ClientIdx)	(FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_ADDR(Dev, ClientIdx) + 0x02)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_PROTECT_ODUk_ADDR(Dev, ClientIdx)	(FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_ADDR(Dev, ClientIdx) + 0x03)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_WTR_ADDR(Dev, ClientIdx)						(FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_ADDR(Dev, ClientIdx) + 0x04)
#define FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_HO_ADDR(Dev, ClientIdx)						(FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_ADDR(Dev, ClientIdx) + 0x05)

#define FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_ADDR(Dev, ODU1FramerIdx)	(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x200 + (ODU1FramerIdx))
#define FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_MUX_ADDR(Dev, ODU1FramerIdx)						(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x204 + (ODU1FramerIdx))
#define FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_ADDR(Dev, ODU1FramerIdx, ODU0Idx)	(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + (ODU0Idx))

#define FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_ADDR(Dev, ODU0FramerIdx)	(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + (ODU0FramerIdx))

static void FPGA_CLIENT_ADM_2_5_WriteByte(FPGA_CLIENT_ADM_2_5_T *Dev, uint32_t Addr, uint8_t Data)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteByte: 0x%08x 0x%02x", Addr, Data);
	*((uint8_t*)(Addr)) = Data;
}

static void FPGA_CLIENT_ADM_2_5_WriteCmd(FPGA_CLIENT_ADM_2_5_T *Dev, uint32_t Cmd, uint8_t Opt1, uint8_t Opt2, uint8_t Opt3)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteCmd: 0x%02x 0x%02x 0x%02x 0x%02x", Cmd, Opt1, Opt2, Opt3);
	*((uint8_t*)FPGA_CLIENT_ADM_2_5_CMD_OPT1_ADDR(Dev)) = Opt1;
	*((uint8_t*)FPGA_CLIENT_ADM_2_5_CMD_OPT2_ADDR(Dev)) = Opt2;
	*((uint8_t*)FPGA_CLIENT_ADM_2_5_CMD_OPT3_ADDR(Dev)) = Opt3;
	*((uint8_t*)FPGA_CLIENT_ADM_2_5_CMD_ADDR(Dev)) = Cmd;
}

int32_t FPGA_CLIENT_ADM_2_5_Init(FPGA_CLIENT_ADM_2_5_T *Dev)
{
	uint32_t i, j;
	uint8_t Status, SwVersion[4];
	uint16_t Temp;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
	if (Status == 0)
		goto error;
	
	for (i = 0; i < 4; i++)
	{
		SwVersion[i] = *((char*)(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + i));
	}
	sprintf(Dev->SwVersion, "%d.%d.%d.%d", SwVersion[1], SwVersion[0], SwVersion[3], SwVersion[2]);
	
	Temp = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;
	
	for (i = 0; i < FPGA_CLIENT_ADM_2_5_CLIENT_CNT; i++)
	{		
		Dev->Client[i].Mode = Dev->Client[i].__Mode = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_MODE_ADDR(Dev, i)));
		Dev->Client[i].PMAStatus = Dev->Client[i].__PMAStatus = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_PMASTATUS_ADDR(Dev, i)));
		Dev->Client[i].__PCSStatus = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_PCSSTATUS_ADDR(Dev, i)));
		Dev->Client[i].__CDRStatus = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_CDRSTATUS_ADDR(Dev, i)));
		Dev->Client[i].Ctrl = Dev->Client[i].__Ctrl = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_ADDR(Dev, i)));
		
		Dev->Client[i].__SDHRxStatus = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_SDH_RX_STATUS_ADDR(Dev, i)));
		
		Dev->Client[i].__SDHBIP8 = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_SDH_BIP8_ADDR(Dev, i)));
		Dev->Client[i].SDHBIP8 = 0;
		
		Dev->Client[i].__SDHBIP24 = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_SDH_BIP24_ADDR(Dev, i)));
		Dev->Client[i].SDHBIP24 = 0;
		
		Dev->Client[i].__EthTxPkts = *((uint32_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_TX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].EthTxPkts = 0;
		
		Dev->Client[i].__EthRxPkts = *((uint32_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_RX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].EthRxPkts = 0;
		
		Dev->Client[i].__EthTxOcts = *((uint32_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_TX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].EthTxOcts = 0;
		
		Dev->Client[i].__EthRxOcts = *((uint32_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_RX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].EthRxOcts = 0;
		
		Dev->Client[i].__EthTxErrs = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_TX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].EthTxErrs = 0;
		
		Dev->Client[i].__EthRxErrs = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_RX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].EthRxErrs = 0;
		
		Dev->Client[i].ODU1PG.Mode = Dev->Client[i].ODU1PG.__Mode = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_ADDR(Dev, i));
		Dev->Client[i].ODU1PG.__Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_STATUS_ADDR(Dev, i));
		Dev->Client[i].ODU1PG.PrimaryODUk = Dev->Client[i].ODU1PG.__PrimaryODUk = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_PRIMARY_ODUk_ADDR(Dev, i));
		Dev->Client[i].ODU1PG.ProtectODUk = Dev->Client[i].ODU1PG.__ProtectODUk = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_PROTECT_ODUk_ADDR(Dev, i));
		Dev->Client[i].ODU1PG.WTRTmr = Dev->Client[i].ODU1PG.__WTRTmr = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_WTR_ADDR(Dev, i));
		Dev->Client[i].ODU1PG.HOTmr = Dev->Client[i].ODU1PG.__HOTmr = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_HO_ADDR(Dev, i));
	}
	
	for (i = 0; i < FPGA_CLIENT_ADM_2_5_LINE_CNT; i++)
	{		
		Dev->Line[i].PMAStatus = Dev->Line[i].__PMAStatus = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_LINE_TRANS_PMASTATUS_ADDR(Dev, i)));
		Dev->Line[i].Ctrl = Dev->Line[i].__Ctrl = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_LINE_OTU1_CTRL_ADDR(Dev, i)));
		
		Dev->Line[i].__OTU1Alarms = *((uint16_t*)FPGA_CLIENT_ADM_2_5_LINE_OTU1_ALARMS_ADDR(Dev, i));
		
		Dev->Line[i].__OTU1BIP = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_LINE_OTU1_BIP_ADDR(Dev, i)));
		Dev->Line[i].OTU1BIP = 0;
		
		Dev->Line[i].__OTU1FECCorr = *((uint32_t*)(FPGA_CLIENT_ADM_2_5_LINE_OTU1_FEC_CORR_ADDR(Dev, i)));
		Dev->Line[i].OTU1FECCorr = 0;
		
		Dev->Line[i].__OTU1FECUncorr = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_LINE_OTU1_FEC_UNCORR_ADDR(Dev, i)));
		Dev->Line[i].OTU1FECUncorr = 0;
		
		Dev->Line[i].__ODU1Alarms = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_LINE_ODU1_ALARMS_ADDR(Dev, i)));	
			
		Dev->Line[i].__ODU1BIP = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_LINE_ODU1_BIP_ADDR(Dev, i)));
		Dev->Line[i].ODU1BIP = 0;
		
		for (j = 0; j < FPGA_CLIENT_ADM_2_5_LINE_ODU0_CNT; j++)
		{
			Dev->Line[i].__ODU0Alarms[j] = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_LINE_ODU0_ALARMS_ADDR(Dev, i, j)));
			
			Dev->Line[i].__ODU0BIP[j] = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_LINE_ODU0_BIP_ADDR(Dev, i, j)));
			Dev->Line[i].ODU0BIP[j] = 0;
		}
	}
	
	for (i = 0; i < FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_CNT; i++)
	{
		Dev->ODU0Framer[i].OPU0PayloadType = Dev->ODU0Framer[i].__OPU0PayloadType = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_ADDR(Dev, i)));
	}
	
	for (i = 0; i < FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_CNT; i++)
	{
		Dev->ODU1Framer[i].OPU1PayloadType = Dev->ODU1Framer[i].__OPU1PayloadType = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_ADDR(Dev, i)));
		Dev->ODU1Framer[i].OPU1Mux = Dev->ODU1Framer[i].__OPU1Mux = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_MUX_ADDR(Dev, i)));
		for (j = 0; j < 2; j++)
		{
			Dev->ODU1Framer[i].ODU0Mux[j] = Dev->ODU1Framer[i].__ODU0Mux[j] = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_ADDR(Dev, i, j)));
		}
	}
	
	BSP_DEV_INIT_OK(Dev);
	return 0;

error:
	BSP_DEV_INIT_FAIL(Dev);
	return 0;
}

int32_t FPGA_CLIENT_ADM_2_5_Update(FPGA_CLIENT_ADM_2_5_T *Dev)
{
	uint32_t i, j;
	uint8_t Status;
	uint16_t Temp, SDHBIP8, SDHBIP24, EthRxErrs, EthTxErrs;
	uint32_t EthRxPkts, EthRxOcts, EthTxPkts, EthTxOcts;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	Temp = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_ADDR(Dev) + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;
	
	for (i = 0; i < FPGA_CLIENT_ADM_2_5_CLIENT_CNT; i++)
	{
		Dev->Client[i].__Mode = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_MODE_ADDR(Dev, i)));
		Dev->Client[i].__PMAStatus = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_PMASTATUS_ADDR(Dev, i)));
		Dev->Client[i].__PCSStatus = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_PCSSTATUS_ADDR(Dev, i)));
		Dev->Client[i].__CDRStatus = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_CDRSTATUS_ADDR(Dev, i)));
		Dev->Client[i].__Ctrl = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_ADDR(Dev, i)));
		
		Dev->Client[i].__SDHRxStatus = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_SDH_RX_STATUS_ADDR(Dev, i)));
		
		SDHBIP8 = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_SDH_BIP8_ADDR(Dev, i)));
		Dev->Client[i].SDHBIP8 = SDHBIP8 - Dev->Client[i].__SDHBIP8;
		Dev->Client[i].__SDHBIP8 = SDHBIP8;
		
		SDHBIP24 = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_SDH_BIP24_ADDR(Dev, i)));
		Dev->Client[i].SDHBIP24 = SDHBIP24 - Dev->Client[i].__SDHBIP24;
		Dev->Client[i].__SDHBIP24 = SDHBIP24;
		
		EthTxPkts = *((uint32_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_TX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].EthTxPkts = EthTxPkts - Dev->Client[i].__EthTxPkts;
		Dev->Client[i].__EthTxPkts = EthTxPkts;
		
		EthRxPkts = *((uint32_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_RX_PKTS_ADDR(Dev, i)));
		Dev->Client[i].EthRxPkts = EthRxPkts - Dev->Client[i].__EthRxPkts;
		Dev->Client[i].__EthRxPkts = EthRxPkts;
		
		EthTxOcts = *((uint32_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_TX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].EthTxOcts = EthTxOcts - Dev->Client[i].__EthTxOcts;
		Dev->Client[i].__EthTxOcts = EthTxOcts;
		
		EthRxOcts = *((uint32_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_RX_OCTS_ADDR(Dev, i)));
		Dev->Client[i].EthRxOcts = EthRxOcts - Dev->Client[i].__EthRxOcts;
		Dev->Client[i].__EthRxOcts = EthRxOcts;
		
		EthTxErrs = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_TX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].EthTxErrs = EthTxErrs - Dev->Client[i].__EthTxErrs;
		Dev->Client[i].__EthTxErrs = EthTxErrs;
		
		EthRxErrs = *((uint16_t*)(FPGA_CLIENT_ADM_2_5_CLIENT_ETH_RX_ERRS_ADDR(Dev, i)));
		Dev->Client[i].EthRxErrs = EthRxErrs - Dev->Client[i].__EthRxErrs;
		Dev->Client[i].__EthRxErrs = EthRxErrs;
				
		if (Dev->Client[i].__Ctrl !=  Dev->Client[i].Ctrl)
		{
			FPGA_CLIENT_ADM_2_5_WriteByte(Dev, FPGA_CLIENT_ADM_2_5_CLIENT_CTRL_ADDR(Dev, i), Dev->Client[i].Ctrl);
		}
		
		if (Dev->Client[i].__Mode !=  Dev->Client[i].Mode)
		{
			Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
			if (Status == 0)
				continue;
			FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x01, 0x00, i, Dev->Client[i].Mode);
			osDelay(5);
		}
		
		if ((Dev->Client[i].PMAStatus & FPGA_CLIENT_ADM_2_5_CLIENT_PMASTATUS_LOOPBACK_MASK) != (Dev->Client[i].__PMAStatus & FPGA_CLIENT_ADM_2_5_CLIENT_PMASTATUS_LOOPBACK_MASK))
		{
			if (Dev->Client[i].PMAStatus & FPGA_CLIENT_ADM_2_5_CLIENT_PMASTATUS_LOOPBACK_SERIAL_MASK)
			{
				Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x03, 0x00, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Client[i].PMAStatus & FPGA_CLIENT_ADM_2_5_CLIENT_PMASTATUS_LOOPBACK_POST_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x02, 0x01, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Client[i].PMAStatus & FPGA_CLIENT_ADM_2_5_CLIENT_PMASTATUS_LOOPBACK_PRE_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x02, 0x00, i, 0x01);
				osDelay(5);
			}
			else
			{
				Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_ADM_2_5_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
			}
		}
		
		Dev->Client[i].ODU1PG.__Mode = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_ADDR(Dev, i));
		Dev->Client[i].ODU1PG.__Status = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_STATUS_ADDR(Dev, i));
		Dev->Client[i].ODU1PG.__PrimaryODUk = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_PRIMARY_ODUk_ADDR(Dev, i));
		Dev->Client[i].ODU1PG.__ProtectODUk = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_PROTECT_ODUk_ADDR(Dev, i));
		Dev->Client[i].ODU1PG.__WTRTmr = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_WTR_ADDR(Dev, i));
		Dev->Client[i].ODU1PG.__HOTmr = *((uint8_t*)FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_HO_ADDR(Dev, i));
		
		if (Dev->Client[i].ODU1PG.Mode != Dev->Client[i].ODU1PG.__Mode)
			FPGA_CLIENT_ADM_2_5_WriteByte(Dev, FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_MODE_ADDR(Dev, i), Dev->Client[i].ODU1PG.Mode);		
		if (Dev->Client[i].ODU1PG.PrimaryODUk != Dev->Client[i].ODU1PG.__PrimaryODUk)
			FPGA_CLIENT_ADM_2_5_WriteByte(Dev, FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_PRIMARY_ODUk_ADDR(Dev, i), Dev->Client[i].ODU1PG.PrimaryODUk);
		if (Dev->Client[i].ODU1PG.ProtectODUk != Dev->Client[i].ODU1PG.__ProtectODUk)
			FPGA_CLIENT_ADM_2_5_WriteByte(Dev, FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_PROTECT_ODUk_ADDR(Dev, i), Dev->Client[i].ODU1PG.ProtectODUk);
		if (Dev->Client[i].ODU1PG.WTRTmr != Dev->Client[i].ODU1PG.__WTRTmr)
			FPGA_CLIENT_ADM_2_5_WriteByte(Dev, FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_WTR_ADDR(Dev, i), Dev->Client[i].ODU1PG.WTRTmr);
		if (Dev->Client[i].ODU1PG.HOTmr != Dev->Client[i].ODU1PG.__HOTmr)
			FPGA_CLIENT_ADM_2_5_WriteByte(Dev, FPGA_CLIENT_ADM_2_5_CLIENT_ODU1_PG_HO_ADDR(Dev, i), Dev->Client[i].ODU1PG.HOTmr);
		
	}

	for (i = 0; i < FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_CNT; i++)
	{
		Dev->ODU0Framer[i].__OPU0PayloadType = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_ADDR(Dev, i)));
		if (Dev->ODU0Framer[i].OPU0PayloadType != Dev->ODU0Framer[i].__OPU0PayloadType)
			FPGA_CLIENT_ADM_2_5_WriteByte(Dev, FPGA_CLIENT_ADM_2_5_ODU0_FRAMER_OPU0_PAYLOAD_TYPE_ADDR(Dev, i), Dev->ODU0Framer[i].OPU0PayloadType);
	}
	
	for (i = 0; i < FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_CNT; i++)
	{
		Dev->ODU1Framer[i].__OPU1PayloadType = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_ADDR(Dev, i)));
		Dev->ODU1Framer[i].__OPU1Mux = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_MUX_ADDR(Dev, i)));
		
		if (Dev->ODU1Framer[i].OPU1PayloadType != Dev->ODU1Framer[i].__OPU1PayloadType)
			FPGA_CLIENT_ADM_2_5_WriteByte(Dev, FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_PAYLOAD_TYPE_ADDR(Dev, i), Dev->ODU1Framer[i].OPU1PayloadType);
		if (Dev->ODU1Framer[i].OPU1Mux != Dev->ODU1Framer[i].__OPU1Mux)
			FPGA_CLIENT_ADM_2_5_WriteByte(Dev, FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_OPU1_MUX_ADDR(Dev, i), Dev->ODU1Framer[i].OPU1PayloadType);
		for (j = 0; j < 2; j++)
		{
			Dev->ODU1Framer[i].__ODU0Mux[j] = *((uint8_t*)(FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_ADDR(Dev, i, j)));
			if (Dev->ODU1Framer[i].ODU0Mux[j] != Dev->ODU1Framer[i].__ODU0Mux[j])
				FPGA_CLIENT_ADM_2_5_WriteByte(Dev, FPGA_CLIENT_ADM_2_5_ODU1_FRAMER_ODU0_MUX_ADDR(Dev, i, j), Dev->ODU1Framer[i].ODU0Mux[j]);
		}
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
}

void FPGA_CLIENT_ADM_2_5_Deinit(FPGA_CLIENT_ADM_2_5_T *Dev)
{
	if (!Dev)
		return;
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "Deinit");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
}
