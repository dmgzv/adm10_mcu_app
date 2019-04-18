#ifndef __PCAL6524_H
#define __PCAL6524_H

#include "bsp_dev.h"
#include "i2c.h"

#define PCAL6524_BANKS_CNT	3

#define PCAL6524_PIN_CONFIG_OUT	0
#define PCAL6524_PIN_CONFIG_IN	1

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	uint8_t Addr;
	uint8_t Ins[PCAL6524_BANKS_CNT];
	uint8_t Outs[PCAL6524_BANKS_CNT];
	uint8_t Cfgs[PCAL6524_BANKS_CNT];
	uint8_t Pols[PCAL6524_BANKS_CNT];
} PCAL6524_T;

int32_t PCAL6524_SetPinCfg(PCAL6524_T *dev, uint8_t Bank, uint8_t Pin, uint8_t Cfg);
int32_t PCAL6524_SetPinOut(PCAL6524_T *dev, uint8_t Bank, uint8_t Pin, uint8_t Out);
int32_t PCAL6524_SetPinPol(PCAL6524_T *Dev, uint8_t Bank, uint8_t Pin, uint8_t Pol);

int32_t PCAL6524_GetBankIn(PCAL6524_T *dev, uint8_t Bank);
int32_t PCAL6524_GetBankCfg(PCAL6524_T *dev, uint8_t Bank);
int32_t PCAL6524_SetBankCfg(PCAL6524_T *dev, uint8_t Bank, uint8_t Cfg);
int32_t PCAL6524_GetBankOut(PCAL6524_T *dev, uint8_t Bank);
int32_t PCAL6524_SetBankOut(PCAL6524_T *dev, uint8_t Bank, uint8_t Out);
int32_t PCAL6524_GetBankPol(PCAL6524_T *dev, uint8_t Bank);
int32_t PCAL6524_SetBankPol(PCAL6524_T *dev, uint8_t Bank, uint8_t Out);

int32_t PCAL6524_Init(PCAL6524_T *dev, I2C_TypeDef *I2C, uint8_t Addr);

#endif /* __PCAL6524_H */
