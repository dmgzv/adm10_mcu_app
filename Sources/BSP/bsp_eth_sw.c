#include "bsp_eth_sw.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"

KSZ8895_T ksz8895;

static int32_t BSP_ETH_SW_InitPin(void *Priv, uint32_t Type);
static int32_t BSP_ETH_SW_SetPinState(void *Priv, uint32_t Pin, uint32_t State);

const BSP_PIN_CTRL_T ksz8895_ksz8895_pin_ctrl = {
	BSP_ETH_SW_InitPin,
	BSP_ETH_SW_SetPinState,
	NULL,
	NULL
};

static int32_t BSP_ETH_SW_InitPin(void *Priv, uint32_t Type)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	switch (Type)
	{
		case KSZ8895_PIN_SPI_CS:
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
		case KSZ8895_PIN_RESET:
			break;
		case KSZ8895_PIN_PWR_DOWN:
			break;
		default:
			return -1;
	}
	return 0;	
}

static int32_t BSP_ETH_SW_SetPinState(void *Priv, uint32_t Pin, uint32_t State)
{
	switch (Pin)
	{
		case KSZ8895_PIN_SPI_CS:
			HAL_GPIO_WritePin(BSP_ETH_SW_SPI_CS_GPIO_PORT, BSP_ETH_SW_SPI_CS_GPIO_PIN, State ? GPIO_PIN_SET : GPIO_PIN_RESET);
			break;
		case KSZ8895_PIN_RESET:
			break;
		case KSZ8895_PIN_PWR_DOWN:
			break;
		default:
			return -1;
	}
	return 0;	
}

static int32_t BSP_ETH_SW_CheckConditions(void)
{
	return 0;
}

void BSP_ETH_SW_Process(void)
{	
	if (BSP_ETH_SW_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&ksz8895) != BSP_DEV_STATE_RESET)
		{
			KSZ8895_Deinit(&ksz8895);
		}
	}
	else
	{
		/* External faults absent */
		if(BSP_DEV_GET_STATE(&ksz8895) != BSP_DEV_STATE_OK)
		{
			KSZ8895_Init(&ksz8895, BSP_ETH_SW_ID, BSP_ETH_SW_SPI, &ksz8895_ksz8895_pin_ctrl);
		}
		else
		{
			KSZ8895_Update(&ksz8895);
		}		
	}
}
