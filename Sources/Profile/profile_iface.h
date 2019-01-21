#ifndef __PROFILE_UART_H
#define __PROFILE_UART_H

/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "profile.h"
#include "profile_protocol.h"

#define PROFILE_IFACE_STOP_BYTE					0x0D
#define PROFILE_IFACE_CMD_MAX_SIZE			PROFILE_PROTOCOL_CMD_MAX_SIZE
#define PROFILE_IFACE_RSP_MAX_SIZE			PROFILE_PROTOCOL_RSP_MAX_SIZE

typedef struct {
	PROFILE_T *Profile;
	USART_TypeDef *Uart;
	uint8_t StartByte;
	osThreadId ThreadId;
	char Command[PROFILE_IFACE_CMD_MAX_SIZE];
	uint32_t CommandLen;
	char Responce[PROFILE_IFACE_RSP_MAX_SIZE];
	uint32_t ResponceLen;
} PROFILE_IFACE_T;

int32_t PROFILE_IFACE_Init(PROFILE_IFACE_T *Iface, char *Name, USART_TypeDef *Uart, uint8_t SlotAddress, PROFILE_T *Profile);

#endif /* __PROFILE_UART_H */
