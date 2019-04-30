#ifndef __PCA9505_H
#define __PCA9505_H

#include "i2c.h"
#include "bsp_dev.h"

#define PCA9505_BANKS_CNT 5

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	uint8_t Addr;
	uint8_t Ins[PCA9505_BANKS_CNT];
	uint8_t Outs[PCA9505_BANKS_CNT];
	uint8_t Dirs[PCA9505_BANKS_CNT];
	uint8_t Pols[PCA9505_BANKS_CNT];
	uint8_t Msks[PCA9505_BANKS_CNT];
} PCA9505_T;

int32_t PCA9505_SetPinDir(PCA9505_T *dev, uint32_t Bank, uint8_t Pin, uint8_t Dir);
int32_t PCA9505_SetPinOut(PCA9505_T *dev, uint32_t Bank, uint8_t Pin, uint8_t Out);
int32_t PCA9505_SetPinPol(PCA9505_T *Dev, uint32_t Bank, uint8_t Pin, uint8_t Pol);
int32_t PCA9505_SetPinMsk(PCA9505_T *Dev, uint32_t Bank, uint8_t Pin, uint8_t Msk);

int32_t PCA9505_GetBankIn(PCA9505_T *dev, uint32_t Bank);
int32_t PCA9505_GetBankDir(PCA9505_T *dev, uint32_t Bank);
int32_t PCA9505_SetBankDir(PCA9505_T *dev, uint32_t Bank, uint8_t Dir);
int32_t PCA9505_GetBankOut(PCA9505_T *dev, uint32_t Bank);
int32_t PCA9505_SetBankOut(PCA9505_T *dev, uint32_t Bank, uint8_t Out);
int32_t PCA9505_GetBankPol(PCA9505_T *dev, uint32_t Bank);
int32_t PCA9505_SetBankPol(PCA9505_T *dev, uint32_t Bank, uint8_t Out);
int32_t PCA9505_GetBankMsk(PCA9505_T *dev, uint32_t Bank);
int32_t PCA9505_SetBankMsk(PCA9505_T *dev, uint32_t Bank, uint8_t Out);

int32_t PCA9505_Init(PCA9505_T *dev, I2C_TypeDef *I2C, uint8_t Addr);
void PCA9505_Deinit(PCA9505_T *Dev);

#endif /* __PCA9505_H */
