#include "user.h"
#include <string.h>

static const USER_T UserList[] = {
	{
		.Login = "user",
		.Password = "12345678",
		.Name = "User",
		.Tel = "+12345678",
		.Email = "user@email.com",
		.Role = "User"
	},
	{
		.Login = "admin",
		.Password = "12345678",
		.Name = "admin",
		.Tel = "+12345678",
		.Email = "admin@email.com",
		.Role = "Admin"
	},
	{
		.Login = "superuser",
		.Password = "12345678",
		.Name = "superuser",
		.Tel = "+12345678",
		.Email = "superuser@email.com",
		.Role = "Superuser"
	},
};

static const uint32_t UserListSize = sizeof(UserList) / sizeof(UserList[0]);

int32_t USER_Init(void)
{
	return 0;
}

int32_t USER_FindByLoginAndPassword(char* Login, uint32_t LoginSize, char* Password, uint32_t PasswordSize, USER_T *User)
{
	uint32_t i;
	
	for (i = 0; i < UserListSize; i++)
	{
		if (((LoginSize == strlen(UserList[i].Login)) && (!memcmp(Login, UserList[i].Login, LoginSize))) && \
			((PasswordSize == strlen(UserList[i].Password)) && (!memcmp(Password, UserList[i].Password, PasswordSize))))
		{
			memcpy(User, &UserList[i], sizeof(USER_T));
			return 0;
		}
	}
	
	return -1;
}

uint32_t USER_GetUsersCnt(void)
{
	return UserListSize;
}

int32_t USER_GetUser(uint32_t Idx, USER_T *User)
{
	if (Idx >= UserListSize)
		return -1;
	memcpy(User, &UserList[Idx], sizeof(USER_T));
	return 0;
}

