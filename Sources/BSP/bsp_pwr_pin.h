#ifndef __BSP_PWR_PIN_H
#define __BSP_PWR_PIN_H

#include "pca9536.h"

#define BSP_PWR_PIN_PCA9536_I2C				I2C3

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_PWR_PIN
#endif


#define BSP_PWR_PIN_UCD_RST					(1 << 0)
#define BSP_PWR_PIN_UCD_CTL					(1 << 1)
#define BSP_PWR_PIN_12V_PWR					(1 << 2)
	
#define BSP_PWR_PIN_PCA9536_POL_DEFAULT		(0x0000) // All pins are not inverted
#define BSP_PWR_PIN_PCA9536_DIR_DEFAULT		(0x0000) // All pins are outputs
#define BSP_PWR_PIN_PCA9536_OUT_DEFAULT		( BSP_PWR_PIN_UCD_RST | BSP_PWR_PIN_UCD_CTL | BSP_PWR_PIN_12V_PWR )
	
typedef struct {
	PCA9536_T pca9536;
} BSP_PWR_PIN_T;

int32_t BSP_PWR_PIN_Init(BSP_PWR_PIN_T *Dev);
int32_t BSP_PWR_PIN_SetState(BSP_PWR_PIN_T *Dev, uint8_t PinMask, uint8_t State);
int32_t BSP_PWR_PIN_GetState(BSP_PWR_PIN_T *Dev, uint8_t PinMask, uint8_t *State);

#endif /* __BSP_PWR_PIN_H */
