#ifndef __UCD90120A_H
#define __UCD90120A_H

#include "pmbus.h"
#include "ucd90120a_cfg.h"
#include "bsp_config.h"
#include "bsp_dev.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#define UCD90120A_RAILS_CNT	12

typedef struct {
	char Id[PMBUS_BLOCK_MAX_SIZE];
} UCD90120A_INFO_T;

typedef struct {
	int8_t VoutExp;
	float VoutCmd;
	float VoutScale;
} UCD90120A_RAIL_CFG_T;

typedef struct {
	UCD90120A_RAIL_CFG_T Rail[UCD90120A_RAILS_CNT];
} UCD90120A_SET_T;

typedef struct {
	float Vout;
	float Iout;
	float Temp;
	uint8_t StatusVout;
	uint8_t StatusIout;
	uint32_t StatusMFR;
} UCD90120A_RAIL_MON_T;

typedef struct {
	UCD90120A_RAIL_MON_T Rail[UCD90120A_RAILS_CNT];
	float Temp;
	uint16_t Status;
	uint8_t CML;
} UCD90120A_MON_T;

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	uint8_t Addr;
	UCD90120A_INFO_T Info;
	UCD90120A_SET_T Set;
	UCD90120A_MON_T	Mon;
} UCD90120A_T;

int32_t UCD90120A_Init(UCD90120A_T *Dev, I2C_TypeDef *I2C, uint8_t Addr);
int32_t UCD90120A_Update(UCD90120A_T *Dev);
int32_t UCD90120A_CheckConfig(UCD90120A_T *Dev, UCD90120A_CFG_T *Config, uint32_t ConfigSize);
int32_t UCD90120A_WriteConfig(UCD90120A_T *Dev, UCD90120A_CFG_T *Config, uint32_t ConfigSize);

#endif /* __UCD90120A_H */
