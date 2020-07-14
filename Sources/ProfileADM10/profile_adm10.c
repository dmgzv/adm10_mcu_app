#include "profile_adm10.h"
#include "profile_adm10_enums.h"
#include "profile_adm10_bitmsks.h"

static void PROFILE_ADM10_UpdatepId(PROFILE_T *Profile)
{
	PROFILE_SetState(Profile, PROFILE_TABLE_1, PROFILE_ADM10_PID_IDX, PROFILE_STATE_IS);	
}

static void PROFILE_ADM10_UpdateSrNumber(PROFILE_T *Profile)
{
	PROFILE_SetState(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SRNUMBER_IDX, PROFILE_STATE_IS);	
}

static void PROFILE_ADM10_UpdateSwNumber(PROFILE_T *Profile)
{
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_SWNUMBER_IDX, BSP_SW_VERSION, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateMCUSwNumber(PROFILE_T *Profile)
{
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_MCUSWNUMBER_IDX, BSP_MCU_SW_VERSION, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateMaxVFPGASwNumber(PROFILE_T *Profile)
{
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
	{
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_MAXVFPGASWNUMBER_IDX, "", PROFILE_STATE_IS);
	}
	else
	{
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_MAXVFPGASWNUMBER_IDX, fpga_maxv.SwVersion, PROFILE_STATE_IS);
	}
}

static void PROFILE_ADM10_UpdateLineFPGASwNumber(PROFILE_T *Profile)
{
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
		goto error;
	
	if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_BS)
	{
		if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_LINEFPGASWNUMBER_IDX, fpga_line_agg_2x10_bs.SwVersion, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10)
	{
		if (BSP_DEV_GET_STATE(&fpga_line_adm_10) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_LINEFPGASWNUMBER_IDX, fpga_line_adm_10.SwVersion, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_EFEC)
	{
		if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_efec) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_LINEFPGASWNUMBER_IDX, fpga_line_agg_2x10_efec.SwVersion, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5)
	{
		if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_LINEFPGASWNUMBER_IDX, fpga_line_agg_2x10_bs.SwVersion, PROFILE_STATE_IS);
	}
	else
	{
		goto error;
	}	
	return;
	
error:
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_LINEFPGASWNUMBER_IDX, "", PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateClient1FPGASwNumber(PROFILE_T *Profile)
{
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
		goto error;
	
	if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_BS)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[0]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_CLIENT1FPGASWNUMBER_IDX, fpga_client_agg_2x10_bs[0].SwVersion, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_adm_10[0]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_CLIENT1FPGASWNUMBER_IDX, fpga_client_adm_10[0].SwVersion, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_EFEC)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[0]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_CLIENT1FPGASWNUMBER_IDX, fpga_client_agg_2x10_efec[0].SwVersion, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[0]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_CLIENT1FPGASWNUMBER_IDX, fpga_client_agg_2x10_bs[0].SwVersion, PROFILE_STATE_IS);
	}
	else
	{
		goto error;
	}
	return;

error:
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_CLIENT1FPGASWNUMBER_IDX, "", PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateClient2FPGASwNumber(PROFILE_T *Profile)
{
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
		goto error;
	
	if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_BS)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[1]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_CLIENT2FPGASWNUMBER_IDX, fpga_client_agg_2x10_bs[1].SwVersion, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_adm_10[1]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_CLIENT2FPGASWNUMBER_IDX, fpga_client_adm_10[1].SwVersion, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_EFEC)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[1]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_CLIENT2FPGASWNUMBER_IDX, fpga_client_agg_2x10_efec[1].SwVersion, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_adm_2_5[1]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_CLIENT2FPGASWNUMBER_IDX, fpga_client_adm_2_5[1].SwVersion, PROFILE_STATE_IS);
	}
	else
	{
		goto error;
	}
	return;

error:
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_CLIENT2FPGASWNUMBER_IDX, "", PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateHwNumber(PROFILE_T *Profile)
{
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_1, PROFILE_ADM10_HWNUMBER_IDX, BSP_HW_VERSION, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateTimeValue(PROFILE_T *Profile)
{
	uint32_t Time;
	
	Time = osKernelSysTick() / osKernelSysTickFrequency;	
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_TIME_IDX, &Time, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdatePowerAlarmsValue(PROFILE_T *Profile)
{
	uint32_t Value, i;
	
	Value = 0;
	
	if (BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
	{
		Value |= PROFILE_ADM10_POWER_ALARMS_BITMSK_48V_A | PROFILE_ADM10_POWER_ALARMS_BITMSK_48V_B;
	}
	else
	{
		if (!(dim3r3400_power_good & (1 << 0)))
			Value |= PROFILE_ADM10_POWER_ALARMS_BITMSK_48V_A;
		if (!(dim3r3400_power_good & (1 << 1)))
			Value |= PROFILE_ADM10_POWER_ALARMS_BITMSK_48V_B;			
	}
	
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
	{
		for (i = 0; i < UCD90120A_RAILS_CNT; i++)
		{
			Value |= (1 << (2 + i));
		}
	}
	else
	{
		for (i = 0; i < UCD90120A_RAILS_CNT; i++)
		{
			if (!UCD90120A_RAIL_POWER_GOOD(&ucd90120a, i))
			{
				Value |= (1 << (2 + i));
			}
		}
	}
	
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_POWERALARMS_IDX, &Value, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateHwAlarmsValue(PROFILE_T *Profile)
{
	uint32_t Value;
	
	Value = 0;
	
	if (BSP_DEV_GET_STATE(&at45db641e) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_AT45DB641E_D94;
	if (BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_DIM3R3400_A5;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_PCA9536_D107;
	if (BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_UCD90120A_D28;
	//if (BSP_DEV_GET_STATE(&gdt080a[0]) != BSP_DEV_STATE_OK)
	//	Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_GDT080A_A6;
	//if (BSP_DEV_GET_STATE(&gdt080a[1]) != BSP_DEV_STATE_OK)
	//	Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_GDT080A_A7;
	if (BSP_DEV_GET_STATE(&si5332[0]) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_SI5332_D83;
	if (BSP_DEV_GET_STATE(&si5332[1]) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_SI5332_D84;
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
	{
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_MAXV_D85;
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_LN_D92;
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_1_D88;
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_2_D90;
	}
	else
	{
		if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_BS)
		{
			if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_LN_D92;
			if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[0]) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_1_D88;
			if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[1]) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_2_D90;
		}
		else if (fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10)
		{
			if (BSP_DEV_GET_STATE(&fpga_line_adm_10) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_LN_D92;
			if (BSP_DEV_GET_STATE(&fpga_client_adm_10[0]) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_1_D88;
			if (BSP_DEV_GET_STATE(&fpga_client_adm_10[1]) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_2_D90;
		}
		else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_EFEC)
		{
			if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_efec) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_LN_D92;
			if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[0]) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_1_D88;
			if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[1]) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_2_D90;
		}
		else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5)
		{
			if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_LN_D92;
			if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[0]) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_1_D88;
			if (BSP_DEV_GET_STATE(&fpga_client_adm_2_5[1]) != BSP_DEV_STATE_OK)
				Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_2_D90;
		}
		else
		{
			Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_LN_D92;
			Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_1_D88;
			Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_FPGA_CL_2_D90;
		}
	}
	if (BSP_DEV_GET_STATE(&pca9548a[0]) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_PCA9548_D72;
	if (BSP_DEV_GET_STATE(&pca9548a[1]) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_PCA9548_D73;
	if (BSP_DEV_GET_STATE(&pca9548a[2]) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_PCA9548_D74;
	if (BSP_DEV_GET_STATE(&pcal6524[0]) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_PCAL6624_D67;
	if (BSP_DEV_GET_STATE(&pcal6524[1]) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_PCAL6624_D58;
	if (BSP_DEV_GET_STATE(&pcal6524[2]) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_PCAL6624_D57;
	if (BSP_DEV_GET_STATE(&ksz8895) != BSP_DEV_STATE_OK)
		Value |= PROFILE_ADM10_HW_ALARMS_BITMSK_KSZ8895_D99;
	
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_HWALARMS_IDX, &Value, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateLineFPGATemp(PROFILE_T *Profile)
{
	float Temp;
	
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
		goto error;
	
	if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_BS)
	{
		if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_LINEFPGATEMP_IDX, &fpga_line_agg_2x10_bs.Temp, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10)
	{
		if (BSP_DEV_GET_STATE(&fpga_line_adm_10) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_LINEFPGATEMP_IDX, &fpga_line_adm_10.Temp, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_EFEC)
	{
		if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_efec) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_LINEFPGATEMP_IDX, &fpga_line_agg_2x10_efec.Temp, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5)
	{
		if (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_LINEFPGATEMP_IDX, &fpga_line_agg_2x10_bs.Temp, PROFILE_STATE_IS);
	}
	else
	{
		goto error;
	}	
	return;

error:
	Temp = 0;
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_LINEFPGATEMP_IDX, &Temp, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateClient1FPGATemp(PROFILE_T *Profile)
{
	float Temp;
	
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
		goto error;
	
	if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_BS)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[0]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CLIENT1FPGATEMP_IDX, &fpga_client_agg_2x10_bs[0].Temp, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_adm_10[0]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CLIENT1FPGATEMP_IDX, &fpga_client_adm_10[0].Temp, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[0]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CLIENT1FPGATEMP_IDX, &fpga_client_agg_2x10_bs[0].Temp, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_EFEC)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[0]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CLIENT1FPGATEMP_IDX, &fpga_client_agg_2x10_efec[0].Temp, PROFILE_STATE_IS);
	}
	else
	{
		goto error;
	}	
	return;
	
error:
	Temp = 0;
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CLIENT1FPGATEMP_IDX, &Temp, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateClient2FPGATemp(PROFILE_T *Profile)
{
	float Temp;

	
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
		goto error;
	
	if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_BS)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[1]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CLIENT2FPGATEMP_IDX, &fpga_client_agg_2x10_bs[1].Temp, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_adm_10[1]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CLIENT2FPGATEMP_IDX, &fpga_client_adm_10[1].Temp, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_EFEC)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[1]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CLIENT2FPGATEMP_IDX, &fpga_client_agg_2x10_efec[1].Temp, PROFILE_STATE_IS);
	}
	else if (fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5)
	{
		if (BSP_DEV_GET_STATE(&fpga_client_adm_2_5[1]) != BSP_DEV_STATE_OK)
			goto error;
		PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CLIENT2FPGATEMP_IDX, &fpga_client_adm_2_5[1].Temp, PROFILE_STATE_IS);
	}
	else
	{
		goto error;
	}	
	return;

error:
	Temp = 0;
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_CLIENT2FPGATEMP_IDX, &Temp, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateBlockStateValue(PROFILE_T *Profile)
{
	uint32_t Value;
	uint32_t HwAlarms, PowerAlarms;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_BLOCKSTATE_IDX, &Value))
		return;
	
	if ((!Value) || ((Value == PROFILE_ADM10_BLOCK_STATE_ENUM_NOT_READY) && (osKernelSysTick() < (30 * osKernelSysTickFrequency))))
	{
		Value = PROFILE_ADM10_BLOCK_STATE_ENUM_NOT_READY;
	}
	else
	{
		Value = PROFILE_ADM10_BLOCK_STATE_ENUM_OK;
		
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_POWERALARMS_IDX, &PowerAlarms))
			return;
		if (PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_HWALARMS_IDX, &HwAlarms))
			return;
		
		if (HwAlarms != 0)
			Value = PROFILE_ADM10_BLOCK_STATE_ENUM_FAIL;
		if ((PowerAlarms & (~(PROFILE_ADM10_POWER_ALARMS_BITMSK_48V_A | PROFILE_ADM10_POWER_ALARMS_BITMSK_48V_B))) != 0)
			Value = PROFILE_ADM10_BLOCK_STATE_ENUM_FAIL;
		
#ifdef BSP_PWR_IN_POWER_RESET_TIMEOUT
		if (Value == PROFILE_ADM10_BLOCK_STATE_ENUM_OK)
			 dim3r3400_power_reset_timeout = BSP_PWR_IN_POWER_RESET_TIMEOUT;
#endif
	}
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_BLOCKSTATE_IDX, &Value, PROFILE_STATE_IS);		
}

static void PROFILE_ADM10_UpdateBlockColorValue(PROFILE_T *Profile)
{
	uint32_t i;
	uint32_t BlockColor;
	uint8_t Color;
	uint8_t State;
	
	BlockColor = PROFILE_COLOR_NORMAL;
	for (i = 1; i <= Profile->Descr[Profile->Idx]->Table2Size; i++)
	{
		PROFILE_GetColor(Profile, PROFILE_TABLE_2, i, &Color);
		PROFILE_GetState(Profile, PROFILE_TABLE_2, i, &State);
		if ((State == PROFILE_STATE_IS) && (BlockColor < Color) && (i != PROFILE_ADM10_BLOCKCOLOR_IDX))
			BlockColor = Color;
	}
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, PROFILE_ADM10_BLOCKCOLOR_IDX, &BlockColor, PROFILE_STATE_IS);
}

static void PROFILE_ADM10_UpdateLedsValue(PROFILE_T *Profile)
{
	uint32_t BlockColor;
	uint32_t LedColor;
	
	/*Update Leds*/
	PROFILE_GetValue(Profile, PROFILE_TABLE_2, PROFILE_ADM10_BLOCKCOLOR_IDX, &BlockColor);	
	if (BlockColor == 7)
	{
		BSP_LED_Set(BSP_LED_TYPE_ALARM, BSP_LED_COLOR_RED);
		LedColor = 2;
		PROFILE_SetValue(Profile, PROFILE_TABLE_5, PROFILE_ADM10_ALARMLED_IDX, &LedColor);
	}
	else if ((BlockColor > 3) && (BlockColor < 7))
	{
		BSP_LED_Set(BSP_LED_TYPE_ALARM, BSP_LED_COLOR_YELLOW);
		LedColor = 3;
		PROFILE_SetValue(Profile, PROFILE_TABLE_5, PROFILE_ADM10_ALARMLED_IDX, &LedColor);
	}
	else
	{
		BSP_LED_Set(BSP_LED_TYPE_ALARM, BSP_LED_COLOR_NO);
		LedColor = 0;
		PROFILE_SetValue(Profile, PROFILE_TABLE_5, PROFILE_ADM10_ALARMLED_IDX, &LedColor);
	}
	
	if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good) ||
		(BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
	{
		BSP_LED_Set(BSP_LED_TYPE_POWER, BSP_LED_COLOR_NO);
		LedColor = 0;
		PROFILE_SetValue(Profile, PROFILE_TABLE_5, PROFILE_ADM10_POWERLED_IDX, &LedColor);
	}
	else
	{		
		BSP_LED_Set(BSP_LED_TYPE_POWER, BSP_LED_COLOR_GREEN);
		LedColor = 1;
		PROFILE_SetValue(Profile, PROFILE_TABLE_5, PROFILE_ADM10_POWERLED_IDX, &LedColor);
	}
}

void PROFILE_ADM10_UpdateValues(PROFILE_T *Profile)
{
	PROFILE_ADM10_UpdatepId(Profile);
	
	PROFILE_ADM10_UpdateSrNumber(Profile);
	
	PROFILE_ADM10_UpdateSwNumber(Profile);
	
	PROFILE_ADM10_UpdateMCUSwNumber(Profile);
	
	PROFILE_ADM10_UpdateMaxVFPGASwNumber(Profile);
	
	PROFILE_ADM10_UpdateLineFPGASwNumber(Profile);
	
	PROFILE_ADM10_UpdateClient1FPGASwNumber(Profile);
	
	PROFILE_ADM10_UpdateClient2FPGASwNumber(Profile);
	
	PROFILE_ADM10_UpdateHwNumber(Profile);

	PROFILE_ADM10_UpdateTimeValue(Profile);

	PROFILE_ADM10_UpdatePowerAlarmsValue(Profile);

	PROFILE_ADM10_UpdateHwAlarmsValue(Profile);
	
	PROFILE_ADM10_UpdateLineFPGATemp(Profile);
	
	PROFILE_ADM10_UpdateClient1FPGATemp(Profile);
	
	PROFILE_ADM10_UpdateClient2FPGATemp(Profile);

	PROFILE_ADM10_UpdateBlockStateValue(Profile);
	
	PROFILE_ADM10_LINE_UpdateValues(Profile);
	
	PROFILE_ADM10_CLIENT_UpdateValues(Profile);
	
	PROFILE_ADM10_PG_UpdateValues(Profile);

	PROFILE_ADM10_UpdateBlockColorValue(Profile);

	PROFILE_ADM10_UpdateLedsValue(Profile);
}

int32_t PROFILE_ADM10_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	if (PROFILE_ADM10_LINE_CheckConfigValue(Profile, Table, Idx, Value))
		return -1;
	if (PROFILE_ADM10_CLIENT_CheckConfigValue(Profile, Table, Idx, Value))
		return -1;
	if (PROFILE_ADM10_XC_CheckConfigValue(Profile, Table, Idx, Value))
		return -1;
	if (PROFILE_ADM10_PG_CheckConfigValue(Profile, Table, Idx, Value))
		return -1;
	return 0;
}


int32_t PROFILE_ADM10_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	if (Idx == PROFILE_ADM10_SETDEFAULT_IDX)
		PROFILE_SetDefault(Profile);
	
	if (Idx == PROFILE_ADM10_SETFACTORY_IDX)
	{
		if (((*(uint32_t*)Value) == PROFILE_ADM10_SET_FACTORY_ENUM_ALL) || ((*(uint32_t*)Value) == PROFILE_ADM10_SET_FACTORY_ENUM_INFO))
			PROFILE_SetFactoryTable(Profile, PROFILE_TABLE_1);
		if (((*(uint32_t*)Value) == PROFILE_ADM10_SET_FACTORY_ENUM_ALL) || ((*(uint32_t*)Value) == PROFILE_ADM10_SET_FACTORY_ENUM_SETTINGS))
			PROFILE_SetFactoryTable(Profile, PROFILE_TABLE_3);
		if (((*(uint32_t*)Value) == PROFILE_ADM10_SET_FACTORY_ENUM_ALL) || ((*(uint32_t*)Value) == PROFILE_ADM10_SET_FACTORY_ENUM_THRESHOLDS))
			PROFILE_SetFactoryTable(Profile, PROFILE_TABLE_4);
		if (((*(uint32_t*)Value) == PROFILE_ADM10_SET_FACTORY_ENUM_ALL) || ((*(uint32_t*)Value) == PROFILE_ADM10_SET_FACTORY_ENUM_COLORS))
			PROFILE_SetFactoryTable(Profile, PROFILE_TABLE_2);
		if (((*(uint32_t*)Value) == PROFILE_ADM10_SET_FACTORY_ENUM_ALL) || ((*(uint32_t*)Value) == PROFILE_ADM10_SET_FACTORY_ENUM_PORTS))
			PROFILE_SetFactoryTable(Profile, PROFILE_TABLE_103);
	}
	
	if (Idx == PROFILE_ADM10_RESET_IDX)
		BSP_Reset();
	
	
	if (PROFILE_ADM10_LINE_SetConfigValueDependencies(Profile, Table, Idx, Value))
		return -1;
	if (PROFILE_ADM10_CLIENT_SetConfigValueDependencies(Profile, Table, Idx, Value))
		return -1;
	if (PROFILE_ADM10_XC_SetConfigValueDependencies(Profile, Table, Idx, Value))
		return -1;
	if (PROFILE_ADM10_PG_SetConfigValueDependencies(Profile, Table, Idx, Value))
		return -1;	
	return 0;
}

void PROFILE_ADM10_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t Mode, SI5332_1_SetAdjCap, SI5332_2_SetAdjCap;
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SETMODE_IDX, &Mode))
		return;
	
	if (Mode == PROFILE_ADM10_SET_MODE_ENUM_ADM_10)
	{
		FPGA_MAXV_MODE_SET(&fpga_maxv, FPGA_MAXV_MODE_ADM_10);
		si5332_mode[0] = BSP_CLK_SI5332_MODE_RSFEC;
		si5332_mode[1] = BSP_CLK_SI5332_MODE_RSFEC;
	}
	else if (Mode == PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_BS)
	{
		FPGA_MAXV_MODE_SET(&fpga_maxv, FPGA_MAXV_MODE_AGG_2X10_BS);
		si5332_mode[0] = BSP_CLK_SI5332_MODE_RSFEC;
		si5332_mode[1] = BSP_CLK_SI5332_MODE_RSFEC;
	}
	else if (Mode == PROFILE_ADM10_SET_MODE_ENUM_AGG_2X10_EFEC)
	{
		FPGA_MAXV_MODE_SET(&fpga_maxv, FPGA_MAXV_MODE_AGG_2X10_EFEC);
		si5332_mode[0] = BSP_CLK_SI5332_MODE_EFEC_I_4;
		si5332_mode[1] = BSP_CLK_SI5332_MODE_EFEC_I_4;
	}
	else if (Mode == PROFILE_ADM10_SET_MODE_ENUM_AGG_10_BS_ADM_2_5)
	{
		FPGA_MAXV_MODE_SET(&fpga_maxv, FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5);
		si5332_mode[0] = BSP_CLK_SI5332_MODE_RSFEC;
		si5332_mode[1] = BSP_CLK_SI5332_MODE_RSFEC;
	}
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SI5332_1_SETADJCAP_IDX, &SI5332_1_SetAdjCap))
		return;
	
	if (SI5332_1_SetAdjCap == PROFILE_ADM10_SI5332_1_SET_ADJ_CAP_ENUM_10_0)
	{
		si5332_cfg[0] = BSP_CLK_SI5332_10P0_CFG;
	}
	
	if (PROFILE_GetValue(Profile, PROFILE_TABLE_3, PROFILE_ADM10_SI5332_2_SETADJCAP_IDX, &SI5332_2_SetAdjCap))
		return;
	
	if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_6_0)
		si5332_cfg[1] = BSP_CLK_SI5332_6P0_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_6_5)
		si5332_cfg[1] = BSP_CLK_SI5332_6P5_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_7_0)
		si5332_cfg[1] = BSP_CLK_SI5332_7P0_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_7_5)
		si5332_cfg[1] = BSP_CLK_SI5332_7P5_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_8_0)
		si5332_cfg[1] = BSP_CLK_SI5332_8P0_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_8_5)
		si5332_cfg[1] = BSP_CLK_SI5332_8P5_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_9_0)
		si5332_cfg[1] = BSP_CLK_SI5332_9P0_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_9_5)
		si5332_cfg[1] = BSP_CLK_SI5332_9P5_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_10_0)
		si5332_cfg[1] = BSP_CLK_SI5332_10P0_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_10_5)
		si5332_cfg[1] = BSP_CLK_SI5332_10P5_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_11_0)
		si5332_cfg[1] = BSP_CLK_SI5332_11P0_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_11_5)
		si5332_cfg[1] = BSP_CLK_SI5332_11P5_CFG;
	else if (SI5332_2_SetAdjCap == PROFILE_ADM10_SI5332_2_SET_ADJ_CAP_ENUM_12_0)
		si5332_cfg[1] = BSP_CLK_SI5332_12P0_CFG;
	
	PROFILE_ADM10_LINE_UpdateConfigs(Profile);
	PROFILE_ADM10_CLIENT_UpdateConfigs(Profile);
	PROFILE_ADM10_XC_UpdateConfigs(Profile);
	PROFILE_ADM10_PG_UpdateConfigs(Profile);
}
