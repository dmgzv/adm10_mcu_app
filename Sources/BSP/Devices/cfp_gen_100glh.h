#ifndef __CFP_GEN_100GLH_H
#define __CFP_GEN_100GLH_H

#include "cfp_gen.h"
#include "bsp_config.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

typedef enum {
	CFP_GEN_100GLH_STATE_INIT = (1 << 0),
	CFP_GEN_100GLH_STATE_LOW_PWR = (1 << 1),
	CFP_GEN_100GLH_STATE_HI_PWR_UP = (1 << 2),
	CFP_GEN_100GLH_STATE_TX_OFF = (1 << 3),
	CFP_GEN_100GLH_STATE_TX_TURN_ON = (1 << 4),
	CFP_GEN_100GLH_STATE_READY = (1 << 5),
	CFP_GEN_100GLH_STATE_FAULT = (1 << 6),
	CFP_GEN_100GLH_STATE_TX_TURN_OFF = (1 << 7),
	CFP_GEN_100GLH_STATE_HI_PWR_DOWN = (1 << 8)
} CFP_GEN_100GLH_STATE_T;

typedef enum {
	CFP_GEN_100GLH_GRID_SPACING_100GHZ = 0,
	CFP_GEN_100GLH_GRID_SPACING_50GHZ = 1,
	CFP_GEN_100GLH_GRID_SPACING_33GHZ = 2,
	CFP_GEN_100GLH_GRID_SPACING_25GHZ = 3,
	CFP_GEN_100GLH_GRID_SPACING_12P5GHZ = 4,
	CFP_GEN_100GLH_GRID_SPACING_6P25GHZ = 5,
} CFP_GEN_100GLH_GRID_SPACING_T;

typedef struct {
	uint16_t Status;
	uint16_t StatusLatch;
	
	float RxPwr; /* dBm */
	float TxPwr; /* dBm */
	
	float TxPwrCfg;	/* dBm */
	
	uint32_t TxGridSpacing;	/* MHz */
	uint32_t TxFreq;
		
	uint32_t RxGridSpacing;	/* MHz */
	uint32_t RxFreq;
	
	uint16_t OTUAlarms;
	
} CFP_GEN_100GLH_LANE_T;

typedef struct {
	CFP_GEN_T Gen; /*Must be the first member*/
	
	uint16_t State;
	uint16_t StateLatch;
	uint16_t Status;
	uint16_t StatusLatch;
	
	uint16_t Alarms;
	uint16_t GlobalAlarms;
	uint16_t ModuleAlarms;
	uint16_t ModuleAlarmsLatch;
	uint16_t ModuleFaults;
	uint16_t ModuleFaultsLatch;
	uint16_t VendorAlarms;
	uint16_t VendorAlarmsLatch;
	
	float Temp; /* Deg */
	
	uint64_t OTUFECCor;
	uint64_t OTUFECUncor;
	uint64_t OTUSMBIP;
	uint64_t OTUSMBEI;
	
	CFP_GEN_100GLH_LANE_T Lane[CFP_GEN_MAX_LANES_CNT];
	
} CFP_GEN_100GLH_T;

#define CFP_GEN_100GLH_OTU_ALARMS_LOF			(1 << 15)
#define CFP_GEN_100GLH_OTU_ALARMS_OOF			(1 << 14) 
#define CFP_GEN_100GLH_OTU_ALARMS_LOM			(1 << 13) 
#define CFP_GEN_100GLH_OTU_ALARMS_OOM			(1 << 12) 
#define CFP_GEN_100GLH_OTU_ALARMS_SMIAE		(1 << 11) 
#define CFP_GEN_100GLH_OTU_ALARMS_SMBDI		(1 << 10)
#define CFP_GEN_100GLH_OTU_ALARMS_OTUAIS	(1 << 9) 
#define CFP_GEN_100GLH_OTU_ALARMS_ODUAIS	(1 << 8) 

#define CFP_GEN_100GLH_SetState(Dev, State)								CFP_GEN_SetState(&(Dev)->Gen, (State))

#define CFP_GEN_100GLH_SetTxDisable(Dev, Lane, TxDisable) CFP_GEN_SetTxDisable(&(Dev)->Gen, (Lane), (TxDisable))
#define CFP_GEN_100GLH_GetTxDisable(Dev, Lane) 						CFP_GEN_GetTxDisable(&(Dev)->Gen, (Lane)) 
#define CFP_GEN_100GLH_ConfigTxDisable(Dev, Lane, TxDisable) 	CFP_GEN_ConfigTxDisable(&(Dev)->Gen, (Lane), (TxDisable)) 

#define CFP_GEN_100GLH_SetState(Dev, State) 							CFP_GEN_SetState(&(Dev)->Gen, (State))
#define CFP_GEN_100GLH_GetState(Dev) 											CFP_GEN_GetState(&(Dev)->Gen)

#define CFP_GEN_100GLH_GetModAbs(Dev) 										CFP_GEN_GetModAbs(&(Dev)->Gen)
#define CFP_GEN_100GLH_GetRxLos(Dev, Lane) 								CFP_GEN_GetRxLos(&(Dev)->Gen, (Lane)) 

#define CFP_GEN_100GLH_Read(Dev, Addr, Data) 							CFP_GEN_Read(&(Dev)->Gen, (Addr), (Data))
#define CFP_GEN_100GLH_Write(Dev, Addr, Data) 						CFP_GEN_Write(&(Dev)->Gen, (Addr), (Data))
int32_t CFP_GEN_100GLH_ExecuteCmd(CFP_GEN_100GLH_T *Dev, uint16_t Addr, uint16_t Data);

int32_t CFP_GEN_100GLH_ReadStatus(CFP_GEN_100GLH_T *Dev);
int32_t CFP_GEN_100GLH_ReadAlarms(CFP_GEN_100GLH_T *Dev);
int32_t CFP_GEN_100GLH_ReadTemp(CFP_GEN_100GLH_T *Dev);
int32_t CFP_GEN_100GLH_ReadOTUPerformance(CFP_GEN_100GLH_T *Dev);

int32_t CFP_GEN_100GLH_ReadLaneRxPwr(CFP_GEN_100GLH_T *Dev, uint32_t Lane);
int32_t CFP_GEN_100GLH_ReadLaneTxPwr(CFP_GEN_100GLH_T *Dev, uint32_t Lane);
int32_t CFP_GEN_100GLH_ReadLaneStatus(CFP_GEN_100GLH_T *Dev, uint32_t Lane);
int32_t CFP_GEN_100GLH_ReadLaneOTUAlarms(CFP_GEN_100GLH_T *Dev, uint32_t Lane);

int32_t CFP_GEN_100GLH_SetLaneTxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane, uint32_t TxFreq);
int32_t CFP_GEN_100GLH_ReadLaneTxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane);
int32_t CFP_GEN_100GLH_ConfigLaneTxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane, uint32_t TxFreq);
int32_t CFP_GEN_100GLH_SetLaneRxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane, uint32_t RxFreq);
int32_t CFP_GEN_100GLH_ReadLaneRxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane);
int32_t CFP_GEN_100GLH_ConfigLaneRxFreq(CFP_GEN_100GLH_T *Dev, uint32_t Lane, uint32_t TxFreq);
int32_t CFP_GEN_100GLH_SetLaneTxPwrCfg(CFP_GEN_100GLH_T *Dev, uint32_t Lane, float TxPwrCfg);
int32_t CFP_GEN_100GLH_ConfigLaneTxPwrCfg(CFP_GEN_100GLH_T *Dev, uint32_t Lane, float TxPwrCfg);

int32_t CFP_GEN_100GLH_Init(CFP_GEN_100GLH_T *Dev);
int32_t CFP_GEN_100GLH_Update(CFP_GEN_100GLH_T *Dev);

#endif /* __CFP_GEN_100GLH_H */
