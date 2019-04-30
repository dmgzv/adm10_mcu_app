#include "cfp_gen.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define CFP_GEN_REG_MOD_ID						 	0x8000
#define CFP_GEN_REG_VENDOR						 	0x8021
#define CFP_GEN_REG_PT_NUMBER					 	0x8034
#define CFP_GEN_REG_SR_NUMBER					 	0x8044
#define CFP_GEN_REG_SW_NUMBER						0x806C
#define CFP_GEN_REG_HW_NUMBER						0x806A
#define CFP_GEN_REG_LANES_CNT						0x8009
	
#define CFP_GEN_REG_RX_PWR_THR_CMAX			0x80C0
#define CFP_GEN_REG_RX_PWR_THR_WMAX			0x80C2
#define CFP_GEN_REG_RX_PWR_THR_WMIN			0x80C4
#define CFP_GEN_REG_RX_PWR_THR_CMIN			0x80C6
	
#define CFP_GEN_REG_TX_PWR_THR_CMAX			0x80B0
#define CFP_GEN_REG_TX_PWR_THR_WMAX			0x80B2
#define CFP_GEN_REG_TX_PWR_THR_WMIN			0x80B4
#define CFP_GEN_REG_TX_PWR_THR_CMIN			0x80B6

#define CFP_GEN_REG_TEMP_THR_CMAX				0x8080
#define CFP_GEN_REG_TEMP_THR_WMAX				0x8082
#define CFP_GEN_REG_TEMP_THR_WMIN				0x8084
#define CFP_GEN_REG_TEMP_THR_CMIN				0x8086

#define CFP_GEN_REG_POWER_THR_CMAX			0x8088
#define CFP_GEN_REG_POWER_THR_WMAX			0x808A
#define CFP_GEN_REG_POWER_THR_WMIN			0x808C
#define CFP_GEN_REG_POWER_THR_CMIN			0x808E

#define CFP_GEN_REG_LASER_MIN_FREQ_THZ	0x818A
#define CFP_GEN_REG_LASER_MIN_FREQ_GHZ	0x818C
#define CFP_GEN_REG_LASER_MAX_FREQ_THZ	0x818E
#define CFP_GEN_REG_LASER_MAX_FREQ_GHZ	0x8190
#define CFP_GEN_REG_LASER_GRID_CAPS			0x8196

static int32_t CFP_GEN_ReadRxPwrThr(CFP_GEN_T *Dev)
{
	uint16_t Data;
	float RxPwrThr[4];
	uint32_t i;
	uint16_t Addr[4];
	
	Addr[CFP_GEN_THR_TYPE_CMAX] = CFP_GEN_REG_RX_PWR_THR_CMAX;
	Addr[CFP_GEN_THR_TYPE_WMAX] = CFP_GEN_REG_RX_PWR_THR_WMAX;
	Addr[CFP_GEN_THR_TYPE_WMIN] = CFP_GEN_REG_RX_PWR_THR_WMIN;
	Addr[CFP_GEN_THR_TYPE_CMIN] = CFP_GEN_REG_RX_PWR_THR_CMIN;
	
	for (i = 0; i < 4; i++)
	{
		if (CFP_GEN_Read(Dev, Addr[i], &Data))
			return -1;
		RxPwrThr[i] = (Data & 0xFF) << 8;
		if (CFP_GEN_Read(Dev, Addr[i] + 1, &Data))
			return -1;
		RxPwrThr[i] += (Data & 0xFF);
		RxPwrThr[i] = 10 * log10((RxPwrThr[i]) * 0.0001f);
	}	
	
	for (i = 0; i < 4; i++)
	{
		Dev->Thr.RxPwr[i] = RxPwrThr[i];
	}
	return 0;	
}

static int32_t CFP_GEN_ReadTxPwrThr(CFP_GEN_T *Dev)
{
	uint16_t Data;
	float TxPwrThr[4];
	uint32_t i;
	uint16_t Addr[4];
	
	Addr[CFP_GEN_THR_TYPE_CMAX] = CFP_GEN_REG_TX_PWR_THR_CMAX;
	Addr[CFP_GEN_THR_TYPE_WMAX] = CFP_GEN_REG_TX_PWR_THR_WMAX;
	Addr[CFP_GEN_THR_TYPE_WMIN] = CFP_GEN_REG_TX_PWR_THR_WMIN;
	Addr[CFP_GEN_THR_TYPE_CMIN] = CFP_GEN_REG_TX_PWR_THR_CMIN;
	
	for (i = 0; i < 4; i++)
	{
		if (CFP_GEN_Read(Dev, Addr[i], &Data))
			return -1;
		TxPwrThr[i] = (Data & 0xFF) << 8;
		if (CFP_GEN_Read(Dev, Addr[i] + 1, &Data))
			return -1;
		TxPwrThr[i] += (Data & 0xFF);
		TxPwrThr[i] = 10 * log10((TxPwrThr[i]) * 0.0001f);
	}	
	
	for (i = 0; i < 4; i++)
	{
		Dev->Thr.TxPwr[i] = TxPwrThr[i];
	}
	return 0;	
}

static int32_t CFP_GEN_ReadTempThr(CFP_GEN_T *Dev)
{
	uint16_t Data;
	int16_t TempThr[4];
	uint32_t i;
	uint16_t Addr[4];
	
	Addr[CFP_GEN_THR_TYPE_CMAX] = CFP_GEN_REG_TEMP_THR_CMAX;
	Addr[CFP_GEN_THR_TYPE_WMAX] = CFP_GEN_REG_TEMP_THR_WMAX;
	Addr[CFP_GEN_THR_TYPE_WMIN] = CFP_GEN_REG_TEMP_THR_WMIN;
	Addr[CFP_GEN_THR_TYPE_CMIN] = CFP_GEN_REG_TEMP_THR_CMIN;
	
	for (i = 0; i < 4; i++)
	{
		TempThr[i] = 0;
		if (CFP_GEN_Read(Dev, Addr[i], &Data))
			return -1;
		TempThr[i] |= (Data & 0xFF) << 8;
		if (CFP_GEN_Read(Dev, Addr[i] + 1, &Data))
			return -1;
		TempThr[i] |= (Data & 0xFF);
	}	
	
	for (i = 0; i < 4; i++)
	{
		Dev->Thr.Temp[i] = TempThr[i] / 256.0;;
	}
	return 0;	
}

static int32_t CFP_GEN_ReadPowerThr(CFP_GEN_T *Dev)
{
	uint16_t Data;
	uint16_t PowerThr[4];
	uint32_t i;
	uint16_t Addr[4];
	
	Addr[CFP_GEN_THR_TYPE_CMAX] = CFP_GEN_REG_POWER_THR_CMAX;
	Addr[CFP_GEN_THR_TYPE_WMAX] = CFP_GEN_REG_POWER_THR_WMAX;
	Addr[CFP_GEN_THR_TYPE_WMIN] = CFP_GEN_REG_POWER_THR_WMIN;
	Addr[CFP_GEN_THR_TYPE_CMIN] = CFP_GEN_REG_POWER_THR_CMIN;
	
	for (i = 0; i < 4; i++)
	{
		PowerThr[i] = 0;
		if (CFP_GEN_Read(Dev, Addr[i], &Data))
			return -1;
		PowerThr[i] |= (Data & 0xFF) << 8;
		if (CFP_GEN_Read(Dev, Addr[i] + 1, &Data))
			return -1;
		PowerThr[i] |= (Data & 0xFF);
	}	
	
	for (i = 0; i < 4; i++)
	{
		Dev->Thr.Power[i] = PowerThr[i] * 0.0001;;
	}
	return 0;	
}

static int32_t CFP_GEN_ReadFreqInfo(CFP_GEN_T *Dev)
{
	uint16_t Data;
	uint32_t FreqMin;
	uint32_t FreqMax;
	uint32_t Temp;
	uint8_t FreqGridCaps;
	
	if (CFP_GEN_Read(Dev, CFP_GEN_REG_LASER_MIN_FREQ_GHZ, &Data))
		return -1;
	Temp = (Data & 0xFF) << 8;
	if (CFP_GEN_Read(Dev, CFP_GEN_REG_LASER_MIN_FREQ_GHZ + 1, &Data))
		return -1;
	Temp += (Data & 0xFF);
	FreqMin = Temp * 50;
	if (CFP_GEN_Read(Dev, CFP_GEN_REG_LASER_MIN_FREQ_THZ, &Data))
		return -1;
	Temp = (Data & 0xFF) << 8;
	if (CFP_GEN_Read(Dev, CFP_GEN_REG_LASER_MIN_FREQ_THZ + 1, &Data))
		return -1;
	Temp += (Data & 0xFF);
	FreqMin += Temp * 1000000;
	
	if (CFP_GEN_Read(Dev, CFP_GEN_REG_LASER_MAX_FREQ_GHZ, &Data))
		return -1;
	Temp = (Data & 0xFF) << 8;
	if (CFP_GEN_Read(Dev, CFP_GEN_REG_LASER_MAX_FREQ_GHZ + 1, &Data))
		return -1;
	Temp += (Data & 0xFF);
	FreqMax = Temp * 50;
	if (CFP_GEN_Read(Dev, CFP_GEN_REG_LASER_MAX_FREQ_THZ, &Data))
		return -1;
	Temp = (Data & 0xFF) << 8;
	if (CFP_GEN_Read(Dev, CFP_GEN_REG_LASER_MAX_FREQ_THZ + 1, &Data))
		return -1;
	Temp += (Data & 0xFF);
	FreqMax += Temp * 1000000;
	
	if (CFP_GEN_Read(Dev, CFP_GEN_REG_LASER_GRID_CAPS, &Data))
		return -1;
	FreqGridCaps = Data;
	
	Dev->FreqMin = FreqMin;
	Dev->FreqMax = FreqMax;
	Dev->FreqGridCaps = FreqGridCaps;
	return 0;
}

static int32_t CFP_GEN_ReadInfo(CFP_GEN_T *Dev)
{
	uint16_t Vendor[16];
	uint16_t PtNumber[16];
	uint16_t SrNumber[16];
	uint16_t SwNumber[2];
	uint16_t HwNumber[2];
	uint16_t LanesCnt;
	uint16_t ModId;
	uint16_t i;
	
	if (CFP_GEN_Read(Dev, CFP_GEN_REG_MOD_ID, &ModId))
		return -1;	
	if (ModId == 0xFFFF)
		return -1;
	Dev->Info.ModId = ModId;
	
	for (i = 0; i < 16; i++)
	{
		if (CFP_GEN_Read(Dev, CFP_GEN_REG_VENDOR + i, &Vendor[i]))
			return -1;
		if (CFP_GEN_Read(Dev, CFP_GEN_REG_PT_NUMBER + i, &PtNumber[i]))
			return -1;
		if (CFP_GEN_Read(Dev, CFP_GEN_REG_SR_NUMBER + i, &SrNumber[i]))
			return -1;
	}
	
	for (i = 0; i < 2; i++)
	{
		if (CFP_GEN_Read(Dev, CFP_GEN_REG_SW_NUMBER + i, &SwNumber[i]))
			return -1;
		if (CFP_GEN_Read(Dev, CFP_GEN_REG_HW_NUMBER + i, &HwNumber[i]))
			return -1;
	}
	
	if (CFP_GEN_Read(Dev, CFP_GEN_REG_LANES_CNT, &LanesCnt))
		return -1;
	
	for (i = 0; i < 16; i++)
	{
		Dev->Info.Vendor[i] = Vendor[i];
		Dev->Info.PtNumber[i] = PtNumber[i];
		Dev->Info.SrNumber[i] = SrNumber[i];
	}
	
	Dev->Info.Vendor[16] = 0;
	Dev->Info.PtNumber[16] = 0;
	Dev->Info.SrNumber[16] = 0;
	
	sprintf(Dev->Info.SwNumber, "%d.%d", (SwNumber[0] & 0xFF), (SwNumber[1] & 0xFF));
	sprintf(Dev->Info.HwNumber, "%d.%d", (HwNumber[0] & 0xFF), (HwNumber[1] & 0xFF));
	
	Dev->LanesCnt = (LanesCnt >> 4) & 0x0F;
	
	return 0;
}

int32_t CFP_GEN_SetState(CFP_GEN_T *Dev, CFP_GEN_STATE_T State)
{
	switch (State) 
	{
		case CFP_GEN_STATE_RESET:
			if (CFP_GEN_SetModRst(Dev, 0))
				return -1;
			if (CFP_GEN_SetModLowPwr(Dev, 1))
				return -1;
			Dev->State = CFP_GEN_STATE_RESET;
			break;
		case CFP_GEN_STATE_LOW_PWR:
			if (CFP_GEN_SetModRst(Dev, 1))
				return -1;
			if (CFP_GEN_SetModLowPwr(Dev, 1))
				return -1;
			Dev->State = CFP_GEN_STATE_LOW_PWR;
			break;
		case CFP_GEN_STATE_HIGH_PWR:
			if (CFP_GEN_SetModRst(Dev, 1))
				return -1;
			if (CFP_GEN_SetModLowPwr(Dev, 0))
				return -1;
			Dev->State = CFP_GEN_STATE_HIGH_PWR;
			break;
		default:
			return -1;
	}
	return 0;
}

static int32_t CFP_GEN_InitDevice(CFP_GEN_T *Dev)
{
	if (CFP_GEN_ReadInfo(Dev))
		return -1;
	if (CFP_GEN_ReadFreqInfo(Dev))
		return -1;
	if (CFP_GEN_ReadRxPwrThr(Dev))
		return -1;
	if (CFP_GEN_ReadTxPwrThr(Dev))
		return -1;
	if (CFP_GEN_ReadTempThr(Dev))
		return -1;
	if (CFP_GEN_ReadPowerThr(Dev))
		return -1;
	return 0;
}

int32_t CFP_GEN_Init(CFP_GEN_T *Dev, CFP_IFACE_T *Iface)
{
	uint32_t i;
	
	if (!Dev)
		return -1;
	
	Dev->Iface = Iface;
	
	if (CFP_GEN_InitModRst(Dev))
		return -1;
	if (CFP_GEN_InitModAbs(Dev))
		return -1;
	if (CFP_GEN_InitModLowPwr(Dev))
		return -1;
	if (CFP_GEN_InitGlbAlrm(Dev))
		return -1;
	
	for (i = 0; i < CFP_GEN_MAX_LANES_CNT; i++)
	{
		if (CFP_GEN_InitRxLos(Dev, i))
			return -1;
		if (CFP_GEN_InitTxDisable(Dev, i))
			return -1;
	}
	
	if (CFP_GEN_GetModRst(Dev))
		return -1;
	if (CFP_GEN_GetModAbs(Dev))
		return -1;
	if (CFP_GEN_GetModLowPwr(Dev))
		return -1;
	if (CFP_GEN_GetGlbAlrm(Dev))
		return -1;
	
	for (i = 0; i < CFP_GEN_MAX_LANES_CNT; i++)
	{
		if (CFP_GEN_GetRxLos(Dev, i))
			return -1;
		if (CFP_GEN_GetTxDisable(Dev, i))
			return -1;
	}
	
	if (Dev->Pin.ModAbs)
		return -1;
	
	if (!Dev->Pin.ModRst)
		Dev->State = CFP_GEN_STATE_RESET;
	else if (Dev->Pin.ModLowPwr)
		Dev->State = CFP_GEN_STATE_LOW_PWR;
	else
		Dev->State = CFP_GEN_STATE_HIGH_PWR;
	
	switch (Dev->State)
	{
		case CFP_GEN_STATE_HIGH_PWR:
		case CFP_GEN_STATE_LOW_PWR:		
			if (CFP_GEN_InitDevice(Dev))
				return -1;
			break;
		case CFP_GEN_STATE_RESET:
		default:
			CFP_GEN_SetState(Dev, CFP_GEN_STATE_LOW_PWR);
			return -1;
	}
	
	return 0;
}

int32_t CFP_GEN_Update(CFP_GEN_T *Dev)
{
	uint32_t i;
	
	if (!Dev)
		return -1;

	if (CFP_GEN_GetModAbs(Dev))
		return -1;
	if (CFP_GEN_GetGlbAlrm(Dev))
		return -1;
	
	for (i = 0; i < Dev->LanesCnt; i++)
	{
		if (CFP_GEN_GetRxLos(Dev, i))
			return -1;
	}
	
	if (Dev->Pin.ModAbs)
		return -1;
	
	for (i = 0; i < Dev->LanesCnt; i++)
	{
		if (CFP_GEN_UpdateTxDisable(Dev, i))
			return -1;
	}
	
	return 0;
}
