#include "ksz8895.h"
#include "log.h"
#include <string.h>

#define KSZ8895_REG_GLOBAL_CONTROL(Idx)	(0x02 + (Idx))

#define KSZ8895_REG_PORT_CONTROL_0(Idx)	(0x10 + (0x10 * (Idx)))
#define KSZ8895_REG_PORT_CONTROL_1(Idx)	(0x11 + (0x10 * (Idx)))
#define KSZ8895_REG_PORT_CONTROL_2(Idx)	(0x12 + (0x10 * (Idx)))
#define KSZ8895_REG_PORT_CONTROL_3(Idx)	(0x13 + (0x10 * (Idx)))
#define KSZ8895_REG_PORT_CONTROL_4(Idx)	(0x14 + (0x10 * (Idx)))
#define KSZ8895_REG_PORT_CONTROL_5(Idx)	(0x1C + (0x10 * (Idx)))
#define KSZ8895_REG_PORT_STATUS_0(Idx)	(0x19 + (0x10 * (Idx)))
#define KSZ8895_REG_PORT_STATUS_1(Idx)	(0x1E + (0x10 * (Idx)))
#define KSZ8895_REG_INDIRECT_ACCESS_CONTROL_0	0x6E
#define KSZ8895_REG_INDIRECT_ACCESS_CONTROL_1	0x6F
#define KSZ8895_REG_INDIRECT_DATA_REGISTER(Idx)	(0x78 - (Idx))

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
	result = SPI_DMA_TransmitReceive(Dev->SPI, &Dev->CS, (char*)Buffer, (char*)Buffer, sizeof(Buffer));
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
	result = SPI_DMA_Transmit(Dev->SPI, &Dev->CS, (char*)Buffer, sizeof(Buffer));
	if (result)
		return -1;
	return 0;
}

static int32_t __KSZ8895_ReadVLANTable(KSZ8895_T *Dev, uint16_t VID, uint16_t *VIDData)
{
	uint8_t Data[7];
	uint16_t i;
	uint32_t __Data;
	uint32_t BitFirst, BitsCnt, ByteFirst, ByteLast, ByteFirstShift;
	
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_0, (0x01 << 4) | (0x01 << 2) | (((VID >> 2) >> 8) & 0x03)))
			goto error;
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_1, (VID >> 2) & 0xFF))
		goto error;
	for (i = 0; i < sizeof(Data); i++)
	{
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_INDIRECT_DATA_REGISTER(i), &Data[i]))
			goto error;
	}
	
	BitsCnt = 13;
	if ((VID & 0x03) == 0)
		BitFirst = 0;
	else if ((VID & 0x03) == 1)
		BitFirst = 13;
	else if ((VID & 0x03) == 2)
		BitFirst = 26;
	else
		BitFirst = 39;
	
	ByteFirst = BitFirst / 8;
	ByteLast = (BitFirst + BitsCnt - 1)  / 8;
	ByteFirstShift = BitFirst % 8;
	
	__Data = 0;
	for (i = ByteFirst; i <= ByteLast; i++)
		__Data |= Data[i] << (8 * (i - ByteFirst));
	
	*VIDData = (__Data >> ByteFirstShift) & ((1 << BitsCnt) - 1);
	
	return 0;
	
error:
	return -1;
}

static int32_t __KSZ8895_WriteVLANTable(KSZ8895_T *Dev, uint16_t VID, uint16_t VIDData)
{
	uint8_t Data[7];
	uint16_t i;
	uint32_t __Data;
	uint32_t BitFirst, BitsCnt, ByteFirst, ByteLast, ByteFirstShift;
	
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_0, (0x01 << 4) | (0x01 << 2) | (((VID >> 2) >> 8) & 0x03)))
			goto error;
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_1, (VID >> 2) & 0xFF))
		goto error;
	for (i = 0; i < sizeof(Data); i++)
	{
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_INDIRECT_DATA_REGISTER(i), &Data[i]))
			goto error;
	}
	
	BitsCnt = 13;
	if ((VID & 0x03) == 0)
		BitFirst = 0;
	else if ((VID & 0x03) == 1)
		BitFirst = 13;
	else if ((VID & 0x03) == 2)
		BitFirst = 26;
	else
		BitFirst = 39;
	
	ByteFirst = BitFirst / 8;
	ByteLast = (BitFirst + BitsCnt - 1)  / 8;
	ByteFirstShift = BitFirst % 8;
	
	__Data = 0;
	for (i = ByteFirst; i <= ByteLast; i++)
		__Data |= Data[i] << (8 * (i - ByteFirst));
	
	__Data &= ~(((1 << BitsCnt) - 1) << ByteFirstShift);
	__Data |= ((VIDData & ((1 << BitsCnt) - 1)) << ByteFirstShift);
	for (i = ByteFirst; i <= ByteLast; i++)
		Data[i] = (__Data >> (8 * (i - ByteFirst))) & 0xFF;
	
	for (i = 0; i < sizeof(Data); i++)
	{
		if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_DATA_REGISTER(i), Data[i]))
			goto error;
	}
	
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_0, (0x00 << 4) | (0x01 << 2) | (((VID >> 2) >> 8) & 0x03)))
			goto error;
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_1, (VID >> 2) & 0xFF))
		goto error;
	return 0;
	
error:
	return -1;
}

static int32_t __KSZ8895_EnableVLAN(KSZ8895_T *Dev, uint32_t Enable)
{
	uint8_t Data;
	
	if (Enable)
		Data = Dev->GlobalControl[3] | (1 << 7);
	else
		Data = Dev->GlobalControl[3] & (1 << 7);
	if (Data != Dev->GlobalControl[3])
	{
		if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_GLOBAL_CONTROL(3), Data))
			goto error;
		Dev->GlobalControl[3] = Data;
	}
	return 0;
	
error:
	return -1;
}

int32_t KSZ8895_Init(KSZ8895_T *Dev)
{
	uint32_t i, j;
	uint8_t Data;

	if ((!Dev) || (!Dev->SPI) || (!Dev->PinCtrl))
		return -1;
	
	BSP_DEV_INIT(Dev);

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
	
	for (i = 0; i < KSZ8895_PORTS_CNT; i++)
	{
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_0(i), &Data))
			goto error;
		Dev->PortControl[i][0] = Data;
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_1(i), &Data))
			goto error;
		Dev->PortControl[i][1] = Data;
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_2(i), &Data))
			goto error;
		Dev->PortControl[i][2] = Data;
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_3(i), &Data))
			goto error;
		Dev->PortControl[i][3] = Data;
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_4(i), &Data))
			goto error;
		Dev->PortControl[i][4] = Data;
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_5(i), &Data))
			goto error;
		Dev->PortControl[i][5] = Data;
		
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_STATUS_0(i), &Data))
			goto error;
		Dev->PortStatus[i][0] = Data;
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_STATUS_1(i), &Data))
			goto error;
		Dev->PortStatus[i][1] = Data;
	}
	
	if ((Dev->VLANTable) && (Dev->VLANTableSize))
	{
		for (i = 0; i < 4096; i++)
		{
			for (j = 0; j < Dev->VLANTableSize; j++)
			{
				if (i == Dev->VLANTable[j].VID)
				{
					if (__KSZ8895_WriteVLANTable(Dev, Dev->VLANTable[j].VID, Dev->VLANTable[j].Data))
						goto error;
					break;
				}
			}
			if (j == Dev->VLANTableSize)
			{
				if (__KSZ8895_WriteVLANTable(Dev, i, KSZ8895_VLAN_INVALID))
					goto error;
			}
		}
		if (__KSZ8895_EnableVLAN(Dev, 1))
			goto error;
	}
	else
	{
		if (__KSZ8895_EnableVLAN(Dev, 0))
			goto error;
	}
	
	
	BSP_DEV_INIT_OK(Dev);	
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void KSZ8895_Deinit(KSZ8895_T *Dev)
{
	if (!Dev)
		return;
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
	
	BSP_DEV_UPDATE(Dev);
	
	for (i = 0; i < KSZ8895_GLOBAL_CONTROL_CNT; i++)
	{
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_GLOBAL_CONTROL(i), &Data))
			goto error;
		Dev->GlobalControl[i] = Data;
	}		
	
	for (i = 0; i < KSZ8895_PORTS_CNT; i++)
	{
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_0(i), &Data))
			goto error;	
		if (Data != Dev->PortControl[i][0])
		{
			if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_PORT_CONTROL_0(i), Dev->PortControl[i][0]))
				goto error;
		}
		
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_1(i), &Data))
			goto error;	
		if (Data != Dev->PortControl[i][1])
		{
			if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_PORT_CONTROL_1(i), Dev->PortControl[i][1]))
				goto error;
		}
		
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_2(i), &Data))
			goto error;	
		if (Data != Dev->PortControl[i][2])
		{
			if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_PORT_CONTROL_2(i), Dev->PortControl[i][2]))
				goto error;
		}
		
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_3(i), &Data))
			goto error;	
		if (Data != Dev->PortControl[i][3])
		{
			if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_PORT_CONTROL_3(i), Dev->PortControl[i][3]))
				goto error;
		}
		
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_4(i), &Data))
			goto error;	
		if (Data != Dev->PortControl[i][4])
		{
			if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_PORT_CONTROL_4(i), Dev->PortControl[i][4]))
				goto error;
		}
		
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_CONTROL_5(i), &Data))
			goto error;	
		if (Data != Dev->PortControl[i][5])
		{
			if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_PORT_CONTROL_5(i), Dev->PortControl[i][5]))
				goto error;
		}
		
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_STATUS_0(i), &Data))
			goto error;	
		Dev->PortStatus[i][0] = Data;
		
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_STATUS_1(i), &Data))
			goto error;		
		Dev->PortStatus[i][1] = Data;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t KSZ8895_ConfigPortMode(KSZ8895_T *Dev, uint32_t Idx, uint32_t Mode)
{
	if ((!Dev) || (Idx > 4))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	switch (Mode)
	{
		case KSZ8895_PORT_MODE_AUTONEG:
			Dev->PortControl[Idx][5] = 0xFF & (~(1 << 7));
			Dev->PortControl[Idx][2] |= (1 << 1) | (1 << 2);
			break;
		case KSZ8895_PORT_MODE_100M_FULL_DUPLEX:
			Dev->PortControl[Idx][5] = 0xFF;
			Dev->PortControl[Idx][2] |= (1 << 1) | (1 << 2);
			break;
		case KSZ8895_PORT_MODE_100M_HALF_DUPLEX:
			Dev->PortControl[Idx][5] = 0xFF & (~(1 << 5));
			Dev->PortControl[Idx][2] |= (1 << 1) | (1 << 2);
			break;
		case KSZ8895_PORT_MODE_10M_FULL_DUPLEX:
			Dev->PortControl[Idx][5] = 0xFF & (~(1 << 6));
			Dev->PortControl[Idx][2] |= (1 << 1) | (1 << 2);
			break;
		case KSZ8895_PORT_MODE_10M_HALF_DUPLEX:
			Dev->PortControl[Idx][5] = 0xFF & (~(1 << 6)) & (~(1 << 5));
			Dev->PortControl[Idx][2] |= (1 << 1) | (1 << 2);
			break;
		case KSZ8895_PORT_MODE_DISABLED:
			Dev->PortControl[Idx][5] = 0xFF & (~(1 << 7));
			Dev->PortControl[Idx][2] &= ~((1 << 1) | (1 << 2));
			break;
		default:
			goto error;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t KSZ8895_ConfigPortVLAN(KSZ8895_T *Dev, uint32_t Idx, uint16_t PVID, uint32_t Flags)
{
	if ((!Dev) || (Idx > 4))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (Flags & KSZ8895_PORT_VLAN_TAG_INSERTION)
		Dev->PortControl[Idx][0] |= (1 << 2);
	else
		Dev->PortControl[Idx][0] &= ~(1 << 2);
	
	if (Flags & KSZ8895_PORT_VLAN_TAG_REMOVAL)
		Dev->PortControl[Idx][0] |= (1 << 1);
	else
		Dev->PortControl[Idx][0] &= ~(1 << 1);
	
	Dev->PortControl[Idx][3] = (PVID >> 8) & 0x0F;
	Dev->PortControl[Idx][4] = PVID & 0xFF;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t KSZ8895_ReadVLANTable(KSZ8895_T *Dev, uint16_t VID, uint16_t *VIDData)
{
	
	if ((!Dev) || (VID > 4095) || (!VIDData))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__KSZ8895_ReadVLANTable(Dev, VID, VIDData))
			goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t KSZ8895_ReadReg(KSZ8895_T *Dev, uint8_t Addr, uint8_t *Data)
{
	if ((!Dev) || (!Data))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__KSZ8895_ReadReg(Dev, Addr, Data))
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t KSZ8895_WriteReg(KSZ8895_T *Dev, uint8_t Addr, uint8_t Data)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__KSZ8895_WriteReg(Dev, Addr, Data))
		goto error;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t KSZ8895_UpdatePortStatus(KSZ8895_T *Dev, uint32_t Idx)
{
	uint8_t Data;
	
	if ((!Dev) || (Idx > 4))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_STATUS_0(Idx), &Data))
		goto error;	
	Dev->PortStatus[Idx][0] = Data;

	
	if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_PORT_STATUS_1(Idx), &Data))
		goto error;
	
	Dev->PortStatus[Idx][1] = Data;
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
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
	
	BSP_DEV_UPDATE(Dev);
	
	Idx = 0;
	TableSize = 0;
	
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_0, (0x01 << 4) | (0x02 << 2) | ((Idx >> 8) & 0x03)))
			goto error;
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_1, Idx & 0xFF))
		goto error;
	if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_INDIRECT_DATA_REGISTER(7), &Data[0]))
				goto error;
	if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_INDIRECT_DATA_REGISTER(8), &Data[1]))
				goto error;
	if (!(Data[1] & (1 << 7)))
	{
		TableSize = ((Data[1] & 0x7F) << 4) + ((Data[0] >> 5) & 0x07) + 1;
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
			Table[Idx].Port = Data[7] & 0x07;
			Table[Idx].FID = Data[6] & 0x7F;
			memcpy(Table[Idx].Mac, &Data[0], 6);
		}
	}
	*Size = TableSize;
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t KSZ8895_ReadPortStat(KSZ8895_T *Dev, uint32_t Idx, uint32_t *Stat)
{
	uint8_t Data[4];
	uint16_t i, j;
	uint16_t Addr;
	
	if ((!Dev) || (!Stat) || (Idx >= KSZ8895_PORTS_CNT))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	for (i = 0; i < 0x20; i++)
	{
		Addr = (Idx * 0x20) + i;
		
		if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_0, (0x01 << 4) | (0x03 << 2) | ((Addr >> 8) & 0x03)))
			goto error;
		if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_1, Addr & 0xFF))
			goto error;
		for (j = 0; j < 4; j++)
		{
			if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_INDIRECT_DATA_REGISTER(j), &Data[j]))
				goto error;
		}
		Stat[i] += (((Data[3] << 24) | (Data[2] << 16) | (Data[1] << 8) | (Data[0]))) & ((1 << 30) - 1);
	}
	
	Addr = 0x100 + Idx;
	
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_0, (0x01 << 4) | (0x03 << 2) | ((Addr >> 8) & 0x03)))
		goto error;
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_1, Addr & 0xFF))
		goto error;
	for (j = 0; j < 2; j++)
	{
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_INDIRECT_DATA_REGISTER(j), &Data[j]))
			goto error;
	}
	Stat[0x20] = (Data[1] << 8) | (Data[0]);
	
	
	Addr = 0x105 + Idx;
	
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_0, (0x01 << 4) | (0x03 << 2) | ((Addr >> 8) & 0x03)))
		goto error;
	if (__KSZ8895_WriteReg(Dev, KSZ8895_REG_INDIRECT_ACCESS_CONTROL_1, Addr & 0xFF))
		goto error;
	for (j = 0; j < 2; j++)
	{
		if (__KSZ8895_ReadReg(Dev, KSZ8895_REG_INDIRECT_DATA_REGISTER(j), &Data[j]))
			goto error;
	}
	Stat[0x21] = (Data[1] << 8) | (Data[0]);
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}
