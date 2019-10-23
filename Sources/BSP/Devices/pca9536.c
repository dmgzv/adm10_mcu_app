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
	
	BSP_DEV_LOCK(Dev);
	
	if (Dir)
		Dir = Dev->Dirs | (1 << Pin);
	else
		Dir = Dev->Dirs & ~(1 << Pin);
	if (Dir != Dev->Dirs)
	{
		LOG_Add("PCA9536 Write: 0x%02x 0x%02x", pca9536_reg_ioc, Dir);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_ioc, sizeof(pca9536_reg_ioc), (char*)&Dir, sizeof(Dir)))
			goto error;
		Dev->Dirs = Dir;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9536_SetPinOut(PCA9536_T *Dev, uint8_t Pin, uint8_t Out)
{	
	if ((!Dev) || (Pin > 7) || (Out > 1))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	if (Out)
		Out = Dev->Outs | (1 << Pin);
	else
		Out = Dev->Outs & ~(1 << Pin);
	if (Dev->Outs != Out)
	{
		LOG_Add("PCA9536 Write: 0x%02x 0x%02x", pca9536_reg_op, Out);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_op, sizeof(pca9536_reg_op), (char*)&Out, sizeof(Out)))
			goto error;
		Dev->Outs = Out;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9536_SetPinPol(PCA9536_T *Dev, uint8_t Pin, uint8_t Pol)
{	
	if ((!Dev) || (Pin > 7) || (Pol > 1))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	if (Pol)
		Pol = Dev->Pols | (1 << Pin);
	else
		Pol = Dev->Pols & ~(1 << Pin);
	if (Dev->Pols != Pol)
	{
		LOG_Add("PCA9536 Write: 0x%02x 0x%02x", pca9536_reg_pl, Pol);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_pl, sizeof(pca9536_reg_pl), (char*)&Pol, sizeof(Pol)))
			goto error;
		Dev->Pols = Pol;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9536_GetBankIn(PCA9536_T *Dev)
{
	uint8_t In;
	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_ip, sizeof(pca9536_reg_ip), (char*)&In, sizeof(In)))
		goto error;	
	Dev->Ins = In;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9536_GetBankDir(PCA9536_T *Dev)
{
	uint8_t Dir;
	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_ioc, sizeof(pca9536_reg_ioc), (char*)&Dir, sizeof(Dir)))
		goto error;	
	Dev->Dirs = Dir;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9536_SetBankDir(PCA9536_T *Dev, uint8_t Dir)
{	
	if ((!Dev))
		return -1;		
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	if (Dev->Dirs != Dir)
	{
		LOG_Add("PCA9536 Write: 0x%02x 0x%02x", pca9536_reg_ioc, Dir);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_ioc, sizeof(pca9536_reg_ioc), (char*)&Dir, sizeof(Dir)))
			goto error;
		Dev->Dirs = Dir;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9536_GetBankOut(PCA9536_T *Dev)
{
	uint8_t Out;
	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_op, sizeof(pca9536_reg_op), (char*)&Out, sizeof(Out)))
		goto error;	
	Dev->Outs = Out;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9536_SetBankOut(PCA9536_T *Dev, uint8_t Out)
{	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (Dev->Outs != Out)
	{
		LOG_Add("PCA9536 Write: 0x%02x 0x%02x", pca9536_reg_op, Out);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_op, sizeof(pca9536_reg_op), (char*)&Out, sizeof(Out)))
			goto error;
		Dev->Outs = Out;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9536_GetBankPol(PCA9536_T *Dev)
{
	uint8_t pol;
	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_LOCK(Dev);
	
	if (I2C_Sequential_Receive(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_pl, sizeof(pca9536_reg_pl), (char*)&pol, sizeof(pol)))
		goto error;	
	Dev->Pols = pol;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9536_SetBankPol(PCA9536_T *Dev, uint8_t Pol)
{	
	if ((!Dev))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (Dev->Pols != Pol)
	{
		LOG_Add("PCA9536 Write: 0x%02x 0x%02x", pca9536_reg_pl, Pol);
		if (I2C_Sequential_Transmit(Dev->I2C, PCA9536_I2C_ADDR, (uint8_t*)&pca9536_reg_pl, sizeof(pca9536_reg_pl), (char*)&Pol, sizeof(Pol)))
			goto error;
		Dev->Pols = Pol;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t PCA9536_Init(PCA9536_T *Dev, I2C_TypeDef *I2C)
{
	uint8_t Ins;
	uint8_t	Outs;
	uint8_t Pols;
	uint8_t Dirs;
	
	if ((!Dev) || (!I2C))
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	Dev->I2C = I2C;
	
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
	
	LOG_Add("PCA9536 Init: Ok");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	LOG_Add("PCA9536 Init: Fail");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

void PCA9536_Deinit(PCA9536_T *Dev)
{
	if (!Dev)
		return;
	LOG_Add("PCA9536 Deinit");
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_RESET);
}
