#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H

//#define BSP_FLASH_TYPE_AT45DB081D
//#define BSP_FLASH_TYPE_AT45DB161E
#define BSP_FLASH_TYPE_AT45DB641E

#if defined(BSP_FLASH_TYPE_AT45DB081D)
#include "at45db081d.h"

#define BSP_FLASH_AT45DB081D_SPI	SPI5

#ifndef BSP_CONFIG_USE_SPI5
	#error	Define BSP_CONFIG_USE_SPI5 in bsp_config.h for BSP_FLASH
#endif

#define BSP_FLASH_AT45DB081D_SPI_CS_GPIO_PIN            GPIO_PIN_5
#define BSP_FLASH_AT45DB081D_SPI_CS_GPIO_PORT           GPIOE
#define BSP_FLASH_AT45DB081D_SPI_CS_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE() 

#define BSP_FLASH_AT45DB081D_WP_GPIO_PIN                GPIO_PIN_6
#define BSP_FLASH_AT45DB081D_WP_GPIO_PORT               GPIOE
#define BSP_FLASH_AT45DB081D_WP_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()

#define BSP_FLASH_AT45DB081D_RESET_GPIO_PIN             GPIO_PIN_7
#define BSP_FLASH_AT45DB081D_RESET_GPIO_PORT            GPIOE
#define BSP_FLASH_AT45DB081D_RESET_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOE_CLK_ENABLE()

typedef AT45DB081D_T BSP_FLASH_T;
extern const BSP_PIN_CTRL_T bsp_flash_at45db081d_pin_ctrl;
extern BSP_FLASH_T at45db081d;

#define BSP_FLASH_PAGE_SIZE															AT45DB081D_PAGE_SIZE
#define BSP_FLASH_SIZE																	AT45DB081D_SIZE
#define	BSP_FLASH_Init(Dev)															AT45DB081D_Init((Dev), BSP_FLASH_AT45DB081D_SPI, (BSP_PIN_CTRL_T*)&bsp_flash_at45db081d_pin_ctrl)
#define BSP_FLASH_Read(Dev, Addr, Buffer, Size) 				AT45DB081D_Read((Dev), (Addr), (Buffer), (Size))
#define BSP_FLASH_ProgramPage(Dev, Addr, Buffer, Size) 	AT45DB081D_ProgramPage((Dev), (Addr), (Buffer), (Size))
#define BSP_FLASH_ErasePage(Dev, Addr) 									AT45DB081D_ErasePage((Dev), (Addr))
#define BSP_FLASH_WaitReady(Dev) 												AT45DB081D_WaitReady((Dev))

#elif defined(BSP_FLASH_TYPE_AT45DB161E)
#include "at45db161e.h"

#define BSP_FLASH_AT45DB161E_SPI	SPI5

#ifndef BSP_CONFIG_USE_SPI5
	#error	Define BSP_CONFIG_USE_SPI5 in bsp_config.h for BSP_FLASH
#endif

#define BSP_FLASH_AT45DB161E_SPI_CS_GPIO_PIN            GPIO_PIN_5
#define BSP_FLASH_AT45DB161E_SPI_CS_GPIO_PORT           GPIOE
#define BSP_FLASH_AT45DB161E_SPI_CS_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOE_CLK_ENABLE() 

#define BSP_FLASH_AT45DB161E_WP_GPIO_PIN                GPIO_PIN_6
#define BSP_FLASH_AT45DB161E_WP_GPIO_PORT               GPIOE
#define BSP_FLASH_AT45DB161E_WP_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()

#define BSP_FLASH_AT45DB161E_RESET_GPIO_PIN             GPIO_PIN_7
#define BSP_FLASH_AT45DB161E_RESET_GPIO_PORT            GPIOE
#define BSP_FLASH_AT45DB161E_RESET_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOE_CLK_ENABLE()

typedef AT45DB161E_T BSP_FLASH_T;
extern const BSP_PIN_CTRL_T bsp_flash_at45db161e_pin_ctrl;
extern BSP_FLASH_T at45db161e;

#define BSP_FLASH_PAGE_SIZE															AT45DB161E_PAGE_SIZE
#define BSP_FLASH_SIZE																	AT45DB161E_SIZE
#define	BSP_FLASH_Init(Dev)															AT45DB161E_Init((Dev), BSP_FLASH_AT45DB161E_SPI, (BSP_PIN_CTRL_T*)&bsp_flash_at45db161e_pin_ctrl)
#define BSP_FLASH_Read(Dev, Addr, Buffer, Size) 				AT45DB161E_Read((Dev), (Addr), (Buffer), (Size))
#define BSP_FLASH_ProgramPage(Dev, Addr, Buffer, Size) 	AT45DB161E_ProgramPage((Dev), (Addr), (Buffer), (Size))
#define BSP_FLASH_ErasePage(Dev, Addr) 									AT45DB161E_ErasePage((Dev), (Addr))
#define BSP_FLASH_WaitReady(Dev) 												AT45DB161E_WaitReady((Dev))

#elif defined(BSP_FLASH_TYPE_AT45DB641E)
#include "at45db641e.h"

#define BSP_FLASH_AT45DB641E_ID		"D94"
#define BSP_FLASH_AT45DB641E_SPI	SPI5

#ifndef BSP_CONFIG_USE_SPI5
	#error	Define BSP_CONFIG_USE_SPI5 in bsp_config.h for BSP_FLASH
#endif

#define BSP_FLASH_AT45DB641E_SPI_CS_GPIO_PIN            GPIO_PIN_13
#define BSP_FLASH_AT45DB641E_SPI_CS_GPIO_PORT           GPIOC
#define BSP_FLASH_AT45DB641E_SPI_CS_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE() 

#define BSP_FLASH_AT45DB641E_WP_GPIO_PIN                GPIO_PIN_14
#define BSP_FLASH_AT45DB641E_WP_GPIO_PORT               GPIOC
#define BSP_FLASH_AT45DB641E_WP_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()

#define BSP_FLASH_AT45DB641E_RESET_GPIO_PIN             GPIO_PIN_10
#define BSP_FLASH_AT45DB641E_RESET_GPIO_PORT            GPIOF
#define BSP_FLASH_AT45DB641E_RESET_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOF_CLK_ENABLE()

typedef AT45DB641E_T BSP_FLASH_T;
extern const BSP_PIN_CTRL_T bsp_flash_at45db641e_pin_ctrl; 
extern BSP_FLASH_T at45db641e;
	
#define BSP_FLASH_PAGE_SIZE												AT45DB641E_PAGE_SIZE
#define BSP_FLASH_SIZE														AT45DB641E_SIZE

#define	BSP_FLASH_Init()													AT45DB641E_Init(&at45db641e, BSP_FLASH_AT45DB641E_ID, BSP_FLASH_AT45DB641E_SPI, (BSP_PIN_CTRL_T*)&bsp_flash_at45db641e_pin_ctrl)
#define BSP_FLASH_Read(Addr, Buffer, Size) 				AT45DB641E_Read(&at45db641e, (Addr), (Buffer), (Size))
#define BSP_FLASH_ProgramPage(Addr, Buffer, Size) AT45DB641E_ProgramPage(&at45db641e, (Addr), (Buffer), (Size))
#define BSP_FLASH_ErasePage(Addr) 								AT45DB641E_ErasePage(&at45db641e, (Addr))
#define BSP_FLASH_WaitReady() 										AT45DB641E_WaitReady(&at45db641e)

#endif

#if (defined(STM32F407VE) || defined(STM32F407ZE) || defined(STM32F407IE))

/* 0x00000000 - 0x00010000 - CONFIGS   */
/* 0x00010000 - 0x00080000 - PROGRAMS  */
/* 0x00080000 - 0x00100000 - LANGPACKS */

#define BSP_FLASH_CFG_ADDR												0x00000000
#define BSP_FLASH_CFG_SIZE												0x00010000
#define BSP_FLASH_FW_ADDR													(BSP_FLASH_CFG_ADDR + BSP_FLASH_CFG_SIZE)
#define BSP_FLASH_FW_SIZE													0x00070000
#define BSP_FLASH_LANG_ADDR												(BSP_FLASH_FW_ADDR + BSP_FLASH_FW_SIZE)
#define BSP_FLASH_LANG_SIZE												0x00080000

#if ( BSP_FLASH_SIZE < (BSP_FLASH_LANG_ADDR + BSP_FLASH_LANG_SIZE))
	#error	BSP_FLASH_SIZE is too small
#endif

#elif (defined(STM32F407VG) || defined(STM32F407ZG) || defined(STM32F407IG) || \
		defined(STM32F427VG) || defined(STM32F427ZG) || defined(STM32F427IG))

/* 0x00000000 - 0x00010000 - CONFIGS   */
/* 0x00010000 - 0x00100000 - PROGRAMS  */
/* 0x00180000 - 0x00180000 - LANGPACKS */

#define BSP_FLASH_CFG_ADDR												0x00000000
#define BSP_FLASH_CFG_SIZE												0x00010000
#define BSP_FLASH_FW_ADDR													(BSP_FLASH_CFG_ADDR + BSP_FLASH_CFG_SIZE)
#define BSP_FLASH_FW_SIZE													0x000F0000
#define BSP_FLASH_LANG_ADDR												(BSP_FLASH_FW_ADDR + BSP_FLASH_FW_SIZE)
#define BSP_FLASH_LANG_SIZE												0x00080000

#if ( BSP_FLASH_SIZE < (BSP_FLASH_LANG_ADDR + BSP_FLASH_LANG_SIZE))
	#error	BSP_FLASH_SIZE is too small
#endif

#elif (defined(STM32F427VI) || defined(STM32F427ZI) || defined(STM32F427II))

/* 0x00000000 - 0x00010000 - CONFIGS   */
/* 0x00010000 - 0x00200000 - PROGRAMS  */
/* 0x00200000 - 0x00280000 - LANGPACKS */

#define BSP_FLASH_CFG_ADDR												0x00000000
#define BSP_FLASH_CFG_SIZE												0x00010000
#define BSP_FLASH_FW_ADDR													(BSP_FLASH_CFG_ADDR + BSP_FLASH_CFG_SIZE)
#define BSP_FLASH_FW_SIZE													0x001F0000
#define BSP_FLASH_LANG_ADDR												(BSP_FLASH_FW_ADDR + BSP_FLASH_FW_SIZE)
#define BSP_FLASH_LANG_SIZE												0x00080000

#if ( BSP_FLASH_SIZE < (BSP_FLASH_LANG_ADDR + BSP_FLASH_LANG_SIZE))
	#error	BSP_FLASH_SIZE is too small
#endif

#endif

#endif /* __BSP_FLASH_H */
