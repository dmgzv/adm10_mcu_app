#include "user.h"
#include "bsp_flash.h"
#include "crc.h"
#include "log.h"
#include "tiny-json.h"
#include <string.h>
#include <stdio.h>

#define USER_BUFFER_SIZE		(2 * BSP_FLASH_PAGE_SIZE)
#define USER_ADDR(UserIdx)	(BSP_FLASH_USER_ADDR + ((UserIdx) * USER_BUFFER_SIZE))

static osMutexId UserMutexId;
static osMutexDef(USER);
static char UserBuffer[USER_BUFFER_SIZE];

static USER_T __attribute__((section ("CCM_DATA"))) Users[USER_MAX_CNT];

static const USER_T DefaultUsers[] = {
	{
		.Login = USER_DEFAULT_SUPERUSER_LOGIN,
		.PassHash = USER_DEFAULT_SUPERUSER_PASSHASH,
		.Group = USER_DEFAULT_SUPERUSER_GROUP,
		.Name = USER_DEFAULT_SUPERUSER_NAME,
		.Tel = USER_DEFAULT_SUPERUSER_TEL,
		.Email = USER_DEFAULT_SUPERUSER_EMAIL,
		.SnmpHash = USER_DEFAULT_SUPERUSER_SNMPHASH,
	},
	{
		.Login = USER_DEFAULT_ADMIN_LOGIN,
		.PassHash = USER_DEFAULT_ADMIN_PASSHASH,
		.Group = USER_DEFAULT_ADMIN_GROUP,
		.Name = USER_DEFAULT_ADMIN_NAME,
		.Tel = USER_DEFAULT_ADMIN_TEL,
		.Email = USER_DEFAULT_ADMIN_EMAIL,
		.SnmpHash = USER_DEFAULT_ADMIN_SNMPHASH,
	},
};

static uint32_t DefaultUsersCnt = sizeof(DefaultUsers) / sizeof(DefaultUsers[0]);

static int32_t __USER_ReadUser(USER_T *User, uint32_t Idx)
{
	uint32_t Addr;
	uint16_t CalcCrc16, ReadCrc16;
	json_t mem[16];
	json_t const* json;
	json_t const* jsonProp;
	char *Data;
	uint32_t DataSize;
	
	Addr = USER_ADDR(Idx);
	if (BSP_FLASH_Read(Addr, UserBuffer, sizeof(UserBuffer)))
		return -1;
	ReadCrc16 = *(uint16_t*)UserBuffer;
	CalcCrc16 = Calc_CRC16(UserBuffer + 2, sizeof(UserBuffer) - 2, 0);
	if (ReadCrc16 != CalcCrc16)
		return 1;
	
	json = json_create((UserBuffer + 2), mem, sizeof(mem) / sizeof(mem[0]) );
	if ( !json )
		return 1;
	
	jsonProp = json_getProperty( json, "login" );
	if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		return 1;	
	Data = (char*)json_getValue( jsonProp ); 
	DataSize = strlen(Data);
	if (DataSize >= sizeof(User->Login ))
		return 1;
	strcpy(User->Login, Data);
	
	jsonProp = json_getProperty( json, "passhash" );
	if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		return 1;
	Data = (char*)json_getValue( jsonProp ); 
	DataSize = strlen(Data);
	if (DataSize >= sizeof(User->PassHash ))
		return 1;
	strcpy(User->PassHash, Data);
	
	jsonProp = json_getProperty( json, "snmphash" );
	if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		return 1;
	Data = (char*)json_getValue( jsonProp ); 
	DataSize = strlen(Data);
	if (DataSize >= sizeof(User->SnmpHash ))
		return 1;
	strcpy(User->SnmpHash, Data);
	
	jsonProp = json_getProperty( json, "group" );
	if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		return 1;
	Data = (char*)json_getValue( jsonProp ); 
	DataSize = strlen(Data);
	if (DataSize >= sizeof(User->Group ))
		return 1;
	strcpy(User->Group, Data);
	
	jsonProp = json_getProperty( json, "name" );
	if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		strcpy(User->Name, "");
	else
	{
		Data = (char*)json_getValue( jsonProp ); 
		DataSize = strlen(Data);
		if (DataSize >= sizeof(User->Name ))
			strcpy(User->Name, "");
		else
			strcpy(User->Name, Data);
	}
	
	jsonProp = json_getProperty( json, "tel" );
	if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		strcpy(User->Tel, "");
	else
	{
		Data = (char*)json_getValue( jsonProp ); 
		DataSize = strlen(Data);
		if (DataSize >= sizeof(User->Tel ))
			strcpy(User->Tel, "");
		else
			strcpy(User->Tel, Data);
	}
	
	jsonProp = json_getProperty( json, "email" );
	if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		strcpy(User->Email, "");
	else
	{
		Data = (char*)json_getValue( jsonProp ); 
		DataSize = strlen(Data);
		if (DataSize >= sizeof(User->Email ))
			strcpy(User->Email, "");
		else
			strcpy(User->Email, Data);
	}
	return 0;
}

static int32_t __USER_CreateUser(USER_T *User, uint32_t Idx)
{
	char *Data;
	uint32_t DataSize;
	uint16_t CalcCrc16;
	uint32_t Addr, Size;
	
	if (Users[Idx].Login[0])
		goto error;
	
	memset(UserBuffer, 0, sizeof(UserBuffer));
	Data = UserBuffer + 2;
	DataSize = 0;
	DataSize += sprintf(&Data[DataSize], "{");
	DataSize += sprintf(&Data[DataSize], "\"login\":\"%s\"", User->Login);
	DataSize += sprintf(&Data[DataSize], "\"passhash\":\"%s\"", User->PassHash);
	DataSize += sprintf(&Data[DataSize], "\"snmphash\":\"%s\"", User->SnmpHash);
	DataSize += sprintf(&Data[DataSize], "\"group\":\"%s\"", User->Group);
	DataSize += sprintf(&Data[DataSize], "\"name\":\"%s\"", User->Name);
	DataSize += sprintf(&Data[DataSize], "\"tel\":\"%s\"", User->Tel);
	DataSize += sprintf(&Data[DataSize], "\"email\":\"%s\"", User->Email);
	DataSize += sprintf(&Data[DataSize], "}");
	CalcCrc16 = Calc_CRC16(UserBuffer + 2, sizeof(UserBuffer) - 2, 0);
	*(uint16_t*)UserBuffer = CalcCrc16;
	
	Addr = USER_ADDR(Idx);
	Data = UserBuffer;
	DataSize = sizeof(UserBuffer);
	while (DataSize)
	{
		if (DataSize >= BSP_FLASH_PAGE_SIZE)
			Size = BSP_FLASH_PAGE_SIZE;
		else
			Size = DataSize;
		
		if (BSP_FLASH_ProgramPage(Addr, Data, Size))
			goto error;
		
		DataSize -= Size;
		Data += Size;
		Addr += Size;
	}
	memcpy(&Users[Idx], User, sizeof(Users[0]));
	LOG_Info("USER", "", "UserCreate", "%d;%s", Idx, User->Login);
	return 0;
	
error:
	LOG_Error("USER", "", "UserCreateError", "%d;%s", Idx, User->Login);
	return -1;
}

static int32_t __USER_UpdateUser(USER_T *User, uint32_t Idx)
{
	char *Data;
	uint32_t DataSize;
	uint16_t CalcCrc16;
	uint32_t Addr, Size;
	
	if (!Users[Idx].Login[0])
		goto error;
	
	memset(UserBuffer, 0, sizeof(UserBuffer));
	Data = UserBuffer + 2;
	DataSize = 0;
	DataSize += sprintf(&Data[DataSize], "{");
	DataSize += sprintf(&Data[DataSize], "\"login\":\"%s\"", User->Login);
	DataSize += sprintf(&Data[DataSize], "\"passhash\":\"%s\"", User->PassHash);
	DataSize += sprintf(&Data[DataSize], "\"snmphash\":\"%s\"", User->SnmpHash);
	DataSize += sprintf(&Data[DataSize], "\"group\":\"%s\"", User->Group);
	DataSize += sprintf(&Data[DataSize], "\"name\":\"%s\"", User->Name);
	DataSize += sprintf(&Data[DataSize], "\"tel\":\"%s\"", User->Tel);
	DataSize += sprintf(&Data[DataSize], "\"email\":\"%s\"", User->Email);
	DataSize += sprintf(&Data[DataSize], "}");
	CalcCrc16 = Calc_CRC16(UserBuffer + 2, sizeof(UserBuffer) - 2, 0);
	*(uint16_t*)UserBuffer = CalcCrc16;
	
	Addr = USER_ADDR(Idx);
	Data = UserBuffer;
	DataSize = sizeof(UserBuffer);
	while (DataSize)
	{
		if (DataSize >= BSP_FLASH_PAGE_SIZE)
			Size = BSP_FLASH_PAGE_SIZE;
		else
			Size = DataSize;
		
		if (BSP_FLASH_ProgramPage(Addr, Data, Size))
			goto error;
		
		DataSize -= Size;
		Data += Size;
		Addr += Size;
	}
	memcpy(&Users[Idx], User, sizeof(Users[0]));
	LOG_Info("USER", "", "UserUpdate", "%d;%s", Idx, User->Login);
	return 0;
	
error:
	LOG_Error("USER", "", "UserUpdateError", "%d;%s", Idx, User->Login);
	return -1;
}

static int32_t __USER_DeleteUser(uint32_t Idx)
{
	uint32_t Addr;
	
	Addr = USER_ADDR(Idx);
	
	if (BSP_FLASH_ErasePage(Addr))
		goto error;
	
	LOG_Info("USER", "", "UserDelete", "%d;%s", Idx, Users[Idx].Login);
	memset(&Users[Idx], 0, sizeof(Users[0]));
	return 0;
	
error:
	LOG_Error("USER", "", "UserDeleteError", "%d;%s", Idx, Users[Idx].Login);
	return -1;
}

int32_t USER_Init(void)
{
	int32_t Error;
	uint32_t i;
	USER_T User;
	char IdxStr[4];
	
	if (!UserMutexId)
		UserMutexId = osMutexCreate(osMutex(USER));
	if (!UserMutexId)
		return -1;
	
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (i < DefaultUsersCnt)
			memcpy(&Users[i], &DefaultUsers[i], sizeof(Users[0]));
		else
			memset(&Users[i], 0, sizeof(Users[0]));
	}
	
	osMutexWait(UserMutexId, osWaitForever);
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		Error = __USER_ReadUser(&User, i);
		if (!Error)
		{
			memcpy(&Users[i], &User, sizeof(User));
			if (User.Login[0])
			{
				sprintf(IdxStr, "%d", i);
				LOG_Info("USER", "", "UserInit", "%d;%s", i, User.Login);
			}
		}
	}
	osMutexRelease(UserMutexId);
	return 0;
}

int32_t USER_CreateUser(USER_T *User)
{
	uint32_t i;
	
	if (!UserMutexId)
		return -1;
	
	if (!User)
		return -1;
	
	/*Check min length of login*/
	if (strlen(User->Login) < 3)
		return -1;
	
	/*Check valid symbols of login*/
	for (i = 0; i < sizeof(User->Login); i++)
	{
		if (User->Login[i] == 0)
			break;
		if ((User->Login[i] >= '0') && (User->Login[i] <= '9'))
			continue;
		if ((User->Login[i] >= 'a') && (User->Login[i] <= 'z'))
			continue;
		return -1;
	}
	
	/*Check length of PassHash*/
	if (strlen(User->PassHash) != (sizeof(User->PassHash) - 1))
		return -1;
	
	/*Check length of SnmpHash*/
	if (strlen(User->SnmpHash) != (sizeof(User->SnmpHash) - 1))
		return -1;
	
	if ((strcmp(User->Group, USER_GROUP_MONITOR)) && (strcmp(User->Group, USER_GROUP_USER)) && (strcmp(User->Group, USER_GROUP_ADMIN)) && (strcmp(User->Group, USER_GROUP_SUPERUSER)))
		return -1;
	
	osMutexWait(UserMutexId, osWaitForever);
	/*Do not allow to create user with existing login*/
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (Users[i].Login[0])
		{
			if (!strcmp(Users[i].Login, User->Login))
				goto error;
		}
	}
	
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (!Users[i].Login[0])
		{
			if (__USER_CreateUser(User, i))
				goto error;
			break;
		}
	}
	osMutexRelease(UserMutexId);
	if (i == USER_MAX_CNT)
		return -1;
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

int32_t USER_GetUserByIndex(uint32_t Idx, USER_T *User)
{
	if (!UserMutexId)
		return -1;
	
	if ((Idx >= USER_MAX_CNT) || (!User))
		return -1;
	
	osMutexWait(UserMutexId, osWaitForever);
	memcpy(User, &Users[Idx], sizeof(Users[0]));	
	osMutexRelease(UserMutexId);
	return 0;
}

int32_t USER_GetUserByLogin(char* Login, USER_T *User)
{
	uint32_t i;
	
	if (!UserMutexId)
		return -1;
	
	if ((!Login) || (!User))
		return -1;
	
	/*Check min length of login*/
	if (strlen(Login) < 3)
		return -1;
	
	osMutexWait(UserMutexId, osWaitForever);	
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (!strcmp(Login, Users[i].Login))
		{
			memcpy(User, &Users[i], sizeof(Users[0]));
			break;
		}
	}
	osMutexRelease(UserMutexId);
	if (i == USER_MAX_CNT)
		return -1;
	return 0;
}

int32_t USER_UpdateUserByIndex(uint32_t Idx, USER_T *User)
{
	if (!UserMutexId)
		return -1;
	
	if ((Idx >= USER_MAX_CNT) || (!User))
		return -1;
	
	osMutexWait(UserMutexId, osWaitForever);
	if ((Users[Idx].Login[0]) && (User->Login[0]))
	{
		if (__USER_UpdateUser(User, Idx))
			goto error;
	}
	else if ((!Users[Idx].Login[0]) && (User->Login[0]))
	{
		if (__USER_CreateUser(User, Idx))
			goto error;
	}
	else if ((Users[Idx].Login[0]) && (!User->Login[0]))
	{
		if (__USER_DeleteUser(Idx))
			goto error;
	}
	osMutexRelease(UserMutexId);
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

int32_t USER_UpdateUserByLogin(char* Login, USER_T *User)
{
	uint32_t i;
	
	if (!UserMutexId)
		return -1;
	
	if ((!Login) || (!User))
		return -1;
	
	/*Check min length of login*/
	if (strlen(Login) < 3)
		return -1;
	
	/*Check valid symbols of login*/
	for (i = 0; i < sizeof(User->Login); i++)
	{
		if (User->Login[i] == 0)
			break;
		if ((User->Login[i] >= '0') && (User->Login[i] <= '9'))
			continue;
		if ((User->Login[i] >= 'a') && (User->Login[i] <= 'z'))
			continue;
		return -1;
	}
	
	/*Check length of PassHash*/
	if (strlen(User->PassHash) != (sizeof(User->PassHash) - 1))
		return -1;
	
	/*Check length of SnmpHash*/
	if (strlen(User->SnmpHash) != (sizeof(User->SnmpHash) - 1))
		return -1;
	
	if ((strcmp(User->Group, USER_GROUP_MONITOR)) && (strcmp(User->Group, USER_GROUP_USER)) && (strcmp(User->Group, USER_GROUP_ADMIN)) && (strcmp(User->Group, USER_GROUP_SUPERUSER)))
		return -1;
	
	osMutexWait(UserMutexId, osWaitForever);
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (!strcmp(Users[i].Login, Login))
		{
			if (__USER_UpdateUser(User, i))
				goto error;
			break;
		}
	}
	osMutexRelease(UserMutexId);
	if (i == USER_MAX_CNT)
		return -1;
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

int32_t USER_DeleteUserByIndex(uint32_t Idx)
{
	if (!UserMutexId)
		return -1;
	
	if (Idx >= USER_MAX_CNT)
		return -1;
	
	osMutexWait(UserMutexId, osWaitForever);
	if (__USER_DeleteUser(Idx))
		goto error;
	osMutexRelease(UserMutexId);
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

int32_t USER_DeleteUserByLogin(char* Login)
{
	uint32_t i;
	uint32_t SuperusersCnt, AdminsCnt;
	
	if (!UserMutexId)
		return -1;
	
	if (!Login)
		return -1;
	
	/*Check min length of login*/
	if (strlen(Login) < 3)
		return -1;
	
	osMutexWait(UserMutexId, osWaitForever);
	SuperusersCnt = 0;
	AdminsCnt = 0;
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (!strcmp(Users[i].Group, USER_GROUP_ADMIN))
			AdminsCnt++;
		else if (!strcmp(Users[i].Group, USER_GROUP_SUPERUSER))
			SuperusersCnt++;
	}
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (!strcmp(Login, Users[i].Login))
		{
			/*Do not allow to delete last admin*/
			if ((AdminsCnt == 1) && (!strcmp(Users[i].Group, USER_GROUP_ADMIN)))
				goto error;
			/*Do not allow to delete last superuser*/
			if ((SuperusersCnt == 1) && (!strcmp(Users[i].Group, USER_GROUP_SUPERUSER)))
				goto error;
			if (__USER_DeleteUser(i))
				goto error;
			break;
		}
	}
	osMutexRelease(UserMutexId);
	if (i == USER_MAX_CNT)
		return -1;
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

int32_t USER_Reset(void)
{
	uint32_t i;
	
	if (!UserMutexId)
		return -1;
	
	osMutexWait(UserMutexId, osWaitForever);
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (i < DefaultUsersCnt)
		{
			if (Users[i].Login[0])
			{
				if (__USER_UpdateUser((USER_T*)&DefaultUsers[i], i))
					goto error;
			}
			else
			{
				if (__USER_CreateUser((USER_T*)&DefaultUsers[i], i))
					goto error;
			}
		}
		else
		{
			if (__USER_DeleteUser(i))
				goto error;
		}
	}
	osMutexRelease(UserMutexId);
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

