#include "pcal6524.h"
#include "log.h"

static const uint8_t pcal6524_reg_input[PCAL6524_BANKS_CNT] = {0x00, 0x01, 0x02};
static const uint8_t pcal6524_reg_output[PCAL6524_BANKS_CNT] = {0x04, 0x05, 0x06};
static const uint8_t pcal6524_reg_polarity[PCAL6524_BANKS_CNT] = {0x08, 0x09, 0x0A};
static const uint8_t pcal6524_reg_config[PCAL6524_BANKS_CNT] = {0x0C, 0x0D, 0x0E};


int32_t PCAL6524_SetPinCfg(PCAL6524_T *Dev, uint8_t Bank, uint8_t Pin, uint8_t Cfg)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCAL6524_BANKS_CNT) || (Pin > 7) || (Cfg > 1))
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	cmd = pcal6524_reg_config[Bank];
	if (Cfg)
		Cfg = Dev->Cfgs[Bank] | (1 << Pin);
	else
		Cfg = Dev->Cfgs[Bank] & ~(1 << Pin);
	if (Dev->Cfgs[Bank] != Cfg)
	{
		LOG_Info("BSP", Dev->DevName, Dev->DevId, "Write Cfg: 0x%02x 0x%02x", cmd, Cfg);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Cfg, sizeof(Cfg)))
			goto error;
		Dev->Cfgs[Bank] = Cfg;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCAL6524_SetPinOut(PCAL6524_T *Dev, uint8_t Bank, uint8_t Pin, uint8_t Out)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCAL6524_BANKS_CNT) || (Pin > 7) || (Out > 1))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	cmd = pcal6524_reg_output[Bank];
	if (Out)
		Out = Dev->Outs[Bank] | (1 << Pin);
	else
		Out = Dev->Outs[Bank] & ~(1 << Pin);
	if (Dev->Outs[Bank] != Out)
	{
		LOG_Info("BSP", Dev->DevName, Dev->DevId, "Write Out: 0x%02x 0x%02x", cmd, Out);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Out, sizeof(Out)))
			goto error;
		Dev->Outs[Bank] = Out;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCAL6524_SetPinPol(PCAL6524_T *Dev, uint8_t Bank, uint8_t Pin, uint8_t Pol)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCAL6524_BANKS_CNT) || (Pin > 7) || (Pol > 1))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	cmd = pcal6524_reg_polarity[Bank];
	if (Pol)
		Pol = Dev->Pols[Bank] | (1 << Pin);
	else
		Pol = Dev->Pols[Bank] & ~(1 << Pin);
	if (Dev->Pols[Bank] != Pol)
	{
		LOG_Info("BSP", Dev->DevName, Dev->DevId, "Write Pol: 0x%02x 0x%02x", cmd, Pol);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Pol, sizeof(Pol)))
			goto error;
		Dev->Pols[Bank] = Pol;
	}
		
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCAL6524_GetBankIn(PCAL6524_T *Dev, uint8_t Bank)
{
	uint8_t cmd;
	uint8_t in;
	
	if ((!Dev) || (Bank >= PCAL6524_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	cmd = pcal6524_reg_input[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&in, sizeof(in)))
		goto error;	
	Dev->Ins[Bank] = in;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCAL6524_GetBankCfg(PCAL6524_T *Dev, uint8_t Bank)
{
	uint8_t cmd;
	uint8_t cfg;
	
	if ((!Dev) || (Bank >= PCAL6524_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	cmd = pcal6524_reg_config[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&cfg, sizeof(cfg)))
		goto error;	
	Dev->Cfgs[Bank] = cfg;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCAL6524_SetBankCfg(PCAL6524_T *Dev, uint8_t Bank, uint8_t Cfg)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCAL6524_BANKS_CNT))
		return -1;		
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->Cfgs[Bank] != Cfg)
	{
		cmd = pcal6524_reg_config[Bank];
		LOG_Info("BSP", Dev->DevName, Dev->DevId, "Write Cfg: 0x%02x 0x%02x", cmd, Cfg);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Cfg, sizeof(Cfg)))
			goto error;
		Dev->Cfgs[Bank] = Cfg;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCAL6524_GetBankOut(PCAL6524_T *Dev, uint8_t Bank)
{
	uint8_t cmd;
	uint8_t out;
	
	if ((!Dev) || (Bank >= PCAL6524_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	cmd = pcal6524_reg_output[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&out, sizeof(out)))
		goto error;	
	Dev->Outs[Bank] = out;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCAL6524_SetBankOut(PCAL6524_T *Dev, uint8_t Bank, uint8_t Out)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCAL6524_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->Outs[Bank] != Out)
	{
		cmd = pcal6524_reg_output[Bank];
		LOG_Info("BSP", Dev->DevName, Dev->DevId, "Write Out: 0x%02x 0x%02x", cmd, Out);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Out, sizeof(Out)))
			goto error;
		Dev->Outs[Bank] = Out;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCAL6524_GetBankPol(PCAL6524_T *Dev, uint8_t Bank)
{
	uint8_t cmd;
	uint8_t pol;
	
	if ((!Dev) || (Bank >= PCAL6524_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;	
	
	BSP_DEV_UPDATE(Dev);
	
	cmd = pcal6524_reg_polarity[Bank];
	if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&pol, sizeof(pol)))
		goto error;	
	Dev->Pols[Bank] = pol;	
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCAL6524_SetBankPol(PCAL6524_T *Dev, uint8_t Bank, uint8_t Pol)
{
	uint8_t cmd;
	
	if ((!Dev) || (Bank >= PCAL6524_BANKS_CNT))
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_UPDATE(Dev);
	
	if (Dev->Pols[Bank] != Pol)
	{
		cmd = pcal6524_reg_polarity[Bank];
		LOG_Info("BSP", Dev->DevName, Dev->DevId, "Write Pol: 0x%02x 0x%02x", cmd, Pol);
		if (I2C_Sequential_Transmit(Dev->I2C, Dev->Addr, &cmd, sizeof(cmd), (char*)&Pol, sizeof(Pol)))
			goto error;
		Dev->Pols[Bank] = Pol;
	}
	
	BSP_DEV_UPDATE_OK(Dev);
	return 0;
	
error:
	BSP_DEV_UPDATE_FAIL(Dev);
	return -1;
}

int32_t PCAL6524_Init(PCAL6524_T *Dev)
{
	uint8_t Ins[PCAL6524_BANKS_CNT];
	uint8_t	Outs[PCAL6524_BANKS_CNT];
	uint8_t Pols[PCAL6524_BANKS_CNT];
	uint8_t Cfgs[PCAL6524_BANKS_CNT];
	uint32_t i;
	
	if ((!Dev) || (Dev->Addr < (0x20 << 1)) || (Dev->Addr > (0x23 << 1)))
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	for (i = 0; i < PCAL6524_BANKS_CNT; i++)
	{
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&pcal6524_reg_input[i], 1, (char*)&Ins[i], 1))
			goto error;		
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&pcal6524_reg_output[i], 1, (char*)&Outs[i], 1))
			goto error;		
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&pcal6524_reg_polarity[i], 1, (char*)&Pols[i], 1))
			goto error;	
		if (I2C_Sequential_Receive(Dev->I2C, Dev->Addr, (uint8_t*)&pcal6524_reg_config[i], 1, (char*)&Cfgs[i], 1))
			goto error;
	}
	
	for (i = 0; i < PCAL6524_BANKS_CNT; i++)
	{		
		Dev->Ins[i] = Ins[i];
		Dev->Outs[i] = Outs[i];
		Dev->Cfgs[i] = Cfgs[i];
		Dev->Pols[i] = Pols[i];
	}

	BSP_DEV_INIT_OK(Dev);
	return 0;
	
error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

void PCAL6524_Deinit(PCAL6524_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);
}
