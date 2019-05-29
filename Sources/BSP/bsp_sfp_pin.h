#ifndef __BSP_SFP_PIN_H
#define __BSP_SFP_PIN_H

#include "pcal6524.h"
#include "pca9536.h"
#include "bsp_sfp_i2c_switch.h"

#define BSP_SFP_PIN_PCAL6524_CNT	3

#define BSP_SFP_PIN_PCAL6524_1_I2C			I2C2
#define BSP_SFP_PIN_PCAL6524_1_I2C_ADDR	0x21

#define BSP_SFP_PIN_PCAL6524_2_I2C			I2C2
#define BSP_SFP_PIN_PCAL6524_2_I2C_ADDR	0x22

#define BSP_SFP_PIN_PCAL6524_3_I2C			I2C2
#define BSP_SFP_PIN_PCAL6524_3_I2C_ADDR	0x23

#define BSP_SFP_PIN_PCA9536_CNT	20
#define BSP_SFP_PIN_PCA9536_I2C	I2C2

#define BSP_SFP_PIN_LED_OFF			0
#define BSP_SFP_PIN_LED_RED			1
#define BSP_SFP_PIN_LED_GREEN		2
#define BSP_SFP_PIN_LED_YELLOW	3

#ifndef BSP_CONFIG_USE_I2C2
	#error	Define BSP_CONFIG_USE_I2C2 in bsp_config.h for BSP_SFP_PIN
#endif

extern PCAL6524_T pcal6524[BSP_SFP_PIN_PCAL6524_CNT];
extern PCA9536_T pca9536[BSP_SFP_PIN_PCA9536_CNT];	

void BSP_SFP_PIN_Process(void);
	
int32_t BSP_SFP_PIN_InitTxFault(uint32_t SfpNum);
int32_t BSP_SFP_PIN_GetTxFault(uint32_t SfpNum, uint32_t *TxFault);

int32_t BSP_SFP_PIN_InitTxDisable(uint32_t SfpNum);	
int32_t BSP_SFP_PIN_SetTxDisable(uint32_t SfpNum, uint32_t TxDisable);
int32_t BSP_SFP_PIN_GetTxDisable(uint32_t SfpNum, uint32_t *TxDisable);
	
int32_t BSP_SFP_PIN_InitLinkLed(uint32_t SfpNum);
int32_t BSP_SFP_PIN_SetLinkLed(uint32_t SfpNum, uint32_t Color);
int32_t BSP_SFP_PIN_GetLinkLed(uint32_t SfpNum, uint32_t *Color);
	
int32_t BSP_SFP_PIN_InitLed(uint32_t SfpNum);
int32_t BSP_SFP_PIN_SetLed(uint32_t SfpNum, uint32_t Color);
int32_t BSP_SFP_PIN_GetLed(uint32_t SfpNum, uint32_t *Color);
	
int32_t BSP_SFP_PIN_InitRateSel(uint32_t SfpNum);
int32_t BSP_SFP_PIN_SetRateSel(uint32_t SfpNum, uint32_t RateSel);
int32_t BSP_SFP_PIN_GetRateSel(uint32_t SfpNum, uint32_t *RateSel);
	
int32_t BSP_SFP_PIN_InitModAbs(uint32_t SfpNum);
int32_t BSP_SFP_PIN_GetModAbs(uint32_t SfpNum, uint32_t *ModAbs);

#endif /* __BSP_SFP_PIN_H */
