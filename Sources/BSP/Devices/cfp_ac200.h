#ifndef __CFP_AC200_H
#define __CFP_AC200_H

#include "cfp_gen_100glh.h"

#define CFP_AC200_THR_TXPWR_CMAX	(1.0f)
#define CFP_AC200_THR_TXPWR_WMAX	(0.5f)
#define CFP_AC200_THR_TXPWR_WMIN	(-14.0f)
#define CFP_AC200_THR_TXPWR_CMIN	(-15.0f)

typedef enum {
	CFP_AC200_SIGNAL_OTU4,
	CFP_AC200_SIGNAL_OTUC2
} CFP_AC200_SIGNAL_T;

typedef enum {
	CFP_AC200_MODULATION_16QAM = 0,
	CFP_AC200_MODULATION_QPSK = 1,
	CFP_AC200_MODULATION_8QAM = 2
}	CFP_AC200_MODULATION_T;

typedef enum {
	CFP_AC200_FEC_SDFEC_EVEREST = 0,
	CFP_AC200_FEC_SDFEC_DENALI = 1,
	CFP_AC200_FEC_RSFEC = 2,
	CFP_AC200_FEC_TRANSPARENT = 3,
	CFP_AC200_FEC_EFEC_STAIRCASE = 4	
} CFP_AC200_FEC_T;

typedef struct {
	CFP_GEN_100GLH_T Gen100GLH; /*Must be the first member*/
	CFP_AC200_SIGNAL_T Signal;
	CFP_AC200_MODULATION_T	Modulation;
	CFP_AC200_FEC_T	FEC;
	uint16_t ProductOpts;
	float OTUFECBER;
} CFP_AC200_T;

#define CFP_AC200_SetTxDisable(Dev, TxDisable)		CFP_GEN_100GLH_SetTxDisable(&(Dev)->Gen100GLH, 0, (TxDisable))
#define CFP_AC200_GetTxDisable(Dev) 							CFP_GEN_100GLH_GetTxDisable(&(Dev)->Gen100GLH, 0)
#define CFP_AC200_ConfigTxDisable(Dev, TxDisable) CFP_GEN_100GLH_ConfigTxDisable(&(Dev)->Gen100GLH, 0, (TxDisable))
#define CFP_AC200_GetTxDisable(Dev) 							CFP_GEN_100GLH_GetTxDisable(&(Dev)->Gen100GLH, 0) 
#define CFP_AC200_GetRxLos(Dev)										CFP_GEN_100GLH_GetRxLos(&(Dev)->Gen100GLH, 0) 
#define CFP_AC200_SetState(Dev, State)						CFP_GEN_100GLH_SetState(&(Dev)->Gen100GLH, (State))
#define CFP_AC200_GetState(Dev) 									CFP_GEN_100GLH_GetState(&(Dev)->Gen100GLH)
#define CFP_AC200_GetModAbs(Dev) 									CFP_GEN_100GLH_GetModAbs(&(Dev)->Gen100GLH)

#define CFP_AC200_Read(Dev, Addr, Data) 					CFP_GEN_100GLH_Read(&(Dev)->Gen100GLH, (Addr), (Data))
#define CFP_AC200_Write(Dev, Addr, Data) 					CFP_GEN_100GLH_Write(&(Dev)->Gen100GLH, (Addr), (Data))
#define CFP_AC200_ExecuteCmd(Dev, Addr, Data) 		CFP_GEN_100GLH_ExecuteCmd(&(Dev)->Gen100GLH, (Addr), (Data))

#define CFP_AC200_ConfigLaneTxFreq(Dev, TxFreq)		CFP_GEN_100GLH_ConfigLaneTxFreq(&(Dev)->Gen100GLH, 0, (TxFreq))
#define CFP_AC200_ConfigLaneRxFreq(Dev, RxFreq)		CFP_GEN_100GLH_ConfigLaneRxFreq(&(Dev)->Gen100GLH, 0, (RxFreq))
#define CFP_AC200_ConfigLaneTxPwrCfg(Dev, TxPwrCfg)		CFP_GEN_100GLH_ConfigLaneTxPwrCfg(&(Dev)->Gen100GLH, 0, (TxPwrCfg))

int32_t CFP_AC200_CheckDeviceType(CFP_AC200_T *Dev);

int32_t CFP_AC200_ConfigSignal(CFP_AC200_T *Dev, CFP_AC200_SIGNAL_T Signal);
int32_t CFP_AC200_ConfigModulation(CFP_AC200_T *Dev, CFP_AC200_MODULATION_T Modulation);
int32_t CFP_AC200_ConfigFEC(CFP_AC200_T *Dev, CFP_AC200_FEC_T FEC);

int32_t CFP_AC200_Init(CFP_AC200_T *Dev);
int32_t CFP_AC200_Update(CFP_AC200_T *Dev);

#endif /* __CFP_AC200_H */
