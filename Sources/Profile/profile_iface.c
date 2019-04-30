#include "profile_iface.h"
#include "profile_protocol.h"
#include "bsp.h"

#define PROFILE_IFACE_TransmitResponce(Iface) UART_Transmit((Iface)->Uart, (Iface)->Responce, (Iface)->ResponceLen)
#define PROFILE_IFACE_ReceiveCommand(Iface, Idx) UART_ReceiveCommand((Iface)->Uart, (Iface)->Commands[(Idx)], sizeof((Iface)->Commands[(Idx)]), PROFILE_IFACE_STOP_BYTE, &(Iface)->CommandsLen[(Idx)], osWaitForever)

static void PROFILE_IFACE_ReceiveThread(void const *argument)
{
	PROFILE_IFACE_T *Iface = (PROFILE_IFACE_T*)argument;
	osEvent Event;
	
  for(;;)
  {
		while (1)
		{
			Event = osMessageGet(Iface->ReceiveQId, osWaitForever);
			if (Event.status == osEventMessage)
				break;
		}

		while (1)
		{
			if (PROFILE_IFACE_ReceiveCommand(Iface, Event.value.v))
				continue;
			
			if (Iface->Commands[Event.value.v][0] == Iface->StartByte)
				break;
		}
		
		while (1)
		{
			if (osMessagePut(Iface->ProcessQId, Event.value.v, 0) == osOK)
				break;
		}
  }
}

static void PROFILE_IFACE_ProcessThread(void const *argument)
{
	PROFILE_IFACE_T *Iface = (PROFILE_IFACE_T*)argument;
	uint32_t MaxResponceLen = 0;
	uint32_t ParseTimeMax = 0, TransmitTimeMax = 0;
	uint32_t TimeStart, TimeEnd;
	uint32_t Complete;
	osEvent Event;
	
  for(;;)
  {
		
		Event = osMessageGet(Iface->ProcessQId, osWaitForever);
		if (Event.status != osEventMessage)
			continue;
		
		TimeStart = osKernelSysTick();
		Complete = 0;
		
		Iface->ResponceLen = PROFILE_PROTOCOL_Command(Iface->Profile, Iface->Commands[Event.value.v], Iface->Responce, &Complete);		
		
		TimeEnd = osKernelSysTick();		
		if (ParseTimeMax < (TimeEnd - TimeStart))
			ParseTimeMax = TimeEnd - TimeStart;
		
		if (MaxResponceLen < Iface->ResponceLen)
			MaxResponceLen = Iface->ResponceLen;
		
		TimeStart = osKernelSysTick();
		if (Iface->ResponceLen)
			PROFILE_IFACE_TransmitResponce(Iface);
		
		TimeEnd = osKernelSysTick();
		
		if (TransmitTimeMax < (TimeEnd - TimeStart))
			TransmitTimeMax = TimeEnd - TimeStart;
		
		if (Complete)
		{
			BSP_Reset();
		}

		while (1)
		{
			if (osMessagePut(Iface->ReceiveQId, Event.value.v, 0) == osOK)
				break;
		}
  }
}

int32_t PROFILE_IFACE_Init(PROFILE_IFACE_T *Iface, char *ReceiveThreadName, char *ProcessThreadName, USART_TypeDef *Uart, uint8_t SlotAddress, PROFILE_T *Profile)
{
	osThreadDef_t ReceiveThreadDef = { ReceiveThreadName, PROFILE_IFACE_ReceiveThread, osPriorityHigh, 0, 1024};
	osThreadDef_t ProcessThreadDef = { ProcessThreadName, PROFILE_IFACE_ProcessThread, osPriorityHigh, 0, 1024};
	osMessageQDef(Receive, PROFILE_IFACE_CMD_QUEUE_SIZE, uint32_t);
	osMessageQDef(Process, PROFILE_IFACE_CMD_QUEUE_SIZE, uint32_t);
	uint32_t i;	
	
	Iface->StartByte = 0x80 + SlotAddress;
	Iface->Uart = Uart;
	Iface->Profile = Profile;
	Iface->ReceiveQId = osMessageCreate (osMessageQ(Receive), NULL);
	if (!Iface->ReceiveQId)
		return -1;
	for (i = 0; i < PROFILE_IFACE_CMD_QUEUE_SIZE; i++)
	{
		osMessagePut(Iface->ReceiveQId, i, 0);
	}
	Iface->ProcessQId = osMessageCreate (osMessageQ(Process), NULL);
	if (!Iface->ProcessQId)
		return -1;
	Iface->ReceiveThreadId = osThreadCreate (&ReceiveThreadDef, Iface);
	if (!Iface->ReceiveThreadId)
		return -1;
	Iface->ProcessThreadId = osThreadCreate (&ProcessThreadDef, Iface);
	if (!Iface->ProcessThreadId)
		return -1;
	return 0;
}
