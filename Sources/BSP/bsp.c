#include "bsp.h"
#include "mt48lc16m16a2.h"

uint32_t BSP_Stage = 0;

uint8_t SlotAddress;
const BSP_DEV_T *Devices[] = {
	(BSP_DEV_T*)&at45db641e,
	(BSP_DEV_T*)&mt48lc16m16a2,
	(BSP_DEV_T*)&dim3r3400,
	(BSP_DEV_T*)&pwr_pca9536,
	(BSP_DEV_T*)&ucd90120a,
	(BSP_DEV_T*)&ksz8895,
	(BSP_DEV_T*)&gdt080a[0],
	(BSP_DEV_T*)&gdt080a[1],
	(BSP_DEV_T*)&si5332[0],
	(BSP_DEV_T*)&si5332[1],
	(BSP_DEV_T*)&s29gl01gs,
	(BSP_DEV_T*)&fpga_iface,
	(BSP_DEV_T*)&fpga_maxv,
	(BSP_DEV_T*)&fpga_fw,
	(BSP_DEV_T*)&fpga_line_adm_10,
	(BSP_DEV_T*)&fpga_client_adm_10[0],
	(BSP_DEV_T*)&fpga_client_adm_10[1],
	(BSP_DEV_T*)&fpga_line_agg_2x10_bs,
	(BSP_DEV_T*)&fpga_client_agg_2x10_bs[0],
	(BSP_DEV_T*)&fpga_client_agg_2x10_bs[1],
	(BSP_DEV_T*)&fpga_line_agg_2x10_efec,
	(BSP_DEV_T*)&fpga_client_agg_2x10_efec[0],
	(BSP_DEV_T*)&fpga_client_agg_2x10_efec[1],
	(BSP_DEV_T*)&fpga_client_adm_2_5[0],
	(BSP_DEV_T*)&fpga_client_adm_2_5[1],
	(BSP_DEV_T*)&pca9548a[0],
	(BSP_DEV_T*)&pca9548a[1],
	(BSP_DEV_T*)&pca9548a[2],
	(BSP_DEV_T*)&pcal6524[0],
	(BSP_DEV_T*)&pcal6524[1],
	(BSP_DEV_T*)&pcal6524[2],
	(BSP_DEV_T*)&pca9536[0],
	(BSP_DEV_T*)&pca9536[1],
	(BSP_DEV_T*)&pca9536[2],
	(BSP_DEV_T*)&pca9536[3],
	(BSP_DEV_T*)&pca9536[4],
	(BSP_DEV_T*)&pca9536[5],
	(BSP_DEV_T*)&pca9536[6],
	(BSP_DEV_T*)&pca9536[7],
	(BSP_DEV_T*)&pca9536[8],
	(BSP_DEV_T*)&pca9536[9],
	(BSP_DEV_T*)&pca9536[10],
	(BSP_DEV_T*)&pca9536[11],
	(BSP_DEV_T*)&pca9536[12],
	(BSP_DEV_T*)&pca9536[13],
	(BSP_DEV_T*)&pca9536[14],
	(BSP_DEV_T*)&pca9536[15],
	(BSP_DEV_T*)&pca9536[16],
	(BSP_DEV_T*)&pca9536[17],
	(BSP_DEV_T*)&pca9536[18],
	(BSP_DEV_T*)&pca9536[19],
};
const uint32_t DevicesCnt = sizeof(Devices) / sizeof(Devices[0]);

const BSP_DEV_T *Modules[] = {
	(BSP_DEV_T*)&sfp[0],
	(BSP_DEV_T*)&sfp[1],
	(BSP_DEV_T*)&sfp[2],
	(BSP_DEV_T*)&sfp[3],
	(BSP_DEV_T*)&sfp[4],
	(BSP_DEV_T*)&sfp[5],
	(BSP_DEV_T*)&sfp[6],
	(BSP_DEV_T*)&sfp[7],
	(BSP_DEV_T*)&sfp[8],
	(BSP_DEV_T*)&sfp[9],
	(BSP_DEV_T*)&sfp[10],
	(BSP_DEV_T*)&sfp[11],
	(BSP_DEV_T*)&sfp[12],
	(BSP_DEV_T*)&sfp[13],
	(BSP_DEV_T*)&sfp[14],
	(BSP_DEV_T*)&sfp[15],
	(BSP_DEV_T*)&sfp[16],
	(BSP_DEV_T*)&sfp[17],
	(BSP_DEV_T*)&sfp[18],
	(BSP_DEV_T*)&sfp[19],
	(BSP_DEV_T*)&seeprom,
	(BSP_DEV_T*)&ps[0],
	(BSP_DEV_T*)&ps[1],
};
const uint32_t ModulesCnt = sizeof(Modules) / sizeof(Modules[0]);

void os_assert_failed(char* file, unsigned int line)
{
	LOG_Error("OS", "", "ASSERT", "%s [%d]", file, line);
	osDelay(1000);
	BSP_Reset();
}

void hal_assert_failed(char* file, unsigned int line)
{
	LOG_Error("HAL", "", "ASSERT", "%s [%d]", file, line);
	osDelay(1000);
	BSP_Reset();
}

void lwip_assert_failed(char* file, unsigned int line)
{
	LOG_Error("LWIP", "", "ASSERT", "%s [%d]", file, line);
	osDelay(1000);
	BSP_Reset();
}

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
	/* Configure the RTC */
	RTC_Init();
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
	//if (Addr)
	//	Addr = (Addr << 1) - 1;
	SlotAddress = Addr;
}

void BSP_UpdateSlotAddress(void)
{
	uint32_t i;
	uint8_t Addr;
	
	Addr = 0;
	for (i = 0; i < BSP_SLOT_ADDR_GPIO_CNT; i++)
	{
		if (HAL_GPIO_ReadPin((GPIO_TypeDef*)SlotAddressGPIOPort[i], SlotAddressGPIOPin[i]) == GPIO_PIN_SET)
			Addr |= 1 << i;
	}
	//if (Addr)
	//	Addr = (Addr << 1) - 1;
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

	BSP_Stage = 0;
	
	//BSP_PWR_IN_Process();
	
	BSP_Stage++;
	
	BSP_PWR_PIN_Process();
	
	BSP_Stage++;
	
	BSP_PWR_SEQ_Process();
	
	BSP_Stage++;
	
	BSP_ETH_PIN_Process();
	
	BSP_Stage++;
	
	BSP_ETH_SW_Process();
	
	BSP_Stage++;
	
	BSP_ETH_Process();
	
	BSP_Stage++;
	
	BSP_SFP_I2C_SWITCH_Process();
	
	BSP_Stage++;
	
	BSP_SFP_PIN_Process();
	
	BSP_Stage++;
	
	BSP_CLK_Process();
	
	BSP_Stage++;
	
	/*Processing is disabled because it cause errors with pca9548a*/
	//BSP_FPGA_PWR_Process();
	
	BSP_Stage++;
	
	BSP_FPGA_IFACE_Process();
	
	BSP_Stage++;
	
	BSP_FPGA_MAXV_Process();
	
	BSP_Stage++;
	
	BSP_FPGA_FLASH_Process();
	
	BSP_Stage++;
	
	BSP_FPGA_FW_Process();
	
	BSP_Stage++;
	
	BSP_FPGA_LINE_Process();
	
	BSP_Stage++;
	
	BSP_FPGA_CLIENT_Process();
	
	BSP_Stage++;
	
	for (i = 0; i < BSP_SFP_CNT; i++)
	{
		BSP_SFP_Process(i);
		
		BSP_Stage++;
	}
	
	BSP_SEEPROM_Process();
	
	BSP_Stage++;
	
	BSP_PS_Process();
	
	BSP_Stage++;
}
