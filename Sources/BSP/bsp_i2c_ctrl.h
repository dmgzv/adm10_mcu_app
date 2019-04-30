#ifndef __BSP_I2C_CTRL_H
#define __BSP_I2C_CTRL_H

#include "i2c.h"

typedef struct {
	I2C_TypeDef *I2C;
	int32_t (*ConfigSwitch)(void *Priv);
	int32_t (*ResetSwitch)(void *Priv);
	void *Priv;
} BSP_I2C_CTRL_T;

#define BSP_I2C_CTRL_ConfigSwitch(Dev)											((Dev)->ConfigSwitch ? (Dev)->ConfigSwitch((Dev)->Priv) : 0)
#define BSP_I2C_CTRL_ResetSwitch(Dev)												((Dev)->ResetSwitch ? (Dev)->ResetSwitch((Dev)->Priv) : 0)

#endif /* __BSP_I2C_CTRL_H */
