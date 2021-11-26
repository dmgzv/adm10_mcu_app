#ifndef __IP1554_H
#define __IP1554_H

#include "max7306.h"

#define IP1554_HW_NUMBER	"IP1554"

typedef struct {
	MAX7306_T max7306;
} IP1554_T;

int32_t IP1554_Process(IP1554_T *Dev, uint8_t ChassisAddress);
void IP1554_Deinit(IP1554_T *Dev);

#endif /*__IP1554_H*/
