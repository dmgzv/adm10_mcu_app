#include "profile_flash.h"
#include "cmsis_os.h"
#include "crc.h"
#include <string.h>

typedef struct {
	uint32_t Idx;
	char Project[sizeof(BSP_PROJECT)];
	char SwVersion[sizeof(BSP_SW_VERSION)];
	char SwDate[sizeof(BSP_SW_DATE)];
	char SwTime[sizeof(BSP_SW_TIME)];
	char HwVersion[sizeof(BSP_HW_VERSION)];
	uint32_t ProfileIdx;
	uint16_t Crc16;
}PROFILE_FLASH_HEADER_T;

#define PROFILE_FLASH_HEADERS_CNT				( 2 )
#define PROFILE_FLASH_HEADER_SIZE				( PROFILE_FLASH_HEADERS_SIZE / PROFILE_FLASH_HEADERS_CNT)
#define PROFILE_FLASH_HEADER_ADDR(Idx)	( PROFILE_FLASH_HEADERS_START + ( PROFILE_FLASH_HEADER_SIZE * (Idx % PROFILE_FLASH_HEADERS_CNT) ) )

typedef struct {
	uint32_t Idx;
	char Project[sizeof(BSP_PROJECT)];
	char SwVersion[sizeof(BSP_SW_VERSION)];
	char SwDate[sizeof(BSP_SW_DATE)];
	char SwTime[sizeof(BSP_SW_TIME)];
	char HwVersion[sizeof(BSP_HW_VERSION)];
	uint32_t ProfileIdx;
	uint32_t CalibsSize;
	uint16_t CalibsCrc16;
	uint16_t Crc16;	
}PROFILE_FLASH_CALIB_HEADER_T;

#define __PROFILE_FLASH_CALIB_PAGES_CNT(ProfileFlash)	\
	( ((ProfileFlash)->CalibsSize) % BSP_FLASH_PAGE_SIZE  ? \
		((ProfileFlash)->CalibsSize) / BSP_FLASH_PAGE_SIZE + 1 : \
		((ProfileFlash)->CalibsSize) / BSP_FLASH_PAGE_SIZE + 0	)

#define PROFILE_FLASH_CALIB_PAGES_CNT 	( PROFILE_FLASH_CALIB_SIZE / BSP_FLASH_PAGE_SIZE )

#define PROFILE_FLASH_CALIB_RECORDS_CNT						( 2 )
#define PROFILE_FLASH_CALIB_RECORD_PAGES_CNT  		( PROFILE_FLASH_CALIB_PAGES_CNT / PROFILE_FLASH_CALIB_RECORDS_CNT )
#define PROFILE_FLASH_CALIB_RECORD_SIZE						( PROFILE_FLASH_CALIB_RECORD_PAGES_CNT * BSP_FLASH_PAGE_SIZE )
#define PROFILE_FLASH_CALIB_RECORD_ADDR(ProfileIdx, RecordIdx) ( PROFILE_FLASH_CALIB_START(ProfileIdx) + (((RecordIdx) % PROFILE_FLASH_CALIB_RECORDS_CNT) * PROFILE_FLASH_CALIB_RECORD_SIZE) )

typedef struct {
	uint32_t Idx;
	char Project[sizeof(BSP_PROJECT)];
	char SwVersion[sizeof(BSP_SW_VERSION)];
	char SwDate[sizeof(BSP_SW_DATE)];
	char SwTime[sizeof(BSP_SW_TIME)];
	char HwVersion[sizeof(BSP_HW_VERSION)];
	uint32_t ProfileIdx;
	uint32_t ConfigsSize;
	uint16_t ConfigsCrc16;
	uint16_t Crc16;	
}PROFILE_FLASH_CONFIG_HEADER_T;

#define __PROFILE_FLASH_CONFIG_PAGES_CNT(ProfileFlash)	\
	( ((ProfileFlash)->ConfigsSize) % BSP_FLASH_PAGE_SIZE  ? \
		((ProfileFlash)->ConfigsSize) / BSP_FLASH_PAGE_SIZE + 1 : \
		((ProfileFlash)->ConfigsSize) / BSP_FLASH_PAGE_SIZE + 0	)

#define PROFILE_FLASH_CONFIG_PAGES_CNT 	( PROFILE_FLASH_CONFIG_SIZE / BSP_FLASH_PAGE_SIZE )

#define PROFILE_FLASH_CONFIG_RECORDS_CNT						( 4 )
#define PROFILE_FLASH_CONFIG_RECORD_PAGES_CNT  			( PROFILE_FLASH_CONFIG_PAGES_CNT / PROFILE_FLASH_CONFIG_RECORDS_CNT )
#define PROFILE_FLASH_CONFIG_RECORD_SIZE						( PROFILE_FLASH_CONFIG_RECORD_PAGES_CNT * BSP_FLASH_PAGE_SIZE )
#define PROFILE_FLASH_CONFIG_RECORD_ADDR(ProfileIdx, RecordIdx) ( PROFILE_FLASH_CONFIG_START(ProfileIdx) + (((RecordIdx) % PROFILE_FLASH_CONFIG_RECORDS_CNT) * PROFILE_FLASH_CONFIG_RECORD_SIZE) )

#define PROFILE_FLASH_MESSAGE_QUEUE_SIZE	( 2 )

static int32_t PROFILE_FLASH_HeaderLoad(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *HeaderIdx, uint32_t *ProfileIdx)
{
	PROFILE_FLASH_HEADER_T Header;
	uint32_t Addr;
	uint16_t Crc16;
	
	Addr = PROFILE_FLASH_HEADER_ADDR(Idx);
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		return -1;
	if ((Header.Idx % PROFILE_FLASH_HEADERS_CNT) != (Idx % PROFILE_FLASH_HEADERS_CNT))
		return -1;
	if (memcmp(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		return -1;
	if (memcmp(Header.SwVersion, BSP_SW_VERSION, sizeof(BSP_SW_VERSION)))
		return -1;
	if (memcmp(Header.SwDate, BSP_SW_DATE, sizeof(BSP_SW_DATE)))
		return -1;
	if (memcmp(Header.SwTime, BSP_SW_TIME, sizeof(BSP_SW_TIME)))
		return -1;
	if (memcmp(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		return -1;
	if (Header.ProfileIdx >= BSP_CONFIG_PROFILES_CNT)
		return -1;
	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		return -1;
	*HeaderIdx = Header.Idx;
	*ProfileIdx = Header.ProfileIdx;
	return 0;
}

int32_t PROFILE_FLASH_ProfileIdxLoad(PROFILE_FLASH_T *ProfileFlash, uint32_t *ProfileIdx)
{
	uint32_t i;
	uint32_t HeaderIdx, HeaderIdxSave;
	uint32_t HeaderProfileIdx, HeaderProfileIdxSave;
	uint32_t HeadersAbs;
	
	HeaderIdx = 0;
	HeaderProfileIdx = 0;
	HeaderIdxSave = 0;
	HeaderProfileIdxSave = 0;
	HeadersAbs = 1;
	for (i = 0; i < PROFILE_FLASH_HEADERS_CNT; i++)
	{
		if (PROFILE_FLASH_HeaderLoad(ProfileFlash, i, &HeaderIdx, &HeaderProfileIdx))
			continue;
		HeadersAbs = 0;
		if (HeaderIdx >= HeaderIdxSave)
		{
			HeaderIdxSave = HeaderIdx;
			HeaderProfileIdxSave = HeaderProfileIdx;
		}
	}
	if (HeadersAbs)
		return -1;
	ProfileFlash->HeaderIdx = HeaderIdxSave + 1;
	ProfileFlash->ProfileIdx = HeaderProfileIdxSave;
	*ProfileIdx = ProfileFlash->ProfileIdx;
	return 0;
}

int32_t PROFILE_FLASH_ProfileIdxSave(PROFILE_FLASH_T *ProfileFlash, uint32_t ProfileIdx)
{
	PROFILE_FLASH_HEADER_T Header;
	uint32_t i, Addr;
	uint16_t Crc16;
	
	memset(&Header, 0, sizeof(Header));
	memcpy(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Header.SwVersion, BSP_SW_VERSION, sizeof(BSP_SW_VERSION));
	memcpy(Header.SwDate, BSP_SW_DATE, sizeof(BSP_SW_DATE));
	memcpy(Header.SwTime, BSP_SW_TIME, sizeof(BSP_SW_TIME));
	memcpy(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Header.Idx = ProfileFlash->HeaderIdx;
	Header.ProfileIdx = ProfileIdx;
	Header.Crc16 = Calc_CRC16((char*)&Header, sizeof(Header), 0);
	
	Addr = PROFILE_FLASH_HEADER_ADDR(Header.Idx);
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		return -1;
	
	memset(&Header, 0, sizeof(Header));
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		return -1;
	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		return -1;
	
	if (Header.Idx == 0)
	{
		for (i = 1; i < PROFILE_FLASH_HEADERS_CNT; i++)
		{
			Addr = PROFILE_FLASH_HEADER_ADDR(i);
			if (BSP_FLASH_WaitReady())
				return -1;
			if (BSP_FLASH_ErasePage(Addr))
				return -1;
		}
	}
	
	ProfileFlash->HeaderIdx++;
	ProfileFlash->ProfileIdx = ProfileIdx;
	return 0;
}

static int32_t __PROFILE_FLASH_SaveCalibs(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CALIB_HEADER_T Overhead;
	char *p;
	uint32_t Addr;
	uint32_t i, Size;
	uint16_t Crc16;
	char Page[BSP_FLASH_PAGE_SIZE];
	
	osMutexWait(ProfileFlash->Mutex, osWaitForever);
	Crc16 = Calc_CRC16((char*)ProfileFlash->Calibs, ProfileFlash->CalibsSize, 0);
	osMutexRelease( ProfileFlash->Mutex );

	p = (char*)ProfileFlash->Calibs;
	
	memset(&Overhead, 0, sizeof(Overhead));
	memcpy(Overhead.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Overhead.SwVersion, BSP_SW_VERSION, sizeof(BSP_SW_VERSION));
	memcpy(Overhead.SwDate, BSP_SW_DATE, sizeof(BSP_SW_DATE));
	memcpy(Overhead.SwTime, BSP_SW_TIME, sizeof(BSP_SW_TIME));
	memcpy(Overhead.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Overhead.Idx = ProfileFlash->CalibIdx;
	Overhead.ProfileIdx = ProfileFlash->ProfileIdx;
	Overhead.CalibsSize = ProfileFlash->CalibsSize;
	Overhead.CalibsCrc16 = Crc16;
	Overhead.Crc16 = Calc_CRC16((char*)&Overhead, sizeof(Overhead), 0);
	
	Addr = PROFILE_FLASH_CALIB_RECORD_ADDR(ProfileFlash->ProfileIdx, ProfileFlash->CalibIdx);
	
	/*First page with overhead*/
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Overhead, sizeof(Overhead)))
		return -1;
	
	memset(&Overhead, 0, sizeof(Overhead));
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_Read(Addr, (char*)&Overhead, sizeof(Overhead)))
		return -1;
	Crc16 = Overhead.Crc16;
	Overhead.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Overhead, sizeof(Overhead), 0))
		return -1;
	
	Addr += BSP_FLASH_PAGE_SIZE;
	
	Crc16 = 0;
	for (i = 0; i < __PROFILE_FLASH_CALIB_PAGES_CNT(ProfileFlash); i++)
	{
		if (i == __PROFILE_FLASH_CALIB_PAGES_CNT(ProfileFlash) - 1)
			Size = ProfileFlash->CalibsSize - (i * BSP_FLASH_PAGE_SIZE);
		else 
			Size = BSP_FLASH_PAGE_SIZE;
		if (BSP_FLASH_WaitReady())
			return -1;
		osMutexWait(ProfileFlash->Mutex, osWaitForever);
		if (BSP_FLASH_ProgramPage(Addr, p, Size))
		{
			osMutexRelease( ProfileFlash->Mutex );
			return -1;
		}
		osMutexRelease( ProfileFlash->Mutex );
		if (BSP_FLASH_WaitReady())
			return -1;
		if (BSP_FLASH_Read(Addr, Page, Size))
			return -1;
		Crc16 = Calc_CRC16(Page, Size, Crc16);
		Addr += Size;
		p += Size;			
	}
	/*Check changes of values during writing process.*/
	if (Crc16 != Overhead.CalibsCrc16)
		return -1;
	
	if (Overhead.Idx == 0)
	{
		for (i = 1; i < PROFILE_FLASH_CALIB_RECORDS_CNT; i++)
		{
			Addr = PROFILE_FLASH_CALIB_RECORD_ADDR(ProfileFlash->ProfileIdx, ProfileFlash->CalibIdx);
			if (BSP_FLASH_WaitReady())
				return -1;
			if (BSP_FLASH_ErasePage(Addr))
				return -1;
		}
	}
	ProfileFlash->CalibIdx++;
	return 0;
}

static int32_t __PROFILE_FLASH_SaveConfigs(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CONFIG_HEADER_T Overhead;
	char *p;
	uint32_t Addr;
	uint32_t i, Size;
	uint16_t Crc16;
	char Page[BSP_FLASH_PAGE_SIZE];
	
	osMutexWait(ProfileFlash->Mutex, osWaitForever);
	Crc16 = Calc_CRC16((char*)ProfileFlash->Configs, ProfileFlash->ConfigsSize, 0);
	osMutexRelease( ProfileFlash->Mutex );

	p = (char*)ProfileFlash->Configs;
	
	memset(&Overhead, 0, sizeof(Overhead));
	memcpy(Overhead.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Overhead.SwVersion, BSP_SW_VERSION, sizeof(BSP_SW_VERSION));
	memcpy(Overhead.SwDate, BSP_SW_DATE, sizeof(BSP_SW_DATE));
	memcpy(Overhead.SwTime, BSP_SW_TIME, sizeof(BSP_SW_TIME));
	memcpy(Overhead.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Overhead.Idx = ProfileFlash->ConfigIdx;
	Overhead.ProfileIdx = ProfileFlash->ProfileIdx;
	Overhead.ConfigsSize = ProfileFlash->ConfigsSize;
	Overhead.ConfigsCrc16 = Crc16;
	Overhead.Crc16 = Calc_CRC16((char*)&Overhead, sizeof(Overhead), 0);
	
	Addr = PROFILE_FLASH_CONFIG_RECORD_ADDR(ProfileFlash->ProfileIdx, ProfileFlash->ConfigIdx);
	
	/*First page with overhead*/
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Overhead, sizeof(Overhead)))
		return -1;
	
	memset(&Overhead, 0, sizeof(Overhead));
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_Read(Addr, (char*)&Overhead, sizeof(Overhead)))
		return -1;
	Crc16 = Overhead.Crc16;
	Overhead.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Overhead, sizeof(Overhead), 0))
		return -1;
	
	Addr += BSP_FLASH_PAGE_SIZE;
	
	Crc16 = 0;
	for (i = 0; i < __PROFILE_FLASH_CONFIG_PAGES_CNT(ProfileFlash); i++)
	{
		if (i == __PROFILE_FLASH_CONFIG_PAGES_CNT(ProfileFlash) - 1)
			Size = ProfileFlash->ConfigsSize - (i * BSP_FLASH_PAGE_SIZE);
		else 
			Size = BSP_FLASH_PAGE_SIZE;
		if (BSP_FLASH_WaitReady())
			return -1;
		osMutexWait(ProfileFlash->Mutex, osWaitForever);
		if (BSP_FLASH_ProgramPage(Addr, p, Size))
		{
			osMutexRelease( ProfileFlash->Mutex );
			return -1;
		}
		osMutexRelease( ProfileFlash->Mutex );
		if (BSP_FLASH_WaitReady())
			return -1;
		if (BSP_FLASH_Read(Addr, Page, Size))
			return -1;
		Crc16 = Calc_CRC16(Page, Size, Crc16);
		Addr += Size;
		p += Size;			
	}
	/*Check changes of values during writing process.*/
	if (Crc16 != Overhead.ConfigsCrc16)
		return -1;
	
	if (Overhead.Idx == 0)
	{
		for (i = 1; i < PROFILE_FLASH_CONFIG_RECORDS_CNT; i++)
		{
			Addr = PROFILE_FLASH_CONFIG_RECORD_ADDR(ProfileFlash->ProfileIdx, ProfileFlash->ConfigIdx);
			if (BSP_FLASH_WaitReady())
				return -1;
			if (BSP_FLASH_ErasePage(Addr))
				return -1;
		}
	}
	ProfileFlash->ConfigIdx++;
	return 0;
}

static void ProfileFlash_Thread(void const *argument)
{
	PROFILE_FLASH_T *ProfileFlash = (PROFILE_FLASH_T*)argument;
	osEvent Event;
	uint32_t ErrorCntr;
	
	while (1)
	{
		ErrorCntr = 0;
    Event = osMessageGet(ProfileFlash->MessageQId, osWaitForever);
		if (Event.status != osEventMessage) 
			continue;
		/*Save Profile to Flash*/
repeat:
		if (Event.value.v == 0)
		{
			if (__PROFILE_FLASH_SaveCalibs(ProfileFlash))
				goto error;
			continue;
		}
		else if (Event.value.v == 1)
		{
			if (__PROFILE_FLASH_SaveConfigs(ProfileFlash))
				goto error;
			continue;
		}
error:
		ErrorCntr++;
		if (ErrorCntr < 25)
			goto repeat;
	}
}

int32_t PROFILE_FLASH_Init(PROFILE_FLASH_T *ProfileFlash, char *Name, void *Calibs, uint32_t CalibsSize, void *Configs, uint32_t ConfigsSize, osMutexId Mutex)
{	
	osThreadDef_t ProfileFlash_osThreadDef = { Name, ProfileFlash_Thread, osPriorityNormal, 0, 512};
	osMessageQDef_t ProfileFlash_osMessageQDef = {PROFILE_FLASH_MESSAGE_QUEUE_SIZE, sizeof(uint32_t)};
	
	ProfileFlash->Calibs = Calibs;
	ProfileFlash->CalibsSize = CalibsSize;
	ProfileFlash->Configs = Configs;
	ProfileFlash->ConfigsSize = ConfigsSize;
	ProfileFlash->Mutex = Mutex;
	ProfileFlash->HeaderIdx = 0;
	ProfileFlash->ConfigIdx = 1;
	ProfileFlash->CalibIdx = 1;
	ProfileFlash->ProfileIdx = 0;
	ProfileFlash->EnableSave = 1;
	ProfileFlash->MessageQId = osMessageCreate (&ProfileFlash_osMessageQDef, NULL);
	if (!ProfileFlash->MessageQId)
		return -1;	
	ProfileFlash->ThreadId = osThreadCreate(&ProfileFlash_osThreadDef, ProfileFlash);	
	if ( !ProfileFlash->ThreadId )
		return -1;
	//PROFILE_FLASH_LangpackClear(ProfileFlash);
	return 0;
}

void PROFILE_FLASH_CalibsSave(PROFILE_FLASH_T *ProfileFlash)
{
	/*Wake up Profile Save Thread*/
	if (ProfileFlash->MessageQId && ProfileFlash->EnableSave)
		osMessagePut(ProfileFlash->MessageQId, 0, 0);	
}

void PROFILE_FLASH_ConfigsSave(PROFILE_FLASH_T *ProfileFlash)
{
	/*Wake up Profile Save Thread*/
	if (ProfileFlash->MessageQId && ProfileFlash->EnableSave)
		osMessagePut(ProfileFlash->MessageQId, 1, 0);	
}

static int32_t PROFILE_FLASH_CalibsRecordLoad(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *RecordIdx)
{
	uint32_t Addr;
	uint16_t Crc16;
	PROFILE_FLASH_CALIB_HEADER_T CalibRecordOverhead;
	
	Addr = PROFILE_FLASH_CALIB_RECORD_ADDR(ProfileFlash->ProfileIdx, Idx);
	if (BSP_FLASH_Read(Addr, (char*)&CalibRecordOverhead, sizeof(CalibRecordOverhead)))
		return -1;
	if ((CalibRecordOverhead.Idx % PROFILE_FLASH_HEADERS_CNT) != (Idx % PROFILE_FLASH_HEADERS_CNT))
		return -1;
	if (memcmp(CalibRecordOverhead.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		return -1;
	if (memcmp(CalibRecordOverhead.SwVersion, BSP_SW_VERSION, sizeof(BSP_SW_VERSION)))
		return -1;
	if (memcmp(CalibRecordOverhead.SwDate, BSP_SW_DATE, sizeof(BSP_SW_DATE)))
		return -1;
	if (memcmp(CalibRecordOverhead.SwTime, BSP_SW_TIME, sizeof(BSP_SW_TIME)))
		return -1;
	if (memcmp(CalibRecordOverhead.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		return -1;
	if (CalibRecordOverhead.ProfileIdx != ProfileFlash->ProfileIdx)
		return -1;
	if (CalibRecordOverhead.CalibsSize != ProfileFlash->CalibsSize)
		return -1;
	Crc16 = CalibRecordOverhead.Crc16;
	CalibRecordOverhead.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&CalibRecordOverhead, sizeof(CalibRecordOverhead), 0))
		return -1;
	
	Addr += BSP_FLASH_PAGE_SIZE;
	
	if( osMutexWait( ProfileFlash->Mutex, osWaitForever ) != osOK )
		return -1;
	if (BSP_FLASH_Read(Addr, ProfileFlash->Calibs, ProfileFlash->CalibsSize))
		goto error;
	Crc16 = Calc_CRC16((char*)ProfileFlash->Calibs, ProfileFlash->CalibsSize, 0);
	if (Crc16 != CalibRecordOverhead.CalibsCrc16)
		goto error;
	osMutexRelease( ProfileFlash->Mutex );
	
	*RecordIdx = CalibRecordOverhead.Idx;
	return 0;

error:
	osMutexRelease( ProfileFlash->Mutex );
	return -1;
}

static int32_t PROFILE_FLASH_ConfigsRecordLoad(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *RecordIdx)
{
	uint32_t Addr;
	uint16_t Crc16;
	PROFILE_FLASH_CONFIG_HEADER_T ConfigRecordOverhead;
	
	Addr = PROFILE_FLASH_CONFIG_RECORD_ADDR(ProfileFlash->ProfileIdx, Idx);
	if (BSP_FLASH_Read(Addr, (char*)&ConfigRecordOverhead, sizeof(ConfigRecordOverhead)))
		return -1;
	if ((ConfigRecordOverhead.Idx % PROFILE_FLASH_HEADERS_CNT) != (Idx % PROFILE_FLASH_HEADERS_CNT))
		return -1;
	if (memcmp(ConfigRecordOverhead.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		return -1;
	if (memcmp(ConfigRecordOverhead.SwVersion, BSP_SW_VERSION, sizeof(BSP_SW_VERSION)))
		return -1;
	if (memcmp(ConfigRecordOverhead.SwDate, BSP_SW_DATE, sizeof(BSP_SW_DATE)))
		return -1;
	if (memcmp(ConfigRecordOverhead.SwTime, BSP_SW_TIME, sizeof(BSP_SW_TIME)))
		return -1;
	if (memcmp(ConfigRecordOverhead.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		return -1;
	if (ConfigRecordOverhead.ProfileIdx != ProfileFlash->ProfileIdx)
		return -1;
	if (ConfigRecordOverhead.ConfigsSize != ProfileFlash->ConfigsSize)
		return -1;
	Crc16 = ConfigRecordOverhead.Crc16;
	ConfigRecordOverhead.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&ConfigRecordOverhead, sizeof(ConfigRecordOverhead), 0))
		return -1;
	
	Addr += BSP_FLASH_PAGE_SIZE;
	
	if( osMutexWait( ProfileFlash->Mutex, osWaitForever ) != osOK )
		return -1;
	if (BSP_FLASH_Read(Addr, ProfileFlash->Configs, ProfileFlash->ConfigsSize))
		goto error;
	Crc16 = Calc_CRC16((char*)ProfileFlash->Configs, ProfileFlash->ConfigsSize, 0);
	if (Crc16 != ConfigRecordOverhead.ConfigsCrc16)
		goto error;
	osMutexRelease( ProfileFlash->Mutex );
	
	*RecordIdx = ConfigRecordOverhead.Idx;
	return 0;

error:
	osMutexRelease( ProfileFlash->Mutex );
	return -1;
}

int32_t PROFILE_FLASH_CalibsLoad(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t i;
	uint32_t RecordIdx, RecordIdxSave, RecordsAbs;
	
	RecordIdx = 0;
	RecordIdxSave = 0;
	RecordsAbs = 1;
	for (i = 0; i < PROFILE_FLASH_CALIB_RECORDS_CNT; i++)
	{
		if (PROFILE_FLASH_CalibsRecordLoad(ProfileFlash, i, &RecordIdx))
			continue;			
		RecordsAbs = 0;
		if (RecordIdx >= RecordIdxSave)
		{
			RecordIdxSave = RecordIdx;
		}
	}
	if (RecordsAbs)
		return -1;	
	if (PROFILE_FLASH_CalibsRecordLoad(ProfileFlash, RecordIdxSave, &RecordIdx))
		return -1;	
	ProfileFlash->CalibIdx = RecordIdxSave + 1;
	return 0;
}

int32_t PROFILE_FLASH_ConfigsLoad(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t i;
	uint32_t RecordIdx, RecordIdxSave, RecordsAbs;
	
	RecordIdx = 0;
	RecordIdxSave = 0;
	RecordsAbs = 1;
	for (i = 0; i < PROFILE_FLASH_CONFIG_RECORDS_CNT; i++)
	{
		if (PROFILE_FLASH_ConfigsRecordLoad(ProfileFlash, i, &RecordIdx))
			continue;			
		RecordsAbs = 0;
		if (RecordIdx >= RecordIdxSave)
		{
			RecordIdxSave = RecordIdx;
		}
	}
	if (RecordsAbs)
		return -1;	
	if (PROFILE_FLASH_ConfigsRecordLoad(ProfileFlash, RecordIdxSave, &RecordIdx))
		return -1;	
	ProfileFlash->ConfigIdx = RecordIdxSave + 1;
	return 0;
}

void PROFILE_FLASH_LangpackSave(PROFILE_FLASH_T *ProfileFlash, char *Block, uint32_t Index)
{
	uint32_t Addr;
	
	Addr = PROFILE_FLASH_LANG_START(ProfileFlash->ProfileIdx) + Index * BSP_FLASH_PAGE_SIZE;
	if (BSP_FLASH_WaitReady())
		return;
	BSP_FLASH_ProgramPage(Addr, Block, BSP_FLASH_PAGE_SIZE);
}
void PROFILE_FLASH_LangpackLoad(PROFILE_FLASH_T *ProfileFlash, char *Block, uint32_t Index)
{
	uint32_t Addr;
	Addr = PROFILE_FLASH_LANG_START(ProfileFlash->ProfileIdx) + Index * BSP_FLASH_PAGE_SIZE;
	if (BSP_FLASH_WaitReady())
		return;
	BSP_FLASH_Read(Addr, Block, BSP_FLASH_PAGE_SIZE);
}
void PROFILE_FLASH_LangpackClear(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t Addr;
	Addr = PROFILE_FLASH_LANG_START(ProfileFlash->ProfileIdx);
	if (BSP_FLASH_WaitReady())
		return;
	BSP_FLASH_ErasePage(Addr);
}
