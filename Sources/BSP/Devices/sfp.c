#include "sfp.h"
#include <string.h>
#include <math.h>

#define SFP_InitPin(Dev, Pin)							((Dev)->PinCtrl ? BSP_PIN_CTRL_Init((Dev)->PinCtrl, Pin) : 0)	
#define SFP_SetPinState(Dev, Pin, State)	((Dev)->PinCtrl ? BSP_PIN_CTRL_SetState((Dev)->PinCtrl, Pin, State) : 0)
#define SFP_GetPinState(Dev, Pin, State)	((Dev)->PinCtrl ? BSP_PIN_CTRL_GetState((Dev)->PinCtrl, Pin, State) : 0)

#define SFP_I2C(Dev)											((Dev)->I2CCtrl ? (Dev)->I2CCtrl->I2C : NULL)
#define SFP_I2C_ConfigSwitch(Dev)					((Dev)->I2CCtrl ? BSP_I2C_CTRL_ConfigSwitch((Dev)->I2CCtrl) : 0)
#define SFP_I2C_ResetSwitch(Dev)					((Dev)->I2CCtrl ? BSP_I2C_CTRL_ResetSwitch((Dev)->I2CCtrl) : 0)

static int32_t SFP_ReadVendor(SFP_T *Dev)
{
	uint8_t Addr;	
	
	Addr = 20;
	if (I2C_Sequential_Receive(SFP_I2C(Dev), 0xA0, &Addr, 1, Dev->Info.Vendor, 16))
		return -1;	
	Dev->Info.Vendor[16] = 0;
	return 0;
}

static int32_t SFP_ReadPartNumber(SFP_T *Dev)
{
	uint8_t Addr;	
	
	Addr = 40;
	if (I2C_Sequential_Receive(SFP_I2C(Dev), 0xA0, &Addr, 1, Dev->Info.PartNumber, 16))
		return -1;	
	Dev->Info.PartNumber[16] = 0;
	return 0;
}

static int32_t SFP_ReadSerialNumber(SFP_T *Dev)
{
	uint8_t Addr;	
	
	Addr = 68;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA0, &Addr, 1, Dev->Info.SerialNumber, 16))
		return -1;	
	Dev->Info.SerialNumber[16] = 0;
	return 0;
}

static int32_t SFP_ReadTempThrs(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[4][2];
	int16_t val;
	
	Addr = 00;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[0][0], 2))
		return -1;
	Addr = 02;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[1][0], 2))
		return -1;
	Addr = 04;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[2][0], 2))
		return -1;
	Addr = 06;	
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[3][0], 2))
		return -1;	
	
	val = (tmp[0][0] << 8) | tmp[0][1];
	Dev->Thr.Temp[SFP_THR_CMAX] = val / 256.0;
	val = (tmp[1][0] << 8) | tmp[1][1];
	Dev->Thr.Temp[SFP_THR_CMIN] = val / 256.0;
	val = (tmp[2][0] << 8) | tmp[2][1];
	Dev->Thr.Temp[SFP_THR_WMAX] = val / 256.0;
	val = (tmp[3][0] << 8) | tmp[3][1];
	Dev->Thr.Temp[SFP_THR_WMIN] = val / 256.0;
	
	return 0;
}

static int32_t SFP_ReadRxPwrThrs(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[4][2];
	uint16_t val;
	
	Addr = 32;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[0][0], 2))
		return -1;
	Addr = 34;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[1][0], 2))
		return -1;
	Addr = 36;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[2][0], 2))
		return -1;
	Addr = 38;	
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[3][0], 2))
		return -1;
	
	val = (tmp[0][0] << 8) | tmp[0][1];
	Dev->Thr.RxPwr[SFP_THR_CMAX] = 10 * log10(val * 0.0001);
	val = (tmp[1][0] << 8) | tmp[1][1];
	Dev->Thr.RxPwr[SFP_THR_CMIN] = 10 * log10(val * 0.0001);
	val = (tmp[2][0] << 8) | tmp[2][1];
	Dev->Thr.RxPwr[SFP_THR_WMAX] = 10 * log10(val * 0.0001);
	val = (tmp[3][0] << 8) | tmp[3][1];
	Dev->Thr.RxPwr[SFP_THR_WMIN] = 10 * log10(val * 0.0001);
	return 0;
}

static int32_t SFP_ReadTxPwrThrs(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[4][2];
	uint16_t val;
	
	Addr = 24;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[0][0], 2))
		return -1;
	Addr = 26;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[1][0], 2))
		return -1;
	Addr = 28;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[2][0], 2))
		return -1;
	Addr = 30;	
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&tmp[3][0], 2))
		return -1;	
	
	val = (tmp[0][0] << 8) | tmp[0][1];
	Dev->Thr.TxPwr[SFP_THR_CMAX] = 10 * log10(val * 0.0001);
	val = (tmp[1][0] << 8) | tmp[1][1];
	Dev->Thr.TxPwr[SFP_THR_CMIN] = 10 * log10(val * 0.0001);
	val = (tmp[2][0] << 8) | tmp[2][1];
	Dev->Thr.TxPwr[SFP_THR_WMAX] = 10 * log10(val * 0.0001);
	val = (tmp[3][0] << 8) | tmp[3][1];
	Dev->Thr.TxPwr[SFP_THR_WMIN] = 10 * log10(val * 0.0001);
	return 0;
}

static int32_t SFP_ReadTemp(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[2];
	int16_t val;
	
	Addr = 96;
	if (I2C_Sequential_Receive(SFP_I2C(Dev), 0xA2, &Addr, 1, (char*)&tmp[0], 2))
		return -1;
	
	val = (tmp[0] << 8) | tmp[1];
	Dev->Temp = val / 256.0;
	return 0;
}

static int32_t SFP_ReadRxPwr(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[2];
	uint16_t val;
	
	Addr = 104;
	if (I2C_Sequential_Receive(SFP_I2C(Dev), 0xA2, &Addr, 1, (char*)&tmp[0], 2))
		return -1;
	
	val = (tmp[0] << 8) | tmp[1];
	if (val == 0)
		Dev->RxPwr = -40;
	else
		Dev->RxPwr = 10 * log10(val * 0.0001);
	return 0;
}

static int32_t SFP_ReadTxPwr(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t tmp[2];
	uint16_t val;
	
	Addr = 102;
	if (I2C_Sequential_Receive(SFP_I2C(Dev), 0xA2, &Addr, 1, (char*)&tmp[0], 2))
		return -1;

	val = (tmp[0] << 8) | tmp[1];
	if (val == 0)
		Dev->TxPwr = -40;
	else
		Dev->TxPwr = 10 * log10(val * 0.0001);
	return 0;
}

int32_t SFP_GetModAbs(SFP_T *Dev, uint32_t *ModAbs)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	*ModAbs = Dev->Pin.ModAbs;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_GetRxLos(SFP_T *Dev, uint32_t *RxLos)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	*RxLos = Dev->Pin.RxLos;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_GetTxDisable(SFP_T *Dev, uint32_t *TxDisable)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	*TxDisable = Dev->Pin.TxDisable;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_GetTxFault(SFP_T *Dev, uint32_t *TxFault)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	*TxFault = Dev->Pin.TxFault;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_GetInfo(SFP_T *Dev, SFP_INFO_T *Info)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	memcpy(Info, &Dev->Info, sizeof(Dev->Info));
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_GetThr(SFP_T *Dev, SFP_THR_T *Thr)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	memcpy(Thr, &Dev->Thr, sizeof(Dev->Thr));
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_GetRxPwr(SFP_T *Dev, float *RxPwr)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	memcpy(RxPwr, &Dev->RxPwr, sizeof(Dev->RxPwr));
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_GetTxPwr(SFP_T *Dev, float *TxPwr)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	memcpy(TxPwr, &Dev->TxPwr, sizeof(Dev->TxPwr));
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_GetTemp(SFP_T *Dev, float *Temp)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	memcpy(Temp, &Dev->Temp, sizeof(Dev->Temp));
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_SetTxDisable(SFP_T *Dev, uint32_t TxDisable)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (SFP_SetPinState(Dev, SFP_PIN_TX_DISABLE, TxDisable))
		goto error;
	Dev->Pin.TxDisable = TxDisable;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t SFP_ConfigTxDisable(SFP_T *Dev, uint32_t TxDisable)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Dev->Pin.TxDisable = TxDisable;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_Update(SFP_T *Dev)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (SFP_GetPinState(Dev, SFP_PIN_MOD_ABS, &Dev->Pin.ModAbs))
		goto error;
	if (Dev->Pin.ModAbs)
		goto reset;
	
	if (SFP_SetPinState(Dev, SFP_PIN_TX_DISABLE, Dev->Pin.TxDisable))
		goto error;
	if (SFP_GetPinState(Dev, SFP_PIN_TX_FAULT, &Dev->Pin.TxFault))
		goto error;
	if (SFP_GetPinState(Dev, SFP_PIN_RX_LOS, &Dev->Pin.RxLos))
		goto error;
	
	if (SFP_I2C_ConfigSwitch(Dev))
		goto error;
	
	if (SFP_ReadTemp(Dev))
		goto error;
	if (SFP_ReadRxPwr(Dev))
		goto error;
	if (SFP_ReadTxPwr(Dev))
		goto error;
	
	SFP_I2C_ResetSwitch(Dev);	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	SFP_I2C_ResetSwitch(Dev);	
	BSP_DEV_UNLOCK(Dev);
	return -1;
	
reset:
	BSP_DEV_DEINIT(Dev);
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_Init(SFP_T *Dev, BSP_I2C_CTRL_T *I2CCtrl, BSP_PIN_CTRL_T *PinCtrl)
{	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->I2CCtrl = I2CCtrl;
	Dev->PinCtrl = PinCtrl;
	
	if (SFP_InitPin(Dev, SFP_PIN_MOD_ABS))
		goto error;
	if (SFP_GetPinState(Dev, SFP_PIN_MOD_ABS, &Dev->Pin.ModAbs))
		goto error;
	if (Dev->Pin.ModAbs)
		goto reset;
	
	if (SFP_InitPin(Dev, SFP_PIN_TX_DISABLE))
		goto error;
	if (SFP_GetPinState(Dev, SFP_PIN_TX_DISABLE, &Dev->Pin.TxDisable))
		goto error;
	
	if (SFP_InitPin(Dev, SFP_PIN_TX_FAULT))
		goto error;
	if (SFP_GetPinState(Dev, SFP_PIN_TX_FAULT, &Dev->Pin.TxFault))
		goto error;
	
	if (SFP_InitPin(Dev, SFP_PIN_RX_LOS))
		goto error;
	if (SFP_GetPinState(Dev, SFP_PIN_RX_LOS, &Dev->Pin.RxLos))
		goto error;	
	
	if (SFP_I2C_ConfigSwitch(Dev))
		goto error;
	
	if (SFP_ReadVendor(Dev))
		goto error;
	if (SFP_ReadPartNumber(Dev))
		goto error;
	if (SFP_ReadSerialNumber(Dev))
		goto error;
	if (SFP_ReadTempThrs(Dev))
		goto error;
	if (SFP_ReadRxPwrThrs(Dev))
		goto error;
	if (SFP_ReadTxPwrThrs(Dev))
		goto error;
	if (SFP_ReadTemp(Dev))
		goto error;
	if (SFP_ReadRxPwr(Dev))
		goto error;
	if (SFP_ReadTxPwr(Dev))
		goto error;
	
	SFP_I2C_ResetSwitch(Dev);	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	SFP_I2C_ResetSwitch(Dev);	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;

reset:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
	BSP_DEV_UNLOCK(Dev);
	return 0;	
}

void SFP_Deinit(SFP_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
