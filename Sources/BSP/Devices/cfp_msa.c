#include "cfp_msa.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define CFP_MSA_REG_TX_PWR_LANE_0								0xA2B0
#define CFP_MSA_REG_RX_PWR_LANE_0								0xA2D0
#define CFP_MSA_REG_TEMP												0xA02F
#define CFP_MSA_REG_POWER												0xA030
#define CFP_MSA_REG_MODULE_STATE								0xA016
#define CFP_MSA_REG_MODULE_STATE_LATCH					0xA022
#define CFP_MSA_REG_MODULE_GENERAL_STATUS				0xA01D
#define CFP_MSA_REG_MODULE_GENERAL_STATUS_LATCH	0xA023
#define CFP_MSA_REG_MODULE_FAULT_STATUS					0xA01E
#define CFP_MSA_REG_MODULE_FAULT_STATUS_LATCH		0xA024
#define CFP_MSA_REG_MODULE_LANE_0_STATUS				0xA210
#define CFP_MSA_REG_MODULE_LANE_0_STATUS_LATCH	0xA230

static int32_t CFP_MSA_ReadLaneRxPwr(CFP_MSA_T *Dev, uint32_t Lane)
{
	uint16_t Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_RX_PWR_LANE_0 + Lane, (uint16_t*)&Data))
		return -1;
	
	if (Data > 0)
		Dev->Lane[Lane].RxPwr = 10 * log10((Data) * 0.0001f);
	else
		Dev->Lane[Lane].RxPwr = -40.0;
	return 0;
}

static int32_t CFP_MSA_ReadLaneTxPwr(CFP_MSA_T *Dev, uint32_t Lane)
{
	uint16_t Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_TX_PWR_LANE_0 + Lane, (uint16_t*)&Data))
		return -1;
	
	if (Data > 0)
		Dev->Lane[Lane].TxPwr = 10 * log10((Data) * 0.0001f);
	else
		Dev->Lane[Lane].TxPwr = -40.0;
	
	return 0;
}

static int32_t CFP_MSA_ReadLaneStatus(CFP_MSA_T *Dev, uint32_t Lane)
{
	uint16_t Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_MODULE_LANE_0_STATUS + Lane, (uint16_t*)&Data))
		return -1;
	Dev->Lane[Lane].Status = Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_MODULE_LANE_0_STATUS_LATCH + Lane, (uint16_t*)&Data))
		return -1;
	Dev->Lane[Lane].StatusLatch = Data;
	
	return 0;
}

static int32_t CFP_MSA_ReadTemp(CFP_MSA_T *Dev)
{
	int16_t Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_TEMP, (uint16_t*)&Data))
		return -1;	
	Dev->Temp = (float)Data / 256.0f;
	return 0;
}

static int32_t CFP_MSA_ReadPower(CFP_MSA_T *Dev)
{
	uint16_t Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_POWER, (uint16_t*)&Data))
		return -1;	
	Dev->Power = (float)Data * 0.0001f;
	return 0;
}

static int32_t CFP_MSA_ReadStatus(CFP_MSA_T *Dev)
{
	uint16_t Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_MODULE_STATE, &Data))
		return -1;
	Dev->ModuleState = Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_MODULE_STATE_LATCH, &Data))
		return -1;
	Dev->ModuleStateLatch = Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_MODULE_GENERAL_STATUS, &Data))
		return -1;	
	Dev->GeneralStatus = Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_MODULE_GENERAL_STATUS_LATCH, &Data))
		return -1;	
	Dev->GeneralStatusLatch = Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_MODULE_FAULT_STATUS, &Data))
		return -1;	
	Dev->GeneralStatus = Data;
	
	if (CFP_MSA_Read(Dev, CFP_MSA_REG_MODULE_FAULT_STATUS_LATCH, &Data))
		return -1;	
	Dev->GeneralStatusLatch = Data;
	
	return 0;
}

static int32_t CFP_MSA_UpdateDevice(CFP_MSA_T *Dev)
{
	uint32_t i;
	
	if (CFP_MSA_ReadStatus(Dev))
		return -1;
	if (CFP_MSA_ReadTemp(Dev))
		return -1;
	if (CFP_MSA_ReadPower(Dev))
		return -1;
	for (i = 0; i < Dev->Gen.LanesCnt; i++)
	{
		if (CFP_MSA_ReadLaneStatus(Dev, i))
			return -1;
		if (CFP_MSA_ReadLaneRxPwr(Dev, i))
			return -1;
		if (CFP_MSA_ReadLaneTxPwr(Dev, i))
			return -1;
	}
	
	return 0;
}

int32_t CFP_MSA_Init(CFP_MSA_T *Dev, CFP_IFACE_T *Iface)
{	
	if (!Dev)
		return -1;
	
	if (CFP_GEN_Init(&Dev->Gen, Iface))
		return -1;
	
	return 0;
}

int32_t CFP_MSA_Update(CFP_MSA_T *Dev)
{	
	if (!Dev)
		return -1;
	
	if (Dev->Gen.State != CFP_GEN_STATE_HIGH_PWR)
	{
		if (CFP_MSA_SetState(Dev, CFP_GEN_STATE_HIGH_PWR))
			return -1;
	}
	
	if (CFP_GEN_Update(&Dev->Gen))
		return -1;
	
	if (CFP_MSA_UpdateDevice(Dev))
		return -1;
	
	return 0;
}

