#ifndef __BSP_FLASH_H
#define __BSP_FLASH_H

//#define BSP_FLASH_TYPE_AT45DB081D
//#define BSP_FLASH_TYPE_AT45DB161E
#define BSP_FLASH_TYPE_AT45DB641E

#if defined(BSP_FLASH_TYPE_AT45DB081D)
#include "at45db081d.h"

#define BSP_FLASH_AT45DB081D_SPI	SPI1

#ifndef BSP_CONFIG_USE_SPI1
	#error	Define BSP_CONFIG_USE_SPI1 in bsp_config.h for BSP_FLASH
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

#define BSP_FLASH_PAGE_SIZE												AT45DB081D_PAGE_SIZE
#define BSP_FLASH_SIZE														AT45DB081D_SIZE

/* 0x00000000 - 0x00010000 - CONFIGS   */
/* 0x00010000 - 0x00080000 - PROGRAMS  */
/* 0x00080000 - 0x00100000 - LANGPACKS */

#define BSP_FLASH_CFG_ADDR												0x00000000
#define BSP_FLASH_CFG_SIZE												0x00010000
#define BSP_FLASH_FW_ADDR													(BSP_FLASH_CFG_ADDR + BSP_FLASH_CFG_SIZE)
#define BSP_FLASH_FW_SIZE													0x00070000
#define BSP_FLASH_LANG_ADDR												(BSP_FLASH_FW_ADDR + BSP_FLASH_FW_SIZE)
#define BSP_FLASH_LANG_SIZE												0x00080000

#define	BSP_FLASH_Init()													AT45DB081D_Init(&at45db081d, BSP_FLASH_AT45DB081D_SPI, (BSP_PIN_CTRL_T*)&bsp_flash_at45db081d_pin_ctrl)
#define BSP_FLASH_Read(Addr, Buffer, Size) 				AT45DB081D_Read(&at45db081d, (Addr), (Buffer), (Size))
#define BSP_FLASH_ProgramPage(Addr, Buffer, Size) AT45DB081D_ProgramPage(&at45db081d, (Addr), (Buffer), (Size))
#define BSP_FLASH_ErasePage(Addr) 								AT45DB081D_ErasePage(&at45db081d, (Addr))
#define BSP_FLASH_WaitReady() 										AT45DB081D_WaitReady(&at45db081d)

#elif defined(BSP_FLASH_TYPE_AT45DB161E)
#include "at45db161e.h"

#define BSP_FLASH_AT45DB161E_SPI	SPI1

#ifndef BSP_CONFIG_USE_SPI1
	#error	Define BSP_CONFIG_USE_SPI1 in bsp_config.h for BSP_FLASH
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

#define BSP_FLASH_PAGE_SIZE												AT45DB161E_PAGE_SIZE
#define BSP_FLASH_SIZE														AT45DB161E_SIZE

/* 0x00000000 - 0x00010000 - CONFIGS   */
/* 0x00010000 - 0x00080000 - PROGRAMS  */
/* 0x00080000 - 0x00100000 - LANGPACKS */

#define BSP_FLASH_CFG_ADDR												0x00000000
#define BSP_FLASH_CFG_SIZE												0x00010000
#define BSP_FLASH_FW_ADDR													(BSP_FLASH_CFG_ADDR + BSP_FLASH_CFG_SIZE)
#define BSP_FLASH_FW_SIZE													0x00070000
#define BSP_FLASH_LANG_ADDR												(BSP_FLASH_FW_ADDR + BSP_FLASH_FW_SIZE)
#define BSP_FLASH_LANG_SIZE												0x00080000 

#define	BSP_FLASH_Init()													AT45DB161D_Init(&at45db161e, BSP_FLASH_AT45DB161D_SPI, (BSP_PIN_CTRL_T*)&bsp_flash_at45db161e_pin_ctrl)
#define BSP_FLASH_Read(Addr, Buffer, Size) 				AT45DB161D_Read(&at45db161e, (Addr), (Buffer), (Size))
#define BSP_FLASH_ProgramPage(Addr, Buffer, Size) AT45DB161D_ProgramPage(&at45db161e, (Addr), (Buffer), (Size))
#define BSP_FLASH_ErasePage(Addr) 								AT45DB161D_ErasePage(&at45db161e, (Addr))
#define BSP_FLASH_WaitReady() 										AT45DB161D_WaitReady(&at45db161e)

#elif defined(BSP_FLASH_TYPE_AT45DB641E)
#include "at45db641e.h"

#define BSP_FLASH_AT45DB641E_SPI	SPI5

#ifndef BSP_CONFIG_USE_SPI5
	#error	Define BSP_CONFIG_USE_SPI5 in bsp_config.h for BSP_FLASH
#endif

#define BSP_FLASH_AT45DB641E_SPI_CS_GPIO_PIN            GPIO_PIN_5
#define BSP_FLASH_AT45DB641E_SPI_CS_GPIO_PORT           GPIOA
#define BSP_FLASH_AT45DB641E_SPI_CS_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE() 

#define BSP_FLASH_AT45DB641E_WP_GPIO_PIN                GPIO_PIN_6
#define BSP_FLASH_AT45DB641E_WP_GPIO_PORT               GPIOA
#define BSP_FLASH_AT45DB641E_WP_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()

#define BSP_FLASH_AT45DB641E_RESET_GPIO_PIN             GPIO_PIN_13
#define BSP_FLASH_AT45DB641E_RESET_GPIO_PORT            GPIOC
#define BSP_FLASH_AT45DB641E_RESET_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()

typedef AT45DB641E_T BSP_FLASH_T;
extern const BSP_PIN_CTRL_T bsp_flash_at45db641e_pin_ctrl; 
extern BSP_FLASH_T at45db641e;
	
#define BSP_FLASH_PAGE_SIZE												AT45DB641E_PAGE_SIZE
#define BSP_FLASH_SIZE														AT45DB641E_SIZE

/* 0x00000000 - 0x00010000 - CONFIGS   */
/* 0x00010000 - 0x00080000 - PROGRAMS  */
/* 0x00080000 - 0x00100000 - LANGPACKS */

#define BSP_FLASH_CFG_ADDR												0x00000000
#define BSP_FLASH_CFG_SIZE												0x00010000
#define BSP_FLASH_FW_ADDR													(BSP_FLASH_CFG_ADDR + BSP_FLASH_CFG_SIZE)
#define BSP_FLASH_FW_SIZE													0x00070000
#define BSP_FLASH_LANG_ADDR												(BSP_FLASH_FW_ADDR + BSP_FLASH_FW_SIZE)
#define BSP_FLASH_LANG_SIZE												0x00080000

#define	BSP_FLASH_Init()													AT45DB641E_Init(&at45db641e, BSP_FLASH_AT45DB641E_SPI, (BSP_PIN_CTRL_T*)&bsp_flash_at45db641e_pin_ctrl)
#define BSP_FLASH_Read(Addr, Buffer, Size) 				AT45DB641E_Read(&at45db641e, (Addr), (Buffer), (Size))
#define BSP_FLASH_ProgramPage(Addr, Buffer, Size) AT45DB641E_ProgramPage(&at45db641e, (Addr), (Buffer), (Size))
#define BSP_FLASH_ErasePage(Addr) 								AT45DB641E_ErasePage(&at45db641e, (Addr))
#define BSP_FLASH_WaitReady() 										AT45DB641E_WaitReady(&at45db641e)

#endif

#endif /* __BSP_FLASH_H */
