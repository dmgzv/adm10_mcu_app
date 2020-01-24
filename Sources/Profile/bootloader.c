#include "bootloader.h"
#include "crc.h"
#include <string.h>

static int32_t BOOTLOADER_CheckEraseFlag(BOOTLOADER_T *Bootloader)
{
	BOOTLOADER_INFO_T *BootloaderInfo;
	/*Check Erase FW Size*/
	BootloaderInfo = (BOOTLOADER_INFO_T*)(BOOTLOADER_INFO);
	if (BootloaderInfo->CRC16 == 0xFFFF)
		return -1;
	return 0;
}

static int32_t BOOTLOADER_ResetEraseFlag(BOOTLOADER_T *Bootloader)
{
	BOOTLOADER_INFO_T *BootloaderInfo;
	uint16_t CRC16;
	
	BootloaderInfo = (BOOTLOADER_INFO_T*)(BOOTLOADER_INFO);
	CRC16 = 0x1234;
	if (FLASH_Program((uint32_t)&BootloaderInfo->CRC16, (uint8_t*)&CRC16, sizeof(CRC16)))
		return -1;
	return 0;
}

static int32_t BOOTLOADER_CheckFw(BOOTLOADER_T *Bootloader)
{
	BOOTLOADER_INFO_T *FwInfo;
	uint16_t CRC16;
	
	FwInfo = (BOOTLOADER_INFO_T*)(BOOTLOADER_FW_INFO);	
	/*Check Project*/
	if (memcmp(FwInfo->Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		return -1;
	/*Check HW Version*/
	if (memcmp(FwInfo->HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		return -1;
	/*Check FW CRC16*/
	CRC16 = Calc_CRC16((char*)BOOTLOADER_FW_ADDR, BOOTLOADER_FW_INFO - BOOTLOADER_FW_ADDR, 0);
	if (CRC16 != FwInfo->CRC16)
		return -1;
	return 0;
}

static int32_t BOOTLOADER_CheckNewFw(BOOTLOADER_T *Bootloader)
{
	BOOTLOADER_INFO_T FwInfo;
	uint32_t Addr, i;
	uint16_t CRC16;
	
	/*Check FW Size*/
	Addr = BSP_FLASH_FW_INFO;
	if (BSP_FLASH_Read(BSP_FLASH_FW_INFO, (char*)&FwInfo, sizeof(FwInfo)))
		return -1;
	/*Check Project*/
	if (memcmp(FwInfo.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		return -1;
	/*Check HW Version*/
	if (memcmp(FwInfo.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		return -1;
	/*Check FW CRC16*/
	CRC16 = 0;
	for (Addr = BSP_FLASH_FW_ADDR; Addr < BSP_FLASH_FW_INFO; Addr += BSP_FLASH_PAGE_SIZE)
	{
		if (BSP_FLASH_Read(Addr, (char*)Bootloader->Buffer, BSP_FLASH_PAGE_SIZE))
			return -1;		
		for (i = 0; i < BSP_FLASH_PAGE_SIZE; i++)
			CRC16 = Calc_CRC16_Value(Bootloader->Buffer[i], CRC16);
	}
	if (CRC16 != FwInfo.CRC16)
		return -1;
	return 0;
}

static int32_t BOOTLOADER_EraseNewFw(BOOTLOADER_T *Bootloader)
{
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_ErasePage(BSP_FLASH_FW_INFO))
		return -1;
	if (BSP_FLASH_WaitReady())
		return -1;
	return 0;
}

static int32_t BOOTLOADER_EraseFw(BOOTLOADER_T *Bootloader)
{
	uint32_t FirstSector, LastSector;
	
	if (FLASH_GetSector(BOOTLOADER_FW_ADDR, &FirstSector))
		return -1;
	if (FLASH_GetSector(BOOTLOADER_FW_ADDR + BOOTLOADER_FW_SIZE - 1, &LastSector))
		return -1;	
	if (FLASH_EraseSectors(FirstSector, LastSector - FirstSector + 1))
		return -1;
	
	return 0;
}

static int32_t BOOTLOADER_ResetFw(BOOTLOADER_T *Bootloader)
{
	BOOTLOADER_INFO_T *BootloaderInfo;
	uint16_t CRC16;
	
	BootloaderInfo = (BOOTLOADER_INFO_T*)(BOOTLOADER_FW_INFO);
	CRC16 = 0x0000;
	if (FLASH_Program((uint32_t)&BootloaderInfo->CRC16, (uint8_t*)&CRC16, sizeof(CRC16)))
		return -1;
	return 0;
}

static int32_t BOOTLOADER_UpdateFw(BOOTLOADER_T *Bootloader)
{
	uint32_t FlashFwAddr, FwAddr;
	
	FwAddr = BOOTLOADER_FW_ADDR;
	for (FlashFwAddr = BSP_FLASH_FW_ADDR; FlashFwAddr < (BSP_FLASH_FW_ADDR + BSP_FLASH_FW_SIZE); FlashFwAddr += BSP_FLASH_PAGE_SIZE)
	{
		if (BSP_FLASH_Read(FlashFwAddr, (char*)Bootloader->Buffer, BSP_FLASH_PAGE_SIZE))
			return -1;
		if (FLASH_Program(FwAddr, Bootloader->Buffer, BSP_FLASH_PAGE_SIZE))
			return -1;
		FwAddr += BSP_FLASH_PAGE_SIZE;
	}
	
	return 0;
}

void BOOTLOADER_StartFw(BOOTLOADER_T *Bootloader)
{
	// ¬ нормальном режиме копи€ равна оригиналу и обе целостные, попадаем сюда
	uint32_t SP = *(uint32_t*)( BSP_FLASH_FW_ADDR );
	uint32_t FwAddr;
	if( ( SP & 0x2FFE0000 ) == 0x20000000 )
	{
		__disable_irq();
		__set_MSP( SP );// инициализируем SP значением из основной программы (первые 4 байта)
		FwAddr = *(uint32_t*)( BSP_FLASH_FW_ADDR + 4 );//стартовый адрес в следующих 4 байтах
		( (void (*)(void))FwAddr )();//«апуск программы
	}
}

int32_t BOOTLOADER_CheckAndUpdateFw(BOOTLOADER_T *Bootloader)
{	
	if (BOOTLOADER_CheckEraseFlag(Bootloader))
	{
		BOOTLOADER_EraseFw(Bootloader);
		BOOTLOADER_EraseNewFw(Bootloader);
		BOOTLOADER_ResetEraseFlag(Bootloader);
	}
	
	if (!BOOTLOADER_CheckFw(Bootloader))
	{
		/* No need to update*/
		return 0;
	}
	else if (!BOOTLOADER_CheckNewFw(Bootloader))
	{
		/*Need to update*/
		if( BOOTLOADER_EraseFw(Bootloader))
			return -1;
		if( BOOTLOADER_UpdateFw(Bootloader))
			return -1;
		if (BOOTLOADER_CheckFw(Bootloader))
			return -1;
		if( BOOTLOADER_EraseNewFw(Bootloader))
			return -1;
		return 0;
	}
	/* No valid FW images*/
	return -1;
}

int32_t BOOTLOADER_UpdateNewFwStart(BOOTLOADER_T *Bootloader)
{	
	Bootloader->FwCRC16 = 0;
	Bootloader->FlashAddr = BSP_FLASH_FW_ADDR;
	Bootloader->BufferAddr = 0;
	return 0;
}

int32_t BOOTLOADER_UpdateNewFwByte(BOOTLOADER_T *Bootloader, uint8_t FwByte)
{	
	/*Check that FW is not too big*/
	if (Bootloader->FlashAddr > BSP_FLASH_FW_INFO)
		return -1;
	
	Bootloader->Buffer[Bootloader->BufferAddr] = FwByte;
	if (Bootloader->FlashAddr < (BSP_FLASH_FW_INFO))
		Bootloader->FwCRC16 = Calc_CRC16_Value(FwByte, Bootloader->FwCRC16);
	Bootloader->BufferAddr++;
	
	if (Bootloader->BufferAddr == BSP_FLASH_PAGE_SIZE)
	{
		if (BSP_FLASH_WaitReady())
			return -1;
		if (BSP_FLASH_ProgramPage(Bootloader->FlashAddr, (char*)Bootloader->Buffer, BSP_FLASH_PAGE_SIZE))
			return -1;
		Bootloader->BufferAddr = 0;
		Bootloader->FlashAddr += BSP_FLASH_PAGE_SIZE;
	}
	return 0;
}

int32_t BOOTLOADER_UpdateNewFwBlock(BOOTLOADER_T *Bootloader, char *FwBlock, uint32_t FwBlockSize)
{
	uint32_t Size;
	/*Check that FW is not too big*/
	if (Bootloader->FlashAddr > BSP_FLASH_FW_INFO)
		return -1;
	
	while (FwBlockSize)
	{
		if (FwBlockSize > (BSP_FLASH_PAGE_SIZE - Bootloader->BufferAddr) )
			Size = BSP_FLASH_PAGE_SIZE - Bootloader->BufferAddr;
		else
			Size = FwBlockSize;
		
		memcpy(&Bootloader->Buffer[Bootloader->BufferAddr], FwBlock, Size);
		Bootloader->FwCRC16 = Calc_CRC16(FwBlock, Size, Bootloader->FwCRC16);
		Bootloader->BufferAddr += Size;
		FwBlock += Size;
		FwBlockSize -= Size;
		if (Bootloader->BufferAddr == BSP_FLASH_PAGE_SIZE)
		{
			if (BSP_FLASH_WaitReady())
				return -1;
			if (BSP_FLASH_ProgramPage(Bootloader->FlashAddr, (char*)Bootloader->Buffer, BSP_FLASH_PAGE_SIZE))
				return -1;
			Bootloader->BufferAddr = 0;
			Bootloader->FlashAddr += BSP_FLASH_PAGE_SIZE;
		}
	}
	return 0;
}

int32_t BOOTLOADER_UpdateNewFwComplete(BOOTLOADER_T *Bootloader)
{
	BOOTLOADER_INFO_T *FwInfo;
	
	/* Check if fw image is too small*/
	if (Bootloader->FlashAddr != BSP_FLASH_FW_INFO)
		return -1;
	/*Get fw info*/
	FwInfo = (BOOTLOADER_INFO_T*)Bootloader->Buffer;
	/*Check Project*/
	if (memcmp(FwInfo->Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		return -1;
	/*Check HW Version*/
	if (memcmp(FwInfo->HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		return -1;
	/*Write fw CRC16*/
	FwInfo->CRC16 = Bootloader->FwCRC16;
	/*Write fw info to flash*/
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_ProgramPage(Bootloader->FlashAddr, (char*)Bootloader->Buffer, BSP_FLASH_PAGE_SIZE))
		return -1;
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BOOTLOADER_CheckNewFw(Bootloader))
		return -1;
	if( BOOTLOADER_ResetFw(Bootloader))
		return -1;
	return 0;
}
