#include "als.h"

void ALS_Init(ALS_T *ALS)
{
	ALS->Time = 0;
	ALS->RestartTime = 0;
	ALS->State = ALS_STATE_DISABLED;
	ALS->Status = ALS_STATUS_DISABLED;
	ALS->TxEnable = 0;
}
void ALS_Update(ALS_T *ALS, uint32_t State, uint32_t TxEnable, uint32_t LOS, uint32_t RestartPeriod, uint32_t RestartWidth)
{
	if (!ALS)
		return;
	
	if (State == ALS_STATE_ENABLED)
	{
		if (ALS->State != ALS_STATE_ENABLED)
		{
			ALS->State = ALS_STATE_ENABLED;
			ALS->Status = ALS_STATUS_PASSIVE;
		}
		if (!TxEnable)
		{
			ALS->TxEnable = 0;
			ALS->Status = ALS_STATUS_PASSIVE;
			ALS->Time = 0;
		}
		else
		{
			if (ALS->Status == ALS_STATUS_PASSIVE)
			{
				ALS->TxEnable = 1;
				if (LOS)
				{
					ALS->Time++;
					if (ALS->Time > 1)
					{
						ALS->Status = ALS_STATUS_ACTIVE;
						ALS->Time = 0;
						ALS->RestartTime = 0;
					}
				}
				else
				{
					ALS->Time = 0;
				}
			}
			else
			{
				if (ALS->RestartTime > RestartPeriod)
					ALS->TxEnable = 1;
				else
					ALS->TxEnable = 0;
				
				ALS->RestartTime++;
				if (ALS->RestartTime > (RestartPeriod + RestartWidth))
					ALS->RestartTime = RestartWidth;
				
				if (!LOS)
				{
					ALS->Time++;
					if (ALS->Time > 1)
					{
						ALS->Status = ALS_STATUS_PASSIVE;
						ALS->Time = 0;
						ALS->RestartTime = 0;
					}
				}
				else
				{
					ALS->Time = 0;
				}
			}
		}
	}
	else
	{
		ALS->Time = 0;
		ALS->State = ALS_STATE_DISABLED;
		ALS->Status = ALS_STATUS_DISABLED;
		ALS->TxEnable = 0;
	}	
}
