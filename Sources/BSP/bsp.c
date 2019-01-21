#include "bsp.h"
#include "mt48lc16m16a2.h"

int32_t BSP_Init(void)
{
	int32_t result = 0;

	/* STM32F4xx HAL library initialization:
		 - Configure the Flash prefetch, instruction and Data caches
		 - Configure the Systick to generate an interrupt each 1 msec
		 - Set NVIC Group Priority to 4
		 - Global MSP (MCU Support Package) initialization
	*/
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();
	
	return result;
}

int32_t BSP_InitPeriferia(void)
{
	int32_t result = 0;
	
	/* Configure the I2C */
	result |= I2C_Init();
	/* Configure the UART */
	result |= UART_Init();
	/* Configure the SPI */	
	result |= SPI_Init();
	return 0;
}

void BSP_DeinitPeriferia(void)
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
}

uint8_t BSP_GetSlotAddress(void)
{
	uint32_t i;
	uint8_t Addr;
	
	Addr = 0;
	for (i = 0; i < BSP_SLOT_ADDR_GPIO_CNT; i++)
	{
		if (HAL_GPIO_ReadPin((GPIO_TypeDef*)SlotAddressGPIOPort[i], SlotAddressGPIOPin[i]) == GPIO_PIN_SET)
			Addr |= 1 << i;
	}
	return Addr;
}

int32_t BSP_InitDevices(BSP_DEVICES_T *Devices)
{
	uint32_t i;
	int32_t result = 0;
	
	BSP_InitSlotAddress();
	/* Init Leds */
	BSP_LED_Init(&Devices->PowerLed, BSP_LED_TYPE_POWER);
	BSP_LED_Init(&Devices->AlarmLed, BSP_LED_TYPE_ALARM);
	BSP_LED_Set(&Devices->PowerLed, BSP_LED_COLOR_GREEN);	
	/* Init Power Pins */
	if (BSP_PWR_PIN_Init(&Devices->PowerPin))
		result = -1;
	/* Init Power*/
	if (BSP_PWR_Init(&Devices->Power, &Devices->PowerPin))
		result = -1;
	if (MT48LC16M16A2_Init())
		result = -1;
	/* Init Clocks*/
	//if (BSP_CLK_Init(&Devices->Clk))
	//	result = -1;
	/* Init SFP I2C Switches*/
	if (BSP_SFP_I2C_SWITCH_Init(&Devices->SfpSwitch))
		result = -1;	
	/* Init SFP Pins*/
	if (BSP_SFP_PIN_Init(&Devices->SfpPin, &Devices->SfpSwitch))
		result = -1;
	/* Init SFP Modules*/
	for (i = 0; i < BSP_SFP_CNT; i++)
	{
		if (BSP_SFP_Init(&Devices->Sfp[i], i, &Devices->SfpPin, &Devices->SfpSwitch))
			result = -1;
	}
		/* Init Ethernet Pins*/
	if (BSP_ETH_PIN_Init(&Devices->EthPin))
		result = -1;
	/* Init Ethernet Switch*/
	if (BSP_ETH_SW_Init(&Devices->EthSw))
		result = -1;
	/* Init Flash*/
	if (BSP_FLASH_Init(&Devices->Flash))
		result = -1;
	/* Perform Random Flash Test*/
	//if (BSP_FLASH_RandomTest(&Devices->Flash))
	//	result = -1;
	if (result)
		BSP_LED_Set(&Devices->AlarmLed, BSP_LED_COLOR_RED);
	
	return result;
}

void BSP_DeinitDevices(BSP_DEVICES_T *Devices)
{
	/* Init Leds */
	BSP_LED_Set(&Devices->AlarmLed, BSP_LED_COLOR_NO);
	BSP_LED_Set(&Devices->PowerLed, BSP_LED_COLOR_NO);
}

int32_t BSP_UpdateDevices(BSP_DEVICES_T *Devices, BSP_DEVICES_CFG_T *Cfg)
{
	uint32_t i;
	
	BSP_PWR_Update(&Devices->Power);
	for (i = 0; i < BSP_SFP_CNT; i++)
	{
		BSP_SFP_Update(&Devices->Sfp[i], &Cfg->Sfp[i]);
	}
	BSP_ETH_SW_Update(&Devices->EthSw);
	return 0;
}
