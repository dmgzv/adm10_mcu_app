#ifndef __BSP_SFP_H
#define __BSP_SFP_H

#include "sfp.h"
#include "bsp_sfp_pin.h"
#include "bsp_sfp_i2c_switch.h"
//#include "bsp_fpga.h"

#define BSP_SFP_CNT	20

#define BSP_SFP_1_I2C	I2C3
#define BSP_SFP_2_I2C	I2C3
#define BSP_SFP_3_I2C	I2C3
#define BSP_SFP_4_I2C	I2C3
#define BSP_SFP_5_I2C	I2C3
#define BSP_SFP_6_I2C	I2C3
#define BSP_SFP_7_I2C	I2C3
#define BSP_SFP_8_I2C	I2C3
#define BSP_SFP_9_I2C	I2C3
#define BSP_SFP_10_I2C	I2C3
#define BSP_SFP_11_I2C	I2C3
#define BSP_SFP_12_I2C	I2C3
#define BSP_SFP_13_I2C	I2C3
#define BSP_SFP_14_I2C	I2C3
#define BSP_SFP_15_I2C	I2C3
#define BSP_SFP_16_I2C	I2C3
#define BSP_SFP_17_I2C	I2C3
#define BSP_SFP_18_I2C	I2C3
#define BSP_SFP_19_I2C	I2C3
#define BSP_SFP_20_I2C	I2C3

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_SFP
#endif

#define BSP_SFP_LINE_IDX(Idx) 	(Idx + 16)
#define BSP_SFP_CLIENT_IDX(Idx) (Idx)
	
typedef struct {
	uint32_t TxDisable;
} BSP_SFP_CFG_T;
	
typedef struct {	
	uint32_t Num;	
  BSP_SFP_I2C_SWITCH_T *i2c_switch;
  BSP_SFP_PIN_T *pin;
	//BSP_FPGA_T *fpga;
	uint32_t ModAbs;
	uint32_t TxFault;
	uint32_t LOS;
	uint32_t TxDisable;
	SFP_T sfp;
} BSP_SFP_T;

uint32_t BSP_SFP_GetModAbs(BSP_SFP_T *Dev);
uint32_t BSP_SFP_GetRxLos(BSP_SFP_T *Dev);
uint32_t BSP_SFP_GetTxFault(BSP_SFP_T *Dev);
uint32_t BSP_SFP_GetTxDisable(BSP_SFP_T *Dev);
int32_t BSP_SFP_SetTxDisable(BSP_SFP_T *Dev, uint32_t TxDisable);

int32_t BSP_SFP_InitDevice(BSP_SFP_T *Dev);
int32_t BSP_SFP_UpdateDevice(BSP_SFP_T *Dev);

//int32_t BSP_SFP_Init(BSP_SFP_T *Dev, uint32_t Num, BSP_SFP_PIN_T *pin, BSP_SFP_I2C_SWITCH_T *i2c_switch, BSP_FPGA_T *fpga);
int32_t BSP_SFP_Init(BSP_SFP_T *Dev, uint32_t Num, BSP_SFP_PIN_T *pin, BSP_SFP_I2C_SWITCH_T *i2c_switch);
void BSP_SFP_Update(BSP_SFP_T *Dev, BSP_SFP_CFG_T *Cfg);


#endif /* __BSP_SFP_H */
