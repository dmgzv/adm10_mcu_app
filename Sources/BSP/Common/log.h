#ifndef __LOG_H
#define __LOG_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include <stdint.h>

#ifdef BSP_CONFIG_USE_LOG

#define LOG_LINE_SIZE	64
#define LOG_LINES_CNT	160

void __LOG_Init(void);
void __LOG_Add(char *str, ...);
void __LOG_AddLine(char *Line);
void __LOG_GetLine(uint32_t Index, uint32_t *Time, char *Line);
uint32_t __LOG_GetLinesCnt(void);

#define LOG_Init() __LOG_Init()
#define LOG_Add(...) __LOG_Add(__VA_ARGS__)
#define LOG_AddLine(Line) __LOG_AddLine(Line)
#define LOG_GetLine(Index, Time, Line) __LOG_GetLine((Index), (Time), (Line))
#define LOG_GetLinesCnt() __LOG_GetLinesCnt()

#else /* BSP_CONFIG_USE_LOG */

#define LOG_Init()
#define LOG_Add(str, ...)
#define LOG_AddLine(Line)
#define LOG_GetLine(Index, Time, Line)	NULL
#define LOG_GetLinesCnt()		0

#endif /* BSP_CONFIG_USE_LOG */

#endif /* __LOG_H */
