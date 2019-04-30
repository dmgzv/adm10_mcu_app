#ifndef __PCA9546A_H
#define __PCA9546A_H

#include "i2c.h"
#include "bsp_dev.h"

typedef enum {
	PCA9546A_CH_NO = 0,
	PCA9546A_CH_0 = (1 << 0),
	PCA9546A_CH_1 = (1 << 1),
	PCA9546A_CH_2 = (1 << 2),
	PCA9546A_CH_3 = (1 << 3),
}PCA9546A_CH_T;

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	uint8_t Addr;
	PCA9546A_CH_T Channel;
} PCA9546A_T;

/* 
	Device is locked when any channel is selected.
	Devices is unlocked when no channels are selected.
*/

int32_t PCA9546A_SelectChannel(PCA9546A_T *dev, PCA9546A_CH_T Channel);
int32_t PCA9546A_Init(PCA9546A_T *dev, I2C_TypeDef *I2C, uint8_t Addr);
void PCA9546A_Deinit(PCA9546A_T *dev);

#endif /* __PCA9546A_H */
