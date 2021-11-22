#include "bsp_fpga_maxv.h"
#include "bsp_fpga_iface.h"
#include "bsp_fpga_flash.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_clk.h"
#include "log.h"

FPGA_MAXV_T fpga_maxv =
{
	.DevName = "FPGA_MAXV",
	.InitContErrThr = 10,
	.UpdateContErrThr = 10,
	.BaseAddr = BSP_FPGA_MAXV_BASE_ADDR,
};

static int32_t BSP_FPGA_MAXV_CheckConditions(void)
{
	uint32_t i;
	
	//if ((BSP_DEV_GET_STATE(&dim3r3400) != BSP_DEV_STATE_OK) || (!dim3r3400_power_good))
	//	return -1;
	if (BSP_DEV_GET_STATE(&pwr_pca9536) != BSP_DEV_STATE_OK)
		return -1;
	if ((BSP_DEV_GET_STATE(&ucd90120a) != BSP_DEV_STATE_OK) || (!UCD90120A_POWER_GOOD(&ucd90120a)))
		return -1;
	for (i = 0; i < BSP_CLK_SI5332_CNT; i++)
	{
		if ((BSP_DEV_GET_STATE(&si5332[i]) != BSP_DEV_STATE_OK) || (si5332[i].Status != SI5332_STATUS_ACTIVE))
			return -1;
	}
	if (BSP_DEV_GET_STATE(&fpga_iface) != BSP_DEV_STATE_OK)
		return -1;
	return 0;
}

void BSP_FPGA_MAXV_Process(void)
{
	if (BSP_FPGA_MAXV_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_RESET)
		{
			FPGA_MAXV_Deinit(&fpga_maxv);
		}
	}
	else
	{
		/* External faults absent */
		if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
		{
			FPGA_MAXV_Init(&fpga_maxv);
		}
		else
		{
			FPGA_MAXV_Update(&fpga_maxv);
		}
	}
}
