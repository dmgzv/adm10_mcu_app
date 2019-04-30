#ifndef __SI5332_H
#define __SI5332_H

#include "i2c.h"
#include "bsp_dev.h"

typedef struct {
	uint8_t Addr;
	uint8_t Data;
}SI5332_CFG_T;

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	uint8_t Addr;
} SI5332_T;

int32_t SI5332_Init(SI5332_T *Dev, I2C_TypeDef *I2C, uint8_t Addr, SI5332_CFG_T *Config, uint32_t ConfigSize);
void SI5332_Deinit(SI5332_T *Dev);

#endif /* __SI5332_H */
