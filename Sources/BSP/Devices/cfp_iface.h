#ifndef __CFP_IFACE_H
#define __CFP_IFACE_H

#include <stdint.h>

typedef enum {
	CFP_GPIO_COM_TYPE_MOD_RST = 0,
	CFP_GPIO_COM_TYPE_MOD_LOPWR = 1,
	CFP_GPIO_COM_TYPE_MOD_ABS = 2,
	CFP_GPIO_COM_TYPE_GLB_ALRM = 3,
	CFP_GPIO_COM_TYPE_PRG_CNTL_1 = 4,
	CFP_GPIO_COM_TYPE_PRG_CNTL_2 = 5,
	CFP_GPIO_COM_TYPE_PRG_CNTL_3 = 6,
} CFP_GPIO_COM_TYPE_T;

typedef enum {
	CFP_GPIO_LANE_TYPE_TX_DIS = 0,
	CFP_GPIO_LANE_TYPE_RX_LOS = 1
} CFP_GPIO_LANE_TYPE_T;

typedef struct {
	int32_t (*Init)(void *Priv, uint32_t Type);
	int32_t (*SetState)(void *Priv, uint32_t Type, uint32_t State);
	int32_t (*GetState)(void *Priv, uint32_t Type, uint32_t *State);
} CFP_GPIO_COM_T;

typedef struct {
	int32_t (*Init)(void *Priv, uint32_t Lane, uint32_t Type);
	int32_t (*SetState)(void *Priv, uint32_t Lane, uint32_t Type, uint32_t State);
	int32_t (*GetState)(void *Priv, uint32_t Lane, uint32_t Type, uint32_t *State);
} CFP_GPIO_LANE_T;

typedef struct {
	CFP_GPIO_COM_T Com;
	CFP_GPIO_LANE_T Lane;
} CFP_GPIO_T;

typedef struct {
	uint8_t PhyAddr;
	uint8_t DevAddr;
	int32_t (*Read)(void *Priv, uint8_t PhyAddr, uint8_t DevAddr, uint16_t Addr, uint16_t *Data);
	int32_t (*Write)(void *Priv, uint8_t PhyAddr, uint8_t DevAddr, uint16_t Addr, uint16_t Data);
} CFP_MDIO_T;

typedef struct {
	CFP_GPIO_T GPIO;
	CFP_MDIO_T MDIO;
	void *Priv;
} CFP_IFACE_T;

#define CFP_IFACE_GPIO_COM_Init(Dev, Pin) 						((Dev)->GPIO.Com.Init ? (Dev)->GPIO.Com.Init((Dev)->Priv, (Pin)) : 0)
#define CFP_IFACE_GPIO_COM_GetState(Dev, Pin, State) 	(Dev)->GPIO.Com.GetState((Dev)->Priv, (Pin), (State))	
#define CFP_IFACE_GPIO_COM_SetState(Dev, Pin, State) 	(Dev)->GPIO.Com.SetState((Dev)->Priv, (Pin), (State))

#define CFP_IFACE_GPIO_LANE_Init(Dev, Idx, Pin)							((Dev)->GPIO.Lane.Init ? (Dev)->GPIO.Lane.Init((Dev)->Priv, (Idx), (Pin)) : 0)
#define CFP_IFACE_GPIO_LANE_GetState(Dev, Idx, Pin, State)	(Dev)->GPIO.Lane.GetState((Dev)->Priv, (Idx), (Pin), (State))
#define CFP_IFACE_GPIO_LANE_SetState(Dev, Idx, Pin, State)	(Dev)->GPIO.Lane.SetState((Dev)->Priv, (Idx), (Pin), (State))

#define CFP_IFACE_MDIO_Read(Dev, Addr, Data)	(Dev)->MDIO.Read((Dev)->Priv, (Dev)->MDIO.PhyAddr, (Dev)->MDIO.DevAddr, Addr, Data)
#define CFP_IFACE_MDIO_Write(Dev, Addr, Data)	(Dev)->MDIO.Write((Dev)->Priv, (Dev)->MDIO.PhyAddr, (Dev)->MDIO.DevAddr, Addr, Data)

#endif /* __CFP_IFACE_H */
