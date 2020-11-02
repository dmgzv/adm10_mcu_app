#include "user.h"
#include "bsp_flash.h"
#include "crc.h"
#include "log.h"
#include "tiny-json.h"
#include <string.h>

#define USER_VALUE_TYPE_STRING

#define USER_BUFFER_SIZE		(2 * BSP_FLASH_PAGE_SIZE)
#define USER_ADDR(UserIdx)	(BSP_FLASH_USER_ADDR + ((UserIdx) * 512))

typedef struct {
	char Present;
	char Login[USER_LOGIN_SIZE];
	char Role[USER_ROLE_SIZE];
} USER_CACHE_T;

static osMutexId UserMutexId;
static osMutexDef(USER);
static char UserBuffer[USER_BUFFER_SIZE];
static int32_t UserFlashError;

static USER_CACHE_T UserCacheList[USER_MAX_CNT];

static const USER_T UserDefaultList[] = {
	{
		.Login = USER_DEFAULT_SUPERUSER_LOGIN,
		.Password = USER_DEFAULT_SUPERUSER_PASSWORD,
		.Role = USER_DEFAULT_SUPERUSER_ROLE,
		.Name = USER_DEFAULT_SUPERUSER_NAME,
		.Tel = USER_DEFAULT_SUPERUSER_TEL,
		.Email = USER_DEFAULT_SUPERUSER_EMAIL,
	},
	{
		.Login = USER_DEFAULT_ADMIN_LOGIN,
		.Password = USER_DEFAULT_ADMIN_PASSWORD,
		.Role = USER_DEFAULT_ADMIN_ROLE,
		.Name = USER_DEFAULT_ADMIN_NAME,
		.Tel = USER_DEFAULT_ADMIN_TEL,
		.Email = USER_DEFAULT_ADMIN_EMAIL,
	},
};

static const uint32_t UserDefaultListSize = sizeof(UserDefaultList) / sizeof(UserDefaultList[0]);

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
	if (BSP_FLASH_WaitReady())
		return -1;
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
	
	jsonProp = json_getProperty( json, "password" );
	if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		return 1;
	Data = (char*)json_getValue( jsonProp ); 
	DataSize = strlen(Data);
	if (DataSize >= sizeof(User->Password ))
		return 1;
	strcpy(User->Password, Data);
	
	jsonProp = json_getProperty( json, "role" );
	if ( !jsonProp || JSON_TEXT != json_getType( jsonProp ) )
		return 1;
	Data = (char*)json_getValue( jsonProp ); 
	DataSize = strlen(Data);
	if (DataSize >= sizeof(User->Role ))
		return 1;
	if ((memcmp(Data, USER_ROLE_MONITOR, sizeof(USER_ROLE_MONITOR))) && 
		(memcmp(Data, USER_ROLE_USER, sizeof(USER_ROLE_USER))) && 
		(memcmp(Data, USER_ROLE_ADMIN, sizeof(USER_ROLE_ADMIN))) && 
		(memcmp(Data, USER_ROLE_SUPERUSER, sizeof(USER_ROLE_SUPERUSER))))
		return 1;
	strcpy(User->Role, Data);
	
		
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

static int32_t __USER_WriteUser(USER_T *User, uint32_t Idx)
{
	char *Data;
	uint32_t DataSize;
	uint16_t CalcCrc16;
	uint32_t Addr, Size;
	
	memset(UserBuffer, 0, sizeof(UserBuffer));
	Data = UserBuffer + 2;
	DataSize = 0;
	DataSize += sprintf(&Data[DataSize], "{");
	DataSize += sprintf(&Data[DataSize], "\"login\":\"%s\"", User->Login);
	DataSize += sprintf(&Data[DataSize], "\"password\":\"%s\"", User->Password);
	DataSize += sprintf(&Data[DataSize], "\"role\":\"%s\"", User->Role);
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
		
		if (BSP_FLASH_WaitReady())
			return -1;
		if (BSP_FLASH_ProgramPage(Addr, Data, Size))
			return -1;
		DataSize -= Size;
		Data += Size;
		Addr += Size;
	}
	return 0;
}

static int32_t __USER_DeleteUser(uint32_t Idx)
{
	uint32_t Addr;
	
	Addr = USER_ADDR(Idx);
		
	if (BSP_FLASH_WaitReady())
		return -1;
	if (BSP_FLASH_ErasePage(Addr))
		return -1;
	return 0;
}

int32_t USER_Init(void)
{
	int32_t Error;
	uint32_t i;
	USER_T User;
	
	if (!UserMutexId)
		UserMutexId = osMutexCreate(osMutex(USER));
	if (!UserMutexId)
		return -1;
	
	LOG_Info("USER", "", "", "Init");
	osMutexWait(UserMutexId, osWaitForever);
	UserFlashError = 0;	
	memset (&UserCacheList[0], 0, sizeof(UserCacheList[0]));
	Error = __USER_ReadUser(&User, 0);
	if (Error == -1)
	{
		goto flash_error;
	}
	else if (!Error)
	{
		UserCacheList[0].Present = 1;
		memcpy(UserCacheList[0].Login, User.Login, sizeof(User.Login));
		memcpy(UserCacheList[0].Role, User.Role, sizeof(User.Role));
		
		LOG_Info("USER", "", "", "Init: %s (%s)", User.Login, User.Role);
		
		for (i = 1; i < USER_MAX_CNT; i++)
		{
			memset (&UserCacheList[i], 0, sizeof(UserCacheList[0]));
			Error = __USER_ReadUser(&User, i);
			if (Error == -1)
			{
				goto flash_error;
			}
			else if (!Error)
			{
				UserCacheList[i].Present = 1;
				memcpy(UserCacheList[i].Login, User.Login, sizeof(User.Login));
				memcpy(UserCacheList[i].Role, User.Role, sizeof(User.Role));
				LOG_Info("USER", "", "", "Init: %s (%s)", User.Login, User.Role);
			}
		}
		LOG_Error("USER", "", "", "Init: Ok");
		osMutexRelease(UserMutexId);
		return 0;
	}

	LOG_Info("USER", "", "", "Init: Write Default Users");
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (i < UserDefaultListSize)
		{
			if (__USER_WriteUser((USER_T*)&UserDefaultList[i], i))
				goto flash_error;
			UserCacheList[i].Present = 1;
			memcpy(UserCacheList[i].Login, UserDefaultList[i].Login, sizeof(UserDefaultList[0].Login));
			memcpy(UserCacheList[i].Role, UserDefaultList[i].Role, sizeof(UserDefaultList[0].Role));
		}
		else
		{
			if (__USER_DeleteUser(i))
				goto flash_error;
			UserCacheList[i].Present = 0;
		}
	}
	LOG_Info("USER", "", "", "Init: Ok");
	osMutexRelease(UserMutexId);
	return 0;
	
flash_error:
	UserFlashError = -1;
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (i < UserDefaultListSize)
		{
			UserCacheList[i].Present = 1;
			memcpy(UserCacheList[i].Login, UserDefaultList[i].Login, sizeof(UserDefaultList[0].Login));
			memcpy(UserCacheList[i].Role, UserDefaultList[i].Role, sizeof(UserDefaultList[0].Role));
		}
		else
		{
			UserCacheList[i].Present = 0;
		}
	}
	LOG_Error("USER", "", "", "Init: Flash Error");
	osMutexRelease(UserMutexId);
	return -1;
}

int32_t USER_CreateUser(USER_T *User)
{
	uint32_t i;
	
	osMutexWait(UserMutexId, osWaitForever);
	/*Do not allow to create user with existing login*/
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (UserCacheList[i].Present)
		{
			if (!strcmp(UserCacheList[i].Login, User->Login))
				goto error;
		}
	}
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if (!UserCacheList[i].Present)
		{
			if (!UserFlashError)
			{
				if (__USER_WriteUser(User, i))
					goto error;
				UserCacheList[i].Present = 1;
				memcpy(UserCacheList[i].Login, User->Login, sizeof( User->Login));
				memcpy(UserCacheList[i].Role,  User->Role, sizeof( User->Role));
				LOG_Info("USER", "", "", "Create: %s (%s)", UserCacheList[i].Login, UserCacheList[i].Role);
			}
			else
			{
				goto error;
			}
			break;
		}
	}
	osMutexRelease(UserMutexId);
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

int32_t USER_GetUserByIndex(uint32_t Idx, USER_T *User)
{
	if (Idx >= USER_MAX_CNT)
		return -1;
	
	osMutexWait(UserMutexId, osWaitForever);
	if (!UserCacheList[Idx].Present)
		goto error;
	if (!UserFlashError)
	{
		if (__USER_ReadUser(User, Idx))
			goto error;
	}
	else
	{
		memcpy(&User, &UserDefaultList[Idx], sizeof(UserDefaultList[0]));
	}	
	osMutexRelease(UserMutexId);
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

int32_t USER_GetUserByLogin(char* Login, USER_T *User)
{
	uint32_t i;
	
	osMutexWait(UserMutexId, osWaitForever);
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if ((UserCacheList[i].Present) && \
			(!strcmp(Login, UserCacheList[i].Login)))
		{
			if (!UserFlashError)
			{
				if (__USER_ReadUser(User, i))
					goto error;
			}
			else
			{
				memcpy(&User, &UserDefaultList[i], sizeof(UserDefaultList[0]));
			}
			break;
		}
	}
	osMutexRelease(UserMutexId);
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

int32_t USER_UpdateUserByIndex(uint32_t Idx, USER_T *User)
{
	if (Idx >= USER_MAX_CNT)
		return -1;
	
	osMutexWait(UserMutexId, osWaitForever);
	if (!UserCacheList[Idx].Present)
		goto error;
	if (!UserFlashError)
	{
		/*Do not allow to update user login or role*/
		if (strcmp(UserCacheList[Idx].Login, User->Login))
			goto error;
		if (strcmp(UserCacheList[Idx].Role, User->Role))
			goto error;
		if (__USER_WriteUser(User, Idx))
			goto error;
		LOG_Info("USER", "", "", "Update: %s (%s)", UserCacheList[Idx].Login, UserCacheList[Idx].Role);
	}
	else
	{
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
	
	osMutexWait(UserMutexId, osWaitForever);
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if ((UserCacheList[i].Present) && \
			(!strcmp(Login, UserCacheList[i].Login)))
		{
			if (!UserFlashError)
			{
				/*Do not allow to update user login or role*/
				if (strcmp(UserCacheList[i].Login, User->Login))
					goto error;
				if (strcmp(UserCacheList[i].Role, User->Role))
					goto error;
				if (__USER_WriteUser(User, i))
					goto error;
				LOG_Info("USER", "", "", "Update: %s (%s)", UserCacheList[i].Login, UserCacheList[i].Role);
			}
			else
			{
				goto error;
			}
			break;
		}
	}
	osMutexRelease(UserMutexId);
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

int32_t USER_DeleteUserByIndex(uint32_t Idx)
{
	if (Idx >= USER_MAX_CNT)
		return -1;
	
	osMutexWait(UserMutexId, osWaitForever);
	if (!UserCacheList[Idx].Present)
		goto error;
	if ((!UserFlashError) && (Idx != 0))
	{
		if (__USER_DeleteUser(Idx))
			goto error;
		UserCacheList[Idx].Present = 0;
		LOG_Info("USER", "", "", "Delete: %s (%s)", UserCacheList[Idx].Login, UserCacheList[Idx].Role);
	}
	else
	{
		goto error;
	}	
	osMutexRelease(UserMutexId);
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

int32_t USER_DeleteUserByLogin(char* Login)
{
	uint32_t i;
	
	osMutexWait(UserMutexId, osWaitForever);
	for (i = 0; i < USER_MAX_CNT; i++)
	{
		if ((UserCacheList[i].Present) && \
			(!strcmp(Login, UserCacheList[i].Login)))
		{
			if ((!UserFlashError) && (i != 0))
			{
				if (__USER_DeleteUser(i))
					goto error;
				UserCacheList[i].Present = 0;
				LOG_Info("USER", "", "", "Delete: %s (%s)", UserCacheList[i].Login, UserCacheList[i].Role);
			}
			else
			{
				goto error;
			}
			break;
		}
	}
	osMutexRelease(UserMutexId);
	return 0;
	
error:
	osMutexRelease(UserMutexId);
	return -1;
}

