#include "ucd90120a.h"
#include <string.h>
#include <math.h>

#define UCD90120A_CMD_PAGE 0x00

#define UCD90120A_CMD_VOUT_MODE 					0x20
#define UCD90120A_CMD_VOUT_COMMAND 				0x21
#define UCD90120A_CMD_VOUT_SCALE_MONITOR 	0x2A

#define UCD90120A_CMD_VOUT_OV_FAULT_LIMIT	0x40
#define UCD90120A_CMD_VOUT_OV_WARN_LIMIT	0x42
#define UCD90120A_CMD_VOUT_UV_WARN_LIMIT	0x43
#define UCD90120A_CMD_VOUT_UV_FAULT_LIMIT	0x44

#define UCD90120A_CMD_STATUS_WORD					0x79
#define UCD90120A_CMD_STATUS_VOUT					0x7A
#define UCD90120A_CMD_STATUS_IOUT					0x7B
#define UCD90120A_CMD_STATUS_CML					0x7E

#define UCD90120A_CMD_READ_VOUT		 				0x8B
#define UCD90120A_CMD_READ_IOUT		 				0x8C
#define UCD90120A_CMD_READ_TEMPERATURE_1	0x8D
#define UCD90120A_CMD_READ_TEMPERATURE_2	0x8E

#define UCD90120A_CMD_DEVICE_ID	0xFD

#define UCD90120A_CMD_MFR_ID 0x99
#define UCD90120A_CMD_MFR_MODEL 0x9A
#define UCD90120A_CMD_MFR_REVISION 0x9B
#define UCD90120A_CMD_MFR_LOCATION 0x9C
#define UCD90120A_CMD_MFR_DATE 0x9D
#define UCD90120A_CMD_MFR_SERIAL	0x9E

#define UCD90120A_CMD_MFR_STATUS	0xF3

#ifdef BSP_CONFIG_USE_FREERTOS
#define	UCD90120A_msDelay(ms) 		osDelay(ms)
#else
#define	UCD90120A_msDelay(ms) 		HAL_Delay(ms)
#endif

#define UCD90120A_InitPin(Dev, Pin)							((Dev)->PinCtrl ? BSP_PIN_CTRL_Init((Dev)->PinCtrl, Pin) : 0)	
#define UCD90120A_SetPinState(Dev, Pin, State)	((Dev)->PinCtrl ? BSP_PIN_CTRL_SetState((Dev)->PinCtrl, Pin, State) : 0)

static int32_t UCD90120A_ReadInfo(UCD90120A_T *Dev)
{
	uint8_t Cmd;
	char Data[PMBUS_BLOCK_MAX_SIZE];

	Cmd = UCD90120A_CMD_DEVICE_ID;
	if (PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Cmd, (uint8_t*)Data, sizeof(Data)))
		return -1;
	strncpy(Dev->Info.Id, Data, sizeof(Dev->Info.Id));
	Dev->Info.Id[sizeof(Dev->Info.Id) - 1] = 0;
	return 0;
}

static int32_t UCD90120A_ReadRailCfg(UCD90120A_T *Dev, uint8_t Num)
{
	int8_t Mode;
	uint8_t Cmd[2], Scale[2];
	
	if(PMBUS_WriteByte(Dev->I2C, Dev->Addr, UCD90120A_CMD_PAGE, &Num))
		return -1;
	if(PMBUS_ReadByte(Dev->I2C, Dev->Addr, UCD90120A_CMD_VOUT_MODE, (uint8_t*)&Mode))
		return -1;
	Dev->Cfg.Rail[Num].VoutExp = Mode & (1 << 4) ? Mode | 0xF0 : Mode;
	if(PMBUS_ReadWord(Dev->I2C, Dev->Addr, UCD90120A_CMD_VOUT_COMMAND, Cmd))
		return -1;
	Dev->Cfg.Rail[Num].VoutCmd =  (Cmd[0] + (Cmd[1] << 8)) * pow(2, Dev->Cfg.Rail[Num].VoutExp);;
	if(PMBUS_ReadWord(Dev->I2C, Dev->Addr, UCD90120A_CMD_VOUT_SCALE_MONITOR, Scale))
		return -1;
	Dev->Cfg.Rail[Num].VoutScale =  PMBUS_LinearToFloat(Scale[0] + (Scale[1] << 8));
	return 0;
}

static int32_t UCD90120A_ReadCfg(UCD90120A_T *Dev)
{
	uint32_t i;
	
	for (i = 0; i < UCD90120A_RAILS_CNT; i++)
	{
		if (UCD90120A_ReadRailCfg(Dev, i))
			return -1;
	}
	return 0;
}

static int32_t UCD90120A_ReadRailThr(UCD90120A_T *Dev, uint8_t Num)
{
	uint8_t Data[2];
	
	if(PMBUS_WriteByte(Dev->I2C, Dev->Addr, UCD90120A_CMD_PAGE, &Num))
		return -1;
	if(PMBUS_ReadWord(Dev->I2C, Dev->Addr, UCD90120A_CMD_VOUT_OV_FAULT_LIMIT, Data))
		return -1;
	Dev->Thr.Rail[Num].Vout[UCD90120A_THR_CMAX] = (Data[0] + (Data[1] << 8)) * pow(2, Dev->Cfg.Rail[Num].VoutExp);
	if(PMBUS_ReadWord(Dev->I2C, Dev->Addr, UCD90120A_CMD_VOUT_OV_WARN_LIMIT, Data))
		return -1;
	Dev->Thr.Rail[Num].Vout[UCD90120A_THR_WMAX] = (Data[0] + (Data[1] << 8)) * pow(2, Dev->Cfg.Rail[Num].VoutExp);
	if(PMBUS_ReadWord(Dev->I2C, Dev->Addr, UCD90120A_CMD_VOUT_UV_WARN_LIMIT, Data))
		return -1;
	Dev->Thr.Rail[Num].Vout[UCD90120A_THR_WMIN] = (Data[0] + (Data[1] << 8)) * pow(2, Dev->Cfg.Rail[Num].VoutExp);
	if(PMBUS_ReadWord(Dev->I2C, Dev->Addr, UCD90120A_CMD_VOUT_UV_FAULT_LIMIT, Data))
		return -1;
	Dev->Thr.Rail[Num].Vout[UCD90120A_THR_CMIN] = (Data[0] + (Data[1] << 8)) * pow(2, Dev->Cfg.Rail[Num].VoutExp);
	return 0;
}

static int32_t UCD90120A_ReadThr(UCD90120A_T *Dev)
{
	uint32_t i;
	
	for (i = 0; i < UCD90120A_RAILS_CNT; i++)
	{
		if (UCD90120A_ReadRailThr(Dev, i))
			return -1;
	}
	return 0;
}

static int32_t UCD90120A_UpdateRailMon(UCD90120A_T *Dev, uint8_t Num)
{
	uint8_t Data[4];
	
	if(PMBUS_WriteByte(Dev->I2C, Dev->Addr, UCD90120A_CMD_PAGE, &Num))
		return -1;
	
	if(PMBUS_ReadWord(Dev->I2C, Dev->Addr, UCD90120A_CMD_READ_VOUT, Data))
		return -1;
	Dev->Mon.Rail[Num].Vout = (Data[0] + (Data[1] << 8)) * pow(2, Dev->Cfg.Rail[Num].VoutExp);
	
	if(PMBUS_ReadWord(Dev->I2C, Dev->Addr, UCD90120A_CMD_READ_IOUT, Data))
		return -1;
	Dev->Mon.Rail[Num].Iout = PMBUS_LinearToFloat(Data[0] + (Data[1] << 8));
	
	if(PMBUS_ReadWord(Dev->I2C, Dev->Addr, UCD90120A_CMD_READ_TEMPERATURE_2, Data))
		return -1;
	Dev->Mon.Rail[Num].Temp = PMBUS_LinearToFloat(Data[0] + (Data[1] << 8));
	
	if(PMBUS_ReadByte(Dev->I2C, Dev->Addr, UCD90120A_CMD_STATUS_VOUT, Data))
		return -1;
	Dev->Mon.Rail[Num].StatusVout = Data[0];
	
	if(PMBUS_ReadByte(Dev->I2C, Dev->Addr, UCD90120A_CMD_STATUS_IOUT, Data))
		return -1;
	Dev->Mon.Rail[Num].StatusIout = Data[0];
	
	if(PMBUS_ReadBlock(Dev->I2C, Dev->Addr, UCD90120A_CMD_MFR_STATUS, Data, 4))
		return -1;
	Dev->Mon.Rail[Num].StatusMFR = Data[0] + (Data[1] << 8) + (Data[2] << 16) + (Data[3] << 24);
	
	return 0;
}

static int32_t UCD90120A_UpdateTemp(UCD90120A_T *Dev)
{
	uint8_t Data[2];
		
	if(PMBUS_ReadWord(Dev->I2C, Dev->Addr, UCD90120A_CMD_READ_TEMPERATURE_1, Data))
		return -1;
	Dev->Mon.Temp = PMBUS_LinearToFloat(Data[0] + (Data[1] << 8));
	
	return 0;
}

static int32_t UCD90120A_UpdateStatus(UCD90120A_T *Dev)
{
	uint8_t Data[2];
		
	if(PMBUS_ReadWord(Dev->I2C, Dev->Addr, UCD90120A_CMD_STATUS_WORD, Data))
		return -1;
	Dev->Mon.Status = Data[0] + (Data[1] << 8);
	
	return 0;
}

static int32_t UCD90120A_UpdateCML(UCD90120A_T *Dev)
{
	uint8_t Data;
		
	if(PMBUS_ReadByte(Dev->I2C, Dev->Addr, UCD90120A_CMD_STATUS_CML, &Data))
		return -1;
	Dev->Mon.CML = Data;
	
	return 0;
}

static int32_t UCD90120A_CheckConfig(UCD90120A_T *Dev, const UCD90120A_CFG_CMD_T *Config, const uint32_t ConfigSize)
{
	uint32_t i;
	uint8_t Data[PMBUS_BLOCK_MAX_SIZE];
	
	if (Config)
	{
		for (i = 0; i < ConfigSize; i++)
		{		
			if ((Config[i].Cmd == UCD90120A_CMD_MFR_ID) || (Config[i].Cmd == UCD90120A_CMD_MFR_MODEL) || (Config[i].Cmd == UCD90120A_CMD_MFR_REVISION) ||
				(Config[i].Cmd == UCD90120A_CMD_MFR_LOCATION) || (Config[i].Cmd == UCD90120A_CMD_MFR_DATE) || (Config[i].Cmd == UCD90120A_CMD_MFR_SERIAL))
			{
				if(PMBUS_ReadBlock(Dev->I2C, Dev->Addr, Config[i].Cmd, Data, Config[i].DataLen))
					return -1;
				if (memcmp(Data, Config[i].Data, Config[i].DataLen))
					return -1;
			}
		}
	}
	
	return 0;
}

static int32_t UCD90120A_WriteConfig(UCD90120A_T *Dev, const UCD90120A_CFG_CMD_T *Config, const uint32_t ConfigSize)
{
	uint32_t i;
	
	if (Config)
	{	
		for (i = 0; i < ConfigSize; i++)
		{
			if(PMBUS_ProcessCmd(Dev->I2C, Dev->Addr, Config[i].CmdType, Config[i].Cmd, Config[i].Data, Config[i].DataLen))
				return -1;
		}
		UCD90120A_msDelay(2000);
	}	
	if (UCD90120A_ReadCfg(Dev))
		return -1;
	if (UCD90120A_ReadThr(Dev))
		return -1;

	return 0;
}

int32_t UCD90120A_Init(UCD90120A_T *Dev, I2C_TypeDef *I2C, uint8_t Addr, BSP_PIN_CTRL_T *PinCtrl, const UCD90120A_CFG_CMD_T *Config, const uint32_t ConfigSize)
{	
	if ((!Dev) || (!I2C))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->Addr = Addr << 1;
	Dev->I2C = I2C;
	Dev->PinCtrl = (BSP_PIN_CTRL_T*)PinCtrl;
	
	if (UCD90120A_InitPin(Dev, UCD90120A_PIN_RESET))
		goto error;
	if (UCD90120A_SetPinState(Dev, UCD90120A_PIN_RESET, 1))
		goto error;
	
	if (UCD90120A_InitPin(Dev, UCD90120A_PIN_CONTROL))
		goto error;
	if (UCD90120A_SetPinState(Dev, UCD90120A_PIN_CONTROL, 0))
		goto error;
	
	if (UCD90120A_ReadInfo(Dev))
		goto error;
	if (UCD90120A_ReadCfg(Dev))
		goto error;
	if (UCD90120A_ReadThr(Dev))
		return -1;
	if (UCD90120A_CheckConfig(Dev, Config, ConfigSize))
	{
		if (UCD90120A_WriteConfig(Dev, Config, ConfigSize))
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

void UCD90120A_Deinit(UCD90120A_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}

int32_t UCD90120A_Update(UCD90120A_T *Dev)
{
	uint32_t i, PowerGood;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if( UCD90120A_UpdateStatus(Dev))
		goto error;
	
	if( UCD90120A_UpdateCML(Dev))
		goto error;
	
	if( UCD90120A_UpdateTemp(Dev))
		goto error;
	
	PowerGood = 0;
	for (i = 0; i < UCD90120A_RAILS_CNT; i++)
	{
		PowerGood |= 1 << i;
		if (Dev->Cfg.Rail[i].VoutScale == 0)
			continue;
		if (UCD90120A_UpdateRailMon(Dev, i))
			goto error;
		if (Dev->Mon.Rail[i].Vout > Dev->Thr.Rail[i].Vout[UCD90120A_THR_CMAX])
			PowerGood &= ~(1 << i);
		if (Dev->Mon.Rail[i].Vout < Dev->Thr.Rail[i].Vout[UCD90120A_THR_CMIN])
			PowerGood &= ~(1 << i);
	}
	Dev->PowerGood = PowerGood;

	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}
