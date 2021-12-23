#include "user_md5crypt.h"
#include "md5.h"
#include <string.h>
#include <stdlib.h>

static const char md5_salt_prefix[] = "$1$";
/*
static const char salt_alphabet[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "./";
*/
void b64_from_24bit(
    char** cp, int* buflen, unsigned int b2, unsigned int b1, unsigned int b0, int n)
{
    /* Table with characters for base64 transformation.  */
    static const char b64t[64] = {'.', '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',
        'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
        'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
        'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    unsigned int w = (b2 << 16) | (b1 << 8) | b0;
    while (n-- > 0 && (*buflen) > 0)
    {
        *(*cp)++ = b64t[w & 0x3f];
        --(*buflen);
        w >>= 6;
    }
}

int32_t USER_MD5CRYPT_GenPassHash(const char* key, const char* salt, char *buffer)
{
		int buflen = USER_MD5CRYPT_PASSWORD_HASH_LENGTH;
		char alt_result[16];
    size_t key_len = 0;
    size_t cnt = 0;
    char* cp = NULL;
		MD5_CTX ctx;
    MD5_CTX alt_ctx;

    key_len = strlen(key);

    /* Prepare for the real work.  */
    md5_init(&ctx);

    /* Add the key string.  */
    md5_update(&ctx, key, key_len);

    /* Because the SALT argument do not have the salt prefix we
       add it separately.  */
    md5_update(&ctx, md5_salt_prefix, sizeof(md5_salt_prefix) - 1);

    /* The last part is the salt string.  This must be at most 8
       characters and it ends at the first `$' character (for
       compatibility with existing implementations).  */
    md5_update(&ctx, salt, USER_MD5CRYPT_PASSWORD_SALT_LENGTH);

    /* Compute alternate MD5 sum with input KEY, SALT, and KEY.  The
       final result will be added to the first context.  */
    md5_init(&alt_ctx);

    /* Add key.  */
    md5_update(&alt_ctx, key, key_len);

    /* Add salt.  */
    md5_update(&alt_ctx, salt, USER_MD5CRYPT_PASSWORD_SALT_LENGTH);

    /* Add key again.  */
    md5_update(&alt_ctx, key, key_len);

    /* Now get result of this (16 bytes) and add it to the other
       context.  */
    md5_final(&alt_ctx, alt_result);

    /* Add for any character in the key one byte of the alternate sum.  */
    for (cnt = key_len; cnt > 16; cnt -= 16)
        md5_update(&ctx, alt_result, 16);

    md5_update(&ctx, alt_result, cnt);

    /* For the following code we need a NULL byte.  */
    *alt_result = '\0';

    /* The original implementation now does something weird: for every 1
       bit in the key the first 0 is added to the buffer, for every 0
       bit the first character of the key.  This does not seem to be
       what was intended but we have to follow this to be compatible.  */
    for (cnt = key_len; cnt > 0; cnt >>= 1)
    {
        md5_update(&ctx, (cnt & 1) != 0 ? (const void*)alt_result : (const void*)key, 1);
    }

    /* Create intermediate result.  */
    md5_final(&ctx, alt_result);

    /* Now comes another weirdness.  In fear of password crackers here
       comes a quite long loop which just processes the output of the
       previous round again.  We cannot ignore this here.  */
    for (cnt = 0; cnt < 1000; ++cnt)
    {
        /* New context.  */
        md5_init(&ctx);

        /* Add key or last result.  */
        if ((cnt & 1) != 0)
            md5_update(&ctx, key, key_len);
        else
            md5_update(&ctx, alt_result, 16);

        /* Add salt for numbers not divisible by 3.  */
        if (cnt % 3 != 0)
            md5_update(&ctx, salt, USER_MD5CRYPT_PASSWORD_SALT_LENGTH);

        /* Add key for numbers not divisible by 7.  */
        if (cnt % 7 != 0)
            md5_update(&ctx, key, key_len);

        /* Add key or last result.  */
        if ((cnt & 1) != 0)
            md5_update(&ctx, alt_result, 16);
        else
            md5_update(&ctx, key, key_len);

        /* Create intermediate result.  */
        md5_final(&ctx, alt_result);
    }

    /* Now we can construct the result string */
    strcpy(buffer, salt);
    buflen -= USER_MD5CRYPT_PASSWORD_SALT_LENGTH;

		cp = &buffer[USER_MD5CRYPT_PASSWORD_SALT_LENGTH];
    b64_from_24bit(&cp, &buflen, alt_result[0], alt_result[6], alt_result[12], 4);
    b64_from_24bit(&cp, &buflen, alt_result[1], alt_result[7], alt_result[13], 4);
    b64_from_24bit(&cp, &buflen, alt_result[2], alt_result[8], alt_result[14], 4);
    b64_from_24bit(&cp, &buflen, alt_result[3], alt_result[9], alt_result[15], 4);
    b64_from_24bit(&cp, &buflen, alt_result[4], alt_result[10], alt_result[5], 4);
    b64_from_24bit(&cp, &buflen, 0, 0, alt_result[11], 2);
    *cp = '\0'; /* Terminate the string.  */
    return 0;
}
