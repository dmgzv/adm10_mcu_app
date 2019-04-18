#include "bsp_flash.h"
#include <stdlib.h>

#if defined(BSP_FLASH_TYPE_AT45DB081D)
void BSP_AT45DB081D_InitPins(void *Priv)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	BSP_AT45DB081D_SPI_CS_GPIO_CLK_ENABLE();
	BSP_AT45DB081D_WP_GPIO_CLK_ENABLE();
	BSP_AT45DB081D_RESET_GPIO_CLK_ENABLE();  
	/* SPI CS GPIO pin configuration  */
	GPIO_InitStruct.Pin       = BSP_AT45DB081D_SPI_CS_GPIO_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;	
	HAL_GPIO_Init(BSP_AT45DB081D_SPI_CS_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_AT45DB081D_SPI_CS_GPIO_PORT, BSP_AT45DB081D_SPI_CS_GPIO_PIN, GPIO_PIN_SET);
	/* WP GPIO pin configuration  */
	GPIO_InitStruct.Pin       = BSP_AT45DB081D_WP_GPIO_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;	
	HAL_GPIO_Init(BSP_AT45DB081D_WP_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_AT45DB081D_WP_GPIO_PORT, BSP_AT45DB081D_WP_GPIO_PIN, GPIO_PIN_SET);
	/* RESET GPIO pin configuration  */
	GPIO_InitStruct.Pin       = BSP_AT45DB081D_RESET_GPIO_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;	
	HAL_GPIO_WritePin(BSP_AT45DB081D_RESET_GPIO_PORT, BSP_AT45DB081D_RESET_GPIO_PIN, GPIO_PIN_SET);
}

void BSP_AT45DB081D_SetPinState(void *Priv, AT45DB081D_PIN_T Pin, AT45DB081D_PIN_STATE_T PinState)
{
	switch (Pin)
	{
		case AT45DB081D_PIN_SPI_CS:
			if (PinState == AT45DB081D_PIN_STATE_DEASSERT)
				HAL_GPIO_WritePin(BSP_AT45DB081D_SPI_CS_GPIO_PORT, BSP_AT45DB081D_SPI_CS_GPIO_PIN, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(BSP_AT45DB081D_SPI_CS_GPIO_PORT, BSP_AT45DB081D_SPI_CS_GPIO_PIN, GPIO_PIN_SET);
			break;
		case AT45DB081D_PIN_WP:
				if (PinState == AT45DB081D_PIN_STATE_DEASSERT)
					HAL_GPIO_WritePin(BSP_AT45DB081D_WP_GPIO_PORT, BSP_AT45DB081D_WP_GPIO_PIN, GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(BSP_AT45DB081D_WP_GPIO_PORT, BSP_AT45DB081D_WP_GPIO_PIN, GPIO_PIN_SET);
			break;
		case AT45DB081D_PIN_RESET:
				if (PinState == AT45DB081D_PIN_STATE_DEASSERT)
					HAL_GPIO_WritePin(BSP_AT45DB081D_RESET_GPIO_PORT, BSP_AT45DB081D_RESET_GPIO_PIN, GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(BSP_AT45DB081D_RESET_GPIO_PORT, BSP_AT45DB081D_RESET_GPIO_PIN, GPIO_PIN_SET);
			break;
	}
}
#elif defined(BSP_FLASH_TYPE_AT45DB161E)
void BSP_AT45DB161E_InitPins(void *Priv)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	BSP_AT45DB161E_SPI_CS_GPIO_CLK_ENABLE();
	BSP_AT45DB161E_WP_GPIO_CLK_ENABLE();
	BSP_AT45DB161E_RESET_GPIO_CLK_ENABLE();  
	/* SPI CS GPIO pin configuration  */
	GPIO_InitStruct.Pin       = BSP_AT45DB161E_SPI_CS_GPIO_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;	
	HAL_GPIO_Init(BSP_AT45DB161E_SPI_CS_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_AT45DB161E_SPI_CS_GPIO_PORT, BSP_AT45DB161E_SPI_CS_GPIO_PIN, GPIO_PIN_SET);
	/* WP GPIO pin configuration  */
	GPIO_InitStruct.Pin       = BSP_AT45DB161E_WP_GPIO_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;	
	HAL_GPIO_Init(BSP_AT45DB161E_WP_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_AT45DB161E_WP_GPIO_PORT, BSP_AT45DB161E_WP_GPIO_PIN, GPIO_PIN_SET);
	/* RESET GPIO pin configuration  */
	GPIO_InitStruct.Pin       = BSP_AT45DB161E_RESET_GPIO_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;	
	HAL_GPIO_WritePin(BSP_AT45DB161E_RESET_GPIO_PORT, BSP_AT45DB161E_RESET_GPIO_PIN, GPIO_PIN_SET);
}

void BSP_AT45DB161E_SetPinState(void *Priv, AT45DB161E_PIN_T Pin, AT45DB161E_PIN_STATE_T PinState)
{
	switch (Pin)
	{
		case AT45DB161E_PIN_SPI_CS:
			if (PinState == AT45DB161E_PIN_STATE_DEASSERT)
				HAL_GPIO_WritePin(BSP_AT45DB161E_SPI_CS_GPIO_PORT, BSP_AT45DB161E_SPI_CS_GPIO_PIN, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(BSP_AT45DB161E_SPI_CS_GPIO_PORT, BSP_AT45DB161E_SPI_CS_GPIO_PIN, GPIO_PIN_SET);
			break;
		case AT45DB161E_PIN_WP:
				if (PinState == AT45DB161E_PIN_STATE_DEASSERT)
					HAL_GPIO_WritePin(BSP_AT45DB161E_WP_GPIO_PORT, BSP_AT45DB161E_WP_GPIO_PIN, GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(BSP_AT45DB161E_WP_GPIO_PORT, BSP_AT45DB161E_WP_GPIO_PIN, GPIO_PIN_SET);
			break;
		case AT45DB161E_PIN_RESET:
				if (PinState == AT45DB161E_PIN_STATE_DEASSERT)
					HAL_GPIO_WritePin(BSP_AT45DB161E_RESET_GPIO_PORT, BSP_AT45DB161E_RESET_GPIO_PIN, GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(BSP_AT45DB161E_RESET_GPIO_PORT, BSP_AT45DB161E_RESET_GPIO_PIN, GPIO_PIN_SET);
			break;
	}
}
#elif defined(BSP_FLASH_TYPE_AT45DB641E)
void BSP_AT45DB641E_InitPins(void *Priv)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	BSP_AT45DB641E_SPI_CS_GPIO_CLK_ENABLE();
	BSP_AT45DB641E_WP_GPIO_CLK_ENABLE();
	BSP_AT45DB641E_RESET_GPIO_CLK_ENABLE();  
	/* SPI CS GPIO pin configuration  */
	GPIO_InitStruct.Pin       = BSP_AT45DB641E_SPI_CS_GPIO_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;	
	HAL_GPIO_Init(BSP_AT45DB641E_SPI_CS_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_AT45DB641E_SPI_CS_GPIO_PORT, BSP_AT45DB641E_SPI_CS_GPIO_PIN, GPIO_PIN_SET);
	/* WP GPIO pin configuration  */
	GPIO_InitStruct.Pin       = BSP_AT45DB641E_WP_GPIO_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;	
	HAL_GPIO_Init(BSP_AT45DB641E_WP_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_AT45DB641E_WP_GPIO_PORT, BSP_AT45DB641E_WP_GPIO_PIN, GPIO_PIN_SET);
	/* RESET GPIO pin configuration  */
	GPIO_InitStruct.Pin       = BSP_AT45DB641E_RESET_GPIO_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull      = GPIO_NOPULL;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;	
	HAL_GPIO_WritePin(BSP_AT45DB641E_RESET_GPIO_PORT, BSP_AT45DB641E_RESET_GPIO_PIN, GPIO_PIN_SET);
}

void BSP_AT45DB641E_SetPinState(void *Priv, AT45DB641E_PIN_T Pin, AT45DB641E_PIN_STATE_T PinState)
{
	switch (Pin)
	{
		case AT45DB641E_PIN_SPI_CS:
			if (PinState == AT45DB641E_PIN_STATE_DEASSERT)
				HAL_GPIO_WritePin(BSP_AT45DB641E_SPI_CS_GPIO_PORT, BSP_AT45DB641E_SPI_CS_GPIO_PIN, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(BSP_AT45DB641E_SPI_CS_GPIO_PORT, BSP_AT45DB641E_SPI_CS_GPIO_PIN, GPIO_PIN_SET);
			break;
		case AT45DB641E_PIN_WP:
				if (PinState == AT45DB641E_PIN_STATE_DEASSERT)
					HAL_GPIO_WritePin(BSP_AT45DB641E_WP_GPIO_PORT, BSP_AT45DB641E_WP_GPIO_PIN, GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(BSP_AT45DB641E_WP_GPIO_PORT, BSP_AT45DB641E_WP_GPIO_PIN, GPIO_PIN_SET);
			break;
		case AT45DB641E_PIN_RESET:
				if (PinState == AT45DB641E_PIN_STATE_DEASSERT)
					HAL_GPIO_WritePin(BSP_AT45DB641E_RESET_GPIO_PORT, BSP_AT45DB641E_RESET_GPIO_PIN, GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(BSP_AT45DB641E_RESET_GPIO_PORT, BSP_AT45DB641E_RESET_GPIO_PIN, GPIO_PIN_SET);
			break;
	}
}
#elif defined(BSP_FLASH_TYPE_S29GL01GS)

int32_t BSP_FLASH_EraseFPGA(BSP_FLASH_T *Dev, uint32_t Idx)
{
	uint32_t Addr;
	
	if (Idx == 0)
	{
		for (Addr = BSP_FLASH_FPGA_1_FW_ADDR_START; Addr < BSP_FLASH_FPGA_1_FW_ADDR_END; Addr+=BSP_FLASH_PAGE_SIZE)
		{
			if (BSP_FLASH_WaitReady(Dev))
				return -1;
			if (BSP_FLASH_ErasePage(Dev, Addr))
				return -1;
		}
		if (BSP_FLASH_WaitReady(Dev))
			return -1;
	}
	else if (Idx == 1)
	{
		for (Addr = BSP_FLASH_FPGA_2_FW_ADDR_START; Addr < BSP_FLASH_FPGA_2_FW_ADDR_END; Addr+=BSP_FLASH_PAGE_SIZE)
		{
			if (BSP_FLASH_WaitReady(Dev))
				return -1;
			if (BSP_FLASH_ErasePage(Dev, Addr))
				return -1;
		}
		if (BSP_FLASH_WaitReady(Dev))
			return -1;
	}
	else if (Idx == 2)
	{
		for (Addr = BSP_FLASH_FPGA_3_FW_ADDR_START; Addr < BSP_FLASH_FPGA_3_FW_ADDR_END; Addr+=BSP_FLASH_PAGE_SIZE)
		{
			if (BSP_FLASH_WaitReady(Dev))
				return -1;
			if (BSP_FLASH_ErasePage(Dev, Addr))
				return -1;
		}
		if (BSP_FLASH_WaitReady(Dev))
			return -1;
	}
	
	return 0;
}

int32_t BSP_FLASH_EraseChip(BSP_FLASH_T *Dev)
{
	uint32_t i;
	
	for (i = 0; i < BSP_FLASH_PAGES_CNT; i++)
	{
		if (BSP_FLASH_WaitReady(Dev))
			return -1;
		if (BSP_FLASH_ErasePage(Dev, i * BSP_FLASH_PAGE_SIZE))
			return -1;
	}
	if (BSP_FLASH_WaitReady(Dev))
		return -1;
	
	return 0;
}

#if 0
#define BSP_FLASH_SHIFT	0 /*0x4000000*/

#define BSP_FLASH_BUFFER_SIZE	0x10000
//static uint8_t RxBuffer[BSP_FLASH_BUFFER_SIZE];
static uint8_t RxBuffer[BSP_FLASH_BUFFER_SIZE];
static uint32_t SectorEraseMaxTime = 0;
static uint32_t ChipEraseTime = 0;
static uint32_t BufferReadMaxTime = 0;
static uint32_t ChipReadTime = 0;
static uint32_t BufferProgramMaxTime = 0;
static uint32_t ChipProgramTime = 0;

int32_t BSP_FLASH_RandomTest(BSP_FLASH_T *Dev)
{
	uint32_t i, j;
	uint32_t Time;
	

	ChipEraseTime = osKernelSysTick();
	for (i = 0; i < BSP_FLASH_PAGES_CNT; i++)
	{
		Time = osKernelSysTick();
		if (BSP_FLASH_WaitReady(Dev))
			return -1;
		if (BSP_FLASH_ErasePage(Dev, BSP_FLASH_SHIFT + i * BSP_FLASH_PAGE_SIZE))
			return -1;
		Time = osKernelSysTick() - Time;
		if (Time > SectorEraseMaxTime)
			SectorEraseMaxTime = Time;
	}
	if (BSP_FLASH_WaitReady(Dev))
		return -1;
	ChipEraseTime = osKernelSysTick() - ChipEraseTime;
	
	/* Check data after Erase */
	ChipReadTime = osKernelSysTick();
	for (i = 0; i < BSP_FLASH_SIZE / sizeof(RxBuffer); i++)
	{
		Time = osKernelSysTick();
		if (BSP_FLASH_Read(Dev, BSP_FLASH_SHIFT + i * sizeof(RxBuffer), RxBuffer, sizeof(RxBuffer)))
			return -1;
		for (j = 0; j < sizeof(RxBuffer)/sizeof(RxBuffer[0]); j++)
		{
			if (RxBuffer[j] != 0xFF)
				return -1;
		}
		Time = osKernelSysTick() - Time;
		if (Time > BufferReadMaxTime)
			BufferReadMaxTime = Time;
	}
	ChipReadTime = osKernelSysTick() - ChipReadTime;

	/*Program memory with random data*/
	srand(0);
	for (i = 0; i < sizeof(RxBuffer)/sizeof(RxBuffer[0]); i++)
	{
		RxBuffer[j] = rand() % 0xFF; /*Fill buffer with random data from 0 to 255*/
	}
	ChipProgramTime = osKernelSysTick();
	for (i = 0; i < BSP_FLASH_SIZE / sizeof(RxBuffer); i++)
	{
		Time = osKernelSysTick();
		if (BSP_FLASH_WaitReady(Dev))
			return -1;
		if (BSP_FLASH_ProgramPage(Dev, BSP_FLASH_SHIFT + i * sizeof(RxBuffer), RxBuffer, sizeof(RxBuffer)))
			return -1;
		Time = osKernelSysTick() - Time;
		if (Time > BufferProgramMaxTime)
			BufferProgramMaxTime = Time;
	}
	if (BSP_FLASH_WaitReady(Dev))
		return -1;
	ChipProgramTime = osKernelSysTick() - ChipProgramTime;
	
	/*Check random data after programing*/
	srand(0);
	for (i = 0; i < BSP_FLASH_SIZE / sizeof(RxBuffer); i++)
	{
		if (BSP_FLASH_Read(Dev, BSP_FLASH_SHIFT + i * sizeof(RxBuffer), RxBuffer, sizeof(RxBuffer)))
			return -1;
		for (j = 0; j < sizeof(RxBuffer)/sizeof(RxBuffer[0]); j++)
		{
			if (RxBuffer[j] != RxBuffer[j]) /*Check random data in buffer */
				return -1;
			RxBuffer[j] = 0;
		}
	}
	return 0;
}
#endif	

#endif
