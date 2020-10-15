#ifndef __BSP_SFP_H
#define __BSP_SFP_H

#include "sfp.h"

#define BSP_SFP_CNT	20

#define BSP_SFP_1_ID	"CL1"
#define BSP_SFP_2_ID	"CL2"
#define BSP_SFP_3_ID	"CL3"
#define BSP_SFP_4_ID	"CL4"
#define BSP_SFP_5_ID	"CL5"
#define BSP_SFP_6_ID	"CL6"
#define BSP_SFP_7_ID	"CL7"
#define BSP_SFP_8_ID	"CL8"
#define BSP_SFP_9_ID	"CL9"
#define BSP_SFP_10_ID	"CL10"
#define BSP_SFP_11_ID	"CL11"
#define BSP_SFP_12_ID	"CL12"
#define BSP_SFP_13_ID	"CL13"
#define BSP_SFP_14_ID	"CL14"
#define BSP_SFP_15_ID	"CL15"
#define BSP_SFP_16_ID	"CL16"
#define BSP_SFP_17_ID	"LN1"
#define BSP_SFP_18_ID	"LN2"
#define BSP_SFP_19_ID	"LN3"
#define BSP_SFP_20_ID	"LN4"

#define BSP_SFP_I2C	I2C3
#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_SFP
#endif

#define BSP_SFP_LINE_IDX(Idx) 	(Idx + 16)
#define BSP_SFP_CLIENT_IDX(Idx) (Idx)
	
extern SFP_T sfp[BSP_SFP_CNT];
extern const BSP_PIN_CTRL_T bsp_sfp_pin_ctrl[BSP_SFP_CNT];
extern const BSP_I2C_CTRL_T bsp_sfp_i2c_ctrl[BSP_SFP_CNT];
extern const char* bsp_sfp_id[BSP_SFP_CNT];

void BSP_SFP_Process(uint32_t Idx);


#endif /* __BSP_SFP_H */
