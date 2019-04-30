#ifndef __CFP_GEN_H
#define __CFP_GEN_H

#include "cfp_iface.h"

#define CFP_GEN_MAX_LANES_CNT 16

typedef enum {
	CFP_GEN_STATE_RESET = 0,
	CFP_GEN_STATE_LOW_PWR = 1,
	CFP_GEN_STATE_HIGH_PWR = 2
} CFP_GEN_STATE_T;

typedef enum {
	CFP_GEN_THR_TYPE_CMAX = 0,
	CFP_GEN_THR_TYPE_WMAX = 1,
	CFP_GEN_THR_TYPE_WMIN = 2,
	CFP_GEN_THR_TYPE_CMIN = 3
} CFP_GEN_THR_TYPE_T;

typedef struct {
	uint32_t RxLos;
	uint32_t TxDisable;
} CFP_GEN_LANE_PIN_T;

typedef struct {
	uint32_t ModRst;
	uint32_t ModLowPwr;
	uint32_t ModAbs;
	uint32_t GlbAlrm;
	uint32_t PrgCtl[3];
	CFP_GEN_LANE_PIN_T Lane[CFP_GEN_MAX_LANES_CNT];
} CFP_GEN_PIN_T;

typedef struct {
	uint16_t Status;
	uint16_t StatusLatch;
	float RxPwr;
	float TxPwr;
} CFP_GEN_LANE_T;

typedef struct {
	uint8_t ModId;	
	char Vendor[17];
	char PtNumber[17];
	char SrNumber[17];
	char SwNumber[8];
	char HwNumber[8];
} CFP_GEN_INFO_T;

typedef struct {
	float RxPwr[4]; /*dBm*/
	float TxPwr[4]; /*dBm*/
	float Temp[4]; /*Deg*/
	float Power[4]; /*V*/
} CFP_GEN_THR_T;

typedef struct {
	CFP_IFACE_T *Iface;
	
	CFP_GEN_STATE_T State;
	CFP_GEN_PIN_T	Pin;	
	CFP_GEN_INFO_T Info;
	CFP_GEN_THR_T	Thr;
	
	uint8_t LanesCnt;
	
	uint32_t FreqMin; /*MHz*/
	uint32_t FreqMax; /*MHz*/
	uint8_t FreqGridCaps;	
	
} CFP_GEN_T;

#define CFP_GEN_Read(Dev, Addr, Data) CFP_IFACE_MDIO_Read((Dev)->Iface, (Addr), (Data))
#define CFP_GEN_Write(Dev, Addr, Data) CFP_IFACE_MDIO_Write((Dev)->Iface, (Addr), (Data))

#define CFP_GEN_GPIO_COM_Init(Dev, Pin) 						CFP_IFACE_GPIO_COM_Init((Dev)->Iface, (Pin))	
#define CFP_GEN_GPIO_COM_SetState(Dev, Pin, State) 	CFP_IFACE_GPIO_COM_SetState((Dev)->Iface, (Pin), (State))	
#define CFP_GEN_GPIO_COM_GetState(Dev, Pin, State) 	CFP_IFACE_GPIO_COM_GetState((Dev)->Iface, (Pin), (State))

#define CFP_GEN_GPIO_LANE_Init(Dev, Idx, Pin)						CFP_IFACE_GPIO_LANE_Init((Dev)->Iface, (Idx), (Pin))	
#define CFP_GEN_GPIO_LANE_SetState(Dev, Idx, Pin, State)	CFP_IFACE_GPIO_LANE_SetState((Dev)->Iface, (Idx), (Pin), (State))	
#define CFP_GEN_GPIO_LANE_GetState(Dev, Idx, Pin, State) CFP_IFACE_GPIO_LANE_GetState((Dev)->Iface, (Idx), (Pin), (State))

#define CFP_GEN_InitRxLos(Dev, Idx) 	CFP_GEN_GPIO_LANE_Init((Dev), (Idx), CFP_GPIO_LANE_TYPE_RX_LOS)
#define CFP_GEN_GetRxLos(Dev, Idx) 	CFP_GEN_GPIO_LANE_GetState((Dev), (Idx), CFP_GPIO_LANE_TYPE_RX_LOS, &(Dev)->Pin.Lane[Idx].RxLos)

#define CFP_GEN_InitModAbs(Dev)		CFP_GEN_GPIO_COM_Init((Dev), CFP_GPIO_COM_TYPE_MOD_ABS)
#define CFP_GEN_GetModAbs(Dev)		CFP_GEN_GPIO_COM_GetState((Dev), CFP_GPIO_COM_TYPE_MOD_ABS, &(Dev)->Pin.ModAbs)

#define CFP_GEN_InitGlbAlrm(Dev)	CFP_GEN_GPIO_COM_Init((Dev), CFP_GPIO_COM_TYPE_GLB_ALRM)
#define CFP_GEN_GetGlbAlrm(Dev)		CFP_GEN_GPIO_COM_GetState((Dev), CFP_GPIO_COM_TYPE_GLB_ALRM, &(Dev)->Pin.GlbAlrm)

#define CFP_GEN_InitTxDisable(Dev, Idx)						CFP_GEN_GPIO_LANE_Init((Dev), (Idx), CFP_GPIO_LANE_TYPE_TX_DIS)
#define CFP_GEN_GetTxDisable(Dev, Idx)						CFP_GEN_GPIO_LANE_GetState((Dev), (Idx), CFP_GPIO_LANE_TYPE_TX_DIS, &(Dev)->Pin.Lane[Idx].TxDisable)
#define CFP_GEN_SetTxDisable(Dev, Idx, State)			(CFP_GEN_GPIO_LANE_SetState((Dev), (Idx), CFP_GPIO_LANE_TYPE_TX_DIS, (State)) ? -1 : ((Dev)->Pin.Lane[Idx].TxDisable = (State), 0))
#define CFP_GEN_ConfigTxDisable(Dev, Idx, State)	(((Idx) >= (Dev)->LanesCnt) ? -1 : ((Dev)->Pin.Lane[Idx].TxDisable = (State), 0))
#define CFP_GEN_UpdateTxDisable(Dev, Idx)					CFP_GEN_GPIO_LANE_SetState((Dev), (Idx), CFP_GPIO_LANE_TYPE_TX_DIS, (Dev)->Pin.Lane[Idx].TxDisable)

#define CFP_GEN_InitModRst(Dev)				CFP_GEN_GPIO_COM_Init((Dev), CFP_GPIO_COM_TYPE_MOD_RST)
#define CFP_GEN_GetModRst(Dev)				CFP_GEN_GPIO_COM_GetState((Dev), CFP_GPIO_COM_TYPE_MOD_RST, &(Dev)->Pin.ModRst)
#define CFP_GEN_SetModRst(Dev, State) (CFP_GEN_GPIO_COM_SetState((Dev), CFP_GPIO_COM_TYPE_MOD_RST, (State)) ? -1 : ((Dev)->Pin.ModRst = (State), 0))

#define CFP_GEN_InitModLowPwr(Dev)				CFP_GEN_GPIO_COM_Init((Dev), CFP_GPIO_COM_TYPE_MOD_LOPWR)
#define CFP_GEN_GetModLowPwr(Dev)					CFP_GEN_GPIO_COM_GetState((Dev), CFP_GPIO_COM_TYPE_MOD_LOPWR, &(Dev)->Pin.ModLowPwr)
#define CFP_GEN_SetModLowPwr(Dev, State)	(CFP_GEN_GPIO_COM_SetState((Dev), CFP_GPIO_COM_TYPE_MOD_LOPWR, (State)) ? -1 : ((Dev)->Pin.ModLowPwr = (State), 0))

int32_t CFP_GEN_SetState(CFP_GEN_T *Dev, CFP_GEN_STATE_T State);
int32_t CFP_GEN_GetState(CFP_GEN_T *Dev, CFP_GEN_STATE_T *State);

int32_t CFP_GEN_Init(CFP_GEN_T *Dev, CFP_IFACE_T *Iface);
int32_t CFP_GEN_Update(CFP_GEN_T *Dev);

#endif /* __CFP_GEN_H */
