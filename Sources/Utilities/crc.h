#ifndef __CRC_H
#define __CRC_H

unsigned int Calc_CRC16_Value( unsigned char by, unsigned int CRC16 );
unsigned int Calc_CRC16_ASCII( char * buf, unsigned int MaxSize );
unsigned int Calc_CRC16( char * buf, unsigned int size, unsigned int crc );

#endif /* __CRC_H */
