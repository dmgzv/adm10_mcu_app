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
	uint32_t __PowerGood;
	UCD90120A_INFO_T Info;
	UCD90120A_CFG_T Cfg;
	UCD90120A_MON_T	Mon;
	UCD90120A_THR_T	Thr;
} UCD90120A_T;

#define UCD90120A_FAULT_LOG_RECORD_TYPE_SWD	0x01
#define UCD90120A_FAULT_LOG_RECORD_TYPE_SEQ	0x02
#define UCD90120A_FAULT_LOG_RECORD_TYPE_WD	0x03
#define UCD90120A_FAULT_LOG_RECORD_TYPE_FAN	0x08
#define UCD90120A_FAULT_LOG_RECORD_TYPE_GPI	0x09

#define UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_VOUT_OV	0x10
#define UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_VOUT_UV	0x11
#define UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_TON_MAX	0x12
#define UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_IOUT_OC	0x13
#define UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_IOUT_UC	0x14
#define UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_TEMP_OT	0x15
#define UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_SEQ_ON	0x16
#define UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_SEQ_OFF	0x17

typedef struct {
	uint32_t Time;
	uint32_t Day;
	uint16_t Type;
	uint8_t Rail;
	union {
		float f;
		uint32_t u;
	} Value;
} UCD90120A_FAULT_LOG_RECORD_T;

int32_t UCD90120A_Init(UCD90120A_T *Dev, I2C_TypeDef *I2C, uint8_t Addr, BSP_PIN_CTRL_T *PinCtrl, const UCD90120A_CFG_CMD_T *Config, const uint32_t ConfigSize);
void UCD90120A_Deinit(UCD90120A_T *Dev);
int32_t UCD90120A_Update(UCD90120A_T *Dev);
int32_t UCD90120A_ClearFaultLog(UCD90120A_T *Dev);
int32_t UCD90120A_ReadFaultLog(UCD90120A_T *Dev, uint8_t *FaultLog);
int32_t UCD90120A_ReadFaultLogSize(UCD90120A_T *Dev, uint32_t *Size);
int32_t UCD90120A_ReadFaultLogRecord(UCD90120A_T *Dev, uint32_t RecordIdx, UCD90120A_FAULT_LOG_RECORD_T *Record);

#endif /* __UCD90120A_H */
