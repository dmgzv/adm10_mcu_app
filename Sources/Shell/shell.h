#ifndef __SHELL_H
#define __SHELL_H

#include "cmsis_os.h"

#define SHELL_CMD_NAME_MAX_LEN				16
#define SHELL_CMD_DESCRIPTION_MAX_LEN	128

#define SHELL_RX_BUFFER_SIZE	64
#define SHELL_TX_BUFFER_SIZE	4096

typedef struct {
	char Name[SHELL_CMD_NAME_MAX_LEN];
	char Description[SHELL_CMD_DESCRIPTION_MAX_LEN];
	void (*Callback)(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
} SHELL_CMD_T;

typedef struct {
	int32_t (*Init)(void *Priv);
	int32_t (*Connect)(void *Priv);
	int32_t (*Receive)(void *Priv, char *Buffer, uint32_t BufferSize);
	int32_t (*Transmit)(void *Priv, char *Buffer, uint32_t BufferSize);
	int32_t (*Disconnect)(void *Priv);
	void *Priv;
} SHELL_IFACE_T;

typedef struct {
	SHELL_CMD_T *CmdList;
	uint32_t CmdListSize;
	SHELL_IFACE_T *Iface;
	char RxBuffer[SHELL_RX_BUFFER_SIZE];
	char TxBuffer[SHELL_TX_BUFFER_SIZE];
	osThreadId ThreadId;
} SHELL_T;

int32_t SHELL_Init(SHELL_T *Shell, char *Name, SHELL_CMD_T* CmdList, uint32_t CmdListSize, SHELL_IFACE_T *Iface);

int32_t SHELL_Transmit(void *Shell, const char *str, ...);
int32_t SHELL_TransmitBuffer(void *Shell, char *TxBuffer, uint32_t TxBufferLen);

#endif /* __SHELL_H */

