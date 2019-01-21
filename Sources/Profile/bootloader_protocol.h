#ifndef __BOOTLOADER_PROTOCOL_H
#define __BOOTLOADER_PROTOCOL_H

#include "bootloader.h"

uint32_t BOOTLOADER_PROTOCOL_UpdateFw(BOOTLOADER_T *Bootloader, char *cmd, char *rsp, uint32_t *complete);
uint32_t BOOTLOADER_PROTOCOL_ParseCommand(BOOTLOADER_T *Bootloader, char *cmd, char *rsp, uint32_t *complete);

#endif /* __BOOTLOADER_PROTOCOL_H */
