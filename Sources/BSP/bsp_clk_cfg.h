/*
 * Si5332-GM2 Rev C Configuration Register Export Header File
 *
 * This file represents a series of Silicon Labs Si5332-GM2 Rev C 
 * register writes that can be performed to load a single configuration 
 * on a device. It was created by a Silicon Labs ClockBuilder Pro
 * export tool.
 *
 * Part:		                                       Si5332-GM2 Rev C
 * Design ID:                                          
 * Includes Pre/Post Download Control Register Writes: Yes
 * Created By:                                         ClockBuilder Pro v2.29 [2018-11-04]
 * Timestamp:                                          2019-01-10 18:45:47 GMT+03:00
 *
 * A complete design report corresponding to this export is included at the end 
 * of this header file.
 *
 */

#ifndef __BSP_CLK_CFG_H
#define __BSP_CLK_CFG_H

#include "si5332.h"

#define BSP_CLK_CFG_NUM_REGS				72

const SI5332_CFG_T bsp_clk_cfg[BSP_CLK_CFG_NUM_REGS] =
{
	{ 0x17, 0x00 },
	{ 0x18, 0x00 },
	{ 0x19, 0x00 },
	{ 0x1A, 0x00 },
	{ 0x1B, 0x00 },
	{ 0x1C, 0x00 },
	{ 0x21, 0x6A },
	{ 0x24, 0x01 },
	{ 0x25, 0x00 },
	{ 0x26, 0x00 },
	{ 0x27, 0x00 },
	{ 0x28, 0x00 },
	{ 0x29, 0x00 },
	{ 0x2A, 0x00 },
	{ 0x2B, 0x20 },
	{ 0x3C, 0x00 },
	{ 0x48, 0x00 },
	{ 0x54, 0x00 },
	{ 0x60, 0x00 },
	{ 0x67, 0x1E },
	{ 0x68, 0xED },
	{ 0x69, 0x00 },
	{ 0x6A, 0x33 },
	{ 0x6B, 0x00 },
	{ 0x6C, 0x61 },
	{ 0x73, 0x00 },
	{ 0x74, 0x00 },
	{ 0x75, 0x01 },
	{ 0x7A, 0x0C },
	{ 0x7B, 0x01 },
	{ 0x7C, 0x00 },
	{ 0x7D, 0x00 },
	{ 0x7F, 0x0C },
	{ 0x80, 0x01 },
	{ 0x81, 0x00 },
	{ 0x82, 0x00 },
	{ 0x89, 0x0C },
	{ 0x8A, 0x01 },
	{ 0x8B, 0x00 },
	{ 0x8C, 0x00 },
	{ 0x8E, 0x0C },
	{ 0x8F, 0x01 },
	{ 0x90, 0x00 },
	{ 0x91, 0x00 },
	{ 0x98, 0x0C },
	{ 0x99, 0x01 },
	{ 0x9A, 0x00 },
	{ 0x9B, 0x00 },
	{ 0x9D, 0x0C },
	{ 0x9E, 0x01 },
	{ 0x9F, 0x00 },
	{ 0xA0, 0x00 },
	{ 0xA7, 0x0C },
	{ 0xA8, 0x01 },
	{ 0xA9, 0x00 },
	{ 0xAA, 0x00 },
	{ 0xAC, 0x0C },
	{ 0xAD, 0x01 },
	{ 0xAE, 0x00 },
	{ 0xAF, 0x00 },
	{ 0xB6, 0xDB },
	{ 0xB7, 0x06 },
	{ 0xB9, 0x06 },
	{ 0xBA, 0x7E },
	{ 0xBC, 0x00 },
	{ 0xBD, 0x00 },
	{ 0xBE, 0x20 },
	{ 0xBF, 0x00 },
	{ 0xC0, 0x00 },
	{ 0xC1, 0x00 },

};

/*
 * Design Report
 *
 * Overview
 * ========
 * Part:               Si5332-GM2 Rev C
 * Project File:       D:\Horizon\ADM10\Si5332\Si5332-GM2-RevD-Project.slabtimeproj
 * Design ID:          <none>
 * Created By:         ClockBuilder Pro v2.29 [2018-11-04]
 * Timestamp:          2019-01-10 18:45:47 GMT+03:00
 * 
 * Design Rule Check
 * =================
 * Errors:
 * - No errors
 * 
 * Warnings:
 * - No warnings
 * 
 * Design
 * ======
 * Universal Hardware Input Pins:
 *    INPUT1 (P10): None
 *    INPUT2 (P19): None
 *    INPUT3 (P20): None
 *    INPUT4 (P31): None
 *    INPUT5 (P32): None
 *    INPUT6 (P36): None
 *    INPUT7 (P37): None
 * 
 * Inputs:
 *    XAXB: 38.8 MHz [ 38 + 4/5 MHz ]
 *          Oscillator Mode
 *  CLKIN2: Unused
 *  CLKIN3: Unused
 * 
 * Outputs:
 *    OUT0: 75 MHz LVPECL 3.3 V
 *    OUT1: 75 MHz LVPECL 3.3 V
 *    OUT2: 75 MHz LVPECL 3.3 V
 *    OUT3: 75 MHz LVPECL 3.3 V
 *    OUT4: 75 MHz LVPECL 3.3 V
 *    OUT5: 75 MHz LVPECL 3.3 V
 *    OUT6: 75 MHz LVPECL 3.3 V
 *    OUT7: 75 MHz LVPECL 3.3 V
 * 
 * Frequency Plan
 * ==============
 * Fpfd = 38.8 MHz [ 38 + 4/5 MHz ]
 * Fvco = 2.4 GHz [ 2 + 2/5 GHz ]
 * 
 * P divider = 1
 * M = 61.8556701030927835... [ 61 + 83/97 ]
 * N dividers:
 *    N0:
 *       Unused
 *    N1:
 *       Unused
 * 
 * O dividers:
 *    O0:
 *       Value: 32
 *       OUT0: 75 MHz
 *       OUT1: 75 MHz
 *       OUT2: 75 MHz
 *       OUT3: 75 MHz
 *       OUT4: 75 MHz
 *       OUT5: 75 MHz
 *       OUT6: 75 MHz
 *       OUT7: 75 MHz
 *    O1:
 *       Unused
 *    O2:
 *       Unused
 *    O3:
 *       Unused
 *    O4:
 *       Unused
 * 
 * R dividers:
 *    R0 = 1
 *    R1 = 1
 *    R2 = 1
 *    R3 = 1
 *    R4 = 1
 *    R5 = 1
 *    R6 = 1
 *    R7 = 1
 * 
 * Estimated Power
 * ===============
 * Total Power: 875 mW, Tj: 92 °C
 * 
 *                  Frequency  Format   Voltage   Current     Power
 *                  ---------  ------  --------  --------  --------
 * VDD                                    1.8 V   19.7 mA     35 mW
 * VDD Dig                                1.8 V    4.8 mA      9 mW
 * VDD Xtal                               1.8 V    2.3 mA      4 mW
 * VDDO0                                                           
 *            OUT0     75 MHz  LVPECL     3.3 V   31.3 mA    103 mW
 * VDDO1                                                           
 *            OUT1     75 MHz  LVPECL     3.3 V   31.3 mA    103 mW
 * VDDO2                                                           
 *            OUT2     75 MHz  LVPECL     3.3 V   31.3 mA    103 mW
 *            OUT3     75 MHz  LVPECL     3.3 V   31.3 mA    103 mW
 * VDDO3                                                           
 *            OUT4     75 MHz  LVPECL     3.3 V   31.3 mA    103 mW
 *            OUT5     75 MHz  LVPECL     3.3 V   31.3 mA    103 mW
 * VDDO4                                                           
 *            OUT6     75 MHz  LVPECL     3.3 V   31.3 mA    103 mW
 * VDDO5                                                           
 *            OUT7     75 MHz  LVPECL     3.3 V   31.3 mA    103 mW
 *                                               --------  --------
 *                                        Total  277.2 mA    875 mW
 * 
 * Note:
 * 
 * -Total power includes on- and off-chip power. This is a typical value and estimate only.
 * -Use an EVB for a more exact power measurement
 * -On-chip power excludes power dissipated in external terminations.
 * 
 * Settings
 * ========
 * 
 * Location    Setting Name     Decimal Value      Hex Value        
 * ----------  ---------------  -----------------  -----------------
 * 0x17[7:0]   DESIGN_ID0       0                  0x00             
 * 0x18[7:0]   DESIGN_ID1       0                  0x00             
 * 0x19[7:0]   DESIGN_ID2       0                  0x00             
 * 0x1A[7:0]   DESIGN_ID3       0                  0x00             
 * 0x1B[7:0]   DESIGN_ID4       0                  0x00             
 * 0x1C[7:0]   DESIGN_ID5       0                  0x00             
 * 0x21[6:0]   I2C_ADDR         106                0x6A             
 * 0x24[1:0]   IMUX_SEL         1                  0x1              
 * 0x25[1:0]   OMUX0_SEL0       0                  0x0              
 * 0x25[6:4]   OMUX0_SEL1       0                  0x0              
 * 0x26[1:0]   OMUX1_SEL0       0                  0x0              
 * 0x26[6:4]   OMUX1_SEL1       0                  0x0              
 * 0x27[1:0]   OMUX2_SEL0       0                  0x0              
 * 0x27[6:4]   OMUX2_SEL1       0                  0x0              
 * 0x28[1:0]   OMUX3_SEL0       0                  0x0              
 * 0x28[6:4]   OMUX3_SEL1       0                  0x0              
 * 0x29[1:0]   OMUX4_SEL0       0                  0x0              
 * 0x29[6:4]   OMUX4_SEL1       0                  0x0              
 * 0x2A[1:0]   OMUX5_SEL0       0                  0x0              
 * 0x2A[6:4]   OMUX5_SEL1       0                  0x0              
 * 0x2B[7:0]   HSDIV0A_DIV      32                 0x20             
 * 0x3C[0]     ID0A_SS_ENA      0                  0x0              
 * 0x3C[2:1]   ID0A_SS_MODE     0                  0x0              
 * 0x48[0]     ID0B_SS_ENA      0                  0x0              
 * 0x48[2:1]   ID0B_SS_MODE     0                  0x0              
 * 0x54[0]     ID1A_SS_ENA      0                  0x0              
 * 0x54[2:1]   ID1A_SS_MODE     0                  0x0              
 * 0x60[0]     ID1B_SS_ENA      0                  0x0              
 * 0x60[2:1]   ID1B_SS_MODE     0                  0x0              
 * 0x67[14:0]  IDPA_INTG        7917               0x1EED           
 * 0x69[14:0]  IDPA_RES         51                 0x0033           
 * 0x6B[14:0]  IDPA_DEN         97                 0x0061           
 * 0x73[1:0]   CLKIN_2_CLK_SEL  0                  0x0              
 * 0x74[1:0]   CLKIN_3_CLK_SEL  0                  0x0              
 * 0x75[4:0]   P_VAL            1                  0x01             
 * 0x7A[3:0]   OUT0_MODE        12                 0xC              
 * 0x7B[5:0]   OUT0_DIV         1                  0x01             
 * 0x7C[2:0]   OUT0_SKEW        0                  0x0              
 * 0x7D[0]     OUT0_STOP_HIGHZ  0                  0x0              
 * 0x7F[3:0]   OUT1_MODE        12                 0xC              
 * 0x80[5:0]   OUT1_DIV         1                  0x01             
 * 0x81[2:0]   OUT1_SKEW        0                  0x0              
 * 0x82[0]     OUT1_STOP_HIGHZ  0                  0x0              
 * 0x89[3:0]   OUT2_MODE        12                 0xC              
 * 0x8A[5:0]   OUT2_DIV         1                  0x01             
 * 0x8B[2:0]   OUT2_SKEW        0                  0x0              
 * 0x8C[0]     OUT2_STOP_HIGHZ  0                  0x0              
 * 0x8E[3:0]   OUT3_MODE        12                 0xC              
 * 0x8F[5:0]   OUT3_DIV         1                  0x01             
 * 0x90[2:0]   OUT3_SKEW        0                  0x0              
 * 0x91[0]     OUT3_STOP_HIGHZ  0                  0x0              
 * 0x98[3:0]   OUT4_MODE        12                 0xC              
 * 0x99[5:0]   OUT4_DIV         1                  0x01             
 * 0x9A[2:0]   OUT4_SKEW        0                  0x0              
 * 0x9B[0]     OUT4_STOP_HIGHZ  0                  0x0              
 * 0x9D[3:0]   OUT5_MODE        12                 0xC              
 * 0x9E[5:0]   OUT5_DIV         1                  0x01             
 * 0x9F[2:0]   OUT5_SKEW        0                  0x0              
 * 0xA0[0]     OUT5_STOP_HIGHZ  0                  0x0              
 * 0xA7[3:0]   OUT6_MODE        12                 0xC              
 * 0xA8[5:0]   OUT6_DIV         1                  0x01             
 * 0xA9[2:0]   OUT6_SKEW        0                  0x0              
 * 0xAA[0]     OUT6_STOP_HIGHZ  0                  0x0              
 * 0xAC[3:0]   OUT7_MODE        12                 0xC              
 * 0xAD[5:0]   OUT7_DIV         1                  0x01             
 * 0xAE[2:0]   OUT7_SKEW        0                  0x0              
 * 0xAF[0]     OUT7_STOP_HIGHZ  0                  0x0              
 * 0xB6[0]     OUT0_OE          1                  0x1              
 * 0xB6[1]     OUT1_OE          1                  0x1              
 * 0xB6[3]     OUT2_OE          1                  0x1              
 * 0xB6[4]     OUT3_OE          1                  0x1              
 * 0xB6[6]     OUT4_OE          1                  0x1              
 * 0xB6[7]     OUT5_OE          1                  0x1              
 * 0xB7[1]     OUT6_OE          1                  0x1              
 * 0xB7[2]     OUT7_OE          1                  0x1              
 * 0xB9[0]     XOSC_DIS         0                  0x0              
 * 0xB9[1]     IBUF0_DIS        1                  0x1              
 * 0xB9[2]     IBUF1_DIS        1                  0x1              
 * 0xB9[3]     IMUX_DIS         0                  0x0              
 * 0xB9[4]     PDIV_DIS         0                  0x0              
 * 0xB9[5]     PLL_DIS          0                  0x0              
 * 0xBA[5]     ID0_DIS          1                  0x1              
 * 0xBA[6]     ID1_DIS          1                  0x1              
 * 0xBA[0]     HSDIV0_DIS       0                  0x0              
 * 0xBA[1]     HSDIV1_DIS       1                  0x1              
 * 0xBA[2]     HSDIV2_DIS       1                  0x1              
 * 0xBA[3]     HSDIV3_DIS       1                  0x1              
 * 0xBA[4]     HSDIV4_DIS       1                  0x1              
 * 0xBC[0]     OUT0_DIS         0                  0x0              
 * 0xBC[1]     OUT1_DIS         0                  0x0              
 * 0xBC[3]     OUT2_DIS         0                  0x0              
 * 0xBC[4]     OUT3_DIS         0                  0x0              
 * 0xBC[6]     OUT4_DIS         0                  0x0              
 * 0xBC[7]     OUT5_DIS         0                  0x0              
 * 0xBD[1]     OUT6_DIS         0                  0x0              
 * 0xBD[2]     OUT7_DIS         0                  0x0              
 * 0xBE[7:0]   PLL_MODE         32                 0x20             
 * 0xBF[0]     XOSC_CINT_ENA    0                  0x0              
 * 0xC0[5:0]   XOSC_CTRIM_XIN   0                  0x00             
 * 0xC1[5:0]   XOSC_CTRIM_XOUT  0                  0x00
 * 
 *
 */

#endif
