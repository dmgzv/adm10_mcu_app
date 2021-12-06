#include "bsp_fpga_fw.h"
#include "bsp_fpga_maxv.h"
#include "bsp_fpga_flash.h"
#include "bsp_pwr_in.h"
#include "bsp_pwr_pin.h"
#include "bsp_pwr_seq.h"
#include "bsp_clk.h"
#include "log.h"

FPGA_FW_T fpga_fw =
{
	.DevName = "FPGA_FW",
	.InitContErrThr = 0,
	.UpdateContErrThr = 0,
	.Status = 0,
	.Addr = 0,
};

static int32_t FPGA_FW_Init(FPGA_FW_T *Dev)
{
	uint32_t i;
	
	if (!Dev)
		return -1;
	
	BSP_DEV_INIT(Dev);
	
	if (Dev->Status == 0)
	{
		/*todo: Add smart check for FPGA FW*/
		if (BSP_FPGA_FLASH_ReadSection(BSP_FLASH_FPGA_BOOT_SECTION, 0x3FE0000, Dev->Buffer, 1))
			goto error;
		if (Dev->Buffer[0] == ((char)0xFF))
		{
			Dev->Status = 1;
			Dev->Addr = 0;
		}
		else
		{
			Dev->Status = 2;
		}
	}
	else if (Dev->Status == 1)
	{
		for (i = 0; i < ((S29GL01GS_SIZE / 2) / (128 * sizeof(Dev->Buffer))); i++)
		{
			if (BSP_FPGA_FLASH_ReadSection(BSP_FLASH_FPGA_UPDATE_SECTION, Dev->Addr, Dev->Buffer, sizeof(Dev->Buffer)))
				goto error;
			if (BSP_FPGA_FLASH_WriteSection(BSP_FLASH_FPGA_BOOT_SECTION, Dev->Addr, Dev->Buffer, sizeof(Dev->Buffer)))
				goto error;
			Dev->Addr += sizeof(Dev->Buffer);
		}
		if (Dev->Addr >= (S29GL01GS_SIZE / 2))
		{
			Dev->Status = 2;
			/*This will force fw reload in maxv*/
			FPGA_MAXV_MODE_RESET(&fpga_maxv); 
		}
	}
	else if (Dev->Status == 2)
	{
		FPGA_MAXV_FW_STATUS_SET(&fpga_maxv, 1);
		BSP_DEV_INIT_OK(Dev);
		return 0;
	}
	
	BSP_DEV_UNLOCK(Dev);
	return 0;

error:
	BSP_DEV_INIT_FAIL(Dev);
	return -1;
}

static void FPGA_FW_Deinit(FPGA_FW_T *Dev)
{
	if (!Dev)
		return;
	BSP_DEV_DEINIT(Dev);	
}

static int32_t BSP_FPGA_FW_CheckConditions(void)
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
	if (BSP_DEV_GET_STATE(&fpga_maxv) != BSP_DEV_STATE_OK)
		return -1;
	if (BSP_DEV_GET_STATE(&s29gl01gs) != BSP_DEV_STATE_OK)
		return -1;
	return 0;
}

void BSP_FPGA_FW_Process(void)
{
	if (BSP_FPGA_FW_CheckConditions())
	{
		/* External faults present */
		if(BSP_DEV_GET_STATE(&fpga_fw) != BSP_DEV_STATE_RESET)
		{
			FPGA_FW_Deinit(&fpga_fw);
		}
	}
	else
	{
		/* External faults absent */
		if (BSP_DEV_GET_STATE(&fpga_fw) != BSP_DEV_STATE_OK)
		{
			FPGA_FW_Init(&fpga_fw);
		}
	}
}
