#ifndef TFTP_BOOTLOADER_TYPES_H
#define TFTP_BOOTLOADER_TYPES_H

#include <stdint.h>
#include "cmsis_os.h"

#define TFTP_BOOTLOADER_STATUS_IDLE					0
#define TFTP_BOOTLOADER_STATUS_IN_PROGRESS	1
#define TFTP_BOOTLOADER_STATUS_ERROR				2
#define TFTP_BOOTLOADER_STATUS_COMPLETE			3

#define TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD								0
#define TFTP_BOOTLOADER_FILE_STATUS_VERIFY									1
#define TFTP_BOOTLOADER_FILE_STATUS_COMPLETE								2
#define TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_TIMEOUT_ERROR	3
#define TFTP_BOOTLOADER_FILE_STATUS_DOWNLOAD_SIZE_ERROR			4
#define TFTP_BOOTLOADER_FILE_STATUS_VERIFY_ERROR						5

typedef struct {
	uint32_t Status;
	char Name[128];
	uint32_t Size;
	char *Data;
	uint32_t MaxSize;
} TFTP_BOOTLOADER_FILE_T;

typedef struct {
	uint32_t Status;
	char *Addr;
	char *Dir;
	char Version[64];
	osMessageQId MessageQId;
	TFTP_BOOTLOADER_FILE_T *Files;
	uint32_t FilesCnt;
	void (*ProcessEvent)(uint32_t Event);
	int32_t (*VerifyInfo)(char *Data);
} TFTP_BOOTLOADER_COMPONENT_T;

typedef struct {
	uint32_t Status;
	char Dir[64];
	char Addr[4];
	char Version[64];
	osMessageQId MessageQId;
	osThreadId ThreadId;
	osMutexId MutexId;
	TFTP_BOOTLOADER_FILE_T Info;
	TFTP_BOOTLOADER_COMPONENT_T **Components;
	uint32_t ComponentsCnt;
} TFTP_BOOTLOADER_T;

#endif /*TFTP_BOOTLOADER_TYPES_H*/
