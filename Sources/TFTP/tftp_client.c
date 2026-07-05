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
static int32_t (*TFTP_CLIENT_ReceiveCallback)(void *Priv, uint16_t Block, char *Data, uint32_t Size);
static int32_t (*TFTP_CLIENT_TransmitCallback)(void *Priv, uint16_t Block, char *Data, uint32_t *Size);
static void *TFTP_CLIENT_Priv;
static osMessageQId TFTP_CLIENT_MessageQId;
static osMutexId TFTP_CLIENT_MutexId;
static ip_addr_t *TFTP_CLIENT_Addr;
static uint16_t TFTP_CLIENT_Block;
static uint32_t TFTP_CLIENT_TimeoutCntr;
static uint32_t TFTP_CLIENT_TimeoutTotalCntr;
static char TFTP_CLIENT_Data[TFTP_DATA_LEN_MAX];
static uint32_t TFTP_CLIENT_Size;

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

static void TFTP_CLIENT_SetFilename(char *buffer, char* filename)
{
  strcpy(&buffer[TFTP_OPCODE_LEN], filename);
}

static void TFTP_CLIENT_SetMode(char *buffer, char* mode)
{
  strcpy(&buffer[TFTP_OPCODE_LEN + strlen(&buffer[TFTP_OPCODE_LEN]) + 1], mode);
}

static void TFTP_CLIENT_SetBlock(char* packet, uint16_t block)
{
  uint16_t *p = (uint16_t *)packet;
  p[1] = htons(block);
}

static void TFTP_CLIENT_SetData(char* packet, char *data, uint16_t size)
{
  memcpy(&packet[TFTP_DATA_PKT_HDR_LEN], data, size);
}


static int32_t TFTP_CLIENT_SendReadRequest(struct udp_pcb *upcb, char *filename, const ip_addr_t *addr, uint16_t port)
{
	err_t err;
	struct pbuf *pkt_buf;
	uint16_t pkt_len;
	
	pkt_len = TFTP_OPCODE_LEN + strlen(filename) + 1 + sizeof("octet");
	pkt_buf = pbuf_alloc(PBUF_TRANSPORT, pkt_len, PBUF_RAM);
	if (!pkt_buf)
		return -1;
	memset(pkt_buf->payload, 0, pkt_len);
	TFTP_CLIENT_SetOpcode(pkt_buf->payload, TFTP_RRQ);
	TFTP_CLIENT_SetFilename(pkt_buf->payload, filename);
	TFTP_CLIENT_SetMode(pkt_buf->payload, "octet");
	err = udp_sendto(upcb, pkt_buf, addr, port);
	pbuf_free(pkt_buf);

	if (err != ERR_OK)
		return -1;
	return 0;
}

static int32_t TFTP_CLIENT_SendWriteRequest(struct udp_pcb *upcb, char *filename, const ip_addr_t *addr, uint16_t port)
{
	err_t err;
	struct pbuf *pkt_buf;
	uint16_t pkt_len;
	
	pkt_len = TFTP_OPCODE_LEN + strlen(filename) + 1 + sizeof("octet");
	pkt_buf = pbuf_alloc(PBUF_TRANSPORT, pkt_len, PBUF_RAM);
	if (!pkt_buf)
		return -1;
	memset(pkt_buf->payload, 0, pkt_len);
	TFTP_CLIENT_SetOpcode(pkt_buf->payload, TFTP_WRQ);
	TFTP_CLIENT_SetFilename(pkt_buf->payload, filename);
	TFTP_CLIENT_SetMode(pkt_buf->payload, "octet");
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
	
	pkt_buf = pbuf_alloc(PBUF_TRANSPORT, TFTP_ACK_PKT_LEN, PBUF_RAM);
	if (!pkt_buf)
		return -1;
	memset(pkt_buf->payload, 0, TFTP_ACK_PKT_LEN);
	TFTP_CLIENT_SetOpcode(pkt_buf->payload, TFTP_ACK);
	TFTP_CLIENT_SetBlock(pkt_buf->payload, block);
	err = udp_sendto(upcb, pkt_buf, addr, port);
	pbuf_free(pkt_buf);
	
	if (err != ERR_OK)
		return -1;
	return 0;
}

static int32_t TFTP_CLIENT_SendData(struct udp_pcb *upcb, uint16_t block, const ip_addr_t *addr, uint16_t port, char* data, uint16_t size)
{
	err_t err;
	struct pbuf *pkt_buf;
	uint16_t pkt_len;

	if (size > TFTP_DATA_LEN_MAX)
		return -1;

	pkt_len = TFTP_DATA_PKT_HDR_LEN + size;
	pkt_buf = pbuf_alloc(PBUF_TRANSPORT, pkt_len, PBUF_RAM);
	if (!pkt_buf)
		return -1;
	memset(pkt_buf->payload, 0, pkt_len);
	TFTP_CLIENT_SetOpcode(pkt_buf->payload, TFTP_DATA);
	TFTP_CLIENT_SetBlock(pkt_buf->payload, block);
	TFTP_CLIENT_SetData(pkt_buf->payload, data, size);
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
	
	if ((!pkt_buf) || (pkt_buf->len < TFTP_DATA_PKT_HDR_LEN) || (pkt_buf->len != pkt_buf->tot_len))
		goto exit;

	opcode = TFTP_CLIENT_GetOpcode(pkt_buf->payload);
	block = TFTP_CLIENT_GetBlock(pkt_buf->payload);
	if (opcode == TFTP_DATA)
	{
		if (TFTP_CLIENT_ReceiveCallback)
		{
			if (TFTP_CLIENT_Block != block)
			{
				if (TFTP_CLIENT_ReceiveCallback(TFTP_CLIENT_Priv, block, &((char*)pkt_buf->payload)[TFTP_DATA_PKT_HDR_LEN], pkt_buf->len - TFTP_DATA_PKT_HDR_LEN))
				{
					osMessagePut(TFTP_CLIENT_MessageQId, TFTP_EVENT_ERROR, 0);
					goto exit;
				}
			}
			TFTP_CLIENT_Block = block;
			if (TFTP_CLIENT_SendAck(upcb, block, addr, port))
			{
				osMessagePut(TFTP_CLIENT_MessageQId, TFTP_EVENT_ERROR, 0);
				goto exit;
			}
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
	else if (opcode == TFTP_ACK)
	{
		if (TFTP_CLIENT_TransmitCallback)
		{
			if ((block == 0) || (TFTP_CLIENT_Block == (block + 1)) || (TFTP_CLIENT_Size == TFTP_DATA_LEN_MAX))
			{
				TFTP_CLIENT_Block = block + 1;
				if (TFTP_CLIENT_TransmitCallback(TFTP_CLIENT_Priv, TFTP_CLIENT_Block, TFTP_CLIENT_Data, &TFTP_CLIENT_Size))
				{
					osMessagePut(TFTP_CLIENT_MessageQId, TFTP_EVENT_ERROR, 0);
					goto exit;
				}
				if (TFTP_CLIENT_SendData(upcb, TFTP_CLIENT_Block, addr, port, TFTP_CLIENT_Data, TFTP_CLIENT_Size))
				{
					osMessagePut(TFTP_CLIENT_MessageQId, TFTP_EVENT_ERROR, 0);
					goto exit;
				}
				osMessagePut(TFTP_CLIENT_MessageQId, TFTP_EVENT_IN_PROGRESS, 0);
			}
			else
			{
				osMessagePut(TFTP_CLIENT_MessageQId, TFTP_EVENT_COMPLETE, 0);
			}
		}
	}
	else
	{
		osMessagePut(TFTP_CLIENT_MessageQId, TFTP_EVENT_ERROR, 0);
	}
exit:
	if (pkt_buf)
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
		return TFTP_CLIENT_ERROR;
	
  if (!TFTP_CLIENT_udp_pcb)
		TFTP_CLIENT_udp_pcb = udp_new();
	
	if (!TFTP_CLIENT_udp_pcb)
		return TFTP_CLIENT_ERROR;
	
	/* Bind this PCB to port TFTP_PORT  */
  err = udp_bind(TFTP_CLIENT_udp_pcb, IP_ADDR_ANY, TFTP_PORT);
	if (err != ERR_OK)
		goto error;
	
	udp_recv(TFTP_CLIENT_udp_pcb, __TFTP_CLIENT_ReceiveCallback, NULL);
	
	return TFTP_CLIENT_OK;

error:
	udp_remove(TFTP_CLIENT_udp_pcb);
	TFTP_CLIENT_udp_pcb = 0;
	return TFTP_CLIENT_ERROR;
}

int32_t TFTP_CLIENT_DownloadFile(char *filename, ip_addr_t *addr, int32_t (*ReceiveCallback)(void *Priv, uint16_t Block, char *Data, uint32_t Size), void *Priv)
{
	osEvent evt;	
	/* TFTP is not initialized*/
	if ((!TFTP_CLIENT_udp_pcb) || (!TFTP_CLIENT_MessageQId) || (!TFTP_CLIENT_MutexId))
		return TFTP_CLIENT_ERROR;
	
	osMutexWait(TFTP_CLIENT_MutexId, osWaitForever);
	LOG_Info("TFTP", "CLIENT", "", "Download");
	
	TFTP_CLIENT_Addr = addr;
	TFTP_CLIENT_Block = 0;
	TFTP_CLIENT_TimeoutCntr = 0;
	TFTP_CLIENT_TimeoutTotalCntr = 0;
	TFTP_CLIENT_ReceiveCallback = ReceiveCallback;
	TFTP_CLIENT_TransmitCallback = NULL;
	TFTP_CLIENT_Priv = Priv;
	if (TFTP_CLIENT_SendReadRequest(TFTP_CLIENT_udp_pcb, filename, addr, TFTP_PORT))
		goto error;

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

	LOG_Info("TFTP", "CLIENT", "", "Timeouts: %d", TFTP_CLIENT_TimeoutTotalCntr);
	LOG_Info("TFTP", "CLIENT", "", "Download: Complete");
	osMutexRelease(TFTP_CLIENT_MutexId);
	return 0;

error:
	TFTP_CLIENT_ReceiveCallback = NULL;
	TFTP_CLIENT_TransmitCallback = NULL;
	TFTP_CLIENT_Priv = NULL;
	LOG_Info("TFTP", "CLIENT", "", "Timeouts: %d", TFTP_CLIENT_TimeoutTotalCntr);
	LOG_Error("TFTP", "CLIENT", "", "Download: Error");
	osMutexRelease(TFTP_CLIENT_MutexId);
	return TFTP_CLIENT_ERROR;

timeout:
	if (TFTP_CLIENT_TimeoutCntr < TFTP_TIMEOUT_THR)
	{
		if (TFTP_CLIENT_Block != 0)
		{
			if (TFTP_CLIENT_SendAck(TFTP_CLIENT_udp_pcb, TFTP_CLIENT_Block, TFTP_CLIENT_Addr, TFTP_PORT))
				goto error;
		}
		else
		{
			if (TFTP_CLIENT_SendReadRequest(TFTP_CLIENT_udp_pcb, filename, addr, TFTP_PORT))
				goto error;
		}
		TFTP_CLIENT_TimeoutCntr++;
		TFTP_CLIENT_TimeoutTotalCntr++;
		goto in_progress;
	}
	TFTP_CLIENT_ReceiveCallback = NULL;
	TFTP_CLIENT_TransmitCallback = NULL;
	TFTP_CLIENT_Priv = NULL;
	LOG_Info("TFTP", "CLIENT", "", "Timeouts: %d", TFTP_CLIENT_TimeoutTotalCntr);
	LOG_Error("TFTP", "CLIENT", "", "Download: Timeout");
	osMutexRelease(TFTP_CLIENT_MutexId);
	return TFTP_CLIENT_TIMEOUT;
}

int32_t TFTP_CLIENT_UploadFile(char *filename, ip_addr_t *addr, int32_t (*TransmitCallback)(void *Priv, uint16_t Block, char *Data, uint32_t *Size), void *Priv)
{
	osEvent evt;
	/* TFTP is not initialized*/
	if ((!TFTP_CLIENT_udp_pcb) || (!TFTP_CLIENT_MessageQId) || (!TFTP_CLIENT_MutexId))
		return TFTP_CLIENT_ERROR;

	osMutexWait(TFTP_CLIENT_MutexId, osWaitForever);
	LOG_Info("TFTP", "CLIENT", "", "Upload");

	TFTP_CLIENT_Addr = addr;
	TFTP_CLIENT_Block = 0;
	TFTP_CLIENT_TimeoutCntr = 0;
	TFTP_CLIENT_TimeoutTotalCntr = 0;
	TFTP_CLIENT_ReceiveCallback = NULL;
	TFTP_CLIENT_TransmitCallback = TransmitCallback;
	TFTP_CLIENT_Priv = Priv;
	if (TFTP_CLIENT_SendWriteRequest(TFTP_CLIENT_udp_pcb, filename, addr, TFTP_PORT))
		goto error;

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
	
	LOG_Info("TFTP", "CLIENT", "", "Timeouts: %d", TFTP_CLIENT_TimeoutTotalCntr);
	LOG_Info("TFTP", "CLIENT", "", "Upload: Complete");
	osMutexRelease(TFTP_CLIENT_MutexId);
	return 0;
	
error:
	TFTP_CLIENT_ReceiveCallback = NULL;
	TFTP_CLIENT_TransmitCallback = NULL;
	TFTP_CLIENT_Priv = NULL;
	LOG_Info("TFTP", "CLIENT", "", "Timeouts: %d", TFTP_CLIENT_TimeoutTotalCntr);
	LOG_Error("TFTP", "CLIENT", "", "Upload: Error");
	osMutexRelease(TFTP_CLIENT_MutexId);
	return TFTP_CLIENT_ERROR;
	
timeout:
	if (TFTP_CLIENT_TimeoutCntr < TFTP_TIMEOUT_THR)
	{
		if (TFTP_CLIENT_Block != 0)
		{
			if (TFTP_CLIENT_SendData(TFTP_CLIENT_udp_pcb, TFTP_CLIENT_Block, TFTP_CLIENT_Addr, TFTP_PORT, TFTP_CLIENT_Data, TFTP_CLIENT_Size))
				goto error;
		}
		else
		{
			if (TFTP_CLIENT_SendWriteRequest(TFTP_CLIENT_udp_pcb, filename, addr, TFTP_PORT))
				goto error;
		}
		TFTP_CLIENT_TimeoutCntr++;
		TFTP_CLIENT_TimeoutTotalCntr++;
		goto in_progress;
	}
	TFTP_CLIENT_ReceiveCallback = NULL;
	TFTP_CLIENT_TransmitCallback = NULL;
	TFTP_CLIENT_Priv = NULL;
	LOG_Info("TFTP", "CLIENT", "", "Timeouts: %d", TFTP_CLIENT_TimeoutTotalCntr);
	LOG_Error("TFTP", "CLIENT", "", "Upload: Timeout");
	osMutexRelease(TFTP_CLIENT_MutexId);
	return TFTP_CLIENT_TIMEOUT;
}
