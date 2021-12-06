#include "max31790.h"

#define MAX31790_REG_GLOBAL_CONFIG	0x00
#define MAX31790_REG_PWM_FREQ				0x01
#define MAX31790_REG_FAN_CONFIG(Idx)		(0x02 + (Idx))
#define MAX31790_REG_FAN_DYNAMICS(Idx)	(0x08 + (Idx))
#define MAX31790_REG_TACH_COUNT(Idx)		(0x18 + ((Idx) * 2))
#define MAX31790_REG_PWM_DUTY_CYCLE(Idx)		(0x30 + ((Idx) * 2))
#define MAX31790_REG_PWM_TARGET_DUTY_CYCLE(Idx)		(0x40 + ((Idx) * 2))
#define MAX31790_REG_TACH_TARGET_COUNT(Idx)		(0x50 + ((Idx) * 2))

int32_t MAX31790_ConfigPWMOUT(MAX31790_T *Dev, uint32_t Idx, uint8_t Mode, uint16_t PwmTargetDutyCycle, uint16_t TachTargetCount)
{
	if ((!Dev) || (Idx >= MAX31790_PWMOUTS_CNT))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (Mode == MAX31790_PWMOUT_MODE_PWM)
	{
		if (Dev->FanConfig[Idx] != 0x08)
		{
			Dev->FanConfig[Idx] = 0x08;
			Dev->__FanUpdate[Idx] = 1;
		}
		if (Dev->PwmTargetDutyCycle[Idx] != PwmTargetDutyCycle)
		{
			Dev->PwmTargetDutyCycle[Idx] = PwmTargetDutyCycle << 7;
			Dev->TachTargetCount[Idx] = 0xFFE0;
			Dev->__FanUpdate[Idx] = 1;
		}
	}
	else if (Mode == MAX31790_PWMOUT_MODE_RPM)
	{
		if (Dev->FanConfig[Idx] != 0x90)
		{
			Dev->FanConfig[Idx] = 0x90;
			Dev->__FanUpdate[Idx] = 1;
		}
		if (Dev->TachTargetCount[Idx] != TachTargetCount)
		{
			Dev->TachTargetCount[Idx] = TachTargetCount << 5;
			Dev->__FanUpdate[Idx] = 1;
		}
	}
	else if (Mode == MAX31790_PWMOUT_MODE_TACH)
	{
		if (Dev->FanConfig[Idx] != 0x19)
		{
			Dev->FanConfig[Idx] = 0x19;
			Dev->__FanUpdate[Idx] = 1;
		}
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t MAX31790_Init(MAX31790_T *Dev)
{
	uint32_t i;
	uint8_t Addr;
	uint8_t Data[2];
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	Addr = MAX31790_REG_GLOBAL_CONFIG;
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Dev->GlobalConfig, 1))
		goto error;
	
	Addr = MAX31790_REG_PWM_FREQ;
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Dev->PwmFreq, 1))
		goto error;
	
	for (i = 0; i < MAX31790_PWMOUTS_CNT; i++)
	{
		Addr = MAX31790_REG_FAN_CONFIG(i);
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Dev->FanConfig[i], 1))
			goto error;
		Addr = MAX31790_REG_FAN_DYNAMICS(i);
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Dev->FanDynamics[i], 1))
			goto error;
		Addr = MAX31790_REG_PWM_DUTY_CYCLE(i);
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, 1, (char*)Data, 2))
			goto error;
		Dev->PwmDutyCycle[i] = (Data[0] << 8) | Data[1];
		
		Addr = MAX31790_REG_PWM_TARGET_DUTY_CYCLE(i);
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, 1, (char*)Data, 2))
			goto error;
		Dev->PwmTargetDutyCycle[i] = (Data[0] << 8) | Data[1];
		
		Addr = MAX31790_REG_TACH_TARGET_COUNT(i);
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, 1, (char*)Data, 2))
			goto error;
		Dev->TachTargetCount[i] = (Data[0] << 8) | Data[1];
	}
	
	for (i = 0; i < MAX31790_TACHS_CNT; i++)
	{
		Addr = MAX31790_REG_TACH_COUNT(i);
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, 1, (char*)Data, 2))
			goto error;
		Dev->TachCount[i] = (Data[0] << 8) | Data[1];
	}
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void MAX31790_Deinit(MAX31790_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}

int32_t MAX31790_Update(MAX31790_T *Dev)
{
	uint8_t Data[2];
	uint32_t i;
	uint8_t Addr;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	for (i = 0; i < MAX31790_PWMOUTS_CNT; i++)
	{
		if (Dev->__FanUpdate[i])
		{
			Addr = MAX31790_REG_FAN_CONFIG(i);
			if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Dev->FanConfig[i], 1))
				goto error;
			Addr = MAX31790_REG_FAN_DYNAMICS(i);
			if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Dev->FanDynamics[i], 1))
				goto error;
			Data[0] = Dev->PwmTargetDutyCycle[i] >> 8;
			Data[1] = Dev->PwmTargetDutyCycle[i];
			Addr = MAX31790_REG_PWM_TARGET_DUTY_CYCLE(i);
			if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Addr, 1, (char*)Data, 2))
				goto error;
			Data[0] = Dev->TachTargetCount[i] >> 8;
			Data[1] = Dev->TachTargetCount[i];
			Addr = MAX31790_REG_TACH_TARGET_COUNT(i);
			if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Addr, 1, (char*)Data, 2))
				goto error;
			Dev->__FanUpdate[i] = 0;
		}
		Addr = MAX31790_REG_PWM_DUTY_CYCLE(i);
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, 1, (char*)Data, 2))
			goto error;
		Dev->PwmDutyCycle[i] = (Data[0] << 8) | Data[1];
	}
	
	for (i = 0; i < MAX31790_TACHS_CNT; i++)
	{
		Addr = MAX31790_REG_TACH_COUNT(i);
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, 1, (char*)Data, 2))
			goto error;
		Dev->TachCount[i] = (Data[0] << 8) | Data[1];
	}	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}
