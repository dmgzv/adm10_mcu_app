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
	uint32_t ValuesSize;
	uint16_t ValuesCrc16;
	uint16_t Crc16;	
}PROFILE_FLASH_CONFIG_HEADER_T;

#define PROFILE_FLASH_MESSAGE_QUEUE_SIZE	( 1 )

#define PROFILE_FLASH_PARAMS_PAGES_CNT(ProfileFlash)	\
	( ((ProfileFlash)->ValuesSize) % BSP_FLASH_PAGE_SIZE  ? \
		((ProfileFlash)->ValuesSize) / BSP_FLASH_PAGE_SIZE + 1 : \
		((ProfileFlash)->ValuesSize) / BSP_FLASH_PAGE_SIZE + 0	)

#define PROFILE_FLASH_CONFIG_PAGES_CNT 	( PROFILE_FLASH_CONFIG_SIZE / BSP_FLASH_PAGE_SIZE )

#define PROFILE_FLASH_CONFIG_RECORDS_CNT						( 4 )
#define PROFILE_FLASH_CONFIG_RECORD_PAGES_CNT  			( PROFILE_FLASH_CONFIG_PAGES_CNT / PROFILE_FLASH_CONFIG_RECORDS_CNT )
#define PROFILE_FLASH_CONFIG_RECORD_SIZE						( PROFILE_FLASH_CONFIG_RECORD_PAGES_CNT * BSP_FLASH_PAGE_SIZE )
#define PROFILE_FLASH_CONFIG_RECORD_ADDR(ProfileIdx, RecordIdx) ( PROFILE_FLASH_CONFIG_START(ProfileIdx) + (((RecordIdx) % PROFILE_FLASH_CONFIG_RECORDS_CNT) * PROFILE_FLASH_CONFIG_RECORD_SIZE) )

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

static void ProfileFlash_Thread(void const *argument)
{
	PROFILE_FLASH_T *ProfileFlash = (PROFILE_FLASH_T*)argument;
	PROFILE_FLASH_CONFIG_HEADER_T Overhead;
	osEvent Event;
	char *p;
	uint32_t Addr;
	uint32_t i, Size;
	uint16_t Crc16;
	uint32_t ErrorCntr;
	char Page[BSP_FLASH_PAGE_SIZE];
	
	while (1)
	{
		ErrorCntr = 0;
    Event = osMessageGet(ProfileFlash->MessageQId, osWaitForever);
		if (Event.status != osEventMessage) 
			continue;
		/*Save Profile to Flash*/
repeat:
		osMutexWait(ProfileFlash->ValuesMutex, osWaitForever);
		Crc16 = Calc_CRC16((char*)ProfileFlash->Values, ProfileFlash->ValuesSize, 0);
		osMutexRelease( ProfileFlash->ValuesMutex );

		p = (char*)ProfileFlash->Values;
		
		memset(&Overhead, 0, sizeof(Overhead));
		memcpy(Overhead.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
		memcpy(Overhead.SwVersion, BSP_SW_VERSION, sizeof(BSP_SW_VERSION));
		memcpy(Overhead.SwDate, BSP_SW_DATE, sizeof(BSP_SW_DATE));
		memcpy(Overhead.SwTime, BSP_SW_TIME, sizeof(BSP_SW_TIME));
		memcpy(Overhead.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
		Overhead.Idx = ProfileFlash->ConfigIdx;
		Overhead.ProfileIdx = ProfileFlash->ProfileIdx;
		Overhead.ValuesSize = ProfileFlash->ValuesSize;
		Overhead.ValuesCrc16 = Crc16;
		Overhead.Crc16 = Calc_CRC16((char*)&Overhead, sizeof(Overhead), 0);
		
		Addr = PROFILE_FLASH_CONFIG_RECORD_ADDR(ProfileFlash->ProfileIdx, ProfileFlash->ConfigIdx);
		
		/*First page with overhead*/
		if (BSP_FLASH_WaitReady())
			goto error;
		if (BSP_FLASH_ProgramPage(Addr, (char*)&Overhead, sizeof(Overhead)))
			goto error;
		
		memset(&Overhead, 0, sizeof(Overhead));
		if (BSP_FLASH_WaitReady())
			goto error;
		if (BSP_FLASH_Read(Addr, (char*)&Overhead, sizeof(Overhead)))
			goto error;
		Crc16 = Overhead.Crc16;
		Overhead.Crc16 = 0;
		if (Crc16 != Calc_CRC16((char*)&Overhead, sizeof(Overhead), 0))
			goto error;
		
		Addr += BSP_FLASH_PAGE_SIZE;
		
		Crc16 = 0;
		for (i = 0; i < PROFILE_FLASH_PARAMS_PAGES_CNT(ProfileFlash); i++)
		{
			if (i == PROFILE_FLASH_PARAMS_PAGES_CNT(ProfileFlash) - 1)
				Size = ProfileFlash->ValuesSize - (i * BSP_FLASH_PAGE_SIZE);
			else 
				Size = BSP_FLASH_PAGE_SIZE;
			if (BSP_FLASH_WaitReady())
				goto error;
			osMutexWait(ProfileFlash->ValuesMutex, osWaitForever);
			if (BSP_FLASH_ProgramPage(Addr, p, Size))
			{
				osMutexRelease( ProfileFlash->ValuesMutex );
				goto error;
			}
			osMutexRelease( ProfileFlash->ValuesMutex );
			if (BSP_FLASH_WaitReady())
				goto error;
			if (BSP_FLASH_Read(Addr, Page, Size))
				goto error;
			Crc16 = Calc_CRC16(Page, Size, Crc16);
			Addr += Size;
			p += Size;			
		}
		/*Check changes of values during writing process.*/
		if (Crc16 != Overhead.ValuesCrc16)
			goto repeat;
		
		if (Overhead.Idx == 0)
		{
			for (i = 1; i < PROFILE_FLASH_CONFIG_RECORDS_CNT; i++)
			{
				Addr = PROFILE_FLASH_CONFIG_RECORD_ADDR(ProfileFlash->ProfileIdx, ProfileFlash->ConfigIdx);
				if (BSP_FLASH_WaitReady())
					goto error;
				if (BSP_FLASH_ErasePage(Addr))
					goto error;
			}
		}
		ProfileFlash->ConfigIdx++;		
		continue;
		
error:
		ErrorCntr++;
		if (ErrorCntr < 25)
			goto repeat;
	}
}

int32_t PROFILE_FLASH_Init(PROFILE_FLASH_T *ProfileFlash, char *Name, void *Values, uint32_t ValuesSize, osMutexId ValuesMutex)
{	
	osThreadDef_t ProfileFlash_osThreadDef = { Name, ProfileFlash_Thread, osPriorityNormal, 0, 1024};
	osMessageQDef_t ProfileFlash_osMessageQDef = {PROFILE_FLASH_MESSAGE_QUEUE_SIZE, sizeof(uint32_t)};
	
	ProfileFlash->Values = Values;
	ProfileFlash->ValuesSize = ValuesSize;
	ProfileFlash->ValuesMutex = ValuesMutex;
	ProfileFlash->HeaderIdx = 0;
	ProfileFlash->ConfigIdx = 0;
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

void PROFILE_FLASH_ParamsSave(PROFILE_FLASH_T *ProfileFlash)
{
	/*Wake up Profile Save Thread*/
	if (ProfileFlash->MessageQId && ProfileFlash->EnableSave)
		osMessagePut(ProfileFlash->MessageQId, 0, 0);	
}

static int32_t PROFILE_FLASH_RecordLoad(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *RecordIdx)
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
	if (ConfigRecordOverhead.ValuesSize != ProfileFlash->ValuesSize)
		return -1;
	Crc16 = ConfigRecordOverhead.Crc16;
	ConfigRecordOverhead.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&ConfigRecordOverhead, sizeof(ConfigRecordOverhead), 0))
		return -1;
	
	Addr += BSP_FLASH_PAGE_SIZE;
	
	if( osMutexWait( ProfileFlash->ValuesMutex, osWaitForever ) != osOK )
		return -1;
	if (BSP_FLASH_Read(Addr, ProfileFlash->Values, ProfileFlash->ValuesSize))
		goto error;
	Crc16 = Calc_CRC16((char*)ProfileFlash->Values, ProfileFlash->ValuesSize, 0);
	if (Crc16 != ConfigRecordOverhead.ValuesCrc16)
		goto error;
	osMutexRelease( ProfileFlash->ValuesMutex );
	
	*RecordIdx = ConfigRecordOverhead.Idx;
	return 0;

error:
	osMutexRelease( ProfileFlash->ValuesMutex );
	return -1;
}

int32_t PROFILE_FLASH_LastRecordLoad(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t Dummy;
	return PROFILE_FLASH_RecordLoad(ProfileFlash, ProfileFlash->ConfigIdx -1, &Dummy);
}

int32_t PROFILE_FLASH_ParamsLoad(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t i;
	uint32_t RecordIdx, RecordIdxSave, RecordsAbs;
	
	RecordIdx = 0;
	RecordIdxSave = 0;
	RecordsAbs = 1;
	for (i = 0; i < PROFILE_FLASH_CONFIG_RECORDS_CNT; i++)
	{
		if (PROFILE_FLASH_RecordLoad(ProfileFlash, i, &RecordIdx))
			continue;			
		RecordsAbs = 0;
		if (RecordIdx >= RecordIdxSave)
		{
			RecordIdxSave = RecordIdx;
		}
	}
	if (RecordsAbs)
		return -1;	
	if (PROFILE_FLASH_RecordLoad(ProfileFlash, RecordIdxSave, &RecordIdx))
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
