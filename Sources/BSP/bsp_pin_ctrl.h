#ifndef __BSP_PIN_CTRL_H
#define __BSP_PIN_CTRL_H

#include <stdint.h>

typedef struct {
	int32_t (*Init)(void *Priv, uint32_t Pin);
	int32_t (*SetState)(void *Priv, uint32_t Pin, uint32_t State);
	int32_t (*GetState)(void *Priv, uint32_t Pin, uint32_t *State);
	void *Priv;
} BSP_PIN_CTRL_T;

#define BSP_PIN_CTRL_Init(Dev, Pin)							((Dev)->Init ? (Dev)->Init((Dev)->Priv, Pin) : 0)
#define BSP_PIN_CTRL_SetState(Dev, Pin, State)	((Dev)->SetState ? (Dev)->SetState((Dev)->Priv, Pin, State) : 0)
#define BSP_PIN_CTRL_GetState(Dev, Pin, State)	((Dev)->GetState ? (Dev)->GetState((Dev)->Priv, Pin, State) : (*State = 0, 0))

#endif /* __BSP_PIN_CTRL_H */
