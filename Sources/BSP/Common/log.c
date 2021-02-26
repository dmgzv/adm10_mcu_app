#include "log.h"
#include "bsp_flash.h"
#include "crc.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef BSP_CONFIG_USE_LOG

#ifdef BSP_CONFIG_USE_LOG_RAM
LOG_T __attribute__((section ("CCM_DATA"))) Log;

osMutexDef(Log);

int32_t __LOG_Init(void)
{	
	if (!Log.MutexId)
		Log.MutexId = osMutexCreate(osMutex(Log));
	
	if (!Log.MutexId)
		return -1;

	osMutexWait( Log.MutexId, osWaitForever);

	Log.LinesIdx = 0;
	memset(Log.Lines, 0, sizeof(Log.Lines));
	
	osMutexRelease( Log.MutexId);
	
	return 0;
}

void __LOG_Add(const char* Category, uint8_t Access, uint8_t Severity, const char* Object, const char* ObjectId, char *Description, ...)
{
	va_list va;
	
	if (!Log.MutexId)
		return;
	
	osMutexWait( Log.MutexId, osWaitForever);
	
	va_start(va, Description);
	vsnprintf(Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Description, LOG_LINE_DESCRIPTION_SIZE, Description, va);
	va_end(va);
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Description[LOG_LINE_DESCRIPTION_SIZE - 1] = 0;
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Object = Object;
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].ObjectId = ObjectId;
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Access = Access;
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Severity = Severity;
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Category = Category;
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Time = osKernelSysTick();
	Log.LinesIdx++;
	
	osMutexRelease( Log.MutexId);
}

uint32_t __LOG_GetLinesCnt(void)
{
	return (Log.LinesIdx >= LOG_LINES_CNT ? LOG_LINES_CNT : Log.LinesIdx);
}

int32_t __LOG_GetLine(uint32_t Index, LOG_LINE_T *Line)
{
	if (Index >= (Log.LinesIdx >= LOG_LINES_CNT ? LOG_LINES_CNT : Log.LinesIdx))
		return -1;	
	if (Log.LinesIdx >= LOG_LINES_CNT)
		Index = (Log.LinesIdx - Index) % LOG_LINES_CNT;
	osMutexWait( Log.MutexId, osWaitForever);
	memcpy(Line, &Log.Lines[Index], sizeof(Log.Lines[0]));	
	osMutexRelease( Log.MutexId);
	return 0;
}
#endif

#ifdef BSP_CONFIG_USE_LOG_ROM

static void __LOG_Thread(void const *argument);

LOG_T /*__attribute__((section ("CCM_DATA")))*/ Log;

osMutexDef(LOG);
osMessageQDef(LOG, LOG_LINES_CNT - 3, uint32_t);
osThreadDef(LOG, __LOG_Thread, osPriorityBelowNormal, 0, 256);

static int32_t __LOG_ReadLine(uint32_t Index, LOG_LINE_T *Line)
{
	uint16_t ReadCrc16, CalcCrc16;
	
	if (BSP_FLASH_WaitReady())
		return -1;
	//BSP_FLASH_ErasePage(BSP_FLASH_LOG_ADDR + (Index * BSP_FLASH_PAGE_SIZE));
	if (BSP_FLASH_Read(BSP_FLASH_LOG_ADDR + (Index * BSP_FLASH_PAGE_SIZE),  (char*)Line, sizeof(*Line)))
		return -1;
	
	ReadCrc16 = Line->Crc16;
	Line->Crc16 = 0;
	CalcCrc16 = Calc_CRC16((char*)Line, sizeof(*Line), 0);	
	if (ReadCrc16 != CalcCrc16)
		return -1;
	
	return 0;	
}

static int32_t __LOG_WriteLine(uint32_t Index, LOG_LINE_T *Line)
{	
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_ProgramPage(BSP_FLASH_LOG_ADDR + (Index * BSP_FLASH_PAGE_SIZE),  (char*)Line, sizeof(*Line)))
		return -1;	
	return 0;	
}

static void __LOG_Thread(void const *argument)
{
	osEvent Event;
	
	while(1)
  {
		Event = osMessageGet(Log.MessageQId, osWaitForever);
		if (Event.status != osEventMessage)
			continue;
		
		__LOG_WriteLine(Event.value.v % (BSP_FLASH_LOG_SIZE / BSP_FLASH_PAGE_SIZE), &Log.Lines[Event.value.v % LOG_LINES_CNT]);
		Log.LinesFlashIdx = Event.value.v % (BSP_FLASH_LOG_SIZE / BSP_FLASH_PAGE_SIZE);
		if (Log.LinesFlashCnt < (BSP_FLASH_LOG_SIZE / BSP_FLASH_PAGE_SIZE))
			Log.LinesFlashCnt++;
	}
}

int32_t __LOG_Init(void)
{
	uint32_t i;
	LOG_LINE_T Line;
	
	Log.LinesIdx = 0xFFFFFFFF;
	Log.LinesFlashIdx = 0xFFFFFFFF;
	Log.LinesFlashCnt = 0;
	memset(Log.Lines, 0, sizeof(Log.Lines));
	
	for (i = 0; i < BSP_FLASH_LOG_SIZE / BSP_FLASH_PAGE_SIZE; i++)
	{
		if (__LOG_ReadLine(i, &Line))
			continue;
		if ((Line.Index == 0) || (Log.LinesIdx < Line.Index))
		{
			Log.LinesIdx = Line.Index;
			Log.LinesFlashIdx = i;
		}
		Log.LinesFlashCnt++;
	}
	
	if (Log.LinesFlashCnt == 0)
	{
		Log.LinesIdx = 0;
		Log.LinesFlashIdx = 0;
	}
	
	Log.MutexId = osMutexCreate(osMutex(LOG));	
	if (!Log.MutexId)
		return -1;
	
	Log.MessageQId = osMessageCreate (osMessageQ(LOG), NULL);
	if (!Log.MessageQId)
		return -1;
	
	Log.ThreadId = osThreadCreate (osThread(LOG), NULL);
	if (!Log.ThreadId)
		return -1;
	
	return 0;
}

void __LOG_Add(const char* Category, uint8_t Access, uint8_t Severity, const char* Object, const char* ObjectId, char *Description, ...)
{
	va_list va;
	
	if ((!Log.MutexId) || (!Log.MessageQId) || (!Log.ThreadId))
		return;
	
	osMutexWait( Log.MutexId, osWaitForever);
	
	Log.LinesIdx++;
	
	va_start(va, Description);
	vsnprintf(Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Description, LOG_LINE_DESCRIPTION_SIZE, Description, va);
	va_end(va);
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Index = Log.LinesIdx;
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Description[LOG_LINE_DESCRIPTION_SIZE - 1] = 0;
	strncpy(Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Object, Object, sizeof(Log.Lines[0].Object) - 1);
	strncpy(Log.Lines[Log.LinesIdx % LOG_LINES_CNT].ObjectId, ObjectId, sizeof(Log.Lines[0].ObjectId) - 1);
	strncpy(Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Category, Category, sizeof(Log.Lines[0].Category) - 1);
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Access = Access;
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Severity = Severity;
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Time = osKernelSysTick();
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Crc16 = 0;
	Log.Lines[Log.LinesIdx % LOG_LINES_CNT].Crc16 = Calc_CRC16((char*)&Log.Lines[Log.LinesIdx % LOG_LINES_CNT], sizeof(Log.Lines[0]), 0);
	osMessagePut(Log.MessageQId, Log.LinesIdx, osWaitForever);
	
	osMutexRelease( Log.MutexId);
}

uint32_t __LOG_GetLinesCnt(void)
{
	return Log.LinesFlashCnt;
}

int32_t __LOG_GetLine(uint32_t Index, LOG_LINE_T *Line)
{
	if (Index >= Log.LinesFlashCnt)
		return -1;
	osMutexWait( Log.MutexId, osWaitForever);
	__LOG_ReadLine((Log.LinesFlashIdx - Index) % (BSP_FLASH_LOG_SIZE / BSP_FLASH_PAGE_SIZE), Line);
	osMutexRelease( Log.MutexId);
	return 0;
}

#endif

#endif /*BSP_CONFIG_USE_LOG*/
