#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include "dev_agent_log.h"



pthread_mutex_t dm_mutex;

int
dm_lock_init()
{
	int ret;
	ret = pthread_mutex_init(&dm_mutex, NULL);
	if (ret != 0) {
		AGT_LOG_ERROR("Failed to init DM lock, errno=%d", ret);
		return -1;
	}
	AGT_LOG_DEBUG("DM lock initialized");
	return 0;
}

int
dm_lock()
{
	int ret;
	ret = pthread_mutex_lock(&dm_mutex);
	if (ret != 0) {
		AGT_LOG_ERROR("Failed to aquire DM lock, errno=%d", ret);
		return -1;
	}
	AGT_LOG_DEBUG("Aquired DM lock");
	return 0;
}

int dm_unlock()
{
	int ret;
	ret = pthread_mutex_unlock(&dm_mutex);
	if (ret != 0) {
		AGT_LOG_ERROR("Failed to release DM lock, errno=%d", ret);
		return -1;
	}
	AGT_LOG_DEBUG("Released DM lock");
	return 0;
}


