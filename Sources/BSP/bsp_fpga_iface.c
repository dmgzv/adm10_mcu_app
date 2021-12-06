#include "bsp_fpga_iface.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_clk.h"
#include "log.h"

FPGA_IFACE_T fpga_iface =
{
	.DevName = "FPGA_IFACE",
	.InitContErrThr = 10,
	.UpdateContErrThr = 10,
};

static int32_t BSP_FPGA_IFACE_CheckConditions(void)
{
	uint32_t i;

#ifdef BSP_PWR_IN_DIM3R3400_ENABLE 	
	if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good))
		return -1;
#endif
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	for (i = 0; i < BSP_CLK_SI5332_CNT; i++)
	{
		if ((BSP_DEV_GET_STATE(&si5332[i]) != BSP_DEV_STATE_OK) || (si5332[i].Status != SI5332_STATUS_ACTIVE))
			return -1;
	}
	return 0;
}

void BSP_FPGA_IFACE_Process(void)
{
	if (BSP_FPGA_IFACE_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&fpga_iface) != BSP_DEV_STATE_RESET)
		{
			FPGA_IFACE_Deinit(&fpga_iface);
		}
	}
	else
	{
		/* External faults absent */
		if(BSP_DEV_GET_STATE(&fpga_iface) != BSP_DEV_STATE_OK)
		{
			FPGA_IFACE_Init(&fpga_iface);
		}
	}
}
