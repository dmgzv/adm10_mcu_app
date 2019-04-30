#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"

DIM3R3400_T dim3r3400;
uint32_t dim3r3400_power_good;

static int32_t BSP_PWR_IN_CheckConditions(void)
{
	return 0;
}

void BSP_PWR_IN_Process(void)
{
	uint32_t i, PowerGood, State;
	
	if (BSP_PWR_IN_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_RESET)
		{
			dim3r3400_power_good = 0;
			DIM3R3400_Deinit(&dim3r3400);
			BSP_PWR_PIN_SetState(BSP_PWR_PIN_12V_PWR, 0);
		}
	}
	else
	{
		/* External faults absent */
		if(BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
		{
			dim3r3400_power_good = 0;
			DIM3R3400_Init(&dim3r3400, BSP_PWR_IN_DIM3R3400_I2C, BSP_PWR_IN_DIM3R3400_I2C_ADDR);
		}
		else
		{
			DIM3R3400_Update(&dim3r3400);
			if (dim3r3400_power_good)
			{
				PowerGood = 0;
				for (i = 0; i < DIM3R3400_IN_CNT; i++)
				{
					if ((dim3r3400.Vin[i] > BSP_PWR_IN_MIN_OFF) && (dim3r3400.Vin[i] < BSP_PWR_IN_MAX_OFF))
					{
						PowerGood |= 1 << i;
					}
				}
				dim3r3400_power_good = PowerGood;
			}
			else
			{
				PowerGood = 0;
				for (i = 0; i < DIM3R3400_IN_CNT; i++)
				{
					if ((dim3r3400.Vin[i] > BSP_PWR_IN_MIN_ON) && (dim3r3400.Vin[i] < BSP_PWR_IN_MAX_ON))
					{
						PowerGood |= 1 << i;
					}
				}
				dim3r3400_power_good = PowerGood;
			}
			if (!BSP_PWR_PIN_Init(BSP_PWR_PIN_12V_PWR))
			{
				if(!BSP_PWR_PIN_GetState(BSP_PWR_PIN_12V_PWR, &State))
				{
					if ((dim3r3400_power_good) && (!State))
					{
						BSP_PWR_PIN_SetState(BSP_PWR_PIN_12V_PWR, 1);
						osDelay(1000);
						I2C_Deinit();
						I2C_Init();
					}
					else if ((!dim3r3400_power_good) && (State))
					{
						BSP_PWR_PIN_SetState(BSP_PWR_PIN_12V_PWR, 0);
					}
				}
			}
		}		
	}
}

