/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, adv tables
//
*/

#include "umc_config.h"
#if defined UMC_ENABLE_VC1_VIDEO_DECODER || defined UMC_ENABLE_VC1_SPLITTER || defined UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_common_tables_adv.h"
#include "umc_vc1_common_defs.h"

//VC-1 Table 107: Refdist table
//    REFDIST     VLC         VLC
//                Size
//    0            2           0
//    1            2           1
//    2            2           2
//    3            3           6
//    4            4           14
//    5            5           30
//    6            6           62
//    7            7           126
//    8            8           254
//    9            9           510
//    10           10          1022
//    11           11          2046
//    12           12          4094
//    13           13          8190
//    14           14          16382
//    15           15          32766
//    16           16          65534

extern const Ipp32s VC1_FieldRefdistTable[] =
{

    16, /* max bits */
    3,  /* total subtables */
    5, 6, 5,/* subtable sizes */

    0, /* 1-bit codes */
    3, /* 2-bit codes */
        0, 0,      1, 1,      2, 2,
    1, /* 3-bit codes */
        6,3,
    1, /* 4-bit codes */
        14, 4,
    1, /* 5-bit codes */
        30, 5,
    1, /* 6-bit codes */
        62, 6,
    1, /* 7-bit codes */
        126, 7,
    1, /* 8-bit codes */
        254, 8,
    1, /* 9-bit codes */
        510, 9,
    1, /* 10-bit codes */
        1022, 10,
    1, /* 11-bit codes */
        2046, 11,
    1, /* 12-bit codes */
        4094, 12,
    1, /* 13-bit codes */
        8190, 13,
    1, /* 14-bit codes */
        16382, 14,
    1, /* 15-bit codes */
        32766, 15,
    1, /* 16-bit codes */
        65534, 16,

-1 /* end of table */
};
#endif //UMC_ENABLE_VC1_VIDEO_DECODER
