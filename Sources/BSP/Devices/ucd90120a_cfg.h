#ifndef __UCD90120A_CFG_CMD_H
#define __UCD90120A_CFG_CMD_H

#include "pmbus.h"

typedef struct {
	PMBUS_CMD_TYPE_T CmdType;
	uint8_t Cmd;
	uint8_t *Data;
	uint32_t DataLen;
}UCD90120A_CFG_CMD_T;


#endif /* __UCD90120A_CFG_CMD_H */
