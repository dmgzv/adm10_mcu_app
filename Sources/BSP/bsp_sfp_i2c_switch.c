#include "bsp_sfp_i2c_switch.h"

#ifdef BSP_CONFIG_USE_FREERTOS
#define	BSP_SFP_I2C_SWITCH_msDelay(ms) 		osDelay(ms)
#else
#define	BSP_SFP_I2C_SWITCH_msDelay(ms) 		HAL_Delay(ms)
#endif

static const I2C_TypeDef *pca9548a_i2c[BSP_SFP_I2C_SWITCH_PCA9548_CNT] = 
{
	BSP_SFP_I2C_SWITCH_PCA9548_1_I2C,
	BSP_SFP_I2C_SWITCH_PCA9548_2_I2C
};

static const I2C_TypeDef *pca9546a_i2c[BSP_SFP_I2C_SWITCH_PCA9546_CNT] = 
{
	BSP_SFP_I2C_SWITCH_PCA9546_1_I2C,
};

static const uint8_t pca9548a_addr[BSP_SFP_I2C_SWITCH_PCA9548_CNT] =
{
	BSP_SFP_I2C_SWITCH_PCA9548_1_I2C_ADDR,
	BSP_SFP_I2C_SWITCH_PCA9548_2_I2C_ADDR
};

static const uint8_t pca9546a_addr[BSP_SFP_I2C_SWITCH_PCA9546_CNT] =
{
	BSP_SFP_I2C_SWITCH_PCA9546_1_I2C_ADDR
};

static const PCA9548A_CH_T pca9548a_ch_set[20][BSP_SFP_I2C_SWITCH_PCA9548_CNT] =
{
	{PCA9548A_CH_0,	PCA9548A_CH_NO}, /*Client 1 SFP*/
	{PCA9548A_CH_1,	PCA9548A_CH_NO}, /*Client 2 SFP*/
	{PCA9548A_CH_2,	PCA9548A_CH_NO}, /*Client 3 SFP*/
	{PCA9548A_CH_3,	PCA9548A_CH_NO}, /*Client 4 SFP*/
	{PCA9548A_CH_4, PCA9548A_CH_NO}, /*Client 5 SFP*/
	{PCA9548A_CH_5, PCA9548A_CH_NO}, /*Client 6 SFP*/
	{PCA9548A_CH_6, PCA9548A_CH_NO}, /*Client 7 SFP*/
	{PCA9548A_CH_7, PCA9548A_CH_NO}, /*Client 8 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_0}, /*Client 9 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_1}, /*Client 10 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_2}, /*Client 11 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_3}, /*Client 12 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_4}, /*Client 13 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_5}, /*Client 14 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_6}, /*Client 15 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_7}, /*Client 16 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO}, /*Line 1 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO}, /*Line 2 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO}, /*Line 3 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO}, /*Line 4 SFP*/
};

static const PCA9546A_CH_T pca9546a_ch_set[20][BSP_SFP_I2C_SWITCH_PCA9546_CNT] =
{
	{PCA9546A_CH_NO}, /*Client 1 SFP*/
	{PCA9546A_CH_NO}, /*Client 2 SFP*/
	{PCA9546A_CH_NO}, /*Client 3 SFP*/
	{PCA9546A_CH_NO}, /*Client 4 SFP*/
	{PCA9546A_CH_NO}, /*Client 5 SFP*/
	{PCA9546A_CH_NO}, /*Client 6 SFP*/
	{PCA9546A_CH_NO}, /*Client 7 SFP*/
	{PCA9546A_CH_NO}, /*Client 8 SFP*/
	{PCA9546A_CH_NO}, /*Client 9 SFP*/
	{PCA9546A_CH_NO}, /*Client 10 SFP*/
	{PCA9546A_CH_NO}, /*Client 11 SFP*/
	{PCA9546A_CH_NO}, /*Client 12 SFP*/
	{PCA9546A_CH_NO}, /*Client 13 SFP*/
	{PCA9546A_CH_NO}, /*Client 14 SFP*/
	{PCA9546A_CH_NO}, /*Client 15 SFP*/
	{PCA9546A_CH_NO}, /*Client 16 SFP*/
	{PCA9546A_CH_0}, /*Line 1 SFP*/
	{PCA9546A_CH_1}, /*Line 2 SFP*/
	{PCA9546A_CH_2}, /*Line 3 SFP*/
	{PCA9546A_CH_3}, /*Line 4 SFP*/
};

static void BSP_SFP_I2C_SWITCH_InitPins(BSP_SFP_I2C_SWITCH_T *dev)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	/* Enable the GPIO Clock */
	BSP_SFP_I2C_SWITCH_RESET_GPIO_CLK_ENABLE();
	/* Configure the GPIO pin */
	GPIO_InitStruct.Pin = BSP_SFP_I2C_SWITCH_RESET_GPIO_PIN;
	GPIO_InitStruct.Mode = BSP_SFP_I2C_SWITCH_RESET_GPIO_MODE;
	GPIO_InitStruct.Pull = BSP_SFP_I2C_SWITCH_RESET_GPIO_PULL;
	GPIO_InitStruct.Speed = BSP_SFP_I2C_SWITCH_RESET_GPIO_SPEED;
	HAL_GPIO_Init(BSP_SFP_I2C_SWITCH_RESET_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_SFP_I2C_SWITCH_RESET_GPIO_PORT, BSP_SFP_I2C_SWITCH_RESET_GPIO_PIN, GPIO_PIN_RESET);
	BSP_SFP_I2C_SWITCH_msDelay(10);
	HAL_GPIO_WritePin(BSP_SFP_I2C_SWITCH_RESET_GPIO_PORT, BSP_SFP_I2C_SWITCH_RESET_GPIO_PIN, GPIO_PIN_SET);
	BSP_SFP_I2C_SWITCH_msDelay(10);
}

int32_t BSP_SFP_I2C_SWITCH_SelectChannel(BSP_SFP_I2C_SWITCH_T *dev, uint32_t Channel)
{
	uint32_t i;
	int32_t result = 0;
	
	if (Channel > 19)
		return -1;
	
	for (i = 0; i< BSP_SFP_I2C_SWITCH_PCA9548_CNT; i++)
	{
		if (PCA9548A_SelectChannel(&dev->pca9548a[i], pca9548a_ch_set[Channel][i]))
			result = -1;
	}
	
	for (i = 0; i< BSP_SFP_I2C_SWITCH_PCA9546_CNT; i++)
	{
		if (PCA9546A_SelectChannel(&dev->pca9546a[i], pca9546a_ch_set[Channel][i]))
			result = -1;
	}
	
	dev->Channel = Channel;
	return result;
}
int32_t BSP_SFP_I2C_SWITCH_Init(BSP_SFP_I2C_SWITCH_T *dev)
{
	uint32_t i;
	int32_t result = 0;
	
	BSP_SFP_I2C_SWITCH_InitPins(dev);
	
	for (i = 0; i< BSP_SFP_I2C_SWITCH_PCA9548_CNT; i++)
	{
		if (PCA9548A_Init(&dev->pca9548a[i], (I2C_TypeDef*)pca9548a_i2c[i], pca9548a_addr[i]))
			result = -1;
	}
	
	for (i = 0; i< BSP_SFP_I2C_SWITCH_PCA9546_CNT; i++)
	{
		if (PCA9546A_Init(&dev->pca9546a[i], (I2C_TypeDef*)pca9546a_i2c[i], pca9546a_addr[i]))
			result = -1;
	}
	
	if (BSP_SFP_I2C_SWITCH_SelectChannel(dev, 0))
		return -1;
	
	return result;
}
