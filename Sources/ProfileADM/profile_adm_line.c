#include "profile_adm_line.h"
#include "profile_adm_values.h"
#include "als.h"
#include <string.h>

static const uint32_t ProfileADMDynamicLnRxPwrIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1RXPWR_IDX, PROFILE_ADM_LN2RXPWR_IDX, PROFILE_ADM_LN3RXPWR_IDX, PROFILE_ADM_LN4RXPWR_IDX
};

static const uint32_t ProfileADMDynamicLnTxPwrIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1TXPWR_IDX, PROFILE_ADM_LN2TXPWR_IDX, PROFILE_ADM_LN3TXPWR_IDX, PROFILE_ADM_LN4TXPWR_IDX
};

static const uint32_t ProfileADMDynamicLnOTUAlarmsIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1OTUALARMS_IDX, PROFILE_ADM_LN2OTUALARMS_IDX, PROFILE_ADM_LN3OTUALARMS_IDX, PROFILE_ADM_LN4OTUALARMS_IDX
};
/*
static const uint32_t ProfileADMDynamicLnODUAlarmsIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ODUALARMS_IDX, PROFILE_ADM_LN2ODUALARMS_IDX, PROFILE_ADM_LN3ODUALARMS_IDX, PROFILE_ADM_LN4ODUALARMS_IDX
};

static const uint32_t ProfileADMDynamicLnPreFECBERIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1PREFECBER_IDX, PROFILE_ADM_LN2PREFECBER_IDX, PROFILE_ADM_LN3PREFECBER_IDX, PROFILE_ADM_LN4PREFECBER_IDX
	PROFILE_ADM_LN16PREFECBER_IDX
};

static const uint32_t ProfileADMDynamicLnPostFECBERIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1POSTFECBER_IDX, PROFILE_ADM_LN2POSTFECBER_IDX, PROFILE_ADM_LN3POSTFECBER_IDX, PROFILE_ADM_LN4POSTFECBER_IDX
};

static const uint32_t ProfileADMDynamicLnFECLoadIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1FECLOAD_IDX, PROFILE_ADM_LN2FECLOAD_IDX, PROFILE_ADM_LN3FECLOAD_IDX, PROFILE_ADM_LN4FECLOAD_IDX
};
*/
static const uint32_t ProfileADMDynamicLnSFPStateIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SFPSTATE_IDX, PROFILE_ADM_LN2SFPSTATE_IDX, PROFILE_ADM_LN3SFPSTATE_IDX, PROFILE_ADM_LN4SFPSTATE_IDX
};

static const uint32_t ProfileADMDynamicLnSFPTempIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SFPTEMP_IDX, PROFILE_ADM_LN2SFPTEMP_IDX, PROFILE_ADM_LN3SFPTEMP_IDX, PROFILE_ADM_LN4SFPTEMP_IDX
};
/*
static const uint32_t ProfileADMDynamicLnALSStateIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ALSSTATE_IDX, PROFILE_ADM_LN2ALSSTATE_IDX, PROFILE_ADM_LN3ALSSTATE_IDX, PROFILE_ADM_LN4ALSSTATE_IDX
	PROFILE_ADM_LN16ALSSTATE_IDX
};
*/
static const uint32_t ProfileADMInfoLnDestinationIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1DESTINATION_IDX, PROFILE_ADM_LN2DESTINATION_IDX, PROFILE_ADM_LN3DESTINATION_IDX, PROFILE_ADM_LN4DESTINATION_IDX
};

static const uint32_t ProfileADMInfoLnSFPVendorIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SFPVENDOR_IDX, PROFILE_ADM_LN2SFPVENDOR_IDX, PROFILE_ADM_LN3SFPVENDOR_IDX, PROFILE_ADM_LN4SFPVENDOR_IDX
};

static const uint32_t ProfileADMInfoLnSFPSrNumberIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SFPSRNUMBER_IDX, PROFILE_ADM_LN2SFPSRNUMBER_IDX, PROFILE_ADM_LN3SFPSRNUMBER_IDX, PROFILE_ADM_LN4SFPSRNUMBER_IDX
};

static const uint32_t ProfileADMInfoLnSFPPtNumberIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SFPPTNUMBER_IDX, PROFILE_ADM_LN2SFPPTNUMBER_IDX, PROFILE_ADM_LN3SFPPTNUMBER_IDX, PROFILE_ADM_LN4SFPPTNUMBER_IDX
};

static const uint32_t ProfileADMStaticLnRxPwrIdx[PROFILE_ADM_LINE_PORTS][4] = 
{
	{PROFILE_ADM_LN1RXPWRCMIN_IDX, PROFILE_ADM_LN1RXPWRWMIN_IDX, PROFILE_ADM_LN1RXPWRWMAX_IDX, PROFILE_ADM_LN1RXPWRCMAX_IDX},
	{PROFILE_ADM_LN2RXPWRCMIN_IDX, PROFILE_ADM_LN2RXPWRWMIN_IDX, PROFILE_ADM_LN2RXPWRWMAX_IDX, PROFILE_ADM_LN2RXPWRCMAX_IDX},
	{PROFILE_ADM_LN3RXPWRCMIN_IDX, PROFILE_ADM_LN3RXPWRWMIN_IDX, PROFILE_ADM_LN3RXPWRWMAX_IDX, PROFILE_ADM_LN3RXPWRCMAX_IDX},
	{PROFILE_ADM_LN4RXPWRCMIN_IDX, PROFILE_ADM_LN4RXPWRWMIN_IDX, PROFILE_ADM_LN4RXPWRWMAX_IDX, PROFILE_ADM_LN4RXPWRCMAX_IDX}
};

static const uint32_t ProfileADMStaticLnTxPwrIdx[PROFILE_ADM_LINE_PORTS][4] = 
{
	{PROFILE_ADM_LN1TXPWRCMIN_IDX, PROFILE_ADM_LN1TXPWRWMIN_IDX, PROFILE_ADM_LN1TXPWRWMAX_IDX, PROFILE_ADM_LN1TXPWRCMAX_IDX},
	{PROFILE_ADM_LN2TXPWRCMIN_IDX, PROFILE_ADM_LN2TXPWRWMIN_IDX, PROFILE_ADM_LN2TXPWRWMAX_IDX, PROFILE_ADM_LN2TXPWRCMAX_IDX},
	{PROFILE_ADM_LN3TXPWRCMIN_IDX, PROFILE_ADM_LN3TXPWRWMIN_IDX, PROFILE_ADM_LN3TXPWRWMAX_IDX, PROFILE_ADM_LN3TXPWRCMAX_IDX},
	{PROFILE_ADM_LN4TXPWRCMIN_IDX, PROFILE_ADM_LN4TXPWRWMIN_IDX, PROFILE_ADM_LN4TXPWRWMAX_IDX, PROFILE_ADM_LN4TXPWRCMAX_IDX}
};

static const uint32_t ProfileADMStaticLnSFPTempIdx[PROFILE_ADM_LINE_PORTS][4] = 
{
	{PROFILE_ADM_LN1SFPTEMPCMIN_IDX, PROFILE_ADM_LN1SFPTEMPWMIN_IDX, PROFILE_ADM_LN1SFPTEMPWMAX_IDX, PROFILE_ADM_LN1SFPTEMPCMAX_IDX},
	{PROFILE_ADM_LN2SFPTEMPCMIN_IDX, PROFILE_ADM_LN2SFPTEMPWMIN_IDX, PROFILE_ADM_LN2SFPTEMPWMAX_IDX, PROFILE_ADM_LN2SFPTEMPCMAX_IDX},
	{PROFILE_ADM_LN3SFPTEMPCMIN_IDX, PROFILE_ADM_LN3SFPTEMPWMIN_IDX, PROFILE_ADM_LN3SFPTEMPWMAX_IDX, PROFILE_ADM_LN3SFPTEMPCMAX_IDX},
	{PROFILE_ADM_LN4SFPTEMPCMIN_IDX, PROFILE_ADM_LN4SFPTEMPWMIN_IDX, PROFILE_ADM_LN4SFPTEMPWMAX_IDX, PROFILE_ADM_LN4SFPTEMPCMAX_IDX}
};

static const uint32_t ProfileADMConfigLnSetStateIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SETSTATE_IDX, PROFILE_ADM_LN2SETSTATE_IDX, PROFILE_ADM_LN3SETSTATE_IDX, PROFILE_ADM_LN4SETSTATE_IDX
};

static const uint32_t ProfileADMConfigLnEnableTxIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ENABLETX_IDX, PROFILE_ADM_LN2ENABLETX_IDX, PROFILE_ADM_LN3ENABLETX_IDX, PROFILE_ADM_LN4ENABLETX_IDX
};
/*
static const uint32_t ProfileADMConfigLnSetLoopbackIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SETLOOPBACK_IDX, PROFILE_ADM_LN2SETLOOPBACK_IDX, PROFILE_ADM_LN3SETLOOPBACK_IDX, PROFILE_ADM_LN4SETLOOPBACK_IDX
};

static const uint32_t ProfileADMConfigLnSetDataTypeIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SETDATATYPE_IDX, PROFILE_ADM_LN2SETDATATYPE_IDX, PROFILE_ADM_LN3SETDATATYPE_IDX, PROFILE_ADM_LN4SETDATATYPE_IDX
};

static const uint32_t ProfileADMConfigLnSetFECTypeIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SETFECTYPE_IDX, PROFILE_ADM_LN2SETFECTYPE_IDX, PROFILE_ADM_LN3SETFECTYPE_IDX, PROFILE_ADM_LN4SETFECTYPE_IDX
};

static const uint32_t ProfileADMConfigLnSetALSModeIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SETALSMODE_IDX, PROFILE_ADM_LN2SETALSMODE_IDX, PROFILE_ADM_LN3SETALSMODE_IDX, PROFILE_ADM_LN4SETALSMODE_IDX
};
*/
static const uint32_t ProfileADMConfigLnSetFactoryIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SETFACTORY_IDX, PROFILE_ADM_LN2SETFACTORY_IDX, PROFILE_ADM_LN3SETFACTORY_IDX, PROFILE_ADM_LN4SETFACTORY_IDX
};
/*
static const uint32_t ProfileADMPerfLnOTUEBIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1OTUEB_IDX, PROFILE_ADM_LN2OTUEB_IDX, PROFILE_ADM_LN3OTUEB_IDX, PROFILE_ADM_LN4OTUEB_IDX
};

static const uint32_t ProfileADMPerfLnOTUESIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1OTUES_IDX, PROFILE_ADM_LN2OTUES_IDX, PROFILE_ADM_LN3OTUES_IDX, PROFILE_ADM_LN4OTUES_IDX
};

static const uint32_t ProfileADMPerfLnOTUSESIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1OTUSES_IDX, PROFILE_ADM_LN2OTUSES_IDX, PROFILE_ADM_LN3OTUSES_IDX, PROFILE_ADM_LN4OTUSES_IDX
};

static const uint32_t ProfileADMPerfLnOTUBBEIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1OTUBBE_IDX, PROFILE_ADM_LN2OTUBBE_IDX, PROFILE_ADM_LN3OTUBBE_IDX, PROFILE_ADM_LN4OTUBBE_IDX
};

static const uint32_t ProfileADMPerfLnOTUUASIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1OTUUAS_IDX, PROFILE_ADM_LN2OTUUAS_IDX, PROFILE_ADM_LN3OTUUAS_IDX, PROFILE_ADM_LN4OTUUAS_IDX
};

static const uint32_t ProfileADMPerfLnODUEBIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ODUEB_IDX, PROFILE_ADM_LN2ODUEB_IDX, PROFILE_ADM_LN3ODUEB_IDX, PROFILE_ADM_LN4ODUEB_IDX
};

static const uint32_t ProfileADMPerfLnODUESIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ODUES_IDX, PROFILE_ADM_LN2ODUES_IDX, PROFILE_ADM_LN3ODUES_IDX, PROFILE_ADM_LN4ODUES_IDX
};

static const uint32_t ProfileADMPerfLnODUSESIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ODUSES_IDX, PROFILE_ADM_LN2ODUSES_IDX, PROFILE_ADM_LN3ODUSES_IDX, PROFILE_ADM_LN4ODUSES_IDX
};

static const uint32_t ProfileADMPerfLnODUBBEIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ODUBBE_IDX, PROFILE_ADM_LN2ODUBBE_IDX, PROFILE_ADM_LN3ODUBBE_IDX, PROFILE_ADM_LN4ODUBBE_IDX
};

static const uint32_t ProfileADMPerfLnODUUASIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ODUUAS_IDX, PROFILE_ADM_LN2ODUUAS_IDX, PROFILE_ADM_LN3ODUUAS_IDX, PROFILE_ADM_LN4ODUUAS_IDX
};

static const uint32_t ProfileADMPerfLnSDHRSEBIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SDHRSEB_IDX, PROFILE_ADM_LN2SDHRSEB_IDX, PROFILE_ADM_LN3SDHRSEB_IDX, PROFILE_ADM_LN4SDHRSEB_IDX
};

static const uint32_t ProfileADMPerfLnSDHRSESIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SDHRSES_IDX, PROFILE_ADM_LN2SDHRSES_IDX, PROFILE_ADM_LN3SDHRSES_IDX, PROFILE_ADM_LN4SDHRSES_IDX
};

static const uint32_t ProfileADMPerfLnSDHRSSESIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SDHRSSES_IDX, PROFILE_ADM_LN2SDHRSSES_IDX, PROFILE_ADM_LN3SDHRSSES_IDX, PROFILE_ADM_LN4SDHRSSES_IDX
};

static const uint32_t ProfileADMPerfLnSDHRSBBEIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SDHRSBBE_IDX, PROFILE_ADM_LN2SDHRSBBE_IDX, PROFILE_ADM_LN3SDHRSBBE_IDX, PROFILE_ADM_LN4SDHRSBBE_IDX
};

static const uint32_t ProfileADMPerfLnSDHRSUASIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SDHRSUAS_IDX, PROFILE_ADM_LN2SDHRSUAS_IDX, PROFILE_ADM_LN3SDHRSUAS_IDX, PROFILE_ADM_LN4SDHRSUAS_IDX
};

static const uint32_t ProfileADMPerfLnSDHMSEBIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SDHMSEB_IDX, PROFILE_ADM_LN2SDHMSEB_IDX, PROFILE_ADM_LN3SDHMSEB_IDX, PROFILE_ADM_LN4SDHMSEB_IDX
};

static const uint32_t ProfileADMPerfLnSDHMSESIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SDHMSES_IDX, PROFILE_ADM_LN2SDHMSES_IDX, PROFILE_ADM_LN3SDHMSES_IDX, PROFILE_ADM_LN4SDHMSES_IDX
};

static const uint32_t ProfileADMPerfLnSDHMSSESIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SDHMSSES_IDX, PROFILE_ADM_LN2SDHMSSES_IDX, PROFILE_ADM_LN3SDHMSSES_IDX, PROFILE_ADM_LN4SDHMSSES_IDX
};

static const uint32_t ProfileADMPerfLnSDHMSBBEIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SDHMSBBE_IDX, PROFILE_ADM_LN2SDHMSBBE_IDX, PROFILE_ADM_LN3SDHMSBBE_IDX, PROFILE_ADM_LN4SDHMSBBE_IDX
};

static const uint32_t ProfileADMPerfLnSDHMSUASIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1SDHMSUAS_IDX, PROFILE_ADM_LN2SDHMSUAS_IDX, PROFILE_ADM_LN3SDHMSUAS_IDX, PROFILE_ADM_LN4SDHMSUAS_IDX
};

static const uint32_t ProfileADMPerfLnEthRxPktsIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ETHRXPKTS_IDX, PROFILE_ADM_LN2ETHRXPKTS_IDX, PROFILE_ADM_LN3ETHRXPKTS_IDX, PROFILE_ADM_LN4ETHRXPKTS_IDX
};

static const uint32_t ProfileADMPerfLnEthRxOctsIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ETHRXOCTS_IDX, PROFILE_ADM_LN2ETHRXOCTS_IDX, PROFILE_ADM_LN3ETHRXOCTS_IDX, PROFILE_ADM_LN4ETHRXOCTS_IDX
};

static const uint32_t ProfileADMPerfLnEthRxErrsIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ETHRXERRS_IDX, PROFILE_ADM_LN2ETHRXERRS_IDX, PROFILE_ADM_LN3ETHRXERRS_IDX, PROFILE_ADM_LN4ETHRXERRS_IDX
};

static const uint32_t ProfileADMPerfLnEthTxPktsIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ETHTXPKTS_IDX, PROFILE_ADM_LN2ETHTXPKTS_IDX, PROFILE_ADM_LN3ETHTXPKTS_IDX, PROFILE_ADM_LN4ETHTXPKTS_IDX
};

static const uint32_t ProfileADMPerfLnEthTxOctsIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ETHTXOCTS_IDX, PROFILE_ADM_LN2ETHTXOCTS_IDX, PROFILE_ADM_LN3ETHTXOCTS_IDX, PROFILE_ADM_LN4ETHTXOCTS_IDX
};

static const uint32_t ProfileADMPerfLnEthTxErrsIdx[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_LN1ETHTXERRS_IDX, PROFILE_ADM_LN2ETHTXERRS_IDX, PROFILE_ADM_LN3ETHTXERRS_IDX, PROFILE_ADM_LN4ETHTXERRS_IDX
};
*/

static const uint32_t ProfileADMPortLnInfo[PROFILE_ADM_LINE_PORTS] = 
{ 
	PROFILE_ADM_PTLINE1INFO_IDX, PROFILE_ADM_PTLINE2INFO_IDX, PROFILE_ADM_PTLINE3INFO_IDX, PROFILE_ADM_PTLINE4INFO_IDX
};
static const uint32_t ProfileADMPortLnCat[PROFILE_ADM_LINE_PORTS] = 
{
	PROFILE_ADM_PTLINE1CAT_IDX, PROFILE_ADM_PTLINE2CAT_IDX, PROFILE_ADM_PTLINE3CAT_IDX, PROFILE_ADM_PTLINE4CAT_IDX
};

//static ALS_T LnALSLn[PROFILE_ADM_LINE_PORTS];
//static ALS_T LnALSLn[PROFILE_ADM_LINE_PORTS];

static int32_t PROFILE_ADM_LINE_CheckLnSetStateValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value, uint32_t Idx)
{
	uint32_t SetState;
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	/* Do not alow to go from OOS to IS */
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &SetState);
	if ((SetState == PROFILE_STATE_OOS) && (*(uint32_t*)Value == PROFILE_STATE_IS))
		return -1;
	return 0;
}

static int32_t PROFILE_ADM_LINE_CheckLnEnableTxValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value, uint32_t Idx)
{
	uint32_t SetState;
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	/* Do not alow to enable tx if set state if OOS */
	if (*(uint32_t*)Value == 1)
	{
		PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &SetState);
		if (SetState == PROFILE_STATE_OOS)
			return -1;
	}
	return 0;
}
/*
static int32_t PROFILE_ADM_LINE_CheckLnSetLoopbackValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value, uint32_t Idx)
{
	return 0;
}

static int32_t PROFILE_ADM_LINE_CheckLnSetDataTypeValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value, uint32_t Idx)
{
	return 0;
}

static int32_t PROFILE_ADM_LINE_CheckLnSetFECTypeValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value, uint32_t Idx)
{
	return 0;
}
*/
int32_t PROFILE_ADM_LINE_CheckConfigValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM_LINE_PORTS; i++)
	{
		if (Param->Index == ProfileADMConfigLnSetStateIdx[i])
		{
			if (PROFILE_ADM_LINE_CheckLnSetStateValue(Profile, Devices, Param, Value, i))
				return -1;
			return 0;
		}
		if (Param->Index == ProfileADMConfigLnEnableTxIdx[i])
		{
			if (PROFILE_ADM_LINE_CheckLnEnableTxValue(Profile, Devices, Param, Value, i))
				return -1;
			return 0;
		}
		/*
		if (Param->Index == ProfileADMConfigLnSetLoopbackIdx[i])
		{
			if (PROFILE_ADM_LINE_CheckLnSetLoopbackValue(Profile, Devices, Param, Value, i))
				return -1;
			return 0;
		}
		if (Param->Index == ProfileADMConfigLnSetDataTypeIdx[i])
		{
			if (PROFILE_ADM_LINE_CheckLnSetDataTypeValue(Profile, Devices, Param, Value, i))
				return -1;
			return 0;
		}
		if (Param->Index == ProfileADMConfigLnSetFECTypeIdx[i])
		{
			if (PROFILE_ADM_LINE_CheckLnSetFECTypeValue(Profile, Devices, Param, Value, i))
				return -1;
			return 0;
		}
		*/
	}
	return 0;
}

static int32_t PROFILE_ADM_LINE_SetLnSetStateValueDependencies(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value, uint32_t Idx)
{
	uint32_t EnableTx;
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	
	/* Disable tx when state is OOS*/
	if (*(uint32_t*)Value == PROFILE_STATE_OOS)
	{
		EnableTx = 0;
		PROFILE_SetValue(Profile, &ConfigParams[ProfileADMConfigLnEnableTxIdx[Idx] - 1], &EnableTx);
	}
	return 0;
}

static int32_t PROFILE_ADM_LINE_SetLnEnableTxValueDependencies(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value, uint32_t Idx)
{
	return 0;
}
/*
static int32_t PROFILE_ADM_LINE_SetLnSetLoopbackValueDependencies(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value, uint32_t Idx)
{
	return 0;
}

static int32_t PROFILE_ADM_LINE_SetLnSetDataTypeValueDependencies(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value, uint32_t Idx)
{
	return 0;
}

static int32_t PROFILE_ADM_LINE_SetLnSetFECTypeValueDependencies(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value, uint32_t Idx)
{
	return 0;
}
*/
static int32_t PROFILE_ADM_LINE_SetLnSetFactoryValueDependencies(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value, uint32_t Idx)
{
	uint32_t i;
	PROFILE_PARAM_T	*InfoParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_1];
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	PROFILE_PARAM_T	*StaticParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_4];
	PROFILE_PARAM_T	*PortParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_103];
	
	if ((*(uint32_t*)Value == 0) || (*(uint32_t*)Value == 1))
	{
		/* Set all info client parametes to factory*/
		PROFILE_SetFactoryValue(Profile, &InfoParams[ProfileADMInfoLnDestinationIdx[Idx] - 1]);
		PROFILE_SetFactoryValue(Profile, &InfoParams[ProfileADMInfoLnSFPVendorIdx[Idx] - 1]);
		PROFILE_SetFactoryValue(Profile, &InfoParams[ProfileADMInfoLnSFPSrNumberIdx[Idx] - 1]);
		PROFILE_SetFactoryValue(Profile, &InfoParams[ProfileADMInfoLnSFPPtNumberIdx[Idx] - 1]);
	}
	if ((*(uint32_t*)Value == 0) || (*(uint32_t*)Value == 2))
	{
		/* Set all config client parametes to factory*/
		PROFILE_SetFactoryValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1]);
		PROFILE_SetFactoryValue(Profile, &ConfigParams[ProfileADMConfigLnEnableTxIdx[Idx] - 1]);
		//PROFILE_SetFactoryValue(Profile, &ConfigParams[ProfileADMConfigLnSetLoopbackIdx[Idx] - 1]);
		//PROFILE_SetFactoryValue(Profile, &ConfigParams[ProfileADMConfigLnSetDataTypeIdx[Idx] - 1]);
		//PROFILE_SetFactoryValue(Profile, &ConfigParams[ProfileADMConfigLnSetALSModeIdx[Idx] - 1]);
	}
	if ((*(uint32_t*)Value == 0) || (*(uint32_t*)Value == 3))
	{
		/* Set all threshoulds client parametes to factory*/
		for (i = 0; i < 4; i++)
		{
			PROFILE_SetFactoryValue(Profile, &StaticParams[ProfileADMStaticLnRxPwrIdx[Idx][i] - 1]);
			PROFILE_SetFactoryValue(Profile, &StaticParams[ProfileADMStaticLnTxPwrIdx[Idx][i] - 1]);
			PROFILE_SetFactoryValue(Profile, &StaticParams[ProfileADMStaticLnSFPTempIdx[Idx][i] - 1]);
		}
	}
	if ((*(uint32_t*)Value == 0) || (*(uint32_t*)Value == 4))
	{
		/* Set all dynamic client parametes colors to factory*/
		PROFILE_SetFactoryAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnRxPwrIdx[Idx] - 1]);
		PROFILE_SetFactoryAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnTxPwrIdx[Idx] - 1]);
		PROFILE_SetFactoryAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnOTUAlarmsIdx[Idx] - 1]);
		//PROFILE_SetFactoryAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnODUAlarmsIdx[Idx] - 1]);
		//PROFILE_SetFactoryAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnSDHAlarmsIdx[Idx] - 1]);
		//PROFILE_SetFactoryAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnEthAlarmsIdx[Idx] - 1]);
		PROFILE_SetFactoryAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1]);
		PROFILE_SetFactoryAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnSFPTempIdx[Idx] - 1]);
	}
	if ((*(uint32_t*)Value == 0) || (*(uint32_t*)Value == 5))
	{
		/* Set all port line parameters to factory */
		PROFILE_SetFactoryValue(Profile, &PortParams[ProfileADMPortLnInfo[Idx] - 1]);
		PROFILE_SetFactoryValue(Profile, &PortParams[ProfileADMPortLnCat[Idx] - 1]);
	}
	return 0;
}

int32_t PROFILE_ADM_LINE_SetConfigValueDependencies(PROFILE_T *Profile, BSP_DEVICES_T *Devices, PROFILE_PARAM_T	*Param, void *Value)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM_LINE_PORTS; i++)
	{
		if (Param->Index == ProfileADMConfigLnSetStateIdx[i])
		{
			if (PROFILE_ADM_LINE_SetLnSetStateValueDependencies(Profile, Devices, Param, Value, i))
				return -1;
			return 0;
		}
		if (Param->Index == ProfileADMConfigLnEnableTxIdx[i])
		{
			if (PROFILE_ADM_LINE_SetLnEnableTxValueDependencies(Profile, Devices, Param, Value, i))
				return -1;
			return 0;
		}
		/*
		if (Param->Index == ProfileADMConfigLnSetLoopbackIdx[i])
		{
			if (PROFILE_ADM_LINE_SetLnSetLoopbackValueDependencies(Profile, Devices, Param, Value, i))
				return -1;
			return 0;
		}
		if (Param->Index == ProfileADMConfigLnSetDataTypeIdx[i])
		{
			if (PROFILE_ADM_LINE_SetLnSetDataTypeValueDependencies(Profile, Devices, Param, Value, i))
				return -1;
			return 0;
		}
		if (Param->Index == ProfileADMConfigLnSetFECTypeIdx[i])
		{
			if (PROFILE_ADM_LINE_SetLnSetFECTypeValueDependencies(Profile, Devices, Param, Value, i))
				return -1;
			return 0;
		}
		*/
		if (Param->Index == ProfileADMConfigLnSetFactoryIdx[i])
		{
			if (PROFILE_ADM_LINE_SetLnSetFactoryValueDependencies(Profile, Devices, Param, Value, i))
				return -1;
			return 0;
		}
	}
	return 0;
}

void PROFILE_ADM_LINE_UpdateConfigs(PROFILE_T *Profile, BSP_DEVICES_T *Devices, BSP_DEVICES_CFG_T *DevicesCfg)
{
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];	
	uint32_t i;
	uint32_t PortState, EnableTx;
	
	for (i = 0; i < PROFILE_ADM_LINE_PORTS; i++)
	{
		PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[i] - 1], &PortState);
		PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnEnableTxIdx[i] - 1], &EnableTx);
		
		DevicesCfg->Sfp[BSP_SFP_LINE_IDX(i)].TxDisable  = EnableTx ? 0 : 1;
		
		/*PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetLoopbackIdx[i] - 1], &SetLoopback);
		PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetDataTypeIdx[i] - 1], &DataType);
		PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetFECTypeIdx[i] - 1], &FECType);
		PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetALSModeIdx[i] - 1], &SetALSMode);
		
		if (SetALSMode == 1)
		{
			ALS_Update(&LnALSLn[i], ALS_STATE_DISABLED, EnableTx, Devices->SFP[i].LOS, 100, 5);
			ALS_Update(&LnALSLn[i], ALS_STATE_ENABLED, EnableTx, Devices->CFP[(i < 10) ? 0 : 1].Instance.Unknown.MSA.RxLos[0], 100, 5);
			DevicesCfg->SFP[i].TxDisable  = LnALSLn[i].TxEnable ? 0 : 1;
		}
		else if (SetALSMode == 2)
		{
			ALS_Update(&LnALSLn[i], ALS_STATE_ENABLED, EnableTx, Devices->SFP[i].LOS, 100, 5);
			ALS_Update(&LnALSLn[i], ALS_STATE_DISABLED, EnableTx, Devices->CFP[(i < 10) ? 0 : 1].Instance.Unknown.MSA.RxLos[0], 100, 5);
			DevicesCfg->SFP[i].TxDisable  = LnALSLn[i].TxEnable ? 0 : 1;
		}
		else 
		{
			ALS_Update(&LnALSLn[i], ALS_STATE_DISABLED, EnableTx, Devices->SFP[i].LOS, 100, 5);
			ALS_Update(&LnALSLn[i], ALS_STATE_DISABLED, EnableTx, Devices->CFP[(i < 10) ? 0 : 1].Instance.Unknown.MSA.RxLos[0], 100, 5);
			DevicesCfg->SFP[i].TxDisable  = EnableTx ? 0 : 1;
		}
		
		BSP_CPU_SetPortState(&Devices->CPU, BSP_CPU_LINE_PORT(i), PortState ? BSP_CPU_PORT_STATE_IS : BSP_CPU_PORT_STATE_OOS);
		
		if (DataType == 258)
			BSP_CPU_SetPortDataType(&Devices->CPU, BSP_CPU_LINE_PORT(i), BSP_CPU_PORT_DATA_TYPE_ETH);
		else if (DataType == 131)
			BSP_CPU_SetPortDataType(&Devices->CPU, BSP_CPU_LINE_PORT(i), BSP_CPU_PORT_DATA_TYPE_SDH);
		else
			BSP_CPU_SetPortDataType(&Devices->CPU, BSP_CPU_LINE_PORT(i), BSP_CPU_PORT_DATA_TYPE_OTN);
		
		BSP_CPU_SetPortLoopback(&Devices->CPU, BSP_CPU_LINE_PORT(i), SetLoopback);
		
		if (FECType == 0)
			BSP_CPU_SetPortOTNFECType(&Devices->CPU, BSP_CPU_LINE_PORT(i), BSP_CPU_PORT_OTN_FEC_TYPE_NO_FEC);
		else if (FECType == 257)
			BSP_CPU_SetPortOTNFECType(&Devices->CPU, BSP_CPU_LINE_PORT(i), BSP_CPU_PORT_OTN_FEC_TYPE_EFEC_I4);
		else if (FECType == 260)
			BSP_CPU_SetPortOTNFECType(&Devices->CPU, BSP_CPU_LINE_PORT(i), BSP_CPU_PORT_OTN_FEC_TYPE_EFEC_I7);
		else
			BSP_CPU_SetPortOTNFECType(&Devices->CPU, BSP_CPU_LINE_PORT(i), BSP_CPU_PORT_OTN_FEC_TYPE_GFEC);
		*/
	}
}

static void PROFILE_ADM_LINE_UpdateInfoAndStaticValues(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	char Tmp[PROFILE_STRING_MAX_LEN];
	uint32_t UpdateFlag;
	uint32_t State;
	PROFILE_PARAM_T	*InfoParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_1];
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	PROFILE_PARAM_T	*StaticParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_4];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	
	switch (Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].ModAbs)
	{
		case 1:
			PROFILE_SetState(Profile, &InfoParams[ProfileADMInfoLnDestinationIdx[Idx] - 1], State);
			PROFILE_SetState(Profile, &InfoParams[ProfileADMInfoLnSFPVendorIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &InfoParams[ProfileADMInfoLnSFPSrNumberIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &InfoParams[ProfileADMInfoLnSFPPtNumberIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case 0:
			//Check if inserted module is same as previous
			UpdateFlag = 1;
			PROFILE_GetValue(Profile, &InfoParams[ProfileADMInfoLnSFPVendorIdx[Idx] - 1], Tmp);
			if (!strncmp(Tmp, Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.Vendor, sizeof(Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.Vendor)))
			{
				PROFILE_GetValue(Profile, &InfoParams[ProfileADMInfoLnSFPSrNumberIdx[Idx] - 1], Tmp);
				if (!strncmp(Tmp, Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.SerialNumber, sizeof(Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.SerialNumber)))
				{
					PROFILE_GetValue(Profile, &InfoParams[ProfileADMInfoLnSFPPtNumberIdx[Idx] - 1], Tmp);
					if (!strncmp(Tmp, Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.PartNumber, sizeof(Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.PartNumber)))
					{
						UpdateFlag = 0;
					}
				}
			}
			//If module is not the same as previous, then update info and thresholds values
			if (UpdateFlag)
			{
				PROFILE_SetValue(Profile, &InfoParams[ProfileADMInfoLnSFPVendorIdx[Idx] - 1], Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.Vendor);
				PROFILE_SetValue(Profile, &InfoParams[ProfileADMInfoLnSFPSrNumberIdx[Idx] - 1], Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.SerialNumber);
				PROFILE_SetValue(Profile, &InfoParams[ProfileADMInfoLnSFPPtNumberIdx[Idx] - 1], Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.PartNumber);
				
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnRxPwrIdx[Idx][0] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.RxPwrThrs[SFP_THR_CMIN]);
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnRxPwrIdx[Idx][1] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.RxPwrThrs[SFP_THR_WMIN]);
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnRxPwrIdx[Idx][2] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.RxPwrThrs[SFP_THR_WMAX]);
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnRxPwrIdx[Idx][3] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.RxPwrThrs[SFP_THR_CMAX]);
				
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnTxPwrIdx[Idx][0] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.TxPwrThrs[SFP_THR_CMIN]);
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnTxPwrIdx[Idx][1] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.TxPwrThrs[SFP_THR_WMIN]);
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnTxPwrIdx[Idx][2] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.TxPwrThrs[SFP_THR_WMAX]);
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnTxPwrIdx[Idx][3] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.TxPwrThrs[SFP_THR_CMAX]);
				
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnSFPTempIdx[Idx][0] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.TempThrs[SFP_THR_CMIN]);
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnSFPTempIdx[Idx][1] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.TempThrs[SFP_THR_WMIN]);
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnSFPTempIdx[Idx][2] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.TempThrs[SFP_THR_WMAX]);
				PROFILE_SetValue(Profile, &StaticParams[ProfileADMStaticLnSFPTempIdx[Idx][3] - 1], &Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.TempThrs[SFP_THR_CMAX]);
				
				PROFILE_FLASH_ParamsSave(&Profile->ProfileFlash);
			}
			PROFILE_SetState(Profile, &InfoParams[ProfileADMInfoLnDestinationIdx[Idx] - 1], State);
			PROFILE_SetState(Profile, &InfoParams[ProfileADMInfoLnSFPVendorIdx[Idx] - 1], State);
			PROFILE_SetState(Profile, &InfoParams[ProfileADMInfoLnSFPSrNumberIdx[Idx] - 1], State);
			PROFILE_SetState(Profile, &InfoParams[ProfileADMInfoLnSFPPtNumberIdx[Idx] - 1], State);
			break;	
	}
}

static void PROFILE_ADM_LINE_UpdateRxPwrValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	float Value = -40.0;
	uint32_t State;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			Value = -40.0f;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnRxPwrIdx[Idx] - 1], &Value);
			PROFILE_SetColorToThresholds(Profile, &DynamicParams[ProfileADMDynamicLnRxPwrIdx[Idx] - 1]);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnRxPwrIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			Value = Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.RxPwr;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnRxPwrIdx[Idx] - 1], &Value);
			PROFILE_SetColorToThresholds(Profile, &DynamicParams[ProfileADMDynamicLnRxPwrIdx[Idx] - 1]);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnRxPwrIdx[Idx] - 1], State);
			break;		
	}
}

static void PROFILE_ADM_LINE_UpdateTxPwrValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	float Value = -40.0;
	uint32_t State;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			Value = -40.0f;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnTxPwrIdx[Idx] - 1], &Value);
			PROFILE_SetColorToThresholds(Profile, &DynamicParams[ProfileADMDynamicLnTxPwrIdx[Idx] - 1]);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnTxPwrIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			Value = Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.TxPwr;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnTxPwrIdx[Idx] - 1], &Value);
			PROFILE_SetColorToThresholds(Profile, &DynamicParams[ProfileADMDynamicLnTxPwrIdx[Idx] - 1]);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnTxPwrIdx[Idx] - 1], State);
			break;		
	}
}

static void PROFILE_ADM_LINE_UpdateOTUAlarmsValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	PROFILE_COLOR_T Color, DegradeColor, FailureColor;
	uint32_t Value;
	uint32_t State;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	PROFILE_GetAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnOTUAlarmsIdx[Idx] - 1], &DegradeColor, &FailureColor);
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			Value = 0;
			Color = PROFILE_COLOR_NORMAL;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnOTUAlarmsIdx[Idx] - 1], &Value);
			PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnOTUAlarmsIdx[Idx] - 1], Color);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnOTUAlarmsIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			Value = 0;
			Color = PROFILE_COLOR_NORMAL;
			if (Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].LOS)
				Value |= 1 << 0;
			if (Value & 0x07)
				Color = FailureColor;
			else if (Value & (0x0F << 3))
				Color = DegradeColor;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnOTUAlarmsIdx[Idx] - 1], &Value);
			PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnOTUAlarmsIdx[Idx] - 1], Color);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnOTUAlarmsIdx[Idx] - 1], State);
			break;		
	}
}
/*
static void PROFILE_ADM_LINE_UpdateODUAlarmsValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	PROFILE_COLOR_T Color, DegradeColor, FailureColor;
	uint32_t Value;
	uint32_t DataType;
	uint32_t State;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetDataTypeIdx[Idx] - 1], &DataType);	
	PROFILE_GetAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnODUAlarmsIdx[Idx] - 1], &DegradeColor, &FailureColor);
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			Value = 0;
			Color = PROFILE_COLOR_NORMAL;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnODUAlarmsIdx[Idx] - 1], &Value);
			PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnODUAlarmsIdx[Idx] - 1], Color);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnODUAlarmsIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			Value = 0;
			Color = PROFILE_COLOR_NORMAL;
			if (DataType != 1)
			{
				PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnODUAlarmsIdx[Idx] - 1], &Value);
				PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnODUAlarmsIdx[Idx] - 1], Color);
				PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnODUAlarmsIdx[Idx] - 1], PROFILE_STATE_OOS);
			}
			else
			{
				if (Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].LOS)
					Value = 0;
				else
				{
					BSP_CPU_GetPortOTNOTUAlarms(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &Value);
					if (!(Value & (0x03)))
					{
						BSP_CPU_GetPortOTNODUAlarms(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &Value);
					}
				}
				if (Value & 0x07)
					Color = FailureColor;
				else if (Value & (0x07 << 3))
					Color = DegradeColor;
				PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnODUAlarmsIdx[Idx] - 1], &Value);
				PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnODUAlarmsIdx[Idx] - 1], Color);
				PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnODUAlarmsIdx[Idx] - 1], State);
			}
			break;		
	}
}

static void PROFILE_ADM_LINE_UpdateSDHAlarmsValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	PROFILE_COLOR_T Color, DegradeColor, FailureColor;
	uint32_t Value;
	uint32_t MSAlarms, RSAlarms;
	uint32_t DataType;
	uint32_t State;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetDataTypeIdx[Idx] - 1], &DataType);	
	PROFILE_GetAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnSDHAlarmsIdx[Idx] - 1], &DegradeColor, &FailureColor);
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			Value = 0;
			Color = PROFILE_COLOR_NORMAL;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnSDHAlarmsIdx[Idx] - 1], &Value);
			PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnSDHAlarmsIdx[Idx] - 1], Color);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnSDHAlarmsIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			Value = 0;
			Color = PROFILE_COLOR_NORMAL;
			if (DataType != 131)
			{
				PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnSDHAlarmsIdx[Idx] - 1], &Value);
				PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnSDHAlarmsIdx[Idx] - 1], Color);
				PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnSDHAlarmsIdx[Idx] - 1], PROFILE_STATE_OOS);
			}
			else
			{
				if (Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].LOS)
					Value = 0;
				else
				{
					BSP_CPU_GetPortSDHRSAlarms(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &RSAlarms);
					Value = RSAlarms;
					if (!(Value & (0x03)))
					{
						BSP_CPU_GetPortSDHMSAlarms(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &MSAlarms);
						Value |= MSAlarms << 4;
					}
				}
				if (Value & 0x03)
					Color = FailureColor;
				else if (Value & (0x3F << 3))
					Color = DegradeColor;
				PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnSDHAlarmsIdx[Idx] - 1], &Value);
				PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnSDHAlarmsIdx[Idx] - 1], Color);
				PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnSDHAlarmsIdx[Idx] - 1], State);
			}
			break;		
	}
}

static void PROFILE_ADM_LINE_UpdateEthAlarmsValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	PROFILE_COLOR_T Color, DegradeColor, FailureColor;
	uint32_t Value;
	uint32_t DataType;
	uint32_t State;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetDataTypeIdx[Idx] - 1], &DataType);	
	PROFILE_GetAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnEthAlarmsIdx[Idx] - 1], &DegradeColor, &FailureColor);
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			Value = 0;
			Color = PROFILE_COLOR_NORMAL;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnEthAlarmsIdx[Idx] - 1], &Value);
			PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnEthAlarmsIdx[Idx] - 1], Color);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnEthAlarmsIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			Value = 0;
			Color = PROFILE_COLOR_NORMAL;
			if (DataType != 258)
			{
				PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnEthAlarmsIdx[Idx] - 1], &Value);
				PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnEthAlarmsIdx[Idx] - 1], Color);
				PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnEthAlarmsIdx[Idx] - 1], PROFILE_STATE_OOS);
			}
			else
			{
				if (Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].LOS)
					Value = 0;
				else
				{
					BSP_CPU_GetPortEthAlarms(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &Value);
				}
				if (Value & 0x03)
					Color = FailureColor;
				else if (Value & (0x3F << 3))
					Color = DegradeColor;
				PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnEthAlarmsIdx[Idx] - 1], &Value);
				PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnEthAlarmsIdx[Idx] - 1], Color);
				PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnEthAlarmsIdx[Idx] - 1], State);
			}
			break;		
	}
}

static void PROFILE_ADM_LINE_UpdatePreFECBERValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	float Value;
	uint32_t DataType;
	uint32_t State;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetDataTypeIdx[Idx] - 1], &DataType);
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			Value = 0;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnPreFECBERIdx[Idx] - 1], &Value);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnPreFECBERIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			Value = 0;
			if (DataType != 1)
			{
				PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnPreFECBERIdx[Idx] - 1], &Value);
				PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnPreFECBERIdx[Idx] - 1], PROFILE_STATE_OOS);
			}
			else
			{	
				BSP_CPU_GetPortOTNFECBER(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &Value);
				PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnPreFECBERIdx[Idx] - 1], &Value);
				PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnPreFECBERIdx[Idx] - 1], State);
			}
			break;		
	}
}
*/
static void PROFILE_ADM_LINE_UpdateSFPStateValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	uint32_t Value = 0;
	PROFILE_COLOR_T Color, DegradeColor, FailureColor;
	uint32_t State;
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);	
	PROFILE_GetAlarmColors(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], &DegradeColor, &FailureColor);
		
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
			Value = 0;
			Color = FailureColor;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], &Value);
			PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], Color);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], State);
			break;
		case BSP_DEV_STATE_INIT:
			Value = 2;
			Color = PROFILE_COLOR_NORMAL;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], &Value);
			PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], Color);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], State);
			break;
		case BSP_DEV_STATE_OK:
			Value = 1;
			Color = PROFILE_COLOR_NORMAL;
			/* todo: Add additional check for failure states */
			if (Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].TxFault)
			{
				Value = 3;
				Color = FailureColor;
			}
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], &Value);
			PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], Color);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], State);
			break;
		case BSP_DEV_STATE_FAIL:
			Value = 3;
			Color = FailureColor;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], &Value);
			PROFILE_SetColor(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], Color);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnSFPStateIdx[Idx] - 1], State);
			break;
	}
}

static void PROFILE_ADM_LINE_UpdateSFPTempValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	float Value = 0.0;
	uint32_t State;
	PROFILE_PARAM_T	*DynamicParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_2];
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
		
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			Value = 0.0;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnSFPTempIdx[Idx] - 1], &Value);
			PROFILE_SetColorToThresholds(Profile, &DynamicParams[ProfileADMDynamicLnSFPTempIdx[Idx] - 1]);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnSFPTempIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			Value = Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp.Temp;
			PROFILE_SetValue(Profile, &DynamicParams[ProfileADMDynamicLnSFPTempIdx[Idx] - 1], &Value);
			PROFILE_SetColorToThresholds(Profile, &DynamicParams[ProfileADMDynamicLnSFPTempIdx[Idx] - 1]);
			PROFILE_SetState(Profile, &DynamicParams[ProfileADMDynamicLnSFPTempIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;		
	}
}
/*
static void PROFILE_ADM_LINE_UpdateALSValue(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
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
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
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
static void PROFILE_ADM_LINE_UpdateDynamicValues(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	PROFILE_ADM_LINE_UpdateRxPwrValue(Profile, Devices, Idx);
	PROFILE_ADM_LINE_UpdateTxPwrValue(Profile, Devices, Idx);
	PROFILE_ADM_LINE_UpdateOTUAlarmsValue(Profile, Devices, Idx);
	//PROFILE_ADM_LINE_UpdateODUAlarmsValue(Profile, Devices, Idx);
	//PROFILE_ADM_LINE_UpdateSDHAlarmsValue(Profile, Devices, Idx);
	//PROFILE_ADM_LINE_UpdateEthAlarmsValue(Profile, Devices, Idx);
	//PROFILE_ADM_LINE_UpdatePreFECBERValue(Profile, Devices, Idx);
	PROFILE_ADM_LINE_UpdateSFPStateValue(Profile, Devices, Idx);
	PROFILE_ADM_LINE_UpdateSFPTempValue(Profile, Devices, Idx);
	//PROFILE_ADM_LINE_UpdateALSValue(Profile, Devices, Idx);
}
/*
static void PROFILE_ADM_LINE_UpdatePerfOTNValues(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	uint32_t EB, ES, SES, BBE, UAS;
	uint32_t State;
	uint32_t DataType;
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	PROFILE_PARAM_T	*PerfParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_8];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetDataTypeIdx[Idx] - 1], &DataType);
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUEBIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUESIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUBBEIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUSESIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUUASIdx[Idx] - 1], PROFILE_STATE_OOS);
		
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUEBIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUESIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUBBEIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUSESIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUUASIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			if (DataType != 1)
			{
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUEBIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUESIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUBBEIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUSESIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUUASIdx[Idx] - 1], PROFILE_STATE_OOS);
				
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUEBIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUESIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUBBEIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUSESIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUUASIdx[Idx] - 1], PROFILE_STATE_OOS);
			}
			else
			{
				BSP_CPU_GetPortOTNOTUEB(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &EB);
				BSP_CPU_GetPortOTNOTUES(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &ES);
				BSP_CPU_GetPortOTNOTUBBE(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &BBE);
				BSP_CPU_GetPortOTNOTUSES(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &SES);
				BSP_CPU_GetPortOTNOTUUAS(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &UAS);
				
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnOTUEBIdx[Idx] - 1], &EB);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnOTUESIdx[Idx] - 1], &ES);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnOTUBBEIdx[Idx] - 1], &BBE);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnOTUSESIdx[Idx] - 1], &SES);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnOTUUASIdx[Idx] - 1], &UAS);
				
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUEBIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUESIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUBBEIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUSESIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnOTUUASIdx[Idx] - 1], State);
				
				BSP_CPU_GetPortOTNODUEB(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &EB);
				BSP_CPU_GetPortOTNODUES(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &ES);
				BSP_CPU_GetPortOTNODUBBE(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &BBE);
				BSP_CPU_GetPortOTNODUSES(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &SES);
				BSP_CPU_GetPortOTNODUUAS(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &UAS);
				
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnODUEBIdx[Idx] - 1], &EB);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnODUESIdx[Idx] - 1], &ES);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnODUBBEIdx[Idx] - 1], &BBE);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnODUSESIdx[Idx] - 1], &SES);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnODUUASIdx[Idx] - 1], &UAS);
				
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUEBIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUESIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUBBEIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUSESIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnODUUASIdx[Idx] - 1], State);
			}
			break;		
	}	
}

static void PROFILE_ADM_LINE_UpdatePerfSDHValues(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	uint32_t EB, ES, SES, BBE, UAS;
	uint32_t State;
	uint32_t DataType;
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	PROFILE_PARAM_T	*PerfParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_8];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetDataTypeIdx[Idx] - 1], &DataType);
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSEBIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSESIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSBBEIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSSESIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSUASIdx[Idx] - 1], PROFILE_STATE_OOS);
		
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSEBIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSESIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSBBEIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSSESIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSUASIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			if (DataType != 131)
			{
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSEBIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSESIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSBBEIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSSESIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSUASIdx[Idx] - 1], PROFILE_STATE_OOS);
				
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSEBIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSESIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSBBEIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSSESIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSUASIdx[Idx] - 1], PROFILE_STATE_OOS);
			}
			else
			{
				BSP_CPU_GetPortSDHRSEB(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &EB);
				BSP_CPU_GetPortSDHRSES(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &ES);
				BSP_CPU_GetPortSDHRSBBE(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &BBE);
				BSP_CPU_GetPortSDHRSSES(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &SES);
				BSP_CPU_GetPortSDHRSUAS(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &UAS);
				
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnSDHRSEBIdx[Idx] - 1], &EB);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnSDHRSESIdx[Idx] - 1], &ES);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnSDHRSBBEIdx[Idx] - 1], &BBE);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnSDHRSSESIdx[Idx] - 1], &SES);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnSDHRSUASIdx[Idx] - 1], &UAS);
				
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSEBIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSESIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSBBEIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSSESIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHRSUASIdx[Idx] - 1], State);
				
				BSP_CPU_GetPortSDHMSEB(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &EB);
				BSP_CPU_GetPortSDHMSES(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &ES);
				BSP_CPU_GetPortSDHMSBBE(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &BBE);
				BSP_CPU_GetPortSDHMSSES(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &SES);
				BSP_CPU_GetPortSDHMSUAS(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &UAS);
				
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnSDHMSEBIdx[Idx] - 1], &EB);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnSDHMSESIdx[Idx] - 1], &ES);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnSDHMSBBEIdx[Idx] - 1], &BBE);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnSDHMSSESIdx[Idx] - 1], &SES);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnSDHMSUASIdx[Idx] - 1], &UAS);
				
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSEBIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSESIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSBBEIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSSESIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnSDHMSUASIdx[Idx] - 1], State);
			}
			break;		
	}	
}

static void PROFILE_ADM_LINE_UpdatePerfEthValues(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	uint32_t RxPkts, RxOcts, RxErrs, TxPkts, TxOcts, TxErrs;
	uint32_t State;
	uint32_t DataType;
	PROFILE_PARAM_T	*ConfigParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_3];
	PROFILE_PARAM_T	*PerfParams = (PROFILE_PARAM_T*)Profile->Descr[Profile->Idx]->Tables[PROFILE_TABLE_8];
	
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetStateIdx[Idx] - 1], &State);
	PROFILE_GetValue(Profile, &ConfigParams[ProfileADMConfigLnSetDataTypeIdx[Idx] - 1], &DataType);
	
	switch (BSP_DEV_GET_STATE(&Devices->Sfp[BSP_SFP_LINE_IDX(Idx)].sfp))
	{
		case BSP_DEV_STATE_RESET:
		case BSP_DEV_STATE_INIT:
		case BSP_DEV_STATE_FAIL:
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthRxPktsIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthRxOctsIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthRxErrsIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthTxPktsIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthTxOctsIdx[Idx] - 1], PROFILE_STATE_OOS);
			PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthTxErrsIdx[Idx] - 1], PROFILE_STATE_OOS);
			break;
		case BSP_DEV_STATE_OK:
			if (DataType != 258)
			{
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthRxPktsIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthRxOctsIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthRxErrsIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthTxPktsIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthTxOctsIdx[Idx] - 1], PROFILE_STATE_OOS);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthTxErrsIdx[Idx] - 1], PROFILE_STATE_OOS);
			}
			else
			{
				BSP_CPU_GetPortEthRxPkts(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &RxPkts);
				BSP_CPU_GetPortEthRxOcts(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &RxOcts);
				BSP_CPU_GetPortEthRxErrs(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &RxErrs);
				BSP_CPU_GetPortEthTxPkts(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &TxPkts);
				BSP_CPU_GetPortEthTxOcts(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &TxOcts);
				BSP_CPU_GetPortEthTxErrs(&Devices->CPU, BSP_CPU_LINE_PORT(Idx), &TxErrs);
				
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnEthRxPktsIdx[Idx] - 1], &RxPkts);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnEthRxOctsIdx[Idx] - 1], &RxOcts);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnEthRxErrsIdx[Idx] - 1], &RxErrs);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnEthTxPktsIdx[Idx] - 1], &TxPkts);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnEthTxOctsIdx[Idx] - 1], &TxOcts);
				PROFILE_SetValue(Profile, &PerfParams[ProfileADMPerfLnEthTxErrsIdx[Idx] - 1], &TxErrs);
				
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthRxPktsIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthRxOctsIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthRxErrsIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthTxPktsIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthTxOctsIdx[Idx] - 1], State);
				PROFILE_SetState(Profile, &PerfParams[ProfileADMPerfLnEthTxErrsIdx[Idx] - 1], State);
			}
			break;		
	}	
}
*/
static void PROFILE_ADM_LINE_UpdatePerfValues(PROFILE_T *Profile, BSP_DEVICES_T *Devices, uint32_t Idx)
{
	//PROFILE_ADM_LINE_UpdatePerfOTNValues(Profile, Devices, Idx);
	//PROFILE_ADM_LINE_UpdatePerfSDHValues(Profile, Devices, Idx);
	//PROFILE_ADM_LINE_UpdatePerfEthValues(Profile, Devices, Idx);
}


void PROFILE_ADM_LINE_UpdateValues(PROFILE_T *Profile, BSP_DEVICES_T *Devices)
{
	uint32_t i;
	
	for (i = 0; i < PROFILE_ADM_LINE_PORTS; i++)
	{
		PROFILE_ADM_LINE_UpdateInfoAndStaticValues(Profile, Devices, i);
		PROFILE_ADM_LINE_UpdateDynamicValues(Profile, Devices, i);
		PROFILE_ADM_LINE_UpdatePerfValues(Profile, Devices, i);
	}
}
