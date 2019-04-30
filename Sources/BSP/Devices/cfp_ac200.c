#include "cfp_ac200.h"
#include <string.h>

#ifdef BSP_CONFIG_USE_FREERTOS
#define	CFP_AC200_msDelay(ms) 		osDelay(ms)
#else
#define	CFP_AC200_msDelay(ms) 		HAL_Delay(ms)
#endif

#define CFP_AC200_VENDOR "Acacia Comm Inc."
#define CFP_AC200_PT_NUMBER	"AC200"

#define CFP_AC200_REG_GENERAL_MODE	0x900B
#define CFP_AC200_REG_PRODUCT_OPTS	0x9017

typedef enum {
	CFP_AC200_PHASE_ENC_DIFF = 0,
	CFP_AC200_PHASE_ENC_NON_DIFF = 1
} CFP_AC200_PHASE_ENC_T;

#define CFP_AC200_REG_NTWK_AVE_BER_PM_WH	0x9052
#define CFP_AC200_REG_NTWK_AVE_BER_PM_WL	0x9053

static int32_t CFP_AC200_GetOTUFECBER(CFP_AC200_T *Dev)
{
	uint16_t Data16;
	uint32_t Data32;
	
	Data32 = 0;
	
	if (CFP_AC200_Read(Dev, CFP_AC200_REG_NTWK_AVE_BER_PM_WH, &Data16))
		return -1;
	Data32 |= (Data16 << 16);
	
	if (CFP_AC200_Read(Dev, CFP_AC200_REG_NTWK_AVE_BER_PM_WL, &Data16))
		return -1;
	Data32 |= Data16;
	
	memcpy(&Dev->OTUFECBER, &Data32, sizeof(Data32));
	return 0;	
}

static int32_t CFP_AC200_GetProductOps(CFP_AC200_T *Dev)
{
	uint16_t Data;
	
	if (CFP_AC200_Read(Dev, CFP_AC200_REG_PRODUCT_OPTS, &Data))
		return -1;
	
	Dev->ProductOpts = Data;
	return 0;
}

int32_t CFP_AC200_GetMode(CFP_AC200_T *Dev)
{
	uint16_t Data;
	
	if (CFP_AC200_Read(Dev, CFP_AC200_REG_GENERAL_MODE, &Data))
		return -1;
	
	Dev->FEC = (CFP_AC200_FEC_T)((Data >> 0) & 0x07);
	Dev->Modulation = (CFP_AC200_MODULATION_T)((Data >> 8) & 0x0F);
	switch (Dev->Modulation)
	{
		case CFP_AC200_MODULATION_QPSK:
			Dev->Signal = CFP_AC200_SIGNAL_OTU4;
			break;
		case CFP_AC200_MODULATION_8QAM:
		case CFP_AC200_MODULATION_16QAM:
			Dev->Signal = CFP_AC200_SIGNAL_OTUC2;
			break;
	}
	return 0;
}

static int32_t __CFP_AC200_SetMode(CFP_AC200_T *Dev, CFP_AC200_MODULATION_T Modulation, CFP_AC200_FEC_T FEC)
{
	uint16_t Data;
	
	/*Prepare command data*/
	switch (Modulation)
	{
		case CFP_AC200_MODULATION_QPSK:
			switch (FEC)
			{
				case CFP_AC200_FEC_SDFEC_EVEREST:
					Data = (CFP_AC200_MODULATION_QPSK << 8) + (CFP_AC200_PHASE_ENC_DIFF << 7) + (CFP_AC200_FEC_SDFEC_EVEREST << 0);
					break;
				case CFP_AC200_FEC_SDFEC_DENALI:
					Data = (CFP_AC200_MODULATION_QPSK << 8) + (CFP_AC200_PHASE_ENC_NON_DIFF << 7) + (CFP_AC200_FEC_SDFEC_DENALI << 0);
					break;
				case CFP_AC200_FEC_RSFEC:
					Data = (CFP_AC200_MODULATION_QPSK << 8) + (CFP_AC200_PHASE_ENC_DIFF << 7) + (CFP_AC200_FEC_RSFEC << 0);
					break;
				case CFP_AC200_FEC_TRANSPARENT:
					Data = (CFP_AC200_MODULATION_QPSK << 8) + (CFP_AC200_PHASE_ENC_DIFF << 7) + (CFP_AC200_FEC_TRANSPARENT << 0);
					break;
				case CFP_AC200_FEC_EFEC_STAIRCASE:
					Data = (CFP_AC200_MODULATION_QPSK << 8) + (CFP_AC200_PHASE_ENC_DIFF << 7) + (CFP_AC200_FEC_EFEC_STAIRCASE << 0);
					break;
				default:
					return -1;
			}			
			break;
		case CFP_AC200_MODULATION_8QAM:
			switch (FEC)
			{
				case CFP_AC200_FEC_SDFEC_DENALI:
					Data = (CFP_AC200_MODULATION_8QAM << 8) + (CFP_AC200_PHASE_ENC_NON_DIFF << 7) + (CFP_AC200_FEC_SDFEC_DENALI << 0);
					break;
				default:
					return -1;
			}
			break;
		case CFP_AC200_MODULATION_16QAM:
			switch (FEC)
			{
				case CFP_AC200_FEC_SDFEC_DENALI:
					Data = (CFP_AC200_MODULATION_16QAM << 8) + (CFP_AC200_PHASE_ENC_NON_DIFF << 7) + (CFP_AC200_FEC_SDFEC_DENALI << 0);
					break;
				default:
					return -1;
			}
			break;

		default:
			return -1;
	}	
	if (CFP_AC200_ExecuteCmd(Dev, CFP_AC200_REG_GENERAL_MODE, Data))
		return -1;
	
	return 0;
}

int32_t CFP_AC200_ConfigSignal(CFP_AC200_T *Dev, CFP_AC200_SIGNAL_T Signal)
{
	if (!Dev)
		return -1;
	
	switch (Signal)
	{
		case CFP_AC200_SIGNAL_OTU4:
			/* Check OTU4 support in Product Options*/
			if (!(Dev->ProductOpts & (1 << 1)))
				return -1;
			Dev->Signal = Signal;
			Dev->Modulation = CFP_AC200_MODULATION_QPSK;
			break;
		case CFP_AC200_SIGNAL_OTUC2:
			/* Check OTUC2 support in Product Options*/
			if (!(Dev->ProductOpts & ((1 << 2) || (1 << 0))))
				return -1;
			if (Dev->Modulation == CFP_AC200_MODULATION_8QAM)
			{
				Dev->Signal = Signal;
			}
			else if (Dev->Modulation == CFP_AC200_MODULATION_16QAM)
			{
				Dev->Signal = Signal;
			}
			else
			{
				Dev->Signal = Signal;
				Dev->Modulation = CFP_AC200_MODULATION_8QAM;
			}
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t CFP_AC200_ConfigModulation(CFP_AC200_T *Dev, CFP_AC200_MODULATION_T Modulation)
{
	if (!Dev)
		return -1;
	
	switch (Modulation)
	{
		case CFP_AC200_MODULATION_QPSK:
			/* Check QPSK support in Product Options*/
			if (!(Dev->ProductOpts & (1 << 1)))
				return -1;
			if (Dev->Signal != CFP_AC200_SIGNAL_OTU4)
				return -1;
			Dev->Modulation = Modulation;
			break;
		case CFP_AC200_MODULATION_8QAM:
			/* Check 8QAM format support in Product Options*/
			if (!(Dev->ProductOpts & (1 << 2)))
				return -1;
			if (Dev->Signal != CFP_AC200_SIGNAL_OTUC2)
				return -1;
			Dev->Modulation = Modulation;
			Dev->FEC = CFP_AC200_FEC_SDFEC_DENALI;
			break;
		case CFP_AC200_MODULATION_16QAM:
			/* Check 16QAM format support in Product Options*/
			if (!(Dev->ProductOpts & (1 << 0)))
				return -1;
			if (Dev->Signal != CFP_AC200_SIGNAL_OTUC2)
				return -1;
			Dev->Modulation = Modulation;
			Dev->FEC = CFP_AC200_FEC_SDFEC_DENALI;
			break;
		default:
			return -1;
	}	
	return 0;
}

int32_t CFP_AC200_ConfigFEC(CFP_AC200_T *Dev, CFP_AC200_FEC_T FEC)
{
	if (!Dev)
		return -1;
	
	switch (Dev->Modulation)
	{
		case CFP_AC200_MODULATION_QPSK:
			switch (FEC)
			{
				case CFP_AC200_FEC_SDFEC_EVEREST:
				case CFP_AC200_FEC_SDFEC_DENALI:
				case CFP_AC200_FEC_RSFEC:
				case CFP_AC200_FEC_TRANSPARENT:
				case CFP_AC200_FEC_EFEC_STAIRCASE:
					Dev->FEC = FEC;
					break;
				default:
					return -1;
			}			
			break;
		case CFP_AC200_MODULATION_8QAM:
			switch (FEC)
			{
				case CFP_AC200_FEC_SDFEC_DENALI:
					Dev->FEC = FEC;
					break;
				default:
					return -1;
			}
			break;
		case CFP_AC200_MODULATION_16QAM:
			switch (FEC)
			{
				case CFP_AC200_FEC_SDFEC_DENALI:
					Dev->FEC = FEC;
					break;
				default:
					return -1;
			}
			break;

		default:
			return -1;
	}	
	return 0;
}

int32_t CFP_AC200_UpdateMode(CFP_AC200_T *Dev)
{
	uint16_t Data;
	CFP_AC200_MODULATION_T Modulation;
	CFP_AC200_FEC_T FEC;
	
	if (!Dev)
		return -1;
	
	if (CFP_AC200_Read(Dev, CFP_AC200_REG_GENERAL_MODE, &Data))
		return -1;
	
	FEC = (CFP_AC200_FEC_T)((Data >> 0) & 0x07);
	Modulation = (CFP_AC200_MODULATION_T)((Data >> 8) & 0x0F);
	
	if ((FEC == Dev->FEC) && (Modulation == Dev->Modulation))
	{
		/*Return to High Power Mode*/
		if (Dev->Gen100GLH.Gen.State != CFP_GEN_STATE_HIGH_PWR)
		{
			if (CFP_AC200_SetState(Dev, CFP_GEN_STATE_HIGH_PWR))
				return -1;
		}
		return 0;
	}
	
	/*Modulation format and FEC type can be set in Low power state only*/
	if (Dev->Gen100GLH.State != CFP_GEN_100GLH_STATE_LOW_PWR)
	{
		if (CFP_AC200_SetState(Dev, CFP_GEN_STATE_LOW_PWR))
			return -1;
		return 0;
	}
	
	/*Prepare command data*/
	if (__CFP_AC200_SetMode(Dev, Dev->Modulation, Dev->FEC))
		return -1;
	
	return 0;
}

int32_t CFP_AC200_Init(CFP_AC200_T *Dev)
{
		/*Manualy update Tx Power threshoulds*/
	Dev->Gen100GLH.Gen.Thr.TxPwr[CFP_GEN_THR_TYPE_CMAX] = CFP_AC200_THR_TXPWR_CMAX;
	Dev->Gen100GLH.Gen.Thr.TxPwr[CFP_GEN_THR_TYPE_WMAX] = CFP_AC200_THR_TXPWR_WMAX;
	Dev->Gen100GLH.Gen.Thr.TxPwr[CFP_GEN_THR_TYPE_WMIN] = CFP_AC200_THR_TXPWR_WMIN;
	Dev->Gen100GLH.Gen.Thr.TxPwr[CFP_GEN_THR_TYPE_CMIN] = CFP_AC200_THR_TXPWR_CMIN;	
	
	if (CFP_GEN_100GLH_Init(&Dev->Gen100GLH))
		return -1;
	if (CFP_AC200_GetProductOps(Dev))
		return -1;
	if (CFP_AC200_GetMode(Dev))
		return -1;
	return 0;
}

int32_t CFP_AC200_Update(CFP_AC200_T *Dev)
{
	if (CFP_GEN_Update(&Dev->Gen100GLH.Gen))
		return -1;
	
	if (CFP_GEN_100GLH_Update(&Dev->Gen100GLH))
		return -1;
	
	if (Dev->Gen100GLH.State == CFP_GEN_100GLH_STATE_FAULT)
		return -1;
	
	if (CFP_AC200_UpdateMode(Dev))
		return -1;
	
	if (CFP_AC200_GetOTUFECBER(Dev))
		return -1;
	
	return 0;
}

int32_t CFP_AC200_CheckDeviceType(CFP_AC200_T *Dev)
{
	if (strncmp(Dev->Gen100GLH.Gen.Info.Vendor, CFP_AC200_VENDOR, sizeof(CFP_AC200_VENDOR)))
		return -1;
	if (memcmp(Dev->Gen100GLH.Gen.Info.PtNumber, CFP_AC200_PT_NUMBER, sizeof(CFP_AC200_PT_NUMBER) - 1))
		return -1;	
	return 0;
}

