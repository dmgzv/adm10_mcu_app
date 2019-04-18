#ifndef __SHELL_TELNET_H
#define __SHELL_TELNET_H

#include "shell.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/sys.h"

typedef struct {
	struct netconn *conn;
	struct netconn *newconn;
} SHELL_TELNET_PRIV_T;

int32_t SHELL_TELNET_Init(void *Priv);
int32_t SHELL_TELNET_Connect(void *Priv);
int32_t SHELL_TELNET_Receive(void *Priv, char *Buffer, uint32_t BufferSize);
int32_t SHELL_TELNET_Transmit(void *Priv, char *Buffer, uint32_t BufferSize);
int32_t SHELL_TELNET_Disconnect(void *Priv);

#endif /* __SHELL_TELNET_H */
