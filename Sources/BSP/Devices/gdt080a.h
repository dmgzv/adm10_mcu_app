#ifndef __GDT080A_H
#define __GDT080A_H

#include "pmbus.h"
#include "bsp_dev.h"

#define GDT080A_DEV_NAME "GDT080A"

#define GDT080A_STATUS_VOUT_OV 5
#define GDT080A_STATUS_IOUT_OC 4
#define GDT080A_STATUS_VIN_UV 3
#define GDT080A_STATUS_TEMP 2
#define GDT080A_STATUS_CML 1

#define GDT080A_VREF	0.6f
#define GDT080A_RTRIM_TO_VOUT(Rtrim)	( GDT080A_VREF * (2.0f + (Rtrim)) / (Rtrim))

typedef struct {
	uint8_t Id;
	uint8_t Revision;
} GDT080A_INFO_T;

typedef struct {
	float Vout;
	float Vin;
	float Iout;
	float Temp1;
	float Temp2;
} GDT080A_MON_T;

typedef struct {
	float Value;
	float Scale;
	float Trim;
	float MarginHigh;
	float MarginLow;
}GDT080A_CFG_VOUT_T;

typedef struct {
	GDT080A_CFG_VOUT_T Vout;
} GDT080A_CFG_T;

typedef struct {
	BSP_DEV;
	const I2C_TypeDef *I2C;
	const uint8_t Addr;
	const float RTrim;
	uint16_t Status;
	GDT080A_INFO_T Info;
	GDT080A_MON_T Mon;
	GDT080A_CFG_T Cfg;
}GDT080A_T;

int32_t GDT080A_Init(GDT080A_T *Dev);
void GDT080A_Deinit(GDT080A_T *Dev);
int32_t GDT080A_Update(GDT080A_T *Dev);
int32_t GDT080A_SetVout(GDT080A_T *Dev, float Vout);

#endif /* __GDT080A_H */
