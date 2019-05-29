#include "bsp_fpga_flash.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_fpga.h"
#include <stdlib.h>

S29GL01GS_T s29gl01gs;

int32_t BSP_FPGA_FLASH_EraseFPGA(uint32_t Idx)
{
	uint32_t Addr;
	
	if (Idx == 0)
	{
		for (Addr = BSP_FLASH_FPGA_1_FW_ADDR_START; Addr < BSP_FLASH_FPGA_1_FW_ADDR_END; Addr+=S29GL01GS_SECTOR_SIZE)
		{
			if (S29GL01GS_WaitReady(&s29gl01gs))
				return -1;
			if (S29GL01GS_EraseSector(&s29gl01gs, Addr))
				return -1;
		}
		if (S29GL01GS_WaitReady(&s29gl01gs))
			return -1;
	}
	else if (Idx == 1)
	{
		for (Addr = BSP_FLASH_FPGA_2_FW_ADDR_START; Addr < BSP_FLASH_FPGA_2_FW_ADDR_END; Addr+=S29GL01GS_SECTOR_SIZE)
		{
			if (S29GL01GS_WaitReady(&s29gl01gs))
				return -1;
			if (S29GL01GS_EraseSector(&s29gl01gs, Addr))
				return -1;
		}
		if (S29GL01GS_WaitReady(&s29gl01gs))
			return -1;
	}
	else if (Idx == 2)
	{
		for (Addr = BSP_FLASH_FPGA_3_FW_ADDR_START; Addr < BSP_FLASH_FPGA_3_FW_ADDR_END; Addr+=S29GL01GS_SECTOR_SIZE)
		{
			if (S29GL01GS_WaitReady(&s29gl01gs))
				return -1;
			if (S29GL01GS_EraseSector(&s29gl01gs, Addr))
				return -1;
		}
		if (S29GL01GS_WaitReady(&s29gl01gs))
			return -1;
	}
	
	return 0;
}

int32_t BSP_FPGA_FLASH_EraseChip(void)
{
	uint32_t i;
	
	for (i = 0; i < S29GL01GS_SECTORS_COUNT; i++)
	{
		if (S29GL01GS_WaitReady(&s29gl01gs))
			return -1;
		if (S29GL01GS_EraseSector(&s29gl01gs, i * S29GL01GS_SECTOR_SIZE))
			return -1;
	}
	if (S29GL01GS_WaitReady(&s29gl01gs))
		return -1;
	
	return 0;
}

#if 0
#define S29GL01GS_SHIFT	0 /*0x4000000*/

#define S29GL01GS_BUFFER_SIZE	0x10000
//static uint8_t RxBuffer[S29GL01GS_BUFFER_SIZE];
static uint8_t RxBuffer[S29GL01GS_BUFFER_SIZE];
static uint32_t SectorEraseMaxTime = 0;
static uint32_t ChipEraseTime = 0;
static uint32_t BufferReadMaxTime = 0;
static uint32_t ChipReadTime = 0;
static uint32_t BufferProgramMaxTime = 0;
static uint32_t ChipProgramTime = 0;

int32_t BSP_FPGA_FLASH_RandomTest(void)
{
	uint32_t i, j;
	uint32_t Time;
	

	ChipEraseTime = osKernelSysTick();
	for (i = 0; i < S29GL01GS_SECTORS_COUNT; i++)
	{
		Time = osKernelSysTick();
		if (S29GL01GS_WaitReady(&s29gl01gs))
			return -1;
		if (S29GL01GS_EraseSector(&s29gl01gs, S29GL01GS_SHIFT + i * S29GL01GS_SECTOR_SIZE))
			return -1;
		Time = osKernelSysTick() - Time;
		if (Time > SectorEraseMaxTime)
			SectorEraseMaxTime = Time;
	}
	if (S29GL01GS_WaitReady(&s29gl01gs))
		return -1;
	ChipEraseTime = osKernelSysTick() - ChipEraseTime;
	
	/* Check data after Erase */
	ChipReadTime = osKernelSysTick();
	for (i = 0; i < S29GL01GS_SIZE / sizeof(RxBuffer); i++)
	{
		Time = osKernelSysTick();
		if (S29GL01GS_Read(&s29gl01gs, S29GL01GS_SHIFT + i * sizeof(RxBuffer), RxBuffer, sizeof(RxBuffer)))
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
	for (i = 0; i < S29GL01GS_SIZE / sizeof(RxBuffer); i++)
	{
		Time = osKernelSysTick();
		if (S29GL01GS_WaitReady(&s29gl01gs))
			return -1;
		if (S29GL01GS_ProgramSector(&s29gl01gs, S29GL01GS_SHIFT + i * sizeof(RxBuffer), RxBuffer, sizeof(RxBuffer)))
			return -1;
		Time = osKernelSysTick() - Time;
		if (Time > BufferProgramMaxTime)
			BufferProgramMaxTime = Time;
	}
	if (S29GL01GS_WaitReady(&s29gl01gs))
		return -1;
	ChipProgramTime = osKernelSysTick() - ChipProgramTime;
	
	/*Check random data after programing*/
	srand(0);
	for (i = 0; i < S29GL01GS_SIZE / sizeof(RxBuffer); i++)
	{
		if (S29GL01GS_Read(&s29gl01gs, S29GL01GS_SHIFT + i * sizeof(RxBuffer), RxBuffer, sizeof(RxBuffer)))
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

static int32_t BSP_FPGA_FLASH_CheckConditions(void)
{
	uint32_t i;
	
	if (BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
		return -1;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
		return -1;
	if (BSP_DEV_GET_STATE(&fpga_line) != BSP_DEV_STATE_OK)
		return -1;
	for (i = 0 ; i < BSP_FPGA_CLIENT_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&fpga_client[i]) != BSP_DEV_STATE_OK)
			return -1;
	}
	return 0;
}

void BSP_FPGA_FLASH_Process(void)
{	
	if (BSP_FPGA_FLASH_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&s29gl01gs) != BSP_DEV_STATE_RESET)
		{
			S29GL01GS_Deinit(&s29gl01gs);
		}
	}
	else
	{
		/* External faults absent */
		if(BSP_DEV_GET_STATE(&s29gl01gs) != BSP_DEV_STATE_OK)
		{
			S29GL01GS_Init(&s29gl01gs);
		}	
	}
}
