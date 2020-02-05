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
#define SFP_PIN_RATE_SEL		4

#define SFP_PRODUCT_OPTS_RDT_MASK					(1 << 7)
#define SFP_PRODUCT_OPTS_TX_TUNABLE_MASK	(1 << 6)
#define SFP_PRODUCT_OPTS_RATE_SELECT_MASK	(1 << 5)
#define SFP_PRODUCT_OPTS_TX_DISABLE_MASK	(1 << 4)
#define SFP_PRODUCT_OPTS_TX_FAULT_MASK		(1 << 3)
#define SFP_PRODUCT_OPTS_RX_LOS_INV_MASK	(1 << 2)
#define SFP_PRODUCT_OPTS_RX_LOS_MASK			(1 << 1)

#define SFP_SPEED_OF_LIGHT 299792458.0f

typedef struct {
	uint8_t ProductOpts;
	char Vendor[17];
	char SerialNumber[17];
	char PartNumber[17];
} SFP_INFO_T;

typedef struct {
	uint32_t ModAbs;
	uint32_t TxDisable;
	uint32_t TxFault;
	uint32_t RxLos;
	uint32_t RateSel;
} SFP_PIN_T;

typedef struct {
	float Temp[4];
	float RxPwr[4];
	float TxPwr[4];
} SFP_THR_T;

typedef struct {
	uint32_t Cur;
	uint32_t Min;
	uint32_t Max;
	uint16_t Spacing;
	uint16_t Channel;
} SFP_FREQ_T;

typedef struct {
	BSP_DEV;
	const BSP_I2C_CTRL_T *I2CCtrl;
	const BSP_PIN_CTRL_T *PinCtrl;
	SFP_PIN_T Pin;	
	SFP_INFO_T Info;
	SFP_THR_T Thr;
	SFP_FREQ_T Freq;
	float Temp;
	float RxPwr;
	float TxPwr;
	float RDT;
} SFP_T;

int32_t SFP_GetInfo(SFP_T *Dev, SFP_INFO_T *Info);
int32_t SFP_GetThr(SFP_T *Dev, SFP_THR_T *Thr);
int32_t SFP_GetRxPwr(SFP_T *Dev, float *RxPwr);
int32_t SFP_GetTxPwr(SFP_T *Dev, float *TxPwr);
int32_t SFP_GetTemp(SFP_T *Dev, float *Temp);
int32_t SFP_GetModAbs(SFP_T *Dev, uint32_t *ModAbs);
int32_t SFP_GetRxLos(SFP_T *Dev, uint32_t *RxLos);
int32_t SFP_GetTxFault(SFP_T *Dev, uint32_t *TxFault);

int32_t SFP_GetRateSel(SFP_T *Dev, uint32_t *RateSel);
int32_t SFP_SetRateSel(SFP_T *Dev, uint32_t RateSel);
int32_t SFP_ConfigRateSel(SFP_T *Dev, uint32_t RateSel);

int32_t SFP_GetTxDisable(SFP_T *Dev, uint32_t *TxDisable);
int32_t SFP_SetTxDisable(SFP_T *Dev, uint32_t TxDisable);
int32_t SFP_ConfigTxDisable(SFP_T *Dev, uint32_t TxDisable);

int32_t SFP_WriteFreq(SFP_T *Dev, uint32_t Freq);
int32_t SFP_ConfigFreq(SFP_T *Dev, uint32_t Freq);

int32_t SFP_WriteRDT(SFP_T *Dev, float RDT);
int32_t SFP_ConfigRDT(SFP_T *Dev, float RDT);

int32_t SFP_Update(SFP_T *Dev);
int32_t SFP_Init(SFP_T *Dev, const char *DevId, const BSP_I2C_CTRL_T *I2CCtrl, const BSP_PIN_CTRL_T *PinCtrl);
void SFP_Deinit(SFP_T *Dev);


#endif /* __SFP_H */
