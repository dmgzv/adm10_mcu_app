#ifndef __ADC_H
#define __ADC_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "stm32f4xx_hal.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

/* Definition for ADC1 Channel Pin */
#define ADC_CHANNEL_GPIO_PIN           GPIO_PIN_3
#define ADC_CHANNEL_GPIO_PORT          GPIOA

/* Definition for ADC1's Channel */
#define ADC_CHANNEL                    ADC_CHANNEL_3

#ifdef BSP_CONFIG_USE_FREERTOS
#define ADC_IRQ_PRIORITY               (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#else
#define ADC_IRQ_PRIORITY               (0)
#endif

int32_t ADC_Init(void);
void ADC_Deinit(void);
float ADC_GetVoltage(void);

#endif /* __ADC_H */
