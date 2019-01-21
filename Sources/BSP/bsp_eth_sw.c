#include "bsp_eth_sw.h"

static int32_t BSP_ETH_SW_InitPin(void *Priv, KSZ8895_PIN_TYPE_T Type)
{
	BSP_ETH_SW_T *Dev = Priv;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	switch (Type)
	{
		case KSZ8895_PIN_TYPE_SPI_CS:
			/* Enable the GPIO Clock */
			BSP_ETH_SW_SPI_CS_GPIO_CLK_ENABLE();
			/* Configure the GPIO pin */
			GPIO_InitStruct.Pin = BSP_ETH_SW_SPI_CS_GPIO_PIN;
			GPIO_InitStruct.Mode = BSP_ETH_SW_SPI_CS_GPIO_MODE;
			GPIO_InitStruct.Pull = BSP_ETH_SW_SPI_CS_GPIO_PULL;
			GPIO_InitStruct.Speed = BSP_ETH_SW_SPI_CS_GPIO_SPEED;
			HAL_GPIO_Init(BSP_ETH_SW_SPI_CS_GPIO_PORT, &GPIO_InitStruct);
			HAL_GPIO_WritePin(BSP_ETH_SW_SPI_CS_GPIO_PORT, BSP_ETH_SW_SPI_CS_GPIO_PIN, GPIO_PIN_SET);
			break;
		case KSZ8895_PIN_TYPE_RESET:
			break;
		case KSZ8895_PIN_TYPE_PWR_DOWN:
			break;
		default:
			return -1;
	}
	return 0;	
}

static int32_t BSP_ETH_SW_SetPinState(void *Priv, KSZ8895_PIN_TYPE_T Type, KSZ8895_PIN_STATE_T State)
{
	BSP_ETH_SW_T *Dev = Priv;
	
	switch (Type)
	{
		case KSZ8895_PIN_TYPE_SPI_CS:
			HAL_GPIO_WritePin(BSP_ETH_SW_SPI_CS_GPIO_PORT, BSP_ETH_SW_SPI_CS_GPIO_PIN, State == KSZ8895_PIN_STATE_ASSERT ? GPIO_PIN_SET : GPIO_PIN_RESET);
			break;
		case KSZ8895_PIN_TYPE_RESET:
			break;
		case KSZ8895_PIN_TYPE_PWR_DOWN:
			break;
		default:
			return -1;
	}
	return 0;	
}

int32_t BSP_ETH_SW_Init(BSP_ETH_SW_T *Dev)
{
	KSZ8895_PIN_T ksz8895_pin;
	
	if (!Dev)
		return -1;
	
	KSZ8895_InitPin(&ksz8895_pin, BSP_ETH_SW_InitPin, BSP_ETH_SW_SetPinState, Dev);
	if (KSZ8895_Init(&Dev->ksz8895, BSP_ETH_SW_SPI, &ksz8895_pin))
		return -1;
	
	return 0;
}

#define KSZ8895_MAC_TABLE_SIZE 64
static KSZ8895_MAC_TABLE_T ksz_mac_table[KSZ8895_MAC_TABLE_SIZE];

int32_t BSP_ETH_SW_Update(BSP_ETH_SW_T *Dev)
{
	uint32_t i;
	uint32_t ksz_mac_table_size;
	
	if (!Dev)
		return -1;
	
	if (KSZ8895_ReadMACTable(&Dev->ksz8895, ksz_mac_table, KSZ8895_MAC_TABLE_SIZE, &ksz_mac_table_size))
		return -1;
	
	for (i = 0; i < KSZ8895_PORTS_CNT; i++)
	{
		if (KSZ8895_UpdatePortStatus(&Dev->ksz8895, i))
			return -1;
	}
	
	return 0;
}
