#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#include "bsp.h"

#define BOOTLOADER_INFO_SHIFT			( BSP_FLASH_PAGE_SIZE )

#define BOOTLOADER_ADDR						( FLASH_START_ADDR )
#define BOOTLOADER_SIZE						( FLASH_SECTOR_4_ADDR - BOOTLOADER_ADDR )
#define BOOTLOADER_INFO						( BOOTLOADER_ADDR + BOOTLOADER_SIZE - BOOTLOADER_INFO_SHIFT )

#define BOOTLOADER_FW_ADDR				( BOOTLOADER_ADDR + BOOTLOADER_SIZE )
#define BOOTLOADER_FW_SIZE				( FLASH_END_ADDR - BOOTLOADER_FW_ADDR )
#define BOOTLOADER_FW_INFO				( BOOTLOADER_FW_ADDR + BOOTLOADER_FW_SIZE - BOOTLOADER_INFO_SHIFT )

#define BSP_FLASH_FW_INFO					( BSP_FLASH_FW_ADDR + BSP_FLASH_FW_SIZE - BOOTLOADER_INFO_SHIFT )

#if (BOOTLOADER_FW_SIZE != BSP_FLASH_FW_SIZE)
	#error	BOOTLOADER_FW_SIZE is not equal to BSP_FLASH_FW_SIZE
#endif

typedef struct {
	char Project[16];
	char HwVersion[16];
	char SwVersion[16];
	uint16_t CRC16;
} BOOTLOADER_INFO_T;
	
typedef struct {
	uint8_t Buffer[BSP_FLASH_PAGE_SIZE];
	uint32_t BufferAddr;
	uint32_t FlashAddr;
	uint16_t FwCRC16;
} BOOTLOADER_T;

int32_t BOOTLOADER_CheckAndUpdateFw(BOOTLOADER_T *Bootloader);
void BOOTLOADER_StartFw(BOOTLOADER_T *Bootloader);
int32_t BOOTLOADER_UpdateNewFwStart(BOOTLOADER_T *Bootloader);
int32_t BOOTLOADER_UpdateNewFwByte(BOOTLOADER_T *Bootloader, uint8_t FwByte);
int32_t BOOTLOADER_UpdateNewFwBlock(BOOTLOADER_T *Bootloader, char *FwBlock, uint32_t FwBlockSize);
int32_t BOOTLOADER_UpdateNewFwFinish(BOOTLOADER_T *Bootloader);
int32_t BOOTLOADER_CheckNewFw(BOOTLOADER_T *Bootloader);
int32_t BOOTLOADER_UpdateNewFw(BOOTLOADER_T *Bootloader);

#endif /* __BOOTLOADER_H */
