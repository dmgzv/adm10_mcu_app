#include "bsp.h"
#include "mt48lc16m16a2.h"
#include "dm.h"
#include "dm_adm_10.h"

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

extern struct dm g_dm;

static osThreadId SFPThreadHandle;

static void BSP_SFP_Thread(void const *argument);

osThreadDef(SFP, BSP_SFP_Thread, osPriorityNormal, 0, 512);

const schema_node_t* sn_adm_10_2020_03_20__adm__client__admin_state[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____admin_state, &sn_adm_10_2020_03_20__adm__client___1_____admin_state, &sn_adm_10_2020_03_20__adm__client___2_____admin_state, &sn_adm_10_2020_03_20__adm__client___3_____admin_state,
	&sn_adm_10_2020_03_20__adm__client___4_____admin_state, &sn_adm_10_2020_03_20__adm__client___5_____admin_state, &sn_adm_10_2020_03_20__adm__client___6_____admin_state, &sn_adm_10_2020_03_20__adm__client___7_____admin_state,
	&sn_adm_10_2020_03_20__adm__client___8_____admin_state, &sn_adm_10_2020_03_20__adm__client___9_____admin_state, &sn_adm_10_2020_03_20__adm__client___10_____admin_state, &sn_adm_10_2020_03_20__adm__client___11_____admin_state,
	&sn_adm_10_2020_03_20__adm__client___12_____admin_state, &sn_adm_10_2020_03_20__adm__client___13_____admin_state, &sn_adm_10_2020_03_20__adm__client___14_____admin_state, &sn_adm_10_2020_03_20__adm__client___15_____admin_state,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__service_state[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____service_state, &sn_adm_10_2020_03_20__adm__client___1_____service_state, &sn_adm_10_2020_03_20__adm__client___2_____service_state, &sn_adm_10_2020_03_20__adm__client___3_____service_state,
	&sn_adm_10_2020_03_20__adm__client___4_____service_state, &sn_adm_10_2020_03_20__adm__client___5_____service_state, &sn_adm_10_2020_03_20__adm__client___6_____service_state, &sn_adm_10_2020_03_20__adm__client___7_____service_state,
	&sn_adm_10_2020_03_20__adm__client___8_____service_state, &sn_adm_10_2020_03_20__adm__client___9_____service_state, &sn_adm_10_2020_03_20__adm__client___10_____service_state, &sn_adm_10_2020_03_20__adm__client___11_____service_state,
	&sn_adm_10_2020_03_20__adm__client___12_____service_state, &sn_adm_10_2020_03_20__adm__client___13_____service_state, &sn_adm_10_2020_03_20__adm__client___14_____service_state, &sn_adm_10_2020_03_20__adm__client___15_____service_state,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__module_type[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____module_type, &sn_adm_10_2020_03_20__adm__client___1_____module_type, &sn_adm_10_2020_03_20__adm__client___2_____module_type, &sn_adm_10_2020_03_20__adm__client___3_____module_type,
	&sn_adm_10_2020_03_20__adm__client___4_____module_type, &sn_adm_10_2020_03_20__adm__client___5_____module_type, &sn_adm_10_2020_03_20__adm__client___6_____module_type, &sn_adm_10_2020_03_20__adm__client___7_____module_type,
	&sn_adm_10_2020_03_20__adm__client___8_____module_type, &sn_adm_10_2020_03_20__adm__client___9_____module_type, &sn_adm_10_2020_03_20__adm__client___10_____module_type, &sn_adm_10_2020_03_20__adm__client___11_____module_type,
	&sn_adm_10_2020_03_20__adm__client___12_____module_type, &sn_adm_10_2020_03_20__adm__client___13_____module_type, &sn_adm_10_2020_03_20__adm__client___14_____module_type, &sn_adm_10_2020_03_20__adm__client___15_____module_type,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__sfp__vendor[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___1_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___2_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___3_____sfp__vendor,
	&sn_adm_10_2020_03_20__adm__client___4_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___5_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___6_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___7_____sfp__vendor,
	&sn_adm_10_2020_03_20__adm__client___8_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___9_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___10_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___11_____sfp__vendor,
	&sn_adm_10_2020_03_20__adm__client___12_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___13_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___14_____sfp__vendor, &sn_adm_10_2020_03_20__adm__client___15_____sfp__vendor,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__sfp__serial_number[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___1_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___2_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___3_____sfp__serial_number,
	&sn_adm_10_2020_03_20__adm__client___4_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___5_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___6_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___7_____sfp__serial_number,
	&sn_adm_10_2020_03_20__adm__client___8_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___9_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___10_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___11_____sfp__serial_number,
	&sn_adm_10_2020_03_20__adm__client___12_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___13_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___14_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__client___15_____sfp__serial_number,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__sfp__part_number[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___1_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___2_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___3_____sfp__part_number,
	&sn_adm_10_2020_03_20__adm__client___4_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___5_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___6_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___7_____sfp__part_number,
	&sn_adm_10_2020_03_20__adm__client___8_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___9_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___10_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___11_____sfp__part_number,
	&sn_adm_10_2020_03_20__adm__client___12_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___13_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___14_____sfp__part_number, &sn_adm_10_2020_03_20__adm__client___15_____sfp__part_number,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__sfp__temp[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___1_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___2_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___3_____sfp__temp,
	&sn_adm_10_2020_03_20__adm__client___4_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___5_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___6_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___7_____sfp__temp,
	&sn_adm_10_2020_03_20__adm__client___8_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___9_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___10_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___11_____sfp__temp,
	&sn_adm_10_2020_03_20__adm__client___12_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___13_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___14_____sfp__temp, &sn_adm_10_2020_03_20__adm__client___15_____sfp__temp,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__sfp__temp_fault[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___1_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___2_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___3_____sfp__temp_fault,
	&sn_adm_10_2020_03_20__adm__client___4_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___5_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___6_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___7_____sfp__temp_fault,
	&sn_adm_10_2020_03_20__adm__client___8_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___9_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___10_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___11_____sfp__temp_fault,
	&sn_adm_10_2020_03_20__adm__client___12_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___13_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___14_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__client___15_____sfp__temp_fault,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__sfp__rx_port__channel__optical_pwr[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___1_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___2_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___3_____sfp__rx_port__channel__optical_pwr,
	&sn_adm_10_2020_03_20__adm__client___4_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___5_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___6_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___7_____sfp__rx_port__channel__optical_pwr,
	&sn_adm_10_2020_03_20__adm__client___8_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___9_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___10_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___11_____sfp__rx_port__channel__optical_pwr,
	&sn_adm_10_2020_03_20__adm__client___12_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___13_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___14_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___15_____sfp__rx_port__channel__optical_pwr,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__sfp__rx_port__channel__optical_pwr_flt[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___1_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___2_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___3_____sfp__rx_port__channel__optical_pwr_flt,
	&sn_adm_10_2020_03_20__adm__client___4_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___5_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___6_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___7_____sfp__rx_port__channel__optical_pwr_flt,
	&sn_adm_10_2020_03_20__adm__client___8_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___9_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___10_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___11_____sfp__rx_port__channel__optical_pwr_flt,
	&sn_adm_10_2020_03_20__adm__client___12_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___13_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___14_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___15_____sfp__rx_port__channel__optical_pwr_flt,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__sfp__tx_port__channel__optical_pwr[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___1_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___2_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___3_____sfp__tx_port__channel__optical_pwr,
	&sn_adm_10_2020_03_20__adm__client___4_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___5_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___6_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___7_____sfp__tx_port__channel__optical_pwr,
	&sn_adm_10_2020_03_20__adm__client___8_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___9_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___10_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___11_____sfp__tx_port__channel__optical_pwr,
	&sn_adm_10_2020_03_20__adm__client___12_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___13_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___14_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__client___15_____sfp__tx_port__channel__optical_pwr,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__sfp__tx_port__channel__optical_pwr_flt[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___1_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___2_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___3_____sfp__tx_port__channel__optical_pwr_flt,
	&sn_adm_10_2020_03_20__adm__client___4_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___5_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___6_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___7_____sfp__tx_port__channel__optical_pwr_flt,
	&sn_adm_10_2020_03_20__adm__client___8_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___9_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___10_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___11_____sfp__tx_port__channel__optical_pwr_flt,
	&sn_adm_10_2020_03_20__adm__client___12_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___13_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___14_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__client___15_____sfp__tx_port__channel__optical_pwr_flt,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__client__sfp__tx_port__laser_enable[ADM_10_2020_03_20__ADM__CLIENT_CNT] = {
	&sn_adm_10_2020_03_20__adm__client___0_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___1_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___2_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___3_____sfp__tx_port__laser_enable,
	&sn_adm_10_2020_03_20__adm__client___4_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___5_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___6_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___7_____sfp__tx_port__laser_enable,
	&sn_adm_10_2020_03_20__adm__client___8_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___9_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___10_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___11_____sfp__tx_port__laser_enable,
	&sn_adm_10_2020_03_20__adm__client___12_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___13_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___14_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__client___15_____sfp__tx_port__laser_enable,
};

static void BSP_SFP_UpdateClientConfig(void)
{
	uint32_t i;
	uint32_t AdminState, ModuleType, LaserEnable;
	
	for (i = 0; i < ADM_10_2020_03_20__ADM__CLIENT_CNT; i++)
	{
		dm_get_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__admin_state[i], &AdminState);
		if (AdminState == ADM_10_2020_03_20__ADM__CLIENT__ADMIN_STATE_ENUM_DSBLD)
		{
			sfp[BSP_SFP_CLIENT_IDX(i)].Pin.TxDisable = 1;
		}
		else
		{
			dm_get_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__module_type[i], &ModuleType);
			if (ModuleType == ADM_10_2020_03_20__ADM__CLIENT__MODULE_TYPE_ENUM_SFP)
			{
				dm_get_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__sfp__tx_port__laser_enable[i], &LaserEnable);
				sfp[BSP_SFP_CLIENT_IDX(i)].Pin.TxDisable = (LaserEnable == ADM_10_2020_03_20__ADM__CLIENT__SFP__TX_PORT__LASER_ENABLE_ENUM_DISABLE) ? 1 : 0;
			}	
		}
	}
}

static void BSP_SFP_UpdateClientState(void)
{
	uint32_t i;
	uint32_t DeviceServiceState;
	uint32_t AdminState, ServiceState, ModuleType;
	uint8_t SFPTempFault, SFPRxPwrFault, SFPTxPwrFault;
	
	dm_get_param_value(&g_dm, &sn_adm_10_2020_03_20__adm__service_state, &DeviceServiceState);
	
	for (i = 0; i < ADM_10_2020_03_20__ADM__CLIENT_CNT; i++)
	{
		dm_get_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__admin_state[i], &AdminState);
		if ((AdminState == ADM_10_2020_03_20__ADM__CLIENT__ADMIN_STATE_ENUM_DSBLD) || (DeviceServiceState == 1))
		{
			/* Update service state */
			ServiceState = 1;
			dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__service_state[i], &ServiceState);
		}
		else
		{
			if (BSP_DEV_GET_STATE(&sfp[BSP_SFP_CLIENT_IDX(i)]) == BSP_DEV_STATE_OK)
			{
				dm_get_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__module_type[i], &ModuleType);
				if (ModuleType == ADM_10_2020_03_20__ADM__CLIENT__MODULE_TYPE_ENUM_SFP)
				{
					/* Update service state */
					if (DeviceServiceState == 4)
					{
						ServiceState = 4;
					}
					else
					{
						if (AdminState == ADM_10_2020_03_20__ADM__CLIENT__ADMIN_STATE_ENUM_IS)
							ServiceState = 5;
						else
							ServiceState = 4;
					}
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__service_state[i], &ServiceState);
					
					/*Update SFP Inventory*/
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__sfp__vendor[i], sfp[BSP_SFP_CLIENT_IDX(i)].Info.Vendor);
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__sfp__serial_number[i], sfp[BSP_SFP_CLIENT_IDX(i)].Info.SerialNumber);
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__sfp__part_number[i], sfp[BSP_SFP_CLIENT_IDX(i)].Info.PartNumber);
							
					/*Update SFP Temperature*/
					SFPTempFault = 0;
					if ((sfp[BSP_SFP_CLIENT_IDX(i)].Temp > sfp[BSP_SFP_CLIENT_IDX(i)].Thr.Temp[SFP_THR_CMAX]) || (sfp[BSP_SFP_CLIENT_IDX(i)].Temp < sfp[BSP_SFP_CLIENT_IDX(i)].Thr.Temp[SFP_THR_CMIN]))
						SFPTempFault = 1;
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__sfp__temp[i], &sfp[BSP_SFP_CLIENT_IDX(i)].Temp);
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__sfp__temp_fault[i], &SFPTempFault);
					
					/*Update SFP Rx Optical Power*/
					SFPRxPwrFault = 0;
					if ((sfp[BSP_SFP_CLIENT_IDX(i)].RxPwr > sfp[BSP_SFP_CLIENT_IDX(i)].Thr.RxPwr[SFP_THR_CMAX]) || (sfp[BSP_SFP_CLIENT_IDX(i)].RxPwr < sfp[BSP_SFP_CLIENT_IDX(i)].Thr.RxPwr[SFP_THR_CMIN]))
						SFPRxPwrFault = 1;
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__sfp__rx_port__channel__optical_pwr[i], &sfp[BSP_SFP_CLIENT_IDX(i)].RxPwr);
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__sfp__rx_port__channel__optical_pwr_flt[i], &SFPRxPwrFault);
					
					/*Update SFP Tx Optical Power*/
					SFPTxPwrFault = 0;
					if ((sfp[BSP_SFP_CLIENT_IDX(i)].TxPwr > sfp[BSP_SFP_CLIENT_IDX(i)].Thr.TxPwr[SFP_THR_CMAX]) || (sfp[BSP_SFP_CLIENT_IDX(i)].TxPwr < sfp[BSP_SFP_CLIENT_IDX(i)].Thr.TxPwr[SFP_THR_CMIN]))
						SFPTxPwrFault = 1;						
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__sfp__tx_port__channel__optical_pwr[i], &sfp[BSP_SFP_CLIENT_IDX(i)].TxPwr);
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__sfp__tx_port__channel__optical_pwr_flt[i], &SFPTxPwrFault);
					
				}
				else
				{
					/* Update service state */
					ServiceState = 3;
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__service_state[i], &ServiceState);
				}
			}
			else
			{
				/* Update service state */
				ServiceState = 2;
				dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__client__service_state[i], &ServiceState);
			}
		}
	}
}

const schema_node_t* sn_adm_10_2020_03_20__adm__line__admin_state[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____admin_state, &sn_adm_10_2020_03_20__adm__line___1_____admin_state, &sn_adm_10_2020_03_20__adm__line___2_____admin_state, &sn_adm_10_2020_03_20__adm__line___3_____admin_state,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__service_state[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____service_state, &sn_adm_10_2020_03_20__adm__line___1_____service_state, &sn_adm_10_2020_03_20__adm__line___2_____service_state, &sn_adm_10_2020_03_20__adm__line___3_____service_state,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__module_type[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____module_type, &sn_adm_10_2020_03_20__adm__line___1_____module_type, &sn_adm_10_2020_03_20__adm__line___2_____module_type, &sn_adm_10_2020_03_20__adm__line___3_____module_type,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__sfp__vendor[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____sfp__vendor, &sn_adm_10_2020_03_20__adm__line___1_____sfp__vendor, &sn_adm_10_2020_03_20__adm__line___2_____sfp__vendor, &sn_adm_10_2020_03_20__adm__line___3_____sfp__vendor,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__sfp__serial_number[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__line___1_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__line___2_____sfp__serial_number, &sn_adm_10_2020_03_20__adm__line___3_____sfp__serial_number,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__sfp__part_number[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____sfp__part_number, &sn_adm_10_2020_03_20__adm__line___1_____sfp__part_number, &sn_adm_10_2020_03_20__adm__line___2_____sfp__part_number, &sn_adm_10_2020_03_20__adm__line___3_____sfp__part_number,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__sfp__temp[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____sfp__temp, &sn_adm_10_2020_03_20__adm__line___1_____sfp__temp, &sn_adm_10_2020_03_20__adm__line___2_____sfp__temp, &sn_adm_10_2020_03_20__adm__line___3_____sfp__temp,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__sfp__temp_fault[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__line___1_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__line___2_____sfp__temp_fault, &sn_adm_10_2020_03_20__adm__line___3_____sfp__temp_fault,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__sfp__rx_port__channel__optical_pwr[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__line___1_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__line___2_____sfp__rx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__line___3_____sfp__rx_port__channel__optical_pwr,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__sfp__rx_port__channel__optical_pwr_flt[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__line___1_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__line___2_____sfp__rx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__line___3_____sfp__rx_port__channel__optical_pwr_flt,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__sfp__tx_port__channel__optical_pwr[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__line___1_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__line___2_____sfp__tx_port__channel__optical_pwr, &sn_adm_10_2020_03_20__adm__line___3_____sfp__tx_port__channel__optical_pwr,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__sfp__tx_port__channel__optical_pwr_flt[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__line___1_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__line___2_____sfp__tx_port__channel__optical_pwr_flt, &sn_adm_10_2020_03_20__adm__line___3_____sfp__tx_port__channel__optical_pwr_flt,
};

const schema_node_t* sn_adm_10_2020_03_20__adm__line__sfp__tx_port__laser_enable[ADM_10_2020_03_20__ADM__LINE_CNT] = {
	&sn_adm_10_2020_03_20__adm__line___0_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__line___1_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__line___2_____sfp__tx_port__laser_enable, &sn_adm_10_2020_03_20__adm__line___3_____sfp__tx_port__laser_enable,
};

static void BSP_SFP_UpdateLineConfig(void)
{
	uint32_t i;
	uint32_t AdminState, ModuleType, LaserEnable;
	
	for (i = 0; i < ADM_10_2020_03_20__ADM__LINE_CNT; i++)
	{
		dm_get_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__admin_state[i], &AdminState);
		if (AdminState == ADM_10_2020_03_20__ADM__LINE__ADMIN_STATE_ENUM_DSBLD)
		{
			sfp[BSP_SFP_LINE_IDX(i)].Pin.TxDisable = 1;
		}
		else
		{
			dm_get_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__module_type[i], &ModuleType);
			if (ModuleType == ADM_10_2020_03_20__ADM__LINE__MODULE_TYPE_ENUM_SFP)
			{
				dm_get_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__sfp__tx_port__laser_enable[i], &LaserEnable);
				sfp[BSP_SFP_LINE_IDX(i)].Pin.TxDisable = (LaserEnable == ADM_10_2020_03_20__ADM__LINE__SFP__TX_PORT__LASER_ENABLE_ENUM_DISABLE) ? 1 : 0;
			}	
		}
	}
}

static void BSP_SFP_UpdateLineState(void)
{
	uint32_t i;
	uint32_t DeviceServiceState;
	uint32_t AdminState, ServiceState, ModuleType;
	uint8_t SFPTempFault, SFPRxPwrFault, SFPTxPwrFault;
	
	dm_get_param_value(&g_dm, &sn_adm_10_2020_03_20__adm__service_state, &DeviceServiceState);
	
	for (i = 0; i < ADM_10_2020_03_20__ADM__LINE_CNT; i++)
	{
		dm_get_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__admin_state[i], &AdminState);
		if ((AdminState == ADM_10_2020_03_20__ADM__LINE__ADMIN_STATE_ENUM_DSBLD) || (DeviceServiceState == 1))
		{
			/* Update service state */
			ServiceState = 1;
			dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__service_state[i], &ServiceState);
		}
		else
		{
			if (BSP_DEV_GET_STATE(&sfp[BSP_SFP_LINE_IDX(i)]) == BSP_DEV_STATE_OK)
			{
				dm_get_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__module_type[i], &ModuleType);
				if (ModuleType == ADM_10_2020_03_20__ADM__LINE__MODULE_TYPE_ENUM_SFP)
				{
					/* Update service state */
					if (DeviceServiceState == 4)
					{
						ServiceState = 4;
					}
					else
					{
						if (AdminState == ADM_10_2020_03_20__ADM__LINE__ADMIN_STATE_ENUM_IS)
							ServiceState = 5;
						else
							ServiceState = 4;
					}
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__service_state[i], &ServiceState);
					
					/*Update SFP Inventory*/
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__sfp__vendor[i], sfp[BSP_SFP_LINE_IDX(i)].Info.Vendor);
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__sfp__serial_number[i], sfp[BSP_SFP_LINE_IDX(i)].Info.SerialNumber);
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__sfp__part_number[i], sfp[BSP_SFP_LINE_IDX(i)].Info.PartNumber);
							
					/*Update SFP Temperature*/
					SFPTempFault = 0;
					if ((sfp[BSP_SFP_LINE_IDX(i)].Temp > sfp[BSP_SFP_LINE_IDX(i)].Thr.Temp[SFP_THR_CMAX]) || (sfp[BSP_SFP_LINE_IDX(i)].Temp < sfp[BSP_SFP_LINE_IDX(i)].Thr.Temp[SFP_THR_CMIN]))
						SFPTempFault = 1;
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__sfp__temp[i], &sfp[BSP_SFP_LINE_IDX(i)].Temp);
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__sfp__temp_fault[i], &SFPTempFault);
					
					/*Update SFP Rx Optical Power*/
					SFPRxPwrFault = 0;
					if ((sfp[BSP_SFP_LINE_IDX(i)].RxPwr > sfp[BSP_SFP_LINE_IDX(i)].Thr.RxPwr[SFP_THR_CMAX]) || (sfp[BSP_SFP_LINE_IDX(i)].RxPwr < sfp[BSP_SFP_LINE_IDX(i)].Thr.RxPwr[SFP_THR_CMIN]))
						SFPRxPwrFault = 1;
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__sfp__rx_port__channel__optical_pwr[i], &sfp[BSP_SFP_LINE_IDX(i)].RxPwr);
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__sfp__rx_port__channel__optical_pwr_flt[i], &SFPRxPwrFault);
					
					/*Update SFP Tx Optical Power*/
					SFPTxPwrFault = 0;
					if ((sfp[BSP_SFP_LINE_IDX(i)].TxPwr > sfp[BSP_SFP_LINE_IDX(i)].Thr.TxPwr[SFP_THR_CMAX]) || (sfp[BSP_SFP_LINE_IDX(i)].TxPwr < sfp[BSP_SFP_LINE_IDX(i)].Thr.TxPwr[SFP_THR_CMIN]))
						SFPTxPwrFault = 1;						
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__sfp__tx_port__channel__optical_pwr[i], &sfp[BSP_SFP_LINE_IDX(i)].TxPwr);
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__sfp__tx_port__channel__optical_pwr_flt[i], &SFPTxPwrFault);
					
				}
				else
				{
					/* Update service state */
					ServiceState = 3;
					dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__service_state[i], &ServiceState);
				}
			}
			else
			{				
				/* Update service state */
				ServiceState = 2;
				dm_set_param_value(&g_dm, sn_adm_10_2020_03_20__adm__line__service_state[i], &ServiceState);
			}
		}
	}
}

static void BSP_SFP_UpdateConfig(void)
{
	BSP_SFP_UpdateClientConfig();
	BSP_SFP_UpdateLineConfig();
}

static void BSP_SFP_UpdateState(void)
{
	BSP_SFP_UpdateClientState();
	BSP_SFP_UpdateLineState();
}

static void BSP_SFP_Thread(void const *argument)
{
	uint32_t Time, i;
	
	while(1)
	{
		Time = osKernelSysTick();
		BSP_SFP_UpdateConfig();
		BSP_SFP_I2C_SWITCH_Process();
	
		BSP_SFP_PIN_Process();
		
		BSP_CLK_Process();
		
		//BSP_FPGA_PWR_Process();
		
		BSP_FPGA_Process();
		
		//BSP_FPGA_FLASH_Process();
		
		for (i = 0; i < BSP_SFP_CNT; i++)
		{
			BSP_SFP_Process(i);
		}
		BSP_SFP_UpdateState();
		osDelayUntil(&Time, 1000);
	}
}

void BSP_UpdateCardConfig(void)
{
	
}

void BSP_UpdateCardState(void)
{
	float Temp;
	uint8_t TempFault, PowerFault, HardwareFault;
	uint32_t SlotAdminState, SlotServiceState;
	uint32_t DeviceAdminState, DeviceServiceState;
	uint32_t i;
	
	/* Update Slot Service State*/
	dm_get_param_value(&g_dm, &sn___HIDDEN____admin_state, &SlotAdminState);
	if (SlotAdminState == __HIDDEN____ADMIN_STATE_ENUM_DSBLD)
		SlotServiceState = 1;
	else if (SlotAdminState == __HIDDEN____ADMIN_STATE_ENUM_MT)
		SlotServiceState = 4;
	else
		SlotServiceState = 5;
	dm_set_param_value(&g_dm, &sn___HIDDEN____service_state, &SlotServiceState);	
	
	/* Update Device Service State*/
	dm_get_param_value(&g_dm, &sn_adm_10_2020_03_20__adm__admin_state, &DeviceAdminState);
	if ((DeviceAdminState == ADM_10_2020_03_20__ADM__ADMIN_STATE_ENUM_DSBLD) || (SlotServiceState == 1))
	{
		DeviceServiceState = 1;
	}
	else
	{
		if (SlotServiceState == 4)
		{
			DeviceServiceState = 4;
		}
		else
		{
			if (DeviceAdminState == ADM_10_2020_03_20__ADM__ADMIN_STATE_ENUM_MT)
				DeviceServiceState = 4;
			else
				DeviceServiceState = 5;
		}
	}
	dm_set_param_value(&g_dm, &sn_adm_10_2020_03_20__adm__service_state, &DeviceServiceState);
	
	/*Update Inventory Values*/
	dm_set_param_value(&g_dm, &sn_adm_10_2020_03_20__vendor, "IPG Photonics");
	dm_set_param_value(&g_dm, &sn_adm_10_2020_03_20__part_number, "ADM-10-SFP/SFP+-H8");
	dm_set_param_value(&g_dm, &sn_adm_10_2020_03_20__serial_number, "0123-4567-89AB");
	dm_set_param_value(&g_dm, &sn_adm_10_2020_03_20__hardware_number, BSP_HW_VERSION);
	dm_set_param_value(&g_dm, &sn_adm_10_2020_03_20__software_number, BSP_SW_VERSION);
	
	/*Update Temperature*/
	Temp = 36.7;
	dm_set_param_value(&g_dm, &sn_adm_10_2020_03_20__temp, &Temp);
	TempFault = 0;
	if ((Temp > 45.0f) || (Temp < 5.0f))
		TempFault = 1;
	dm_set_param_value(&g_dm, &sn_adm_10_2020_03_20__temp_fault, &TempFault);
	
	/*Update Power Fault*/
	if (BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
	{
		 PowerFault = 1;
	}
	else
	{
		if (!dim3r3400_power_good)
			PowerFault = 1;
	}

	if (BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK)
	{
		PowerFault = 1;
	}
	else
	{
		for (i = 0; i < UCD90120A_RAILS_CNT; i++)
		{
			//if (!UCD90120A_RAIL_POWER_GOOD(&ucd90120a, i))
			if (!(ucd90120a.__PowerGood & (1 << (i))))
			{
				PowerFault = 1;
			}
		}
	}
	dm_set_param_value(&g_dm, &sn_adm_10_2020_03_20__power_fault, &PowerFault);
	
	/*Update Hardware Fault*/
	if (BSP_DEV_GET_STATE(&at45db641e) != BSP_DEV_STATE_OK)
		HardwareFault = 1;
	if (BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
		HardwareFault = 1;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		HardwareFault = 1;
	if (BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK)
		HardwareFault = 1;
	/*
	for (i = 0; i < BSP_FPGA_PWR_GDT080A_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&gdt080a[i]) != BSP_DEV_STATE_OK)
			HardwareFault = 1;
	}
	*/
	for (i = 0; i < BSP_CLK_SI5332_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&si5332[i]) != BSP_DEV_STATE_OK)
			HardwareFault = 1;
	}
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
		HardwareFault = 1;
	if (BSP_DEV_GET_STATE(&fpga_line) != BSP_DEV_STATE_OK)
		HardwareFault = 1;
	if (BSP_DEV_GET_STATE(&fpga_client[0]) != BSP_DEV_STATE_OK)
		HardwareFault = 1;
	if (BSP_DEV_GET_STATE(&fpga_client[1]) != BSP_DEV_STATE_OK)
		HardwareFault = 1;
	if (BSP_DEV_GET_STATE(&ksz8895) != BSP_DEV_STATE_OK)
		HardwareFault = 1;
	for (i = 0; i < BSP_SFP_PIN_PCA9536_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&pca9536[i]) != BSP_DEV_STATE_OK)
			HardwareFault = 1;
	}	
	for (i = 0; i < BSP_SFP_PIN_PCAL6524_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&pcal6524[i]) != BSP_DEV_STATE_OK)
			HardwareFault = 1;
	}
	for (i = 0; i < BSP_PCA9548_CNT; i++)
	{
		if (BSP_DEV_GET_STATE(&pca9548a[i]) != BSP_DEV_STATE_OK)
			HardwareFault = 1;
	}
	dm_set_param_value(&g_dm, &sn_adm_10_2020_03_20__hardware_fault, &HardwareFault);
	
}

void BSP_UpdateCard(void)
{
	BSP_UpdateCardConfig();
	
	BSP_PWR_IN_Process();
	
	BSP_PWR_PIN_Process();
	
	BSP_PWR_SEQ_Process();
	
	BSP_ETH_PIN_Process();
	
	BSP_ETH_SW_Process();
	
	BSP_ETH_Process(SlotAddress);
	
	if (!SFPThreadHandle)
		SFPThreadHandle = osThreadCreate (osThread(SFP), NULL);
	
	BSP_UpdateCardState();
}
