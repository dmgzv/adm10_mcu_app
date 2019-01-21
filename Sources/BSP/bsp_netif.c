/* Includes ------------------------------------------------------------------*/
#include "bsp_netif.h"
#include "netif/etharp.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Stack size of the interface thread */
#define BSP_NETIF_INPUT_THREAD_STACK_SIZE            ( 350 )

/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

static void BSP_NETIF_Input( void const * argument );
osThreadDef(Netif, BSP_NETIF_Input, osPriorityRealtime, 0, BSP_NETIF_INPUT_THREAD_STACK_SIZE);

static void BSP_NETIF_Input( void const * argument )
{
  struct pbuf *p;
  struct netif *netif = (struct netif *) argument;
  
  for( ;; )
  {
		p = ETH_Receive();
		if (p != NULL)
		{
			if (netif->input( p, netif) != ERR_OK )
			{
				pbuf_free(p);
			}
		}
  }
}

static err_t BSP_NETIF_Output(struct netif *netif, struct pbuf *p)
{
	return ETH_Transmit(p);
}

static err_t BSP_NETIF_Init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "ipg";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

  netif->output = etharp_output;
  netif->linkoutput = BSP_NETIF_Output;

	/* Set netif link flag */
	netif->flags |= NETIF_FLAG_LINK_UP;
  
  /* set netif MAC hardware address length */
  netif->hwaddr_len = ETH_HWADDR_LEN;
	
  /* set netif MAC hardware address */
	ETH_GetMacAddress(netif->hwaddr);

  /* set netif maximum transfer unit */
  netif->mtu = 1500;

  /* Accept broadcast address and ARP traffic */
  netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

  /* create the task that handles the ETH_MAC */
  osThreadCreate (osThread(Netif), netif);

  return ERR_OK;
}

int32_t BSP_ETH_Init(BSP_ETH_T *Eth, uint8_t Addr)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
	
	if (!Eth)
		return -1;
	
	Eth->mac[0] = MAC_ADDR0;
	Eth->mac[1] = MAC_ADDR1;
	Eth->mac[2] = MAC_ADDR2;
	Eth->mac[3] = MAC_ADDR3;
	Eth->mac[4] = MAC_ADDR4;
	Eth->mac[5] = MAC_ADDR5(Addr);
	
	if (ETH_Init(Eth->mac))
		return -1;
	
	/* Create tcp_ip stack thread */
  //lwip_init();
	tcpip_init(NULL, NULL);
	
  IP_ADDR4(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3(Addr));
  IP_ADDR4(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
  IP_ADDR4(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
    
  netif_add(&Eth->netif, &ipaddr, &netmask, &gw, NULL, &BSP_NETIF_Init, &ethernet_input);
	//netif_add(netif, &ipaddr, &netmask, &gw, NULL, &BSP_NETIF_Init, &tcpip_input);
  
  /*  Registers the default network interface. */
  netif_set_default(&Eth->netif);
  
  if (netif_is_link_up(&Eth->netif))
  {
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&Eth->netif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&Eth->netif);
  }	
	return 0;
}

u32_t sys_now(void)
{
  return HAL_GetTick();
}
