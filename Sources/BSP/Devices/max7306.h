#ifndef __MAX7306_H
#define __MAX7306_H

#include "bsp_dev.h"
#include "i2c.h"

#define MAX7306_DEV_NAME "MAX7306"

#define MAX7306_PINS_CNT		4
#define MAX7306_CONFIGS_CNT	2

typedef struct {
	BSP_DEV;
	const I2C_TypeDef *I2C;
	uint8_t Addr;
	uint8_t Pins[MAX7306_PINS_CNT];
	uint8_t Configs[MAX7306_CONFIGS_CNT];
} MAX7306_T;

int32_t MAX7306_WritePin(MAX7306_T *Dev, uint32_t Idx, uint8_t Pin);
int32_t MAX7306_ReadPin(MAX7306_T *Dev, uint32_t Idx);

int32_t MAX7306_Init(MAX7306_T *Dev);
void MAX7306_Deinit(MAX7306_T *Dev);

#endif /* __MAX7306_H */
