#ifndef __PROFILE_UART_H
#define __PROFILE_UART_H

/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "profile.h"
#include "profile_protocol.h"

//#define PROFILE_IFACE_MULTITHREAD

#define PROFILE_IFACE_START_BYTE(SlotAddress)	(((SlotAddress) == 1) ? 0x81 : (((SlotAddress) == 2) ? 0x82 : 0x83))
#define PROFILE_IFACE_STOP_BYTE					0x0D
#define PROFILE_IFACE_CMD_MAX_SIZE			PROFILE_PROTOCOL_CMD_MAX_SIZE
#define PROFILE_IFACE_RSP_MAX_SIZE			PROFILE_PROTOCOL_RSP_MAX_SIZE

#ifdef PROFILE_IFACE_MULTITHREAD
#define PROFILE_IFACE_CMD_QUEUE_SIZE 3
#endif

typedef struct {
	PROFILE_PROTOCOL_T *Protocol;
	USART_TypeDef *Uart;
	uint8_t *SlotAddress;
#ifdef PROFILE_IFACE_MULTITHREAD
	osThreadId ReceiveThreadId;
	osThreadId ProcessThreadId;
	osMessageQId ProcessQId;
	osMessageQId ReceiveQId;
	char Commands[PROFILE_IFACE_CMD_QUEUE_SIZE][PROFILE_IFACE_CMD_MAX_SIZE];
	uint32_t CommandsLen[PROFILE_IFACE_CMD_QUEUE_SIZE];
#else
	osThreadId ThreadId;
	char Buffer[PROFILE_IFACE_CMD_MAX_SIZE];
	char Command[PROFILE_IFACE_CMD_MAX_SIZE];
	uint32_t CommandLen;
#endif 
	char Responce[PROFILE_IFACE_RSP_MAX_SIZE];
	uint32_t ResponceLen;
	uint32_t MaxResponceLen;
	uint8_t MaxResponceLenCommand;
	uint32_t MaxParseTime;
	uint8_t MaxParseTimeCommand;
	uint32_t MaxTransmitTime;
	uint8_t MaxTransmitTimeCommand;
} PROFILE_IFACE_T;

#ifdef PROFILE_IFACE_MULTITHREAD
int32_t PROFILE_IFACE_Init(PROFILE_IFACE_T *Iface, char *ReceiveName, char *ProcessName, USART_TypeDef *Uart, uint8_t SlotAddress, PROFILE_PROTOCOL_T *Protocol);
#else
int32_t PROFILE_IFACE_Init(PROFILE_IFACE_T *Iface, char *ThreadName, USART_TypeDef *Uart, uint8_t *SlotAddress, PROFILE_PROTOCOL_T *Protocol);
#endif 
#endif /* __PROFILE_UART_H */
