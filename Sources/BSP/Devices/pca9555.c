#include "pca9555.h"
#include "log.h"

static const uint8_t pca9555_reg_ip[PCA9555_BANKS_CNT] = {0x00, 0x01};
static const uint8_t pca9555_reg_op[PCA9555_BANKS_CNT] = {0x02, 0x03};
static const uint8_t pca9555_reg_pl[PCA9555_BANKS_CNT] = {0x04, 0x05};
static const uint8_t pca9555_reg_ioc[PCA9555_BANKS_CNT] = {0x06, 0x07};


int32_t PCA9555_SetPinDir(PCA9555_T *Dev, uint32_t Bank, uint8_t Pin, uint8_t Dir)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9555_BANKS_CNT) || (Pin > 7) || (Dir > 1))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9555_reg_ioc[Bank];
	if (Dir)
		Dir = Dev->Dirs[Bank] | (1 << Pin);
	else
		Dir = Dev->Dirs[Bank] & ~(1 << Pin);
	
	if (Dir != Dev->Dirs[Bank])
	{
		LOG_Add("PCA9555 Write: 0x%02x 0x%02x", cmd, Dir);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Dir, sizeof(Dir)))
			goto error;
		Dev->Dirs[Bank] = Dir;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9555_SetPinOut(PCA9555_T *Dev, uint32_t Bank, uint8_t Pin, uint8_t Out)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9555_BANKS_CNT) || (Pin > 7) || (Out > 1))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9555_reg_op[Bank];
	if (Out)
		Out = Dev->Outs[Bank] | (1 << Pin);
	else
		Out = Dev->Outs[Bank] & ~(1 << Pin);
	if (Out != Dev->Outs[Bank])
	{
		LOG_Add("PCA9555 Write: 0x%02x 0x%02x", cmd, Out);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Out, sizeof(Out)))
			goto error;
		Dev->Outs[Bank] = Out;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9555_SetPinPol(PCA9555_T *Dev, uint32_t Bank, uint8_t Pin, uint8_t Pol)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9555_BANKS_CNT) || (Pin > 7) || (Pol > 1))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9555_reg_pl[Bank];
	if (Pol)
		Pol = Dev->Pols[Bank] | (1 << Pin);
	else
		Pol = Dev->Pols[Bank] & ~(1 << Pin);
	
	if (Pol != Dev->Pols[Bank])
	{
		LOG_Add("PCA9555 Write: 0x%02x 0x%02x", cmd, Pol);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Pol, sizeof(Pol)))
			goto error;
		Dev->Pols[Bank] = Pol;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9555_GetBankIn(PCA9555_T *Dev, uint32_t Bank)
{
	uint8_t cmd;
	uint8_t in;
	
	if ((!Dev) || (Bank >= PCA9555_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9555_reg_ip[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&in, sizeof(in)))
		goto error;	
	Dev->Ins[Bank] = in;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9555_GetBankDir(PCA9555_T *Dev, uint32_t Bank)
{
	uint8_t cmd;
	uint8_t dir;
	
	if ((!Dev) || (Bank >= PCA9555_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9555_reg_ioc[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&dir, sizeof(dir)))
		goto error;	
	Dev->Dirs[Bank] = dir;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9555_SetBankDir(PCA9555_T *Dev, uint32_t Bank, uint8_t Dir)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9555_BANKS_CNT))
		return -1;		
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9555_reg_ioc[Bank];
	if (Dir != Dev->Dirs[Bank])
	{
		LOG_Add("PCA9555 Write: 0x%02x 0x%02x", cmd, Dir);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Dir, sizeof(Dir)))
			goto error;
		Dev->Dirs[Bank] = Dir;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9555_GetBankOut(PCA9555_T *Dev, uint32_t Bank)
{
	uint8_t cmd;
	uint8_t out;
	
	if ((!Dev) || (Bank >= PCA9555_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9555_reg_op[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&out, sizeof(out)))
		goto error;	
	Dev->Outs[Bank] = out;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9555_SetBankOut(PCA9555_T *Dev, uint32_t Bank, uint8_t Out)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9555_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9555_reg_op[Bank];
	if (Out != Dev->Outs[Bank])
	{
		LOG_Add("PCA9555 Write: 0x%02x 0x%02x", cmd, Out);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Out, sizeof(Out)))
			goto error;
		Dev->Outs[Bank] = Out;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9555_GetBankPol(PCA9555_T *Dev, uint32_t Bank)
{
	uint8_t cmd;
	uint8_t pol;
	
	if ((!Dev) || (Bank >= PCA9555_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9555_reg_pl[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&pol, sizeof(pol)))
		goto error;	
	Dev->Pols[Bank] = pol;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9555_SetBankPol(PCA9555_T *Dev, uint32_t Bank, uint8_t Pol)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9555_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9555_reg_pl[Bank];
	if (Pol != Dev->Pols[Bank])
	{
		LOG_Add("PCA9555 Write: 0x%02x 0x%02x", cmd, Pol);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Pol, sizeof(Pol)))
			goto error;
		Dev->Pols[Bank] = Pol;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9555_Init(PCA9555_T *Dev, I2C_TypeDef *I2C, uint8_t Addr)
{
	uint8_t Ins[PCA9555_BANKS_CNT];
	uint8_t	Outs[PCA9555_BANKS_CNT];
	uint8_t Pols[PCA9555_BANKS_CNT];
	uint8_t Dirs[PCA9555_BANKS_CNT];
	uint32_t i;
	
	if ((!Dev) || (!I2C) || (Addr < 0x20) || (Addr > 0x27))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->Addr = Addr << 1;
	Dev->I2C = I2C;
	
	for (i = 0; i < PCA9555_BANKS_CNT; i++)
	{
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&pca9555_reg_ip[i], 1, (char*)&Ins[i], 1))
			goto error;		
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&pca9555_reg_op[i], 1, (char*)&Outs[i], 1))
			goto error;		
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&pca9555_reg_pl[i], 1, (char*)&Pols[i], 1))
			goto error;	
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&pca9555_reg_ioc[i], 1, (char*)&Dirs[i], 1))
			goto error;	
	}
	
	for (i = 0; i < PCA9555_BANKS_CNT; i++)
	{		
		Dev->Ins[i] = Ins[i];
		Dev->Outs[i] = Outs[i];
		Dev->Dirs[i] = Dirs[i];
		Dev->Pols[i] = Pols[i];
	}
	
	LOG_Add("PCA9555 Init: Ok");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	
	LOG_Add("PCA9555 Init: Fail");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

void PCA9555_Deinit(PCA9555_T *Dev)
{
	if (!Dev)
		return;
	LOG_Add("PCA9555 Deinit");
	BSP_DEV_DEINIT(Dev);
}

