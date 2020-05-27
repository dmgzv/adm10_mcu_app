#ifndef TFTP_BOOTLOADER_H
#define TFTP_BOOTLOADER_H

#include "tftp_bootloader_types.h"

extern TFTP_BOOTLOADER_T TFTP_BOOTLOADER;

int32_t TFTP_BOOTLOADER_Init(void);
int32_t TFTP_BOOTLOADER_Update(char* Dir, char *Addr);

#endif /*TFTP_BOOTLOADER_H*/
