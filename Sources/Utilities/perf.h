#ifndef __PERF_H
#define __PERF_H

#include <stdint.h>

#define PERF_CNTR_STATUS_AVALIABLE	0
#define PERF_CNTR_STATUS_UNAVALIABLE	1

#define PERF_ODU4_SES_ERRORS_THR		128459
#define PERF_ODU3_SES_ERRORS_THR		49424
#define PERF_ODU2e_SES_ERRORS_THR		12748
#define PERF_ODU2_SES_ERRORS_THR		12304
#define PERF_ODU1_SES_ERRORS_THR		3064
#define PERF_ODU0_SES_ERRORS_THR		1526

typedef struct {
	uint32_t State;
	uint32_t StateTime;
	
	uint32_t EB;
	uint32_t ES;
	uint32_t BBE;
	uint32_t SES;
	uint32_t UAS;
} PERF_T;

void PERF_Init(PERF_T *Dev);
void PERF_Reset(PERF_T *Dev);
void PERF_Update(PERF_T *Dev, uint32_t Defects, uint32_t Errors, uint32_t ErrorsThr);

#endif /* __PERF_H */
