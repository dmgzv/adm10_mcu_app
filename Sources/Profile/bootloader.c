#include "bootloader.h"
#include "crc.h"
#include <string.h>

#define BOOTLOADER_FW_UPDATE_FLAG	0x12345678

static int32_t BOOTLOADER_CheckFw(BOOTLOADER_T *Bootloader, uint32_t *FwSize, uint16_t *FwCRC16)
{
	uint32_t ReadFwSize;
	uint16_t ReadFwCRC16, CheckFwCRC16;
	
	/*Check FW Size*/
	ReadFwSize = *(uint32_t*)(BOOTLOADER_FW_ADDR + BOOTLOADER_FW_SIZE - sizeof(Bootloader->FwCRC16) - sizeof(Bootloader->FwSize));	
	if ((ReadFwSize >= BOOTLOADER_FW_SIZE) || (ReadFwSize == 0))
		return -1;
	/*Check FW CRC16*/
	ReadFwCRC16 = *(uint16_t*)(BOOTLOADER_FW_ADDR + BOOTLOADER_FW_SIZE - sizeof(Bootloader->FwCRC16));
	CheckFwCRC16 = Calc_CRC16((char*)BOOTLOADER_FW_ADDR, ReadFwSize, 0);
	if (CheckFwCRC16 != ReadFwCRC16)
		return -1;
	*FwSize = ReadFwSize;
	*FwCRC16 = ReadFwCRC16;
	return 0;
}

static int32_t BOOTLOADER_CheckNewFw(BOOTLOADER_T *Bootloader, uint32_t *FwSize, uint16_t *FwCRC16)
{
	uint32_t ReadFwSize, FwUpdateFlag;
	uint32_t FlashFwAddr, i, ReadSize, Size;
	uint16_t ReadFwCRC16, CheckFwCRC16;
	
	/*Check FW Update Flag*/
	FlashFwAddr = BOOTLOADER_FLASH_FW_ADDR + BOOTLOADER_FLASH_FW_SIZE  - sizeof(Bootloader->FwCRC16) - sizeof(Bootloader->FwSize) - sizeof(uint32_t);;
	if (BSP_FLASH_Read(FlashFwAddr, (char*)&FwUpdateFlag, sizeof(FwUpdateFlag)))
		return -1;
	if (FwUpdateFlag != BOOTLOADER_FW_UPDATE_FLAG)
		return -1;
	/*Check FW Size*/
	FlashFwAddr = BOOTLOADER_FLASH_FW_ADDR + BOOTLOADER_FLASH_FW_SIZE - sizeof(Bootloader->FwCRC16) - sizeof(Bootloader->FwSize);
	if (BSP_FLASH_Read(FlashFwAddr, (char*)&ReadFwSize, sizeof(ReadFwSize)))
		return -1;
	if ((ReadFwSize >= BOOTLOADER_FLASH_FW_SIZE) || (ReadFwSize == 0))
		return -1;
	/*Check FW CRC16*/
	Size = ReadFwSize;
	FlashFwAddr = BOOTLOADER_FLASH_FW_ADDR;
	CheckFwCRC16 = 0;
	while (Size)
	{
		ReadSize = Size > BSP_FLASH_PAGE_SIZE ? BSP_FLASH_PAGE_SIZE : Size;
		if (BSP_FLASH_Read(FlashFwAddr, (char*)Bootloader->Buffer, ReadSize))
			return -1;
		for (i = 0; i < ReadSize; i++)
			CheckFwCRC16 = Calc_CRC16_Value(Bootloader->Buffer[i], CheckFwCRC16);
		FlashFwAddr += ReadSize;
		Size -= ReadSize;
	}
	FlashFwAddr = BOOTLOADER_FLASH_FW_ADDR + BOOTLOADER_FLASH_FW_SIZE - sizeof(Bootloader->FwCRC16);
	if (BSP_FLASH_Read(FlashFwAddr, (char*)&ReadFwCRC16, sizeof(ReadFwCRC16)))
		return -1;
	if (CheckFwCRC16 != ReadFwCRC16)
		return -1;
	FlashFwAddr = BOOTLOADER_FLASH_FW_ADDR + BOOTLOADER_FLASH_FW_SIZE  - sizeof(Bootloader->FwCRC16) - sizeof(Bootloader->FwSize) - sizeof(uint32_t);;
	if (BSP_FLASH_Read(FlashFwAddr, (char*)&ReadFwCRC16, sizeof(ReadFwCRC16)))
		return -1;
	*FwSize = ReadFwSize;
	*FwCRC16 = ReadFwCRC16;
	return 0;
}

static int32_t BOOTLOADER_UpdateFw(BOOTLOADER_T *Bootloader, uint32_t NewFwSize, uint16_t NewFwCRC16)
{
	uint32_t FlashFwAddr, FwAddr, i, ReadSize, Size;
	uint16_t CheckFwCRC16;
	uint32_t FirstSector, LastSector;
	
	if (FLASH_GetSector(BOOTLOADER_FW_ADDR, &FirstSector))
		return -1;
	if (FLASH_GetSector(BOOTLOADER_FW_ADDR + BOOTLOADER_FW_SIZE - 1, &LastSector))
		return -1;	
	if (FLASH_EraseSectors(FirstSector, LastSector - FirstSector + 1))
		return -1;
	
	Size = NewFwSize;
	FlashFwAddr = BOOTLOADER_FLASH_FW_ADDR;
	FwAddr = BOOTLOADER_FW_ADDR;
	CheckFwCRC16 = 0;
	while (Size)
	{
		ReadSize = Size > BSP_FLASH_PAGE_SIZE ? BSP_FLASH_PAGE_SIZE : Size;
		if (BSP_FLASH_Read(FlashFwAddr, (char*)Bootloader->Buffer, ReadSize))
			return -1;
		if (FLASH_Program(FwAddr, Bootloader->Buffer, ReadSize))
			return -1;
		for (i = 0; i < ReadSize; i++)
		{
			if (*(uint8_t*)FwAddr != Bootloader->Buffer[i])
				return -1;
			CheckFwCRC16 = Calc_CRC16_Value(*(uint8_t*)FwAddr, CheckFwCRC16);
			FwAddr++;
		}
		FlashFwAddr += ReadSize;
		Size -= ReadSize;
	}
	if (CheckFwCRC16 != NewFwCRC16)
		return -1;
	FwAddr = (BOOTLOADER_FW_ADDR + BOOTLOADER_FW_SIZE - sizeof(Bootloader->FwCRC16) - sizeof(Bootloader->FwSize));
	if (FLASH_Program(FwAddr, (uint8_t*)&NewFwSize, sizeof(NewFwSize)))
		return -1;
	if (*(uint32_t*)FwAddr != NewFwSize)
		return -1;
	FwAddr = (BOOTLOADER_FW_ADDR + BOOTLOADER_FW_SIZE - sizeof(Bootloader->FwCRC16));
	if (FLASH_Program(FwAddr, (uint8_t*)&CheckFwCRC16, sizeof(CheckFwCRC16)))
		return -1;
	if (*(uint16_t*)FwAddr != CheckFwCRC16)
		return -1;
	FlashFwAddr = BOOTLOADER_FLASH_FW_ADDR + BOOTLOADER_FLASH_FW_SIZE - BSP_FLASH_PAGE_SIZE;
	if (BSP_FLASH_ErasePage(FlashFwAddr))
		return -1;
	return 0;
}

void BOOTLOADER_StartFw(BOOTLOADER_T *Bootloader)
{
	// ¬ нормальном режиме копи€ равна оригиналу и обе целостные, попадаем сюда
	uint32_t SP = *(uint32_t*)( BOOTLOADER_FLASH_FW_ADDR );
	uint32_t FwAddr;
	if( ( SP & 0x2FFE0000 ) == 0x20000000 )
	{
		__disable_irq();
		__set_MSP( SP );// инициализируем SP значением из основной программы (первые 4 байта)
		FwAddr = *(uint32_t*)( BOOTLOADER_FLASH_FW_ADDR + 4 );//стартовый адрес в следующих 4 байтах
		( (void (*)(void))FwAddr )();//«апуск программы
	}
}

void BOOTLOADER_Init(BOOTLOADER_T *Bootloader)
{
	
}

int32_t BOOTLOADER_CheckAndUpdateFw(BOOTLOADER_T *Bootloader)
{
	uint32_t FwSize, NewFwSize;
	uint16_t FwCRC16, NewFwCRC16;
	
	if (!BOOTLOADER_CheckNewFw(Bootloader, &NewFwSize, &NewFwCRC16))
	{
		return BOOTLOADER_UpdateFw(Bootloader, NewFwSize, NewFwCRC16);
	}
	
	if (!BOOTLOADER_CheckFw(Bootloader, &FwSize, &FwCRC16))
	{
		return 0;
	}
	
	return -1;
}

void BOOTLOADER_UpdateNewFwStart(BOOTLOADER_T *Bootloader)
{
	Bootloader->FwCRC16 = 0;
	Bootloader->FwSize = 0;
	Bootloader->BufferAddr = 0;
}

int32_t BOOTLOADER_UpdateNewFwByte(BOOTLOADER_T *Bootloader, uint8_t FwByte)
{
	uint32_t FlashFwAddr;
	Bootloader->Buffer[Bootloader->BufferAddr] = FwByte;
	Bootloader->FwCRC16 = Calc_CRC16_Value(FwByte, Bootloader->FwCRC16);	
	if (Bootloader->BufferAddr == (BSP_FLASH_PAGE_SIZE - 1))
	{
		if (BSP_FLASH_WaitReady())
			return -1;
		FlashFwAddr = BOOTLOADER_FLASH_FW_ADDR + Bootloader->FwSize;
		if (BSP_FLASH_ProgramPage(FlashFwAddr, (char*)Bootloader->Buffer, BSP_FLASH_PAGE_SIZE))
			return -1;
		Bootloader->BufferAddr = 0;
		Bootloader->FwSize += BSP_FLASH_PAGE_SIZE;				
	}
	else
	{
		Bootloader->BufferAddr++;
	}
	return 0;
}

int32_t BOOTLOADER_UpdateNewFwComplete(BOOTLOADER_T *Bootloader)
{
	uint32_t FlashFwAddr;
	uint32_t BufferAddr;
	
	if (Bootloader->BufferAddr)
	{
		if (BSP_FLASH_WaitReady())
			return -1;
		FlashFwAddr = BOOTLOADER_FLASH_FW_ADDR + Bootloader->FwSize;
		if (BSP_FLASH_ProgramPage(FlashFwAddr, (char*)Bootloader->Buffer, Bootloader->BufferAddr))
			return -1;
		Bootloader->FwSize += Bootloader->BufferAddr;
	}	
	if (BSP_FLASH_WaitReady())
		return -1;	
	FlashFwAddr = BOOTLOADER_FLASH_FW_ADDR + BOOTLOADER_FLASH_FW_SIZE - BSP_FLASH_PAGE_SIZE;
	if (BSP_FLASH_Read(FlashFwAddr, (char*)Bootloader->Buffer, BSP_FLASH_PAGE_SIZE))
		return -1;	
	BufferAddr = BSP_FLASH_PAGE_SIZE - sizeof(Bootloader->FwCRC16);
	memcpy(&Bootloader->Buffer[BufferAddr], &Bootloader->FwCRC16, sizeof(Bootloader->FwCRC16));
	BufferAddr = BSP_FLASH_PAGE_SIZE - sizeof(Bootloader->FwCRC16) - sizeof(Bootloader->FwSize);
	memcpy(&Bootloader->Buffer[BufferAddr], &Bootloader->FwSize, sizeof(Bootloader->FwSize));
	BufferAddr = BSP_FLASH_PAGE_SIZE - sizeof(Bootloader->FwCRC16) - sizeof(Bootloader->FwSize) - sizeof(uint32_t);
	*(uint32_t*)&Bootloader->Buffer[BufferAddr] = BOOTLOADER_FW_UPDATE_FLAG;
	FlashFwAddr = BOOTLOADER_FLASH_FW_ADDR + BOOTLOADER_FLASH_FW_SIZE - BSP_FLASH_PAGE_SIZE;
	if (BSP_FLASH_ProgramPage(FlashFwAddr, (char*)Bootloader->Buffer, BSP_FLASH_PAGE_SIZE))
		return -1;
	if (BSP_FLASH_WaitReady())
		return -1;
	return 0;
}
