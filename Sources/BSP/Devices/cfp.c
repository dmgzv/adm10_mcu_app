#include "cfp.h"
#include <string.h>

int32_t CFP_ConfigModeSignal(CFP_T *Dev, CFP_MODE_SIGNAL_T Signal)
{
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			switch (Signal)
			{
				case CFP_MODE_SIGNAL_OTU4:
					if (CFP_AC200_ConfigSignal(&Dev->Instance.AC200, CFP_AC200_SIGNAL_OTU4))
						goto error;
					break;
				case CFP_MODE_SIGNAL_OTUC2:
					if (CFP_AC200_ConfigSignal(&Dev->Instance.AC200, CFP_AC200_SIGNAL_OTUC2))
						goto error;
					break;
				default:
					goto error;
			}
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			switch (Signal)
			{
				case CFP_MODE_SIGNAL_OTU4:
					break;
				case CFP_MODE_SIGNAL_OTUC2:
				default:
					goto error;
			}
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_ConfigModeModulation(CFP_T *Dev, CFP_MODE_MODULATION_T Modulation)
{
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			switch (Modulation)
			{
				case CFP_MODE_MODULATION_QPSK:
					if (CFP_AC200_ConfigModulation(&Dev->Instance.AC200, CFP_AC200_MODULATION_QPSK))
						goto error;
					break;
				case CFP_MODE_MODULATION_8QAM:
					if (CFP_AC200_ConfigModulation(&Dev->Instance.AC200, CFP_AC200_MODULATION_8QAM))
						goto error;
					break;
				case CFP_MODE_MODULATION_16QAM:
					if (CFP_AC200_ConfigModulation(&Dev->Instance.AC200, CFP_AC200_MODULATION_16QAM))
						goto error;
					break;
				default:
					goto error;
			}
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			switch (Modulation)
			{
				case CFP_MODE_MODULATION_NA:
					break;
				case CFP_MODE_MODULATION_QPSK:
				case CFP_MODE_MODULATION_8QAM:
				case CFP_MODE_MODULATION_16QAM:
				default:
					goto error;
			}
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_ConfigModeFEC(CFP_T *Dev, CFP_MODE_FEC_T FEC)
{
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			switch (FEC)
			{
				case CFP_MODE_FEC_NA:
					if (CFP_AC200_ConfigFEC(&Dev->Instance.AC200, CFP_AC200_FEC_TRANSPARENT))
						goto error;
					break;
				case CFP_MODE_FEC_RSFEC:
					if (CFP_AC200_ConfigFEC(&Dev->Instance.AC200, CFP_AC200_FEC_RSFEC))
						goto error;
					break;
				case CFP_MODE_FEC_EFEC_STAIRCASE:
					if (CFP_AC200_ConfigFEC(&Dev->Instance.AC200, CFP_AC200_FEC_EFEC_STAIRCASE))
						goto error;
					break;
				case CFP_MODE_FEC_SDFEC_EVEREST:
					if (CFP_AC200_ConfigFEC(&Dev->Instance.AC200, CFP_AC200_FEC_SDFEC_EVEREST))
						goto error;
					break;
					case CFP_MODE_FEC_SDFEC_DENALI:
					if (CFP_AC200_ConfigFEC(&Dev->Instance.AC200, CFP_AC200_FEC_SDFEC_DENALI))
						goto error;
					break;
				default:
					goto error;
			}
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			switch (FEC)
			{
				case CFP_MODE_FEC_NA:
					break;
				case CFP_MODE_FEC_RSFEC:
				case CFP_MODE_FEC_SDFEC_EVEREST:
				case CFP_MODE_FEC_SDFEC_DENALI:
				case CFP_MODE_FEC_EFEC_STAIRCASE:
				default:
					goto error;
			}
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_ConfigMode(CFP_T *Dev, CFP_MODE_T *Mode)
{
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			switch (Mode->Signal)
			{
				case CFP_MODE_SIGNAL_OTU4:
					if (CFP_AC200_ConfigSignal(&Dev->Instance.AC200, CFP_AC200_SIGNAL_OTU4))
						goto error;
					break;
				case CFP_MODE_SIGNAL_OTUC2:
					if (CFP_AC200_ConfigSignal(&Dev->Instance.AC200, CFP_AC200_SIGNAL_OTUC2))
						goto error;
					break;
				default:
					goto error;
			}
			switch (Mode->Modulation)
			{
				case CFP_MODE_MODULATION_QPSK:
					if (CFP_AC200_ConfigModulation(&Dev->Instance.AC200, CFP_AC200_MODULATION_QPSK))
						goto error;
					break;
				case CFP_MODE_MODULATION_8QAM:
					if (CFP_AC200_ConfigModulation(&Dev->Instance.AC200, CFP_AC200_MODULATION_8QAM))
						goto error;
					break;
				case CFP_MODE_MODULATION_16QAM:
					if (CFP_AC200_ConfigModulation(&Dev->Instance.AC200, CFP_AC200_MODULATION_16QAM))
						goto error;
					break;
				default:
					goto error;
			}
			switch (Mode->FEC)
			{
				case CFP_MODE_FEC_NA:
					if (CFP_AC200_ConfigFEC(&Dev->Instance.AC200, CFP_AC200_FEC_TRANSPARENT))
						goto error;
					break;
				case CFP_MODE_FEC_RSFEC:
					if (CFP_AC200_ConfigFEC(&Dev->Instance.AC200, CFP_AC200_FEC_RSFEC))
						goto error;
					break;
				case CFP_MODE_FEC_EFEC_STAIRCASE:
					if (CFP_AC200_ConfigFEC(&Dev->Instance.AC200, CFP_AC200_FEC_EFEC_STAIRCASE))
						goto error;
					break;
				case CFP_MODE_FEC_SDFEC_EVEREST:
					if (CFP_AC200_ConfigFEC(&Dev->Instance.AC200, CFP_AC200_FEC_SDFEC_EVEREST))
						goto error;
					break;
					case CFP_MODE_FEC_SDFEC_DENALI:
					if (CFP_AC200_ConfigFEC(&Dev->Instance.AC200, CFP_AC200_FEC_SDFEC_DENALI))
						goto error;
					break;
				default:
					goto error;
			}			
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			switch (Mode->Signal)
			{
				case CFP_MODE_SIGNAL_OTU4:
					break;
				case CFP_MODE_SIGNAL_OTUC2:
				default:
					goto error;
			}
			switch (Mode->Modulation)
			{
				case CFP_MODE_MODULATION_NA:
					break;
				case CFP_MODE_MODULATION_QPSK:
				case CFP_MODE_MODULATION_8QAM:
				case CFP_MODE_MODULATION_16QAM:
				default:
					goto error;
			}
			switch (Mode->FEC)
			{
				case CFP_MODE_FEC_NA:
					break;
				case CFP_MODE_FEC_RSFEC:
				case CFP_MODE_FEC_SDFEC_EVEREST:
				case CFP_MODE_FEC_SDFEC_DENALI:
				case CFP_MODE_FEC_EFEC_STAIRCASE:
				default:
					goto error;
			}
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_ConfigLaneTxDisable(CFP_T *Dev, uint32_t Lane, uint32_t TxDisable)
{
	uint32_t i;
	
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			for (i = 0; i < Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt; i++)
			{
				if (CFP_AC200_ConfigTxDisable(&Dev->Instance.AC200, TxDisable))
					goto error;
			}
			break;
#endif
		case CFP_TYPE_MSA:
			for (i = 0; i < Dev->Instance.MSA.Gen.LanesCnt; i++)
			{
				if (CFP_MSA_ConfigTxDisable(&Dev->Instance.MSA, i, TxDisable))
					goto error;
			}
			break;
		default:
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_ConfigLaneTxPwrCfg(CFP_T *Dev, uint32_t Lane, float TxPwr)
{
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (CFP_AC200_ConfigLaneTxPwrCfg(&Dev->Instance.AC200, TxPwr))
				goto error;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_ConfigLaneTxFreq(CFP_T *Dev, uint32_t Lane, uint32_t TxFreq)
{
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (CFP_AC200_ConfigLaneTxFreq(&Dev->Instance.AC200, TxFreq))
				goto error;
			if (CFP_AC200_ConfigLaneRxFreq(&Dev->Instance.AC200, TxFreq))
				goto error;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_GetLanesCnt(CFP_T *Dev, uint32_t *LanesCnt)
{
	if ((!Dev) || (!LanesCnt))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			*LanesCnt = Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			*LanesCnt = Dev->Instance.MSA.Gen.LanesCnt;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t CFP_GetInfo(CFP_T *Dev, CFP_INFO_T *Info)
{
	if ((!Dev) || (!Info))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			memcpy(Info, &Dev->Instance.AC200.Gen100GLH.Gen.Info,  sizeof(Dev->Instance.AC200.Gen100GLH.Gen.Info));
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			memcpy(Info, &Dev->Instance.MSA.Gen.Info,  sizeof(Dev->Instance.MSA.Gen.Info));
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t CFP_GetThr(CFP_T *Dev, CFP_THR_T *Thr)
{
	if ((!Dev) || (!Thr))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			memcpy(Thr, &Dev->Instance.AC200.Gen100GLH.Gen.Thr,  sizeof(Dev->Instance.AC200.Gen100GLH.Gen.Thr));
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			memcpy(Thr, &Dev->Instance.MSA.Gen.Thr,  sizeof(Dev->Instance.MSA.Gen.Thr));
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t CFP_GetMode(CFP_T *Dev, CFP_MODE_T *Mode)
{
	if ((!Dev) || (!Mode))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			switch (Dev->Instance.AC200.Signal)
			{
				case CFP_AC200_SIGNAL_OTUC2:
					Mode->Signal = CFP_MODE_SIGNAL_OTUC2;
					break;
				case CFP_AC200_SIGNAL_OTU4:
					Mode->Signal = CFP_MODE_SIGNAL_OTU4;
					break;
			}
			switch (Dev->Instance.AC200.Modulation)
			{
				case CFP_AC200_MODULATION_16QAM:
					Mode->Modulation = CFP_MODE_MODULATION_16QAM;
					break;
				case CFP_AC200_MODULATION_8QAM:
					Mode->Modulation = CFP_MODE_MODULATION_8QAM;
					break;
				case CFP_AC200_MODULATION_QPSK:
					Mode->Modulation = CFP_MODE_MODULATION_QPSK;
					break;
			}
			switch (Dev->Instance.AC200.FEC)
			{
				case CFP_AC200_FEC_SDFEC_EVEREST:
					Mode->FEC = CFP_MODE_FEC_SDFEC_EVEREST;
					break;
				case CFP_AC200_FEC_SDFEC_DENALI:
					Mode->FEC = CFP_MODE_FEC_SDFEC_DENALI;
					break;
				case CFP_AC200_FEC_RSFEC:
					Mode->FEC = CFP_MODE_FEC_RSFEC;
					break;
				case CFP_AC200_FEC_TRANSPARENT:
					Mode->FEC = CFP_MODE_FEC_NA;
					break;
				case CFP_AC200_FEC_EFEC_STAIRCASE:
					Mode->FEC = CFP_MODE_FEC_EFEC_STAIRCASE;
					break;
			}
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			Mode->Signal = CFP_MODE_SIGNAL_OTU4;
			Mode->Modulation = CFP_MODE_MODULATION_QPSK;
			Mode->FEC = CFP_MODE_FEC_RSFEC;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t CFP_GetTemp(CFP_T *Dev, float *Temp)
{
	if ((!Dev) || (!Temp))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			*Temp = Dev->Instance.AC200.Gen100GLH.Temp;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			*Temp = Dev->Instance.MSA.Temp;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t CFP_GetLaneTxFreq(CFP_T *Dev, uint32_t Lane, uint32_t *Freq)
{
	if ((!Dev) || (!Freq))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (Lane >= Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt)
				goto error;
			*Freq = Dev->Instance.AC200.Gen100GLH.Lane[Lane].TxFreq;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (Lane >= Dev->Instance.MSA.Gen.LanesCnt)
				goto error;
			// todo: find how to get lane freq from CFP MSA
			*Freq = 193100000;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_GetLaneTxFreqInfo(CFP_T *Dev, uint32_t Lane, CFP_LANE_FREQ_INFO_T *Info)
{
	if ((!Dev) || (!Info))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (Lane >= Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt)
				goto error;
			Info->Max = Dev->Instance.AC200.Gen100GLH.Gen.FreqMax;
			Info->Min = Dev->Instance.AC200.Gen100GLH.Gen.FreqMin;
			Info->Spacing = Dev->Instance.AC200.Gen100GLH.Lane[Lane].TxGridSpacing;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (Lane >= Dev->Instance.MSA.Gen.LanesCnt)
				goto error;
			Info->Max = 193100000;
			Info->Min = 193100000;
			Info->Spacing = 50000;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_GetLaneTxPwrCfg(CFP_T *Dev, uint32_t Lane, float *TxPwrCfg)
{
		if ((!Dev) || (!TxPwrCfg))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (Lane >= Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt)
				goto error;
			*TxPwrCfg = Dev->Instance.AC200.Gen100GLH.Lane[Lane].TxPwrCfg;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (Lane >= Dev->Instance.MSA.Gen.LanesCnt)
				goto error;
			*TxPwrCfg = 0;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}
int32_t CFP_GetLaneTxPwrCfgInfo(CFP_T *Dev, uint32_t Lane, CFP_LANE_PWR_INFO_T *Info)
{
		if ((!Dev) || (!Info))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (Lane >= Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt)
				goto error;
			Info->Max = CFP_AC200_THR_TXPWR_CMAX;
			Info->Min = CFP_AC200_THR_TXPWR_CMIN;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (Lane >= Dev->Instance.MSA.Gen.LanesCnt)
				goto error;
			Info->Max = 0;
			Info->Min = 0;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_GetLaneRxLos(CFP_T *Dev, uint32_t Lane, uint32_t *RxLos)
{
	if ((!Dev) || (!RxLos))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (Lane >= Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt)
				goto error;
			*RxLos = Dev->Instance.AC200.Gen100GLH.Gen.Pin.Lane[Lane].RxLos;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (Lane >= Dev->Instance.MSA.Gen.LanesCnt)
				goto error;
			*RxLos = Dev->Instance.MSA.Gen.Pin.Lane[Lane].RxLos;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_GetLaneOTUAlarms(CFP_T *Dev, uint32_t Lane, uint32_t *OTUAlarms)
{
	if ((!Dev) || (!OTUAlarms))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (Lane >= Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt)
				goto error;
			*OTUAlarms = Dev->Instance.AC200.Gen100GLH.Lane[Lane].OTUAlarms;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (Lane >= Dev->Instance.MSA.Gen.LanesCnt)
				goto error;
			*OTUAlarms = 0;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_GetLaneOTUFECBER(CFP_T *Dev, uint32_t Lane, float *OTUFECBER)
{
	if ((!Dev) || (!OTUFECBER))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (Lane >= Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt)
				goto error;
			*OTUFECBER = Dev->Instance.AC200.OTUFECBER;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (Lane >= Dev->Instance.MSA.Gen.LanesCnt)
				goto error;
			*OTUFECBER = 0;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_GetLaneOTUSMBIP(CFP_T *Dev, uint32_t Lane, uint32_t *OTUSMBIP)
{
	if ((!Dev) || (!OTUSMBIP))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (Lane >= Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt)
				goto error;
			*OTUSMBIP = Dev->Instance.AC200.Gen100GLH.OTUSMBIP;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (Lane >= Dev->Instance.MSA.Gen.LanesCnt)
				goto error;
			*OTUSMBIP = 0;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_GetLaneOTUSMBEI(CFP_T *Dev, uint32_t Lane, uint32_t *OTUSMBEI)
{
	if ((!Dev) || (!OTUSMBEI))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (Lane >= Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt)
				goto error;
			*OTUSMBEI = Dev->Instance.AC200.Gen100GLH.OTUSMBEI;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (Lane >= Dev->Instance.MSA.Gen.LanesCnt)
				goto error;
			*OTUSMBEI = 0;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_GetLaneRxPwr(CFP_T *Dev, uint32_t Lane, float *RxPwr)
{
	if ((!Dev) || (!RxPwr))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (Lane >= Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt)
				goto error;
			*RxPwr = Dev->Instance.AC200.Gen100GLH.Lane[Lane].RxPwr;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (Lane >= Dev->Instance.MSA.Gen.LanesCnt)
				goto error;
			*RxPwr = Dev->Instance.MSA.Lane[Lane].RxPwr;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_GetLaneTxPwr(CFP_T *Dev, uint32_t Lane, float *TxPwr)
{
	if ((!Dev) || (!TxPwr))
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (Lane >= Dev->Instance.AC200.Gen100GLH.Gen.LanesCnt)
				goto error;
			*TxPwr = Dev->Instance.AC200.Gen100GLH.Lane[Lane].TxPwr;
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (Lane >= Dev->Instance.MSA.Gen.LanesCnt)
				goto error;
			*TxPwr = Dev->Instance.MSA.Lane[Lane].TxPwr;
			break;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t CFP_Init(CFP_T *Dev, CFP_IFACE_T *Iface)
{
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->Type = CFP_TYPE_MSA;
	if (CFP_MSA_Init(&Dev->Instance.MSA, Iface))
		goto reset;

#ifdef CFP_AC200_SUPPORT
	if (!CFP_AC200_CheckDeviceType(&Dev->Instance.AC200))
	{
		Dev->Type = CFP_TYPE_AC200;
		if (CFP_AC200_Init(&Dev->Instance.AC200))
			goto error;
	}
#endif
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;

reset:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
	BSP_DEV_UNLOCK(Dev);
	return 0;	
}

int32_t CFP_Update(CFP_T *Dev)
{
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Dev->Type)
	{
#ifdef CFP_AC200_SUPPORT
		case CFP_TYPE_AC200:
			if (CFP_AC200_Update(&Dev->Instance.AC200))
			{
				if (Dev->Instance.AC200.Gen100GLH.Gen.Pin.ModAbs)
					goto reset;
				goto error;
			}
			break;
#endif
		case CFP_TYPE_MSA:
		default:
			if (CFP_MSA_Update(&Dev->Instance.MSA))
			{
				if (Dev->Instance.MSA.Gen.Pin.ModAbs)
					goto reset;
				goto error;
			}
			break;
	}
	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;

reset:
	BSP_DEV_DEINIT(Dev);
	BSP_DEV_UNLOCK(Dev);
	return 0;	
}

void CFP_Deinit(CFP_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
