#ifndef __BSP_FPGA_H
#define __BSP_FPGA_H

#include "spi.h"
#include "bsp_dev.h"

#define BSP_FPGA_SPI	SPI1
#ifndef BSP_CONFIG_USE_SPI1
	#error	Define BSP_CONFIG_USE_SPI1 in bsp_config.h for BSP_FPGA
#endif

#define FPGA_REG_STATUS_CNT	4	
#define FPGA_REG_STICKY_CNT	4
#define FPGA_REG_MASK_CNT	4
#define FPGA_REG_MGMT_CNT	1
	
typedef struct {
	BSP_DEV;
	SPI_CS_T SpiCS;
	uint16_t Status[FPGA_REG_STATUS_CNT];
	uint16_t Sticky[FPGA_REG_STICKY_CNT];
	uint16_t Mask[FPGA_REG_MASK_CNT];
	uint16_t Mgmt[FPGA_REG_MGMT_CNT];	
}BSP_FPGA_T;

typedef enum {
	BSP_FPGA_SMI_45_OP_ADDR = 0,
	BSP_FPGA_SMI_45_OP_WRITE = 1,
	BSP_FPGA_SMI_45_OP_READ = 3,
	BSP_FPGA_SMI_45_OP_READ_INC = 2
}BSP_FPGA_SMI_45_OP_T;

#define BSP_FPGA_SPI_CS_GPIO_PIN            GPIO_PIN_3
#define BSP_FPGA_SPI_CS_GPIO_PORT           GPIOC
#define BSP_FPGA_SPI_CS_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE() 

/* SFP Status */
#define BSP_FPGA_SFP_LOS_STATUS_IDX(Idx)				((Idx) > 15 ? 1 : 0)
#define BSP_FPGA_SFP_LOS_BIT_IDX(Idx)						((Idx) > 15 ? (Idx) - 16 : (Idx))
#define BSP_FPGA_SFP_LOS(Dev, Idx)							(((Dev)->Status[BSP_FPGA_SFP_LOS_STATUS_IDX(Idx)] >> BSP_FPGA_SFP_LOS_BIT_IDX(Idx)) & 0x01)
/* PLL Status */
#define BSP_FPGA_PLL_LOCK_STATUS_IDX(Idx)				(3)
#define BSP_FPGA_PLL_LOCK_BIT_IDX(Idx)					((Idx) + 10)
#define BSP_FPGA_PLL_LOCK(Dev, Idx)							(((Dev)->Status[BSP_FPGA_PLL_LOCK_STATUS_IDX(Idx)] >> BSP_FPGA_PLL_LOCK_BIT_IDX(Idx)) & 0x01)
/* CFP Status */
#define BSP_FPGA_CFP_LOS_STATUS_IDX(Idx)				(2)
#define BSP_FPGA_CFP_LOS_BIT_IDX(Idx)						(Idx)
#define BSP_FPGA_CFP_LOS(Dev, Idx)							(((Dev)->Status[BSP_FPGA_CFP_LOS_STATUS_IDX(Idx)] >> BSP_FPGA_CFP_LOS_BIT_IDX(Idx)) & 0x01)
#define BSP_FPGA_CFP_PRG_ALRM1_STATUS_IDX(Idx)	(3)
#define BSP_FPGA_CFP_PRG_ALRM1_BIT_IDX(Idx)			(((Idx)*5) + 0)
#define BSP_FPGA_CFP_PRG_ALRM1(Dev, Idx)				(((Dev)->Status[BSP_FPGA_CFP_PRG_ALRM1_STATUS_IDX(Idx)] >> BSP_FPGA_CFP_PRG_ALRM1_BIT_IDX(Idx)) & 0x01)
#define BSP_FPGA_CFP_PRG_ALRM2_STATUS_IDX(Idx)	(3)
#define BSP_FPGA_CFP_PRG_ALRM2_BIT_IDX(Idx)			(((Idx)*5) + 1)
#define BSP_FPGA_CFP_PRG_ALRM2(Dev, Idx)				(((Dev)->Status[BSP_FPGA_CFP_PRG_ALRM2_STATUS_IDX(Idx)] >> BSP_FPGA_CFP_PRG_ALRM2_BIT_IDX(Idx)) & 0x01)
#define BSP_FPGA_CFP_PRG_ALRM3_STATUS_IDX(Idx)	(3)
#define BSP_FPGA_CFP_PRG_ALRM3_BIT_IDX(Idx)			(((Idx)*5) + 2)
#define BSP_FPGA_CFP_PRG_ALRM3(Dev, Idx)				(((Dev)->Status[BSP_FPGA_CFP_PRG_ALRM3_STATUS_IDX(Idx)] >> BSP_FPGA_CFP_PRG_ALRM3_BIT_IDX(Idx)) & 0x01)
#define BSP_FPGA_CFP_MOD_ABS_STATUS_IDX(Idx)		(3)
#define BSP_FPGA_CFP_MOD_ABS_BIT_IDX(Idx)				(((Idx)*5) + 3)
#define BSP_FPGA_CFP_MOD_ABS(Dev, Idx)					(((Dev)->Status[BSP_FPGA_CFP_MOD_ABS_STATUS_IDX(Idx)] >> BSP_FPGA_CFP_MOD_ABS_BIT_IDX(Idx)) & 0x01)
#define BSP_FPGA_CFP_GLB_ALRM_STATUS_IDX(Idx)		(3)
#define BSP_FPGA_CFP_GLB_ALRM_BIT_IDX(Idx)			(((Idx)*5) + 4)
#define BSP_FPGA_CFP_GLB_ALRM(Dev, Idx)					(((Dev)->Status[BSP_FPGA_CFP_GLB_ALRM_STATUS_IDX(Idx)] >> BSP_FPGA_CFP_GLB_ALRM_BIT_IDX(Idx)) & 0x01)
/* CFP Management */
#define BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)	(0)
#define BSP_FPGA_CFP_PRG_CTRL1_BIT_IDX(Idx)		(((Idx)*6) + 0)
#define BSP_FPGA_CFP_PRG_CTRL1(Dev, Idx)			(((Dev)->Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)] >> BSP_FPGA_CFP_PRG_CTRL1_BIT_IDX(Idx)) & 0x01)
#define BSP_FPGA_CFP_PRG_CTRL2_MGMT_IDX(Idx)	(0)
#define BSP_FPGA_CFP_PRG_CTRL2_BIT_IDX(Idx)		(((Idx)*6) + 1)
#define BSP_FPGA_CFP_PRG_CTRL2(Dev, Idx)			(((Dev)->Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)] >> BSP_FPGA_CFP_PRG_CTRL2_BIT_IDX(Idx)) & 0x01)
#define BSP_FPGA_CFP_PRG_CTRL3_MGMT_IDX(Idx)	(0)
#define BSP_FPGA_CFP_PRG_CTRL3_BIT_IDX(Idx)		(((Idx)*6) + 2)
#define BSP_FPGA_CFP_PRG_CTRL3(Dev, Idx)			(((Dev)->Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)] >> BSP_FPGA_CFP_PRG_CTRL3_BIT_IDX(Idx)) & 0x01)
#define BSP_FPGA_CFP_TX_DIS_MGMT_IDX(Idx)			(0)
#define BSP_FPGA_CFP_TX_DIS_BIT_IDX(Idx)			(((Idx)*6) + 3)
#define BSP_FPGA_CFP_TX_DIS(Dev, Idx)					(((Dev)->Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)] >> BSP_FPGA_CFP_TX_DIS_BIT_IDX(Idx)) & 0x01)
#define BSP_FPGA_CFP_LOW_PWR_MGMT_IDX(Idx)		(0)
#define BSP_FPGA_CFP_LOW_PWR_BIT_IDX(Idx)			(((Idx)*6) + 4)
#define BSP_FPGA_CFP_LOW_PWR(Dev, Idx)				(((Dev)->Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)] >> BSP_FPGA_CFP_LOW_PWR_BIT_IDX(Idx)) & 0x01)
#define BSP_FPGA_CFP_RESET_MGMT_IDX(Idx)			(0)
#define BSP_FPGA_CFP_RESET_BIT_IDX(Idx)				(((Idx)*6) + 5)
#define BSP_FPGA_CFP_RESET(Dev, Idx)					(((Dev)->Mgmt[BSP_FPGA_CFP_PRG_CTRL1_MGMT_IDX(Idx)] >> BSP_FPGA_CFP_RESET_BIT_IDX(Idx)) & 0x01)

int32_t BSP_FPGA_WriteReg(BSP_FPGA_T *Dev, uint16_t Addr, uint16_t Data);
int32_t BSP_FPGA_ReadReg(BSP_FPGA_T *Dev, uint16_t Addr, uint16_t *Data);
int32_t BSP_FPGA_WriteSMI22(BSP_FPGA_T *Dev, uint8_t PhyAddr, uint8_t DevAddr, uint16_t Data);
int32_t BSP_FPGA_ReadSMI22(BSP_FPGA_T *Dev, uint8_t PhyAddr, uint8_t DevAddr, uint16_t *Data);
int32_t BSP_FPGA_WriteSMI45(BSP_FPGA_T *Dev, uint8_t PhyAddr, uint8_t RegAddr, uint16_t Addr, uint16_t Data);
int32_t BSP_FPGA_ReadSMI45(BSP_FPGA_T *Dev, uint8_t PhyAddr, uint8_t RegAddr, uint16_t Addr, uint16_t *Data);

int32_t BSP_FPGA_Init(BSP_FPGA_T *Dev);

int32_t BSP_FPGA_GetSFPLOS(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *LOS);

int32_t BSP_FPGA_GetPLLLock(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *Lock);

int32_t BSP_FPGA_GetCFPLOS(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *LOS);
int32_t BSP_FPGA_GetCFPModAbs(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *ModAbs);
int32_t BSP_FPGA_GetCFPGlbAlrm(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *GlbAlrm);
int32_t BSP_FPGA_SetCFPReset(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t Reset);
int32_t BSP_FPGA_GetCFPReset(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *Reset);
int32_t BSP_FPGA_SetCFPTxDisable(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t TxDisable);
int32_t BSP_FPGA_GetCFPTxDisable(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *TxDisable);
int32_t BSP_FPGA_SetCFPLowPwr(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t LowPwr);
int32_t BSP_FPGA_GetCFPLowPwr(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *LowPwr);
int32_t BSP_FPGA_SetCFPPrgCntl1(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t PrgCntl);
int32_t BSP_FPGA_GetCFPPrgCntl1(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *PrgCntl);
int32_t BSP_FPGA_SetCFPPrgCntl2(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t PrgCntl);
int32_t BSP_FPGA_GetCFPPrgCntl2(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *PrgCntl);
int32_t BSP_FPGA_SetCFPPrgCntl3(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t PrgCntl);
int32_t BSP_FPGA_GetCFPPrgCntl3(BSP_FPGA_T *Dev, uint32_t Idx, uint32_t *PrgCntl);

#endif /* __BSP_FPGA_H */
