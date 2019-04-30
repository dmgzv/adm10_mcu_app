#ifndef __SI5351_H
#define __SI5351_H

#include "i2c.h"
#include "bsp_dev.h"

#define SI5351_OUT_0_MASK (1 << 0) 
#define SI5351_OUT_1_MASK (1 << 1) 
#define SI5351_OUT_2_MASK (1 << 2) 
#define SI5351_OUT_3_MASK (1 << 3) 
#define SI5351_OUT_4_MASK (1 << 4) 
#define SI5351_OUT_5_MASK (1 << 5)
#define SI5351_OUT_6_MASK (1 << 6)
#define SI5351_OUT_7_MASK (1 << 7)
#define SI5351_OUT_ALL_MASK (0xFF) 

typedef struct {
	uint8_t Addr;
	uint8_t Data;
}SI5351_CFG_T;

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	uint8_t Addr;
	uint8_t Status;
} SI5351_T;

int32_t SI5351_Init(SI5351_T *Dev, I2C_TypeDef *I2C, uint8_t Addr, const SI5351_CFG_T *Config, const uint32_t ConfigSize, uint8_t OutsMask);
void SI5351_Deinit(SI5351_T *Dev);

#endif /* __SI5351_H */
