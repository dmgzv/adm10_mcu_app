#ifndef __BSP_SFP_PIN_H
#define __BSP_SFP_PIN_H

#include "pcal6524.h"
#include "pca9536.h"
#include "bsp_sfp_i2c_switch.h"

#define BSP_SFP_PIN_PCAL6524_CNT	3

#define BSP_SFP_PIN_PCAL6524_1_I2C			I2C3
#define BSP_SFP_PIN_PCAL6524_1_I2C_ADDR	0x21

#define BSP_SFP_PIN_PCAL6524_2_I2C			I2C3
#define BSP_SFP_PIN_PCAL6524_2_I2C_ADDR	0x22

#define BSP_SFP_PIN_PCAL6524_3_I2C			I2C3
#define BSP_SFP_PIN_PCAL6524_3_I2C_ADDR	0x23

#define BSP_SFP_PIN_PCA9536_CNT	20
#define BSP_SFP_PIN_PCA9536_I2C	I2C3

#define BSP_SFP_PIN_LED_OFF			0
#define BSP_SFP_PIN_LED_RED			1
#define BSP_SFP_PIN_LED_GREEN		2
#define BSP_SFP_PIN_LED_YELLOW	3

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_SFP_PIN
#endif

typedef struct {
	PCAL6524_T pcal6524[BSP_SFP_PIN_PCAL6524_CNT];
	BSP_SFP_I2C_SWITCH_T *I2CSwitch;
	PCA9536_T pca9536[BSP_SFP_PIN_PCA9536_CNT];	
}BSP_SFP_PIN_T;

int32_t BSP_SFP_PIN_Init(BSP_SFP_PIN_T *Dev, BSP_SFP_I2C_SWITCH_T *I2CSwitch);
int32_t BSP_SFP_PIN_SetTxDisable(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t TxDisable);
int32_t BSP_SFP_PIN_GetTxDisable(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *TxDisable);
int32_t BSP_SFP_PIN_SetLinkLed(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t Color);
int32_t BSP_SFP_PIN_GetLinkLed(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *Color);
int32_t BSP_SFP_PIN_SetLed(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t Color);
int32_t BSP_SFP_PIN_GetLed(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *Color);
int32_t BSP_SFP_PIN_SetRateSel(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t RateSel);
int32_t BSP_SFP_PIN_GetRateSel(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *RateSel);
int32_t BSP_SFP_PIN_GetModAbs(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *ModAbs);
int32_t BSP_SFP_PIN_GetTxFault(BSP_SFP_PIN_T *Dev, uint32_t SfpNum, uint32_t *TxFault);

#endif /* __BSP_SFP_PIN_H */
