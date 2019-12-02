#ifndef __ENDIAN_H__
#define __ENDIAN_H__

#include "lwip/def.h"

#define bswap16(x) ((((x) >> 8) | ((x) << 8)) & 0xFFFF)
#define bswap32(x) (((x) >> 24) | (((x) & 0x00FF0000) >> 8) \
                  | (((x) & 0x0000FF00) << 8) | ((x) << 24))
#define bswap64(x) (((x) >> 56) | (((x) & 0x00FF000000000000) >> 40) \
									| (((x) & 0x0000FF0000000000) >> 24) | (((x) & 0x000000FF00000000) >> 8) \
									| (((x) & 0x00000000FF000000) << 8) | (((x) & 0x0000000000FF0000) << 24) \
                  | (((x) & 0x000000000000FF00) << 40) | ((x) << 56))

// Big Endian Compilers
#if BYTE_ORDER == BIG_ENDIAN

#define htobe16(x) (x)
#define htobe32(x) (x)
#define htobe64(x) (x)
#define be16toh(x) (x)
#define be32toh(x) (x)
#define be64toh(x) (x)

#define htole16(x) bswap16(x)
#define htole32(x) bswap32(x)
#define htole64(x) bswap64(x)
#define le16toh(x) bswap16(x)
#define le32toh(x) bswap32(x)
#define le64toh(x) bswap64(x)

#elif BYTE_ORDER == LITTLE_ENDIAN

#define htobe16(x) bswap16(x)
#define htobe32(x) bswap32(x)
#define htobe64(x) bswap64(x)
#define be16toh(x) bswap16(x)
#define be32toh(x) bswap32(x)
#define be64toh(x) bswap64(x)

#define htole16(x) (x)
#define htole32(x) (x)
#define htole64(x) (x)
#define le16toh(x) (x)
#define le32toh(x) (x)
#define le64toh(x) (x)

#else

#define htobe16(x) (x)
#define htobe32(x) (x)
#define htobe64(x) (x)
#define be16toh(x) (x)
#define be32toh(x) (x)
#define be64toh(x) (x)

#define htole16(x) (x)
#define htole32(x) (x)
#define htole64(x) (x)
#define le16toh(x) (x)
#define le32toh(x) (x)
#define le64toh(x) (x)

#endif

#endif  // __ENDIAN_H__
