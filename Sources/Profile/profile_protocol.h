#ifndef __PROFILE_PROTOCOL_H
#define __PROFILE_PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include "profile_types.h"
#include "profile.h"
#include "profile_flash.h"
#include "bootloader_protocol.h"

#define PROFILE_PROTOCOL_CMD_MAX_SIZE		(512 + 128)
#define PROFILE_PROTOCOL_RSP_MAX_SIZE		(6*1024)

#define PROFILE_PROTOCOL_CMD_GET_CLASS				0x01
#define PROFILE_PROTOCOL_CMD_GET_CLASS_EXT		0x61
#define PROFILE_PROTOCOL_CMD_SET_ACCESS				0x49
#define PROFILE_PROTOCOL_CMD_SET_ACCESS_EXT		0x62
#define PROFILE_PROTOCOL_CMD_GET_VALUES_1			0x71
#define PROFILE_PROTOCOL_CMD_GET_VALUES_2			0x77
#define PROFILE_PROTOCOL_CMD_SET_VALUE_1			0x72
#define PROFILE_PROTOCOL_CMD_SET_VALUE_2			0x76
#define PROFILE_PROTOCOL_CMD_GET_PROFILE			0x73
#define PROFILE_PROTOCOL_CMD_GET_COLORS				0x7D
#define PROFILE_PROTOCOL_CMD_SET_COLOR				0x7C
#define PROFILE_PROTOCOL_CMD_READ_LANG				0x7B
#define PROFILE_PROTOCOL_CMD_WRITE_LANG				0x7A
#define PROFILE_PROTOCOL_CMD_UPDATE_FW				0x0F
#define PROFILE_PROTOCOL_CMD_CONFIG						0x63

typedef struct {
	PROFILE_T *Profile;
	BOOTLOADER_T *Bootloader;
	PROFILE_FLASH_T *ProfileFlash;
}PROFILE_PROTOCOL_T;

void PROFILE_PROTOCOL_Init(PROFILE_PROTOCOL_T *Protocol, PROFILE_T *Profile, BOOTLOADER_T *Bootloader, PROFILE_FLASH_T *ProfileFlash);
uint32_t PROFILE_PROTOCOL_Command(PROFILE_PROTOCOL_T *Protocol, char *cmd, char *rsp, uint32_t *complete);

#endif /* __PROFILE_PROTOCOL_H */
