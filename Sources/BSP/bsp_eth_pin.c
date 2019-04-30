#include "bsp_eth_pin.h"

BSP_ETH_PIN_T bsp_eth_pin;

static int32_t BSP_ETH_PIN_Init(BSP_ETH_PIN_T *Dev)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	uint32_t State;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	State = 0;
	
	/* Enable the GPIO Clock */
	BSP_ETH_PIN_EN_GPIO_CLK_ENABLE();
	/* Configure the GPIO pin */
	GPIO_InitStruct.Pin = BSP_ETH_PIN_EN_GPIO_PIN;
	GPIO_InitStruct.Mode = BSP_ETH_PIN_EN_GPIO_MODE;
	GPIO_InitStruct.Pull = BSP_ETH_PIN_EN_GPIO_PULL;
	GPIO_InitStruct.Speed = BSP_ETH_PIN_EN_GPIO_SPEED;
	HAL_GPIO_Init(BSP_ETH_PIN_EN_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_ETH_PIN_EN_GPIO_PORT, BSP_ETH_PIN_EN_GPIO_PIN, GPIO_PIN_RESET);
	State |= HAL_GPIO_ReadPin(BSP_ETH_PIN_EN_GPIO_PORT, BSP_ETH_PIN_EN_GPIO_PIN) == GPIO_PIN_SET ? BSP_ETH_PIN_EN_MASK : 0;
	
	/* Enable the GPIO Clock */
	BSP_ETH_PIN_SW_GPIO_CLK_ENABLE();
	/* Configure the GPIO pin */
	GPIO_InitStruct.Pin = BSP_ETH_PIN_SW_GPIO_PIN;
	GPIO_InitStruct.Mode = BSP_ETH_PIN_SW_GPIO_MODE;
	GPIO_InitStruct.Pull = BSP_ETH_PIN_SW_GPIO_PULL;
	GPIO_InitStruct.Speed = BSP_ETH_PIN_SW_GPIO_SPEED;
	HAL_GPIO_Init(BSP_ETH_PIN_SW_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_ETH_PIN_SW_GPIO_PORT, BSP_ETH_PIN_SW_GPIO_PIN, GPIO_PIN_RESET);
	State |= HAL_GPIO_ReadPin(BSP_ETH_PIN_SW_GPIO_PORT, BSP_ETH_PIN_SW_GPIO_PIN) == GPIO_PIN_SET ? BSP_ETH_PIN_SW_MASK : 0;
	
	/* Enable the GPIO Clock */
	BSP_ETH_PIN_SEL_GPIO_CLK_ENABLE();
	/* Configure the GPIO pin */
	GPIO_InitStruct.Pin = BSP_ETH_PIN_SEL_GPIO_PIN;
	GPIO_InitStruct.Mode = BSP_ETH_PIN_SEL_GPIO_MODE;
	GPIO_InitStruct.Pull = BSP_ETH_PIN_SEL_GPIO_PULL;
	GPIO_InitStruct.Speed = BSP_ETH_PIN_SEL_GPIO_SPEED;
	HAL_GPIO_Init(BSP_ETH_PIN_SEL_GPIO_PORT, &GPIO_InitStruct);
	State |= HAL_GPIO_ReadPin(BSP_ETH_PIN_SEL_GPIO_PORT, BSP_ETH_PIN_SEL_GPIO_PIN) == GPIO_PIN_SET ? BSP_ETH_PIN_SEL_MASK : 0;
	
	Dev->State = State;
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);	
	return 0;
}

static void BSP_ETH_PIN_Deinit(BSP_ETH_PIN_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}

static int32_t BSP_ETH_PIN_Update(BSP_ETH_PIN_T *Dev)
{
	uint32_t State;
	
	if ((!Dev))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	State = 0;
	State |= HAL_GPIO_ReadPin(BSP_ETH_PIN_SEL_GPIO_PORT, BSP_ETH_PIN_SEL_GPIO_PIN) == GPIO_PIN_RESET ? BSP_ETH_PIN_SEL_MASK : 0;
	
	if (State & BSP_ETH_PIN_SEL_MASK)
		HAL_GPIO_WritePin(BSP_ETH_PIN_EN_GPIO_PORT, BSP_ETH_PIN_SW_GPIO_PIN, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(BSP_ETH_PIN_EN_GPIO_PORT, BSP_ETH_PIN_SW_GPIO_PIN, GPIO_PIN_RESET);
	
	State |= HAL_GPIO_ReadPin(BSP_ETH_PIN_EN_GPIO_PORT, BSP_ETH_PIN_EN_GPIO_PIN) == GPIO_PIN_SET ? BSP_ETH_PIN_EN_MASK : 0;
	State |= HAL_GPIO_ReadPin(BSP_ETH_PIN_SW_GPIO_PORT, BSP_ETH_PIN_SW_GPIO_PIN) == GPIO_PIN_SET ? BSP_ETH_PIN_SW_MASK : 0;
	
	Dev->State = State;
	
	BSP_DEV_UNLOCK(Dev);	
	return 0;
}


static int32_t BSP_ETH_PIN_CheckConditions(void)
{
	return 0;
}

void BSP_ETH_PIN_Process(void)
{
	if (BSP_ETH_PIN_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&bsp_eth_pin) != BSP_DEV_STATE_RESET)
		{
			BSP_ETH_PIN_Deinit(&bsp_eth_pin);
		}
	}
	else
	{
		/* External faults absent */
		if(BSP_DEV_GET_STATE(&bsp_eth_pin) != BSP_DEV_STATE_OK)
		{
			BSP_ETH_PIN_Init(&bsp_eth_pin);
		}
		else
		{
			BSP_ETH_PIN_Update(&bsp_eth_pin);
		}		
	}
}

int32_t BSP_ETH_PIN_SetState(uint32_t Pin, uint32_t State)
{
	if (BSP_DEV_GET_STATE(&bsp_eth_pin) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(&bsp_eth_pin);
	
	if (Pin == BSP_ETH_PIN_EN)
	{
		HAL_GPIO_WritePin(BSP_ETH_PIN_EN_GPIO_PORT, BSP_ETH_PIN_EN_GPIO_PIN, State ? GPIO_PIN_SET : GPIO_PIN_RESET);
		if (State)
			bsp_eth_pin.State |= BSP_ETH_PIN_EN_MASK;
		else
			bsp_eth_pin.State &= ~BSP_ETH_PIN_EN_MASK;
	}	
	else if (Pin == BSP_ETH_PIN_SW)
	{
		HAL_GPIO_WritePin(BSP_ETH_PIN_SW_GPIO_PORT, BSP_ETH_PIN_SW_GPIO_PIN, State ? GPIO_PIN_SET : GPIO_PIN_RESET);
		if (State)
			bsp_eth_pin.State |= BSP_ETH_PIN_SW_MASK;
		else
			bsp_eth_pin.State &= ~BSP_ETH_PIN_SW_MASK;
	}
	else
	{
		goto error;
	}
	
	BSP_DEV_UNLOCK(&bsp_eth_pin);	
	return 0;

error:
	BSP_DEV_UNLOCK(&bsp_eth_pin);	
	return -1;
}

int32_t BSP_ETH_PIN_GetState(uint32_t Pin, uint32_t *State)
{
	if (BSP_DEV_GET_STATE(&bsp_eth_pin) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(&bsp_eth_pin);
	
	if (Pin == BSP_ETH_PIN_EN)
	{
		*State = HAL_GPIO_ReadPin(BSP_ETH_PIN_EN_GPIO_PORT, BSP_ETH_PIN_EN_GPIO_PIN) == GPIO_PIN_SET ? 0 : 1;
	}	
	else if (Pin == BSP_ETH_PIN_SW)
	{
		*State = HAL_GPIO_ReadPin(BSP_ETH_PIN_SW_GPIO_PORT, BSP_ETH_PIN_SW_GPIO_PIN) == GPIO_PIN_SET ? 0 : 1;
	}
	else if (Pin == BSP_ETH_PIN_SEL)
	{
		*State = HAL_GPIO_ReadPin(BSP_ETH_PIN_SEL_GPIO_PORT, BSP_ETH_PIN_SEL_GPIO_PIN) == GPIO_PIN_SET ? 0 : 1;
	}
	else
	{
		goto error;
	}
	
	BSP_DEV_UNLOCK(&bsp_eth_pin);	
	return 0;

error:
	BSP_DEV_UNLOCK(&bsp_eth_pin);	
	return -1;
}
