/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "cmsis_os.h"
#include "bsp.h"

#include "bsp_netif.h"

#include "log.h"
#include "watchdog.h"

#include "profile.h"
#include "profile_iface.h"
#include "profile_types.h"
#include "profile_protocol.h"
#include "profile_adm.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PROFILE_IFACE_UART_4 UART4
#ifndef BSP_CONFIG_USE_UART4
	#error	Define BSP_CONFIG_USE_UART4 in bsp_config.h for PROFILE_IFACE
#endif
	
#define PROFILE_IFACE_UART_5 UART5
#ifndef BSP_CONFIG_USE_UART5
	#error	Define BSP_CONFIG_USE_UART5 in bsp_config.h for PROFILE_IFACE
#endif


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

typedef union {
	PROFILE_ADM_VALUES_DYNAMIC_T	adm;
} PROFILE_VALUES_DYNAMIC_T;

typedef union {
	PROFILE_ADM_VALUES_STATIC_T	adm;
}PROFILE_VALUES_STATIC_T;

extern PROFILE_DESCR_T ProfileADMDescr;
PROFILE_DESCR_T /*__attribute__((section ("CCM_DATA")))*/ *ProfileDescr[BSP_CONFIG_PROFILES_CNT] = {&ProfileADMDescr};

PROFILE_T /*__attribute__((section ("CCM_DATA")))*/ Profile;
PROFILE_VALUES_STATIC_T /*__attribute__((section ("CCM_DATA")))*/ ProfileValuesStatic;
PROFILE_VALUES_DYNAMIC_T /*__attribute__((section ("CCM_DATA")))*/ ProfileValuesDynamic;
PROFILE_IFACE_T /*__attribute__((section ("CCM_DATA")))*/ ProfileIfaceUart4;
PROFILE_IFACE_T /*__attribute__((section ("CCM_DATA")))*/ ProfileIfaceUart5;

BSP_DEVICES_T /*__attribute__((section ("CCM_DATA")))*/ Devices;
BSP_DEVICES_CFG_T /*__attribute__((section ("CCM_DATA")))*/ DevicesCfg;

BSP_ETH_T Eth;

osThreadId SystemThreadHandle;
/* Private functions */

/* Threads */

/* Private function prototypes -----------------------------------------------*/
static void System_Thread(void const *argument);
/* System Thread definition */
osThreadDef(System, System_Thread, osPriorityNormal, 0, 1024);

/* Private functions ---------------------------------------------------------*/

void PROFILE_UpdateValues(void)
{
	PROFILE_ADM_UpdateValues(&Profile, &Devices);	
}

int32_t PROFILE_CheckConfigValue(PROFILE_T *Profile, PROFILE_PARAM_T	*Param, void *Value)
{
	if (PROFILE_ADM_CheckConfigValue(Profile, &Devices, Param, Value))
		return -1;
	return 0;
}

int32_t PROFILE_SetConfigValueDependencies(PROFILE_T *Profile, PROFILE_PARAM_T	*Param, void *Value)
{
	if (PROFILE_ADM_SetConfigValueDependencies(Profile, &Devices, Param, Value))
		return -1;
	return 0;
}

void PROFILE_UpdateConfigs(void)
{
	PROFILE_ADM_UpdateConfigs(&Profile, &Devices, &DevicesCfg);
}

void PROFILE_InitStates(void)
{
	PROFILE_ADM_InitStates(&Profile);
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	
	/* Initialization of MCU*/
 	BSP_Init();
  WATCHDOG_Init(10000);
  /* Start System thread */
  SystemThreadHandle = osThreadCreate (osThread(System), NULL);
  
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  for(;;);
}

/**
  * @brief  System thread
  * @param  argument not used
  * @retval None
  */
static void System_Thread(void const *argument)
{
	uint32_t Time = 0, Period = 0, MaxPeriod = 0, MinPeriod = 0xFFFFFFFF;
	WATCHDOG_Refresh();
	LOG_Init();
	WATCHDOG_Refresh();
	BSP_InitPeriferia();
	WATCHDOG_Refresh();
	BSP_InitDevices(&Devices);
	WATCHDOG_Refresh();
	BSP_ETH_Init(&Eth, BSP_GetSlotAddress());
	WATCHDOG_Refresh();
	PROFILE_Init(&Profile, ProfileDescr, &ProfileValuesStatic, sizeof(ProfileValuesStatic), &ProfileValuesDynamic, sizeof(ProfileValuesDynamic), &Devices.Flash);
	WATCHDOG_Refresh();
	PROFILE_InitStates();
	WATCHDOG_Refresh();
	PROFILE_IFACE_Init(&ProfileIfaceUart4, "Profile Uart 4", PROFILE_IFACE_UART_4, BSP_GetSlotAddress(), &Profile);
	WATCHDOG_Refresh();
	PROFILE_IFACE_Init(&ProfileIfaceUart5, "Profile Uart 5", PROFILE_IFACE_UART_5, BSP_GetSlotAddress(), &Profile);
	WATCHDOG_Refresh();
	//SHELL_Init(&ShellUart1, "Shell Uart 1" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellUart1Iface);
	//SHELL_Init(&ShellUart2, "Shell Uart 2" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellUart2Iface);
	//SHELL_Init(&ShellUart6, "Shell Uart 6" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellUart6Iface);
	//SHELL_Init(&ShellTelnet, "Shell Telnet" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellTelnetIface);
	//WATCHDOG_Refresh();
	for(;;)
  {
		WATCHDOG_Refresh();
		Time = osKernelSysTick();
		PROFILE_UpdateConfigs();
		BSP_UpdateDevices(&Devices, &DevicesCfg);
		PROFILE_UpdateValues();
		Period = osKernelSysTick() - Time;
		if (Period > MaxPeriod)
			MaxPeriod = Period;
		if (Period < MinPeriod)
			MinPeriod = Period;
		osDelayUntil(&Time, 1000);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
