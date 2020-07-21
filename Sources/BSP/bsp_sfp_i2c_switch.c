#include "bsp_sfp_i2c_switch.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"

static const PCA9548A_CH_T pca9548a_ch_set[24][BSP_PCA9548A_CNT] =
{
	{PCA9548A_CH_0,	PCA9548A_CH_NO,	PCA9548A_CH_NO}, /*Client 1 SFP*/
	{PCA9548A_CH_1,	PCA9548A_CH_NO,	PCA9548A_CH_NO}, /*Client 2 SFP*/
	{PCA9548A_CH_2,	PCA9548A_CH_NO,	PCA9548A_CH_NO}, /*Client 3 SFP*/
	{PCA9548A_CH_3,	PCA9548A_CH_NO,	PCA9548A_CH_NO}, /*Client 4 SFP*/
	{PCA9548A_CH_4, PCA9548A_CH_NO,	PCA9548A_CH_NO}, /*Client 5 SFP*/
	{PCA9548A_CH_5, PCA9548A_CH_NO,	PCA9548A_CH_NO}, /*Client 6 SFP*/
	{PCA9548A_CH_6, PCA9548A_CH_NO,	PCA9548A_CH_NO}, /*Client 7 SFP*/
	{PCA9548A_CH_7, PCA9548A_CH_NO,	PCA9548A_CH_NO}, /*Client 8 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_0,	PCA9548A_CH_NO}, /*Client 9 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_1,	PCA9548A_CH_NO}, /*Client 10 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_2,	PCA9548A_CH_NO}, /*Client 11 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_3,	PCA9548A_CH_NO}, /*Client 12 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_4,	PCA9548A_CH_NO}, /*Client 13 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_5,	PCA9548A_CH_NO}, /*Client 14 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_6,	PCA9548A_CH_NO}, /*Client 15 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_7,	PCA9548A_CH_NO}, /*Client 16 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO,	PCA9548A_CH_0}, /*GDT 2*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO,	PCA9548A_CH_1}, /*GDT 1*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO,	PCA9548A_CH_2}, /*SI53342 1*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO,	PCA9548A_CH_3}, /*SI53342 2*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO,	PCA9548A_CH_4}, /*Line 1 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO,	PCA9548A_CH_5}, /*Line 2 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO,	PCA9548A_CH_6}, /*Line 3 SFP*/
	{PCA9548A_CH_NO, PCA9548A_CH_NO,	PCA9548A_CH_7}, /*Line 4 SFP*/
};

PCA9548A_T pca9548a[BSP_PCA9548A_CNT] =
{
	{
		.DevName = PCA9548A_DEV_NAME,
		.DevId = BSP_PCA9548A_1_ID,
		.I2C = BSP_PCA9548A_1_I2C,
		.Addr = BSP_PCA9548A_1_ADDR,
		.PinCtrl = NULL,
	},
	{
		.DevName = PCA9548A_DEV_NAME,
		.DevId = BSP_PCA9548A_2_ID,
		.I2C = BSP_PCA9548A_2_I2C,
		.Addr = BSP_PCA9548A_2_ADDR,
		.PinCtrl = NULL,
	},
	{
		.DevName = PCA9548A_DEV_NAME,
		.DevId = BSP_PCA9548A_3_ID,
		.I2C = BSP_PCA9548A_3_I2C,
		.Addr = BSP_PCA9548A_3_ADDR,
		.PinCtrl = NULL,
	},
};

int32_t BSP_SFP_I2C_SWITCH_SelectChannel(uint32_t Channel)
{
	uint32_t i;
	
	if (Channel == BSP_SFP_I2C_SWITCH_CH_NO)
	{
		for (i = 0; i< BSP_PCA9548A_CNT; i++)
		{
			if (PCA9548A_SelectChannel(&pca9548a[i], PCA9548A_CH_NO))
				goto error;
		}		
	}
	else if (Channel < 24)
	{	
		for (i = 0; i< BSP_PCA9548A_CNT; i++)
		{
			if (PCA9548A_SelectChannel(&pca9548a[i], pca9548a_ch_set[Channel][i]))
				goto error;
		}
	}
	else
	{
		return -1;
	}

	return 0;
	
error:
	for (i = 0; i< BSP_PCA9548A_CNT; i++)
	{
		PCA9548A_SelectChannel(&pca9548a[i], PCA9548A_CH_NO);
	}
	return -1;
}

static int32_t BSP_SFP_I2C_SWITCH_CheckConditions(void)
{
	if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good))
		return -1;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	return 0;
}

void BSP_SFP_I2C_SWITCH_Process(void)
{
	uint32_t i;
	
	if (BSP_SFP_I2C_SWITCH_CheckConditions())
	{
		for (i = 0; i < BSP_PCA9548A_CNT; i++)
		{
			/* External faults present */
			if(BSP_DEV_GET_STATE(&pca9548a[i]) != BSP_DEV_STATE_RESET)
			{
				PCA9548A_Deinit(&pca9548a[i]);
			}
		}
	}
	else
	{		
		for (i = 0; i < BSP_PCA9548A_CNT; i++)
		{
			/* External faults present */
			if(BSP_DEV_GET_STATE(&pca9548a[i]) != BSP_DEV_STATE_OK)
			{
				PCA9548A_Init(&pca9548a[i]);
			}
		}
	}
}
