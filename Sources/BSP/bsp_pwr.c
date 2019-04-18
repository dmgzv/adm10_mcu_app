#include "bsp_pwr.h"

#ifdef BSP_CONFIG_USE_FREERTOS
#define	BSP_PWR_msDelay(ms) 		osDelay(ms)
#else
#define	BSP_PWR_msDelay(ms) 		HAL_Delay(ms)
#endif

static const I2C_TypeDef *bsp_pwr_gdt080a_i2c[BSP_PWR_GDT080A_CNT] = {BSP_PWR_GDT080A_0V9_I2C, BSP_PWR_GDT080A_0V95_I2C};
static const uint8_t bsp_pwr_gdt080a_i2c_addr[BSP_PWR_GDT080A_CNT] = {BSP_PWR_GDT080A_0V9_I2C_ADDR, BSP_PWR_GDT080A_0V95_I2C_ADDR};
static const float bsp_pwr_gdt080a_vout[BSP_PWR_GDT080A_CNT] = {GDT080A_RTRIM_TO_VOUT(BSP_PWR_GDT080A_0V9_RTIM), GDT080A_RTRIM_TO_VOUT(BSP_PWR_GDT080A_0V95_RTIM)};

extern const UCD90120A_CFG_T BSP_UCD90120A_CFG_Table[];
extern const uint32_t BSP_UCD90120A_CFG_TableSize;

int32_t BSP_PWR_SetDigiVout(BSP_PWR_T *Dev, float Vout)
{
	uint32_t i;
	
	for (i = 0; i < BSP_PWR_GDT080A_CNT; i++)
	{
		if (GDT080A_SetVout(&Dev->gdt080a[i], Vout))
			return -1;
	}
	return 0;
}

int32_t BSP_PWR_Init(BSP_PWR_T *Dev, BSP_PWR_PIN_T *Pin)
{
	uint32_t i;
	
	if ((!Dev) || (!Pin))
		return -1;
	
	Dev->Pin = Pin;	
	
	if (DIM3R3400_Init(&Dev->dim3r3400, BSP_PWR_DIM3R3400_I2C, BSP_PWR_DIM3R3400_I2C_ADDR))
		return -1;

	
	if (BSP_PWR_PIN_SetState(Dev->Pin, BSP_PWR_PIN_UCD_RST, 0))
		return -1;
	
	BSP_PWR_msDelay(200);
	
	if (BSP_PWR_PIN_SetState(Dev->Pin, BSP_PWR_PIN_UCD_RST, 1))
		return -1;
	
	BSP_PWR_msDelay(200);
	
	if (UCD90120A_Init(&Dev->ucd90120a, BSP_PWR_UCD90120A_I2C, BSP_PWR_UCD90120A_I2C_ADDR))
		return -1;
	
	if (UCD90120A_CheckConfig(&Dev->ucd90120a, (UCD90120A_CFG_T*)BSP_UCD90120A_CFG_Table, BSP_UCD90120A_CFG_TableSize))
	{
		if (UCD90120A_WriteConfig(&Dev->ucd90120a, (UCD90120A_CFG_T*)BSP_UCD90120A_CFG_Table, BSP_UCD90120A_CFG_TableSize))
			return -1;
	}
	if (BSP_PWR_PIN_SetState(Dev->Pin, BSP_PWR_PIN_UCD_CTL, 0))
		return -1;
	
	/* todo: Add check voltages values*/
	BSP_PWR_msDelay(1000);
	
	I2C_Deinit();
	I2C_Init();
	
	for (i = 0; i < BSP_PWR_GDT080A_CNT; i++)
	{
		if (GDT080A_Init(&Dev->gdt080a[i], (I2C_TypeDef *)bsp_pwr_gdt080a_i2c[i], bsp_pwr_gdt080a_i2c_addr[i], bsp_pwr_gdt080a_vout[i]))
			return -1;
	}
	return 0;
}

int32_t BSP_PWR_Update(BSP_PWR_T *Dev)
{
	uint32_t i;
	
	DIM3R3400_Update(&Dev->dim3r3400);
	UCD90120A_Update(&Dev->ucd90120a);	
	for (i = 0; i < BSP_PWR_GDT080A_CNT; i++)
	{
		if (GDT080A_Update(&Dev->gdt080a[i]))
			return -1;
	}
	return 0;
}
