/* Includes ------------------------------------------------------------------*/
#include "bsp_eth.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_eth_sw.h"
#include "bsp_eth_pin.h"
#include "netif/etharp.h"
#include "lwip/init.h"
#include "lwip/tcpip.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "log.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Stack size of the interface thread */
#define BSP_NETIF_INPUT_THREAD_STACK_SIZE            ( 3 * 128 )

/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

osThreadDef(netif_input, ETH_NETIF_Input, osPriorityRealtime, 0, BSP_NETIF_INPUT_THREAD_STACK_SIZE);
osThreadId bsp_netif_input_thread_id;
struct netif bsp_netif;
uint8_t bsp_netif_tcpip_init_done;

uint8_t bsp_netif_mac[6] = {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5};
ip_addr_t bsp_netif_ipaddr = IPADDR4_INIT_BYTES(IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
ip_addr_t bsp_netif_netmask = IPADDR4_INIT_BYTES(NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
ip_addr_t bsp_netif_gw = IPADDR4_INIT_BYTES(GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
unsigned short int bsp_netif_vlan_prio_vid = 0x02;

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
  netif->linkoutput = ETH_NETIF_Output;

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
	if (!bsp_netif_input_thread_id)
		bsp_netif_input_thread_id = osThreadCreate (osThread(netif_input), netif);
	
	if (!bsp_netif_input_thread_id)
		return ERR_MEM;
	
  return ERR_OK;
}

static void tcpip_init_done_callback(void *arg)
{
	bsp_netif_tcpip_init_done = 1;
}

int32_t BSP_ETH_Init(void)
{
	if (!bsp_netif_tcpip_init_done)
		tcpip_init(tcpip_init_done_callback, NULL);
	return 0;
}

u32_t sys_jiffies(void)
{
  return HAL_GetTick();
}

u32_t sys_now(void)
{
  return HAL_GetTick();
}

static int32_t BSP_NETIF_Init(struct netif *netif)
{
	char ip[IP4ADDR_STRLEN_MAX];
	char netmask[IP4ADDR_STRLEN_MAX];
	char gw[IP4ADDR_STRLEN_MAX];
	
	if (ETH_Init(bsp_netif_mac))
		return -1;	
	if (!bsp_netif_tcpip_init_done)
		tcpip_init(tcpip_init_done_callback, NULL);
  
  if (!netif_add(netif, &bsp_netif_ipaddr, &bsp_netif_netmask, &bsp_netif_gw, NULL, &__BSP_NETIF_Init, &tcpip_input))
		return -1;
  
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
	LOG_Info("NETIF", "", "MacInit", "%02x:%02x:%02x:%02x:%02x:%02x", bsp_netif_mac[0], bsp_netif_mac[1], bsp_netif_mac[2], bsp_netif_mac[3], bsp_netif_mac[4], bsp_netif_mac[5]);
	ip4addr_ntoa_r(&bsp_netif_ipaddr, ip, sizeof(ip));
	LOG_Info("NETIF", "", "IpInit", "%s", ip);
	ip4addr_ntoa_r(&bsp_netif_netmask, netmask, sizeof(netmask));
	LOG_Info("NETIF", "", "NetmaskInit", "%s", netmask);
	ip4addr_ntoa_r(&bsp_netif_gw, gw, sizeof(gw));
	LOG_Info("NETIF", "", "GatewayInit", "%s", gw);
	return 0;
}

static int32_t BSP_NETIF_Update(struct netif *netif)
{
	char str[IP4ADDR_STRLEN_MAX];
	
	if (memcmp(&bsp_netif_ipaddr, netif_ip4_addr(netif), sizeof(bsp_netif_ipaddr)))
	{
		ip4addr_ntoa_r(&bsp_netif_ipaddr, str, sizeof(str));
		LOG_Info("NETIF", "", "IpUpdate", "%s", str);
		netif_set_ipaddr(netif, &bsp_netif_ipaddr);
	}
	if (memcmp(&bsp_netif_netmask, netif_ip4_netmask(netif), sizeof(bsp_netif_netmask)))
	{
		ip4addr_ntoa_r(&bsp_netif_netmask, str, sizeof(str));
		LOG_Info("NETIF", "", "NetmaskUpdate", "%s", str);
		netif_set_netmask(netif, &bsp_netif_netmask);
	}
	if (memcmp(&bsp_netif_gw, netif_ip4_gw(netif), sizeof(bsp_netif_gw)))
	{
		ip4addr_ntoa_r(&bsp_netif_gw, str, sizeof(str));
		LOG_Info("NETIF", "", "GatewayUpdate", "%s", str);
		netif_set_gw(netif, &bsp_netif_gw);
	}
	return 0;
}

static void BSP_NETIF_Deinit(struct netif *netif)
{
	LOG_Info("NETIF", "", "Deinit", "");
	netif_remove(netif);
	ETH_Deinit();
}

int32_t BSP_ETH_CheckConditions(void)
{	
	//if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good))
	//	return -1;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	if (BSP_DEV_GET_STATE(&ksz8895) != BSP_DEV_STATE_OK)
		return -1;
	if (BSP_DEV_GET_STATE(&bsp_eth_pin) != BSP_DEV_STATE_OK)
		return -1;
	return 0;
}

void BSP_ETH_Process(void)
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
			BSP_NETIF_Init(&bsp_netif);
		}
		else
		{
			BSP_NETIF_Update(&bsp_netif);
		}
	}
}
