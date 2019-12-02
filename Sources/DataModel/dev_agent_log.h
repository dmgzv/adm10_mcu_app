#ifndef _DEV_AGENT_LOG_H_
#define _DEV_AGENT_LOG_H_

#if 0

#include <stdio.h>

#define AGT_LOG(level, fmt, ...) do { \
	fprintf(stdout, "%s:%s:%d: " fmt "\n", \
			level, __FILE__, __LINE__, ##__VA_ARGS__); \
} while (0);
#define LOG_ERROR(...) do { \
	AGT_LOG("ERROR", __VA_ARGS__); \
} while (0);
#define LOG_INFO(...) do { \
	AGT_LOG("INFO", __VA_ARGS__); \
} while (0);
#define LOG_DEBUG(...) do { \
	AGT_LOG("DEBUG", __VA_ARGS__); \
} while (0);
#define LOG_VERBOSE(...) do { \
	AGT_LOG("VERBOSE", __VA_ARGS__); \
} while (0);
#define LOG_printf printf		// TODO: FIXME

#else

#define AGT_LOG(level, fmt, ...) 
#define LOG_ERROR(...) 
#define LOG_INFO(...) 
#define LOG_DEBUG(...) 
#define LOG_VERBOSE(...) 
#define LOG_printf(...) 

#endif 



#endif // _DEV_AGENT_LOG_H_
