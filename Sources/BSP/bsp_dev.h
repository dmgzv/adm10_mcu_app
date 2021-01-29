#ifndef __BSP_DEV_H
#define __BSP_DEV_H

#include <stdint.h>
#include "bsp_config.h"
#include "cmsis_os.h"
#include "log.h"

typedef enum {
	BSP_DEV_STATE_RESET = 0,
	BSP_DEV_STATE_INIT = 1,
	BSP_DEV_STATE_OK = 2,
	BSP_DEV_STATE_FAIL = 3
} BSP_DEV_STATE_T;

#define BSP_DEV		\
	BSP_DEV_STATE_T DevState;\
	osMutexDef_t DevMutexDef;\
	osMutexId DevMutexId;\
	const char *DevName;\
	const char *DevId;\
	uint32_t InitCnt;\
	uint32_t InitErr;\
	uint32_t InitContErr;\
	uint32_t InitContErrThr;\
	uint32_t UpdateCnt;\
	uint32_t UpdateErr;\
	uint32_t UpdateContErr;\
	uint32_t UpdateContErrThr

typedef struct {
	BSP_DEV;
} BSP_DEV_T;

#define BSP_DEV_CONT_ERR_THR_INF	0xFFFFFFFF

#define BSP_DEV_INIT(Dev)	\
do { \
	if (!(Dev)->DevMutexId) \
		(Dev)->DevMutexId = osMutexCreate(&(Dev)->DevMutexDef); \
	osMutexWait((Dev)->DevMutexId, osWaitForever); \
	if ((Dev)->DevState != BSP_DEV_STATE_FAIL) \
	{ \
		if ((Dev)->DevState != BSP_DEV_STATE_INIT) \
		{ \
			(Dev)->DevState = BSP_DEV_STATE_INIT; \
		} \
	} \
	(Dev)->InitCnt++; \
} while(0)

#define BSP_DEV_INIT_FAIL(Dev)	\
do {\
	(Dev)->InitErr++; \
	(Dev)->InitContErr++; \
	if ((Dev)->InitContErr > (Dev)->InitContErrThr) \
	{ \
		if ((Dev)->DevState != BSP_DEV_STATE_FAIL) \
		{ \
			LOG_Crit("BSP", (Dev)->DevName, (Dev)->DevId, "Init: Fail"); \
			(Dev)->DevState = BSP_DEV_STATE_FAIL; \
		} \
	} \
	osMutexRelease((Dev)->DevMutexId); \
} while(0)

#define BSP_DEV_INIT_OK(Dev)	\
do {\
	LOG_Info("BSP", (Dev)->DevName, (Dev)->DevId, "Init: Ok"); \
	(Dev)->DevState = BSP_DEV_STATE_OK; \
	(Dev)->InitContErr = 0; \
	osMutexRelease((Dev)->DevMutexId); \
} while(0)

#define BSP_DEV_UPDATE(Dev)	\
do { \
	osMutexWait((Dev)->DevMutexId, osWaitForever); \
	(Dev)->UpdateCnt++;\
} while(0)

#define BSP_DEV_UPDATE_FAIL(Dev)	\
do {\
	(Dev)->UpdateErr++; \
	(Dev)->UpdateContErr++; \
	if ((Dev)->UpdateContErr > (Dev)->UpdateContErrThr) \
	{ \
		if ((Dev)->DevState != BSP_DEV_STATE_FAIL) \
		{ \
			LOG_Crit("BSP", (Dev)->DevName, (Dev)->DevId, "Update: Fail"); \
			(Dev)->DevState = BSP_DEV_STATE_FAIL; \
		} \
	} \
	osMutexRelease((Dev)->DevMutexId); \
} while(0)

#define BSP_DEV_UPDATE_OK(Dev)	\
do {\
	(Dev)->UpdateContErr = 0; \
	osMutexRelease((Dev)->DevMutexId); \
} while(0)

#define BSP_DEV_DEINIT(Dev)	\
do { \
	if ((Dev)->DevState != BSP_DEV_STATE_RESET) \
	{ \
		LOG_Info("BSP", (Dev)->DevName, (Dev)->DevId, "Deinit"); \
		(Dev)->DevState = BSP_DEV_STATE_RESET; \
		(Dev)->InitContErr = 0; \
		(Dev)->UpdateContErr = 0; \
	} \
} while(0)

#define BSP_DEV_GET_STATE(Dev)			(Dev)->DevState
#define BSP_DEV_GET_STATE_STR(Dev)	(Dev)->DevState == BSP_DEV_STATE_RESET ? "RESET" : ((Dev)->DevState == BSP_DEV_STATE_INIT ? "INIT" : ((Dev)->DevState == BSP_DEV_STATE_OK ? "OK" : "FAIL"))
#define BSP_DEV_SET_STATE(Dev, State)	(Dev)->DevState = (State)
#define BSP_DEV_CHECK_STATE_OK(Dev)	((Dev)->DevState == BSP_DEV_STATE_OK)

#define BSP_DEV_LOCK(Dev) 	osMutexWait((Dev)->DevMutexId, osWaitForever)
#define BSP_DEV_UNLOCK(Dev) osMutexRelease((Dev)->DevMutexId)


/* ========== Usage Template ==============
	typedef struct device_t {
		BSP_DEV;
		...
	} DEVICE_T;

	void InitDevice(DEVICE_T *Device)
	{
		BSP_DEV_INIT(Device);
		...
		BSP_DEV_INIT_OK(Device);
	}
	
	void UpdateDevice(DEVICE_T *Device)
	{	
		BSP_DEV_UPDATE(Device);
		...
		BSP_DEV_UPDATE_OK(Device);
	}
	
	void DeinitDevice(DEVICE_T *Device)
	{
		BSP_DEV_DEINIT(Device);
	}
	
	void GetDeviceData(DEVICE_T *Device)
	{
		BSP_DEV_LOCK(Device);
		...
		BSP_DEV_UNLOCK(Device);
	}
	

*/

#endif /* __BSP_DEV_H */
