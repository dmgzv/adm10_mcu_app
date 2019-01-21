#ifndef __BSP_NETIF_H
#define __BSP_NETIF_H

#include "eth.h"
#include "lwip/netif.h"

/*Static IP ADDRESS*/
#define IP_ADDR0 				((uint8_t) 192)
#define IP_ADDR1 				((uint8_t) 168)
#define IP_ADDR2  			((uint8_t) 0)
#define IP_ADDR3(Addr)  ((uint8_t) 128 + Addr)
   
/*NETMASK*/
#define NETMASK_ADDR0   ((uint8_t) 255)
#define NETMASK_ADDR1   ((uint8_t) 255)
#define NETMASK_ADDR2   ((uint8_t) 255)
#define NETMASK_ADDR3   ((uint8_t) 0)

/*Gateway Address*/
#define GW_ADDR0   ((uint8_t) 192)
#define GW_ADDR1   ((uint8_t) 168)
#define GW_ADDR2   ((uint8_t) 0)
#define GW_ADDR3   ((uint8_t) 1)

/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0   		2U
#define MAC_ADDR1   		0U
#define MAC_ADDR2   		0U
#define MAC_ADDR3   		0U
#define MAC_ADDR4   		0U
#define MAC_ADDR5(Addr) (Addr)

#define BSP_NETIF_ENABLE_GPIO_PIN          	GPIO_PIN_15
#define BSP_NETIF_ENABLE_GPIO_PORT          GPIOB
#define BSP_NETIF_ENABLE_GPIO_MODE					GPIO_MODE_OUTPUT_PP
#define BSP_NETIF_ENABLE_GPIO_PULL					GPIO_PULLUP
#define BSP_NETIF_ENABLE_GPIO_SPEED					GPIO_SPEED_FAST
#define BSP_NETIF_ENABLE_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()   

#define BSP_NETIF_SELECT_GPIO_PIN           GPIO_PIN_14
#define BSP_NETIF_SELECT_GPIO_PORT          GPIOB
#define BSP_NETIF_SELECT_GPIO_MODE					GPIO_MODE_OUTPUT_PP
#define BSP_NETIF_SELECT_GPIO_PULL					GPIO_PULLUP
#define BSP_NETIF_SELECT_GPIO_SPEED					GPIO_SPEED_FAST
#define BSP_NETIF_SELECT_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOD_CLK_ENABLE()  

typedef struct {
	struct netif netif;
	uint8_t mac[6];
} BSP_ETH_T;

int32_t BSP_ETH_Init(BSP_ETH_T *Eth, uint8_t Addr);
																						
#endif /* __BSP_NETIF_H */
