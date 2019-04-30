#ifndef __CFP_H
#define __CFP_H

#include "bsp_dev.h"
#include "cfp_msa.h"
#define CFP_AC200_SUPPORT
#ifdef CFP_AC200_SUPPORT
#include "cfp_ac200.h"
#endif

typedef enum {
	CFP_MODE_SIGNAL_OTU4,
	CFP_MODE_SIGNAL_OTUC2
} CFP_MODE_SIGNAL_T;

typedef enum {
	CFP_MODE_MODULATION_NA,
	CFP_MODE_MODULATION_QPSK,
	CFP_MODE_MODULATION_8QAM,
	CFP_MODE_MODULATION_16QAM
} CFP_MODE_MODULATION_T;

typedef enum {
	CFP_MODE_FEC_NA,
	CFP_MODE_FEC_RSFEC,
	CFP_MODE_FEC_SDFEC_EVEREST,
	CFP_MODE_FEC_SDFEC_DENALI,
	CFP_MODE_FEC_EFEC_STAIRCASE
} CFP_MODE_FEC_T;

typedef enum {
	CFP_TYPE_MSA,
#ifdef CFP_AC200_SUPPORT
	CFP_TYPE_AC200,
#endif
} CFP_TYPE_T;

typedef union {	
	CFP_MSA_T MSA;
#ifdef CFP_AC200_SUPPORT
	CFP_AC200_T AC200;
#endif
} CFP_INSTANCE_T;

typedef struct {
	CFP_MODE_SIGNAL_T Signal;
	CFP_MODE_MODULATION_T Modulation;
	CFP_MODE_FEC_T FEC;
} CFP_MODE_T;

typedef struct {
	uint32_t Min;
	uint32_t Max;
	uint32_t Spacing;
} CFP_LANE_FREQ_INFO_T;

typedef struct {
	float Min;
	float Max;
} CFP_LANE_PWR_INFO_T;

typedef CFP_GEN_INFO_T CFP_INFO_T;
typedef CFP_GEN_THR_T CFP_THR_T;

typedef struct {
	BSP_DEV;
	CFP_TYPE_T Type;
	CFP_INSTANCE_T Instance;
} CFP_T;

int32_t CFP_ConfigModeSignal(CFP_T *Dev, CFP_MODE_SIGNAL_T Signal);
int32_t CFP_ConfigModeModulation(CFP_T *Dev, CFP_MODE_MODULATION_T Modulation);
int32_t CFP_ConfigModeFEC(CFP_T *Dev, CFP_MODE_FEC_T FEC);
int32_t CFP_ConfigMode(CFP_T *Dev, CFP_MODE_T *Mode);
int32_t CFP_ConfigLaneTxDisable(CFP_T *Dev, uint32_t Lane, uint32_t TxDisable);
int32_t CFP_ConfigLaneTxPwrCfg(CFP_T *Dev, uint32_t Lane, float TxPwrCfg);
int32_t CFP_ConfigLaneTxFreq(CFP_T *Dev, uint32_t Lane, uint32_t TxFreq);

int32_t CFP_GetLanesCnt(CFP_T *Dev, uint32_t *LanesCnt);
int32_t CFP_GetInfo(CFP_T *Dev, CFP_INFO_T *Info);
int32_t CFP_GetThr(CFP_T *Dev, CFP_THR_T *Thr);
int32_t CFP_GetMode(CFP_T *Dev, CFP_MODE_T *Mode);
int32_t CFP_GetTemp(CFP_T *Dev, float *Temp);
int32_t CFP_GetLaneTxFreq(CFP_T *Dev, uint32_t Lane, uint32_t *TxFreq);
int32_t CFP_GetLaneTxFreqInfo(CFP_T *Dev, uint32_t Lane, CFP_LANE_FREQ_INFO_T *Info);
int32_t CFP_GetLaneTxPwrCfg(CFP_T *Dev, uint32_t Lane, float *TxPwrCfg);
int32_t CFP_GetLaneTxPwrCfgInfo(CFP_T *Dev, uint32_t Lane, CFP_LANE_PWR_INFO_T *Info);
int32_t CFP_GetLaneRxLos(CFP_T *Dev, uint32_t Lane, uint32_t *RxLos);
int32_t CFP_GetLaneOTUAlarms(CFP_T *Dev, uint32_t Lane, uint32_t *OTUAlarms);
int32_t CFP_GetLaneOTUFECBER(CFP_T *Dev, uint32_t Lane, float *OTUFECBER);
int32_t CFP_GetLaneOTUSMBIP(CFP_T *Dev, uint32_t Lane, uint32_t *OTUSMBIP);
int32_t CFP_GetLaneOTUSMBEI(CFP_T *Dev, uint32_t Lane, uint32_t *OTUSMBEI);
int32_t CFP_GetLaneRxPwr(CFP_T *Dev, uint32_t Lane, float *RxPwr);
int32_t CFP_GetLaneTxPwr(CFP_T *Dev, uint32_t Lane, float *TxPwr);

int32_t CFP_Init(CFP_T *Dev, CFP_IFACE_T *Iface);
int32_t CFP_Update(CFP_T *Dev);
void CFP_Deinit(CFP_T *Dev);

#endif /* __CFP_H */
