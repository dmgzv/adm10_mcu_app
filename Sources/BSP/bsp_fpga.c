#include "bsp_fpga.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_clk.h"
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
	
	PowerUpMarker = *((uint8_t*)(BSP_FPGA_MAXV_ADDR + 0x03));
	if (PowerUpMarker != 0xA5)
		goto error;
	
	SwVersion = *((uint8_t*)BSP_FPGA_MAXV_ADDR);
	snprintf(Dev->SwVersion, sizeof(Dev->SwVersion), "%d", SwVersion);
	Dev->Status = *((uint8_t*)(BSP_FPGA_MAXV_ADDR + 0x01));
	Dev->Mode = ((*((uint8_t*)(BSP_FPGA_MAXV_ADDR + 0x02))) >> 7) & 0x01;
	
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
	uint32_t Mode;
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Dev->Status = *((uint8_t*)(BSP_FPGA_MAXV_ADDR + 0x01));
	Mode = ((*((uint8_t*)(BSP_FPGA_MAXV_ADDR + 0x02))) >> 7) & 0x01;
	if (Dev->Mode != Mode)
	{
		*((uint8_t*)(BSP_FPGA_MAXV_ADDR + 0x02)) = ((Dev->Mode & 0x01) << 7) | (1 << 0);
		Dev->Status = 0;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

void BSP_FPGA_MAXV_Deinit(BSP_FPGA_MAXV_T *Dev)
{
	if (!Dev)
		return;	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
}

int32_t BSP_FPGA_LINE_Init(BSP_FPGA_LINE_T *Dev, uint32_t Mode)
{
	uint8_t Status, SwVersion[4];
	uint32_t i;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
	if (Status == 0)
		goto error;
	
	Dev->Mode = Mode;
	
	for (i = 0; i < 4; i++)
	{
		SwVersion[i] = *((char*)(BSP_FPGA_LINE_ADDR + i));
	}
	sprintf(Dev->SwVersion, "%d.%d.%d.%d", SwVersion[1], SwVersion[0], SwVersion[3], SwVersion[2]);
	
	for (i = 0; i < BSP_FPGA_LINE_IFACE_CNT; i++)
	{
		Dev->Iface[i].PMAStatus = *((uint8_t*)(BSP_FPGA_LINE_IFACE_ADDR(i)+ 0x0E));		
		Dev->Iface[i].OTU2Alarms = *((uint16_t*)BSP_FPGA_LINE_IFACE_ADDR(i));
	}
	if (Dev->Mode == BSP_FPGA_MODE_ADM10)
	{
		for (i = 0; i < BSP_FPGA_LINE_MUX_OPU0_CNT; i++)
		{
			Dev->Mux.OPU0PayloadType[i] = *((uint8_t*)(BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_ADDR(i)));
		}
		
		for (i = 0; i < BSP_FPGA_LINE_MUX_OPU1_CNT; i++)
		{
			Dev->Mux.OPU1PayloadType[i] = *((uint8_t*)(BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_ADDR(i)));
		}
		
		for (i = 0; i < BSP_FPGA_LINE_MUX_CLIENT_CNT; i++)
		{
			Dev->Mux.Client[i] = *((uint8_t*)(BSP_FPGA_LINE_MUX_CLIENT_ADDR(i)));
		}
		
		for (i = 0; i < BSP_FPGA_LINE_MUX_ODU01_CNT; i+=2)
		{
			Dev->Mux.ODU01[i] = *((uint8_t*)(BSP_FPGA_LINE_MUX_ODU01_ADDR(i)));
			Dev->Mux.ODU01[i+1] = *((uint8_t*)(BSP_FPGA_LINE_MUX_ODU01_ADDR(i+1)));
		}
		
		for (i = 0; i < BSP_FPGA_LINE_MUX_ODU12_CNT; i++)
		{
			Dev->Mux.ODU12[i] = *((uint8_t*)(BSP_FPGA_LINE_MUX_ODU12_ADDR(i)));
		}
	}
	else if (Dev->Mode == BSP_FPGA_MODE_ATP_2x10_BS)
	{
		for (i = 0; i < 2; i++)
		{
			Dev->AGG[i].Mode = *(uint8_t*)(BSP_FPGA_LINE_AGG_MODE_ADDR(i));
		}
	}
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t BSP_FPGA_LINE_Update(BSP_FPGA_LINE_T *Dev)
{
	uint32_t i, j;
	uint8_t Status;
	uint16_t Temp, OTU2BIP, OTU2FECUncorr, ODU2BIP, ODU1BIP, ODU0BIP;
	uint32_t OTU2FECCorr;
	uint8_t NotUsedODU1Mux, NotUsedODU0Mux;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Temp = *((uint16_t*)(BSP_FPGA_LINE_ADDR + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;
	
	for (i = 0; i < BSP_FPGA_LINE_IFACE_CNT; i++)
	{
		
		if ((Dev->Mode == BSP_FPGA_MODE_ADM10) && (i > 2))
			continue;
		
		Dev->Iface[i].__PMAStatus = *((uint8_t*)(BSP_FPGA_LINE_IFACE_ADDR(i)+ 0x0E));		
		Dev->Iface[i].__OTU2Alarms = *((uint16_t*)BSP_FPGA_LINE_IFACE_ADDR(i));
		
		OTU2BIP = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x02));
		Dev->Iface[i].OTU2BIP = OTU2BIP - Dev->Iface[i].__OTU2BIP;
		Dev->Iface[i].__OTU2BIP = OTU2BIP;
		
		OTU2FECCorr = *((uint32_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x04));
		Dev->Iface[i].OTU2FECCorr = OTU2FECCorr - Dev->Iface[i].__OTU2FECCorr;
		Dev->Iface[i].__OTU2FECCorr = OTU2FECCorr;
		
		OTU2FECUncorr = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x08));
		Dev->Iface[i].OTU2FECUncorr = OTU2FECUncorr - Dev->Iface[i].__OTU2FECUncorr;
		Dev->Iface[i].__OTU2FECUncorr = OTU2FECUncorr;
		
		Dev->Iface[i].ODU2Alarms = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x10));
		
		ODU2BIP = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x12));
		Dev->Iface[i].ODU2BIP = ODU2BIP - Dev->Iface[i].__ODU2BIP;
		Dev->Iface[i].__ODU2BIP = ODU2BIP;
		
		if (Dev->Mode == BSP_FPGA_MODE_ADM10)
		{
			for (j = 0; j < BSP_FPGA_LINE_IFACE_ODU1_CNT; j++)
			{
				Dev->Iface[i].ODU1Alarms[j] = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x18 + (0x08 * j)));
				
				ODU1BIP = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x18 + (0x08 * j) + 2));
				Dev->Iface[i].ODU1BIP[j] = ODU1BIP - Dev->Iface[i].__ODU1BIP[j];
				Dev->Iface[i].__ODU1BIP[j] = ODU1BIP;
			}
			
			for (j = 0; j < BSP_FPGA_LINE_IFACE_ODU0_CNT; j++)
			{
				Dev->Iface[i].ODU0Alarms[j] = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x38 + (0x08 * j)));
				
				ODU0BIP = *((uint16_t*)(BSP_FPGA_LINE_IFACE_ADDR(i) + 0x38 + (0x08 * j) + 2));
				Dev->Iface[i].ODU0BIP[j] = ODU0BIP - Dev->Iface[i].__ODU0BIP[j];
				Dev->Iface[i].__ODU0BIP[j] = ODU0BIP;
			}
		}
		
		if ((Dev->Iface[i].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_FEC_DISABLED_MASK) != (Dev->Iface[i].__OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_FEC_DISABLED_MASK))
		{
			if ((Dev->Iface[i].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_FEC_DISABLED_MASK))
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x01;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x30;
				osDelay(5);
			}
			else
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x30;
				osDelay(5);
			}
		}
		
		if ((Dev->Iface[i].PMAStatus & BSP_FPGA_LINE_IFACE_PMASTATUS_LOOPBACK_MASK) != (Dev->Iface[i].__PMAStatus & BSP_FPGA_LINE_IFACE_PMASTATUS_LOOPBACK_MASK))
		{
			if (Dev->Iface[i].PMAStatus & BSP_FPGA_LINE_IFACE_PMASTATUS_LOOPBACK_SERIAL_MASK)
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
			}
			else if (Dev->Iface[i].PMAStatus & BSP_FPGA_LINE_IFACE_PMASTATUS_LOOPBACK_POST_CDR_MASK)
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
			}
			else if (Dev->Iface[i].PMAStatus & BSP_FPGA_LINE_IFACE_PMASTATUS_LOOPBACK_PRE_CDR_MASK)
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
			}
			else
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
			}
		}
	}
	if (Dev->Mode == BSP_FPGA_MODE_ADM10)
	{
		
		NotUsedODU1Mux = 0xFF;
		for (i = 0; i < BSP_FPGA_LINE_MUX_ODU12_CNT; i++)
		{
			for (j = 0; j < BSP_FPGA_LINE_MUX_ODU12_CNT; j++)
			{
				if (Dev->Mux.ODU12[j] == BSP_FPGA_LINE_MUX_ODU12_ODU1(i))
				{
					break;
				}
			}
			if (j == BSP_FPGA_LINE_MUX_ODU12_CNT)
			{
				NotUsedODU1Mux = i;
				break;
			}
		}
		
		if (NotUsedODU1Mux != 0xFF)
		{
			for (i = 0; i < BSP_FPGA_LINE_MUX_ODU12_CNT; i++)
			{
				if (Dev->Mux.ODU12[i] == BSP_FPGA_LINE_MUX_ODU12_NONE)
				{
					Dev->Mux.ODU12[i] = BSP_FPGA_LINE_MUX_ODU12_ODU1(NotUsedODU1Mux);
				}
			}
			BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_SET(NotUsedODU1Mux, BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_NULL);
		}
		
		NotUsedODU0Mux = 0xFF;
		for (i = 0; i < BSP_FPGA_LINE_MUX_ODU01_CNT; i++)
		{
			for (j = 0; j < BSP_FPGA_LINE_MUX_ODU01_CNT; j++)
			{
				if (Dev->Mux.ODU01[j] == BSP_FPGA_LINE_MUX_ODU01_ODU0(i))
				{
					break;
				}
			}
			if (j == BSP_FPGA_LINE_MUX_ODU01_CNT)
			{
				NotUsedODU0Mux = i;
				break;
			}
		}
		
		if (NotUsedODU0Mux != BSP_FPGA_LINE_MUX_ODU01_NONE)
		{
			for (i = 0; i < BSP_FPGA_LINE_MUX_ODU01_CNT; i++)
			{
				if (Dev->Mux.ODU01[i] == BSP_FPGA_LINE_MUX_ODU01_NONE)
				{
					Dev->Mux.ODU01[i] = BSP_FPGA_LINE_MUX_ODU01_ODU0(NotUsedODU0Mux);
				}
			}
			BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_SET(NotUsedODU0Mux, BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_NULL);
		}
		
		for (i = 0; i < BSP_FPGA_LINE_MUX_OPU0_CNT; i++)
		{
			Dev->Mux.__OPU0PayloadType[i] = *((uint8_t*)(BSP_FPGA_LINE_MUX_OPU0_PAYLOAD_TYPE_ADDR(i)));
			
			if (Dev->Mux.__OPU0PayloadType[i] != Dev->Mux.OPU0PayloadType[i])
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i + 0x10;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = Dev->Mux.OPU0PayloadType[i];
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x23;
				osDelay(5);
			}
		}
		
		for (i = 0; i < BSP_FPGA_LINE_MUX_OPU1_CNT; i++)
		{
			Dev->Mux.__OPU1PayloadType[i] = *((uint8_t*)(BSP_FPGA_LINE_MUX_OPU1_PAYLOAD_TYPE_ADDR(i)));
			if (Dev->Mux.__OPU1PayloadType[i] != Dev->Mux.OPU1PayloadType[i])
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i + 0x08;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = Dev->Mux.OPU1PayloadType[i];
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x21;
				osDelay(5);
			}
		}
		
		for (i = 0; i < BSP_FPGA_LINE_MUX_CLIENT_CNT; i++)
		{
			Dev->Mux.__Client[i] = *((uint8_t*)(BSP_FPGA_LINE_MUX_CLIENT_ADDR(i)));
			if (Dev->Mux.__Client[i] != Dev->Mux.Client[i])
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = Dev->Mux.Client[i];
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x24;
				osDelay(5);
			}
		}
		
		for (i = 0; i < BSP_FPGA_LINE_MUX_ODU01_CNT; i+=2)
		{
			Dev->Mux.__ODU01[i] = *((uint8_t*)(BSP_FPGA_LINE_MUX_ODU01_ADDR(i)));
			Dev->Mux.__ODU01[i+1] = *((uint8_t*)(BSP_FPGA_LINE_MUX_ODU01_ADDR(i+1)));
			if ((Dev->Mux.__ODU01[i] != Dev->Mux.ODU01[i]) || (Dev->Mux.__ODU01[i+1] != Dev->Mux.ODU01[i+1])) 
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = i / 2 + 0x08;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = Dev->Mux.ODU01[i];
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = Dev->Mux.ODU01[i + 1];
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x22;
				osDelay(5);
			}
		}
		
		for (i = 0; i < BSP_FPGA_LINE_MUX_ODU12_CNT; i++)
		{
			Dev->Mux.__ODU12[i] = *((uint8_t*)(BSP_FPGA_LINE_MUX_ODU12_ADDR(i)));
			if (Dev->Mux.__ODU12[i] != Dev->Mux.ODU12[i])
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = Dev->Mux.ODU12[i];
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x20;
				osDelay(5);
			}
		}
	}
	else if (Dev->Mode == BSP_FPGA_MODE_ATP_2x10_BS)
	{
		for (i = 0; i < 2; i++)
		{
			Dev->AGG[i].__Mode = *(uint8_t*)(BSP_FPGA_LINE_AGG_MODE_ADDR(i));
			Dev->AGG[i].Status = *(uint8_t*)(BSP_FPGA_LINE_AGG_STATUS_ADDR(i));
			
			if (Dev->AGG[i].__Mode != Dev->AGG[i].Mode)
			{
				Status = *((uint8_t*)BSP_FPGA_LINE_STATUS_ADDR);
				if (Status == 0)
					continue;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT1_ADDR) = 0x00;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT2_ADDR) = i;
				*((uint8_t*)BSP_FPGA_LINE_CMD_OPT3_ADDR) = Dev->AGG[i].Mode;
				*((uint8_t*)BSP_FPGA_LINE_CMD_ADDR) = 0x25;
				osDelay(5);
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

int32_t BSP_FPGA_CLIENT_Init(BSP_FPGA_CLIENT_T *Dev, uint32_t Idx, uint32_t Mode)
{
	uint32_t i;
	uint8_t Status, SwVersion[4];
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->Idx = Idx;
	Dev->Mode = Mode;
	
	Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
	if (Status == 0)
		goto error;
	
	for (i = 0; i < 4; i++)
	{
		SwVersion[i] = *((char*)(BSP_FPGA_CLIENT_ADDR(Dev->Idx) + i));
	}
	sprintf(Dev->SwVersion, "%d.%d.%d.%d", SwVersion[1], SwVersion[0], SwVersion[3], SwVersion[2]);
	
	for (i = 0; i < BSP_FPGA_CLIENT_IFACE_CNT; i++)
	{
		Dev->Iface[i].Mode = *((uint8_t*)(BSP_FPGA_CLIENT_IFACE_TRANS_ADDR(Dev->Idx, i)+ 0x00));
		Dev->Iface[i].PMAStatus = *((uint8_t*)(BSP_FPGA_CLIENT_IFACE_TRANS_ADDR(Dev->Idx, i)+ 0x01));
		Dev->Iface[i].TxStatus = *((uint8_t*)(BSP_FPGA_CLIENT_IFACE_TX_STATUS_ADDR(Dev->Idx, i)));
	}
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t BSP_FPGA_CLIENT_Update(BSP_FPGA_CLIENT_T *Dev)
{
	uint32_t i;
	uint8_t Status;
	uint16_t Temp, SDHBIP8, SDHBIP24, EthRxErrs, EthTxErrs;
	uint16_t ODU1BIP, ODU0BIP;
	uint32_t EthRxPkts, EthRxOcts, EthTxPkts, EthTxOcts;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Temp = *((uint16_t*)(BSP_FPGA_CLIENT_ADDR(Dev->Idx) + 0x06));
	Dev->Temp = (((Temp & 0x3FF) * 693.0f) / 1024.0f) - 265.0f;
	
	for (i = 0; i < BSP_FPGA_CLIENT_IFACE_CNT; i++)
	{
		//if (i > 0)
		//	continue;
		Dev->Iface[i].__Mode = *((uint8_t*)(BSP_FPGA_CLIENT_IFACE_TRANS_ADDR(Dev->Idx, i)+ 0x00));
		Dev->Iface[i].__PMAStatus = *((uint8_t*)(BSP_FPGA_CLIENT_IFACE_TRANS_ADDR(Dev->Idx, i)+ 0x01));
		Dev->Iface[i].__PCSStatus = *((uint8_t*)(BSP_FPGA_CLIENT_IFACE_TRANS_ADDR(Dev->Idx, i)+ 0x02));
		Dev->Iface[i].__CDRStatus = *((uint8_t*)(BSP_FPGA_CLIENT_IFACE_TRANS_ADDR(Dev->Idx, i)+ 0x03));
		Dev->Iface[i].__TxStatus = *((uint8_t*)(BSP_FPGA_CLIENT_IFACE_TX_STATUS_ADDR(Dev->Idx, i)));
		
		Dev->Iface[i].SDHAlarms = *((uint16_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i)+ 0x02));
		
		SDHBIP8 = *((uint16_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x04));
		Dev->Iface[i].SDHBIP8 = SDHBIP8 - Dev->Iface[i].__SDHBIP8;
		Dev->Iface[i].__SDHBIP8 = SDHBIP8;
		
		SDHBIP24 = *((uint16_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x06));
		Dev->Iface[i].SDHBIP24 = SDHBIP24 - Dev->Iface[i].__SDHBIP24;
		Dev->Iface[i].__SDHBIP24 = SDHBIP24;
		
		EthTxPkts = *((uint32_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x08));
		Dev->Iface[i].EthTxPkts = EthTxPkts - Dev->Iface[i].__EthTxPkts;
		Dev->Iface[i].__EthTxPkts = EthTxPkts;
		
		EthRxPkts = *((uint32_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x0C));
		Dev->Iface[i].EthRxPkts = EthRxPkts - Dev->Iface[i].__EthRxPkts;
		Dev->Iface[i].__EthRxPkts = EthRxPkts;
		
		EthTxOcts = *((uint32_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x10));
		Dev->Iface[i].EthTxOcts = EthTxOcts - Dev->Iface[i].__EthTxOcts;
		Dev->Iface[i].__EthTxOcts = EthTxOcts;
		
		EthRxOcts = *((uint32_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x14));
		Dev->Iface[i].EthRxOcts = EthRxOcts - Dev->Iface[i].__EthRxOcts;
		Dev->Iface[i].__EthRxOcts = EthRxOcts;
		
		EthTxErrs = *((uint16_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x18));
		Dev->Iface[i].EthTxErrs = EthTxErrs - Dev->Iface[i].__EthTxErrs;
		Dev->Iface[i].__EthTxErrs = EthTxErrs;
		
		EthRxErrs = *((uint16_t*)(BSP_FPGA_CLIENT_IFACE_STAT_ADDR(Dev->Idx, i) + 0x1A));
		Dev->Iface[i].EthRxErrs = EthRxErrs - Dev->Iface[i].__EthRxErrs;
		Dev->Iface[i].__EthRxErrs = EthRxErrs;
				
		if (Dev->Iface[i].__TxStatus !=  Dev->Iface[i].TxStatus)
		{
			Status = *((uint8_t*)BSP_FPGA_CLIENT_STATUS_ADDR(Dev->Idx));
			if (Status == 0)
				continue;
			*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT1_ADDR(Dev->Idx)) = 0x00;
			*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT2_ADDR(Dev->Idx)) = i;
			*((uint8_t*)BSP_FPGA_CLIENT_CMD_OPT3_ADDR(Dev->Idx)) = Dev->Iface[i].TxStatus;
			*((uint8_t*)BSP_FPGA_CLIENT_CMD_ADDR(Dev->Idx)) = 0x05;
			osDelay(5);
		}
		
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
	
	if (Dev->Mode == BSP_FPGA_MODE_ATP_2x10_BS)
	{
		for (i = 0; i < BSP_FPGA_CLIENT_ODU1_CNT; i++)
		{
			Dev->ODU1Alarms[i] = *((uint16_t*)(BSP_FPGA_CLIENT_ODU1_ALARM_ADDR(Dev->Idx, i)));			
			ODU1BIP =  *((uint16_t*)(BSP_FPGA_CLIENT_ODU1_BIP_ADDR(Dev->Idx, i)));
			Dev->ODU1BIP[i] = ODU1BIP - Dev->__ODU1BIP[i];
			Dev->__ODU1BIP[i] = ODU1BIP;
		}
		
		for (i = 0; i < BSP_FPGA_CLIENT_ODU0_CNT; i++)
		{
			Dev->ODU0Alarms[i] = *((uint16_t*)(BSP_FPGA_CLIENT_ODU0_ALARM_ADDR(Dev->Idx, i)));			
			ODU0BIP =  *((uint16_t*)(BSP_FPGA_CLIENT_ODU0_BIP_ADDR(Dev->Idx, i)));
			Dev->ODU0BIP[i] = ODU0BIP - Dev->__ODU0BIP[i];
			Dev->__ODU0BIP[i] = ODU0BIP;
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
	uint32_t i;
	
	if (BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
		return -1;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	for (i = 0; i < BSP_CLK_SI5332_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&si5332[i]) != BSP_DEV_STATE_OK)
			return -1;
	}
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
						BSP_FPGA_LINE_Init(&fpga_line, fpga_maxv.Mode);
					}
					else
					{
						BSP_FPGA_LINE_Update(&fpga_line);
					}
				}
				else
				{
					BSP_FPGA_LINE_Deinit(&fpga_line);
				}
				for (i = 0; i < BSP_FPGA_CLIENT_CNT; i++)
				{
					if (fpga_maxv.Status & BSP_FPGA_MAXV_CLIENT_STATUS_MASK(i))
					{
						if (BSP_DEV_GET_STATE(&fpga_client[i]) != BSP_DEV_STATE_OK)
						{
							BSP_FPGA_CLIENT_Init(&fpga_client[i], i, fpga_maxv.Mode);
						}
						else
						{
							BSP_FPGA_CLIENT_Update(&fpga_client[i]);
						}
					}
					else
					{
						BSP_FPGA_CLIENT_Deinit(&fpga_client[i]);
					}
				}
			}
		}
	}
}
