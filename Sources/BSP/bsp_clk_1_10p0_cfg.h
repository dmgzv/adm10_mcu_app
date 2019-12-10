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

#ifndef __BSP_CLK_1_10P0_CFG_H
#define __BSP_CLK_1_10P0_CFG_H

#include "si5332.h"

#define BSP_CLK_1_10P0_CFG_NUM_REGS				94

const SI5332_CFG_T bsp_clk_1_10p0_cfg[BSP_CLK_1_10P0_CFG_NUM_REGS] =
{
	{ 0x17, 0x41 },
	{ 0x18, 0x44 },
	{ 0x19, 0x4D },
	{ 0x1A, 0x31 },
	{ 0x1B, 0x30 },
	{ 0x1C, 0x20 },
	{ 0x21, 0x6A },
	{ 0x24, 0x01 },
	{ 0x26, 0x50 },
	{ 0x27, 0x00 },
	{ 0x28, 0x60 },
	{ 0x2A, 0x50 },
	{ 0x2B, 0x0F },
	{ 0x36, 0x08 },
	{ 0x37, 0x00 },
	{ 0x38, 0x01 },
	{ 0x39, 0x80 },
	{ 0x3A, 0x02 },
	{ 0x3B, 0x78 },
	{ 0x3C, 0x00 },
	{ 0x48, 0x00 },
	{ 0x4E, 0x07 },
	{ 0x4F, 0x88 },
	{ 0x50, 0x00 },
	{ 0x51, 0x08 },
	{ 0x52, 0x00 },
	{ 0x53, 0x4F },
	{ 0x54, 0x00 },
	{ 0x60, 0x00 },
	{ 0x67, 0x20 },
	{ 0x68, 0x47 },
	{ 0x69, 0x00 },
	{ 0x6A, 0x17 },
	{ 0x6B, 0x00 },
	{ 0x6C, 0x4F },
	{ 0x73, 0x00 },
	{ 0x74, 0x00 },
	{ 0x75, 0x01 },
	{ 0x7A, 0x0C },
	{ 0x7C, 0x00 },
	{ 0x7D, 0x00 },
	{ 0x7F, 0x07 },
	{ 0x80, 0x01 },
	{ 0x81, 0x00 },
	{ 0x82, 0x00 },
	{ 0x84, 0x07 },
	{ 0x85, 0x01 },
	{ 0x86, 0x00 },
	{ 0x87, 0x00 },
	{ 0x89, 0x07 },
	{ 0x8A, 0x01 },
	{ 0x8B, 0x00 },
	{ 0x8C, 0x00 },
	{ 0x8E, 0x07 },
	{ 0x8F, 0x01 },
	{ 0x90, 0x00 },
	{ 0x91, 0x00 },
	{ 0x93, 0x07 },
	{ 0x94, 0x01 },
	{ 0x95, 0x00 },
	{ 0x96, 0x00 },
	{ 0x98, 0x07 },
	{ 0x99, 0x01 },
	{ 0x9A, 0x00 },
	{ 0x9B, 0x00 },
	{ 0x9D, 0x07 },
	{ 0x9E, 0x01 },
	{ 0x9F, 0x00 },
	{ 0xA0, 0x00 },
	{ 0xA2, 0x07 },
	{ 0xA3, 0x01 },
	{ 0xA4, 0x00 },
	{ 0xA5, 0x00 },
	{ 0xA7, 0x0C },
	{ 0xA9, 0x00 },
	{ 0xAA, 0x00 },
	{ 0xAC, 0x07 },
	{ 0xAD, 0x01 },
	{ 0xAE, 0x00 },
	{ 0xAF, 0x00 },
	{ 0xB1, 0x07 },
	{ 0xB2, 0x01 },
	{ 0xB3, 0x00 },
	{ 0xB4, 0x00 },
	{ 0xB6, 0xFE },
	{ 0xB7, 0x0D },
	{ 0xB9, 0x06 },
	{ 0xBA, 0x1E },
	{ 0xBC, 0x01 },
	{ 0xBD, 0x02 },
	{ 0xBE, 0x20 },
	{ 0xBF, 0x00 },
	{ 0xC0, 0x1F },
	{ 0xC1, 0x1F },
};


/*
 * Design Report
 *
 * Overview
 * ========
 * Part:               Si5332-GM3 Rev C
 * Project File:       E:\Design\Cadence\CYC10GX_r2\si5332\Si5332-38.88-ADM10.slabtimeproj
 * Design ID:          ADM10
 * Created By:         ClockBuilder Pro v2.34.1 [2019-05-31]
 * Timestamp:          2019-06-04 18:21:44 GMT+03:00
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
 *    INPUT2 (P11): None
 *    INPUT3 (P12): None
 *    INPUT4 (P23): None
 *    INPUT5 (P38): None
 *    INPUT6 (P42): None
 *    INPUT7 (P43): None
 * 
 * Inputs:
 *    XAXB: 38.88 MHz
 *          Crystal Mode
 *          Adjusted Capacitance: 10 pF
 *  CLKIN2: Unused
 *  CLKIN3: Unused
 * 
 * Outputs:
 *    OUT0: Unused
 *    OUT1: 156.8268907563025210... MHz [ 156 + 492/595 MHz ] LVDS Fast 1.8 V, Disabled: Stop Low
 *          [ IN0*960/238 ]
 *    OUT2: 156.8268907563025210... MHz [ 156 + 492/595 MHz ] LVDS Fast 1.8 V, Disabled: Stop Low
 *          [ IN0*960/238 ]
 *    OUT3: 167.3316455696202531... MHz [ 167 + 131/395 MHz ] LVDS Fast 1.8 V, Disabled: Stop Low
 *          [ IN0*4*255/237 ]
 *    OUT4: 167.3316455696202531... MHz [ 167 + 131/395 MHz ] LVDS Fast 1.8 V, Disabled: Stop Low
 *          [ IN0*4*255/237 ]
 *    OUT5: 167.3316455696202531... MHz [ 167 + 131/395 MHz ] LVDS Fast 1.8 V, Disabled: Stop Low
 *          [ IN0*4*255/237 ]
 *    OUT6: 166.6285714285714285... MHz [ 166 + 22/35 MHz ] LVDS Fast 1.8 V, Disabled: Stop Low
 *          [ IN0*4*255/238 ]
 *    OUT7: 166.6285714285714285... MHz [ 166 + 22/35 MHz ] LVDS Fast 1.8 V, Disabled: Stop Low
 *          [ IN0*4*255/238 ]
 *    OUT8: 166.6285714285714285... MHz [ 166 + 22/35 MHz ] LVDS Fast 1.8 V, Disabled: Stop Low
 *          [ IN0*4*255/238 ]
 *    OUT9: Unused
 *   OUT10: 156.8268907563025210... MHz [ 156 + 492/595 MHz ] LVDS Fast 1.8 V, Disabled: Stop Low
 *          [ IN0*960/238 ]
 *   OUT11: 156.8268907563025210... MHz [ 156 + 492/595 MHz ] LVDS Fast 1.8 V, Disabled: Stop Low
 *          [ IN0*960/238 ]
 * 
 * Frequency Plan
 * ==============
 * Fpfd = 38.88 MHz
 * Fvco = 2.5099746835443037... GHz [ 2 + 5036/9875 GHz ]
 * 
 * P divider = 1
 * M = 64.5569620253164556... [ 64 + 44/79 ]
 * N dividers:
 *    N0:
 *       Value: 16.0047468354430379... [ 16 + 3/632 ]
 *       OUT1: 156.82689... MHz [ 156 + 492/595 MHz ], Error: 0 ppm
 *       OUT2: 156.82689... MHz [ 156 + 492/595 MHz ], Error: 0 ppm
 *       OUT10: 156.82689... MHz [ 156 + 492/595 MHz ], Error: 0 ppm
 *       OUT11: 156.82689... MHz [ 156 + 492/595 MHz ], Error: 0 ppm
 *    N1:
 *       Value: 15.0632911392405063... [ 15 + 5/79 ]
 *       OUT6: 166.62857... MHz [ 166 + 22/35 MHz ], Error: 0 ppm
 *       OUT7: 166.62857... MHz [ 166 + 22/35 MHz ], Error: 0 ppm
 *       OUT8: 166.62857... MHz [ 166 + 22/35 MHz ], Error: 0 ppm
 * 
 * O dividers:
 *    O0:
 *       Value: 15
 *       OUT3: 167.33164... MHz [ 167 + 131/395 MHz ], Error: 0 ppm
 *       OUT4: 167.33164... MHz [ 167 + 131/395 MHz ], Error: 0 ppm
 *       OUT5: 167.33164... MHz [ 167 + 131/395 MHz ], Error: 0 ppm
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
 *    R0 = Unused
 *    R1 = 1
 *    R2 = 1
 *    R3 = 1
 *    R4 = 1
 *    R5 = 1
 *    R6 = 1
 *    R7 = 1
 *    R8 = 1
 *    R9 = Unused
 *    R10 = 1
 *    R11 = 1
 * 
 * Estimated Power
 * ===============
 * Total Power: 291 mW, Tj: 31 °C
 * 
 *                          Frequency    Format   Voltage   Current     Power
 *                  -----------------  --------  --------  --------  --------
 * VDD                                              1.8 V   38.8 mA     70 mW
 * VDD Dig                                          1.8 V    8.9 mA     16 mW
 * VDD Xtal                                         1.8 V    2.3 mA      4 mW
 * VDDO0                                                                     
 *           Unused                                                           
 * VDDO1                                                                     
 *            OUT1  156.826890&hellip; MHz  LVDSFast     1.8 V   11.1 mA     20 mW
 *            OUT2  156.826890&hellip; MHz  LVDSFast     1.8 V   11.1 mA     20 mW
 * VDDO2                                                                     
 *            OUT3  167.331645&hellip; MHz  LVDSFast     1.8 V   11.2 mA     20 mW
 *            OUT4  167.331645&hellip; MHz  LVDSFast     1.8 V   11.2 mA     20 mW
 *            OUT5  167.331645&hellip; MHz  LVDSFast     1.8 V   11.2 mA     20 mW
 * VDDO3                                                                     
 *            OUT6  166.628571&hellip; MHz  LVDSFast     1.8 V   11.2 mA     20 mW
 *            OUT7  166.628571&hellip; MHz  LVDSFast     1.8 V   11.2 mA     20 mW
 *            OUT8  166.628571&hellip; MHz  LVDSFast     1.8 V   11.2 mA     20 mW
 * VDDO4                                                                     
 *           Unused                                                           
 * VDDO5                                                                     
 *           OUT10  156.826890&hellip; MHz  LVDSFast     1.8 V   11.1 mA     20 mW
 *           OUT11  156.826890&hellip; MHz  LVDSFast     1.8 V   11.1 mA     20 mW
 *                                                         --------  --------
 *                                                  Total  161.8 mA    291 mW
 * 
 * Note:
 * 
 * -Tj is junction temperature.
 * -Overall power includes on-chip power dissipation and adds differential load power dissipation to estimate total power requirements.
 * -Above are estimates only: power and temperature should be measured on your PCB.
 * 
 * Settings
 * ========
 * 
 * Location    Setting Name      Decimal Value      Hex Value        
 * ----------  ----------------  -----------------  -----------------
 * 0x17[7:0]   DESIGN_ID0        65                 0x41             
 * 0x18[7:0]   DESIGN_ID1        68                 0x44             
 * 0x19[7:0]   DESIGN_ID2        77                 0x4D             
 * 0x1A[7:0]   DESIGN_ID3        49                 0x31             
 * 0x1B[7:0]   DESIGN_ID4        48                 0x30             
 * 0x1C[7:0]   DESIGN_ID5        32                 0x20             
 * 0x21[6:0]   I2C_ADDR          106                0x6A             
 * 0x24[1:0]   IMUX_SEL          1                  0x1              
 * 0x26[1:0]   OMUX1_SEL0        0                  0x0              
 * 0x26[6:4]   OMUX1_SEL1        5                  0x5              
 * 0x27[1:0]   OMUX2_SEL0        0                  0x0              
 * 0x27[6:4]   OMUX2_SEL1        0                  0x0              
 * 0x28[1:0]   OMUX3_SEL0        0                  0x0              
 * 0x28[6:4]   OMUX3_SEL1        6                  0x6              
 * 0x2A[1:0]   OMUX5_SEL0        0                  0x0              
 * 0x2A[6:4]   OMUX5_SEL1        5                  0x5              
 * 0x2B[7:0]   HSDIV0A_DIV       15                 0x0F             
 * 0x36[14:0]  ID0A_INTG         2048               0x0800           
 * 0x38[14:0]  ID0A_RES          384                0x0180           
 * 0x3A[14:0]  ID0A_DEN          632                0x0278           
 * 0x3C[0]     ID0A_SS_ENA       0                  0x0              
 * 0x3C[2:1]   ID0A_SS_MODE      0                  0x0              
 * 0x48[0]     ID0B_SS_ENA       0                  0x0              
 * 0x48[2:1]   ID0B_SS_MODE      0                  0x0              
 * 0x4E[14:0]  ID1A_INTG         1928               0x0788           
 * 0x50[14:0]  ID1A_RES          8                  0x0008           
 * 0x52[14:0]  ID1A_DEN          79                 0x004F           
 * 0x54[0]     ID1A_SS_ENA       0                  0x0              
 * 0x54[2:1]   ID1A_SS_MODE      0                  0x0              
 * 0x60[0]     ID1B_SS_ENA       0                  0x0              
 * 0x60[2:1]   ID1B_SS_MODE      0                  0x0              
 * 0x67[14:0]  IDPA_INTG         8263               0x2047           
 * 0x69[14:0]  IDPA_RES          23                 0x0017           
 * 0x6B[14:0]  IDPA_DEN          79                 0x004F           
 * 0x73[1:0]   CLKIN_2_CLK_SEL   0                  0x0              
 * 0x74[1:0]   CLKIN_3_CLK_SEL   0                  0x0              
 * 0x75[4:0]   P_VAL             1                  0x01             
 * 0x7A[3:0]   OUT0_MODE         12                 0xC              
 * 0x7C[2:0]   OUT0_SKEW         0                  0x0              
 * 0x7D[0]     OUT0_STOP_HIGHZ   0                  0x0              
 * 0x7D[5:4]   OUT0_CMOS_INV     0                  0x0              
 * 0x7D[6]     OUT0_DIFF_INV     0                  0x0              
 * 0x7F[3:0]   OUT1_MODE         7                  0x7              
 * 0x80[5:0]   OUT1_DIV          1                  0x01             
 * 0x81[2:0]   OUT1_SKEW         0                  0x0              
 * 0x82[0]     OUT1_STOP_HIGHZ   0                  0x0              
 * 0x82[5:4]   OUT1_CMOS_INV     0                  0x0              
 * 0x82[6]     OUT1_DIFF_INV     0                  0x0              
 * 0x84[3:0]   OUT2_MODE         7                  0x7              
 * 0x85[5:0]   OUT2_DIV          1                  0x01             
 * 0x86[2:0]   OUT2_SKEW         0                  0x0              
 * 0x87[0]     OUT2_STOP_HIGHZ   0                  0x0              
 * 0x87[5:4]   OUT2_CMOS_INV     0                  0x0              
 * 0x87[6]     OUT2_DIFF_INV     0                  0x0              
 * 0x89[3:0]   OUT3_MODE         7                  0x7              
 * 0x8A[5:0]   OUT3_DIV          1                  0x01             
 * 0x8B[2:0]   OUT3_SKEW         0                  0x0              
 * 0x8C[0]     OUT3_STOP_HIGHZ   0                  0x0              
 * 0x8C[5:4]   OUT3_CMOS_INV     0                  0x0              
 * 0x8C[6]     OUT3_DIFF_INV     0                  0x0              
 * 0x8E[3:0]   OUT4_MODE         7                  0x7              
 * 0x8F[5:0]   OUT4_DIV          1                  0x01             
 * 0x90[2:0]   OUT4_SKEW         0                  0x0              
 * 0x91[0]     OUT4_STOP_HIGHZ   0                  0x0              
 * 0x91[5:4]   OUT4_CMOS_INV     0                  0x0              
 * 0x91[6]     OUT4_DIFF_INV     0                  0x0              
 * 0x93[3:0]   OUT5_MODE         7                  0x7              
 * 0x94[5:0]   OUT5_DIV          1                  0x01             
 * 0x95[2:0]   OUT5_SKEW         0                  0x0              
 * 0x96[0]     OUT5_STOP_HIGHZ   0                  0x0              
 * 0x96[5:4]   OUT5_CMOS_INV     0                  0x0              
 * 0x96[6]     OUT5_DIFF_INV     0                  0x0              
 * 0x98[3:0]   OUT6_MODE         7                  0x7              
 * 0x99[5:0]   OUT6_DIV          1                  0x01             
 * 0x9A[2:0]   OUT6_SKEW         0                  0x0              
 * 0x9B[0]     OUT6_STOP_HIGHZ   0                  0x0              
 * 0x9B[5:4]   OUT6_CMOS_INV     0                  0x0              
 * 0x9B[6]     OUT6_DIFF_INV     0                  0x0              
 * 0x9D[3:0]   OUT7_MODE         7                  0x7              
 * 0x9E[5:0]   OUT7_DIV          1                  0x01             
 * 0x9F[2:0]   OUT7_SKEW         0                  0x0              
 * 0xA0[0]     OUT7_STOP_HIGHZ   0                  0x0              
 * 0xA0[5:4]   OUT7_CMOS_INV     0                  0x0              
 * 0xA0[6]     OUT7_DIFF_INV     0                  0x0              
 * 0xA2[3:0]   OUT8_MODE         7                  0x7              
 * 0xA3[5:0]   OUT8_DIV          1                  0x01             
 * 0xA4[2:0]   OUT8_SKEW         0                  0x0              
 * 0xA5[0]     OUT8_STOP_HIGHZ   0                  0x0              
 * 0xA5[5:4]   OUT8_CMOS_INV     0                  0x0              
 * 0xA5[6]     OUT8_DIFF_INV     0                  0x0              
 * 0xA7[3:0]   OUT9_MODE         12                 0xC              
 * 0xA9[2:0]   OUT9_SKEW         0                  0x0              
 * 0xAA[0]     OUT9_STOP_HIGHZ   0                  0x0              
 * 0xAA[5:4]   OUT9_CMOS_INV     0                  0x0              
 * 0xAA[6]     OUT9_DIFF_INV     0                  0x0              
 * 0xAC[3:0]   OUT10_MODE        7                  0x7              
 * 0xAD[5:0]   OUT10_DIV         1                  0x01             
 * 0xAE[2:0]   OUT10_SKEW        0                  0x0              
 * 0xAF[0]     OUT10_STOP_HIGHZ  0                  0x0              
 * 0xAF[5:4]   OUT10_CMOS_INV    0                  0x0              
 * 0xAF[6]     OUT10_DIFF_INV    0                  0x0              
 * 0xB1[3:0]   OUT11_MODE        7                  0x7              
 * 0xB2[5:0]   OUT11_DIV         1                  0x01             
 * 0xB3[2:0]   OUT11_SKEW        0                  0x0              
 * 0xB4[0]     OUT11_STOP_HIGHZ  0                  0x0              
 * 0xB4[5:4]   OUT11_CMOS_INV    0                  0x0              
 * 0xB4[6]     OUT11_DIFF_INV    0                  0x0              
 * 0xB6[0]     OUT0_OE           0                  0x0              
 * 0xB6[1]     OUT1_OE           1                  0x1              
 * 0xB6[2]     OUT2_OE           1                  0x1              
 * 0xB6[3]     OUT3_OE           1                  0x1              
 * 0xB6[4]     OUT4_OE           1                  0x1              
 * 0xB6[5]     OUT5_OE           1                  0x1              
 * 0xB6[6]     OUT6_OE           1                  0x1              
 * 0xB6[7]     OUT7_OE           1                  0x1              
 * 0xB7[0]     OUT8_OE           1                  0x1              
 * 0xB7[1]     OUT9_OE           0                  0x0              
 * 0xB7[2]     OUT10_OE          1                  0x1              
 * 0xB7[3]     OUT11_OE          1                  0x1              
 * 0xB9[0]     XOSC_DIS          0                  0x0              
 * 0xB9[1]     IBUF0_DIS         1                  0x1              
 * 0xB9[2]     IBUF1_DIS         1                  0x1              
 * 0xB9[3]     IMUX_DIS          0                  0x0              
 * 0xB9[4]     PDIV_DIS          0                  0x0              
 * 0xB9[5]     PLL_DIS           0                  0x0              
 * 0xBA[5]     ID0_DIS           0                  0x0              
 * 0xBA[6]     ID1_DIS           0                  0x0              
 * 0xBA[0]     HSDIV0_DIS        0                  0x0              
 * 0xBA[1]     HSDIV1_DIS        1                  0x1              
 * 0xBA[2]     HSDIV2_DIS        1                  0x1              
 * 0xBA[3]     HSDIV3_DIS        1                  0x1              
 * 0xBA[4]     HSDIV4_DIS        1                  0x1              
 * 0xBC[0]     OUT0_DIS          1                  0x1              
 * 0xBC[1]     OUT1_DIS          0                  0x0              
 * 0xBC[2]     OUT2_DIS          0                  0x0              
 * 0xBC[3]     OUT3_DIS          0                  0x0              
 * 0xBC[4]     OUT4_DIS          0                  0x0              
 * 0xBC[5]     OUT5_DIS          0                  0x0              
 * 0xBC[6]     OUT6_DIS          0                  0x0              
 * 0xBC[7]     OUT7_DIS          0                  0x0              
 * 0xBD[0]     OUT8_DIS          0                  0x0              
 * 0xBD[1]     OUT9_DIS          1                  0x1              
 * 0xBD[2]     OUT10_DIS         0                  0x0              
 * 0xBD[3]     OUT11_DIS         0                  0x0              
 * 0xBE[7:0]   PLL_MODE          32                 0x20             
 * 0xBF[0]     XOSC_CINT_ENA     0                  0x0              
 * 0xC0[5:0]   XOSC_CTRIM_XIN    31                 0x1F             
 * 0xC1[5:0]   XOSC_CTRIM_XOUT   31                 0x1F
 * 
 *
 */
 
#endif
