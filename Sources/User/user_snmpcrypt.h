#ifndef __USER_SNMPCRYPT_H__
#define __USER_SNMPCRYPT_H__

#include <stdint.h>

#define USER_SNMPCRYPT_PASSWORD_HASH_LENGTH (41)

int32_t USER_SNMPCRYPT_GenPassHash(const char* key, char *buffer);

#endif /* __USER_SNMPCRYPT_H__ */
