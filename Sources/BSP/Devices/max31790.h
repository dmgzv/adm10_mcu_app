#ifndef __MAX31790_H
#define __MAX31790_H

#include "bsp_dev.h"
#include "i2c.h"

#define MAX31790_DEV_NAME "MAX31790"

#define MAX31790_PWMOUTS_CNT	6
#define MAX31790_TACHS_CNT		12

#define MAX31790_PWMOUT_MODE_PWM	0
#define MAX31790_PWMOUT_MODE_RPM	1
#define MAX31790_PWMOUT_MODE_TACH	2

typedef struct {
	BSP_DEV;
	const I2C_TypeDef *I2C;
	uint8_t Addr;
	uint8_t GlobalConfig;
	uint8_t PwmFreq;
	uint8_t FanConfig[MAX31790_PWMOUTS_CNT];
	uint8_t FanDynamics[MAX31790_PWMOUTS_CNT];
	uint16_t TachCount[MAX31790_TACHS_CNT];
	uint16_t PwmDutyCycle[MAX31790_PWMOUTS_CNT];
	uint16_t PwmTargetDutyCycle[MAX31790_PWMOUTS_CNT];
	uint16_t TachTargetCount[MAX31790_PWMOUTS_CNT];
	uint8_t __FanUpdate[MAX31790_PWMOUTS_CNT];
} MAX31790_T;

int32_t MAX31790_ConfigPWMOUT(MAX31790_T *Dev, uint32_t Idx, uint8_t Mode, uint16_t PwmTargetDutyCycle, uint16_t TachTargetCount);

int32_t MAX31790_Init(MAX31790_T *Dev);
void MAX31790_Deinit(MAX31790_T *Dev);
int32_t MAX31790_Update(MAX31790_T *Dev);

#endif /* __MAX31790_H */
