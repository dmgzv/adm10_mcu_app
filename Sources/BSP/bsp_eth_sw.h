#ifndef __BSP_ETH_SW_H
#define __BSP_ETH_SW_H

#include "ksz8895.h"

#define BSP_ETH_SW_ID		"D99"
#define BSP_ETH_SW_SPI	SPI5
#ifndef BSP_CONFIG_USE_SPI5
	#error	Define BSP_CONFIG_USE_SPI5 in bsp_config.h for BSP_ETH_SW
#endif

#define BSP_ETH_SW_SPI_CS_GPIO_PIN          	GPIO_PIN_6
#define BSP_ETH_SW_SPI_CS_GPIO_PORT           GPIOF
#define BSP_ETH_SW_SPI_CS_GPIO_MODE						GPIO_MODE_OUTPUT_PP
#define BSP_ETH_SW_SPI_CS_GPIO_PULL						GPIO_PULLUP
#define BSP_ETH_SW_SPI_CS_GPIO_SPEED					GPIO_SPEED_FAST
#define BSP_ETH_SW_SPI_CS_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOF_CLK_ENABLE() 

extern KSZ8895_T ksz8895;
	
void BSP_ETH_SW_Process(void);

#endif /* __BSP_ETH_SW_H */
