#include "profile_flash.h"
#include "profile.h"
#include "cmsis_os.h"
#include "crc.h"
#include "log.h"
#include <string.h>

typedef struct {
	uint32_t Idx;
	char Project[sizeof(BSP_PROJECT)];
	char HwVersion[sizeof(BSP_HW_VERSION)];
	uint32_t ProfileIdx;
	uint16_t Crc16;
}PROFILE_FLASH_HEADER_T;

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

int32_t PROFILE_FLASH_SaveHeader(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_HEADER_T Header;
	uint32_t Addr;
	uint16_t Crc16;
	
	memset(&Header, 0, sizeof(Header));
	memcpy(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Header.Idx = ProfileFlash->HeaderIdx;
	Header.ProfileIdx = ProfileFlash->Profile->Idx;
	Header.Crc16 = Calc_CRC16((char*)&Header, sizeof(Header), 0);
	
	Addr = PROFILE_FLASH_HEADER_ADDR(Header.Idx % PROFILE_FLASH_HEADERS_CNT);
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
	
	if (Header.Idx == 0)
	{
		Addr = PROFILE_FLASH_HEADER_ADDR((Header.Idx + 1) % PROFILE_FLASH_HEADERS_CNT);
		if (BSP_FLASH_WaitReady())
			goto error;
		if (BSP_FLASH_ErasePage(Addr))
			goto error;
	}
	
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Save Header %d: Header Idx %d, Profile Idx %d", Header.Idx % PROFILE_FLASH_HEADERS_CNT, Header.Idx, Header.ProfileIdx);
	ProfileFlash->HeaderIdx++;	
	return 0;

error:
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Save Header %d: Error", Header.Idx % PROFILE_FLASH_HEADERS_CNT);
	return -1;
}

int32_t PROFILE_FLASH_LoadHeader(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *HeaderIdx, uint32_t *ProfileIdx)
{
	PROFILE_FLASH_HEADER_T Header;
	uint32_t Addr;
	uint16_t Crc16;
	
	if (BSP_FLASH_WaitReady())
		goto error;
	
	memset(&Header, 0, sizeof(Header));
	Addr = PROFILE_FLASH_HEADER_ADDR(Idx);
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	if (memcmp(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		goto error;
	if (memcmp(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		goto error;
	if (Header.ProfileIdx >= BSP_CONFIG_PROFILES_CNT)
		goto error;
	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		goto error;
	
	*ProfileIdx = Header.ProfileIdx;
	*HeaderIdx = Header.Idx;

	LOG_Add("PROFILE_FLASH", "", "Load Header %d: Header Idx %d, Profile Idx %d", Idx, *HeaderIdx, *ProfileIdx);
	return 0;
	
error:
	LOG_Add("PROFILE_FLASH", "", "Load Header %d: Error", Idx);
	return -1;
}

int32_t PROFILE_FLASH_SaveProfileIdx(PROFILE_FLASH_T *ProfileFlash, uint32_t ProfileIdx)
{
	ProfileFlash->Profile->Idx = ProfileIdx;
	return PROFILE_FLASH_SaveHeader(ProfileFlash);	
}

int32_t PROFILE_FLASH_InitHeaders(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t i, HeaderLoaded, HeaderIdx, HeaderIdxSave, HeaderProfileIdx, HeaderProfileIdxSave;
	
	HeaderLoaded = 0;
	HeaderIdx = 0;
	HeaderProfileIdx = 0;
	HeaderIdxSave = 0;
	HeaderProfileIdxSave = 0;
	
	for (i = 0; i < PROFILE_FLASH_HEADERS_CNT; i++)
	{
		if (PROFILE_FLASH_LoadHeader(ProfileFlash, i, &HeaderIdx, &HeaderProfileIdx))
			continue;
		
		if (HeaderIdx >= HeaderIdxSave)
		{
			HeaderLoaded = 1;
			HeaderIdxSave = HeaderIdx;
			HeaderProfileIdxSave = HeaderProfileIdx;
		}
	}
	
	if (HeaderLoaded)
	{
		ProfileFlash->HeaderIdx = HeaderIdxSave + 1;
		ProfileFlash->Profile->Idx = HeaderProfileIdxSave;
		LOG_Add("PROFILE_FLASH", "", "Init Profile Idx %d Class %s", ProfileFlash->Profile->Idx, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass);
		return 0;
	}
	
	ProfileFlash->HeaderIdx = 0;
	ProfileFlash->Profile->Idx = 0;
	LOG_Add("PROFILE_FLASH", "", "Init Profile Idx %d Class %s", ProfileFlash->Profile->Idx, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass);
	return PROFILE_FLASH_SaveHeader(ProfileFlash);
}

int32_t PROFILE_FLASH_CheckCalibsMap(PROFILE_FLASH_T *ProfileFlash, uint32_t *RestoreValues)
{
	PROFILE_FLASH_CALIB_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, Addr, Size, Pages, Len;
	uint16_t Crc16;
	
	if (BSP_FLASH_WaitReady())
		goto error;
	
	Addr = PROFILE_FLASH_CALIB_MAP_HEADER_ADDR(ProfileFlash->Profile->Idx);
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
	
	Addr = PROFILE_FLASH_CALIB_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
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
	
	Addr = PROFILE_FLASH_CALIB_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
	Size = 0;
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1Size; i++) {
		if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_1, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Name) + 1;
		if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
			goto error;
		if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Name, Page, Len))
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Type != Page[Len])
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Table != Page[Len + 1])
			goto mismatch;
		Size += Len + 2;
	}
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2Size; i++) {
		if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_2, i+1))
			continue;
		if (!ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].DegradeColor)
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Name) + 1;
		if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
			goto error;
		if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Name, Page, Len))
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Type != Page[Len])
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Table != Page[Len + 1])
			goto mismatch;
		Size += Len + 2;
	}
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3Size; i++) {
		if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_3, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Name) + 1;
		if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
			goto error;
		if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Name, Page, Len))
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Type != Page[Len])
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Table != Page[Len + 1])
			goto mismatch;
		Size += Len + 2;
	}
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4Size; i++) {
		if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_4, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Name) + 1;
		if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
			goto error;
		if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Name, Page, Len))
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Type != Page[Len])
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Table != Page[Len + 1])
			goto mismatch;
		Size += Len + 2;
	}
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103Size; i++) {
		if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_103, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Name) + 1;
		if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
			goto error;
		if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Name, Page, Len))
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Type != Page[Len])
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Table != Page[Len + 1])
			goto mismatch;
		Size += Len + 2;
	}
	
	if (Header.CalibsMapSize != Size)
		goto mismatch;
	
	*RestoreValues = 0;
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Calibs Map Check: Ok");
	return 0;
	
error:
	*RestoreValues = 0;
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Calibs Map Check: Invalid");
	return -1;
	
mismatch:
	*RestoreValues = 1;
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Calibs Map Check: Mismatch");
	return -1;
}

int32_t PROFILE_FLASH_SaveCalibsMap(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CALIB_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, Addr, Size, Len, PageIdx;
	uint16_t Crc16;
	
	Addr = PROFILE_FLASH_CALIB_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
	Size = 0;
	Crc16 = 0;
	PageIdx = 0;
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1Size; i++)
	{
		if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_1, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Name) + 1;
		for (j = 0; j < (Len + 2); j++)
		{
			if (j == Len)
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Type;
			else if (j == (Len + 1))
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Table;
			else
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Name[j];
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
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2Size; i++)
	{
		if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_2, i+1))
			continue;
		if (!ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].DegradeColor)
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Name) + 1;
		for (j = 0; j < (Len + 2); j++)
		{
			if (j == Len)
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Type;
			else if (j == (Len + 1))
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Table;
			else
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Name[j];
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
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3Size; i++)
	{
		if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_3, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Name) + 1;
		for (j = 0; j < (Len + 2); j++)
		{
			if (j == Len)
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Type;
			else if (j == (Len + 1))
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Table;
			else
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Name[j];
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
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4Size; i++)
	{
		if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_4, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Name) + 1;
		for (j = 0; j < (Len + 2); j++)
		{
			if (j == Len)
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Type;
			else if (j == (Len + 1))
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Table;
			else
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Name[j];
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
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103Size; i++)
	{
		if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_103, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Name) + 1;
		for (j = 0; j < (Len + 2); j++)
		{
			if (j == Len)
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Type;
			else if (j == (Len + 1))
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Table;
			else
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Name[j];
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
	
	Addr = PROFILE_FLASH_CALIB_MAP_HEADER_ADDR(ProfileFlash->Profile->Idx);
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Save Calibs Map: %d bytes", Size);
	return -1;
	
error:
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Save Calibs Map: Error");
	return -1;	
}

int32_t PROFILE_FLASH_GetCalibsValuesIdx(PROFILE_FLASH_T *ProfileFlash, uint32_t *CalibsValuesIdx)
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
		Addr = PROFILE_FLASH_CALIB_RECORD_HEADER_ADDR(ProfileFlash->Profile->Idx, i);
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
		
		Addr = PROFILE_FLASH_CALIB_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, i);
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
		LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Get Calibs Values: %d", *CalibsValuesIdx);
		return 0;
	}
	
error:	
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Get Calibs Values: Error");
	return -1;
}

void PROFILE_FLASH_RestoreCalibsValues(PROFILE_FLASH_T *ProfileFlash, uint32_t CalibsValuesIdx)
{
	PROFILE_FLASH_CALIB_MAP_HEADER_T CalibMapHeader;
	uint32_t i, Len, MapAddr, ValAddr, RestoredParamsCnt;
	char Map[BSP_FLASH_PAGE_SIZE];
	char Value[PROFILE_STRING_MAX_LEN];
	
	if (BSP_FLASH_WaitReady())
		goto exit;
	
	RestoredParamsCnt = 0;
	
	MapAddr = PROFILE_FLASH_CALIB_MAP_HEADER_ADDR(ProfileFlash->Profile->Idx);
	if (BSP_FLASH_Read(MapAddr, (char*)&CalibMapHeader, sizeof(CalibMapHeader)))
		goto exit;
	
	MapAddr = PROFILE_FLASH_CALIB_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
	ValAddr = PROFILE_FLASH_CALIB_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, CalibsValuesIdx % PROFILE_FLASH_CALIB_RECORDS_CNT);
	
	while (MapAddr < (PROFILE_FLASH_CALIB_MAP_BODY_ADDR(ProfileFlash->Profile->Idx) + CalibMapHeader.CalibsMapSize)) {
		if (BSP_FLASH_Read(MapAddr, Map, sizeof(Map)))
			goto exit;
		if (BSP_FLASH_Read(ValAddr, Value, sizeof(Value)))
			goto exit;
		
		Len = 0;
		for (i = 0; i < sizeof(Map); i++)
		{
			if (Map[i] == 0)
			{
				Len = i + 1;
				break;
			}			
		}
		
		if ((Len == 0) || (Len > (sizeof(Map) - 2)))
			goto exit;
		
		if (Map[Len+1] == PROFILE_TABLE_1)
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1Size; i++)
			{
				if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_1, i+1))
					continue;
				if (!strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Name))
				{
					if (Map[Len] == ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Type)
					{
						/*Set Param value with checks and dependencies*/
						if (PROFILE_CheckValue(ProfileFlash->Profile, PROFILE_TABLE_1, i + 1, Value, 1) == 0)
						{
							if (PROFILE_SetValue(ProfileFlash->Profile, PROFILE_TABLE_1, i + 1, Value) == 0)
							{
								PROFILE_SetValueDependencies(ProfileFlash->Profile, PROFILE_TABLE_1, i + 1, Value, 1);
								RestoredParamsCnt++;
							}
						}				
					}
				}
			}
		}
		else if (Map[Len+1] == PROFILE_TABLE_2)
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2Size; i++)
			{
				if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_2, i+1))
					continue;
				if (!ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].DegradeColor)
					continue;
				if (!strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Name))
				{
					if (Map[Len] == ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Type)
					{
						/*Set Param value with checks and dependencies*/
						PROFILE_SetAlarmColors(ProfileFlash->Profile, PROFILE_TABLE_2, i + 1, Value[0], Value[1]);
						RestoredParamsCnt++;
					}
				}
			}
		}
		else if (Map[Len+1] == PROFILE_TABLE_3)
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3Size; i++)
			{
				if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_3, i+1))
					continue;
				if (!strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Name))
				{
					if (Map[Len] == ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Type)
					{
						/*Set Param value with checks and dependencies*/
						if (PROFILE_CheckValue(ProfileFlash->Profile, PROFILE_TABLE_3, i + 1, Value, 1) == 0)
						{
							if (PROFILE_SetValue(ProfileFlash->Profile, PROFILE_TABLE_3, i + 1, Value) == 0)
							{
								PROFILE_SetValueDependencies(ProfileFlash->Profile, PROFILE_TABLE_3, i + 1, Value, 1);
								RestoredParamsCnt++;
							}
						}
					}
				}
			}
		}
		else if (Map[Len+1] == PROFILE_TABLE_4)
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4Size; i++)
			{
				if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_4, i+1))
					continue;
				if (!strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Name))
				{
					if (Map[Len] == ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Type)
					{
						/*Set Param value with checks and dependencies*/
						if (PROFILE_CheckValue(ProfileFlash->Profile, PROFILE_TABLE_4, i + 1, Value, 1) == 0)
						{
							if (PROFILE_SetValue(ProfileFlash->Profile, PROFILE_TABLE_4, i + 1, Value) == 0)
							{
								PROFILE_SetValueDependencies(ProfileFlash->Profile, PROFILE_TABLE_4, i + 1, Value, 1);
								RestoredParamsCnt++;
							}
						}
					}
				}
			}
		}
		else if (Map[Len+1] == PROFILE_TABLE_103)
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103Size; i++)
			{
				if (!PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_103, i+1))
					continue;
				if (!strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Name))
				{
					if (Map[Len] == ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Type)
					{
						/*Set Param value with checks and dependencies*/
						if (PROFILE_CheckValue(ProfileFlash->Profile, PROFILE_TABLE_103, i + 1, Value, 1) == 0)
						{
							if (PROFILE_SetValue(ProfileFlash->Profile, PROFILE_TABLE_103, i + 1, Value) == 0)
							{
								PROFILE_SetValueDependencies(ProfileFlash->Profile, PROFILE_TABLE_103, i + 1, Value, 1);
								RestoredParamsCnt++;
							}
						}
					}
				}
			}
		}
		MapAddr += Len + 2;
		if (Map[Len + 1] == PROFILE_TABLE_2)
		{
			ValAddr += 2;
		}
		else
		{
			switch( Map[Len] )
			{
				case PROFILE_TYPE_BOOL:
				case PROFILE_TYPE_UINT32:
				case PROFILE_TYPE_CNT:
				case PROFILE_TYPE_BITMSK:
				case PROFILE_TYPE_HEX32:
				case PROFILE_TYPE_IP:
				case PROFILE_TYPE_ENUM:
				case PROFILE_TYPE_INT32:
				case PROFILE_TYPE_FLOAT_1:
				case PROFILE_TYPE_FLOAT_2:
				case PROFILE_TYPE_FLOAT_3:
				case PROFILE_TYPE_FLOAT_E:
					ValAddr += 4;
					break;
				case PROFILE_TYPE_UINT64:
					ValAddr += 8;
					break;
				case PROFILE_TYPE_OID:// строка из чисел, разделённых "."
				case PROFILE_TYPE_STRING:// произвольная строка
					ValAddr += PROFILE_STRING_MAX_LEN;
					break;
				default:
					goto exit;
			}
		}
	}

exit:
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Restore Calibs Values: %d params", RestoredParamsCnt);
}

static int32_t PROFILE_FLASH_LoadCalibsRecord(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *RecordIdx)
{
	PROFILE_FLASH_CALIB_RECORD_HEADER_T CalibRecordHeader;
	uint32_t Addr;
	uint16_t Crc16;
	
	if (BSP_FLASH_WaitReady())
		goto error;
	
	Addr = PROFILE_FLASH_CALIB_RECORD_HEADER_ADDR(ProfileFlash->Profile->Idx, Idx);
	if (BSP_FLASH_Read(Addr, (char*)&CalibRecordHeader, sizeof(CalibRecordHeader)))
		goto error;
	if (memcmp(CalibRecordHeader.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		goto error;
	if (memcmp(CalibRecordHeader.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		goto error;
	if (CalibRecordHeader.CalibsSize != ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize)
		goto error;
	Crc16 = CalibRecordHeader.Crc16;
	CalibRecordHeader.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&CalibRecordHeader, sizeof(CalibRecordHeader), 0))
		goto error;
	
	Addr = PROFILE_FLASH_CALIB_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, Idx);	
	if( osMutexWait( ProfileFlash->Profile->Values->Mutex, osWaitForever ) != osOK )
		goto error;
	if (BSP_FLASH_Read(Addr, ProfileFlash->Profile->Values->Calib, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize))
		goto release_and_error;
	Crc16 = Calc_CRC16((char*)ProfileFlash->Profile->Values->Calib, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize, 0);
	if (Crc16 != CalibRecordHeader.CalibsCrc16)
		goto release_and_error;
	osMutexRelease( ProfileFlash->Profile->Values->Mutex );
	
	*RecordIdx = CalibRecordHeader.Idx;
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Load Calibs Values %d: Idx %d", Idx, *RecordIdx);
	return 0;

release_and_error:
	osMutexRelease( ProfileFlash->Profile->Values->Mutex );
error:	
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Load Calibs Values %d: Error", Idx);
	return -1;
}

int32_t PROFILE_FLASH_SaveCalibsValues(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CALIB_RECORD_HEADER_T Header;
	char *p;
	uint32_t Addr;
	uint32_t i, Size, Pages;
	uint16_t Crc16;
	char Page[BSP_FLASH_PAGE_SIZE];
	
	osMutexWait(ProfileFlash->Profile->Values->Mutex, osWaitForever);
	Crc16 = Calc_CRC16((char*)ProfileFlash->Profile->Values->Calib, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize, 0);
	osMutexRelease( ProfileFlash->Profile->Values->Mutex );

	p = (char*)ProfileFlash->Profile->Values->Calib;
	
	memset(&Header, 0, sizeof(Header));
	memcpy(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Header.Idx = ProfileFlash->CalibIdx;
	Header.CalibsSize = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize;
	Header.CalibsCrc16 = Crc16;
	Header.Crc16 = Calc_CRC16((char*)&Header, sizeof(Header), 0);
	
	Addr = PROFILE_FLASH_CALIB_RECORD_HEADER_ADDR(ProfileFlash->Profile->Idx, (ProfileFlash->CalibIdx % PROFILE_FLASH_CALIB_RECORDS_CNT));
	
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
	
	Addr = PROFILE_FLASH_CALIB_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, (ProfileFlash->CalibIdx % PROFILE_FLASH_CALIB_RECORDS_CNT));
	if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize % BSP_FLASH_PAGE_SIZE)
		Pages = (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize / BSP_FLASH_PAGE_SIZE) + 1;
	else
		Pages = (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize / BSP_FLASH_PAGE_SIZE);
	Crc16 = 0;
	for (i = 0; i < Pages; i++)
	{
		if (i == (Pages - 1))
			Size = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize - (i * BSP_FLASH_PAGE_SIZE);
		else 
			Size = BSP_FLASH_PAGE_SIZE;
		if (BSP_FLASH_WaitReady())
			goto error;
		osMutexWait(ProfileFlash->Profile->Values->Mutex, osWaitForever);
		if (BSP_FLASH_ProgramPage(Addr, p, Size))
		{
			osMutexRelease( ProfileFlash->Profile->Values->Mutex );
			goto error;
		}
		osMutexRelease( ProfileFlash->Profile->Values->Mutex );
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
			Addr = PROFILE_FLASH_CALIB_RECORD_HEADER_ADDR(ProfileFlash->Profile->Idx, ((Header.Idx + i) % PROFILE_FLASH_CALIB_RECORDS_CNT));
			if (BSP_FLASH_WaitReady())
				goto error;
			if (BSP_FLASH_ErasePage(Addr))
				goto error;
		}
	}
	
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Save Calibs Values %d: %d bytes", ProfileFlash->CalibIdx, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize);
	ProfileFlash->CalibIdx++;
	return 0;
	
error:
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Save Calibs Values %d: Error", ProfileFlash->CalibIdx);
	return -1;	
}

int32_t PROFILE_FLASH_InitCalibsValues(PROFILE_FLASH_T *ProfileFlash)
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
		LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Init Calibs Values: Idx %d", ProfileFlash->CalibIdx);
		return 0;
	}

error:
	ProfileFlash->CalibIdx = 0;
	memcpy(ProfileFlash->Profile->Values->Calib, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibDefaultValues, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize);
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Init Calibs Values: Idx %d", ProfileFlash->CalibIdx);
	return PROFILE_FLASH_SaveCalibsValues(ProfileFlash);
}

int32_t PROFILE_FLASH_InitCalibs(PROFILE_FLASH_T *ProfileFlash)
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

int32_t PROFILE_FLASH_CheckConfigsMap(PROFILE_FLASH_T *ProfileFlash, uint32_t *RestoreValues)
{
	PROFILE_FLASH_CONFIG_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, Addr, Size, Pages, Len;
	uint16_t Crc16;
	
	if (BSP_FLASH_WaitReady())
		goto error;
	
	Addr = PROFILE_FLASH_CONFIG_MAP_HEADER_ADDR(ProfileFlash->Profile->Idx);
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
	
	Addr = PROFILE_FLASH_CONFIG_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
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
	
	Addr = PROFILE_FLASH_CONFIG_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
	Size = 0;
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1Size; i++) {
		if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_1, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Name) + 1;
		if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
			goto error;
		if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Name, Page, Len))
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Type != Page[Len])
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Table != Page[Len + 1])
			goto mismatch;
		Size += Len + 2;
	}
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2Size; i++) {
		if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_2, i+1))
			continue;
		if (!ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].DegradeColor)
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Name) + 1;
		if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
			goto error;
		if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Name, Page, Len))
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Type != Page[Len])
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Table != Page[Len + 1])
			goto mismatch;
		Size += Len + 2;
	}
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3Size; i++) {
		if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_3, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Name) + 1;
		if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
			goto error;
		if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Name, Page, Len))
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Type != Page[Len])
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Table != Page[Len + 1])
			goto mismatch;
		Size += Len + 2;
	}
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4Size; i++) {
		if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_4, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Name) + 1;
		if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
			goto error;
		if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Name, Page, Len))
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Type != Page[Len])
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Table != Page[Len + 1])
			goto mismatch;
		Size += Len + 2;
	}
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103Size; i++) {
		if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_103, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Name) + 1;
		if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
			goto error;
		if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Name, Page, Len))
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Type != Page[Len])
			goto mismatch;
		if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Table != Page[Len + 1])
			goto mismatch;
		Size += Len + 2;
	}
	
	if (Header.ConfigsMapSize != Size)
		goto mismatch;
	
	*RestoreValues = 0;
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Configs Map Check: Ok");
	return 0;
	
error:
	*RestoreValues = 0;
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Configs Map Check: Invalid");
	return -1;
	
mismatch:
	*RestoreValues = 1;
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Configs Map Check: Mismatch");
	return -1;
}

int32_t PROFILE_FLASH_SaveConfigsMap(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CONFIG_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, Addr, Size, Len, PageIdx;
	uint16_t Crc16;
	
	Addr = PROFILE_FLASH_CONFIG_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
	Size = 0;
	Crc16 = 0;
	PageIdx = 0;
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1Size; i++)
	{
		if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_1, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Name) + 1;
		for (j = 0; j < (Len + 2); j++)
		{
			if (j == Len)
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Type;
			else if (j == (Len + 1))
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Table;
			else
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Name[j];
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
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2Size; i++)
	{
		if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_2, i+1))
			continue;
		if (!ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].DegradeColor)
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Name) + 1;
		for (j = 0; j < (Len + 2); j++)
		{
			if (j == Len)
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Type;
			else if (j == (Len + 1))
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Table;
			else
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Name[j];
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
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3Size; i++)
	{
		if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_3, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Name) + 1;
		for (j = 0; j < (Len + 2); j++)
		{
			if (j == Len)
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Type;
			else if (j == (Len + 1))
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Table;
			else
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Name[j];
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
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4Size; i++)
	{
		if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_4, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Name) + 1;
		for (j = 0; j < (Len + 2); j++)
		{
			if (j == Len)
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Type;
			else if (j == (Len + 1))
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Table;
			else
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Name[j];
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
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103Size; i++)
	{
		if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_103, i+1))
			continue;
		Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Name) + 1;
		for (j = 0; j < (Len + 2); j++)
		{
			if (j == Len)
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Type;
			else if (j == (Len + 1))
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Table;
			else
				Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Name[j];
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
	
	Addr = PROFILE_FLASH_CONFIG_MAP_HEADER_ADDR(ProfileFlash->Profile->Idx);
	if (BSP_FLASH_WaitReady())
		goto error;
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Save Configs Map: %d bytes", Size);
	return -1;
	
error:
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Save Configs Map: Error");
	return -1;	
}

int32_t PROFILE_FLASH_GetConfigsValuesIdx(PROFILE_FLASH_T *ProfileFlash, uint32_t *ConfigsValuesIdx)
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
		Addr = PROFILE_FLASH_CONFIG_RECORD_HEADER_ADDR(ProfileFlash->Profile->Idx, i);
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
		
		Addr = PROFILE_FLASH_CONFIG_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, i);
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
		LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Get Configs Values: %d", *ConfigsValuesIdx);
		return 0;
	}
	
error:	
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Get Configs Values: Error");
	return -1;
}

void PROFILE_FLASH_RestoreConfigsValues(PROFILE_FLASH_T *ProfileFlash, uint32_t ConfigsValuesIdx)
{
	PROFILE_FLASH_CONFIG_MAP_HEADER_T ConfigMapHeader;
	uint32_t i, Len, MapAddr, ValAddr, RestoredParamsCnt;
	char Map[BSP_FLASH_PAGE_SIZE];
	char Value[PROFILE_STRING_MAX_LEN];
	
	if (BSP_FLASH_WaitReady())
		goto exit;
	
	RestoredParamsCnt = 0;
	
	MapAddr = PROFILE_FLASH_CONFIG_MAP_HEADER_ADDR(ProfileFlash->Profile->Idx);
	if (BSP_FLASH_Read(MapAddr, (char*)&ConfigMapHeader, sizeof(ConfigMapHeader)))
		goto exit;
	
	MapAddr = PROFILE_FLASH_CONFIG_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
	ValAddr = PROFILE_FLASH_CONFIG_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, ConfigsValuesIdx % PROFILE_FLASH_CONFIG_RECORDS_CNT);
	
	while (MapAddr < (PROFILE_FLASH_CONFIG_MAP_BODY_ADDR(ProfileFlash->Profile->Idx) + ConfigMapHeader.ConfigsMapSize)) {
		if (BSP_FLASH_Read(MapAddr, Map, sizeof(Map)))
			goto exit;
		if (BSP_FLASH_Read(ValAddr, Value, sizeof(Value)))
			goto exit;
		
		Len = 0;
		for (i = 0; i < sizeof(Map); i++)
		{
			if (Map[i] == 0)
			{
				Len = i + 1;
				break;
			}			
		}
		
		if ((Len == 0) || (Len > (sizeof(Map) - 2)))
			goto exit;
		if (Map[Len+1] == PROFILE_TABLE_1)
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1Size; i++)
			{
				if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_1, i+1))
					continue;
				if (!strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Name))
				{
					if (Map[Len] == ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[i].Type)
					{
						/*Set Param value with checks and dependencies*/
						if (PROFILE_CheckValue(ProfileFlash->Profile, PROFILE_TABLE_1, i + 1, Value, 1) == 0)
						{
							if (PROFILE_SetValue(ProfileFlash->Profile, PROFILE_TABLE_1, i + 1, Value) == 0)
							{
								PROFILE_SetValueDependencies(ProfileFlash->Profile, PROFILE_TABLE_1, i + 1, Value, 1);
								RestoredParamsCnt++;
							}
						}				
					}
				}
			}
		}
		else if (Map[Len+1] == PROFILE_TABLE_2)
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2Size; i++)
			{
				if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_2, i+1))
					continue;
				if (!ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].DegradeColor)
					continue;
				if (!strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Name))
				{
					if (Map[Len] == ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table2[i].Type)
					{
						/*Set Param value with checks and dependencies*/
						PROFILE_SetAlarmColors(ProfileFlash->Profile, PROFILE_TABLE_2, i + 1, Value[0], Value[1]);
						RestoredParamsCnt++;
					}
				}
			}
		}
		else if (Map[Len+1] == PROFILE_TABLE_3)
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3Size; i++)
			{
				if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_3, i+1))
					continue;
				if (!strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Name))
				{
					if (Map[Len] == ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[i].Type)
					{
						/*Set Param value with checks and dependencies*/
						if (PROFILE_CheckValue(ProfileFlash->Profile, PROFILE_TABLE_3, i + 1, Value, 1) == 0)
						{
							if (PROFILE_SetValue(ProfileFlash->Profile, PROFILE_TABLE_3, i + 1, Value) == 0)
							{
								PROFILE_SetValueDependencies(ProfileFlash->Profile, PROFILE_TABLE_3, i + 1, Value, 1);
								RestoredParamsCnt++;
							}
						}
					}
				}
			}
		}
		else if (Map[Len+1] == PROFILE_TABLE_4)
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4Size; i++)
			{
				if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_4, i+1))
					continue;
				if (!strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Name))
				{
					if (Map[Len] == ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table4[i].Type)
					{
						/*Set Param value with checks and dependencies*/
						if (PROFILE_CheckValue(ProfileFlash->Profile, PROFILE_TABLE_4, i + 1, Value, 1) == 0)
						{
							if (PROFILE_SetValue(ProfileFlash->Profile, PROFILE_TABLE_4, i + 1, Value) == 0)
							{
								PROFILE_SetValueDependencies(ProfileFlash->Profile, PROFILE_TABLE_4, i + 1, Value, 1);
								RestoredParamsCnt++;
							}
						}
					}
				}
			}
		}
		else if (Map[Len+1] == PROFILE_TABLE_103)
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103Size; i++)
			{
				if (PROFILE_IsCalibParam(ProfileFlash->Profile, PROFILE_TABLE_103, i+1))
					continue;
				if (!strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Name))
				{
					if (Map[Len] == ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table103[i].Type)
					{
						/*Set Param value with checks and dependencies*/
						if (PROFILE_CheckValue(ProfileFlash->Profile, PROFILE_TABLE_103, i + 1, Value, 1) == 0)
						{
							if (PROFILE_SetValue(ProfileFlash->Profile, PROFILE_TABLE_103, i + 1, Value) == 0)
							{
								PROFILE_SetValueDependencies(ProfileFlash->Profile, PROFILE_TABLE_103, i + 1, Value, 1);
								RestoredParamsCnt++;
							}
						}
					}
				}
			}
		}
		MapAddr += Len + 2;
		if (Map[Len + 1] == PROFILE_TABLE_2)
		{
			ValAddr += 2;
		}
		else
		{
			switch( Map[Len] )
			{
				case PROFILE_TYPE_BOOL:
				case PROFILE_TYPE_UINT32:
				case PROFILE_TYPE_CNT:
				case PROFILE_TYPE_BITMSK:
				case PROFILE_TYPE_HEX32:
				case PROFILE_TYPE_IP:
				case PROFILE_TYPE_ENUM:
				case PROFILE_TYPE_INT32:
				case PROFILE_TYPE_FLOAT_1:
				case PROFILE_TYPE_FLOAT_2:
				case PROFILE_TYPE_FLOAT_3:
				case PROFILE_TYPE_FLOAT_E:
					ValAddr += 4;
					break;
				case PROFILE_TYPE_UINT64:
					ValAddr += 8;
					break;
				case PROFILE_TYPE_OID:// строка из чисел, разделённых "."
				case PROFILE_TYPE_STRING:// произвольная строка
					ValAddr += PROFILE_STRING_MAX_LEN;
					break;
				default:
					goto exit;
			}
		}
	}

exit:
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Restore Configs Values: %d params", RestoredParamsCnt);
}

static int32_t PROFILE_FLASH_LoadConfigsRecord(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *RecordIdx)
{
	PROFILE_FLASH_CONFIG_RECORD_HEADER_T ConfigRecordHeader;
	uint32_t Addr;
	uint16_t Crc16;
	
	if (BSP_FLASH_WaitReady())
		goto error;
	
	Addr = PROFILE_FLASH_CONFIG_RECORD_HEADER_ADDR(ProfileFlash->Profile->Idx, Idx);
	if (BSP_FLASH_Read(Addr, (char*)&ConfigRecordHeader, sizeof(ConfigRecordHeader)))
		goto error;
	if (memcmp(ConfigRecordHeader.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		goto error;
	if (memcmp(ConfigRecordHeader.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		goto error;
	if (ConfigRecordHeader.ConfigsSize != ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize)
		goto error;
	Crc16 = ConfigRecordHeader.Crc16;
	ConfigRecordHeader.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&ConfigRecordHeader, sizeof(ConfigRecordHeader), 0))
		goto error;
	
	Addr = PROFILE_FLASH_CONFIG_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, Idx);	
	if( osMutexWait( ProfileFlash->Profile->Values->Mutex, osWaitForever ) != osOK )
		goto error;
	if (BSP_FLASH_Read(Addr, ProfileFlash->Profile->Values->Static, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize))
		goto release_and_error;
	Crc16 = Calc_CRC16((char*)ProfileFlash->Profile->Values->Static, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize, 0);
	if (Crc16 != ConfigRecordHeader.ConfigsCrc16)
		goto release_and_error;
	osMutexRelease( ProfileFlash->Profile->Values->Mutex );
	
	*RecordIdx = ConfigRecordHeader.Idx;
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Load Configs Values %d: Idx %d", Idx, *RecordIdx);
	return 0;

release_and_error:
	osMutexRelease( ProfileFlash->Profile->Values->Mutex );
error:	
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Load Configs Values %d: Error", Idx);
	return -1;
}

int32_t PROFILE_FLASH_SaveConfigsValues(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CONFIG_RECORD_HEADER_T Header;
	char *p;
	uint32_t Addr;
	uint32_t i, Size, Pages;
	uint16_t Crc16;
	char Page[BSP_FLASH_PAGE_SIZE];
	
	osMutexWait(ProfileFlash->Profile->Values->Mutex, osWaitForever);
	Crc16 = Calc_CRC16((char*)ProfileFlash->Profile->Values->Static, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize, 0);
	osMutexRelease( ProfileFlash->Profile->Values->Mutex );

	p = (char*)ProfileFlash->Profile->Values->Static;
	
	memset(&Header, 0, sizeof(Header));
	memcpy(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Header.Idx = ProfileFlash->ConfigIdx;
	Header.ConfigsSize = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize;
	Header.ConfigsCrc16 = Crc16;
	Header.Crc16 = Calc_CRC16((char*)&Header, sizeof(Header), 0);
	
	Addr = PROFILE_FLASH_CONFIG_RECORD_HEADER_ADDR(ProfileFlash->Profile->Idx, (ProfileFlash->ConfigIdx % PROFILE_FLASH_CONFIG_RECORDS_CNT));
	
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
	
	Addr = PROFILE_FLASH_CONFIG_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, (ProfileFlash->ConfigIdx % PROFILE_FLASH_CONFIG_RECORDS_CNT));
	if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize % BSP_FLASH_PAGE_SIZE)
		Pages = (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize / BSP_FLASH_PAGE_SIZE) + 1;
	else
		Pages = (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize / BSP_FLASH_PAGE_SIZE);
	Crc16 = 0;
	for (i = 0; i < Pages; i++)
	{
		if (i == (Pages - 1))
			Size = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize - (i * BSP_FLASH_PAGE_SIZE);
		else 
			Size = BSP_FLASH_PAGE_SIZE;
		if (BSP_FLASH_WaitReady())
			goto error;
		osMutexWait(ProfileFlash->Profile->Values->Mutex, osWaitForever);
		if (BSP_FLASH_ProgramPage(Addr, p, Size))
		{
			osMutexRelease( ProfileFlash->Profile->Values->Mutex );
			goto error;
		}
		osMutexRelease( ProfileFlash->Profile->Values->Mutex );
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
			Addr = PROFILE_FLASH_CONFIG_RECORD_HEADER_ADDR(ProfileFlash->Profile->Idx, ((Header.Idx + i) % PROFILE_FLASH_CONFIG_RECORDS_CNT));
			if (BSP_FLASH_WaitReady())
				goto error;
			if (BSP_FLASH_ErasePage(Addr))
				goto error;
		}
	}
	
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Save Configs Values %d: %d bytes", ProfileFlash->ConfigIdx, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize);
	ProfileFlash->ConfigIdx++;
	return 0;
	
error:
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Save Configs Values %d: Error", ProfileFlash->ConfigIdx);
	return -1;	
}

int32_t PROFILE_FLASH_InitConfigsValues(PROFILE_FLASH_T *ProfileFlash)
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
		LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Init Configs Values: Idx %d", ProfileFlash->ConfigIdx);
		return 0;
	}

error:
	ProfileFlash->ConfigIdx = 0;
	memcpy(ProfileFlash->Profile->Values->Static, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticDefaultValues, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize);
	LOG_Add("PROFILE_FLASH", ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass, "Init Configs Values: Idx %d", ProfileFlash->ConfigIdx);
	return PROFILE_FLASH_SaveConfigsValues(ProfileFlash);
}

int32_t PROFILE_FLASH_InitConfigs(PROFILE_FLASH_T *ProfileFlash)
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
int32_t PROFILE_FLASH_Init(PROFILE_FLASH_T *ProfileFlash, char *ThreadName, PROFILE_T *Profile)
{
	osThreadDef_t ProfileFlash_osThreadDef = { ThreadName, PROFILE_FLASH_Thread, osPriorityNormal, 0, 256};
	osMessageQDef_t ProfileFlash_osMessageQDef = {PROFILE_FLASH_MESSAGE_QUEUE_SIZE, sizeof(uint32_t)};
	
	if ((!ProfileFlash) || (!Profile))
		return -1;
	
	ProfileFlash->Profile = Profile;
	
	ProfileFlash->HeaderIdx = 0;
	ProfileFlash->CalibIdx = 0;
	ProfileFlash->ConfigIdx = 0;
	ProfileFlash->SaveEnable = 1;
	
	PROFILE_FLASH_InitHeaders(ProfileFlash);
	PROFILE_FLASH_InitCalibs(ProfileFlash);
	PROFILE_FLASH_InitConfigs(ProfileFlash);
	
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
	if (ProfileFlash->MessageQId && ProfileFlash->SaveEnable)
		osMessagePut(ProfileFlash->MessageQId, 0, 0);	
}

void PROFILE_FLASH_SaveConfigs(PROFILE_FLASH_T *ProfileFlash)
{
	/*Wake up Profile Save Thread*/
	if (ProfileFlash->MessageQId && ProfileFlash->SaveEnable)
		osMessagePut(ProfileFlash->MessageQId, 1, 0);	
}

void PROFILE_FLASH_SaveValue(PROFILE_FLASH_T *ProfileFlash, uint8_t Table, uint16_t Idx)
{
	switch (Table)
	{
		case PROFILE_TABLE_1:
			if (((ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table1[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				PROFILE_FLASH_SaveCalibs(ProfileFlash);
			PROFILE_FLASH_SaveConfigs(ProfileFlash);
			break;
		case PROFILE_TABLE_3:
			if (((ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) == PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE)) && 
				((ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Table3[Idx - 1].Access & PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)) != PROFILE_ACCESS(PROFILE_ACCESS_NONE, PROFILE_ACCESS_NONE, PROFILE_ACCESS_WRITE, PROFILE_ACCESS_NONE)))
				PROFILE_FLASH_SaveCalibs(ProfileFlash);
			PROFILE_FLASH_SaveConfigs(ProfileFlash);
			break;
		case PROFILE_TABLE_4:
			PROFILE_FLASH_SaveConfigs(ProfileFlash);
			break;
		case PROFILE_TABLE_103:
			PROFILE_FLASH_SaveConfigs(ProfileFlash);
			break;
	}
}

void PROFILE_FLASH_LangpackSave(PROFILE_FLASH_T *ProfileFlash, char *Block, uint32_t Index)
{
	uint32_t Addr;
	
	Addr = PROFILE_FLASH_LANG_ADDR(ProfileFlash->Profile->Idx) + Index * BSP_FLASH_PAGE_SIZE;
	if (BSP_FLASH_WaitReady())
		return;
	BSP_FLASH_ProgramPage(Addr, Block, BSP_FLASH_PAGE_SIZE);
}
void PROFILE_FLASH_LangpackLoad(PROFILE_FLASH_T *ProfileFlash, char *Block, uint32_t Index)
{
	uint32_t Addr;
	Addr = PROFILE_FLASH_LANG_ADDR(ProfileFlash->Profile->Idx) + Index * BSP_FLASH_PAGE_SIZE;
	if (BSP_FLASH_WaitReady())
		return;
	BSP_FLASH_Read(Addr, Block, BSP_FLASH_PAGE_SIZE);
}
void PROFILE_FLASH_LangpackClear(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t Addr;
	Addr = PROFILE_FLASH_LANG_ADDR(ProfileFlash->Profile->Idx);
	if (BSP_FLASH_WaitReady())
		return;
	BSP_FLASH_ErasePage(Addr);
}
