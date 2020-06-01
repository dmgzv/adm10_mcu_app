#include "bootloader_protocol.h"
#include "string_fnc.h"
#include "crc.h"
#include	<string.h>

#define BOOTLOADER_PROTOCOL_DEVICE_CLASS	"btldr6"

static uint32_t BOOTLOADER_PROTOCOL_GetDeviceClass(char *cmd, char *rsp)
{
	memcpy(rsp, cmd, 2);
	strcpy(rsp + 2, BOOTLOADER_PROTOCOL_DEVICE_CLASS"\r");
	return 2 + sizeof(BOOTLOADER_PROTOCOL_DEVICE_CLASS) + 1;	
}

static uint32_t BOOTLOADER_PROTOCOL_FwConfig(char *cmd, char *rsp)
{
	char * str;
	memcpy(rsp, cmd, 2);
	str = rsp + 2;
	strcpy( str, "FWCONFIG;HEXFWLOADER;FWSTARTADDR:" );
	str += strlen( str );
	str = UintToHexStr( str, BOOTLOADER_FW_ADDR );//адрес начала основной программы
	strcpy( str, ";FWRESSIZE:" );
	str += strlen( str );
	str = UintToHexStr( str, BOOTLOADER_FW_SIZE );//размер основной программы
	strcpy( str, ";LANGRESSIZE:" );
	str += strlen( str );
	str = UintToHexStr( str, BSP_FLASH_LANG_SIZE );//размер основной программы
	*str++ = ';';
	str = WordToHexStr( str, Calc_CRC16( rsp, str - rsp, 0 ) );//Контрольная сумма и OD
	*str++ = '\r';
	*str = 0;
	return (uint32_t)( str - rsp );
}

static uint32_t BOOTLOADER_PROTOCOL_FwAddr( char *cmd, char *rsp, uint32_t Addr )
{
	char * str;
	memcpy(rsp, cmd, 2);
	str = rsp + 2;
	UintToHexStr( str, Addr );
	str += strlen( str );
	str = WordToHexStr( str, Calc_CRC16( rsp, str - rsp, 0 ) );//Контрольная сумма и OD
	*str++ = '\r';
	*str = 0;
	return (uint32_t)( str - rsp );
}

static uint32_t BOOTLOADER_PROTOCOL_FwStr( char *cmd, char *rsp, char *fw_str )
{
	char * str;
	memcpy(rsp, cmd, 2);
	str = rsp + 2;
	strcpy(str, fw_str);
	str += strlen( str );
	str = WordToHexStr( str, Calc_CRC16( rsp, str - rsp, 0 ) );//Контрольная сумма и OD
	*str++ = '\r';
	*str = 0;
	return (uint32_t)( str - rsp );
}

uint32_t BOOTLOADER_PROTOCOL_UpdateFw(BOOTLOADER_T *Bootloader, char *cmd, char *rsp, uint32_t *complete)
{
	static uint32_t FwAddr;	
	static uint32_t FwUpdateStarted = 0;
	// проверка длины запроса
	uint16_t ReadCRC16, CalcCRC16;
	uint32_t rsp_len = 0;
	uint32_t cmd_len = 0;
	uint32_t ReadAddr;
	uint32_t i;
	uint8_t FwByte;
	uint32_t Error;
	char *str;
	
	str = strchr( cmd, 0x0D );	
	cmd_len = str - cmd;
	if( cmd_len < 6 ) 
		return 0; // ошибка в запросе - нет ответа
	// чтение, рассчёт и проверка CRC	
	if(!HexStr4ToWord(cmd + cmd_len - 4, &ReadCRC16))
		return 0;
	*(char*)( cmd + cmd_len - 4 ) = 0;
	CalcCRC16 = Calc_CRC16( cmd, cmd_len - 4, 0 );
	if( ReadCRC16 != CalcCRC16 ) 
		return 0;// ошибка в запросе - нет ответа
	
		// Целостный запрос обрабатываем в соответствии с типом
	if( !strcmp( cmd + 2, "FWCONFIG" ) )// запрос конфигурации загрузчика - печатаем конфигурацию
	{
		rsp_len = BOOTLOADER_PROTOCOL_FwConfig( cmd, rsp );
	}
	else if( !strcmp( cmd + 2, "SOFTRESET" ) )// запрос програмной перезагрузки - перезагружаем
	{
		BSP_Reset();
	}
	else if( !strcmp( cmd + 2, "DISABLESIGNATURE" ) )// запрос програмной перезагрузки - перезагружаем
	{
		rsp_len = BOOTLOADER_PROTOCOL_FwStr( cmd, rsp, "DISABLESIGNATURE;OK" );// печатаем ответ об аварии
	}
	else if( !strcmp( cmd + 2, "HEXLOAD" ) )// запрос старта перезаливки основной программы
	{
		// Инициализируем начало загрузки основной программы
		FwUpdateStarted = 1; //флаг запущенной загрузки
		FwAddr = BOOTLOADER_FW_ADDR;// начальый адрес загружаемой программы
		BOOTLOADER_UpdateNewFwStart(Bootloader);
		rsp_len = BOOTLOADER_PROTOCOL_FwAddr( cmd, rsp, FwAddr ); //печатаем ответ с загружаемым текущим адресом программы
	}
	else if( FwUpdateStarted && !strcmp( cmd + 2, "FINISH" ) )//запрос завершения загрузки
	{//---------- Окончание загрузки --------------
		FwUpdateStarted = 0;//очищаем флаг загрузки 
		if(!BOOTLOADER_UpdateNewFwFinish(Bootloader))
		{			
			if(!BOOTLOADER_CheckNewFw(Bootloader) )
			{
				//успешное завершение: посылаем сообщение, выжидаем пока оно будет отправлено и затем перезагрузка
				BOOTLOADER_UpdateNewFw(Bootloader);
				rsp_len = BOOTLOADER_PROTOCOL_FwStr( cmd, rsp, "COMPLETE" );// печатаем ответ об аварии
				*complete = 1;
			}
			else
			{
				rsp_len = BOOTLOADER_PROTOCOL_FwStr( cmd, rsp, "EFINISH" );// печатаем ответ об аварии
			}			
		}
		else
		{
			rsp_len = BOOTLOADER_PROTOCOL_FwStr( cmd, rsp, "EFINISH" );// печатаем ответ об аварии
		}
	}
	else if( FwUpdateStarted )//Принятие и копирование блоков основной программы во внешнюю флэш копии
	{//--------      Чтение прошивки    ----------		
		Error = 0;
		if( !HexStr8ToUint( cmd + 2, &ReadAddr ) )
			Error = 1;//адрес пакета с проверкой
		if( !Error && (ReadAddr == FwAddr) )// загрузка должна произвадиться непрерывно адрес за адресом
		{
			//проверка содержимого пакета
			str = cmd + ( 2 + 8 );
			for( i = 0; i < 64; str += 2, i++ )
			{
				if( !HexStr2ToByte( str, &FwByte ) )
				{
					Error = 1;
					break;
				}				
				if ( BOOTLOADER_UpdateNewFwByte(Bootloader, FwByte) )
				{
					Error = 1;
					break;
				}
			}
			if( !Error )
			{
					FwAddr += 64;
			}
		}
		if( Error )
		{// ответ в случае ошибки записи пакета - аварийное завершение перепрошивки
			FwUpdateStarted = 0; 
			// печатаем ответ об аварии
			rsp_len = BOOTLOADER_PROTOCOL_FwStr( cmd, rsp, "EFINISH" );
		}
		else 
		{
			rsp_len = BOOTLOADER_PROTOCOL_FwAddr( cmd, rsp, FwAddr );
		}
	}
	if( rsp_len == 0 )
	{	//<0x80+SlotAddr><0x0F>NA<CRC><0x0D>
		// ответ на некорректную команду 
		rsp_len = BOOTLOADER_PROTOCOL_FwStr( cmd, rsp, "NA" );
	}
	return rsp_len;
}

uint32_t BOOTLOADER_PROTOCOL_ParseCommand(BOOTLOADER_T *Bootloader, char *cmd, char *rsp, uint32_t *complete)
{
	uint32_t len = 0;
	
	if ( !cmd || !rsp )
		return 0;
	
	switch (cmd[1])
	{
		case 0x01:
			len = BOOTLOADER_PROTOCOL_GetDeviceClass(cmd, rsp);
			break;
		case 0x0F:
			len = BOOTLOADER_PROTOCOL_UpdateFw(Bootloader, cmd, rsp, complete);
			break;
		default:
			break;
	}	
	return len;	
}
