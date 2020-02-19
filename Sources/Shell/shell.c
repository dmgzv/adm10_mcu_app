#include "shell.h"
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define SHELL_THREAD_PRIORITY		osPriorityAboveNormal
#define SHELL_THREAD_STACK_SIZE	1024

#define SHELL_CMD_PARAMS_MAX_CNT	8

#define SHELL_IFACE_Init(Shell) (Shell)->Iface->Init ? (Shell)->Iface->Init((Shell)->Iface->Priv) : 0
#define SHELL_IFACE_Connect(Shell) (Shell)->Iface->Connect ? (Shell)->Iface->Connect((Shell)->Iface->Priv) : 0
#define SHELL_IFACE_Receive(Shell, RxBuffer, RxBufferSize) (Shell)->Iface->Receive((Shell)->Iface->Priv, RxBuffer, RxBufferSize)
#define SHELL_IFACE_Transmit(Shell, TxBuffer, TxBufferSize) (Shell)->Iface->Transmit((Shell)->Iface->Priv, TxBuffer, TxBufferSize)
#define SHELL_IFACE_Disconnect(Shell) (Shell)->Iface->Disconnect ? (Shell)->Iface->Disconnect((Shell)->Iface->Priv) : 0

#define SHELL_VERSION	"1.0.0"
#define SHELL_HELLO 	"Welcome to IPG IRE-Polus Shell (ver. "SHELL_VERSION")\r"\
											"=====================================================\r"

#define SHELL_UNKNOWN_CMD 	"Unknown command\r"
#define SHELL_GOOD_BYE 			"Good bye...\r"

#define SHELL_POINTER	"> "

#define SHELL_IFACE_TransmitHello(Shell) Shell->Iface->Transmit(Shell->Iface->Priv, SHELL_HELLO, sizeof(SHELL_HELLO))
#define SHELL_IFACE_TransmitGoodBye(Shell) Shell->Iface->Transmit(Shell->Iface->Priv, SHELL_GOOD_BYE, sizeof(SHELL_GOOD_BYE))
#define SHELL_IFACE_TransmitEmptyCmd(Shell)
#define SHELL_IFACE_TransmitUnknownCmd(Shell) Shell->Iface->Transmit(Shell->Iface->Priv, SHELL_UNKNOWN_CMD, sizeof(SHELL_UNKNOWN_CMD))
#define SHELL_IFACE_TransmitPointer(Shell) Shell->Iface->Transmit(Shell->Iface->Priv, SHELL_POINTER, sizeof(SHELL_POINTER))

#define SHELL_CMD_HELP_NAME	"help"
#define SHELL_CMD_EXIT_NAME	"exit"
#define SHELL_CMD_RESET_NAME	"reset"

#define SHELL_CMD_HELP_HEADER						"Avaliable commands:\r"
#define SHELL_CMD_HELP_HELP_DESCRIPTION	"    "SHELL_CMD_HELP_NAME" - This help\r"
#define SHELL_CMD_HELP_EXIT_DESCRIPTION	"    "SHELL_CMD_EXIT_NAME" - Exit from this shell\r"
#define SHELL_CMD_HELP_RESET_DESCRIPTION	"    "SHELL_CMD_RESET_NAME" - Software reset\r"

typedef enum {
	SHELL_CMD_TYPE_EMPTY,
	SHELL_CMD_TYPE_UNKNOWN,
	SHELL_CMD_TYPE_HELP,
	SHELL_CMD_TYPE_EXIT,
	SHELL_CMD_TYPE_RESET,
	SHELL_CMD_TYPE_USER
} SHELL_CMD_TYPE_T;

static void SHELL_IFACE_TransmitHelp(SHELL_T *Shell)
{
	uint32_t i;
	
	SHELL_IFACE_Transmit(Shell, SHELL_CMD_HELP_HEADER, sizeof(SHELL_CMD_HELP_HEADER));
	SHELL_IFACE_Transmit(Shell, SHELL_CMD_HELP_HELP_DESCRIPTION, sizeof(SHELL_CMD_HELP_HELP_DESCRIPTION));
	SHELL_IFACE_Transmit(Shell, SHELL_CMD_HELP_EXIT_DESCRIPTION, sizeof(SHELL_CMD_HELP_EXIT_DESCRIPTION));
	SHELL_IFACE_Transmit(Shell, SHELL_CMD_HELP_RESET_DESCRIPTION, sizeof(SHELL_CMD_HELP_RESET_DESCRIPTION));
	for (i = 0; i < Shell->CmdListSize; i++)
	{
		strcpy(Shell->TxBuffer, "    ");
		strcat(Shell->TxBuffer, Shell->CmdList[i].Name);
		strcat(Shell->TxBuffer, " - ");
		strcat(Shell->TxBuffer, Shell->CmdList[i].Description);
		strcat(Shell->TxBuffer, "\r");
		SHELL_IFACE_Transmit(Shell, Shell->TxBuffer, strlen(Shell->TxBuffer));
	}
}

static int32_t SHELL_ParseCmd(SHELL_T *Shell)
{
	char *ptr;
	char *CmdParamsList[SHELL_CMD_PARAMS_MAX_CNT];
	uint32_t CmdParamsCnt;
	uint32_t i;
	
	memset(CmdParamsList, 0, sizeof(CmdParamsList));
	CmdParamsCnt = 0;
	ptr = Shell->RxBuffer;
	
	/*Parse command string*/
	do {
		if ((*ptr != ' ') && (*ptr != 0))
			CmdParamsList[CmdParamsCnt++] = ptr;	
		ptr = strchr(ptr, ' ');
		if (!ptr)
			break;
		*ptr = 0;
		ptr++;
	} while (CmdParamsCnt < SHELL_CMD_PARAMS_MAX_CNT);
	
	/* Check Empty command*/
	if (CmdParamsCnt == 0)
		return 0;
	
	/* Check Help command*/
	if (strcmp(CmdParamsList[0], SHELL_CMD_HELP_NAME) == 0)
	{
		SHELL_IFACE_TransmitHelp(Shell);
		return 0;
	}
	
	/* Check Exit command*/
	if (strcmp(CmdParamsList[0], SHELL_CMD_EXIT_NAME) == 0)
	{
		SHELL_IFACE_TransmitGoodBye(Shell);
		return 1;
	}
	
	/* Check Exit command*/
	if (strcmp(CmdParamsList[0], SHELL_CMD_RESET_NAME) == 0)
	{
		SHELL_IFACE_TransmitGoodBye(Shell);
		HAL_NVIC_SystemReset();
		return 1;
	}
	
	/* Search for command in list */
	for (i = 0; i < Shell->CmdListSize; i++)
	{
		if (strcmp(CmdParamsList[0], Shell->CmdList[i].Name) == 0)
		{
			/* Execute command callback*/
			Shell->CmdList[i].Callback(Shell, CmdParamsList, CmdParamsCnt);
			return 0;
		}
	}
	SHELL_IFACE_TransmitUnknownCmd(Shell);
	return 0;
}

static void SHELL_Thread(void const *argument)
{	
	SHELL_T *Shell = (SHELL_T*)argument;
	
	if (SHELL_IFACE_Init(Shell))
		return;
	
	while (1)
	{	
		if (SHELL_IFACE_Connect(Shell))
			continue;
		
		SHELL_IFACE_TransmitHello(Shell);
		SHELL_IFACE_TransmitPointer(Shell);
		while (1) {
			if (SHELL_IFACE_Receive(Shell, Shell->RxBuffer, sizeof(Shell->RxBuffer)))
				break;
			if (SHELL_ParseCmd(Shell))
				break;
			SHELL_IFACE_TransmitPointer(Shell);
		}	
		SHELL_IFACE_Disconnect(Shell);
	}
}

int32_t SHELL_Init(SHELL_T *Shell, char *Name, SHELL_CMD_T* CmdList, uint32_t CmdListSize, SHELL_IFACE_T *Iface)
{
	const osThreadDef_t ShellThreadDef = { Name, SHELL_Thread, SHELL_THREAD_PRIORITY, 0, SHELL_THREAD_STACK_SIZE};
	
	if ((!Shell) || (!CmdList) || (CmdListSize == 0) || (!Iface))
		return -1;
	if ((!Iface->Receive) || (!Iface->Transmit))
		return -1;
	Shell->CmdList = CmdList;
	Shell->CmdListSize = CmdListSize;
	Shell->Iface = Iface;
	Shell->ThreadId = osThreadCreate (&ShellThreadDef, Shell);
	if (Shell->ThreadId == NULL)
		return -1;
	return 0;
}

int32_t SHELL_Transmit(void *Shell, const char *str, ...)
{
	int32_t TxBufferLen;
	
	va_list va;
	va_start(va, str);
	TxBufferLen = vsnprintf(((SHELL_T*)Shell)->TxBuffer, sizeof(((SHELL_T*)Shell)->TxBuffer), str, va);
	va_end(va);
	if ((TxBufferLen < 0) || (TxBufferLen >= sizeof(((SHELL_T*)Shell)->TxBuffer)))
		return -1;
	if (SHELL_IFACE_Transmit((SHELL_T*)Shell, ((SHELL_T*)Shell)->TxBuffer, TxBufferLen))
		return -1;
  return 0;
}

int32_t SHELL_TransmitBuffer(void *Shell, char *TxBuffer, uint32_t TxBufferLen)
{
	return SHELL_IFACE_Transmit((SHELL_T*)Shell, TxBuffer, TxBufferLen);
}
