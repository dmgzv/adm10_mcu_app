#include "pca9548a.h"

int32_t PCA9548A_SelectChannel(PCA9548A_T *Dev, PCA9548A_CH_T Channel)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (I2C_Transmit(Dev->I2C, Dev->Addr, (char*)&Channel, sizeof(Channel)))
		goto error;	
	Dev->Channel = Channel;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}


int32_t PCA9548A_Init(PCA9548A_T *Dev, I2C_TypeDef *I2C, uint8_t Addr)
{
	uint8_t Channel;
	
	if ((!Dev) || (!I2C) || (Addr < 0x70) || (Addr > 0x77))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->Addr = Addr << 1;
	Dev->I2C = I2C;
	
	Channel = 0;
	if (I2C_Transmit(Dev->I2C, Dev->Addr, (char*)&Channel, sizeof(Channel)))
		goto error;	
	Dev->Channel = PCA9548A_CH_NO;
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}
