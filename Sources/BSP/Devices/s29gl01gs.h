#ifndef __S29GL01GS_H
#define __S29GL01GS_H

#include "stm32f4xx_hal.h"
#include "bsp_config.h"
#include "bsp_Dev.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#define S29GL01GS_ID								"D86"
#define S29GL01GS_SECTOR_SIZE				(128 * 1024)
#define S29GL01GS_SECTORS_COUNT			(1024)
#define S29GL01GS_SIZE							(S29GL01GS_SECTOR_SIZE * S29GL01GS_SECTORS_COUNT)

typedef struct {
	BSP_DEV;
	NOR_HandleTypeDef hnor[2];
	NOR_IDTypeDef Id;
} S29GL01GS_T;

int32_t S29GL01GS_Init(S29GL01GS_T *Dev, const char* DevId);
void S29GL01GS_Deinit(S29GL01GS_T *Dev);
int32_t S29GL01GS_Read(S29GL01GS_T *Dev, uint32_t Addr, void *Buffer, uint32_t Size);
int32_t S29GL01GS_ProgramSector(S29GL01GS_T *Dev, uint32_t Addr, void *Buffer, uint32_t Size);
int32_t S29GL01GS_EraseSector(S29GL01GS_T *Dev, uint32_t Addr);
int32_t S29GL01GS_WaitReady(S29GL01GS_T *Dev);

#endif /* __S29GL01GS_H */
