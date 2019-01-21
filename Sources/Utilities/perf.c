#include "perf.h"
#include <string.h>

void PERF_Init(PERF_T *Dev)
{
	memset(Dev, 0, sizeof(*Dev));
}

void PERF_Reset(PERF_T *Dev)
{
	Dev->State = PERF_CNTR_STATUS_AVALIABLE;
	Dev->StateTime = 0;
}

void PERF_Update(PERF_T *Dev, uint32_t Defects, uint32_t Errors, uint32_t ErrorsThr)
{
	if (Dev->State == PERF_CNTR_STATUS_UNAVALIABLE)
	{
		Dev->UAS++;
		if ((Defects) || (Errors > ErrorsThr))
		{
			Dev->StateTime = 0;
			return;
		}
		else
		{
			Dev->StateTime++;
			if (Dev->StateTime == 10)
			{
				Dev->State = PERF_CNTR_STATUS_AVALIABLE;
				Dev->StateTime = 0;
			}
		}
	}
	else
	{
		if ((Defects) || (Errors >= ErrorsThr))
		{
			Dev->EB += Errors;
			Dev->ES++;
			Dev->SES++;
			Dev->StateTime++;
			if (Dev->StateTime == 10)
			{
				Dev->State = PERF_CNTR_STATUS_UNAVALIABLE;
				Dev->StateTime = 0;
			}
		}
		else if (Errors)
		{
			Dev->EB += Errors;
			Dev->BBE += Errors;
			Dev->StateTime = 0;
		}
		else
		{
			Dev->StateTime = 0;
		}
	}
}
