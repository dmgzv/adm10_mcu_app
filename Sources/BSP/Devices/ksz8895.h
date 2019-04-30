#ifndef __KSZ8895_H
#define __KSZ8895_H

#include "spi.h"
#include "bsp_dev.h"
#include "bsp_pin_ctrl.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#define KSZ8895_PORTS_CNT	5

#define KSZ8895_PIN_SPI_CS		0	
#define KSZ8895_PIN_RESET			1
#define KSZ8895_PIN_PWR_DOWN	2

#define KSZ8895_PORT_STATUS_CNT	3

typedef struct {
	BSP_DEV;
	SPI_TypeDef *SPI;
	SPI_CS_T CS;
	BSP_PIN_CTRL_T *PinCtrl;
	uint8_t FamilyId;
	uint8_t ChipId;
	uint8_t PortStatus[KSZ8895_PORTS_CNT][KSZ8895_PORT_STATUS_CNT];
} KSZ8895_T;

typedef struct {
	uint8_t Port;
	uint8_t Mac[6];
} KSZ8895_MAC_TABLE_T;

#define KSZ8895_PORT_SPEED_BIT	2
#define KSZ8895_PORT_SPEED_IDX	0
#define KSZ8895_PORT_SPEED(Dev, Idx)	((Dev)->PortStatus[(Idx)][KSZ8895_PORT_SPEED_IDX] & (1 << KSZ8895_PORT_SPEED_BIT) ? 100 : 10)

#define KSZ8895_PORT_DUPLEX_BIT	1
#define KSZ8895_PORT_DUPLEX_IDX	0
#define KSZ8895_PORT_DUPLEX_FULL	1
#define KSZ8895_PORT_DUPLEX_HALF	0
#define KSZ8895_PORT_DUPLEX(Dev, Idx)	((Dev)->PortStatus[(Idx)][KSZ8895_PORT_DUPLEX_IDX] & (1 << KSZ8895_PORT_DUPLEX_BIT) ? KSZ8895_PORT_DUPLEX_FULL : KSZ8895_PORT_DUPLEX_HALF)

#define KSZ8895_PORT_LINK_BIT	5
#define KSZ8895_PORT_LINK_IDX	1
#define KSZ8895_PORT_LINK(Dev, Idx)	((Dev)->PortStatus[(Idx)][KSZ8895_PORT_LINK_IDX] & (1 << KSZ8895_PORT_LINK_BIT) ? 1 : 0)

int32_t KSZ8895_Init(KSZ8895_T *dev, SPI_TypeDef *SPI, BSP_PIN_CTRL_T *PinCtrl);
void KSZ8895_Deinit(KSZ8895_T *Dev);
int32_t KSZ8895_Update(KSZ8895_T *Dev);

int32_t KSZ8895_ReadReg(KSZ8895_T *Dev, uint8_t Addr, uint8_t *Data);
int32_t KSZ8895_WriteReg(KSZ8895_T *Dev, uint8_t Addr, uint8_t Data);

int32_t KSZ8895_UpdatePortStatus(KSZ8895_T *Dev, uint32_t Idx);
int32_t KSZ8895_ReadMACTable(KSZ8895_T *Dev, KSZ8895_MAC_TABLE_T *Table, uint32_t MaxSize, uint32_t *Size);

#endif /* __KSZ8895_H */
