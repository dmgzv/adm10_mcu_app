#include "tftp_client.h"
#include "log.h"
#include <string.h>

#define TFTP_EVENT_IN_PROGRESS	0
#define TFTP_EVENT_COMPLETE			1
#define TFTP_EVENT_ERROR				2

#define TFTP_MESSAGE_QUEUE_SIZE 10

osMessageQDef(TFTP, TFTP_MESSAGE_QUEUE_SIZE, uint32_t);
osMutexDef(TFTP);
static struct udp_pcb *TFTP_CLIENT_udp_pcb;
static int (*TFTP_CLIENT_ReceiveCallback)(void *Priv, uint16_t Block, char *Data, uint32_t Size);
static void *TFTP_CLIENT_Priv;
static osMessageQId TFTP_CLIENT_MessageQId;
static osMutexId TFTP_CLIENT_MutexId;
static ip_addr_t *TFTP_CLIENT_Addr;
static uint16_t TFTP_CLIENT_Block;
static uint32_t TFTP_CLIENT_TimeoutCntr;
static uint32_t TFTP_CLIENT_TimeoutTotalCntr;

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
			if (TFTP_CLIENT_Block != block)
			{
				if (TFTP_CLIENT_ReceiveCallback(TFTP_CLIENT_Priv, block, &((char*)pkt_buf->payload)[4], pkt_buf->len - TFTP_DATA_PKT_HDR_LEN))
				{
					osMessagePut(TFTP_CLIENT_MessageQId, TFTP_EVENT_ERROR, 0);
					goto exit;
				}
			}
			TFTP_CLIENT_Block = block;
			TFTP_CLIENT_SendAck(upcb, block, addr, port);
			if (pkt_buf->len == TFTP_DATA_PKT_LEN_MAX)
			{
				osMessagePut(TFTP_CLIENT_MessageQId, TFTP_EVENT_IN_PROGRESS, 0);
			}
			else
			{
				osMessagePut(TFTP_CLIENT_MessageQId, TFTP_EVENT_COMPLETE, 0);
			}
		}
	}
exit:
	pbuf_free(pkt_buf);
}

int32_t TFTP_CLIENT_Init(void)
{
	err_t err;
	
	/* TFTP is already initialized*/
	if ((TFTP_CLIENT_udp_pcb) && (TFTP_CLIENT_MessageQId) && (TFTP_CLIENT_MutexId))
		return TFTP_CLIENT_OK;
	
	if (!TFTP_CLIENT_MutexId)
		TFTP_CLIENT_MutexId = osMutexCreate(osMutex(TFTP));
	
	if (!TFTP_CLIENT_MessageQId)
		TFTP_CLIENT_MessageQId = osMessageCreate(osMessageQ(TFTP), NULL);
	
	if (!TFTP_CLIENT_MessageQId)
		goto error;
	
  if (!TFTP_CLIENT_udp_pcb)
		TFTP_CLIENT_udp_pcb = udp_new();
	
	if (!TFTP_CLIENT_udp_pcb)
		goto error;
	
	/* Bind this PCB to port TFTP_PORT  */
  err = udp_bind(TFTP_CLIENT_udp_pcb, IP_ADDR_ANY, TFTP_PORT);
	if (err != ERR_OK)
		goto remove;
	
	udp_recv(TFTP_CLIENT_udp_pcb, __TFTP_CLIENT_ReceiveCallback, NULL);
	
	LOG_Info("TFTP_CLIENT", "", "Init", "");
	return TFTP_CLIENT_OK;

remove:
	udp_remove(TFTP_CLIENT_udp_pcb);
	TFTP_CLIENT_udp_pcb = 0;
error:
	LOG_Info("TFTP_CLIENT", "", "InitError", "");
	return TFTP_CLIENT_ERROR;
}

int32_t TFTP_CLIENT_DownloadFile(char *filename, ip_addr_t *addr, int (*ReceiveCallback)(void *Priv, uint16_t Block, char *Data, uint32_t Size), void *Priv)
{
	osEvent evt;	
	/* TFTP is not initialized*/
	if ((!TFTP_CLIENT_udp_pcb) || (!TFTP_CLIENT_MessageQId) || (!TFTP_CLIENT_MutexId))
		return TFTP_CLIENT_ERROR;
	
	osMutexWait(TFTP_CLIENT_MutexId, osWaitForever);
	LOG_Info("TFTP_CLIENT", "", "DownloadStart", "%d.%d.%d.%d;%s",
		(addr->addr >> 24) & 0xFF,
		(addr->addr >> 16) & 0xFF,
		(addr->addr >> 8) & 0xFF,
		(addr->addr >> 0) & 0xFF,
		filename
	);
	
	TFTP_CLIENT_Addr = addr;
	TFTP_CLIENT_Block = 0;
	TFTP_CLIENT_TimeoutCntr = 0;
	TFTP_CLIENT_TimeoutTotalCntr = 0;
	TFTP_CLIENT_ReceiveCallback = ReceiveCallback;
	TFTP_CLIENT_Priv = Priv;
	TFTP_CLIENT_SendReadRequest(TFTP_CLIENT_udp_pcb, filename, addr, TFTP_PORT);
	
in_progress:	
	evt = osMessageGet(TFTP_CLIENT_MessageQId, TFTP_TIMEOUT_INTERVAL);
	if (evt.status != osEventMessage) 
		goto timeout;
	
	if (evt.value.v == TFTP_EVENT_IN_PROGRESS)
	{
		TFTP_CLIENT_TimeoutCntr = 0;
		goto in_progress;
	}
	else if (evt.value.v != TFTP_EVENT_COMPLETE)
		goto error;
	
	LOG_Info("TFTP_CLIENT", "", "DownloadComplete", "%d", TFTP_CLIENT_TimeoutTotalCntr);
	osMutexRelease(TFTP_CLIENT_MutexId);
	return 0;
	
error:
	TFTP_CLIENT_ReceiveCallback = NULL;
	TFTP_CLIENT_Priv = NULL;
	LOG_Error("TFTP_CLIENT", "", "DownloadError", "%d", TFTP_CLIENT_TimeoutTotalCntr);
	osMutexRelease(TFTP_CLIENT_MutexId);
	return TFTP_CLIENT_ERROR;
	
timeout:
	if (TFTP_CLIENT_TimeoutCntr < TFTP_TIMEOUT_THR)
	{
		if (TFTP_CLIENT_Block != 0)
		{
			TFTP_CLIENT_SendAck(TFTP_CLIENT_udp_pcb, TFTP_CLIENT_Block, TFTP_CLIENT_Addr, TFTP_PORT);
		}
		else
		{
			TFTP_CLIENT_SendReadRequest(TFTP_CLIENT_udp_pcb, filename, addr, TFTP_PORT);
		}
		TFTP_CLIENT_TimeoutCntr++;
		TFTP_CLIENT_TimeoutTotalCntr++;
		goto in_progress;
	}
	TFTP_CLIENT_ReceiveCallback = NULL;
	TFTP_CLIENT_Priv = NULL;
	LOG_Error("TFTP_CLIENT", "", "DownloadTimeout", "%d", TFTP_CLIENT_TimeoutTotalCntr);
	osMutexRelease(TFTP_CLIENT_MutexId);
	return TFTP_CLIENT_TIMEOUT;
}
