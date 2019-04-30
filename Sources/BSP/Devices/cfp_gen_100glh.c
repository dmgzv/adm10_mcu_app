#include "cfp_gen_100glh.h"
#include <string.h>
#include <math.h>

#ifdef BSP_CONFIG_USE_FREERTOS
#define	CFP_GEN_100GLH_msDelay(ms) 		osDelay(ms)
#else
#define	CFP_GEN_100GLH_msDelay(ms) 		HAL_Delay(ms)
#endif

#define CFP_GEN_100GLH_REG_MODULE_STATE 				0xB016
#define CFP_GEN_100GLH_REG_MODULE_STATE_LATCH 	0xB022
#define CFP_GEN_100GLH_REG_MODULE_STATUS 				0xB01D
#define CFP_GEN_100GLH_REG_MODULE_STATUS_LATCH	0xB023
#define CFP_GEN_100GLH_REG_STATUS_LANE_0 				0xB1A0
#define CFP_GEN_100GLH_REG_STATUS_LANE_0_LATCH	0xB1D0

#define CFP_GEN_100GLH_REG_ALARMS								0xB017
#define CFP_GEN_100GLH_REG_GLOBAL_ALARMS				0xB018
#define CFP_GEN_100GLH_REG_MODULE_FAULTS 				0xB01E
#define CFP_GEN_100GLH_REG_MODULE_FAULTS_LATCH	0xB024
#define CFP_GEN_100GLH_REG_MODULE_ALARMS				0xB01F
#define CFP_GEN_100GLH_REG_MODULE_ALARMS_LATCH	0xB025
#define CFP_GEN_100GLH_REG_VENDOR_ALARMS				0xB021
#define CFP_GEN_100GLH_REG_VENDOR_ALARMS_LATCH	0xB027

#define CFP_GEN_100GLH_REG_RX_PWR_UW_LANE_0			0xB350
#define CFP_GEN_100GLH_REG_TX_PWR_UW_LANE_0			0xB330
#define CFP_GEN_100GLH_REG_RX_PWR_DBM_LANE_0		0xB4E0
#define CFP_GEN_100GLH_REG_TX_PWR_DBM_LANE_0		0xB4A0
#define CFP_GEN_100GLH_REG_TEMP									0xB02F

#define CFP_GEN_100GLH_REG_TX_CH_CTL_LANE_0			0xB400
#define CFP_GEN_100GLH_REG_RX_CH_CTL_LANE_0			0xB420
#define CFP_GEN_100GLH_REG_TX_CH_FREQ_THZ_LANE_0 0xB450
#define CFP_GEN_100GLH_REG_TX_CH_FREQ_GHZ_LANE_0 0xB460
#define CFP_GEN_100GLH_REG_RX_CH_FREQ_THZ_LANE_0 0xB470
#define CFP_GEN_100GLH_REG_RX_CH_FREQ_GHZ_LANE_0 0xB480

#define CFP_GEN_100GLH_REG_TX_PWR_CTL_LANE_0		0xB410

#define CFP_GEN_100GLH_REG_CMD_ERR_ADDR					0xB00C
#define CFP_GEN_100GLH_REG_CMD_ERR_DATA					0xB00D
#define CFP_GEN_100GLH_REG_CMD_ERR_DATA_MASK		0xB00E
#define CFP_GEN_100GLH_REG_CMD_ERR_STAT					0xB00F
#define CFP_GEN_100GLH_REG_CMD_CTL							0xB050

#define CFP_GEN_100GLH_REG_OTN_STATUS_LANE_0				0xB580
#define CFP_GEN_100GLH_REG_OTN_STATUS_LANE_0_LATCH	0xB590
#define CFP_GEN_100GLH_REG_OTN_FEC_COR							0xB5C0
#define CFP_GEN_100GLH_REG_OTN_FEC_UNCOR						0xB5C3
#define CFP_GEN_100GLH_REG_OTN_SM_BIP								0xB5C5
#define CFP_GEN_100GLH_REG_OTN_SM_BEI	 							0xB5C7

int32_t CFP_GEN_100GLH_ExecuteCmd(CFP_GEN_100GLH_T *Dev, uint16_t Addr, uint16_t Data)
{
	uint32_t Timeout;
	uint16_t Status, ErrAddr, ErrData, ErrDataMask, ErrStatus;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_CMD_CTL, &Status))
		return -1;	
	if (!(Status & (1 << 15)))
		return -1;
	
	if (CFP_GEN_100GLH_Write(Dev, Addr, Data))
		return -1;
	
	Timeout = 1000;
	do 
	{
		if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_CMD_CTL, &Status))
			return -1;
		if ((Status & (1 << 14)))
		{
			if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_CMD_ERR_ADDR, &ErrAddr))
				return -1;
			if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_CMD_ERR_DATA, &ErrData))
				return -1;	
			if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_CMD_ERR_DATA_MASK, &ErrDataMask))
				return -1;	
			if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_CMD_ERR_STAT, &ErrStatus))
				return -1;	
			return -1;
		}
		if ((Status & (1 << 15)))
			return 0;
		CFP_GEN_100GLH_msDelay(5);	
		Timeout -= 5;		
	} while (Timeout);
	return -1;
}

int32_t CFP_GEN_100GLH_ReadStatus(CFP_GEN_100GLH_T *Dev)
{
	uint16_t Data;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_MODULE_STATE, &Data))
		return -1;
	Dev->State = Data;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_MODULE_STATE_LATCH, &Data))
		return -1;
	Dev->StateLatch = Data;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_MODULE_STATUS, &Data))
		return -1;	
	Dev->Status = Data;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_MODULE_STATUS_LATCH, &Data))
		return -1;	
	Dev->StatusLatch = Data;
	
	return 0;
}

int32_t CFP_GEN_100GLH_ReadAlarms(CFP_GEN_100GLH_T *Dev)
{
	uint16_t Data;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_ALARMS, &Data))
		return -1;
	Dev->Alarms = Data;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_GLOBAL_ALARMS, &Data))
		return -1;	
	Dev->GlobalAlarms = Data;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_MODULE_ALARMS, &Data))
		return -1;	
	Dev->ModuleAlarms = Data;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_MODULE_ALARMS_LATCH, &Data))
		return -1;	
	Dev->ModuleAlarmsLatch = Data;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_MODULE_FAULTS, &Data))
		return -1;	
	Dev->ModuleFaults = Data;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_MODULE_FAULTS_LATCH, &Data))
		return -1;	
	Dev->ModuleFaultsLatch = Data;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_VENDOR_ALARMS, &Data))
		return -1;	
	Dev->VendorAlarms = Data;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_VENDOR_ALARMS_LATCH, &Data))
		return -1;	
	Dev->VendorAlarmsLatch = Data;
	return 0;
}

int32_t CFP_GEN_100GLH_ReadTemp(CFP_GEN_100GLH_T *Dev)
{
	int16_t Data;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_TEMP, (uint16_t*)&Data))
		return -1;
	
	Dev->Temp = Data / 256.0f;
	return 0;
}

int32_t CFP_GEN_100GLH_ReadOTUPerformance(CFP_GEN_100GLH_T *Dev)
{
	uint16_t Data;
	uint64_t OTUFECCor;
	uint64_t OTUFECUncor;
	uint64_t OTUSMBIP;
	uint64_t OTUSMBEI;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_OTN_FEC_COR, &Data))
		return -1;
	OTUFECCor = (uint64_t)Data << 32;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_OTN_FEC_COR + 1, &Data))
		return -1;
	OTUFECCor += (uint64_t)Data << 16;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_OTN_FEC_COR + 2, &Data))
		return -1;
	OTUFECCor += (uint64_t)Data;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_OTN_FEC_UNCOR, &Data))
		return -1;
	OTUFECUncor += (uint64_t)Data << 16;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_OTN_FEC_UNCOR + 1, &Data))
		return -1;
	OTUFECUncor += (uint64_t)Data;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_OTN_SM_BIP, &Data))
		return -1;
	OTUSMBIP = (uint64_t)Data << 16;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_OTN_SM_BIP + 1, &Data))
		return -1;
	OTUSMBIP += (uint64_t)Data;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_OTN_SM_BIP, &Data))
		return -1;
	OTUSMBEI = (uint64_t)Data << 16;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_OTN_SM_BIP + 1, &Data))
		return -1;
	OTUSMBEI += (uint64_t)Data;
	
	Dev->OTUFECCor = OTUFECCor;
	Dev->OTUFECUncor = OTUFECUncor;
	Dev->OTUSMBIP = OTUSMBIP;
	Dev->OTUSMBEI = OTUSMBEI;
	
	return 0;
}

int32_t CFP_GEN_100GLH_ReadLaneRxPwr(CFP_GEN_100GLH_T *Dev, uint32_t Lane)
{
	int16_t Data;

	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_RX_PWR_DBM_LANE_0 + Lane, (uint16_t*)&Data))
		return -1;
	Dev->Lane[Lane].RxPwr = Data * 0.01f;

	return 0;
}

int32_t CFP_GEN_100GLH_ReadLaneTxPwr(CFP_GEN_100GLH_T *Dev, uint32_t Lane)
{
	int16_t Data;

	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_TX_PWR_DBM_LANE_0 + Lane, (uint16_t*)&Data))
		return -1;
	Dev->Lane[Lane].TxPwr = Data * 0.01f;

	return 0;
}

int32_t CFP_GEN_100GLH_ReadLaneStatus(CFP_GEN_100GLH_T *Dev, uint32_t Lane)
{
	uint16_t Data;
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_STATUS_LANE_0 + Lane, &Data))
		return -1;
	Dev->Lane[Lane].Status = Data;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_STATUS_LANE_0_LATCH + Lane, &Data))
		return -1;
	Dev->Lane[Lane].StatusLatch = Data;
	
	return 0;
}

int32_t CFP_GEN_100GLH_ReadLaneOTUAlarms(CFP_GEN_100GLH_T *Dev, uint32_t Lane)
{
	uint16_t Data;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_OTN_STATUS_LANE_0 + Lane, &Data))
		return -1;
	
	Dev->Lane[Lane].OTUAlarms = Data;

	return 0;
}

int32_t CFP_GEN_100GLH_ReadLaneTxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane)
{
	uint16_t Data;
	uint16_t TxChannel;
	uint32_t TxGridSpacing;
	
	if (!Dev)
		return -1;
	
	if (Lane > Dev->Gen.LanesCnt)
		return -1;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_TX_CH_CTL_LANE_0 + Lane, &Data))
		return -1;
	
	TxChannel = Data & 0x3FF;
	switch ((Data >> 13) & 0x07) 
	{
		case CFP_GEN_100GLH_GRID_SPACING_100GHZ:
			TxGridSpacing = 100000;
			break;
		case CFP_GEN_100GLH_GRID_SPACING_50GHZ:
			TxGridSpacing = 50000;
			break;
		case CFP_GEN_100GLH_GRID_SPACING_33GHZ:
			TxGridSpacing = 33000;
			break;
		case CFP_GEN_100GLH_GRID_SPACING_25GHZ:
			TxGridSpacing = 25000;
			break;
		case CFP_GEN_100GLH_GRID_SPACING_12P5GHZ:
			TxGridSpacing = 12500;
			break;
		case CFP_GEN_100GLH_GRID_SPACING_6P25GHZ:
			TxGridSpacing = 6250;
			break;
		default:
			return -1;
	}
	
	Dev->Lane[Lane].TxGridSpacing = TxGridSpacing;
	Dev->Lane[Lane].TxFreq = (TxGridSpacing * (TxChannel - 1)) + Dev->Gen.FreqMin;	
	
	return 0;
}

int32_t CFP_GEN_100GLH_SetLaneTxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane, uint32_t TxFreq)
{
	uint16_t TxChannel;
	uint16_t Data;
	
	if ((!Dev) || (Lane >= Dev->Gen.LanesCnt))
		return -1;	
	
	/*TxFreq = TxChannel*TxGrid + FreqMin*/	
	if ((TxFreq > Dev->Gen.FreqMax) || (TxFreq < Dev->Gen.FreqMin))
		return -1;
	
	if (!Dev->Lane[Lane].TxGridSpacing)
		return -1;
	
	if ((TxFreq - Dev->Gen.FreqMin) % Dev->Lane[Lane].TxGridSpacing)
		return -1;
	
	TxChannel = ((TxFreq - Dev->Gen.FreqMin) / Dev->Lane[Lane].TxGridSpacing) + 1;
	
	switch (Dev->Lane[Lane].TxGridSpacing) 
	{
		case 100000:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_100GHZ << 13);
			break;
		case 50000:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_50GHZ << 13);
			break;
		case 33000:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_33GHZ << 13);
			break;
		case 25000:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_25GHZ << 13);
			break;
		case 12500:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_12P5GHZ << 13);
			break;
		case 6250:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_6P25GHZ << 13);
			break;
		default:
			return -1;
	}
	
	if (CFP_GEN_100GLH_ExecuteCmd(Dev, CFP_GEN_100GLH_REG_TX_CH_CTL_LANE_0 + Lane, Data))
		return -1;
	
	Dev->Lane[Lane].TxFreq = TxFreq;
	
	return 0;
}

int32_t CFP_GEN_100GLH_ConfigLaneTxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane, uint32_t TxFreq)
{
	if ((!Dev) || (Lane >= Dev->Gen.LanesCnt))
		return -1;	
	
	/*TxFreq = TxChannel*TxGrid + FreqMin*/	
	if ((TxFreq > Dev->Gen.FreqMax) || (TxFreq < Dev->Gen.FreqMin))
		return -1;
	
	if (!Dev->Lane[Lane].TxGridSpacing)
		return -1;
	
	if ((TxFreq - Dev->Gen.FreqMin) % Dev->Lane[Lane].TxGridSpacing)
		return -1;
	
	Dev->Lane[Lane].TxFreq = TxFreq;
	
	return 0;
}

int32_t CFP_GEN_100GLH_UpdateLaneTxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane)
{
	uint16_t TxChannel;
	uint16_t Data;
	
	if ((!Dev) || (Lane >= Dev->Gen.LanesCnt))
		return -1;	
	
	/*TxFreq = TxChannel*TxGrid + FreqMin*/	
	if ((Dev->Lane[Lane].TxFreq > Dev->Gen.FreqMax) || (Dev->Lane[Lane].TxFreq < Dev->Gen.FreqMin))
		return -1;
	
	if (!Dev->Lane[Lane].TxGridSpacing)
		return -1;
	
	if ((Dev->Lane[Lane].TxFreq - Dev->Gen.FreqMin) % Dev->Lane[Lane].TxGridSpacing)
		return -1;
	
	TxChannel = ((Dev->Lane[Lane].TxFreq - Dev->Gen.FreqMin) / Dev->Lane[Lane].TxGridSpacing) + 1;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_TX_CH_CTL_LANE_0 + Lane, &Data))
		return -1;
	
	if (TxChannel == (Data & 0x3FF))
		return 0;
	
	switch (Dev->Lane[Lane].TxGridSpacing) 
	{
		case 100000:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_100GHZ << 13);
			break;
		case 50000:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_50GHZ << 13);
			break;
		case 33000:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_33GHZ << 13);
			break;
		case 25000:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_25GHZ << 13);
			break;
		case 12500:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_12P5GHZ << 13);
			break;
		case 6250:
			Data = TxChannel + (CFP_GEN_100GLH_GRID_SPACING_6P25GHZ << 13);
			break;
		default:
			return -1;
	}
	
	if (CFP_GEN_100GLH_ExecuteCmd(Dev, CFP_GEN_100GLH_REG_TX_CH_CTL_LANE_0 + Lane, Data))
		return -1;
	
	return 0;
}

int32_t CFP_GEN_100GLH_ReadLaneRxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane)
{
	uint16_t Data;
	uint16_t RxChannel;
	uint32_t RxGridSpacing;
	
	if (!Dev)
		return -1;
	
	if (Lane > Dev->Gen.LanesCnt)
		return -1;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_RX_CH_CTL_LANE_0 + Lane, &Data))
		return -1;
	
	RxChannel = (Data & 0x3FF);
	switch ((Data >> 13) & 0x07) 
	{
		case CFP_GEN_100GLH_GRID_SPACING_100GHZ:
			RxGridSpacing = 100000;
			break;
		case CFP_GEN_100GLH_GRID_SPACING_50GHZ:
			RxGridSpacing = 50000;
			break;
		case CFP_GEN_100GLH_GRID_SPACING_33GHZ:
			RxGridSpacing = 33000;
			break;
		case CFP_GEN_100GLH_GRID_SPACING_25GHZ:
			RxGridSpacing = 25000;
			break;
		case CFP_GEN_100GLH_GRID_SPACING_12P5GHZ:
			RxGridSpacing = 12500;
			break;
		case CFP_GEN_100GLH_GRID_SPACING_6P25GHZ:
			RxGridSpacing = 6250;
			break;
		default:
			return -1;
	}
	
	Dev->Lane[Lane].RxGridSpacing = RxGridSpacing;
	Dev->Lane[Lane].RxFreq = (RxGridSpacing * (RxChannel - 1)) + Dev->Gen.FreqMin;	
	return 0;
}

int32_t CFP_GEN_100GLH_SetLaneRxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane, uint32_t RxFreq)
{
	uint16_t RxChannel;
	uint16_t Data;
	
	if ((!Dev) || (Lane >= Dev->Gen.LanesCnt))
		return -1;	
		
	/*RxFreq = RxChannel*RxGridSpacing + FreqMin*/
	
	if ((RxFreq > Dev->Gen.FreqMax) || (RxFreq < Dev->Gen.FreqMin))
		return -1;
	
	if (!Dev->Lane[Lane].RxGridSpacing)
		return -1;
	
	if ((RxFreq - Dev->Gen.FreqMin) % Dev->Lane[Lane].RxGridSpacing)
		return -1;
	
	RxChannel = ((RxFreq - Dev->Gen.FreqMin) / Dev->Lane[Lane].RxGridSpacing) + 1;
	
	switch (Dev->Lane[Lane].RxGridSpacing) 
	{
		case 100000:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_100GHZ << 13);
			break;
		case 50000:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_50GHZ << 13);
			break;
		case 33000:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_33GHZ << 13);
			break;
		case 25000:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_25GHZ << 13);
			break;
		case 12500:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_12P5GHZ << 13);
			break;
		case 6250:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_6P25GHZ << 13);
			break;
		default:
			return -1;
	}
	
	if (CFP_GEN_100GLH_ExecuteCmd(Dev, CFP_GEN_100GLH_REG_RX_CH_CTL_LANE_0 + Lane, Data))
		return -1;
	
	Dev->Lane[Lane].RxFreq = RxFreq;
	
	return 0;
}

int32_t CFP_GEN_100GLH_ConfigLaneRxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane, uint32_t RxFreq)
{
	
	if ((!Dev) || (Lane >= Dev->Gen.LanesCnt))
		return -1;	
		
	/*RxFreq = RxChannel*RxGridSpacing + FreqMin*/
	
	if ((RxFreq > Dev->Gen.FreqMax) || (RxFreq < Dev->Gen.FreqMin))
		return -1;
	
	if (!Dev->Lane[Lane].RxGridSpacing)
		return -1;
	
	if ((RxFreq - Dev->Gen.FreqMin) % Dev->Lane[Lane].RxGridSpacing)
		return -1;
	
	Dev->Lane[Lane].RxFreq = RxFreq;
	
	return 0;	
}

int32_t CFP_GEN_100GLH_UpdateLaneRxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane)
{
	uint16_t RxChannel;
	uint16_t Data;
	
	if ((!Dev) || (Lane >= Dev->Gen.LanesCnt))
		return -1;
	
	/*RxFreq = RxChannel*RxGridSpacing + FreqMin*/
	
	if ((Dev->Lane[Lane].RxFreq > Dev->Gen.FreqMax) || (Dev->Lane[Lane].RxFreq < Dev->Gen.FreqMin))
		return -1;
	
	if (!Dev->Lane[Lane].RxGridSpacing)
		return -1;
	
	if ((Dev->Lane[Lane].RxFreq - Dev->Gen.FreqMin) % Dev->Lane[Lane].RxGridSpacing)
		return -1;
	
	RxChannel = ((Dev->Lane[Lane].RxFreq - Dev->Gen.FreqMin) / Dev->Lane[Lane].RxGridSpacing) + 1;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_RX_CH_CTL_LANE_0 + Lane, &Data))
		return -1;
	
	if (RxChannel == (Data & 0x3FF))
		return 0;
	
	switch (Dev->Lane[Lane].RxGridSpacing) 
	{
		case 100000:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_100GHZ << 13);
			break;
		case 50000:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_50GHZ << 13);
			break;
		case 33000:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_33GHZ << 13);
			break;
		case 25000:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_25GHZ << 13);
			break;
		case 12500:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_12P5GHZ << 13);
			break;
		case 6250:
			Data = RxChannel + (CFP_GEN_100GLH_GRID_SPACING_6P25GHZ << 13);
			break;
		default:
			return -1;
	}
	
	if (CFP_GEN_100GLH_ExecuteCmd(Dev, CFP_GEN_100GLH_REG_RX_CH_CTL_LANE_0 + Lane, Data))
		return -1;
	
	return 0;
}

int32_t CFP_GEN_100GLH_ReadLaneTxPwrCfg(CFP_GEN_100GLH_T *Dev, uint32_t Lane)
{
	int16_t Data;
	
	if (!Dev)
		return -1;
	
	if (Lane > Dev->Gen.LanesCnt)
		return -1;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_TX_PWR_CTL_LANE_0 + Lane, (uint16_t*)&Data))
		return -1;
	
	Dev->Lane[Lane].TxPwrCfg = Data * 0.01;
	
	return 0;
}

int32_t CFP_GEN_100GLH_SetLaneTxPwrCfg(CFP_GEN_100GLH_T *Dev, uint32_t Lane, float TxPwrCfg)
{
	uint16_t Data;
	
	if ((!Dev) || (Lane >= Dev->Gen.LanesCnt))
		return -1;
	
	if (TxPwrCfg > Dev->Gen.Thr.TxPwr[CFP_GEN_THR_TYPE_CMAX])
		return -1;
	if (TxPwrCfg < Dev->Gen.Thr.TxPwr[CFP_GEN_THR_TYPE_CMIN])
		return -1;
	
	Data = (int16_t)(TxPwrCfg * 100);
	
	if (CFP_GEN_100GLH_ExecuteCmd(Dev, CFP_GEN_100GLH_REG_TX_PWR_CTL_LANE_0 + Lane, Data))
		return -1;
	
	Dev->Lane[Lane].TxPwrCfg = TxPwrCfg;
	return 0;
}

int32_t CFP_GEN_100GLH_ConfigLaneTxPwrCfg(CFP_GEN_100GLH_T *Dev, uint32_t Lane, float TxPwrCfg)
{	
	if ((!Dev) || (Lane >= Dev->Gen.LanesCnt))
		return -1;
	
	if (TxPwrCfg > Dev->Gen.Thr.TxPwr[CFP_GEN_THR_TYPE_CMAX])
		return -1;
	if (TxPwrCfg < Dev->Gen.Thr.TxPwr[CFP_GEN_THR_TYPE_CMIN])
		return -1;
	
	Dev->Lane[Lane].TxPwrCfg = TxPwrCfg;
	return 0;
}

int32_t CFP_GEN_100GLH_UpdateLaneTxPwrCfg(CFP_GEN_100GLH_T *Dev, uint32_t Lane)
{
	int16_t Data;
	
	if ((!Dev) || (Lane >= Dev->Gen.LanesCnt))
		return -1;
	
	if (Dev->Lane[Lane].TxPwrCfg > Dev->Gen.Thr.TxPwr[CFP_GEN_THR_TYPE_CMAX])
		return -1;
	if (Dev->Lane[Lane].TxPwrCfg < Dev->Gen.Thr.TxPwr[CFP_GEN_THR_TYPE_CMIN])
		return -1;
	
	if (CFP_GEN_100GLH_Read(Dev, CFP_GEN_100GLH_REG_TX_PWR_CTL_LANE_0 + Lane, (uint16_t*)&Data))
		return -1;
	
	if ((int16_t)(Dev->Lane[Lane].TxPwrCfg * 100) == Data)
		return 0;
	
	Data = (int16_t)(Dev->Lane[Lane].TxPwrCfg * 100);
	
	if (CFP_GEN_100GLH_ExecuteCmd(Dev, CFP_GEN_100GLH_REG_TX_PWR_CTL_LANE_0 + Lane, (uint16_t)Data))
		return -1;
	
	return 0;
}

int32_t CFP_GEN_100GLH_Init(CFP_GEN_100GLH_T *Dev)
{
	uint32_t i;
	
	if (!Dev)
		return -1;
	
	for (i = 0; i < Dev->Gen.LanesCnt; i++)
	{	
		if (CFP_GEN_100GLH_ReadLaneTxPwrCfg(Dev, i))
			return -1;
		if (CFP_GEN_100GLH_ReadLaneTxFreq(Dev, i))
			return -1;
		if (CFP_GEN_100GLH_ReadLaneRxFreq(Dev, i))
			return -1;
	}
	
	return 0;
}

int32_t CFP_GEN_100GLH_Update(CFP_GEN_100GLH_T *Dev)
{
	uint32_t i;
	
	if (!Dev)
		return -1;
	
	if (CFP_GEN_100GLH_ReadTemp(Dev))
		return -1;
	if (CFP_GEN_100GLH_ReadStatus(Dev))
		return -1;
	if (CFP_GEN_100GLH_ReadAlarms(Dev))
		return -1;
	if (CFP_GEN_100GLH_ReadOTUPerformance(Dev))
		return -1;
	
	for (i = 0; i < Dev->Gen.LanesCnt; i++)
	{	
		if (CFP_GEN_100GLH_ReadLaneRxPwr(Dev, i))
			return -1;
		if (CFP_GEN_100GLH_ReadLaneTxPwr(Dev, i))
			return -1;
		if (CFP_GEN_100GLH_ReadLaneOTUAlarms(Dev, i))
			return -1;
	}
	
	for (i = 0; i < Dev->Gen.LanesCnt; i++)
	{	
		if (CFP_GEN_100GLH_UpdateLaneTxFreq(Dev, i))
			return -1;
		
		if (CFP_GEN_100GLH_UpdateLaneRxFreq(Dev, i))
			return -1;
		
		if (CFP_GEN_100GLH_UpdateLaneTxPwrCfg(Dev, i))
			return -1;
	}
	
	return 0;
}
