#ifndef __USER_H__
#define __USER_H__

#include <stdint.h>

#define USER_MAX_CNT 64

#define USER_LOGIN_SIZE 		16
#define USER_PASSWORD_SIZE 	16
#define USER_ROLE_SIZE 			16
#define USER_NAME_SIZE 			32
#define USER_TEL_SIZE 			16
#define USER_EMAIL_SIZE 		32

#define USER_ROLE_MONITOR		"Monitor"
#define USER_ROLE_USER			"User"
#define USER_ROLE_ADMIN			"Admin"
#define USER_ROLE_SUPERUSER	"Superuser"

#define USER_DEFAULT_SUPERUSER_LOGIN			"Superuser"
#define USER_DEFAULT_SUPERUSER_PASSWORD		"12345678"
#define USER_DEFAULT_SUPERUSER_ROLE				USER_ROLE_SUPERUSER
#define USER_DEFAULT_SUPERUSER_NAME				""
#define USER_DEFAULT_SUPERUSER_TEL				""
#define USER_DEFAULT_SUPERUSER_EMAIL			""

#define USER_DEFAULT_ADMIN_LOGIN			"Admin"
#define USER_DEFAULT_ADMIN_PASSWORD		"12345678"
#define USER_DEFAULT_ADMIN_ROLE				USER_ROLE_ADMIN
#define USER_DEFAULT_ADMIN_NAME				""
#define USER_DEFAULT_ADMIN_TEL 				""
#define USER_DEFAULT_ADMIN_EMAIL 			""

typedef struct {
	char Login[USER_LOGIN_SIZE];
	char Password[USER_PASSWORD_SIZE];
	char Role[USER_ROLE_SIZE];
	char Name[USER_NAME_SIZE];
	char Tel[USER_TEL_SIZE];
	char Email[USER_EMAIL_SIZE];
} USER_T;

int32_t USER_Init(void);
int32_t USER_CreateUser(USER_T *User);
int32_t USER_GetUserByIndex(uint32_t Idx, USER_T *User);
int32_t USER_GetUserByLogin(char* Login, USER_T *User);
int32_t USER_UpdateUserByIndex(uint32_t Idx, USER_T *User);
int32_t USER_UpdateUserByLogin(char* Login, USER_T *User);
int32_t USER_DeleteUserByIndex(uint32_t Idx);
int32_t USER_DeleteUserByLogin(char* Login);


#endif /* __USER_H__ */
