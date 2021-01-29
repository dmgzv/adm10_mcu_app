#include "profile_flash.h"
#include "profile.h"
#include "cmsis_os.h"
#include "crc.h"
#include "log.h"
#include <string.h>

typedef struct {
	char Project[sizeof(BSP_PROJECT)];
	char HwVersion[sizeof(BSP_HW_VERSION)];
	uint32_t CalibsMapSize;
	uint16_t CalibsMapCrc16;
	uint16_t Crc16;
}PROFILE_FLASH_CALIB_MAP_HEADER_T;

typedef struct {
	uint32_t Idx;
	char Project[sizeof(BSP_PROJECT)];
	char HwVersion[sizeof(BSP_HW_VERSION)];
	uint32_t CalibsSize;
	uint16_t CalibsCrc16;
	uint16_t Crc16;	
}PROFILE_FLASH_CALIB_RECORD_HEADER_T;

typedef struct {
	char Project[sizeof(BSP_PROJECT)];
	char HwVersion[sizeof(BSP_HW_VERSION)];
	uint32_t ConfigsMapSize;
	uint16_t ConfigsMapCrc16;
	uint16_t Crc16;
}PROFILE_FLASH_CONFIG_MAP_HEADER_T;

typedef struct {
	uint32_t Idx;
	char Project[sizeof(BSP_PROJECT)];
	char HwVersion[sizeof(BSP_HW_VERSION)];
	uint32_t ConfigsSize;
	uint16_t ConfigsCrc16;
	uint16_t Crc16;	
}PROFILE_FLASH_CONFIG_RECORD_HEADER_T;

typedef struct {
	char Project[sizeof(BSP_PROJECT)];
	char HwVersion[sizeof(BSP_HW_VERSION)];
	uint32_t LangpackType;
	uint32_t LangpackBlocks;
	uint16_t Crc16;
}PROFILE_FLASH_LANGPACK_RECORD_HEADER_T;

#define PROFILE_FLASH_DATA_TYPE_VALUE		0
#define PROFILE_FLASH_DATA_TYPE_NCOLOR	1
#define PROFILE_FLASH_DATA_TYPE_DCOLOR	2
#define PROFILE_FLASH_DATA_TYPE_FCOLOR	3

static int32_t PROFILE_FLASH_CheckCalibsMap(PROFILE_FLASH_T *ProfileFlash, uint32_t *RestoreValues)
{
	PROFILE_FLASH_CALIB_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, Addr, Size, Pages, DataAddr;
	uint8_t Len, NameLen, Section, DataType, DataSize;
	uint16_t Crc16;
	
	if (BSP_FLASH_WaitReady())
		goto error;
	
	Addr = PROFILE_FLASH_CALIB_MAP_HEADER_ADDR;
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	if (memcmp(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		goto error;
	if (memcmp(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		goto error;
	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		goto error;
	
	Addr = PROFILE_FLASH_CALIB_MAP_BODY_ADDR;
	if (Header.CalibsMapSize % BSP_FLASH_PAGE_SIZE)
		Pages = (Header.CalibsMapSize / BSP_FLASH_PAGE_SIZE) + 1;
	else
		Pages = (Header.CalibsMapSize / BSP_FLASH_PAGE_SIZE);
	Crc16 = 0;
	for (i = 0; i < Pages; i++)
	{
		if (i == (Pages - 1))
			Size = Header.CalibsMapSize - (i * BSP_FLASH_PAGE_SIZE);
		else
			Size = BSP_FLASH_PAGE_SIZE;			
		if (BSP_FLASH_Read(Addr, Page, Size))
			goto error;
		Addr += Size;
		Crc16 = Calc_CRC16(Page, Size, Crc16);
	}
	if (Crc16 != Header.CalibsMapCrc16)
		goto error;
	
	Addr = PROFILE_FLASH_CALIB_MAP_BODY_ADDR;
	Size = 0;
	for (i = 0; i < ProfileFlash->Profile->SectionsCnt; i++)
	{
		for (j = 0; j < ProfileFlash->Profile->Sections[i]->ParamsCnt; j++)
		{
			if (PROFILE_IsCalibParam(ProfileFlash->Profile, ProfileFlash->Profile->Sections[i]->Params[j]))
				continue;
			NameLen = strlen(ProfileFlash->Profile->Sections[i]->Params[j]->Name) + 1;
			Section = ProfileFlash->Profile->Sections[i]->Params[j]->Section;
			DataType = PROFILE_FLASH_DATA_TYPE_VALUE;
			DataAddr = (uint32_t)(ProfileFlash->Profile->Sections[i]->Params[j]->Value) - (uint32_t)(ProfileFlash->Profile->Calib->Values);
			if (PROFILE_GetParamValueSize(ProfileFlash->Profile->Sections[i]->Params[j]->Type, &DataSize))
				goto error;
			if (BSP_FLASH_Read(Addr + Size, (char*)&Len, sizeof(Len)))
				goto error;
			if (Len != sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr)  + sizeof(DataSize))
				goto mismatch;
			Size += sizeof(Len);
			
			if (BSP_FLASH_Read(Addr + Size, (char*)&Page, Len))
				goto error;
			if (memcmp(&NameLen, Page, sizeof(NameLen)))
				goto mismatch;
			if (memcmp(ProfileFlash->Profile->Sections[i]->Params[j]->Name, &Page[sizeof(NameLen)], NameLen))
				goto mismatch;
			if (memcmp(&Section, &Page[sizeof(NameLen) + NameLen], sizeof(Section)))
				goto mismatch;
			if (memcmp(&DataType, &Page[sizeof(NameLen) + NameLen + sizeof(Section)], sizeof(DataType)))
				goto mismatch;
			if (memcmp(&DataAddr, &Page[sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType)], sizeof(DataAddr)))
				goto mismatch;
			if (memcmp(&DataSize, &Page[sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr)], sizeof(DataSize)))
				goto mismatch;
			Size += Len;
		}
	}
	
	if (Header.CalibsMapSize != Size)
		goto mismatch;
	
	*RestoreValues = 0;
	LOG_Info("PROFILE_FLASH", "",  "", "Check Calibs Map: Ok");
	return 0;
	
error:
	*RestoreValues = 0;
	LOG_Warn("PROFILE_FLASH", "",  "", "Check Calibs Map: Invalid");
	return -1;
	
mismatch:
	*RestoreValues = 1;
	LOG_Warn("PROFILE_FLASH", "",  "", "Check Calibs Map: Mismatch");
	return -1;
}

static int32_t PROFILE_FLASH_SaveCalibsMap(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CALIB_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, k, Addr, Size, PageIdx, DataAddr;
	uint8_t Len, NameLen, Section, DataType, DataSize;
	uint16_t Crc16;
	
	Addr = PROFILE_FLASH_CALIB_MAP_BODY_ADDR;
	Size = 0;
	Crc16 = 0;
	PageIdx = 0;
	
	for (i = 0; i < ProfileFlash->Profile->SectionsCnt; i++)
	{
		for (j = 0; j < ProfileFlash->Profile->Sections[i]->ParamsCnt; j++)
		{
			if (PROFILE_IsCalibParam(ProfileFlash->Profile, ProfileFlash->Profile->Sections[i]->Params[j]))
				continue;
			NameLen = strlen(ProfileFlash->Profile->Sections[i]->Params[j]->Name) + 1;
			Section = ProfileFlash->Profile->Sections[i]->Params[j]->Section;
			DataType = PROFILE_FLASH_DATA_TYPE_VALUE;
			DataAddr = (uint32_t)(ProfileFlash->Profile->Sections[i]->Params[j]->Value) - (uint32_t)(ProfileFlash->Profile->Calib->Values);
			if (PROFILE_GetParamValueSize(ProfileFlash->Profile->Sections[i]->Params[j]->Type, &DataSize))
				goto error;
			Len = sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize);			
			for (k = 0; k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize)); k++)
			{
				if (k < sizeof(Len))
					Page[PageIdx++] = ((uint8_t*)&Len)[k];
				else if ((k >= sizeof(Len)) && (k < (sizeof(Len) + sizeof(NameLen))))
					Page[PageIdx++] = ((uint8_t*)&NameLen)[k - sizeof(Len)];
				else if ((k >= (sizeof(Len) + sizeof(NameLen))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen)))
					Page[PageIdx++] = ProfileFlash->Profile->Sections[i]->Params[j]->Name[k - sizeof(Len) - sizeof(NameLen)];
				else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen)) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section))))
					Page[PageIdx++] = ((uint8_t*)&Section)[k - sizeof(Len) - sizeof(NameLen) - NameLen];
				else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType))))
					Page[PageIdx++] = ((uint8_t*)&DataType)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section)];
				else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr))))
					Page[PageIdx++] = ((uint8_t*)&DataAddr)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section) - sizeof(DataType)];
				else
					Page[PageIdx++] = ((uint8_t*)&DataSize)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section) - sizeof(DataType) - sizeof(DataAddr)];
				if (PageIdx == BSP_FLASH_PAGE_SIZE)
				{
					if (BSP_FLASH_WaitReady())
						goto error;
					if (BSP_FLASH_ProgramPage(Addr + Size, Page, PageIdx))
						goto error;
					Size += PageIdx;
					Crc16 = Calc_CRC16(Page, PageIdx, Crc16);
					PageIdx = 0;
				}
			}
		}
	}
	if (PageIdx)
	{
		if (BSP_FLASH_WaitReady())
			goto error;
		if (BSP_FLASH_ProgramPage(Addr + Size, Page, PageIdx))
			goto error;
		Size += PageIdx;
		Crc16 = Calc_CRC16(Page, PageIdx, Crc16);
		PageIdx = 0;
	}
	
	memset(&Header, 0, sizeof(Header));
	memcpy(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Header.CalibsMapCrc16 = Crc16;
	Header.CalibsMapSize = Size;
	Header.Crc16 = Calc_CRC16((char*)&Header, sizeof(Header), 0);
	
	Addr = PROFILE_FLASH_CALIB_MAP_HEADER_ADDR;
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	LOG_Info("PROFILE_FLASH", "",  "", "Save Calibs Map: %d bytes", Size);
	return 0;
	
error:
	LOG_Crit("PROFILE_FLASH", "",  "", "Save Calibs Map: Error");
	return -1;	
}

static int32_t PROFILE_FLASH_GetCalibsValuesIdx(PROFILE_FLASH_T *ProfileFlash, uint32_t *CalibsValuesIdx)
{
	PROFILE_FLASH_CALIB_RECORD_HEADER_T CalibRecordHeader;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, Addr, Pages, Size;
	uint32_t CalibsValuesIdxSave, CalibsValuesIdxFounded;
	uint16_t Crc16;
	
	CalibsValuesIdxSave = 0;
	CalibsValuesIdxFounded = 0;
	
	if (BSP_FLASH_WaitReady())
		goto error;
	
	for (i = 0; i < PROFILE_FLASH_CALIB_RECORDS_CNT; i++)
	{
		Addr = PROFILE_FLASH_CALIB_RECORD_HEADER_ADDR(i);
		if (BSP_FLASH_Read(Addr, (char*)&CalibRecordHeader, sizeof(CalibRecordHeader)))
			goto error;
		if (memcmp(CalibRecordHeader.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
			continue;
		if (memcmp(CalibRecordHeader.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
			continue;
		Crc16 = CalibRecordHeader.Crc16;
		CalibRecordHeader.Crc16 = 0;
		if (Crc16 != Calc_CRC16((char*)&CalibRecordHeader, sizeof(CalibRecordHeader), 0))
			continue;
		
		Addr = PROFILE_FLASH_CALIB_RECORD_BODY_ADDR(i);
		if (CalibRecordHeader.CalibsSize % BSP_FLASH_PAGE_SIZE)
			Pages = (CalibRecordHeader.CalibsSize / BSP_FLASH_PAGE_SIZE) + 1;
		else
			Pages = (CalibRecordHeader.CalibsSize / BSP_FLASH_PAGE_SIZE);
		Crc16 = 0;
		for (j = 0; j < Pages; j++)
		{
			if (j == (Pages - 1))
				Size = CalibRecordHeader.CalibsSize - (j * BSP_FLASH_PAGE_SIZE);
			else
				Size = BSP_FLASH_PAGE_SIZE;			
			if (BSP_FLASH_Read(Addr, Page, Size))
				goto error;
			Addr += Size;
			Crc16 = Calc_CRC16(Page, Size, Crc16);
		}
		if (Crc16 != CalibRecordHeader.CalibsCrc16)
			continue;
		
		if (CalibRecordHeader.Idx >= CalibsValuesIdxSave)
		{
			CalibsValuesIdxFounded = 1;
			CalibsValuesIdxSave = CalibRecordHeader.Idx;			
		}
	}
	
	if (CalibsValuesIdxFounded)
	{
		*CalibsValuesIdx = CalibsValuesIdxSave;
		LOG_Info("PROFILE_FLASH", "",  "", "Get Calibs Values Idx: %d", *CalibsValuesIdx);
		return 0;
	}
	
error:	
	LOG_Warn("PROFILE_FLASH", "",  "", "Get Calibs Values Idx: Error");
	return -1;
}

static void PROFILE_FLASH_RestoreCalibsValues(PROFILE_FLASH_T *ProfileFlash, uint32_t CalibsValuesIdx)
{
	PROFILE_FLASH_CALIB_MAP_HEADER_T CalibMapHeader;
	uint32_t i, j, MapAddr, __MapAddr, DataAddr, ValAddr, RestoredParamsCnt;
	uint8_t Len, NameLen, Section, DataType, DataSize, __DataSize;
	char Name[BSP_FLASH_PAGE_SIZE];
	PROFILE_VALUE_T Value;
	
	if (BSP_FLASH_WaitReady())
		goto exit;
	
	RestoredParamsCnt = 0;
	
	MapAddr = PROFILE_FLASH_CALIB_MAP_HEADER_ADDR;
	if (BSP_FLASH_Read(MapAddr, (char*)&CalibMapHeader, sizeof(CalibMapHeader)))
		goto exit;
	
	MapAddr = PROFILE_FLASH_CALIB_MAP_BODY_ADDR;
	ValAddr = PROFILE_FLASH_CALIB_RECORD_BODY_ADDR(CalibsValuesIdx % PROFILE_FLASH_CALIB_RECORDS_CNT);
	
	PROFILE_MUTEX_LOCK(ProfileFlash->Profile);
	while (MapAddr < (PROFILE_FLASH_CALIB_MAP_BODY_ADDR + CalibMapHeader.CalibsMapSize))
	{
		if (BSP_FLASH_Read(MapAddr, (char*)&Len, sizeof(Len)))
			goto exit;
		if ((Len == 0) || (Len == 0xFF))
			goto exit;
		MapAddr += sizeof(Len);
		__MapAddr = MapAddr + Len;
		
		if (BSP_FLASH_Read(MapAddr, (char*)&NameLen, sizeof(NameLen)))
			goto exit;
		if ((NameLen == 0) || (NameLen == 0xFF) || ((sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize)) != Len))
			goto exit;
		MapAddr += sizeof(NameLen);
		
		if (BSP_FLASH_Read(MapAddr, (char*)&Name, NameLen))
			goto exit;
		MapAddr += NameLen;
		
		if (BSP_FLASH_Read(MapAddr, (char*)&Section, sizeof(Section)))
			goto exit;
		MapAddr += sizeof(Section);
		
		if (BSP_FLASH_Read(MapAddr, (char*)&DataType, sizeof(DataType)))
			goto exit;
		MapAddr += sizeof(DataType);
		
		if (BSP_FLASH_Read(MapAddr, (char*)&DataAddr, sizeof(DataAddr)))
			goto exit;
		MapAddr += sizeof(DataAddr);
		
		if (BSP_FLASH_Read(MapAddr, (char*)&DataSize, sizeof(DataSize)))
			goto exit;
		if ((DataSize == 0) || (DataSize > sizeof(Value)))
			goto exit;
		MapAddr += sizeof(DataSize);
		
		if (BSP_FLASH_Read(ValAddr + DataAddr, (char*)&Value, DataSize))
			goto exit;
		
		for (i = 0; i < ProfileFlash->Profile->SectionsCnt; i++)
		{
			if (ProfileFlash->Profile->Sections[i]->Index != Section)
				continue;
			for (j = 0; j < ProfileFlash->Profile->Sections[i]->ParamsCnt; j++)
			{
				if (memcmp(ProfileFlash->Profile->Sections[i]->Params[j]->Name, Name, NameLen))
					continue;
				if (DataType == PROFILE_FLASH_DATA_TYPE_VALUE)
				{
					if (PROFILE_IsCalibParam(ProfileFlash->Profile, ProfileFlash->Profile->Sections[i]->Params[j]))
						break;
					if (PROFILE_GetParamValueSize(ProfileFlash->Profile->Sections[i]->Params[j]->Type, &__DataSize))
						break;
					if (DataSize != __DataSize)
						break;
					if (!__PROFILE_SetParamValue(ProfileFlash->Profile, ProfileFlash->Profile->Sections[i]->Params[j], &Value))
						RestoredParamsCnt++;
				}
				break;
			}
			break;
		}
		MapAddr = __MapAddr;
	}
exit:
	PROFILE_MUTEX_UNLOCK(ProfileFlash->Profile);
	LOG_Info("PROFILE_FLASH", "",  "", "Restore Calibs Values: %d params", RestoredParamsCnt);
}

static int32_t PROFILE_FLASH_LoadCalibsRecord(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *RecordIdx)
{
	PROFILE_FLASH_CALIB_RECORD_HEADER_T CalibRecordHeader;
	uint32_t Addr;
	uint16_t Crc16;
	
	if (BSP_FLASH_WaitReady())
		goto error;
	
	Addr = PROFILE_FLASH_CALIB_RECORD_HEADER_ADDR(Idx);
	if (BSP_FLASH_Read(Addr, (char*)&CalibRecordHeader, sizeof(CalibRecordHeader)))
		goto error;
	if (memcmp(CalibRecordHeader.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		goto error;
	if (memcmp(CalibRecordHeader.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		goto error;
	if (CalibRecordHeader.CalibsSize != ProfileFlash->Profile->Calib->ValuesSize)
		goto error;
	Crc16 = CalibRecordHeader.Crc16;
	CalibRecordHeader.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&CalibRecordHeader, sizeof(CalibRecordHeader), 0))
		goto error;
	
	Addr = PROFILE_FLASH_CALIB_RECORD_BODY_ADDR(Idx);	
	PROFILE_MUTEX_LOCK(ProfileFlash->Profile);
	if (BSP_FLASH_Read(Addr, ProfileFlash->Profile->Calib->Values, ProfileFlash->Profile->Calib->ValuesSize))
		goto release_and_error;
	Crc16 = Calc_CRC16((char*)ProfileFlash->Profile->Calib->Values, ProfileFlash->Profile->Calib->ValuesSize, 0);
	if (Crc16 != CalibRecordHeader.CalibsCrc16)
		goto release_and_error;
	PROFILE_MUTEX_UNLOCK(ProfileFlash->Profile);
	
	*RecordIdx = CalibRecordHeader.Idx;
	LOG_Info("PROFILE_FLASH", "",  "", "Load Calibs Values %d: Idx %d", Idx, *RecordIdx);
	return 0;

release_and_error:
	PROFILE_MUTEX_UNLOCK(ProfileFlash->Profile);
error:	
	LOG_Warn("PROFILE_FLASH", "",  "", "Load Calibs Values %d: Error", Idx);
	return -1;
}

static int32_t PROFILE_FLASH_SaveCalibsValues(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CALIB_RECORD_HEADER_T Header;
	char *p;
	uint32_t Addr;
	uint32_t i, Size, Pages;
	uint16_t Crc16;
	char Page[BSP_FLASH_PAGE_SIZE];
	
	PROFILE_MUTEX_LOCK(ProfileFlash->Profile);
	Crc16 = Calc_CRC16((char*)ProfileFlash->Profile->Calib->Values, ProfileFlash->Profile->Calib->ValuesSize, 0);
	PROFILE_MUTEX_UNLOCK(ProfileFlash->Profile);

	p = (char*)ProfileFlash->Profile->Calib->Values;
	
	memset(&Header, 0, sizeof(Header));
	memcpy(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Header.Idx = ProfileFlash->CalibIdx;
	Header.CalibsSize = ProfileFlash->Profile->Calib->ValuesSize;
	Header.CalibsCrc16 = Crc16;
	Header.Crc16 = Calc_CRC16((char*)&Header, sizeof(Header), 0);
	
	Addr = PROFILE_FLASH_CALIB_RECORD_HEADER_ADDR(ProfileFlash->CalibIdx % PROFILE_FLASH_CALIB_RECORDS_CNT);
	
	/*First page with overhead*/
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	memset(&Header, 0, sizeof(Header));
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		goto error;
	
	Addr = PROFILE_FLASH_CALIB_RECORD_BODY_ADDR(ProfileFlash->CalibIdx % PROFILE_FLASH_CALIB_RECORDS_CNT);
	if (ProfileFlash->Profile->Calib->ValuesSize % BSP_FLASH_PAGE_SIZE)
		Pages = (ProfileFlash->Profile->Calib->ValuesSize / BSP_FLASH_PAGE_SIZE) + 1;
	else
		Pages = (ProfileFlash->Profile->Calib->ValuesSize / BSP_FLASH_PAGE_SIZE);
	Crc16 = 0;
	for (i = 0; i < Pages; i++)
	{
		if (i == (Pages - 1))
			Size = ProfileFlash->Profile->Calib->ValuesSize - (i * BSP_FLASH_PAGE_SIZE);
		else 
			Size = BSP_FLASH_PAGE_SIZE;
		if (BSP_FLASH_WaitReady())
			goto error;
		if (BSP_FLASH_ProgramPage(Addr, p, Size))
		{
			goto error;
		}
		if (BSP_FLASH_WaitReady())
			goto error;
		if (BSP_FLASH_Read(Addr, Page, Size))
			goto error;
		Crc16 = Calc_CRC16(Page, Size, Crc16);
		Addr += Size;
		p += Size;			
	}
	
	/*Check changes of values during writing process.*/
	if (Crc16 != Header.CalibsCrc16)
		goto error;
	
	if (Header.Idx == 0)
	{
		for (i = 1; i < PROFILE_FLASH_CALIB_RECORDS_CNT; i++)
		{
			Addr = PROFILE_FLASH_CALIB_RECORD_HEADER_ADDR((Header.Idx + i) % PROFILE_FLASH_CALIB_RECORDS_CNT);
			if (BSP_FLASH_WaitReady())
				goto error;
			if (BSP_FLASH_ErasePage(Addr))
				goto error;
		}
	}
	
	LOG_Info("PROFILE_FLASH", "",  "", "Save Calibs Values %d: %d bytes", ProfileFlash->CalibIdx, ProfileFlash->Profile->Calib->ValuesSize);
	ProfileFlash->CalibIdx++;
	return 0;
	
error:
	LOG_Crit("PROFILE_FLASH", "",  "", "Save Calibs Values %d: Error", ProfileFlash->CalibIdx);
	return -1;
}

static int32_t PROFILE_FLASH_InitCalibsValues(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t i, CalibsValuesIdx, CalibsValuesIdxSave, CalibsValuesLoaded;
	
	CalibsValuesIdx = 0;
	CalibsValuesIdxSave = 0;
	CalibsValuesLoaded = 0;
	
	for (i = 0; i < PROFILE_FLASH_CALIB_RECORDS_CNT; i++)
	{
		if (PROFILE_FLASH_LoadCalibsRecord(ProfileFlash, i, &CalibsValuesIdx))
			continue;		
		if (CalibsValuesIdx >= CalibsValuesIdxSave)
		{
			CalibsValuesLoaded = 1;
			CalibsValuesIdxSave = CalibsValuesIdx;
		}
	}
	if (CalibsValuesLoaded)
	{
		ProfileFlash->CalibIdx = CalibsValuesIdxSave + 1;
		if (PROFILE_FLASH_LoadCalibsRecord(ProfileFlash, CalibsValuesIdxSave % PROFILE_FLASH_CALIB_RECORDS_CNT, &CalibsValuesIdxSave))
			goto error;
		LOG_Info("PROFILE_FLASH", "",  "", "Init Calibs Values: Idx %d", ProfileFlash->CalibIdx);
		return 0;
	}

error:
	ProfileFlash->CalibIdx = 0;
	memcpy(ProfileFlash->Profile->Calib->Values, ProfileFlash->Profile->Calib->DefaultValues, ProfileFlash->Profile->Calib->ValuesSize);
	LOG_Info("PROFILE_FLASH", "",  "", "Init Calibs Values: Idx %d", ProfileFlash->CalibIdx);
	return PROFILE_FLASH_SaveCalibsValues(ProfileFlash);
}

static int32_t PROFILE_FLASH_InitCalibs(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t CalibsValuesIdx, RestoreValues;
	
	if (PROFILE_FLASH_CheckCalibsMap(ProfileFlash, &RestoreValues))
	{
		if (RestoreValues)
		{
			if (PROFILE_FLASH_GetCalibsValuesIdx(ProfileFlash, &CalibsValuesIdx) == 0)
				PROFILE_FLASH_RestoreCalibsValues(ProfileFlash, CalibsValuesIdx);
		}
		PROFILE_FLASH_SaveCalibsMap(ProfileFlash);		
		PROFILE_FLASH_SaveCalibsValues(ProfileFlash);
	}
	
	return PROFILE_FLASH_InitCalibsValues(ProfileFlash);
}

static int32_t PROFILE_FLASH_CheckConfigsMap(PROFILE_FLASH_T *ProfileFlash, uint32_t *RestoreValues)
{
	PROFILE_FLASH_CONFIG_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, Addr, Size, Pages, DataAddr;
	uint8_t Len, NameLen, Section, DataType, DataSize;
	uint16_t Crc16;
	
	if (BSP_FLASH_WaitReady())
		goto error;
	
	Addr = PROFILE_FLASH_CONFIG_MAP_HEADER_ADDR;
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	if (memcmp(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		goto error;
	if (memcmp(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		goto error;
	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		goto error;
	
	Addr = PROFILE_FLASH_CONFIG_MAP_BODY_ADDR;
	if (Header.ConfigsMapSize % BSP_FLASH_PAGE_SIZE)
		Pages = (Header.ConfigsMapSize / BSP_FLASH_PAGE_SIZE) + 1;
	else
		Pages = (Header.ConfigsMapSize / BSP_FLASH_PAGE_SIZE);
	Crc16 = 0;
	for (i = 0; i < Pages; i++)
	{
		if (i == (Pages - 1))
			Size = Header.ConfigsMapSize - (i * BSP_FLASH_PAGE_SIZE);
		else
			Size = BSP_FLASH_PAGE_SIZE;			
		if (BSP_FLASH_Read(Addr, Page, Size))
			goto error;
		Addr += Size;
		Crc16 = Calc_CRC16(Page, Size, Crc16);
	}
	if (Crc16 != Header.ConfigsMapCrc16)
		goto error;
	
	Addr = PROFILE_FLASH_CONFIG_MAP_BODY_ADDR;
	Size = 0;
	for (i = 0; i < ProfileFlash->Profile->SectionsCnt; i++)
	{
		for (j = 0; j < ProfileFlash->Profile->Sections[i]->ParamsCnt; j++)
		{
			if (ProfileFlash->Profile->Sections[i]->Params[j]->Colors)
			{
				if (ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Normal)
				{
					NameLen = strlen(ProfileFlash->Profile->Sections[i]->Params[j]->Name) + 1;
					Section = ProfileFlash->Profile->Sections[i]->Params[j]->Section;
					DataType = PROFILE_FLASH_DATA_TYPE_NCOLOR;
					DataAddr = (uint32_t)(ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Normal) - (uint32_t)(ProfileFlash->Profile->Config->Values);
					DataSize = sizeof(uint8_t);
					if (BSP_FLASH_Read(Addr + Size, (char*)&Len, sizeof(Len)))
						goto error;
					if (Len != sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr)  + sizeof(DataSize))
						goto mismatch;
					Size += sizeof(Len);
					
					if (BSP_FLASH_Read(Addr + Size, (char*)&Page, Len))
						goto error;
					if (memcmp(&NameLen, Page, sizeof(NameLen)))
						goto mismatch;
					if (memcmp(ProfileFlash->Profile->Sections[i]->Params[j]->Name, &Page[sizeof(NameLen)], NameLen))
						goto mismatch;
					if (memcmp(&Section, &Page[sizeof(NameLen) + NameLen], sizeof(Section)))
						goto mismatch;
					if (memcmp(&DataType, &Page[sizeof(NameLen) + NameLen + sizeof(Section)], sizeof(DataType)))
						goto mismatch;
					if (memcmp(&DataAddr, &Page[sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType)], sizeof(DataAddr)))
						goto mismatch;
					if (memcmp(&DataSize, &Page[sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr)], sizeof(DataSize)))
						goto mismatch;
					Size += Len;
				}
				if (ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Degrade)
				{
					NameLen = strlen(ProfileFlash->Profile->Sections[i]->Params[j]->Name) + 1;
					Section = ProfileFlash->Profile->Sections[i]->Params[j]->Section;
					DataType = PROFILE_FLASH_DATA_TYPE_DCOLOR;
					DataAddr = (uint32_t)(ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Degrade) - (uint32_t)(ProfileFlash->Profile->Config->Values);
					DataSize = sizeof(uint8_t);
					if (BSP_FLASH_Read(Addr + Size, (char*)&Len, sizeof(Len)))
						goto error;
					if (Len != sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr)  + sizeof(DataSize))
						goto mismatch;
					Size += sizeof(Len);
					
					if (BSP_FLASH_Read(Addr + Size, (char*)&Page, Len))
						goto error;
					if (memcmp(&NameLen, Page, sizeof(NameLen)))
						goto mismatch;
					if (memcmp(ProfileFlash->Profile->Sections[i]->Params[j]->Name, &Page[sizeof(NameLen)], NameLen))
						goto mismatch;
					if (memcmp(&Section, &Page[sizeof(NameLen) + NameLen], sizeof(Section)))
						goto mismatch;
					if (memcmp(&DataType, &Page[sizeof(NameLen) + NameLen + sizeof(Section)], sizeof(DataType)))
						goto mismatch;
					if (memcmp(&DataAddr, &Page[sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType)], sizeof(DataAddr)))
						goto mismatch;
					if (memcmp(&DataSize, &Page[sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr)], sizeof(DataSize)))
						goto mismatch;
					Size += Len;
				}
				if (ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Failure)
				{
					NameLen = strlen(ProfileFlash->Profile->Sections[i]->Params[j]->Name) + 1;
					Section = ProfileFlash->Profile->Sections[i]->Params[j]->Section;
					DataType = PROFILE_FLASH_DATA_TYPE_FCOLOR;
					DataAddr = (uint32_t)(ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Failure) - (uint32_t)(ProfileFlash->Profile->Config->Values);
					DataSize = sizeof(uint8_t);
					if (BSP_FLASH_Read(Addr + Size, (char*)&Len, sizeof(Len)))
						goto error;
					if (Len != sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr)  + sizeof(DataSize))
						goto mismatch;
					Size += sizeof(Len);
					
					if (BSP_FLASH_Read(Addr + Size, (char*)&Page, Len))
						goto error;
					if (memcmp(&NameLen, Page, sizeof(NameLen)))
						goto mismatch;
					if (memcmp(ProfileFlash->Profile->Sections[i]->Params[j]->Name, &Page[sizeof(NameLen)], NameLen))
						goto mismatch;
					if (memcmp(&Section, &Page[sizeof(NameLen) + NameLen], sizeof(Section)))
						goto mismatch;
					if (memcmp(&DataType, &Page[sizeof(NameLen) + NameLen + sizeof(Section)], sizeof(DataType)))
						goto mismatch;
					if (memcmp(&DataAddr, &Page[sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType)], sizeof(DataAddr)))
						goto mismatch;
					if (memcmp(&DataSize, &Page[sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr)], sizeof(DataSize)))
						goto mismatch;
					Size += Len;
				}
			}			
			if (PROFILE_IsConfigParam(ProfileFlash->Profile, ProfileFlash->Profile->Sections[i]->Params[j]))
				continue;			
			NameLen = strlen(ProfileFlash->Profile->Sections[i]->Params[j]->Name) + 1;
			Section = ProfileFlash->Profile->Sections[i]->Params[j]->Section;
			DataType = PROFILE_FLASH_DATA_TYPE_VALUE;
			DataAddr = (uint32_t)(ProfileFlash->Profile->Sections[i]->Params[j]->Value) - (uint32_t)(ProfileFlash->Profile->Config->Values);
			if (PROFILE_GetParamValueSize(ProfileFlash->Profile->Sections[i]->Params[j]->Type, &DataSize))
				goto error;
			if (BSP_FLASH_Read(Addr + Size, (char*)&Len, sizeof(Len)))
				goto error;
			if (Len != sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr)  + sizeof(DataSize))
				goto mismatch;
			Size += sizeof(Len);
			
			if (BSP_FLASH_Read(Addr + Size, (char*)&Page, Len))
				goto error;
			if (memcmp(&NameLen, Page, sizeof(NameLen)))
				goto mismatch;
			if (memcmp(ProfileFlash->Profile->Sections[i]->Params[j]->Name, &Page[sizeof(NameLen)], NameLen))
				goto mismatch;
			if (memcmp(&Section, &Page[sizeof(NameLen) + NameLen], sizeof(Section)))
				goto mismatch;
			if (memcmp(&DataType, &Page[sizeof(NameLen) + NameLen + sizeof(Section)], sizeof(DataType)))
				goto mismatch;
			if (memcmp(&DataAddr, &Page[sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType)], sizeof(DataAddr)))
				goto mismatch;
			if (memcmp(&DataSize, &Page[sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr)], sizeof(DataSize)))
				goto mismatch;
			Size += Len;
		}
	}
	
	if (Header.ConfigsMapSize != Size)
		goto mismatch;
	
	*RestoreValues = 0;
	LOG_Info("PROFILE_FLASH", "",  "", "Check Configs Map: Ok");
	return 0;
	
error:
	*RestoreValues = 0;
	LOG_Warn("PROFILE_FLASH", "",  "", "Check Configs Map: Invalid");
	return -1;
	
mismatch:
	*RestoreValues = 1;
	LOG_Warn("PROFILE_FLASH", "",  "", "Check Configs Map: Mismatch");
	return -1;
}

static int32_t PROFILE_FLASH_SaveConfigsMap(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CONFIG_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, k, Addr, Size, PageIdx, DataAddr;
	uint8_t Len, NameLen, Section, DataType, DataSize;
	uint16_t Crc16;
	uint32_t ParamsCnt;
	
	Addr = PROFILE_FLASH_CONFIG_MAP_BODY_ADDR;
	Size = 0;
	Crc16 = 0;
	PageIdx = 0;
	ParamsCnt = 0;
	for (i = 0; i < ProfileFlash->Profile->SectionsCnt; i++)
	{
		for (j = 0; j < ProfileFlash->Profile->Sections[i]->ParamsCnt; j++)
		{
			if (ProfileFlash->Profile->Sections[i]->Params[j]->Colors)
			{
				if (ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Normal)
				{
					NameLen = strlen(ProfileFlash->Profile->Sections[i]->Params[j]->Name) + 1;
					Section = ProfileFlash->Profile->Sections[i]->Params[j]->Section;
					DataType = PROFILE_FLASH_DATA_TYPE_NCOLOR;
					DataAddr = (uint32_t)(ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Normal) - (uint32_t)(ProfileFlash->Profile->Config->Values);
					DataSize = sizeof(uint8_t);
					Len = sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize);			
					for (k = 0; k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize)); k++)
					{
						if (k < sizeof(Len))
							Page[PageIdx++] = ((uint8_t*)&Len)[k];
						else if ((k >= sizeof(Len)) && (k < (sizeof(Len) + sizeof(NameLen))))
							Page[PageIdx++] = ((uint8_t*)&NameLen)[k - sizeof(Len)];
						else if ((k >= (sizeof(Len) + sizeof(NameLen))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen)))
							Page[PageIdx++] = ProfileFlash->Profile->Sections[i]->Params[j]->Name[k - sizeof(Len) - sizeof(NameLen)];
						else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen)) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section))))
							Page[PageIdx++] = ((uint8_t*)&Section)[k - sizeof(Len) - sizeof(NameLen) - NameLen];
						else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType))))
							Page[PageIdx++] = ((uint8_t*)&DataType)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section)];
						else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr))))
							Page[PageIdx++] = ((uint8_t*)&DataAddr)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section) - sizeof(DataType)];
						else
							Page[PageIdx++] = ((uint8_t*)&DataSize)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section) - sizeof(DataType) - sizeof(DataAddr)];
						if (PageIdx == BSP_FLASH_PAGE_SIZE)
						{
							if (BSP_FLASH_WaitReady())
								goto error;
							if (BSP_FLASH_ProgramPage(Addr + Size, Page, PageIdx))
								goto error;
							Size += PageIdx;
							Crc16 = Calc_CRC16(Page, PageIdx, Crc16);
							PageIdx = 0;
						}
					}
					ParamsCnt++;
				}
				if (ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Degrade)
				{
					NameLen = strlen(ProfileFlash->Profile->Sections[i]->Params[j]->Name) + 1;
					Section = ProfileFlash->Profile->Sections[i]->Params[j]->Section;
					DataType = PROFILE_FLASH_DATA_TYPE_DCOLOR;
					DataAddr = (uint32_t)(ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Degrade) - (uint32_t)(ProfileFlash->Profile->Config->Values);
					DataSize = sizeof(uint8_t);
					Len = sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize);			
					for (k = 0; k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize)); k++)
					{
						if (k < sizeof(Len))
							Page[PageIdx++] = ((uint8_t*)&Len)[k];
						else if ((k >= sizeof(Len)) && (k < (sizeof(Len) + sizeof(NameLen))))
							Page[PageIdx++] = ((uint8_t*)&NameLen)[k - sizeof(Len)];
						else if ((k >= (sizeof(Len) + sizeof(NameLen))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen)))
							Page[PageIdx++] = ProfileFlash->Profile->Sections[i]->Params[j]->Name[k - sizeof(Len) - sizeof(NameLen)];
						else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen)) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section))))
							Page[PageIdx++] = ((uint8_t*)&Section)[k - sizeof(Len) - sizeof(NameLen) - NameLen];
						else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType))))
							Page[PageIdx++] = ((uint8_t*)&DataType)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section)];
						else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr))))
							Page[PageIdx++] = ((uint8_t*)&DataAddr)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section) - sizeof(DataType)];
						else
							Page[PageIdx++] = ((uint8_t*)&DataSize)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section) - sizeof(DataType) - sizeof(DataAddr)];
						if (PageIdx == BSP_FLASH_PAGE_SIZE)
						{
							if (BSP_FLASH_WaitReady())
								goto error;
							if (BSP_FLASH_ProgramPage(Addr + Size, Page, PageIdx))
								goto error;
							Size += PageIdx;
							Crc16 = Calc_CRC16(Page, PageIdx, Crc16);
							PageIdx = 0;
						}
					}
					ParamsCnt++;
				}
				if (ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Failure)
				{
					NameLen = strlen(ProfileFlash->Profile->Sections[i]->Params[j]->Name) + 1;
					Section = ProfileFlash->Profile->Sections[i]->Params[j]->Section;
					DataType = PROFILE_FLASH_DATA_TYPE_FCOLOR;
					DataAddr = (uint32_t)(ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Failure) - (uint32_t)(ProfileFlash->Profile->Config->Values);
					DataSize = sizeof(uint8_t);
					Len = sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize);			
					for (k = 0; k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize)); k++)
					{
						if (k < sizeof(Len))
							Page[PageIdx++] = ((uint8_t*)&Len)[k];
						else if ((k >= sizeof(Len)) && (k < (sizeof(Len) + sizeof(NameLen))))
							Page[PageIdx++] = ((uint8_t*)&NameLen)[k - sizeof(Len)];
						else if ((k >= (sizeof(Len) + sizeof(NameLen))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen)))
							Page[PageIdx++] = ProfileFlash->Profile->Sections[i]->Params[j]->Name[k - sizeof(Len) - sizeof(NameLen)];
						else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen)) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section))))
							Page[PageIdx++] = ((uint8_t*)&Section)[k - sizeof(Len) - sizeof(NameLen) - NameLen];
						else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType))))
							Page[PageIdx++] = ((uint8_t*)&DataType)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section)];
						else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr))))
							Page[PageIdx++] = ((uint8_t*)&DataAddr)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section) - sizeof(DataType)];
						else
							Page[PageIdx++] = ((uint8_t*)&DataSize)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section) - sizeof(DataType) - sizeof(DataAddr)];
						if (PageIdx == BSP_FLASH_PAGE_SIZE)
						{
							if (BSP_FLASH_WaitReady())
								goto error;
							if (BSP_FLASH_ProgramPage(Addr + Size, Page, PageIdx))
								goto error;
							Size += PageIdx;
							Crc16 = Calc_CRC16(Page, PageIdx, Crc16);
							PageIdx = 0;
						}
					}
					ParamsCnt++;
				}
			}
			if (PROFILE_IsConfigParam(ProfileFlash->Profile, ProfileFlash->Profile->Sections[i]->Params[j]))
				continue;
			NameLen = strlen(ProfileFlash->Profile->Sections[i]->Params[j]->Name) + 1;
			Section = ProfileFlash->Profile->Sections[i]->Params[j]->Section;
			DataType = PROFILE_FLASH_DATA_TYPE_VALUE;
			DataAddr = (uint32_t)(ProfileFlash->Profile->Sections[i]->Params[j]->Value) - (uint32_t)(ProfileFlash->Profile->Config->Values);
			if (PROFILE_GetParamValueSize(ProfileFlash->Profile->Sections[i]->Params[j]->Type, &DataSize))
				goto error;
			Len = sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize);			
			for (k = 0; k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize)); k++)
			{
				if (k < sizeof(Len))
					Page[PageIdx++] = ((uint8_t*)&Len)[k];
				else if ((k >= sizeof(Len)) && (k < (sizeof(Len) + sizeof(NameLen))))
					Page[PageIdx++] = ((uint8_t*)&NameLen)[k - sizeof(Len)];
				else if ((k >= (sizeof(Len) + sizeof(NameLen))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen)))
					Page[PageIdx++] = ProfileFlash->Profile->Sections[i]->Params[j]->Name[k - sizeof(Len) - sizeof(NameLen)];
				else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen)) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section))))
					Page[PageIdx++] = ((uint8_t*)&Section)[k - sizeof(Len) - sizeof(NameLen) - NameLen];
				else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType))))
					Page[PageIdx++] = ((uint8_t*)&DataType)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section)];
				else if ((k >= (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType))) && (k < (sizeof(Len) + sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr))))
					Page[PageIdx++] = ((uint8_t*)&DataAddr)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section) - sizeof(DataType)];
				else
					Page[PageIdx++] = ((uint8_t*)&DataSize)[k - sizeof(Len) - sizeof(NameLen) - NameLen - sizeof(Section) - sizeof(DataType) - sizeof(DataAddr)];
				if (PageIdx == BSP_FLASH_PAGE_SIZE)
				{
					if (BSP_FLASH_WaitReady())
						goto error;
					if (BSP_FLASH_ProgramPage(Addr + Size, Page, PageIdx))
						goto error;
					Size += PageIdx;
					Crc16 = Calc_CRC16(Page, PageIdx, Crc16);
					PageIdx = 0;
				}
			}
			ParamsCnt++;
		}
	}
	if (PageIdx)
	{
		if (BSP_FLASH_WaitReady())
			goto error;
		if (BSP_FLASH_ProgramPage(Addr + Size, Page, PageIdx))
			goto error;
		Size += PageIdx;
		Crc16 = Calc_CRC16(Page, PageIdx, Crc16);
		PageIdx = 0;
	}
	
	memset(&Header, 0, sizeof(Header));
	memcpy(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Header.ConfigsMapCrc16 = Crc16;
	Header.ConfigsMapSize = Size;
	Header.Crc16 = Calc_CRC16((char*)&Header, sizeof(Header), 0);
	
	Addr = PROFILE_FLASH_CONFIG_MAP_HEADER_ADDR;
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	LOG_Info("PROFILE_FLASH", "",  "", "Save Configs Map: %d bytes", Size);
	return 0;
	
error:
	LOG_Crit("PROFILE_FLASH", "",  "", "Save Configs Map: Error");
	return -1;	
}

static int32_t PROFILE_FLASH_GetConfigsValuesIdx(PROFILE_FLASH_T *ProfileFlash, uint32_t *ConfigsValuesIdx)
{
	PROFILE_FLASH_CONFIG_RECORD_HEADER_T ConfigRecordHeader;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, Addr, Pages, Size;
	uint32_t ConfigsValuesIdxSave, ConfigsValuesIdxFounded;
	uint16_t Crc16;
	
	if (BSP_FLASH_WaitReady())
		goto error;
	
	ConfigsValuesIdxSave = 0;
	ConfigsValuesIdxFounded = 0;
	
	for (i = 0; i < PROFILE_FLASH_CONFIG_RECORDS_CNT; i++)
	{
		Addr = PROFILE_FLASH_CONFIG_RECORD_HEADER_ADDR(i);
		if (BSP_FLASH_Read(Addr, (char*)&ConfigRecordHeader, sizeof(ConfigRecordHeader)))
			goto error;
		if (memcmp(ConfigRecordHeader.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
			continue;
		if (memcmp(ConfigRecordHeader.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
			continue;
		Crc16 = ConfigRecordHeader.Crc16;
		ConfigRecordHeader.Crc16 = 0;
		if (Crc16 != Calc_CRC16((char*)&ConfigRecordHeader, sizeof(ConfigRecordHeader), 0))
			continue;
		
		Addr = PROFILE_FLASH_CONFIG_RECORD_BODY_ADDR(i);
		if (ConfigRecordHeader.ConfigsSize % BSP_FLASH_PAGE_SIZE)
			Pages = (ConfigRecordHeader.ConfigsSize / BSP_FLASH_PAGE_SIZE) + 1;
		else
			Pages = (ConfigRecordHeader.ConfigsSize / BSP_FLASH_PAGE_SIZE);
		Crc16 = 0;
		for (j = 0; j < Pages; j++)
		{
			if (j == (Pages - 1))
				Size = ConfigRecordHeader.ConfigsSize - (j * BSP_FLASH_PAGE_SIZE);
			else
				Size = BSP_FLASH_PAGE_SIZE;			
			if (BSP_FLASH_Read(Addr, Page, Size))
				goto error;
			Addr += Size;
			Crc16 = Calc_CRC16(Page, Size, Crc16);
		}
		if (Crc16 != ConfigRecordHeader.ConfigsCrc16)
			continue;
		
		if (ConfigRecordHeader.Idx >= ConfigsValuesIdxSave)
		{
			ConfigsValuesIdxFounded = 1;
			ConfigsValuesIdxSave = ConfigRecordHeader.Idx;			
		}
	}
	
	if (ConfigsValuesIdxFounded)
	{
		*ConfigsValuesIdx = ConfigsValuesIdxSave;
		LOG_Info("PROFILE_FLASH", "",  "", "Get Configs Values Idx: %d", *ConfigsValuesIdx);
		return 0;
	}
	
error:	
	LOG_Warn("PROFILE_FLASH", "",  "", "Get Configs Values Idx: Error");
	return -1;
}

static void PROFILE_FLASH_RestoreConfigsValues(PROFILE_FLASH_T *ProfileFlash, uint32_t ConfigsValuesIdx)
{
	PROFILE_FLASH_CONFIG_MAP_HEADER_T ConfigMapHeader;
	uint32_t i, j, MapAddr, __MapAddr, DataAddr, ValAddr, RestoredParamsCnt;
	uint8_t Len, NameLen, Section, DataType, DataSize, __DataSize;
	char Name[BSP_FLASH_PAGE_SIZE];
	PROFILE_VALUE_T Value;
	
	if (BSP_FLASH_WaitReady())
		goto exit;
	
	RestoredParamsCnt = 0;
	
	MapAddr = PROFILE_FLASH_CONFIG_MAP_HEADER_ADDR;
	if (BSP_FLASH_Read(MapAddr, (char*)&ConfigMapHeader, sizeof(ConfigMapHeader)))
		goto exit;
	
	MapAddr = PROFILE_FLASH_CONFIG_MAP_BODY_ADDR;
	ValAddr = PROFILE_FLASH_CONFIG_RECORD_BODY_ADDR(ConfigsValuesIdx % PROFILE_FLASH_CONFIG_RECORDS_CNT);
	
	PROFILE_MUTEX_LOCK(ProfileFlash->Profile);
	while (MapAddr < (PROFILE_FLASH_CONFIG_MAP_BODY_ADDR + ConfigMapHeader.ConfigsMapSize))
	{
		if (BSP_FLASH_Read(MapAddr, (char*)&Len, sizeof(Len)))
			goto exit;
		if ((Len == 0) || (Len == 0xFF))
			goto exit;
		MapAddr += sizeof(Len);
		__MapAddr = MapAddr + Len;
		
		if (BSP_FLASH_Read(MapAddr, (char*)&NameLen, sizeof(NameLen)))
			goto exit;
		if ((NameLen == 0) || (NameLen == 0xFF) || ((sizeof(NameLen) + NameLen + sizeof(Section) + sizeof(DataType) + sizeof(DataAddr) + sizeof(DataSize)) != Len))
			goto exit;
		MapAddr += sizeof(NameLen);
		
		if (BSP_FLASH_Read(MapAddr, (char*)&Name, NameLen))
			goto exit;
		MapAddr += NameLen;
		
		if (BSP_FLASH_Read(MapAddr, (char*)&Section, sizeof(Section)))
			goto exit;
		MapAddr += sizeof(Section);
		
		if (BSP_FLASH_Read(MapAddr, (char*)&DataType, sizeof(DataType)))
			goto exit;
		MapAddr += sizeof(DataType);
		
		if (BSP_FLASH_Read(MapAddr, (char*)&DataAddr, sizeof(DataAddr)))
			goto exit;
		MapAddr += sizeof(DataAddr);
		
		if (BSP_FLASH_Read(MapAddr, (char*)&DataSize, sizeof(DataSize)))
			goto exit;
		if ((DataSize == 0) || (DataSize > sizeof(Value)))
			goto exit;
		MapAddr += sizeof(DataSize);
		
		if (BSP_FLASH_Read(ValAddr + DataAddr, (char*)&Value, DataSize))
			goto exit;

		for (i = 0; i < ProfileFlash->Profile->SectionsCnt; i++)
		{
			if (ProfileFlash->Profile->Sections[i]->Index != Section)
				continue;
			for (j = 0; j < ProfileFlash->Profile->Sections[i]->ParamsCnt; j++)
			{
				if (memcmp(ProfileFlash->Profile->Sections[i]->Params[j]->Name, Name, NameLen))
					continue;
				if (DataType == PROFILE_FLASH_DATA_TYPE_VALUE)
				{
					if (PROFILE_IsConfigParam(ProfileFlash->Profile, ProfileFlash->Profile->Sections[i]->Params[j]))
						break;
					if (PROFILE_GetParamValueSize(ProfileFlash->Profile->Sections[i]->Params[j]->Type, &__DataSize))
						break;
					if (DataSize != __DataSize)
						break;
					if (!__PROFILE_SetParamValue(ProfileFlash->Profile, ProfileFlash->Profile->Sections[i]->Params[j], &Value))
						RestoredParamsCnt++;
				}
				else if (DataType == PROFILE_FLASH_DATA_TYPE_NCOLOR)
				{
					if (!ProfileFlash->Profile->Sections[i]->Params[j]->Colors)
						break;
					if (DataSize != sizeof(uint8_t))
						break;
					if (!__PROFILE_SetParamColors(ProfileFlash->Profile, ProfileFlash->Profile->Sections[i]->Params[j],
							Value.s64[0],
							*ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Degrade,
							*ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Failure))
						RestoredParamsCnt++;
				}
				else if (DataType == PROFILE_FLASH_DATA_TYPE_DCOLOR)
				{
					if (!ProfileFlash->Profile->Sections[i]->Params[j]->Colors)
						break;
					if (DataSize != sizeof(uint8_t))
						break;
					if (!__PROFILE_SetParamColors(ProfileFlash->Profile, ProfileFlash->Profile->Sections[i]->Params[j],
							*ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Normal,
							Value.s64[0],
							*ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Failure))
						RestoredParamsCnt++;
				}
				else if (DataType == PROFILE_FLASH_DATA_TYPE_FCOLOR)
				{
					if (!ProfileFlash->Profile->Sections[i]->Params[j]->Colors)
						break;
					if (DataSize != sizeof(uint8_t))
						break;
					if (!__PROFILE_SetParamColors(ProfileFlash->Profile, ProfileFlash->Profile->Sections[i]->Params[j],
							*ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Normal,
							*ProfileFlash->Profile->Sections[i]->Params[j]->Colors->Degrade,
							Value.s64[0]))
						RestoredParamsCnt++;
				}
				break;
			}
			break;
		}
		MapAddr = __MapAddr;
	}
exit:
	PROFILE_MUTEX_UNLOCK(ProfileFlash->Profile);
	LOG_Info("PROFILE_FLASH", "",  "", "Restore Configs Values: %d params", RestoredParamsCnt);
}

static int32_t PROFILE_FLASH_LoadConfigsRecord(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *RecordIdx)
{
	PROFILE_FLASH_CONFIG_RECORD_HEADER_T ConfigRecordHeader;
	uint32_t Addr;
	uint16_t Crc16;
	
	if (BSP_FLASH_WaitReady())
		goto error;
	
	Addr = PROFILE_FLASH_CONFIG_RECORD_HEADER_ADDR(Idx);
	if (BSP_FLASH_Read(Addr, (char*)&ConfigRecordHeader, sizeof(ConfigRecordHeader)))
		goto error;
	if (memcmp(ConfigRecordHeader.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		goto error;
	if (memcmp(ConfigRecordHeader.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		goto error;
	if (ConfigRecordHeader.ConfigsSize != ProfileFlash->Profile->Config->ValuesSize)
		goto error;
	Crc16 = ConfigRecordHeader.Crc16;
	ConfigRecordHeader.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&ConfigRecordHeader, sizeof(ConfigRecordHeader), 0))
		goto error;
	
	Addr = PROFILE_FLASH_CONFIG_RECORD_BODY_ADDR(Idx);	
	PROFILE_MUTEX_LOCK(ProfileFlash->Profile);
	if (BSP_FLASH_Read(Addr, ProfileFlash->Profile->Config->Values, ProfileFlash->Profile->Config->ValuesSize))
		goto release_and_error;
	Crc16 = Calc_CRC16((char*)ProfileFlash->Profile->Config->Values, ProfileFlash->Profile->Config->ValuesSize, 0);
	if (Crc16 != ConfigRecordHeader.ConfigsCrc16)
		goto release_and_error;
	PROFILE_MUTEX_UNLOCK(ProfileFlash->Profile);
	
	*RecordIdx = ConfigRecordHeader.Idx;
	LOG_Info("PROFILE_FLASH", "",  "", "Load Configs Values %d: Idx %d", Idx, *RecordIdx);
	return 0;

release_and_error:
	PROFILE_MUTEX_UNLOCK(ProfileFlash->Profile);
error:	
	LOG_Warn("PROFILE_FLASH", "",  "", "Load Configs Values %d: Error", Idx);
	return -1;
}

static int32_t PROFILE_FLASH_SaveConfigsValues(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CONFIG_RECORD_HEADER_T Header;
	char *p;
	uint32_t Addr;
	uint32_t i, Size, Pages;
	uint16_t Crc16;
	char Page[BSP_FLASH_PAGE_SIZE];
	
	PROFILE_MUTEX_LOCK(ProfileFlash->Profile);
	Crc16 = Calc_CRC16((char*)ProfileFlash->Profile->Config->Values, ProfileFlash->Profile->Config->ValuesSize, 0);
	PROFILE_MUTEX_UNLOCK(ProfileFlash->Profile);

	p = (char*)ProfileFlash->Profile->Config->Values;
	
	memset(&Header, 0, sizeof(Header));
	memcpy(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Header.Idx = ProfileFlash->ConfigIdx;
	Header.ConfigsSize = ProfileFlash->Profile->Config->ValuesSize;
	Header.ConfigsCrc16 = Crc16;
	Header.Crc16 = Calc_CRC16((char*)&Header, sizeof(Header), 0);
	
	Addr = PROFILE_FLASH_CONFIG_RECORD_HEADER_ADDR(ProfileFlash->ConfigIdx % PROFILE_FLASH_CONFIG_RECORDS_CNT);
	
	/*First page with overhead*/
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	memset(&Header, 0, sizeof(Header));
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		goto error;
	
	Addr = PROFILE_FLASH_CONFIG_RECORD_BODY_ADDR(ProfileFlash->ConfigIdx % PROFILE_FLASH_CONFIG_RECORDS_CNT);
	if (ProfileFlash->Profile->Config->ValuesSize % BSP_FLASH_PAGE_SIZE)
		Pages = (ProfileFlash->Profile->Config->ValuesSize / BSP_FLASH_PAGE_SIZE) + 1;
	else
		Pages = (ProfileFlash->Profile->Config->ValuesSize / BSP_FLASH_PAGE_SIZE);
	Crc16 = 0;
	for (i = 0; i < Pages; i++)
	{
		if (i == (Pages - 1))
			Size = ProfileFlash->Profile->Config->ValuesSize - (i * BSP_FLASH_PAGE_SIZE);
		else 
			Size = BSP_FLASH_PAGE_SIZE;
		if (BSP_FLASH_WaitReady())
			goto error;
		if (BSP_FLASH_ProgramPage(Addr, p, Size))
			goto error;
		if (BSP_FLASH_WaitReady())
			goto error;
		if (BSP_FLASH_Read(Addr, Page, Size))
			goto error;
		Crc16 = Calc_CRC16(Page, Size, Crc16);
		Addr += Size;
		p += Size;			
	}
	
	/*Check changes of values during writing process.*/
	if (Crc16 != Header.ConfigsCrc16)
		goto error;
	
	if (Header.Idx == 0)
	{
		for (i = 1; i < PROFILE_FLASH_CONFIG_RECORDS_CNT; i++)
		{
			Addr = PROFILE_FLASH_CONFIG_RECORD_HEADER_ADDR((Header.Idx + i) % PROFILE_FLASH_CONFIG_RECORDS_CNT);
			if (BSP_FLASH_WaitReady())
				goto error;
			if (BSP_FLASH_ErasePage(Addr))
				goto error;
		}
	}
	
	LOG_Info("PROFILE_FLASH", "",  "", "Save Configs Values %d: %d bytes", ProfileFlash->ConfigIdx, ProfileFlash->Profile->Config->ValuesSize);
	ProfileFlash->ConfigIdx++;
	return 0;
	
error:
	LOG_Crit("PROFILE_FLASH", "",  "", "Save Configs Values %d: Error", ProfileFlash->ConfigIdx);
	return -1;	
}

static int32_t PROFILE_FLASH_InitConfigsValues(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t i, ConfigsValuesIdx, ConfigsValuesIdxSave, ConfigsValuesLoaded;
	
	ConfigsValuesIdx = 0;
	ConfigsValuesIdxSave = 0;
	ConfigsValuesLoaded = 0;
	
	for (i = 0; i < PROFILE_FLASH_CONFIG_RECORDS_CNT; i++)
	{
		if (PROFILE_FLASH_LoadConfigsRecord(ProfileFlash, i, &ConfigsValuesIdx))
			continue;		
		if (ConfigsValuesIdx >= ConfigsValuesIdxSave)
		{
			ConfigsValuesLoaded = 1;
			ConfigsValuesIdxSave = ConfigsValuesIdx;
		}
	}
	if (ConfigsValuesLoaded)
	{
		ProfileFlash->ConfigIdx = ConfigsValuesIdxSave + 1;
		if (PROFILE_FLASH_LoadConfigsRecord(ProfileFlash, ConfigsValuesIdxSave % PROFILE_FLASH_CONFIG_RECORDS_CNT, &ConfigsValuesIdxSave))
			goto error;
		LOG_Info("PROFILE_FLASH", "",  "", "Init Configs Values: Idx %d", ProfileFlash->ConfigIdx);
		return 0;
	}

error:
	ProfileFlash->ConfigIdx = 0;
	memcpy(ProfileFlash->Profile->Config->Values, ProfileFlash->Profile->Config->DefaultValues, ProfileFlash->Profile->Config->ValuesSize);
	LOG_Info("PROFILE_FLASH", "",  "", "Init Configs Values: Idx %d", ProfileFlash->ConfigIdx);
	return PROFILE_FLASH_SaveConfigsValues(ProfileFlash);
}

static int32_t PROFILE_FLASH_InitConfigs(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t ConfigsValuesIdx, RestoreValues;
	
	if (PROFILE_FLASH_CheckConfigsMap(ProfileFlash, &RestoreValues))
	{
		if (RestoreValues)
		{
			if (PROFILE_FLASH_GetConfigsValuesIdx(ProfileFlash, &ConfigsValuesIdx) == 0)
				PROFILE_FLASH_RestoreConfigsValues(ProfileFlash, ConfigsValuesIdx);
		}
		PROFILE_FLASH_SaveConfigsMap(ProfileFlash);		
		PROFILE_FLASH_SaveConfigsValues(ProfileFlash);
	}
	
	return PROFILE_FLASH_InitConfigsValues(ProfileFlash);
}

static int32_t __PROFILE_FLASH_LoadLangpackHeader(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *LangpackBlocks, uint32_t *LangpackType)
{
	PROFILE_FLASH_LANGPACK_RECORD_HEADER_T Header;
	uint32_t Addr;
	uint16_t Crc16;
	
	Idx = Idx % PROFILE_FLASH_LANG_RECORDS_CNT;
	Addr = PROFILE_FLASH_LANG_RECORD_HEADER_ADDR(Idx);
	
	memset(&Header, 0, sizeof(Header));
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	if (memcmp(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		goto error;
	if (memcmp(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		goto error;
	if (Header.LangpackBlocks > PROFILE_FLASH_LANG_RECORD_BODY_SIZE)
		goto error;
	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		goto error;
	
	*LangpackBlocks = Header.LangpackBlocks;
	*LangpackType = Header.LangpackType;

	LOG_Info("PROFILE_FLASH", "", "", "Load Langpack %d Header: Blocks %d, Type %d", Idx, *LangpackBlocks, *LangpackType);
	return 0;
	
error:
	LOG_Warn("PROFILE_FLASH", "", "", "Load Langpack %d Header: Error", Idx);
	return -1;
}

static int32_t __PROFILE_FLASH_SaveLangpackHeader(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t LangpackBlocks, uint32_t LangpackType)
{
	PROFILE_FLASH_LANGPACK_RECORD_HEADER_T Header;
	uint32_t i, Addr;
	uint16_t Crc16;
	
	memset(&Header, 0, sizeof(Header));
	memcpy(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Header.LangpackType = LangpackType;
	Header.LangpackBlocks = LangpackBlocks;
	Header.Crc16 = Calc_CRC16((char*)&Header, sizeof(Header), 0);
	
	Idx = Idx % PROFILE_FLASH_LANG_RECORDS_CNT;
	Addr = PROFILE_FLASH_LANG_RECORD_HEADER_ADDR(Idx);
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	memset(&Header, 0, sizeof(Header));
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		goto error;
	
	for (i = 0; i < PROFILE_FLASH_LANG_RECORDS_CNT; i++)
	{
		if (i != Idx)
		{
			Addr = PROFILE_FLASH_LANG_RECORD_HEADER_ADDR(i);
			if (BSP_FLASH_WaitReady())
				goto error;
			if (BSP_FLASH_ErasePage(Addr))
				goto error;
		}
	}
	
	LOG_Info("PROFILE_FLASH", "",  "", "Save Langpack %d Header: Blocks %d, Type %d", Idx, Header.LangpackBlocks, Header.LangpackType);
	return 0;

error:
	LOG_Crit("PROFILE_FLASH", "",  "", "Save Langpack %d Header: Error", Idx);
	return -1;
}

static int32_t __PROFILE_FLASH_LoadLangpackData(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t DataAddr, char *Data, uint32_t DataSize)
{
	uint32_t Addr;
	
	Idx = Idx % PROFILE_FLASH_LANG_RECORDS_CNT;
	Addr = PROFILE_FLASH_LANG_RECORD_BODY_ADDR(Idx) + DataAddr;
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_Read(Addr, Data, DataSize))
		goto error;
	return 0;
	
error:
	LOG_Crit("PROFILE_FLASH", "",  "", "Load Langpack %d Addr %d Size %d: Error", Idx, DataAddr, DataSize);
	return -1;
}

int32_t __PROFILE_FLASH_SaveLangpackData(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t DataAddr, char *Data, uint32_t DataSize)
{
	char Buffer[BSP_FLASH_PAGE_SIZE];
	uint32_t Addr;
	uint32_t DataIdx, DataBlockSize;
	uint32_t PageAddr, PageIdx;
	
	DataIdx = 0;
	
	while (DataIdx != DataSize)
	{
		PageIdx = (DataAddr + DataIdx) / BSP_FLASH_PAGE_SIZE;
		PageAddr = (DataAddr + DataIdx) % BSP_FLASH_PAGE_SIZE;
		Idx = Idx % PROFILE_FLASH_LANG_RECORDS_CNT;
		Addr = PROFILE_FLASH_LANG_RECORD_BODY_ADDR(Idx) + (PageIdx * BSP_FLASH_PAGE_SIZE);
		if (PageAddr)
		{
			if (BSP_FLASH_WaitReady())
				goto error;
			if (BSP_FLASH_Read(Addr, Buffer, PageAddr))
				goto error;
			DataBlockSize = DataSize > (BSP_FLASH_PAGE_SIZE - PageAddr) ? BSP_FLASH_PAGE_SIZE - PageAddr : DataSize;
			memcpy(&Buffer[PageAddr], &Data[DataIdx], DataBlockSize);
			if (DataBlockSize == DataSize)
				memset(&Buffer[PageAddr + DataBlockSize], 0x03, BSP_FLASH_PAGE_SIZE - PageAddr - DataBlockSize);
			if (BSP_FLASH_WaitReady())
				goto error;
			if (BSP_FLASH_ProgramPage(Addr, Buffer, BSP_FLASH_PAGE_SIZE))
				goto error;
			DataIdx += DataBlockSize;
		}
		else
		{
			if ((DataSize - DataIdx) >= BSP_FLASH_PAGE_SIZE)
			{
				if (BSP_FLASH_WaitReady())
					goto error;
				if (BSP_FLASH_ProgramPage(Addr, &Data[DataIdx], BSP_FLASH_PAGE_SIZE))
					goto error;
				DataIdx += BSP_FLASH_PAGE_SIZE;
			}
			else
			{
				DataBlockSize = DataSize - DataIdx;
				memcpy(&Buffer, &Data[DataIdx], DataBlockSize);
				memset(&Buffer[DataBlockSize], 0x03, BSP_FLASH_PAGE_SIZE - DataBlockSize);
				if (BSP_FLASH_WaitReady())
					goto error;
				if (BSP_FLASH_ProgramPage(Addr, Buffer, BSP_FLASH_PAGE_SIZE))
					goto error;
				DataIdx += DataBlockSize;
			}
		}
	}
	return 0;
	
error:
	LOG_Crit("PROFILE_FLASH", "",  "", "Save Langpack %d Addr %d Size %d: Error", Idx, DataAddr, DataSize);
	return -1;
}

int32_t PROFILE_FLASH_InitLangpacks(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t Idx, LangpackBlocks, LangpackType;
	
	for (Idx = 0; Idx < PROFILE_FLASH_LANG_RECORDS_CNT; Idx++)
	{
		if (__PROFILE_FLASH_LoadLangpackHeader(ProfileFlash, Idx, &LangpackBlocks, &LangpackType))
			continue;
		ProfileFlash->LangpackIdx = Idx;
		LOG_Info("PROFILE_FLASH", "",  "", "Init Langpack %d: Blocks %d, Type %d", Idx, LangpackBlocks, LangpackType);
		return 0;
	}

	ProfileFlash->LangpackIdx = 0;
	__PROFILE_FLASH_SaveLangpackHeader(ProfileFlash, 0, 0, 1);
	LOG_Info("PROFILE_FLASH", "",  "", "Init Langpack 0: Blocks 0, Type 1");
	return 0;	
}

static void PROFILE_FLASH_Thread(void const *argument)
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
			if (PROFILE_FLASH_SaveCalibsValues(ProfileFlash))
				goto error;
			continue;
		}
		else if (Event.value.v == 1)
		{
			if (PROFILE_FLASH_SaveConfigsValues(ProfileFlash))
				goto error;
			continue;
		}
error:
		ErrorCntr++;
		if (ErrorCntr < 5)
			goto repeat;
	}
}

#define PROFILE_FLASH_MESSAGE_QUEUE_SIZE 2
int32_t PROFILE_FLASH_Init(PROFILE_FLASH_T *ProfileFlash, char *ThreadName, const PROFILE_T *Profile)
{
	osThreadDef_t ProfileFlash_osThreadDef = { ThreadName, PROFILE_FLASH_Thread, osPriorityNormal, 0, 256};
	osMessageQDef_t ProfileFlash_osMessageQDef = {PROFILE_FLASH_MESSAGE_QUEUE_SIZE, sizeof(uint32_t)};
	
	if ((!ProfileFlash) || (!Profile))
		return -1;
	
	ProfileFlash->Profile = Profile;
	
	ProfileFlash->CalibIdx = 0;
	ProfileFlash->ConfigIdx = 0;
	ProfileFlash->LangpackIdx = 0;
	
	PROFILE_FLASH_InitCalibs(ProfileFlash);
	PROFILE_FLASH_InitConfigs(ProfileFlash);
	PROFILE_FLASH_InitLangpacks(ProfileFlash);
	
	ProfileFlash->MessageQId = osMessageCreate (&ProfileFlash_osMessageQDef, NULL);
	if (!ProfileFlash->MessageQId)
		return -1;	
	ProfileFlash->ThreadId = osThreadCreate(&ProfileFlash_osThreadDef, ProfileFlash);	
	if ( !ProfileFlash->ThreadId )
		return -1;
	
	return 0;
}

void PROFILE_FLASH_SaveCalibs(PROFILE_FLASH_T *ProfileFlash)
{
	/*Wake up Profile Save Thread*/
	osMessagePut(ProfileFlash->MessageQId, 0, 0);	
}

int32_t PROFILE_FLASH_LoadCalibs(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t CalibsValuesIdx;
	if (PROFILE_FLASH_LoadCalibsRecord(ProfileFlash, (ProfileFlash->CalibIdx - 1) % PROFILE_FLASH_CALIB_RECORDS_CNT, &CalibsValuesIdx))
		goto error;	
	LOG_Info("PROFILE_FLASH", "",  "", "Load Calibs %d", ProfileFlash->CalibIdx - 1);
	return 0;

error:
	memcpy(ProfileFlash->Profile->Calib->Values, ProfileFlash->Profile->Calib->DefaultValues, ProfileFlash->Profile->Calib->ValuesSize);
	LOG_Crit("PROFILE_FLASH", "",  "", "Load Calibs %d: Error", ProfileFlash->CalibIdx - 1);
	return -1;
}

void PROFILE_FLASH_SaveConfigs(PROFILE_FLASH_T *ProfileFlash)
{
	/*Wake up Profile Save Thread*/
	osMessagePut(ProfileFlash->MessageQId, 1, 0);	
}

int32_t PROFILE_FLASH_LoadConfigs(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t ConfigsValuesIdx;
	if (PROFILE_FLASH_LoadConfigsRecord(ProfileFlash, (ProfileFlash->ConfigIdx - 1) % PROFILE_FLASH_CONFIG_RECORDS_CNT, &ConfigsValuesIdx))
		goto error;	
	LOG_Info("PROFILE_FLASH", "",  "", "Load Configs %d", ProfileFlash->ConfigIdx - 1);
	return 0;

error:
	ProfileFlash->ConfigIdx = 0;
	memcpy(ProfileFlash->Profile->Config->Values, ProfileFlash->Profile->Config->DefaultValues, ProfileFlash->Profile->Config->ValuesSize);
	LOG_Crit("PROFILE_FLASH", "",  "", "Load Configs %d: Error", ProfileFlash->ConfigIdx - 1);
	return -1;
}

void PROFILE_FLASH_SaveParamValue(PROFILE_FLASH_T *ProfileFlash, PROFILE_PARAM_T *Param)
{
	if (!PROFILE_IsCalibParam(ProfileFlash->Profile, Param))
		PROFILE_FLASH_SaveCalibs(ProfileFlash);
	else if (!PROFILE_IsConfigParam(ProfileFlash->Profile, Param))
		PROFILE_FLASH_SaveConfigs(ProfileFlash);
}

void PROFILE_FLASH_SaveParamColors(PROFILE_FLASH_T *ProfileFlash, PROFILE_PARAM_T *Param)
{
	PROFILE_FLASH_SaveConfigs(ProfileFlash);
}

int32_t PROFILE_FLASH_LoadLangpackHeader(PROFILE_FLASH_T *ProfileFlash, uint32_t *LangpackBlocks, uint32_t *LangpackType)
{
	return __PROFILE_FLASH_LoadLangpackHeader(ProfileFlash, ProfileFlash->LangpackIdx % PROFILE_FLASH_LANG_RECORDS_CNT, LangpackBlocks, LangpackType);
}

int32_t PROFILE_FLASH_SaveLangpackHeader(PROFILE_FLASH_T *ProfileFlash, uint32_t LangpackBlocks, uint32_t LangpackType)
{
	if (__PROFILE_FLASH_SaveLangpackHeader(ProfileFlash, (ProfileFlash->LangpackIdx + 1) % PROFILE_FLASH_LANG_RECORDS_CNT, LangpackBlocks, LangpackType))
		return -1;
	ProfileFlash->LangpackIdx++;
	return 0;
}

int32_t PROFILE_FLASH_LoadLangpackData(PROFILE_FLASH_T *ProfileFlash, uint32_t DataAddr, char *Data, uint32_t DataSize)
{
	return __PROFILE_FLASH_LoadLangpackData(ProfileFlash, ProfileFlash->LangpackIdx % PROFILE_FLASH_LANG_RECORDS_CNT, DataAddr, Data, DataSize);
}

int32_t PROFILE_FLASH_SaveLangpackData(PROFILE_FLASH_T *ProfileFlash, uint32_t DataAddr, char *Data, uint32_t DataSize)
{
	return __PROFILE_FLASH_SaveLangpackData(ProfileFlash, (ProfileFlash->LangpackIdx + 1) % PROFILE_FLASH_LANG_RECORDS_CNT, DataAddr, Data, DataSize);
}
