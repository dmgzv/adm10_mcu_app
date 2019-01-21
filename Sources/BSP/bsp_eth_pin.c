#include "bsp_eth_pin.h"

int32_t BSP_ETH_PIN_Init(BSP_ETH_PIN_T *Dev)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	/* Enable the GPIO Clock */
	BSP_ETH_PIN_EN_GPIO_CLK_ENABLE();
	/* Configure the GPIO pin */
	GPIO_InitStruct.Pin = BSP_ETH_PIN_EN_GPIO_PIN;
	GPIO_InitStruct.Mode = BSP_ETH_PIN_EN_GPIO_MODE;
	GPIO_InitStruct.Pull = BSP_ETH_PIN_EN_GPIO_PULL;
	GPIO_InitStruct.Speed = BSP_ETH_PIN_EN_GPIO_SPEED;
	HAL_GPIO_Init(BSP_ETH_PIN_EN_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_ETH_PIN_EN_GPIO_PORT, BSP_ETH_PIN_EN_GPIO_PIN, GPIO_PIN_RESET);
	
	/* Enable the GPIO Clock */
	BSP_ETH_PIN_SW_GPIO_CLK_ENABLE();
	/* Configure the GPIO pin */
	GPIO_InitStruct.Pin = BSP_ETH_PIN_SW_GPIO_PIN;
	GPIO_InitStruct.Mode = BSP_ETH_PIN_SW_GPIO_MODE;
	GPIO_InitStruct.Pull = BSP_ETH_PIN_SW_GPIO_PULL;
	GPIO_InitStruct.Speed = BSP_ETH_PIN_SW_GPIO_SPEED;
	HAL_GPIO_Init(BSP_ETH_PIN_SW_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_ETH_PIN_SW_GPIO_PORT, BSP_ETH_PIN_SW_GPIO_PIN, GPIO_PIN_RESET);
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);	
	return 0;
}
int32_t BSP_ETH_PIN_SetState(BSP_ETH_PIN_T *Dev, uint32_t Mask, uint32_t State)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (Mask & BSP_ETH_PIN_EN_MASK)
	{
		HAL_GPIO_WritePin(BSP_ETH_PIN_EN_GPIO_PORT, BSP_ETH_PIN_EN_GPIO_PIN, State ? GPIO_PIN_SET : GPIO_PIN_RESET);
		if (State)
			Dev->State |= BSP_ETH_PIN_EN_MASK;
		else
			Dev->State &= ~BSP_ETH_PIN_EN_MASK;
	}	
	if (Mask & BSP_ETH_PIN_SW_MASK)
	{
		HAL_GPIO_WritePin(BSP_ETH_PIN_SW_GPIO_PORT, BSP_ETH_PIN_SW_GPIO_PIN, State ? GPIO_PIN_SET : GPIO_PIN_RESET);
		if (State)
			Dev->State |= BSP_ETH_PIN_SW_MASK;
		else
			Dev->State &= ~BSP_ETH_PIN_SW_MASK;
	}
	
	BSP_DEV_UNLOCK(Dev);	
	return 0;
}

int32_t BSP_ETH_PIN_GetState(BSP_ETH_PIN_T *Dev, uint32_t Mask, uint32_t *State)
{
	if ((!Dev) || (!State))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (Mask & BSP_ETH_PIN_EN_MASK)
	{
		*State = HAL_GPIO_ReadPin(BSP_ETH_PIN_EN_GPIO_PORT, BSP_ETH_PIN_EN_GPIO_PIN) == GPIO_PIN_SET ? 1 : 0;
	}	
	else if (Mask & BSP_ETH_PIN_SW_MASK)
	{
		*State = HAL_GPIO_ReadPin(BSP_ETH_PIN_SW_GPIO_PORT, BSP_ETH_PIN_SW_GPIO_PIN) == GPIO_PIN_SET ? 1 : 0;
	}
	else
	{
		goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);	
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);	
	return -1;
}

