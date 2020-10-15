#ifndef __BSP_SFP_PIN_H
#define __BSP_SFP_PIN_H

#include "pcal6524.h"
#include "pca9536.h"
#include "bsp_sfp_i2c_switch.h"

#define BSP_SFP_PIN_PCAL6524_CNT	3

#define BSP_SFP_PIN_PCAL6524_1_ID				"D67"
#define BSP_SFP_PIN_PCAL6524_1_I2C			I2C3
#define BSP_SFP_PIN_PCAL6524_1_I2C_ADDR	(0x21 << 1)

#define BSP_SFP_PIN_PCAL6524_2_ID				"D58"
#define BSP_SFP_PIN_PCAL6524_2_I2C			I2C3
#define BSP_SFP_PIN_PCAL6524_2_I2C_ADDR	(0x22 << 1)

#define BSP_SFP_PIN_PCAL6524_3_ID				"D57"
#define BSP_SFP_PIN_PCAL6524_3_I2C			I2C3
#define BSP_SFP_PIN_PCAL6524_3_I2C_ADDR	(0x23 << 1)

#define BSP_SFP_PIN_PCA9536_CNT	20

#define BSP_SFP_PIN_PCA9536_1_ID "D1"
#define BSP_SFP_PIN_PCA9536_2_ID "D2"
#define BSP_SFP_PIN_PCA9536_3_ID "D3"
#define BSP_SFP_PIN_PCA9536_4_ID "D4"
#define BSP_SFP_PIN_PCA9536_5_ID "D5"
#define BSP_SFP_PIN_PCA9536_6_ID "D6"
#define BSP_SFP_PIN_PCA9536_7_ID "D7"
#define BSP_SFP_PIN_PCA9536_8_ID "D8"
#define BSP_SFP_PIN_PCA9536_9_ID "D9"
#define BSP_SFP_PIN_PCA9536_10_ID "D10"
#define BSP_SFP_PIN_PCA9536_11_ID "D11"
#define BSP_SFP_PIN_PCA9536_12_ID "D12"
#define BSP_SFP_PIN_PCA9536_13_ID "D13"
#define BSP_SFP_PIN_PCA9536_14_ID "D14"
#define BSP_SFP_PIN_PCA9536_15_ID "D15"
#define BSP_SFP_PIN_PCA9536_16_ID "D16"
#define BSP_SFP_PIN_PCA9536_17_ID "D17"
#define BSP_SFP_PIN_PCA9536_18_ID "D18"
#define BSP_SFP_PIN_PCA9536_19_ID "D19"
#define BSP_SFP_PIN_PCA9536_20_ID "D20"

#define BSP_SFP_PIN_PCA9536_I2C	I2C3

#define BSP_SFP_PIN_LED_OFF			0
#define BSP_SFP_PIN_LED_RED			1
#define BSP_SFP_PIN_LED_GREEN		2
#define BSP_SFP_PIN_LED_YELLOW	3

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_SFP_PIN
#endif

extern PCAL6524_T pcal6524[BSP_SFP_PIN_PCAL6524_CNT];
extern PCA9536_T pca9536[BSP_SFP_PIN_PCA9536_CNT];	

void BSP_SFP_PIN_Process(void);
	
int32_t BSP_SFP_PIN_InitTxFault(uint32_t SfpNum);
int32_t BSP_SFP_PIN_GetTxFault(uint32_t SfpNum, uint32_t *TxFault);

int32_t BSP_SFP_PIN_InitTxDisable(uint32_t SfpNum);	
int32_t BSP_SFP_PIN_SetTxDisable(uint32_t SfpNum, uint32_t TxDisable);
int32_t BSP_SFP_PIN_GetTxDisable(uint32_t SfpNum, uint32_t *TxDisable);
	
int32_t BSP_SFP_PIN_InitTxEnableLed(uint32_t SfpNum);
int32_t BSP_SFP_PIN_SetTxEnableLed(uint32_t SfpNum, uint32_t Color);
int32_t BSP_SFP_PIN_GetTxEnableLed(uint32_t SfpNum, uint32_t *Color);
	
int32_t BSP_SFP_PIN_InitAlarmLed(uint32_t SfpNum);
int32_t BSP_SFP_PIN_SetAlarmLed(uint32_t SfpNum, uint32_t Color);
int32_t BSP_SFP_PIN_GetAlarmLed(uint32_t SfpNum, uint32_t *Color);
	
int32_t BSP_SFP_PIN_InitRateSel(uint32_t SfpNum);
int32_t BSP_SFP_PIN_SetRateSel(uint32_t SfpNum, uint32_t RateSel);
int32_t BSP_SFP_PIN_GetRateSel(uint32_t SfpNum, uint32_t *RateSel);
	
int32_t BSP_SFP_PIN_InitModAbs(uint32_t SfpNum);
int32_t BSP_SFP_PIN_GetModAbs(uint32_t SfpNum, uint32_t *ModAbs);

#endif /* __BSP_SFP_PIN_H */
