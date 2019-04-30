#include "si5342.h"

#ifdef BSP_CONFIG_USE_FREERTOS
#define	SI5342_msDelay(ms) 		osDelay(ms)
#else
#define	SI5342_msDelay(ms) 		HAL_Delay(ms)
#endif

#define SI5342_InitPin(Dev, Pin)						((Dev)->PinCtrl ? BSP_PIN_CTRL_Init((Dev)->PinCtrl, Pin) : 0)	
#define SI5342_SetPinState(Dev, Pin, State)	((Dev)->PinCtrl ? BSP_PIN_CTRL_SetState((Dev)->PinCtrl, Pin, State) : 0)
#define SI5342_GetPinState(Dev, Pin, State)	((Dev)->PinCtrl ? BSP_PIN_CTRL_GetState((Dev)->PinCtrl, Pin, State) : 0)

static int32_t SI5342_ReadReg(SI5342_T *Dev, uint16_t RegAddr, uint8_t *RegData)
{
	uint8_t Addr, Data;
	
	Addr = 0x01;
	Data = RegAddr >> 8;
	if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Data, 1))
		return -1;
	Addr = RegAddr;
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Data, 1))
		return -1;
	*RegData = Data;
	return 0;
}

static int32_t SI5342_WriteReg(SI5342_T *Dev, uint16_t RegAddr, uint8_t RegData)
{
	uint8_t Addr, Data;
	
	Addr = 0x01;
	Data = RegAddr >> 8;
	if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Data, 1))
		return -1;
	Addr = RegAddr;
	Data = RegData;
	if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &Addr, 1, (char*)&Data, 1))
		return -1;
	return 0;
}


/* Configuration algorithm:
 * 1. Write preamble.
 *     For revision D:
 *         Write 0x0B24 = 0xC0
 *         Write 0x0B25 = 0x00 
 *         Write 0x0540 = 0x01 
 * 2. Wait 300 ms.
 * 3. Perform the registers configuration.
 * 4. Perform soft reset.
 *     Write 0x001C = 0x01
 * 5. Write preamble.
 *     For revision D:
 *         Write 0x0540 = 0x00
 *         Write 0x0B24 = 0xC3
 *         Write 0x0B25 = 0x02 
 */

static int32_t SI5342_Config(SI5342_T *Dev, const SI5342_CFG_T *Config, const uint32_t ConfigSize)
{
	uint32_t i;
	
	//todo: Add revision B support
	
	if (SI5342_WriteReg(Dev, 0x0B24, 0xC0))
		return -1;
	if (SI5342_WriteReg(Dev, 0x0B25, 0x00))
		return -1;
	if (SI5342_WriteReg(Dev, 0x0540, 0x01))
		return -1;
	
	SI5342_msDelay(300);
	
	for (i = 0; i < ConfigSize; i++)
	{
		if (SI5342_WriteReg(Dev, Config[i].Addr, Config[i].Data))
			return -1;
	}
	
	if (SI5342_WriteReg(Dev, 0x0514, 0x01))
		return -1;
	if (SI5342_WriteReg(Dev, 0x001C, 0x01))
		return -1;
	if (SI5342_WriteReg(Dev, 0x0540, 0x00))
		return -1;
	if (SI5342_WriteReg(Dev, 0x0B24, 0xC3))
		return -1;
	if (SI5342_WriteReg(Dev, 0x0B25, 0x02))
		return -1;
	
	return 0;
}

int32_t SI5342_Init(SI5342_T *Dev, I2C_TypeDef *I2C, uint8_t Addr, BSP_PIN_CTRL_T *PinCtrl, const SI5342_CFG_T *Config, const uint32_t ConfigSize)
{
	uint8_t Data[2];
	
	if ((!Dev) || (!I2C) || (Addr < 0x68) || (Addr > 0x6B))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->I2C = I2C;
	Dev->Addr = Addr << 1;
	Dev->PinCtrl = PinCtrl;
	
	if (SI5342_InitPin(Dev, SI5342_PIN_RESET))
		goto error;
	if (SI5342_InitPin(Dev, SI5342_PIN_I2C_SEL))
		goto error;
	if (SI5342_InitPin(Dev, SI5342_PIN_IN_0_LOS))
		goto error;
	if (SI5342_InitPin(Dev, SI5342_PIN_IN_1_LOS))
		goto error;
	if (SI5342_InitPin(Dev, SI5342_PIN_IN_2_LOS))
		goto error;
	if (SI5342_InitPin(Dev, SI5342_PIN_IN_3_LOS))
		goto error;
	if (SI5342_InitPin(Dev, SI5342_PIN_INTR))
		goto error;
	if (SI5342_InitPin(Dev, SI5342_PIN_XAXB_LOS))
		goto error;
	if (SI5342_InitPin(Dev, SI5342_PIN_LOL))
		goto error;
	
	SI5342_msDelay(100);
	
	if (SI5342_ReadReg(Dev, 0x0002, &Data[0]))
		goto error;
	if (SI5342_ReadReg(Dev, 0x0003, &Data[1]))
		goto error;
	Dev->PtNumber = (Data[0] << 8) | Data[1];
	
	if (Config)
	{
		if (SI5342_Config(Dev, Config, ConfigSize))
			goto error;
	}
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

void SI5342_Deinit(SI5342_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}

int32_t SI5342_Update(SI5342_T *Dev)
{
	uint8_t State;
	uint32_t PinState;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	State = 0;
	if (SI5342_GetPinState(Dev, SI5342_PIN_IN_0_LOS, &PinState))
		goto error;
	else
		State |= SI5342_STATE_IN_0_LOS;
	
	if (SI5342_GetPinState(Dev, SI5342_PIN_IN_1_LOS, &PinState))
		goto error;
	else
		State |= SI5342_STATE_IN_1_LOS;
	
	if (SI5342_GetPinState(Dev, SI5342_PIN_IN_2_LOS, &PinState))
		goto error;
	else
		State |= SI5342_STATE_IN_2_LOS;
	
	if (SI5342_GetPinState(Dev, SI5342_PIN_IN_2_LOS, &PinState))
		goto error;
	else
		State |= SI5342_STATE_IN_3_LOS;
	
	if (SI5342_GetPinState(Dev, SI5342_PIN_XAXB_LOS, &PinState))
		goto error;
	else
		State |= SI5342_STATE_XAXB_LOS;
	
	if (SI5342_GetPinState(Dev, SI5342_PIN_LOL, &PinState))
		goto error;
	else
		State |= SI5342_STATE_LOL;
	
	Dev->State = State;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}
