#include "tftp_client.h"
#include <string.h>

static void TFTP_CLIENT_Timeout(void const* args);

osTimerDef(TFTP, TFTP_CLIENT_Timeout);
osMutexDef(TFTP);
static struct udp_pcb *TFTP_CLIENT_udp_pcb;
static int (*TFTP_CLIENT_ReceiveCallback)(void *Priv, uint16_t Block, char *Data, uint32_t Size);
static void (*TFTP_CLIENT_TimeoutCallback)(void *Priv);
static void *TFTP_CLIENT_Priv;
static osTimerId TFTP_CLIENT_TimerId;
static osMutexId TFTP_CLIENT_MutexId;
static uint32_t TFTP_CLIENT_Status;

static TFTP_OPCODE_T TFTP_CLIENT_GetOpcode(char *buf)
{
  return (TFTP_OPCODE_T)(buf[1]);
}

static uint16_t TFTP_CLIENT_GetBlock(char *buf)
{
  uint16_t *b = (uint16_t*)buf;
  return ntohs(b[1]);
}

static void TFTP_CLIENT_SetOpcode(char *buffer, TFTP_OPCODE_T opcode)
{
  buffer[0] = 0;
  buffer[1] = (uint8_t)opcode;
}

static void TFTP_CLIENT_SetBlock(char* packet, uint16_t block)
{
  uint16_t *p = (uint16_t *)packet;
  p[1] = htons(block);
}


static int32_t TFTP_CLIENT_SendReadRequest(struct udp_pcb *upcb, char *filename, const ip_addr_t *addr, uint16_t port)
{
	err_t err;
	char req[256];
	uint16_t req_size;
	struct pbuf *pkt_buf;
	
	TFTP_CLIENT_SetOpcode(req, TFTP_RRQ);
	strcpy(&req[2], filename);
	req_size = strlen(&req[2]) + 3;
	strcpy(&req[req_size], "octet");
	req_size += 6;
	
	pkt_buf = pbuf_alloc(PBUF_TRANSPORT, req_size, PBUF_POOL);
	memcpy(pkt_buf->payload, req, req_size);
	err = udp_sendto(upcb, pkt_buf, addr, port);
	pbuf_free(pkt_buf);	
	if (err != ERR_OK)
		return -1;
	return 0;
}

static int32_t TFTP_CLIENT_SendAck(struct udp_pcb *upcb, uint16_t block, const ip_addr_t *addr, uint16_t port)
{
	err_t err;
	struct pbuf *pkt_buf;
	
	pkt_buf = pbuf_alloc(PBUF_TRANSPORT, TFTP_ACK_PKT_LEN, PBUF_POOL);
	memset(pkt_buf->payload, 0, TFTP_ACK_PKT_LEN);
	TFTP_CLIENT_SetOpcode(pkt_buf->payload, TFTP_ACK);
	TFTP_CLIENT_SetBlock(pkt_buf->payload, block);
	err = udp_sendto(upcb, pkt_buf, addr, port);
	pbuf_free(pkt_buf);
	
	if (err != ERR_OK)
		return -1;
	return 0;
}

static void __TFTP_CLIENT_ReceiveCallback(void *arg, struct udp_pcb *upcb, struct pbuf *pkt_buf, const ip_addr_t *addr, uint16_t port)
{
	TFTP_OPCODE_T opcode;
	uint16_t block;
	
	opcode = TFTP_CLIENT_GetOpcode(pkt_buf->payload);
	block = TFTP_CLIENT_GetBlock(pkt_buf->payload);
	if (opcode == TFTP_DATA)
	{
		block = ntohs(((uint16_t*)pkt_buf->payload)[1]);
		if (TFTP_CLIENT_ReceiveCallback)
		{
			if (!TFTP_CLIENT_ReceiveCallback(TFTP_CLIENT_Priv, block, &((char*)pkt_buf->payload)[4], pkt_buf->len - TFTP_DATA_PKT_HDR_LEN))
			{
				TFTP_CLIENT_SendAck(upcb, block, addr, port);
				if (pkt_buf->len == TFTP_DATA_PKT_LEN_MAX)
				{
					osTimerStart(TFTP_CLIENT_TimerId, TFTP_TIMEOUT_INTERVAL);
				}
				else
				{
					osTimerStop(TFTP_CLIENT_TimerId);
					TFTP_CLIENT_Status = TFTP_CLIENT_DOWNLOAD_SUCCESS;
				}
			}
			else
			{
				osTimerStop(TFTP_CLIENT_TimerId);
				TFTP_CLIENT_Status = TFTP_CLIENT_DOWNLOAD_FAILED;
			}
		}
	}
	pbuf_free(pkt_buf);
}

static void TFTP_CLIENT_Timeout(void const* args)
{
	TFTP_CLIENT_Status = TFTP_CLIENT_DOWNLOAD_TIMEOUT;
	if (TFTP_CLIENT_TimeoutCallback)
		TFTP_CLIENT_TimeoutCallback(TFTP_CLIENT_Priv);
	TFTP_CLIENT_TimeoutCallback = NULL;
	TFTP_CLIENT_ReceiveCallback = NULL;
	TFTP_CLIENT_Priv = NULL;
}

int32_t TFTP_CLIENT_Init(void)
{
	err_t err;
	
	if (!TFTP_CLIENT_MutexId)
		TFTP_CLIENT_MutexId = osMutexCreate(osMutex(TFTP));
	
	if (!TFTP_CLIENT_TimerId)
		TFTP_CLIENT_TimerId = osTimerCreate(osTimer(TFTP), osTimerOnce, NULL);
	
	if (!TFTP_CLIENT_TimerId)
		return -1;
	
  if (!TFTP_CLIENT_udp_pcb)
		TFTP_CLIENT_udp_pcb = udp_new();
	
	if (!TFTP_CLIENT_udp_pcb)
		return -1;
	
	/* Bind this PCB to port TFTP_PORT  */
  err = udp_bind(TFTP_CLIENT_udp_pcb, IP_ADDR_ANY, TFTP_PORT);
	if (err != ERR_OK)
		goto error;
	
	udp_recv(TFTP_CLIENT_udp_pcb, __TFTP_CLIENT_ReceiveCallback, NULL);
	
	return 0;

error:
	udp_remove(TFTP_CLIENT_udp_pcb);
	TFTP_CLIENT_udp_pcb = 0;
	return -1;
}

int32_t TFTP_CLIENT_DownloadFile(char *filename, ip_addr_t *addr, int (*ReceiveCallback)(void *Priv, uint16_t Block, char *Data, uint32_t Size), void (*TimeoutCallback)(void *Priv), void *Priv)
{	
	/* TFTP is already initialized*/
	if ((!TFTP_CLIENT_udp_pcb) || (!TFTP_CLIENT_TimerId) || (!TFTP_CLIENT_MutexId))
		return -1;
	
	osMutexWait(TFTP_CLIENT_MutexId, osWaitForever);
	
	if (TFTP_CLIENT_Status == TFTP_CLIENT_DOWNLOAD_IN_PROGRESS)
		goto error;
	
	TFTP_CLIENT_TimeoutCallback = TimeoutCallback;
	TFTP_CLIENT_ReceiveCallback = ReceiveCallback;
	TFTP_CLIENT_Priv = Priv;
	TFTP_CLIENT_SendReadRequest(TFTP_CLIENT_udp_pcb, filename, addr, TFTP_PORT);
	
	osTimerStart(TFTP_CLIENT_TimerId, TFTP_TIMEOUT_INTERVAL);
	TFTP_CLIENT_Status = TFTP_CLIENT_DOWNLOAD_IN_PROGRESS;
	
	osMutexRelease(TFTP_CLIENT_MutexId);
	return 0;
	
error:
	osMutexRelease(TFTP_CLIENT_MutexId);
	return -1;
}
