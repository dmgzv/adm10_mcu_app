#ifndef __BSP_SDRAM_H
#define __BSP_SDRAM_H

#define BSP_SDRAM_TYPE_MT48LC16M16A2

#if defined(BSP_SDRAM_TYPE_MT48LC16M16A2)
#include "mt48lc16m16a2.h"

typedef MT48LC16M16A2_T BSP_SDRAM_T;

#define BSP_SDRAM_ADDR																	MT48LC16M16A2_ADDR
#define BSP_SDRAM_SIZE																	MT48LC16M16A2_SIZE
#define	BSP_SDRAM_Init(Dev)															MT48LC16M16A2_Init((Dev))

#endif /* defined(BSP_SDRAM_TYPE_MT48LC16M16A2) */

#endif /* __BSP_SDRAM_H */
