#include "http_server_files.h"
#include "log.h"
#include "crc.h"
#include <string.h>
#include <stdlib.h>

typedef struct {
	char Project[sizeof(BSP_PROJECT)];
	char HwVersion[sizeof(BSP_HW_VERSION)];
	uint16_t Crc16;
}	HTTP_SERVER_FILES_RECORD_HEADER_T;

static uint32_t HTTP_SERVER_FILES_RecordIdx;

static int32_t __HTTP_SERVER_FILES_ReadRecordHeader(uint32_t Idx)
{
	HTTP_SERVER_FILES_RECORD_HEADER_T Header;
	uint32_t Addr;
	uint16_t Crc16;
	
	Idx = Idx % HTTP_SERVER_FILES_RECORDS_CNT;
	Addr = HTTP_SERVER_FILES_RECORD_HEADER_ADDR(Idx);
	
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_Read(Addr, (char*)&Header, sizeof(Header)))
		return -1;
	
	if (memcmp(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT)))
		goto error;
	if (memcmp(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION)))
		goto error;

	Crc16 = Header.Crc16;
	Header.Crc16 = 0;
	if (Crc16 != Calc_CRC16((char*)&Header, sizeof(Header), 0))
		goto error;
	
	LOG_Info("HTTP_SERVER_FILES", "", "", "Read Files %d Header: Ok", Idx);
	return 0;
	
error:
	LOG_Warn("HTTP_SERVER_FILES", "", "", "Read Files %d Header: Error", Idx);
	return -1;
}

static int32_t __HTTP_SERVER_FILES_WriteRecordHeader(uint32_t Idx)
{
	HTTP_SERVER_FILES_RECORD_HEADER_T Header;
	uint32_t i, Addr;
	uint16_t Crc16;
	
	memset(&Header, 0, sizeof(Header));
	memcpy(Header.Project, BSP_PROJECT, sizeof(BSP_PROJECT));
	memcpy(Header.HwVersion, BSP_HW_VERSION, sizeof(BSP_HW_VERSION));
	Header.Crc16 = Calc_CRC16((char*)&Header, sizeof(Header), 0);
	
	Idx = Idx % HTTP_SERVER_FILES_RECORDS_CNT;
	Addr = HTTP_SERVER_FILES_RECORD_HEADER_ADDR(Idx);
	
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
	
	for (i = 0; i < HTTP_SERVER_FILES_RECORDS_CNT; i++)
	{
		if (i != Idx)
		{
			Addr = HTTP_SERVER_FILES_RECORD_HEADER_ADDR(i);
			if (BSP_FLASH_WaitReady())
				goto error;
			if (BSP_FLASH_ErasePage(Addr))
				goto error;
		}
	}
	
	LOG_Info("HTTP_SERVER_FILES", "",  "", "Write Files %d Header: Ok", Idx);
	return 0;

error:
	LOG_Error("HTTP_SERVER_FILES", "",  "", "Write Files %d Header: Error", Idx);
	return -1;
}

static int32_t __HTTP_SERVER_FILES_ReadRecordBody(uint32_t Idx, uint32_t DataAddr, char *Data, uint32_t DataSize)
{
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_Read(HTTP_SERVER_FILES_RECORD_BODY_ADDR(Idx) + DataAddr, Data, DataSize))
		return -1;
	return 0;
}

static int32_t __HTTP_SERVER_FILES_WriteRecordBody(uint32_t Idx, uint32_t DataAddr, char *Data, uint32_t DataSize)
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
		Idx = Idx % HTTP_SERVER_FILES_RECORDS_CNT;
		Addr = HTTP_SERVER_FILES_RECORD_BODY_ADDR(Idx) + (PageIdx * BSP_FLASH_PAGE_SIZE);
		if (PageAddr)
		{
			if (BSP_FLASH_WaitReady())
				goto error;
			if (BSP_FLASH_Read(Addr, Buffer, PageAddr))
				goto error;
			DataBlockSize = DataSize > (BSP_FLASH_PAGE_SIZE - PageAddr) ? BSP_FLASH_PAGE_SIZE - PageAddr : DataSize;
			memcpy(&Buffer[PageAddr], &Data[DataIdx], DataBlockSize);
			if (DataBlockSize == DataSize)
				memset(&Buffer[PageAddr + DataBlockSize], 0xFF, BSP_FLASH_PAGE_SIZE - PageAddr - DataBlockSize);
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
				memset(&Buffer[DataBlockSize], 0xFF, BSP_FLASH_PAGE_SIZE - DataBlockSize);
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
	LOG_Error("HTTP_SERVER_FILES", "",  "", "Write Files %d Addr %d Size %d: Error", Idx, DataAddr, DataSize);
	return -1;
}

void HTTP_SERVER_FILES_Init(void)
{
	uint32_t Idx;
	
	for (Idx = 0; Idx < HTTP_SERVER_FILES_RECORDS_CNT; Idx++)
	{
		if (__HTTP_SERVER_FILES_ReadRecordHeader(Idx))
			continue;
		HTTP_SERVER_FILES_RecordIdx = Idx;
		LOG_Info("HTTP_SERVER_FILES", "",  "", "Init Files %d: Ok", HTTP_SERVER_FILES_RecordIdx);
		return;
	}
	
	HTTP_SERVER_FILES_RecordIdx = 0;
	__HTTP_SERVER_FILES_WriteRecordHeader(0);
	LOG_Info("HTTP_SERVER_FILES", "",  "", "Init Files %d: Ok", HTTP_SERVER_FILES_RecordIdx);
}

int32_t HTTP_SERVER_FILES_FindByName(char *Name, uint32_t NameLen, uint32_t *DataAddr, uint32_t *DataSize, uint32_t *DataType)
{
	char Buffer[BSP_FLASH_PAGE_SIZE];
	uint32_t Addr;
	char *__Name, *__DataType, *__Size, *__Data;
	uint32_t __NameLen, __DataSize;
	
	Addr = HTTP_SERVER_FILES_RECORD_BODY_ADDR(HTTP_SERVER_FILES_RecordIdx);
	while(1) {
		if (BSP_FLASH_Read(Addr, Buffer, sizeof(Buffer)))
			goto error;
		Buffer[sizeof(Buffer) - 1] = 0;
		
		__Name = strstr(Buffer, "name: ");
		if (__Name)
			__Name += sizeof("name: ") - 1;
		
		__DataType = strstr(Buffer, "type: ");
		if (__DataType)
			__DataType += sizeof("type: ") - 1;
		
		__Size = strstr(Buffer, "size: ");
		if (__Size)
			__Size += sizeof("size: ") - 1;
		
		__Data = strstr(Buffer, "\r\n\r\n");
		if (__Data)
			__Data += sizeof("\r\n\r\n") - 1;
		
		if ((!__Name) || (!__DataType) || (!__Size) || (!__Data))
			goto error;
		
		__NameLen = strchr(__Name, '\r') - __Name;		
		__DataSize = strtoul(__Size, NULL, 10);
		
		if ((__NameLen == NameLen) && (!memcmp(Name, __Name, __NameLen)))
		{
			*DataAddr = Addr - HTTP_SERVER_FILES_RECORD_BODY_ADDR(HTTP_SERVER_FILES_RecordIdx) + __Data - Buffer;
			*DataSize = __DataSize;
			
			if (!memcmp(__DataType, "html", sizeof("html") - 1))
			{
				*DataType = HTTP_SERVER_FILES_DATA_TYPE_HTML;
			}
			else if (!memcmp(__DataType, "js", sizeof("js") - 1))
			{
				*DataType = HTTP_SERVER_FILES_DATA_TYPE_JS;
			}
			else if (!memcmp(__DataType, "css", sizeof("css") - 1))
			{
				*DataType = HTTP_SERVER_FILES_DATA_TYPE_CSS;
			}
			else if (!memcmp(__DataType, "woff2", sizeof("woff2") - 1))
			{
				*DataType = HTTP_SERVER_FILES_DATA_TYPE_WOFF2;
			}
			else if (!memcmp(__DataType, "json", sizeof("json") - 1))
			{
				*DataType = HTTP_SERVER_FILES_DATA_TYPE_JSON;
			}
			else
			{
				*DataType = HTTP_SERVER_FILES_DATA_TYPE_BIN;
			}
			return 0;			
		}		
		Addr += __DataSize + __Data - Buffer;		
	}
error:
	return -1;
}

int32_t HTTP_SERVER_FILES_ReadData(uint32_t DataAddr, char *Data, uint32_t DataSize)
{
	return __HTTP_SERVER_FILES_ReadRecordBody(HTTP_SERVER_FILES_RecordIdx % HTTP_SERVER_FILES_RECORDS_CNT, DataAddr, Data, DataSize);
}

int32_t HTTP_SERVER_FILES_WriteData(uint32_t DataAddr, char *Data, uint32_t DataSize)
{
	return __HTTP_SERVER_FILES_WriteRecordBody((HTTP_SERVER_FILES_RecordIdx + 1)% HTTP_SERVER_FILES_RECORDS_CNT, DataAddr, Data, DataSize);
}

int32_t HTTP_SERVER_FILES_WriteDataComplete(void)
{
	if (__HTTP_SERVER_FILES_WriteRecordHeader((HTTP_SERVER_FILES_RecordIdx + 1)% HTTP_SERVER_FILES_RECORDS_CNT))
		return -1;
	HTTP_SERVER_FILES_RecordIdx++;
	LOG_Info("HTTP_SERVER_FILES", "",  "", "Write Files %d Complete: Ok", HTTP_SERVER_FILES_RecordIdx % HTTP_SERVER_FILES_RECORDS_CNT);
	return 0;
}
