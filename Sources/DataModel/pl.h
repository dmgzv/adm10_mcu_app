#ifndef _PL_H_
#define _PL_H_

#define PL_LINUX    1  // Porting layer for Linux
#define PL_FREERTOS 0  // Porting layer for FreeRTOS

#define  PL PL_FREERTOS

#if (PL == PL_LINUX)
#include "pl_linux.h"
#elif (PL == PL_FREERTOS)
#include "pl_freertos.h"
#else
#error Porting layer is not defined
#endif

#define PL_LOG_LEVEL               0

#if (PL_LOG_LEVEL > 0)
#define PL_LOG_ERROR( fmt, ... ) printf ("[ERROR]:%s:%d: " fmt "\n",  __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define PL_LOG_ERROR( fmt, ... )
#endif

#if (PL_LOG_LEVEL > 1)
#define PL_LOG_INFO( fmt, ... ) printf("[INFO]:%s:%d: " fmt "\n",  __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define PL_LOG_INFO( fmt, ... )
#endif

#if (PL_LOG_LEVEL > 2)
#define PL_LOG_DEBUG( fmt, ... ) printf("[DEBUG]:%s:%d: " fmt "\n",  __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define PL_LOG_DEBUG( fmt, ... )
#endif

#if (PL_LOG_LEVEL > 3)
#define PL_LOG_VERBOSE( fmt, ... ) printf("[VERBOSE]:%s:%d: " fmt "\n",  __FILE__, __LINE__, ##__VA_ARGS__);
#else
#define PL_LOG_VERBOSE( fmt, ... )
#endif

#endif  /* _PL_H_ */
