#include "profile_protocol.h"
#include <string.h>
#include "string_fnc.h"
#include "crc.h"

static uint32_t PROFILE_PROTOCOL_GetClass(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp )
{
	char *p = rsp;
	
	// для уменьшения путаницы класс берём прямо из профиля!!!
	memcpy( p, cmd, 2 );
	p += 2;
	strcpy( p, Protocol->Profile->Descr[Protocol->Profile->Idx]->DeviceClass );
	p += strlen(p);
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

static uint32_t PROFILE_PROTOCOL_GetClassExt(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp )
{
	char *p = rsp;
	uint32_t i;
	
	// для уменьшения путаницы класс берём прямо из профиля!!!
	memcpy( p, cmd, 2 );
	p += 2;
	strcpy( p, Protocol->Profile->Descr[Protocol->Profile->Idx]->DeviceClass );
	p += strlen(p);
	*p++ = ';';
	
	if (Protocol->Profile->SetEnable)
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
	
	if (Protocol->ProfileFlash->SaveEnable)
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
		if (Protocol->Profile->TablesChanges & (1 << i))
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

static uint32_t PROFILE_PROTOCOL_SetAccess(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp )
{
	if( !strncmp( cmd + 2, "IPG", 3 ) )
	{//Пароль верный
		Protocol->Profile->SetEnable = 1;
		memcpy( rsp, cmd, 2 );
		strcpy( rsp + 2, "OK\r");
	}
	else
	{//Пароль не верен
		Protocol->Profile->SetEnable = 0;
		memcpy( rsp, cmd, 2 );
		strcpy( rsp + 2, "NA\r");
	}
	return 5;
}

static uint32_t PROFILE_PROTOCOL_SetAccessExt(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp )
{
	char *p = rsp;
	
	memcpy( p, cmd, 2 );
	p += 2;
	
	if( (cmd[2] == 'O') && (cmd[3] == 'K') && (cmd[4] == ';')	)
	{
		Protocol->Profile->SetEnable = 1;
	}
	else if( (cmd[2] == 'N') && (cmd[3] == 'A') && (cmd[4] == ';')	)
	{
		Protocol->Profile->SetEnable = 0;
	}
	else
	{
		strcpy( p,"NA" );
		p+=2;
		goto exit;
	}
	
	if( (cmd[5] == 'O') && (cmd[6] == 'K') && (cmd[7] == ';')	)
	{
		if (!Protocol->ProfileFlash->SaveEnable)
			PROFILE_FLASH_SaveConfigs(Protocol->ProfileFlash);
		Protocol->ProfileFlash->SaveEnable = 1;
	}
	else if( (cmd[5] == 'N') && (cmd[6] == 'A') && (cmd[7] == ';')	)
	{
		Protocol->ProfileFlash->SaveEnable = 0;
	}
	else
	{
		strcpy( p,"NA" );
		p+=2;
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

static uint32_t PROFILE_PROTOCOL_GetProfile(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp )
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
		p = UintToStr( p, *Protocol->Profile->Descr[Protocol->Profile->Idx]->TextSize ); 
	}
	else if( idx <= *Protocol->Profile->Descr[Protocol->Profile->Idx]->TextSize )
	{//Запрос строки профиля
		strcpy( p, Protocol->Profile->Descr[Protocol->Profile->Idx]->Text[ idx - 1 ] );
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

static uint32_t PROFILE_PROTOCOL_GetParams(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp)
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
		goto error;

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
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table1Size;
			break;
		case 2:
			TableId = PROFILE_TABLE_2;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table2Size;
			break;
		case 3:
			TableId = PROFILE_TABLE_3;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table3Size;
			break;
		case 4:
			TableId = PROFILE_TABLE_4;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table4Size;
			break;
		case 5:
			TableId = PROFILE_TABLE_5;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table5Size;
			break;
		case 8:
			TableId = PROFILE_TABLE_8;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table8Size;
			break;
		case 103:
			TableId = PROFILE_TABLE_103;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table103Size;
			break;
		default:
			TableId = 0xFFFFFFFF;
			TableSize = 0;
		break;
	}
	
	if ((TableId != 0xFFFFFFFF) && (TableSize != 0))
	{
		if ((cmd[1] == PROFILE_PROTOCOL_CMD_GET_VALUES_2) && (TableId == PROFILE_TABLE_2))
			AddColor = 1;
		
		for(Item = 1 ; Item <= TableSize; Item++ )//перебор параметров секции	с их печатью
		{
			if (PROFILE_GetState(Protocol->Profile, TableId, Item, &State))
				goto error;
			if (State != PROFILE_STATE_OOS)
			{
				if (PROFILE_GetValue(Protocol->Profile, TableId, Item, TempValue))
				{
					goto error;
				}
				else
				{
					if (PROFILE_ValueToString(Protocol->Profile, TableId, Item, TempValue, p , &len) == 0)
						p += len;
				}
				if (PROFILE_GetColor(Protocol->Profile, TableId, Item, &Color))
				{
					goto error;
				}
				if( AddColor && (Color != PROFILE_COLOR_NA))//Добавляем цвет, если секция с цветовой раскраской и цвет не равен NA
				{
					if (State != PROFILE_STATE_IS)
						Color = PROFILE_COLOR_NO;
					strcpy( p++, "@" );
					p = ByteToStr( p, Color );
				}
			}
			else
			{
				*p++ = 0x01;
			}				
			
			*p++ = ';';
		}
		Protocol->Profile->TablesChanges &= ~(1 << TableId);
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
	
error:
	p = rsp;
	*p++ = cmd[0];
	*p++ = cmd[1];
	*p++ = 'N';
	*p++ = 'A';
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

static uint32_t PROFILE_PROTOCOL_SetParam(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp)
{
	uint32_t Table;
	uint32_t TableId;
	uint32_t TableSize;
	uint32_t Item;
	char *p, *c;
	uint32_t CRCRead, CRCCalc;
	char Value[PROFILE_STRING_MAX_LEN];
	char TempValue[PROFILE_STRING_MAX_LEN];
	
	p = cmd + 2;
	if( !StrToUint( p, &TableId, ';' ) )
		goto cmd_parse_error;
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
		goto cmd_parse_error;
	if( !StrToUint( p + 1, &Item, ';' ) )
		goto cmd_parse_error;
	if( (  p = strchr(  p + 1, ';' ) ) == NULL )
		goto cmd_parse_error;
	c = p + 1;
	if( (  p = strchr(  p + 1, ';' ) ) == NULL )
		goto cmd_parse_error;
	if ((p - c) >= PROFILE_STRING_MAX_LEN)
		goto cmd_parse_error;
	memcpy(Value, c, p - c);
	Value[p - c] = 0;
	if( !StrToUint( p + 1, &CRCRead, ';' ) )
		goto cmd_parse_error;
	CRCCalc = Calc_CRC16_ASCII( cmd, p + 1 - cmd );
	
	switch (TableId)
	{
		case 1:
			Table = PROFILE_TABLE_1;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table1Size;
			break;
		case 2:
			Table = PROFILE_TABLE_2;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table2Size;
			break;
		case 3:
			Table = PROFILE_TABLE_3;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table3Size;
			break;
		case 4:
			Table = PROFILE_TABLE_4;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table4Size;
			break;
		case 5:
			Table = PROFILE_TABLE_5;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table5Size;
			break;
		case 8:
			Table = PROFILE_TABLE_8;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table8Size;
			break;
		case 103:
			Table = PROFILE_TABLE_103;
			TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table103Size;
			break;
		default:
			goto cmd_exec_error;
	}
	
	if (!Protocol->Profile->SetEnable)
		goto cmd_exec_error;
	
	if ((Item < 1) || (Item > TableSize))
		goto cmd_exec_error;
	
	if( CRCRead != CRCCalc )
		goto cmd_exec_error;
	
	if (PROFILE_CheckWriteAccess(Protocol->Profile, Table, Item))
		goto cmd_exec_error;
	if (PROFILE_StringToValue(Protocol->Profile, Table, Item, TempValue, Value))
		goto cmd_exec_error;
	if (PROFILE_CheckValue(Protocol->Profile, Table, Item, TempValue, 0))
		goto cmd_exec_error;
	if (PROFILE_SetValue(Protocol->Profile, Table, Item, TempValue))
		goto cmd_exec_error;
	if (PROFILE_SetValueDependencies(Protocol->Profile, Table, Item, TempValue, 0))
		goto cmd_exec_error;
	PROFILE_FLASH_SaveValue(Protocol->ProfileFlash, Table, Item);

	strcpy(rsp, cmd);
	return strlen(rsp);

cmd_parse_error:
	p = rsp;
	*p++ = cmd[0];
	*p++ = cmd[1];
	*p++ = 'N';
	*p++ = 'A';
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
	*p++ = '\r';
	*p = 0;
	return p - rsp;
	
cmd_exec_error:
	p = rsp;
	*p++ = cmd[0];
	*p++ = cmd[1];
	p = UintToStr( p, TableId );
	*p++ = ';';
	p = UintToStr( p, Item );
	*p++ = ';';
	*p++ = 'N';
	*p++ = 'A';
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

int32_t PROFILE_PROTOCOL_SetColor(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp)
{
	uint32_t TableId;
	uint32_t Item;
	uint32_t tmp;
	uint8_t ColorDegrade;
	uint8_t ColorFailure;
	uint32_t TableSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table2Size;
	char *p;
	uint32_t CRCRead, CRCCalc;

	p = cmd + 2;
	if( !StrToUint( p, &TableId, ';' ) )
		goto cmd_parse_error;
	if( ( p = strchr( p, ';' ) ) == NULL )
		goto cmd_parse_error;
	if( !StrToUint( p + 1, &Item, ';' ) )
		goto cmd_parse_error;
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
		goto cmd_parse_error;
	if( !StrToUint( p + 1, &tmp, ';' ) )
		goto cmd_parse_error;
	ColorDegrade = tmp;
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
		goto cmd_parse_error;
	if( !StrToUint( p + 1, &tmp, ';' ) )
		goto cmd_parse_error;
	ColorFailure = tmp;
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
		goto cmd_parse_error;
	if( !StrToUint( p + 1, &CRCRead, ';' ) ) 
		goto cmd_parse_error;
	CRCCalc = Calc_CRC16_ASCII( cmd, p + 1 - cmd );

	if (!Protocol->Profile->SetEnable)
		goto cmd_exec_error;
	
	if (TableId != 2)
		goto cmd_exec_error;
	
	if ((Item < 1) || (Item > TableSize))
		goto cmd_exec_error;
	
	if( CRCRead != CRCCalc )
		goto cmd_exec_error;
	
	if (PROFILE_SetAlarmColors(Protocol->Profile, PROFILE_TABLE_2, Item, ColorDegrade, ColorFailure))
		goto cmd_exec_error;
	PROFILE_FLASH_SaveConfigs(Protocol->ProfileFlash);
	
	strcpy(rsp, cmd);
	return strlen(rsp);

cmd_parse_error:
	p = rsp;
	*p++ = cmd[0];
	*p++ = cmd[1];
	*p++ = 'N';
	*p++ = 'A';
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
	*p++ = '\r';
	*p = 0;
	return p - rsp;
	
cmd_exec_error:
	p = rsp;
	*p++ = cmd[0];
	*p++ = cmd[1];
	p = UintToStr( p, TableId );
	*p++ = ';';
	p = UintToStr( p, Item );
	*p++ = ';';
	*p++ = 'N';
	*p++ = 'A';
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

int32_t PROFILE_PROTOCOL_GetColors(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp)
{
	uint32_t TableId;
	char* p = rsp;
	uint32_t ParamsCnt = Protocol->Profile->Descr[Protocol->Profile->Idx]->Table2Size;
	uint32_t i;
	uint8_t ColorDegrade; //Degrade цвет
	uint8_t ColorFailure; //Failure цвет
	
	if( !StrToUint( cmd + 2, &TableId, ';' ) )
		goto error;
	
	memcpy(p ,cmd ,2);
	p += 2;	
	p = UintToStr( p, TableId );//Номер таблицы
	*p++ = ';';
	if( TableId == 2 )// цвета только для динимических параметров
	{
		//Выводим для всех параметров
		for( i = 1; i <= ParamsCnt; i++ )
		{
			if (PROFILE_GetAlarmColors(Protocol->Profile, PROFILE_TABLE_2, i, &ColorDegrade, &ColorFailure))
				goto error;
			p = UintToStr( p, ColorDegrade );//Degrade цвет
			*p++ = '@';
			p = UintToStr( p, ColorFailure );//Failure цвет
			*p++ = ';';
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

error:
	p = rsp;
	*p++ = cmd[0];
	*p++ = cmd[1];
	*p++ = 'N';
	*p++ = 'A';
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

int32_t PROFILE_PROTOCOL_WriteLangpack(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp)
{
	char* p;
	char* LangBlock;
	uint32_t Item;
	uint32_t error = 0;
	uint32_t LangpackBlocks, LangpackType;

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
	if((!error) && (Item < (PROFILE_FLASH_LANGS_SIZE / PROFILE_PROTOCOL_LANG_BLOCK_SIZE)))
	{
		if (Item == 0)
		{
			if( !StrToUint( LangBlock, &LangpackBlocks, ';' ) )
				goto error;
			if( ( LangBlock = strchr( LangBlock + 1, ';' ) ) == NULL )
				goto error;
			LangBlock++;
			if( !StrToUint( LangBlock, &LangpackType, ';' ) )
				goto error;
			Protocol->LangpackBlocks = LangpackBlocks;
			Protocol->LangpackType = LangpackType;
			p = UintToStr( p, Item );
			strcpy( p, ";OK;" );
			p += 4;
		}
		else if (Item <= Protocol->LangpackBlocks)
		{
			if (PROFILE_FLASH_SaveLangpackData(Protocol->ProfileFlash, (Item - 1) * PROFILE_PROTOCOL_LANG_BLOCK_SIZE, LangBlock, PROFILE_PROTOCOL_LANG_BLOCK_SIZE))
				goto error;
			if (Item == Protocol->LangpackBlocks)
			{
				if (PROFILE_FLASH_SaveLangpackHeader(Protocol->ProfileFlash, Protocol->LangpackBlocks, Protocol->LangpackType))
					goto error;
			}
			p = UintToStr( p, Item );
			strcpy( p, ";OK;" );
			p += 4;
		}
		else
		{
			goto error;
		}
	}
	else 
	{// ответ об ошибке
error:
		p = UintToStr( p, Item );
		strcpy( p, ";NA;" );
		p += 4;
	}
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );//Контрольная сумма и OD
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

int32_t PROFILE_PROTOCOL_ReadLangpack(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp)
{
	uint32_t Item;
	uint32_t LangpackBlocks, LangpackType;
	uint32_t error = 0, i;
	char *p;
	
	if( !StrToUint( cmd + 2, &Item, ';' ) )
		error = 1;
	
	p = rsp;
	memcpy(p, cmd, 2);
	p += 2;
	p = UintToStr( p, Item );// начинаем формировать ответ
	*p++ = ';';
	
	if((!error) && (Item < (PROFILE_FLASH_LANGS_SIZE / PROFILE_PROTOCOL_LANG_BLOCK_SIZE)))
	{// читаем блок из флеш, если он доступен
		if (Item == 0)
		{
			if (PROFILE_FLASH_LoadLangpackHeader(Protocol->ProfileFlash, &LangpackBlocks, &LangpackType))
				goto error;
			i = sprintf(p, "%d;%d", LangpackBlocks, LangpackType);
			for (;i < PROFILE_PROTOCOL_LANG_BLOCK_SIZE; i++)
			{
				p[i] = '\x03';
			}
			p += PROFILE_PROTOCOL_LANG_BLOCK_SIZE;
		}
		else
		{
			if (PROFILE_FLASH_LoadLangpackData(Protocol->ProfileFlash, (Item - 1) * PROFILE_PROTOCOL_LANG_BLOCK_SIZE, p, PROFILE_PROTOCOL_LANG_BLOCK_SIZE))
				goto error;
			p += PROFILE_PROTOCOL_LANG_BLOCK_SIZE;
		}
	}
	else
	{//блок не доступен - ошибка
error:
		strcpy( p,"NA" );
		p += 2;
	}
	*p++ =';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );//Контрольная сумма и OD
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

int32_t PROFILE_PROTOCOL_ConfigExt(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp)
{
	char *s, *p, *Data, c;
	uint32_t CRCRead, Index, DataSize, i;
	uint16_t CRCCalc;
	uint32_t BlocksCnt;
	
	s = cmd;
	p = rsp;
	memcpy(p, cmd, 2);
	p += 2;
	s += 2;
	
	if (!memcmp(s, PROFILE_PROTOCOL_CONFIG_STARTREAD, sizeof(PROFILE_PROTOCOL_CONFIG_STARTREAD) - 1))
	{
		s += sizeof(PROFILE_PROTOCOL_CONFIG_STARTREAD) - 1;
		if (*s != ';')
		{
			goto error;
		}
		
		s++;
		if( !StrToUint( s, &CRCRead, ';' ) ) 
		{
			goto error;
		}
		CRCCalc = Calc_CRC16_ASCII( cmd, s - cmd );
		if( CRCRead != CRCCalc )
		{
			goto error;
		}
		
		BlocksCnt = Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize % 256 ? (Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize / 256) + 1 : Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize / 256;
		BlocksCnt += 1;
		
		Protocol->Profile->ValuesLock = 1;
		osMutexWait( Protocol->Profile->Values->Mutex, osWaitForever );
		Protocol->Profile->ValuesCRC16 = Calc_CRC16( Protocol->Profile->Values->Static, Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize, 0);
		osMutexRelease( Protocol->Profile->Values->Mutex );
		
		
		*p++ = 'O';
		*p++ = 'K';
		*p++ =';';
		memcpy(p, PROFILE_PROTOCOL_CONFIG_STARTREAD, sizeof(PROFILE_PROTOCOL_CONFIG_STARTREAD) - 1);
		p += sizeof(PROFILE_PROTOCOL_CONFIG_STARTREAD) - 1;
		*p++ =';';
		p = UintToStr( p, BlocksCnt);
	}
	else if (!memcmp(s, PROFILE_PROTOCOL_CONFIG_READBLOCK, sizeof(PROFILE_PROTOCOL_CONFIG_READBLOCK) - 1))
	{
		s += sizeof(PROFILE_PROTOCOL_CONFIG_READBLOCK) - 1;
		if (*s != ';')
		{
			goto error;
		}
		
		BlocksCnt = Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize % 256 ? (Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize / 256) + 1 : Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize / 256;
		BlocksCnt += 1;
		
		s++;		
		if( !StrToUint( s, &Index, ';' ) ) 
		{
			goto error;
		}
		if (Index >= BlocksCnt)
		{
			goto error;
		}
		s = strchr( s, ';' );		
		if (*s != ';')
		{
			goto error;
		}
		
		s++;
		if( !StrToUint( s, &CRCRead, ';' ) ) 
		{
			goto error;
		}
		CRCCalc = Calc_CRC16_ASCII( cmd, s - cmd );
		if( CRCRead != CRCCalc )
		{
			goto error;
		}
		
		*p++ = 'O';
		*p++ = 'K';
		*p++ =';';
		memcpy(p, PROFILE_PROTOCOL_CONFIG_READBLOCK, sizeof(PROFILE_PROTOCOL_CONFIG_READBLOCK) - 1);
		p += sizeof(PROFILE_PROTOCOL_CONFIG_READBLOCK) - 1;
		*p++ =';';
		p = UintToStr( p, Index );
		*p++ =';';
		
		if (Index == 0)
		{
			Data = p;
			
			for (i = 0; i < sizeof(BSP_PROJECT); i++)
			{
				Data = ByteToHexStr(Data, BSP_PROJECT[i]);
			}
			
			for (i = 0; i < sizeof(BSP_HW_VERSION); i++)
			{
				Data = ByteToHexStr(Data, BSP_HW_VERSION[i]);
			}
			
			for (i = 0; i < sizeof(BSP_SW_VERSION); i++)
			{
				Data = ByteToHexStr(Data, BSP_SW_VERSION[i]);
			}
			
			for (i = 0; i < sizeof(BSP_MCU_SW_VERSION); i++)
			{
				Data = ByteToHexStr(Data, BSP_MCU_SW_VERSION[i]);
			}
			
			for (i = 0; i < sizeof(BSP_SW_DATE); i++)
			{
				Data = ByteToHexStr(Data, BSP_SW_DATE[i]);
			}
			
			for (i = 0; i < sizeof(BSP_SW_TIME); i++)
			{
				Data = ByteToHexStr(Data, BSP_SW_TIME[i]);
			}
			
			Data = WordToHexStr(Data, Protocol->Profile->ValuesCRC16);
			
			memset(Data, '0', 512 - (Data - p));
			p += 512;
		}
		else
		{
			osMutexWait( Protocol->Profile->Values->Mutex, osWaitForever );
			Data = &((char*)Protocol->Profile->Values->Static)[(Index - 1) * 256];
			DataSize = Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize - ((Index - 1) * 256);
			for (i = 0; i < 256; i++)
			{
				if (i < DataSize)
				{
					p = ByteToHexStr(p, Data[i]);
				}
				else
				{
					*p++ = '0';
					*p++ = '0';
				}
			}
			osMutexRelease( Protocol->Profile->Values->Mutex );
		}
	}
	else if (!memcmp(s, PROFILE_PROTOCOL_CONFIG_FINISHREAD, sizeof(PROFILE_PROTOCOL_CONFIG_FINISHREAD) - 1))
	{
		s += sizeof(PROFILE_PROTOCOL_CONFIG_FINISHREAD) - 1;
		if (*s != ';')
		{
			goto error;
		}
		
		s++;
		if( !StrToUint( s, &CRCRead, ';' ) )
		{
			goto error;
		}
		CRCCalc = Calc_CRC16_ASCII( cmd, s - cmd );
		if( CRCRead != CRCCalc )
		{
			goto error;
		}
		
		Protocol->Profile->ValuesLock = 0;
		
		*p++ = 'O';
		*p++ = 'K';
		*p++ =';';
		memcpy(p, PROFILE_PROTOCOL_CONFIG_FINISHREAD, sizeof(PROFILE_PROTOCOL_CONFIG_FINISHREAD) - 1);
		p += sizeof(PROFILE_PROTOCOL_CONFIG_FINISHREAD) - 1;
	}
	else if (!memcmp(s, PROFILE_PROTOCOL_CONFIG_STARTWRITE, sizeof(PROFILE_PROTOCOL_CONFIG_STARTWRITE) - 1))
	{
		s += sizeof(PROFILE_PROTOCOL_CONFIG_STARTWRITE) - 1;
		if (*s != ';')
		{
			goto error;
		}
		
		s++;
		if( !StrToUint( s, &CRCRead, ';' ) ) 
		{
			goto error;
		}
		CRCCalc = Calc_CRC16_ASCII( cmd, s - cmd );
		if( CRCRead != CRCCalc )
		{
			goto error;
		}
		
		Protocol->Profile->ValuesLock = 1;
		osMutexWait( Protocol->Profile->Values->Mutex, osWaitForever );
		Protocol->Profile->ValuesCRC16 = Calc_CRC16( Protocol->Profile->Values->Static, Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize, 0 );
		osMutexRelease( Protocol->Profile->Values->Mutex );
		
		BlocksCnt = Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize % 256 ? (Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize / 256) + 1 : Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize / 256;
		BlocksCnt += 1;
		
		*p++ = 'O';
		*p++ = 'K';
		*p++ =';';
		memcpy(p, PROFILE_PROTOCOL_CONFIG_STARTWRITE, sizeof(PROFILE_PROTOCOL_CONFIG_STARTWRITE) - 1);
		p += sizeof(PROFILE_PROTOCOL_CONFIG_STARTWRITE) - 1;
		*p++ =';';
		p = UintToStr( p, BlocksCnt);
	}
	else if (!memcmp(s, PROFILE_PROTOCOL_CONFIG_WRITEBLOCK, sizeof(PROFILE_PROTOCOL_CONFIG_WRITEBLOCK) - 1))
	{
		s += sizeof(PROFILE_PROTOCOL_CONFIG_WRITEBLOCK) - 1;
		if (*s != ';')
		{
			goto error;
		}
		
		BlocksCnt = Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize % 256 ? (Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize / 256) + 1 : Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize / 256;
		BlocksCnt += 1;
		
		s++;		
		if( !StrToUint( s, &Index, ';' ) ) 
		{
			goto error;
		}
		if (Index >= BlocksCnt)
		{
			goto error;
		}
		s = strchr( s, ';' );		
		if (*s != ';')
		{
			goto error;
		}
		
		s++;
		Data = s;
		s = strchr( s, ';' );		
		if (*s != ';')
		{
			goto error;
		}
		if ((s - Data) != 512)
		{
			goto error;
		}
		
		s++;
		if( !StrToUint( s, &CRCRead, ';' ) ) 
		{
			goto error;
		}
		CRCCalc = Calc_CRC16_ASCII( cmd, s - cmd );
		if( CRCRead != CRCCalc )
		{
			goto error;
		}
		
		if (Index == 0)
		{
			for (i = 0; i < sizeof(BSP_PROJECT); i++)
			{
				if (!HexStr2ToByte(Data, (uint8_t*)&c))
					goto error;
				if (c != BSP_PROJECT[i])
					goto error;
				Data += 2;
			}
			
			for (i = 0; i < sizeof(BSP_HW_VERSION); i++)
			{
				if (!HexStr2ToByte(Data, (uint8_t*)&c))
					goto error;
				if (c != BSP_HW_VERSION[i])
					goto error;
				Data += 2;
			}
			
			for (i = 0; i < sizeof(BSP_SW_VERSION); i++)
			{
				if (!HexStr2ToByte(Data, (uint8_t*)&c))
					goto error;
				if (c != BSP_SW_VERSION[i])
					goto error;
				Data += 2;
			}
			
			for (i = 0; i < sizeof(BSP_MCU_SW_VERSION); i++)
			{
				if (!HexStr2ToByte(Data, (uint8_t*)&c))
					goto error;
				if (c != BSP_MCU_SW_VERSION[i])
					goto error;
				Data += 2;
			}
			
			for (i = 0; i < sizeof(BSP_SW_DATE); i++)
			{
				if (!HexStr2ToByte(Data, (uint8_t*)&c))
					goto error;
				if (c != BSP_SW_DATE[i])
					goto error;
				Data += 2;
			}
			
			for (i = 0; i < sizeof(BSP_SW_TIME); i++)
			{
				if (!HexStr2ToByte(Data, (uint8_t*)&c))
					goto error;
				if (c != BSP_SW_TIME[i])
					goto error;
				Data += 2;
			}
			
			if (!HexStr4ToWord(Data, (uint16_t*)&CRCRead))
				goto error;
			Protocol->Profile->ValuesCRC16 = CRCRead;
		}
		else
		{
			osMutexWait( Protocol->Profile->Values->Mutex, osWaitForever );		
			for (i = 0; i < 256; i++)
			{
				HexStr2ToByte(Data, (uint8_t*)&c);
				if ((((Index - 1) * 256) + i) < Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize)
					((char*)Protocol->Profile->Values->Static)[((Index - 1) * 256) + i] = c;
				Data += 2;
			}
			osMutexRelease( Protocol->Profile->Values->Mutex );
		}
		
		*p++ = 'O';
		*p++ = 'K';
		*p++ =';';
		memcpy(p, PROFILE_PROTOCOL_CONFIG_WRITEBLOCK, sizeof(PROFILE_PROTOCOL_CONFIG_WRITEBLOCK) - 1);
		p += sizeof(PROFILE_PROTOCOL_CONFIG_WRITEBLOCK) - 1;
		*p++ =';';
		p = UintToStr( p, Index );
	}
	else if (!memcmp(s, PROFILE_PROTOCOL_CONFIG_FINISHWRITE, sizeof(PROFILE_PROTOCOL_CONFIG_FINISHWRITE) - 1))
	{
		s += sizeof(PROFILE_PROTOCOL_CONFIG_FINISHWRITE) - 1;
		if (*s != ';')
		{
			goto error;
		}
		
		s++;
		if( !StrToUint( s, &CRCRead, ';' ) )
		{
			goto error;
		}
		CRCCalc = Calc_CRC16_ASCII( cmd, s - cmd );
		if( CRCRead != CRCCalc )
		{
			goto error;
		}
		
		osMutexWait( Protocol->Profile->Values->Mutex, osWaitForever );	
		CRCCalc = Calc_CRC16( Protocol->Profile->Values->Static, Protocol->Profile->Descr[Protocol->Profile->Idx]->StaticValuesSize, 0 );
		osMutexRelease( Protocol->Profile->Values->Mutex );
		
		if (Protocol->Profile->ValuesCRC16 != CRCCalc)
		{
			PROFILE_FLASH_InitConfigs(Protocol->ProfileFlash);
			Protocol->Profile->ValuesLock = 0;		
			*p++ = 'N';
			*p++ = 'A';
			*p++ =';';
			memcpy(p, PROFILE_PROTOCOL_CONFIG_FINISHWRITE, sizeof(PROFILE_PROTOCOL_CONFIG_FINISHWRITE) - 1);
			p += sizeof(PROFILE_PROTOCOL_CONFIG_FINISHWRITE) - 1;
			*p++ =';';
			*p++ = '2';
		}
		else
		{
			if (Protocol->Profile->Descr[Protocol->Profile->Idx]->Table1Size )
				Protocol->Profile->TablesChanges |= (1 << PROFILE_TABLE_1);
			if (Protocol->Profile->Descr[Protocol->Profile->Idx]->Table2Size )
				Protocol->Profile->TablesChanges |= (1 << PROFILE_TABLE_2);
			if (Protocol->Profile->Descr[Protocol->Profile->Idx]->Table3Size )
				Protocol->Profile->TablesChanges |= (1 << PROFILE_TABLE_3);
			if (Protocol->Profile->Descr[Protocol->Profile->Idx]->Table4Size )
				Protocol->Profile->TablesChanges |= (1 << PROFILE_TABLE_4);
			if (Protocol->Profile->Descr[Protocol->Profile->Idx]->Table5Size )
				Protocol->Profile->TablesChanges |= (1 << PROFILE_TABLE_5);
			if (Protocol->Profile->Descr[Protocol->Profile->Idx]->Table8Size )
				Protocol->Profile->TablesChanges |= (1 << PROFILE_TABLE_8);
			if (Protocol->Profile->Descr[Protocol->Profile->Idx]->Table103Size )
				Protocol->Profile->TablesChanges |= (1 << PROFILE_TABLE_103);
			PROFILE_FLASH_SaveConfigs(Protocol->ProfileFlash);
			Protocol->Profile->ValuesLock = 0;			
			*p++ = 'O';
			*p++ = 'K';
			*p++ =';';
			memcpy(p, PROFILE_PROTOCOL_CONFIG_FINISHWRITE, sizeof(PROFILE_PROTOCOL_CONFIG_FINISHWRITE) - 1);
			p += sizeof(PROFILE_PROTOCOL_CONFIG_FINISHWRITE) - 1;
			*p++ =';';
			*p++ = '0';
		}
	}
	else
	{
		goto error;
	}
	
	*p++ =';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
	*p++ = '\r';
	*p = 0;
	return p - rsp;

error:
	*p++ = 'N';
	*p++ = 'A';
	*p++ =';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

void PROFILE_PROTOCOL_Init(PROFILE_PROTOCOL_T *Protocol, PROFILE_T *Profile, BOOTLOADER_T *Bootloader, PROFILE_FLASH_T *ProfileFlash)
{
	Protocol->Profile = Profile;
	Protocol->Bootloader = Bootloader;
	Protocol->ProfileFlash = ProfileFlash;
}

uint32_t PROFILE_PROTOCOL_Command(PROFILE_PROTOCOL_T *Protocol, char *cmd, char *rsp, uint32_t *complete)
{
	uint32_t len = 0;
	
	switch( cmd[ 1 ] )//Команда
	{
		case PROFILE_PROTOCOL_CMD_GET_CLASS://Чтение класса устройства:
			len = PROFILE_PROTOCOL_GetClass(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_GET_CLASS_EXT://Расширенное чтение класса устройства:
			len = PROFILE_PROTOCOL_GetClassExt(Protocol, cmd, rsp);
			break; 
		case PROFILE_PROTOCOL_CMD_SET_ACCESS://Разрешить запись в устройство
			len = PROFILE_PROTOCOL_SetAccess(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_SET_ACCESS_EXT://Расширенное разрешение на запись в устройство
			len = PROFILE_PROTOCOL_SetAccessExt(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_GET_VALUES_1:
		case PROFILE_PROTOCOL_CMD_GET_VALUES_2://Чтение параметров без раскраски, с раскраской 
			len = PROFILE_PROTOCOL_GetParams(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_SET_VALUE_1:
		case PROFILE_PROTOCOL_CMD_SET_VALUE_2://Запись парамерта в профиль
			len = PROFILE_PROTOCOL_SetParam(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_GET_PROFILE://Запрос строки из профиля
			len = PROFILE_PROTOCOL_GetProfile(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_SET_COLOR://Запись цветов	состояния
			len = PROFILE_PROTOCOL_SetColor(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_GET_COLORS://Чтение цветов	состояния
			len = PROFILE_PROTOCOL_GetColors(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_WRITE_LANG://Запись языкового пакета
			len = PROFILE_PROTOCOL_WriteLangpack(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_READ_LANG://Чтение языкового пакета
			len = PROFILE_PROTOCOL_ReadLangpack(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_UPDATE_FW:
			len = BOOTLOADER_PROTOCOL_UpdateFw(Protocol->Bootloader, cmd, rsp, complete);
			break;
		case PROFILE_PROTOCOL_CMD_CONFIG://Расширенное чтение/запись конфигурации устройства
			len = PROFILE_PROTOCOL_ConfigExt(Protocol, cmd, rsp);
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
