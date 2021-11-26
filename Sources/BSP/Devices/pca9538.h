#ifndef __PCA9538_H
#define __PCA9538_H

#include "bsp_dev.h"
#include "i2c.h"

#define PCA9538_DEV_NAME "PCA9538"

#define PCA9538_PIN_DIR_OUT	0
#define PCA9538_PIN_DIR_IN	1

#define PCA9538_BANK_DIR_OUT	0
#define PCA9538_BANK_DIR_IN		0xFF

typedef struct {
	BSP_DEV;
	const I2C_TypeDef *I2C;
	uint8_t Addr;
	uint8_t Ins;
	uint8_t Outs;
	uint8_t Dirs;
	uint8_t Pols;
} PCA9538_T;

int32_t PCA9538_SetPinDir(PCA9538_T *Dev, uint8_t Pin, uint8_t Dir);
int32_t PCA9538_SetPinOut(PCA9538_T *Dev, uint8_t Pin, uint8_t Out);
int32_t PCA9538_SetPinPol(PCA9538_T *Dev, uint8_t Pin, uint8_t Pol);

int32_t PCA9538_GetBankIn(PCA9538_T *Dev);
int32_t PCA9538_GetBankDir(PCA9538_T *Dev);
int32_t PCA9538_SetBankDir(PCA9538_T *Dev, uint8_t Dir);
int32_t PCA9538_GetBankOut(PCA9538_T *Dev);
int32_t PCA9538_SetBankOut(PCA9538_T *Dev, uint8_t Out);
int32_t PCA9538_GetBankPol(PCA9538_T *Dev);
int32_t PCA9538_SetBankPol(PCA9538_T *Dev, uint8_t Out);

int32_t PCA9538_Init(PCA9538_T *Dev);
void PCA9538_Deinit(PCA9538_T *Dev);

#endif /* __PCA9538_H */
