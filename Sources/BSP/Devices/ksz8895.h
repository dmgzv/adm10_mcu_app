#ifndef __KSZ8895_H
#define __KSZ8895_H

#include "spi.h"
#include "bsp_dev.h"
#include "bsp_pin_ctrl.h"
#ifdef BSP_CONFIG_USE_FREERTOS
#include "cmsis_os.h"
#endif

#define KSZ8895_DEV_NAME "KSZ8895"

#define KSZ8895_PORTS_CNT	5

#define KSZ8895_PIN_SPI_CS		0	
#define KSZ8895_PIN_RESET			1
#define KSZ8895_PIN_PWR_DOWN	2

#define KSZ8895_GLOBAL_CONTROL_CNT	12
#define KSZ8895_PORT_CONTROL_CNT	6
#define KSZ8895_PORT_STATUS_CNT	2

typedef struct {
	uint16_t VID;
	uint16_t Data;
} KSZ8895_VLAN_TABLE_RECORD_T;

typedef struct {
	BSP_DEV;
	const SPI_TypeDef *SPI;
	const BSP_PIN_CTRL_T *PinCtrl;
	SPI_CS_T CS;
	uint8_t FamilyId;
	uint8_t ChipId;
	uint8_t GlobalControl[KSZ8895_GLOBAL_CONTROL_CNT];
	uint8_t PortControl[KSZ8895_PORTS_CNT][KSZ8895_PORT_CONTROL_CNT];
	uint8_t PortStatus[KSZ8895_PORTS_CNT][KSZ8895_PORT_STATUS_CNT];
	const KSZ8895_VLAN_TABLE_RECORD_T *VLANTable;
	const uint32_t VLANTableSize;
} KSZ8895_T;

typedef struct {
	uint8_t Port;
	uint8_t Mac[6];
	uint8_t FID;
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

#define KSZ8895_PORT_MODE_AUTONEG	0
#define KSZ8895_PORT_MODE_100M_FULL_DUPLEX	1
#define KSZ8895_PORT_MODE_100M_HALF_DUPLEX	2
#define KSZ8895_PORT_MODE_10M_FULL_DUPLEX	3
#define KSZ8895_PORT_MODE_10M_HALF_DUPLEX	4
#define KSZ8895_PORT_MODE_DISABLED	0xFF

#define KSZ8895_PORT_VLAN_TAG_INSERTION	(1 << 0)
#define KSZ8895_PORT_VLAN_TAG_REMOVAL		(1 << 1)

#define KSZ8895_VLAN_PORT(Idx)	(1 << ((Idx) + 7))
#define KSZ8895_VLAN_FID(FID)		((FID) << 0)
#define KSZ8895_VLAN_VALID			(1 << 12)
#define KSZ8895_VLAN_INVALID		(0)

int32_t KSZ8895_Init(KSZ8895_T *Dev);
void KSZ8895_Deinit(KSZ8895_T *Dev);
int32_t KSZ8895_Update(KSZ8895_T *Dev);

int32_t KSZ8895_ConfigPortMode(KSZ8895_T *Dev, uint32_t Idx, uint32_t Mode);
int32_t KSZ8895_ConfigPortVLAN(KSZ8895_T *Dev, uint32_t Idx, uint16_t PVID, uint32_t Flags);

int32_t KSZ8895_ReadReg(KSZ8895_T *Dev, uint8_t Addr, uint8_t *Data);
int32_t KSZ8895_WriteReg(KSZ8895_T *Dev, uint8_t Addr, uint8_t Data);

int32_t KSZ8895_UpdatePortStatus(KSZ8895_T *Dev, uint32_t Idx);
int32_t KSZ8895_ReadMACTable(KSZ8895_T *Dev, KSZ8895_MAC_TABLE_T *Table, uint32_t MaxSize, uint32_t *Size);
int32_t KSZ8895_ReadPortStat(KSZ8895_T *Dev, uint32_t Idx, uint32_t *Stat);
int32_t KSZ8895_ReadVLANTable(KSZ8895_T *Dev, uint16_t VID, uint16_t *VIDData);

#endif /* __KSZ8895_H */
