#include "pca9536.h"
#include "log.h"

static const uint8_t pca9536_reg_ip = 0x00;
static const uint8_t pca9536_reg_op = 0x01;
static const uint8_t pca9536_reg_pl = 0x02;
static const uint8_t pca9536_reg_ioc = 0x03;

int32_t PCA9536_SetPinDir(PCA9536_T *Dev, uint8_t Pin, uint8_t Dir)
{
	
	if ((!Dev) || (Pin > 7) || (Dir > 1))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	if (Dir)
		Dir = Dev->Dirs | (1 << Pin);
	else
		Dir = Dev->Dirs & ~(1 << Pin);
	if (Dir != Dev->Dirs)
	{
		LOG_Info(Dev->DevName, "", "DirWrite", "0x%02x;0x%02x", pca9536_reg_ioc, Dir);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_ioc, sizeof(pca9536_reg_ioc), (char*)&Dir, sizeof(Dir)))
			goto error;
		Dev->Dirs = Dir;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCA9536_SetPinOut(PCA9536_T *Dev, uint8_t Pin, uint8_t Out)
{	
	if ((!Dev) || (Pin > 7) || (Out > 1))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	if (Out)
		Out = Dev->Outs | (1 << Pin);
	else
		Out = Dev->Outs & ~(1 << Pin);
	if (Dev->Outs != Out)
	{
		LOG_Info(Dev->DevName, "", "OutWrite", "0x%02x;0x%02x", pca9536_reg_op, Out);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_op, sizeof(pca9536_reg_op), (char*)&Out, sizeof(Out)))
			goto error;
		Dev->Outs = Out;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCA9536_SetPinPol(PCA9536_T *Dev, uint8_t Pin, uint8_t Pol)
{	
	if ((!Dev) || (Pin > 7) || (Pol > 1))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	if (Pol)
		Pol = Dev->Pols | (1 << Pin);
	else
		Pol = Dev->Pols & ~(1 << Pin);
	if (Dev->Pols != Pol)
	{
		LOG_Info(Dev->DevName, "", "PolWrite", "0x%02x;0x%02x", pca9536_reg_pl, Pol);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_pl, sizeof(pca9536_reg_pl), (char*)&Pol, sizeof(Pol)))
			goto error;
		Dev->Pols = Pol;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCA9536_GetBankIn(PCA9536_T *Dev)
{
	uint8_t In;
	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_ip, sizeof(pca9536_reg_ip), (char*)&In, sizeof(In)))
		goto error;	
	Dev->Ins = In;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCA9536_GetBankDir(PCA9536_T *Dev)
{
	uint8_t Dir;
	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_ioc, sizeof(pca9536_reg_ioc), (char*)&Dir, sizeof(Dir)))
		goto error;	
	Dev->Dirs = Dir;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCA9536_SetBankDir(PCA9536_T *Dev, uint8_t Dir)
{	
	if ((!Dev))
		return -1;		
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->Dirs != Dir)
	{
		LOG_Info(Dev->DevName, "", "DirWrite", "0x%02x;0x%02x", pca9536_reg_ioc, Dir);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_ioc, sizeof(pca9536_reg_ioc), (char*)&Dir, sizeof(Dir)))
			goto error;
		Dev->Dirs = Dir;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCA9536_GetBankOut(PCA9536_T *Dev)
{
	uint8_t Out;
	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_op, sizeof(pca9536_reg_op), (char*)&Out, sizeof(Out)))
		goto error;	
	Dev->Outs = Out;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCA9536_SetBankOut(PCA9536_T *Dev, uint8_t Out)
{	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->Outs != Out)
	{
		LOG_Info(Dev->DevName, "", "OutWrite", "0x%02x;0x%02x", pca9536_reg_op, Out);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_op, sizeof(pca9536_reg_op), (char*)&Out, sizeof(Out)))
			goto error;
		Dev->Outs = Out;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCA9536_GetBankPol(PCA9536_T *Dev)
{
	uint8_t pol;
	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_pl, sizeof(pca9536_reg_pl), (char*)&pol, sizeof(pol)))
		goto error;	
	Dev->Pols = pol;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCA9536_SetBankPol(PCA9536_T *Dev, uint8_t Pol)
{	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->Pols != Pol)
	{
		LOG_Info(Dev->DevName, "", "PolWrite", "0x%02x;0x%02x", pca9536_reg_pl, Pol);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_pl, sizeof(pca9536_reg_pl), (char*)&Pol, sizeof(Pol)))
			goto error;
		Dev->Pols = Pol;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCA9536_Init(PCA9536_T *Dev)
{
	uint8_t Ins;
	uint8_t	Outs;
	uint8_t Pols;
	uint8_t Dirs;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_ip, 1, (char*)&Ins, 1))
		goto error;		
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_op, 1, (char*)&Outs, 1))
		goto error;		
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_pl, 1, (char*)&Pols, 1))
		goto error;	
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_ioc, 1, (char*)&Dirs, 1))
		goto error;

	Dev->Ins = Ins;
	Dev->Outs = Outs;
	Dev->Dirs = Dirs;
	Dev->Pols = Pols;
	
	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void PCA9536_Deinit(PCA9536_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
