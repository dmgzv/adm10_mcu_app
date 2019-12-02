#include "cmsis_os.h"

osMutexDef(dm);
osMutexId dm_mutex_id;

int dm_lock_init(void)
{
	dm_mutex_id = osMutexCreate(osMutex(dm));
	return 0;
}
/*
 * Lock the data model. Reentrancy not supported.
 * Returns 0 on success; -1 on failure to acquire lock
 */
int dm_lock(void)
{
	osMutexWait(dm_mutex_id, osWaitForever);
	return 0;
}
/*
 * Unlock the data model. Reentrancy not supported.
 * Returns 0 on success; -1 on failure to release lock
 */
int dm_unlock(void)
{
	osMutexRelease(dm_mutex_id);
	return 0;
}
