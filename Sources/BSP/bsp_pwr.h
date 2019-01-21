#ifndef __BSP_PWR_H
#define __BSP_PWR_H

#include "ucd90120a.h"
#include "gdt080a.h"
#include "dim3r3400.h"
#include "bsp_pwr_pin.h"

#define BSP_PWR_UCD90120A_I2C				I2C3
#define BSP_PWR_UCD90120A_I2C_ADDR	0x7E

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_PWR
#endif

#define BSP_PWR_AVS_RSTB_GPIO_PIN          		GPIO_PIN_0
#define BSP_PWR_AVS_RSTB_GPIO_PORT            GPIOD
#define BSP_PWR_AVS_RSTB_GPIO_MODE						GPIO_MODE_OUTPUT_PP
#define BSP_PWR_AVS_RSTB_GPIO_PULL						GPIO_PULLUP
#define BSP_PWR_AVS_RSTB_GPIO_SPEED						GPIO_SPEED_FAST
#define BSP_PWR_AVS_RSTB_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE() 

#define BSP_PWR_AVS_ENB_GPIO_PIN          		GPIO_PIN_1
#define BSP_PWR_AVS_ENB_GPIO_PORT             GPIOD
#define BSP_PWR_AVS_ENB_GPIO_MODE							GPIO_MODE_INPUT
#define BSP_PWR_AVS_ENB_GPIO_PULL							GPIO_NOPULL
#define BSP_PWR_AVS_ENB_GPIO_SPEED						GPIO_SPEED_FAST
#define BSP_PWR_AVS_ENB_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOD_CLK_ENABLE() 

#define BSP_PWR_GDT080A_0V9		0
#define BSP_PWR_GDT080A_0V95	1
#define BSP_PWR_GDT080A_CNT		2
	
#define BSP_PWR_GDT080A_0V9_I2C				I2C3
#define BSP_PWR_GDT080A_0V9_I2C_ADDR	42
#define BSP_PWR_GDT080A_0V9_RTIM			4.0f /* 4 kOhm */

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_PWR
#endif

#define BSP_PWR_GDT080A_0V95_I2C				I2C3
#define BSP_PWR_GDT080A_0V95_I2C_ADDR	34
#define BSP_PWR_GDT080A_0V95_RTIM			3.4f /* 3.4 kOhm */
	
#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_PWR
#endif
	
#define BSP_PWR_DIM3R3400_I2C				I2C3
#define BSP_PWR_DIM3R3400_I2C_ADDR	0x2B

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_PWR
#endif
	
typedef struct {
	UCD90120A_T ucd90120a;
	GDT080A_T gdt080a[BSP_PWR_GDT080A_CNT];
	DIM3R3400_T dim3r3400;
	BSP_PWR_PIN_T *Pin;
} BSP_PWR_T;

int32_t BSP_PWR_SetDigiVout(BSP_PWR_T *Dev, float Vout);
int32_t BSP_PWR_Init(BSP_PWR_T *Dev, BSP_PWR_PIN_T *Pin);
int32_t BSP_PWR_Update(BSP_PWR_T *Dev);
#define BSP_PWR_UCD90120A_Update(Dev) 		UCD90120A_Update(&(Dev)->ucd90120a)
#define BSP_PWR_GDT080A_Update(Dev, Idx) 	GDT080A_Update(&(Dev)->gdt080a[(Idx)])


#endif /* __BSP_PWR_H */
