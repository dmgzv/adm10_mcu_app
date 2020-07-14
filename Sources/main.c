/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "cmsis_os.h"
#include "bsp.h"
#include "profile.h"
#include "profile_iface.h"
#include "profile_types.h"
#include "profile_protocol.h"
#include "profile_adm10.h"
#include "shell_cmd.h"
#include "shell_uart.h"
#include "shell_telnet.h"
#include "http_server.h"
#include "tftp_client.h"
#include "tftp_bootloader.h"
#include "log.h"
#include "watchdog.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define PROFILE_IFACE_UART4 UART4
#ifndef BSP_CONFIG_USE_UART4
	#error	Define BSP_CONFIG_USE_UART4 in bsp_config.h for PROFILE_IFACE
#endif

#define PROFILE_IFACE_UART5 UART5
#ifndef BSP_CONFIG_USE_UART5
	#error	Define BSP_CONFIG_USE_UART5 in bsp_config.h for PROFILE_IFACE
#endif
	
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

SHELL_T /*__attribute__((section ("CCM_DATA")))*/ ShellTelnet;
SHELL_TELNET_PRIV_T /*__attribute__((section ("CCM_DATA")))*/ ShellTelnetPriv;
const SHELL_IFACE_T ShellTelnetIface = 
{
	&SHELL_TELNET_Init,
	&SHELL_TELNET_Connect,
	&SHELL_TELNET_Receive,
	&SHELL_TELNET_Transmit,
	&SHELL_TELNET_Disconnect,
	&ShellTelnetPriv
};
	
typedef union {
	PROFILE_ADM10_VALUES_DYNAMIC_T	ADM10;
} PROFILE_VALUES_DYNAMIC_T;

typedef union {
	PROFILE_ADM10_VALUES_STATIC_T	ADM10;
}PROFILE_VALUES_STATIC_T;

typedef union {
	PROFILE_ADM10_VALUES_STATIC_T	ADM10;
}PROFILE_VALUES_CALIB_T;

extern PROFILE_DESCR_T ProfileADM10Descr;
PROFILE_DESCR_T __attribute__((section ("CCM_DATA"))) *ProfileDescr[BSP_CONFIG_PROFILES_CNT] = {&ProfileADM10Descr};

PROFILE_T __attribute__((section ("CCM_DATA"))) Profile;
PROFILE_VALUES_STATIC_T /*__attribute__((section ("CCM_DATA")))*/ ProfileValuesStatic;
PROFILE_VALUES_DYNAMIC_T __attribute__((section ("CCM_DATA"))) ProfileValuesDynamic;
PROFILE_VALUES_CALIB_T /*__attribute__((section ("CCM_DATA")))*/ ProfileValuesCalib;
PROFILE_VALUES_T __attribute__((section ("CCM_DATA"))) ProfileValues = {
	.Dynamic = &ProfileValuesDynamic,
	.Static = &ProfileValuesStatic,
	.Calib = &ProfileValuesCalib,
};
PROFILE_FLASH_T /*__attribute__((section ("CCM_DATA")))*/ ProfileFlash;
PROFILE_PROTOCOL_T /*__attribute__((section ("CCM_DATA")))*/ ProfileProtocol;
PROFILE_IFACE_T /*__attribute__((section ("CCM_DATA")))*/ ProfileIfaceUart5;
PROFILE_IFACE_T /*__attribute__((section ("CCM_DATA")))*/ ProfileIfaceUart4;

BOOTLOADER_T /*__attribute__((section ("CCM_DATA")))*/ Bootloader;
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
osThreadDef(System, System_Thread, osPriorityNormal, 0, 320);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	
	/* Initialization of MCU and Pereferia*/
 	BSP_Init();
  WATCHDOG_Init(10000);
  /* Start System thread */
  SystemThreadHandle = osThreadCreate (osThread(System), NULL);
  
  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  for(;;);
}

void PROFILE_UpdateValues(void)
{
	PROFILE_ADM10_UpdateValues(&Profile);	
}

int32_t PROFILE_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	if (PROFILE_ADM10_CheckConfigValue(Profile, Table, Idx, Value))
		return -1;
	return 0;
}

int32_t PROFILE_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value)
{
	if (PROFILE_ADM10_SetConfigValueDependencies(Profile, Table, Idx, Value))
		return -1;
	return 0;
}

void PROFILE_UpdateConfigs(void)
{
	PROFILE_ADM10_UpdateConfigs(&Profile);
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
	PROFILE_Init(&Profile, ProfileDescr, &ProfileValues);
	WATCHDOG_Refresh();
	
	/* Init Profile Flash*/
	PROFILE_FLASH_Init(&ProfileFlash, "Profile FLASH", &Profile);
	WATCHDOG_Refresh();
	
	/* Init Profile Protocol*/
	PROFILE_PROTOCOL_Init(&ProfileProtocol, &Profile, &Bootloader, &ProfileFlash);
	
	/* Init Profile Interfaces*/
	PROFILE_IFACE_Init(&ProfileIfaceUart5, "Profile UART5", PROFILE_IFACE_UART5, SlotAddress, &ProfileProtocol);
	WATCHDOG_Refresh();
	PROFILE_IFACE_Init(&ProfileIfaceUart4, "Profile UART4", PROFILE_IFACE_UART4, SlotAddress, &ProfileProtocol);
	WATCHDOG_Refresh();
	
	/*Init Ethernet stack*/
	BSP_ETH_Init();
	WATCHDOG_Refresh();
	
	/*Init Shell over Ethernet*/
	SHELL_Init(&ShellTelnet, "Shell TELNET" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellTelnetIface);
	WATCHDOG_Refresh();	
	
	TFTP_CLIENT_Init();
	TFTP_BOOTLOADER_Init();
	HTTP_SERVER_Init();
	
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
