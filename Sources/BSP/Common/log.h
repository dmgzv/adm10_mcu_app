#ifndef __LOG_H
#define __LOG_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include <stdint.h>

#ifdef BSP_CONFIG_USE_LOG

#define LOG_LINE_SIZE	64
#define LOG_LINES_CNT	256

void __LOG_Init(void);
void __LOG_Add(const char *Name, const char *Id, char *str, ...);
void __LOG_GetLine(uint32_t Index, uint32_t *Time, const char **Name, const char **Id, const char **Line);
uint32_t __LOG_GetLinesCnt(void);

#define LOG_Init() __LOG_Init()
#define LOG_Add(Name, Id, ...) __LOG_Add((Name), (Id), __VA_ARGS__)
#define LOG_GetLine(Index, Time, Name, Id, Line) __LOG_GetLine((Index), (Time), (Name), (Id), (Line))
#define LOG_GetLinesCnt() __LOG_GetLinesCnt()

#else /* BSP_CONFIG_USE_LOG */

#define LOG_Init()
#define LOG_Add(Name, Id, ...)
#define LOG_GetLine(Index, Time, Name, Id, Line)	NULL
#define LOG_GetLinesCnt()		0

#endif /* BSP_CONFIG_USE_LOG */

#endif /* __LOG_H */
