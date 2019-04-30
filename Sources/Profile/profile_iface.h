#ifndef __PROFILE_UART_H
#define __PROFILE_UART_H

/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "profile.h"
#include "profile_protocol.h"

#define PROFILE_IFACE_STOP_BYTE					0x0D
#define PROFILE_IFACE_CMD_MAX_SIZE			PROFILE_PROTOCOL_CMD_MAX_SIZE
#define PROFILE_IFACE_RSP_MAX_SIZE			PROFILE_PROTOCOL_RSP_MAX_SIZE

#define PROFILE_IFACE_CMD_QUEUE_SIZE 3

typedef struct {
	PROFILE_T *Profile;
	USART_TypeDef *Uart;
	uint8_t StartByte;
	osThreadId ReceiveThreadId;
	osThreadId ProcessThreadId;
	osMessageQId ProcessQId;
	osMessageQId ReceiveQId;
	char Commands[PROFILE_IFACE_CMD_QUEUE_SIZE][PROFILE_IFACE_CMD_MAX_SIZE];
	uint32_t CommandsLen[PROFILE_IFACE_CMD_QUEUE_SIZE];
	char Responce[PROFILE_IFACE_RSP_MAX_SIZE];
	uint32_t ResponceLen;
} PROFILE_IFACE_T;

int32_t PROFILE_IFACE_Init(PROFILE_IFACE_T *Iface, char *ReceiveName, char *ProcessName, USART_TypeDef *Uart, uint8_t SlotAddress, PROFILE_T *Profile);

#endif /* __PROFILE_UART_H */
