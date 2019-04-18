#include "shell_telnet.h"

#define SHELL_TELNET_PORT 23

#define SHELL_TELNET_RING_BUFFER_SIZE	(1 << 12) 

static char ring_buffer[SHELL_TELNET_RING_BUFFER_SIZE];
static uint32_t ring_buffer_ptr = 0;
static uint32_t ring_buffer_len = 0;

int32_t SHELL_TELNET_Init(void *Priv)
{
		SHELL_TELNET_PRIV_T *Telnet = Priv;
  err_t err;

  /* Create a new connection identifier. */
  Telnet->conn = netconn_new(NETCONN_TCP);  
  if (Telnet->conn != NULL)
  {  
    /* Bind connection to well known port number 23. */
    err = netconn_bind(Telnet->conn, NULL, SHELL_TELNET_PORT);    
    if (err == ERR_OK)
		{
			/* Tell connection to go into listening mode. */
      err = netconn_listen(Telnet->conn);
			if (err == ERR_OK)
			{
				return 0;
			}
		}
		netconn_delete(Telnet->conn);
	}
	return -1;
}

int32_t SHELL_TELNET_Connect(void *Priv)
{
	
	SHELL_TELNET_PRIV_T *Telnet = Priv;
  err_t err; 
	
	/* Grab new connection. */
	 err = netconn_accept(Telnet->conn, &Telnet->newconn);    
	/* Process the new connection. */
	if (err == ERR_OK) 
	{
		ring_buffer_ptr = 0;
		ring_buffer_len = 0;
		return 0;
	}
	return -1;
}

int32_t SHELL_TELNET_Receive(void *Priv, char *Buffer, uint32_t BufferSize)
{
	SHELL_TELNET_PRIV_T *Telnet = Priv;
	struct netbuf *buf;
	void *data;
	u16_t len;
	uint32_t i, j;
	
	i = 0;
	do {
		while (ring_buffer_len)
		{
			Buffer[i] = ring_buffer[ring_buffer_ptr & (SHELL_TELNET_RING_BUFFER_SIZE - 1)];
			ring_buffer_len--;
			ring_buffer_ptr++;
			if (Buffer[i] == '\r')
			{
				Buffer[i] = 0;
				return 0;
			}
			i++;
		}
		
		if (netconn_recv(Telnet->newconn, &buf) != ERR_OK)
			return -1;
		
		do {
			netbuf_data(buf, &data, &len);							
			for (j = 0; j < len; j++)
			{
				if (((char*)data)[j] == '\n')
					continue;
				ring_buffer[(ring_buffer_ptr + ring_buffer_len++) & (SHELL_TELNET_RING_BUFFER_SIZE - 1)] = ((char*)data)[j];
			}          
		} 
		while (netbuf_next(buf) >= 0);
		netbuf_delete(buf);
	} while (1);
	
	return 0;
}

int32_t SHELL_TELNET_Transmit(void *Priv, char *Buffer, uint32_t BufferSize)
{
	SHELL_TELNET_PRIV_T *Telnet = Priv;
	
	if (netconn_write(Telnet->newconn, Buffer, BufferSize, NETCONN_COPY) != ERR_OK)
			return -1;
	return 0;
}

int32_t SHELL_TELNET_Disconnect(void *Priv)
{
	SHELL_TELNET_PRIV_T *Telnet = Priv;
	
	netconn_close(Telnet->newconn);
	netconn_delete(Telnet->newconn);
	return 0;
}
