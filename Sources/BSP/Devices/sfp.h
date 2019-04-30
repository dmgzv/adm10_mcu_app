#ifndef __SFP_H
#define __SFP_H

#include "i2c.h"
#include "bsp_dev.h"
#include "bsp_pin_ctrl.h"
#include "bsp_i2c_ctrl.h"

#define SFP_THR_CMAX	0
#define SFP_THR_WMAX	1
#define SFP_THR_WMIN	2
#define SFP_THR_CMIN	3

#define SFP_PIN_MOD_ABS			0
#define SFP_PIN_TX_DISABLE	1
#define SFP_PIN_TX_FAULT		2
#define SFP_PIN_RX_LOS			3

typedef struct {
	char Vendor[17];
	char SerialNumber[17];
	char PartNumber[17];
} SFP_INFO_T;

typedef struct {
	uint32_t ModAbs;
	uint32_t TxDisable;
	uint32_t TxFault;
	uint32_t RxLos;
} SFP_PIN_T;

typedef struct {
	float Temp[4];
	float RxPwr[4];
	float TxPwr[4];
} SFP_THR_T;

typedef struct {
	BSP_DEV;
	BSP_I2C_CTRL_T *I2CCtrl;
	BSP_PIN_CTRL_T *PinCtrl;
	SFP_PIN_T Pin;	
	SFP_INFO_T Info;
	SFP_THR_T Thr;	
	float Temp;
	float RxPwr;
	float TxPwr;
} SFP_T;

int32_t SFP_GetInfo(SFP_T *Dev, SFP_INFO_T *Info);
int32_t SFP_GetThr(SFP_T *Dev, SFP_THR_T *Thr);
int32_t SFP_GetRxPwr(SFP_T *Dev, float *RxPwr);
int32_t SFP_GetTxPwr(SFP_T *Dev, float *TxPwr);
int32_t SFP_GetTemp(SFP_T *Dev, float *Temp);
int32_t SFP_GetModAbs(SFP_T *Dev, uint32_t *ModAbs);
int32_t SFP_GetRxLos(SFP_T *Dev, uint32_t *RxLos);
int32_t SFP_GetTxDisable(SFP_T *Dev, uint32_t *TxDisable);
int32_t SFP_GetTxFault(SFP_T *Dev, uint32_t *TxFault);

int32_t SFP_SetTxDisable(SFP_T *Dev, uint32_t TxDisable);

int32_t SFP_ConfigTxDisable(SFP_T *Dev, uint32_t TxDisable);

int32_t SFP_Update(SFP_T *Dev);
int32_t SFP_Init(SFP_T *Dev, BSP_I2C_CTRL_T *I2CCtrl, BSP_PIN_CTRL_T *PinCtrl);
void SFP_Deinit(SFP_T *Dev);


#endif /* __SFP_H */
