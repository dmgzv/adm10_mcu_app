#ifndef __UCD90120A_H
#define __UCD90120A_H

#include "pmbus.h"
#include "ucd90120a_cfg.h"
#include "bsp_dev.h"
#include "bsp_pin_ctrl.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#define UCD90120A_RAILS_CNT	12

#define UCD90120A_PIN_RESET		0
#define UCD90120A_PIN_CONTROL	1

#define UCD90120A_THR_CMAX	0
#define UCD90120A_THR_WMAX	1
#define UCD90120A_THR_WMIN	2
#define UCD90120A_THR_CMIN	3

#define UCD90120A_POWER_GOOD(Dev) 						(((Dev)->PowerGood == ((1 << UCD90120A_RAILS_CNT) - 1)) ? 1 : 0)
#define UCD90120A_RAIL_POWER_GOOD(Dev, Rail) 	(((Dev)->PowerGood & (1 << (Rail))) ? 1 : 0)

typedef struct {
	float Vout[4];
} UCD90120A_RAIL_THR_T;
typedef struct {
	UCD90120A_RAIL_THR_T Rail[UCD90120A_RAILS_CNT];
} UCD90120A_THR_T;

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
} UCD90120A_CFG_T;

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
	BSP_PIN_CTRL_T *PinCtrl;
	uint32_t PowerGood;
	UCD90120A_INFO_T Info;
	UCD90120A_CFG_T Cfg;
	UCD90120A_MON_T	Mon;
	UCD90120A_THR_T	Thr;
} UCD90120A_T;

int32_t UCD90120A_Init(UCD90120A_T *Dev, I2C_TypeDef *I2C, uint8_t Addr, BSP_PIN_CTRL_T *PinCtrl, const UCD90120A_CFG_CMD_T *Config, const uint32_t ConfigSize);
void UCD90120A_Deinit(UCD90120A_T *Dev);
int32_t UCD90120A_Update(UCD90120A_T *Dev);

#endif /* __UCD90120A_H */
