#ifndef __DIM3R3400_H
#define __DIM3R3400_H

#include "i2c.h"
#include "bsp_dev.h"

#define DIM3R3400_DEV_NAME	"DIM3R400"

#define DIM3R3400_STATUS_MASK_ENABLE_A	(1 << 0)
#define DIM3R3400_STATUS_MASK_ENABLE_B	(1 << 1)
#define DIM3R3400_STATUS_MASK_ALARM			(1 << 2)
#define DIM3R3400_STATUS_MASK_HOLDUP		(1 << 4)
#define DIM3R3400_STATUS_MASK_HOTSWAP		(1 << 5)
#define DIM3R3400_STATUS_MASK_VOUT_LOW	(1 << 6)

#define DIM3R3400_IN_CNT	2

typedef struct {
	BSP_DEV;
	const I2C_TypeDef *I2C;
	const uint8_t Addr;
	uint8_t Status;
	float Vin[DIM3R3400_IN_CNT];
	float Temp;
	float Vout;
	float Iout;
}DIM3R3400_T;

int32_t DIM3R3400_Init(DIM3R3400_T *Dev);
void DIM3R3400_Deinit(DIM3R3400_T *Dev);
int32_t DIM3R3400_Update(DIM3R3400_T *Dev);

#endif /* __DIM3R3400_H */
