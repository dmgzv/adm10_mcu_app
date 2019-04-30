#include "profile_protocol.h"
#include "bootloader_protocol.h"
#include "profile.h"
#include <string.h>
#include "string_fnc.h"
#include "crc.h"

#define PROFILE_PROTOCOL_LANG_PACKET_SIZE 256

static uint32_t PROFILE_PROTOCOL_GetClass(PROFILE_T *Profile, char * cmd, char * rsp )
{
	char *p = rsp;
	
	// для уменьшения путаницы класс берём прямо из профиля!!!
	memcpy( p, cmd, 2 );
	p += 2;
	strcpy( p, Profile->Descr[Profile->Idx]->DeviceClass );
	p += strlen(p);
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

static uint32_t PROFILE_PROTOCOL_GetClassExt(PROFILE_T *Profile, char * cmd, char * rsp )
{
	char *p = rsp;
	uint32_t i;
	
	// для уменьшения путаницы класс берём прямо из профиля!!!
	memcpy( p, cmd, 2 );
	p += 2;
	strcpy( p, Profile->Descr[Profile->Idx]->DeviceClass );
	p += strlen(p);
	*p++ = ';';
	
	if (Profile->SetEnable)
	{
		*p++ = 'O';
		*p++ = 'K';
	}
	else
	{
		*p++ = 'N';
		*p++ = 'A';
	}
	*p++ = ';';
	
	if (Profile->ProfileFlash.EnableSave)
	{
		*p++ = 'O';
		*p++ = 'K';
	}
	else
	{
		*p++ = 'N';
		*p++ = 'A';
	}
	*p++ = ';';
	
	for (i = 0; i < PROFILE_TABLES_CNT; i++)
	{
		if (Profile->TablesChanges & (1 << i))
		{
			if (i < PROFILE_TABLE_100)
				p = UintToStr( p, i + 1 );
			else
				p = UintToStr( p, i - PROFILE_TABLE_100 + 100 );
			*p++ = ';'; 
		}
	}
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );//Контрольная сумма и OD
	*p++ = '\r';
	*p = 0;
	return p - rsp;	
}

static uint32_t PROFILE_PROTOCOL_SetAccess(PROFILE_T *Profile, char * cmd, char * rsp )
{
	if( !strncmp( cmd + 2, "IPG", 3 ) )
	{//Пароль верный
		Profile->SetEnable = 1;
		memcpy( rsp, cmd, 2 );
		strcpy( rsp + 2, "OK\r");
	}
	else
	{//Пароль не верен
		Profile->SetEnable = 0;
		memcpy( rsp, cmd, 2 );
		strcpy( rsp + 2, "NA\r");
	}
	return 5;
}

static uint32_t PROFILE_PROTOCOL_SetAccessExt(PROFILE_T *Profile, char * cmd, char * rsp )
{
	char *p = rsp;
	
	memcpy( p, cmd, 2 );
	p += 2;
	
	if( (cmd[2] == 'O') && (cmd[3] == 'K') && (cmd[4] == ';')	)
	{
		Profile->SetEnable = 1;
	}
	else if( (cmd[2] == 'N') && (cmd[3] == 'A') && (cmd[4] == ';')	)
	{
		Profile->SetEnable = 0;
	}
	else
	{
		strcpy( p,"NA" );
		goto exit;
	}
	
	if( (cmd[5] == 'O') && (cmd[6] == 'K') && (cmd[7] == ';')	)
	{
		if (!Profile->ProfileFlash.EnableSave)
			PROFILE_FLASH_ParamsSave(&Profile->ProfileFlash);
		Profile->ProfileFlash.EnableSave = 1;
	}
	else if( (cmd[5] == 'N') && (cmd[6] == 'A') && (cmd[7] == ';')	)
	{
		Profile->ProfileFlash.EnableSave = 0;
	}
	else
	{
		strcpy( p,"NA" );
		goto exit;
	}
	
	strcpy( p,"OK" );
	p += 2;
exit:
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );//Контрольная сумма и OD
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

static uint32_t PROFILE_PROTOCOL_GetProfile(PROFILE_T *Profile, char * cmd, char * rsp )
{
	uint32_t idx;
	char *p = rsp;

	memcpy(p, cmd, 2);
	p += 2;
	
	if( !StrToUint( &cmd[2], &idx, ';' ) )
		idx = 0xFFFFFFFF; //номер запрашиваемой строки
	
	p = UintToStr( p, idx );
	*p++ = ';'; 
	if( idx == 0 )
	{//Запрос количетсва строк
		p = UintToStr( p, *Profile->Descr[Profile->Idx]->TextSize ); 
	}
	else if( idx <= *Profile->Descr[Profile->Idx]->TextSize )
	{//Запрос строки профиля
		strcpy( p, Profile->Descr[Profile->Idx]->Text[ idx - 1 ] );
		p += strlen( p );
	}
	else
	{//некорректный номер строки
		strcpy( p,"NA" );
		p += 2;
	}
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );//Контрольная сумма и OD
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

static uint32_t PROFILE_PROTOCOL_GetParams(PROFILE_T *Profile, char * cmd, char * rsp)
{	
	uint32_t AddColor = 0;
	uint32_t TableId;//номер таблицы (секции)
	uint32_t Item;//номер таблицы (секции)
	uint32_t TableSize = 0;
	char* p;
	uint32_t len;
	uint8_t Color;
	uint8_t State;
	char TempValue[PROFILE_STRING_MAX_LEN];
	
	if( !StrToUint( cmd + 2, &TableId, ';' ) )
		TableId = 0xFFFFFFFF;

	//начинаем формировать ответ
	p = rsp;
	memcpy(p, cmd, 2);
	p += 2;
	p = UintToStr( p, TableId );//Номер таблицы
	*p++ = ';';
	
	switch (TableId)
	{
		case 1:
			TableId = PROFILE_TABLE_1;
			TableSize = Profile->Descr[Profile->Idx]->Table1Size;
			break;
		case 2:
			TableId = PROFILE_TABLE_2;
			TableSize = Profile->Descr[Profile->Idx]->Table2Size;
			break;
		case 3:
			TableId = PROFILE_TABLE_3;
			TableSize = Profile->Descr[Profile->Idx]->Table3Size;
			break;
		case 4:
			TableId = PROFILE_TABLE_4;
			TableSize = Profile->Descr[Profile->Idx]->Table4Size;
			break;
		case 5:
			TableId = PROFILE_TABLE_5;
			TableSize = Profile->Descr[Profile->Idx]->Table5Size;
			break;
		case 8:
			TableId = PROFILE_TABLE_8;
			TableSize = Profile->Descr[Profile->Idx]->Table8Size;
			break;
		case 103:
			TableId = PROFILE_TABLE_103;
			TableSize = Profile->Descr[Profile->Idx]->Table103Size;
			break;
		default:
			TableId = 0xFFFFFFFF;
			TableSize = 0;
		break;
	}
	
	if ((TableId != 0xFFFFFFFF) && (TableSize != 0))
	{
		if ((cmd[1] == 0x77) && (TableId == PROFILE_TABLE_2))
			AddColor = 1;
		
		for(Item = 1 ; Item <= TableSize; Item++ )//перебор параметров секции	с их печатью
		{
			PROFILE_GetState(Profile, TableId, Item, &State);
			PROFILE_GetColor(Profile, TableId, Item, &Color);
			//if (State != PROFILE_STATE_OOS)
			{
				if (PROFILE_GetValue(Profile, TableId, Item, TempValue) == 0)
				{
					if (PROFILE_ValueToString( Profile, TableId, Item, TempValue, p , &len) == 0)
						p += len;
				}
			}			
			if( AddColor && (Color != PROFILE_COLOR_NA))//Добавляем цвет, если секция с цветовой раскраской и цвет не равен NA
			{
				if (State != PROFILE_STATE_IS)
					Color = PROFILE_COLOR_NO;
				strcpy( p++, "@" );
				p = ByteToStr( p, Color );
			}
			*p++ = ';';
		}
		Profile->TablesChanges &= ~(1 << TableId);
	}
	else
	{//Нет такой таблицы
		strcpy( p, "NA;" );
		p += 3;
	}
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );//Контрольная сумма и OD
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

static uint32_t PROFILE_PROTOCOL_SetParam(PROFILE_T *Profile, char * cmd, char * rsp)
{
	uint32_t TableId;//номер таблицы (секции)
	uint32_t Item = 0;
	char * p = cmd;
	uint32_t error = 0;
	uint32_t len;
	uint32_t CRCRead, CRCCalc;
	char *Value;
	char TempValue[PROFILE_STRING_MAX_LEN];

	//проверка и разбор структуры запроса
	p = cmd + 2;
	if( !StrToUint( p, &TableId, ';' ) ) 
		error = 1;
	if( ( p = strchr( p, ';' ) ) == NULL )
		error = 1;		//;Конец таблицы
	if( !StrToUint( p + 1, &Item, ';' ) )
		error = 1;
	if( (  p = strchr(  p + 1, ';' ) ) == NULL )
		error = 1;		//;Конец индекса
	Value =  p + 1;	  //Указатель на параметр 
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
		error = 1;		//;Конец параметра
	if( strchr( p + 1, ';' ) )
		error = 1;		 // ; на конце CRC быть не должно
	if( p && !error )//успешно найдены точно 3 символа ";"
	{	
		//Проверка контрольной суммы	
		p++;		
		if( !StrToUint( p, &CRCRead, ';' ) )
			CRCRead = 0xFFFFFFFF;
		CRCCalc = Calc_CRC16_ASCII( cmd, p - cmd );
		*--p = 0;
		if( CRCRead != CRCCalc ) 
			error = 1; //Неверная контрольная сумма
	}
	else 
		error = 1;
	
	p = rsp;
	memcpy(p, cmd, 2);
	p += 2;
	p = UintToStr( p, TableId );
	*p++ = ';';
	p = UintToStr( p, Item );
	*p++ = ';';
	
	switch (TableId)
	{
		case 1:
			TableId = PROFILE_TABLE_1;
			break;
		case 2:
			TableId = PROFILE_TABLE_2;
			break;
		case 3:
			TableId = PROFILE_TABLE_3;
			break;
		case 4:
			TableId = PROFILE_TABLE_4;
			break;
		case 5:
			TableId = PROFILE_TABLE_5;
			break;
		case 8:
			TableId = PROFILE_TABLE_8;
			break;
		case 103:
			TableId = PROFILE_TABLE_103;
			break;
		default:
			TableId = 0xFFFFFFFF;
		break;
	}
	
	if (( (!error) && (Profile->SetEnable) && (TableId != 0xFFFFFFFF))		//Есть доступ на установку параметров
		&& ( ( cmd[ 1 ] == 0x72 && TableId != PROFILE_TABLE_6 ) || ( cmd[ 1 ] == 0x76 && TableId == PROFILE_TABLE_6 ) ) ) //Проверка номера таблицы и команды
	{	
		if (PROFILE_StringToValue(Profile, TableId, Item, TempValue, Value) == 0)
		{
			if (PROFILE_CheckValue(Profile, TableId, Item, TempValue) == 0)
			{
				PROFILE_SetValue(Profile, TableId, Item, TempValue);
				PROFILE_SetValueDependencies(Profile, TableId, Item, TempValue);
				PROFILE_FLASH_ParamsSave(&Profile->ProfileFlash);
			}				
		}
		if (PROFILE_GetValue(Profile, TableId, Item, TempValue) == 0)
		{
			if (PROFILE_ValueToString(Profile, TableId, Item, TempValue, p , &len) == 0)
				p += len;
		}
	}
	else//Ошибка установки параметров
	{
		strcpy( p, "NA" );
		p += 2;
	}
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );//Контрольная сумма и OD
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

int32_t PROFILE_PROTOCOL_SetColor(PROFILE_T *Profile, char * cmd, char * rsp)
{
	uint32_t TableId = 0;
	uint32_t Item = 0;
	uint32_t tmp;
	uint8_t ColorDegrade; //Degrade цвет
	uint8_t ColorFailure; //Failure цвет
	uint32_t ParamsCnt = Profile->Descr[Profile->Idx]->Table2Size;
	int32_t error = 0;
	char *p = rsp;
	uint32_t CRCRead, CRCCalc;

	//проверка и разбор структуры запроса
	p = cmd + 2;
	if( !StrToUint( p, &TableId, ';' ) )
		error = 1;
	if( ( p = strchr( p, ';' ) ) == NULL )
		error = 1;		//;Конец таблицы
	if( !StrToUint( p + 1, &Item, ';' ) )
		error = 1;
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
		error = 1;		//;Конец индекса
	if( !StrToUint( p + 1, &tmp, ';' ) )
		error = 1; 
	ColorDegrade = tmp;
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
		error = 1;		//;Конец цвета degrade
	if( !StrToUint( p + 1, &tmp, ';' ) )
		error = 1;
	ColorFailure = tmp;
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
		error = 1;		//;Конец цвета failure
	if( strchr( p + 1, ';' ) )
		error = 1;			 // ; на конце CRC быть не должно
	if( p && !error )//успешно найдены точно 4 символа ";"
	{	
		//Проверка контрольной суммы	
		p++;
		if( !StrToUint( p, &CRCRead, ';' ) ) 
			CRCRead = 0xFFFFFFFF;
		CRCCalc = Calc_CRC16_ASCII( cmd, p - cmd );
		*--p = 0;
		if( CRCRead != CRCCalc )
			error = 1; //Неверная контрольная сумма
	}
	else error = 1;

	memcpy(rsp, cmd, 2);
	p = rsp + 2;
	// установка при отсутствии ошибок в запросе
	if( !error && Profile->SetEnable && TableId == 2 && Item && Item <= ParamsCnt)
	{
		PROFILE_SetAlarmColors(Profile, PROFILE_TABLE_2, Item, ColorDegrade, ColorFailure);
		PROFILE_FLASH_ParamsSave(&Profile->ProfileFlash);
		PROFILE_GetAlarmColors(Profile, PROFILE_TABLE_2, Item, &ColorDegrade, &ColorFailure);
		p = UintToStr( p, TableId );
		*p++ = ';';
		p = UintToStr( p, Item );
		*p++ = ';';
		p = UintToStr( p, ColorDegrade ); 
		*p++ = ';';
		p = UintToStr( p, ColorFailure );
		*p++ = ';';
	}
	else//Ошибка установки
	{
		p = UintToStr( p, TableId );
		*p++ = ';';
		p = UintToStr( p, Item );
		strcpy( p, ";NA;" ); 
		p += 4;
	}
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );//Контрольная сумма и OD
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

int32_t PROFILE_PROTOCOL_GetColors(PROFILE_T *Profile, char * cmd, char * rsp)
{
	uint32_t TableId;
	char* p = rsp;
	PROFILE_TABLE2_PARAM_T *Param = (PROFILE_TABLE2_PARAM_T*)Profile->Descr[Profile->Idx]->Table2;
	uint32_t ParamsCnt = Profile->Descr[Profile->Idx]->Table2Size;
	uint32_t i;
	uint8_t ColorDegrade; //Degrade цвет
	uint8_t ColorFailure; //Failure цвет
	
	if( !StrToUint( cmd + 2, &TableId, ';' ) )
		TableId = 0xFFFFFFFF;
	
	memcpy(p ,cmd ,2);
	p += 2;	
	p = UintToStr( p, TableId );//Номер таблицы
	*p++ = ';';
	if( TableId == 2 )// цвета только для динимических параметров
	{
		//Выводим для всех параметров
		for( i = 1; i <= ParamsCnt; i++ )
		{
			if( Param[i-1].Color )
			{
				PROFILE_GetAlarmColors(Profile, PROFILE_TABLE_2, i, &ColorDegrade, &ColorFailure);
				p = UintToStr( p, ColorDegrade );//Degrade цвет
				*p++ = '@';
				p = UintToStr( p, ColorFailure );//Failure цвет
				*p++ = ';';
			}
			else
			{
				strcpy( p, "0@0;" ); 
				p += 4;// для нераскрашиваемых параметров
			}
		}
	}
	else 
	{
		strcpy( p, "NA;" ); 
		p += 3;
	}
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );//Контрольная сумма и OD
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

int32_t PROFILE_PROTOCOL_WriteLangpack(PROFILE_T *Profile, char * cmd, char * rsp)
{
	char* p;
	char* LangBlock;
	uint32_t Item;
	uint32_t error = 0;

	//проверка и разбор структуры запроса	
	p = cmd + 2;
	if( !StrToUint( p, &Item, ';' ) )
		error = 1;
	if( ( p = strchr( p, ';' ) ) == NULL )
		error = 1;		//;Конец индекса
	LangBlock = p + 1;

	p = rsp;
	memcpy(p , cmd, 2);
	p += 2;
	if( !error && Item < PROFILE_FLASH_LANGS_SIZE / PROFILE_PROTOCOL_LANG_PACKET_SIZE)
	{
		PROFILE_FLASH_LangpackSave(&Profile->ProfileFlash, LangBlock, Item);
		p = UintToStr( p, Item );
		strcpy( p, ";OK;" );
		p += 4;
	}
	else 
	{// ответ об ошибке
		p = UintToStr( p, Item );
		strcpy( p, ";NA;" );
		p += 4;
	}
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );//Контрольная сумма и OD
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

int32_t PROFILE_PROTOCOL_ReadLangpack(PROFILE_T *Profile, char * cmd, char * rsp)
{
	uint32_t Item;
	uint32_t error = 0, i;
	char *p;
	
	if( !StrToUint( cmd + 2, &Item, ';' ) )
		error = 1;
	
	p = rsp;
	memcpy(p, cmd, 2);
	p += 2;
	p = UintToStr( p, Item );// начинаем формировать ответ
	*p++ = ';';
	
	if( !error && Item < PROFILE_FLASH_LANGS_SIZE / PROFILE_PROTOCOL_LANG_PACKET_SIZE )
	{// читаем блок из флеш, если он доступен
		PROFILE_FLASH_LangpackLoad(&Profile->ProfileFlash, p, Item);
		for(i = 0; i < 256; i++)
		{
			if (*p == '\r')
				*p = '\x03';
			//if (*p == ';')
			//	*p = '\x02';
			if (*p == '\0')
				*p = '\x03';
			p++;
		}
	}
	else
	{//блок не доступен - ошибка
		strcpy( p,"NA" );
		p += 2;
	}
	*p++ =';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );//Контрольная сумма и OD
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

uint32_t PROFILE_PROTOCOL_Command(PROFILE_T *Profile, char *cmd, char *rsp, uint32_t *complete)
{
	uint32_t len = 0;
	
	switch( cmd[ 1 ] )//Команда
	{
		case 0x01://Чтение класса устройства:
			len = PROFILE_PROTOCOL_GetClass(Profile, cmd, rsp);
			break;
		case 0x61://Расширенное чтение класса устройства:
			len = PROFILE_PROTOCOL_GetClassExt(Profile, cmd, rsp);
			break; 
		case 0x49://Разрешить запись в устройство
			len = PROFILE_PROTOCOL_SetAccess(Profile, cmd, rsp);
			break;
		case 0x62://Расширенное разрешение на запись в устройство
			len = PROFILE_PROTOCOL_SetAccessExt(Profile, cmd, rsp);
			break;
		case 0x71:
		case 0x77://Чтение параметров без раскраски, с раскраской 
			len = PROFILE_PROTOCOL_GetParams(Profile, cmd, rsp);
			break;
		case 0x72:
		case 0x76://Запись парамерта в профиль
			len = PROFILE_PROTOCOL_SetParam(Profile, cmd, rsp);
			break;
		case 0x73://Запрос строки из профиля
			len = PROFILE_PROTOCOL_GetProfile(Profile, cmd, rsp);
			break;
		case 0x7C://Запись цветов	состояния
			len = PROFILE_PROTOCOL_SetColor(Profile, cmd, rsp);
			break;
		case 0x7D://Чтение цветов	состояния
			len = PROFILE_PROTOCOL_GetColors(Profile, cmd, rsp);
			break;
		case 0x7A://Запись языкового пакета
			len = PROFILE_PROTOCOL_WriteLangpack(Profile, cmd, rsp);
			break;
		case 0x7B://Чтение языкового пакета
			len = PROFILE_PROTOCOL_ReadLangpack(Profile, cmd, rsp);
			break;
		case 0x0F:
			len = BOOTLOADER_PROTOCOL_UpdateFw(&Profile->Bootloader, cmd, rsp, complete);
			break;
	}
	if( len == 0 )// если нет ответа, отвечаем об ошибке
	{
		memcpy(rsp, cmd, 2);
		strcpy( rsp + 2, "NA\r" );
		len = 5;
	}
	return len;
}
