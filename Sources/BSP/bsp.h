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
#include "bsp_pwr_pin.h"
#include "bsp_pwr.h"
#include "bsp_flash.h"
#include "bsp_sfp_i2c_switch.h"
#include "bsp_sfp_pin.h"
#include "bsp_sfp.h"
#include "bsp_clk.h"
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

typedef struct {	
	BSP_LED_T PowerLed;
	BSP_LED_T AlarmLed;
	BSP_PWR_PIN_T PowerPin;
	BSP_PWR_T Power;
	BSP_SFP_I2C_SWITCH_T SfpSwitch;
	BSP_SFP_PIN_T SfpPin;
	BSP_SFP_T Sfp[BSP_SFP_CNT];
	BSP_FLASH_T Flash;
	BSP_CLK_T Clk;
	BSP_ETH_PIN_T EthPin;
	BSP_ETH_SW_T EthSw;
}BSP_DEVICES_T;

typedef struct {
	BSP_SFP_CFG_T Sfp[BSP_SFP_CNT];
} BSP_DEVICES_CFG_T;

#define BSP_Reset HAL_NVIC_SystemReset
int32_t BSP_Init(void);
int32_t BSP_InitPeriferia(void);
void BSP_DeinitPeriferia(void);
int32_t BSP_InitDevices(BSP_DEVICES_T *Devices);
void BSP_DeinitDevices(BSP_DEVICES_T *Devices);
int32_t BSP_UpdateDevices(BSP_DEVICES_T *Devices, BSP_DEVICES_CFG_T *Cfg);

uint8_t BSP_GetSlotAddress(void);

#endif /* __BSP_H */
