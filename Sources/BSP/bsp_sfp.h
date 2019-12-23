#ifndef __BSP_SFP_H
#define __BSP_SFP_H

#include "sfp.h"

#define BSP_SFP_CNT	20

#define BSP_SFP_1_I2C	I2C2
#define BSP_SFP_2_I2C	I2C2
#define BSP_SFP_3_I2C	I2C2
#define BSP_SFP_4_I2C	I2C2
#define BSP_SFP_5_I2C	I2C2
#define BSP_SFP_6_I2C	I2C2
#define BSP_SFP_7_I2C	I2C2
#define BSP_SFP_8_I2C	I2C2
#define BSP_SFP_9_I2C	I2C2
#define BSP_SFP_10_I2C	I2C2
#define BSP_SFP_11_I2C	I2C2
#define BSP_SFP_12_I2C	I2C2
#define BSP_SFP_13_I2C	I2C2
#define BSP_SFP_14_I2C	I2C2
#define BSP_SFP_15_I2C	I2C2
#define BSP_SFP_16_I2C	I2C2
#define BSP_SFP_17_I2C	I2C2
#define BSP_SFP_18_I2C	I2C2
#define BSP_SFP_19_I2C	I2C2
#define BSP_SFP_20_I2C	I2C2

#ifndef BSP_CONFIG_USE_I2C2
	#error	Define BSP_CONFIG_USE_I2C2 in bsp_config.h for BSP_SFP
#endif

#define BSP_SFP_LINE_IDX(Idx) 	(Idx + 16)
#define BSP_SFP_CLIENT_IDX(Idx) (Idx)
	
extern SFP_T sfp[BSP_SFP_CNT];
extern const BSP_PIN_CTRL_T bsp_sfp_pin_ctrl[BSP_SFP_CNT];
extern const BSP_I2C_CTRL_T bsp_sfp_i2c_ctrl[BSP_SFP_CNT];
extern const char* bsp_sfp_id[BSP_SFP_CNT];

void BSP_SFP_Process(uint32_t Idx);


#endif /* __BSP_SFP_H */
