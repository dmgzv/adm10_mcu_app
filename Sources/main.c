/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "cmsis_os.h"
#include "bsp.h"

#include "bsp_eth.h"
#include "tftp_client.h"

#include "shell_cmd.h"
#include "shell_uart.h"
#include "shell_telnet.h"

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
	
#define SHELL_IFACE_UART4 UART4
#ifndef BSP_CONFIG_USE_UART4
	#error	Define BSP_CONFIG_USE_UART4 in bsp_config.h for SHELL_IFACE
#endif
	
#define SHELL_IFACE_UART5 UART5
#ifndef BSP_CONFIG_USE_UART5
	#error	Define BSP_CONFIG_USE_UART5 in bsp_config.h for SHELL_IFACE
#endif

SHELL_T __attribute__((section ("CCM_DATA"))) ShellUart4;
const SHELL_IFACE_T ShellUart4Iface = 
{
	NULL,
	NULL,
	&SHELL_UART_Receive,
	&SHELL_UART_Transmit,
	NULL,
	SHELL_IFACE_UART4
};

SHELL_T __attribute__((section ("CCM_DATA"))) ShellUart5;
const SHELL_IFACE_T ShellUart5Iface = 
{
	NULL,
	NULL,
	&SHELL_UART_Receive,
	&SHELL_UART_Transmit,
	NULL,
	SHELL_IFACE_UART5
};

SHELL_T __attribute__((section ("CCM_DATA"))) ShellTelnet;
SHELL_TELNET_PRIV_T __attribute__((section ("CCM_DATA"))) ShellTelnetPriv;
const SHELL_IFACE_T ShellTelnetIface = 
{
	&SHELL_TELNET_Init,
	&SHELL_TELNET_Connect,
	&SHELL_TELNET_Receive,
	&SHELL_TELNET_Transmit,
	&SHELL_TELNET_Disconnect,
	&ShellTelnetPriv
};
	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

typedef union {
	PROFILE_ADM_VALUES_DYNAMIC_T	adm;
} PROFILE_VALUES_DYNAMIC_T;

typedef union {
	PROFILE_ADM_VALUES_STATIC_T	adm;
}PROFILE_VALUES_STATIC_T;

extern PROFILE_DESCR_T ProfileADMDescr;
PROFILE_DESCR_T __attribute__((section ("CCM_DATA"))) *ProfileDescr[BSP_CONFIG_PROFILES_CNT] = {&ProfileADMDescr};

PROFILE_T __attribute__((section ("CCM_DATA"))) Profile;
PROFILE_VALUES_STATIC_T __attribute__((section ("CCM_DATA"))) ProfileValuesStatic;
PROFILE_VALUES_DYNAMIC_T __attribute__((section ("CCM_DATA"))) ProfileValuesDynamic;
PROFILE_IFACE_T __attribute__((section ("CCM_DATA"))) ProfileIfaceUart4;
PROFILE_IFACE_T __attribute__((section ("CCM_DATA"))) ProfileIfaceUart5;

const BOOTLOADER_INFO_T __attribute__((at(BOOTLOADER_FW_INFO))) BootloaderInfo= {
	BSP_PROJECT,
	BSP_HW_VERSION,
	BSP_SW_VERSION,
	0xFFFF
};

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
	PROFILE_ADM_UpdateValues(&Profile);	
}

int32_t PROFILE_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	if (PROFILE_ADM_CheckConfigValue(Profile, Table, Idx, Value))
		return -1;
	return 0;
}

int32_t PROFILE_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	if (PROFILE_ADM_SetConfigValueDependencies(Profile, Table, Idx, Value))
		return -1;
	return 0;
}

void PROFILE_UpdateConfigs(void)
{
	PROFILE_ADM_UpdateConfigs(&Profile);
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
	
	/* Init Log*/	
	LOG_Init();
	WATCHDOG_Refresh();
	
	/* Init Slot Address */	
	BSP_InitSlotAddress();
	WATCHDOG_Refresh();
	
	/* Init Leds */
	BSP_InitLeds();
	WATCHDOG_Refresh();
	
	/* Init Peripheria*/
	BSP_InitInterfaces();
	WATCHDOG_Refresh();
	
	/* Init Memory*/
	BSP_InitMemory();
	WATCHDOG_Refresh();
	
	/* Init Profile*/
	PROFILE_Init(&Profile, ProfileDescr, &ProfileValuesStatic, sizeof(ProfileValuesStatic), &ProfileValuesDynamic, sizeof(ProfileValuesDynamic));
	WATCHDOG_Refresh();
	
	/* Init Profile Protocol*/
	PROFILE_IFACE_Init(&ProfileIfaceUart4, "Profile UART4", PROFILE_IFACE_UART_4, SlotAddress, &Profile);
	WATCHDOG_Refresh();
	PROFILE_IFACE_Init(&ProfileIfaceUart5, "Profile UART5", PROFILE_IFACE_UART_5, SlotAddress, &Profile);
	WATCHDOG_Refresh();

	BSP_ETH_Init();
	TFTP_CLIENT_Init();
	SHELL_Init(&ShellTelnet, "Shell Telnet" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellTelnetIface);
	//WATCHDOG_Refresh();
	//SHELL_Init(&ShellUart4, "Shell UART4" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellUart4Iface);
	//WATCHDOG_Refresh();
	//SHELL_Init(&ShellUart5, "Shell UART5" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellUart5Iface);
	
	WATCHDOG_Refresh();
	for(;;)
  {
		WATCHDOG_Refresh();
		Time = osKernelSysTick();
		PROFILE_UpdateConfigs();
		BSP_UpdateCard();
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
