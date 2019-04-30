#ifndef __MAX6639_H
#define __MAX6639_H

#include "i2c.h"
#include "bsp_dev.h"

typedef enum {
	MAX6639_CH_1 = 0,
	MAX6639_CH_2 = 1
}MAX6639_CH_T;

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	uint8_t Addr;
	uint8_t DevId;
	uint8_t ManufId;
	uint8_t Temp[2];
} MAX6639_T;

int32_t MAX6639_ReadChTemp(MAX6639_T *dev, MAX6639_CH_T Channel);

int32_t MAX6639_Init(MAX6639_T *dev, I2C_TypeDef *I2C, uint8_t Addr);
void MAX6639_Deinit(MAX6639_T *Dev);

#endif /* __MAX6639_H */
