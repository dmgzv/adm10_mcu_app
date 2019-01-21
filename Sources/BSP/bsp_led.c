#include "bsp_led.h"

int32_t BSP_LED_Init(BSP_LED_T *Led, BSP_LED_TYPE_T Type)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	if (!Led)
		return -1;	
	switch (Type)
	{
		case BSP_LED_TYPE_POWER:  
			/* Enable the GPIO Clock */
			BSP_LED_POWER_GREEN_GPIO_CLK_ENABLE();
			/* Configure the GPIO pin */
			GPIO_InitStruct.Pin = BSP_LED_POWER_GREEN_GPIO_PIN;
			GPIO_InitStruct.Mode = BSP_LED_POWER_GREEN_GPIO_MODE;
			GPIO_InitStruct.Pull = BSP_LED_POWER_GREEN_GPIO_PULL;
			GPIO_InitStruct.Speed = BSP_LED_POWER_GREEN_GPIO_SPEED;
			HAL_GPIO_Init(BSP_LED_POWER_GREEN_GPIO_PORT, &GPIO_InitStruct);
			HAL_GPIO_WritePin(BSP_LED_POWER_GREEN_GPIO_PORT, BSP_LED_POWER_GREEN_GPIO_PIN, GPIO_PIN_SET);
			break;
		case BSP_LED_TYPE_ALARM:
			/* Enable the GPIO Clock */
			BSP_LED_ALARM_YELLOW_GPIO_CLK_ENABLE();
			/* Configure the GPIO pin */
			GPIO_InitStruct.Pin = BSP_LED_ALARM_YELLOW_GPIO_PIN;
			GPIO_InitStruct.Mode = BSP_LED_ALARM_YELLOW_GPIO_MODE;
			GPIO_InitStruct.Pull = BSP_LED_ALARM_YELLOW_GPIO_PULL;
			GPIO_InitStruct.Speed = BSP_LED_ALARM_YELLOW_GPIO_SPEED;
			HAL_GPIO_Init(BSP_LED_ALARM_YELLOW_GPIO_PORT, &GPIO_InitStruct);
			HAL_GPIO_WritePin(BSP_LED_ALARM_YELLOW_GPIO_PORT, BSP_LED_ALARM_YELLOW_GPIO_PIN, GPIO_PIN_RESET);
			/* Enable the GPIO Clock */
			BSP_LED_ALARM_RED_GPIO_CLK_ENABLE();
			/* Configure the GPIO pin */
			GPIO_InitStruct.Pin = BSP_LED_ALARM_RED_GPIO_PIN;
			GPIO_InitStruct.Mode = BSP_LED_ALARM_RED_GPIO_MODE;
			GPIO_InitStruct.Pull = BSP_LED_ALARM_RED_GPIO_PULL;
			GPIO_InitStruct.Speed = BSP_LED_ALARM_RED_GPIO_SPEED;
			HAL_GPIO_Init(BSP_LED_ALARM_RED_GPIO_PORT, &GPIO_InitStruct);
			HAL_GPIO_WritePin(BSP_LED_ALARM_RED_GPIO_PORT, BSP_LED_ALARM_RED_GPIO_PIN, GPIO_PIN_RESET);
			break;
		default:
			return -1;
	}
	Led->Type = Type;
	Led->Color = BSP_LED_COLOR_NO;
	return 0;
}

int32_t BSP_LED_Set(BSP_LED_T *Led, BSP_LED_COLOR_T Color)
{	
	if (!Led)
		return -1;	
	switch (Led->Type)
	{
		case BSP_LED_TYPE_POWER:
			switch (Color)
			{
				case BSP_LED_COLOR_NO:
					HAL_GPIO_WritePin(BSP_LED_POWER_GREEN_GPIO_PORT, BSP_LED_POWER_GREEN_GPIO_PIN, GPIO_PIN_SET);
					break;
				case BSP_LED_COLOR_GREEN:
					HAL_GPIO_WritePin(BSP_LED_POWER_GREEN_GPIO_PORT, BSP_LED_POWER_GREEN_GPIO_PIN, GPIO_PIN_RESET);
					break;
				case BSP_LED_COLOR_RED:
				case BSP_LED_COLOR_YELLOW:
				default:
					return -1;
			}
			break;
		case BSP_LED_TYPE_ALARM:
		switch (Color)
			{
				case BSP_LED_COLOR_NO:
					HAL_GPIO_WritePin(BSP_LED_ALARM_YELLOW_GPIO_PORT, BSP_LED_ALARM_YELLOW_GPIO_PIN, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(BSP_LED_ALARM_RED_GPIO_PORT, BSP_LED_ALARM_RED_GPIO_PIN, GPIO_PIN_RESET);
					break;
				case BSP_LED_COLOR_RED:
					HAL_GPIO_WritePin(BSP_LED_ALARM_YELLOW_GPIO_PORT, BSP_LED_ALARM_YELLOW_GPIO_PIN, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(BSP_LED_ALARM_RED_GPIO_PORT, BSP_LED_ALARM_RED_GPIO_PIN, GPIO_PIN_SET);
					break;
				case BSP_LED_COLOR_YELLOW:
					HAL_GPIO_WritePin(BSP_LED_ALARM_YELLOW_GPIO_PORT, BSP_LED_ALARM_YELLOW_GPIO_PIN, GPIO_PIN_SET);
					HAL_GPIO_WritePin(BSP_LED_ALARM_RED_GPIO_PORT, BSP_LED_ALARM_RED_GPIO_PIN, GPIO_PIN_RESET);
					break;
				case BSP_LED_COLOR_GREEN:
				default:
					return -1;
			}
			break;
		default:
			return -1;
	}
	return 0;
}
