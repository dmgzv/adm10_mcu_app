#include "shell_cmd.h"
#include <string.h>
#include <stdlib.h>
#include "cmsis_os.h"
#include "stdio.h"
#include "bsp.h"
#include "tftp_client.h"

static void SHELL_CMD_SystemCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_InfoCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
#ifdef BSP_CONFIG_USE_LOG
static void SHELL_CMD_LOGCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
#endif
static void SHELL_CMD_I2CCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_SPICallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_UARTCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_StatusCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_FPGACallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_DIM3RCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_UCDCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_GDTCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_SFPCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_PCA9536Callback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_PCAL6524Callback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_PCA9548Callback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_KSZCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);
static void SHELL_CMD_SDRAMCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt);

const SHELL_CMD_T ShellCmdList[] = 
{
	{ "system", 	"System information", &SHELL_CMD_SystemCallback},
	{ "info", 		"Project information", &SHELL_CMD_InfoCallback},
#ifdef BSP_CONFIG_USE_LOG
	{ "log", 			"Log control", &SHELL_CMD_LOGCallback},
#endif
	{ "i2c", 			"I2C statistics", &SHELL_CMD_I2CCallback},
	{ "spi", 			"SPI statistics", &SHELL_CMD_SPICallback},
	{ "uart", 		"UART statistics", &SHELL_CMD_UARTCallback},
	{ "status", 	"Status of devices", &SHELL_CMD_StatusCallback},
	{ "dim3r", 		"DIM3R3400 control", &SHELL_CMD_DIM3RCallback},
	{ "ucd", 			"UCD90120A control", &SHELL_CMD_UCDCallback},
	{ "gdt", 			"GDT080A control", &SHELL_CMD_GDTCallback},
	{ "sfp", 			"SFP control", &SHELL_CMD_SFPCallback},
	{ "fpga", 		"FPGA control", &SHELL_CMD_FPGACallback},
	{ "pca9536", 	"PCA9536 control", &SHELL_CMD_PCA9536Callback},
	{ "pcal6524", "PCAL6524 control", &SHELL_CMD_PCAL6524Callback},
	{ "pca9548", 	"PCA9548 control", &SHELL_CMD_PCA9548Callback},
	{ "ksz", 			"KSZ8895 control", &SHELL_CMD_KSZCallback},
	{ "sdram", 		"SDRAM control", &SHELL_CMD_SDRAMCallback}
};

const uint32_t ShellCmdListSize = sizeof(ShellCmdList)/ sizeof(ShellCmdList[0]);

/* Shell commands */
static void SHELL_CMD_SystemCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	char *p1,*p2;
	uint32_t len;
	
	SHELL_Transmit(Shell, "Heap size: %d bytes\r", xPortGetFreeHeapSize());
	SHELL_Transmit(Shell, "Name          State Priority   Stack   Num\r");
	osThreadList((uint8_t*)((SHELL_T*)Shell)->TxBuffer);
	
	len = strlen(((SHELL_T*)Shell)->TxBuffer);
	p1 = p2 = ((SHELL_T*)Shell)->TxBuffer;
	while(p2 != &((SHELL_T*)Shell)->TxBuffer[len - 1])
	{
		if (*p2 == '\n')
			p2++;
		if (p1 != p2)
			*p1 = *p2;
		p1++;
		p2++;
	}
	*p1 = 0;
	SHELL_TransmitBuffer(Shell,  ((SHELL_T*)Shell)->TxBuffer, strlen(((SHELL_T*)Shell)->TxBuffer));
}

static void SHELL_CMD_InfoCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	SHELL_Transmit(Shell, "Project: %s \r", BSP_PROJECT);
	SHELL_Transmit(Shell,  "HW Version: %s \r", BSP_HW_VERSION);
	SHELL_Transmit(Shell,  "SW Version: %s \r", BSP_SW_VERSION);
	SHELL_Transmit(Shell,  "SW Date: %s \r", BSP_SW_DATE);
	SHELL_Transmit(Shell,  "SW Time: %s \r", BSP_SW_TIME);
}

#ifdef BSP_CONFIG_USE_LOG
static void SHELL_CMD_LOGCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t i, LinesCnt, LineTime;
	float Time;
	char Line[LOG_LINE_SIZE];
	
	LinesCnt = LOG_GetLinesCnt();
	SHELL_Transmit(Shell, "Size: %d\r", LOG_LINES_CNT);
	SHELL_Transmit(Shell, "Records Count: %d\r", LOG_GetLinesCnt());
	for (i = 0; i < LinesCnt; i++)
	{
		LOG_GetLine(i, &LineTime, Line);
		Time = (float)LineTime / osKernelSysTickFrequency;
		SHELL_Transmit(Shell, "[%4.4f] %s\r", Time, Line);		
	}	
}
#endif

static void SHELL_CMD_I2CCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
#ifdef BSP_CONFIG_USE_I2C1
	SHELL_Transmit(Shell, "I2C1:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", i2c1.overrun_errors);
	SHELL_Transmit(Shell, "  BERR errors: %d\r", i2c1.berr_errors);
	SHELL_Transmit(Shell, "  ARLO errors: %d\r", i2c1.arlo_errors);
	SHELL_Transmit(Shell, "  AF errors: %d\r", i2c1.af_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", i2c1.dma_errors);
	SHELL_Transmit(Shell, "  Receive timeouts: %d\r", i2c1.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential receive timeouts: %d\r", i2c1.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit timeouts: %d\r", i2c1.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential transmit timeouts: %d\r", i2c1.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "  Queue overflows: %d\r", i2c1.message_queue_overflow_errors);
#endif
#ifdef BSP_CONFIG_USE_I2C2
	SHELL_Transmit(Shell, "I2C2:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", i2c2.overrun_errors);
	SHELL_Transmit(Shell, "  BERR errors: %d\r", i2c2.berr_errors);
	SHELL_Transmit(Shell, "  ARLO errors: %d\r", i2c2.arlo_errors);
	SHELL_Transmit(Shell, "  AF errors: %d\r", i2c2.af_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", i2c2.dma_errors);
	SHELL_Transmit(Shell, "  Receive timeouts: %d\r", i2c2.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential receive timeouts: %d\r", i2c2.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit timeouts: %d\r", i2c2.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential transmit timeouts: %d\r", i2c2.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "  Queue overflows: %d\r", i2c2.message_queue_overflow_errors);
#endif
#ifdef BSP_CONFIG_USE_I2C3
	SHELL_Transmit(Shell, "I2C3:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", i2c3.overrun_errors);
	SHELL_Transmit(Shell, "  BERR errors: %d\r", i2c3.berr_errors);
	SHELL_Transmit(Shell, "  ARLO errors: %d\r", i2c3.arlo_errors);
	SHELL_Transmit(Shell, "  AF errors: %d\r", i2c3.af_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", i2c3.dma_errors);
	SHELL_Transmit(Shell, "  Receive timeouts: %d\r", i2c3.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential receive timeouts: %d\r", i2c3.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit timeouts: %d\r", i2c3.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential transmit timeouts: %d\r", i2c3.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "  Queue overflows: %d\r", i2c3.message_queue_overflow_errors);
#endif
}

static void SHELL_CMD_SPICallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
#ifdef BSP_CONFIG_USE_SPI1
	SHELL_Transmit(Shell, "SPI1:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", spi1.overrun_errors);
	SHELL_Transmit(Shell, "  Mode fault errors: %d\r", spi1.mode_fault_errors);
	SHELL_Transmit(Shell, "  CRC errors: %d\r", spi1.crc_errors);
	SHELL_Transmit(Shell, "  Frame errors: %d\r", spi1.frame_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", spi1.dma_errors);
	SHELL_Transmit(Shell, "  Receive timeouts: %d\r", spi1.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential receive timeouts: %d\r", spi1.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit timeouts: %d\r", spi1.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential transmit timeouts: %d\r", spi1.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit and receive timeouts: %d\r", spi1.tx_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Queue overflows: %d\r", spi1.message_queue_overflow_errors);
#endif
#ifdef BSP_CONFIG_USE_SPI2
	SHELL_Transmit(Shell, "SPI2:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", spi2.overrun_errors);
	SHELL_Transmit(Shell, "  Mode fault errors: %d\r", spi2.mode_fault_errors);
	SHELL_Transmit(Shell, "  CRC errors: %d\r", spi2.crc_errors);
	SHELL_Transmit(Shell, "  Frame errors: %d\r", spi2.frame_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", spi2.dma_errors);
	SHELL_Transmit(Shell, "  Receive timeouts: %d\r", spi2.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential receive timeouts: %d\r", spi2.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit timeouts: %d\r", spi2.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential transmit timeouts: %d\r", spi2.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit and receive timeouts: %d\r", spi2.tx_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Queue overflows: %d\r", spi2.message_queue_overflow_errors);
#endif
#ifdef BSP_CONFIG_USE_SPI3
	SHELL_Transmit(Shell, "SPI3:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", spi3.overrun_errors);
	SHELL_Transmit(Shell, "  Mode fault errors: %d\r", spi3.mode_fault_errors);
	SHELL_Transmit(Shell, "  CRC errors: %d\r", spi3.crc_errors);
	SHELL_Transmit(Shell, "  Frame errors: %d\r", spi3.frame_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", spi3.dma_errors);
	SHELL_Transmit(Shell, "  Receive timeouts: %d\r", spi3.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential receive timeouts: %d\r", spi3.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit timeouts: %d\r", spi3.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential transmit timeouts: %d\r", spi3.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit and receive timeouts: %d\r", spi3.tx_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Queue overflows: %d\r", spi3.message_queue_overflow_errors);
#endif
#ifdef BSP_CONFIG_USE_SPI4
	SHELL_Transmit(Shell, "SPI4:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", spi4.overrun_errors);
	SHELL_Transmit(Shell, "  Mode fault errors: %d\r", spi4.mode_fault_errors);
	SHELL_Transmit(Shell, "  CRC errors: %d\r", spi4.crc_errors);
	SHELL_Transmit(Shell, "  Frame errors: %d\r", spi4.frame_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", spi4.dma_errors);
	SHELL_Transmit(Shell, "  Receive timeouts: %d\r", spi4.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential receive timeouts: %d\r", spi4.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit timeouts: %d\r", spi4.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential transmit timeouts: %d\r", spi4.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit and receive timeouts: %d\r", spi4.tx_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Queue overflows: %d\r", spi4.message_queue_overflow_errors);
#endif
#ifdef BSP_CONFIG_USE_SPI5
	SHELL_Transmit(Shell, "SPI5:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", spi5.overrun_errors);
	SHELL_Transmit(Shell, "  Mode fault errors: %d\r", spi5.mode_fault_errors);
	SHELL_Transmit(Shell, "  CRC errors: %d\r", spi5.crc_errors);
	SHELL_Transmit(Shell, "  Frame errors: %d\r", spi5.frame_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", spi5.dma_errors);
	SHELL_Transmit(Shell, "  Receive timeouts: %d\r", spi5.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential receive timeouts: %d\r", spi5.seq_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit timeouts: %d\r", spi5.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Sequential transmit timeouts: %d\r", spi5.seq_tx_timeout_errors);
	SHELL_Transmit(Shell, "  Transmit and receive timeouts: %d\r", spi5.tx_rx_timeout_errors);
	SHELL_Transmit(Shell, "  Queue overflows: %d\r", spi5.message_queue_overflow_errors);
#endif
}

static void SHELL_CMD_UARTCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
#ifdef BSP_CONFIG_USE_UART1
	SHELL_Transmit(Shell, "UART1:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", uart1.overrun_errors);
	SHELL_Transmit(Shell, "  Parity errors: %d\r", uart1.parity_errors);
	SHELL_Transmit(Shell, "  Noise errors: %d\r", uart1.noise_errors);
	SHELL_Transmit(Shell, "  Frame errors: %d\r", uart1.frame_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", uart1.dma_errors);
	SHELL_Transmit(Shell, "  Rx timeouts: %d\r", uart1.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Rx cmd timeouts: %d\r", uart1.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "  Tx timeouts: %d\r", uart1.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Tx queue overflows: %d\r", uart1.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "  Rx queue overflows: %d\r", uart1.rx_message_queue_overflow_errors);
#endif
#ifdef BSP_CONFIG_USE_UART2
	SHELL_Transmit(Shell, "UART2:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", uart2.overrun_errors);
	SHELL_Transmit(Shell, "  Parity errors: %d\r", uart2.parity_errors);
	SHELL_Transmit(Shell, "  Noise errors: %d\r", uart2.noise_errors);
	SHELL_Transmit(Shell, "  Frame errors: %d\r", uart2.frame_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", uart2.dma_errors);
	SHELL_Transmit(Shell, "  Rx timeouts: %d\r", uart2.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Rx cmd timeouts: %d\r", uart2.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "  Tx timeouts: %d\r", uart2.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Tx queue overflows: %d\r", uart2.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "  Rx queue overflows: %d\r", uart2.rx_message_queue_overflow_errors);
#endif
#ifdef BSP_CONFIG_USE_UART3
	SHELL_Transmit(Shell, "UART3:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", uart3.overrun_errors);
	SHELL_Transmit(Shell, "  Parity errors: %d\r", uart3.parity_errors);
	SHELL_Transmit(Shell, "  Noise errors: %d\r", uart3.noise_errors);
	SHELL_Transmit(Shell, "  Frame errors: %d\r", uart3.frame_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", uart3.dma_errors);
	SHELL_Transmit(Shell, "  Rx timeouts: %d\r", uart3.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Rx cmd timeouts: %d\r", uart3.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "  Tx timeouts: %d\r", uart3.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Tx queue overflows: %d\r", uart3.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "  Rx queue overflows: %d\r", uart3.rx_message_queue_overflow_errors);
#endif
#ifdef BSP_CONFIG_USE_UART4
	SHELL_Transmit(Shell, "UART4:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", uart4.overrun_errors);
	SHELL_Transmit(Shell, "  Parity errors: %d\r", uart4.parity_errors);
	SHELL_Transmit(Shell, "  Noise errors: %d\r", uart4.noise_errors);
	SHELL_Transmit(Shell, "  Frame errors: %d\r", uart4.frame_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", uart4.dma_errors);
	SHELL_Transmit(Shell, "  Rx timeouts: %d\r", uart4.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Rx cmd timeouts: %d\r", uart4.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "  Tx timeouts: %d\r", uart4.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Tx queue overflows: %d\r", uart4.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "  Rx queue overflows: %d\r", uart4.rx_message_queue_overflow_errors);
#endif
#ifdef BSP_CONFIG_USE_UART5
	SHELL_Transmit(Shell, "UART5:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", uart5.overrun_errors);
	SHELL_Transmit(Shell, "  Parity errors: %d\r", uart5.parity_errors);
	SHELL_Transmit(Shell, "  Noise errors: %d\r", uart5.noise_errors);
	SHELL_Transmit(Shell, "  Frame errors: %d\r", uart5.frame_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", uart5.dma_errors);
	SHELL_Transmit(Shell, "  Rx timeouts: %d\r", uart5.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Rx cmd timeouts: %d\r", uart5.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "  Tx timeouts: %d\r", uart5.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Tx queue overflows: %d\r", uart5.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "  Rx queue overflows: %d\r", uart5.rx_message_queue_overflow_errors);
#endif
#ifdef BSP_CONFIG_USE_UART6
	SHELL_Transmit(Shell, "UART6:\r");
	SHELL_Transmit(Shell, "  Overrun errors: %d\r", uart6.overrun_errors);
	SHELL_Transmit(Shell, "  Parity errors: %d\r", uart6.parity_errors);
	SHELL_Transmit(Shell, "  Noise errors: %d\r", uart6.noise_errors);
	SHELL_Transmit(Shell, "  Frame errors: %d\r", uart6.frame_errors);
	SHELL_Transmit(Shell, "  DMA errors: %d\r", uart6.dma_errors);
	SHELL_Transmit(Shell, "  Rx timeouts: %d\r", uart6.rx_timeout_errors);
	SHELL_Transmit(Shell, "  Rx cmd timeouts: %d\r", uart6.rx_cmd_timeout_errors);
	SHELL_Transmit(Shell, "  Tx timeouts: %d\r", uart6.tx_timeout_errors);
	SHELL_Transmit(Shell, "  Tx queue overflows: %d\r", uart6.tx_message_queue_overflow_errors);
	SHELL_Transmit(Shell, "  Rx queue overflows: %d\r", uart6.rx_message_queue_overflow_errors);
#endif
}

const char *pca9536_pcb_id[BSP_SFP_PIN_PCA9536_CNT] = 
{
	"D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", 
	"D13", "D14", "D15", "D16", "D17", "D18", "D19", "D20",
	"D9", "D10", "D11", "D12"
};

static void SHELL_CMD_StatusCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t i;
	
	SHELL_Transmit(Shell,  "Device     PCB  Presence  Init\r");
	SHELL_Transmit(Shell,   "PCA9536    D107     1     %s\r", BSP_DEV_GET_STATE_STR(&pwr_pca9536));
	SHELL_Transmit(Shell,   "DIM3R3400  A5       1     %s\r", BSP_DEV_GET_STATE_STR(&dim3r3400));
	SHELL_Transmit(Shell,   "UCD90120A  D108     1     %s\r", BSP_DEV_GET_STATE_STR(&ucd90120a));
	SHELL_Transmit(Shell,   "GDT080A    A6       1     %s\r", BSP_DEV_GET_STATE_STR(&gdt080a[0]));
	SHELL_Transmit(Shell,   "GDT080A    A7       1     %s\r", BSP_DEV_GET_STATE_STR(&gdt080a[1]));
	SHELL_Transmit(Shell,   "SI5332     D83      1     %s\r", BSP_DEV_GET_STATE_STR(&si5332[0]));
	SHELL_Transmit(Shell,   "SI5332     D84      1     %s\r", BSP_DEV_GET_STATE_STR(&si5332[1]));
	SHELL_Transmit(Shell,   "FPGA_MAXV  D85      1     %s\r", BSP_DEV_GET_STATE_STR(&fpga_maxv));
	SHELL_Transmit(Shell,   "FPGA_LN    D92      1     %s\r", BSP_DEV_GET_STATE_STR(&fpga_line));
	SHELL_Transmit(Shell,   "FPGA_CL1   D88      1     %s\r", BSP_DEV_GET_STATE_STR(&fpga_client[0]));
	SHELL_Transmit(Shell,   "FPGA_CL2   D90      1     %s\r", BSP_DEV_GET_STATE_STR(&fpga_client[0]));
	SHELL_Transmit(Shell,   "PCA9548    D72      1     %s\r", BSP_DEV_GET_STATE_STR(&pca9548a[0]));
	SHELL_Transmit(Shell,   "PCA9548    D73      1     %s\r", BSP_DEV_GET_STATE_STR(&pca9548a[1]));
	SHELL_Transmit(Shell,   "PCA9548    D74      1     %s\r", BSP_DEV_GET_STATE_STR(&pca9548a[2]));
	SHELL_Transmit(Shell,   "PCAL6624   D67      1     %s\r", BSP_DEV_GET_STATE_STR(&pcal6524[0]));
	SHELL_Transmit(Shell,   "PCAL6624   D58      1     %s\r", BSP_DEV_GET_STATE_STR(&pcal6524[1]));
	SHELL_Transmit(Shell,   "PCAL6624   D57      1     %s\r", BSP_DEV_GET_STATE_STR(&pcal6524[2]));
	for (i = 0; i < BSP_SFP_PIN_PCA9536_CNT; i++)
		SHELL_Transmit(Shell,   "PCA9536    %3s      1     %s\r", pca9536_pcb_id[i], BSP_DEV_GET_STATE_STR(&pca9536[i]));
	for (i = 0; i < BSP_SFP_CNT; i++)
		SHELL_Transmit(Shell,   "SFP        ---      %d     %s\r", sfp[i].Pin.ModAbs ? 0 : 1, BSP_DEV_GET_STATE_STR(&sfp[i]));
	SHELL_Transmit(Shell,   "KSZ8895    D99      1     %s\r", BSP_DEV_GET_STATE_STR(&ksz8895));
	SHELL_Transmit(Shell,   "AT45DB641E D94      1     %s\r", BSP_DEV_GET_STATE_STR(&at45db641e));
}

#define SHELL_CMD_DIM3R_HELP \
"Usage:\r"\
"    dim3r <cmd> [<data1> <data2> ...]\r"\
"Commands:\r"\
"    help - Display this help.\r"\
"    init - Init device.\r"\
"    mntr - Display device monitor values.\r"

static void SHELL_CMD_DIM3RCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell,   "Init state: %s\r", BSP_DEV_GET_STATE_STR(&dim3r3400));
		SHELL_Transmit(Shell,   "I2C Addr: 0x%x\r", dim3r3400.Addr >> 1);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_Transmit(Shell, SHELL_CMD_DIM3R_HELP);
		}
		else if (!strcmp(CmdParamsList[1], "init"))
		{
			if (DIM3R3400_Init(&dim3r3400, BSP_PWR_IN_DIM3R3400_I2C, BSP_PWR_IN_DIM3R3400_I2C_ADDR))
				goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "mntr"))
		{
			if (DIM3R3400_Update(&dim3r3400))
				goto cmd_err;
			SHELL_Transmit(Shell, "Status: 0x%02X\r", dim3r3400.Status);
			SHELL_Transmit(Shell, "VOUT: %2.2f\r", dim3r3400.Vout);
			SHELL_Transmit(Shell, "IOUT: %2.2f\r", dim3r3400.Iout);
			SHELL_Transmit(Shell, "VINA: %2.2f\r", dim3r3400.Vin[0]);
			SHELL_Transmit(Shell, "VINB: %2.2f\r", dim3r3400.Vin[1]);
			SHELL_Transmit(Shell, "Temperature: %2.2f\r", dim3r3400.Temp);
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
	SHELL_Transmit(Shell, "Error: invalid command\r");
	return;
cmd_err:
	SHELL_Transmit(Shell, "Error: command error\r");
	return;
}

#define SHELL_CMD_PCA9536_HELP \
"Usage:\r"\
"    pca9536 <cmd> [<data1> <data2> ...]\r"\
"Commands:\r"\
"    help - Display this help.\r"\
"    pin - Display device pin states.\r"\
"    out <pin> <data> - Set device pin output.\r"\
"    dir <pin> <data> - Set device pin direction.\r"\
"    pol <pin> <data> - Set device pin polarity.\r"

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
		SHELL_Transmit(Shell, "Init state: %s\r", BSP_DEV_GET_STATE_STR(&pwr_pca9536));
		SHELL_Transmit(Shell, "I2C Addr: 0x%x\r", PCA9536_I2C_ADDR >> 1);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_Transmit(Shell, SHELL_CMD_PCA9536_HELP);
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
			SHELL_Transmit(Shell,   "Idx  In  Dir  Out  Pol  Info\r");
			for (i = 0; i < 4; i++)
			{
				SHELL_Transmit(Shell,   "%3d  %2d  %3d  %3d  %3d  %s\r",
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
	SHELL_Transmit(Shell, "Error: invalid command\r");
	return;
invalid_pin:
	SHELL_Transmit(Shell, "Error: invalid pin\r");
	return;
invalid_data:
	SHELL_Transmit(Shell, "Error: invalid data\r");
	return;
cmd_err:
	SHELL_Transmit(Shell, "Error: command error\r");
	return;
}

#define SHELL_CMD_PCAL6524_HELP \
"Usage:\r"\
"    pcal6524 <cmd> [<data1> <data2> ...]\r"\
"Commands:\r"\
"    help - Display this help.\r"\
"    pin - Display device pin states.\r"\
"    out <pin> <data> - Set device pin output.\r"\
"    dir <pin> <data> - Set device pin direction.\r"\
"    pol <pin> <data> - Set device pin polarity.\r"

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
		SHELL_Transmit(Shell,   "Init state: %s\r", BSP_DEV_GET_STATE_STR(&pcal6524[Idx]));
		SHELL_Transmit(Shell,   "I2C Addr: 0x%x\r", pcal6524[Idx].Addr >> 1);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_Transmit(Shell, SHELL_CMD_PCAL6524_HELP);
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
			SHELL_Transmit(Shell,   "Idx  In  Cfg  Out  Pol  Info\r");
			for (i = 0; i < PCAL6524_BANKS_CNT * 8; i++)
			{
				SHELL_Transmit(Shell,   "%3d  %2d  %3d  %3d  %3d  %s\r",
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
	SHELL_Transmit(Shell, "Error: invalid command\r");
	return;
invalid_idx:
	SHELL_Transmit(Shell, "Error: invalid index\r");
	return;
invalid_pin:
	SHELL_Transmit(Shell, "Error: invalid pin\r");
	return;
invalid_data:
	SHELL_Transmit(Shell, "Error: invalid data\r");
	return;
cmd_err:
	SHELL_Transmit(Shell, "Error: command error\r");
	return;
}

#define SHELL_CMD_PCA9548_HELP \
"Usage:\r"\
"    pca9548 <cmd> [<data1> <data2> ...]\r"\
"Commands:\r"\
"    help - Display this help.\r"\
"    init - Init device.\r"\
"    select <idx> - Select device. <idx>: 0, 1\r"\

static void SHELL_CMD_PCA9548Callback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	static uint32_t Idx = 0;
	uint32_t Data;
	char *p;
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell,   "Init state: %s\r", BSP_DEV_GET_STATE_STR(&pca9548a[Idx]));
		SHELL_Transmit(Shell,   "I2C Addr: 0x%x\r", pca9548a[Idx].Addr >> 1);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_Transmit(Shell, SHELL_CMD_PCA9548_HELP);
		}
		else if (!strcmp(CmdParamsList[1], "init"))
		{
			if (PCA9548A_Init(&pca9548a[Idx], pca9548a[Idx].I2C, pca9548a[Idx].Addr >> 1))
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
			if (Data >= BSP_PCA9548_CNT)
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
	SHELL_Transmit(Shell, "Error: invalid command\r");
	return;
invalid_idx:
	SHELL_Transmit(Shell, "Error: invalid index\r");
	return;
cmd_err:
	SHELL_Transmit(Shell, "Error: command error\r");
	return;
}

#define SHELL_CMD_SFP_HELP \
"Usage:\r"\
"    sfp <cmd> [<data1> <data2> ...]\r"\
"Commands:\r"\
"    help - Display this help.\r"\
"    select <idx> - Select device. <idx>: 0, ... ,19.\r"\
"    tx <state> - Set device tx enable. <state>: 0 - Disable, 1 - Enable.\r"\
"    init - Init device.\r"\
"    pin - Display device pin states.\r"\
"    info - Display device info values.\r"\
"    mntr - Display device monitor values.\r"\

static void SHELL_CMD_SFPCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t Data;
	static uint32_t Idx = 0;
	char *p;
	
	if (CmdParamsCnt == 1)
	{		
		SHELL_Transmit(Shell,   "Index: %d\r", Idx);
		SHELL_Transmit(Shell,   "Presence: %d (%s)\r", sfp[Idx].Pin.ModAbs, sfp[Idx].Pin.ModAbs ? "Absent" : "Present");
		SHELL_Transmit(Shell,   "Init state: %d (%s)\r", BSP_DEV_GET_STATE(&sfp[Idx]), BSP_DEV_GET_STATE_STR(&sfp[Idx]));
		SHELL_Transmit(Shell,   "Tx Disable: %d (%s)\r", sfp[Idx].Pin.TxDisable, sfp[Idx].Pin.TxDisable ? "Disable" : "Enable");
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_Transmit(Shell, SHELL_CMD_SFP_HELP);
		}
		else if (!strcmp(CmdParamsList[1], "info"))
		{
			/*Check module precence*/
			if (sfp[Idx].Pin.ModAbs)
				goto mod_abs;
			/*Check module init state*/
			if (BSP_DEV_GET_STATE(&sfp[Idx]) != BSP_DEV_STATE_OK)
				goto invalid_init_state;
			SHELL_Transmit(Shell,   "Vendor: %s\r", sfp[Idx].Info.Vendor);
			SHELL_Transmit(Shell,   "Part Number: %s\r", sfp[Idx].Info.PartNumber);
			SHELL_Transmit(Shell,   "Serial Number: %s\r", sfp[Idx].Info.SerialNumber);
		}
		else if (!strcmp(CmdParamsList[1], "pin"))
		{
			SHELL_Transmit(Shell, "MOD_ABS  RX_LOS  TX_DIS  TX_FAULT\r");
			SHELL_Transmit(Shell,   "   %d       %d       %d     %d\r",
				sfp[Idx].Pin.ModAbs, sfp[Idx].Pin.RxLos, sfp[Idx].Pin.TxDisable, sfp[Idx].Pin.TxFault);
		}
		else if (!strcmp(CmdParamsList[1], "init"))
		{
			/*Check module precence*/
			if (sfp[Idx].Pin.ModAbs)
				goto mod_abs;			
			if (SFP_Init(&sfp[Idx], (BSP_I2C_CTRL_T*)&bsp_sfp_i2c_ctrl[Idx], (BSP_PIN_CTRL_T*)&bsp_sfp_pin_ctrl[Idx]))
				goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "mntr"))
		{
			/*Check module precence*/
			if (sfp[Idx].Pin.ModAbs)
				goto mod_abs;
			/*Check module init state*/
			if (BSP_DEV_GET_STATE(&sfp[Idx]) != BSP_DEV_STATE_OK)
				goto invalid_init_state;
			
			if (SFP_Update(&sfp[Idx]))
				goto cmd_err;
			SHELL_Transmit(Shell,   "Rx Power: %.2f\r", sfp[Idx].RxPwr);
			SHELL_Transmit(Shell,   "Tx Power: %.2f\r", sfp[Idx].TxPwr);
			SHELL_Transmit(Shell,   "Temperature: %.2f\r", sfp[Idx].Temp);		
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
			SFP_SetTxDisable(&sfp[Idx], Data ? 0 : 1);			
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
	SHELL_Transmit(Shell, "Error: invalid command\r");
	return;
invalid_idx:
	SHELL_Transmit(Shell, "Error: invalid index\r");
	return;
cmd_err:
	SHELL_Transmit(Shell, "Error: command error\r");
	return;
mod_abs:
	SHELL_Transmit(Shell, "Error: module is absent\r");
	return;
invalid_init_state:
	SHELL_Transmit(Shell, "Error: invalid module init state\r");
	return;
}

#define SHELL_CMD_FPGA_HELP \
"Usage:\r"\
"    fpga <cmd> [<data1> <data2> ...]\r"\
"Commands:\r"\
"    help - Display this help.\r"\
"    select <dev> - Select fpga device. <dev>: 0 - MAXV, 1 - FPGA Line, 2 - FPGA Client 1, 3 - FPGA Client 2\r"\
"    test - Test fpga device.\r"\
"        Example: fpga test 0x01 0x10\r"\
"    r <addr> - Read device register.\r"\
"        Example: fpga r 0x01\r"\
"    w <addr> <data> - Write device register.\r"\
"        Example: fpga w 0x01 0x10\r"\
"    update <filename> - Update FPGA firmware via TFTP.\r"\

static void TFTP_CLIENT_ProgramFPGA(void *Priv, uint32_t Addr, char *Source, uint32_t Size)
{
	BSP_FPGA_FLASH_ProgramSector(Addr, Source, Size);
}

static void SHELL_CMD_FPGACallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	static uint8_t Dev = 3;
	ip_addr_t tftp_server_ip;
	uint32_t Addr, Data, i;
	char *p;
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell, "Init state: %s\r", BSP_DEV_GET_STATE_STR(&fpga_maxv));
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_Transmit(Shell, SHELL_CMD_FPGA_HELP);
		}
		else if (!strcmp(CmdParamsList[1], "test"))
		{
			if (Dev == 0)
			{
				Addr = BSP_FPGA_LINE_ADDR + 0xF0;
			}
			else if (Dev == 1)
			{
				Addr = BSP_FPGA_CLIENT_ADDR(0) + 0xF0;
			}
			else if (Dev == 2)
			{
				Addr = BSP_FPGA_CLIENT_ADDR(1) + 0xF0;
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
					SHELL_Transmit(Shell, "Test Failed: %d\r", i);
					return;
				}
			}
			SHELL_Transmit(Shell, "Test Successful\r", Addr);
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
				Data = *(uint8_t*)(BSP_FPGA_LINE_ADDR + Addr);
			}
			else if (Dev == 1)
			{
				Data = *(uint8_t*)(BSP_FPGA_CLIENT_ADDR(0) + Addr);
			}
			else if (Dev == 2)
			{
				Data = *(uint8_t*)(BSP_FPGA_CLIENT_ADDR(1) + Addr);
			}
			else if (Dev == 3)
			{
				Data = *(uint8_t*)(BSP_FPGA_MAXV_ADDR + Addr);
			}
			else
			{
				goto invalid_dev;
			}
			
			SHELL_Transmit(Shell, "0x%x\r", Data);
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
		else if (!strcmp(CmdParamsList[1], "update"))
		{
			IP_ADDR4(&tftp_server_ip,192,168,2,32);
			
			
			if (Dev == 0)
			{
				BSP_FPGA_FLASH_EraseFPGA(0);
				TFTP_CLIENT_DownloadFile(CmdParamsList[2], &tftp_server_ip, 69, TFTP_CLIENT_ProgramFPGA, NULL, BSP_FLASH_FPGA_1_FW_ADDR_START);
			}
			else if (Dev == 1)
			{
				BSP_FPGA_FLASH_EraseFPGA(1);
				TFTP_CLIENT_DownloadFile(CmdParamsList[2], &tftp_server_ip, 69, TFTP_CLIENT_ProgramFPGA, NULL, BSP_FLASH_FPGA_2_FW_ADDR_START);
			}
			else if (Dev == 2)
			{
				BSP_FPGA_FLASH_EraseFPGA(2);
				TFTP_CLIENT_DownloadFile(CmdParamsList[2], &tftp_server_ip, 69, TFTP_CLIENT_ProgramFPGA, NULL, BSP_FLASH_FPGA_3_FW_ADDR_START);
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
	else if (CmdParamsCnt == 4)
	{
		
		if (!strcmp(CmdParamsList[1], "w"))
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
				*(uint8_t*)(BSP_FPGA_LINE_ADDR + Addr) = Data;
			}
			else if (Dev == 1)
			{
				*(uint8_t*)(BSP_FPGA_CLIENT_ADDR(0) + Addr) = Data;
			}
			else if (Dev == 2)
			{
				*(uint8_t*)(BSP_FPGA_CLIENT_ADDR(1) + Addr) = Data;
			}
			else if (Dev == 3)
			{
				*(uint8_t*)(BSP_FPGA_MAXV_ADDR + Addr) = Data;
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
	SHELL_Transmit(Shell, "Error: invalid command\r");
	return;
invalid_addr:
	SHELL_Transmit(Shell, "Error: invalid address\r");
	return;
invalid_data:
	SHELL_Transmit(Shell, "Error: invalid data\r");
	return;
invalid_dev:
	SHELL_Transmit(Shell, "Error: invalid device\r");
	return;
}

#define SHELL_CMD_UCD_HELP \
"Usage:\r"\
"    ucd <cmd> [<data1> <data2> ...]\r"\
"Commands:\r"\
"    help - Display this help.\r"\
"    init - Init device.\r"\
"    mntr - Display device monitor values.\r"\
"    log  - Display device log.\r"\
"    mntr - Clear device log.\r"

static void SHELL_CMD_UCDCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t i;
	uint32_t LogSize;
	UCD90120A_FAULT_LOG_RECORD_T LogRecord;
	uint8_t Log[14];
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell,   "Init state: %s\r", BSP_DEV_GET_STATE_STR(&ucd90120a));
		SHELL_Transmit(Shell,   "I2C Addr: 0x%x\r", ucd90120a.Addr >> 1);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_Transmit(Shell, SHELL_CMD_UCD_HELP);
		}
		else if (!strcmp(CmdParamsList[1], "init"))
		{
			if (UCD90120A_Init(&ucd90120a, BSP_PWR_SEQ_UCD90120A_I2C, BSP_PWR_SEQ_UCD90120A_I2C_ADDR, (BSP_PIN_CTRL_T*) &ucd90120a_pin_ctrl, BSP_UCD90120A_CFG_Table, BSP_UCD90120A_CFG_TableSize))
				goto cmd_err;
		}
		else if (!strcmp(CmdParamsList[1], "mntr"))
		{
			if (UCD90120A_Update(&ucd90120a))
				goto cmd_err;
			SHELL_Transmit(Shell, "Device temp: %2.2f\r", ucd90120a.Mon.Temp);
			SHELL_Transmit(Shell,   "Rail   Vout    Iout    Temp    Status\r");
			for (i = 0; i < UCD90120A_RAILS_CNT; i++)
			{
				SHELL_Transmit(Shell,   " %2d    %2.2f    %2.2f    %2.2f    %02x\r", i + 1, ucd90120a.Mon.Rail[i].Vout, ucd90120a.Mon.Rail[i].Iout, ucd90120a.Mon.Rail[i].Temp, ucd90120a.Mon.Rail[i].StatusVout);
			}
		}
		else if (!strcmp(CmdParamsList[1], "log"))
		{
			if (UCD90120A_ReadFaultLog(&ucd90120a, Log))
				goto cmd_err;
			SHELL_Transmit(Shell, "Non-Paged faults: 0x%x\r", Log[0]);
			SHELL_Transmit(Shell, "GPI faults: 0x%x\r", Log[1]);
			for (i = 0; i < UCD90120A_RAILS_CNT; i++)
			{
				SHELL_Transmit(Shell, "Rail %d faults: 0x%x\r", i + 1, Log[i+2]);
			}
			if (UCD90120A_ReadFaultLogSize(&ucd90120a, &LogSize))
				goto cmd_err;
			SHELL_Transmit(Shell, "Log records: %d\r", LogSize);
			for (i = 0; i < LogSize; i++)
			{
				if (UCD90120A_ReadFaultLogRecord(&ucd90120a, i, &LogRecord))
					goto cmd_err;
				switch (LogRecord.Type)
				{
					case UCD90120A_FAULT_LOG_RECORD_TYPE_SWD:
						SHELL_Transmit(Shell,   "%d-%d: System Watchdog Timeout\r", LogRecord.Day, LogRecord.Time);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_SEQ:
						SHELL_Transmit(Shell,   "%d-%d: Resequense Error\r", LogRecord.Day, LogRecord.Time);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_WD:
						SHELL_Transmit(Shell,   "%d-%d: Watchdog Timeout\r", LogRecord.Day, LogRecord.Time);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_FAN:
						SHELL_Transmit(Shell,   "%d-%d: Fan Fault\r", LogRecord.Day, LogRecord.Time);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_GPI:
						SHELL_Transmit(Shell,   "%d-%d: GPI Fault\r", LogRecord.Day, LogRecord.Time);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_VOUT_OV:
						SHELL_Transmit(Shell,   "%d-%d: Rail %d VOUT_OV %.1fV\r", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_VOUT_UV:
						SHELL_Transmit(Shell,   "%d-%d: Rail %d VOUT_UV %.1fV\r", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_TON_MAX:
						SHELL_Transmit(Shell,   "%d-%d: Rail %d TON_MAX %.1fV\r", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_IOUT_OC:
						SHELL_Transmit(Shell,   "%d-%d: Rail %d IOUT_OC %.1fA\r", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_IOUT_UC:
						SHELL_Transmit(Shell,   "%d-%d: Rail %d IOUT_UC %.1fA\r", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_TEMP_OT:
						SHELL_Transmit(Shell,   "%d-%d: Rail %d TEMPERATURE_OT %.1fA\r", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.f);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_SEQ_ON:
						SHELL_Transmit(Shell,   "%d-%d: Rail %d Sequence On Timeout 0x%x\r", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.u);
						break;
					case UCD90120A_FAULT_LOG_RECORD_TYPE_RAIL_SEQ_OFF:
						SHELL_Transmit(Shell,   "%d-%d: Rail %d Sequence Off Timeout 0x%x\r", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Value.u);
						break;
					default:
						SHELL_Transmit(Shell,   "%d-%d: Rail %d Reserved(0x%x) 0x%x\r", LogRecord.Day, LogRecord.Time, LogRecord.Rail, LogRecord.Type, LogRecord.Value.u);
						break;
				}
			}
		}
		else if (!strcmp(CmdParamsList[1], "clr"))
		{
			if (UCD90120A_ClearFaultLog(&ucd90120a))
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
	SHELL_Transmit(Shell, "Error: invalid command\r");
	return;
cmd_err:
	SHELL_Transmit(Shell, "Error: command error\r");
	return;
}

#define SHELL_CMD_GDT_HELP \
"Usage:\r"\
"    gdt <cmd> [<data1> <data2> ...]\r"\
"Commands:\r"\
"    help - Display this help.\r"\
"    select <idx> - Select device.\r"\
"    mntr - Display device monitor values.\r"\
"    vout <data> - Set device Vout value.\r"

static void SHELL_CMD_GDTCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t Data;
	float Vout;
	static uint32_t Idx = 0;
	char *p;
	
	if (CmdParamsCnt == 1)
	{	
		SHELL_Transmit(Shell,   "Index: %d\r", Idx);
		SHELL_Transmit(Shell,   "Init state: %s\r", BSP_DEV_GET_STATE_STR(&gdt080a[Idx]));
		SHELL_Transmit(Shell,   "I2C Addr: 0x%x\r", gdt080a[Idx].Addr >> 1);
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_Transmit(Shell, SHELL_CMD_GDT_HELP);
		}
		else if (!strcmp(CmdParamsList[1], "mntr"))
		{
			if (GDT080A_Update(&gdt080a[(Idx)]))
				goto cmd_err;
			SHELL_Transmit(Shell,   "Set Vout: %2.2f\r", gdt080a[Idx].Cfg.Vout.Value);
			SHELL_Transmit(Shell,   "Vin    Vout    Iout    Temp1    Temp2\r");
			SHELL_Transmit(Shell,   "%2.2f   %2.2f   %2.2f   %2.2f   %2.2f\r",
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
	SHELL_Transmit(Shell, "Error: invalid command\r");
	return;
invalid_idx:
	SHELL_Transmit(Shell, "Error: invalid index\r");
	return;
invalid_data:
	SHELL_Transmit(Shell, "Error: invalid data\r");
	return;
cmd_err:
	SHELL_Transmit(Shell, "Error: command error\r");
	return;
}

#define SHELL_CMD_KSZ_HELP \
"Usage:\r"\
"    ksz <cmd> [<data1> <data2> ...]\r"\
"Commands:\r"\
"    help - Display this help.\r"\
"    mac - Display mac table.\r"\
"    link - Display port link.\r"\
"    r <addr> - Read device register.\r"\
"        Example: ksz r 0x01\r"\
"    w <addr> <data> - Write device register.\r"\
"        Example: ksz w 0x01 0x10\r"

#define KSZ8895_MAC_TABLE_SIZE 64
static KSZ8895_MAC_TABLE_T ksz_mac_table[KSZ8895_MAC_TABLE_SIZE];

static void SHELL_CMD_KSZCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t Addr, Data, i, ksz_mac_table_size;
	uint8_t Data8;
	char *p;
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell,   "Init state: %s\r", BSP_DEV_GET_STATE_STR(&ksz8895));
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_Transmit(Shell, SHELL_CMD_KSZ_HELP);
		}
		else if (!strcmp(CmdParamsList[1], "link"))
		{
			for (i = 0; i < KSZ8895_PORTS_CNT; i++)
			{
				if (KSZ8895_UpdatePortStatus(&ksz8895, i))
					goto cmd_err;
				SHELL_Transmit(Shell, "Port %d:\r", i);
				SHELL_Transmit(Shell, "    Link - %s\r", KSZ8895_PORT_LINK(&ksz8895, i) ? "Up" : "Down");
				SHELL_Transmit(Shell, "    Speed - %d M\r", KSZ8895_PORT_SPEED(&ksz8895, i));
				SHELL_Transmit(Shell, "    Duplex - %s\r", KSZ8895_PORT_DUPLEX(&ksz8895, i) ? "Full" : "Half");
			}
		}
		else if (!strcmp(CmdParamsList[1], "mac"))
		{
			if (KSZ8895_ReadMACTable(&ksz8895, ksz_mac_table, KSZ8895_MAC_TABLE_SIZE, &ksz_mac_table_size))
				goto cmd_err;
			SHELL_Transmit(Shell, "Idx    Port    Mac\r", i);
			for (i = 0; i < ksz_mac_table_size; i++)
			{
				SHELL_Transmit(Shell, "%3d    %4d    %02X:%02X:%02X:%02X:%02X:%02X\r", i, ksz_mac_table[i].Port, ksz_mac_table[i].Mac[0],
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
			
			SHELL_Transmit(Shell, "0x%x\r", Data8);
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
	SHELL_Transmit(Shell, "Error: invalid command\r");
	return;
cmd_err:
	SHELL_Transmit(Shell, "Error: command error\r");
	return;
invalid_addr:
	SHELL_Transmit(Shell, "Error: invalid address\r");
	return;
invalid_data:
	SHELL_Transmit(Shell, "Error: invalid data\r");
	return;
}

#define SHELL_CMD_SDRAM_HELP \
"Usage:\r"\
"    sdram <cmd> [<data1> <data2> ...]\r"\
"Commands:\r"\
"    help - Display this help.\r"\
"    load <filename> - Load file via TFTP.\r"\

static void TFTP_CLIENT_ProgramSDRAM(void *Priv, uint32_t Addr, char *Source, uint32_t Size)
{
	memcpy((char*)Addr, Source, Size);
}

static char Buffer[2048];

static void SHELL_CMD_SDRAMCallback(void *Shell, char **CmdParamsList, uint32_t CmdParamsCnt)
{
	uint32_t Dev, Addr;
	char *p;
	ip_addr_t tftp_server_ip;
	
	if (CmdParamsCnt == 1)
	{
		SHELL_Transmit(Shell,   "Init state: %s\r", BSP_DEV_GET_STATE_STR(&mt48lc16m16a2));
	}
	else if (CmdParamsCnt == 2)
	{
		if (!strcmp(CmdParamsList[1], "help"))
		{
			SHELL_Transmit(Shell, SHELL_CMD_SDRAM_HELP);
		}
		else
		{
			goto invalid_cmd;
		}
	}
	else if (CmdParamsCnt == 3)
	{
		if (!strcmp(CmdParamsList[1], "load"))
		{
			memset((char*)BSP_SDRAM_ADDR, 0xFF, BSP_FLASH_FPGA_1_FW_ADDR_END - BSP_FLASH_FPGA_1_FW_ADDR_START);
			IP_ADDR4(&tftp_server_ip,192,168,2,32);
			TFTP_CLIENT_DownloadFile(CmdParamsList[2], &tftp_server_ip, 69, TFTP_CLIENT_ProgramSDRAM, NULL, BSP_SDRAM_ADDR);
		}
		else if (!strcmp(CmdParamsList[1], "cmp"))
		{
			Dev = strtoul(&CmdParamsList[2][0], &p, 10);
			if ((*p != 0) || (p == &CmdParamsList[2][2]))
				goto invalid_dev;
			
			if (Dev > 2)
				goto invalid_dev;
			
			if (Dev == 0)
			{
				for (Addr = 0; Addr < (BSP_FLASH_FPGA_1_FW_ADDR_END - BSP_FLASH_FPGA_1_FW_ADDR_START); Addr += sizeof(Buffer))
				{
					BSP_FPGA_FLASH_Read(BSP_FLASH_FPGA_1_FW_ADDR_START + Addr, Buffer, sizeof(Buffer));
					if (memcmp((char*)(BSP_SDRAM_ADDR + Addr), Buffer, sizeof(Buffer)))
					{
						SHELL_Transmit(Shell, "Compare Failed: %d\r", Addr);
						return;
					}
				}
			}
			else if (Dev == 1)
			{
				for (Addr = 0; Addr < (BSP_FLASH_FPGA_2_FW_ADDR_END - BSP_FLASH_FPGA_2_FW_ADDR_START); Addr += sizeof(Buffer))
				{
					BSP_FPGA_FLASH_Read(BSP_FLASH_FPGA_2_FW_ADDR_START + Addr, Buffer, sizeof(Buffer));
					if (memcmp((char*)(BSP_SDRAM_ADDR + Addr), Buffer, sizeof(Buffer)))
					{
						SHELL_Transmit(Shell, "Compare Failed: %d\r", Addr);
						return;
					}
				}
			}
			else if (Dev == 2)
			{
				for (Addr = 0; Addr < (BSP_FLASH_FPGA_3_FW_ADDR_END - BSP_FLASH_FPGA_3_FW_ADDR_START); Addr += sizeof(Buffer))
				{
					BSP_FPGA_FLASH_Read(BSP_FLASH_FPGA_3_FW_ADDR_START + Addr, Buffer, sizeof(Buffer));
					if (memcmp((char*)(BSP_SDRAM_ADDR + Addr), Buffer, sizeof(Buffer)))
					{
						SHELL_Transmit(Shell, "Compare Failed: %d\r", Addr);
						return;
					}
				}
			}
			SHELL_Transmit(Shell, "Compare Successful\r");
		}
		else
		{
			goto invalid_cmd;
		}
	}
	return;
	
invalid_cmd:
	SHELL_Transmit(Shell, "Error: invalid command\r");
	return;
invalid_dev:
	SHELL_Transmit(Shell, "Error: invalid device\r");
	return;
}
