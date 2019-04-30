#ifndef __SI5342_H
#define __SI5342_H

#include "i2c.h"
#include "bsp_dev.h"
#include "bsp_pin_ctrl.h"

#define SI5342_STATE_IN_0_LOS (1 << 0)
#define SI5342_STATE_IN_1_LOS (1 << 1)
#define SI5342_STATE_IN_2_LOS (1 << 2)
#define SI5342_STATE_IN_3_LOS (1 << 3)
#define SI5342_STATE_INTR 		(1 << 4)
#define SI5342_STATE_XAXB_LOS (1 << 5)
#define SI5342_STATE_LOL 			(1 << 6)

#define SI5342_PIN_RESET 		0
#define SI5342_PIN_I2C_SEL	1
#define SI5342_PIN_IN_0_LOS	2
#define SI5342_PIN_IN_1_LOS	3
#define SI5342_PIN_IN_2_LOS	4
#define SI5342_PIN_IN_3_LOS	5
#define SI5342_PIN_INTR			6
#define SI5342_PIN_XAXB_LOS	7
#define SI5342_PIN_LOL			8

typedef struct { 
	uint16_t Addr;
	uint8_t Data;
}SI5342_CFG_T;

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	BSP_PIN_CTRL_T *PinCtrl;
	void *Priv;
	uint8_t Addr;
	uint8_t State;
	uint16_t PtNumber;
} SI5342_T;

int32_t SI5342_Init(SI5342_T *Dev, I2C_TypeDef *I2C, uint8_t Addr, BSP_PIN_CTRL_T *PinCtrl, const SI5342_CFG_T *Config, const uint32_t ConfigSize);
void SI5342_Deinit(SI5342_T *Dev);
int32_t SI5342_Update(SI5342_T *Dev);	

#endif /* __SI5342_H */
