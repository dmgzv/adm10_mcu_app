#ifndef __USER_H__
#define __USER_H__

#include <stdint.h>

typedef struct {
	char Login[16];
	char Name[32];
	char Password[16];
	char Tel[16];
	char Email[32];
	char Role[16];
} USER_T;

int32_t USER_Init(void);
int32_t USER_FindByLoginAndPassword(char* Login, uint32_t LoginSize, char* Password, uint32_t PasswordSize, USER_T *User);

#endif /* __USER_H__ */
