/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, run level tables
//
*/

#include "umc_config.h"
#ifdef UMC_ENABLE_VC1_VIDEO_DECODER

#include "umc_vc1_common_defs.h"
#include "umc_vc1_dec_run_level_tbl.h"
#include "umc_vc1_common_acintra.h"
#include "umc_vc1_common_acinter.h"


//////////////////////////////////////////////////////////////////////////
//////////////////////Intra Decoding Sets/////////////////////////////////
//////////////////////////////////////////////////////////////////////////
VC1ACDecodeSet LowMotionIntraACDecodeSet =
{
    0,
    VC1_LowMotionIntraDeltaLevelLast0,
    VC1_LowMotionIntraDeltaLevelLast1,
    VC1_LowMotionIntraDeltaRunLast0,
    VC1_LowMotionIntraDeltaRunLast1
};

VC1ACDecodeSet HighMotionIntraACDecodeSet =
{
    0,
    VC1_HighMotionIntraDeltaLevelLast0,
    VC1_HighMotionIntraDeltaLevelLast1,
    VC1_HighMotionIntraDeltaRunLast0,
    VC1_HighMotionIntraDeltaRunLast1
};

VC1ACDecodeSet MidRateIntraACDecodeSet =
{
    0,
    VC1_MidRateIntraDeltaLevelLast0,
    VC1_MidRateIntraDeltaLevelLast1,
    VC1_MidRateIntraDeltaRunLast0,
    VC1_MidRateIntraDeltaRunLast1
};

VC1ACDecodeSet HighRateIntraACDecodeSet =
{
    0,
    VC1_HighRateIntraDeltaLevelLast0,
    VC1_HighRateIntraDeltaLevelLast1,
    VC1_HighRateIntraDeltaRunLast0,
    VC1_HighRateIntraDeltaRunLast1
};


VC1ACDecodeSet* IntraACDecodeSetPQINDEXle7[3] =
{
    &HighRateIntraACDecodeSet,
    &HighMotionIntraACDecodeSet,
    &MidRateIntraACDecodeSet
};


VC1ACDecodeSet* IntraACDecodeSetPQINDEXgt7[3] =
{
    &LowMotionIntraACDecodeSet,
    &HighMotionIntraACDecodeSet,
    &MidRateIntraACDecodeSet
};

//////////////////////////////////////////////////////////////////////////
//////////////////////Inter Decoding Sets/////////////////////////////////
//////////////////////////////////////////////////////////////////////////
VC1ACDecodeSet LowMotionInterACDecodeSet =
{
    0,
    VC1_LowMotionInterDeltaLevelLast0,
    VC1_LowMotionInterDeltaLevelLast1,
    VC1_LowMotionInterDeltaRunLast0,
    VC1_LowMotionInterDeltaRunLast1
};

VC1ACDecodeSet HighMotionInterACDecodeSet =
{
    0,
    VC1_HighMotionInterDeltaLevelLast0,
    VC1_HighMotionInterDeltaLevelLast1,
    VC1_HighMotionInterDeltaRunLast0,
    VC1_HighMotionInterDeltaRunLast1
};

VC1ACDecodeSet MidRateInterACDecodeSet =
{
    0,
    VC1_MidRateInterDeltaLevelLast0,
    VC1_MidRateInterDeltaLevelLast1,
    VC1_MidRateInterDeltaRunLast0,
    VC1_MidRateInterDeltaRunLast1
};

VC1ACDecodeSet HighRateInterACDecodeSet =
{
    0,
    VC1_HighRateInterDeltaLevelLast0,
    VC1_HighRateInterDeltaLevelLast1,
    VC1_HighRateInterDeltaRunLast0,
    VC1_HighRateInterDeltaRunLast1
};

VC1ACDecodeSet* InterACDecodeSetPQINDEXle7[3] =
{
    &HighRateInterACDecodeSet,
    &HighMotionInterACDecodeSet,
    &MidRateInterACDecodeSet
};

VC1ACDecodeSet* InterACDecodeSetPQINDEXgt7[3] =
{
    &LowMotionInterACDecodeSet,
    &HighMotionInterACDecodeSet,
    &MidRateInterACDecodeSet
};


//Table 73: DQScale
const Ipp32s VC1_DQScaleTbl[64] =
{
     -1, //NA
     262144, 131072,  87381,  65536,
     52429,  43691,   37449,  32768,
     29127,  26214,   23831,  21845,
     20165,  18725,   17476,  16384,
     15420,  14564,   13797,  13107,
     12483,  11916,   11398,  10923,
     10486,  10082,   9709,   9362,
     9039,   8738,    8456,   8192,
     7944,   7710,    7490,   7282,
     7085,   6899,    6722,   6554,
     6394,   6242,    6096,   5958,
     5825,   5699,    5578,   5461,
     5350,   5243,    5140,   5041,
     4946,   4855,    4766,   4681,
     4599,   4520,    4443,   4369,
     4297,   4228,    4161
};

const  Ipp32s VC1_HighMotionIntraAC[] =
{
 15, /* max bits */
 2,  /* total subtables */
 8,7 ,/* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
    1, 0x000001,        //index = 0, last = 0, run = 0, level = 1
 1, /* 3-bit codes */
    5, 0x000002,        //index = 1, last = 0, run = 0, level = 2

 3, /* 4-bit codes */
    13, 0x000003,       //index = 2,   last = 0, run = 0, level = 3
     0, 0x000101,       //index = 19,  last = 0, run = 1, level = 1
    14, 0x010001,       //index = 119, last = 1, run = 0, level = 1

 5, /* 5-bit codes */
    18, 0x000004,       //index = 3,   last = 0, run = 0, level = 4
    16, 0x000102,       //index = 20,  last = 0, run = 1, level = 2
     3, 0x000201,       //index = 34,  last = 0, run = 2, level = 1
    19, 0x000301,       //index = 46,  last = 0, run = 3, level = 1
    25, 0x010101,       //index = 125, last = 1, run = 1, level = 1

 5, /* 6-bit codes */
    14, 0x000005,       //index = 4, last = 0, run = 0, level = 5
    48, 0x010201,       //index = 130, last = 1, run = 2, level = 1
    63, 0x010301,       //index = 134, last = 1, run = 3, level = 1
    12, 0x000401,       //index = 57, last = 0, run = 4, level = 1
    61, 0x000501,       //index = 63, last = 0, run = 5, level = 1

 10,/* 7-bit codes */
    21, 0x000006,       //index = 5,   last = 0, run = 0, level = 6
     8, 0x000103,       //index = 21,  last = 0, run = 1, level = 3
    17, 0x000202,       //index = 35,  last = 0, run = 2, level = 2
    68, 0x010401,       //index = 138, last = 1, run = 4, level = 1
    99, 0x010501,       //index = 141, last = 1, run = 5, level = 1
   120, 0x000302,       //index = 47,  last = 0, run = 3, level = 2
    27, 0x000701,       //index = 72,  last = 0, run = 7, level = 1
    19, 0x000601,       //index = 68,  last = 0, run = 6, level = 1
   121, 0x000801,       //index = 76,  last = 0, run = 8, level = 1
    69, 0x010002,       //index = 120, last = 1, run = 0, level = 2

 15,/* 8-bit codes */
    19, 0x000007,       //index = 6,   last = 0, run = 0,  level = 7
    63, 0x000008,       //index = 7,   last = 0, run = 0,  level = 8
    32, 0x000104,       //index = 22,  last = 0, run = 1,  level = 4
   196, 0x000203,       //index = 36,  last = 0, run = 2,  level = 3
    33, 0x000901,       //index = 80,  last = 0, run = 9,  level = 1
    53, 0x000A01,       //index = 84,  last = 0, run = 10, level = 1
   197, 0x000B01,       //index = 87,  last = 0, run = 11, level = 1
    21, 0x010601,       //index = 143, last = 1, run = 6,  level = 1
    23, 0x010701,       //index = 145, last = 1, run = 7,  level = 1
    44, 0x010801,       //index = 147, last = 1, run = 8,  level = 1
    47, 0x010901,       //index = 149, last = 1, run = 9,  level = 1
   141, 0x010A01,       //index = 151, last = 1, run = 10, level = 1
   142, 0x010B01,       //index = 153, last = 1, run = 11, level = 1
   251, 0x010C01,       //index = 155, last = 1, run = 12, level = 1
    45, 0x010D01,       //index = 157, last = 1, run = 13, level = 1

 15, /* 9-bit codes */
    75, 0x000009,       //index = 8, last = 0, run = 0, level = 9
   287, 0x00000A,       //index = 9, last = 0, run = 0, level = 10
    41, 0x000105,       //index = 23, last = 0, run = 1, level = 5
   500, 0x000106,       //index = 24, last = 0, run = 1, level = 6
   105, 0x000303,       //index = 48, last = 0, run = 3, level = 3
    36, 0x000402,       //index = 58, last = 0, run = 4, level = 2
    83, 0x000502,       //index = 64, last = 0, run = 5, level = 2
   124, 0x000602,       //index = 69, last = 0, run = 6, level = 2
    72, 0x000C01,       //index = 90, last = 0, run = 12, level = 1
    44, 0x000D01,       //index = 93, last = 0, run = 13, level = 1
   499, 0x010003,       //index = 121, last = 1, run = 0, level = 3
    40, 0x010102,       //index = 126, last = 1, run = 1, level = 2
   121, 0x010E01,       //index = 159, last = 1, run =14 , level = 1
   122, 0x010F01,       //index = 161, last = 1, run = 15, level = 1
    74, IPPVC_ESCAPE,   //IPPVC_ESCAPE

 15, /* 10-bit codes */
   184, 0x00000B,       //index = 10, last = 0, run = 0, level = 11
   995, 0x00000C,       //index = 11, last = 0, run = 0, level = 12
   563, 0x000107,       //index = 25, last = 0, run = 1, level = 7
    75, 0x000204,       //index = 37, last = 0, run = 2, level = 4
   562, 0x000304,       //index = 49, last = 0, run = 3, level = 4
   160, 0x000702,       //index = 73, last = 0, run = 7, level = 2
   993, 0x000802,       //index = 77, last = 0, run = 8, level = 2
   572, 0x000902,       //index = 81, last = 0, run = 9, level = 2
   250, 0x000D02,       //index = 94, last = 0, run = 13, level = 2
   146, 0x000E01,       //index = 96, last = 0, run = 14, level = 1
   163, 0x000F01,       //index = 99, last = 0, run = 15, level = 1
   162, 0x010202,       //index = 131, last = 1, run = 2, level = 2
   165, 0x010302,       //index = 135, last = 1, run = 3, level = 2
   561, 0x011001,       //index = 163, last = 1, run = 16, level = 1
   996, 0x011101,       //index = 164, last = 1, run = 17, level = 1

 21, /* 11-bit codes */
   370, 0x00000D,       //index = 12,  last = 0, run = 0,  level = 13
   480, 0x000108,       //index = 26,  last = 0, run = 1,  level = 8
   180, 0x000205,       //index = 38,  last = 0, run = 2,  level = 5
  2004, 0x000206,       //index = 39,  last = 0, run = 2,  level = 6
  1121, 0x000305,       //index = 50,  last = 0, run = 3,  level = 5
   148, 0x000403,       //index = 59,  last = 0, run = 4,  level = 3
   416, 0x000503,       //index = 65,  last = 0, run = 5,  level = 3
  1985, 0x000603,       //index = 70,  last = 0, run = 6,  level = 3
   373, 0x000A02,       //index = 85,  last = 0, run = 10, level = 2
   372, 0x000B02,       //index = 88,  last = 0, run = 11, level = 2
   419, 0x000C02,       //index = 91,  last = 0, run = 12, level = 2
  2006, 0x000D03,       //index = 95,  last = 0, run = 13, level = 3
   482, 0x001001,       //index = 102, last = 0, run = 16, level = 1
  1988, 0x001101,       //index = 104, last = 0, run = 17, level = 1
  1146, 0x010004,       //index = 122, last = 1, run = 0,  level = 4
   374, 0x010103,       //index = 127, last = 1, run = 1,  level = 3
  1995, 0x010402,       //index = 139, last = 1, run = 4,  level = 2
  1984, 0x010D02,       //index = 158, last = 1, run = 13, level = 2
   417, 0x011201,       //index = 165, last = 1, run = 18, level = 1
   323, 0x011301,       //index = 166, last = 1, run = 19, level = 1
   503, 0x011401,       //index = 167, last = 1, run = 20, level = 1

 25, /* 12-bit codes */
   589, 0x00000E,       //index = 13,  last = 0, run = 0,  level = 14
   986, 0x00000F,       //index = 14,  last = 0, run = 0,  level = 15
   298, 0x000109,       //index = 27,  last = 0, run = 1,  level = 9
   989, 0x00010A,       //index = 28,  last = 0, run = 1,  level = 10
   837, 0x000207,       //index = 40,  last = 0, run = 2,  level = 7
  1004, 0x000306,       //index = 51,  last = 0, run = 3,  level = 6
  2240, 0x000404,       //index = 60,  last = 0, run = 4,  level = 4
   836, 0x000703,       //index = 74,  last = 0, run = 7,  level = 3
  4014, 0x000903,       //index = 82,  last = 0, run = 9,  level = 3
  1005, 0x000F02,       //index = 100, last = 0, run = 15, level = 2
   657, 0x001201,       //index = 106, last = 0, run = 18, level = 1
   659, 0x001301,       //index = 107, last = 0, run = 19, level = 1
  3978, 0x001401,       //index = 108, last = 0, run = 20, level = 1
   751, 0x010203,       //index = 132, last = 1, run = 2,  level = 3
   987, 0x010303,       //index = 136, last = 1, run = 3,  level = 3
   963, 0x010502,       //index = 142, last = 1, run = 5,  level = 2
  2294, 0x010602,       //index = 144, last = 1, run = 6,  level = 2
   367, 0x011501,       //index = 168, last = 1, run = 21, level = 1
   658, 0x011601,       //index = 169, last = 1, run = 22, level = 1
   743, 0x011701,       //index = 170, last = 1, run = 23, level = 1
   364, 0x011801,       //index = 171, last = 1, run = 24, level = 1
   365, 0x011901,       //index = 172, last = 1, run = 25, level = 1
   988, 0x011A01,       //index = 173, last = 1, run = 26, level = 1
  3979, 0x011B01,       //index = 174, last = 1, run = 27, level = 1
   984, 0x011D01,       //index = 176, last = 1, run = 29, level = 1

 35, /* 13-bit codes */
   733, 0x000010,       //index = 15,  last = 0, run = 0,  level = 16
  8021, 0x000011,       //index = 16,  last = 0, run = 0,  level = 17
  1290, 0x00010B,       //index = 29,  last = 0, run = 1,  level = 11
  7977, 0x00010C,       //index = 30,  last = 0, run = 1,  level = 12
   727, 0x000208,       //index = 41,  last = 0, run = 2,  level = 8
  1983, 0x000209,       //index = 42,  last = 0, run = 2,  level = 9
  1312, 0x000307,       //index = 52,  last = 0, run = 3,  level = 7
  7978, 0x000308,       //index = 53,  last = 0, run = 3,  level = 8
   726, 0x000504,       //index = 66,  last = 0, run = 5,  level = 4
   724, 0x000803,       //index = 78,  last = 0, run = 8,  level = 3
  1971, 0x000A03,       //index = 86,  last = 0, run = 10, level = 3
  1925, 0x000B03,       //index = 89,  last = 0, run = 11, level = 3
  1182, 0x000C03,       //index = 92,  last = 0, run = 12, level = 3
  1484, 0x000E02,       //index = 97,  last = 0, run = 14, level = 2
  1289, 0x001501,       //index = 109, last = 0, run = 21, level = 1
  1288, 0x001601,       //index = 110, last = 0, run = 22, level = 1
  1933, 0x001701,       //index = 111, last = 0, run = 23, level = 1
  1982, 0x001801,       //index = 112, last = 0, run = 24, level = 1
  1932, 0x001901,       //index = 113, last = 0, run = 25, level = 1
  1500, 0x010005,       //index = 123, last = 1, run = 0,  level = 5
  1181, 0x010104,       //index = 128, last = 1, run = 1,  level = 4
  1176, 0x010702,       //index = 146, last = 1, run = 7,  level = 2
  1970, 0x010802,       //index = 148, last = 1, run = 8,  level = 2
  8020, 0x010902,       //index = 150, last = 1, run = 9,  level = 2
  1981, 0x010A02,       //index = 152, last = 1, run = 10, level = 2
  4482, 0x010B02,       //index = 154, last = 1, run = 11, level = 2
  1291, 0x010C02,       //index = 156, last = 1, run = 12, level = 2
  8031, 0x010E02,       //index = 160, last = 1, run = 14, level = 2
  8022, 0x010F02,       //index = 162, last = 1, run = 15, level = 2
  1177, 0x011C01,       //index = 175, last = 1, run = 28, level = 1
  1934, 0x011E01,       //index = 177, last = 1, run = 30, level = 1
   725, 0x011F01,       //index = 178, last = 1, run = 31, level = 1
  8030, 0x012001,       //index = 179, last = 1, run = 32, level = 1
  7979, 0x012101,       //index = 180, last = 1, run = 33, level = 1
  1935, 0x012201,       //index = 181, last = 1, run = 34, level = 1

 25, /* 14-bit codes */
  1465, 0x000012,       //index = 17,  last = 0, run = 0,  level = 18
 16046, 0x000013,       //index = 18,  last = 0, run = 0,  level = 19
  2626, 0x00010D,       //index = 31,  last = 0, run = 1,  level = 13
  2360, 0x00020A,       //index = 43,  last = 0, run = 2,  level = 10
  3003, 0x00020B,       //index = 44,  last = 0, run = 2,  level = 11
 15952, 0x000309,       //index = 54,  last = 0, run = 3,  level = 9
 15953, 0x00030A,       //index = 55,  last = 0, run = 3,  level = 10
  3849, 0x000405,       //index = 61,  last = 0, run = 4,  level = 5
  3848, 0x000505,       //index = 67,  last = 0, run = 5,  level = 5
  1196, 0x000604,       //index = 71,  last = 0, run = 6,  level = 4
  3961, 0x000704,       //index = 75,  last = 0, run = 7,  level = 4
  8966, 0x000804,       //index = 79,  last = 0, run = 8,  level = 4
  9182, 0x000904,       //index = 83,  last = 0, run = 9,  level = 4
  2366, 0x000F03,       //index = 101, last = 0, run = 15, level = 3
  1198, 0x001A01,       //index = 114, last = 0, run = 26, level = 1
  3002, 0x001B01,       //index = 115, last = 0, run = 27, level = 1
  8967, 0x001C01,       //index = 116, last = 0, run = 28, level = 1
  2970, 0x001D01,       //index = 117, last = 0, run = 29, level = 1
  9183, 0x010006,       //index = 124, last = 1, run = 0,  level = 6
  9181, 0x010105,       //index = 129, last = 1, run = 1,  level = 5
  1464, 0x010204,       //index = 133, last = 1, run = 2,  level = 4
  2367, 0x010304,       //index = 137, last = 1, run = 3,  level = 4
  1197, 0x012301,       //index = 182, last = 1, run = 35, level = 1
 16047, 0x012401,       //index = 183, last = 1, run = 36, level = 1
  9180, 0x012501,       //index = 184, last = 1, run = 37, level = 1

 10, /* 15-bit codes */
  4722, 0x00010E,       //index = 32,  last = 0, run = 1,  level = 14
  5943, 0x00010F,       //index = 33,  last = 0, run = 1,  level = 15
  2398, 0x00020C,       //index = 45,  last = 0, run = 2,  level = 12
  5254, 0x00030B,       //index = 56,  last = 0, run = 3,  level = 11
  7920, 0x000406,       //index = 62,  last = 0, run = 4,  level = 6
  7921, 0x000E03,       //index = 98,  last = 0, run = 14, level = 3
  4723, 0x001002,       //index = 103, last = 0, run = 16, level = 2
  5255, 0x001102,       //index = 105, last = 0, run = 17  level = 2
  5942, 0x001E01,       //index = 118, last = 0, run = 30, level = 1
  2399, 0x010403,       //index = 140, last = 1, run = 2,  level = 3

-1 /* end of table */
};

const Ipp32s VC1_HighMotionInterAC[] =
{
 15, /* max bits */
 2,  /* total subtables */
 8,7 ,/* subtable sizes */

 0, /* 1-bit codes */
 0, /* 2-bit codes */
 2, /* 3-bit codes */
        0, 0x000001,            //index = 0
        3, 0x010001,            //index = 99

 4, /* 4-bit codes */
        3, 0x000002,            //index = 1
        4, 0x000101,            //index = 23
        9, 0x010101,            //index = 108
        13, 0x010201,            //index = 113

 7, /* 5-bit codes */
        11, 0x000003,            //index = 2
        30, 0x000102,            //index = 24
         5, 0x000201,            //index = 34
        24, 0x000301,            //index = 42
        17, 0x010301,            //index = 117
        25, 0x010401,            //index = 121
        29, 0x010501,            //index = 124

 8, /* 6-bit codes */
        20, 0x000004,            //index = 3
        63, 0x000005,            //index = 4
        41, 0x000401,            //index = 49
        56, 0x000501,            //index = 54
        42, 0x010002,            //index = 100
        43, 0x010601,            //index = 127
        47, 0x010701,            //index = 130
        62, 0x010901,            //index = 134

 6,/* 7-bit codes */
        93, 0x000006,            //index = 5
        66, 0x000103,            //index = 25
        64, 0x000202,            //index = 35
        88, 0x000601,            //index = 59
        16, 0x010801,            //index = 132
        89, 0x010A01,            //index = 136

 12,/* 8-bit codes */
        162, 0x000007,            //index = 6
        182, 0x000104,            //index = 26
         37, 0x000302,            //index = 43
         35, 0x000701,            //index = 63
        161, 0x000801,            //index = 67
        131, 0x000901,            //index = 70
        228, 0x010003,            //index = 101
         84, 0x010102,            //index = 109
         38, 0x010B01,            //index = 138
         85, 0x010C01,            //index = 140
         87, 0x010D01,            //index = 142
        160,0x010E01,             //index = 144

 13, /* 9-bit codes */
        172, 0x000008,            //index = 7
        366, 0x000009,            //index = 8
        371, 0x000105,            //index = 27
         73, 0x000203,            //index = 36
         79, 0x000402,            //index = 50
        270, 0x000502,            //index = 55
         68, 0x000A01,            //index = 73
        173, 0x010202,            //index = 114
        363, 0x010302,            //index = 118
        368, 0x010F01,            //index = 146
        462, 0x011101,            //index = 148
        360, 0x011401,            //index = 151
        361,IPPVC_ESCAPE,

 19, /* 10-bit codes */
        522, 0x00000A,            //index = 9
        738, 0x00000B,            //index = 10
        917, 0x000106,            //index = 28
        655, 0x000204,            //index = 37
        138, 0x000303,            //index = 44
        543, 0x000602,            //index = 60
        739, 0x000702,            //index = 64
        139, 0x000B01,            //index = 76
        520, 0x000C01,            //index = 79
        921, 0x000D01,            //index = 81
        926, 0x000E01,            //index = 83
        654, 0x010004,            //index = 102
        920, 0x010103,            //index = 110
        539, 0x010402,            //index = 122
        916, 0x010502,            //index = 125
        144, 0x011001,            //index = 147
        538, 0x011201,            //index = 149
        536, 0x011301,            //index = 150
        542, 0x011501,            //index = 152

 19, /* 11-bit codes */
        1074, 0x00000C,            //index = 11
        1481, 0x00000D,            //index = 12
        1838, 0x000107,            //index = 29
        1483, 0x000205,            //index = 38
        1307, 0x000304,            //index = 45
        1042, 0x000403,            //index = 51
        1448, 0x000503,            //index = 56
        1470, 0x000802,            //index = 68
         314, 0x000902,            //index = 71
        1451, 0x000F01,            //index = 85
        1847, 0x001001,            //index = 87
        1075, 0x001401,            //index = 92
        1845, 0x010005,            //index = 103
        1468, 0x010602,            //index = 128
        1846, 0x011701,            //index = 154
         312, 0x011801,            //index = 155
        1305, 0x011901,            //index = 156
        1836, 0x011B01,            //index = 158
        1304, 0x012301,            //index = 166

 19, /* 12-bit codes */
        2087, 0x00000E,            //index = 13
        2900, 0x00000F,            //index = 14
        2964, 0x000108,            //index = 30
        3679, 0x000305,            //index = 46
        3710, 0x000603,            //index = 61
         630, 0x000A02,            //index = 74
        2093, 0x001101,            //index = 89
        3689, 0x001201,            //index = 90
        3688, 0x001301,            //index = 91
        2939, 0x001501,            //index = 93
        2086, 0x010203,            //index = 115
        2943, 0x010303,            //index = 119
         583, 0x010702,            //index = 131
        2613, 0x010802,            //index = 133
        2938, 0x010902,            //index = 135
        3675, 0x010D02,            //index = 143
        580,  0x011601,            //index = 153
        3678, 0x011A01,            //index = 157
        2901, 0x011C01,            //index = 159

 22, /* 13-bit codes */
        1254, 0x000010,           //index = 15
        4191, 0x000011,           //index = 16
        5930, 0x000012,           //index = 17
        5796, 0x000109,           //index = 31
        1162, 0x000206,           //index = 39
        1165, 0x000404,           //index = 52
        4188, 0x000504,           //index = 57
        1253, 0x000703,           //index = 65
        5921, 0x000903,           //index = 72
        1263, 0x000B02,           //index = 77
        7422, 0x000C02,           //index = 80
        7348, 0x000D02,           //index = 82
        4184, 0x010006,           //index = 104
        7418, 0x010007,           //index = 105
        1163, 0x010104,           //index = 111
        5885, 0x010403,           //index = 123
        4190, 0x010A02,           //index = 137
        7349, 0x010C02,           //index = 141
        5224, 0x010E02,           //index = 145
        1164, 0x011F01,           //index = 162
        5923, 0x012001,           //index = 163
        5797, 0x012201,           //index = 165
26, /* 14-bit codes */
        8370,  0x000013,          //index = 18
        11598, 0x000014,          //index = 19
        14832, 0x000015,          //index = 20
         8371, 0x00010A,          //index = 32
        11845, 0x00010B,          //index = 33
         2525, 0x000207,          //index = 40
         2505, 0x000306,          //index = 47
        11841, 0x000405,          //index = 53
        14834, 0x000505,          //index = 58
        14847, 0x000604,          //index = 62
        11840, 0x000704,          //index = 66
         2504, 0x000803,          //index = 69
        14838, 0x000A03,          //index = 75
        14835, 0x000E02,          //index = 84
        11768, 0x001601,          //index = 94
        11862, 0x001701,          //index = 95
        11863, 0x001801,          //index = 96
        14839, 0x001901,          //index = 97
        11769, 0x010008,          //index = 106
        11596, 0x010204,          //index = 116
        10451, 0x010503,          //index = 126
         2511, 0x010B02,          //index = 139
         2524, 0x011D01,          //index = 160
         8379, 0x011E01,          //index = 161
        11844, 0x012101,          //index = 164
        14846, 0x012401,          //index = 167

 12, /* 15-bit codes */
        16757, 0x000016,          //index = 21
        23198, 0x000017,          //index = 22
        29666, 0x000208,          //index = 41
         5020, 0x000307,          //index = 48
        23195, 0x000B03,          //index = 78
        29667, 0x000F02,          //index = 86
        23199, 0x001002,          //index = 88
        20901, 0x001A01,          //index = 98
        16756, 0x010009,          //index = 107
         5021, 0x010105,          //index = 112
        20900, 0x010304,          //index = 120
        23194, 0x010603,          //index = 129

-1 /* end of table */
};

extern const Ipp32s VC1_LowMotionIntraAC[] =
{
 13, /* max bits */
 2,  /* total subtables */
 7,6 ,/* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
        1, 0x000001,       //index = 0

 1, /* 3-bit codes */
        6, 0x000002,       //index = 1

 3, /* 4-bit codes */
        15, 0x000003,      //index = 2
         1, 0x000101,      //index = 16
        10, 0x010001,      //index = 85

 5, /* 5-bit codes */
        22, 0x000004,      //index = 3
        17, 0x000102,      //index = 17
         4, 0x000201,      //index = 27
        23, 0x000301,      //index = 35
        18, 0x010101,      //index = 89

 4, /* 6-bit codes */
        32, 0x000005,      //index = 4
        14, 0x000401,      //index = 42
        59, 0x000501,      //index = 47
        57, 0x010201,      //index = 93

 12,/* 7-bit codes */
        24, 0x000006,      //index = 5
         2, 0x000103,      //index = 18
        20, 0x000202,      //index = 28
        78, 0x000302,      //index = 36
         6, 0x000601,      //index = 51
        27, 0x000701,      //index = 55
        113, 0x000801,     //index = 58
        66, 0x010002,      //index = 86
         0, 0x010301,      //index = 96
        31, 0x010401,      //index = 99
        67, 0x010501,      //index = 101
        22, IPPVC_ESCAPE,

 17,/* 8-bit codes */
          8, 0x000007,     //index = 6
        154, 0x000008,     //index = 7
         11, 0x000104,     //index = 19
        158, 0x000203,     //index = 29
        225, 0x000402,     //index = 43
          7, 0x000901,     //index = 61
         52, 0x000A01,     //index = 64
        224, 0x000B01,     //index = 67
        232, 0x010102,     //index = 90
          3, 0x010601,     //index = 103
          6, 0x010701,     //index = 105
         42, 0x010801,     //index = 107
         15, 0x010901,     //index = 109
         51, 0x010A01,     //index = 111
        152, 0x010B01,     //index = 113
        234, 0x010C01,     //index = 115
         46, 0x010D01,     //index = 117

 12, /* 9-bit codes */
        86, 0x000009,      //index = 8
       318, 0x00000A,      //index = 9
        18, 0x000105,      //index = 20
       470, 0x000106,      //index = 21
        94, 0x000303,      //index = 37
        28, 0x000502,      //index = 48
       122, 0x000602,      //index = 52
        21, 0x000C01,      //index = 70
        20, 0x000D01,      //index = 73
       467, 0x010003,      //index = 87
       310, 0x010E01,      //index = 119
       106, 0x010F01,      //index = 120

 16, /* 10-bit codes */
        240, 0x00000B,    //index = 10
        933, 0x00000C,    //index = 11
        638, 0x000107,    //index = 22
          9, 0x000204,    //index = 30
        243, 0x000304,    //index = 38
        932, 0x000403,    //index = 44
          8, 0x000702,    //index = 56
        215, 0x000802,    //index = 59
        175, 0x000902,    //index = 62
        613, 0x000A02,    //index = 65
        246, 0x000D02,    //index = 74
        203, 0x000E01,    //index = 76
        202, 0x000F01,    //index = 78
        612, 0x010202,    //index = 94
        174, 0x010302,    //index = 97
        943, 0x011101,    //index = 122

 24, /* 11-bit codes */
         119, 0x00000D,    //index = 12
         495, 0x00000E,    //index = 13
         401, 0x000108,    //index = 23
         428, 0x000205,    //index = 31
         482, 0x000206,    //index = 32
         429, 0x000305,    //index = 39
          20, 0x000503,    //index = 49
         400, 0x000603,    //index = 53
        1884, 0x000703,    //index = 57
        1228, 0x000903,    //index = 63
          22, 0x000B02,    //index = 68
         381, 0x000C02,    //index = 71
         484, 0x000D03,    //index = 75
         383, 0x001001,    //index = 80
        1229, 0x001101,    //index = 81
        1278, 0x001301,    //index = 83
        1245, 0x010004,    //index = 88
          76, 0x010103,    //index = 91
        1246, 0x010402,    //index = 100
        1244, 0x010502,    //index = 102
         402, 0x010D02,    //index = 118
          21, 0x011001,    //index = 121
         483, 0x011201,    //index = 123
         116, 0x011301,    //index = 124

 26, /* 12-bit codes */
         154, 0x00000F,   //index = 14
         234, 0x000109,   //index = 24
         988, 0x00010A,   //index = 25
         970, 0x000207,   //index = 33
         236, 0x000306,   //index = 40
         156, 0x000404,   //index = 45
        2494, 0x000504,   //index = 50
        2495, 0x000803,   //index = 60
         159, 0x000A03,   //index = 66
         807, 0x000B03,   //index = 69
        3771, 0x000C03,   //index = 72
        2461, 0x000E02,   //index = 77
         764, 0x000F02,   //index = 79
         765, 0x001201,   //index = 82
        3770, 0x010203,   //index = 95
        2460, 0x010303,   //index = 98
         971, 0x010602,   //index = 104
        2462, 0x010702,   //index = 106
        2558, 0x010902,   //index = 110
        2559, 0x010A02,   //index = 112
        2463, 0x010B02,   //index = 114
         235, 0x011401,   //index = 125
         761, 0x011501,   //index = 126
         237, 0x011701,   //index = 128
         989, 0x011801,   //index = 129
         806, 0x011901,   //index = 130

 12, /* 13-bit codes */
          93, 0x000010,   //index = 15
         315, 0x00010B,   //index = 26
          95, 0x000208,   //index = 34
        1520, 0x000307,   //index = 41
         317, 0x000405,   //index = 46
         311, 0x000604,   //index = 54
         314, 0x001401,   //index = 84
         310, 0x010104,   //index = 92
        1521, 0x010802,   //index = 108
         316, 0x010C02,   //index = 116
          92, 0x011601,   //index = 127
          94, 0x011A01,   //index = 131

-1 /* end of table */
};

const Ipp32s VC1_LowMotionInterAC[] =
{
 15, /* max bits */
 2,  /* total subtables */
 8,7 ,/* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
        3, 0x010001,              //index = 81
 1, /* 3-bit codes */
        4, 0x000001,              //index = 0

 3, /* 4-bit codes */
        11, 0x000101,             //index = 14
         5, 0x010101,             //index = 86
         4, 0x010201,             //index = 90

 5, /* 5-bit codes */
        20, 0x000002,              //index = 1
         7, 0x000201,              //index = 23
         4, 0x010301,              //index = 93
        14, 0x010401,              //index = 96
        12, 0x010501,              //index = 98
 7, /* 6-bit codes */
         2, 0x000301,              //index = 28
        26, 0x000401,              //index = 32
        30, 0x000501,              //index = 36
         4, 0x010601,              //index = 100
         6, 0x010701,              //index = 102
        13, 0x010801,              //index = 104
         7, 0x010901,              //index = 106

 11,/* 7-bit codes */
        23, 0x000003,              //index = 2
        55, 0x000102,              //index = 15
         6, 0x000601,              //index = 40
        25, 0x000701,              //index = 43
        87, 0x000801,              //index = 46
        84, 0x010002,              //index = 82
         1, 0x010A01,              //index = 108
        20, 0x010B01,              //index = 110
        10, 0x010C01,              //index = 112
        22, 0x010D01,              //index = 114
        21, 0x010E01,              //index = 116

 8,/* 8-bit codes */
        127, 0x000004,             //index = 3
        109, 0x000202,             //index = 24
          4, 0x000901,             //index = 49
        125, 0x000A01,             //index = 52
        173, 0x000C01,             //index = 58
         15, 0x010F01,             //index = 118
        126, 0x011001,             //index = 120
        171, 0x011101,             //index = 121

 13, /* 9-bit codes */
        340, 0x000005,             //index = 4
         98, 0x000103,             //index = 16
         97, 0x000302,             //index = 29
          2, 0x000B01,             //index = 55
         28, 0x000D01,             //index = 61
          3, 0x000E01,             //index = 63
        217, 0x000F01,             //index = 65
        248, 0x010102,             //index = 87
         45, 0x011201,             //index = 122
        216, 0x011301,             //index = 123
         11, 0x011401,             //index = 124
         46, 0x011B01,             //index = 131
         13, IPPVC_ESCAPE,

 12, /* 10-bit codes */
        498, 0x000006,             //index = 5
         30, 0x000402,             //index = 33
         31, 0x000502,             //index = 37
        683, 0x010003,             //index = 83
         28, 0x010202,             //index = 91
         20, 0x011501,             //index = 125
        691, 0x011601,             //index = 126
        499, 0x011701,             //index = 127
         58, 0x011801,             //index = 128
          0, 0x011901,             //index = 129
         88, 0x011A01,             //index = 130
         94, 0x011C01,             //index = 132

 16, /* 11-bit codes */
        191, 0x000007,             //index = 6
          7, 0x000104,             //index = 17
          3, 0x000203,             //index = 25
          4, 0x000602,             //index = 41
          6, 0x000702,             //index = 44
        386, 0x000802,             //index = 47
        384, 0x000902,             //index = 50
        397, 0x000B02,             //index = 56
          2, 0x001001,             //index = 67
        387, 0x001101,             //index = 68
         49, 0x001401,             //index = 71
         58, 0x001B01,             //index = 78
        119, 0x010302,             //index = 94
       1378, 0x010502,             //index = 99
       1379, 0x011D01,             //index = 133
        385, 0x012301,             //index = 139

 22, /* 12-bit codes */
        101, 0x000008,             //index = 7
       2730, 0x000009,             //index = 8
        358, 0x000105,             //index = 18
        799, 0x000204,             //index = 26
         85, 0x000303,             //index = 30
       2761, 0x000403,             //index = 34
       2755, 0x000503,             //index = 38
        356, 0x000A02,             //index = 53
         96, 0x000C02,             //index = 59
         87, 0x001201,             //index = 69
         97, 0x001301,             //index = 70
        102, 0x001501,             //index = 72
        797, 0x001901,             //index = 76
        118, 0x001A01,             //index = 77
        357, 0x001C01,             //index = 79
       2729, 0x010103,             //index = 88
         10, 0x010402,             //index = 97
        796, 0x010602,             //index = 101
       2754, 0x010C02,             //index = 113
        236, 0x011E01,             //index = 134
         84, 0x011F01,             //index = 135
       2753, 0x012001,             //index = 136

 25, /* 13-bit codes */
       1584, 0x00000A,            //index = 9
       5527, 0x00000B,            //index = 10
        206, 0x000106,            //index = 19
       5520, 0x000107,            //index = 20
        760, 0x000603,            //index = 42
       1597, 0x000703,            //index = 45
       5505, 0x000B03,            //index = 57
        238, 0x000D02,            //index = 62
        719, 0x000E02,            //index = 64
       5504, 0x000F02,            //index = 66
       1585, 0x001601,            //index = 73
       1586, 0x001701,            //index = 74
        172, 0x001801,            //index = 75
         22, 0x010004,            //index = 84
       5456, 0x010203,            //index = 92
        200, 0x010702,            //index = 103
        474, 0x010802,            //index = 105
        201, 0x010902,            //index = 107
       5526, 0x010B02,            //index = 111
       5462, 0x012101,            //index = 137
        762, 0x012201,            //index = 138
       5463, 0x012401,            //index = 140
       1596, 0x012901,            //index = 145
        207, 0x012A01,            //index = 146
       5524, 0x012B01,            //index = 147

 19, /* 14-bit codes */
        951, 0x00000C,           //index = 11
      11042, 0x00000D,           //index = 12
       1526, 0x000108,           //index = 21
       1522, 0x000205,           //index = 27
        479, 0x000304,           //index = 31
      11043, 0x000404,           //index = 35
      11051, 0x000504,           //index = 39
      10914, 0x000803,           //index = 48
       1436, 0x000903,           //index = 51
       3175, 0x000C03,           //index = 60
       3174, 0x001D01,           //index = 80
       1527, 0x010005,           //index = 85
         46, 0x010A02,           //index = 109
        347, 0x010D02,           //index = 115
        346, 0x010E02,           //index = 117
       1437, 0x012501,           //index = 141
      10915, 0x012601,           //index = 142
      11050, 0x012701,           //index = 143
        478, 0x012801,           //index = 144

 6, /* 15-bit codes */
       3046, 0x00000E,           //index = 13
       3047, 0x000109,           //index = 22
       1901, 0x000A03,           //index = 54
         95, 0x010104,           //index = 89
       1900, 0x010303,           //index = 95
         94, 0x010F02,           //index = 119

-1 /* end of table */
};


const Ipp32s VC1_MidRateIntraAC[] =
{
 12, /* max bits */
 2,  /* total subtables */
 6,6 ,/* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
        2, 0x000001,       //index = 0

 1, /* 3-bit codes */
        6, 0x000002,       //index = 1

 3, /* 4-bit codes */
        15, 0x000003,      //index = 2
        14, 0x000101,      //index = 27
         7, 0x010001,      //index = 67

 3, /* 5-bit codes */
        13, 0x000004,      //index = 3
        12, 0x000005,      //index = 4
        11, 0x000201,      //index = 37

 10, /* 6-bit codes */
        21, 0x000006,      //index = 5
        19, 0x000007,      //index = 6
        18, 0x000008,      //index = 7
        20, 0x000102,      //index = 28
        17, 0x000301,      //index = 42
        16, 0x000401,      //index = 46
        13, 0x000501,      //index = 49
        12, 0x010002,      //index = 68
        15, 0x010101,      //index = 75
        14, 0x010201,      //index = 78

 9,/* 7-bit codes */
        23, 0x000009,      //index = 8
        22, 0x000103,      //index = 29
        21, 0x000202,      //index = 38
        18, 0x000601,      //index = 52
        20, 0x000701,      //index = 55
        17, 0x010301,      //index = 80
        16, 0x010401,      //index = 82
        19, 0x010501,      //index = 84
        3, IPPVC_ESCAPE,

 13,/* 8-bit codes */
        31, 0x00000A,      //index = 9
        30, 0x00000B,      //index = 10
        29, 0x00000C,      //index = 11
        28, 0x000104,      //index = 30
        27, 0x000302,      //index = 43
        25, 0x000801,      //index = 58
        24, 0x000901,      //index = 60
        23, 0x000A01,      //index = 62
        22, 0x010003,      //index = 69
        21, 0x010601,      //index = 86
        20, 0x010701,      //index = 88
        19, 0x010801,      //index = 89
        26, 0x010901,      //index = 90

 21, /* 9-bit codes */
        37, 0x00000D,     //index = 12
        36, 0x00000E,     //index = 13
        35, 0x00000F,     //index = 14
        33, 0x000010,     //index = 15
        32, 0x000105,     //index = 31
        31, 0x000106,     //index = 32
        30, 0x000203,     //index = 39
        29, 0x000303,     //index = 44
        34, 0x000402,     //index = 47
        28, 0x000502,     //index = 50
        27, 0x000602,     //index = 53
        26, 0x000702,     //index = 56
        25, 0x000B01,     //index = 63
        24, 0x000C01,     //index = 64
        23, 0x010004,     //index = 70
        22, 0x010102,     //index = 76
        21, 0x010A01,     //index = 91
        20, 0x010B01,     //index = 92
        19, 0x010C01,     //index = 93
        18, 0x010D01,     //index = 94
        17, 0x010E01,     //index = 95

 14, /* 10-bit codes */
        33, 0x000011,     //index = 16
        32, 0x000012,     //index = 17
        15, 0x000013,     //index = 18
        14, 0x000014,     //index = 19
        13, 0x000107,     //index = 33
        12, 0x000204,     //index = 40
        11, 0x000304,     //index = 45
        10, 0x000403,     //index = 48
         8, 0x000503,     //index = 51
         9, 0x000802,     //index = 59
         7, 0x000D01,     //index = 65
         6, 0x010005,     //index = 71
         5, 0x010103,     //index = 77
         4, 0x010202,     //index = 79

 12, /* 11-bit codes */
         7, 0x000015,      //index = 20
         6, 0x000016,      //index = 21
        32, 0x000017,      //index = 22
        33, 0x000018,      //index = 23
        34, 0x000108,      //index = 34
        35, 0x000902,      //index = 61
         5, 0x010006,      //index = 72
         4, 0x010007,      //index = 73
        36, 0x010302,      //index = 81
        37, 0x010402,      //index = 83
        38, 0x010F01,      //index = 96
        39, 0x011001,      //index = 97

 16, /* 12-bit codes */
        80, 0x000019,      //index = 24
        81, 0x00001A,      //index = 25
        82, 0x00001B,      //index = 26
        83, 0x000109,      //index = 35
        85, 0x00010A,      //index = 36
        86, 0x000205,      //index = 41
        84, 0x000603,      //index = 54
        87, 0x000703,      //index = 57
        88, 0x000E01,      //index = 66
        89, 0x010008,      //index = 74
        90, 0x010502,      //index = 85
        91, 0x010602,      //index = 87
        92, 0x011101,      //index = 98
        93, 0x011201,      //index = 99
        94, 0x011301,      //index = 100
        95, 0x011401,      //index = 101

-1 /* end of table */
};

const Ipp32s VC1_MidRateInterAC[] =
{
 12, /* max bits */
 2,  /* total subtables */
 6, 6 ,/* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
        2, 0x000001,       //index = 0
 1, /* 3-bit codes */
        6, 0x000101,      //index = 12

 3, /* 4-bit codes */
        15, 0x000002,      //index = 1
        14, 0x000201,      //index = 18
         7, 0x010001,      //index = 58

 3, /* 5-bit codes */
        13, 0x000301,     //index = 22
        12, 0x000401,     //index = 25
        11, 0x000501,     //index = 28

 10, /* 6-bit codes */
        21, 0x000003,     //index = 2
        20, 0x000102,     //index = 13
        19, 0x000601,     //index = 31
        18, 0x000701,     //index = 34
        17, 0x000801,     //index = 36
        16, 0x000901,     //index = 38
        15, 0x010101,     //index = 61
        14, 0x010201,     //index = 63
        13, 0x010301,     //index = 64
        12, 0x010401,     //index = 65

 9,/* 7-bit codes */
        23, 0x000004,     //index = 3
        22, 0x000A01,     //index = 40
        21, 0x000B01,     //index = 42
        20, 0x000C01,     //index = 43
        19, 0x010501,     //index = 66
        18, 0x010601,     //index = 67
        17, 0x010701,     //index = 68
        16, 0x010801,     //index = 69
         3, IPPVC_ESCAPE,

 13,/* 8-bit codes */
        31, 0x000005,     //index = 4
        30, 0x000103,     //index = 14
        29, 0x000202,     //index = 19
        28, 0x000D01,     //index = 44
        27, 0x000E01,     //index = 45
        26, 0x010901,     //index = 70
        25, 0x010A01,     //index = 71
        24, 0x010B01,     //index = 72
        23, 0x010C01,     //index = 73
        22, 0x010D01,     //index = 74
        21, 0x010E01,     //index = 75
        20, 0x010F01,     //index = 76
        19, 0x011001,     //index = 77

 21, /* 9-bit codes */
        37, 0x000006,     //index = 5
        36, 0x000007,     //index = 6
        35, 0x000302,     //index = 23
        34, 0x000402,     //index = 26
        33, 0x000F01,     //index = 46
        32, 0x001001,     //index = 47
        31, 0x001101,     //index = 48
        30, 0x001201,     //index = 49
        29, 0x001301,     //index = 50
        28, 0x001401,     //index = 51
        27, 0x001501,     //index = 52
        26, 0x001601,     //index = 53
        25, 0x010002,     //index = 59
        24, 0x011101,     //index = 78
        23, 0x011201,     //index = 79
        22, 0x011301,     //index = 80
        21, 0x011401,     //index = 81
        20, 0x011501,     //index = 82
        19, 0x011601,     //index = 83
        18, 0x011701,     //index = 84
        17, 0x011801,     //index = 85

 14, /* 10-bit codes */
        33, 0x000008,      //index = 7
        32, 0x000009,      //index = 8
        15, 0x000104,      //index = 15
        14, 0x000203,      //index = 20
        13, 0x000303,      //index = 24
        12, 0x000502,      //index = 29
        11, 0x000602,      //index = 32
        10, 0x000702,      //index = 35
         9, 0x000802,      //index = 37
         8, 0x000902,      //index = 39
         7, 0x011901,      //index = 86
         6, 0x011A01,      //index = 87
         5, 0x011B01,      //index = 88
         4, 0x011C01,      //index = 89

 12, /* 11-bit codes */
         7, 0x00000A,       //index = 9
         6, 0x00000B,       //index = 10
        32, 0x00000C,       //index = 11
        33, 0x000105,       //index = 16
        34, 0x001701,       //index = 54
        35, 0x001801,       //index = 55
         5, 0x010003,       //index = 60
         4, 0x010102,       //index = 62
        36, 0x011D01,       //index = 90
        37, 0x011E01,       //index = 91
        38, 0x011F01,       //index = 92
        39, 0x012001,       //index = 93

 16, /* 12-bit codes */
        80, 0x000106,     //index = 17
        81, 0x000204,     //index = 21
        82, 0x000403,     //index = 27
        83, 0x000503,     //index = 30
        84, 0x000603,     //index = 33
        85, 0x000A02,     //index = 41
        86, 0x001901,     //index = 56
        87, 0x001A01,     //index = 57
        88, 0x012101,     //index = 94
        89, 0x012201,     //index = 95
        90, 0x012301,     //index = 96
        91, 0x012401,     //index = 97
        92, 0x012501,     //index = 98
        93, 0x012601,     //index = 99
        94, 0x012701,     //index = 100
        95, 0x012801,     //index = 101

-1 /* end of table */
};


const Ipp32s VC1_HighRateIntraAC[] =
{
 16, /* max bits */
 2,  /* total subtables */
 6,10 ,/* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
        0, 0x000001,       //index = 0

 1, /* 3-bit codes */
        3, 0x000002,       //index = 1

 3, /* 4-bit codes */
        13, 0x000003,      //index = 2
         5, 0x000004,      //index = 3
         9, 0x000101,      //index = 56

 4, /* 5-bit codes */
        28, 0x000005,      //index = 4
        22, 0x000006,      //index = 5
        16, 0x000102,      //index = 57
         8, 0x000201,      //index = 76

 6, /* 6-bit codes */
        63, 0x000007,      //index = 6
        58, 0x000008,      //index = 7
        46, 0x000009,      //index = 8
        34, 0x00000A,      //index = 9
        41, 0x000103,      //index = 58
        40, 0x000301,      //index = 86

 11,/* 7-bit codes */
        123, 0x00000B,     //index = 10
        103, 0x00000C,     //index = 11
         95, 0x00000D,     //index = 12
         71, 0x00000E,     //index = 13
         38, 0x00000F,     //index = 14
         98, 0x000104,     //index = 59
         99, 0x000202,     //index = 77
        101, 0x000401,     //index = 93
         37, 0x000501,     //index = 99
        120, 0x010001,     //index = 126
        122, IPPVC_ESCAPE,

 13,/* 8-bit codes */
        239, 0x000010,    //index = 15
        205, 0x000011,    //index = 16
        193, 0x000012,    //index = 17
        169, 0x000013,    //index = 18
         79, 0x000014,    //index = 19
        243, 0x000105,    //index = 60
        173, 0x000106,    //index = 61
        175, 0x000203,    //index = 78
        195, 0x000302,    //index = 87
        201, 0x000601,    //index = 104
         72, 0x000701,    //index = 108
        250, 0x010101,    //index = 130
        172, 0x010201,    //index = 133

 19, /* 9-bit codes */
        498, 0x000015,    //index = 20
        477, 0x000016,    //index = 21
        409, 0x000017,    //index = 22
        389, 0x000018,    //index = 23
        349, 0x000019,    //index = 24
        283, 0x00001A,    //index = 25
        485, 0x000107,    //index = 62
        377, 0x000108,    //index = 63
        156, 0x000109,    //index = 64
        379, 0x000204,    //index = 79
        337, 0x000303,    //index = 88
        474, 0x000402,    //index = 94
        280, 0x000502,    //index = 100
        384, 0x000801,    //index = 111
        341, 0x010002,    //index = 127
        502, 0x010301,    //index = 136
        476, 0x010401,    //index = 138
        388, 0x010501,    //index = 140
        342, 0x010601,    //index = 142

 23, /* 10-bit codes */
       1007, 0x00001B,     //index = 26
        993, 0x00001C,     //index = 27
        968, 0x00001D,     //index = 28
        817, 0x00001E,     //index = 29
        771, 0x00001F,     //index = 30
        753, 0x000020,     //index = 31
        672, 0x000021,     //index = 32
        563, 0x000022,     //index = 33
        294, 0x000023,     //index = 34
        945, 0x00010A,     //index = 65
        686, 0x00010B,     //index = 66
        295, 0x00010C,     //index = 67
        947, 0x000205,     //index = 80
        673, 0x000304,     //index = 89
        687, 0x000403,     //index = 95
        801, 0x000602,     //index = 105
        994, 0x000901,     //index = 114
        756, 0x000A01,     //index = 117
        999, 0x010701,     //index = 144
        946, 0x010801,     //index = 146
        757, 0x010901,     //index = 148
        802, 0x010A01,     //index = 150
        564, 0x010B01,     //index = 152

 27, /* 11-bit codes */
        1984, 0x000024,    //index = 35
        1903, 0x000025,    //index = 36
        1900, 0x000026,    //index = 37
        1633, 0x000027,    //index = 38
        1540, 0x000028,    //index = 39
        1394, 0x000029,    //index = 40
        1361, 0x00002A,    //index = 41
        1130, 0x00002B,    //index = 42
         628, 0x00002C,    //index = 43
        1902, 0x00010D,    //index = 68
        1392, 0x00010E,    //index = 69
         629, 0x00010F,    //index = 70
        2013, 0x000206,    //index = 81
        1600, 0x000207,    //index = 82
        1395, 0x000305,    //index = 90
         631, 0x000404,    //index = 96
        1606, 0x000503,    //index = 101
        1996, 0x000702,    //index = 109
        1125, 0x000802,    //index = 112
        1985, 0x000B01,    //index = 119
        1505, 0x000C01,    //index = 121
        1362, 0x010003,    //index = 128
        2012, 0x010102,    //index = 131
         585, 0x010202,    //index = 134
        1991, 0x010C01,    //index = 154
        1632, 0x010D01,    //index = 156
         587, 0x010E01,    //index = 158

 28, /* 12-bit codes */
        3879, 0x00002D,    //index = 44
        3876, 0x00002E,    //index = 45
        3803, 0x00002F,    //index = 46
        3214, 0x000030,    //index = 47
        3083, 0x000031,    //index = 48
        3082, 0x000032,    //index = 49
        2787, 0x000033,    //index = 50
        2262, 0x000034,    //index = 51
        1168, 0x000035,    //index = 52
        1173, 0x000036,    //index = 53
        3877, 0x000110,    //index = 71
        3776, 0x000111,    //index = 72
        2720, 0x000112,    //index = 73
        2263, 0x000113,    //index = 74
        3981, 0x000208,    //index = 83
        3009, 0x000209,    //index = 84
        1169, 0x00020A,    //index = 85
        3779, 0x000306,    //index = 91
        2249, 0x000405,    //index = 97
        2726, 0x000504,    //index = 102
        3995, 0x000603,    //index = 106
        2721, 0x000703,    //index = 110
        3777, 0x000902,    //index = 115
        2248, 0x000A02,    //index = 118
        3778, 0x000D01,    //index = 123
        2786, 0x010302,    //index = 137
        1261, 0x010402,    //index = 139
        2727, 0x010F01,    //index = 160

 16, /* 13-bit codes */
        7961, 0x000037,    //index = 54
        7605, 0x000038,    //index = 55
        7756, 0x000114,    //index = 75
        7989, 0x000307,    //index = 92
        6017, 0x000406,    //index = 98
        6016, 0x000505,    //index = 103
        6430, 0x000604,    //index = 107
        6405, 0x000803,    //index = 113
        2344, 0x000B02,    //index = 120
        7988, 0x000E01,    //index = 125
        6431, 0x010004,    //index = 129
        6407, 0x010103,    //index = 132
        6404, 0x010502,    //index = 141
        2521, 0x010602,    //index = 143
        2345, 0x010702,    //index = 145
        7960, 0x011001,    //index = 161

  6, /* 14-bit codes */
        15515, 0x000903,   //index = 116
        12813, 0x000C02,   //index = 122
         5041, 0x010203,   //index = 135
        15208, 0x010802,   //index = 147
         5040, 0x010902,   //index = 149
        15209, 0x010A02,   //index = 151

 3, /* 15-bit codes */
        25624, 0x000D02,   //index = 124
        31029, 0x010B02,   //index = 153
        31028, 0x010D02,   //index = 157

 2, /* 16-bit codes */
        51251, 0x010C02,   //index = 155
        51250, 0x010E02,   //index = 159

-1 /* end of table */
};

const Ipp32s VC1_HighRateInterAC[] =
{
 23, /* max bits */
 3,  /* total subtables */
 8, 8, 7,/* subtable sizes */

 0, /* 1-bit codes */
 1, /* 2-bit codes */
    2, 0x000001,       //index = 0

 2, /* 3-bit codes */
    0, 0x000002,       //index = 1
    3,0x000101,       //index = 32

 1, /* 4-bit codes */
    12, 0x000201,     //index = 45

 5, /* 5-bit codes */
    30, 0x000003,      //index = 2
    4, 0x000004,       //index = 3
    10, 0x000102,     //index = 33
    27, 0x000301,     //index = 53
    7, 0x000401,      //index = 59

 6 , /* 6-bit codes */
    18, 0x000005,      //index = 4
    53, 0x000501,     //index = 64
    12, 0x000601,     //index = 68
    63, 0x010001,    //index = 109
    19, 0x010101,    //index = 113
    11, 0x010201,    //index = 116

 11,/* 7-bit codes */
    112, 0x000006,     //index = 5
    26, 0x000007,      //index = 6
    119, 0x000103,    //index = 34
    125, 0x000202,    //index = 46
    104, 0x000701,    //index = 72
    32, 0x000801,     //index = 75
    117, 0x010301,   //index = 119
    46, 0x010401,    //index = 122
    33, 0x010501,    //index = 124
    20, 0x010601,    //index = 126
    115, IPPVC_ESCAPE,

 10,/* 8-bit codes */
    95, 0x000008,      //index = 7
    71, 0x000009,      //index = 8
    229, 0x000104,    //index = 35
    68, 0x000203,     //index = 47
    226, 0x000302,    //index = 54
    232, 0x000901,    //index = 78
    69, 0x000A01,     //index = 81
    228, 0x010701,   //index = 128
    94, 0x010801,    //index = 130
    55, 0x010901,    //index = 132

 14, /* 9-bit codes */
    467, 0x00000A,    //index = 9
    181, 0x00000B,   //index = 10
    87, 0x00000C,    //index = 11
    473, 0x000105,   //index = 36
    472, 0x000402,   //index = 60
    497, 0x000B01,   //index = 83
    423, 0x000C01,   //index = 85
    86, 0x000D01,    //index = 87
    109, 0x010002,  //index = 110
    475, 0x010A01,  //index = 134
    455, 0x010B01,  //index = 136
    422, 0x010C01,  //index = 138
    180, 0x010D01,  //index = 140
    176, 0x010E01,  //index = 142

 17, /* 10-bit codes */
    949, 0x00000D,    //index = 12
    365, 0x00000E,    //index = 13
    354, 0x00000F,    //index = 14
    997, 0x000106,    //index = 37
    358, 0x000107,    //index = 38
    992, 0x000204,    //index = 48
    933, 0x000303,    //index = 55
    993, 0x000502,    //index = 65
    357, 0x000602,    //index = 69
    909, 0x000E01,    //index = 89
    170, 0x000F01,    //index = 91
    281, 0x010102,   //index = 114
    998, 0x010F01,   //index = 144
    366, 0x011001,   //index = 146
    283, 0x011101,   //index = 148
    217, 0x011201,   //index = 150
    168, 0x011301,    //index = 152

 20, /* 11-bit codes */
    1998, 0x000010,    //index = 15
    1817, 0x000011,    //index = 16
    1681, 0x000012,    //index = 17
    710, 0x000013,     //index = 18
    342, 0x000014,     //index = 19
    1684, 0x000108,    //index = 39
    338, 0x000109,     //index = 40
    1897, 0x000205,    //index = 49
    713, 0x000304,     //index = 56
    728, 0x000403,     //index = 61
    1683, 0x000702,    //index = 73
    735, 0x001001,     //index = 93
    712, 0x001101,     //index = 95
    432, 0x001201,     //index = 97
    565, 0x010202,    //index = 117
    1865, 0x011401,   //index = 154
    1686, 0x011501,   //index = 156
    734, 0x011601,    //index = 158
    561, 0x011701,    //index = 160
    433, 0x011801,    //index = 162

 21, /* 12-bit codes */
    3986, 0x000015,    //index = 20
    3374, 0x000016,    //index = 21
    3360, 0x000017,    //index = 22
    1438, 0x000018,    //index = 23
    1128, 0x000019,    //index = 24
    678, 0x00001A,     //index = 25
    1439, 0x00010A,    //index = 41
    3633, 0x000206,    //index = 50
    1436, 0x000503,    //index = 66
    3984, 0x000802,    //index = 76
    1423, 0x000902,    //index = 79
    3999, 0x001301,    //index = 99
    3792, 0x001401,   //index = 101
    3370, 0x001501,   //index = 103
    1121, 0x001601,   //index = 105
    3728, 0x010003,   //index = 111
    3364, 0x010302,   //index = 120
    3371, 0x011901,   //index = 164
    3375, 0x011A01,   //index = 166
    1458, 0x011B01,   //index = 168
    1129, 0x011C01,   //index = 170

 23, /* 13-bit codes */
    7586, 0x00001B,    //index = 26
    7264, 0x00001C,    //index = 27
    6723, 0x00001D,    //index = 28
    2845, 0x00001E,    //index = 29
    2240, 0x00001F,    //index = 30
    1373, 0x000020,    //index = 31
    7996, 0x00010B,    //index = 42
    6731, 0x00010C,    //index = 43
    1374, 0x00010D,    //index = 44
    7974, 0x000207,    //index = 51
    1372, 0x000208,    //index = 52
    7971, 0x000305,    //index = 57
    7975, 0x000404,    //index = 62
    7459, 0x000603,    //index = 70
    2874, 0x000A02,    //index = 82
    2919, 0x001701,   //index = 107
    1375, 0x001801,   //index = 108
    1358, 0x010004,   //index = 112
    2918, 0x010103,   //index = 115
    7970, 0x010402,   //index = 123
    1359, 0x010502,   //index = 125
    6722, 0x011D01,   //index = 172
    2241, 0x011E01,   //index = 173

 8 , /* 14-bit codes */
    15175, 0x000306,    //index = 58
    13460, 0x000405,    //index = 63
    14531, 0x000504,    //index = 67
    5688, 0x000604,     //index = 71
    14917, 0x000703,    //index = 74
    15174, 0x000B02,    //index = 84
    5750, 0x000C02,     //index = 86
    14916, 0x010602,   //index = 127

 8, /* 15-bit codes */
    31990, 0x000803,    //index = 77
    11503, 0x000903,    //index = 80
    26922, 0x000D02,    //index = 88
    31989, 0x010203,   //index = 118
    31991, 0x010702,   //index = 129
    29061, 0x010802,   //index = 131
    11379, 0x010902,   //index = 133
    26923, 0x010B02,   //index = 137

 4 , /* 16-bit codes */
    58121, 0x000E02,    //index = 90
    63977, 0x010303,   //index = 121
    23005, 0x010A02,   //index = 135
    22757, 0x010C02,   //index = 139

 4 , /* 17-bit codes */
    116241, 0x000F02,    //index = 92
    46009, 0x001002,     //index = 94
    127952, 0x010D02,   //index = 141
    45513, 0x010E02,    //index = 143

 5 , /* 18-bit codes */
    232480, 0x001102,    //index = 96
    91024, 0x001202,     //index = 98
    92017, 0x001302,    //index = 100
    92016, 0x010F02,    //index = 145
    255906, 0x011002,   //index = 147

 2 , /* 19-bit codes */
    464963, 0x001402,    //index = 102
    182051, 0x011302,    //index = 153

 6, /* 20-bit codes */
    1023628, 0x001502,    //index = 104
    1023630, 0x001602,    //index = 106
    1023629, 0x011102,    //index = 149
    1023631, 0x011202,    //index = 151
    929924, 0x011402,     //index = 155
    364101, 0x011502,     //index = 157

 2 , /* 21-bit codes */
    728200, 0x011602,    //index = 159
    1859850, 0x011702,   //index = 161

 3, /* 22-bit codes */
    3719703, 0x011902,    //index = 165
    1456403, 0x011A02,    //index = 167
    1456402, 0x011B02,    //index = 169

 2, /* 23-bit codes */
    7439405, 0x011802,    //index = 163
    7439404, 0x011C02,    //index = 171


-1 /* end of table */
};

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
