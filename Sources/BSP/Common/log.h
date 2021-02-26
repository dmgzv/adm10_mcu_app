#ifndef __LOG_H
#define __LOG_H

/* Includes ------------------------------------------------------------------*/
#include "bsp_config.h"
#include "cmsis_os.h"
#include <stdint.h>

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

#ifdef BSP_CONFIG_USE_LOG

#ifdef BSP_CONFIG_USE_LOG_RAM

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

typedef struct {
	osMutexId MutexId;
	uint32_t LineIndex;
	LOG_LINE_T LogLines[LOG_LINES_CNT];
} LOG_T;

int32_t __LOG_Init(void);
void __LOG_Add(const char* Category, uint8_t Access, uint8_t Severity, const char* Object, const char* ObjectId, char *Description, ...);
int32_t __LOG_GetLine(uint32_t Index, LOG_LINE_T *Line);
uint32_t __LOG_GetLinesCnt(void);

#define LOG_Init() __LOG_Init()
#define LOG_Info(Category, Object, ObjectId, ...) __LOG_Add((Category), LOG_ACCESS_SU, LOG_SEVERITY_INFO, (Object), (ObjectId), __VA_ARGS__)
#define LOG_Warn(Category, Object, ObjectId, ...) __LOG_Add((Category), LOG_ACCESS_SU, LOG_SEVERITY_WARNING, (Object), (ObjectId), __VA_ARGS__)
#define LOG_Crit(Category, Object, ObjectId, ...) __LOG_Add((Category), LOG_ACCESS_SU, LOG_SEVERITY_CRITICAL, (Object), (ObjectId), __VA_ARGS__)
#define LOG_GetLine(Index, Line) __LOG_GetLine((Index), (Line))
#define LOG_GetLinesCnt() __LOG_GetLinesCnt()
#endif

#ifdef BSP_CONFIG_USE_LOG_ROM

#define LOG_LINE_CATEGORY_SIZE		16
#define LOG_LINE_OBJECT_SIZE			64
#define LOG_LINE_SUBJECT_SIZE			64
#define LOG_LINE_DESCRIPTION_SIZE	64
#define LOG_LINES_CNT	64

typedef struct {
	uint32_t Index;
	uint32_t Time;
	char Category[LOG_LINE_CATEGORY_SIZE];
	char Object[LOG_LINE_OBJECT_SIZE];
	char ObjectId[LOG_LINE_SUBJECT_SIZE];	
	char Description[LOG_LINE_DESCRIPTION_SIZE];
	uint8_t Access;
	uint8_t Severity;
	uint16_t Crc16;
} LOG_LINE_T;

typedef struct {
	osMutexId MutexId;
	osMessageQId MessageQId;
	osThreadId ThreadId;
	uint32_t LinesIdx;
	uint32_t LinesFlashIdx;
	uint32_t LinesFlashCnt;
	LOG_LINE_T Lines[LOG_LINES_CNT];
} LOG_T;

int32_t __LOG_Init(void);
void __LOG_Add(const char* Category, uint8_t Access, uint8_t Severity, const char* Object, const char* ObjectId, char *Description, ...);
int32_t __LOG_GetLine(uint32_t Index, LOG_LINE_T *Line);
uint32_t __LOG_GetLinesCnt(void);

#define LOG_Init() __LOG_Init()
#define LOG_Info(Category, Object, ObjectId, ...) __LOG_Add((Category), LOG_ACCESS_SU, LOG_SEVERITY_INFO, (Object), (ObjectId), __VA_ARGS__)
#define LOG_Warn(Category, Object, ObjectId, ...) __LOG_Add((Category), LOG_ACCESS_SU, LOG_SEVERITY_WARNING, (Object), (ObjectId), __VA_ARGS__)
#define LOG_Crit(Category, Object, ObjectId, ...) __LOG_Add((Category), LOG_ACCESS_SU, LOG_SEVERITY_CRITICAL, (Object), (ObjectId), __VA_ARGS__)
#define LOG_GetLine(Index, Line) __LOG_GetLine((Index), (Line))
#define LOG_GetLinesCnt() __LOG_GetLinesCnt()

#endif

#else /* BSP_CONFIG_USE_LOG */

#define LOG_Init() 0
#define LOG_Info(Category, Object, ObjectId, ...)
#define LOG_Warn(Category, Object, ObjectId, ...)
#define LOG_Crit(Category, Object, ObjectId, ...)
#define LOG_GetLine(Index, Line)	-1
#define LOG_GetLinesCnt() 0

#endif /* BSP_CONFIG_USE_LOG */

#endif /* __LOG_H */
