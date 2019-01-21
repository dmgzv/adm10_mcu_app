#include "bsp_fpga.h"
#include <string.h>

#define FPGA_REG_STATUS_1_ADDR	0x00
#define FPGA_REG_STATUS_2_ADDR	0x01
#define FPGA_REG_STATUS_3_ADDR	0x02
#define FPGA_REG_STATUS_4_ADDR	0x03

#define FPGA_REG_STICKY_1_ADDR	0x04
#define FPGA_REG_STICKY_2_ADDR	0x05
#define FPGA_REG_STICKY_3_ADDR	0x06
#define FPGA_REG_STICKY_4_ADDR	0x07

#define FPGA_REG_MASK_1_ADDR	0x08
#define FPGA_REG_MASK_2_ADDR	0x09
#define FPGA_REG_MASK_3_ADDR	0x0A
#define FPGA_REG_MASK_4_ADDR	0x0B

#define FPGA_REG_MGMT_ADDR	0x0C

static const uint16_t fpga_reg_status_addr[FPGA_REG_STATUS_CNT] = 
{ FPGA_REG_STATUS_1_ADDR, FPGA_REG_STATUS_2_ADDR, FPGA_REG_STATUS_3_ADDR, FPGA_REG_STATUS_4_ADDR };
/*
static const uint16_t fpga_reg_sticky_addr[FPGA_REG_STICKY_CNT] = 
{ FPGA_REG_STICKY_1_ADDR, FPGA_REG_STICKY_2_ADDR, FPGA_REG_STICKY_3_ADDR, FPGA_REG_STICKY_4_ADDR };
static const uint16_t fpga_reg_mask_addr[FPGA_REG_MASK_CNT] = 
{ FPGA_REG_MASK_1_ADDR, FPGA_REG_MASK_2_ADDR, FPGA_REG_MASK_3_ADDR, FPGA_REG_MASK_4_ADDR };
*/
static const uint16_t fpga_reg_mgmt_addr[FPGA_REG_MGMT_CNT] = 
{ FPGA_REG_MGMT_ADDR };

#ifdef BSP_CONFIG_USE_FREERTOS
#define	BSP_FPGA_msDelay(ms) 		osDelay(ms)
#else
#define BSP_FPGA_msDelay(ms) 		HAL_Delay(ms)
#endif

static void BSP_FPGA_SetSpiCS(void *Priv, uint32_t PinState)
{
	HAL_GPIO_WritePin(BSP_FPGA_SPI_CS_GPIO_PORT, BSP_FPGA_SPI_CS_GPIO_PIN, PinState ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void BSP_FPGA_InitSpiCS(BSP_FPGA_T *Dev)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	BSP_FPGA_SPI_CS_GPIO_CLK_ENABLE();  
	/* SPI CS GPIO pin configuration  */
	GPIO_InitStruct.Pin       = BSP_FPGA_SPI_CS_GPIO_PIN;
	GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull      = GPIO_PULLUP;
	GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;	
	HAL_GPIO_Init(BSP_FPGA_SPI_CS_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(BSP_FPGA_SPI_CS_GPIO_PORT, BSP_FPGA_SPI_CS_GPIO_PIN, GPIO_PIN_SET);
	
	Dev->SpiCS.Priv = Dev;
	Dev->SpiCS.SetState = &BSP_FPGA_SetSpiCS;
}

static int32_t __BSP_FPGA_WriteReg(BSP_FPGA_T *Dev, uint16_t Addr, uint16_t Data)
{
	uint8_t Buffer[4];
	int32_t result;
	
	Buffer[0] = (0x02 << 6) | (0x01 << 4) | ((Addr >> 8) & 0xFF);
	Buffer[1] = Addr & 0xFF;
	Buffer[2] = (Data >> 8) & 0xFF;
	Buffer[3] = Data & 0xFF;
	
	result = SPI_Transmit(BSP_FPGA_SPI, &Dev->SpiCS, (char*) Buffer, sizeof(Buffer));
	if (result)
		return -1;	
	return 0;
}

static int32_t __BSP_FPGA_ReadReg(BSP_FPGA_T *Dev, uint16_t Addr, uint16_t *Data)
{
	uint8_t Buffer[4];
	int32_t result;
	
	Buffer[0] = (0x02 << 6) | (0x02 << 4) | ((Addr >> 8) & 0x0F);
	Buffer[1] = Addr & 0xFF;
	
	result = SPI_TransmitReceive(BSP_FPGA_SPI, &Dev->SpiCS, (char*) Buffer, (char*)Buffer, sizeof(Buffer));
	if (result)
		return -1;
	*Data = (Buffer[2] << 8) | Buffer[3];
	return 0;
}

int32_t BSP_FPGA_WriteReg(BSP_FPGA_T *Dev, uint16_t Addr, uint16_t Data)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_WriteReg(Dev, Addr, Data))
		goto error;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_ReadReg(BSP_FPGA_T *Dev, uint16_t Addr, uint16_t *Data)
{
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, Addr, Data))
		goto error;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_WriteSMI22(BSP_FPGA_T *Dev, uint8_t PhyAddr, uint8_t DevAddr, uint16_t Data)
{
	uint8_t Buffer[4];
	int32_t result;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
		
	Buffer[0] = (0x01 << 6) | (0x01 << 4) | ((PhyAddr >> 1) & 0x0F);
	Buffer[1] = ((PhyAddr & 0x01) << 7) | ((DevAddr & 0x1F) << 2) | 0x02;
	Buffer[2] = (Data >> 8) & 0xFF;
	Buffer[3] = Data & 0xFF;
	
	result = SPI_Transmit(BSP_FPGA_SPI, &Dev->SpiCS, (char*) Buffer, sizeof(Buffer));
	if (result)
		goto error;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_ReadSMI22(BSP_FPGA_T *Dev, uint8_t PhyAddr, uint8_t DevAddr, uint16_t *Data)
{
	uint8_t Buffer[12];
	int32_t result;
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Buffer[0] = (0x01 << 6) | (0x02 << 4) | ((PhyAddr >> 1) & 0x0F);
	Buffer[1] = ((PhyAddr & 0x01) << 7) | ((DevAddr & 0x1F) << 2) | 0x02;
	
	result = SPI_TransmitReceive(BSP_FPGA_SPI, &Dev->SpiCS, (char*) Buffer, (char*)Buffer, sizeof(Buffer));
	if (result)
		goto error;
	
	*Data = (Buffer[10] << 8) | Buffer[11];
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_WriteSMI45(BSP_FPGA_T *Dev, uint8_t PhyAddr, uint8_t RegAddr, uint16_t Addr, uint16_t Data)
{
	uint8_t Buffer[8];
	int32_t result;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Buffer[0] = (0x00 << 6) | (0x00 << 4) | ((PhyAddr >> 1) & 0x0F);
	Buffer[1] = ((PhyAddr & 0x01) << 7) | ((RegAddr & 0x1F) << 2) | 0x02;
	Buffer[2] = (Addr >> 8) & 0xFF;
	Buffer[3] = Addr & 0xFF;
	
	result = SPI_Transmit(BSP_FPGA_SPI, &Dev->SpiCS, (char*) Buffer, sizeof(Buffer));
	if (result)
		goto error;
	
	Buffer[0] = (0x00 << 6) | (0x01 << 4) | ((PhyAddr >> 1) & 0x0F);
	Buffer[1] = ((PhyAddr & 0x01) << 7) | ((RegAddr & 0x1F) << 2) | 0x02;
	Buffer[2] = (Data >> 8) & 0xFF;
	Buffer[3] = Data & 0xFF;
	
	result = SPI_Transmit(BSP_FPGA_SPI, &Dev->SpiCS, (char*) Buffer, sizeof(Buffer));
	if (result)
		goto error;
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_ReadSMI45(BSP_FPGA_T *Dev, uint8_t PhyAddr, uint8_t RegAddr, uint16_t Addr, uint16_t *Data)
{
	uint8_t Buffer[12];
	int32_t result;
	
	if (!Dev)
		return -1;
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Buffer[0] = (0x00 << 6) | (0x00 << 4) | ((PhyAddr >> 1) & 0x0F);
	Buffer[1] = ((PhyAddr & 0x01) << 7) | ((RegAddr & 0x1F) << 2) | 0x02;
	Buffer[2] = (Addr >> 8) & 0xFF;
	Buffer[3] = Addr & 0xFF;
	
	result = SPI_Transmit(BSP_FPGA_SPI, &Dev->SpiCS, (char*) Buffer, 8);
	if (result)
		goto error;
	
	Buffer[0] = (0x00 << 6) | (0x03 << 4) | ((PhyAddr >> 1) & 0x0F);
	Buffer[1] = ((PhyAddr & 0x01) << 7) | ((RegAddr & 0x1F) << 2) | 0x02;
	
	result = SPI_TransmitReceive(BSP_FPGA_SPI, &Dev->SpiCS, (char*) Buffer, (char*)Buffer, sizeof(Buffer));
	if (result)
		goto error;
	
	*Data = (Buffer[10] << 8) | Buffer[11];
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_Init(BSP_FPGA_T *Dev)
{
	uint32_t i;
	uint16_t Data;
	uint16_t Mgmt[FPGA_REG_MGMT_CNT];
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	BSP_DEV_LOCK(Dev);
	
	BSP_FPGA_InitSpiCS(Dev);
	
	for (i = 0; i < FPGA_REG_MGMT_CNT; i++)
	{
		if (__BSP_FPGA_ReadReg(Dev, fpga_reg_mgmt_addr[i], &Data))
			goto error;
		if (Data == 0xFFFF)
			goto error;
		Dev->Mgmt[i] = Data;	
	}	
	
	for (i = 0; i < FPGA_REG_STATUS_CNT; i++)
	{
		if (__BSP_FPGA_ReadReg(Dev, fpga_reg_status_addr[i], &Data))
			goto error;
		Dev->Status[i] = Data;	
	}
	
	for (i = 0; i < FPGA_REG_MGMT_CNT; i++)
	{
		Mgmt[i] = Dev->Mgmt[i];
	}	
	
	Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(0)] = Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(0)] | (1 << BSP_FPGA_CFP_PRG_CTRL1_BIT_IDX(0));
	Mgmt[BSP_FPGA_CFP_PRG_CTRL2_MGMT_IDX(0)] = Mgmt[BSP_FPGA_CFP_PRG_CTRL2_MGMT_IDX(0)] | (1 << BSP_FPGA_CFP_PRG_CTRL2_BIT_IDX(0));	
	Mgmt[BSP_FPGA_CFP_PRG_CTRL3_MGMT_IDX(0)] = Mgmt[BSP_FPGA_CFP_PRG_CTRL3_MGMT_IDX(0)] | (1 << BSP_FPGA_CFP_PRG_CTRL3_BIT_IDX(0));

	Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(1)] = Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(1)] | 1 << BSP_FPGA_CFP_PRG_CTRL1_BIT_IDX(1);
	Mgmt[BSP_FPGA_CFP_PRG_CTRL2_MGMT_IDX(1)] = Mgmt[BSP_FPGA_CFP_PRG_CTRL2_MGMT_IDX(1)] | 1 << BSP_FPGA_CFP_PRG_CTRL2_BIT_IDX(1);	
	Mgmt[BSP_FPGA_CFP_PRG_CTRL3_MGMT_IDX(1)] = Mgmt[BSP_FPGA_CFP_PRG_CTRL3_MGMT_IDX(1)] | 1 << BSP_FPGA_CFP_PRG_CTRL3_BIT_IDX(1);
	
	for (i = 0; i < FPGA_REG_MGMT_CNT; i++)
	{
		if (__BSP_FPGA_WriteReg(Dev, fpga_reg_mgmt_addr[i], Mgmt[i]))
			goto error;
		Dev->Mgmt[i] = Mgmt[i];	
	}	
	
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_OK);
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_SET_STATE(Dev, BSP_DEV_STATE_FAIL);
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_GetSFPLOS(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *LOS)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, fpga_reg_status_addr[BSP_FPGA_SFP_LOS_STATUS_IDX(Idx)], &Dev->Status[BSP_FPGA_SFP_LOS_STATUS_IDX(Idx)]))
		goto error;
	*LOS = BSP_FPGA_SFP_LOS(Dev, Idx);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_GetPLLLock(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *Lock)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, fpga_reg_status_addr[BSP_FPGA_PLL_LOCK_STATUS_IDX(Idx)], &Dev->Status[BSP_FPGA_PLL_LOCK_STATUS_IDX(Idx)]))
		goto error;
	*Lock = BSP_FPGA_PLL_LOCK(Dev, Idx);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_GetCFPLOS(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *LOS)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, fpga_reg_status_addr[BSP_FPGA_CFP_LOS_STATUS_IDX(Idx)], &Dev->Status[BSP_FPGA_CFP_LOS_STATUS_IDX(Idx)]))
		goto error;
	*LOS = BSP_FPGA_CFP_LOS(Dev, Idx);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_GetCFPModAbs(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *Abs)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, fpga_reg_status_addr[BSP_FPGA_CFP_MOD_ABS_STATUS_IDX(Idx)], &Dev->Status[BSP_FPGA_CFP_MOD_ABS_STATUS_IDX(Idx)]))
		goto error;
	*Abs = BSP_FPGA_CFP_MOD_ABS(Dev, Idx);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_GetCFPGlbAlrm(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *GlbAlrm)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, fpga_reg_status_addr[BSP_FPGA_CFP_GLB_ALRM_STATUS_IDX(Idx)], &Dev->Status[BSP_FPGA_CFP_GLB_ALRM_STATUS_IDX(Idx)]))
		goto error;
	*GlbAlrm = BSP_FPGA_CFP_GLB_ALRM(Dev, Idx);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_SetCFPReset(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t Reset)
{
	uint16_t Mgmt;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Mgmt = Dev->Mgmt[BSP_FPGA_CFP_RESET_MGMT_IDX(Idx)];	
	if (Reset)
		Mgmt |= (1 << BSP_FPGA_CFP_RESET_BIT_IDX(Idx));
	else
		Mgmt &= ~(1 << BSP_FPGA_CFP_RESET_BIT_IDX(Idx));
	if (__BSP_FPGA_WriteReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_RESET_MGMT_IDX(Idx)], Mgmt))
		goto error;
	Dev->Mgmt[BSP_FPGA_CFP_RESET_MGMT_IDX(Idx)] = Mgmt;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_GetCFPReset(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *Reset)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_RESET_MGMT_IDX(Idx)], &Dev->Mgmt[BSP_FPGA_CFP_RESET_MGMT_IDX(Idx)]))
		goto error;
	*Reset = BSP_FPGA_CFP_RESET(Dev, Idx);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_SetCFPTxDisable(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t TxDisable)
{
	uint16_t Mgmt;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Mgmt = Dev->Mgmt[BSP_FPGA_CFP_TX_DIS_MGMT_IDX(Idx)];	
	if (TxDisable)
		Mgmt |= (1 << BSP_FPGA_CFP_TX_DIS_BIT_IDX(Idx));
	else
		Mgmt &= ~(1 << BSP_FPGA_CFP_TX_DIS_BIT_IDX(Idx));
	if (__BSP_FPGA_WriteReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_TX_DIS_MGMT_IDX(Idx)], Mgmt))
		goto error;
	Dev->Mgmt[BSP_FPGA_CFP_TX_DIS_MGMT_IDX(Idx)] = Mgmt;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_GetCFPTxDisable(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *TxDisable)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_TX_DIS_MGMT_IDX(Idx)], &Dev->Mgmt[BSP_FPGA_CFP_TX_DIS_MGMT_IDX(Idx)]))
		goto error;
	*TxDisable = BSP_FPGA_CFP_TX_DIS(Dev, Idx);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_SetCFPLowPwr(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t LowPwr)
{
	uint16_t Mgmt;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Mgmt = Dev->Mgmt[BSP_FPGA_CFP_LOW_PWR_MGMT_IDX(Idx)];	
	if (LowPwr)
		Mgmt |= (1 << BSP_FPGA_CFP_LOW_PWR_BIT_IDX(Idx));
	else
		Mgmt &= ~(1 << BSP_FPGA_CFP_LOW_PWR_BIT_IDX(Idx));
	if (__BSP_FPGA_WriteReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_LOW_PWR_MGMT_IDX(Idx)], Mgmt))
		goto error;
	Dev->Mgmt[BSP_FPGA_CFP_LOW_PWR_MGMT_IDX(Idx)] = Mgmt;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_GetCFPLowPwr(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *LowPwr)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_LOW_PWR_MGMT_IDX(Idx)], &Dev->Mgmt[BSP_FPGA_CFP_LOW_PWR_MGMT_IDX(Idx)]))
		goto error;
	*LowPwr = BSP_FPGA_CFP_LOW_PWR(Dev, Idx);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_SetCFPPrgCntl1(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t PrgCntl)
{
	uint16_t Mgmt;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Mgmt = Dev->Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)];	
	if (PrgCntl)
		Mgmt |= (1 << BSP_FPGA_CFP_PRG_CTRL1_BIT_IDX(Idx));
	else
		Mgmt &= ~(1 << BSP_FPGA_CFP_PRG_CTRL1_BIT_IDX(Idx));
	if (__BSP_FPGA_WriteReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)], Mgmt))
		goto error;
	Dev->Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)] = Mgmt;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_GetCFPPrgCntl1(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *PrgCntl)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)], &Dev->Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)]))
		goto error;
	*PrgCntl = BSP_FPGA_CFP_PRG_CTRL1(Dev, Idx);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_SetCFPPrgCntl2(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t PrgCntl)
{
	uint16_t Mgmt;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Mgmt = Dev->Mgmt[BSP_FPGA_CFP_PRG_CTRL2_MGMT_IDX(Idx)];	
	if (PrgCntl)
		Mgmt |= (1 << BSP_FPGA_CFP_PRG_CTRL2_BIT_IDX(Idx));
	else
		Mgmt &= ~(1 << BSP_FPGA_CFP_PRG_CTRL2_BIT_IDX(Idx));
	if (__BSP_FPGA_WriteReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_PRG_CTRL2_MGMT_IDX(Idx)], Mgmt))
		goto error;
	Dev->Mgmt[BSP_FPGA_CFP_PRG_CTRL2_MGMT_IDX(Idx)] = Mgmt;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_GetCFPPrgCntl2(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *PrgCntl)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_PRG_CTRL2_MGMT_IDX(Idx)], &Dev->Mgmt[BSP_FPGA_CFP_PRG_CTRL2_MGMT_IDX(Idx)]))
		goto error;
	*PrgCntl = BSP_FPGA_CFP_PRG_CTRL2(Dev, Idx);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_SetCFPPrgCntl3(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t PrgCntl)
{
	uint16_t Mgmt;
	
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	Mgmt = Dev->Mgmt[BSP_FPGA_CFP_PRG_CTRL3_MGMT_IDX(Idx)];	
	if (PrgCntl)
		Mgmt |= (1 << BSP_FPGA_CFP_PRG_CTRL3_BIT_IDX(Idx));
	else
		Mgmt &= ~(1 << BSP_FPGA_CFP_PRG_CTRL3_BIT_IDX(Idx));
	if (__BSP_FPGA_WriteReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_PRG_CTRL3_MGMT_IDX(Idx)], Mgmt))
		goto error;
	Dev->Mgmt[BSP_FPGA_CFP_PRG_CTRL3_MGMT_IDX(Idx)] = Mgmt;	
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}

int32_t BSP_FPGA_GetCFPPrgCntl3(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *PrgCntl)
{
	if (!Dev)
		return -1;	
	if (BSP_DEV_GET_STATE(Dev) != BSP_DEV_STATE_OK)
		return -1;
	
	BSP_DEV_LOCK(Dev);
	
	if (__BSP_FPGA_ReadReg(Dev, fpga_reg_mgmt_addr[BSP_FPGA_CFP_PRG_CTRL3_MGMT_IDX(Idx)], &Dev->Mgmt[BSP_FPGA_CFP_PRG_CTRL3_MGMT_IDX(Idx)]))
		goto error;
	*PrgCntl = BSP_FPGA_CFP_PRG_CTRL3(Dev, Idx);
	
	BSP_DEV_UNLOCK(Dev);
	return 0;
	
error:
	BSP_DEV_UNLOCK(Dev);
	return -1;
}
