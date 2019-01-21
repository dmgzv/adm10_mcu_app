#include "profile_iface.h"
#include "profile_protocol.h"
#include "bsp.h"

static int32_t PROFILE_IFACE_ReceiveCommand(PROFILE_IFACE_T *Iface)
{
	char Byte;
	uint32_t BytesCnt;
	
	BytesCnt = 0;
	do {
		if (UART_Receive(Iface->Uart, &Byte, 1))
			return -1;
		if (BytesCnt == 0)
		{
			if (Byte == Iface->StartByte)
				Iface->Command[BytesCnt++] = Byte;
		}
		else
		{
			if (Byte == Iface->StartByte)
			{
				BytesCnt = 0;
				Iface->Command[BytesCnt++] = Byte;
			}
			else if (Byte == PROFILE_IFACE_STOP_BYTE)
			{
				Iface->Command[BytesCnt++] = Byte;
				break;
			}
			else
			{
				Iface->Command[BytesCnt++] = Byte;
			}
		}
		
		if (BytesCnt >= sizeof(Iface->Command))
			BytesCnt = 0;
		
	} while(1);
	Iface->CommandLen = BytesCnt;
	return 0;
}
static int32_t PROFILE_IFACE_TransmitResponce(PROFILE_IFACE_T *Iface)
{
	if (UART_Transmit(Iface->Uart, Iface->Responce, Iface->ResponceLen))
		return -1;
	return 0;
}

static void ProfileIface_Thread(void const *argument)
{
	PROFILE_IFACE_T *Iface = (PROFILE_IFACE_T*)argument;
	uint32_t MaxResponceLen = 0;
	uint32_t ParseTimeMax = 0, TransmitTimeMax = 0;
	uint32_t TimeStart, TimeEnd;
	uint32_t Complete;
	
  for(;;)
  {
		if (PROFILE_IFACE_ReceiveCommand(Iface))
			continue;
		
		TimeStart = osKernelSysTick();
		Complete = 0;
		
		Iface->ResponceLen = PROFILE_PROTOCOL_Command(Iface->Profile, Iface->Command, Iface->Responce, &Complete);		
		
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
  }
}

int32_t PROFILE_IFACE_Init(PROFILE_IFACE_T *Iface, char *Name, USART_TypeDef *Uart, uint8_t SlotAddress, PROFILE_T *Profile)
{
	osThreadDef_t ProfileFlash_osThreadDef = { Name, ProfileIface_Thread, osPriorityHigh, 0, 1024};
	if (SlotAddress > 0x0F)
		return -1;
	Iface->StartByte = 0x80 + SlotAddress;
	Iface->Uart = Uart;
	Iface->Profile = Profile;
	Iface->ThreadId = osThreadCreate (&ProfileFlash_osThreadDef, Iface);
	return 0;
}
