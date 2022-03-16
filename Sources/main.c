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
#include "log.h"
#include "user.h"
#include "watchdog.h"

#include "http_server.h"
#include "http_server_files.h"
#include "tftp_client.h"
#include "tftp_bootloader.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

SHELL_T ShellUart5;
const SHELL_IFACE_T ShellUart5Iface = 
{
	NULL,
	NULL,
	&SHELL_UART_Receive,
	&SHELL_UART_Transmit,
	NULL,
	UART5
};

SHELL_T ShellTelnet;
SHELL_TELNET_PRIV_T ShellTelnetPriv;
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
PROFILE_DESCR_T *ProfileDescr[BSP_CONFIG_PROFILES_CNT] = {&ProfileADM10Descr};

PROFILE_T Profile;
PROFILE_VALUES_STATIC_T ProfileValuesStatic;
/*
  * IMPORTANT NOTE!
  * Data in ccmram section are not initialized in startup code.
  * Data in ccmram section could not be used in DMA transfers.
  */
PROFILE_VALUES_DYNAMIC_T __attribute__((section (".ccmram"))) ProfileValuesDynamic;
PROFILE_VALUES_CALIB_T ProfileValuesCalib;
PROFILE_VALUES_T ProfileValues = {
	.Dynamic = &ProfileValuesDynamic,
	.Static = &ProfileValuesStatic,
	.Calib = &ProfileValuesCalib,
};
PROFILE_FLASH_T ProfileFlash;
PROFILE_PROTOCOL_T ProfileProtocol;
//PROFILE_IFACE_T ProfileIfaceUart5;
PROFILE_IFACE_T ProfileIfaceUart4;

BOOTLOADER_T Bootloader;
const BOOTLOADER_INFO_T __attribute__((section(".bldr_info"))) BootloaderInfo= {
	BSP_PROJECT,
	BSP_HW_VERSION,
	BSP_SW_VERSION,
	0xFFFF
};

osThreadId SystemThreadHandle;
uint32_t SystemThreadStage = 0;
uint32_t SystemThreadPeriod = 0;
uint32_t SystemThreadMaxPeriod = 0;
/* Private functions */

/* Threads */

/* Private function prototypes -----------------------------------------------*/
static void System_Thread(void const *argument);
/* System Thread definition */
osThreadDef(System, System_Thread, osPriorityNormal, 0, 448);

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

void __PROFILE_GetStringColor(PROFILE_T *Profile, uint8_t TableIdx, uint16_t ParamIdx, uint8_t *Color, uint8_t DegradeColor, uint8_t FailureColor)
{
	PROFILE_ADM10_GetStringColor(Profile, TableIdx, ParamIdx, Color, DegradeColor, FailureColor);
}

void PROFILE_UpdateValues(void)
{
	PROFILE_ADM10_UpdateValues(&Profile);	
}

int32_t PROFILE_CheckConfigValue(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore)
{
	if (PROFILE_ADM10_CheckConfigValue(Profile, Table, Idx, Value, Restore))
		return -1;
	return 0;
}

int32_t PROFILE_SetConfigValueDependencies(PROFILE_T *Profile, uint8_t Table, uint16_t Idx, void *Value, uint32_t Restore)
{
	if (PROFILE_ADM10_SetConfigValueDependencies(Profile, Table, Idx, Value, Restore))
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
	uint32_t Time = 0;
	
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
	
	/* Init Log */
	LOG_Init();
	WATCHDOG_Refresh();
	
	/* Init Users */
	USER_Init();
	WATCHDOG_Refresh();
	
	/* Init Profile*/
	PROFILE_Init(&Profile, ProfileDescr, &ProfileValues);
	WATCHDOG_Refresh();
	
	/* Init Profile Flash*/
	PROFILE_FLASH_Init(&ProfileFlash, "Profile FLASH", &Profile);
	WATCHDOG_Refresh();
	
	/* Init Profile Protocol*/
	PROFILE_PROTOCOL_Init(&ProfileProtocol, &Profile, &Bootloader, &ProfileFlash);
	
	/* Init Profile Interface over UART*/
	PROFILE_IFACE_Init(&ProfileIfaceUart4, "Profile UART4", UART4, &SlotAddress, &ProfileProtocol);
	WATCHDOG_Refresh();
	
	/* Init Shell Interface over UART*/
	SHELL_Init(&ShellUart5, "Shell UART5" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellUart5Iface);
	WATCHDOG_Refresh();	
	
	/*Init Ethernet stack*/
	BSP_ETH_Init();
	WATCHDOG_Refresh();
	
	/*Init Shell over Ethernet*/
	SHELL_Init(&ShellTelnet, "Shell TELNET" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellTelnetIface);
	WATCHDOG_Refresh();	
	
	TFTP_CLIENT_Init();
	TFTP_BOOTLOADER_Init();
	HTTP_SERVER_FILES_Init();
	HTTP_SERVER_Init();
	
	for(;;)
	{
		SystemThreadStage = 0;
		
		WATCHDOG_Refresh();
		Time = osKernelSysTick();
		
		SystemThreadStage++;
		
		BSP_UpdateSlotAddress();
		
		SystemThreadStage++;
		
		PROFILE_UpdateConfigs();
		
		SystemThreadStage++;
		
		BSP_UpdateCard();
		
		SystemThreadStage++;
		
		PROFILE_UpdateValues();
		
		SystemThreadStage++;
		
		SystemThreadPeriod = osKernelSysTick() - Time;
		if (SystemThreadPeriod > SystemThreadMaxPeriod)
			SystemThreadMaxPeriod = SystemThreadPeriod;
		
		SystemThreadStage++;
		
		osDelayUntil(&Time, 1000);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
