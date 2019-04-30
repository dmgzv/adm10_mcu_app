#ifndef __MAX16046_H
#define __MAX16046_H

#include "i2c.h"

typedef struct
{
	I2C_TypeDef *I2C;
	uint8_t Addr;
} MAX16046_T;

int32_t MAX16046_Init(MAX16046_T *dev, I2C_TypeDef *I2C, uint8_t Addr);

#endif /* __MAX16046_H */
