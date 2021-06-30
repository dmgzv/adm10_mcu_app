#include "at45db641e.h"
#include "log.h"

#define AT45DB641E_PAGE_PROGRAM_TIMEOUT	(35)
#define AT45DB641E_PAGE_ERASE_TIMEOUT		(32)
#define AT45DB641E_BLOCK_ERASE_TIMEOUT	(75)
#define AT45DB641E_SECTOR_ERASE_TIMEOUT	(5000)

#define AT45DB641E_STATUS_PAGE_SIZE_MASK 	(1 << 0)
#define AT45DB641E_STATUS_PROTECT_MASK 		(1 << 1)
#define AT45DB641E_STATUS_COMP_MASK 			(1 << 6)
#define AT45DB641E_STATUS_READY_MASK 			(1 << 7)

#define AT45DB641E_InitPin(Dev, Pin)							((Dev)->PinCtrl ? BSP_PIN_CTRL_Init((Dev)->PinCtrl, Pin) : 0)	
#define AT45DB641E_SetPinState(Dev, Pin, State)	((Dev)->PinCtrl ? BSP_PIN_CTRL_SetState((Dev)->PinCtrl, Pin, State) : 0)

#ifdef BSP_CONFIG_USE_FREERTOS
#define	AT45DB641E_msDelay(ms) 		osDelay(ms)
#else
#define	AT45DB641E_msDelay(ms) 		HAL_Delay(ms)
#endif

static void AT45DB641E_SetCS(void *Priv, uint32_t State)
{
	AT45DB641E_T *Dev = Priv;
	AT45DB641E_SetPinState(Dev, AT45DB641E_PIN_SPI_CS, State);
}

static int32_t AT45DB641E_ReadId(AT45DB641E_T *Dev)
{
	char data[] = {0x9F, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	result = SPI_DMA_TransmitReceive(Dev->SPI, &Dev->CS, data, data, sizeof(data));
	if (result)
		return -1;
	Dev->ManufId = data[1];
	Dev->DeviceId = data[2] << 8 | data[3];
	/*Check Id's*/
	if ((Dev->ManufId != 0x1F) || (Dev->DeviceId != 0x2800))
		return -1;
	return 0;
}


static int32_t AT45DB641E_ReadStatus(AT45DB641E_T *Dev)
{
	char data[] = {0xD7, 0xFF, 0xFF};
	int32_t result;
	
	result = SPI_DMA_TransmitReceive(Dev->SPI, &Dev->CS, data, data, sizeof(data));
	if (result)
		return -1;
	Dev->Status = data[1] | (data[2] << 8);
	return 0;
}

static int32_t AT45DB641E_SetBinaryPageSize(AT45DB641E_T *Dev)
{
	char data[] = {0x3D, 0x2A, 0x80, 0xA6};
	int32_t result;
	
	result = SPI_DMA_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		return -1;
	return 0;
}

static int32_t __AT45DB641E_ProgramPage(AT45DB641E_T *Dev, uint32_t Addr, char *Buffer, uint32_t Size)
{
	char data[] = {0x82, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_DMA_SequentialTransmit(Dev->SPI, &Dev->CS, data, sizeof(data), Buffer, Size);
	if (result)
		return -1;
	return 0;
}

static int32_t __AT45DB641E_WaitReady(AT45DB641E_T *Dev)
{
	while (1)
	{
		if (AT45DB641E_ReadStatus(Dev))
			goto error;
		if ((Dev->Status & (AT45DB641E_STATUS_READY_MASK << 8)) && (Dev->Status & (AT45DB641E_STATUS_READY_MASK << 0)))
			break;
		AT45DB641E_msDelay(1);
	}
	return 0;
	
error:
	LOG_Error(Dev->DevName, "", "WaitReadyError", "");
	return -1;
}

int32_t AT45DB641E_ErasePage(AT45DB641E_T *Dev, uint32_t Addr)
{
	char data[] = {0x81, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__AT45DB641E_WaitReady(Dev))
		goto error;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_DMA_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	LOG_Error(Dev->DevName, "", "PageEraseError", "");
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;	
}

int32_t AT45DB641E_WaitReady(AT45DB641E_T *Dev)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__AT45DB641E_WaitReady(Dev))
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t AT45DB641E_EraseBlock(AT45DB641E_T *Dev, uint32_t Addr)
{
	char data[] = {0x50, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if ((!Dev) || (Addr > AT45DB641E_SIZE))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__AT45DB641E_WaitReady(Dev))
		goto error;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_DMA_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		goto error;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	LOG_Error(Dev->DevName, "", "BlockEraseError", "");
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;	
}

int32_t AT45DB641E_EraseSector(AT45DB641E_T *Dev, uint32_t Addr)
{
	char data[] = {0x7C, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if ((!Dev) || (Addr > AT45DB641E_SIZE))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__AT45DB641E_WaitReady(Dev))
		goto error;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_DMA_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	LOG_Error(Dev->DevName, "", "SectorEraseError", "");
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;	
}

int32_t AT45DB641E_Erase(AT45DB641E_T *Dev, uint32_t Addr)
{
	char data[] = {0xC7, 0x94, 0x80, 0x9A};
	int32_t result;
	
	if ((!Dev) || (Addr > AT45DB641E_SIZE))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__AT45DB641E_WaitReady(Dev))
		goto error;
	
	result = SPI_DMA_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	LOG_Error(Dev->DevName, "", "EraseError", "");
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t AT45DB641E_Read(AT45DB641E_T *Dev, uint32_t Addr, char *Buffer, uint32_t Size)
{
	char data[] = {0x03, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if ((!Dev) || (Addr > AT45DB641E_SIZE) || ((Addr + Size) > AT45DB641E_SIZE))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__AT45DB641E_WaitReady(Dev))
		goto error;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_DMA_SequentialReceive(Dev->SPI, &Dev->CS, data, sizeof(data), Buffer, Size);
	if (result)
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	LOG_Error(Dev->DevName, "", "ReadError", "");
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t AT45DB641E_ProgramPage(AT45DB641E_T *Dev, uint32_t Addr, char *Buffer, uint32_t Size)
{	
	if ((!Dev) || (Addr > AT45DB641E_SIZE) || ((Addr + Size) > AT45DB641E_SIZE))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__AT45DB641E_WaitReady(Dev))
		goto error;

	if (__AT45DB641E_ProgramPage(Dev, Addr, Buffer, Size))
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	LOG_Error(Dev->DevName, "", "PageProgramError", "");
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t AT45DB641E_ModifyPage(AT45DB641E_T *Dev, uint32_t Addr, char *Buffer, uint32_t Size)
{
	char data[] = {0x53, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if ((!Dev) || (Addr > AT45DB641E_SIZE) || ((Addr + Size) > AT45DB641E_SIZE))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__AT45DB641E_WaitReady(Dev))
		goto error;
		
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_DMA_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		goto error;
	AT45DB641E_msDelay(1);
	result = AT45DB641E_ReadStatus(Dev);
	if ((Dev->Status & AT45DB641E_STATUS_READY_MASK) == 0)
		goto error;	
	if (__AT45DB641E_ProgramPage(Dev, Addr, Buffer, Size))
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	LOG_Error(Dev->DevName, "", "PageModifyError", "");
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}


int32_t AT45DB641E_Init(AT45DB641E_T *Dev)
{	
	if ((!Dev) || (!Dev->SPI) || (!Dev->PinCtrl))
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Dev->CS.SetState = &AT45DB641E_SetCS;
	Dev->CS.Priv = Dev;
	
	
	if (AT45DB641E_InitPin(Dev, AT45DB641E_PIN_SPI_CS))
		goto error;
	if (AT45DB641E_InitPin(Dev, AT45DB641E_PIN_WP))
		goto error;
	if (AT45DB641E_InitPin(Dev, AT45DB641E_PIN_RESET))
		goto error;
	
	if (AT45DB641E_ReadId(Dev) != 0)
		goto error;	
	if (AT45DB641E_ReadStatus(Dev) != 0)
		goto error;	
	if ((Dev->Status & AT45DB641E_STATUS_PAGE_SIZE_MASK) == 0)
	{
		if (AT45DB641E_SetBinaryPageSize(Dev) != 0)
			goto error;	
		AT45DB641E_msDelay(20);
		AT45DB641E_SetPinState(Dev, AT45DB641E_PIN_RESET, 0);
		AT45DB641E_msDelay(20);
		AT45DB641E_SetPinState(Dev, AT45DB641E_PIN_RESET, 1);
		AT45DB641E_msDelay(20);
	}
	if (AT45DB641E_ReadStatus(Dev) != 0)
		goto error;	
	if ((Dev->Status & AT45DB641E_STATUS_PAGE_SIZE_MASK) == 0)
		goto error;	
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}
