#ifndef __CFP_MSA_H
#define __CFP_MSA_H

#include "cfp_gen.h"

typedef struct {
	uint16_t Status;
	uint16_t StatusLatch;
	float RxPwr;
	float TxPwr;
} CFP_MSA_LANE_T;

typedef struct {
	CFP_GEN_T Gen;
	
	float Temp;
	float Power;
	
	uint16_t ModuleState;
	uint16_t ModuleStateLatch;
	uint16_t GeneralStatus;
	uint16_t GeneralStatusLatch;
	uint16_t FaultStatus;
	uint16_t FaultStatusLatch;
	
	CFP_MSA_LANE_T Lane[CFP_GEN_MAX_LANES_CNT];
	
} CFP_MSA_T;

#define CFP_MSA_SetTxDisable(Dev, Lane, TxDisable)		CFP_GEN_SetTxDisable(&(Dev)->Gen, (Lane), (TxDisable))
#define CFP_MSA_GetTxDisable(Dev, Lane) 							CFP_GEN_GetTxDisable(&(Dev)->Gen, (Lane)) 
#define CFP_MSA_ConfigTxDisable(Dev, Lane, TxDisable)	CFP_GEN_ConfigTxDisable(&(Dev)->Gen, (Lane), (TxDisable))

#define CFP_MSA_SetState(Dev, State) 								CFP_GEN_SetState(&(Dev)->Gen, (State))
#define CFP_MSA_GetState(Dev) 											CFP_GEN_GetState(&(Dev)->Gen)

#define CFP_MSA_GetModAbs(Dev) 											CFP_GEN_GetModAbs(&(Dev)->Gen)
#define CFP_MSA_GetRxLos(Dev, Lane) 								CFP_GEN_GetRxLos(&(Dev)->Gen, (Lane)) 

#define CFP_MSA_Read(Dev, Addr, Data) 							CFP_GEN_Read(&(Dev)->Gen, (Addr), (Data))
#define CFP_MSA_Write(Dev, Addr, Data) 							CFP_GEN_Write(&(Dev)->Gen, (Addr), (Data))

#define CFP_MSA_SetState(Dev, State)								CFP_GEN_SetState(&(Dev)->Gen, (State))

int32_t CFP_MSA_Init(CFP_MSA_T *Dev, CFP_IFACE_T *Iface);
int32_t CFP_MSA_Update(CFP_MSA_T *Dev);

#endif /* __CFP_MSA_H */
