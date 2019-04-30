#include "at45db081d.h"

#define AT45DB081D_PAGE_PROGRAM_TIMEOUT	(35)
#define AT45DB081D_PAGE_ERASE_TIMEOUT		(32)
#define AT45DB081D_BLOCK_ERASE_TIMEOUT	(75)
#define AT45DB081D_SECTOR_ERASE_TIMEOUT	(5000)

#define AT45DB081D_STATUS_PAGE_SIZE_MASK 	(1 << 0)
#define AT45DB081D_STATUS_PROTECT_MASK 		(1 << 1)
#define AT45DB081D_STATUS_COMP_MASK 			(1 << 6)
#define AT45DB081D_STATUS_READY_MASK 			(1 << 7)

#ifdef BSP_CONFIG_USE_FREERTOS
#define	AT45DB081D_msDelay(ms) 		osDelay(ms)
#else
#define	AT45DB081D_msDelay(ms) 		HAL_Delay(ms)
#endif

static int32_t AT45DB081D_ReadId(AT45DB081D_T *dev)
{
	char data[] = {0x9F, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_DEASSERT);
	result = SPI_TransmitReceive(dev->SPI, data, data, sizeof(data));
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
	if (result)
		return -1;
	dev->ManufId = data[1];
	dev->DevId = data[2] << 8 | data[3];
	/*Check Id's*/
	if ((dev->ManufId != 0x1F) || (dev->DevId != 0x2500))
		return -1;
	return 0;
}


static int32_t AT45DB081D_ReadStatus(AT45DB081D_T *dev)
{
	char data[] = {0xD7, 0xFF};
	int32_t result;
	
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_DEASSERT);
	result = SPI_TransmitReceive(dev->SPI, data, data, sizeof(data));
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
	if (result)
		return -1;
	dev->Status = data[1];
	return 0;
}

static int32_t AT45DB081D_SetBinaryPageSize(AT45DB081D_T *dev)
{
	char data[] = {0x3D, 0x2A, 0x80, 0xA6};
	int32_t result;
	
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_DEASSERT);
	result = SPI_Transmit(dev->SPI, data, sizeof(data));
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
	if (result)
		return -1;
	return 0;
}

int32_t AT45DB081D_ErasePage(AT45DB081D_T *dev, uint32_t Addr)
{
	char data[] = {0x81, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if (Addr > AT45DB081D_SIZE)
		return -1;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_DEASSERT);
	result = SPI_Transmit(dev->SPI, data, sizeof(data));
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
	if (result)
		return -1;
	return 0;	
}

int32_t AT45DB081D_WaitReady(AT45DB081D_T *dev)
{
	while (1)
	{
		if (AT45DB081D_ReadStatus(dev))
			return -1;	
		if ( dev->Status & AT45DB081D_STATUS_READY_MASK )
			break;
		AT45DB081D_msDelay(5);
	}
	return 0;
}

int32_t AT45DB081D_EraseBlock(AT45DB081D_T *dev, uint32_t Addr)
{
	char data[] = {0x50, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if (Addr > AT45DB081D_SIZE)
		return -1;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_DEASSERT);
	result = SPI_Transmit(dev->SPI, data, sizeof(data));
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
	if (result)
		return -1;	
	return 0;	
}

int32_t AT45DB081D_EraseSector(AT45DB081D_T *dev, uint32_t Addr)
{
	char data[] = {0x7C, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if (Addr > AT45DB081D_SIZE)
		return -1;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_DEASSERT);
	result = SPI_Transmit(dev->SPI, data, sizeof(data));
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
	if (result)
		return -1;
	return 0;	
}

int32_t AT45DB081D_Erase(AT45DB081D_T *dev, uint32_t Addr)
{
	char data[] = {0xC7, 0x94, 0x80, 0x9A};
	int32_t result;
	
	if (Addr > AT45DB081D_SIZE)
		return -1;
	
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_DEASSERT);
	result = SPI_Transmit(dev->SPI, data, sizeof(data));
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
	if (result)
		return -1;
	return 0;	
}

int32_t AT45DB081D_Read(AT45DB081D_T *dev, uint32_t Addr, char *Buffer, uint32_t Size)
{
	char data[] = {0x03, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if ((Addr > AT45DB081D_SIZE) || ((Addr + Size) > AT45DB081D_SIZE))
		return -1;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_DEASSERT);
	result = SPI_Transmit(dev->SPI, data, sizeof(data));
	if (result)
	{
		dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
		return -1;
	}
	result = SPI_Receive(dev->SPI, Buffer, Size);
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
	if (result)
		return -1;
	return 0;	
}

int32_t AT45DB081D_ProgramPage(AT45DB081D_T *dev, uint32_t Addr, char *Buffer, uint32_t Size)
{
	char data[] = {0x82, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if ((Addr > AT45DB081D_SIZE) || ((Addr + Size) > AT45DB081D_SIZE))
		return -1;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_DEASSERT);
	result = SPI_Transmit(dev->SPI, data, sizeof(data));
	if (result)
	{
		dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
		return -1;
	}
	result = SPI_Transmit(dev->SPI, Buffer, Size);
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
	if (result)
		return -1;
	return 0;	
}

int32_t AT45DB081D_ModifyPage(AT45DB081D_T *dev, uint32_t Addr, char *Buffer, uint32_t Size)
{
	char data[] = {0x53, 0xFF, 0xFF, 0xFF};
	int32_t result;
	
	if ((Addr > AT45DB081D_SIZE) || ((Addr + Size) > AT45DB081D_SIZE))
		return -1;
	
	data[3] = Addr;
	data[2]	= Addr >> 8;
	data[1] = Addr >> 16;
	
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_DEASSERT);
	result = SPI_Transmit(dev->SPI, data, sizeof(data));
	dev->SetPinState(dev->Priv, AT45DB081D_PIN_SPI_CS, AT45DB081D_PIN_STATE_ASSERT);
	if (result)
		return -1;
	AT45DB081D_msDelay(1);
	result = AT45DB081D_ReadStatus(dev);
	if ((dev->Status & AT45DB081D_STATUS_READY_MASK) == 0)
		return -1;	
	return AT45DB081D_ProgramPage(dev, Addr, Buffer, Size);	
}


int32_t AT45DB081D_Init(AT45DB081D_T *dev, SPI_TypeDef *SPI, void (*InitPins)(void *Priv),\
	void (*SetPinState)(void *Priv, AT45DB081D_PIN_T Pin, AT45DB081D_PIN_STATE_T PinState), void *Priv)
{	
	if ((!dev) || (!SPI) || (!InitPins) || (!SetPinState))
		return -1;
	dev->SPI = SPI;
	dev->InitPins = InitPins;
	dev->SetPinState = SetPinState;
	dev->Priv = Priv;
	
	dev->InitPins(dev->Priv);
	if (AT45DB081D_ReadId(dev) != 0)
		return -1;
	if (AT45DB081D_ReadStatus(dev) != 0)
		return -1;
	if ((dev->Status & AT45DB081D_STATUS_PAGE_SIZE_MASK) == 0)
	{
		if (AT45DB081D_SetBinaryPageSize(dev) != 0)
			return -1;
		AT45DB081D_msDelay(20);
		dev->SetPinState(dev->Priv, AT45DB081D_PIN_RESET, AT45DB081D_PIN_STATE_DEASSERT);
		AT45DB081D_msDelay(20);
		dev->SetPinState(dev->Priv, AT45DB081D_PIN_RESET, AT45DB081D_PIN_STATE_ASSERT);
		AT45DB081D_msDelay(20);
	}
	if (AT45DB081D_ReadStatus(dev) != 0)
		return -1;
	if ((dev->Status & AT45DB081D_STATUS_PAGE_SIZE_MASK) == 0)
		return -1;	
	return 0;
}
