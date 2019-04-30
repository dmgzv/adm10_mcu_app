#ifndef __PCA9536_H
#define __PCA9536_H

#include "bsp_dev.h"
#include "i2c.h"

#define PCA9536_I2C_ADDR (0x41 << 1)

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	uint8_t Ins;
	uint8_t Outs;
	uint8_t Dirs;
	uint8_t Pols;
} PCA9536_T;

int32_t PCA9536_SetPinDir(PCA9536_T *Dev, uint8_t Pin, uint8_t Dir);
int32_t PCA9536_SetPinOut(PCA9536_T *Dev, uint8_t Pin, uint8_t Out);
int32_t PCA9536_SetPinPol(PCA9536_T *Dev, uint8_t Pin, uint8_t Pol);

int32_t PCA9536_GetBankIn(PCA9536_T *Dev);
int32_t PCA9536_GetBankDir(PCA9536_T *Dev);
int32_t PCA9536_SetBankDir(PCA9536_T *Dev, uint8_t Dir);
int32_t PCA9536_GetBankOut(PCA9536_T *Dev);
int32_t PCA9536_SetBankOut(PCA9536_T *Dev, uint8_t Out);
int32_t PCA9536_GetBankPol(PCA9536_T *Dev);
int32_t PCA9536_SetBankPol(PCA9536_T *Dev, uint8_t Out);

int32_t PCA9536_Init(PCA9536_T *Dev, I2C_TypeDef *I2C);
void PCA9536_Deinit(PCA9536_T *Dev);

#endif /* __PCA9536_H */
