#include "pmbus.h"
#include <string.h>
#include <math.h>

/*
 * Convert a LinearFloat5_11 formatted word
 * into a floating point value
 */
float PMBUS_LinearToFloat(uint16_t input_val) 
{    
	// extract exponent as MS 5 bits  
	int8_t exponent = input_val >> 11;
  // extract mantissa as LS 11 bits  
	int16_t mantissa = input_val & 0x7ff;  
  // sign extend exponent from 5 to 8 bits 
 
  if( exponent > 0x0F )
		exponent |= 0xE0;  
  // sign extend mantissa from 11 to 16 bits
	if( mantissa > 0x03FF )
		mantissa |= 0xF800;     
	// compute value as mantissa * 2^(exponent)
	return mantissa * pow(2,exponent); 
}

/*
 * Convert a floating point value into a 
 * LinearFloat5_11 formatted word 
 */
uint16_t PMBUS_FloatToLinear(float input_val)
{
	uint16_t uExponent, uMantissa;
	// set exponent to -16
	int exponent = -16;
  // extract mantissa from input value
	int mantissa =    (int)(input_val / pow(2.0, exponent));
  // Search for an exponent that produces  
	// a valid 11-bit mantissa
	do {
		if ((mantissa >= -1024) && (mantissa <= +1023))
		{
			break; // stop if mantissa valid
		}   
		exponent++;
		mantissa = (int)(input_val / pow(2.0, exponent));
	} while (exponent < +15);  
  // Format the exponent of the L11
	uExponent = exponent << 11;
  // Format the mantissa of the L11
	uMantissa = mantissa & 0x07FF;
  // Compute value as exponent | mantissa
	return uExponent | uMantissa;
}

int32_t PMBUS_ProcessCmd(const I2C_TypeDef *I2C, uint8_t Addr, PMBUS_CMD_TYPE_T CmdType, uint8_t Cmd,  uint8_t *Data, uint8_t DataSize)
{
	uint8_t Tmp[PMBUS_BLOCK_MAX_SIZE + 1];
	
	if (DataSize > PMBUS_BLOCK_MAX_SIZE)
		return -1;
	
	switch (CmdType)
	{
		case PMBUS_CMD_TYPE_SEND_BYTE:
			return I2C_Transmit(I2C, Addr, (char*)&Cmd, 1);
		case PMBUS_CMD_TYPE_WRITE_BYTE:
			return I2C_Sequential_Transmit(I2C, Addr, &Cmd, 1, (char*)Data, 1);
		case PMBUS_CMD_TYPE_WRITE_WORD:
			return I2C_Sequential_Transmit(I2C, Addr, &Cmd, 1, (char*)Data, 2);
		case PMBUS_CMD_TYPE_WRITE_BLOCK:
			Tmp[0] = DataSize;
			memcpy(&Tmp[1], Data, DataSize);	
			return I2C_Sequential_Transmit(I2C, Addr, &Cmd, 1, (char*)Tmp, DataSize + 1);
		case PMBUS_CMD_TYPE_READ_BYTE:
			return I2C_Sequential_Receive(I2C, Addr, (uint8_t*)&(Cmd), 1, (char*)Data, 1);
		case PMBUS_CMD_TYPE_READ_WORD:
			return I2C_Sequential_Receive(I2C, Addr, (uint8_t*)&(Cmd), 1, (char*)Data, 2);
		case PMBUS_CMD_TYPE_READ_BLOCK:
			if (I2C_Sequential_Receive(I2C, Addr, (uint8_t*)&(Cmd), 1, (char*)Tmp, DataSize + 1))
				return -1;
			memcpy(Data, &Tmp[1], DataSize);
			return 0;
	}
	return -1;
}
