#ifndef __BSP_DEV_H
#define __BSP_DEV_H

#include <stdint.h>
#include "bsp_config.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

typedef enum {
	BSP_DEV_STATE_RESET = 0,
	BSP_DEV_STATE_INIT = 1,
	BSP_DEV_STATE_OK = 2,
	BSP_DEV_STATE_FAIL = 3
} BSP_DEV_STATE_T;

#ifdef BSP_CONFIG_USE_FREERTOS

#define BSP_DEV		\
	BSP_DEV_STATE_T DevState;\
	osMutexDef_t DevMutexDef;\
	osMutexId DevMutexId;\
	const char *DevName;\
	const char *DevId

#if( configSUPPORT_STATIC_ALLOCATION == 1 )

#define BSP_DEV_INIT(Dev, Name, Id)	\
do { \
	(Dev)->DevState = BSP_DEV_STATE_INIT;\
	(Dev)->DevMutexDef.dummy = 0; \
	(Dev)->DevMutexDef.controlblock = NULL; \
	(Dev)->DevName = Name;\
	(Dev)->DevId = Id;\
	if (!(Dev)->DevMutexId) \
		(Dev)->DevMutexId = osMutexCreate(&(Dev)->DevMutexDef);\
} while(0)

#else /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

#define BSP_DEV_INIT(Dev, Name, Id)	\
do { \
	(Dev)->DevState = BSP_DEV_STATE_INIT;\
	(Dev)->DevMutexDef.dummy = 0; \
	(Dev)->DevName = Name;\
	(Dev)->DevId = Id;\
	if (!(Dev)->DevMutexId) \
		(Dev)->DevMutexId = osMutexCreate(&(Dev)->DevMutexDef);\
} while(0)

#define BSP_DEV_DEINIT(Dev)	\
do { \
	(Dev)->DevState = BSP_DEV_STATE_RESET;\
} while(0)

#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

#define BSP_DEV_GET_STATE(Dev)			(Dev)->DevState
#define BSP_DEV_GET_STATE_STR(Dev)	(Dev)->DevState == BSP_DEV_STATE_RESET ? "RESET" : ((Dev)->DevState == BSP_DEV_STATE_INIT ? "INIT" : ((Dev)->DevState == BSP_DEV_STATE_OK ? "OK" : "FAIL"))
#define BSP_DEV_SET_STATE(Dev, State)	(Dev)->DevState = (State)
#define BSP_DEV_CHECK_STATE_OK(Dev)	((Dev)->DevState == BSP_DEV_STATE_OK)

#define BSP_DEV_LOCK(Dev) 	osMutexWait((Dev)->DevMutexId, osWaitForever)
#define BSP_DEV_UNLOCK(Dev) osMutexRelease((Dev)->DevMutexId)

#else /* BSP_CONFIG_USE_FREERTOS */

#define BSP_DEV \
	BSP_DEV_STATE_T DevState;\
	const char *DevName;\
	const char *DevId
	

#define BSP_DEV_INIT(Dev, Name, Id)	\
do { \
	(Dev)->DevState = BSP_DEV_STATE_INIT;\
	(Dev)->DevName = Name;\
	(Dev)->DevId = Id;\
} while(0)

#define BSP_DEV_DEINIT(Dev)	\
do { \
	(Dev)->DevState = BSP_DEV_STATE_RESET;\
} while(0)

#define BSP_DEV_GET_STATE(Dev)			(Dev)->DevState
#define BSP_DEV_GET_STATE_STR(Dev)	(Dev)->DevState == BSP_DEV_STATE_RESET ? "RESET" : ((Dev)->DevState == BSP_DEV_STATE_INIT ? "INIT" : ((Dev)->DevState == BSP_DEV_STATE_OK ? "OK" : "FAIL"))
#define BSP_DEV_SET_STATE(Dev, State)	(Dev)->DevState = (State)

#define BSP_DEV_LOCK(Dev)
#define BSP_DEV_UNLOCK(Dev)

#endif /* BSP_CONFIG_USE_FREERTOS */


/* ========== Usage Template ==============
	typedef struct device_t {
		BSP_DEV;
		...
	} DEVICE_T;

	void InitDevice(DEVICE_T *Device)
	{
		
		BSP_DEV_INIT(Device);
		BSP_DEV_LOCK(Device);
		...
		BSP_DEV_SET_STATE(Device, BSP_DEV_STATE_OK);
		BSP_DEV_UNLOCK(Device);
	}
	
	void DoSomethingWithDevice(DEVICE_T *Device)
	{	
		BSP_DEV_LOCK(Device);
		...
		BSP_DEV_UNLOCK(Device);
	}
	
	void DeinitDevice(DEVICE_T *Device)
	{
		BSP_DEV_DEINIT(Device);
	}
	

*/

#endif /* __BSP_DEV_H */
