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

typedef struct {
	char Project[sizeof(BSP_PROJECT)];
	char HwVersion[sizeof(BSP_HW_VERSION)];
	uint32_t LangpackType;
	uint32_t LangpackBlocks;
	uint16_t Crc16;
}PROFILE_FLASH_LANGPACK_RECORD_HEADER_T;

int32_t PROFILE_FLASH_UpdateHeader(PROFILE_FLASH_T *ProfileFlash)
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
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	memset(&Header, 0, sizeof(Header));
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		goto error;
	
	if (Header.Idx == 0)
	{
		Addr = PROFILE_FLASH_HEADER_ADDR((Header.Idx + 1) % PROFILE_FLASH_HEADERS_CNT);
		if (BSP_FLASH_ErasePage(Addr))
			goto error;
	}
	
	LOG_Info("PROFILE_FLASH", "", "ProfileHeaderUpdate", "%d;%d;%d", Header.Idx % PROFILE_FLASH_HEADERS_CNT, Header.Idx, Header.ProfileIdx);
	ProfileFlash->HeaderIdx++;	
	return 0;

error:
	LOG_Error("PROFILE_FLASH", "", "ProfileHeaderUpdateError", "%d", Header.Idx % PROFILE_FLASH_HEADERS_CNT);
	return -1;
}

int32_t PROFILE_FLASH_ReadHeader(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *HeaderIdx, uint32_t *ProfileIdx)
{
	PROFILE_FLASH_HEADER_T Header;
	uint32_t Addr;
	uint16_t Crc16;
	
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

	LOG_Info("PROFILE_FLASH", "", "ProfileHeaderRead", "%d;%d;%d", Idx, *HeaderIdx, *ProfileIdx);
	return 0;
	
error:
	LOG_Warn("PROFILE_FLASH", "", "ProfileHeaderReadError", "%d", Idx);
	return -1;
}

int32_t PROFILE_FLASH_UpdateProfileIdx(PROFILE_FLASH_T *ProfileFlash, uint32_t ProfileIdx)
{
	ProfileFlash->Profile->Idx = ProfileIdx;
	return PROFILE_FLASH_UpdateHeader(ProfileFlash);	
}

int32_t PROFILE_FLASH_InitHeaders(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t i, HeaderReaded, HeaderIdx, HeaderIdxUpdate, HeaderProfileIdx, HeaderProfileIdxUpdate;
	
	HeaderReaded = 0;
	HeaderIdx = 0;
	HeaderProfileIdx = 0;
	HeaderIdxUpdate = 0;
	HeaderProfileIdxUpdate = 0;
	
	for (i = 0; i < PROFILE_FLASH_HEADERS_CNT; i++)
	{
		if (PROFILE_FLASH_ReadHeader(ProfileFlash, i, &HeaderIdx, &HeaderProfileIdx))
			continue;
		
		if (HeaderIdx >= HeaderIdxUpdate)
		{
			HeaderReaded = 1;
			HeaderIdxUpdate = HeaderIdx;
			HeaderProfileIdxUpdate = HeaderProfileIdx;
		}
	}
	
	if (HeaderReaded)
	{
		ProfileFlash->HeaderIdx = HeaderIdxUpdate + 1;
		ProfileFlash->Profile->Idx = HeaderProfileIdxUpdate;
		LOG_Info("PROFILE_FLASH", "", "ProfileInit", "%d;%s", ProfileFlash->Profile->Idx, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass);
		return 0;
	}
	
	ProfileFlash->HeaderIdx = 0;
	ProfileFlash->Profile->Idx = 0;
	LOG_Info("PROFILE_FLASH", "", "ProfileInit", "%d;%s", ProfileFlash->Profile->Idx, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->DeviceClass);
	return PROFILE_FLASH_UpdateHeader(ProfileFlash);
}

int32_t PROFILE_FLASH_CheckCalibsMap(PROFILE_FLASH_T *ProfileFlash, uint32_t *RestoreValues)
{
	PROFILE_FLASH_CALIB_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, Addr, Size, Pages, Len;
	uint16_t Crc16;
	
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
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->TablesCnt; i++)
	{
		if ((ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_1) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_3) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_4) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_6) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_7) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_103) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_104))
		{
			for (j = 0; j < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->ParamsCnt; j++)
			{
				if (!PROFILE_IS_CALIB_PARAM(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]))
					continue;
				Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name) + 1;
				if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
					goto error;
				if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name, Page, Len))
					goto mismatch;
				if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Type != Page[Len])
					goto mismatch;
				if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Table != Page[Len + 1])
					goto mismatch;
				Size += Len + 2;
			}
		}
	}
	
	if (Header.CalibsMapSize != Size)
		goto mismatch;
	
	*RestoreValues = 0;
	LOG_Info("PROFILE_FLASH", "", "CalibMapCheck", "");
	return 0;
	
error:
	*RestoreValues = 0;
	LOG_Warn("PROFILE_FLASH", "", "CalibMapCheckError", "");
	return -1;
	
mismatch:
	*RestoreValues = 1;
	LOG_Warn("PROFILE_FLASH", "", "CalibMapCheckMismatch", "");
	return -1;
}

int32_t PROFILE_FLASH_UpdateCalibsMap(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CALIB_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, k, Addr, Size, Len, PageIdx;
	uint16_t Crc16;
	
	Addr = PROFILE_FLASH_CALIB_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
	Size = 0;
	Crc16 = 0;
	PageIdx = 0;
	
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->TablesCnt; i++)
	{
		if ((ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_1) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_3) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_4) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_6) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_7) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_103) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_104))
		{
			for (j = 0; j < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->ParamsCnt; j++)
			{
				if (!PROFILE_IS_CALIB_PARAM(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]))
					continue;
				Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name) + 1;
				for (k = 0; k < (Len + 2); k++)
				{
					if (k == Len)
						Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Type;
					else if (k == (Len + 1))
						Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Table;
					else
						Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name[k];
					if (PageIdx == BSP_FLASH_PAGE_SIZE)
					{
						if (BSP_FLASH_ProgramPage(Addr + Size, Page, PageIdx))
							goto error;
						Size += PageIdx;
						Crc16 = Calc_CRC16(Page, PageIdx, Crc16);
						PageIdx = 0;
					}
				}
			}
		}
	}
	
	if (PageIdx)
	{
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
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	LOG_Info("PROFILE_FLASH", "", "CalibMapUpdate", "%d", Size);
	return -1;
	
error:
	LOG_Error("PROFILE_FLASH", "", "CalibMapUpdate", "Error");
	return -1;	
}

int32_t PROFILE_FLASH_GetCalibsValuesIdx(PROFILE_FLASH_T *ProfileFlash, uint32_t *CalibsValuesIdx)
{
	PROFILE_FLASH_CALIB_RECORD_HEADER_T CalibRecordHeader;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, Addr, Pages, Size;
	uint32_t CalibsValuesIdxUpdate, CalibsValuesIdxFounded;
	uint16_t Crc16;
	
	CalibsValuesIdxUpdate = 0;
	CalibsValuesIdxFounded = 0;
	
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
		
		if (CalibRecordHeader.Idx >= CalibsValuesIdxUpdate)
		{
			CalibsValuesIdxFounded = 1;
			CalibsValuesIdxUpdate = CalibRecordHeader.Idx;			
		}
	}
	
	if (CalibsValuesIdxFounded)
	{
		*CalibsValuesIdx = CalibsValuesIdxUpdate;
		LOG_Info("PROFILE_FLASH", "", "CalibValuesIdxGet", "%d", *CalibsValuesIdx);
		return 0;
	}
	
error:	
	LOG_Warn("PROFILE_FLASH", "", "CalibValuesIdxGet", "Error");
	return -1;
}

void PROFILE_FLASH_RestoreCalibsValues(PROFILE_FLASH_T *ProfileFlash, uint32_t CalibsValuesIdx)
{
	PROFILE_FLASH_CALIB_MAP_HEADER_T CalibMapHeader;
	uint32_t i, j, Len, MapAddr, ValAddr, RestoredParamsCnt;
	char Map[BSP_FLASH_PAGE_SIZE];
	char Value[PROFILE_STRING_MAX_LEN];
	
	RestoredParamsCnt = 0;
	
	MapAddr = PROFILE_FLASH_CALIB_MAP_HEADER_ADDR(ProfileFlash->Profile->Idx);
	if (BSP_FLASH_Read(MapAddr, (char*)&CalibMapHeader, sizeof(CalibMapHeader)))
		goto exit;
	
	MapAddr = PROFILE_FLASH_CALIB_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
	ValAddr = PROFILE_FLASH_CALIB_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, CalibsValuesIdx % PROFILE_FLASH_CALIB_RECORDS_CNT);
	
	while (MapAddr < (PROFILE_FLASH_CALIB_MAP_BODY_ADDR(ProfileFlash->Profile->Idx) + CalibMapHeader.CalibsMapSize)) {
		if (BSP_FLASH_Read(MapAddr, Map, sizeof(Map)))
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
		
		MapAddr += Len + 2;
		
		if (Map[Len + 1] != PROFILE_TABLE_2)
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
					/*==========================================
					  Fix data alignment for unpacked structures
					  ==========================================*/
					if (ValAddr % 4)
						ValAddr += 4 - (ValAddr % 4);
					/*==========================================*/
					if (BSP_FLASH_Read(ValAddr, Value, 4))
						goto exit;
					ValAddr += 4;
					break;
				case PROFILE_TYPE_UINT64:
					/*==========================================
					  Fix data alignment for unpacked structures
					  ==========================================*/
					if (ValAddr % 8)
						ValAddr += 8 - (ValAddr % 8);
					/*==========================================*/
					if (BSP_FLASH_Read(ValAddr, Value, 8))
						goto exit;
					ValAddr += 8;
					break;
				case PROFILE_TYPE_OID:
				case PROFILE_TYPE_STRING:
					if (BSP_FLASH_Read(ValAddr, Value, PROFILE_STRING_MAX_LEN))
						goto exit;
					ValAddr += PROFILE_STRING_MAX_LEN;
					break;
				default:
					goto exit;
			}
		}
		else
		{
			if (BSP_FLASH_Read(ValAddr, Value, 2))
				goto exit;
			ValAddr += 2;
		}
		
		
		if ((Map[Len+1] == PROFILE_TABLE_1) ||
			(Map[Len+1] == PROFILE_TABLE_3) ||
			(Map[Len+1] == PROFILE_TABLE_4) ||
			(Map[Len+1] == PROFILE_TABLE_6) ||
			(Map[Len+1] == PROFILE_TABLE_7) ||
			(Map[Len+1] == PROFILE_TABLE_103) ||
			(Map[Len+1] == PROFILE_TABLE_104))
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->TablesCnt; i++)
			{
				if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == Map[Len+1])
				{
					for (j = 0; j < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->ParamsCnt; j++)
					{
						if (strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name))
							continue;
						if (!PROFILE_IS_CALIB_PARAM(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]))
							break;
						if (Map[Len] != ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Type)
							break;
						if (PROFILE_CheckValue(ProfileFlash->Profile, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index, j + 1, Value, 1))
							break;
						if (PROFILE_SetValue(ProfileFlash->Profile, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index, j + 1, Value))
							break;
						PROFILE_SetValueDependencies(ProfileFlash->Profile, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index, j + 1, Value, 1);
						RestoredParamsCnt++;
					}
					break;
				}
			}
		}
	}

exit:
	LOG_Info("PROFILE_FLASH", "", "CalibValuesRestore", "%d", RestoredParamsCnt);
}

static int32_t PROFILE_FLASH_ReadCalibsRecord(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *RecordIdx)
{
	PROFILE_FLASH_CALIB_RECORD_HEADER_T CalibRecordHeader;
	uint32_t Addr;
	uint16_t Crc16;
	
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
	LOG_Info("PROFILE_FLASH", "", "CalibValuesRead", "%d;%d", Idx, *RecordIdx);
	return 0;

release_and_error:
	osMutexRelease( ProfileFlash->Profile->Values->Mutex );
error:	
	LOG_Warn("PROFILE_FLASH", "", "CalibValuesReadError", "%d", Idx);
	return -1;
}

int32_t PROFILE_FLASH_UpdateCalibsValues(PROFILE_FLASH_T *ProfileFlash)
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
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	memset(&Header, 0, sizeof(Header));
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
		osMutexWait(ProfileFlash->Profile->Values->Mutex, osWaitForever);
		if (BSP_FLASH_ProgramPage(Addr, p, Size))
		{
			osMutexRelease( ProfileFlash->Profile->Values->Mutex );
			goto error;
		}
		osMutexRelease( ProfileFlash->Profile->Values->Mutex );
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
			if (BSP_FLASH_ErasePage(Addr))
				goto error;
		}
	}
	
	LOG_Info("PROFILE_FLASH", "", "CalibValuesUpdate", "%d;%d", ProfileFlash->CalibIdx, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize);
	ProfileFlash->CalibIdx++;
	return 0;
	
error:
	LOG_Error("PROFILE_FLASH", "", "CalibValuesUpdateError", "%d", ProfileFlash->CalibIdx);
	return -1;	
}

int32_t PROFILE_FLASH_InitCalibsValues(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t i, CalibsValuesIdx, CalibsValuesIdxUpdate, CalibsValuesReaded;
	
	CalibsValuesIdx = 0;
	CalibsValuesIdxUpdate = 0;
	CalibsValuesReaded = 0;
	
	for (i = 0; i < PROFILE_FLASH_CALIB_RECORDS_CNT; i++)
	{
		if (PROFILE_FLASH_ReadCalibsRecord(ProfileFlash, i, &CalibsValuesIdx))
			continue;		
		if (CalibsValuesIdx >= CalibsValuesIdxUpdate)
		{
			CalibsValuesReaded = 1;
			CalibsValuesIdxUpdate = CalibsValuesIdx;
		}
	}
	if (CalibsValuesReaded)
	{
		ProfileFlash->CalibIdx = CalibsValuesIdxUpdate + 1;
		if (PROFILE_FLASH_ReadCalibsRecord(ProfileFlash, CalibsValuesIdxUpdate % PROFILE_FLASH_CALIB_RECORDS_CNT, &CalibsValuesIdxUpdate))
			goto error;
		LOG_Info("PROFILE_FLASH", "", "CalibValuesInit", "%d", ProfileFlash->CalibIdx);
		return 0;
	}

error:
	ProfileFlash->CalibIdx = 0;
	memcpy(ProfileFlash->Profile->Values->Calib, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibDefaultValues, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->CalibValuesSize);
	LOG_Info("PROFILE_FLASH", "", "CalibValuesInit", "%d", ProfileFlash->CalibIdx);
	return PROFILE_FLASH_UpdateCalibsValues(ProfileFlash);
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
		PROFILE_FLASH_UpdateCalibsMap(ProfileFlash);		
		PROFILE_FLASH_UpdateCalibsValues(ProfileFlash);
	}
	
	return PROFILE_FLASH_InitCalibsValues(ProfileFlash);
}

int32_t PROFILE_FLASH_CheckConfigsMap(PROFILE_FLASH_T *ProfileFlash, uint32_t *RestoreValues)
{
	PROFILE_FLASH_CONFIG_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, Addr, Size, Pages, Len;
	uint16_t Crc16;
	
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
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->TablesCnt; i++)
	{
		if ((ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_1) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_3) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_4) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_6) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_7) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_103) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_104))
		{
			for (j = 0; j < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->ParamsCnt; j++)
			{
				if (PROFILE_IS_CALIB_PARAM(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]))
					continue;
				Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name) + 1;
				if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
					goto error;
				if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name, Page, Len))
					goto mismatch;
				if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Type != Page[Len])
					goto mismatch;
				if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Table != Page[Len + 1])
					goto mismatch;
				Size += Len + 2;
			}
		}
		else if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_2)
		{
			for (j = 0; j < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->ParamsCnt; j++)
			{
				if (!((PROFILE_TABLE2_PARAM_T*)(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]))->DegradeColor)
					continue;
				Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name) + 1;
				if (BSP_FLASH_Read(Addr + Size, Page, Len + 2))
					goto error;
				if (memcmp(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name, Page, Len))
					goto mismatch;
				if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Type != Page[Len])
					goto mismatch;
				if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Table != Page[Len + 1])
					goto mismatch;
				Size += Len + 2;
			}
		}
	}
	
	if (Header.ConfigsMapSize != Size)
		goto mismatch;
	
	*RestoreValues = 0;
	LOG_Info("PROFILE_FLASH", "", "ConfigMapCheck", "");
	return 0;
	
error:
	*RestoreValues = 0;
	LOG_Warn("PROFILE_FLASH", "", "ConfigMapCheckError", "");
	return -1;
	
mismatch:
	*RestoreValues = 1;
	LOG_Warn("PROFILE_FLASH", "", "ConfigMapCheckMismatch", "");
	return -1;
}

int32_t PROFILE_FLASH_UpdateConfigsMap(PROFILE_FLASH_T *ProfileFlash)
{
	PROFILE_FLASH_CONFIG_MAP_HEADER_T Header;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, k, Addr, Size, Len, PageIdx;
	uint16_t Crc16;
	
	Addr = PROFILE_FLASH_CONFIG_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
	Size = 0;
	Crc16 = 0;
	PageIdx = 0;
	
	for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->TablesCnt; i++)
	{
		if ((ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_1) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_3) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_4) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_6) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_7) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_103) ||
			(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_104))
		{
			for (j = 0; j < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->ParamsCnt; j++)
			{
				if (PROFILE_IS_CALIB_PARAM(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]))
					continue;
				Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name) + 1;
				for (k = 0; k < (Len + 2); k++)
				{
					if (k == Len)
						Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Type;
					else if (k == (Len + 1))
						Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Table;
					else
						Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name[k];
					if (PageIdx == BSP_FLASH_PAGE_SIZE)
					{
						if (BSP_FLASH_ProgramPage(Addr + Size, Page, PageIdx))
							goto error;
						Size += PageIdx;
						Crc16 = Calc_CRC16(Page, PageIdx, Crc16);
						PageIdx = 0;
					}
				}
			}
		}
		else if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == PROFILE_TABLE_2)
		{
			for (j = 0; j < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->ParamsCnt; j++)
			{
				if (!((PROFILE_TABLE2_PARAM_T*)(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]))->DegradeColor)
					continue;
				Len = strlen(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name) + 1;
				for (k = 0; k < (Len + 2); k++)
				{
					if (k == Len)
						Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Type;
					else if (k == (Len + 1))
						Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Table;
					else
						Page[PageIdx++] = ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name[k];
					if (PageIdx == BSP_FLASH_PAGE_SIZE)
					{
						if (BSP_FLASH_ProgramPage(Addr + Size, Page, PageIdx))
							goto error;
						Size += PageIdx;
						Crc16 = Calc_CRC16(Page, PageIdx, Crc16);
						PageIdx = 0;
					}
				}
			}
		}
	}
	if (PageIdx)
	{
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
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	LOG_Info("PROFILE_FLASH", "", "ConfigMapUpdate", "%d", Size);
	return -1;
	
error:
	LOG_Info("PROFILE_FLASH", "", "ConfigMapUpdate", "Error");
	return -1;	
}

int32_t PROFILE_FLASH_GetConfigsValuesIdx(PROFILE_FLASH_T *ProfileFlash, uint32_t *ConfigsValuesIdx)
{
	PROFILE_FLASH_CONFIG_RECORD_HEADER_T ConfigRecordHeader;
	char Page[BSP_FLASH_PAGE_SIZE];
	uint32_t i, j, Addr, Pages, Size;
	uint32_t ConfigsValuesIdxUpdate, ConfigsValuesIdxFounded;
	uint16_t Crc16;
	
	ConfigsValuesIdxUpdate = 0;
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
		
		if (ConfigRecordHeader.Idx >= ConfigsValuesIdxUpdate)
		{
			ConfigsValuesIdxFounded = 1;
			ConfigsValuesIdxUpdate = ConfigRecordHeader.Idx;			
		}
	}
	
	if (ConfigsValuesIdxFounded)
	{
		*ConfigsValuesIdx = ConfigsValuesIdxUpdate;
		LOG_Info("PROFILE_FLASH", "", "ConfigValuesGet", "%d", *ConfigsValuesIdx);
		return 0;
	}
	
error:	
	LOG_Warn("PROFILE_FLASH", "", "ConfigValuesGet", "Error");
	return -1;
}

void PROFILE_FLASH_RestoreConfigsValues(PROFILE_FLASH_T *ProfileFlash, uint32_t ConfigsValuesIdx)
{
	PROFILE_FLASH_CONFIG_MAP_HEADER_T ConfigMapHeader;
	uint32_t i, j, Len, MapAddr, ValAddr, RestoredParamsCnt;
	char Map[BSP_FLASH_PAGE_SIZE];
	char Value[PROFILE_STRING_MAX_LEN];
	
	RestoredParamsCnt = 0;
	
	MapAddr = PROFILE_FLASH_CONFIG_MAP_HEADER_ADDR(ProfileFlash->Profile->Idx);
	if (BSP_FLASH_Read(MapAddr, (char*)&ConfigMapHeader, sizeof(ConfigMapHeader)))
		goto exit;
	
	MapAddr = PROFILE_FLASH_CONFIG_MAP_BODY_ADDR(ProfileFlash->Profile->Idx);
	ValAddr = PROFILE_FLASH_CONFIG_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, ConfigsValuesIdx % PROFILE_FLASH_CONFIG_RECORDS_CNT);
	
	while (MapAddr < (PROFILE_FLASH_CONFIG_MAP_BODY_ADDR(ProfileFlash->Profile->Idx) + ConfigMapHeader.ConfigsMapSize)) {
		if (BSP_FLASH_Read(MapAddr, Map, sizeof(Map)))
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
		
		MapAddr += Len + 2;
		
		if (Map[Len + 1] != PROFILE_TABLE_2)
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
					/*==========================================
					  Fix data alignment for unpacked structures
					  ==========================================*/
					if (ValAddr % 4)
						ValAddr += 4 - (ValAddr % 4);
					/*==========================================*/
					if (BSP_FLASH_Read(ValAddr, Value, 4))
						goto exit;
					ValAddr += 4;
					break;
				case PROFILE_TYPE_UINT64:
					/*==========================================
					  Fix data alignment for unpacked structures
					  ==========================================*/
					if (ValAddr % 8)
						ValAddr += 8 - (ValAddr % 8);
					/*==========================================*/
					if (BSP_FLASH_Read(ValAddr, Value, 8))
						goto exit;
					ValAddr += 8;
					break;
				case PROFILE_TYPE_OID:
				case PROFILE_TYPE_STRING:
					if (BSP_FLASH_Read(ValAddr, Value, PROFILE_STRING_MAX_LEN))
						goto exit;
					ValAddr += PROFILE_STRING_MAX_LEN;
					break;
				default:
					goto exit;
			}
		}
		else
		{
			if (BSP_FLASH_Read(ValAddr, Value, 2))
				goto exit;
			ValAddr += 2;
		}
		
		if ((Map[Len+1] == PROFILE_TABLE_1) ||
			(Map[Len+1] == PROFILE_TABLE_3) ||
			(Map[Len+1] == PROFILE_TABLE_4) ||
			(Map[Len+1] == PROFILE_TABLE_6) ||
			(Map[Len+1] == PROFILE_TABLE_7) ||
			(Map[Len+1] == PROFILE_TABLE_103) ||
			(Map[Len+1] == PROFILE_TABLE_104))
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->TablesCnt; i++)
			{
				if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == Map[Len+1])
				{
					for (j = 0; j < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->ParamsCnt; j++)
					{
						if (strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name))
							continue;
						if (PROFILE_IS_CALIB_PARAM(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]))
							break;
						if (Map[Len] != ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Type)
							break;
						if (PROFILE_CheckValue(ProfileFlash->Profile, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index, j + 1, Value, 1))
							break;
						if (PROFILE_SetValue(ProfileFlash->Profile, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index, j + 1, Value))
							break;
						PROFILE_SetValueDependencies(ProfileFlash->Profile, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index, j + 1, Value, 1);
						RestoredParamsCnt++;
					}
					break;
				}
			}
		}
		else if (Map[Len+1] == PROFILE_TABLE_2)
		{
			for (i = 0; i < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->TablesCnt; i++)
			{
				if (ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index == Map[Len+1])
				{
					for (j = 0; j < ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->ParamsCnt; j++)
					{
						if (strcmp(Map, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Name))
							continue;
						if (!((PROFILE_TABLE2_PARAM_T*)(ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]))->DegradeColor)
							break;
						if (Map[Len] != ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Params[j]->Type)
							break;
						PROFILE_SetAlarmColors(ProfileFlash->Profile, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->Tables[i]->Index, j + 1, Value[0], Value[1]);
						RestoredParamsCnt++;
					}
					break;
				}
			}
		}
	}

exit:
	LOG_Info("PROFILE_FLASH", "", "ConfigValuesRestore", "%d", RestoredParamsCnt);
}

static int32_t PROFILE_FLASH_ReadConfigsRecord(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *RecordIdx)
{
	PROFILE_FLASH_CONFIG_RECORD_HEADER_T ConfigRecordHeader;
	uint32_t Addr;
	uint16_t Crc16;
	
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
	LOG_Info("PROFILE_FLASH", "", "ConfigValuesRead", "%d;%d", Idx, *RecordIdx);
	return 0;

release_and_error:
	osMutexRelease( ProfileFlash->Profile->Values->Mutex );
error:	
	LOG_Warn("PROFILE_FLASH", "", "ConfigValuesRead", "Error:%d", Idx);
	return -1;
}

int32_t PROFILE_FLASH_UpdateConfigsValues(PROFILE_FLASH_T *ProfileFlash)
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
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	memset(&Header, 0, sizeof(Header));
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
		osMutexWait(ProfileFlash->Profile->Values->Mutex, osWaitForever);
		if (BSP_FLASH_ProgramPage(Addr, p, Size))
		{
			osMutexRelease( ProfileFlash->Profile->Values->Mutex );
			goto error;
		}
		osMutexRelease( ProfileFlash->Profile->Values->Mutex );
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
			if (BSP_FLASH_ErasePage(Addr))
				goto error;
		}
	}
	
	LOG_Info("PROFILE_FLASH", "", "ConfigValuesUpdate", "%d;%d", ProfileFlash->ConfigIdx, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize);
	ProfileFlash->ConfigIdx++;
	return 0;
	
error:
	LOG_Error("PROFILE_FLASH", "", "ConfigValuesUpdateError", "%d", ProfileFlash->ConfigIdx);
	return -1;	
}

int32_t PROFILE_FLASH_InitConfigsValues(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t i, ConfigsValuesIdx, ConfigsValuesIdxUpdate, ConfigsValuesReaded;
	
	ConfigsValuesIdx = 0;
	ConfigsValuesIdxUpdate = 0;
	ConfigsValuesReaded = 0;
	
	for (i = 0; i < PROFILE_FLASH_CONFIG_RECORDS_CNT; i++)
	{
		if (PROFILE_FLASH_ReadConfigsRecord(ProfileFlash, i, &ConfigsValuesIdx))
			continue;		
		if (ConfigsValuesIdx >= ConfigsValuesIdxUpdate)
		{
			ConfigsValuesReaded = 1;
			ConfigsValuesIdxUpdate = ConfigsValuesIdx;
		}
	}
	if (ConfigsValuesReaded)
	{
		ProfileFlash->ConfigIdx = ConfigsValuesIdxUpdate + 1;
		if (PROFILE_FLASH_ReadConfigsRecord(ProfileFlash, ConfigsValuesIdxUpdate % PROFILE_FLASH_CONFIG_RECORDS_CNT, &ConfigsValuesIdxUpdate))
			goto error;
		LOG_Info("PROFILE_FLASH", "", "ConfigValuesInit", "%d", ProfileFlash->ConfigIdx);
		return 0;
	}

error:
	ProfileFlash->ConfigIdx = 0;
	memcpy(ProfileFlash->Profile->Values->Static, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticDefaultValues, ProfileFlash->Profile->Descr[ProfileFlash->Profile->Idx]->StaticValuesSize);
	LOG_Info("PROFILE_FLASH", "", "ConfigValuesInit", "%d", ProfileFlash->ConfigIdx);
	return PROFILE_FLASH_UpdateConfigsValues(ProfileFlash);
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
		PROFILE_FLASH_UpdateConfigsMap(ProfileFlash);		
		PROFILE_FLASH_UpdateConfigsValues(ProfileFlash);
	}
	
	return PROFILE_FLASH_InitConfigsValues(ProfileFlash);
}

int32_t __PROFILE_FLASH_ReadLangpackHeader(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t *LangpackBlocks, uint32_t *LangpackType)
{
	PROFILE_FLASH_LANGPACK_RECORD_HEADER_T Header;
	uint32_t Addr;
	uint16_t Crc16;
	
	Idx = Idx % PROFILE_FLASH_LANG_RECORDS_CNT;
	Addr = PROFILE_FLASH_LANG_RECORD_HEADER_ADDR(ProfileFlash->Profile->Idx, Idx);
	
	memset(&Header, 0, sizeof(Header));
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

	return 0;
	
error:
	LOG_Warn("PROFILE_FLASH", "", "LangpackHeaderReadError", "%d", Idx);
	return -1;
}

int32_t __PROFILE_FLASH_UpdateLangpackHeader(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t LangpackBlocks, uint32_t LangpackType)
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
	Addr = PROFILE_FLASH_LANG_RECORD_HEADER_ADDR(ProfileFlash->Profile->Idx, Idx);
	if (BSP_FLASH_ProgramPage(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	
	memset(&Header, 0, sizeof(Header));
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		goto error;
	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		goto error;
	
	for (i = 0; i < PROFILE_FLASH_HEADERS_CNT; i++)
	{
		if (i != Idx)
		{
			Addr = PROFILE_FLASH_LANG_RECORD_HEADER_ADDR(ProfileFlash->Profile->Idx, i);
			if (BSP_FLASH_ErasePage(Addr))
				goto error;
		}
	}
	
	LOG_Info("PROFILE_FLASH", "", "LangpackHeaderUpdate", "%d;%d;%d", Idx, Header.LangpackBlocks, Header.LangpackType);
	return 0;

error:
	LOG_Error("PROFILE_FLASH", "", "LangpackHeaderUpdateError", "%d;%d;%d", Idx, LangpackBlocks, LangpackType);
	return -1;
}

int32_t __PROFILE_FLASH_ReadLangpackData(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t DataAddr, char *Data, uint32_t DataSize)
{
	uint32_t Addr;
	
	Idx = Idx % PROFILE_FLASH_LANG_RECORDS_CNT;
	Addr = PROFILE_FLASH_LANG_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, Idx) + DataAddr;
	if (BSP_FLASH_Read(Addr, Data, DataSize))
		goto error;
	return 0;
	
error:
	LOG_Error("PROFILE_FLASH", "", "ReadLangpackError", "%d;%d;%d", Idx, DataAddr, DataSize);
	return -1;
}

int32_t __PROFILE_FLASH_UpdateLangpackData(PROFILE_FLASH_T *ProfileFlash, uint32_t Idx, uint32_t DataAddr, char *Data, uint32_t DataSize)
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
		Addr = PROFILE_FLASH_LANG_RECORD_BODY_ADDR(ProfileFlash->Profile->Idx, Idx) + (PageIdx * BSP_FLASH_PAGE_SIZE);
		if (PageAddr)
		{
			if (BSP_FLASH_Read(Addr, Buffer, PageAddr))
				goto error;
			DataBlockSize = DataSize > (BSP_FLASH_PAGE_SIZE - PageAddr) ? BSP_FLASH_PAGE_SIZE - PageAddr : DataSize;
			memcpy(&Buffer[PageAddr], &Data[DataIdx], DataBlockSize);
			if (DataBlockSize == DataSize)
				memset(&Buffer[PageAddr + DataBlockSize], 0x03, BSP_FLASH_PAGE_SIZE - PageAddr - DataBlockSize);
			if (BSP_FLASH_ProgramPage(Addr, Buffer, BSP_FLASH_PAGE_SIZE))
				goto error;
			DataIdx += DataBlockSize;
		}
		else
		{
			if ((DataSize - DataIdx) >= BSP_FLASH_PAGE_SIZE)
			{
				if (BSP_FLASH_ProgramPage(Addr, &Data[DataIdx], BSP_FLASH_PAGE_SIZE))
					goto error;
				DataIdx += BSP_FLASH_PAGE_SIZE;
			}
			else
			{
				DataBlockSize = DataSize - DataIdx;
				memcpy(&Buffer, &Data[DataIdx], DataBlockSize);
				memset(&Buffer[DataBlockSize], 0x03, BSP_FLASH_PAGE_SIZE - DataBlockSize);
				if (BSP_FLASH_ProgramPage(Addr, Buffer, BSP_FLASH_PAGE_SIZE))
					goto error;
				DataIdx += DataBlockSize;
			}
		}
	}
	return 0;
	
error:
	LOG_Error("PROFILE_FLASH", "", "UpdateLangpackError", "%d;%d;%d", Idx, DataAddr, DataSize);
	return -1;
}

int32_t PROFILE_FLASH_InitLangpacks(PROFILE_FLASH_T *ProfileFlash)
{
	uint32_t Idx, LangpackBlocks, LangpackType;
	
	for (Idx = 0; Idx < PROFILE_FLASH_LANG_RECORDS_CNT; Idx++)
	{
		if (__PROFILE_FLASH_ReadLangpackHeader(ProfileFlash, Idx, &LangpackBlocks, &LangpackType))
			continue;
		ProfileFlash->LangpackIdx = Idx;
		LOG_Info("PROFILE_FLASH", "", "LangpackInit", "%d;%d;%d", Idx, LangpackBlocks, LangpackType);
		return 0;
	}

	ProfileFlash->LangpackIdx = 0;
	__PROFILE_FLASH_UpdateLangpackHeader(ProfileFlash, 0, 0, 1);
	LOG_Info("PROFILE_FLASH", "", "LangpackInit", "0;0;1");
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
		/*Update Profile to Flash*/
repeat:
		if (Event.value.v == 0)
		{
			if (PROFILE_FLASH_UpdateCalibsValues(ProfileFlash))
				goto error;
			continue;
		}
		else if (Event.value.v == 1)
		{
			if (PROFILE_FLASH_UpdateConfigsValues(ProfileFlash))
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
	ProfileFlash->LangpackIdx = 0;
	
	PROFILE_FLASH_InitHeaders(ProfileFlash);
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

void PROFILE_FLASH_UpdateCalibs(PROFILE_FLASH_T *ProfileFlash)
{
	/*Wake up Profile Update Thread*/
	if (ProfileFlash->MessageQId)
		osMessagePut(ProfileFlash->MessageQId, 0, 0);	
}

void PROFILE_FLASH_UpdateConfigs(PROFILE_FLASH_T *ProfileFlash)
{
	/*Wake up Profile Update Thread*/
	if (ProfileFlash->MessageQId)
		osMessagePut(ProfileFlash->MessageQId, 1, 0);	
}

void PROFILE_FLASH_UpdateValue(PROFILE_FLASH_T *ProfileFlash, uint8_t Table, uint16_t Idx)
{
	PROFILE_TABLE_PARAM_T *Param;
	switch (Table)
	{
		case PROFILE_TABLE_1:
		case PROFILE_TABLE_3:
		case PROFILE_TABLE_4:
		case PROFILE_TABLE_6:
		case PROFILE_TABLE_7:
		case PROFILE_TABLE_103:
		case PROFILE_TABLE_104:
			if (PROFILE_GetParam(ProfileFlash->Profile, Table, Idx, &Param))
				return;
			if (PROFILE_IS_CALIB_PARAM(Param))
				PROFILE_FLASH_UpdateCalibs(ProfileFlash);
			PROFILE_FLASH_UpdateConfigs(ProfileFlash);
			break;
	}
}

int32_t PROFILE_FLASH_ReadLangpackHeader(PROFILE_FLASH_T *ProfileFlash, uint32_t *LangpackBlocks, uint32_t *LangpackType)
{
	return __PROFILE_FLASH_ReadLangpackHeader(ProfileFlash, ProfileFlash->LangpackIdx % PROFILE_FLASH_LANG_RECORDS_CNT, LangpackBlocks, LangpackType);
}

int32_t PROFILE_FLASH_UpdateLangpackHeader(PROFILE_FLASH_T *ProfileFlash, uint32_t LangpackBlocks, uint32_t LangpackType)
{
	if (__PROFILE_FLASH_UpdateLangpackHeader(ProfileFlash, (ProfileFlash->LangpackIdx + 1) % PROFILE_FLASH_LANG_RECORDS_CNT, LangpackBlocks, LangpackType))
		return -1;
	ProfileFlash->LangpackIdx++;
	return 0;
}

int32_t PROFILE_FLASH_ReadLangpackData(PROFILE_FLASH_T *ProfileFlash, uint32_t DataAddr, char *Data, uint32_t DataSize)
{
	return __PROFILE_FLASH_ReadLangpackData(ProfileFlash, ProfileFlash->LangpackIdx % PROFILE_FLASH_LANG_RECORDS_CNT, DataAddr, Data, DataSize);
}

int32_t PROFILE_FLASH_UpdateLangpackData(PROFILE_FLASH_T *ProfileFlash, uint32_t DataAddr, char *Data, uint32_t DataSize)
{
	return __PROFILE_FLASH_UpdateLangpackData(ProfileFlash, (ProfileFlash->LangpackIdx + 1) % PROFILE_FLASH_LANG_RECORDS_CNT, DataAddr, Data, DataSize);
}
