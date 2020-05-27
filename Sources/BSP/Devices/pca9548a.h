#ifndef __PCA9548A_H
#define __PCA9548A_H

#include "i2c.h"
#include "bsp_dev.h"
#include "bsp_pin_ctrl.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#define PCA9548A_DEV_NAME "PCA9548A"

#define PCA9548A_PIN_RESET			0

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
	const I2C_TypeDef *I2C;
	const uint8_t Addr;
	const BSP_PIN_CTRL_T *PinCtrl;
	PCA9548A_CH_T Channel;
} PCA9548A_T;

/* 
	Device is locked when any channel is selected.
	Devices is unlocked when no channels are selected.
*/

int32_t PCA9548A_SelectChannel(PCA9548A_T *dev, PCA9548A_CH_T Channel);
int32_t PCA9548A_Init(PCA9548A_T *Dev);
void PCA9548A_Deinit(PCA9548A_T *Dev);

#endif /* __PCA9548A_H */
