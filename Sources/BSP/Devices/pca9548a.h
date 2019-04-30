#ifndef __PCA9548A_H
#define __PCA9548A_H

#include "i2c.h"
#include "bsp_dev.h"

typedef enum {
	PCA9548A_CH_NO = 0,
	PCA9548A_CH_0 = (1 << 0),
	PCA9548A_CH_1 = (1 << 1),
	PCA9548A_CH_2 = (1 << 2),
	PCA9548A_CH_3 = (1 << 3),
	PCA9548A_CH_4 = (1 << 4),
	PCA9548A_CH_5 = (1 << 5),
	PCA9548A_CH_6 = (1 << 6),
	PCA9548A_CH_7 = (1 << 7),
}PCA9548A_CH_T;

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	uint8_t Addr;
	PCA9548A_CH_T Channel;
} PCA9548A_T;

int32_t PCA9548A_SelectChannel(PCA9548A_T *dev, PCA9548A_CH_T Channel);
int32_t PCA9548A_Init(PCA9548A_T *dev, I2C_TypeDef *I2C, uint8_t Addr);
void PCA9548A_Deinit(PCA9548A_T *Dev);

#endif /* __PCA9548A_H */
