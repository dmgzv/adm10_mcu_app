#include "s29gl01gs.h"
#include "log.h"

#ifdef BSP_CONFIG_USE_FREERTOS
#define	S29GL01GS_msDelay(ms) 		osDelay(ms)
#else
#define	S29GL01GS_msDelay(ms) 		HAL_Delay(ms)
#endif

#define S29GL01GS_MANUFACTURER_CODE   ((uint16_t)0x0089)
#define S29GL01GS_DEVICE_CODE1        ((uint16_t)0x227E)
#define S29GL01GS_DEVICE_CODE2        ((uint16_t)0x2228)
#define S29GL01GS_DEVICE_CODE3        ((uint16_t)0x2201)

#define S29GL01GS_NOR_BANK_ADDR       ((uint32_t)0x60000000)
#define S29GL01GS_NOR_BANK_SIZE       ((uint32_t)0x04000000)

void HAL_NOR_MspInit(NOR_HandleTypeDef* hnor)
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

/* FMC initialization function */
int32_t S29GL01GS_Init(S29GL01GS_T *Dev)
{
  FMC_NORSRAM_TimingTypeDef Timing;
  FMC_NORSRAM_TimingTypeDef ExtTiming;

	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
  /** Perform the NOR1 memory initialization sequence
  */
  Dev->hnor[0].Instance = FMC_NORSRAM_DEVICE;
  Dev->hnor[0].Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* Dev->hnor[0].Init */
  Dev->hnor[0].Init.NSBank = FMC_NORSRAM_BANK1;
  Dev->hnor[0].Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  Dev->hnor[0].Init.MemoryType = FMC_MEMORY_TYPE_NOR;
  Dev->hnor[0].Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  Dev->hnor[0].Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  Dev->hnor[0].Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  Dev->hnor[0].Init.WrapMode = FMC_WRAP_MODE_DISABLE;
  Dev->hnor[0].Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  Dev->hnor[0].Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  Dev->hnor[0].Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  Dev->hnor[0].Init.ExtendedMode = FMC_EXTENDED_MODE_ENABLE;
  Dev->hnor[0].Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  Dev->hnor[0].Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  Dev->hnor[0].Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  Dev->hnor[0].Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
	/*
		todo: Set correct values timing values
		HCLK == 180 MHz
		tHCLK = 5,5 ns
		
		tWC == 60 ns
		tWP = 20 ns
		tRC = 100 ns
		tAVQV = 100 ns
	*/
  Timing.AddressSetupTime = 4;
  Timing.AddressHoldTime = 4;
  Timing.DataSetupTime = 20; // tWP = 
  Timing.BusTurnAroundDuration = 4;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_D;
  /* ExtTiming */
  ExtTiming.AddressSetupTime = 4;
  ExtTiming.AddressHoldTime = 4;
  ExtTiming.DataSetupTime = 20;
  ExtTiming.BusTurnAroundDuration = 4;
  ExtTiming.CLKDivision = 16;
  ExtTiming.DataLatency = 17;
  ExtTiming.AccessMode = FMC_ACCESS_MODE_D;

  if (HAL_NOR_Init(&Dev->hnor[0], &Timing, &ExtTiming) != HAL_OK)
		goto error;
	
	if (HAL_NOR_Read_ID(&Dev->hnor[0], &Dev->Id) != HAL_OK)
		goto error;
	
	/* Test the NOR ID correctness */
  if((Dev->Id.Manufacturer_Code != (uint16_t)S29GL01GS_MANUFACTURER_CODE) ||
     (Dev->Id.Device_Code1 != (uint16_t)S29GL01GS_DEVICE_CODE1) ||
     (Dev->Id.Device_Code2 != (uint16_t)S29GL01GS_DEVICE_CODE2) ||
     (Dev->Id.Device_Code3 != (uint16_t)S29GL01GS_DEVICE_CODE3))
  {
    /* NOR ID not correct */
    goto error;
  }
	
	if (HAL_NOR_ReturnToReadMode(&Dev->hnor[0]) != HAL_NOR_STATUS_SUCCESS)
		goto error;

  /** Perform the NOR2 memory initialization sequence
  */
  Dev->hnor[1].Instance = FMC_NORSRAM_DEVICE;
  Dev->hnor[1].Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* Dev->hnor[1].Init */
  Dev->hnor[1].Init.NSBank = FMC_NORSRAM_BANK2;
  Dev->hnor[1].Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  Dev->hnor[1].Init.MemoryType = FMC_MEMORY_TYPE_NOR;
  Dev->hnor[1].Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
  Dev->hnor[1].Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  Dev->hnor[1].Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  Dev->hnor[1].Init.WrapMode = FMC_WRAP_MODE_DISABLE;
  Dev->hnor[1].Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  Dev->hnor[1].Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  Dev->hnor[1].Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  Dev->hnor[1].Init.ExtendedMode = FMC_EXTENDED_MODE_ENABLE;
  Dev->hnor[1].Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  Dev->hnor[1].Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  Dev->hnor[1].Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  Dev->hnor[1].Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 4;
  Timing.AddressHoldTime = 4;
  Timing.DataSetupTime = 20;
  Timing.BusTurnAroundDuration = 4;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_D;
  /* ExtTiming */
  ExtTiming.AddressSetupTime = 4;
  ExtTiming.AddressHoldTime = 4;
  ExtTiming.DataSetupTime = 20;
  ExtTiming.BusTurnAroundDuration = 4;
  ExtTiming.CLKDivision = 16;
  ExtTiming.DataLatency = 17;
  ExtTiming.AccessMode = FMC_ACCESS_MODE_D;

  if (HAL_NOR_Init(&Dev->hnor[1], &Timing, &ExtTiming) != HAL_OK)
		goto error;
	
	if (HAL_NOR_Read_ID(&Dev->hnor[1], &Dev->Id) != HAL_OK)
		goto error;
	
	/* Test the NOR ID correctness */
  if((Dev->Id.Manufacturer_Code != (uint16_t)S29GL01GS_MANUFACTURER_CODE) ||
     (Dev->Id.Device_Code1 != (uint16_t)S29GL01GS_DEVICE_CODE1) ||
     (Dev->Id.Device_Code2 != (uint16_t)S29GL01GS_DEVICE_CODE2) ||
     (Dev->Id.Device_Code3 != (uint16_t)S29GL01GS_DEVICE_CODE3))
  {
    /* NOR ID not correct */
    goto error;
  }
	
	if (HAL_NOR_ReturnToReadMode(&Dev->hnor[1]) != HAL_NOR_STATUS_SUCCESS)
		goto error;
	
	LOG_Add("S29GL01GS Init: Ok");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	LOG_Add("S29GL01GS Init: Fail");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

void S29GL01GS_Deinit(S29GL01GS_T *Dev)
{
	if (!Dev)
		return;
	LOG_Add("S29GL01GS Deinit");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
}

int32_t S29GL01GS_Read(S29GL01GS_T *Dev, uint32_t Addr, void *Buffer, uint32_t Size)
{
	uint16_t Data;
	
	if ((!Dev) || (!Buffer))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	/* todo: Fix reading of data from several banks*/
	if (Addr < S29GL01GS_NOR_BANK_SIZE)
	{
		if (HAL_NOR_GetStatus(&Dev->hnor[0], S29GL01GS_NOR_BANK_ADDR, 0) != HAL_NOR_STATUS_SUCCESS)
			goto error;
		if (HAL_NOR_ReadBuffer(&Dev->hnor[0], Addr + S29GL01GS_NOR_BANK_ADDR, Buffer, Size / 2) != HAL_OK)
			goto error;		
		if (Size % 2)
		{
			if (HAL_NOR_Read(&Dev->hnor[0], (uint32_t*)(Addr + S29GL01GS_NOR_BANK_ADDR + (Size / 2)), &Data) != HAL_OK)
				goto error;
			((char*)Buffer)[Size - 1] = Data;
		}
	}
	else
	{
		if (HAL_NOR_GetStatus(&Dev->hnor[1], S29GL01GS_NOR_BANK_ADDR, 0) != HAL_NOR_STATUS_SUCCESS)
			goto error;
		if (HAL_NOR_ReadBuffer(&Dev->hnor[1], Addr + S29GL01GS_NOR_BANK_ADDR, Buffer, Size/2) != HAL_OK)
			goto error;
		if (Size % 2)
		{
			if (HAL_NOR_Read(&Dev->hnor[1], (uint32_t*)(Addr + S29GL01GS_NOR_BANK_ADDR + (Size / 2)), &Data) != HAL_OK)
				goto error;
			((char*)Buffer)[Size - 1] = Data;
		}
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t S29GL01GS_ProgramSector(S29GL01GS_T *Dev, uint32_t Addr, void *Buffer, uint32_t Size)
{
	uint16_t Data;
	uint16_t *pdata = NULL;
  uint32_t index  = 0;
  uint32_t startaddress = 0;
	
	if ((!Dev) || (!Buffer))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	/* todo: Fix programing of data to several banks*/
	if (Addr < S29GL01GS_NOR_BANK_SIZE)
	{
		pdata = Buffer;
		index = Size / 2;
		startaddress = Addr + S29GL01GS_NOR_BANK_ADDR;
		while(index > 0)
		{
			/* Read NOR device status */
			while(1)
			{
				if (HAL_NOR_GetStatus(&Dev->hnor[0], S29GL01GS_NOR_BANK_ADDR, 0) == HAL_NOR_STATUS_SUCCESS)
					break;
			}
			/* Write data to NOR */
			if (HAL_NOR_Program(&Dev->hnor[0], (uint32_t *)startaddress, pdata) != HAL_OK)
				goto error;
			/* Update the counters */
			index--;
			startaddress += 2;
			pdata++; 
		}
		if (Size % 2)
		{
			Data = 0xFF00 | ((char*)Buffer)[Size - 1];
			if (HAL_NOR_Program(&Dev->hnor[0], (uint32_t*)(Addr + S29GL01GS_NOR_BANK_ADDR + (Size / 2)), &Data) != HAL_OK)
				goto error;
		}
	}
	else
	{
		pdata = Buffer;
		index = Size / 2;
		startaddress = Addr + S29GL01GS_NOR_BANK_ADDR ;
		while(index > 0)
		{
			/* Read NOR device status */
			while(1)
			{
				if (HAL_NOR_GetStatus(&Dev->hnor[1], S29GL01GS_NOR_BANK_ADDR + S29GL01GS_NOR_BANK_SIZE, 0) == HAL_NOR_STATUS_SUCCESS)
					break;
			}		
			/* Write data to NOR */
			if (HAL_NOR_Program(&Dev->hnor[1], (uint32_t *)startaddress, pdata) != HAL_OK)
				goto error;
			/* Update the counters */
			index--;
			startaddress += 2;
			pdata++; 
		}
		if (Size % 2)
		{
			Data = 0xFF00 | ((char*)Buffer)[Size - 1];
			if (HAL_NOR_Program(&Dev->hnor[1], (uint32_t*)(Addr + S29GL01GS_NOR_BANK_ADDR + (Size / 2)), &Data) != HAL_OK)
				goto error;
		}
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t S29GL01GS_EraseSector(S29GL01GS_T *Dev, uint32_t Addr)
{
		if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	
	if (Addr < S29GL01GS_NOR_BANK_SIZE)
	{
		if (HAL_NOR_GetStatus(&Dev->hnor[0], S29GL01GS_NOR_BANK_ADDR, 0) != HAL_NOR_STATUS_SUCCESS)
			goto error;
		if (HAL_NOR_Erase_Block(&Dev->hnor[0], Addr, S29GL01GS_NOR_BANK_ADDR) != HAL_OK)
			goto error;
	}
	else
	{
		if (HAL_NOR_GetStatus(&Dev->hnor[1], S29GL01GS_NOR_BANK_ADDR, 0) != HAL_NOR_STATUS_SUCCESS)
			goto error;
		if (HAL_NOR_Erase_Block(&Dev->hnor[1], Addr, S29GL01GS_NOR_BANK_ADDR) != HAL_OK)
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t S29GL01GS_WaitReady(S29GL01GS_T *Dev)
{
	HAL_NOR_StatusTypeDef Status[2];
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	while (1)
	{
		Status[0] = HAL_NOR_GetStatus(&Dev->hnor[0], S29GL01GS_NOR_BANK_ADDR, 0);
		Status[1] = HAL_NOR_GetStatus(&Dev->hnor[1], S29GL01GS_NOR_BANK_ADDR, 0);
		if ((Status[0] == HAL_NOR_STATUS_SUCCESS) && (Status[1] == HAL_NOR_STATUS_SUCCESS))
			break;
		else if ((Status[0] == HAL_NOR_STATUS_ERROR) || (Status[1] == HAL_NOR_STATUS_ERROR))
			goto error;
		else
			S29GL01GS_msDelay(1);
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

