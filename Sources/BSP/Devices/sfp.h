#ifndef __SFP_H
#define __SFP_H

#include "i2c.h"
#include "bsp_dev.h"

#define SFP_THR_CMAX	0
#define SFP_THR_WMAX	1
#define SFP_THR_WMIN	2
#define SFP_THR_CMIN	3

typedef struct {
	BSP_DEV;
	I2C_TypeDef *I2C;
	char Vendor[17];
	char SerialNumber[17];
	char PartNumber[17];
	float Temp;
	float TempThrs[4];
	float RxPwr;
	float RxPwrThrs[4];
	float TxPwr;
	float TxPwrThrs[4];
} SFP_T;

int32_t SFP_Update(SFP_T *dev);
int32_t SFP_Init(SFP_T *dev, I2C_TypeDef *I2C);
void SFP_Deinit(SFP_T *dev);


#endif /* __SFP_H */
