#include "sfp.h"
#include "log.h"
#include <string.h>
#include <math.h>

#define SFP_InitPin(Dev, Pin)							((Dev)->PinCtrl ? BSP_PIN_CTRL_Init((Dev)->PinCtrl, Pin) : 0)	
#define SFP_SetPinState(Dev, Pin, State)	((Dev)->PinCtrl ? BSP_PIN_CTRL_SetState((Dev)->PinCtrl, Pin, State) : 0)
#define SFP_GetPinState(Dev, Pin, State)	((Dev)->PinCtrl ? BSP_PIN_CTRL_GetState((Dev)->PinCtrl, Pin, State) : 0)

#define SFP_I2C(Dev)											((Dev)->I2CCtrl ? (Dev)->I2CCtrl->I2C : NULL)
#define SFP_I2C_ConfigSwitch(Dev)					((Dev)->I2CCtrl ? BSP_I2C_CTRL_ConfigSwitch((Dev)->I2CCtrl) : 0)
#define SFP_I2C_ResetSwitch(Dev)					((Dev)->I2CCtrl ? BSP_I2C_CTRL_ResetSwitch((Dev)->I2CCtrl) : 0)

static int32_t SFP_ReadProductOpts(SFP_T *Dev)
{
	uint8_t Addr;	
	
	Addr = 65;
	if (I2C_Sequential_Receive(SFP_I2C(Dev), 0xA0, &Addr, 1, (char*)&Dev->Info.ProductOpts, 1))
		return -1;	
	return 0;
}

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

static int32_t SFP_ReadRDT(SFP_T *Dev, uint32_t *Freq)
{
	uint8_t Addr;
	uint8_t Data;
	
	if (Dev->Info.ProductOpts & SFP_PRODUCT_OPTS_RDT_MASK)
	{
		Addr = 127;
		Data = 0x02;
		if (I2C_Sequential_Transmit(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&Data, 1))
			return -1;
		
		osDelay(1);
		
		Addr = 131;
		if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&Data, 2))
			return -1;
		Dev->RDT = ((Data + 128) & 0xFF) / 255.0f;
	}
	else
	{
		Dev->RDT = 0.0f;
	}
	return 0;
}

static int32_t SFP_UpdateRDT(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t Data;
	
	if (!(Dev->Info.ProductOpts & SFP_PRODUCT_OPTS_RDT_MASK))
		return 0;
	
	Addr = 127;
	Data = 0x02;
	if (I2C_Sequential_Transmit(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&Data, 1))
		return -1;
	
	osDelay(1);
	
	Addr = 131;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&Data, 2))
		return -1;
	
	if (Data == ((Dev->RDT * 255.0f) - 128))
		return 0;
	
	Data = ((Dev->RDT * 255.0f) - 128);
	Addr = 131;
	if (I2C_Sequential_Transmit(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&Data, 2))
		return -1;
	LOG_Add(Dev->DevName, Dev->DevId, "Write RDT: %.2f", Dev->RDT);
	
	return 0;
}

int32_t SFP_WriteRDT(SFP_T *Dev, float RDT)
{
	uint8_t Addr;
	uint8_t Data;
	
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (!(Dev->Info.ProductOpts & SFP_PRODUCT_OPTS_RDT_MASK))
		goto error;
	
	if ((RDT > 100.0f) && (RDT < 0.0f))
		goto error;
	
	Addr = 127;
	Data = 0x02;
	if (I2C_Sequential_Transmit(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&Data, 1))
		return -1;
	
	osDelay(1);
	
	Data = (RDT * 255.0f) - 128;
	Addr = 131;
	if (I2C_Sequential_Transmit(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)&Data, 2))
		return -1;
	Dev->RDT = RDT;
	
	LOG_Add(Dev->DevName, Dev->DevId, "Write RDT: %.2f", Dev->RDT);	
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t SFP_ConfigRDT(SFP_T *Dev, float RDT)
{	
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (!(Dev->Info.ProductOpts & SFP_PRODUCT_OPTS_RDT_MASK))
		goto error;
	
	if ((RDT > 100.0f) && (RDT < 0.0f))
		goto error;
	
	Dev->RDT = RDT;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}


static int32_t SFP_ReadFreq(SFP_T *Dev, uint32_t *Freq)
{
	uint8_t Addr;
	uint8_t Data[2];
	
	if (Dev->Info.ProductOpts & SFP_PRODUCT_OPTS_TX_TUNABLE_MASK)
	{
		Addr = 127;
		Data[0] = 0x02;
		if (I2C_Sequential_Transmit(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 1))
			return -1;
		
		osDelay(1);
		
		Addr = 132;
		if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 2))
			return -1;
		Dev->Freq.Min = ((Data[0] << 8) | Data[1]) * 1000000;
		Addr = 134;
		if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 2))
			return -1;
		Dev->Freq.Min += ((Data[0] << 8) | Data[1]) * 100;
		
		Addr = 136;
		if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 2))
			return -1;
		Dev->Freq.Max = ((Data[0] << 8) | Data[1]) * 1000000;
		Addr = 138;
		if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 2))
			return -1;
		Dev->Freq.Max += ((Data[0] << 8) | Data[1]) * 100;
		
		Addr = 140;
		if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 2))
			return -1;
		Dev->Freq.Spacing = ((Data[0] << 8) | Data[1]) * 100;
		
		Addr = 144;
		if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 2))
			return -1;
		Dev->Freq.Channel = (Data[0] << 8) | Data[1];
		
		Dev->Freq.Cur = ((Dev->Freq.Channel - 1) * Dev->Freq.Spacing) + Dev->Freq.Min;
	}
	else
	{
		Addr = 60;
		if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA0, &Addr, 1, (char*)Data, 2))
			return -1;
		
		Dev->Freq.Cur = SFP_SPEED_OF_LIGHT / ((Data[0] << 8) | Data[1]);
		Dev->Freq.Min = Dev->Freq.Cur;
		Dev->Freq.Max = Dev->Freq.Cur;
		Dev->Freq.Spacing = 100;
		Dev->Freq.Channel = 1;
	}
	return 0;
}

static int32_t SFP_UpdateFreq(SFP_T *Dev)
{
	uint8_t Addr;
	uint8_t Data[2];
	uint16_t Channel;
	
	if (!(Dev->Info.ProductOpts & SFP_PRODUCT_OPTS_TX_TUNABLE_MASK))
		return 0;
	
	Addr = 127;
	Data[0] = 0x02;
	if (I2C_Sequential_Transmit(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 1))
		return -1;
	
	osDelay(1);
	
	Addr = 144;
	if (I2C_Sequential_Receive(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 2))
		return -1;
	Channel = (Data[0] << 8) | Data[1];
	
	if (Channel == Dev->Freq.Channel)
		return 0;
	
	Data[0] = Dev->Freq.Channel >> 8;
	Data[1] = Dev->Freq.Channel;	
	Addr = 144;
	if (I2C_Sequential_Transmit(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 2))
		return -1;
	LOG_Add(Dev->DevName, Dev->DevId, "Write Freq: %d", Dev->Freq.Cur);
	
	return 0;
}

int32_t SFP_WriteFreq(SFP_T *Dev, uint32_t Freq)
{
	uint8_t Addr;
	uint8_t Data[2];
	uint16_t Channel;
	
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (!(Dev->Info.ProductOpts & SFP_PRODUCT_OPTS_TX_TUNABLE_MASK))
		goto error;
	
	if ((Freq < Dev->Freq.Min) || (Freq > Dev->Freq.Max) ||(Freq % Dev->Freq.Spacing))
		goto error;
	
	Addr = 127;
	Data[0] = 0x02;
	if (I2C_Sequential_Transmit(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 1))
		return -1;
	
	osDelay(1);
	
	Channel = 1 + (Freq -  Dev->Freq.Min) / Dev->Freq.Spacing;
	Data[0] = Channel >> 8;
	Data[1] = Channel;
	Addr = 144;
	if (I2C_Sequential_Transmit(SFP_I2C(Dev),0xA2, &Addr, 1, (char*)Data, 2))
		return -1;
	Dev->Freq.Channel = Channel;
	Dev->Freq.Cur = Freq;
	
	LOG_Add(Dev->DevName, Dev->DevId, "Write Freq: %d", Dev->Freq.Cur);	
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t SFP_ConfigFreq(SFP_T *Dev, uint32_t Freq)
{	
	if (!Dev)
		return -1;
	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	if (!(Dev->Info.ProductOpts & SFP_PRODUCT_OPTS_TX_TUNABLE_MASK))
		goto error;
	
	if ((Freq < Dev->Freq.Min) || (Freq > Dev->Freq.Max) ||(Freq % Dev->Freq.Spacing))
		goto error;
	
	Dev->Freq.Cur = Freq;
	Dev->Freq.Channel = 1 + (Freq -  Dev->Freq.Min) / Dev->Freq.Spacing;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
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

int32_t SFP_ConfigRateSel(SFP_T *Dev, uint32_t RateSel)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Dev->Pin.RateSel = RateSel;
	
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
	if (SFP_SetPinState(Dev, SFP_PIN_RATE_SEL, Dev->Pin.RateSel))
		goto error;
	if (SFP_GetPinState(Dev, SFP_PIN_TX_FAULT, &Dev->Pin.TxFault))
		goto error;
	if (SFP_GetPinState(Dev, SFP_PIN_RX_LOS, &Dev->Pin.RxLos))
		goto error;
	
	if (SFP_I2C_ConfigSwitch(Dev))
		goto error;
	
	if (SFP_UpdateRDT(Dev))
		goto error;	
	if (SFP_UpdateFreq(Dev))
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
	LOG_Add(Dev->DevName, Dev->DevId, "Deinit");
	BSP_DEV_DEINIT(Dev);
	BSP_DEV_UNLOCK(Dev);
	return 0;
}

int32_t SFP_Init(SFP_T *Dev, const char *DevId, const BSP_I2C_CTRL_T *I2CCtrl, const BSP_PIN_CTRL_T *PinCtrl)
{	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev, "SFP", DevId);
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
	
	if (SFP_InitPin(Dev, SFP_PIN_RATE_SEL))
		goto error;
	if (SFP_GetPinState(Dev, SFP_PIN_RATE_SEL, &Dev->Pin.RateSel))
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
	
	if (SFP_ReadProductOpts(Dev))
		goto error;
	if (SFP_ReadVendor(Dev))
		goto error;
	if (SFP_ReadPartNumber(Dev))
		goto error;
	if (SFP_ReadSerialNumber(Dev))
		goto error;
	if (SFP_ReadRDT(Dev, NULL))
		goto error;
	if (SFP_ReadFreq(Dev, NULL))
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
	LOG_Add(Dev->DevName, Dev->DevId, "Init: Ok");
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
	LOG_Add(Dev->DevName, Dev->DevId, "Deinit");
	BSP_DEV_DEINIT(Dev);
}
