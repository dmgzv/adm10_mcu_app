#include "log.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "cmsis_os.h"

#ifdef BSP_CONFIG_USE_LOG

LOG_LINE_T __attribute__((section ("CCM_DATA"))) LogLines[LOG_LINES_CNT];

static uint32_t LogLineIndex = 0;
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId LogMutexId;
osMutexDef(Log);
#endif /*BSP_CONFIG_USE_FREERTOS*/

void __LOG_Init(void)
{
	LogLineIndex = 0;
	memset(LogLines, 0, sizeof(LogLines));
#ifdef BSP_CONFIG_USE_FREERTOS
	LogMutexId = osMutexCreate(osMutex(Log));
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

void __LOG_Add(const char* Category, uint8_t Severity, const char* Object, const char* ObjectId, char *Description, ...)
{
	va_list va;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait( LogMutexId, osWaitForever);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	va_start(va, Description);
	vsnprintf(LogLines[LogLineIndex % LOG_LINES_CNT].Description, LOG_LINE_DESCRIPTION_SIZE, Description, va);
	va_end(va);
	LogLines[LogLineIndex % LOG_LINES_CNT].Description[LOG_LINE_DESCRIPTION_SIZE - 1] = 0;
	LogLines[LogLineIndex % LOG_LINES_CNT].Object = Object;
	LogLines[LogLineIndex % LOG_LINES_CNT].ObjectId = ObjectId;
	LogLines[LogLineIndex % LOG_LINES_CNT].Severity = Severity;
	LogLines[LogLineIndex % LOG_LINES_CNT].Category = Category;
#ifdef BSP_CONFIG_USE_FREERTOS
	LogLines[LogLineIndex % LOG_LINES_CNT].Time = osKernelSysTick();
#endif /*BSP_CONFIG_USE_FREERTOS*/
	LogLineIndex++;
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease( LogMutexId);
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

uint32_t __LOG_GetLinesCnt(void)
{
	return (LogLineIndex >= LOG_LINES_CNT ? LOG_LINES_CNT : LogLineIndex);
}

void __LOG_GetLine(uint32_t Index, LOG_LINE_T *Line)
{
	if (Index >= (LogLineIndex >= LOG_LINES_CNT ? LOG_LINES_CNT : LogLineIndex))
		return;		
	if (LogLineIndex >= LOG_LINES_CNT)
		Index = (LogLineIndex + Index) % LOG_LINES_CNT;
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait( LogMutexId, osWaitForever);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	memcpy(Line, &LogLines[Index], sizeof(LogLines[0]));	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease( LogMutexId);
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

#endif /*BSP_CONFIG_USE_LOG*/
