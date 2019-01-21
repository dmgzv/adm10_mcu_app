#ifndef __I2C_H
#define __I2C_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "bsp_config.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

/* Definition for I2C1 Pins */
#define I2C1_SCL_PIN					GPIO_PIN_6
#define I2C1_SCL_GPIO_PORT		GPIOB
#define I2C1_SCL_AF						GPIO_AF4_I2C1
#define I2C1_SDA_PIN					GPIO_PIN_7
#define I2C1_SDA_GPIO_PORT		GPIOB
#define I2C1_SDA_AF						GPIO_AF4_I2C1

#define I2C1_SCL_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define I2C1_SDA_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define I2C1_IRQ_PRIORITY		(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2)
#else
#define I2C1_IRQ_PRIORITY		(0)
#endif

#define I2C1_ADDRESSINGMODE 	I2C_ADDRESSINGMODE_7BIT
#define I2C1_CLOCKSPEED				(400000)
#define I2C1_DUALADDRESS			I2C_DUALADDRESS_DISABLE
#define I2C1_DUTYCYCLE				I2C_DUTYCYCLE_16_9
#define I2C1_GENERALCALL			I2C_GENERALCALL_DISABLE
#define I2C1_NOSTRETCH				I2C_NOSTRETCH_DISABLE
#define I2C1_ADDRESS 					(0x3E)

/* Definition for I2C2 Pins */
#define I2C2_SCL_PIN					GPIO_PIN_1
#define I2C2_SCL_GPIO_PORT		GPIOF
#define I2C2_SCL_AF						GPIO_AF4_I2C2
#define I2C2_SDA_PIN					GPIO_PIN_0
#define I2C2_SDA_GPIO_PORT		GPIOF
#define I2C2_SDA_AF						GPIO_AF4_I2C2

#define I2C2_SCL_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOF_CLK_ENABLE()
#define I2C2_SDA_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOF_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define I2C2_IRQ_PRIORITY		(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2)
#else
#define I2C2_IRQ_PRIORITY		(0)
#endif

#define I2C2_ADDRESSINGMODE 	I2C_ADDRESSINGMODE_7BIT
#define I2C2_CLOCKSPEED				(400000)
#define I2C2_DUALADDRESS			I2C_DUALADDRESS_DISABLE
#define I2C2_DUTYCYCLE				I2C_DUTYCYCLE_16_9
#define I2C2_GENERALCALL			I2C_GENERALCALL_DISABLE
#define I2C2_NOSTRETCH				I2C_NOSTRETCH_DISABLE
#define I2C2_ADDRESS 					(0x3E)

/* Definition for I2C3 Pins */
#define I2C3_SCL_PIN					GPIO_PIN_8
#define I2C3_SCL_GPIO_PORT		GPIOA
#define I2C3_SCL_AF						GPIO_AF4_I2C3
#define I2C3_SDA_PIN					GPIO_PIN_9
#define I2C3_SDA_GPIO_PORT		GPIOC
#define I2C3_SDA_AF						GPIO_AF4_I2C3

#define I2C3_SCL_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOA_CLK_ENABLE()
#define I2C3_SDA_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOC_CLK_ENABLE()

#ifdef BSP_CONFIG_USE_FREERTOS
#define I2C3_IRQ_PRIORITY		(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 2)
#else
#define I2C3_IRQ_PRIORITY		(0)
#endif

#define I2C3_ADDRESSINGMODE		I2C_ADDRESSINGMODE_7BIT
#define I2C3_CLOCKSPEED				(400000)
#define I2C3_DUALADDRESS			I2C_DUALADDRESS_DISABLE
#define I2C3_DUTYCYCLE				I2C_DUTYCYCLE_16_9
#define I2C3_GENERALCALL			I2C_GENERALCALL_DISABLE
#define I2C3_NOSTRETCH				I2C_NOSTRETCH_DISABLE
#define I2C3_ADDRESS 					(0x3E)

int32_t I2C_Init(void);
void I2C_Deinit(void);

int32_t I2C_Transmit(I2C_TypeDef * Instance, uint16_t DevAddress, char *Buffer, uint16_t Size);
int32_t I2C_Receive(I2C_TypeDef * Instance, uint16_t DevAddress, char *Buffer, uint16_t Size);

int32_t I2C_Sequential_Transmit(I2C_TypeDef * Instance, uint16_t DevAddress, uint8_t *Address, uint16_t AddressSize, char *Buffer, uint16_t BufferSize);
int32_t I2C_Sequential_Receive(I2C_TypeDef * Instance, uint16_t DevAddress, uint8_t *Address, uint16_t AddressSize, char *Buffer, uint16_t BufferSize);

#endif /* __I2C_H */
