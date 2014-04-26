/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Interlace picture VV block patternVLC tables
//
*/

#include "umc_config.h"
#if defined UMC_ENABLE_VC1_VIDEO_DECODER || defined UMC_ENABLE_VC1_SPLITTER || defined UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_common_defs.h"
#include "umc_vc1_common_mv_block_pattern_tables.h"

//VC-1 Table 117: 4MV block pattern table 0
// 4MV Coded      VLC        VLC
//    Pattern   Codeword     Codeword Size
//        0        14            5
//        1        58            6
//        2        59            6
//        3        25            5
//        4        12            5
//        5        26            5
//        6        15            5
//        7        15            4
//        8        13            5
//        9        24            5
//       10        27            5
//       11         0            3
//       12        28            5
//       13         1            3
//       14         2            3
//       15         2            2

//VC-1 Table 117: 4MV block pattern table 0
extern const Ipp32s VC1_MV4BlockPatternTable0[] =
{
    6, /* max bits */
    1,  /* total subtables */
    6,/* subtable sizes */

    0, /* 1-bit codes */
     1, /* 2-bit codes */
        2, 15,
    3, /* 3-bit codes */
        0, 11,   1, 13,     2, 14,
    1, /* 4-bit codes */
        15, 7,
    9, /* 5-bit codes */
        14, 0,   25, 3,     12, 4,
        26, 5,   15, 6,     13, 8,
        24, 9,   27, 10,    28, 12,
    2, /* 6-bit codes */
        58, 1,   59, 2,
-1 /* end of table */
};


//VC-1 Table 118: 4MV Block Pattern Table 1
//        4MV  Coded      VLC          VLC
//         Pattern      Codeword    Codeword Size
//            0            8           4
//            1            18          5
//            2            19          5
//            3            4           4
//            4            20          5
//            5            5           4
//            6            30          5
//            7            11          4
//            8            21          5
//            9            31          5
//           10            6           4
//           11            12          4
//           12            7           4
//           13            13          4
//           14            14          4
//           15            0           2

//VC-1 Table 118: 4MV Block Pattern Table 1
extern const Ipp32s VC1_MV4BlockPatternTable1[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    0, /* 1-bit codes */
     1, /* 2-bit codes */
        0, 15,
    0, /* 3-bit codes */
    9, /* 4-bit codes */
        8, 0,   4, 3,     5, 5,    11, 7,
        6, 10,  12, 11,   7, 12,   13, 13,
        14, 14,
    6, /* 5-bit codes */
        18, 1,  19, 2,   20, 4,   30, 6,
        21, 8,  31, 9,
-1 /* end of table */
};

//VC-1 Table 119: 4MV Block Pattern Table 2
//    4MV Coded        VLC             VLC Codeword
//    Pattern          Codeword           Size
//        0            15                   4
//        1            6                    4
//        2            7                    4
//        3            2                    4
//        4            8                    4
//        5            3                    4
//        6            28                   5
//        7            9                    4
//        8            10                   4
//        9            29                   5
//        10           4                    4
//        11           11                   4
//        12           5                    4
//        13           12                   4
//        14           13                   4
//        15           0                    3

//VC-1 Table 119: 4MV Block Pattern Table 2
extern const Ipp32s VC1_MV4BlockPatternTable2[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    0, /* 1-bit codes */
    0, /* 2-bit codes */
    1, /* 3-bit codes */
        0, 15,
    13, /* 4-bit codes */
        15, 0,    6,  1,    7,  2,     2,  3,
        8,  4,    3,  5,    9,  7,     10, 8,
        4,  10,   11, 11,   5,  12,    12, 13,
        13, 14,
    2, /* 5-bit codes */
        28, 6,    29, 9,
-1 /* end of table */
};


//VC-1 Table 120: 4MV Block Pattern Table 3
//       4MV  Coded      VLC          VLC
//        Pattern      Codeword    Codeword Size
//           0            0            2
//           1            11           4
//           2            12           4
//           3            4            4
//           4            13           4
//           5            5            4
//           6            30           5
//           7            16           5
//           8            14           4
//           9            31           5
//           10            6           4
//           11            17          5
//           12            7           4
//           13            18          5
//           14            19          5
//           15            10          4

//VC-1 Table 120: 4MV Block Pattern Table 3
extern const Ipp32s VC1_MV4BlockPatternTable3[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    0, /* 1-bit codes */
     1, /* 2-bit codes */
        0,  0,
    0, /* 3-bit codes */
    9, /* 4-bit codes */
        11, 1,   12, 2,    4,  3,     13, 4,
        5,  5,   14, 8,    6, 10,     7, 12,
        10,15,
    6, /* 5-bit codes */\
        30, 6,   16, 7,    31, 9,     17, 11,
        18, 13,  19, 14,
-1 /* end of table */
};


//VC-1 Table 121: Interlace Frame 2 MVP Block Pattern Table 0
//    Top Bottom        VLC Codeword    VLC Size
//     0     0            2                2
//     0     1            1                2
//     1     0            0                2
//     1     1            3                2

//VC-1 Table 121: Interlace Frame 2 MVP Block Pattern Table 0
extern const Ipp32s VC1_MV2BlockPatternTable0[] =
{
    2, /* max bits */
    1,  /* total subtables */
    2,/* subtable sizes */

    0, /* 1-bit codes */
     4, /* 2-bit codes */
        2, 0,    1, 1,    0, 2,     3, 3,
-1 /* end of table */
};

//VC-1 Table 122: Interlace Frame 2 MVP Block Pattern Table 1
//    Top Bottom        VLC Codeword    VLC Size
//     0     0            1                1
//     0     1            0                2
//     1     0            2                3
//     1     1            3                3

//VC-1 Table 122: Interlace Frame 2 MVP Block Pattern Table 1
extern const Ipp32s VC1_MV2BlockPatternTable1[] =
{
    3, /* max bits */
    1,  /* total subtables */
    3,/* subtable sizes */

    1, /* 1-bit codes */
        1, 0,
     1, /* 2-bit codes */
        0, 1,
    2, /* 3-bit codes */
        2, 2,     3, 3,
-1 /* end of table */
};

//VC-1 Table 123: Interlace Frame 2 MVP Block Pattern Table 2
//    Top Bottom        VLC Codeword    VLC Size
//     0     0            2                3
//     0     1            0                2
//     1     0            3                3
//     1     1            1                1

//VC-1 Table 123: Interlace Frame 2 MVP Block Pattern Table 2
extern const Ipp32s VC1_MV2BlockPatternTable2[] =
{
    3, /* max bits */
    1,  /* total subtables */
    3,/* subtable sizes */

    1, /* 1-bit codes */
        1, 3,
     1, /* 2-bit codes */
        0, 1,
    2, /* 3-bit codes */
        2, 0,    3, 2,
-1 /* end of table */
};

//VC-1 Table 124: Interlace Frame 2 MVP Block Pattern Table 3
//    Top Bottom        VLC Codeword    VLC Size
//     0     0            1                1
//     0     1            3                3
//     1     0            2                3
//     1     1            0                2

//VC-1 Table 124: Interlace Frame 2 MVP Block Pattern Table 3
extern const Ipp32s VC1_MV2BlockPatternTable3[] =
{
    3, /* max bits */
    1,  /* total subtables */
    3,/* subtable sizes */

    1, /* 1-bit codes */
        1, 0,
     1, /* 2-bit codes */
        0, 3,
    2, /* 3-bit codes */
        3, 1,      2, 2,
-1 /* end of table */
};

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
