#ifndef __SI5332_H
#define __SI5332_H

#include "i2c.h"
#include "bsp_dev.h"

#define SI5332_DEV_NAME "SI5332"

#define SI5332_STATUS_READY		0x01
#define SI5332_STATUS_ACTIVE	0x02

typedef struct {
	uint8_t Addr;
	uint8_t Data;
}SI5332_CFG_T;

typedef struct {
	BSP_DEV;
	const I2C_TypeDef *I2C;
	const uint8_t Addr;
	uint8_t Status;
} SI5332_T;

int32_t SI5332_Init(SI5332_T *Dev, const SI5332_CFG_T *Config, const uint32_t ConfigSize);
int32_t SI5332_Update(SI5332_T *Dev);
void SI5332_Deinit(SI5332_T *Dev);

#endif /* __SI5332_H */
