#ifndef __BSP_PWR_PIN_H
#define __BSP_PWR_PIN_H

#include "pca9536.h"

#define BSP_PWR_PIN_PCA9536_ID				"D107"
#define BSP_PWR_PIN_PCA9536_I2C				I2C3

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_PWR_PIN
#endif
	
#define BSP_PWR_PIN_UCD_RST					(0)
#define BSP_PWR_PIN_UCD_CTL					(1)
#define BSP_PWR_PIN_12V_PWR					(2)

extern PCA9536_T pwr_pca9536;
	
void BSP_PWR_PIN_Process(void);
int32_t BSP_PWR_PIN_Init(uint32_t Pin);
int32_t BSP_PWR_PIN_SetState(uint32_t Pin, uint32_t State);
int32_t BSP_PWR_PIN_GetState(uint32_t Pin, uint32_t *State);

#endif /* __BSP_PWR_PIN_H */
