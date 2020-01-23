#ifndef __SHELL_TELNET_H
#define __SHELL_TELNET_H

#include "shell.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/sys.h"

#define SHELL_TELNET_RING_BUFFER_SIZE	(1 << 12) 

typedef struct {
	struct netconn *conn;
	struct netconn *newconn;
	char ring_buffer[SHELL_TELNET_RING_BUFFER_SIZE];
	uint32_t ring_buffer_ptr;
	uint32_t ring_buffer_len;
} SHELL_TELNET_PRIV_T;

int32_t SHELL_TELNET_Init(void *Priv);
int32_t SHELL_TELNET_Connect(void *Priv);
int32_t SHELL_TELNET_Receive(void *Priv, char *Buffer, uint32_t BufferSize);
int32_t SHELL_TELNET_Transmit(void *Priv, char *Buffer, uint32_t BufferSize);
int32_t SHELL_TELNET_Disconnect(void *Priv);

#endif /* __SHELL_TELNET_H */
