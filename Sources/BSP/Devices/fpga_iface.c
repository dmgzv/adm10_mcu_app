#include "fpga_iface.h"
#include "log.h"
#include <string.h>

void HAL_SRAM_MspInit(SRAM_HandleTypeDef* hsram)
{
/* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	
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
  PE7   ------> FMC_D4
  PE7   ------> FMC_DA4
  PE8   ------> FMC_D5
  PE8   ------> FMC_DA5
  PE9   ------> FMC_D6
  PE9   ------> FMC_DA6
  PE10   ------> FMC_D7
  PE10   ------> FMC_DA7
  PE11   ------> FMC_D8
  PE11   ------> FMC_DA8
  PE12   ------> FMC_D9
  PE12   ------> FMC_DA9
  PE13   ------> FMC_D10
  PE13   ------> FMC_DA10
  PE14   ------> FMC_D11
  PE14   ------> FMC_DA11
  PE15   ------> FMC_D12
  PE15   ------> FMC_DA12
  PD8   ------> FMC_D13
  PD8   ------> FMC_DA13
  PD9   ------> FMC_D14
  PD9   ------> FMC_DA14
  PD10   ------> FMC_D15
  PD11   ------> FMC_A16
  PD12   ------> FMC_A17
  PD13   ------> FMC_A18
  PD14   ------> FMC_D0
  PD14   ------> FMC_DA0
  PD15   ------> FMC_D1
  PD15   ------> FMC_DA1
  PG2   ------> FMC_A12
  PG3   ------> FMC_A13
  PG4   ------> FMC_A14
  PG5   ------> FMC_A15
  PD0   ------> FMC_D2
  PD0   ------> FMC_DA2
  PD1   ------> FMC_D3
  PD1   ------> FMC_DA3
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

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef *hsram)
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
  PE7   ------> FMC_D4
  PE7   ------> FMC_DA4
  PE8   ------> FMC_D5
  PE8   ------> FMC_DA5
  PE9   ------> FMC_D6
  PE9   ------> FMC_DA6
  PE10   ------> FMC_D7
  PE10   ------> FMC_DA7
  PE11   ------> FMC_D8
  PE11   ------> FMC_DA8
  PE12   ------> FMC_D9
  PE12   ------> FMC_DA9
  PE13   ------> FMC_D10
  PE13   ------> FMC_DA10
  PE14   ------> FMC_D11
  PE14   ------> FMC_DA11
  PE15   ------> FMC_D12
  PE15   ------> FMC_DA12
  PD8   ------> FMC_D13
  PD8   ------> FMC_DA13
  PD9   ------> FMC_D14
  PD9   ------> FMC_DA14
  PD10   ------> FMC_D15
  PD11   ------> FMC_A16
  PD12   ------> FMC_A17
  PD13   ------> FMC_A18
  PD14   ------> FMC_D0
  PD14   ------> FMC_DA0
  PD15   ------> FMC_D1
  PD15   ------> FMC_DA1
  PG2   ------> FMC_A12
  PG3   ------> FMC_A13
  PG4   ------> FMC_A14
  PG5   ------> FMC_A15
  PD0   ------> FMC_D2
  PD0   ------> FMC_DA2
  PD1   ------> FMC_D3
  PD1   ------> FMC_DA3
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

int32_t FPGA_IFACE_Init(FPGA_IFACE_T *Dev)
{
	FMC_NORSRAM_TimingTypeDef Timing;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	/** Perform the SRAM3 memory initialization sequence
  */
  Dev->hsram.Instance = FMC_NORSRAM_DEVICE;
  Dev->hsram.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* Dev->hsram.Init */
  Dev->hsram.Init.NSBank = FMC_NORSRAM_BANK3;
  Dev->hsram.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_ENABLE;
  Dev->hsram.Init.MemoryType = FMC_MEMORY_TYPE_PSRAM;
  Dev->hsram.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_8;
  Dev->hsram.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  Dev->hsram.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  Dev->hsram.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
  Dev->hsram.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  Dev->hsram.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  Dev->hsram.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  Dev->hsram.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  Dev->hsram.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  Dev->hsram.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  Dev->hsram.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  Dev->hsram.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 4;
  Timing.AddressHoldTime = 2;
  Timing.DataSetupTime = 8;
  Timing.BusTurnAroundDuration = 2;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&Dev->hsram, &Timing, NULL) != HAL_OK)
		goto error;
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void FPGA_IFACE_Deinit(FPGA_IFACE_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
