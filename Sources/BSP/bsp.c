#include "bsp.h"
#include "mt48lc16m16a2.h"

uint8_t SlotAddress;

void BSP_Init(void)
{
	/* STM32F4xx HAL library initialization:
		 - Configure the Flash prefetch, instruction and Data caches
		 - Configure the Systick to generate an interrupt each 1 msec
		 - Set NVIC Group Priority to 4
		 - Global MSP (MCU Support Package) initialization
	*/
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();
}

int32_t BSP_InitInterfaces(void)
{
	int32_t result = 0;
	/* Configure the I2C */
	result |= I2C_Init();
	/* Configure the UART */
	result |= UART_Init();
	/* Configure the SPI */	
	result |= SPI_Init();
	
	return result;
}

void BSP_DeinitInterfaces(void)
{
	/* Deinit the I2C */
	I2C_Deinit();
	/* Deinit the UART */
	UART_Deinit();
	/* Deinit the SPI */	
	SPI_Deinit();
}

static const uint32_t SlotAddressGPIOPin[BSP_SLOT_ADDR_GPIO_CNT] = {
	BSP_SLOT_ADDR_0_GPIO_PIN, BSP_SLOT_ADDR_1_GPIO_PIN, BSP_SLOT_ADDR_2_GPIO_PIN, BSP_SLOT_ADDR_3_GPIO_PIN
};
static const GPIO_TypeDef* SlotAddressGPIOPort[BSP_SLOT_ADDR_GPIO_CNT] = {
	BSP_SLOT_ADDR_0_GPIO_PORT, BSP_SLOT_ADDR_1_GPIO_PORT, BSP_SLOT_ADDR_2_GPIO_PORT, BSP_SLOT_ADDR_3_GPIO_PORT
};
static const uint32_t SlotAddressGPIOMode[BSP_SLOT_ADDR_GPIO_CNT] = {
	BSP_SLOT_ADDR_0_GPIO_MODE, BSP_SLOT_ADDR_1_GPIO_MODE, BSP_SLOT_ADDR_2_GPIO_MODE, BSP_SLOT_ADDR_3_GPIO_MODE
};
static const uint32_t SlotAddressGPIOPull[BSP_SLOT_ADDR_GPIO_CNT] = {
	BSP_SLOT_ADDR_0_GPIO_PULL, BSP_SLOT_ADDR_1_GPIO_PULL, BSP_SLOT_ADDR_2_GPIO_PULL, BSP_SLOT_ADDR_3_GPIO_PULL
};
static const uint32_t SlotAddressGPIOSpeed[BSP_SLOT_ADDR_GPIO_CNT] = {
	BSP_SLOT_ADDR_0_GPIO_SPEED, BSP_SLOT_ADDR_1_GPIO_SPEED, BSP_SLOT_ADDR_2_GPIO_SPEED, BSP_SLOT_ADDR_3_GPIO_SPEED
};

void BSP_InitSlotAddress(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	uint32_t i;
	uint8_t Addr;
	
	/* Enable the GPIO Clock */
	BSP_SLOT_ADDR_0_GPIO_CLK_ENABLE();
	BSP_SLOT_ADDR_1_GPIO_CLK_ENABLE();
	BSP_SLOT_ADDR_2_GPIO_CLK_ENABLE();
	BSP_SLOT_ADDR_3_GPIO_CLK_ENABLE();
		/* Configure the GPIO pin */
	for (i = 0; i < BSP_SLOT_ADDR_GPIO_CNT; i++)
	{
		GPIO_InitStruct.Pin = SlotAddressGPIOPin[i];
		GPIO_InitStruct.Mode = SlotAddressGPIOMode[i];
		GPIO_InitStruct.Pull = SlotAddressGPIOPull[i];
		GPIO_InitStruct.Speed = SlotAddressGPIOSpeed[i];
		HAL_GPIO_Init((GPIO_TypeDef*)SlotAddressGPIOPort[i], &GPIO_InitStruct);
	}
	
	Addr = 0;
	for (i = 0; i < BSP_SLOT_ADDR_GPIO_CNT; i++)
	{
		if (HAL_GPIO_ReadPin((GPIO_TypeDef*)SlotAddressGPIOPort[i], SlotAddressGPIOPin[i]) == GPIO_PIN_SET)
			Addr |= 1 << i;
	}
	if (Addr)
		Addr = (Addr << 1) - 1;
	SlotAddress = Addr;
}

void BSP_InitLeds(void)
{	
	BSP_LED_Init(BSP_LED_TYPE_POWER);	
	BSP_LED_Set(BSP_LED_TYPE_POWER, BSP_LED_COLOR_NO);
	
	BSP_LED_Init(BSP_LED_TYPE_ALARM);
	BSP_LED_Set(BSP_LED_TYPE_ALARM, BSP_LED_COLOR_RED);
}

int32_t BSP_InitMemory(void)
{
	int32_t result = 0;
	/* Init SDRAM */
	result |= BSP_SDRAM_Init();
	/* Init Flash */
	result |= BSP_FLASH_Init();
	
	return result;
}

void BSP_UpdateCard(void)
{
	uint32_t i;
	
	BSP_PWR_IN_Process();
	
	BSP_PWR_PIN_Process();
	
	BSP_PWR_SEQ_Process();
		
	BSP_ETH_PIN_Process();
	
	BSP_ETH_SW_Process();
	
	BSP_ETH_Process(SlotAddress);
	
	BSP_CLK_Process();
	
	BSP_SFP_I2C_SWITCH_Process();
	
	BSP_SFP_PIN_Process();
	
	BSP_FPGA_Process();
	
	BSP_FPGA_PWR_Process();
	
	BSP_FPGA_FLASH_Process();
	
	for (i = 0; i < BSP_SFP_CNT; i++)
	{
		BSP_SFP_Process(i);
	}
}
