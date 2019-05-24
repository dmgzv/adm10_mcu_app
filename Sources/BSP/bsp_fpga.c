#include "bsp_fpga.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include <string.h>

BSP_FPGA_IFACE_T fpga_iface;
BSP_FPGA_MAXV_T fpga_maxv;
BSP_FPGA_CLIENT_T fpga_client[2];
BSP_FPGA_LINE_T fpga_line;

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

int32_t BSP_FPGA_IFACE_Init(BSP_FPGA_IFACE_T *Dev)
{
	FMC_NORSRAM_TimingTypeDef Timing;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
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
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

void BSP_FPGA_IFACE_Deinit(BSP_FPGA_IFACE_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
}

int32_t BSP_FPGA_MAXV_Init(BSP_FPGA_MAXV_T *Dev)
{
	uint8_t SwVersion;
	uint8_t PowerUpMarker;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	PowerUpMarker = *((uint8_t*)BSP_FPGA_MAXV_ADDR + 0x03);
	if (PowerUpMarker != 0xA5)
		goto error;
	
	SwVersion = *((uint8_t*)BSP_FPGA_MAXV_ADDR);
	snprintf(Dev->SwVersion, sizeof(Dev->SwVersion), "%d", SwVersion);
	Dev->Status = *((uint8_t*)BSP_FPGA_MAXV_ADDR + 0x01);
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_MAXV_Update(BSP_FPGA_MAXV_T *Dev)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Dev->Status = *((uint8_t*)BSP_FPGA_MAXV_ADDR + 0x01);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

void BSP_FPGA_MAXV_Deinit(BSP_FPGA_MAXV_T *Dev)
{
	if (!Dev)
		return;	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
}

int32_t BSP_FPGA_LINE_Init(BSP_FPGA_LINE_T *Dev)
{
	uint8_t SwVersion[4];
	uint32_t i;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	for (i = 0; i < 4; i++)
	{
		SwVersion[i] = *((char*)BSP_FPGA_LINE_ADDR + i);
	}
	sprintf(Dev->SwVersion, "%d.%d.%d.%d", SwVersion[1], SwVersion[0], SwVersion[3], SwVersion[2]);
	
	for (i = 0; i < BSP_FPGA_LINE_IFACE_CNT; i++)
	{
		//if (/*(i == 1) || (i = 2) ||*/ (i == 3))
		//		continue;
		//*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
		//*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
		//*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x01;
		//*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x10;
		//osDelay(5);
		*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
		*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
		*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
		*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x02;
		osDelay(5);
		*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x01;
		*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
		*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
		*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x02;
		osDelay(5);
		*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
		*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
		*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
		*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x03;
		osDelay(5);
		Dev->Iface[i].__Loopback = BSP_FPGA_LINE_LOOPBACK_NO;
	}
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t BSP_FPGA_LINE_Update(BSP_FPGA_LINE_T *Dev)
{
	uint32_t i, j;
	uint8_t Status;
	uint16_t OTU2BIP, OTU2FECUncorr, ODU2BIP, ODU1BIP, ODU0BIP;
	uint32_t OTU2FECCorr;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	for (i = 0; i < BSP_FPGA_LINE_IFACE_CNT; i++)
	{
		
		//if (/*(i == 1) || (i = 2) ||*/ (i == 3))
		//		continue;
		
		Dev->Iface[i].OTU2Alarms = *((uint16_t*)BSP_FPGA_LINE_IFACE_ADDR(i));
		
		OTU2BIP = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x02));
		//OTU2BIP = ((OTU2BIP & 0xFF00) >> 8) + ((OTU2BIP & 0xFF) << 8);
		Dev->Iface[i].OTU2BIP = OTU2BIP - Dev->Iface[i].__OTU2BIP;
		Dev->Iface[i].__OTU2BIP = OTU2BIP;
		
		OTU2FECCorr = *((uint32_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x04));
		//OTU2FECCorr = ((OTU2FECCorr & 0xFF000000) >> 24) + ((OTU2FECCorr & 0x00FF0000) >> 8) + ((OTU2FECCorr & 0x0000FF00) << 8) + ((OTU2FECCorr & 0x000000FF) << 24);
		Dev->Iface[i].OTU2FECCorr = OTU2FECCorr - Dev->Iface[i].__OTU2FECCorr;
		Dev->Iface[i].__OTU2FECCorr = OTU2FECCorr;
		
		OTU2FECUncorr = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x08));
		//OTU2FECUncorr = ((OTU2FECUncorr & 0xFF00) >> 8) + ((OTU2FECUncorr & 0xFF) << 8);
		Dev->Iface[i].OTU2FECUncorr = OTU2FECUncorr - Dev->Iface[i].__OTU2FECUncorr;
		Dev->Iface[i].__OTU2FECUncorr = OTU2FECUncorr;
		
		Dev->Iface[i].ODU2Alarms = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x10));
		
		ODU2BIP = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x12));
		//ODU2BIP = ((ODU2BIP & 0xFF00) >> 8) + ((ODU2BIP & 0xFF) << 8);
		Dev->Iface[i].ODU2BIP = ODU2BIP - Dev->Iface[i].__ODU2BIP;
		Dev->Iface[i].__ODU2BIP = ODU2BIP;
		
		for (j = 0; j < BSP_FPGA_LINE_IFACE_ODU1_CNT; j++)
		{
			Dev->Iface[i].ODU1Alarms[j] = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x18 + (0x08 * j)));
			
			ODU1BIP = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x18 + (0x08 * j) + 2));
			//ODU1BIP = ((ODU1BIP & 0xFF00) >> 8) + ((ODU1BIP & 0xFF) << 8);
			Dev->Iface[i].ODU1BIP[j] = ODU1BIP - Dev->Iface[i].__ODU1BIP[j];
			Dev->Iface[i].__ODU1BIP[j] = ODU1BIP;
		}
		
		for (j = 0; j < BSP_FPGA_LINE_IFACE_ODU0_CNT; j++)
		{
			Dev->Iface[i].ODU0Alarms[j] = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x38 + (0x08 * j)));
			
			ODU0BIP = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x38 + (0x08 * j) + 2));
			//ODU0BIP = ((ODU0BIP & 0xFF00) >> 8) + ((ODU0BIP & 0xFF) << 8);
			Dev->Iface[i].ODU0BIP[j] = ODU0BIP - Dev->Iface[i].__ODU0BIP[j];
			Dev->Iface[i].__ODU0BIP[j] = ODU0BIP;
		}
		
		if (Dev->Iface[i].__Loopback != Dev->Iface[i].Loopback)
		{
			if (Dev->Iface[i].Loopback == BSP_FPGA_LINE_LOOPBACK_NO)
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x02;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x01;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x02;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x03;
				osDelay(5);
				Dev->Iface[i].__Loopback = Dev->Iface[i].Loopback;
			}
			else if (Dev->Iface[i].Loopback == BSP_FPGA_LINE_LOOPBACK_SERIAL)
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x02;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x01;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x02;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x01;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x03;
				osDelay(5);
				Dev->Iface[i].__Loopback = Dev->Iface[i].Loopback;
			}
			else if (Dev->Iface[i].Loopback == BSP_FPGA_LINE_LOOPBACK_POST_CDR)
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x03;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x02;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x01;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x01;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x02;
				osDelay(5);
				Dev->Iface[i].__Loopback = Dev->Iface[i].Loopback;
			}
			else if (Dev->Iface[i].Loopback == BSP_FPGA_LINE_LOOPBACK_PRE_CDR)
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x03;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x01;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x02;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x01;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x02;
				osDelay(5);
				Dev->Iface[i].__Loopback = Dev->Iface[i].Loopback;
			}
		}		
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

void BSP_FPGA_LINE_Deinit(BSP_FPGA_LINE_T *Dev)
{
	if (!Dev)
		return;	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
}

int32_t BSP_FPGA_CLIENT_Init(BSP_FPGA_CLIENT_T *Dev, uint32_t Idx)
{
	uint32_t i;
	uint8_t SwVersion[4];
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->Idx = Idx;
	for (i = 0; i < 4; i++)
	{
		SwVersion[i] = *((char*)BSP_FPGA_CLIENT_ADDR(Dev->Idx) + i);
	}
	sprintf(Dev->SwVersion, "%d.%d.%d.%d", SwVersion[1], SwVersion[0], SwVersion[3], SwVersion[2]);
	
	for (i = 0; i < BSP_FPGA_CLIENT_IFACE_CNT; i++)
	{
		//*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x00;
		//*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
		//*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x01;
		//*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x10;
		//osDelay(5);
	}
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t BSP_FPGA_CLIENT_Update(BSP_FPGA_CLIENT_T *Dev)
{
	uint32_t i;
	uint8_t Status;
	uint16_t SDHBIP8, SDHBIP24, EthRxErrs, EthTxErrs;
	uint32_t EthRxPkts, EthRxOcts, EthTxPkts, EthTxOcts;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	for (i = 0; i < BSP_FPGA_CLIENT_IFACE_CNT; i++)
	{
		//if (i > 0)
		//	continue;
		Dev->Iface[i].__Mode = *((uint8_t*)(BSP_FPGA_CLIENT_IFACE_TRANS_ADDR(Dev->Idx, i)+ 0x00));
		Dev->Iface[i].__PMAStatus = *((uint8_t*)(BSP_FPGA_CLIENT_IFACE_TRANS_ADDR(Dev->Idx, i)+ 0x01));
		
		Dev->Iface[i].SDHAlarms = *((uint16_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i)+ 0x02));
		
		SDHBIP8 = *((uint16_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x04));
		//SDHBIP8 = ((SDHBIP8 & 0xFF00) >> 8) + ((SDHBIP8 & 0xFF) << 8);
		Dev->Iface[i].SDHBIP8 = SDHBIP8 - Dev->Iface[i].__SDHBIP8;
		Dev->Iface[i].__SDHBIP8 = SDHBIP8;
		
		SDHBIP24 = *((uint16_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x06));
		//SDHBIP24 = ((SDHBIP24 & 0xFF00) >> 8) + ((SDHBIP24 & 0xFF) << 8);
		Dev->Iface[i].SDHBIP24 = SDHBIP24 - Dev->Iface[i].__SDHBIP24;
		Dev->Iface[i].__SDHBIP24 = SDHBIP24;
		
		EthTxPkts = *((uint32_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x08));
		//EthTxPkts = ((EthTxPkts & 0xFF000000) >> 24) + ((EthTxPkts & 0x00FF0000) >> 8) + ((EthTxPkts & 0x0000FF00) << 8) + ((EthTxPkts & 0x000000FF) << 24);
		Dev->Iface[i].EthTxPkts = EthTxPkts - Dev->Iface[i].__EthTxPkts;
		Dev->Iface[i].__EthTxPkts = EthTxPkts;
		
		EthRxPkts = *((uint32_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x0C));
		//EthRxPkts = ((EthRxPkts & 0xFF000000) >> 24) + ((EthRxPkts & 0x00FF0000) >> 8) + ((EthRxPkts & 0x0000FF00) << 8) + ((EthRxPkts & 0x000000FF) << 24);
		Dev->Iface[i].EthRxPkts = EthRxPkts - Dev->Iface[i].__EthRxPkts;
		Dev->Iface[i].__EthRxPkts = EthRxPkts;
		
		EthTxOcts = *((uint32_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x10));
		//EthTxOcts = ((EthTxOcts & 0xFF000000) >> 24) + ((EthTxOcts & 0x00FF0000) >> 8) + ((EthTxOcts & 0x0000FF00) << 8) + ((EthTxOcts & 0x000000FF) << 24);
		Dev->Iface[i].EthTxOcts = EthTxOcts - Dev->Iface[i].__EthTxOcts;
		Dev->Iface[i].__EthTxOcts = EthTxOcts;
		
		EthRxOcts = *((uint32_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x14));
		//EthRxOcts = ((EthRxOcts & 0xFF000000) >> 24) + ((EthRxOcts & 0x00FF0000) >> 8) + ((EthRxOcts & 0x0000FF00) << 8) + ((EthRxOcts & 0x000000FF) << 24);
		Dev->Iface[i].EthRxOcts = EthRxOcts - Dev->Iface[i].__EthRxOcts;
		Dev->Iface[i].__EthRxOcts = EthRxOcts;
		
		EthTxErrs = *((uint16_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x18));
		//EthTxErrs = ((EthTxErrs & 0xFF00) >> 8) + ((EthTxErrs & 0xFF) << 8);
		Dev->Iface[i].EthTxErrs = EthTxErrs - Dev->Iface[i].__EthTxErrs;
		Dev->Iface[i].__EthTxErrs = EthTxErrs;
		
		EthRxErrs = *((uint16_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x1A));
		//EthRxErrs = ((EthRxErrs & 0xFF00) >> 8) + ((EthRxErrs & 0xFF) << 8);
		Dev->Iface[i].EthRxErrs = EthRxErrs - Dev->Iface[i].__EthRxErrs;
		Dev->Iface[i].__EthRxErrs = EthRxErrs;
				
		if (Dev->Iface[i].__Mode !=  Dev->Iface[i].Mode)
		{
			Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
			if (Status == 0)
				continue;
			*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x00;
			*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
			*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = Dev->Iface[i].Mode;
			*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x01;
			osDelay(5);
		}
		
		if ((Dev->Iface[i].PMAStatus & BSP_FPGA_CLIENT_IFACE_PMASTATUS_LOOPBACK_MASK) != (Dev->Iface[i].__PMAStatus & BSP_FPGA_CLIENT_IFACE_PMASTATUS_LOOPBACK_MASK))
		{	
			if (Dev->Iface[i].PMAStatus & BSP_FPGA_CLIENT_IFACE_PMASTATUS_LOOPBACK_SERIAL_MASK)
			{
				Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x02;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x01;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x02;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x01;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x03;
				osDelay(5);
			}
			else if (Dev->Iface[i].PMAStatus & BSP_FPGA_CLIENT_IFACE_PMASTATUS_LOOPBACK_POST_CDR_MASK)
			{
				Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x02;
				osDelay(5);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x03;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x01;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x01;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x02;
				osDelay(5);
			}
			else if (Dev->Iface[i].PMAStatus & BSP_FPGA_CLIENT_IFACE_PMASTATUS_LOOPBACK_PRE_CDR_MASK)
			{
				Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x01;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x02;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x03;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x01;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x02;
				osDelay(5);
			}
			else
			{
				Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x02;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x01;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x02;
				osDelay(5);
				Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = 0x00;
				*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x03;
				osDelay(5);
			}
		}
	}

	BSP_DEV_UNLOCK(Dev);
	return 0;
}

void BSP_FPGA_CLIENT_Deinit(BSP_FPGA_CLIENT_T *Dev)
{
	if (!Dev)
		return;	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
}

static int32_t BSP_FPGA_CheckConditions(void)
{
	if (BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
		return -1;
	if (BSP_DEV_GET_STATE(&pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	return 0;
}

void BSP_FPGA_Process(void)
{
	uint32_t i;
	
	if (BSP_FPGA_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&fpga_iface) != BSP_DEV_STATE_RESET)
		{
			BSP_FPGA_IFACE_Deinit(&fpga_iface);
			BSP_FPGA_MAXV_Deinit(&fpga_maxv);
			BSP_FPGA_LINE_Deinit(&fpga_line);
			for (i = 0; i < BSP_FPGA_CLIENT_CNT; i++)
			{
				BSP_FPGA_CLIENT_Deinit(&fpga_client[i]);
			}
		}
	}
	else
	{
		/* External faults absent */
		if(BSP_DEV_GET_STATE(&fpga_iface) != BSP_DEV_STATE_OK)
		{
			BSP_FPGA_IFACE_Init(&fpga_iface);
		}
		else
		{
			if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
			{
				BSP_FPGA_MAXV_Init(&fpga_maxv);
			}
			else
			{
				BSP_FPGA_MAXV_Update(&fpga_maxv);
				if (fpga_maxv.Status & BSP_FPGA_MAXV_LINE_STATUS_MASK)
				{
					if (BSP_DEV_GET_STATE(&fpga_line)  != BSP_DEV_STATE_OK)
					{
						BSP_FPGA_LINE_Init(&fpga_line);
					}
					else
					{
						BSP_FPGA_LINE_Update(&fpga_line);
					}
				}
				for (i = 0; i < BSP_FPGA_CLIENT_CNT; i++)
				{
					if (fpga_maxv.Status & BSP_FPGA_MAXV_CLIENT_STATUS_MASK(i))
					{
						if (BSP_DEV_GET_STATE(&fpga_client[i]) != BSP_DEV_STATE_OK)
						{
							BSP_FPGA_CLIENT_Init(&fpga_client[i], i);
						}
						else
						{
							BSP_FPGA_CLIENT_Update(&fpga_client[i]);
						}
					}
				}
			}
		}
	}
}
