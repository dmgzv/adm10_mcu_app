#ifndef __BSP_PWR_SEQ_H
#define __BSP_PWR_SEQ_H

#include "ucd90120a.h"
	
#define BSP_PWR_SEQ_UCD90120A_I2C				I2C3
#define BSP_PWR_SEQ_UCD90120A_I2C_ADDR	0x7E

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_PWR
#endif
	
extern UCD90120A_T ucd90120a;
	
extern const UCD90120A_CFG_CMD_T BSP_UCD90120A_CFG_Table[];
extern const uint32_t BSP_UCD90120A_CFG_TableSize;
extern const BSP_PIN_CTRL_T ucd90120a_pin_ctrl;
	
void BSP_PWR_SEQ_Process(void);

#endif /* __BSP_PWR_SEQ_H */
