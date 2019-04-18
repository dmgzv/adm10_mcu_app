#ifndef __SHELL_UART_H
#define __SHELL_UART_H

#include "uart.h"
#include "shell.h"

int32_t SHELL_UART_Receive(void *Priv, char *Buffer, uint32_t BufferSize);
int32_t SHELL_UART_Transmit(void *Priv, char *Buffer, uint32_t BufferSize);

#endif /* __SHELL_UART_H */
