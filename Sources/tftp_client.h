#ifndef TFTP_CLIENT_H
#define TFTP_CLIENT_H

#include "cmsis_os.h"
#include "lwip/mem.h"
#include "lwip/udp.h"

#define TFTP_OPCODE_LEN         2
#define TFTP_BLKNUM_LEN         2
#define TFTP_DATA_LEN_MAX       512
#define TFTP_DATA_PKT_HDR_LEN   (TFTP_OPCODE_LEN + TFTP_BLKNUM_LEN)
#define TFTP_ERR_PKT_HDR_LEN    (TFTP_OPCODE_LEN + TFTP_ERRCODE_LEN)
#define TFTP_ACK_PKT_LEN        (TFTP_OPCODE_LEN + TFTP_BLKNUM_LEN)
#define TFTP_DATA_PKT_LEN_MAX   (TFTP_DATA_PKT_HDR_LEN + TFTP_DATA_LEN_MAX)
#define TFTP_MAX_RETRIES        3
#define TFTP_TIMEOUT_INTERVAL   5

/* TFTP opcodes as specified in RFC1350   */
typedef enum {
  TFTP_RRQ = 1,
  TFTP_WRQ = 2,
  TFTP_DATA = 3,
  TFTP_ACK = 4,
  TFTP_ERROR = 5
} tftp_opcode;

int32_t TFTP_CLIENT_Init(void);

void TFTP_CLIENT_DownloadFile(char *filename, const ip_addr_t *addr, uint16_t port, void (*Program)(void *Priv, uint32_t Addr, char *Source, uint32_t Size), void *Priv, uint32_t BaseAddr);

#endif /*TFTP_CLIENT_H*/
