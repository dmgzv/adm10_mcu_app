#ifndef __ALS_H
#define __ALS_H

#include <stdint.h>

#define ALS_STATUS_DISABLED	0
#define ALS_STATUS_PASSIVE	1
#define ALS_STATUS_ACTIVE	2

#define ALS_STATE_ENABLED	1
#define ALS_STATE_DISABLED	2

typedef struct {
	uint32_t State;
	uint32_t Status;
	uint32_t TxEnable;
	uint32_t Time;
	uint32_t RestartTime;
} ALS_T;

void ALS_Init(ALS_T *ALS);
void ALS_Update(ALS_T *ALS, uint32_t State, uint32_t TxEnable, uint32_t LOS, uint32_t RestartPeriod, uint32_t RestartWidth);

#endif /* __ALS_H */
