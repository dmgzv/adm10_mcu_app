#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#include "bsp.h"

#define BOOTLOADER_FW_ADDR				( FLASH_SECTOR_4_ADDR )
#define BOOTLOADER_FW_SIZE				( FLASH_SECTOR_4_SIZE + FLASH_SECTOR_5_SIZE + FLASH_SECTOR_6_SIZE + FLASH_SECTOR_7_SIZE + \
																		FLASH_SECTOR_8_SIZE + FLASH_SECTOR_9_SIZE + FLASH_SECTOR_10_SIZE + FLASH_SECTOR_11_SIZE)

#define BOOTLOADER_FLASH_FW_ADDR	( BOOTLOADER_FW_ADDR & ( BSP_FLASH_SIZE - 1 ) )
#define BOOTLOADER_FLASH_FW_SIZE	( BOOTLOADER_FW_SIZE )

typedef struct {
	BSP_FLASH_T *Flash;
	uint8_t Buffer[256];
	uint32_t BufferAddr;
	uint16_t FwCRC16;
	uint32_t FwSize;
} BOOTLOADER_T;

void BOOTLOADER_Init(BOOTLOADER_T *Bootloader, BSP_FLASH_T *Flash);
int32_t BOOTLOADER_CheckAndUpdateFw(BOOTLOADER_T *Bootloader);
void BOOTLOADER_StartFw(BOOTLOADER_T *Bootloader);
void BOOTLOADER_UpdateNewFwStart(BOOTLOADER_T *Bootloader);
int32_t BOOTLOADER_UpdateNewFwByte(BOOTLOADER_T *Bootloader, uint8_t FwByte);
int32_t BOOTLOADER_UpdateNewFwComplete(BOOTLOADER_T *Bootloader);

#endif /* __BOOTLOADER_H */
