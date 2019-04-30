#include "at45db161e.h"

#define AT45DB161E_PAGE_PROGRAM_TIMEOUT	(35)
#define AT45DB161E_PAGE_ERASE_TIMEOUT		(32)
#define AT45DB161E_BLOCK_ERASE_TIMEOUT	(75)
#define AT45DB161E_SECTOR_ERASE_TIMEOUT	(5000)

#define AT45DB161E_STATUS_PAGE_SIZE_MASK 	(1 << 0)
#define AT45DB161E_STATUS_PROTECT_MASK 		(1 << 1)
#define AT45DB161E_STATUS_COMP_MASK 			(1 << 6)
#define AT45DB161E_STATUS_READY_MASK 			(1 << 7)

#ifdef BSP_CONFIG_USE_FREERTOS
#define	AT45DB161E_msDelay(ms) 		osDelay(ms)
#else
#define	AT45DB161E_msDelay(ms) 		HAL_Delay(ms)
#endif

static void AT45DB161E_SetCS(void *Priv, uint32_t State)
{
	AT45DB161E_T *Dev = Priv;
	Dev->SetPinState(Dev->Priv, AT45DB161E_PIN_SPI_CS, State? AT45DB161E_PIN_STATE_ASSERT : AT45DB161E_PIN_STATE_DEASSERT);
}

static int32_t AT45DB161E_ReadId(AT45DB161E_T *Dev)
{
	char data[] = {0x9F, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	result = SPI_TransmitReceive(Dev->SPI, &Dev->CS, data, data, sizeof(data));
	if (result)
		return -1;
	Dev->ManufId = data[1];
	Dev->DevId = data[2] << 8 | data[3];
	/*Check Id's*/
	if ((Dev->ManufId != 0x1F) || (Dev->DevId != 0x2600))
		return -1;
	return 0;
}


static int32_t AT45DB161E_ReadStatus(AT45DB161E_T *Dev)
{
	char data[] = {0xD7, 0xFF};
	int32_t result;
	
	result = SPI_TransmitReceive(Dev->SPI, &Dev->CS, data, data, sizeof(data));
	if (result)
		return -1;
	Dev->Status = data[1];
	return 0;
}

static int32_t AT45DB161E_SetBinaryPageSize(AT45DB161E_T *Dev)
{
	char data[] = {0x3D, 0x2A, 0x80, 0xA6};
	int32_t result;
	
	result = SPI_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		return -1;
	return 0;
}

static int32_t __AT45DB161E_ProgramPage(AT45DB161E_T *Dev, uint32_t Addr, char *Buffer, uint32_t Size)
{
	char data[] = {0x82, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_SequentialTransmit(Dev->SPI, &Dev->CS, data, sizeof(data), Buffer, Size);
	if (result)
		return -1;
	return 0;
}

int32_t AT45DB161E_ErasePage(AT45DB161E_T *Dev, uint32_t Addr)
{
	char data[] = {0x81, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;	
}

int32_t AT45DB161E_WaitReady(AT45DB161E_T *Dev)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	while (1)
	{
		if (AT45DB161E_ReadStatus(Dev))
			goto error;	
		if ( Dev->Status & AT45DB161E_STATUS_READY_MASK )
			break;
		AT45DB161E_msDelay(1);
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t AT45DB161E_EraseBlock(AT45DB161E_T *Dev, uint32_t Addr)
{
	char data[] = {0x50, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if ((!Dev) || (Addr > AT45DB161E_SIZE))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		goto error;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;	
}

int32_t AT45DB161E_EraseSector(AT45DB161E_T *Dev, uint32_t Addr)
{
	char data[] = {0x7C, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if ((!Dev) || (Addr > AT45DB161E_SIZE))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;	
}

int32_t AT45DB161E_Erase(AT45DB161E_T *Dev, uint32_t Addr)
{
	char data[] = {0xC7, 0x94, 0x80, 0x9A};
	int32_t result;
	
	if ((!Dev) || (Addr > AT45DB161E_SIZE))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	result = SPI_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t AT45DB161E_Read(AT45DB161E_T *Dev, uint32_t Addr, char *Buffer, uint32_t Size)
{
	char data[] = {0x03, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if ((!Dev) || (Addr > AT45DB161E_SIZE) || ((Addr + Size) > AT45DB161E_SIZE))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_SequentialReceive(Dev->SPI, &Dev->CS, data, sizeof(data), Buffer, Size);
	if (result)
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t AT45DB161E_ProgramPage(AT45DB161E_T *Dev, uint32_t Addr, char *Buffer, uint32_t Size)
{	
	if ((!Dev) || (Addr > AT45DB161E_SIZE) || ((Addr + Size) > AT45DB161E_SIZE))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);

	if (__AT45DB161E_ProgramPage(Dev, Addr, Buffer, Size))
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t AT45DB161E_ModifyPage(AT45DB161E_T *Dev, uint32_t Addr, char *Buffer, uint32_t Size)
{
	char data[] = {0x53, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if ((!Dev) || (Addr > AT45DB161E_SIZE) || ((Addr + Size) > AT45DB161E_SIZE))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
		
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	result = SPI_Transmit(Dev->SPI, &Dev->CS, data, sizeof(data));
	if (result)
		goto error;
	AT45DB161E_msDelay(1);
	result = AT45DB161E_ReadStatus(Dev);
	if ((Dev->Status & AT45DB161E_STATUS_READY_MASK) == 0)
		goto error;	
	if (__AT45DB161E_ProgramPage(Dev, Addr, Buffer, Size))
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}


int32_t AT45DB161E_Init(AT45DB161E_T *Dev, SPI_TypeDef *SPI, void (*InitPins)(void *Priv),\
	void (*SetPinState)(void *Priv, AT45DB161E_PIN_T Pin, AT45DB161E_PIN_STATE_T PinState), void *Priv)
{	
	if ((!Dev) || (!SPI) || (!InitPins) || (!SetPinState))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->SPI = SPI;
	Dev->InitPins = InitPins;
	Dev->SetPinState = SetPinState;
	Dev->Priv = Priv;
	Dev->CS.Priv = Dev;
	Dev->CS.SetState = &AT45DB161E_SetCS;
	
	Dev->InitPins(Dev->Priv);
	if (AT45DB161E_ReadId(Dev) != 0)
		goto error;	
	if (AT45DB161E_ReadStatus(Dev) != 0)
		goto error;	
	if ((Dev->Status & AT45DB161E_STATUS_PAGE_SIZE_MASK) == 0)
	{
		if (AT45DB161E_SetBinaryPageSize(Dev) != 0)
			goto error;	
		AT45DB161E_msDelay(20);
		Dev->SetPinState(Dev->Priv, AT45DB161E_PIN_RESET, AT45DB161E_PIN_STATE_DEASSERT);
		AT45DB161E_msDelay(20);
		Dev->SetPinState(Dev->Priv, AT45DB161E_PIN_RESET, AT45DB161E_PIN_STATE_ASSERT);
		AT45DB161E_msDelay(20);
	}
	if (AT45DB161E_ReadStatus(Dev) != 0)
		goto error;	
	if ((Dev->Status & AT45DB161E_STATUS_PAGE_SIZE_MASK) == 0)
		goto error;	
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}
