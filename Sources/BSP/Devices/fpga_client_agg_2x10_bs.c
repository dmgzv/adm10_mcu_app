#include "fpga_client_agg_2x10_bs.h"

#define FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev)		((Dev)->BaseAddr)

#define FPGA_CLIENT_AGG_2X10_BS_CMD_ADDR(Dev)				(FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x08)
#define FPGA_CLIENT_AGG_2X10_BS_CMD_OPT1_ADDR(Dev)		(FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x09)
#define FPGA_CLIENT_AGG_2X10_BS_CMD_OPT2_ADDR(Dev)		(FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x0A)
#define FPGA_CLIENT_AGG_2X10_BS_CMD_OPT3_ADDR(Dev)		(FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x0B)
#define FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev)			(FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x0C)
#define FPGA_CLIENT_AGG_2X10_BS_STATUS_OPT_ADDR(Dev)	(FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x0D)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_TRANS_ADDR(Dev, IfaceIdx)  (FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x10 + ((IfaceIdx) * 0x04))
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx)   (FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x40 + ((IfaceIdx) * 0x20))
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_ODU1_ADDR(Dev, ODU1Idx)   (FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x200 + ((ODU1Idx) * 0x08))
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_ODU0_ADDR(Dev, ODU0Idx)   (FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x220 + ((ODU0Idx) * 0x08))

#define FPGA_CLIENT_AGG_2X10_BS_IFACE_MODE_ADDR(Dev, IfaceIdx)				(FPGA_CLIENT_AGG_2X10_BS_IFACE_TRANS_ADDR(Dev, IfaceIdx) + 0x00)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_PMASTATUS_ADDR(Dev, IfaceIdx)	(FPGA_CLIENT_AGG_2X10_BS_IFACE_TRANS_ADDR(Dev, IfaceIdx) + 0x01)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_PCSSTATUS_ADDR(Dev, IfaceIdx)	(FPGA_CLIENT_AGG_2X10_BS_IFACE_TRANS_ADDR(Dev, IfaceIdx) + 0x02)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_CDRSTATUS_ADDR(Dev, IfaceIdx)	(FPGA_CLIENT_AGG_2X10_BS_IFACE_TRANS_ADDR(Dev, IfaceIdx) + 0x02)

#define FPGA_CLIENT_AGG_2X10_BS_IFACE_CTRL_ADDR(Dev, IfaceIdx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx) + 0x00)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_STATUS_ADDR(Dev, IfaceIdx) (FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx) + 0x01)

#define FPGA_CLIENT_AGG_2X10_BS_IFACE_SDH_RX_STATUS_ADDR(Dev, IfaceIdx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx) + 0x02)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_SDH_BIP8_ADDR(Dev, IfaceIdx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx) + 0x04)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_SDH_BIP24_ADDR(Dev, IfaceIdx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx) + 0x06)

#define FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_TX_PKTS_ADDR(Dev, IfaceIdx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx) + 0x08)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_RX_PKTS_ADDR(Dev, IfaceIdx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx) + 0x0C)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_TX_OCTS_ADDR(Dev, IfaceIdx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx) + 0x10)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_RX_OCTS_ADDR(Dev, IfaceIdx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx) + 0x14)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_TX_ERRS_ADDR(Dev, IfaceIdx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx) + 0x18)
#define FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_RX_ERRS_ADDR(Dev, IfaceIdx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_STAT_ADDR(Dev, IfaceIdx) + 0x1A)

#define FPGA_CLIENT_AGG_2X10_BS_ODU1_ALARMS_ADDR(Dev, ODU1Idx) (FPGA_CLIENT_AGG_2X10_BS_IFACE_ODU1_ADDR(Dev, ODU1Idx) + 0x00)
#define FPGA_CLIENT_AGG_2X10_BS_ODU1_BIP_ADDR(Dev, ODU1Idx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_ODU1_ADDR(Dev, ODU1Idx) + 0x02)

#define FPGA_CLIENT_AGG_2X10_BS_ODU0_ALARMS_ADDR(Dev, ODU0Idx) (FPGA_CLIENT_AGG_2X10_BS_IFACE_ODU0_ADDR(Dev, ODU0Idx) + 0x00)
#define FPGA_CLIENT_AGG_2X10_BS_ODU0_BIP_ADDR(Dev, ODU0Idx)   (FPGA_CLIENT_AGG_2X10_BS_IFACE_ODU0_ADDR(Dev, ODU0Idx) + 0x02)

static void FPGA_CLIENT_AGG_2X10_BS_WriteByte(FPGA_CLIENT_AGG_2X10_BS_T *Dev, uint32_t Addr, uint8_t Data)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteByte: 0x%08x 0x%02x", Addr, Data);
	*((uint8_t*)(Addr)) = Data;
}

static void FPGA_CLIENT_AGG_2X10_BS_WriteCmd(FPGA_CLIENT_AGG_2X10_BS_T *Dev, uint32_t Cmd, uint8_t Opt1, uint8_t Opt2, uint8_t Opt3)
{
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "WriteCmd: 0x%02x 0x%02x 0x%02x 0x%02x", Cmd, Opt1, Opt2, Opt3);
	*((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_CMD_OPT1_ADDR(Dev)) = Opt1;
	*((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_CMD_OPT2_ADDR(Dev)) = Opt2;
	*((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_CMD_OPT3_ADDR(Dev)) = Opt3;
	*((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_CMD_ADDR(Dev)) = Cmd;
}

int32_t FPGA_CLIENT_AGG_2X10_BS_Init(FPGA_CLIENT_AGG_2X10_BS_T *Dev)
{
	uint32_t i;
	uint8_t Status, SwVersion[4];
	uint16_t Temp;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
	if (Status == 0)
		goto error;
	
	for (i = 0; i < 4; i++)
	{
		SwVersion[i] = *((char*)(FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + i));
	}
	sprintf(Dev->SwVersion, "%d.%d.%d.%d", SwVersion[1], SwVersion[0], SwVersion[3], SwVersion[2]);
	
	Temp = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;
	
	for (i = 0; i < FPGA_CLIENT_AGG_2X10_BS_ODU1_CNT; i++)
	{
		Dev->__ODU1Alarms[i] = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_ODU1_ALARMS_ADDR(Dev, i)));			
		Dev->__ODU1BIP[i] =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_ODU1_BIP_ADDR(Dev, i)));
		Dev->ODU1BIP[i] = 0;
	}
	
	for (i = 0; i < FPGA_CLIENT_AGG_2X10_BS_ODU0_CNT; i++)
	{
		Dev->__ODU0Alarms[i] = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_ODU0_ALARMS_ADDR(Dev, i)));			
		Dev->__ODU0BIP[i] =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_ODU0_BIP_ADDR(Dev, i)));
		Dev->ODU0BIP[i] = 0;
	}
	
	for (i = 0; i < FPGA_CLIENT_AGG_2X10_BS_IFACE_CNT; i++)
	{
		Dev->Iface[i].Mode = Dev->Iface[i].__Mode = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_MODE_ADDR(Dev, i)));
		Dev->Iface[i].PMAStatus = Dev->Iface[i].__PMAStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_PMASTATUS_ADDR(Dev, i)));
		Dev->Iface[i].__PCSStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_PCSSTATUS_ADDR(Dev, i)));
		Dev->Iface[i].__CDRStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_CDRSTATUS_ADDR(Dev, i)));
		Dev->Iface[i].Ctrl = Dev->Iface[i].__Ctrl = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_CTRL_ADDR(Dev, i)));
		
		Dev->Iface[i].__SDHRxStatus = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_SDH_RX_STATUS_ADDR(Dev, i)));
		
		Dev->Iface[i].__SDHBIP8 = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_SDH_BIP8_ADDR(Dev, i)));
		Dev->Iface[i].SDHBIP8 = 0;
		
		Dev->Iface[i].__SDHBIP24 = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_SDH_BIP24_ADDR(Dev, i)));
		Dev->Iface[i].SDHBIP24 = 0;
		
		Dev->Iface[i].__EthTxPkts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_TX_PKTS_ADDR(Dev, i)));
		Dev->Iface[i].EthTxPkts = 0;
		
		Dev->Iface[i].__EthRxPkts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_RX_PKTS_ADDR(Dev, i)));
		Dev->Iface[i].EthRxPkts = 0;
		
		Dev->Iface[i].__EthTxOcts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_TX_OCTS_ADDR(Dev, i)));
		Dev->Iface[i].EthTxOcts = 0;
		
		Dev->Iface[i].__EthRxOcts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_RX_OCTS_ADDR(Dev, i)));
		Dev->Iface[i].EthRxOcts = 0;
		
		Dev->Iface[i].__EthTxErrs = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_TX_ERRS_ADDR(Dev, i)));
		Dev->Iface[i].EthTxErrs = 0;
		
		Dev->Iface[i].__EthRxErrs = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_RX_ERRS_ADDR(Dev, i)));
		Dev->Iface[i].EthRxErrs = 0;
	}
	
	BSP_DEV_INIT_OK(Dev);
	return 0;

error:
	BSP_DEV_INIT_FAIL(Dev);
	return 0;
}

int32_t FPGA_CLIENT_AGG_2X10_BS_Update(FPGA_CLIENT_AGG_2X10_BS_T *Dev)
{
	uint32_t i;
	uint8_t Status;
	uint16_t Temp, SDHBIP8, SDHBIP24, EthRxErrs, EthTxErrs;
	uint16_t ODU1BIP, ODU0BIP;
	uint32_t EthRxPkts, EthRxOcts, EthTxPkts, EthTxOcts;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	Temp = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_ADDR(Dev) + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;

	for (i = 0; i < FPGA_CLIENT_AGG_2X10_BS_ODU1_CNT; i++)
	{
		Dev->__ODU1Alarms[i] = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_ODU1_ALARMS_ADDR(Dev, i)));			
		ODU1BIP =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_ODU1_BIP_ADDR(Dev, i)));
		Dev->ODU1BIP[i] = ODU1BIP - Dev->__ODU1BIP[i];
		Dev->__ODU1BIP[i] = ODU1BIP;
	}
	
	for (i = 0; i < FPGA_CLIENT_AGG_2X10_BS_ODU0_CNT; i++)
	{
		Dev->__ODU0Alarms[i] = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_ODU0_ALARMS_ADDR(Dev, i)));			
		ODU0BIP =  *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_ODU0_BIP_ADDR(Dev, i)));
		Dev->ODU0BIP[i] = ODU0BIP - Dev->__ODU0BIP[i];
		Dev->__ODU0BIP[i] = ODU0BIP;
	}
	
	for (i = 0; i < FPGA_CLIENT_AGG_2X10_BS_IFACE_CNT; i++)
	{
		Dev->Iface[i].__Mode = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_MODE_ADDR(Dev, i)));
		Dev->Iface[i].__PMAStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_PMASTATUS_ADDR(Dev, i)));
		Dev->Iface[i].__PCSStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_PCSSTATUS_ADDR(Dev, i)));
		Dev->Iface[i].__CDRStatus = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_CDRSTATUS_ADDR(Dev, i)));
		Dev->Iface[i].__Ctrl = *((uint8_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_CTRL_ADDR(Dev, i)));
		
		Dev->Iface[i].__SDHRxStatus = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_SDH_RX_STATUS_ADDR(Dev, i)));
		
		SDHBIP8 = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_SDH_BIP8_ADDR(Dev, i)));
		Dev->Iface[i].SDHBIP8 = SDHBIP8 - Dev->Iface[i].__SDHBIP8;
		Dev->Iface[i].__SDHBIP8 = SDHBIP8;
		
		SDHBIP24 = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_SDH_BIP24_ADDR(Dev, i)));
		Dev->Iface[i].SDHBIP24 = SDHBIP24 - Dev->Iface[i].__SDHBIP24;
		Dev->Iface[i].__SDHBIP24 = SDHBIP24;
		
		EthTxPkts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_TX_PKTS_ADDR(Dev, i)));
		Dev->Iface[i].EthTxPkts = EthTxPkts - Dev->Iface[i].__EthTxPkts;
		Dev->Iface[i].__EthTxPkts = EthTxPkts;
		
		EthRxPkts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_RX_PKTS_ADDR(Dev, i)));
		Dev->Iface[i].EthRxPkts = EthRxPkts - Dev->Iface[i].__EthRxPkts;
		Dev->Iface[i].__EthRxPkts = EthRxPkts;
		
		EthTxOcts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_TX_OCTS_ADDR(Dev, i)));
		Dev->Iface[i].EthTxOcts = EthTxOcts - Dev->Iface[i].__EthTxOcts;
		Dev->Iface[i].__EthTxOcts = EthTxOcts;
		
		EthRxOcts = *((uint32_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_RX_OCTS_ADDR(Dev, i)));
		Dev->Iface[i].EthRxOcts = EthRxOcts - Dev->Iface[i].__EthRxOcts;
		Dev->Iface[i].__EthRxOcts = EthRxOcts;
		
		EthTxErrs = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_TX_ERRS_ADDR(Dev, i)));
		Dev->Iface[i].EthTxErrs = EthTxErrs - Dev->Iface[i].__EthTxErrs;
		Dev->Iface[i].__EthTxErrs = EthTxErrs;
		
		EthRxErrs = *((uint16_t*)(FPGA_CLIENT_AGG_2X10_BS_IFACE_ETH_RX_ERRS_ADDR(Dev, i)));
		Dev->Iface[i].EthRxErrs = EthRxErrs - Dev->Iface[i].__EthRxErrs;
		Dev->Iface[i].__EthRxErrs = EthRxErrs;
				
		if (Dev->Iface[i].__Ctrl !=  Dev->Iface[i].Ctrl)
		{
			FPGA_CLIENT_AGG_2X10_BS_WriteByte(Dev, FPGA_CLIENT_AGG_2X10_BS_IFACE_CTRL_ADDR(Dev, i), Dev->Iface[i].Ctrl);
		}
		
		if (Dev->Iface[i].__Mode !=  Dev->Iface[i].Mode)
		{
			Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
			if (Status == 0)
				continue;
			FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x01, 0x00, i, Dev->Iface[i].Mode);
			osDelay(5);
		}
		
		if ((Dev->Iface[i].PMAStatus & FPGA_CLIENT_AGG_2X10_BS_IFACE_PMASTATUS_LOOPBACK_MASK) != (Dev->Iface[i].__PMAStatus & FPGA_CLIENT_AGG_2X10_BS_IFACE_PMASTATUS_LOOPBACK_MASK))
		{
			if (Dev->Iface[i].PMAStatus & FPGA_CLIENT_AGG_2X10_BS_IFACE_PMASTATUS_LOOPBACK_SERIAL_MASK)
			{
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x03, 0x00, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Iface[i].PMAStatus & FPGA_CLIENT_AGG_2X10_BS_IFACE_PMASTATUS_LOOPBACK_POST_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x01, i, 0x01);
				osDelay(5);
			}
			else if (Dev->Iface[i].PMAStatus & FPGA_CLIENT_AGG_2X10_BS_IFACE_PMASTATUS_LOOPBACK_PRE_CDR_MASK)
			{
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x00, i, 0x01);
				osDelay(5);
			}
			else
			{
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x00, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x02, 0x01, i, 0x00);
				osDelay(5);
				Status = *((uint8_t*)FPGA_CLIENT_AGG_2X10_BS_STATUS_ADDR(Dev));
				if (Status == 0)
					continue;
				FPGA_CLIENT_AGG_2X10_BS_WriteCmd(Dev, 0x03, 0x00, i, 0x00);
				osDelay(5);
			}
		}
	}

	BSP_DEV_UPDATE_OK(Dev);
	return 0;
}

void FPGA_CLIENT_AGG_2X10_BS_Deinit(FPGA_CLIENT_AGG_2X10_BS_T *Dev)
{
	if (!Dev)
		return;
	LOG_Info("BSP", Dev->DevName, Dev->DevId, "Deinit");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
}
