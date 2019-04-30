#include "shell_uart.h"

int32_t SHELL_UART_Receive(void *Priv, char *Buffer, uint32_t BufferSize)
{
	USART_TypeDef *uart = Priv;
#if 1
	uint32_t Len;
	
	if (UART_ReceiveCommand(uart, Buffer, BufferSize, '\r',  &Len, osWaitForever))
		return -1;
	Buffer[Len - 1] = 0;
#else	
	char Byte;
	uint32_t BytesCnt;
	
	BytesCnt = 0;
	while(1)
	{
		if (UART_Receive(uart, &Byte, 1))
			continue;

		Buffer[BytesCnt] = 0;			
		if (Byte == '\r')
			break;		
		Buffer[BytesCnt++] = Byte;		
		if (BytesCnt >= BufferSize)	
			return -1;
	};
#endif
	return 0;
}

int32_t SHELL_UART_Transmit(void *Priv, char *Buffer, uint32_t BufferSize)
{
	USART_TypeDef *uart = Priv;
	
	if (UART_Transmit(uart, Buffer, BufferSize))
			return -1;
	return 0;
}

