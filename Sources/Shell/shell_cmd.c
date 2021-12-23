#include "shell_cmd.h"
#include <string.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "stdio.h"
#include "bsp.h"
#include "log.h"
#include "profile_iface.h"
#include "profile_flash.h"
#include "tftp_client.h"
#include "bootloader.h"
#include "http_server_files.h"

extern PROFILE_T Profile;
extern BOOTLOADER_T Bootloader;
extern PROFILE_FLASH_T ProfileFlash;

static void SHELL_CMD_SystemCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_InfoCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
#ifdef BSP_CONFIG_USE_LOG
static void SHELL_CMD_LOGCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
#endif
#if defined(BSP_CONFIG_USE_I2C1) || defined(BSP_CONFIG_USE_I2C2) || defined(BSP_CONFIG_USE_I2C3)
static void SHELL_CMD_I2CCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
#endif
#if defined(BSP_CONFIG_USE_SPI1) || defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || \
	defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5)
static void SHELL_CMD_SPICallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
#endif
#if defined(BSP_CONFIG_USE_UART1) || defined(BSP_CONFIG_USE_UART2) || defined(BSP_CONFIG_USE_UART3) || \
	defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6)
static void SHELL_CMD_UARTCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
#endif
static void SHELL_CMD_StatusCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_USERCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_PROFILECallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_FLASHCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_FPGACallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
#ifdef BSP_PWR_IN_DIM3R3400_ENABLE 
static void SHELL_CMD_DIM3RCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
#endif
static void SHELL_CMD_UCDCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_GDTCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_SFPCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_PCA9536Callback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_PCAL6524Callback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_PCA9548ACallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_KSZCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_SDRAMCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);

const SHELL_CMD_T ShellCmdList[] = 
{
	{ "system", 	"System information", SHELL_ACCESS_SU, &SHELL_CMD_SystemCallback},
	{ "info", 		"Project information", LOG_ACCESS_MONITOR, &SHELL_CMD_InfoCallback},
#ifdef BSP_CONFIG_USE_LOG
	{ "log", 			"Log control", SHELL_ACCESS_SU, &SHELL_CMD_LOGCallback},
#endif
#if defined(BSP_CONFIG_USE_I2C1) || defined(BSP_CONFIG_USE_I2C2) || defined(BSP_CONFIG_USE_I2C3)
	{ "i2c", 			"I2C statistics", SHELL_ACCESS_SU, &SHELL_CMD_I2CCallback},
#endif
#if defined(BSP_CONFIG_USE_SPI1) || defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || \
	defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5)
	{ "spi", 			"SPI statistics", SHELL_ACCESS_SU, &SHELL_CMD_SPICallback},
#endif
#if defined(BSP_CONFIG_USE_UART1) || defined(BSP_CONFIG_USE_UART2) || defined(BSP_CONFIG_USE_UART3) || \
	defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6)
	{ "uart", 		"UART statistics", SHELL_ACCESS_SU, &SHELL_CMD_UARTCallback},
#endif
	{ "status", 	"Status of devices", SHELL_ACCESS_SU, &SHELL_CMD_StatusCallback},
	{ "user", 		"User control", LOG_ACCESS_ADMIN, &SHELL_CMD_USERCallback},
	{ "profile", 	"Profile control", LOG_ACCESS_MONITOR, &SHELL_CMD_PROFILECallback},
	{ "flash", 		"External flash control", SHELL_ACCESS_SU, &SHELL_CMD_FLASHCallback},
#ifdef BSP_PWR_IN_DIM3R3400_ENABLE 
	{ "dim3r", 		"DIM3R3400 control", SHELL_ACCESS_SU, &SHELL_CMD_DIM3RCallback},
#endif
	{ "ucd", 			"UCD90120A control", SHELL_ACCESS_SU, &SHELL_CMD_UCDCallback},
	{ "gdt", 			"GDT080A control", SHELL_ACCESS_SU, &SHELL_CMD_GDTCallback},
	{ "sfp", 			"SFP control", SHELL_ACCESS_SU, &SHELL_CMD_SFPCallback},
	{ "fpga", 		"FPGA control", SHELL_ACCESS_SU, &SHELL_CMD_FPGACallback},
	{ "pca9536", 	"PCA9536 control", SHELL_ACCESS_SU, &SHELL_CMD_PCA9536Callback},
	{ "pcal6524", "PCAL6524 control", SHELL_ACCESS_SU, &SHELL_CMD_PCAL6524Callback},
	{ "pca9548a", "PCA9548A control", SHELL_ACCESS_SU, &SHELL_CMD_PCA9548ACallback},
	{ "ksz", 			"KSZ8895 control", SHELL_ACCESS_SU, &SHELL_CMD_KSZCallback},
	{ "sdram", 		"SDRAM control", SHELL_ACCESS_SU, &SHELL_CMD_SDRAMCallback},
};

const uint32_t ShellCmdListSize = sizeof(ShellCmdList)/ sizeof(ShellCmdList[0]);

/* Shell commands */
static void SHELL_CMD_SystemCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	SHELL_Transmit(Shell, "Heap size: %d bytes\r\n", xPortGetFreeHeapSize());
	SHELL_Transmit(Shell, "Min heap size: %d bytes\r\n", xPortGetMinimumEverFreeHeapSize());
	SHELL_Transmit(Shell, "Name          State Priority   Stack   Num\r\n");
	osThreadList((uint8_t*)((SHELL_T*)Shell)->TxBuffer);
	SHELL_TransmitBuffer(Shell,  ((SHELL_T*)Shell)->TxBuffer, strlen(((SHELL_T*)Shell)->TxBuffer));
}

static void SHELL_CMD_InfoCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t i;
	SHELL_Transmit(Shell, "Project: %s\r\n", BSP_PROJECT);
	SHELL_Transmit(Shell, "HW Version: %s\r\n", BSP_HW_VERSION);
	SHELL_Transmit(Shell, "SW Version: %s\r\n", BSP_SW_VERSION);
	SHELL_Transmit(Shell, "MCU SW Version: %s\r\n", BSP_MCU_SW_VERSION);
	if (BSP_DEV_GET_STATE(&fpga_maxv) == BSP_DEV_STATE_OK)
		SHELL_Transmit(Shell, "FPGA MAXV SW Version: %s\r\n", fpga_maxv.SwVersion );
	else
		SHELL_Transmit(Shell, "FPGA MAXV SW Version: -\r\n");
	if ((fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_BS) && (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs) == BSP_DEV_STATE_OK))
		SHELL_Transmit(Shell, "FPGA LINE Version: %s\r\n", fpga_line_agg_2x10_bs.SwVersion );
	else if ((fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10) && (BSP_DEV_GET_STATE(&fpga_line_adm_10) == BSP_DEV_STATE_OK))
		SHELL_Transmit(Shell, "FPGA LINE Version: %s\r\n", fpga_line_adm_10.SwVersion );
	else if ((fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_EFEC) && (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_efec) == BSP_DEV_STATE_OK))
		SHELL_Transmit(Shell, "FPGA LINE Version: %s\r\n", fpga_line_agg_2x10_efec.SwVersion );
	else if ((fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5) && (BSP_DEV_GET_STATE(&fpga_line_agg_2x10_bs) == BSP_DEV_STATE_OK))
		SHELL_Transmit(Shell, "FPGA LINE Version: %s\r\n", fpga_line_agg_2x10_bs.SwVersion );
	else
		SHELL_Transmit(Shell, "FPGA LINE Version: -\r\n");
	for (i = 0; i < BSP_FPGA_CLIENT_CNT; i++)
	{
		if ((fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_BS) && (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[i]) == BSP_DEV_STATE_OK))
			SHELL_Transmit(Shell, "FPGA CLIENT %d Version: %s\r\n", i, fpga_client_agg_2x10_bs[i].SwVersion );
		else if ((fpga_maxv.Mode == FPGA_MAXV_MODE_ADM_10) && (BSP_DEV_GET_STATE(&fpga_client_adm_10[i]) == BSP_DEV_STATE_OK))
			SHELL_Transmit(Shell, "FPGA CLIENT %d Version: %s\r\n", i, fpga_client_adm_10[i].SwVersion );
		else if ((fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_2X10_EFEC) && (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_efec[i]) == BSP_DEV_STATE_OK))
			SHELL_Transmit(Shell, "FPGA CLIENT %d Version: %s\r\n", i, fpga_client_agg_2x10_efec[i].SwVersion );
		else if ((fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5) && (i == 0) && (BSP_DEV_GET_STATE(&fpga_client_agg_2x10_bs[i]) == BSP_DEV_STATE_OK))
			SHELL_Transmit(Shell, "FPGA CLIENT %d Version: %s\r\n", i, fpga_client_agg_2x10_bs[i].SwVersion );
		else if ((fpga_maxv.Mode == FPGA_MAXV_MODE_AGG_10_BS_ADM_2_5) && (i == 1) && (BSP_DEV_GET_STATE(&fpga_client_adm_2_5[i]) == BSP_DEV_STATE_OK))
			SHELL_Transmit(Shell, "FPGA CLIENT %d Version: %s\r\n", i, fpga_client_adm_2_5[i].SwVersion );
		else
			SHELL_Transmit(Shell, "FPGA CLIENT %d Version: -\r\n", i);
	}
	SHELL_Transmit(Shell, "Time: %d s\r\n", osKernelSysTick() / 1000);
}

#ifdef BSP_CONFIG_USE_LOG

#define SHELL_CMD_LOG_HELP \
"Usage:\r\n"\
"\tlog <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\terase - Erase log records.\r\n"\

static void SHELL_CMD_LOGCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t i, LinesCnt;
	float Time;
	LOG_LINE_T LogLine;
	
	if (CmdParamsCnt == 1)
	{
		LinesCnt = LOG_GetLinesCnt();
		SHELL_Transmit(Shell, "Records Count: %d\r\n", LinesCnt);
		SHELL_Transmit(Shell, "Records Max Count: %d\r\n", LOG_LINES_MAX_CNT);
		for (i = 0; i < LinesCnt; i++)
		{
			LOG_GetLine(i, &LogLine);
			Time = (float)LogLine.TimeTicks / osKernelSysTickFrequency;
			SHELL_Transmit(Shell, "[%4.4f] %s %s %s %s\r\n", Time, LogLine.Category, LogLine.Subject, LogLine.Operation, LogLine.Description);
		}
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_LOG_HELP, sizeof(SHELL_CMD_LOG_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "erase"))
		{
			if (LOG_Erase())
				goto command_error;
		}
		else
		{
			goto invalid_command;
		}
	}
	else
	{
		goto invalid_command;
	}
	return;
	
invalid_command:
	SHELL_TransmitBuffer(Shell, "Error: Invalid command", sizeof("Error: Invalid command") - 1);
	return;
command_error:
	SHELL_TransmitBuffer(Shell, "Error: Command error", sizeof("Error: Command error") - 1);
	return;
}
#endif

#if defined(BSP_CONFIG_USE_I2C1) || defined(BSP_CONFIG_USE_I2C2) || defined(BSP_CONFIG_USE_I2C3)
static void SHELL_CMD_I2CCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{	
#ifdef BSP_CONFIG_USE_I2C1
	SHELL_Transmit(Shell, "I2C1:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", i2c1.overrun_errors);
	SHELL_Transmit(Shell, "\tBERR errors: %d\r\n", i2c1.berr_errors);
	SHELL_Transmit(Shell, "\tARLO errors: %d\r\n", i2c1.arlo_errors);
	SHELL_Transmit(Shell, "\tAF errors: %d\r\n", i2c1.af_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", i2c1.dma_errors);
	SHELL_Transmit(Shell, "\tReceive timeouts: %d\r\n", i2c1.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential receive timeouts: %d\r\n", i2c1.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit timeouts: %d\r\n", i2c1.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential transmit timeouts: %d\r\n", i2c1.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "\tQueue overflows: %d\r\n", i2c1.message_queue_overflow_errors);
	if (osMutexWait(i2c1.mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tLock: Unlocked\r\n");
		osMutexRelease(i2c1.mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tLock: Locked\r\n");
	}
#endif
#ifdef BSP_CONFIG_USE_I2C2
	SHELL_Transmit(Shell, "I2C2:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", i2c2.overrun_errors);
	SHELL_Transmit(Shell, "\tBERR errors: %d\r\n", i2c2.berr_errors);
	SHELL_Transmit(Shell, "\tARLO errors: %d\r\n", i2c2.arlo_errors);
	SHELL_Transmit(Shell, "\tAF errors: %d\r\n", i2c2.af_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", i2c2.dma_errors);
	SHELL_Transmit(Shell, "\tReceive timeouts: %d\r\n", i2c2.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential receive timeouts: %d\r\n", i2c2.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit timeouts: %d\r\n", i2c2.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential transmit timeouts: %d\r\n", i2c2.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "\tQueue overflows: %d\r\n", i2c2.message_queue_overflow_errors);
	if (osMutexWait(i2c2.mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tLock: Unlocked\r\n");
		osMutexRelease(i2c2.mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tLock: Locked\r\n");
	}
#endif
#ifdef BSP_CONFIG_USE_I2C3
	SHELL_Transmit(Shell, "I2C3:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", i2c3.overrun_errors);
	SHELL_Transmit(Shell, "\tBERR errors: %d\r\n", i2c3.berr_errors);
	SHELL_Transmit(Shell, "\tARLO errors: %d\r\n", i2c3.arlo_errors);
	SHELL_Transmit(Shell, "\tAF errors: %d\r\n", i2c3.af_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", i2c3.dma_errors);
	SHELL_Transmit(Shell, "\tReceive timeouts: %d\r\n", i2c3.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential receive timeouts: %d\r\n", i2c3.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit timeouts: %d\r\n", i2c3.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential transmit timeouts: %d\r\n", i2c3.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "\tQueue overflows: %d\r\n", i2c3.message_queue_overflow_errors);
	if (osMutexWait(i2c3.mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tLock: Unlocked\r\n");
		osMutexRelease(i2c3.mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tLock: Locked\r\n");
	}
#endif
}
#endif

#if defined(BSP_CONFIG_USE_SPI1) || defined(BSP_CONFIG_USE_SPI2) || defined(BSP_CONFIG_USE_SPI3) || \
	defined(BSP_CONFIG_USE_SPI4) || defined(BSP_CONFIG_USE_SPI5)
static void SHELL_CMD_SPICallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
#ifdef BSP_CONFIG_USE_SPI1
	SHELL_Transmit(Shell, "SPI1:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", spi1.overrun_errors);
	SHELL_Transmit(Shell, "\tMode fault errors: %d\r\n", spi1.mode_fault_errors);
	SHELL_Transmit(Shell, "\tCRC errors: %d\r\n", spi1.crc_errors);
	SHELL_Transmit(Shell, "\tFrame errors: %d\r\n", spi1.frame_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", spi1.dma_errors);
	SHELL_Transmit(Shell, "\tReceive timeouts: %d\r\n", spi1.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential receive timeouts: %d\r\n", spi1.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit timeouts: %d\r\n", spi1.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential transmit timeouts: %d\r\n", spi1.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit and receive timeouts: %d\r\n", spi1.tx_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tQueue overflows: %d\r\n", spi1.message_queue_overflow_errors);
	if (osMutexWait(spi1.mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tLock: Unlocked\r\n");
		osMutexRelease(spi1.mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tLock: Locked\r\n");
	}
#endif
#ifdef BSP_CONFIG_USE_SPI2
	SHELL_Transmit(Shell, "SPI2:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", spi2.overrun_errors);
	SHELL_Transmit(Shell, "\tMode fault errors: %d\r\n", spi2.mode_fault_errors);
	SHELL_Transmit(Shell, "\tCRC errors: %d\r\n", spi2.crc_errors);
	SHELL_Transmit(Shell, "\tFrame errors: %d\r\n", spi2.frame_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", spi2.dma_errors);
	SHELL_Transmit(Shell, "\tReceive timeouts: %d\r\n", spi2.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential receive timeouts: %d\r\n", spi2.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit timeouts: %d\r\n", spi2.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential transmit timeouts: %d\r\n", spi2.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit and receive timeouts: %d\r\n", spi2.tx_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tQueue overflows: %d\r\n", spi2.message_queue_overflow_errors);
	if (osMutexWait(spi2.mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tLock: Unlocked\r\n");
		osMutexRelease(spi2.mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tLock: Locked\r\n");
	}
#endif
#ifdef BSP_CONFIG_USE_SPI3
	SHELL_Transmit(Shell, "SPI3:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", spi3.overrun_errors);
	SHELL_Transmit(Shell, "\tMode fault errors: %d\r\n", spi3.mode_fault_errors);
	SHELL_Transmit(Shell, "\tCRC errors: %d\r\n", spi3.crc_errors);
	SHELL_Transmit(Shell, "\tFrame errors: %d\r\n", spi3.frame_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", spi3.dma_errors);
	SHELL_Transmit(Shell, "\tReceive timeouts: %d\r\n", spi3.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential receive timeouts: %d\r\n", spi3.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit timeouts: %d\r\n", spi3.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential transmit timeouts: %d\r\n", spi3.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit and receive timeouts: %d\r\n", spi3.tx_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tQueue overflows: %d\r\n", spi3.message_queue_overflow_errors);
	if (osMutexWait(spi3.mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tLock: Unlocked\r\n");
		osMutexRelease(spi3.mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tLock: Locked\r\n");
	}
#endif
#ifdef BSP_CONFIG_USE_SPI4
	SHELL_Transmit(Shell, "SPI4:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", spi4.overrun_errors);
	SHELL_Transmit(Shell, "\tMode fault errors: %d\r\n", spi4.mode_fault_errors);
	SHELL_Transmit(Shell, "\tCRC errors: %d\r\n", spi4.crc_errors);
	SHELL_Transmit(Shell, "\tFrame errors: %d\r\n", spi4.frame_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", spi4.dma_errors);
	SHELL_Transmit(Shell, "\tReceive timeouts: %d\r\n", spi4.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential receive timeouts: %d\r\n", spi4.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit timeouts: %d\r\n", spi4.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential transmit timeouts: %d\r\n", spi4.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit and receive timeouts: %d\r\n", spi4.tx_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tQueue overflows: %d\r\n", spi4.message_queue_overflow_errors);
	if (osMutexWait(spi4.mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tLock: Unlocked\r\n");
		osMutexRelease(spi4.mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tLock: Locked\r\n");
	}
#endif
#ifdef BSP_CONFIG_USE_SPI5
	SHELL_Transmit(Shell, "SPI5:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", spi5.overrun_errors);
	SHELL_Transmit(Shell, "\tMode fault errors: %d\r\n", spi5.mode_fault_errors);
	SHELL_Transmit(Shell, "\tCRC errors: %d\r\n", spi5.crc_errors);
	SHELL_Transmit(Shell, "\tFrame errors: %d\r\n", spi5.frame_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", spi5.dma_errors);
	SHELL_Transmit(Shell, "\tReceive timeouts: %d\r\n", spi5.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential receive timeouts: %d\r\n", spi5.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit timeouts: %d\r\n", spi5.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tSequential transmit timeouts: %d\r\n", spi5.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "\tTransmit and receive timeouts: %d\r\n", spi5.tx_rx_timeout_errors);
	SHELL_Transmit(Shell, "\tQueue overflows: %d\r\n", spi5.message_queue_overflow_errors);
	if (osMutexWait(spi5.mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tLock: Unlocked\r\n");
		osMutexRelease(spi5.mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tLock: Locked\r\n");
	}
#endif
}
#endif

#if defined(BSP_CONFIG_USE_UART1) || defined(BSP_CONFIG_USE_UART2) || defined(BSP_CONFIG_USE_UART3) || \
	defined(BSP_CONFIG_USE_UART4) || defined(BSP_CONFIG_USE_UART5) || defined(BSP_CONFIG_USE_UART6)
static void SHELL_CMD_UARTCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
#ifdef BSP_CONFIG_USE_UART1
	SHELL_Transmit(Shell, "UART1:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", uart1.overrun_errors);
	SHELL_Transmit(Shell, "\tParity errors: %d\r\n", uart1.parity_errors);
	SHELL_Transmit(Shell, "\tNoise errors: %d\r\n", uart1.noise_errors);
	SHELL_Transmit(Shell, "\tFrame errors: %d\r\n", uart1.frame_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", uart1.dma_errors);
	SHELL_Transmit(Shell, "\tRx timeouts: %d\r\n", uart1.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tRx cmd timeouts: %d\r\n", uart1.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "\tTx timeouts: %d\r\n", uart1.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tTx queue overflows: %d\r\n", uart1.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "\tRx queue overflows: %d\r\n", uart1.rx_message_queue_overflow_errors);
	if (osMutexWait(uart1.rx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tRx Lock: Unlocked\r\n");
		osMutexRelease(uart1.rx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tRx Lock: Locked\r\n");
	}
	if (osMutexWait(uart1.tx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tTx Lock: Unlocked\r\n");
		osMutexRelease(uart1.tx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tTx Lock: Locked\r\n");
	}
#endif
#ifdef BSP_CONFIG_USE_UART2
	SHELL_Transmit(Shell, "UART2:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", uart2.overrun_errors);
	SHELL_Transmit(Shell, "\tParity errors: %d\r\n", uart2.parity_errors);
	SHELL_Transmit(Shell, "\tNoise errors: %d\r\n", uart2.noise_errors);
	SHELL_Transmit(Shell, "\tFrame errors: %d\r\n", uart2.frame_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", uart2.dma_errors);
	SHELL_Transmit(Shell, "\tRx timeouts: %d\r\n", uart2.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tRx cmd timeouts: %d\r\n", uart2.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "\tTx timeouts: %d\r\n", uart2.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tTx queue overflows: %d\r\n", uart2.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "\tRx queue overflows: %d\r\n", uart2.rx_message_queue_overflow_errors);
	if (osMutexWait(uart2.rx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tRx Lock: Unlocked\r\n");
		osMutexRelease(uart2.rx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tRx Lock: Locked\r\n");
	}
	if (osMutexWait(uart2.tx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tTx Lock: Unlocked\r\n");
		osMutexRelease(uart2.tx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tTx Lock: Locked\r\n");
	}
#endif
#ifdef BSP_CONFIG_USE_UART3
	SHELL_Transmit(Shell, "UART3:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", uart3.overrun_errors);
	SHELL_Transmit(Shell, "\tParity errors: %d\r\n", uart3.parity_errors);
	SHELL_Transmit(Shell, "\tNoise errors: %d\r\n", uart3.noise_errors);
	SHELL_Transmit(Shell, "\tFrame errors: %d\r\n", uart3.frame_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", uart3.dma_errors);
	SHELL_Transmit(Shell, "\tRx timeouts: %d\r\n", uart3.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tRx cmd timeouts: %d\r\n", uart3.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "\tTx timeouts: %d\r\n", uart3.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tTx queue overflows: %d\r\n", uart3.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "\tRx queue overflows: %d\r\n", uart3.rx_message_queue_overflow_errors);
	if (osMutexWait(uart3.rx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tRx Lock: Unlocked\r\n");
		osMutexRelease(uart3.rx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tRx Lock: Locked\r\n");
	}
	if (osMutexWait(uart3.tx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tTx Lock: Unlocked\r\n");
		osMutexRelease(uart3.tx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tTx Lock: Locked\r\n");
	}
#endif
#ifdef BSP_CONFIG_USE_UART4
	SHELL_Transmit(Shell, "UART4:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", uart4.overrun_errors);
	SHELL_Transmit(Shell, "\tParity errors: %d\r\n", uart4.parity_errors);
	SHELL_Transmit(Shell, "\tNoise errors: %d\r\n", uart4.noise_errors);
	SHELL_Transmit(Shell, "\tFrame errors: %d\r\n", uart4.frame_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", uart4.dma_errors);
	SHELL_Transmit(Shell, "\tRx timeouts: %d\r\n", uart4.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tRx cmd timeouts: %d\r\n", uart4.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "\tTx timeouts: %d\r\n", uart4.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tTx queue overflows: %d\r\n", uart4.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "\tRx queue overflows: %d\r\n", uart4.rx_message_queue_overflow_errors);
	if (osMutexWait(uart4.rx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tRx Lock: Unlocked\r\n");
		osMutexRelease(uart4.rx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tRx Lock: Locked\r\n");
	}
	if (osMutexWait(uart4.tx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tTx Lock: Unlocked\r\n");
		osMutexRelease(uart4.tx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tTx Lock: Locked\r\n");
	}
#endif
#ifdef BSP_CONFIG_USE_UART5
	SHELL_Transmit(Shell, "UART5:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", uart5.overrun_errors);
	SHELL_Transmit(Shell, "\tParity errors: %d\r\n", uart5.parity_errors);
	SHELL_Transmit(Shell, "\tNoise errors: %d\r\n", uart5.noise_errors);
	SHELL_Transmit(Shell, "\tFrame errors: %d\r\n", uart5.frame_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", uart5.dma_errors);
	SHELL_Transmit(Shell, "\tRx timeouts: %d\r\n", uart5.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tRx cmd timeouts: %d\r\n", uart5.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "\tTx timeouts: %d\r\n", uart5.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tTx queue overflows: %d\r\n", uart5.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "\tRx queue overflows: %d\r\n", uart5.rx_message_queue_overflow_errors);
	if (osMutexWait(uart5.rx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tRx Lock: Unlocked\r\n");
		osMutexRelease(uart5.rx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tRx Lock: Locked\r\n");
	}
	if (osMutexWait(uart5.tx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tTx Lock: Unlocked\r\n");
		osMutexRelease(uart5.tx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tTx Lock: Locked\r\n");
	}
#endif
#ifdef BSP_CONFIG_USE_UART6
	SHELL_Transmit(Shell, "UART6:\r\n");
	SHELL_Transmit(Shell, "\tOverrun errors: %d\r\n", uart6.overrun_errors);
	SHELL_Transmit(Shell, "\tParity errors: %d\r\n", uart6.parity_errors);
	SHELL_Transmit(Shell, "\tNoise errors: %d\r\n", uart6.noise_errors);
	SHELL_Transmit(Shell, "\tFrame errors: %d\r\n", uart6.frame_errors);
	SHELL_Transmit(Shell, "\tDMA errors: %d\r\n", uart6.dma_errors);
	SHELL_Transmit(Shell, "\tRx timeouts: %d\r\n", uart6.rx_timeout_errors);
	SHELL_Transmit(Shell, "\tRx cmd timeouts: %d\r\n", uart6.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "\tTx timeouts: %d\r\n", uart6.tx_timeout_errors);
	SHELL_Transmit(Shell, "\tTx queue overflows: %d\r\n", uart6.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "\tRx queue overflows: %d\r\n", uart6.rx_message_queue_overflow_errors);
	if (osMutexWait(uart6.rx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tRx Lock: Unlocked\r\n");
		osMutexRelease(uart6.rx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tRx Lock: Locked\r\n");
	}
	if (osMutexWait(uart6.tx_mutex_id, 1) == osOK)
	{
		SHELL_Transmit(Shell, "\tTx Lock: Unlocked\r\n");
		osMutexRelease(uart6.tx_mutex_id);
	}
	else
	{
		SHELL_Transmit(Shell, "\tTx Lock: Locked\r\n");
	}
#endif
}
#endif

static void SHELL_CMD_StatusCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	char *Locked;
	uint32_t i;
	
	SHELL_Transmit(Shell,  "Device       Id    State InitCnt InitErr UpdCnt UpdErr Lock\r\n");
	for (i = 0; i < DevicesCnt; i++)
	{
		if (osMutexWait(Devices[i]->DevMutexId, 1) == osOK)
		{
			Locked = "Unlocked";
			osMutexRelease(Devices[i]->DevMutexId);
		}
		else
		{
			Locked = "Locked";
		}
		
		SHELL_Transmit(Shell,   "%12s %5s %7d %7d %6d %6d %s\r\n", 
			Devices[i]->DevName,
			BSP_DEV_GET_STATE_STR(Devices[i]),
			Devices[i]->InitCnt,
			Devices[i]->InitErr,
			Devices[i]->UpdateCnt,
			Devices[i]->UpdateErr,
			Locked);
	}
	for (i = 0; i < ModulesCnt; i++)
	{
		if (osMutexWait(Modules[i]->DevMutexId, 1) == osOK)
		{
			Locked = "Unlocked";
			osMutexRelease(Modules[i]->DevMutexId);
		}
		else
		{
			Locked = "Locked";
		}
		
		SHELL_Transmit(Shell,   "%12s %5s %7d %7d %6d %6d %s\r\n", 
			Modules[i]->DevName,
			BSP_DEV_GET_STATE_STR(Modules[i]),
			Modules[i]->InitCnt,
			Modules[i]->InitErr,
			Modules[i]->UpdateCnt,
			Modules[i]->UpdateErr,
			Locked);
	}
}

#define SHELL_CMD_USER_HELP \
"Usage:\r\n"\
"\tuser <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\treset - Reset to default users.\r\n"\
"\tr [<login>] - Read all users.\r\n"\
"\tw <login> [-p <pass>] [-g <group>] [-m <mask>] [-n <name>] [-e <email>] [-t <tel>] - Write user.\r\n"\
"\tc <login> -p <pass> -g <group> -m <mask> [-n <name>] [-e <email>] [-t <tel>] - Create user.\r\n"\
"\td <login> - Delete user.\r\n"

static void SHELL_CMD_USERCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t i;
	uint32_t len;
	USER_T User;
	uint32_t UserAttrMask, UsersCnt;
		
	if (CmdParamsCnt == 1)
	{
		UsersCnt = 0;
		for (i = 0; i < USER_MAX_CNT; i++)
		{
			if (USER_GetUserByIndex(i, &User))
				goto command_error;
			if (User.Login[0])
				UsersCnt++;
		}
		SHELL_Transmit(Shell, "Count: %d \r\n", UsersCnt);
		SHELL_Transmit(Shell, "Max count: %d \r\n", USER_MAX_CNT);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_USER_HELP, sizeof(SHELL_CMD_USER_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "r"))
		{
			for (i = 0; i < USER_MAX_CNT; i++)
			{
				if (USER_GetUserByIndex(i, &User))
					goto command_error;
				if (User.Login[0])
					SHELL_Transmit(Shell, "%s;*;%s;%s;%s;%s\r\n", User.Login, User.Group, User.Name, User.Email, User.Tel);
			}
		}
		else if (!strcmp(CmdParamsList[1], "reset"))
		{
			if (USER_Reset())
				goto command_error;
		}
		else
		{
			goto invalid_command;
		}
	}
	else if (CmdParamsCnt == 3)
	{
		if (!strcmp(CmdParamsList[1], "d"))
		{
			if (USER_DeleteUserByLogin(CmdParamsList[2]))
				goto command_error;
		}
		else if (!strcmp(CmdParamsList[1], "r"))
		{
			if (USER_GetUserByLogin(CmdParamsList[2], &User))
				goto invalid_login;
			SHELL_Transmit(Shell, "%s;*;%s;%s;%s;%s\r\n", User.Login, User.Group, User.Name, User.Email, User.Tel);
		}
		else 
		{
			goto invalid_command;
		}
	}
	else if (CmdParamsCnt > 3)
	{
		if (!strcmp(CmdParamsList[1], "w"))
		{
			if (USER_GetUserByLogin(CmdParamsList[2], &User))
				goto invalid_login;
			
			if ((CmdParamsCnt - 3) % 2)
				goto invalid_attribute;
			
			for (i = 3; i < CmdParamsCnt; i+=2)
			{
				if (!memcmp(CmdParamsList[i], "-p", sizeof("-p")))
				{
					len = strlen(CmdParamsList[i+1]);
					if ((len < 8) || (len > 128))
						goto invalid_password;
					if (USER_MD5CRYPT_GenPassHash(CmdParamsList[i+1], "bpVng.ZK", User.PassHash))
						goto command_error;
					if (USER_SNMPCRYPT_GenPassHash(CmdParamsList[i+1], User.SnmpHash))
						goto command_error;
				}
				else if (!memcmp(CmdParamsList[i], "-g", sizeof("-g")))
				{
					if (strlen(CmdParamsList[i+1]) >= sizeof(User.Group))
						goto invalid_group;
					strcpy(User.Group, CmdParamsList[i+1]);
				}
				else if (!memcmp(CmdParamsList[i], "-n", sizeof("-n")))
				{
					if (strlen(CmdParamsList[i+1]) >= sizeof(User.Name))
						goto invalid_name;
					strcpy(User.Name, CmdParamsList[i+1]);
				}
				else if (!memcmp(CmdParamsList[i], "-e", sizeof("-e")))
				{
					if (strlen(CmdParamsList[i+1]) >= sizeof(User.Email))
						goto invalid_email;
					strcpy(User.Email, CmdParamsList[i+1]);
				}
				else if (!memcmp(CmdParamsList[i], "-t", sizeof("-t")))
				{
					if (strlen(CmdParamsList[i+1]) >= sizeof(User.Tel))
						goto invalid_telephone;
					strcpy(User.Tel, CmdParamsList[i+1]);
				}
				else
				{
					goto invalid_attribute;
				}
			}
			
			if (USER_UpdateUserByLogin(CmdParamsList[2], &User))
				goto command_error;
		}
		else if ((!strcmp(CmdParamsList[1], "c")) && (CmdParamsCnt >= 6))
		{
			UserAttrMask = 0;
			
			memset(&User, 0, sizeof(User));
			
			if (strlen(CmdParamsList[2]) > sizeof(User.Login))
				goto invalid_login;
			strcpy(User.Login, CmdParamsList[2]);
			
			if ((CmdParamsCnt - 3) % 2)
				goto invalid_attribute;
			
			for (i = 3; i < CmdParamsCnt; i+=2)
			{
				if (!memcmp(CmdParamsList[i], "-p", sizeof("-p")))
				{
					len = strlen(CmdParamsList[i+1]);
					if ((len < 8) || (len > 128))
						goto invalid_password;
					if (USER_MD5CRYPT_GenPassHash(CmdParamsList[i+1], "bpVng.ZK", User.PassHash))
						goto command_error;
					if (USER_SNMPCRYPT_GenPassHash(CmdParamsList[i+1], User.SnmpHash))
						goto command_error;
					UserAttrMask |= 1 << 0;
				}
				else if (!memcmp(CmdParamsList[i], "-g", sizeof("-g")))
				{
					if (strlen(CmdParamsList[i+1]) >= sizeof(User.Group))
						goto invalid_group;
					strcpy(User.Group, CmdParamsList[i+1]);
					UserAttrMask |= 1 << 1;
				}
				else if (!memcmp(CmdParamsList[i], "-n", sizeof("-n")))
				{
					if (strlen(CmdParamsList[i+1]) >= sizeof(User.Name))
						goto invalid_name;
					strcpy(User.Name, CmdParamsList[i+1]);
					UserAttrMask |= 1 << 2;
				}
				else if (!memcmp(CmdParamsList[i], "-e", sizeof("-e")))
				{
					if (strlen(CmdParamsList[i+1]) >= sizeof(User.Email))
						goto invalid_email;
					strcpy(User.Email, CmdParamsList[i+1]);
					UserAttrMask |= 1 << 3;
				}
				else if (!memcmp(CmdParamsList[i], "-t", sizeof("-t")))
				{
					if (strlen(CmdParamsList[i+1]) >= sizeof(User.Tel))
						goto invalid_telephone;
					strcpy(User.Tel, CmdParamsList[i+1]);
					UserAttrMask |= 1 << 4;
				}
				else
				{
					goto invalid_attribute;
				}
			}
			
			if ((UserAttrMask & 0x03) != 0x03)
				goto invalid_command;
			
			if (USER_CreateUser(&User))
				goto command_error;
		}
		else 
		{
			goto invalid_command;
		}
	}
	else 
	{
		goto invalid_command;
	}
	
	return;
invalid_command:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
invalid_login:
	SHELL_TransmitBuffer(Shell, "Error: invalid login\r\n", sizeof("Error: invalid login\r\n") - 1);
	return;
invalid_password:
	SHELL_TransmitBuffer(Shell, "Error: invalid password\r\n", sizeof("Error: invalid password\r\n") - 1);
	return;
invalid_group:
	SHELL_TransmitBuffer(Shell, "Error: invalid group\r\n", sizeof("Error: invalid group\r\n") - 1);
	return;
invalid_name:
	SHELL_TransmitBuffer(Shell, "Error: invalid name\r\n", sizeof("Error: invalid name\r\n") - 1);
	return;
invalid_email:
	SHELL_TransmitBuffer(Shell, "Error: invalid email\r\n", sizeof("Error: invalid email\r\n") - 1);
	return;
invalid_telephone:
	SHELL_TransmitBuffer(Shell, "Error: invalid telephone\r\n", sizeof("Error: invalid telephone\r\n") - 1);
	return;
invalid_attribute:
	SHELL_TransmitBuffer(Shell, "Error: invalid attribute\r\n", sizeof("Error: invalid attribute\r\n") - 1);
	return;
command_error:
	SHELL_TransmitBuffer(Shell, "Error: command error\r\n", sizeof("Error: command error\r\n") - 1);
	return;
}

#define SHELL_CMD_PROFILE_HELP \
"Usage:\r\n"\
"\tprofile <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\ttext - Display profile text.\r\n"\
"\trp <section> <param> - Read param.\r\n"\
"\trv <section> <param> - Read param value.\r\n"\
"\twv <section> <param> <value> - Write param value.\r\n"\
"\trc <section> <param> - Read param colors.\r\n"\
"\twc <section> <param> <dcolor> <fcolor> - Write param colors.\r\n"\

static void SHELL_CMD_PROFILECallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	char *c;
	uint32_t i;
	uint32_t TableIdx;
	uint32_t ParamIdx;
	PROFILE_TABLE_T *Table;
	PROFILE_TABLE_PARAM_T *Param;
	char Value[PROFILE_STRING_MAX_LEN];
	char ValueStr[PROFILE_STRING_MAX_LEN];
	uint32_t ValueStrLen;
	uint8_t DegradeColor, FailureColor;
	uint8_t Access;
		
	if (CmdParamsCnt == 1)
	{
		for (i = 0; i <  Profile.Descr[Profile.Idx]->TablesCnt; i++)
		{
			SHELL_Transmit(Shell, "Section %d params count: %d \r\n", Profile.Descr[Profile.Idx]->Tables[i]->Index, Profile.Descr[Profile.Idx]->Tables[i]->ParamsCnt);
		}
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_PROFILE_HELP, sizeof(SHELL_CMD_PROFILE_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "text"))
		{
			for (i = 0; i < *Profile.Descr[Profile.Idx]->TextSize; i++)
			{
				SHELL_Transmit(Shell, "%s\r\n", Profile.Descr[Profile.Idx]->Text[i]);
			}
		}
		else
		{
			goto invalid_command;
		}
	}
	else if (CmdParamsCnt == 4)
	{
		if (!strcmp(CmdParamsList[1], "rp"))
		{
			TableIdx = strtoul(CmdParamsList[2], &c, 10);
			if (!TableIdx)
				goto invalid_section;
			
			if (PROFILE_GetTable(&Profile, TableIdx, &Table))
				goto invalid_param;
			
			ParamIdx = strtoul(CmdParamsList[3], &c, 10);
			if ((!ParamIdx) || (ParamIdx > Table->ParamsCnt))
				goto invalid_param;
			
			if (PROFILE_GetParam(&Profile, TableIdx, ParamIdx, &Param))
				goto command_error;
			
			SHELL_Transmit(Shell, "%s;%d;%d;%d\r\n", Param->Name, Param->Index, Param->Type, Param->Access);
		}
		else if (!strcmp(CmdParamsList[1], "rv"))
		{
			TableIdx = strtoul(CmdParamsList[2], &c, 10);
			if (!TableIdx)
				goto invalid_section;
			
			if (PROFILE_GetTable(&Profile, TableIdx, &Table))
				goto invalid_param;
			
			ParamIdx = strtoul(CmdParamsList[3], &c, 10);
			if ((!ParamIdx) || (ParamIdx > Table->ParamsCnt))
				goto invalid_param;
			
			if (PROFILE_GetValue(&Profile, TableIdx, ParamIdx, Value))
				goto command_error;
			if (PROFILE_ValueToString(&Profile, TableIdx, ParamIdx, Value, ValueStr, &ValueStrLen))
				goto command_error;
			
			SHELL_Transmit(Shell, "%s\r\n", ValueStr);
		}
		else if (!strcmp(CmdParamsList[1], "rc"))
		{
			TableIdx = strtoul(CmdParamsList[2], &c, 10);
			if (!TableIdx)
				goto invalid_section;
			
			if (PROFILE_GetTable(&Profile, TableIdx, &Table))
				goto invalid_param;
			
			ParamIdx = strtoul(CmdParamsList[3], &c, 10);
			if ((!ParamIdx) || (ParamIdx > Table->ParamsCnt))
				goto invalid_param;
			
			if (PROFILE_GetAlarmColors(&Profile, TableIdx, ParamIdx, &DegradeColor, &FailureColor))
				goto command_error;
			
			SHELL_Transmit(Shell, "%d;%d\r\n", DegradeColor, FailureColor);
		}
		else 
		{
			goto invalid_command;
		}
	}
	else if (CmdParamsCnt == 5)
	{
		if (!strcmp(CmdParamsList[1], "wv"))
		{
			TableIdx = strtoul(CmdParamsList[2], &c, 10);
			if (!TableIdx)
				goto invalid_section;
			
			if (PROFILE_GetTable(&Profile, TableIdx, &Table))
				goto invalid_param;
			
			ParamIdx = strtoul(CmdParamsList[3], &c, 10);
			if ((!ParamIdx) || (ParamIdx > Table->ParamsCnt))
				goto invalid_param;
			
			if (!strcmp(((SHELL_T*)Shell)->User.Group, USER_GROUP_MONITOR))
			{
				Access = PROFILE_ACCESS(PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE);
			}
			else if (!strcmp(((SHELL_T*)Shell)->User.Group, USER_GROUP_USER))
			{
				Access = PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE);
			}
			else if (!strcmp(((SHELL_T*)Shell)->User.Group, USER_GROUP_ADMIN))
			{
				Access = PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_WRITE);
			}
			else if (!strcmp(((SHELL_T*)Shell)->User.Group, USER_GROUP_SUPERUSER))
			{
				Access = PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE);
			}
			else
			{
				Access = PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE);
			}
			
			if (PROFILE_StringToValue(&Profile, TableIdx, ParamIdx, Value, CmdParamsList[4]))
				goto invalid_value;
			if (PROFILE_CheckValue(&Profile, TableIdx, ParamIdx, Value, 0))
				goto invalid_value;
			if (PROFILE_CheckWriteAccess(&Profile, TableIdx, ParamIdx, Access))
				goto access_error;
			if (PROFILE_SetValue(&Profile, TableIdx, ParamIdx, Value))
				goto command_error;
			PROFILE_SetValueDependencies(&Profile, TableIdx, ParamIdx, Value, 0);
			PROFILE_FLASH_UpdateValue(&ProfileFlash, TableIdx, ParamIdx);
		}
		else 
		{
			goto invalid_command;
		}
	}
	else if (CmdParamsCnt == 6)
	{
		if (!strcmp(CmdParamsList[1], "wc"))
		{
			TableIdx = strtoul(CmdParamsList[2], &c, 10);
			if (!TableIdx)
				goto invalid_section;
			
			if (PROFILE_GetTable(&Profile, TableIdx, &Table))
				goto invalid_param;
			
			ParamIdx = strtoul(CmdParamsList[3], &c, 10);
			if ((!ParamIdx) || (ParamIdx > Table->ParamsCnt))
				goto invalid_param;
			
			DegradeColor = strtoul(CmdParamsList[4], &c, 10);
			if (DegradeColor > PROFILE_COLOR_CRITICAL)
				goto invalid_value;
			
			FailureColor = strtoul(CmdParamsList[5], &c, 10);
			if (DegradeColor > PROFILE_COLOR_CRITICAL)
				goto invalid_value;
			
			if ((strcmp(((SHELL_T*)Shell)->User.Group, USER_GROUP_ADMIN)) && (strcmp(((SHELL_T*)Shell)->User.Group, USER_GROUP_SUPERUSER)))
				goto access_error;
			
			if (PROFILE_SetAlarmColors(&Profile, TableIdx, ParamIdx, DegradeColor, FailureColor))
				goto command_error;
			PROFILE_FLASH_UpdateConfigs(&ProfileFlash);
		}
		else 
		{
			goto invalid_command;
		}
	}
	else 
	{
		goto invalid_command;
	}
	
	return;
invalid_command:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
invalid_section:
	SHELL_TransmitBuffer(Shell, "Error: invalid section\r\n", sizeof("Error: invalid section\r\n") - 1);
	return;
invalid_param:
	SHELL_TransmitBuffer(Shell, "Error: invalid parameter\r\n", sizeof("Error: invalid parameter\r\n") - 1);
	return;
invalid_value:
	SHELL_TransmitBuffer(Shell, "Error: invalid value\r\n", sizeof("Error: invalid value\r\n") - 1);
	return;
command_error:
	SHELL_TransmitBuffer(Shell, "Error: command error\r\n", sizeof("Error: command error\r\n") - 1);
	return;
access_error:
	SHELL_TransmitBuffer(Shell, "Error: access error\r\n", sizeof("Error: access error\r\n") - 1);
	return;
}

#define SHELL_CMD_FLASH_HELP \
"Usage:\r\n"\
"\tflash <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\tupdate <file-name> <ip> - Update software via TFTP.\r\n"\
"\tload	<file-type> <file-name> <ip> - Read file via TFTP.\r\n"\
"\t\t<file-type>: lng - langpack file, web - web file.\r\n"\

static int32_t SHELL_CMD_FLASH_LOAD_WEB_TFTP_ReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{
	if (HTTP_SERVER_FILES_WriteData(((Block - 1) * TFTP_DATA_LEN_MAX), Data, DataSize))
		return -1;
	if (DataSize != TFTP_DATA_LEN_MAX)
	{
		if (HTTP_SERVER_FILES_WriteDataSize(((Block - 1) * TFTP_DATA_LEN_MAX) + DataSize))
			return -1;
	}
	return 0;
}

static int32_t SHELL_CMD_FLASH_LOAD_LANGPACK_TFTP_ReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{
	uint32_t DataAddr;
	uint32_t TotalDataSize;
	uint32_t Blocks;
	
	if ((((Block - 1) * TFTP_DATA_LEN_MAX) + DataSize) < PROFILE_FLASH_LANG_RECORD_BODY_SIZE)
	{
		DataAddr = (Block - 1) * TFTP_DATA_LEN_MAX;	
		if (PROFILE_FLASH_UpdateLangpackData(&ProfileFlash, DataAddr, Data, DataSize))
			return -1;
		if (DataSize != TFTP_DATA_LEN_MAX)
		{
			TotalDataSize = DataSize + ((Block - 1) * TFTP_DATA_LEN_MAX);
			Blocks = (TotalDataSize % PROFILE_FLASH_LANG_BLOCK_SIZE) ? ((TotalDataSize / PROFILE_FLASH_LANG_BLOCK_SIZE) + 1) : (TotalDataSize / PROFILE_FLASH_LANG_BLOCK_SIZE);
			if (PROFILE_FLASH_UpdateLangpackHeader(&ProfileFlash, Blocks, 1))
				return -1;
		}
		return 0;
	}
	return -1;
}

static int32_t SHELL_CMD_FLASH_UPDATE_TFTP_ReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{	
	if (BOOTLOADER_UpdateNewFwBlock(&Bootloader, Data, DataSize))
		return -1;
	if (DataSize != TFTP_DATA_LEN_MAX)
	{
		if (BOOTLOADER_UpdateNewFwFinish(&Bootloader))
			return -1;
		if (BOOTLOADER_CheckNewFw(&Bootloader))
			return -1;
		if (BOOTLOADER_UpdateNewFw(&Bootloader))
			return -1;
		BSP_Reset();
	}
	return 0;
}

static void SHELL_CMD_FLASHCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	char *c;
	uint32_t ip[4];
	ip_addr_t ip_addr;
	int32_t result;
		
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell, "Init state: %d (%s)\r\n", BSP_DEV_GET_STATE(&at45db641e), BSP_DEV_GET_STATE_STR(&at45db641e));
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_FLASH_HELP, sizeof(SHELL_CMD_FLASH_HELP) - 1);
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else if (CmdParamsCnt == 4)
	{
		if (!strcmp(CmdParamsList[1], "update"))
		{
			c = CmdParamsList[3];
			ip[0] = strtoul(c, &c, 10);
			if ((ip[0] == 0) || (ip[0] > 255) || (*c != '.'))
				goto invalid_ip_addr;
			c++;
			ip[1] = strtoul(c, &c, 10);
			if ((ip[1] > 255) || (*c != '.'))
				goto invalid_ip_addr;
			c++;
			ip[2] = strtoul(c, &c, 10);
			if ((ip[2] > 255) || (*c != '.'))
				goto invalid_ip_addr;
			c++;
			ip[3] = strtoul(c, &c, 10);
			if ((ip[3] == 0) || (ip[3] > 255) || (*c != 0))
				goto invalid_ip_addr;
			
			IP_ADDR4(&ip_addr, ip[0], ip[1], ip[2], ip[3]);	
			
			if (BOOTLOADER_UpdateNewFwStart(&Bootloader))
				goto download_error;
			
			result = TFTP_CLIENT_DownloadFile(CmdParamsList[2], &ip_addr, SHELL_CMD_FLASH_UPDATE_TFTP_ReceiveCallback, (void*)BSP_FLASH_FW_ADDR);
			if (result == TFTP_CLIENT_ERROR)
			{
				goto download_error;
			}
			else if (result == TFTP_CLIENT_TIMEOUT)
			{
				goto download_timeout;
			}
		}
		else 
		{
			goto invalid_cmd;
		}
	}
	else if (CmdParamsCnt == 5)
	{
		if (!strcmp(CmdParamsList[1], "load"))
		{
			c = CmdParamsList[4];
			ip[0] = strtoul(c, &c, 10);
			if ((ip[0] == 0) || (ip[0] > 255) || (*c != '.'))
				goto invalid_ip_addr;
			c++;
			ip[1] = strtoul(c, &c, 10);
			if ((ip[1] > 255) || (*c != '.'))
				goto invalid_ip_addr;
			c++;
			ip[2] = strtoul(c, &c, 10);
			if ((ip[2] > 255) || (*c != '.'))
				goto invalid_ip_addr;
			c++;
			ip[3] = strtoul(c, &c, 10);
			if ((ip[3] == 0) || (ip[3] > 255) || (*c != 0))
				goto invalid_ip_addr;
			
			IP_ADDR4(&ip_addr, ip[0], ip[1], ip[2], ip[3]);	
			
			if (!strcmp(CmdParamsList[2], "lng"))
				result = TFTP_CLIENT_DownloadFile(CmdParamsList[3], &ip_addr, SHELL_CMD_FLASH_LOAD_LANGPACK_TFTP_ReceiveCallback, NULL);
			else if (!strcmp(CmdParamsList[2], "web"))
				result = TFTP_CLIENT_DownloadFile(CmdParamsList[3], &ip_addr, SHELL_CMD_FLASH_LOAD_WEB_TFTP_ReceiveCallback, NULL);
			else
				goto invalid_file_type;
			if (result == TFTP_CLIENT_ERROR)
			{
				goto download_error;
			}
			else if (result == TFTP_CLIENT_TIMEOUT)
			{
				goto download_timeout;
			}
		}
		else 
		{
			goto invalid_cmd;
		}
	}
	else 
	{
		goto invalid_cmd;
	}
	
	return;
invalid_cmd:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
download_error:
	SHELL_TransmitBuffer(Shell, "Error: download error\r\n", sizeof("Error: download error\r\n") - 1);
	return;
download_timeout:
	SHELL_TransmitBuffer(Shell, "Error: download timeout\r\n", sizeof("Error: download timeout\r\n") - 1);
	return;
invalid_file_type:
	SHELL_TransmitBuffer(Shell, "Error: invalid file type\r\n", sizeof("Error: invalid file type\r\n") - 1);
	return;
invalid_ip_addr:
	SHELL_TransmitBuffer(Shell, "Error: invalid ip address\r\n", sizeof("Error: invalid ip address\r\n") - 1);
	return;
}

#ifdef BSP_PWR_IN_DIM3R3400_ENABLE 
#define SHELL_CMD_DIM3R_HELP \
"Usage:\r\n"\
"\tdim3r <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\tinit - Init device.\r\n"\
"\tmntr - Display device monitor values.\r\n"

static void SHELL_CMD_DIM3RCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell, "Init state: %s\r\n", BSP_DEV_GET_STATE_STR(&dim3r3400));
		SHELL_Transmit(Shell, "I2C Addr: 0x%x\r\n", dim3r3400.Addr >> 1);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_DIM3R_HELP, sizeof(SHELL_CMD_DIM3R_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "init"))
		{
			if (DIM3R3400_Init(&dim3r3400))
				goto command_error;
		}
		else if (!strcmp(CmdParamsList[1], "mntr"))
		{
			if (DIM3R3400_Update(&dim3r3400))
				goto command_error;
			SHELL_Transmit(Shell, "Status: 0x%02X\r\n", dim3r3400.Status);
			SHELL_Transmit(Shell, "VOUT: %2.2f\r\n", dim3r3400.Vout);
			SHELL_Transmit(Shell, "IOUT: %2.2f\r\n", dim3r3400.Iout);
			SHELL_Transmit(Shell, "VINA: %2.2f\r\n", dim3r3400.Vin[0]);
			SHELL_Transmit(Shell, "VINB: %2.2f\r\n", dim3r3400.Vin[1]);
			SHELL_Transmit(Shell, "Temperature: %2.2f\r\n", dim3r3400.Temp);
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else 
	{
		goto invalid_cmd;
	}
	
	return;
invalid_cmd:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
command_error:
	SHELL_TransmitBuffer(Shell, "Error: command error\r\n", sizeof("Error: command error\r\n") - 1);
	return;
}
#endif

#define SHELL_CMD_PCA9536_HELP \
"Usage:\r\n"\
"\tpca9536 <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\tinit - Init device.\r\n"\
"\tpin - Display device pin states.\r\n"\
"\tout <pin> <data> - Set device pin output.\r\n"\
"\tdir <pin> <data> - Set device pin direction.\r\n"\
"\tpol <pin> <data> - Set device pin polarity.\r\n"

static const char *shell_pca9536_pin_info[4] = 
{
	"UCD_RESET",
	"UCD_CONTROL",
	"12V_ENABLE",
	""
};

static void SHELL_CMD_PCA9536Callback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t i, Pin, Data;
	char *p;
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell, "Init state: %s\r\n", BSP_DEV_GET_STATE_STR(&pwr_pca9536));
		SHELL_Transmit(Shell, "I2C Addr: 0x%x\r\n", PCA9536_I2C_ADDR >> 1);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_PCA9536_HELP, sizeof(SHELL_CMD_PCA9536_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "init"))
		{
			if (PCA9536_Init(&pwr_pca9536))
					goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "pin"))
		{
			if (PCA9536_GetBankDir(&pwr_pca9536))
				goto cmd_err;
			if (PCA9536_GetBankIn(&pwr_pca9536))
				goto cmd_err;
			if (PCA9536_GetBankOut(&pwr_pca9536))
				goto cmd_err;
			if (PCA9536_GetBankPol(&pwr_pca9536))
				goto cmd_err;
			SHELL_Transmit(Shell,   "Idx  In  Dir  Out  Pol  Info\r\n");
			for (i = 0; i < 4; i++)
			{
				SHELL_Transmit(Shell,   "%3d  %2d  %3d  %3d  %3d  %s\r\n",
					i,
					(pwr_pca9536.Ins >> (i % 8)) & 0x01,
					(pwr_pca9536.Dirs >> (i % 8)) & 0x01,
					(pwr_pca9536.Outs >> (i % 8)) & 0x01,
					(pwr_pca9536.Pols >> (i % 8)) & 0x01,
					shell_pca9536_pin_info[i]);
			}
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else if (CmdParamsCnt == 4)
	{
		if (!strcmp(CmdParamsList[1], "out"))
		{
			Pin = strtoul(CmdParamsList[2], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_pin;
			if (Pin >= 4)
				goto invalid_pin;
			Data = strtoul(CmdParamsList[3], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[3]))
				goto invalid_data;
			if (Data > 1)
				goto invalid_data;
			if (PCA9536_SetPinOut(&pwr_pca9536, Pin, Data))
				goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "dir"))
		{
			Pin = strtoul(CmdParamsList[2], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_pin;
			if (Pin >= 4)
				goto invalid_pin;
			Data = strtoul(CmdParamsList[3], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[3]))
				goto invalid_data;
			if (Data > 1)
				goto invalid_data;
			if (PCA9536_SetPinDir(&pwr_pca9536, Pin, Data))
				goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "pol"))
		{
			Pin = strtoul(CmdParamsList[2], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_pin;
			if (Pin >= 4)
				goto invalid_pin;
			Data = strtoul(CmdParamsList[3], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[3]))
				goto invalid_data;
			if (Data > 1)
				goto invalid_data;
			if (PCA9536_SetPinPol(&pwr_pca9536, Pin, Data))
				goto cmd_err;
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else 
	{
		goto invalid_cmd;
	}
	
	return;
invalid_cmd:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
invalid_pin:
	SHELL_TransmitBuffer(Shell, "Error: invalid pin\r\n", sizeof("Error: invalid pin\r\n") - 1);
	return;
invalid_data:
	SHELL_TransmitBuffer(Shell, "Error: invalid data\r\n", sizeof("Error: invalid data\r\n") - 1);
	return;
cmd_err:
	SHELL_TransmitBuffer(Shell, "Error: command error\r\n", sizeof("Error: command error\r\n") - 1);
	return;
}

#define SHELL_CMD_PCAL6524_HELP \
"Usage:\r\n"\
"\tpcal6524 <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\tinit - Init device.\r\n"\
"\tpin - Display device pin states.\r\n"\
"\tout <pin> <data> - Set device pin output.\r\n"\
"\tdir <pin> <data> - Set device pin direction.\r\n"\
"\tpol <pin> <data> - Set device pin polarity.\r\n"

static const char *shell_pcal6524_pin_info[BSP_SFP_PIN_PCAL6524_CNT][PCAL6524_BANKS_CNT * 8] = 
{
	{
		"CLIENT1_SFP_LINK_LED",
		"CLIENT2_SFP_LINK_LED",
		"CLIENT3_SFP_LINK_LED",
		"CLIENT4_SFP_LINK_LED",
		"CLIENT5_SFP_LINK_LED",
		"CLIENT6_SFP_LINK_LED",
		"CLIENT7_SFP_LINK_LED",
		"CLIENT8_SFP_LINK_LED",
		"CLIENT9_SFP_LINK_LED",
		"CLIENT10_SFP_LINK_LED",
		"CLIENT11_SFP_LINK_LED",
		"CLIENT12_SFP_LINK_LED",
		"CLIENT13_SFP_LINK_LED",
		"CLIENT14_SFP_LINK_LED",
		"CLIENT15_SFP_LINK_LED",
		"CLIENT16_SFP_LINK_LED",
		"LINE1_SFP_LINK_LED",
		"LINE2_SFP_LINK_LED",
		"LINE3_SFP_LINK_LED",
		"LINE4_SFP_LINK_LED",
		"",
		"",
		"",
		"",
	},
	{
		"CLIENT1_SFP_TX_FAULT",
		"CLIENT2_SFP_TX_FAULT",
		"CLIENT3_SFP_TX_FAULT",
		"CLIENT4_SFP_TX_FAULT",
		"CLIENT5_SFP_TX_FAULT",
		"CLIENT6_SFP_TX_FAULT",
		"CLIENT7_SFP_TX_FAULT",
		"CLIENT8_SFP_TX_FAULT",
		"CLIENT9_SFP_TX_FAULT",
		"CLIENT10_SFP_TX_FAULT",
		"CLIENT11_SFP_TX_FAULT",
		"CLIENT12_SFP_TX_FAULT",
		"CLIENT13_SFP_TX_FAULT",
		"CLIENT14_SFP_TX_FAULT",
		"CLIENT15_SFP_TX_FAULT",
		"CLIENT16_SFP_TX_FAULT",
		"LINE1_SFP_TX_FAULT",
		"LINE2_SFP_TX_FAULT",
		"LINE3_SFP_TX_FAULT",
		"LINE4_SFP_TX_FAULT",
		"",
		"",
		"",
		"",
	},
	{
		"CLIENT1_SFP_TX_DISABLE",
		"CLIENT2_SFP_TX_DISABLE",
		"CLIENT3_SFP_TX_DISABLE",
		"CLIENT4_SFP_TX_DISABLE",
		"CLIENT5_SFP_TX_DISABLE",
		"CLIENT6_SFP_TX_DISABLE",
		"CLIENT7_SFP_TX_DISABLE",
		"CLIENT8_SFP_TX_DISABLE",
		"CLIENT9_SFP_TX_DISABLE",
		"CLIENT10_SFP_TX_DISABLE",
		"CLIENT11_SFP_TX_DISABLE",
		"CLIENT12_SFP_TX_DISABLE",
		"CLIENT13_SFP_TX_DISABLE",
		"CLIENT14_SFP_TX_DISABLE",
		"CLIENT15_SFP_TX_DISABLE",
		"CLIENT16_SFP_TX_DISABLE",
		"LINE1_SFP_TX_DISABLE",
		"LINE2_SFP_TX_DISABLE",
		"LINE3_SFP_TX_DISABLE",
		"LINE4_SFP_TX_DISABLE",
		"",
		"",
		"",
		"",
	},
};

static void SHELL_CMD_PCAL6524Callback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	static uint32_t Idx = 0;
	uint32_t i, Pin, Data;
	char *p;
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell,   "Init state: %s\r\n", BSP_DEV_GET_STATE_STR(&pcal6524[Idx]));
		SHELL_Transmit(Shell,   "I2C Addr: 0x%x\r\n", pcal6524[Idx].Addr >> 1);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_PCAL6524_HELP, sizeof(SHELL_CMD_PCAL6524_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "init"))
		{
			if (PCAL6524_Init(&pcal6524[Idx]))
					goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "pin"))
		{
			for (i = 0; i < BSP_SFP_PIN_PCAL6524_CNT; i++)
			{
				if (PCAL6524_GetBankIn(&pcal6524[Idx], i))
					goto cmd_err;
				if (PCAL6524_GetBankOut(&pcal6524[Idx], i))
					goto cmd_err;
				if (PCAL6524_GetBankCfg(&pcal6524[Idx], i))
					goto cmd_err;
				if (PCAL6524_GetBankPol(&pcal6524[Idx], i))
					goto cmd_err;
			}
			SHELL_Transmit(Shell,   "Idx  In  Cfg  Out  Pol  Info\r\n");
			for (i = 0; i < PCAL6524_BANKS_CNT * 8; i++)
			{
				SHELL_Transmit(Shell,   "%3d  %2d  %3d  %3d  %3d  %s\r\n",
					i,
					(pcal6524[Idx].Ins[i / 8] >> (i % 8)) & 0x01,
					(pcal6524[Idx].Cfgs[i / 8] >> (i % 8)) & 0x01,
					(pcal6524[Idx].Outs[i / 8] >> (i % 8)) & 0x01,
					(pcal6524[Idx].Pols[i / 8] >> (i % 8)) & 0x01,
					shell_pcal6524_pin_info[Idx][i]);
			}
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else if (CmdParamsCnt == 3)
	{
		if (!strcmp(CmdParamsList[1], "select"))
		{
			Data = strtoul(CmdParamsList[2], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_cmd;
			if (Data >= BSP_SFP_PIN_PCAL6524_CNT)
				goto invalid_idx;
			Idx = Data;
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else if (CmdParamsCnt == 4)
	{
		if (!strcmp(CmdParamsList[1], "out"))
		{
			Pin = strtoul(CmdParamsList[2], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_pin;
			if (Pin >= PCAL6524_BANKS_CNT * 8)
				goto invalid_pin;
			Data = strtoul(CmdParamsList[3], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[3]))
				goto invalid_data;
			if (Data > 1)
				goto invalid_data;
			if (PCAL6524_SetPinOut(&pcal6524[Idx], Pin / 8, Pin % 8, Data))
				goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "dir"))
		{
			Pin = strtoul(CmdParamsList[2], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_pin;
			if (Pin >= PCAL6524_BANKS_CNT * 8)
				goto invalid_pin;
			Data = strtoul(CmdParamsList[3], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[3]))
				goto invalid_data;
			if (Data > 1)
				goto invalid_data;
			if (PCAL6524_SetPinCfg(&pcal6524[Idx], Pin / 8, Pin % 8, Data))
				goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "pol"))
		{
			Pin = strtoul(CmdParamsList[2], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_pin;
			if (Pin >= PCAL6524_BANKS_CNT * 8)
				goto invalid_pin;
			Data = strtoul(CmdParamsList[3], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[3]))
				goto invalid_data;
			if (Data > 1)
				goto invalid_data;
			if (PCAL6524_SetPinPol(&pcal6524[Idx], Pin / 8, Pin % 8, Data))
				goto cmd_err;
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else 
	{
		goto invalid_cmd;
	}
	
	return;
invalid_cmd:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
invalid_idx:
	SHELL_TransmitBuffer(Shell, "Error: invalid index\r\n", sizeof("Error: invalid index\r\n") - 1);
	return;
invalid_pin:
	SHELL_TransmitBuffer(Shell, "Error: invalid pin\r\n", sizeof("Error: invalid pin\r\n") - 1);
	return;
invalid_data:
	SHELL_TransmitBuffer(Shell, "Error: invalid data\r\n", sizeof("Error: invalid data\r\n") - 1);
	return;
cmd_err:
	SHELL_TransmitBuffer(Shell, "Error: command error\r\n", sizeof("Error: command error\r\n") - 1);
	return;
}

#define SHELL_CMD_PCA9548A_HELP \
"Usage:\r\n"\
"\tpca9548a <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\tinit - Init device.\r\n"\
"\tselect <idx> - Select device. <idx>: 0, 1, 2\r\n"\

static void SHELL_CMD_PCA9548ACallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	static uint32_t Idx = 0;
	uint32_t Data;
	char *p;
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell,   "Init state: %s\r\n", BSP_DEV_GET_STATE_STR(&pca9548a[Idx]));
		SHELL_Transmit(Shell,   "I2C Addr: 0x%x\r\n", pca9548a[Idx].Addr >> 1);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_PCA9548A_HELP, sizeof(SHELL_CMD_PCA9548A_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "init"))
		{
			if (PCA9548A_Init(&pca9548a[Idx]))
				goto cmd_err;
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else if (CmdParamsCnt == 3)
	{
		if (!strcmp(CmdParamsList[1], "select"))
		{
			Data = strtoul(CmdParamsList[2], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_cmd;
			if (Data >= BSP_PCA9548A_CNT)
				goto invalid_idx;
			Idx = Data;
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else 
	{
		goto invalid_cmd;
	}
	
	return;
invalid_cmd:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
invalid_idx:
	SHELL_TransmitBuffer(Shell, "Error: invalid index\r\n", sizeof("Error: invalid index\r\n") - 1);
	return;
cmd_err:
	SHELL_TransmitBuffer(Shell, "Error: command error\r\n", sizeof("Error: command error\r\n") - 1);
	return;
}

#define SHELL_CMD_SFP_HELP \
"Usage:\r\n"\
"\tsfp <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\tselect <idx> - Select device. <idx>: 0, ... ,19.\r\n"\
"\tinit - Init device.\r\n"\
"\tinfo - Display device info values.\r\n"\
"\tupdate - Update device.\r\n"\
"\tmntr - Display device monitor values.\r\n"\
"\tpin - Display device pin states.\r\n"\
"\ttx <state> - Set device tx enable. <state>: 0 - Disable, 1 - Enable.\r\n"

static void SHELL_CMD_SFPCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t Data;
	static uint32_t Idx = 0;
	char *p;
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell, "Index: %d\r\n", Idx);
		SHELL_Transmit(Shell, "Init state: %d (%s)\r\n", BSP_DEV_GET_STATE(&sfp[Idx]), BSP_DEV_GET_STATE_STR(&sfp[Idx]));
		SHELL_Transmit(Shell, "Tx Disable: %d (%s)\r\n", sfp[Idx].Pin.TxDisable, sfp[Idx].Pin.TxDisable ? "Disable" : "Enable");
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_SFP_HELP, sizeof(SHELL_CMD_SFP_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "info"))
		{
			if (BSP_DEV_GET_STATE(&sfp[Idx]) != BSP_DEV_STATE_OK)
				goto invalid_state;
			SHELL_Transmit(Shell, "Vendor: %s\r\n", sfp[Idx].Info.Vendor);
			SHELL_Transmit(Shell, "Part Number: %s\r\n", sfp[Idx].Info.PartNumber);
			SHELL_Transmit(Shell, "Serial Number: %s\r\n", sfp[Idx].Info.SerialNumber);
		}
		else if (!strcmp(CmdParamsList[1], "init"))
		{
			if (SFP_Init(&sfp[Idx]))
				goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "update"))
		{
			if (SFP_Update(&sfp[Idx]))
				goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "pin"))
		{
			if (BSP_DEV_GET_STATE(&sfp[Idx]) != BSP_DEV_STATE_OK)
				goto invalid_state;
			SHELL_Transmit(Shell, "MOD_ABS  RX_LOS  TX_DIS  TX_FAULT\r\n");
			SHELL_Transmit(Shell, "   %d       %d       %d     %d\r\n",
				sfp[Idx].Pin.ModAbs, sfp[Idx].Pin.RxLos, sfp[Idx].Pin.TxDisable, sfp[Idx].Pin.TxFault);
		}
		else if (!strcmp(CmdParamsList[1], "mntr"))
		{
			if (BSP_DEV_GET_STATE(&sfp[Idx]) != BSP_DEV_STATE_OK)
				goto invalid_state;		
			SHELL_Transmit(Shell, "Rx Power: %.2f\r\n", sfp[Idx].RxPwr);	
			SHELL_Transmit(Shell, "Tx Power: %.2f\r\n", sfp[Idx].TxPwr);
			SHELL_Transmit(Shell, "Temperature: %.2f\r\n", sfp[Idx].Temp);		
		}
		else
		{
			goto cmd_err;
		}
	}
	else if (CmdParamsCnt == 3)
	{
		if (!strcmp(CmdParamsList[1], "select"))
		{
			Data = strtoul(CmdParamsList[2], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_idx;
			if (Data >= BSP_SFP_CNT)
				goto invalid_idx;
			Idx = Data;
		}
		else if (!strcmp(CmdParamsList[1], "tx"))
		{
			Data = strtoul(CmdParamsList[2], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_cmd;
			if ((Data != 0) && (Data != 1))
				goto invalid_cmd;			
			if (SFP_SetTxDisable(&sfp[Idx], Data ? 0 : 1))
				goto cmd_err;
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else 
	{
		goto invalid_cmd;
	}
	
	return;
invalid_cmd:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
invalid_idx:
	SHELL_TransmitBuffer(Shell, "Error: invalid index\r\n", sizeof("Error: invalid index\r\n") - 1);
	return;
cmd_err:
	SHELL_TransmitBuffer(Shell, "Error: command error\r\n", sizeof("Error: command error\r\n") - 1);
	return;
invalid_state:
	SHELL_TransmitBuffer(Shell, "Error: invalid module state\r\n", sizeof("Error: invalid module state\r\n") - 1);
	return;
}

#define SHELL_CMD_FPGA_HELP \
"Usage:\r\n"\
"\tfpga <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\tselect <dev> - Select fpga device. <dev>: 0 - FPGA Line, 1 - FPGA Client 1, 2 - FPGA Client 2, 3 - MAXV\r\n"\
"\ttest - Test fpga device.\r\n"\
"\t\tExample: fpga test 0x01 0x10\r\n"\
"\tr <addr> - Read device register.\r\n"\
"\t\tExample: fpga r 0x01\r\n"\
"\tw <addr> <data> - Write device register.\r\n"\
"\t\tExample: fpga w 0x01 0x10\r\n"\
"\tupdate <file1> <file2> <ip> - Update FPGA firmware via TFTP.\r\n"\

static int32_t SHELL_CMD_FPGADownloadImageReceiveCallback(void *Priv, uint16_t Block, char *Data, uint32_t DataSize)
{
	uint32_t Idx;
	Idx = (uint32_t)Priv;
	if (Idx >= 2)
		return -1;
	if (DataSize)
	{
		if (BSP_FPGA_FLASH_WriteSection(BSP_FLASH_FPGA_BOOT_SECTION, (0x2000000 * Idx) + ((Block - 1) * TFTP_DATA_LEN_MAX), Data, DataSize))
			return -1;
	}
	return 0;
}

static void SHELL_CMD_FPGACallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	static uint8_t Dev = 3;
	uint32_t Addr, Data, i;
	char *p;
	uint32_t ip[4];
	ip_addr_t ip_addr;
	int32_t result;
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell, "Init state: %s\r\n", BSP_DEV_GET_STATE_STR(&fpga_maxv));
		SHELL_Transmit(Shell, "Device: %d\r\n", Dev);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_FPGA_HELP, sizeof(SHELL_CMD_FPGA_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "test"))
		{
			if (Dev == 0)
			{
				Addr = BSP_FPGA_LINE_BASE_ADDR + 0xF0;
			}
			else if (Dev == 1)
			{
				Addr = BSP_FPGA_CLIENT_BASE_ADDR(0) + 0xF0;
			}
			else if (Dev == 2)
			{
				Addr = BSP_FPGA_CLIENT_BASE_ADDR(1) + 0xF0;
			}
			else
			{
				goto invalid_dev;
			}
			
			srand(0);
			for (i = 0; i < 16; i++)
			{
				*(uint8_t*)(Addr + i)	 = rand() % 255;		
			}
			
			srand(0);
			for (i = 0; i < 16; i++)
			{
				Data = *(uint8_t*)(Addr + i);
				if (Data != rand() % 255)
				{
					SHELL_Transmit(Shell, "Test Failed: %d\r\n", i);
					return;
				}
			}
			SHELL_Transmit(Shell, "Test Successful\r\n", Addr);
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else if (CmdParamsCnt == 3)
	{
		if (!strcmp(CmdParamsList[1], "r"))
		{
			if ((CmdParamsList[2][0] != '0') || (CmdParamsList[2][1] != 'x'))
				goto invalid_addr;
			
			Addr = strtoul(&CmdParamsList[2][2], &p, 16);
			if ((*p != 0) || (p == &CmdParamsList[2][2]))
				goto invalid_addr;
			
			if (Addr > 0x2000)
				goto invalid_addr;
			
			if (Dev == 0)
			{
				Data = *(uint8_t*)(BSP_FPGA_LINE_BASE_ADDR + Addr);
			}
			else if (Dev == 1)
			{
				Data = *(uint8_t*)(BSP_FPGA_CLIENT_BASE_ADDR(0) + Addr);
			}
			else if (Dev == 2)
			{
				Data = *(uint8_t*)(BSP_FPGA_CLIENT_BASE_ADDR(1) + Addr);
			}
			else if (Dev == 3)
			{
				Data = *(uint8_t*)(BSP_FPGA_MAXV_BASE_ADDR + Addr);
			}
			else
			{
				goto invalid_dev;
			}
			
			SHELL_Transmit(Shell, "0x%x\r\n", Data);
		}
		else if (!strcmp(CmdParamsList[1], "select"))
		{
			Data = strtoul(&CmdParamsList[2][0], &p, 10);
			if ((*p != 0) || (p == &CmdParamsList[2][2]))
				goto invalid_dev;
			
			if (Data > 3)
				goto invalid_dev;
			
			Dev = Data;
		}
		else
		{
			goto invalid_dev;
		}
	}
	else if (CmdParamsCnt == 4)
	{
		if (!strcmp(CmdParamsList[1], "update"))
		{			
			p = CmdParamsList[4];
			ip[0] = strtoul(p, &p, 10);
			if ((ip[0] == 0) || (ip[0] > 255) || (*p != '.'))
				goto invalid_ip_addr;
			p++;
			ip[1] = strtoul(p, &p, 10);
			if ((ip[1] > 255) || (*p != '.'))
				goto invalid_ip_addr;
			p++;
			ip[2] = strtoul(p, &p, 10);
			if ((ip[2] > 255) || (*p != '.'))
				goto invalid_ip_addr;
			p++;
			ip[3] = strtoul(p, &p, 10);
			if ((ip[3] == 0) || (ip[3] > 255) || (*p != 0))
				goto invalid_ip_addr;
			
			IP_ADDR4(&ip_addr, ip[0], ip[1], ip[2], ip[3]);			
			
			result = TFTP_CLIENT_DownloadFile(CmdParamsList[2], &ip_addr, SHELL_CMD_FPGADownloadImageReceiveCallback, (void*)0);
			if (result == TFTP_CLIENT_TIMEOUT)
			{
				goto download_timeout;
			}
			else if (result == TFTP_CLIENT_ERROR)
			{
				goto download_error;
			}
			
			result = TFTP_CLIENT_DownloadFile(CmdParamsList[3], &ip_addr, SHELL_CMD_FPGADownloadImageReceiveCallback, (void*)1);
			if (result == TFTP_CLIENT_TIMEOUT)
			{
				goto download_timeout;
			}
			else if (result == TFTP_CLIENT_ERROR)
			{
				goto download_error;
			}
		}
		else if (!strcmp(CmdParamsList[1], "w"))
		{
			if ((CmdParamsList[2][0] != '0') || (CmdParamsList[2][1] != 'x'))
				goto invalid_addr;
			
			Addr = strtoul(&CmdParamsList[2][2], &p, 16);
			if ((*p != 0) || (p == &CmdParamsList[2][2]))
				goto invalid_addr;
			
			if (Addr > 0x2000)
				goto invalid_addr;
			
			if ((CmdParamsList[3][0] != '0') || (CmdParamsList[3][1] != 'x'))
				goto invalid_data;
			
			Data = strtoul(&CmdParamsList[3][2], &p, 16);
			if ((*p != 0) || (p == &CmdParamsList[3][2]))
				goto invalid_data;
			
			if (Data > 0xFF)
				goto invalid_addr;
			
			if (Dev == 0)
			{
				*(uint8_t*)(BSP_FPGA_LINE_BASE_ADDR + Addr) = Data;
			}
			else if (Dev == 1)
			{
				*(uint8_t*)(BSP_FPGA_CLIENT_BASE_ADDR(0) + Addr) = Data;
			}
			else if (Dev == 2)
			{
				*(uint8_t*)(BSP_FPGA_CLIENT_BASE_ADDR(1) + Addr) = Data;
			}
			else if (Dev == 3)
			{
				*(uint8_t*)(BSP_FPGA_MAXV_BASE_ADDR + Addr) = Data;
			}
			else
			{
				goto invalid_dev;
			}
		}
		else
		{
			goto invalid_cmd;
		}	
	}
	else 
	{
		goto invalid_cmd;
	}
	
	return;
invalid_cmd:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
invalid_ip_addr:
	SHELL_TransmitBuffer(Shell, "Error: invalid ip address\r\n", sizeof("Error: invalid ip address\r\n") - 1);
	return;
invalid_addr:
	SHELL_TransmitBuffer(Shell, "Error: invalid address\r\n", sizeof("Error: invalid address\r\n") - 1);
	return;
invalid_data:
	SHELL_TransmitBuffer(Shell, "Error: invalid data\r\n", sizeof("Error: invalid data\r\n") - 1);
	return;
invalid_dev:
	SHELL_TransmitBuffer(Shell, "Error: invalid device\r\n", sizeof("Error: invalid device\r\n") - 1);
	return;
download_error:
	SHELL_TransmitBuffer(Shell, "Error: download error\r\n", sizeof("Error: download error\r\n") - 1);
	return;
download_timeout:
	SHELL_TransmitBuffer(Shell, "Error: download timeout\r\n", sizeof("Error: download timeout\r\n") - 1);
	return;
}

#define SHELL_CMD_UCD_HELP \
"Usage:\r\n"\
"\tucd <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\tinit - Init device.\r\n"\
"\tmntr - Display device monitor values.\r\n"\
"\tlog  - Display device log.\r\n"\
"\tmntr - Clear device log.\r\n"

static void SHELL_CMD_UCDCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t i;
	uint32_t LogSize;
	UCD90120A_FAULT_LOG_RECORD_T LogRecord;
	uint8_t Log[14];
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell, "Init state: %s\r\n", BSP_DEV_GET_STATE_STR(&ucd90120a));
		SHELL_Transmit(Shell, "I2C Addr: 0x%x\r\n", ucd90120a.Addr);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_UCD_HELP, sizeof(SHELL_CMD_UCD_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "init"))
		{
			if (UCD90120A_Init(&ucd90120a, BSP_UCD90120A_CFG_Table, BSP_UCD90120A_CFG_TableSize))
				goto command_error;
		}
		else if (!strcmp(CmdParamsList[1], "mntr"))
		{
			if (UCD90120A_Update(&ucd90120a))
				goto command_error;
			SHELL_Transmit(Shell, "Device temp: %2.2f\r\n", ucd90120a.Mon.Temp);
			SHELL_Transmit(Shell, "Rail   Vout    Iout    Temp    Status\r\n");
			for (i = 0; i < UCD90120A_RAILS_CNT; i++)
			{
				SHELL_Transmit(Shell, " %2d    %2.2f    %2.2f    %2.2f    %02x\r\n", i + 1, ucd90120a.Mon.Rail[i].Vout, ucd90120a.Mon.Rail[i].Iout, ucd90120a.Mon.Rail[i].Temp, ucd90120a.Mon.Rail[i].StatusVout);
			}
		}
		else if (!strcmp(CmdParamsList[1], "log"))
		{
			if (UCD90120A_ReadFaultLog(&ucd90120a, Log))
				goto command_error;
			SHELL_Transmit(Shell, "Non-Paged faults: 0x%x\r\n", Log[0]);
			SHELL_Transmit(Shell, "GPI faults: 0x%x\r\n", Log[1]);
			for (i = 0; i < UCD90120A_RAILS_CNT; i++)
			{
				SHELL_Transmit(Shell, "Rail %d faults: 0x%x\r\n", i + 1, Log[i+2]);
			}
			if (UCD90120A_ReadFaultLogSize(&ucd90120a, &LogSize))
				goto command_error;
			SHELL_Transmit(Shell, "Log records: %d\r\n", LogSize);
			for (i = 0; i < LogSize; i++)
			{
				if (UCD90120A_ReadFaultLogRecord(&ucd90120a, i, &LogRecord))
					goto command_error;
				switch (LogRecord.Type)
				{
					case UCD90120A_FAULT_LOG_RECORD_TYPE_SWD:
						SHELL_Transmit(Shell, "%d-%d: System Watchdog Timeout\r\n", LogRecord.Day, LogRecord.Time);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_SEQ:
						SHELL_Transmit(Shell, "%d-%d: Resequense Error\r\n", LogRecord.Day, LogRecord.Time);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_WD:
						SHELL_Transmit(Shell, "%d-%d: Watchdog Timeout\r\n", LogRecord.Day, LogRecord.Time);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_FAN:
						SHELL_Transmit(Shell, "%d-%d: Fan Fault\r\n", LogRecord.Day, LogRecord.Time);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_GPI:
						SHELL_Transmit(Shell, "%d-%d: GPI Fault\r\n", LogRecord.Day, LogRecord.Time);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_VOUT_OV:
						SHELL_Transmit(Shell, "%d-%d: Rail %d VOUT_OV %.1fV\r\n", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_VOUT_UV:
						SHELL_Transmit(Shell, "%d-%d: Rail %d VOUT_UV %.1fV\r\n", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_TON_MAX:
						SHELL_Transmit(Shell, "%d-%d: Rail %d TON_MAX %.1fV\r\n", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_IOUT_OC:
						SHELL_Transmit(Shell, "%d-%d: Rail %d IOUT_OC %.1fA\r\n", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_IOUT_UC:
						SHELL_Transmit(Shell, "%d-%d: Rail %d IOUT_UC %.1fA\r\n", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_TEMP_OT:
						SHELL_Transmit(Shell, "%d-%d: Rail %d TEMPERATURE_OT %.1fA\r\n", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_SEQ_ON:
						SHELL_Transmit(Shell, "%d-%d: Rail %d Sequence On Timeout 0x%x\r\n", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.u);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_SEQ_OFF:
						SHELL_Transmit(Shell, "%d-%d: Rail %d Sequence Off Timeout 0x%x\r\n", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.u);
						break;
					default:
						SHELL_Transmit(Shell, "%d-%d: Rail %d Reserved(0x%x) 0x%x\r\n", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Type, LogRecord.Value.u);
						break;
				}
			}
		}
		else if (!strcmp(CmdParamsList[1], "clr"))
		{
			if (UCD90120A_ClearFaultLog(&ucd90120a))
				goto command_error;
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else 
	{
		goto invalid_cmd;
	}
	
	return;
invalid_cmd:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
command_error:
	SHELL_TransmitBuffer(Shell, "Error: command error\r\n", sizeof("Error: command error\r\n") - 1);
	return;
}

#define SHELL_CMD_GDT_HELP \
"Usage:\r\n"\
"\tgdt <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\tselect <idx> - Select device.\r\n"\
"\tmntr - Display device monitor values.\r\n"\
"\tvout <data> - Set device Vout value.\r\n"

static void SHELL_CMD_GDTCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t Data;
	float Vout;
	static uint32_t Idx = 0;
	char *p;
	
	if (CmdParamsCnt == 1)
	{	
		SHELL_Transmit(Shell, "Index: %d\r\n", Idx);
		SHELL_Transmit(Shell, "Init state: %s\r\n", BSP_DEV_GET_STATE_STR(&gdt080a[Idx]));
		SHELL_Transmit(Shell, "I2C Addr: 0x%x\r\n", gdt080a[Idx].Addr >> 1);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_GDT_HELP, sizeof(SHELL_CMD_GDT_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "init"))
		{
			if (GDT080A_Init(&gdt080a[(Idx)]))
				goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "mntr"))
		{
			if (GDT080A_Update(&gdt080a[(Idx)]))
				goto cmd_err;
			SHELL_Transmit(Shell, "Set Vout: %2.2f\r\n", gdt080a[Idx].Cfg.Vout.Value);
			SHELL_Transmit(Shell, "Vin    Vout    Iout    Temp1    Temp2\r\n");
			SHELL_Transmit(Shell, "%2.2f   %2.2f   %2.2f   %2.2f   %2.2f\r\n",
				gdt080a[Idx].Mon.Vin, gdt080a[Idx].Mon.Vout, gdt080a[Idx].Mon.Iout,
				gdt080a[Idx].Mon.Temp1, gdt080a[Idx].Mon.Temp2);
		}
		else
		{
			goto cmd_err;
		}
	}
	else if (CmdParamsCnt == 3)
	{
		if (!strcmp(CmdParamsList[1], "select"))
		{
			Data = strtoul(CmdParamsList[2], &p, 10);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_idx;
			if (Data >= BSP_FPGA_PWR_GDT080A_CNT)
				goto invalid_idx;
			Idx = Data;
		}
		else if (!strcmp(CmdParamsList[1], "vout"))
		{
			Vout = strtof(CmdParamsList[2], &p);
			if ((*p != 0) || (p == CmdParamsList[2]))
				goto invalid_data;
			if (GDT080A_SetVout(&gdt080a[(Idx)], Vout))
				goto cmd_err;
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else 
	{
		goto invalid_cmd;
	}
	
	return;
invalid_cmd:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
invalid_idx:
	SHELL_TransmitBuffer(Shell, "Error: invalid index\r\n", sizeof("Error: invalid index\r\n") - 1);
	return;
invalid_data:
	SHELL_TransmitBuffer(Shell, "Error: invalid data\r\n", sizeof("Error: invalid data\r\n") - 1);
	return;
cmd_err:
	SHELL_TransmitBuffer(Shell, "Error: command error\r\n", sizeof("Error: command error\r\n") - 1);
	return;
}

#define SHELL_CMD_KSZ_HELP \
"Usage:\r\n"\
"\tksz <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\tmac - Display mac table.\r\n"\
"\tlink - Display port link.\r\n"\
"\tr <addr> - Read device register.\r\n"\
"\t\tExample: ksz r 0x01\r\n"\
"\tw <addr> <data> - Write device register.\r\n"\
"\t\tExample: ksz w 0x01 0x10\r\n"

#define KSZ8895_MAC_TABLE_SIZE 64
static KSZ8895_MAC_TABLE_T ksz_mac_table[KSZ8895_MAC_TABLE_SIZE];

static void SHELL_CMD_KSZCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t Addr, Data, i, ksz_mac_table_size;
	uint8_t Data8;
	char *p;
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell, "Init state: %s\r\n", BSP_DEV_GET_STATE_STR(&ksz8895));
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_KSZ_HELP, sizeof(SHELL_CMD_KSZ_HELP) - 1);
		}
		else if (!strcmp(CmdParamsList[1], "link"))
		{
			for (i = 0; i < KSZ8895_PORTS_CNT; i++)
			{
				if (KSZ8895_UpdatePortStatus(&ksz8895, i))
					goto cmd_err;
				SHELL_Transmit(Shell, "Port %d:\r\n", i);
				SHELL_Transmit(Shell, "\tLink - %s\r\n", KSZ8895_PORT_LINK(&ksz8895, i) ? "Up" : "Down");
				SHELL_Transmit(Shell, "\tSpeed - %d M\r\n", KSZ8895_PORT_SPEED(&ksz8895, i));
				SHELL_Transmit(Shell, "\tDuplex - %s\r\n", KSZ8895_PORT_DUPLEX(&ksz8895, i) ? "Full" : "Half");
			}
		}
		else if (!strcmp(CmdParamsList[1], "mac"))
		{
			if (KSZ8895_ReadMACTable(&ksz8895, ksz_mac_table, KSZ8895_MAC_TABLE_SIZE, &ksz_mac_table_size))
				goto cmd_err;
			SHELL_Transmit(Shell, "Idx    Port    Mac\r\n");
			for (i = 0; i < ksz_mac_table_size; i++)
			{
				SHELL_Transmit(Shell, "%3d    %4d    %02X:%02X:%02X:%02X:%02X:%02X\r\n", i, ksz_mac_table[i].Port, ksz_mac_table[i].Mac[0],
					ksz_mac_table[i].Mac[1], ksz_mac_table[i].Mac[2], ksz_mac_table[i].Mac[3], ksz_mac_table[i].Mac[4], ksz_mac_table[i].Mac[5]);
			}
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else if (CmdParamsCnt == 3)
	{
		if (!strcmp(CmdParamsList[1], "r"))
		{
			if ((CmdParamsList[2][0] != '0') || (CmdParamsList[2][1] != 'x'))
				goto invalid_addr;
			
			Addr = strtoul(&CmdParamsList[2][2], &p, 16);
			if ((*p != 0) || (p == &CmdParamsList[2][2]))
				goto invalid_addr;
			
			if (Addr > 0xFF)
				goto invalid_addr;
			
			if (KSZ8895_ReadReg(&ksz8895, Addr, &Data8))
				goto cmd_err;
			
			SHELL_Transmit(Shell, "0x%x\r\n", Data8);
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else if (CmdParamsCnt == 4)
	{
		if (!strcmp(CmdParamsList[1], "w"))
		{
			if ((CmdParamsList[2][0] != '0') || (CmdParamsList[2][1] != 'x'))
				goto invalid_addr;
			
			Addr = strtoul(&CmdParamsList[2][2], &p, 16);
			if ((*p != 0) || (p == &CmdParamsList[2][2]))
				goto invalid_addr;
			
			if (Addr > 0xFF)
				goto invalid_addr;
			
			if ((CmdParamsList[3][0] != '0') || (CmdParamsList[3][1] != 'x'))
				goto invalid_data;
			
			Data = strtoul(&CmdParamsList[3][2], &p, 16);
			if ((*p != 0) || (p == &CmdParamsList[3][2]))
				goto invalid_data;
			
			if (Data > 0xFF)
				goto invalid_addr;
			
			if (KSZ8895_WriteReg(&ksz8895, Addr, Data))
				goto cmd_err;
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else 
	{
		goto invalid_cmd;
	}
	
	return;
invalid_cmd:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
cmd_err:
	SHELL_TransmitBuffer(Shell, "Error: command error\r\n", sizeof("Error: command error\r\n") - 1);
	return;
invalid_addr:
	SHELL_TransmitBuffer(Shell, "Error: invalid address\r\n", sizeof("Error: invalid address\r\n") - 1);
	return;
invalid_data:
	SHELL_TransmitBuffer(Shell, "Error: invalid data\r\n", sizeof("Error: invalid data\r\n") - 1);
	return;
}

#define SHELL_CMD_SDRAM_HELP \
"Usage:\r\n"\
"\tsdram <cmd> [<data1> <data2> ...]\r\n"\
"Commands:\r\n"\
"\thelp - Display this help.\r\n"\
"\tload <file> <ip> - Load file to SDRAM via TFTP.\r\n"\

static int32_t TFTP_CLIENT_ProgramSDRAM(void *Priv, uint16_t Block, char *Source, uint32_t Size)
{
	uint32_t Addr = ((uint32_t)Priv) + ((Block - 1) * TFTP_DATA_LEN_MAX);
	memcpy((char*)Addr, Source, Size);
	return 0;
}

static void SHELL_CMD_SDRAMCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	char *p;
	uint32_t ip[4];
	ip_addr_t ip_addr;
	int32_t result;
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell,   "Init state: %s\r\n", BSP_DEV_GET_STATE_STR(&mt48lc16m16a2));
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_TransmitBuffer(Shell, SHELL_CMD_SDRAM_HELP, sizeof(SHELL_CMD_SDRAM_HELP) - 1);
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else if (CmdParamsCnt == 4)
	{
		if (!strcmp(CmdParamsList[1], "load"))
		{
			p = CmdParamsList[3];
			ip[0] = strtoul(p, &p, 10);
			if ((ip[0] == 0) || (ip[0] > 255) || (*p != '.'))
				goto invalid_ip_addr;
			p++;
			ip[1] = strtoul(p, &p, 10);
			if ((ip[1] > 255) || (*p != '.'))
				goto invalid_ip_addr;
			p++;
			ip[2] = strtoul(p, &p, 10);
			if ((ip[2] > 255) || (*p != '.'))
				goto invalid_ip_addr;
			p++;
			ip[3] = strtoul(p, &p, 10);
			if ((ip[3] == 0) || (ip[3] > 255) || (*p != 0))
				goto invalid_ip_addr;
			
			IP_ADDR4(&ip_addr, ip[0], ip[1], ip[2], ip[3]);
			
			result = TFTP_CLIENT_DownloadFile(CmdParamsList[2], &ip_addr, TFTP_CLIENT_ProgramSDRAM, (void*)BSP_SDRAM_ADDR);
			if (result == TFTP_CLIENT_ERROR)
			{
				goto download_error;
			}
			else if (result == TFTP_CLIENT_TIMEOUT)
			{
				goto download_timeout;
			}
		}
	}
	else
	{
		goto invalid_cmd;
	}
	
	return;
	
invalid_cmd:
	SHELL_TransmitBuffer(Shell, "Error: invalid command\r\n", sizeof("Error: invalid command\r\n") - 1);
	return;
invalid_ip_addr:
	SHELL_TransmitBuffer(Shell, "Error: invalid ip address\r\n", sizeof("Error: invalid ip address\r\n") - 1);
	return;
download_error:
	SHELL_TransmitBuffer(Shell, "Error: download error\r\n", sizeof("Error: download error\r\n") - 1);
	return;
download_timeout:
	SHELL_TransmitBuffer(Shell, "Error: download timeout\r\n", sizeof("Error: download timeout\r\n") - 1);
	return;
}
