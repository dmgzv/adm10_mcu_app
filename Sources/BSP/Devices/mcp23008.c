#include "mcp23008.h"
#include "log.h"

static const uint8_t mcp23008_reg_iodir = 0x00;
static const uint8_t mcp23008_reg_ipol = 0x01;
static const uint8_t mcp23008_reg_gpio = 0x09;

int32_t MCP23008_GetIODIR(MCP23008_T *Dev)
{
	uint8_t IODIR;
	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&mcp23008_reg_iodir, sizeof(mcp23008_reg_iodir), (char*)&IODIR, sizeof(IODIR)))
		goto error;	
	Dev->IODIR = IODIR;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t MCP23008_SetIODIR(MCP23008_T *Dev, uint8_t IODIR)
{	
	if ((!Dev))
		return -1;		
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->IODIR != IODIR)
	{
		LOG_Info(Dev->DevName, "", "IODIRWrite", "0x%02x;0x%02x", mcp23008_reg_iodir, IODIR);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, (uint8_t*)&mcp23008_reg_iodir, sizeof(mcp23008_reg_iodir), (char*)&IODIR, sizeof(IODIR)))
			goto error;
		Dev->IODIR = IODIR;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t MCP23008_GetGPIO(MCP23008_T *Dev)
{
	uint8_t GPIO;
	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&mcp23008_reg_gpio, sizeof(mcp23008_reg_gpio), (char*)&GPIO, sizeof(GPIO)))
		goto error;	
	Dev->GPIO = GPIO;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t MCP23008_SetGPIO(MCP23008_T *Dev, uint8_t GPIO)
{	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->GPIO != GPIO)
	{
		LOG_Info(Dev->DevName, "", "GPIOWrite", "0x%02x;0x%02x", mcp23008_reg_gpio, GPIO);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, (uint8_t*)&mcp23008_reg_gpio, sizeof(mcp23008_reg_gpio), (char*)&GPIO, sizeof(GPIO)))
			goto error;
		Dev->GPIO = GPIO;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t MCP23008_GetIPOL(MCP23008_T *Dev)
{
	uint8_t pol;
	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&mcp23008_reg_ipol, sizeof(mcp23008_reg_ipol), (char*)&pol, sizeof(pol)))
		goto error;	
	Dev->IPOL = pol;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t MCP23008_SetIPOL(MCP23008_T *Dev, uint8_t IPOL)
{	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->IPOL != IPOL)
	{
		LOG_Info(Dev->DevName, "", "IPOLWrite", "0x%02x;0x%02x", mcp23008_reg_ipol, IPOL);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, (uint8_t*)&mcp23008_reg_ipol, sizeof(mcp23008_reg_ipol), (char*)&IPOL, sizeof(IPOL)))
			goto error;
		Dev->IPOL = IPOL;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t MCP23008_Init(MCP23008_T *Dev)
{
	uint8_t	GPIO;
	uint8_t IPOL;
	uint8_t IODIR;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
			
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&mcp23008_reg_gpio, 1, (char*)&GPIO, 1))
		goto error;		
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&mcp23008_reg_ipol, 1, (char*)&IPOL, 1))
		goto error;	
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&mcp23008_reg_iodir, 1, (char*)&IODIR, 1))
		goto error;

	Dev->GPIO = GPIO;
	Dev->IODIR = IODIR;
	Dev->IPOL = IPOL;
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void MCP23008_Deinit(MCP23008_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
