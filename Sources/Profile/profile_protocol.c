#include "profile_protocol.h"
#include <string.h>
#include "string_fnc.h"
#include "crc.h"
#include "user.h"

static uint32_t PROFILE_PROTOCOL_GetClass(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp )
{
	char *p = rsp;
	
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
	
	memcpy( p, cmd, 2 );
	p += 2;
	strcpy( p, Protocol->Profile->Descr[Protocol->Profile->Idx]->DeviceClass );
	p += strlen(p);
	*p++ = ';';
	
	if (Protocol->SetEnable)
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
	
	if (Protocol->SaveEnable)
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
	
	for (i = 0; i < Protocol->Profile->Descr[Protocol->Profile->Idx]->TablesCnt; i++)
	{
		if (Protocol->Profile->TablesChanges & PROFILE_TABLE_CNANGE_MASK(Protocol->Profile->Descr[Protocol->Profile->Idx]->Tables[i]->Index))
		{
			p = UintToStr( p, Protocol->Profile->Descr[Protocol->Profile->Idx]->Tables[i]->Index);
			*p++ = ';'; 
		}
	}
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
	*p++ = '\r';
	*p = 0;
	return p - rsp;	
}

static uint32_t PROFILE_PROTOCOL_SetAccess(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp )
{
	if( !strncmp( cmd + 2, "IPG", 3 ) )
	{
		Protocol->SetEnable = 1;
		memcpy( rsp, cmd, 2 );
		strcpy( rsp + 2, "OK\r");
	}
	else
	{
		Protocol->SetEnable = 0;
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
		Protocol->SetEnable = 1;
	}
	else if( (cmd[2] == 'N') && (cmd[3] == 'A') && (cmd[4] == ';')	)
	{
		Protocol->SetEnable = 0;
	}
	else
	{
		strcpy( p,"NA" );
		goto exit;
	}
	
	if( (cmd[5] == 'O') && (cmd[6] == 'K') && (cmd[7] == ';')	)
	{
		if (!Protocol->SaveEnable)
			PROFILE_FLASH_UpdateConfigs(Protocol->ProfileFlash);
		Protocol->SaveEnable = 1;
	}
	else if( (cmd[5] == 'N') && (cmd[6] == 'A') && (cmd[7] == ';')	)
	{
		Protocol->SaveEnable = 0;
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
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
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
		idx = 0xFFFFFFFF; 
	
	p = UintToStr( p, idx );
	*p++ = ';'; 
	if( idx == 0 )
	{
		p = UintToStr( p, *Protocol->Profile->Descr[Protocol->Profile->Idx]->TextSize ); 
	}
	else if( idx <= *Protocol->Profile->Descr[Protocol->Profile->Idx]->TextSize )
	{
		strcpy( p, Protocol->Profile->Descr[Protocol->Profile->Idx]->Text[ idx - 1 ] );
		p += strlen( p );
	}
	else
	{
		strcpy( p,"NA" );
		p += 2;
	}
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

static uint32_t PROFILE_PROTOCOL_GetParams(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp)
{	
	uint32_t TableIdx;
	uint32_t ParamIdx;
	PROFILE_TABLE_T *Table;
	PROFILE_TABLE_PARAM_T *Param;
	void *Value;
	char* p;
	uint32_t len;
	uint8_t Color;
	uint8_t State;
	
	if( !StrToUint( cmd + 2, &TableIdx, ';' ) )
		goto error;
	
	p = rsp;
	memcpy(p, cmd, 2);
	p += 2;
	p = UintToStr( p, TableIdx );
	*p++ = ';';
	
	if (PROFILE_GetTable(Protocol->Profile, TableIdx, &Table))
		goto error;
	for(ParamIdx = 1 ; ParamIdx <= Table->ParamsCnt; ParamIdx++ )
	{
		if (PROFILE_GetParam(Protocol->Profile, TableIdx, ParamIdx, &Param))
			goto error;
		if (PROFILE_GetParamState(Protocol->Profile, Param, &State))
			goto error;
		if (State != PROFILE_STATE_OOS)
		{
			if (PROFILE_GetParamValue(Protocol->Profile, Param, &Value))
				goto error;
			if (PROFILE_ParamValueToString(Protocol->Profile, Param, Value, p, &len))
				goto error;
			p += len;
			if ((cmd[1] == PROFILE_PROTOCOL_CMD_GET_VALUES_2) && (TableIdx == PROFILE_TABLE_2))
			{
				if (PROFILE_GetParamColor(Protocol->Profile, Param, Value, &Color))
					goto error;
				if ((State != PROFILE_STATE_IS) && (Color != PROFILE_COLOR_NA))
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
	Protocol->Profile->TablesChanges &= ~PROFILE_TABLE_CNANGE_MASK(TableIdx);
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
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
	uint32_t TableId;
	uint32_t Item = 0;
	char * p = cmd;
	uint32_t error = 0;
	uint32_t len;
	uint32_t CRCRead, CRCCalc;
	char *Value;
	char TempValue[PROFILE_STRING_MAX_LEN];
	
	
	p = cmd + 2;
	if( !StrToUint( p, &TableId, ';' ) )
	{
		error = 1;
	}
	if( ( p = strchr( p, ';' ) ) == NULL )
	{
		error = 1;		
	}
	if( !StrToUint( p + 1, &Item, ';' ) )
	{
		error = 1;
	}
	if( (  p = strchr(  p + 1, ';' ) ) == NULL )
	{
		error = 1;		
	}
	Value =  p + 1;	  
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
	{
		error = 1;		
	}
	if( strchr( p + 1, ';' ) )
	{
		error = 1;		 
	}
	if( p && !error )
	{	
		
		p++;		
		if( !StrToUint( p, &CRCRead, ';' ) )
			CRCRead = 0xFFFFFFFF;
		CRCCalc = Calc_CRC16_ASCII( cmd, p - cmd );
		*--p = 0;
		if( CRCRead != CRCCalc )
		{
			error = 1; 
		}
	}
	else
	{
		error = 1;
	}
	
	p = rsp;
	memcpy(p, cmd, 2);
	p += 2;
	p = UintToStr( p, TableId );
	*p++ = ';';
	p = UintToStr( p, Item );
	*p++ = ';';
	
	if ((!error) && (Protocol->SetEnable))
	{	
		if (PROFILE_StringToValue(Protocol->Profile, TableId, Item, TempValue, Value))
		{
			goto error;
		}
		if (PROFILE_CheckValue(Protocol->Profile, TableId, Item, TempValue, 0))
		{
			goto error;
		}
		if (PROFILE_SetValue(Protocol->Profile, TableId, Item, TempValue))
		{
			goto error;
		}
		if (PROFILE_SetValueDependencies(Protocol->Profile, TableId, Item, TempValue, 0))
		{
			goto error;
		}
		if (Protocol->SaveEnable)
		{
			PROFILE_FLASH_UpdateValue(Protocol->ProfileFlash, TableId, Item);
		}
		if (PROFILE_GetValue(Protocol->Profile, TableId, Item, TempValue))
		{
			goto error;
		}
		if (PROFILE_ValueToString(Protocol->Profile, TableId, Item, TempValue, p , &len))
		{
			goto error;
		}
		p += len;
	}
	else 
	{
		strcpy( p, "NA" );
		p += 2;
	}
	*p++ = ';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
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

int32_t PROFILE_PROTOCOL_SetColor(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp)
{
	uint32_t TableIdx = 0;
	uint32_t ParamIdx = 0;
	uint32_t tmp;
	uint8_t ColorDegrade; 
	uint8_t ColorFailure; 
	PROFILE_TABLE_T *Table;
	int32_t error = 0;
	char *p = rsp;
	uint32_t CRCRead, CRCCalc;
	
	
	p = cmd + 2;
	if( !StrToUint( p, &TableIdx, ';' ) )
		error = 1;
	if( ( p = strchr( p, ';' ) ) == NULL )
		error = 1;		
	if( !StrToUint( p + 1, &ParamIdx, ';' ) )
		error = 1;
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
		error = 1;		
	if( !StrToUint( p + 1, &tmp, ';' ) )
		error = 1; 
	ColorDegrade = tmp;
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
		error = 1;		
	if( !StrToUint( p + 1, &tmp, ';' ) )
		error = 1;
	ColorFailure = tmp;
	if( ( p = strchr( p + 1, ';' ) ) == NULL )
		error = 1;		
	if( strchr( p + 1, ';' ) )
		error = 1;			 
	if( p && !error )
	{	
		
		p++;
		if( !StrToUint( p, &CRCRead, ';' ) ) 
			CRCRead = 0xFFFFFFFF;
		CRCCalc = Calc_CRC16_ASCII( cmd, p - cmd );
		*--p = 0;
		if( CRCRead != CRCCalc )
			error = 1; 
	}
	else 
	{
		error = 1;
	}

	memcpy(rsp, cmd, 2);
	p = rsp + 2;
	
	if (PROFILE_GetTable(Protocol->Profile, TableIdx, &Table))
		goto error;
	
	
	if( !error && Protocol->SetEnable && TableIdx == PROFILE_TABLE_2 && ParamIdx && ParamIdx <= Table->ParamsCnt)
	{
		if (PROFILE_SetAlarmColors(Protocol->Profile, PROFILE_TABLE_2, ParamIdx, ColorDegrade, ColorFailure))
			goto error;
		if (Protocol->SaveEnable)
		{
			PROFILE_FLASH_UpdateConfigs(Protocol->ProfileFlash);
		}
		if (PROFILE_GetAlarmColors(Protocol->Profile, PROFILE_TABLE_2, ParamIdx, &ColorDegrade, &ColorFailure))
			goto error;
		p = UintToStr( p, TableIdx );
		*p++ = ';';
		p = UintToStr( p, ParamIdx );
		*p++ = ';';
		p = UintToStr( p, ColorDegrade ); 
		*p++ = ';';
		p = UintToStr( p, ColorFailure );
		*p++ = ';';
	}
	else
	{
		p = UintToStr( p, TableIdx );
		*p++ = ';';
		p = UintToStr( p, ParamIdx );
		strcpy( p, ";NA;" ); 
		p += 4;
	}
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
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

int32_t PROFILE_PROTOCOL_GetColors(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp)
{
	uint32_t TableId;
	char* p = rsp;
	PROFILE_TABLE_T *Table;
	uint32_t i;
	uint8_t ColorDegrade; 
	uint8_t ColorFailure; 
	
	if( !StrToUint( cmd + 2, &TableId, ';' ) )
		goto error;
	
	memcpy(p ,cmd ,2);
	p += 2;	
	p = UintToStr( p, TableId );
	*p++ = ';';
	
	if (PROFILE_GetTable(Protocol->Profile, TableId, &Table))
		goto error;
	
	if( TableId == 2 )
	{
		
		for( i = 1; i <= Table->ParamsCnt; i++ )
		{
			if (PROFILE_GetAlarmColors(Protocol->Profile, PROFILE_TABLE_2, i, &ColorDegrade, &ColorFailure))
				goto error;
			p = UintToStr( p, ColorDegrade );
			*p++ = '@';
			p = UintToStr( p, ColorFailure );
			*p++ = ';';
		}
	}
	else 
	{
		strcpy( p, "NA;" ); 
		p += 3;
	}
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
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

	
	p = cmd + 2;
	if( !StrToUint( p, &Item, ';' ) )
		error = 1;
	if( ( p = strchr( p, ';' ) ) == NULL )
		error = 1;		
	LangBlock = p + 1;

	p = rsp;
	memcpy(p , cmd, 2);
	p += 2;
	if( !error && Item < PROFILE_FLASH_LANGS_SIZE / PROFILE_PROTOCOL_LANG_BLOCK_SIZE)
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
			if (PROFILE_FLASH_UpdateLangpackData(Protocol->ProfileFlash, (Item - 1) * PROFILE_PROTOCOL_LANG_BLOCK_SIZE, LangBlock, PROFILE_PROTOCOL_LANG_BLOCK_SIZE))
				goto error;
			if (Item == Protocol->LangpackBlocks)
			{
				if (PROFILE_FLASH_UpdateLangpackHeader(Protocol->ProfileFlash, Protocol->LangpackBlocks, Protocol->LangpackType))
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
	{
error:
		p = UintToStr( p, Item );
		strcpy( p, ";NA;" );
		p += 4;
	}
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
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
	p = UintToStr( p, Item );
	*p++ = ';';
	
	if( !error && Item < PROFILE_FLASH_LANGS_SIZE / PROFILE_PROTOCOL_LANG_BLOCK_SIZE )
	{
		if (Item == 0)
		{
			if (PROFILE_FLASH_ReadLangpackHeader(Protocol->ProfileFlash, &LangpackBlocks, &LangpackType))
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
			if (PROFILE_FLASH_ReadLangpackData(Protocol->ProfileFlash, (Item - 1) * PROFILE_PROTOCOL_LANG_BLOCK_SIZE, p, PROFILE_PROTOCOL_LANG_BLOCK_SIZE))
				goto error;
			p += PROFILE_PROTOCOL_LANG_BLOCK_SIZE;
		}
	}
	else
	{
error:
		strcpy( p,"NA" );
		p += 2;
	}
	*p++ =';';
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
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
			for (i = 0; i < Protocol->Profile->Descr[Protocol->Profile->Idx]->TablesCnt; i++)
			{
				Protocol->Profile->TablesChanges |= PROFILE_TABLE_CNANGE_MASK(Protocol->Profile->Descr[Protocol->Profile->Idx]->Tables[i]->Index);
			}
			if (Protocol->SaveEnable)
			{
				PROFILE_FLASH_UpdateConfigs(Protocol->ProfileFlash);
			}
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

static uint32_t PROFILE_PROTOCOL_GetUsers(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp )
{
	USER_T User;
	char *p = rsp;
	uint32_t i;
	uint16_t CalcCRC16;
	
	memcpy( p, cmd, 2 );
	p += 2;
	
	p += sprintf(p, "%d;", USER_MAX_CNT);
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		USER_GetUserByIndex(i, &User);
		CalcCRC16 = Calc_CRC16((char*)&User, sizeof(User), 0);
		if (Protocol->UsersCrc16[i] != CalcCRC16)
		{
			p += sprintf(p, "%d;", i + 1);
			Protocol->UsersCrc16[i] = CalcCRC16;
		}
	}
	p = UintToStr( p, Calc_CRC16_ASCII( rsp, p - rsp ) );
	*p++ = '\r';
	*p = 0;
	return p - rsp;
}

static uint32_t PROFILE_PROTOCOL_GetUserAttr(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp )
{
	USER_T User;
	char *c, *p = rsp;
	uint32_t UserIdx;
	
	memcpy( p, cmd, 2 );
	p += 2;
	
	UserIdx = strtoul(cmd + 2, &c, 10);
	if( *c != '\r' )
		goto error;
	
	p += sprintf(p, "%d;", UserIdx);
	if (UserIdx == 0)
	{
		p += sprintf(p, "login;passhash;group;name;email;tel;snmphash;");
	}
	else if ((UserIdx > 0) & (UserIdx <= USER_MAX_CNT))
	{
		USER_GetUserByIndex(UserIdx - 1, &User);
		p += sprintf(p, "%s;%s;%s;%s;%s;%s;%s;", User.Login, User.PassHash, User.Group, User.Name, User.Email, User.Tel, User.SnmpHash);
	}
	else
	{
		p += sprintf(p, "NA;");
	}
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

static uint32_t PROFILE_PROTOCOL_SetUserAttr(PROFILE_PROTOCOL_T *Protocol, char * cmd, char * rsp )
{
	USER_T User;
	char *c1, *c2, *p = rsp;
	uint32_t UserIdx;
	uint32_t len;
	uint16_t ReadCrc16, CalcCrc16;
	
	memcpy( p, cmd, 2 );
	p += 2;
	
	c1 = cmd + 2;
	UserIdx = strtoul(cmd + 2, &c2, 10);
	if( *c2 != ';' )
		goto error;
	
	c1 = c2 + 1;
	c2 = strchr(c1, ';');
	if (*c2 != ';')
		goto error;
	len = c2 - c1;
	if (len >= sizeof(User.Login))
		goto error;
	memcpy(User.Login, c1, len);
	User.Login[len] = 0;
	
	c1 = c2 + 1;
	c2 = strchr(c1, ';');
	if (*c2 != ';')
		goto error;
	len = c2 - c1;
	if (len >= sizeof(User.PassHash))
		goto error;
	memcpy(User.PassHash, c1, len);
	User.PassHash[len] = 0;
	
	c1 = c2 + 1;
	c2 = strchr(c1, ';');
	if (*c2 != ';')
		goto error;
	len = c2 - c1;
	if (len >= sizeof(User.Group))
		goto error;
	memcpy(User.Group, c1, len);
	User.Group[len] = 0;
	
	c1 = c2 + 1;
	c2 = strchr(c1, ';');
	if (*c2 != ';')
		goto error;
	len = c2 - c1;
	if (len >= sizeof(User.Name))
		goto error;
	memcpy(User.Name, c1, len);
	User.Name[len] = 0;
	
	c1 = c2 + 1;
	c2 = strchr(c1, ';');
	if (*c2 != ';')
		goto error;
	len = c2 - c1;
	if (len >= sizeof(User.Email))
		goto error;
	memcpy(User.Email, c1, len);
	User.Email[len] = 0;
	
	c1 = c2 + 1;
	c2 = strchr(c1, ';');
	if (*c2 != ';')
		goto error;
	len = c2 - c1;
	if (len >= sizeof(User.Tel))
		goto error;
	memcpy(User.Tel, c1, len);
	User.Tel[len] = 0;
	
	c1 = c2 + 1;
	c2 = strchr(c1, ';');
	if (*c2 != ';')
		goto error;
	len = c2 - c1;
	if (len >= sizeof(User.SnmpHash))
		goto error;
	memcpy(User.SnmpHash, c1, len);
	User.SnmpHash[len] = 0;
	
	c1 = c2 + 1;
	ReadCrc16 = strtoul(c1, &c2, 10);
	if( *c2 != '\r' )
		goto error;
	
	CalcCrc16 = Calc_CRC16(cmd, c1 - cmd, 0);
	if (CalcCrc16 != ReadCrc16)
		goto error;
	
	p += sprintf(p, "%d;", UserIdx);
	if ((UserIdx > 0) & (UserIdx <= USER_MAX_CNT))
	{
		if (USER_UpdateUserByIndex(UserIdx - 1, &User))
			goto error;
		p += sprintf(p, "OK;");
	}
	else
	{
		p += sprintf(p, "NA;");
	}
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
	Protocol->SetEnable = 0;
	Protocol->SaveEnable = 1;
}

uint32_t PROFILE_PROTOCOL_Command(PROFILE_PROTOCOL_T *Protocol, char *cmd, char *rsp, uint32_t *complete)
{
	uint32_t len = 0;
	
	switch( cmd[ 1 ] )
	{
		case PROFILE_PROTOCOL_CMD_GET_CLASS:
			len = PROFILE_PROTOCOL_GetClass(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_GET_CLASS_EXT:
			len = PROFILE_PROTOCOL_GetClassExt(Protocol, cmd, rsp);
			break; 
		case PROFILE_PROTOCOL_CMD_SET_ACCESS:
			len = PROFILE_PROTOCOL_SetAccess(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_SET_ACCESS_EXT:
			len = PROFILE_PROTOCOL_SetAccessExt(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_GET_VALUES_1:
		case PROFILE_PROTOCOL_CMD_GET_VALUES_2:
			len = PROFILE_PROTOCOL_GetParams(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_SET_VALUE_1:
		case PROFILE_PROTOCOL_CMD_SET_VALUE_2:
			len = PROFILE_PROTOCOL_SetParam(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_GET_PROFILE:
			len = PROFILE_PROTOCOL_GetProfile(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_SET_COLOR:
			len = PROFILE_PROTOCOL_SetColor(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_GET_COLORS:
			len = PROFILE_PROTOCOL_GetColors(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_WRITE_LANG:
			len = PROFILE_PROTOCOL_WriteLangpack(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_READ_LANG:
			len = PROFILE_PROTOCOL_ReadLangpack(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_UPDATE_FW:
			len = BOOTLOADER_PROTOCOL_UpdateFw(Protocol->Bootloader, cmd, rsp, complete);
			break;
		case PROFILE_PROTOCOL_CMD_CONFIG:
			len = PROFILE_PROTOCOL_ConfigExt(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_GET_USERS:
			len = PROFILE_PROTOCOL_GetUsers(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_GET_USER_ATTR:
			len = PROFILE_PROTOCOL_GetUserAttr(Protocol, cmd, rsp);
			break;
		case PROFILE_PROTOCOL_CMD_SET_USER_ATTR:
			len = PROFILE_PROTOCOL_SetUserAttr(Protocol, cmd, rsp);
			break;
	}
	if( len == 0 )
	{
		memcpy(rsp, cmd, 2);
		strcpy( rsp + 2, "NA\r" );
		len = 5;
	}
	return len;
}
