#ifndef __BSP_SFP_I2C_SWITCH_H
#define __BSP_SFP_I2C_SWITCH_H

#include "pca9548a.h"

#define	BSP_PCA9548_CNT	3

#define BSP_PCA9548_1_ID				"D72"
#define BSP_PCA9548_1_I2C				I2C2
#define BSP_PCA9548_1_I2C_ADDR	0x70

#define BSP_PCA9548_2_ID				"D73"
#define BSP_PCA9548_2_I2C				I2C2
#define BSP_PCA9548_2_I2C_ADDR	0x72

#define BSP_PCA9548_3_ID				"D74"
#define BSP_PCA9548_3_I2C				I2C2
#define BSP_PCA9548_3_I2C_ADDR	0x74

#ifndef BSP_CONFIG_USE_I2C2
	#error	Define BSP_CONFIG_USE_I2C2 in bsp_config.h for BSP_SFP_I2C_SWITCH
#endif
	
#define BSP_SFP_I2C_SWITCH_RESET_GPIO_PIN          		GPIO_PIN_4
#define BSP_SFP_I2C_SWITCH_RESET_GPIO_PORT            GPIOA
#define BSP_SFP_I2C_SWITCH_RESET_GPIO_MODE						GPIO_MODE_OUTPUT_PP
#define BSP_SFP_I2C_SWITCH_RESET_GPIO_PULL						GPIO_NOPULL
#define BSP_SFP_I2C_SWITCH_RESET_GPIO_SPEED						GPIO_SPEED_FAST
#define BSP_SFP_I2C_SWITCH_RESET_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE() 

#define BSP_SFP_I2C_SWITCH_SFP_CH(SfpIdx) 			((SfpIdx) < 16 ? (SfpIdx) : ((SfpIdx) + 4))
#define BSP_SFP_I2C_SWITCH_GDT_CH(GdtIdx) 			(17 - (GdtIdx))
#define BSP_SFP_I2C_SWITCH_SI5332_CH(Si5332Idx) ((Si5332Idx) + 18)
#define BSP_SFP_I2C_SWITCH_CH_NO	0xFFFFFFFF

extern const char *pca9548a_id[BSP_PCA9548_CNT];
extern const I2C_TypeDef *pca9548a_i2c[BSP_PCA9548_CNT];
extern const uint8_t pca9548a_addr[BSP_PCA9548_CNT];
extern PCA9548A_T pca9548a[BSP_PCA9548_CNT];

int32_t BSP_SFP_I2C_SWITCH_SelectChannel(uint32_t Channel);	
void BSP_SFP_I2C_SWITCH_Process(void);

#endif /* __BSP_SFP_I2C_SWITCH_H */
