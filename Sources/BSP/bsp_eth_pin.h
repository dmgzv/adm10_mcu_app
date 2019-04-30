#ifndef __BSP_ETH_PIN_H
#define __BSP_ETH_PIN_H

#include "bsp_dev.h"
#include "stm32f4xx_hal.h"

#define BSP_ETH_PIN_EN_GPIO_PIN          	GPIO_PIN_14
#define BSP_ETH_PIN_EN_GPIO_PORT          GPIOB
#define BSP_ETH_PIN_EN_GPIO_MODE					GPIO_MODE_OUTPUT_OD
#define BSP_ETH_PIN_EN_GPIO_PULL					GPIO_NOPULL
#define BSP_ETH_PIN_EN_GPIO_SPEED					GPIO_SPEED_FAST
#define BSP_ETH_PIN_EN_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE() 

#define BSP_ETH_PIN_SW_GPIO_PIN          	GPIO_PIN_15
#define BSP_ETH_PIN_SW_GPIO_PORT          GPIOB
#define BSP_ETH_PIN_SW_GPIO_MODE					GPIO_MODE_OUTPUT_OD
#define BSP_ETH_PIN_SW_GPIO_PULL					GPIO_NOPULL
#define BSP_ETH_PIN_SW_GPIO_SPEED					GPIO_SPEED_FAST
#define BSP_ETH_PIN_SW_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()

#define BSP_ETH_PIN_SEL_GPIO_PIN          GPIO_PIN_2
#define BSP_ETH_PIN_SEL_GPIO_PORT         GPIOB
#define BSP_ETH_PIN_SEL_GPIO_MODE					GPIO_MODE_INPUT
#define BSP_ETH_PIN_SEL_GPIO_PULL					GPIO_NOPULL
#define BSP_ETH_PIN_SEL_GPIO_SPEED				GPIO_SPEED_FAST
#define BSP_ETH_PIN_SEL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()

#define BSP_ETH_PIN_EN 	(0)
#define BSP_ETH_PIN_SW 	(1)
#define BSP_ETH_PIN_SEL (2)

#define BSP_ETH_PIN_EN_MASK 	(1 << BSP_ETH_PIN_EN)
#define BSP_ETH_PIN_SW_MASK 	(1 << BSP_ETH_PIN_SW)
#define BSP_ETH_PIN_SEL_MASK 	(1 << BSP_ETH_PIN_SEL)

typedef struct {
	BSP_DEV;
	uint32_t State;
} BSP_ETH_PIN_T;

extern BSP_ETH_PIN_T bsp_eth_pin;

void BSP_ETH_PIN_Process(void);
int32_t BSP_ETH_PIN_SetState(uint32_t Pin, uint32_t State);
int32_t BSP_ETH_PIN_GetState(uint32_t Pin, uint32_t *State);

#endif /* __BSP_ETH_PIN_H */
