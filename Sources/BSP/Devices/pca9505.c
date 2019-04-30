#include "pca9505.h"

#define PCA9505_CMD_AI (1 << 7)

const uint8_t pca9505_reg_ip[PCA9505_BANKS_CNT] = {0x00, 0x01, 0x02, 0x03, 0x04};
const uint8_t pca9505_reg_op[PCA9505_BANKS_CNT] = {0x08, 0x09, 0x0A, 0x0B, 0x0C};
const uint8_t pca9505_reg_pl[PCA9505_BANKS_CNT] = {0x10, 0x11, 0x12, 0x13, 0x14};
const uint8_t pca9505_reg_ioc[PCA9505_BANKS_CNT] = {0x18, 0x19, 0x1A, 0x1B, 0x1C};
const uint8_t pca9505_reg_msk[PCA9505_BANKS_CNT] = {0x20, 0x21, 0x22, 0x23, 0x24};

int32_t PCA9505_SetPinDir(PCA9505_T *Dev, uint32_t Bank, uint8_t Pin, uint8_t Dir)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT) || (Pin > 7) || (Dir > 1))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_ioc[Bank];
	if (Dir)
		Dir = Dev->Dirs[Bank] | (1 << Pin);
	else
		Dir = Dev->Dirs[Bank] & ~(1 << Pin);
	if (Dev->Dirs[Bank] != Dir)
	{
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

int32_t PCA9505_SetPinOut(PCA9505_T *Dev, uint32_t Bank, uint8_t Pin, uint8_t Out)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT) || (Pin > 7) || (Out > 1))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_op[Bank];
	if (Out)
		Out = Dev->Outs[Bank] | (1 << Pin);
	else
		Out = Dev->Outs[Bank] & ~(1 << Pin);
	if (Dev->Outs[Bank] != Out)
	{
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

int32_t PCA9505_SetPinPol(PCA9505_T *Dev, uint32_t Bank, uint8_t Pin, uint8_t Pol)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT) || (Pin > 7) || (Pol > 1))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_op[Bank];
	if (Pol)
		Pol = Dev->Pols[Bank] | (1 << Pin);
	else
		Pol = Dev->Pols[Bank] & ~(1 << Pin);
	if (Dev->Pols[Bank] != Pol)
	{
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


int32_t PCA9505_SetPinMsk(PCA9505_T *Dev, uint32_t Bank, uint8_t Pin, uint8_t Msk)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT) || (Pin > 7) || (Msk > 1))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_op[Bank];
	if (Msk)
		Msk = Dev->Msks[Bank] | (1 << Pin);
	else
		Msk = Dev->Msks[Bank] & ~(1 << Pin);
	if (Dev->Msks[Bank] != Msk)
	{
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Msk, sizeof(Msk)))
			goto error;	
		Dev->Msks[Bank] = Msk;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9505_GetBankIn(PCA9505_T *Dev, uint32_t Bank)
{
	uint8_t cmd;
	uint8_t in;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_ip[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&in, sizeof(in)))
		goto error;	
	Dev->Ins[Bank] = in;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9505_GetBankDir(PCA9505_T *Dev, uint32_t Bank)
{
	uint8_t cmd;
	uint8_t dir;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_ioc[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&dir, sizeof(dir)))
		goto error;	
	Dev->Dirs[Bank] = dir;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9505_SetBankDir(PCA9505_T *Dev, uint32_t Bank, uint8_t Dir)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_ioc[Bank];
	if (Dev->Dirs[Bank] != Dir)
	{
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

int32_t PCA9505_GetBankOut(PCA9505_T *Dev, uint32_t Bank)
{
	uint8_t cmd;
	uint8_t out;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_op[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&out, sizeof(out)))
		goto error;	
	Dev->Outs[Bank] = out;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9505_SetBankOut(PCA9505_T *Dev, uint32_t Bank, uint8_t Out)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_op[Bank];
	if (Dev->Outs[Bank] != Out)
	{
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

int32_t PCA9505_GetBankPol(PCA9505_T *Dev, uint32_t Bank)
{
	uint8_t cmd;
	uint8_t pol;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_pl[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&pol, sizeof(pol)))
		goto error;	
	Dev->Pols[Bank] = pol;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9505_SetBankPol(PCA9505_T *Dev, uint32_t Bank, uint8_t Pol)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_pl[Bank];
	if (Dev->Pols[Bank] != Pol)
	{
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

int32_t PCA9505_GetBankMsk(PCA9505_T *Dev, uint32_t Bank)
{
	uint8_t cmd;
	uint8_t msk;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_msk[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&msk, sizeof(msk)))
		goto error;	
	Dev->Msks[Bank] = msk;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9505_SetBankMsk(PCA9505_T *Dev, uint32_t Bank, uint8_t Msk)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCA9505_BANKS_CNT))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	cmd = pca9505_reg_msk[Bank];
	if (Dev->Msks[Bank] != Msk)
	{
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Msk, sizeof(Msk)))
			goto error;	
		Dev->Msks[Bank] = Msk;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9505_Init(PCA9505_T *Dev, I2C_TypeDef *I2C, uint8_t Addr)
{
	uint8_t data[PCA9505_BANKS_CNT];
	uint8_t cmd;
	uint32_t i;
	
	if ((!Dev) || (!I2C))
		return -1;	
	if ((Addr < 0x20) || (Addr > 0x27))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->Addr = Addr << 1;
	Dev->I2C = I2C;
	
	cmd = PCA9505_CMD_AI + pca9505_reg_op[0];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&data, sizeof(data)))
		goto error;
	for (i = 0; i < PCA9505_BANKS_CNT; i++)
		Dev->Outs[i] = data[i];
	
	cmd = PCA9505_CMD_AI + pca9505_reg_pl[0];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&data, sizeof(data)))
		goto error;
	for (i = 0; i < PCA9505_BANKS_CNT; i++)
		Dev->Pols[i] = data[i];
	
	cmd = PCA9505_CMD_AI + pca9505_reg_ioc[0];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&data, sizeof(data)))
		goto error;
	for (i = 0; i < PCA9505_BANKS_CNT; i++)
		Dev->Dirs[i] = data[i];
	
	cmd = PCA9505_CMD_AI + pca9505_reg_msk[0];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&data, sizeof(data)))
		goto error;
	for (i = 0; i < PCA9505_BANKS_CNT; i++)
		Dev->Msks[i] = data[i];
		
	cmd = PCA9505_CMD_AI + pca9505_reg_ip[0];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&data, sizeof(data)))
		goto error;	
	for (i = 0; i < PCA9505_BANKS_CNT; i++)
		Dev->Ins[i] = data[i];
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);	
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

void PCA9505_Deinit(PCA9505_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
