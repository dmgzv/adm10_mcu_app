#ifndef __BSP_FPGA_FLASH_H
#define __BSP_FPGA_FLASH_H

#include "s29gl01gs.h"

#define BSP_FLASH_S29GL01GS_ID					"D86"

/* 0x00000000 - 0x04000000 - FPGA Boot Section */
/* 0x00000000 - 0x08000000 - FPGA Update Section */

#define BSP_FLASH_FPGA_BOOT_SECTION		0
#define BSP_FLASH_FPGA_UPDATE_SECTION	1

#define BSP_FLASH_FPGA_1_FW_ADDR_START (0x00000000)
#define BSP_FLASH_FPGA_1_FW_ADDR_END	 (0x02000000)
#define BSP_FLASH_FPGA_1_FW_PAGES_CNT	 ((BSP_FLASH_FPGA_1_FW_ADDR_END - BSP_FLASH_FPGA_1_FW_ADDR_START) / BSP_FLASH_PAGE_SIZE)

#define BSP_FLASH_FPGA_2_FW_ADDR_START (0x02000000)
#define BSP_FLASH_FPGA_2_FW_ADDR_END	 (0x03000000)
#define BSP_FLASH_FPGA_2_FW_PAGES_CNT	 ((BSP_FLASH_FPGA_2_FW_ADDR_END - BSP_FLASH_FPGA_2_FW_ADDR_START) / BSP_FLASH_PAGE_SIZE)

#define BSP_FLASH_FPGA_3_FW_ADDR_START (0x03000000)
#define BSP_FLASH_FPGA_3_FW_ADDR_END	 (0x04000000)
#define BSP_FLASH_FPGA_3_FW_PAGES_CNT	 ((BSP_FLASH_FPGA_2_FW_ADDR_END - BSP_FLASH_FPGA_2_FW_ADDR_START) / BSP_FLASH_PAGE_SIZE)

#define BSP_FLASH_FPGA_3_FW_ADDR_START (0x03000000)
#define BSP_FLASH_FPGA_3_FW_ADDR_END	 (0x04000000)
#define BSP_FLASH_FPGA_3_FW_PAGES_CNT	 ((BSP_FLASH_FPGA_2_FW_ADDR_END - BSP_FLASH_FPGA_2_FW_ADDR_START) / BSP_FLASH_PAGE_SIZE)

extern S29GL01GS_T s29gl01gs;

#define	BSP_FPGA_FLASH_Init()															S29GL01GS_Init(&s29gl01gs)
#define BSP_FPGA_FLASH_Read(Addr, Buffer, Size) 					S29GL01GS_Read(&s29gl01gs, (Addr), (Buffer), (Size))
#define BSP_FPGA_FLASH_ProgramSector(Addr, Buffer, Size)	S29GL01GS_ProgramSector(&s29gl01gs, (Addr), (Buffer), (Size))
#define BSP_FPGA_FLASH_EraseSectorErase(Addr) 						S29GL01GS_EraseSector(&s29gl01gs, (Addr))
#define BSP_FPGA_FLASH_WaitReady() 												S29GL01GS_WaitReady(&s29gl01gs)

int32_t BSP_FPGA_FLASH_EraseSection(uint32_t Section);
int32_t BSP_FPGA_FLASH_WriteSection(uint32_t Section, uint32_t Addr, char *Data, uint32_t Size);
int32_t BSP_FPGA_FLASH_ReadSection(uint32_t Section, uint32_t Addr, char *Data, uint32_t Size);

void BSP_FPGA_FLASH_Process(void);

#endif /* __BSP_FPGA_FLASH_H */
