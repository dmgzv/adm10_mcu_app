#ifndef __AT24C02C_H
#define __AT24C02C_H

#include "i2c.h"
#include "bsp_dev.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#define AT24C02C_DEV_NAME "AT24C02C"

#define AT24C02C_PAGE_SIZE		0x08
#define AT24C02C_PAGES_CNT		0x20
#define AT24C02C_SIZE					(AT24C02C_PAGE_SIZE * AT24C02C_PAGES_CNT)

typedef struct {
	BSP_DEV;
	const I2C_TypeDef *I2C;
	const uint8_t Addr;
} AT24C02C_T;

int32_t AT24C02C_Init(AT24C02C_T *Dev);
void AT24C02C_Deinit(AT24C02C_T *Dev);
int32_t AT24C02C_Write(AT24C02C_T *Dev, uint8_t Addr, char *Buffer, uint32_t Size);
int32_t AT24C02C_Read(AT24C02C_T *Dev, uint8_t Addr, char *Buffer, uint32_t Size);

#endif /*__AT24C02C_H*/
