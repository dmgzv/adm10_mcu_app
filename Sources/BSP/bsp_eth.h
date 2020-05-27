#ifndef __BSP_ETH_H
#define __BSP_ETH_H

#include "eth.h"
#include "lwip/netif.h"

/*Static IP ADDRESS*/
#define IP_ADDR0 				((uint8_t) 192)
#define IP_ADDR1 				((uint8_t) 168)
#define IP_ADDR2  			((uint8_t) 2)
#define IP_ADDR3(Addr)  ((uint8_t) (11 + (((Addr) - 1) * 10)))
   
/*NETMASK*/
#define NETMASK_ADDR0   ((uint8_t) 255)
#define NETMASK_ADDR1   ((uint8_t) 255)
#define NETMASK_ADDR2   ((uint8_t) 255)
#define NETMASK_ADDR3   ((uint8_t) 0)

/*Gateway Address*/
#define GW_ADDR0   ((uint8_t) 192)
#define GW_ADDR1   ((uint8_t) 168)
#define GW_ADDR2   ((uint8_t) 2)
#define GW_ADDR3   ((uint8_t) 1)

/* MAC ADDRESS: MAC_ADDR0:MAC_ADDR1:MAC_ADDR2:MAC_ADDR3:MAC_ADDR4:MAC_ADDR5 */
#define MAC_ADDR0   		2U
#define MAC_ADDR1   		0U
#define MAC_ADDR2   		0U
#define MAC_ADDR3   		0U
#define MAC_ADDR4   		0U
#define MAC_ADDR5(Addr) (0x0B + (((Addr) - 1) * 10))

int32_t BSP_ETH_Init(void);
void BSP_ETH_Process(uint8_t Addr);
																						
#endif /* __BSP_ETH_H */
