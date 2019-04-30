#include "max16046.h"



int32_t MAX16046_Init(MAX16046_T *dev, I2C_TypeDef *I2C, uint8_t Addr)
{
	if ((!dev) || (!I2C))
		return -1;
	
	dev->I2C = I2C;
	dev->Addr = Addr;
	
	return 0;	
}
