/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder,Transform type tables
//
*/

#include "umc_config.h"
#if defined UMC_ENABLE_VC1_VIDEO_DECODER || defined UMC_ENABLE_VC1_SPLITTER || defined UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_common_defs.h"
#include "umc_vc1_common_ttmb_tbl.h"

//VC-1 Table 53: High Rate (PQUANT < 5) TTMB VLC Table
//TTMB VLC    Transform  Type    Signal Level     Subblock Pattern
//11              8x8                 Block           NA
//101110          8x4                 Block           Bottom
//1011111         8x4                 Block           Top
//00              8x4                 Block           Both
//10110           4x8                 Block           Right
//10101           4x8                 Block           Left
//01              4x8                 Block           Both
//100             4x4                 Block           NA
//10100           8x8                 Macroblock      NA
//1011110001      8x4                 Macroblock      Bottom
//101111001       8x4                 Macroblock      Top
//101111011       8x4                 Macroblock      Both
//101111000000    4x8                 Macroblock      Right
//101111000001    4x8                 Macroblock      Left
//10111100001     4x8                 Macroblock      Both
//101111010       4x4                 Macroblock      NA

extern const Ipp32s VC1_HighRateTTMB[] =
{
 12, /* max bits */
 2,  /* total subtables */
 6,6,/* subtable sizes */

 0, /* 1-bit codes */
 3, /* 2-bit codes */
    0x0,      VC1_SBP_8X4_BOTH_BLK,
    0x1,      VC1_SBP_4X8_BOTH_BLK,
    0x3,      VC1_SBP_8X8_BLK,

 1, /* 3-bit codes */
    0x4,      VC1_SBP_4X4_BLK,

 0, /* 4-bit codes */
 3, /* 5-bit codes */
    0x16,      VC1_SBP_4X8_RIGHT_BLK,
    0x15,      VC1_SBP_4X8_LEFT_BLK,
    0x14,      VC1_SBP_8X8_MB,

 1, /* 6-bit codes */
    0x2e,      VC1_SBP_8X4_BOTTOM_BLK,

 1,/* 7-bit codes */
    0x5f,      VC1_SBP_8X4_TOP_BLK,

 0, /* 8-bit codes */
 3,/* 9-bit codes */
    0x17a,      VC1_SBP_4X4_MB,
    0x179,      VC1_SBP_8X4_TOP_MB,
    0x17b,      VC1_SBP_8X4_BOTH_MB,

 1, /* 10-bit codes */
    0x2f1,      VC1_SBP_8X4_BOTTOM_MB,

 1, /* 11-bit codes */
    0x5e1,      VC1_SBP_4X8_BOTH_MB,


 2, /* 12-bit codes */
    0x0bc0,      VC1_SBP_4X8_RIGHT_MB,
    0x0bc1,      VC1_SBP_4X8_LEFT_MB,

 -1
 };

//VC-1 Table 54: Medium Rate (5 <= PQUANT < 13) TTMB VLC Table
//TTMB VLC      Transform Type    Signal Level      Subblock Pattern
//110               8x8             Block               NA
//0110              8x4             Block               Bottom
//0011              8x4             Block               Top
//0111              8x4             Block               Both
//1111              4x8             Block               Right
//1110              4x8             Block               Left
//000               4x8             Block               Both
//010               4x4             Block               NA
//10                8x8             Macroblock          NA
//0010100           8x4             Macroblock          Bottom
//0010001           8x4             Macroblock          Top
//001011            8x4             Macroblock          Both
//001001            4x8             Macroblock          Right
//00100001          4x8             Macroblock          Left
//0010101           4x8             Macroblock          Both
//00100000          4x4             Macroblock          NA

extern const Ipp32s VC1_MediumRateTTMB[] =
{
 8, /* max bits */
 2,  /* total subtables */
 4,4,/* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
    0x2,          VC1_SBP_8X8_MB,

 3, /* 3-bit codes */
    0x0,          VC1_SBP_4X8_BOTH_BLK,
    0x2,          VC1_SBP_4X4_BLK,
    0x6,          VC1_SBP_8X8_BLK,

 5, /* 4-bit codes */
    0x6,          VC1_SBP_8X4_BOTTOM_BLK,
    0x3,          VC1_SBP_8X4_TOP_BLK,
    0x7,          VC1_SBP_8X4_BOTH_BLK,
    0xf,          VC1_SBP_4X8_RIGHT_BLK,
    0xe,          VC1_SBP_4X8_LEFT_BLK,
 0, /* 5-bit codes */
 2, /* 6-bit codes */
    0xb,          VC1_SBP_8X4_BOTH_MB,
    0x9,          VC1_SBP_4X8_RIGHT_MB,

 3,/* 7-bit codes */
    0x14,         VC1_SBP_8X4_BOTTOM_MB,
    0x11,         VC1_SBP_8X4_TOP_MB,
    0x15,         VC1_SBP_4X8_BOTH_MB,

 2, /* 8-bit codes */
    0x21,         VC1_SBP_4X8_LEFT_MB,
    0x20,         VC1_SBP_4X4_MB,

 -1
 };
//VC-1 Table 55: Low Rate (PQUANT >= 13) TTMB VLC Table
//TTMB VLC    Transform Type     Signal Level     Subblock Pattern
//110             8x8             Block               NA
//000             8x4             Block               Bottom
//1110            8x4             Block               Top
//00101           8x4             Block               Both
//010             4x8             Block               Right
//011             4x8             Block               Left
//0011            4x8             Block               Both
//1111            4x4             Block               NA
//10              8x8             Macroblock          NA
//0010000001      8x4             Macroblock          Bottom
//00100001        8x4             Macroblock          Top
//001001          8x4             Macroblock          Both
//00100000001     4x8             Macroblock          Right
//001000001       4x8             Macroblock          Left
//0010001         4x8             Macroblock          Both
//00100000000     4x4             Macroblock          NA

extern const Ipp32s VC1_LowRateTTMB[] =
{
 11, /* max bits */
 2,  /* total subtables */
 6,5,/* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
    0x2           ,   VC1_SBP_8X8_MB,
 4, /* 3-bit codes */
    0x6,   VC1_SBP_8X8_BLK,
    0x0,   VC1_SBP_8X4_BOTTOM_BLK,
    0x2,   VC1_SBP_4X8_RIGHT_BLK,
    0x3,   VC1_SBP_4X8_LEFT_BLK,
 3, /* 4-bit codes */
    0xe,   VC1_SBP_8X4_TOP_BLK,
    0x3,   VC1_SBP_4X8_BOTH_BLK,
    0xf,   VC1_SBP_4X4_BLK,
 1, /* 5-bit codes */
    0x5,   VC1_SBP_8X4_BOTH_BLK,
 1, /* 6-bit codes */
    0x09,   VC1_SBP_8X4_BOTH_MB,
 1,/* 7-bit codes */
    0x11,   VC1_SBP_4X8_BOTH_MB,
 1, /* 8-bit codes */
    0x21,   VC1_SBP_8X4_TOP_MB,
 1, /* 9-bit codes */
    0x41,   VC1_SBP_4X8_LEFT_MB,
 1, /* 10-bit codes */
    0x081,   VC1_SBP_8X4_BOTTOM_MB,
 2, /* 11-bit codes */
    0x100,   VC1_SBP_4X4_MB,
    0x101,   VC1_SBP_4X8_RIGHT_MB,
 -1
 };

//VC-1 Table 61: High Rate (PQUANT < 5) TTBLK VLC Table
//TTBLK VLC   Transform Type  Subblock Pattern
//00                8x4             Both
//01                4x8             Both
//11                8x8             NA
//101               4x4             NA
//10000             8x4             Top
//10001             8x4             Bottom
//10010             4x8             Right
//10011             4x8             Left

extern const Ipp32s VC1_HighRateTTBLK[] =
{
 5, /* max bits */
 1,  /* total subtables */
 5,/* subtable sizes */

 0, /* 1-bit codes */

 3, /* 2-bit codes */
    0x0,              VC1_SBP_8X4_BOTH_BLK,
    0x1,              VC1_SBP_4X8_BOTH_BLK,
    0x3,              VC1_SBP_8X8_BLK,
 1, /* 3-bit codes */
    0x5,              VC1_SBP_4X4_BLK,
 0, /* 4-bit codes */
 4, /* 5-bit codes */
    0x10,             VC1_SBP_8X4_TOP_BLK,
    0x11,             VC1_SBP_8X4_BOTTOM_BLK,
    0x12,             VC1_SBP_4X8_RIGHT_BLK,
    0x13,             VC1_SBP_4X8_LEFT_BLK,
 -1
 };

//VC-1 Table 62: Medium Rate (5 =< PQUANT < 13) TTBLK VLC Table
//TTBLK VLC   Transform Type  Subblock Pattern
//11                8x8             NA
//000               4x8             Right
//001               4x8             Left
//010               4x4             NA
//011               8x4             Both
//101               4x8             Both
//1000              8x4             Bottom
//1001              8x4             Top

extern const Ipp32s VC1_MediumRateTTBLK[] =
{
 4, /* max bits */
 1,  /* total subtables */
 4,/* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
    0x3,               VC1_SBP_8X8_BLK,
 5, /* 3-bit codes */
    0x0,               VC1_SBP_4X8_RIGHT_BLK,
    0x1,               VC1_SBP_4X8_LEFT_BLK,
    0x2,               VC1_SBP_4X4_BLK,
    0x3,               VC1_SBP_8X4_BOTH_BLK,
    0x5,               VC1_SBP_4X8_BOTH_BLK,
 2, /* 4-bit codes */
    0x8,               VC1_SBP_8X4_BOTTOM_BLK,
    0x9,               VC1_SBP_8X4_TOP_BLK,
 -1
 };

//VC-1 Table 63: Low Rate (PQUANT >= 13) TTBLK VLC Table
//TTBLK VLC   Transform Type  Subblock Pattern
//01                8x8             NA
//000               4x8             Both
//001               4x4             NA
//100               8x4             Bottom
//110               4x8             Right
//111               4x8             Left
//1010              8x4             Both
//1011              8x4             Top

extern const Ipp32s VC1_LowRateTTBLK[] =
{
 4, /* max bits */
 1,  /* total subtables */
 4,/* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
    0x1,               VC1_SBP_8X8_BLK,
 5, /* 3-bit codes */
    0x0,               VC1_SBP_4X8_BOTH_BLK,
    0x1,               VC1_SBP_4X4_BLK,
    0x4,               VC1_SBP_8X4_BOTTOM_BLK,
    0x6,               VC1_SBP_4X8_RIGHT_BLK,
    0x7,               VC1_SBP_4X8_LEFT_BLK,
 2, /* 4-bit codes */
    0xa,               VC1_SBP_8X4_BOTH_BLK,
    0xb,               VC1_SBP_8X4_TOP_BLK,
 -1
 };

//VC-1 Table 64: High Rate (PQUANT < 5) SUBBLKPAT VLC Table
//SUBBLKPAT VLC   Subblock Pattern    SUBBLKPAT VLC   Subblock Pattern
//1                     15                  01010          8
//0000                  11                  01011          4
//0001                  13                  01100          2
//0010                  7                   01110          1
//00110                 12                  01111          14
//00111                 3                   011010         6
//01000                 10                  011011         9
//01001                 5

extern const Ipp32s VC1_HighRateSBP[] =
{
 6, /* max bits */
 2,  /* total subtables */
 5,1,/* subtable sizes */

 1, /* 1-bit codes */
    0x1,                   15,
 0, /* 2-bit codes */
 0, /* 3-bit codes */
 3, /* 4-bit codes */
    0x0, 11,    0x1, 13,    0x2, 7,
 9, /* 5-bit codes */
    0x6, 12,    0x7, 3,    0x8, 10,    0x9, 5,
    0xa, 8,     0xb, 4,    0xc, 2,     0xe, 1,
    0xf, 14,
 2, /* 6-bit codes */
    0x1a, 6,    0x1b, 9,
 -1
 };

//VC-1 Table 65: Medium Rate (5 =< PQUANT < 13) SUBBLKPAT VLC Table
//SUBBLKPAT VLC   Subblock Pattern    SUBBLKPAT VLC   Subblock Pattern
//01                    15                  1111            4
//000                   2                   00100           6
//0011                  12                  00101           9
//1000                  3                   10110           14
//1001                  10                  10111           7
//1010                  5                   11000           13
//1101                  8                   11001           11
//1110                  1

extern const Ipp32s VC1_MediumRateSBP[] =
{
 5, /* max bits */
 1,  /* total subtables */
 5, /* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
    0x1, 15,
 1, /* 3-bit codes */
    0x0, 2,
 7, /* 4-bit codes */
    0x3, 12,    0x8, 3,    0x9, 10,    0xa, 5,
    0xd, 8,     0xe, 1,    0xf, 4,
 6, /* 5-bit codes */
    0x4, 6,     0x5, 9,    0x16, 14,   0x17, 7,
    0x18, 13,   0x19, 11,
 -1
 };

//VC-1 Table 66: Low Rate (PQUANT >= 13) SUBBLKPAT VLC Table
//SUBBLKPAT VLC   Subblock Pattern    SUBBLKPAT VLC   Subblock Pattern
//010                   4                   1111            15
//011                   8                   00000           6
//101                   1                   00001           9
//110                   2                   10010           14
//0001                  12                  10011           13
//0010                  3                   11100           7
//0011                  10                  11101           11
//1000                  5

extern const Ipp32s VC1_LowRateSBP[] =
{
 5, /* max bits */
 2,  /* total subtables */
 3,2, /* subtable sizes */

 0, /* 1-bit codes */
 0, /* 2-bit codes */
 4, /* 3-bit codes */
    0x2, 4,     0x3, 8,     0x5, 1,     0x6, 2,

 5, /* 4-bit codes */
    0x1, 12,    0x2, 3,     0x3, 10,    0x8, 5,
    0xf, 15,

 6, /* 5-bit codes */
    0x0, 6,    0x1, 9,    0x12, 14,    0x13, 13,
    0x1c, 7,   0x1d, 11,
 -1
 };

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
