#ifndef __LOG_H
#define __LOG_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include <stdint.h>

#ifdef BSP_CONFIG_USE_LOG

#define LOG_ACCESS_SU 				(0)
#define LOG_ACCESS_ADMIN 			(1 << 0)
#define LOG_ACCESS_USER 			(1 << 1)
#define LOG_ACCESS_MONITOR 		(1 << 2)

#define LOG_SEVERITY_NA 				0 
#define LOG_SEVERITY_NO 				1
#define LOG_SEVERITY_INFO 			2
#define LOG_SEVERITY_NORMAL 		3
#define LOG_SEVERITY_WARNING 		4
#define LOG_SEVERITY_MINOR 			5
#define LOG_SEVERITY_MAJOR 			6
#define LOG_SEVERITY_CRITICAL 	7

#define LOG_LINE_DESCRIPTION_SIZE	64
#define LOG_LINES_CNT	256

typedef struct {
	uint32_t Time;
	const char* Category;
	const char* Object;
	const char* ObjectId;
	uint8_t Access;
	uint8_t Severity;
	char Description[LOG_LINE_DESCRIPTION_SIZE];
} LOG_LINE_T;

void __LOG_Init(void);
void __LOG_Add(const char* Category, uint8_t Access, uint8_t Severity, const char* Object, const char* ObjectId, char *Description, ...);
void __LOG_GetLine(uint32_t Index, LOG_LINE_T *Line);
uint32_t __LOG_GetLinesCnt(void);

#define LOG_Init() __LOG_Init()
#define LOG_Info(Category, Object, ObjectId, ...) __LOG_Add((Category), LOG_ACCESS_SU, LOG_SEVERITY_INFO, (Object), (ObjectId), __VA_ARGS__)
#define LOG_Warn(Category, Object, ObjectId, ...) __LOG_Add((Category), LOG_ACCESS_SU, LOG_SEVERITY_WARNING, (Object), (ObjectId), __VA_ARGS__)
#define LOG_Crit(Category, Object, ObjectId, ...) __LOG_Add((Category), LOG_ACCESS_SU, LOG_SEVERITY_CRITICAL, (Object), (ObjectId), __VA_ARGS__)
#define LOG_GetLine(Index, Line) __LOG_GetLine((Index), (Line))
#define LOG_GetLinesCnt() __LOG_GetLinesCnt()

#else /* BSP_CONFIG_USE_LOG */

#define LOG_Init()
#define LOG_Add(Name, Id, ...)
#define LOG_GetLine(Index, Time, Name, Id, Line)	NULL
#define LOG_GetLinesCnt()		0

#endif /* BSP_CONFIG_USE_LOG */

#endif /* __LOG_H */
