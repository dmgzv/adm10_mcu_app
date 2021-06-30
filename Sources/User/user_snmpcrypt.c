#include "user_snmpcrypt.h"
#include "sha1.h"
#include <string.h>

static const char engine_id[] = { 0x80, 0x00, 0x38, 0xD2, 0x04, 0x48, 0x4F, 0x52, 0x49, 0x5A, 0x4F, 0x4E };
static const uint32_t engine_id_len = sizeof(engine_id);

#if 0
int AuthSHABase::password_to_key(const unsigned char *password,
                                  const unsigned int   password_len,
                                  const unsigned char *engine_id,
                                  const unsigned int   engine_id_len,
                                  unsigned char *key,
                                  unsigned int *key_len)
{
  unsigned char *cp, password_buf[72];
  unsigned long  password_index = 0;
  unsigned long  count = 0, i;

  std::auto_ptr<Hasher> h(get_hasher());
  h->init(); /* initialize SHA */

  *key_len = h->get_key_length();

  /**********************************************/
  /* Use while loop until we've done 1 Megabyte */
  /**********************************************/
  while (count < 1048576) {
    cp = password_buf;
    for (i = 0; i < 64; i++) {
      /*************************************************/
      /* Take the next octet of the password, wrapping */
      /* to the beginning of the password as necessary.*/
      /*************************************************/
      *cp++ = password[password_index++ % password_len];
    }

    h->update(password_buf, 64);
    count += 64;
  }

  h->final(key);          /* tell SHA we're done */

#ifdef __DEBUG
  debughexcprintf(21, "key", key, *key_len);
#endif

  /*****************************************************/
  /* Now localize the key with the engine_id and pass  */
  /* through SHA to produce final key                  */
  /*****************************************************/
  h->init();
  h->update(key,      *key_len);
  h->update(engine_id, engine_id_len);
  h->update(key,      *key_len);
  h->final(key);

#ifdef __DEBUG
  debughexcprintf(21, "localized key", key, *key_len);
#endif

  return SNMPv3_USM_OK;
}
#endif

int32_t USER_SNMPCRYPT_GenPassHash(const char* password, char *key)
{
	char by;
	char *cp, password_buf[64];
  unsigned int  password_index = 0;
  unsigned int  count = 0, i;
	unsigned int password_len = 0;
	SHA1_CTX ctx;
	
	sha1_init(&ctx);
	
	password_len = strlen(password);
	
	/**********************************************/
  /* Use while loop until we've done 1 Megabyte */
  /**********************************************/
	while (count < 1048576) {
    cp = password_buf;
    for (i = 0; i < 64; i++) {
      /*************************************************/
      /* Take the next octet of the password, wrapping */
      /* to the beginning of the password as necessary.*/
      /*************************************************/
      *cp++ = password[password_index++ % password_len];
    }

    sha1_update(&ctx, password_buf, 64);
    count += 64;
  }
	
	sha1_final(&ctx, password_buf);
	
	/*****************************************************/
  /* Now localize the key with the engine_id and pass  */
  /* through SHA to produce final key                  */
  /*****************************************************/
  sha1_init(&ctx);
  sha1_update(&ctx, password_buf, SHA1_BLOCK_SIZE);
  sha1_update(&ctx, engine_id, engine_id_len);
  sha1_update(&ctx, password_buf, SHA1_BLOCK_SIZE);
  sha1_final(&ctx, password_buf);
	
	/*****************************************************/
  /* Now convert final key into hex string             */
  /*****************************************************/
	for (i = 0; i < SHA1_BLOCK_SIZE; i++)
	{
		by = (password_buf[i] >> 4) & 0x0F;
		if( by > 9 )
			*key++ = 'A' + by - 10;
		else
			*key++ = '0' + by;
		
		by = password_buf[i] & 0x0F;
		if( by > 9 )
			*key++ = 'A' + by - 10;
		else
			*key++ = '0' + by;
	}
	
	*key = 0;
	
	return 0;
}
