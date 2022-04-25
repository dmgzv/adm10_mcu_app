#ifndef __DBE0125V2_H
#define __DBE0125V2_H

#include "pmbus.h"
#include "bsp_dev.h"

#define DBE0125V2_DEV_NAME "DBE0125V2"

typedef struct {
	uint16_t Word;
} DBE0125V2_STATUS_T;

typedef struct {
	float Vin;
	float Vout;
	float Iout;
	float Temp;
} DBE0125V2_READ_T;

typedef struct {
	char Id[23];
	char Model[13];
	char Revision[4];
} DBE0125V2_MFR_T;

typedef struct {
	BSP_DEV;
	const I2C_TypeDef *I2C;
	uint8_t Addr;
	DBE0125V2_MFR_T Mfr;
	DBE0125V2_READ_T Read;
	DBE0125V2_STATUS_T Status;
}DBE0125V2_T;

int32_t DBE0125V2_Init(DBE0125V2_T *Dev);
void DBE0125V2_Deinit(DBE0125V2_T *Dev);
int32_t DBE0125V2_Update(DBE0125V2_T *Dev);

#endif /* __DBE0125V2_H */
