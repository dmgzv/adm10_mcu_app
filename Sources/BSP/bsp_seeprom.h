#ifndef __BSP_SEEPROM_H
#define __BSP_SEEPROM_H

#include "seeprom.h"

#define BSP_SEEPROM_ID				""
#define BSP_SEEPROM_I2C				I2C3

#ifndef BSP_CONFIG_USE_I2C3
	#error	Define BSP_CONFIG_USE_I2C3 in bsp_config.h for BSP_SEEPROM
#endif

extern SEEPROM_T seeprom;
	
void BSP_SEEPROM_Process(void);

#endif /* __BSP_SEEPROM_H */
