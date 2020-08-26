#ifndef __BSP_H
#define __BSP_H

/* Includes ------------------------------------------------------------------*/
#include "clock.h"
#include "i2c.h"
#include "uart.h"
#include "spi.h"
#include "flash.h"
#include "bsp_config.h"
#include "bsp_led.h"
#include "bsp_flash.h"
#include "bsp_sdram.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_seq.h"
#include "bsp_fpga_iface.h"
#include "bsp_fpga_maxv.h"
#include "bsp_fpga_fw.h"
#include "bsp_fpga_line.h"
#include "bsp_fpga_client.h"
#include "bsp_fpga_pwr.h"
#include "bsp_fpga_flash.h"
#include "bsp_sfp.h"
#include "bsp_sfp_i2c_switch.h"
#include "bsp_sfp_pin.h"
#include "bsp_clk.h"
#include "bsp_eth.h"
#include "bsp_eth_pin.h"
#include "bsp_eth_sw.h"

#define BSP_SLOT_ADDR_GPIO_CNT	4

#define BSP_SLOT_ADDR_0_GPIO_PIN          		GPIO_PIN_9
#define BSP_SLOT_ADDR_0_GPIO_PORT             GPIOA
#define BSP_SLOT_ADDR_0_GPIO_MODE							GPIO_MODE_INPUT
#define BSP_SLOT_ADDR_0_GPIO_PULL							GPIO_PULLUP
#define BSP_SLOT_ADDR_0_GPIO_SPEED						GPIO_SPEED_FAST
#define BSP_SLOT_ADDR_0_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_SLOT_ADDR_1_GPIO_PIN          		GPIO_PIN_10
#define BSP_SLOT_ADDR_1_GPIO_PORT             GPIOA
#define BSP_SLOT_ADDR_1_GPIO_MODE							GPIO_MODE_INPUT
#define BSP_SLOT_ADDR_1_GPIO_PULL							GPIO_PULLUP
#define BSP_SLOT_ADDR_1_GPIO_SPEED						GPIO_SPEED_FAST
#define BSP_SLOT_ADDR_1_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_SLOT_ADDR_2_GPIO_PIN          		GPIO_PIN_11
#define BSP_SLOT_ADDR_2_GPIO_PORT             GPIOA
#define BSP_SLOT_ADDR_2_GPIO_MODE							GPIO_MODE_INPUT
#define BSP_SLOT_ADDR_2_GPIO_PULL							GPIO_PULLUP
#define BSP_SLOT_ADDR_2_GPIO_SPEED						GPIO_SPEED_FAST
#define BSP_SLOT_ADDR_2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_SLOT_ADDR_3_GPIO_PIN          		GPIO_PIN_12
#define BSP_SLOT_ADDR_3_GPIO_PORT             GPIOA
#define BSP_SLOT_ADDR_3_GPIO_MODE							GPIO_MODE_INPUT
#define BSP_SLOT_ADDR_3_GPIO_PULL							GPIO_PULLUP
#define BSP_SLOT_ADDR_3_GPIO_SPEED						GPIO_SPEED_FAST
#define BSP_SLOT_ADDR_3_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()

extern uint8_t SlotAddress;
extern const BSP_DEV_T *Devices[];
extern const uint32_t DevicesCnt;
extern const BSP_DEV_T *Modules[];
extern const uint32_t ModulesCnt;

#define BSP_Reset HAL_NVIC_SystemReset
void BSP_Init(void);

int32_t BSP_InitInterfaces(void);
void BSP_DeinitInterfaces(void);
void BSP_InitSlotAddress(void);
void BSP_InitLeds(void);
int32_t BSP_InitMemory(void);
void BSP_UpdateCard(void);

#endif /* __BSP_H */
