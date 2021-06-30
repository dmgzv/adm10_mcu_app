#ifndef __USER_MD5CRYPTT_H__
#define __USER_MD5CRYPTT_H__

#include <stdint.h>

#define USER_MD5CRYPT_PASSWORD_SALT_LENGTH 8
#define USER_MD5CRYPT_PASSWORD_HASH_LENGTH (USER_MD5CRYPT_PASSWORD_SALT_LENGTH + 22 + 1)

int32_t USER_MD5CRYPT_GenPassHash(const char* key, const char* salt, char *buffer);

#endif /* __USER_MD5CRYPTT_H__ */
