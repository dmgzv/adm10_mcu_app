#ifndef __USER_H__
#define __USER_H__

#include <stdint.h>
#include "user_md5crypt.h"
#include "user_snmpcrypt.h"

#define USER_MAX_CNT 32

#define USER_LOGIN_SIZE 		33
#define USER_PASSHASH_SIZE 	USER_MD5CRYPT_PASSWORD_HASH_LENGTH
#define USER_NAME_SIZE 			64
#define USER_EMAIL_SIZE 		64
#define USER_TEL_SIZE 			18
#define USER_GROUP_SIZE 		2
#define USER_SNMPHASH_SIZE 	USER_SNMPCRYPT_PASSWORD_HASH_LENGTH

#define USER_GROUP_MONITOR		"1"
#define USER_GROUP_USER				"2"
#define USER_GROUP_ADMIN			"3"
#define USER_GROUP_SUPERUSER	"4"

#define USER_DEFAULT_SUPERUSER_LOGIN			"superuser"
#define USER_DEFAULT_SUPERUSER_PASSHASH		"bpVng.ZKd7GDSg.aNdOPxV6sOvCF/1"
#define USER_DEFAULT_SUPERUSER_SNMPHASH		"C9D9AE734B3ADE186C7165DBD06CC3B7B36B7E15"
#define USER_DEFAULT_SUPERUSER_GROUP			USER_GROUP_SUPERUSER
#define USER_DEFAULT_SUPERUSER_NAME				""
#define USER_DEFAULT_SUPERUSER_TEL				""
#define USER_DEFAULT_SUPERUSER_EMAIL			""

#define USER_DEFAULT_ADMIN_LOGIN			"admin"
#define USER_DEFAULT_ADMIN_PASSHASH		"bpVng.ZKd7GDSg.aNdOPxV6sOvCF/1"
#define USER_DEFAULT_ADMIN_SNMPHASH		"C9D9AE734B3ADE186C7165DBD06CC3B7B36B7E15"
#define USER_DEFAULT_ADMIN_GROUP			USER_GROUP_ADMIN
#define USER_DEFAULT_ADMIN_NAME				""
#define USER_DEFAULT_ADMIN_TEL				""
#define USER_DEFAULT_ADMIN_EMAIL			""

typedef struct {
	char Login[USER_LOGIN_SIZE];
	char PassHash[USER_PASSHASH_SIZE];
	char Name[USER_NAME_SIZE];
	char Email[USER_EMAIL_SIZE];
	char Tel[USER_TEL_SIZE];
	char Group[USER_GROUP_SIZE];
	char SnmpHash[USER_SNMPHASH_SIZE];
} USER_T;

int32_t USER_Init(void);
int32_t USER_CreateUser(USER_T *User);
int32_t USER_GetUserByIndex(uint32_t Idx, USER_T *User);
int32_t USER_GetUserByLogin(char* Login, USER_T *User);
int32_t USER_UpdateUserByIndex(uint32_t Idx, USER_T *User);
int32_t USER_UpdateUserByLogin(char* Login, USER_T *User);
int32_t USER_DeleteUserByIndex(uint32_t Idx);
int32_t USER_DeleteUserByLogin(char* Login);
int32_t USER_Reset(void);


#endif /* __USER_H__ */
