#include "profile_adm_client.h"
#include "profile_adm_values.h"
#include "als.h"
#include "perf.h"
#include <string.h>

static const uint32_t ProfileADMDynamicLnSFPStateIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SFP_STATE_IDX, PROFILE_ADM_LN_2_SFP_STATE_IDX, PROFILE_ADM_LN_3_SFP_STATE_IDX, PROFILE_ADM_LN_4_SFP_STATE_IDX,
};

static const uint32_t ProfileADMDynamicLnSFPTempIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SFP_TEMP_IDX, PROFILE_ADM_LN_2_SFP_TEMP_IDX, PROFILE_ADM_LN_3_SFP_TEMP_IDX, PROFILE_ADM_LN_4_SFP_TEMP_IDX,
};

static const uint32_t ProfileADMDynamicLnSFPRxPwrIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SFP_RXPWR_IDX, PROFILE_ADM_LN_2_SFP_RXPWR_IDX, PROFILE_ADM_LN_3_SFP_RXPWR_IDX, PROFILE_ADM_LN_4_SFP_RXPWR_IDX,
};

static const uint32_t ProfileADMDynamicLnSFPTxPwrIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SFP_TXPWR_IDX, PROFILE_ADM_LN_2_SFP_TXPWR_IDX, PROFILE_ADM_LN_3_SFP_TXPWR_IDX, PROFILE_ADM_LN_4_SFP_TXPWR_IDX,
};

static const uint32_t ProfileADMDynamicLnOTU2AlarmsIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_OTU2_ALARMS_IDX, PROFILE_ADM_LN_2_OTU2_ALARMS_IDX, PROFILE_ADM_LN_3_OTU2_ALARMS_IDX, PROFILE_ADM_LN_4_OTU2_ALARMS_IDX,
};

static const uint32_t ProfileADMDynamicLnOTU2FECBERIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_OTU2_FECBER_IDX, PROFILE_ADM_LN_2_OTU2_FECBER_IDX, PROFILE_ADM_LN_3_OTU2_FECBER_IDX, PROFILE_ADM_LN_4_OTU2_FECBER_IDX,
};

static const uint32_t ProfileADMDynamicLnODU2AlarmsIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_ODU2_ALARMS_IDX, PROFILE_ADM_LN_2_ODU2_ALARMS_IDX, PROFILE_ADM_LN_3_ODU2_ALARMS_IDX, PROFILE_ADM_LN_4_ODU2_ALARMS_IDX,
};

static const uint32_t ProfileADMInfoLnDestinationIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_DESTINATION_IDX, PROFILE_ADM_LN_2_DESTINATION_IDX, PROFILE_ADM_LN_3_DESTINATION_IDX, PROFILE_ADM_LN_4_DESTINATION_IDX,
};

static const uint32_t ProfileADMInfoLnSFPVendorIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SFP_VENDOR_IDX, PROFILE_ADM_LN_2_SFP_VENDOR_IDX, PROFILE_ADM_LN_3_SFP_VENDOR_IDX, PROFILE_ADM_LN_4_SFP_VENDOR_IDX,
};

static const uint32_t ProfileADMInfoLnSFPSrNumberIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SFP_SRNUMBER_IDX, PROFILE_ADM_LN_2_SFP_SRNUMBER_IDX, PROFILE_ADM_LN_3_SFP_SRNUMBER_IDX, PROFILE_ADM_LN_4_SFP_SRNUMBER_IDX,
};

static const uint32_t ProfileADMInfoLnSFPPtNumberIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SFP_PTNUMBER_IDX, PROFILE_ADM_LN_2_SFP_PTNUMBER_IDX, PROFILE_ADM_LN_3_SFP_PTNUMBER_IDX, PROFILE_ADM_LN_4_SFP_PTNUMBER_IDX,
};

static const uint32_t ProfileADMStaticLnSFPRxPwrIdx[PROFILE_ADM_LINE_PORTS][4] = 
{
	{PROFILE_ADM_LN_1_SFP_RXPWRCMIN_IDX, PROFILE_ADM_LN_1_SFP_RXPWRWMIN_IDX, PROFILE_ADM_LN_1_SFP_RXPWRWMAX_IDX, PROFILE_ADM_LN_1_SFP_RXPWRCMAX_IDX},
	{PROFILE_ADM_LN_2_SFP_RXPWRCMIN_IDX, PROFILE_ADM_LN_2_SFP_RXPWRWMIN_IDX, PROFILE_ADM_LN_2_SFP_RXPWRWMAX_IDX, PROFILE_ADM_LN_2_SFP_RXPWRCMAX_IDX},
	{PROFILE_ADM_LN_3_SFP_RXPWRCMIN_IDX, PROFILE_ADM_LN_3_SFP_RXPWRWMIN_IDX, PROFILE_ADM_LN_3_SFP_RXPWRWMAX_IDX, PROFILE_ADM_LN_3_SFP_RXPWRCMAX_IDX},
	{PROFILE_ADM_LN_4_SFP_RXPWRCMIN_IDX, PROFILE_ADM_LN_4_SFP_RXPWRWMIN_IDX, PROFILE_ADM_LN_4_SFP_RXPWRWMAX_IDX, PROFILE_ADM_LN_4_SFP_RXPWRCMAX_IDX},
};

static const uint32_t ProfileADMStaticLnSFPTxPwrIdx[PROFILE_ADM_LINE_PORTS][4] = 
{
	{PROFILE_ADM_LN_1_SFP_TXPWRCMIN_IDX, PROFILE_ADM_LN_1_SFP_TXPWRWMIN_IDX, PROFILE_ADM_LN_1_SFP_TXPWRWMAX_IDX, PROFILE_ADM_LN_1_SFP_TXPWRCMAX_IDX},
	{PROFILE_ADM_LN_2_SFP_TXPWRCMIN_IDX, PROFILE_ADM_LN_2_SFP_TXPWRWMIN_IDX, PROFILE_ADM_LN_2_SFP_TXPWRWMAX_IDX, PROFILE_ADM_LN_2_SFP_TXPWRCMAX_IDX},
	{PROFILE_ADM_LN_3_SFP_TXPWRCMIN_IDX, PROFILE_ADM_LN_3_SFP_TXPWRWMIN_IDX, PROFILE_ADM_LN_3_SFP_TXPWRWMAX_IDX, PROFILE_ADM_LN_3_SFP_TXPWRCMAX_IDX},
	{PROFILE_ADM_LN_4_SFP_TXPWRCMIN_IDX, PROFILE_ADM_LN_4_SFP_TXPWRWMIN_IDX, PROFILE_ADM_LN_4_SFP_TXPWRWMAX_IDX, PROFILE_ADM_LN_4_SFP_TXPWRCMAX_IDX},
};

static const uint32_t ProfileADMStaticLnSFPTempIdx[PROFILE_ADM_LINE_PORTS][4] = 
{
	{PROFILE_ADM_LN_1_SFP_TEMPCMIN_IDX, PROFILE_ADM_LN_1_SFP_TEMPWMIN_IDX, PROFILE_ADM_LN_1_SFP_TEMPWMAX_IDX, PROFILE_ADM_LN_1_SFP_TEMPCMAX_IDX},
	{PROFILE_ADM_LN_2_SFP_TEMPCMIN_IDX, PROFILE_ADM_LN_2_SFP_TEMPWMIN_IDX, PROFILE_ADM_LN_2_SFP_TEMPWMAX_IDX, PROFILE_ADM_LN_2_SFP_TEMPCMAX_IDX},
	{PROFILE_ADM_LN_3_SFP_TEMPCMIN_IDX, PROFILE_ADM_LN_3_SFP_TEMPWMIN_IDX, PROFILE_ADM_LN_3_SFP_TEMPWMAX_IDX, PROFILE_ADM_LN_3_SFP_TEMPCMAX_IDX},
	{PROFILE_ADM_LN_4_SFP_TEMPCMIN_IDX, PROFILE_ADM_LN_4_SFP_TEMPWMIN_IDX, PROFILE_ADM_LN_4_SFP_TEMPWMAX_IDX, PROFILE_ADM_LN_4_SFP_TEMPCMAX_IDX},
};

static const uint32_t ProfileADMConfigLnSetStateIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SETSTATE_IDX, PROFILE_ADM_LN_2_SETSTATE_IDX, PROFILE_ADM_LN_3_SETSTATE_IDX, PROFILE_ADM_LN_4_SETSTATE_IDX,
};

static const uint32_t ProfileADMConfigLnSFPEnableTxIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SFP_ENABLETX_IDX, PROFILE_ADM_LN_2_SFP_ENABLETX_IDX, PROFILE_ADM_LN_3_SFP_ENABLETX_IDX, PROFILE_ADM_LN_4_SFP_ENABLETX_IDX,
};

static const uint32_t ProfileADMConfigLnSetDataTypeIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SETDATATYPE_IDX, PROFILE_ADM_LN_2_SETDATATYPE_IDX, PROFILE_ADM_LN_3_SETDATATYPE_IDX, PROFILE_ADM_LN_4_SETDATATYPE_IDX,
};

static const uint32_t ProfileADMConfigLnSetLoopbackIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SETLOOPBACK_IDX, PROFILE_ADM_LN_2_SETLOOPBACK_IDX, PROFILE_ADM_LN_3_SETLOOPBACK_IDX, PROFILE_ADM_LN_4_SETLOOPBACK_IDX,
};

static const uint32_t ProfileADMConfigLnSetFactoryIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN_1_SETFACTORY_IDX, PROFILE_ADM_LN_2_SETFACTORY_IDX, PROFILE_ADM_LN_3_SETFACTORY_IDX, PROFILE_ADM_LN_4_SETFACTORY_IDX
};

static const uint32_t ProfileADMPerfLnOTU2EBIdx[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_LN_1_OTU2_EB_IDX, PROFILE_ADM_LN_2_OTU2_EB_IDX, PROFILE_ADM_LN_3_OTU2_EB_IDX, PROFILE_ADM_LN_4_OTU2_EB_IDX
};

static const uint32_t ProfileADMPerfLnOTU2ESIdx[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_LN_1_OTU2_ES_IDX, PROFILE_ADM_LN_2_OTU2_ES_IDX, PROFILE_ADM_LN_3_OTU2_ES_IDX, PROFILE_ADM_LN_4_OTU2_ES_IDX
};

static const uint32_t ProfileADMPerfLnOTU2SESIdx[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_LN_1_OTU2_SES_IDX, PROFILE_ADM_LN_2_OTU2_SES_IDX, PROFILE_ADM_LN_3_OTU2_SES_IDX, PROFILE_ADM_LN_4_OTU2_SES_IDX
};

static const uint32_t ProfileADMPerfLnOTU2BBEIdx[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_LN_1_OTU2_BBE_IDX, PROFILE_ADM_LN_2_OTU2_BBE_IDX, PROFILE_ADM_LN_3_OTU2_BBE_IDX, PROFILE_ADM_LN_4_OTU2_BBE_IDX
};

static const uint32_t ProfileADMPerfLnOTU2UASIdx[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_LN_1_OTU2_UAS_IDX, PROFILE_ADM_LN_2_OTU2_UAS_IDX, PROFILE_ADM_LN_3_OTU2_UAS_IDX, PROFILE_ADM_LN_4_OTU2_UAS_IDX
};

static const uint32_t ProfileADMPerfLnODU2EBIdx[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_LN_1_ODU2_EB_IDX, PROFILE_ADM_LN_2_ODU2_EB_IDX, PROFILE_ADM_LN_3_ODU2_EB_IDX, PROFILE_ADM_LN_4_ODU2_EB_IDX
};

static const uint32_t ProfileADMPerfLnODU2ESIdx[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_LN_1_ODU2_ES_IDX, PROFILE_ADM_LN_2_ODU2_ES_IDX, PROFILE_ADM_LN_3_ODU2_ES_IDX, PROFILE_ADM_LN_4_ODU2_ES_IDX
};

static const uint32_t ProfileADMPerfLnODU2SESIdx[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_LN_1_ODU2_SES_IDX, PROFILE_ADM_LN_2_ODU2_SES_IDX, PROFILE_ADM_LN_3_ODU2_SES_IDX, PROFILE_ADM_LN_4_ODU2_SES_IDX
};

static const uint32_t ProfileADMPerfLnODU2BBEIdx[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_LN_1_ODU2_BBE_IDX, PROFILE_ADM_LN_2_ODU2_BBE_IDX, PROFILE_ADM_LN_3_ODU2_BBE_IDX, PROFILE_ADM_LN_4_ODU2_BBE_IDX
};

static const uint32_t ProfileADMPerfLnODU2UASIdx[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_LN_1_ODU2_UAS_IDX, PROFILE_ADM_LN_2_ODU2_UAS_IDX, PROFILE_ADM_LN_3_ODU2_UAS_IDX, PROFILE_ADM_LN_4_ODU2_UAS_IDX
};

static const uint32_t ProfileADMPortLnInfoIdx[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_PTLINE1INFO_IDX, PROFILE_ADM_PTLINE2INFO_IDX, PROFILE_ADM_PTLINE3INFO_IDX, PROFILE_ADM_PTLINE4INFO_IDX
};

static const uint32_t ProfileADMPortLnCatIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_PTLINE1CAT_IDX, PROFILE_ADM_PTLINE2CAT_IDX, PROFILE_ADM_PTLINE3CAT_IDX, PROFILE_ADM_PTLINE4CAT_IDX
};

static PERF_T LnOTU2Perf[PROFILE_ADM_LINE_PORTS];
static PERF_T LnODU2Perf[PROFILE_ADM_LINE_PORTS];

//static ALS_T LnALSLn[PROFILE_ADM_LINE_PORTS];
//static ALS_T LnALSLn[PROFILE_ADM_LINE_PORTS];

static int32_t PROFILE_ADM_LINE_CheckLnSetStateValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Idx)
{
	uint32_t SetState;
	
	/* Do not alow to go from OOS to IS */
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &SetState);
	if ((SetState == PROFILE_STATE_OOS) && (*(uint32_t*)Value == PROFILE_STATE_IS))
		return -1;
	return 0;
}

static int32_t PROFILE_ADM_LINE_CheckLnSFPEnableTxValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Idx)
{
	uint32_t SetState;
	
	/* Do not alow to enable tx if set state if OOS */
	if (*(uint32_t*)Value == 1)
	{
		PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &SetState);
		if (SetState == PROFILE_STATE_OOS)
			return -1;
	}
	return 0;
}

int32_t PROFILE_ADM_LINE_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM_LINE_PORTS; i++)
	{
		if (TableIdx == ProfileADMConfigLnSetStateIdx[i])
		{
			if (PROFILE_ADM_LINE_CheckLnSetStateValue(Profile, Table, TableIdx, Value, i))
				return -1;
			return 0;
		}
		if (TableIdx == ProfileADMConfigLnSFPEnableTxIdx[i])
		{
			if (PROFILE_ADM_LINE_CheckLnSFPEnableTxValue(Profile, Table, TableIdx, Value, i))
				return -1;
			return 0;
		}
	}
	return 0;
}

static int32_t PROFILE_ADM_LINE_SetLnSetStateValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Idx)
{
	uint32_t EnableTx;	
	/* Disable tx when state is OOS*/
	if (*(uint32_t*)Value == PROFILE_STATE_OOS)
	{
		EnableTx = 0;
		PROFILE_SetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSFPEnableTxIdx[Idx], &EnableTx);
	}
	return 0;
}

static int32_t PROFILE_ADM_LINE_SetLnSFPEnableTxValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Idx)
{
	return 0;
}

static int32_t PROFILE_ADM_LINE_SetLnSetFactoryValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value, uint32_t Idx)
{
	uint32_t i;
	
	if ((*(uint32_t*)Value == 0) || (*(uint32_t*)Value == 1))
	{
		/* Set all info client parametes to factory*/
		PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_1, ProfileADMInfoLnDestinationIdx[Idx]);
		PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPVendorIdx[Idx]);
		PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPSrNumberIdx[Idx]);
		PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPPtNumberIdx[Idx]);
	}
	if ((*(uint32_t*)Value == 0) || (*(uint32_t*)Value == 2))
	{
		/* Set all config client parametes to factory*/
		PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx]);
		PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSFPEnableTxIdx[Idx]);
		PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetDataTypeIdx[Idx]);
		PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetLoopbackIdx[Idx]);
	}
	if ((*(uint32_t*)Value == 0) || (*(uint32_t*)Value == 3))
	{
		/* Set all threshoulds client parametes to factory*/
		for (i = 0; i < 4; i++)
		{
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPRxPwrIdx[Idx][i]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPTxPwrIdx[Idx][i]);
			PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPTempIdx[Idx][i]);
		}
	}
	if ((*(uint32_t*)Value == 0) || (*(uint32_t*)Value == 4))
	{
		/* Set all dynamic client parametes colors to factory*/
		PROFILE_SetFactoryAlarmColors(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnSFPStateIdx[Idx]);
		PROFILE_SetFactoryAlarmColors(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnSFPTempIdx[Idx]);
		PROFILE_SetFactoryAlarmColors(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnSFPRxPwrIdx[Idx]);
		PROFILE_SetFactoryAlarmColors(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnSFPTxPwrIdx[Idx]);
		PROFILE_SetFactoryAlarmColors(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnOTU2AlarmsIdx[Idx]);
		PROFILE_SetFactoryAlarmColors(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnODU2AlarmsIdx[Idx]);
	}
	if ((*(uint32_t*)Value == 0) || (*(uint32_t*)Value == 5))
	{
		/* Set all port line parameters to factory */
		PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_103, ProfileADMPortLnInfoIdx[Idx]);
		PROFILE_SetFactoryValue(Profile, PROFILE_TABLE_103, ProfileADMPortLnCatIdx[Idx]);
	}
	return 0;
}

int32_t PROFILE_ADM_LINE_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t TableIdx, void *Value)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM_LINE_PORTS; i++)
	{
		if (TableIdx == ProfileADMConfigLnSetStateIdx[i])
		{
			if (PROFILE_ADM_LINE_SetLnSetStateValueDependencies(Profile, Table, TableIdx, Value, i))
				return -1;
			return 0;
		}
		if (TableIdx == ProfileADMConfigLnSFPEnableTxIdx[i])
		{
			if (PROFILE_ADM_LINE_SetLnSFPEnableTxValueDependencies(Profile, Table, TableIdx, Value, i))
				return -1;
			return 0;
		}
		if (TableIdx == ProfileADMConfigLnSetFactoryIdx[i])
		{
			if (PROFILE_ADM_LINE_SetLnSetFactoryValueDependencies(Profile, Table, TableIdx, Value, i))
				return -1;
			return 0;
		}
	}
	return 0;
}

void PROFILE_ADM_LINE_UpdateConfigs(PROFILE_T *Profile)
{
	uint32_t i;
	uint32_t State, SFPEnableTx, Loopback;
	
	for (i = 0; i < PROFILE_ADM_LINE_PORTS; i++)
	{
		PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[i], &State);
		PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSFPEnableTxIdx[i], &SFPEnableTx);
		PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetLoopbackIdx[i], &Loopback);
		
		sfp[BSP_SFP_LINE_IDX(i)].Pin.TxDisable  = SFPEnableTx ? 0 : 1;
		
		if (Loopback == 0)
		{
			BSP_FPGA_LINE_LOOPBACK_SET(i, BSP_FPGA_LINE_LOOPBACK_NO);
		}
		else if (Loopback == 1)
		{
			BSP_FPGA_LINE_LOOPBACK_SET(i, BSP_FPGA_LINE_LOOPBACK_POST_CDR);
		}
		else if (Loopback == 2)
		{
			BSP_FPGA_LINE_LOOPBACK_SET(i, BSP_FPGA_LINE_LOOPBACK_SERIAL);
		}
		
	}
}

static void PROFILE_ADM_LINE_UpdateInfoAndStaticValues(PROFILE_T *Profile, uint32_t Idx)
{
	char Tmp[PROFILE_STRING_MAX_LEN];
	uint32_t UpdateFlag;
	uint32_t State;
	
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
	
	switch (BSP_DEV_GET_STATE(&sfp[BSP_SFP_LINE_IDX(Idx)]))
	{
		case BSP_DEV_STATE_OK:
			//Check if inserted module is same as previous
			UpdateFlag = 1;
			PROFILE_GetValue(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPVendorIdx[Idx], Tmp);
			if (!strncmp(Tmp, sfp[BSP_SFP_LINE_IDX(Idx)].Info.Vendor, sizeof(sfp[BSP_SFP_LINE_IDX(Idx)].Info.Vendor)))
			{
				PROFILE_GetValue(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPSrNumberIdx[Idx], Tmp);
				if (!strncmp(Tmp, sfp[BSP_SFP_LINE_IDX(Idx)].Info.SerialNumber, sizeof(sfp[BSP_SFP_LINE_IDX(Idx)].Info.SerialNumber)))
				{
					PROFILE_GetValue(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPPtNumberIdx[Idx], Tmp);
					if (!strncmp(Tmp, sfp[BSP_SFP_LINE_IDX(Idx)].Info.PartNumber, sizeof(sfp[BSP_SFP_LINE_IDX(Idx)].Info.PartNumber)))
					{
						UpdateFlag = 0;
					}
				}
			}
			//If module is not the same as previous, then update info and thresholds values
			if (UpdateFlag)
			{
				PROFILE_SetValue(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPVendorIdx[Idx], sfp[BSP_SFP_LINE_IDX(Idx)].Info.Vendor);
				PROFILE_SetValue(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPSrNumberIdx[Idx], sfp[BSP_SFP_LINE_IDX(Idx)].Info.SerialNumber);
				PROFILE_SetValue(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPPtNumberIdx[Idx], sfp[BSP_SFP_LINE_IDX(Idx)].Info.PartNumber);
				
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPRxPwrIdx[Idx][0], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.RxPwr[SFP_THR_CMIN]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPRxPwrIdx[Idx][1], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.RxPwr[SFP_THR_WMIN]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPRxPwrIdx[Idx][2], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.RxPwr[SFP_THR_WMAX]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPRxPwrIdx[Idx][3], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.RxPwr[SFP_THR_CMAX]);
				
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPTxPwrIdx[Idx][0], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.TxPwr[SFP_THR_CMIN]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPTxPwrIdx[Idx][1], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.TxPwr[SFP_THR_WMIN]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPTxPwrIdx[Idx][2], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.TxPwr[SFP_THR_WMAX]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPTxPwrIdx[Idx][3], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.TxPwr[SFP_THR_CMAX]);
				
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPTempIdx[Idx][0], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.Temp[SFP_THR_CMIN]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPTempIdx[Idx][1], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.Temp[SFP_THR_WMIN]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPTempIdx[Idx][2], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.Temp[SFP_THR_WMAX]);
				PROFILE_SetValue(Profile, PROFILE_TABLE_4, ProfileADMStaticLnSFPTempIdx[Idx][3], &sfp[BSP_SFP_LINE_IDX(Idx)].Thr.Temp[SFP_THR_CMAX]);
				
				PROFILE_FLASH_ParamsSave(&Profile->ProfileFlash);
			}
			PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADMInfoLnDestinationIdx[Idx], State);
			PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPVendorIdx[Idx], State);
			PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPSrNumberIdx[Idx], State);
			PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPPtNumberIdx[Idx], State);
			break;
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
		default:
			PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADMInfoLnDestinationIdx[Idx], State);
			PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPVendorIdx[Idx], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPSrNumberIdx[Idx], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, PROFILE_TABLE_1, ProfileADMInfoLnSFPPtNumberIdx[Idx], PROFILE_STATE_OOS);
			break;
	}
}

static void PROFILE_ADM_LINE_UpdateSFPStateValue(PROFILE_T *Profile, uint32_t Idx)
{
	uint32_t Value;
	uint8_t ColorType;
	uint32_t State;	
		
	switch (BSP_DEV_GET_STATE(&sfp[BSP_SFP_LINE_IDX(Idx)]))
	{
		case BSP_DEV_STATE_RESET:
			Value = 0;
			ColorType = PROFILE_COLOR_TYPE_FAILURE;
			PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);	
			break;
		case BSP_DEV_STATE_INIT:
			Value = 2;
			ColorType = PROFILE_COLOR_TYPE_NORMAL;
			PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);	
			break;
		case BSP_DEV_STATE_OK:
			Value = 1;
			ColorType = PROFILE_COLOR_TYPE_NORMAL;
			PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);	
			/* todo: Add check failure states */
			break;
		case BSP_DEV_STATE_FAIL:
		default:
			Value = 3;
			ColorType = PROFILE_COLOR_TYPE_FAILURE;
			PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);	
			break;
	}
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnSFPStateIdx[Idx], &Value, ColorType, State);
}

static void PROFILE_ADM_LINE_UpdateSFPTempValue(PROFILE_T *Profile, uint32_t Idx)
{
	float Value;
	uint8_t ColorType;
	uint32_t State;	
	
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
		
	switch (BSP_DEV_GET_STATE(&sfp[BSP_SFP_LINE_IDX(Idx)]))
	{
		case BSP_DEV_STATE_OK:
			Value = sfp[BSP_SFP_LINE_IDX(Idx)].Temp;
			ColorType = PROFILE_COLOR_TYPE_AUTO;
			PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
			break;
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
		default:
			Value = 0.0;
			ColorType = PROFILE_COLOR_TYPE_AUTO;
			State = PROFILE_STATE_OOS;
			break;		
	}
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnSFPTempIdx[Idx], &Value, ColorType, State);
}

static void PROFILE_ADM_LINE_UpdateSFPRxPwrValue(PROFILE_T *Profile, uint32_t Idx)
{
	float Value;
	uint32_t State;
	uint8_t ColorType;
	
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
	
	switch (BSP_DEV_GET_STATE(&sfp[BSP_SFP_LINE_IDX(Idx)]))
	{
		case BSP_DEV_STATE_OK:
			Value = sfp[BSP_SFP_LINE_IDX(Idx)].RxPwr;
			ColorType = PROFILE_COLOR_TYPE_AUTO;
			PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
			break;
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
		default:
			Value = -40.0f;
			ColorType = PROFILE_COLOR_TYPE_AUTO;
			State = PROFILE_STATE_OOS;
			break;
	}
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnSFPRxPwrIdx[Idx], &Value, ColorType, State);
}

static void PROFILE_ADM_LINE_UpdateSFPTxPwrValue(PROFILE_T *Profile, uint32_t Idx)
{
	float Value;
	uint32_t State;
	uint8_t ColorType;
	
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
	
	switch (BSP_DEV_GET_STATE(&sfp[BSP_SFP_LINE_IDX(Idx)]))
	{
		case BSP_DEV_STATE_OK:
			Value = sfp[BSP_SFP_LINE_IDX(Idx)].TxPwr;
			ColorType = PROFILE_COLOR_TYPE_AUTO;
			PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
			break;
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
		default:
			Value = -40.0f;
			ColorType = PROFILE_COLOR_TYPE_AUTO;
			State = PROFILE_STATE_OOS;
			break;
	}
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnSFPTxPwrIdx[Idx], &Value, ColorType, State);
}

static void PROFILE_ADM_LINE_UpdateOTU2AlarmsValue(PROFILE_T *Profile, uint32_t Idx)
{
	uint32_t OTU2Alarms;
	uint32_t State, DataType;
	uint8_t ColorType;
	
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetDataTypeIdx[Idx], &DataType);	
	
	if (DataType == 1)
	{
		OTU2Alarms = 0;
		ColorType = PROFILE_COLOR_TYPE_NORMAL;
		PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
		
		if ((BSP_DEV_GET_STATE(&fpga_line) != BSP_DEV_STATE_OK) || \
			(BSP_DEV_GET_STATE(&sfp[BSP_SFP_LINE_IDX(Idx)]) != BSP_DEV_STATE_OK) || \
			sfp[BSP_SFP_LINE_IDX(Idx)].Pin.RxLos || \
			(fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_LOS_MASK))
		{
			OTU2Alarms = 1 << 0;
			ColorType = PROFILE_COLOR_TYPE_FAILURE;
		}
		else if (fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_LOF_MASK)
		{
			OTU2Alarms = 1 << 1;
			ColorType = PROFILE_COLOR_TYPE_FAILURE;
		}
		else if (fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_LOM_MASK)
		{
			OTU2Alarms = 1 << 2;
			ColorType = PROFILE_COLOR_TYPE_FAILURE;
		}
		else
		{
			if (fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_SMBIP8_MASK)
			{
				OTU2Alarms |= 1 << 3;
				ColorType = PROFILE_COLOR_TYPE_DEGRADE;
			}
			if (fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_SMIAE_MASK)
			{
				OTU2Alarms |= 1 << 4;
				ColorType = PROFILE_COLOR_TYPE_DEGRADE;
			}
			if (fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_SMBEI_MASK)
			{
				OTU2Alarms |= 1 << 5;
				ColorType = PROFILE_COLOR_TYPE_DEGRADE;
			}
			if (fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_SMBDI_MASK)
			{
				OTU2Alarms |= 1 << 6;
				ColorType = PROFILE_COLOR_TYPE_DEGRADE;
			}
		}			
	}
	else
	{
		OTU2Alarms = 0;
		ColorType = PROFILE_COLOR_TYPE_NORMAL;
		State = PROFILE_STATE_OOS;
	}

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnOTU2AlarmsIdx[Idx], &OTU2Alarms, ColorType, State);
}

static void PROFILE_ADM_LINE_UpdateOTU2FECBERValue(PROFILE_T *Profile, uint32_t Idx)
{
	float OTU2FECBER;
	uint32_t State, DataType;
	uint8_t ColorType;
	
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetDataTypeIdx[Idx], &DataType);	
	
	if (DataType == 1)
	{
		OTU2FECBER = 0;
		ColorType = PROFILE_COLOR_TYPE_NO;
		PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
		
		if (BSP_DEV_GET_STATE(&fpga_line) == BSP_DEV_STATE_OK)
		{
			/*todo: Get FEC BER*/
		}	
	}
	else
	{
		OTU2FECBER = 0;
		ColorType = PROFILE_COLOR_TYPE_NO;
		State = PROFILE_STATE_OOS;
	}

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnOTU2FECBERIdx[Idx], &OTU2FECBER, ColorType, State);
}

static void PROFILE_ADM_LINE_UpdateODU2AlarmsValue(PROFILE_T *Profile, uint32_t Idx)
{
	uint32_t ODU2Alarms;
	uint32_t State, DataType;
	uint8_t ColorType;
	
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetDataTypeIdx[Idx], &DataType);	

	if (DataType == 1)
	{
		ODU2Alarms = 0;
		ColorType = PROFILE_COLOR_TYPE_NORMAL;
		PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
		
		if (BSP_DEV_GET_STATE(&fpga_line) == BSP_DEV_STATE_OK)
		{
			if (fpga_line.Iface[Idx].ODU2Alarms & BSP_FPGA_LINE_IFACE_ODU2_ALARM_AIS_MASK)
			{
				ODU2Alarms = 1 << 0;
				ColorType = PROFILE_COLOR_TYPE_FAILURE;
			}
			else
			{
				if (fpga_line.Iface[Idx].ODU2Alarms & BSP_FPGA_LINE_IFACE_ODU2_ALARM_PMBIP8_MASK)
				{
					ODU2Alarms = 1 << 3;
					ColorType = PROFILE_COLOR_TYPE_DEGRADE;
				}
				if (fpga_line.Iface[Idx].ODU2Alarms & BSP_FPGA_LINE_IFACE_ODU2_ALARM_PMBEI_MASK)
				{
					ODU2Alarms = 1 << 4;
					ColorType = PROFILE_COLOR_TYPE_DEGRADE;
				}
				if (fpga_line.Iface[Idx].ODU2Alarms & BSP_FPGA_LINE_IFACE_ODU2_ALARM_PMBDI_MASK)
				{
					ODU2Alarms = 1 << 5;
					ColorType = PROFILE_COLOR_TYPE_DEGRADE;
				}
			}				
		}
	}
	else
	{
		ODU2Alarms = 0;
		ColorType = PROFILE_COLOR_TYPE_NORMAL;
		State = PROFILE_STATE_OOS;
	}

	PROFILE_UpdateParam(Profile, PROFILE_TABLE_2, ProfileADMDynamicLnODU2AlarmsIdx[Idx], &ODU2Alarms, ColorType, State);
}

/*
static void PROFILE_ADM_LINE_UpdateALSValue(PROFILE_T *Profile, uint32_t Idx)
{
	uint32_t ALSMode;
	uint32_t ALSState;
	uint32_t State;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetALSModeIdx[Idx] - 1], &ALSMode);
		
	switch (ALSMode)
	{
		case 1:
			if (LnALSLn[Idx].Status == ALS_STATUS_DISABLED)
				ALSState = 0;
			else if (LnALSLn[Idx].Status == ALS_STATUS_PASSIVE)
				ALSState = 1;
			else
				ALSState = 2;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnALSStateIdx[Idx] - 1], &ALSState);
			break;
		case 2:
			if (LnALSLn[Idx].Status == ALS_STATUS_DISABLED)
				ALSState = 0;
			else if (LnALSLn[Idx].Status == ALS_STATUS_PASSIVE)
				ALSState = 1;
			else
				ALSState = 2;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnALSStateIdx[Idx] - 1], &ALSState);
			break;
		default:
			ALSState = 0;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnALSStateIdx[Idx] - 1], &ALSState);
			break;		
	}
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[Idx].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnALSStateIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnALSStateIdx[Idx] - 1], State);
			break;		
	}
}
*/
static void PROFILE_ADM_LINE_UpdateDynamicValues(PROFILE_T *Profile, uint32_t Idx)
{
	PROFILE_ADM_LINE_UpdateSFPStateValue(Profile, Idx);
	PROFILE_ADM_LINE_UpdateSFPTempValue(Profile, Idx);
	PROFILE_ADM_LINE_UpdateSFPRxPwrValue(Profile, Idx);
	PROFILE_ADM_LINE_UpdateSFPTxPwrValue(Profile, Idx);
	PROFILE_ADM_LINE_UpdateOTU2AlarmsValue(Profile, Idx);
	PROFILE_ADM_LINE_UpdateOTU2FECBERValue(Profile, Idx);
	PROFILE_ADM_LINE_UpdateODU2AlarmsValue(Profile, Idx);
	
	//PROFILE_ADM_LINE_UpdateALSValue(Profile, Idx);
}

static void PROFILE_ADM_LINE_UpdateOTU2PerfValues(PROFILE_T *Profile, uint32_t Idx)
{
	uint32_t Errors, Defects;
	uint32_t State, DataType;
	
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetDataTypeIdx[Idx], &DataType);
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
	
	if (DataType == 1)
	{
		if ((BSP_DEV_GET_STATE(&fpga_line) != BSP_DEV_STATE_OK) || \
			(BSP_DEV_GET_STATE(&sfp[BSP_SFP_LINE_IDX(Idx)]) != BSP_DEV_STATE_OK) || \
			(sfp[BSP_SFP_LINE_IDX(Idx)].Pin.RxLos) || \
			(fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_LOS_MASK) || \
			(fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_LOF_MASK) || \
			(fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_LOM_MASK))
		{
			Defects = 1;
			Errors = 0;
		}
		else
		{
			if (fpga_line.Iface[Idx].OTU2BIP )
			{
				Defects = 0;
				Errors = fpga_line.Iface[Idx].OTU2BIP;
			}
			else
			{
				Defects = 0;
				Errors = 0;
			}
		}
	}
	else
	{
		Defects = 0;
		Errors = 0;
	}
	
	PERF_Update(&LnOTU2Perf[Idx], Defects, Errors, PERF_ODU2_SES_ERRORS_THR);
	
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_8, ProfileADMPerfLnOTU2EBIdx[Idx], &LnOTU2Perf[Idx].EB, PROFILE_COLOR_TYPE_NO, State);
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_8, ProfileADMPerfLnOTU2ESIdx[Idx], &LnOTU2Perf[Idx].ES, PROFILE_COLOR_TYPE_NO, State);
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_8, ProfileADMPerfLnOTU2SESIdx[Idx], &LnOTU2Perf[Idx].SES, PROFILE_COLOR_TYPE_NO, State);
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_8, ProfileADMPerfLnOTU2BBEIdx[Idx], &LnOTU2Perf[Idx].BBE, PROFILE_COLOR_TYPE_NO, State);
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_8, ProfileADMPerfLnOTU2UASIdx[Idx], &LnOTU2Perf[Idx].UAS, PROFILE_COLOR_TYPE_NO, State);
}

static void PROFILE_ADM_LINE_UpdateODU2PerfValues(PROFILE_T *Profile, uint32_t Idx)
{
	uint32_t Errors, Defects;
	uint32_t State, DataType;
	
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetDataTypeIdx[Idx], &DataType);
	PROFILE_GetValue(Profile, PROFILE_TABLE_3, ProfileADMConfigLnSetStateIdx[Idx], &State);
	
	if (DataType == 1)
	{
		if ((BSP_DEV_GET_STATE(&fpga_line) != BSP_DEV_STATE_OK) || \
			(BSP_DEV_GET_STATE(&sfp[BSP_SFP_LINE_IDX(Idx)]) != BSP_DEV_STATE_OK) || \
			(sfp[BSP_SFP_LINE_IDX(Idx)].Pin.RxLos) || \
			(fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_LOS_MASK) || \
			(fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_LOF_MASK) || \
			(fpga_line.Iface[Idx].OTU2Alarms & BSP_FPGA_LINE_IFACE_OTU2_ALARM_LOM_MASK) || \
			(fpga_line.Iface[Idx].ODU2Alarms & BSP_FPGA_LINE_IFACE_ODU2_ALARM_AIS_MASK))
		{
			Defects = 1;
			Errors = 0;
		}
		else
		{
			if (fpga_line.Iface[Idx].ODU2BIP )
			{
				Defects = 0;
				Errors = fpga_line.Iface[Idx].ODU2BIP;
			}
			else
			{
				Defects = 0;
				Errors = 0;
			}
		}
	}
	else
	{
		Defects = 0;
		Errors = 0;
	}
	
	PERF_Update(&LnODU2Perf[Idx], Defects, Errors, PERF_ODU2_SES_ERRORS_THR);
	
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_8, ProfileADMPerfLnODU2EBIdx[Idx], &LnODU2Perf[Idx].EB, PROFILE_COLOR_TYPE_NO, State);
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_8, ProfileADMPerfLnODU2ESIdx[Idx], &LnODU2Perf[Idx].ES, PROFILE_COLOR_TYPE_NO, State);
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_8, ProfileADMPerfLnODU2SESIdx[Idx], &LnODU2Perf[Idx].SES, PROFILE_COLOR_TYPE_NO, State);
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_8, ProfileADMPerfLnODU2BBEIdx[Idx], &LnODU2Perf[Idx].BBE, PROFILE_COLOR_TYPE_NO, State);
	PROFILE_UpdateParam(Profile, PROFILE_TABLE_8, ProfileADMPerfLnODU2UASIdx[Idx], &LnODU2Perf[Idx].UAS, PROFILE_COLOR_TYPE_NO, State);
}

static void PROFILE_ADM_LINE_UpdatePerfValues(PROFILE_T *Profile, uint32_t Idx)
{
	PROFILE_ADM_LINE_UpdateOTU2PerfValues(Profile, Idx);
	PROFILE_ADM_LINE_UpdateODU2PerfValues(Profile, Idx);
}

void PROFILE_ADM_LINE_UpdateValues(PROFILE_T *Profile)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM_LINE_PORTS; i++)
	{
		PROFILE_ADM_LINE_UpdateInfoAndStaticValues(Profile, i);
		PROFILE_ADM_LINE_UpdateDynamicValues(Profile, i);
		PROFILE_ADM_LINE_UpdatePerfValues(Profile, i);
	}
}
