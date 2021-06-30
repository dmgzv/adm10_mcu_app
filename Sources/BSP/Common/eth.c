/* Includes ------------------------------------------------------------------*/
#include "eth.h"
#include "lwip/tcpip.h"
#include <string.h>


#ifdef BSP_CONFIG_USE_FREERTOS
#define ETH_MESSAGE_QUEUE_SIZE 	(10)
#define ETH_EVENT_COMPLETE			(0)
#define ETH_EVENT_ERROR					(0xFFFFFFFF)
#define ETH_TIMEOUT 						(osWaitForever)
#else
#define ETH_TIMEOUT 						(1000)
#endif

/* ETH structure */
typedef struct {
	ETH_HandleTypeDef Handle;
#ifdef BSP_CONFIG_USE_FREERTOS
	osSemaphoreId s_xSemaphore;
#endif
} ETH_T;

/* ETH declaration */
static ETH_T eth;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__ALIGN_BEGIN ETH_DMADescTypeDef  DMARxDscrTab[ETH_RXBUFNB] __ALIGN_END; /* Ethernet Rx MA Descriptor */
__ALIGN_BEGIN ETH_DMADescTypeDef  DMATxDscrTab[ETH_TXBUFNB] __ALIGN_END; /* Ethernet Tx DMA Descriptor */
__ALIGN_BEGIN uint8_t Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE] __ALIGN_END; /* Ethernet Receive Buffer */
__ALIGN_BEGIN uint8_t Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE] __ALIGN_END; /* Ethernet Transmit Buffer */

/*******************************************************************************
                       Ethernet MSP Routines
*******************************************************************************/
/**
  * @brief  This function handles Ethernet interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{
	HAL_ETH_IRQHandler(&eth.Handle);
}

/**
  * @brief  Initializes the ETH MSP.
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Enable GPIOs clocks */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

/* Ethernet pins configuration ************************************************/
  /*
				MII_CRS ----------------------> PA0
        MII_RX_CLK -------------------> PA1
        MII_MDIO ---------------------> PA2
				MII_COL ----------------------> PA3
				MII_RX_DV --------------------> PA7
	
				MII_RXD2 ---------------------> PB0
				MII_RXD3 ---------------------> PB1
				MII_TXD3 ---------------------> PB8
        MII_TXD0 ---------------------> PB12
        MII_TXD1 ---------------------> PB13
	
	      MII_MDC ----------------------> PC1
        MII_TXD2 ---------------------> PC2
				MII_TX_CLK -------------------> PC3
        MII_RXD0 ---------------------> PC4
        MII_RXD1 ---------------------> PC5
				MII_INT ----------------------> PC6
				
        MII_TX_EN --------------------> PG11
	
  */
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_12|GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	/* Enable ETHERNET clock  */
  __HAL_RCC_ETH_CLK_ENABLE();
	
  /* Enable the Ethernet global Interrupt */
  HAL_NVIC_SetPriority(ETH_IRQn, ETH_IRQ_PRIORITY, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1);
  HAL_NVIC_EnableIRQ(ETH_IRQn);
}

/**
  * @brief  Ethernet Rx Transfer completed callback
  * @param  heth: ETH handle
  * @retval None
  */
void HAL_ETH_RxCpltCallback(ETH_HandleTypeDef *heth)
{
#ifdef BSP_CONFIG_USE_FREERTOS
	osSemaphoreRelease(eth.s_xSemaphore);
#endif
}
/**
  * @brief  This function initiallizes Ethernet interface.
  * @param  None
  * @retval None
  */
int32_t ETH_Init(uint8_t *macaddress)
{
	#ifdef BSP_CONFIG_USE_FREERTOS
	osSemaphoreDef(SEM);
	//osMessageQDef(eth, ETH_MESSAGE_QUEUE_SIZE, uint32_t);
	#endif

	eth.Handle.Instance = ETH;  
	eth.Handle.Init.MACAddr = macaddress;
	eth.Handle.Init.AutoNegotiation = ETH_AUTONEGOTIATION;
	eth.Handle.Init.Speed = ETH_SPEED;
	eth.Handle.Init.DuplexMode = ETH_MODE;
	eth.Handle.Init.MediaInterface = ETH_MEDIA_INTERFACE;
	eth.Handle.Init.RxMode = ETH_RX_MODE;
	eth.Handle.Init.ChecksumMode = ETH_CHECKSUM;
	eth.Handle.Init.PhyAddress = ETH_PHY_ADDRESS;

	/* configure ethernet peripheral (GPIOs, clocks, MAC, DMA) */
	if (HAL_ETH_Init(&eth.Handle) != HAL_OK)
	{
		return -1;
	}

	/* Initialize Tx Descriptors list: Chain Mode */
	HAL_ETH_DMATxDescListInit(&eth.Handle, DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);
	 
	/* Initialize Rx Descriptors list: Chain Mode  */
	HAL_ETH_DMARxDescListInit(&eth.Handle, DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

	#ifdef BSP_CONFIG_USE_FREERTOS
	if (!eth.s_xSemaphore)
		eth.s_xSemaphore = osSemaphoreCreate(osSemaphore(SEM) , 1 );
	#endif

	/* Enable MAC and DMA transmission and reception */
	HAL_ETH_Start(&eth.Handle);
	return 0;
}

/**
  * @brief  This function deinitiallizes Ethernet interface.
  * @param  None
  * @retval None
  */
void ETH_Deinit(void)
{
	/* Disable MAC and DMA transmission and reception */
	HAL_ETH_Stop(&eth.Handle);
	/* Disable MAC and DMA transmission and reception */
	HAL_ETH_DeInit(&eth.Handle);
}

/**
  * @brief This function should do the actual transmission of the packet. The packet is
  * contained in the pbuf that is passed to the function. This pbuf
  * might be chained.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
  * @return ERR_OK if the packet could be sent
  *         an err_t value if the packet couldn't be sent
  *
  * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
  *       strange results. You might consider waiting for space in the DMA queue
  *       to become available since the stack doesn't retry to send a packet
  *       dropped because of memory failure (except for the TCP timers).
  */
err_t ETH_Transmit(struct pbuf *p)
{
	err_t errval;
	struct pbuf *q;
	uint8_t *buffer = (uint8_t *)(eth.Handle.TxDesc->Buffer1Addr);
	__IO ETH_DMADescTypeDef *DmaTxDesc;
	uint32_t framelength = 0;
	uint32_t bufferoffset = 0;
	uint32_t byteslefttocopy = 0;
	uint32_t payloadoffset = 0;

	DmaTxDesc = eth.Handle.TxDesc;
	bufferoffset = 0;

	/* copy frame from pbufs to driver buffers */
	for(q = p; q != NULL; q = q->next)
	{
		/* Is this buffer available? If not, goto error */
		if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
		{
			errval = ERR_USE;
			goto error;
		}

		/* Get bytes in current lwIP buffer */
		byteslefttocopy = q->len;
		payloadoffset = 0;

		/* Check if the length of data to copy is bigger than Tx buffer size*/
		while( (byteslefttocopy + bufferoffset) > ETH_TX_BUF_SIZE )
		{
			/* Copy data to Tx buffer*/
			memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), (ETH_TX_BUF_SIZE - bufferoffset) );

			/* Point to next descriptor */
			DmaTxDesc = (ETH_DMADescTypeDef *)(DmaTxDesc->Buffer2NextDescAddr);

			/* Check if the buffer is available */
			if((DmaTxDesc->Status & ETH_DMATXDESC_OWN) != (uint32_t)RESET)
			{
				errval = ERR_USE;
				goto error;
			}

			buffer = (uint8_t *)(DmaTxDesc->Buffer1Addr);

			byteslefttocopy = byteslefttocopy - (ETH_TX_BUF_SIZE - bufferoffset);
			payloadoffset = payloadoffset + (ETH_TX_BUF_SIZE - bufferoffset);
			framelength = framelength + (ETH_TX_BUF_SIZE - bufferoffset);
			bufferoffset = 0;
		}

		/* Copy the remaining bytes */
		memcpy( (uint8_t*)((uint8_t*)buffer + bufferoffset), (uint8_t*)((uint8_t*)q->payload + payloadoffset), byteslefttocopy );
		bufferoffset = bufferoffset + byteslefttocopy;
		framelength = framelength + byteslefttocopy;
	}

	/* Prepare transmit descriptors to give to DMA */ 
	if (HAL_ETH_TransmitFrame(&eth.Handle, framelength) != HAL_OK)
	{
		errval = ERR_USE;
		goto error;
	}

	errval = ERR_OK;
  
error:
  
  /* When Transmit Underflow flag is set, clear it and issue a Transmit Poll Demand to resume transmission */
  if ((eth.Handle.Instance->DMASR & ETH_DMASR_TUS) != (uint32_t)RESET)
  {
    /* Clear TUS ETHERNET DMA flag */
    eth.Handle.Instance->DMASR = ETH_DMASR_TUS;
    
    /* Resume DMA transmission*/
    eth.Handle.Instance->DMATPDR = 0;
  }
  return errval;
}

/**
  * @brief Should allocate a pbuf and transfer the bytes of the incoming
  * packet from the interface into the pbuf.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return a pbuf filled with the received packet (including MAC header)
  *         NULL on memory error
  */
struct pbuf * ETH_Receive(void)
{
	struct pbuf *p = NULL, *q = NULL;
	uint16_t len = 0;
	uint8_t *buffer;
	__IO ETH_DMADescTypeDef *dmarxdesc;
	uint32_t bufferoffset = 0;
	uint32_t payloadoffset = 0;
	uint32_t byteslefttocopy = 0;
	uint32_t i=0;
	HAL_StatusTypeDef status;
	
	/* get received frame */
	LOCK_TCPIP_CORE();
	status = HAL_ETH_GetReceivedFrame_IT(&eth.Handle);
	UNLOCK_TCPIP_CORE();
	if(status != HAL_OK)
		return NULL; 
	
	/* Obtain the size of the packet and put it into the "len" variable. */
	len = eth.Handle.RxFrameInfos.length;
	buffer = (uint8_t *)eth.Handle.RxFrameInfos.buffer;

	if (len > 0)
	{
		/* We allocate a pbuf chain of pbufs from the Lwip buffer pool */
		p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);
	}

	if (p != NULL)
	{
		dmarxdesc = eth.Handle.RxFrameInfos.FSRxDesc;
		bufferoffset = 0;

		for(q = p; q != NULL; q = q->next)
		{
			byteslefttocopy = q->len;
			payloadoffset = 0;

			/* Check if the length of bytes to copy in current pbuf is bigger than Rx buffer size */
			while( (byteslefttocopy + bufferoffset) > ETH_RX_BUF_SIZE )
			{
				/* Copy data to pbuf */
				memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), (ETH_RX_BUF_SIZE - bufferoffset));

				/* Point to next descriptor */
				dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
				buffer = (uint8_t *)(dmarxdesc->Buffer1Addr);

				byteslefttocopy = byteslefttocopy - (ETH_RX_BUF_SIZE - bufferoffset);
				payloadoffset = payloadoffset + (ETH_RX_BUF_SIZE - bufferoffset);
				bufferoffset = 0;
			}

			/* Copy remaining data in pbuf */
			memcpy( (uint8_t*)((uint8_t*)q->payload + payloadoffset), (uint8_t*)((uint8_t*)buffer + bufferoffset), byteslefttocopy);
			bufferoffset = bufferoffset + byteslefttocopy;
		}
	}

	/* Release descriptors to DMA */
	/* Point to first descriptor */
	dmarxdesc = eth.Handle.RxFrameInfos.FSRxDesc;
	/* Set Own bit in Rx descriptors: gives the buffers back to DMA */
	for (i=0; i< eth.Handle.RxFrameInfos.SegCount; i++)
	{  
		dmarxdesc->Status |= ETH_DMARXDESC_OWN;
		dmarxdesc = (ETH_DMADescTypeDef *)(dmarxdesc->Buffer2NextDescAddr);
	}

	/* Clear Segment_Count */
	eth.Handle.RxFrameInfos.SegCount =0;

	/* When Rx Buffer unavailable flag is set: clear it and resume reception */
	if ((eth.Handle.Instance->DMASR & ETH_DMASR_RBUS) != (uint32_t)RESET)  
	{
		/* Clear RBUS ETHERNET DMA flag */
		eth.Handle.Instance->DMASR = ETH_DMASR_RBUS;
		/* Resume DMA reception */
		eth.Handle.Instance->DMARPDR = 0;
	}
	return p;
}

void ETH_NETIF_Input( void const * argument )
{
	struct pbuf *p;
  struct netif *netif = (struct netif *) argument;
	
  for( ;; )
  {
		if (osSemaphoreWait( eth.s_xSemaphore, ETH_TIMEOUT) != osOK)
			continue;
		do {
			p = ETH_Receive();
			if (p != NULL)
			{
				
				if (netif->input( p, netif) != ERR_OK )
				{
					pbuf_free(p);
				}
				
			}
		} while(p!=NULL);
  }
}

err_t ETH_NETIF_Output(struct netif *netif, struct pbuf *p)
{
	return ETH_Transmit(p);
}

int32_t ETH_ReadPHYRegister(uint16_t PhyAddr, uint16_t PHYReg, uint32_t *RegValue)
{
#ifdef BSP_CONFIG_USE_ETH
	eth.Handle.Init.PhyAddress = PhyAddr;
	if (HAL_ETH_ReadPHYRegister(&eth.Handle, PHYReg, RegValue) != HAL_OK)
		return -1;
	return 0;
#else
	return -1;
#endif
}

int32_t ETH_WritePHYRegister(uint16_t PhyAddr, uint16_t PHYReg, uint32_t RegValue)
{
#ifdef BSP_CONFIG_USE_ETH
	eth.Handle.Init.PhyAddress = PhyAddr;
	if (HAL_ETH_WritePHYRegister(&eth.Handle, PHYReg, RegValue) != HAL_OK)
		return -1;
	return 0;
#else
	return -1;
#endif
}

void ETH_GetMacAddress(uint8_t *macaddress)
{
	macaddress[0] = eth.Handle.Init.MACAddr[0];
	macaddress[1] = eth.Handle.Init.MACAddr[1];
	macaddress[2] = eth.Handle.Init.MACAddr[2];
	macaddress[3] = eth.Handle.Init.MACAddr[3];
	macaddress[4] = eth.Handle.Init.MACAddr[4];
	macaddress[5] = eth.Handle.Init.MACAddr[5];
}
