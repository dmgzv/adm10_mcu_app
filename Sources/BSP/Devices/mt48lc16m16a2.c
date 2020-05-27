#include "mt48lc16m16a2.h"
#include "log.h"
#include <stdlib.h>

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef* hsdram){
	GPIO_InitTypeDef GPIO_InitStruct;
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_ENABLE();
  
  /* FMC GPIO Configuration 
#if (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK2)	
  PB5   ------> FMC_SDCKE1
  PB6   ------> FMC_SDNE1
#endif
	
	PC0   ------> FMC_SDNWE
#if (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK1)	
	PC2   ------> FMC_SDNE0
	PC3   ------> FMC_SDCKE0
#endif
	
	PD0   ------> FMC_D2
  PD1   ------> FMC_D3
	PD8   ------> FMC_D13
  PD9   ------> FMC_D14
  PD10  ------> FMC_D15
  PD14  ------> FMC_D0
  PD15  ------> FMC_D1
	
	PE0   ------> FMC_NBL0
  PE1   ------> FMC_NBL1
	PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10  ------> FMC_D7
  PE11  ------> FMC_D8
  PE12  ------> FMC_D9
  PE13  ------> FMC_D10
  PE14  ------> FMC_D11
  PE15  ------> FMC_D12
	
	PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PF11  ------> FMC_SDNRAS
  PF12  ------> FMC_A6
  PF13  ------> FMC_A7
  PF14  ------> FMC_A8
  PF15  ------> FMC_A9
	
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11 
  PG2   ------> FMC_A12
	PG4   ------> FMC_BA0
	PG5   ------> FMC_BA1
  PG8   ------> FMC_SDCLK
  PG15  ------> FMC_SDNCAS
  */

#if (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK1)
	__HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#elif (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK2)
	__HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	__HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
#endif
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	__HAL_RCC_GPIOE_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	__HAL_RCC_GPIOF_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	__HAL_RCC_GPIOG_CLK_ENABLE();
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
  
}

void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef *hsdram)
{	
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_DISABLE();
  
  /* FMC GPIO Configuration 
#if (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK2)	
  PB5   ------> FMC_SDCKE1
  PB6   ------> FMC_SDNE1
#endif
	
	PC0   ------> FMC_SDNWE
#if (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK1)	
	PC2   ------> FMC_SDNE0
	PC3   ------> FMC_SDCKE0
#endif
	
	PD0   ------> FMC_D2
  PD1   ------> FMC_D3
	PD8   ------> FMC_D13
  PD9   ------> FMC_D14
  PD10  ------> FMC_D15
  PD14  ------> FMC_D0
  PD15  ------> FMC_D1
	
	PE0   ------> FMC_NBL0
  PE1   ------> FMC_NBL1
	PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10  ------> FMC_D7
  PE11  ------> FMC_D8
  PE12  ------> FMC_D9
  PE13  ------> FMC_D10
  PE14  ------> FMC_D11
  PE15  ------> FMC_D12
	
	PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PF11  ------> FMC_SDNRAS
  PF12  ------> FMC_A6
  PF13  ------> FMC_A7
  PF14  ------> FMC_A8
  PF15  ------> FMC_A9
	
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11 
  PG2   ------> FMC_A12
	PG4   ------> FMC_BA0
	PG5   ------> FMC_BA1
  PG8   ------> FMC_SDCLK
  PG15  ------> FMC_SDNCAS
  */
	
#if (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK1)
	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3);
#elif (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK2)
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_6);
	
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);
#endif
	
  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14|GPIO_PIN_15);
	
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15);
}

#define MT48LC16M16A2_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define MT48LC16M16A2_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define MT48LC16M16A2_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define MT48LC16M16A2_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define MT48LC16M16A2_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define MT48LC16M16A2_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define MT48LC16M16A2_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define MT48LC16M16A2_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define MT48LC16M16A2_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define MT48LC16M16A2_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define MT48LC16M16A2_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

static void MT48LC16M16A2_InitSequence ( MT48LC16M16A2_T *Dev )
{
  FMC_SDRAM_CommandTypeDef FMC_SDRAMCommandStructure;
  uint32_t tmpr = 0;
  
  // Step 3 --------------------------------------------------------------------
  // Configure a clock configuration enable command
  FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;    // 0x00000001
#if (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK1)
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;       // 0x00000010
#elif (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK2)
  FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;       // 0x00000008
#endif
  FMC_SDRAMCommandStructure.AutoRefreshNumber = 1;                          //
  FMC_SDRAMCommandStructure.ModeRegisterDefinition = 0;                     //
  
  // Send the command
  // SDCMR = 0x00000011 = xx 0001 0001
  //  MRD = 0
  //  NRFS = 0   - Clock Configuration Enable
  //  CTB1 = 1   - Command issued to SDRAM Bank 1
  //  CTB2 = 0   - Command not issued to SDRAM Bank 2
  //  MODE = 001 - Clock Configuration Enable
  HAL_SDRAM_SendCommand ( &Dev->hsdram, &FMC_SDRAMCommandStructure, 0x1000);  
  
  // Step 4 --------------------------------------------------------------------
  // Insert 100 ms delay
  HAL_Delay(100);
    
  // Step 5 --------------------------------------------------------------------
  // Configure a PALL (precharge all) command
  FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_PALL;
#if (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK1)
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;       // 0x00000010
#elif (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK2)
  FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;       // 0x00000008
#endif
  FMC_SDRAMCommandStructure.AutoRefreshNumber = 1;
  FMC_SDRAMCommandStructure.ModeRegisterDefinition = 0;
  
  // Send the command
  // SDCMR = 0x00000012 = xxx 0001 0010
  //  MRD = 0
  //  NRFS = 0   - Clock Configuration Enable
  //  CTB1 = 1   - Command issued to SDRAM Bank 1
  //  CTB2 = 0   - Command not issued to SDRAM Bank 2
  //  MODE = 010 - Clock Configuration Enable
  HAL_SDRAM_SendCommand ( &Dev->hsdram, &FMC_SDRAMCommandStructure, 0x1000);
  
  // Step 6 --------------------------------------------------------------------
  // Configure a Auto-Refresh command
  FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
#if (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK1)
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;       // 0x00000010
#elif (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK2)
  FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;       // 0x00000008
#endif
  FMC_SDRAMCommandStructure.AutoRefreshNumber = 8;
  FMC_SDRAMCommandStructure.ModeRegisterDefinition = 0;
  
  // Send the command
  // SDCMR = 0x00000013 = xxx 0001 0011
  //  MRD = 0
  //  NRFS = 7   - 7 Auto refresh cycles
  //  CTB1 = 1   - Command issued to SDRAM Bank 1
  //  CTB2 = 0   - Command not issued to SDRAM Bank 2
  //  MODE = 011 - Auto-refresh command
  HAL_SDRAM_SendCommand ( &Dev->hsdram, &FMC_SDRAMCommandStructure, 0x1000);
  
  // Step 7 --------------------------------------------------------------------
  // Program the external memory mode register
  // 12 bits
  tmpr = (uint32_t)MT48LC16M16A2_MODEREG_BURST_LENGTH_1          |              // 0x0000
                   MT48LC16M16A2_MODEREG_BURST_TYPE_SEQUENTIAL   |              // 0x0000
#if (MT48LC16M16A2_FMC_CAS_LATENCY == FMC_SDRAM_CAS_LATENCY_3)
                   MT48LC16M16A2_MODEREG_CAS_LATENCY_3           |              // 0x0030
#elif (MT48LC16M16A2_FMC_CAS_LATENCY == FMC_SDRAM_CAS_LATENCY_2)
									 MT48LC16M16A2_MODEREG_CAS_LATENCY_2           |              // 0x0020
#endif
                   MT48LC16M16A2_MODEREG_OPERATING_MODE_STANDARD |              // 0x0000
                   MT48LC16M16A2_MODEREG_WRITEBURST_MODE_SINGLE;                // 0x0200
  
  // Configure a load Mode register command
  FMC_SDRAMCommandStructure.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;        // 0x00000004
#if (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK1)
	FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;       // 0x00000010
#elif (MT48LC16M16A2_FMC_BANK == FMC_SDRAM_BANK2)
  FMC_SDRAMCommandStructure.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;       // 0x00000008
#endif
  FMC_SDRAMCommandStructure.AutoRefreshNumber = 1;                          // 0 -> NRFS
  FMC_SDRAMCommandStructure.ModeRegisterDefinition = tmpr;                  // 0x0230 << 9
  
  // Send the command
  //  MRD = 0x230
  //  NRFS = 0   - 0 Auto refresh cycles
  //  CTB1 = 1   - Command issued to SDRAM Bank 1
  //  CTB2 = 0   - Command not issued to SDRAM Bank 2
  //  MODE = 100 - Load Mode Register
  HAL_SDRAM_SendCommand ( &Dev->hsdram, &FMC_SDRAMCommandStructure, 0x1000);
  
  // Step 8 --------------------------------------------------------------------
  /* Set the device refresh counter */
	HAL_SDRAM_ProgramRefreshRate(&Dev->hsdram, MT48LC16M16A2_tRFC_CLK);	
}

int32_t MT48LC16M16A2_PatternTest(MT48LC16M16A2_T *Dev, uint16_t Pattern)
{
	uint16_t Data;
	uint32_t i;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	/*Fill memory with pattern*/
	for (i = 0; i < MT48LC16M16A2_SIZE ; i+=2)
	{
			*(__IO uint16_t*) (MT48LC16M16A2_ADDR + i) = Pattern;
	}
	
	/*Check data in memory */
	for (i = 0; i < MT48LC16M16A2_SIZE ; i+=2)
	{
		Data = *(__IO uint16_t*) (MT48LC16M16A2_ADDR + i);
		if (Data != Pattern)
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t MT48LC16M16A2_RandomTest(MT48LC16M16A2_T *Dev)
{
	uint16_t Data;
	uint32_t i;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	/*Fill memory with random data from 0 to 0xFFFF*/
	srand(0);
	for (i = 0; i < MT48LC16M16A2_SIZE ; i+=2)
	{
			*(__IO uint16_t*) (MT48LC16M16A2_ADDR + i) = rand() % 0xFFFF; 
	}
	
	/*Check random data in memory*/
	srand(0);
	for (i = 0; i < MT48LC16M16A2_SIZE ; i+=2)
	{
		Data = *(__IO uint16_t*) (MT48LC16M16A2_ADDR + i);
		if (Data != rand() % 0xFFFF)
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

/* FMC initialization function */
int32_t MT48LC16M16A2_Init(MT48LC16M16A2_T *Dev, const char *DevId)
{
  FMC_SDRAM_TimingTypeDef SdramTiming;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
  /** Perform the SDRAM1 memory initialization sequence
  */
  Dev->hsdram.Instance = FMC_SDRAM_DEVICE;
  /* Dev->hsdram.Init */
  Dev->hsdram.Init.SDBank = MT48LC16M16A2_FMC_BANK;
  Dev->hsdram.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  Dev->hsdram.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  Dev->hsdram.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  Dev->hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  Dev->hsdram.Init.CASLatency = MT48LC16M16A2_FMC_CAS_LATENCY;
  Dev->hsdram.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  Dev->hsdram.Init.SDClockPeriod = MT48LC16M16A2_FMC_CLOCK_PERIOD;
  Dev->hsdram.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  Dev->hsdram.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
	
	/* SdramTiming */
  SdramTiming.LoadToActiveDelay = MT48LC16M16A2_tMRD_CLK; 
  SdramTiming.ExitSelfRefreshDelay = MT48LC16M16A2_tXSR_CLK;
  SdramTiming.SelfRefreshTime = MT48LC16M16A2_tRAS_CLK;
  SdramTiming.RowCycleDelay = MT48LC16M16A2_tRC_CLK;
  SdramTiming.WriteRecoveryTime = MT48LC16M16A2_tWR_CLK;
  SdramTiming.RPDelay = MT48LC16M16A2_tRP_CLK;
  SdramTiming.RCDDelay = MT48LC16M16A2_tRCD_CLK;

  if (HAL_SDRAM_Init(&Dev->hsdram, &SdramTiming) != HAL_OK)
		goto error;
	
	MT48LC16M16A2_InitSequence(Dev);
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}
