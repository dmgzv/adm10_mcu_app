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

#define BSP_FLASH_SHIFT	0 /*0x4000000*/

int32_t BSP_FLASH_RandomTest(BSP_FLASH_T *Dev)
{
	uint32_t i, j;
	uint8_t Buffer[256];
#if 1
	for (i = 0; i < BSP_FLASH_PAGES_CNT; i++)
	{
		if (BSP_FLASH_WaitReady(Dev))
			return -1;
		if (BSP_FLASH_ErasePage(Dev, BSP_FLASH_SHIFT + i * BSP_FLASH_PAGE_SIZE))
			return -1;
	}
	if (BSP_FLASH_WaitReady(Dev))
		return -1;
	
	/* Check data after Erase */
	for (i = 0; i < BSP_FLASH_SIZE / sizeof(Buffer); i++)
	{
		if (BSP_FLASH_Read(Dev, BSP_FLASH_SHIFT + i * sizeof(Buffer), Buffer, sizeof(Buffer)))
			return -1;
		for (j = 0; j < sizeof(Buffer)/sizeof(Buffer[0]); j++)
		{
			if (Buffer[j] != 0xFF)
				return -1;
		}
	}
#endif	
	/*Program memory with random data*/
	srand(0);
	for (i = 0; i < BSP_FLASH_SIZE / sizeof(Buffer); i++)
	{
		for (j = 0; j < sizeof(Buffer)/sizeof(Buffer[0]); j++)
		{
			Buffer[j] = rand() % 256; /*Fill buffer with random data from 0 to 255*/
		}
		if (BSP_FLASH_WaitReady(Dev))
			return -1;
		if (BSP_FLASH_ProgramPage(Dev, BSP_FLASH_SHIFT + i * sizeof(Buffer), Buffer, sizeof(Buffer)))
			return -1;
	}
	if (BSP_FLASH_WaitReady(Dev))
		return -1;
	
	/*Check random data after programing*/
	srand(0);
	for (i = 0; i < BSP_FLASH_SIZE / sizeof(Buffer); i++)
	{
		if (BSP_FLASH_Read(Dev, BSP_FLASH_SHIFT + i * sizeof(Buffer), Buffer, sizeof(Buffer)))
			return -1;
		for (j = 0; j < sizeof(Buffer)/sizeof(Buffer[0]); j++)
		{
			if (Buffer[j] != rand() % 256) /*Check random data in buffer */
				return -1;
		}
	}
	return 0;
}

#endif
