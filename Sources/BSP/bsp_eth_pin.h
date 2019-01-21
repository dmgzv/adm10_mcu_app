#ifndef __BSP_ETH_PIN_H
#define __BSP_ETH_PIN_H

#include "bsp_dev.h"
#include "stm32f4xx_hal.h"

#define BSP_ETH_PIN_EN_GPIO_PIN          	GPIO_PIN_14
#define BSP_ETH_PIN_EN_GPIO_PORT          GPIOB
#define BSP_ETH_PIN_EN_GPIO_MODE					GPIO_MODE_OUTPUT_PP
#define BSP_ETH_PIN_EN_GPIO_PULL					GPIO_PULLUP
#define BSP_ETH_PIN_EN_GPIO_SPEED					GPIO_SPEED_FAST
#define BSP_ETH_PIN_EN_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE() 

#define BSP_ETH_PIN_SW_GPIO_PIN          	GPIO_PIN_15
#define BSP_ETH_PIN_SW_GPIO_PORT          GPIOB
#define BSP_ETH_PIN_SW_GPIO_MODE					GPIO_MODE_OUTPUT_PP
#define BSP_ETH_PIN_SW_GPIO_PULL					GPIO_PULLUP
#define BSP_ETH_PIN_SW_GPIO_SPEED					GPIO_SPEED_FAST
#define BSP_ETH_PIN_SW_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()

#define BSP_ETH_PIN_EN_MASK (1 << 0)
#define BSP_ETH_PIN_SW_MASK (1 << 1)

typedef struct {
	BSP_DEV;
	uint32_t State;
} BSP_ETH_PIN_T;

int32_t BSP_ETH_PIN_Init(BSP_ETH_PIN_T *Dev);
int32_t BSP_ETH_PIN_SetState(BSP_ETH_PIN_T *Dev, uint32_t Mask, uint32_t State);
int32_t BSP_ETH_PIN_GetState(BSP_ETH_PIN_T *Dev, uint32_t Mask, uint32_t *State);

#endif /* __BSP_ETH_PIN_H */
