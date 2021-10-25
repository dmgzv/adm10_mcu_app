#ifndef __BSP_NETIF_H
#define __BSP_NETIF_H

#include "eth.h"
#include "lwip/netif.h"

/*Static IP ADDRESS*/
#define IP_ADDR0 	((uint8_t) 192)
#define IP_ADDR1 	((uint8_t) 168)
#define IP_ADDR2  ((uint8_t) 2)
#define IP_ADDR3  ((uint8_t) 11)
   
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
#define MAC_ADDR0   0x70U
#define MAC_ADDR1   0xB3U
#define MAC_ADDR2   0xD5U
#define MAC_ADDR3   0x6FU
#define MAC_ADDR4   0xE8U
#define MAC_ADDR5 	0x00U

extern uint8_t bsp_netif_mac[6];
extern ip_addr_t bsp_netif_ipaddr;
extern ip_addr_t bsp_netif_netmask;
extern ip_addr_t bsp_netif_gw;
extern unsigned short int bsp_netif_vlan_prio_vid;

int32_t BSP_ETH_Init(void);
void BSP_ETH_Process(void);
																						
#endif /* __BSP_NETIF_H */
