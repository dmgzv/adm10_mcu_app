#include "pca9548a.h"
#include "log.h"

#define PCA9548A_InitPin(Dev, Pin)						((Dev)->PinCtrl ? BSP_PIN_CTRL_Init((Dev)->PinCtrl, Pin) : 0)	
#define PCA9548A_SetPinState(Dev, Pin, State)	((Dev)->PinCtrl ? BSP_PIN_CTRL_SetState((Dev)->PinCtrl, Pin, State) : 0)

int32_t PCA9548A_SelectChannel(PCA9548A_T *Dev, PCA9548A_CH_T Channel)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	if (Channel	!= PCA9548A_CH_NO)
		BSP_DEV_LOCK(Dev);
	
	if (I2C_Transmit(Dev->I2C, Dev->Addr, (char*)&Channel, sizeof(Channel)))
		goto error;	
	Dev->Channel = Channel;
	
	if (Channel	== PCA9548A_CH_NO)
		BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}


int32_t PCA9548A_Init(PCA9548A_T *Dev)
{
	uint8_t Channel;
	
	if ((!Dev) || (!Dev->I2C) || (Dev->Addr < 0xE0) || (Dev->Addr > 0xEE))
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	if (PCA9548A_InitPin(Dev, PCA9548A_PIN_RESET))
		goto error;
	
	if (PCA9548A_SetPinState(Dev, PCA9548A_PIN_RESET, 1))
		goto error;
	
	Channel = PCA9548A_CH_NO;
	if (I2C_Transmit(Dev->I2C, Dev->Addr, (char*)&Channel, sizeof(Channel)))
		goto error;	
	Dev->Channel = (PCA9548A_CH_T)Channel;
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void PCA9548A_Deinit(PCA9548A_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
	if (Dev->Channel != PCA9548A_CH_NO)
	{
		Dev->Channel = PCA9548A_CH_NO;
		BSP_DEV_UNLOCK(Dev);
	}		
}
