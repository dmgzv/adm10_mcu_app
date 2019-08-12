#include "ksz8895.h"
#include "log.h"
#include <string.h>

#define KSZ8895_REG_PORT_STATUS_0(Idx)	(0x19 + (0x10 * (Idx)))
#define KSZ8895_REG_PORT_STATUS_1(Idx)	(0x1E + (0x10 * (Idx)))
#define KSZ8895_REG_INDIRECT_ACCESS_CONTROL_0	0x6E
#define KSZ8895_REG_INDIRECT_ACCESS_CONTROL_1	0x6F
#define KSZ8895_REG_INDIRECT_DATA_REGISTER(Idx)	(0x70 + (Idx))

#ifdef BSP_CONFIG_USE_FREERTOS
#define	KSZ8895_msDelay(ms) 		osDelay(ms)
#else
#define	KSZ8895_msDelay(ms) 		HAL_Delay(ms)
#endif

#define KSZ8895_InitPin(Dev, Pin)							((Dev)->PinCtrl ? BSP_PIN_CTRL_Init((Dev)->PinCtrl, Pin) : 0)	
#define KSZ8895_SetPinState(Dev, Pin, State)	((Dev)->PinCtrl ? BSP_PIN_CTRL_SetState((Dev)->PinCtrl, Pin, State) : 0)

static void __KSZ8895_SetCSState(void *Priv, uint32_t State)
{
	KSZ8895_T *Dev = Priv;
	KSZ8895_SetPinState(Dev, KSZ8895_PIN_SPI_CS, State);
}

static int32_t __KSZ8895_ReadReg(KSZ8895_T *Dev, uint8_t Addr, uint8_t *Data)
{
	int32_t result;
	uint8_t Buffer[3];
	
	Buffer[0] = 0x03;
	Buffer[1] = Addr;
	result = SPI_TransmitReceive(Dev->SPI, &Dev->CS, (char*)Buffer, (char*)Buffer, sizeof(Buffer));
	if (result)
		return -1;
	*Data = Buffer[2];
	return 0;
}

static int32_t __KSZ8895_WriteReg(KSZ8895_T *Dev, uint8_t Addr, uint8_t Data)
{
	int32_t result;
	uint8_t Buffer[3];
	
	Buffer[0] = 0x02;
	Buffer[1] = Addr;
	Buffer[2] = Data;
	result = SPI_Transmit(Dev->SPI, &Dev->CS, (char*)Buffer, sizeof(Buffer));
	if (result)
		return -1;
	return 0;
}

int32_t KSZ8895_Init(KSZ8895_T *Dev, SPI_TypeDef *SPI, BSP_PIN_CTRL_T *PinCtrl)
{
	if ((!Dev) || (!SPI) || (!PinCtrl))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->SPI = SPI;
	Dev->PinCtrl = PinCtrl;

	Dev->CS.SetState = &__KSZ8895_SetCSState;
	Dev->CS.Priv = Dev;
	
	if (KSZ8895_InitPin(Dev, KSZ8895_PIN_SPI_CS))
		goto error;
	if (KSZ8895_InitPin(Dev, KSZ8895_PIN_RESET))
		goto error;
	if (KSZ8895_InitPin(Dev, KSZ8895_PIN_PWR_DOWN))
		goto error;	
	
	KSZ8895_msDelay(10);
	
	if (KSZ8895_SetPinState(Dev, KSZ8895_PIN_SPI_CS, 1))
		goto error;
	if (KSZ8895_SetPinState(Dev, KSZ8895_PIN_RESET, 1))
		goto error;
	if (KSZ8895_SetPinState(Dev, KSZ8895_PIN_PWR_DOWN, 1))
		goto error;	
	
	KSZ8895_msDelay(10);
	
	if (__KSZ8895_ReadReg(Dev, 0x00, &Dev->FamilyId))
		goto error;
	if (Dev->FamilyId != 0x95)
		goto error;
	if (__KSZ8895_ReadReg(Dev, 0x01, &Dev->ChipId))
		goto error;
	if (__KSZ8895_WriteReg(Dev, 0x01, Dev->ChipId | (1 << 0)))
		goto error;
	if (__KSZ8895_ReadReg(Dev, 0x01, &Dev->ChipId))
		goto error;
	
	LOG_Add("KSZ8895 Init: Ok");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);	
	return 0;
	
error:
	LOG_Add("KSZ8895 Init: Fail");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

void KSZ8895_Deinit(KSZ8895_T *Dev)
{
	if (!Dev)
		return;
	LOG_Add("KSZ8895 Deinit");
	BSP_DEV_DEINIT(Dev);
}

int32_t KSZ8895_Update(KSZ8895_T *Dev)
{
	uint8_t Data;
	uint32_t i;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	for (i = 0; i < 4; i++)
	{
	
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_STATUS_0(i), &Data))
			goto error;	
		Dev->PortStatus[i][0] = Data;
		
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_STATUS_1(i), &Data))
			goto error;		
		Dev->PortStatus[i][1] = Data;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t KSZ8895_ReadReg(KSZ8895_T *Dev, uint8_t Addr, uint8_t *Data)
{
	if ((!Dev) || (!Data))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__KSZ8895_ReadReg(Dev, Addr, Data))
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t KSZ8895_WriteReg(KSZ8895_T *Dev, uint8_t Addr, uint8_t Data)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__KSZ8895_WriteReg(Dev, Addr, Data))
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t KSZ8895_UpdatePortStatus(KSZ8895_T *Dev, uint32_t Idx)
{
	uint8_t Data;
	
	if ((!Dev) || (Idx > 4))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_STATUS_0(Idx), &Data))
		goto error;	
	Dev->PortStatus[Idx][0] = Data;

	
	if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_STATUS_1(Idx), &Data))
		goto error;
	
	Dev->PortStatus[Idx][1] = Data;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t KSZ8895_ReadMACTable(KSZ8895_T *Dev, KSZ8895_MAC_TABLE_T *Table, uint32_t MaxSize, uint32_t *Size)
{
	uint8_t Data[9];
	uint16_t Idx, i, TableSize;
	
	if ((!Dev) || (!Table) || (!Size))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	BSP_DEV_LOCK(Dev);
	
	Idx = 0;
	TableSize = 0;
	
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_0, (0x01 << 4) | (0x02 << 2) | ((Idx >> 8) & 0x03)))
			goto error;
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_1, Idx & 0xFF))
		goto error;
	if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_INDIRECT_DATA_REGISTER(0), &Data[0]))
				goto error;
	if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_INDIRECT_DATA_REGISTER(1), &Data[1]))
				goto error;
	if (!(Data[0] & (1 << 7)))
	{
		TableSize = ((Data[1] >> 5) & 0x07) + ((Data[0] & 0x7F) << 4) + 1;
		if (TableSize >= MaxSize)
			TableSize = MaxSize;
		for (Idx = 0; Idx < TableSize; Idx++)
		{
			if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_0, (0x01 << 4) | (0x02 << 2) | ((Idx >> 8) & 0x03)))
				goto error;
			if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_1, Idx & 0xFF))
				goto error;
			for (i = 0; i < 9; i++)
			{
				if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_INDIRECT_DATA_REGISTER(i), &Data[i]))
					goto error;
			}
			Table[Idx].Port = Data[1] & 0x07;
			memcpy(Table[Idx].Mac, &Data[3], 6);
		}
	}
	*Size = TableSize;
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}
