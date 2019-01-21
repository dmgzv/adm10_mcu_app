#ifndef __PROFILE_PROTOCOL_H
#define __PROFILE_PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include "profile.h"
#include "profile_types.h"

#define PROFILE_PROTOCOL_CMD_MAX_SIZE		512
#define PROFILE_PROTOCOL_RSP_MAX_SIZE		(6*1024)

uint32_t PROFILE_PROTOCOL_Command(PROFILE_T *Profile, char *cmd, char *rsp, uint32_t *complete);

#endif /* __PROFILE_PROTOCOL_H */
