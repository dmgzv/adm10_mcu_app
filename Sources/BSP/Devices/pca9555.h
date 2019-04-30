#ifndef __PCA9555_H
#define __PCA9555_H

#include "bsp_dev.h"
#include "i2c.h"

#define PCA9555_BANKS_CNT	2
#define PCA9555_BANK_PINS_CNT	8

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	uint8_t Addr;
	uint8_t Ins[PCA9555_BANKS_CNT];
	uint8_t Outs[PCA9555_BANKS_CNT];
	uint8_t Dirs[PCA9555_BANKS_CNT];
	uint8_t Pols[PCA9555_BANKS_CNT];
} PCA9555_T;

int32_t PCA9555_SetPinDir(PCA9555_T *dev, uint32_t Bank, uint8_t Pin, uint8_t Dir);
int32_t PCA9555_SetPinOut(PCA9555_T *dev, uint32_t Bank, uint8_t Pin, uint8_t Out);
int32_t PCA9555_SetPinPol(PCA9555_T *Dev, uint32_t Bank, uint8_t Pin, uint8_t Pol);

int32_t PCA9555_GetBankIn(PCA9555_T *dev, uint32_t Bank);
int32_t PCA9555_GetBankDir(PCA9555_T *dev, uint32_t Bank);
int32_t PCA9555_SetBankDir(PCA9555_T *dev, uint32_t Bank, uint8_t Dir);
int32_t PCA9555_GetBankOut(PCA9555_T *dev, uint32_t Bank);
int32_t PCA9555_SetBankOut(PCA9555_T *dev, uint32_t Bank, uint8_t Out);
int32_t PCA9555_GetBankPol(PCA9555_T *dev, uint32_t Bank);
int32_t PCA9555_SetBankPol(PCA9555_T *dev, uint32_t Bank, uint8_t Out);

int32_t PCA9555_Init(PCA9555_T *dev, I2C_TypeDef *I2C, uint8_t Addr);
void PCA9555_Deinit(PCA9555_T *Dev);

#endif /* __PCA9555_H */
