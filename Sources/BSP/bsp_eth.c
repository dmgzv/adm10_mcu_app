/* Includes ------------------------------------------------------------------*/
#include "bsp_eth.h"
#include "bsp_eth_sw.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
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

static void __BSP_NETIF_Input( void const * argument );
osThreadDef(netif_input, __BSP_NETIF_Input, osPriorityRealtime, 0, BSP_NETIF_INPUT_THREAD_STACK_SIZE);
osThreadId netif_input_thread_id;

struct netif bsp_netif;
uint8_t tcpip_init_done;

static void __BSP_NETIF_Input( void const * argument )
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

static err_t __BSP_NETIF_Output(struct netif *netif, struct pbuf *p)
{
	return ETH_Transmit(p);
}

static err_t __BSP_NETIF_Init(struct netif *netif)
{
  LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
  /* Initialize interface hostname */
  netif->hostname = "ipg";
#endif /* LWIP_NETIF_HOSTNAME */

  netif->name[0] = IFNAME0;
  netif->name[1] = IFNAME1;

  netif->output = etharp_output;
  netif->linkoutput = __BSP_NETIF_Output;

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
	if (!netif_input_thread_id)
		netif_input_thread_id = osThreadCreate (osThread(netif_input), netif);
	
  return ERR_OK;
}

static void tcpip_init_done_callback(void *arg)
{
	tcpip_init_done = 1;
}

int32_t BSP_ETH_Init(void)
{
	if (!tcpip_init_done)
		tcpip_init(tcpip_init_done_callback, NULL);
	return 0;
}

u32_t sys_now(void)
{
  return HAL_GetTick();
}

static int32_t BSP_NETIF_Init(struct netif *netif, uint8_t Addr)
{
	ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;	
	uint8_t mac[6];
	
	mac[0] = MAC_ADDR0;
	mac[1] = MAC_ADDR1;
	mac[2] = MAC_ADDR2;
	mac[3] = MAC_ADDR3;
	mac[4] = MAC_ADDR4;
	mac[5] = MAC_ADDR5(Addr);
	
	if (ETH_Init(mac))
		return -1;	
	if (!tcpip_init_done)
		tcpip_init(tcpip_init_done_callback, NULL);
	
  IP_ADDR4(&ipaddr,IP_ADDR0,IP_ADDR1,IP_ADDR2,IP_ADDR3(Addr));
  IP_ADDR4(&netmask,NETMASK_ADDR0,NETMASK_ADDR1,NETMASK_ADDR2,NETMASK_ADDR3);
  IP_ADDR4(&gw,GW_ADDR0,GW_ADDR1,GW_ADDR2,GW_ADDR3);
    
  netif_add(netif, &ipaddr, &netmask, &gw, NULL, &__BSP_NETIF_Init, &ethernet_input);
	//netif_add(netif, &ipaddr, &netmask, &gw, NULL, &BSP_NETIF_Init, &tcpip_input);
  
  /*  Registers the default network interface. */
  netif_set_default(netif);
  
  if (netif_is_link_up(netif))
  {
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(netif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(netif);
  }
	return 0;
}

static void BSP_NETIF_Deinit(struct netif *netif)
{
	netif_remove(netif);
	ETH_Deinit();
}

int32_t BSP_ETH_CheckConditions(void)
{	
	//if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good))
	//	return -1;
	//if (BSP_DEV_GET_STATE(&pca9536) != BSP_DEV_STATE_OK)
	//	return -1;
	//if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
	//	return -1;
	if (BSP_DEV_GET_STATE(&ksz8895) != BSP_DEV_STATE_OK)
		return -1;
	return 0;
}

void BSP_ETH_Process(uint8_t Addr)
{	
	if (BSP_ETH_CheckConditions())
	{
		/* External faults present */
		if(netif_default != NULL)
		{
			BSP_NETIF_Deinit(&bsp_netif);
		}
	}
	else
	{
		/* External faults absent */
		if(netif_default == NULL)
		{
			BSP_NETIF_Init(&bsp_netif, Addr);
		}
	}
}
