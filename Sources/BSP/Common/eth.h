#ifndef __ETH_H
#define __ETH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "bsp_config.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif
#include "lwip/pbuf.h"
#include "lwip/netif.h"


#define ETH_AUTONEGOTIATION		ETH_AUTONEGOTIATION_ENABLE
#define ETH_SPEED							ETH_SPEED_100M
#define ETH_MODE							ETH_MODE_FULLDUPLEX
#define ETH_MEDIA_INTERFACE		ETH_MEDIA_INTERFACE_MII
#define ETH_RX_MODE						ETH_RXINTERRUPT_MODE
#define ETH_CHECKSUM					ETH_CHECKSUM_BY_HARDWARE
#define ETH_PHY_ADDRESS				(0x00)

#ifdef BSP_CONFIG_USE_FREERTOS
#define ETH_IRQ_PRIORITY    	(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)
#else
#define ETH_IRQ_PRIORITY			(0)
#endif

int32_t ETH_Init(uint8_t *macaddress);
void ETH_Deinit(void);
err_t ETH_Transmit(struct pbuf *p);
struct pbuf * ETH_Receive(void);

void ETH_NETIF_Input( void const * argument );
err_t ETH_NETIF_Output(struct netif *netif, struct pbuf *p);

int32_t ETH_ReadPHYRegister(uint16_t PhyAddr, uint16_t PHYReg, uint32_t *RegValue);
int32_t ETH_WritePHYRegister(uint16_t PhyAddr, uint16_t PHYReg, uint32_t RegValue);

void ETH_GetMacAddress(uint8_t *macaddress);

#endif /* __ETH_H */
