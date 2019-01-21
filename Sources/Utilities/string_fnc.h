#ifndef __STRING_FNC_H
#define __STRING_FNC_H

char * ByteToStr( char * str, unsigned char byVal );
char * UintToStr( char * str, unsigned int dwVal );
char * UintToStr0( char * str, unsigned int dwVal, unsigned int dwMinDec );
char * UintToHexStr( char * str, unsigned int dwVal );
char * WordToHexStr( char * str, unsigned short int wVal );
char * ByteToHexStr( char * str, unsigned char byVal );
char * IntToStr( char * str, int iVal );
char * FloatToStr( char * str, float f, unsigned int dwDigCount );
char * IPToStr( char * str, unsigned int IPAddr );
int StrToUint( char *str, unsigned int * pResult, char cDelim );
int StrToInt( char *str, int * pResult, char cDelim );
int StrToFloat( char *str, float * pResult, char cDelim );
int HexStr2ToByte( char * str, unsigned char * pResult );
int HexStr4ToWord( char* str, unsigned short int * pResult );
int HexStr8ToUint( char* str, unsigned int * pResult );
int HexStrToUint( char* str, unsigned int * pResult, char cDel );
int StrToIP( char * str, unsigned int *pResult );


#endif /* __STRING_FNC_H */
