#include "nor.h"

static NOR_HandleTypeDef hnor1;
static NOR_HandleTypeDef hnor2;
static NOR_HandleTypeDef hnor3;

void HAL_NOR_MspInit(NOR_HandleTypeDef* hnor){
	GPIO_InitTypeDef GPIO_InitStruct;
	/* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_ENABLE();
  
  /** FMC GPIO Configuration  
  PE2   ------> FMC_A23
  PE3   ------> FMC_A19
  PE4   ------> FMC_A20
  PE5   ------> FMC_A21
  PE6   ------> FMC_A22
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PF12   ------> FMC_A6
  PF13   ------> FMC_A7
  PF14   ------> FMC_A8
  PF15   ------> FMC_A9
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11
  PE7   ------> FMC_DA4
  PE7   ------> FMC_D4
  PE8   ------> FMC_DA5
  PE8   ------> FMC_D5
  PE9   ------> FMC_DA6
  PE9   ------> FMC_D6
  PE10   ------> FMC_DA7
  PE10   ------> FMC_D7
  PE11   ------> FMC_DA8
  PE11   ------> FMC_D8
  PE12   ------> FMC_DA9
  PE12   ------> FMC_D9
  PE13   ------> FMC_DA10
  PE13   ------> FMC_D10
  PE14   ------> FMC_DA11
  PE14   ------> FMC_D11
  PE15   ------> FMC_DA12
  PE15   ------> FMC_D12
  PD8   ------> FMC_DA13
  PD8   ------> FMC_D13
  PD9   ------> FMC_DA14
  PD9   ------> FMC_D14
  PD10   ------> FMC_DA15
  PD10   ------> FMC_D15
  PD11   ------> FMC_A16
  PD12   ------> FMC_A17
  PD13   ------> FMC_A18
  PD14   ------> FMC_DA0
  PD14   ------> FMC_D0
  PD15   ------> FMC_DA1
  PD15   ------> FMC_D1
  PG2   ------> FMC_A12
  PG3   ------> FMC_A13
  PG4   ------> FMC_A14
  PG5   ------> FMC_A15
  PD0   ------> FMC_DA2
  PD0   ------> FMC_D2
  PD1   ------> FMC_DA3
  PD1   ------> FMC_D3
  PD4   ------> FMC_NOE
  PD5   ------> FMC_NWE
  PD7   ------> FMC_NE1
  PG9   ------> FMC_NE2
  PG10   ------> FMC_NE3
  PG13   ------> FMC_A24
  PB7   ------> FMC_NL
  */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_9|GPIO_PIN_10 
                          |GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
}

void HAL_NOR_MspDeInit(NOR_HandleTypeDef *hnor)
{  
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_DISABLE();
  
  /** FMC GPIO Configuration  
  PE2   ------> FMC_A23
  PE3   ------> FMC_A19
  PE4   ------> FMC_A20
  PE5   ------> FMC_A21
  PE6   ------> FMC_A22
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PF12   ------> FMC_A6
  PF13   ------> FMC_A7
  PF14   ------> FMC_A8
  PF15   ------> FMC_A9
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11
  PE7   ------> FMC_DA4
  PE7   ------> FMC_D4
  PE8   ------> FMC_DA5
  PE8   ------> FMC_D5
  PE9   ------> FMC_DA6
  PE9   ------> FMC_D6
  PE10   ------> FMC_DA7
  PE10   ------> FMC_D7
  PE11   ------> FMC_DA8
  PE11   ------> FMC_D8
  PE12   ------> FMC_DA9
  PE12   ------> FMC_D9
  PE13   ------> FMC_DA10
  PE13   ------> FMC_D10
  PE14   ------> FMC_DA11
  PE14   ------> FMC_D11
  PE15   ------> FMC_DA12
  PE15   ------> FMC_D12
  PD8   ------> FMC_DA13
  PD8   ------> FMC_D13
  PD9   ------> FMC_DA14
  PD9   ------> FMC_D14
  PD10   ------> FMC_DA15
  PD10   ------> FMC_D15
  PD11   ------> FMC_A16
  PD12   ------> FMC_A17
  PD13   ------> FMC_A18
  PD14   ------> FMC_DA0
  PD14   ------> FMC_D0
  PD15   ------> FMC_DA1
  PD15   ------> FMC_D1
  PG2   ------> FMC_A12
  PG3   ------> FMC_A13
  PG4   ------> FMC_A14
  PG5   ------> FMC_A15
  PD0   ------> FMC_DA2
  PD0   ------> FMC_D2
  PD1   ------> FMC_DA3
  PD1   ------> FMC_D3
  PD4   ------> FMC_NOE
  PD5   ------> FMC_NWE
  PD7   ------> FMC_NE1
  PG9   ------> FMC_NE2
  PG10   ------> FMC_NE3
  PG13   ------> FMC_A24
  PB7   ------> FMC_NL
  */
  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13 
                          |GPIO_PIN_14|GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_9|GPIO_PIN_10 
                          |GPIO_PIN_13);

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_5 
                          |GPIO_PIN_7);

  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);
}

/* FMC initialization function */
int32_t NOR_Init(void)
{
  FMC_NORSRAM_TimingTypeDef Timing;
  FMC_NORSRAM_TimingTypeDef ExtTiming;

  /** Perform the NOR1 memory initialization sequence
  */
  hnor1.Instance = FMC_NORSRAM_DEVICE;
  hnor1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hnor1.Init */
  hnor1.Init.NSBank = FMC_NORSRAM_BANK1;
  hnor1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hnor1.Init.MemoryType = FMC_MEMORY_TYPE_NOR;
  hnor1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hnor1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hnor1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hnor1.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
  hnor1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hnor1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hnor1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hnor1.Init.ExtendedMode = FMC_EXTENDED_MODE_ENABLE;
  hnor1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hnor1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hnor1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hnor1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 2;
  Timing.AddressHoldTime = 1;
  Timing.DataSetupTime = 10;
  Timing.BusTurnAroundDuration = 1;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_D;
  /* ExtTiming */
  ExtTiming.AddressSetupTime = 2;
  ExtTiming.AddressHoldTime = 1;
  ExtTiming.DataSetupTime = 10;
  ExtTiming.BusTurnAroundDuration = 1;
  ExtTiming.CLKDivision = 16;
  ExtTiming.DataLatency = 17;
  ExtTiming.AccessMode = FMC_ACCESS_MODE_D;

  if (HAL_NOR_Init(&hnor1, &Timing, &ExtTiming) != HAL_OK)
		return -1;

  /** Perform the NOR2 memory initialization sequence
  */
  hnor2.Instance = FMC_NORSRAM_DEVICE;
  hnor2.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hnor2.Init */
  hnor2.Init.NSBank = FMC_NORSRAM_BANK2;
  hnor2.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hnor2.Init.MemoryType = FMC_MEMORY_TYPE_NOR;
  hnor2.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  hnor2.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hnor2.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hnor2.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
  hnor2.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hnor2.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hnor2.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hnor2.Init.ExtendedMode = FMC_EXTENDED_MODE_ENABLE;
  hnor2.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hnor2.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hnor2.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hnor2.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 2;
  Timing.AddressHoldTime = 1;
  Timing.DataSetupTime = 10;
  Timing.BusTurnAroundDuration = 1;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_D;
  /* ExtTiming */
  ExtTiming.AddressSetupTime = 2;
  ExtTiming.AddressHoldTime = 1;
  ExtTiming.DataSetupTime = 10;
  ExtTiming.BusTurnAroundDuration = 1;
  ExtTiming.CLKDivision = 16;
  ExtTiming.DataLatency = 17;
  ExtTiming.AccessMode = FMC_ACCESS_MODE_D;

  if (HAL_NOR_Init(&hnor2, &Timing, &ExtTiming) != HAL_OK)
		return -1;

  /** Perform the NOR3 memory initialization sequence
  */
  hnor3.Instance = FMC_NORSRAM_DEVICE;
  hnor3.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hnor3.Init */
  hnor3.Init.NSBank = FMC_NORSRAM_BANK3;
  hnor3.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_ENABLE;
  hnor3.Init.MemoryType = FMC_MEMORY_TYPE_NOR;
  hnor3.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_8;
  hnor3.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hnor3.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hnor3.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
  hnor3.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hnor3.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hnor3.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hnor3.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hnor3.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hnor3.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hnor3.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hnor3.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 2;
  Timing.AddressHoldTime = 2;
  Timing.DataSetupTime = 4;
  Timing.BusTurnAroundDuration = 2;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_NOR_Init(&hnor3, &Timing, NULL) != HAL_OK)
		return -1;
	return 0;
}
