#ifndef __FLASH_H
#define __FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "stm32f4xx_hal.h"

#if defined(STM32F407xx)

#define FLASH_SECTOR_0_SIZE     ((uint32_t)0x00004000) /* Size of Sector 0, 16 Kbytes */
#define FLASH_SECTOR_1_SIZE     ((uint32_t)0x00004000) /* Size of Sector 1, 16 Kbytes */
#define FLASH_SECTOR_2_SIZE     ((uint32_t)0x00004000) /* Size of Sector 2, 16 Kbytes */
#define FLASH_SECTOR_3_SIZE     ((uint32_t)0x00004000) /* Size of Sector 3, 16 Kbytes */
#define FLASH_SECTOR_4_SIZE     ((uint32_t)0x00010000) /* Size of Sector 4, 64 Kbytes */
#define FLASH_SECTOR_5_SIZE     ((uint32_t)0x00020000) /* Size of Sector 5, 128 Kbytes */
#define FLASH_SECTOR_6_SIZE     ((uint32_t)0x00020000) /* Size of Sector 6, 128 Kbytes */
#define FLASH_SECTOR_7_SIZE     ((uint32_t)0x00020000) /* Size of Sector 7, 128 Kbytes */
#define FLASH_SECTOR_8_SIZE     ((uint32_t)0x00020000) /* Size of Sector 8, 128 Kbytes */
#define FLASH_SECTOR_9_SIZE     ((uint32_t)0x00020000) /* Size of Sector 9, 128 Kbytes */
#define FLASH_SECTOR_10_SIZE    ((uint32_t)0x00020000) /* Size of Sector 10, 128 Kbytes */
#define FLASH_SECTOR_11_SIZE    ((uint32_t)0x00020000) /* Size of Sector 11, 128 Kbytes */

#define FLASH_START_ADDR				((uint32_t)0x08000000)
#define FLASH_SECTOR_0_ADDR     (FLASH_START_ADDR) /* Base @ of Sector 0, 16 Kbytes */
#define FLASH_SECTOR_1_ADDR     (FLASH_SECTOR_0_ADDR + FLASH_SECTOR_0_SIZE) /* Base @ of Sector 1, 16 Kbytes */
#define FLASH_SECTOR_2_ADDR     (FLASH_SECTOR_1_ADDR + FLASH_SECTOR_1_SIZE) /* Base @ of Sector 2, 16 Kbytes */
#define FLASH_SECTOR_3_ADDR     (FLASH_SECTOR_2_ADDR + FLASH_SECTOR_2_SIZE) /* Base @ of Sector 3, 16 Kbytes */
#define FLASH_SECTOR_4_ADDR     (FLASH_SECTOR_3_ADDR + FLASH_SECTOR_3_SIZE) /* Base @ of Sector 4, 64 Kbytes */
#define FLASH_SECTOR_5_ADDR     (FLASH_SECTOR_4_ADDR + FLASH_SECTOR_4_SIZE) /* Base @ of Sector 5, 128 Kbytes */
#define FLASH_SECTOR_6_ADDR     (FLASH_SECTOR_5_ADDR + FLASH_SECTOR_5_SIZE) /* Base @ of Sector 6, 128 Kbytes */
#define FLASH_SECTOR_7_ADDR     (FLASH_SECTOR_6_ADDR + FLASH_SECTOR_6_SIZE) /* Base @ of Sector 7, 128 Kbytes */
#define FLASH_SECTOR_8_ADDR     (FLASH_SECTOR_7_ADDR + FLASH_SECTOR_7_SIZE) /* Base @ of Sector 8, 128 Kbytes */
#define FLASH_SECTOR_9_ADDR     (FLASH_SECTOR_8_ADDR + FLASH_SECTOR_8_SIZE) /* Base @ of Sector 9, 128 Kbytes */
#define FLASH_SECTOR_10_ADDR    (FLASH_SECTOR_9_ADDR + FLASH_SECTOR_9_SIZE) /* Base @ of Sector 10, 128 Kbytes */
#define FLASH_SECTOR_11_ADDR    (FLASH_SECTOR_10_ADDR + FLASH_SECTOR_10_SIZE) /* Base @ of Sector 11, 128 Kbytes */
#define FLASH_END_ADDR					(FLASH_SECTOR_11_ADDR + FLASH_SECTOR_11_SIZE)

#elif defined(STM32F427xx)

#define FLASH_SECTOR_0_SIZE     ((uint32_t)0x00004000) /* Size of Sector 0, 16 Kbytes */
#define FLASH_SECTOR_1_SIZE     ((uint32_t)0x00004000) /* Size of Sector 1, 16 Kbytes */
#define FLASH_SECTOR_2_SIZE     ((uint32_t)0x00004000) /* Size of Sector 2, 16 Kbytes */
#define FLASH_SECTOR_3_SIZE     ((uint32_t)0x00004000) /* Size of Sector 3, 16 Kbytes */
#define FLASH_SECTOR_4_SIZE     ((uint32_t)0x00010000) /* Size of Sector 4, 64 Kbytes */
#define FLASH_SECTOR_5_SIZE     ((uint32_t)0x00020000) /* Size of Sector 5, 128 Kbytes */
#define FLASH_SECTOR_6_SIZE     ((uint32_t)0x00020000) /* Size of Sector 6, 128 Kbytes */
#define FLASH_SECTOR_7_SIZE     ((uint32_t)0x00020000) /* Size of Sector 7, 128 Kbytes */
#define FLASH_SECTOR_8_SIZE     ((uint32_t)0x00020000) /* Size of Sector 8, 128 Kbytes */
#define FLASH_SECTOR_9_SIZE     ((uint32_t)0x00020000) /* Size of Sector 9, 128 Kbytes */
#define FLASH_SECTOR_10_SIZE    ((uint32_t)0x00020000) /* Size of Sector 10, 128 Kbytes */
#define FLASH_SECTOR_11_SIZE    ((uint32_t)0x00020000) /* Size of Sector 11, 128 Kbytes */
#define FLASH_SECTOR_12_SIZE    ((uint32_t)0x00004000) /* Size of Sector 0, 16 Kbytes */
#define FLASH_SECTOR_13_SIZE    ((uint32_t)0x00004000) /* Size of Sector 1, 16 Kbytes */
#define FLASH_SECTOR_14_SIZE    ((uint32_t)0x00004000) /* Size of Sector 2, 16 Kbytes */
#define FLASH_SECTOR_15_SIZE    ((uint32_t)0x00004000) /* Size of Sector 3, 16 Kbytes */
#define FLASH_SECTOR_16_SIZE    ((uint32_t)0x00010000) /* Size of Sector 4, 64 Kbytes */
#define FLASH_SECTOR_17_SIZE    ((uint32_t)0x00020000) /* Size of Sector 5, 128 Kbytes */
#define FLASH_SECTOR_18_SIZE    ((uint32_t)0x00020000) /* Size of Sector 6, 128 Kbytes */
#define FLASH_SECTOR_19_SIZE    ((uint32_t)0x00020000) /* Size of Sector 7, 128 Kbytes */
#define FLASH_SECTOR_20_SIZE    ((uint32_t)0x00020000) /* Size of Sector 8, 128 Kbytes */
#define FLASH_SECTOR_21_SIZE    ((uint32_t)0x00020000) /* Size of Sector 9, 128 Kbytes */
#define FLASH_SECTOR_22_SIZE    ((uint32_t)0x00020000) /* Size of Sector 10, 128 Kbytes */
#define FLASH_SECTOR_23_SIZE    ((uint32_t)0x00020000) /* Size of Sector 11, 128 Kbytes */

#define FLASH_START_ADDR				((uint32_t)0x08000000)
#define FLASH_SECTOR_0_ADDR     (FLASH_START_ADDR) /* Base @ of Sector 0, 16 Kbytes */
#define FLASH_SECTOR_1_ADDR     (FLASH_SECTOR_0_ADDR + FLASH_SECTOR_0_SIZE) /* Base @ of Sector 1, 16 Kbytes */
#define FLASH_SECTOR_2_ADDR     (FLASH_SECTOR_1_ADDR + FLASH_SECTOR_1_SIZE) /* Base @ of Sector 2, 16 Kbytes */
#define FLASH_SECTOR_3_ADDR     (FLASH_SECTOR_2_ADDR + FLASH_SECTOR_2_SIZE) /* Base @ of Sector 3, 16 Kbytes */
#define FLASH_SECTOR_4_ADDR     (FLASH_SECTOR_3_ADDR + FLASH_SECTOR_3_SIZE) /* Base @ of Sector 4, 64 Kbytes */
#define FLASH_SECTOR_5_ADDR     (FLASH_SECTOR_4_ADDR + FLASH_SECTOR_4_SIZE) /* Base @ of Sector 5, 128 Kbytes */
#define FLASH_SECTOR_6_ADDR     (FLASH_SECTOR_5_ADDR + FLASH_SECTOR_5_SIZE) /* Base @ of Sector 6, 128 Kbytes */
#define FLASH_SECTOR_7_ADDR     (FLASH_SECTOR_6_ADDR + FLASH_SECTOR_6_SIZE) /* Base @ of Sector 7, 128 Kbytes */
#define FLASH_SECTOR_8_ADDR     (FLASH_SECTOR_7_ADDR + FLASH_SECTOR_7_SIZE) /* Base @ of Sector 8, 128 Kbytes */
#define FLASH_SECTOR_9_ADDR     (FLASH_SECTOR_8_ADDR + FLASH_SECTOR_8_SIZE) /* Base @ of Sector 9, 128 Kbytes */
#define FLASH_SECTOR_10_ADDR    (FLASH_SECTOR_9_ADDR + FLASH_SECTOR_9_SIZE) /* Base @ of Sector 10, 128 Kbytes */
#define FLASH_SECTOR_11_ADDR    (FLASH_SECTOR_10_ADDR + FLASH_SECTOR_10_SIZE) /* Base @ of Sector 11, 128 Kbytes */
#define FLASH_SECTOR_12_ADDR    (FLASH_SECTOR_11_ADDR + FLASH_SECTOR_11_SIZE) /* Base @ of Sector 1, 16 Kbytes */
#define FLASH_SECTOR_13_ADDR    (FLASH_SECTOR_12_ADDR + FLASH_SECTOR_12_SIZE) /* Base @ of Sector 2, 16 Kbytes */
#define FLASH_SECTOR_14_ADDR    (FLASH_SECTOR_13_ADDR + FLASH_SECTOR_13_SIZE) /* Base @ of Sector 3, 16 Kbytes */
#define FLASH_SECTOR_15_ADDR    (FLASH_SECTOR_14_ADDR + FLASH_SECTOR_14_SIZE) /* Base @ of Sector 4, 64 Kbytes */
#define FLASH_SECTOR_16_ADDR    (FLASH_SECTOR_15_ADDR + FLASH_SECTOR_15_SIZE) /* Base @ of Sector 5, 128 Kbytes */
#define FLASH_SECTOR_17_ADDR    (FLASH_SECTOR_16_ADDR + FLASH_SECTOR_16_SIZE) /* Base @ of Sector 6, 128 Kbytes */
#define FLASH_SECTOR_18_ADDR    (FLASH_SECTOR_17_ADDR + FLASH_SECTOR_17_SIZE) /* Base @ of Sector 7, 128 Kbytes */
#define FLASH_SECTOR_19_ADDR    (FLASH_SECTOR_18_ADDR + FLASH_SECTOR_18_SIZE) /* Base @ of Sector 8, 128 Kbytes */
#define FLASH_SECTOR_20_ADDR    (FLASH_SECTOR_19_ADDR + FLASH_SECTOR_19_SIZE) /* Base @ of Sector 9, 128 Kbytes */
#define FLASH_SECTOR_21_ADDR    (FLASH_SECTOR_20_ADDR + FLASH_SECTOR_20_SIZE) /* Base @ of Sector 10, 128 Kbytes */
#define FLASH_SECTOR_22_ADDR    (FLASH_SECTOR_21_ADDR + FLASH_SECTOR_21_SIZE) /* Base @ of Sector 11, 128 Kbytes */
#define FLASH_SECTOR_23_ADDR    (FLASH_SECTOR_22_ADDR + FLASH_SECTOR_22_SIZE) /* Base @ of Sector 11, 128 Kbytes */
#define FLASH_END_ADDR					(FLASH_SECTOR_23_ADDR + FLASH_SECTOR_23_SIZE)

#endif

int32_t FLASH_GetSector(uint32_t Address, uint32_t *Sector);
int32_t FLASH_GetSectorAddr(uint32_t Sector, uint32_t *Address);
int32_t FLASH_GetSectorSize(uint32_t Sector, uint32_t *Size);
int32_t FLASH_EraseSectors(uint32_t FirstSector ,uint32_t SectorsCnt);
int32_t FLASH_Program(uint32_t StartAddress, uint8_t *Buffer, uint32_t Size);

#endif /* __FLASH_H */
