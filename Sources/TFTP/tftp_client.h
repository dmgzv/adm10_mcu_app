#ifndef TFTP_CLIENT_H
#define TFTP_CLIENT_H

#include "cmsis_os.h"
#include "lwip/mem.h"
#include "lwip/udp.h"

#define TFTP_PORT	69

#define TFTP_OPCODE_LEN         2
#define TFTP_BLKNUM_LEN         2
#define TFTP_DATA_LEN_MAX       512
#define TFTP_DATA_PKT_HDR_LEN   (TFTP_OPCODE_LEN + TFTP_BLKNUM_LEN)
#define TFTP_ERR_PKT_HDR_LEN    (TFTP_OPCODE_LEN + TFTP_ERRCODE_LEN)
#define TFTP_ACK_PKT_LEN        (TFTP_OPCODE_LEN + TFTP_BLKNUM_LEN)
#define TFTP_DATA_PKT_LEN_MAX   (TFTP_DATA_PKT_HDR_LEN + TFTP_DATA_LEN_MAX)
#define TFTP_TIMEOUT_INTERVAL   1000
#define TFTP_TIMEOUT_THR   			5

/* TFTP opcodes as specified in RFC1350   */
typedef enum {
  TFTP_RRQ = 1,
  TFTP_WRQ = 2,
  TFTP_DATA = 3,
  TFTP_ACK = 4,
  TFTP_ERROR = 5
} TFTP_OPCODE_T;

#define TFTP_CLIENT_OK					(0)
#define TFTP_CLIENT_ERROR 			(-1)
#define TFTP_CLIENT_TIMEOUT  		(-2)

int32_t TFTP_CLIENT_Init(void);
int32_t TFTP_CLIENT_DownloadFile(char *filename, ip_addr_t *addr, int (*ReceiveCallback)(void *Priv, uint16_t Block, char *Data, uint32_t Size), void *Priv);
#endif /*TFTP_CLIENT_H*/
