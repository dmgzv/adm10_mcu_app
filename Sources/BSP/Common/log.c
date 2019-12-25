#include "log.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "cmsis_os.h"

#ifdef BSP_CONFIG_USE_LOG

char __attribute__((section ("CCM_DATA"))) LogLines[LOG_LINES_CNT][LOG_LINE_SIZE];
const char* __attribute__((section ("CCM_DATA"))) LogName[LOG_LINES_CNT];
const char* __attribute__((section ("CCM_DATA"))) LogId[LOG_LINES_CNT];
uint32_t LogLineIndex = 0;
uint32_t LogLinesCnt = 0;
#ifdef BSP_CONFIG_USE_FREERTOS
static osMutexId LogMutexId;
osMutexDef(Log);
uint32_t __attribute__((section ("CCM_DATA"))) LogLinesTime[LOG_LINES_CNT];
#endif /*BSP_CONFIG_USE_FREERTOS*/

void __LOG_Init(void)
{
	LogLinesCnt = 0;
	LogLineIndex = 0;
	memset(LogLines, 0, sizeof(LogLines));
#ifdef BSP_CONFIG_USE_FREERTOS
	LogMutexId = osMutexCreate(osMutex(Log));
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

void __LOG_Add(const char *Name, const char *Id, char *str, ...)
{
	va_list va;
	
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait( LogMutexId, osWaitForever);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	va_start(va, str);
	vsnprintf(LogLines[LogLineIndex % LOG_LINES_CNT], LOG_LINE_SIZE, str, va);
	va_end(va);
	LogLines[LogLineIndex % LOG_LINES_CNT][LOG_LINE_SIZE - 1] = 0;
	LogName[LogLineIndex % LOG_LINES_CNT] = Name;
	LogId[LogLineIndex % LOG_LINES_CNT] = Id;
#ifdef BSP_CONFIG_USE_FREERTOS
	LogLinesTime[LogLineIndex % LOG_LINES_CNT] = osKernelSysTick();
#endif /*BSP_CONFIG_USE_FREERTOS*/
	LogLineIndex++;
	LogLinesCnt = LogLineIndex < LOG_LINES_CNT ? LogLineIndex : LOG_LINES_CNT;
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease( LogMutexId);
#endif /*BSP_CONFIG_USE_FREERTOS*/
}

uint32_t __LOG_GetLinesCnt(void)
{
	return LogLinesCnt;
}

void __LOG_GetLine(uint32_t Index, uint32_t *Time, const char **Name, const char **Id, const char **Line)
{	
	if (Index >= LOG_LINES_CNT)
		return;
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexWait( LogMutexId, osWaitForever);
#endif /*BSP_CONFIG_USE_FREERTOS*/
	*Line = LogLines[Index];
	*Name = LogName[Index];
	*Id = LogId[Index];
#ifdef BSP_CONFIG_USE_FREERTOS	
	*Time = LogLinesTime[Index];
#endif /*BSP_CONFIG_USE_FREERTOS*/		
#ifdef BSP_CONFIG_USE_FREERTOS
	osMutexRelease( LogMutexId);
#endif /*BSP_CONFIG_USE_FREERTOS*/	
}

#endif /*BSP_CONFIG_USE_LOG*/
