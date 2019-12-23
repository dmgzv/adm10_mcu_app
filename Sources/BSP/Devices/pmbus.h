#ifndef __PMBUS_H
#define __PMBUS_H

#include "i2c.h"

#define PMBUS_BLOCK_MAX_SIZE	32

float PMBUS_LinearToFloat(uint16_t input_val);
uint16_t PMBUS_FloatToLinear(float input_val);

typedef enum {
	PMBUS_CMD_TYPE_SEND_BYTE,
	PMBUS_CMD_TYPE_WRITE_BYTE,
	PMBUS_CMD_TYPE_WRITE_WORD,
	PMBUS_CMD_TYPE_WRITE_BLOCK,
	PMBUS_CMD_TYPE_READ_BYTE,
	PMBUS_CMD_TYPE_READ_WORD,
	PMBUS_CMD_TYPE_READ_BLOCK,
} PMBUS_CMD_TYPE_T;

int32_t PMBUS_ProcessCmd(const I2C_TypeDef *I2C, uint8_t Addr, PMBUS_CMD_TYPE_T CmdType, uint8_t Cmd,  uint8_t *Data, uint8_t DataSize);

#define PMBUS_SendByte(I2C, Addr, Cmd)				 PMBUS_ProcessCmd((I2C), (Addr), PMBUS_CMD_TYPE_SEND_BYTE, (Cmd), Null, 0)
#define PMBUS_WriteByte(I2C, Addr, Cmd, Data)	 PMBUS_ProcessCmd((I2C), (Addr), PMBUS_CMD_TYPE_WRITE_BYTE, (Cmd), (Data), 1)
#define PMBUS_WriteWord(I2C, Addr, Cmd, Data)	 PMBUS_ProcessCmd((I2C), (Addr), PMBUS_CMD_TYPE_WRITE_WORD, (Cmd), (Data), 2)
#define PMBUS_WriteBlock(I2C, Addr, Cmd, Data, DataSize) PMBUS_ProcessCmd((I2C), (Addr), PMBUS_CMD_TYPE_WRITE_BLOCK, (Cmd), (Data), (DataSize))
#define PMBUS_ReadByte(I2C, Addr, Cmd, Data)	 PMBUS_ProcessCmd((I2C), (Addr), PMBUS_CMD_TYPE_READ_BYTE, (Cmd), (Data), 1)
#define PMBUS_ReadWord(I2C, Addr, Cmd, Data)	 PMBUS_ProcessCmd((I2C), (Addr), PMBUS_CMD_TYPE_READ_WORD, (Cmd), (Data), 2)
#define PMBUS_ReadBlock(I2C, Addr, Cmd, Data, DataSize) PMBUS_ProcessCmd((I2C), (Addr), PMBUS_CMD_TYPE_READ_BLOCK, (Cmd), (Data), (DataSize))

#endif /* __PMBUS_H */
