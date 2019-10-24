#include "flash.h"

int32_t FLASH_GetSector(uint32_t Address, uint32_t *Sector)
{
	if (Address >= FLASH_END_ADDR)
		return -1;
#if (defined(STM32F427VI) || defined(STM32F427ZI) || defined(STM32F427II))
	else if (Address >= FLASH_SECTOR_23_ADDR)
		*Sector = FLASH_SECTOR_23;
	else if (Address >= FLASH_SECTOR_22_ADDR)
		*Sector = FLASH_SECTOR_22;
	else if (Address >= FLASH_SECTOR_21_ADDR)
		*Sector = FLASH_SECTOR_21;
	else if (Address >= FLASH_SECTOR_20_ADDR)
		*Sector = FLASH_SECTOR_20;
	else if (Address >= FLASH_SECTOR_19_ADDR)
		*Sector = FLASH_SECTOR_19;
	else if (Address >= FLASH_SECTOR_18_ADDR)
		*Sector = FLASH_SECTOR_18;
	else if (Address >= FLASH_SECTOR_17_ADDR)
		*Sector = FLASH_SECTOR_17;
	else if (Address >= FLASH_SECTOR_16_ADDR)
		*Sector = FLASH_SECTOR_16;
	else if (Address >= FLASH_SECTOR_15_ADDR)
		*Sector = FLASH_SECTOR_15;
	else if (Address >= FLASH_SECTOR_14_ADDR)
		*Sector = FLASH_SECTOR_14;
	else if (Address >= FLASH_SECTOR_12_ADDR)
		*Sector = FLASH_SECTOR_12;
#endif
#if (defined(STM32F407VG) || defined(STM32F407ZG) || defined(STM32F407IG) || \
		defined(STM32F427VG) || defined(STM32F427ZG) || defined(STM32F427IG) || \
		defined(STM32F427VI) || defined(STM32F427ZI) || defined(STM32F427II))
	else if (Address >= FLASH_SECTOR_11_ADDR)
		*Sector = FLASH_SECTOR_11;
	else if (Address >= FLASH_SECTOR_10_ADDR)
		*Sector = FLASH_SECTOR_10;
	else if (Address >= FLASH_SECTOR_9_ADDR)
		*Sector = FLASH_SECTOR_9;
	else if (Address >= FLASH_SECTOR_8_ADDR)
		*Sector = FLASH_SECTOR_8;
#endif
	else if (Address >= FLASH_SECTOR_7_ADDR)
		*Sector = FLASH_SECTOR_7;
	else if (Address >= FLASH_SECTOR_6_ADDR)
		*Sector = FLASH_SECTOR_6;
	else if (Address >= FLASH_SECTOR_5_ADDR)
		*Sector = FLASH_SECTOR_5;
	else if (Address >= FLASH_SECTOR_4_ADDR)
		*Sector = FLASH_SECTOR_4;
	else if (Address >= FLASH_SECTOR_2_ADDR)
		*Sector = FLASH_SECTOR_2;
	else if (Address >= FLASH_SECTOR_1_ADDR)
		*Sector = FLASH_SECTOR_1;
	else if (Address >= FLASH_SECTOR_0_ADDR)
		*Sector = FLASH_SECTOR_0;
	else
		return -1;
	return 0;
}
int32_t FLASH_GetSectorAddr(uint32_t Sector, uint32_t *Address)
{
	if(Sector == FLASH_SECTOR_0)
		*Address = FLASH_SECTOR_0_ADDR;
	else if(Sector == FLASH_SECTOR_1)
		*Address = FLASH_SECTOR_1_ADDR;
	else if(Sector == FLASH_SECTOR_2)
		*Address = FLASH_SECTOR_2_ADDR;
	else if(Sector == FLASH_SECTOR_3)
		*Address = FLASH_SECTOR_3_ADDR;
	else if(Sector == FLASH_SECTOR_4)
		*Address = FLASH_SECTOR_4_ADDR;
	else if(Sector == FLASH_SECTOR_5)
		*Address = FLASH_SECTOR_5_ADDR;
	else if(Sector == FLASH_SECTOR_6)
		*Address = FLASH_SECTOR_6_ADDR;
	else if(Sector == FLASH_SECTOR_7)
		*Address = FLASH_SECTOR_7_ADDR;
#if (defined(STM32F407VG) || defined(STM32F407ZG) || defined(STM32F407IG) || \
		defined(STM32F427VG) || defined(STM32F427ZG) || defined(STM32F427IG) || \
		defined(STM32F427VI) || defined(STM32F427ZI) || defined(STM32F427II))
	else if(Sector == FLASH_SECTOR_8)
		*Address = FLASH_SECTOR_8_ADDR;
	else if(Sector == FLASH_SECTOR_9)
		*Address = FLASH_SECTOR_9_ADDR;
	else if(Sector == FLASH_SECTOR_10)
		*Address = FLASH_SECTOR_10_ADDR;
	else if(Sector == FLASH_SECTOR_11)
		*Address = FLASH_SECTOR_11_ADDR;
#endif
#if (defined(STM32F427VI) || defined(STM32F427ZI) || defined(STM32F427II))
	else if(Sector == FLASH_SECTOR_12)
		*Address = FLASH_SECTOR_12_ADDR;
	else if(Sector == FLASH_SECTOR_13)
		*Address = FLASH_SECTOR_13_ADDR;
	else if(Sector == FLASH_SECTOR_14)
		*Address = FLASH_SECTOR_14_ADDR;
	else if(Sector == FLASH_SECTOR_15)
		*Address = FLASH_SECTOR_15_ADDR;
	else if(Sector == FLASH_SECTOR_16)
		*Address = FLASH_SECTOR_16_ADDR;
	else if(Sector == FLASH_SECTOR_17)
		*Address = FLASH_SECTOR_17_ADDR;
	else if(Sector == FLASH_SECTOR_18)
		*Address = FLASH_SECTOR_18_ADDR;
	else if(Sector == FLASH_SECTOR_19)
		*Address = FLASH_SECTOR_19_ADDR;
	else if(Sector == FLASH_SECTOR_20)
		*Address = FLASH_SECTOR_20_ADDR;
	else if(Sector == FLASH_SECTOR_21)
		*Address = FLASH_SECTOR_21_ADDR;
	else if(Sector == FLASH_SECTOR_22)
		*Address = FLASH_SECTOR_22_ADDR;
	else if(Sector == FLASH_SECTOR_23)
		*Address = FLASH_SECTOR_23_ADDR;
#endif
	else
		return -1;
	return 0;
}
int32_t FLASH_GetSectorSize(uint32_t Sector, uint32_t *Size)
{
  if(Sector == FLASH_SECTOR_0)
		*Size = FLASH_SECTOR_0_SIZE;
	else if(Sector == FLASH_SECTOR_1)
		*Size = FLASH_SECTOR_1_SIZE;
	else if(Sector == FLASH_SECTOR_2)
		*Size = FLASH_SECTOR_2_SIZE;
	else if(Sector == FLASH_SECTOR_3)
		*Size = FLASH_SECTOR_3_SIZE;
	else if(Sector == FLASH_SECTOR_4)
		*Size = FLASH_SECTOR_4_SIZE;
	else if(Sector == FLASH_SECTOR_5)
		*Size = FLASH_SECTOR_5_SIZE;
	else if(Sector == FLASH_SECTOR_6)
		*Size = FLASH_SECTOR_6_SIZE;
	else if(Sector == FLASH_SECTOR_7)
		*Size = FLASH_SECTOR_7_SIZE;
#if (defined(STM32F407VG) || defined(STM32F407ZG) || defined(STM32F407IG) || \
		defined(STM32F427VG) || defined(STM32F427ZG) || defined(STM32F427IG) || \
		defined(STM32F427VI) || defined(STM32F427ZI) || defined(STM32F427II))
	else if(Sector == FLASH_SECTOR_8)
		*Size = FLASH_SECTOR_8_SIZE;
	else if(Sector == FLASH_SECTOR_9)
		*Size = FLASH_SECTOR_9_SIZE;
	else if(Sector == FLASH_SECTOR_10)
		*Size = FLASH_SECTOR_10_SIZE;
	else if(Sector == FLASH_SECTOR_11)
		*Size = FLASH_SECTOR_11_SIZE;
#endif
#if (defined(STM32F427VI) || defined(STM32F427ZI) || defined(STM32F427II))
	else if(Sector == FLASH_SECTOR_12)
		*Size = FLASH_SECTOR_12_SIZE;
	else if(Sector == FLASH_SECTOR_13)
		*Size = FLASH_SECTOR_13_SIZE;
	else if(Sector == FLASH_SECTOR_14)
		*Size = FLASH_SECTOR_14_SIZE;
	else if(Sector == FLASH_SECTOR_15)
		*Size = FLASH_SECTOR_15_SIZE;
	else if(Sector == FLASH_SECTOR_16)
		*Size = FLASH_SECTOR_16_SIZE;
	else if(Sector == FLASH_SECTOR_17)
		*Size = FLASH_SECTOR_17_SIZE;
	else if(Sector == FLASH_SECTOR_18)
		*Size = FLASH_SECTOR_18_SIZE;
	else if(Sector == FLASH_SECTOR_19)
		*Size = FLASH_SECTOR_19_SIZE;
	else if(Sector == FLASH_SECTOR_20)
		*Size = FLASH_SECTOR_20_SIZE;
	else if(Sector == FLASH_SECTOR_21)
		*Size = FLASH_SECTOR_21_SIZE;
	else if(Sector == FLASH_SECTOR_22)
		*Size = FLASH_SECTOR_22_SIZE;
	else if(Sector == FLASH_SECTOR_23)
		*Size = FLASH_SECTOR_23_SIZE;
#endif
	else
		return -1;
	return 0;
}

int32_t FLASH_EraseSectors(uint32_t FirstSector ,uint32_t SectorsCnt)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SectorError = 0;
	
	HAL_FLASH_Unlock();	
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = FirstSector;
  EraseInitStruct.NbSectors = SectorsCnt;
  if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
		goto error;
	
	/* Note: If an erase operation in Flash memory also concerns data in the data or instruction cache,
     you have to make sure that these data are rewritten before they are accessed during code
     execution. If this cannot be done safely, it is recommended to flush the caches by setting the
     DCRST and ICRST bits in the FLASH_CR register. */
	__HAL_FLASH_DATA_CACHE_DISABLE();
  __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

  __HAL_FLASH_DATA_CACHE_RESET();
  __HAL_FLASH_INSTRUCTION_CACHE_RESET();

  __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
  __HAL_FLASH_DATA_CACHE_ENABLE();
	HAL_FLASH_Lock(); 
	return 0;
error:	
	HAL_FLASH_Lock();
	return -1;
}

int32_t FLASH_Program(uint32_t StartAddress, uint8_t *Buffer, uint32_t Size)
{
	uint32_t i;
	
	if ((StartAddress + Size > FLASH_END_ADDR) || (StartAddress < FLASH_START_ADDR))
		return -1;
	
	HAL_FLASH_Unlock();	
	for ( i = 0; i < Size; i++)
  {
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, StartAddress + i, Buffer[i]) != HAL_OK)
			goto error;
	}
	HAL_FLASH_Lock();
	return 0;
error:	
	HAL_FLASH_Lock();
	return -1;
}
