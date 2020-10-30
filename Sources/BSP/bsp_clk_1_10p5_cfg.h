/*
 * Si5332-GM3 Rev C Configuration Register Export Header File
 *
 * This file represents a series of Silicon Labs Si5332-GM3 Rev C 
 * register writes that can be performed to load a single configuration 
 * on a device. It was created by a Silicon Labs ClockBuilder Pro
 * export tool.
 *
 * Part:		                                       Si5332-GM3 Rev C
 * Design ID:                                          ADM10
 * Includes Pre/Post Download Control Register Writes: Yes
 * Created By:                                         ClockBuilder Pro v2.45 [2020-07-16]
 * Timestamp:                                          2020-10-23 09:55:49 GMT+03:00
 *
 * A complete design report corresponding to this export is included at the end 
 * of this header file.
 *
 */

#ifndef __BSP_CLK_1_10P5_CFG_H
#define __BSP_CLK_1_10P5_CFG_H

#include "si5332.h"

#define BSP_CLK_1_10P5_CFG_NUM_REGS				94

const SI5332_CFG_T bsp_clk_1_10p5_cfg[BSP_CLK_1_10P5_CFG_NUM_REGS] =
{
	/* Start configuration registers */
	{ 0x0017, 0x41 },
	{ 0x0018, 0x44 },
	{ 0x0019, 0x4D },
	{ 0x001A, 0x31 },
	{ 0x001B, 0x30 },
	{ 0x001C, 0x20 },
	{ 0x0021, 0x6A },
	{ 0x0024, 0x01 },
	{ 0x0026, 0x50 },
	{ 0x0027, 0x00 },
	{ 0x0028, 0x60 },
	{ 0x002A, 0x50 },
	{ 0x002B, 0x0F },
	{ 0x0036, 0x08 },
	{ 0x0037, 0x00 },
	{ 0x0038, 0x01 },
	{ 0x0039, 0x80 },
	{ 0x003A, 0x02 },
	{ 0x003B, 0x78 },
	{ 0x003C, 0x00 },
	{ 0x0048, 0x00 },
	{ 0x004E, 0x07 },
	{ 0x004F, 0x88 },
	{ 0x0050, 0x00 },
	{ 0x0051, 0x08 },
	{ 0x0052, 0x00 },
	{ 0x0053, 0x4F },
	{ 0x0054, 0x00 },
	{ 0x0060, 0x00 },
	{ 0x0067, 0x20 },
	{ 0x0068, 0x47 },
	{ 0x0069, 0x00 },
	{ 0x006A, 0x17 },
	{ 0x006B, 0x00 },
	{ 0x006C, 0x4F },
	{ 0x0073, 0x00 },
	{ 0x0074, 0x00 },
	{ 0x0075, 0x01 },
	{ 0x007A, 0x0C },
	{ 0x007C, 0x00 },
	{ 0x007D, 0x00 },
	{ 0x007F, 0x07 },
	{ 0x0080, 0x01 },
	{ 0x0081, 0x00 },
	{ 0x0082, 0x00 },
	{ 0x0084, 0x07 },
	{ 0x0085, 0x01 },
	{ 0x0086, 0x00 },
	{ 0x0087, 0x00 },
	{ 0x0089, 0x07 },
	{ 0x008A, 0x01 },
	{ 0x008B, 0x00 },
	{ 0x008C, 0x00 },
	{ 0x008E, 0x07 },
	{ 0x0090, 0x00 },
	{ 0x0091, 0x00 },
	{ 0x0093, 0x07 },
	{ 0x0095, 0x00 },
	{ 0x0096, 0x00 },
	{ 0x0098, 0x07 },
	{ 0x009A, 0x00 },
	{ 0x009B, 0x00 },
	{ 0x009D, 0x07 },
	{ 0x009E, 0x01 },
	{ 0x009F, 0x00 },
	{ 0x00A0, 0x00 },
	{ 0x00A2, 0x07 },
	{ 0x00A3, 0x01 },
	{ 0x00A4, 0x00 },
	{ 0x00A5, 0x00 },
	{ 0x00A7, 0x0C },
	{ 0x00A9, 0x00 },
	{ 0x00AA, 0x00 },
	{ 0x00AC, 0x07 },
	{ 0x00AD, 0x01 },
	{ 0x00AE, 0x00 },
	{ 0x00AF, 0x00 },
	{ 0x00B1, 0x07 },
	{ 0x00B2, 0x01 },
	{ 0x00B3, 0x00 },
	{ 0x00B4, 0x00 },
	{ 0x00B6, 0x8E },
	{ 0x00B7, 0x0D },
	{ 0x00B9, 0x06 },
	{ 0x00BA, 0x1E },
	{ 0x00BB, 0x11 },
	{ 0x00BC, 0x71 },
	{ 0x00BD, 0x02 },
	{ 0x00BE, 0x20 },
	{ 0x00BF, 0x00 },
	{ 0x00C0, 0x22 },
	{ 0x00C1, 0x22 },
	/* End configuration registers */
};


/*
 * Design Report
 *
 * Overview
 * ========
 * Part:               Si5332AB Rev C
 * Project File:       D:\Quartus\ADM10\si5332\Si5332-38.88-ADM10.slabtimeproj
 * Design ID:          ADM10
 * Created By:         ClockBuilder Pro v2.45 [2020-07-16]
 * Timestamp:          2020-10-23 09:55:49 GMT+03:00
 * 
 * Design Rule Check
 * =================
 * Errors:
 * - No errors
 * 
 * Warnings:
 * - No warnings
 * 
 * Device Grade
 * ============
 * Maximum Output Frequency: 167.3316455696202531... MHz
 * Frequency Synthesis Mode: Fractional
 * Frequency Plan Grade:     B
 * Minimum Base OPN:         Si5332B*
 * 
 * Base       Output Clock         Supported Frequency Synthesis Modes
 * OPN Grade  Frequency Range      (Typical Jitter)
 * ---------  ------------------  --------------------------------------------
 * Si5332A    5 MHz to 312.5 MHz  Integer (~230 fs) and fractional (~500 fs)
 * Si5332B*   5 MHz to 200 MHz    "
 * Si5332C    5 MHz to 312.5 MHz  Integer only (~230 fs)
 * Si5332D    5 MHz to 200 MHz    "
 * 
 * * Based on your calculated frequency plan, a Si5332B grade device is
 * sufficient for your design. For more in-system configuration flexibility
 * (higher frequencies and/or to enable fractional synthesis), consider
 * selecting device grade Si5332A when specifying an ordering part number (OPN)
 * for your application. See the datasheet Ordering Guide for more information.
 * 
 * Design
 * ======
 * Base I2C Address: 0x6A
 * 
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
 *          Adjusted Capacitance: 10.5 pF
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
 *    OUT4: Unused
 *    OUT5: Unused
 *    OUT6: Unused
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
 *       OUT7: 166.62857... MHz [ 166 + 22/35 MHz ], Error: 0 ppm
 *       OUT8: 166.62857... MHz [ 166 + 22/35 MHz ], Error: 0 ppm
 * 
 * O dividers:
 *    O0:
 *       Value: 15
 *       OUT3: 167.33164... MHz [ 167 + 131/395 MHz ], Error: 0 ppm
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
 *    R4 = Unused
 *    R5 = Unused
 *    R6 = Unused
 *    R7 = 1
 *    R8 = 1
 *    R9 = Unused
 *    R10 = 1
 *    R11 = 1
 * 
 * Estimated Power
 * ===============
 * Total Power: 232 mW, On Chip Power: 221 mW, Tj: 30 °C
 * 
 *                          Frequency    Format   Voltage   Current     Power
 *                  -----------------  --------  --------  --------  --------
 * VDD                                              1.8 V   39.5 mA     71 mW
 * VDD Dig                                          1.8 V    8.9 mA     16 mW
 * VDD Xtal                                         1.8 V    2.3 mA      4 mW
 * VDDO0                                                                     
 *           Unused                                                           
 * VDDO1                                                                     
 *            OUT1  156.826890&hellip; MHz  LVDSFast     1.8 V   11.1 mA     20 mW
 *            OUT2  156.826890&hellip; MHz  LVDSFast     1.8 V   11.1 mA     20 mW
 * VDDO2                                                                     
 *            OUT3  167.331645&hellip; MHz  LVDSFast     1.8 V   11.2 mA     20 mW
 *           Unused                                                           
 *           Unused                                                           
 * VDDO3                                                                     
 *           Unused                                                           
 *            OUT7  166.628571&hellip; MHz  LVDSFast     1.8 V   11.2 mA     20 mW
 *            OUT8  166.628571&hellip; MHz  LVDSFast     1.8 V   11.2 mA     20 mW
 * VDDO4                                                                     
 *           Unused                                                           
 * VDDO5                                                                     
 *           OUT10  156.826890&hellip; MHz  LVDSFast     1.8 V   11.1 mA     20 mW
 *           OUT11  156.826890&hellip; MHz  LVDSFast     1.8 V   11.1 mA     20 mW
 *                                                         --------  --------
 *                                                  Total  128.8 mA    232 mW
 * 
 * Note:
 * 
 * -Tj is junction temperature. Tj must be less than 125 °C (on Si5332-GM3 Revision C) for device to comply with datasheet specifications. Tj = Ta + Theta_JA*On_Chip_Power.
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
 * 0x90[2:0]   OUT4_SKEW         0                  0x0              
 * 0x91[0]     OUT4_STOP_HIGHZ   0                  0x0              
 * 0x91[5:4]   OUT4_CMOS_INV     0                  0x0              
 * 0x91[6]     OUT4_DIFF_INV     0                  0x0              
 * 0x93[3:0]   OUT5_MODE         7                  0x7              
 * 0x95[2:0]   OUT5_SKEW         0                  0x0              
 * 0x96[0]     OUT5_STOP_HIGHZ   0                  0x0              
 * 0x96[5:4]   OUT5_CMOS_INV     0                  0x0              
 * 0x96[6]     OUT5_DIFF_INV     0                  0x0              
 * 0x98[3:0]   OUT6_MODE         7                  0x7              
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
 * 0xB6[4]     OUT4_OE           0                  0x0              
 * 0xB6[5]     OUT5_OE           0                  0x0              
 * 0xB6[6]     OUT6_OE           0                  0x0              
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
 * 0xBB[0]     OMUX0_DIS         1                  0x1              
 * 0xBB[1]     OMUX1_DIS         0                  0x0              
 * 0xBB[2]     OMUX2_DIS         0                  0x0              
 * 0xBB[3]     OMUX3_DIS         0                  0x0              
 * 0xBB[4]     OMUX4_DIS         1                  0x1              
 * 0xBB[5]     OMUX5_DIS         0                  0x0              
 * 0xBC[0]     OUT0_DIS          1                  0x1              
 * 0xBC[1]     OUT1_DIS          0                  0x0              
 * 0xBC[2]     OUT2_DIS          0                  0x0              
 * 0xBC[3]     OUT3_DIS          0                  0x0              
 * 0xBC[4]     OUT4_DIS          1                  0x1              
 * 0xBC[5]     OUT5_DIS          1                  0x1              
 * 0xBC[6]     OUT6_DIS          1                  0x1              
 * 0xBC[7]     OUT7_DIS          0                  0x0              
 * 0xBD[0]     OUT8_DIS          0                  0x0              
 * 0xBD[1]     OUT9_DIS          1                  0x1              
 * 0xBD[2]     OUT10_DIS         0                  0x0              
 * 0xBD[3]     OUT11_DIS         0                  0x0              
 * 0xBE[7:0]   PLL_MODE          32                 0x20             
 * 0xBF[0]     XOSC_CINT_ENA     0                  0x0              
 * 0xC0[5:0]   XOSC_CTRIM_XIN    34                 0x22             
 * 0xC1[5:0]   XOSC_CTRIM_XOUT   34                 0x22
 * 
 *
 */
 
#endif
