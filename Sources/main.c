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

#include "dm.h"
#include "dm_link.h"
#include "dev_agent.h"
#include "dev_agent_backend.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
	
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
osThreadId SystemThreadHandle;

/* Private functions */

/* Threads */

/* Private function prototypes -----------------------------------------------*/
static void System_Thread(void const *argument);
/* System Thread definition */
osThreadDef(System, System_Thread, osPriorityNormal, 0, 1024);

/* Private function prototypes -----------------------------------------------*/
static void Agent_Thread(void const *argument);
/* System Thread definition */
osThreadDef(Agent, Agent_Thread, osPriorityAboveNormal, 0, 512);

/* Private functions ---------------------------------------------------------*/

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

static void Agent_Thread(void const *argument)
{
	while (1) {
		dev_agent_process();
	}
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
	
	dm_lock_init();
	dm_init_data_model();
	load_startup_config();
	
	BSP_ETH_Init();
	TFTP_CLIENT_Init();
	SHELL_Init(&ShellTelnet, "Shell Telnet" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellTelnetIface);
	WATCHDOG_Refresh();
	SHELL_Init(&ShellUart4, "Shell UART4" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellUart4Iface);
	WATCHDOG_Refresh();
	SHELL_Init(&ShellUart5, "Shell UART5" ,(SHELL_CMD_T*)ShellCmdList, ShellCmdListSize, (SHELL_IFACE_T*)&ShellUart5Iface);
	
	osThreadCreate (osThread(Agent), NULL);
	
	WATCHDOG_Refresh();
	for(;;)
  {
		WATCHDOG_Refresh();
		Time = osKernelSysTick();
		BSP_UpdateCard();
		Period = osKernelSysTick() - Time;
		if (Period > MaxPeriod)
			MaxPeriod = Period;
		if (Period < MinPeriod)
			MinPeriod = Period;
		osDelayUntil(&Time, 1000);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
