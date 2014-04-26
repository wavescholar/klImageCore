/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, Interlace pictures MB Mode tables
//
*/

#include "umc_config.h"
#if defined UMC_ENABLE_VC1_VIDEO_DECODER || defined UMC_ENABLE_VC1_SPLITTER || defined UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_common_defs.h"
#include "umc_vc1_common_interlace_mb_mode_tables.h"

//VC-1 Table 145: mixed MV MB Mode Table 0
//    MB            VLC            VLC
//    Mode        Codeword    Size
//    0            16            6
//    1            17            6
//    2            3            2
//    3            3            3
//    4            0            2
//    5            5            4
//    6            9            5
//    7            2            2

extern const Ipp32s VC1_Mixed_MV_MB_ModeTable0[] =
{
    6, /* max bits */
    1,  /* total subtables */
    6,/* subtable sizes */

    0, /* 1-bit codes */
     3, /* 2-bit codes */
        3, 2,      0, 4,      2, 7,
    1, /* 3-bit codes */
        3, 3,
    1, /* 4-bit codes */
        5, 5,
    1, /* 5-bit codes */
        9, 6,
    2, /* 6-bit codes */
        16, 0,     17, 1,
-1 /* end of table */
};


//VC-1 Table 146: mixed MV MB Mode Table 1
//    MB            VLC        VLC
//    Mode        Codeword    Size
//    0            8            5
//    1            9            5
//    2            3            3
//    3            6            3
//    4            7            3
//    5            0            2
//    6            5            4
//    7            2            2

extern const Ipp32s VC1_Mixed_MV_MB_ModeTable1[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    0, /* 1-bit codes */
    2, /* 2-bit codes */
        0, 5,   2, 7,

    3, /* 3-bit codes */
        3, 2,   6, 3,    7, 4,

    1, /* 4-bit codes */
        5, 6,

    2, /* 5-bit codes */
        8, 0,      9, 1,

    -1 /* end of table */
};

//VC-1 Table 147: mixed MV MB Mode Table 2
//    MB            VLC        VLC
//    Mode        Codeword    Size
//    0            16            6
//    1            17            6
//    2            5            4
//    3            3            3
//    4            0            2
//    5            3            2
//    6            9            5
//    7            2            2

extern const Ipp32s VC1_Mixed_MV_MB_ModeTable2[] =
{
    6, /* max bits */
    1,  /* total subtables */
    6,/* subtable sizes */

    0, /* 1-bit codes */
     3, /* 2-bit codes */
        0, 4,    3, 5,     2, 7,

    1, /* 3-bit codes */
        3, 3,

    1, /* 4-bit codes */
        5, 2,

    1, /* 5-bit codes */
        9, 6,

    2, /* 6-bit codes */
        16, 0,    17, 1,

    -1 /* end of table */
};

//VC-1 Table 148: mixed MV MB Mode Table 3
//    MB            VLC        VLC
//    Mode        Codeword    Size
//    0            56          6
//    1            57          6
//    2            15          4
//    3            4           3
//    4            5           3
//    5            6           3
//    6            29          5
//    7            0           1

extern const Ipp32s VC1_Mixed_MV_MB_ModeTable3[] =
{
    6, /* max bits */
    1,  /* total subtables */
    6,/* subtable sizes */

    1, /* 1-bit codes */
        0, 7,
    0, /* 2-bit codes */
    3, /* 3-bit codes */
        4, 3,     5, 4,      6, 5,

    1, /* 4-bit codes */
        15, 2,

    1, /* 5-bit codes */
        29, 6,

    2, /* 6-bit codes */
        56, 0,     57, 1,

    -1 /* end of table */
};

//VC-1 Table 149: mixed MV MB Mode Table 4
//    MB            VLC       VLC
//    Mode        Codeword    Size
//    0            52           6
//    1            53           6
//    2            27           5
//    3            14           4
//    4            15           4
//    5            2            2
//    6            12           4
//    7            0            1

extern const Ipp32s VC1_Mixed_MV_MB_ModeTable4[] =
{
    6, /* max bits */
    1,  /* total subtables */
    6,/* subtable sizes */

    1, /* 1-bit codes */
        0, 7,
     1, /* 2-bit codes */
        2, 5,
    0, /* 3-bit codes */
    3, /* 4-bit codes */
        14, 3,     15, 4,      12, 6,
    1, /* 5-bit codes */
        27, 2,
    2, /* 6-bit codes */
        52, 0,     53, 1,
    -1 /* end of table */
};

//VC-1 Table 150: mixed MV MB Mode Table 5
//    MB      VLC        VLC
//    Mode   Codeword    Size
//    0       56            6
//    1       57            6
//    2       29            5
//    3        5            3
//    4        6            3
//    5        0            1
//    6       15            4
//    7        4            3

extern const Ipp32s VC1_Mixed_MV_MB_ModeTable5[] =
{
    6, /* max bits */
    1,  /* total subtables */
    6,/* subtable sizes */

    1, /* 1-bit codes */
        0, 5,
     0, /* 2-bit codes */
    3, /* 3-bit codes */
        5, 3,    6, 4,     4, 7,
    1, /* 4-bit codes */
        15, 6,
    1, /* 5-bit codes */
        29, 2,
    2, /* 6-bit codes */
        56, 0,   57, 1,
    -1 /* end of table */
};

//VC-1 Table 151: mixed MV MB Mode Table 6
//    MB            VLC        VLC
//    Mode        Codeword    Size
//    0            16           5
//    1            17           5
//    2            6            3
//    3            7            3
//    4            0            2
//    5            1            2
//    6            9            4
//    7            5            3

extern const Ipp32s VC1_Mixed_MV_MB_ModeTable6[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    0, /* 1-bit codes */
    2, /* 2-bit codes */
        0, 4,    1, 5,
    3, /* 3-bit codes */
        6, 2,     7, 3,      5, 7,
    1, /* 4-bit codes */
        9, 6,
    2, /* 5-bit codes */
        16, 0,    17, 1,
    -1 /* end of table */
};

//VC-1 Table 152: mixed MV MB Mode Table 7
//    MB            VLC       VLC
//    Mode        Codeword    Size
//    0            56          6
//    1            57          6
//    2            0           1
//    3            5           3
//    4            6           3
//    5            29          5
//    6            4           3
//    7            15          4

extern const Ipp32s VC1_Mixed_MV_MB_ModeTable7[] =
{
    6, /* max bits */
    1,  /* total subtables */
    6,/* subtable sizes */

    1, /* 1-bit codes */
        0, 2,
     0, /* 2-bit codes */
    3, /* 3-bit codes */
        5, 3,     6, 4,      4, 6,
    1, /* 4-bit codes */
        15, 7,
    1, /* 5-bit codes */
        29, 5,
    2, /* 6-bit codes */
        56, 0,      57, 1,
    -1 /* end of table */
};

//VC-1 Table 153: 1-MV MB Mode Table 0
//    MB        VLC        VLC
//    Mode    Codeword    Size
//    0        0            5
//    1        1            5
//    2        1            1
//    3        1            3
//    4        1            2
//    5        1            4
extern const Ipp32s VC1_1MV_MB_ModeTable0[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    1, /* 1-bit codes */
        1, 2,
     1, /* 2-bit codes */
        1, 4,
    1, /* 3-bit codes */
        1, 3,
    1, /* 4-bit codes */
        1, 5,
    2, /* 5-bit codes */
        0, 0,    1, 1,

    -1 /* end of table */
};

//VC-1 Table 154: 1-MV MB Mode Table 1
//    MB        VLC       VLC
//    Mode    Codeword    Size
//    0        0            5
//    1        1            5
//    2        1            1
//    3        1            2
//    4        1            3
//    5        1            4

extern const Ipp32s VC1_1MV_MB_ModeTable1[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    1, /* 1-bit codes */
        1, 2,
    1, /* 2-bit codes */
        1, 3,
    1, /* 3-bit codes */
        1, 4,
    1, /* 4-bit codes */
        1, 5,
    2, /* 5-bit codes */
        0, 0,    1, 1,
    -1 /* end of table */
};

//VC-1 Table 155: 1-MV MB Mode Table 2
//    MB        VLC        VLC
//    Mode    Codeword    Size
//    0        16           5
//    1        17           5
//    2        3            2
//    3        0            1
//    4        9            4
//    5        5            3

extern const Ipp32s VC1_1MV_MB_ModeTable2[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    1, /* 1-bit codes */
        0, 3,
    1, /* 2-bit codes */
        3, 2,
    1, /* 3-bit codes */
        5, 5,
    1, /* 4-bit codes */
        9, 4,
    2, /* 5-bit codes */
        16, 0,   17, 1,

    -1 /* end of table */
};

//VC-1 Table 156: 1-MV MB Mode Table 3
//    MB        VLC       VLC
//    Mode    Codeword    Size
//    0        20           5
//    1        21           5
//    2        3            2
//    3        11           4
//    4        0            1
//    5        4            3

extern const Ipp32s VC1_1MV_MB_ModeTable3[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    1, /* 1-bit codes */
        0, 4,
    1, /* 2-bit codes */
        3, 2,
    1, /* 3-bit codes */
        4, 5,
    1, /* 4-bit codes */
        11, 3,
    2, /* 5-bit codes */
        21, 1,     20, 0,
    -1 /* end of table */
};

//VC-1 Table 157: 1-MV MB Mode Table 4
//    MB        VLC        VLC
//    Mode    Codeword    Size
//    0        4            4
//    1        5            4
//    2        2            2
//    3        3            3
//    4        3            2
//    5        0            2

extern const Ipp32s VC1_1MV_MB_ModeTable4[] =
{
    4, /* max bits */
    1,  /* total subtables */
    4,/* subtable sizes */

    0, /* 1-bit codes */
    3, /* 2-bit codes */
        2, 2,    3, 4,     0, 5,
    1, /* 3-bit codes */
        3, 3,
    2, /* 4-bit codes */
        4, 0,     5, 1,

    -1 /* end of table */
};

//VC-1 Table 158: 1-MV MB Mode Table 5
//    MB        VLC        VLC
//    Mode    Codeword    Size
//    0        4            4
//    1        5            4
//    2        3            3
//    3        2            2
//    4        0            2
//    5        3            2

extern const Ipp32s VC1_1MV_MB_ModeTable5[] =
{
    4, /* max bits */
    1,  /* total subtables */
    4,/* subtable sizes */

    0, /* 1-bit codes */
    3, /* 2-bit codes */
        2, 3,    0, 4,     3, 5,
    1, /* 3-bit codes */
        3, 2,
    2, /* 4-bit codes */
        4, 0,    5, 1,
    -1 /* end of table */
};

//VC-1 Table 159: 1-MV MB Mode Table 6
//    MB        VLC        VLC
//    Mode    Codeword    Size
//    0        0            5
//    1        1            5
//    2        1            3
//    3        1            4
//    4        1            1
//    5        1            2
extern const Ipp32s VC1_1MV_MB_ModeTable6[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    1, /* 1-bit codes */
        1, 4,
    1, /* 2-bit codes */
        1, 5,
    1, /* 3-bit codes */
        1, 2,
    1, /* 4-bit codes */
        1, 3,
    2, /* 5-bit codes */
        0, 0,    1, 1,

    -1 /* end of table */
};

//VC-1 Table 160: 1-MV MB Mode Table 7
//    MB        VLC        VLC
//    Mode    Codeword    Size
//    0        16           5
//    1        17           5
//    2        9            4
//    3        5            3
//    4        3            2
//    5        0            1
extern const Ipp32s VC1_1MV_MB_ModeTable7[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    1, /* 1-bit codes */
        0, 5,
    1, /* 2-bit codes */
        3, 4,
    1, /* 3-bit codes */
        5, 3,
    1, /* 4-bit codes */
        9, 2,
    2, /* 5-bit codes */
        16, 0,   17, 1,

    -1 /* end of table */
};

//Table 161: Interlace Frame 4-MV MB Mode Table 0
//MB Type   MV Present   Field/Frame Transform  VLC Codeword  VLC Size   Index
//  1-MV        1           Frame                   22            5       0
//  1-MV        1           Field                   17            5       1
//  1-MV        1           No CBP                  0             2       2
//  1-MV        0           Frame                   47            6       3
//  1-MV        0           Field                   32            6       4
//  2-MV(F)     N/A         Frame                   10            4       5
//  2-MV(F)     N/A         Field                   1             2       6
//  2-MV(F)     N/A         No CBP                  3             2       7
//  4-MV        N/A         Frame                   67            7       8
//  4-MV        N/A         Field                   133           8       9
//  4-MV        N/A         No CBP                  132           8       10
//  4-MV(F)     N/A         Frame                   92            7       11
//  4-MV(F)     N/A         Field                   19            5       12
//  4-MV(F)     N/A         No CBP                  93            7       13
//  INTRA       N/A         N/A                     18            5       14
extern const Ipp32s VC1_4MV_MB_Mode_PBPic_Table0[] =
{
    8, /* max bits */
    1,  /* total subtables */
    8,/* subtable sizes */

    0, /* 1-bit codes */
    3, /* 2-bit codes */
        0, 2,      1, 6,    3, 7,
    0, /* 3-bit codes */
    1, /* 4-bit codes */
        10, 5,
    4, /* 5-bit codes */
        22, 0,      17, 1,       19, 12,       18, 14,
    2, /* 6-bit codes */
        47, 3,      32, 4,
    3, /* 7bit codes */
        67, 8,      92, 11,      93, 13,
    2, /* 8-bit codes */
        133, 9,     132, 10,
    -1 /* end of table */
};

//Table 162: Interlace Frame 4-MV MB Mode Table 1
//MB Type   MV Present   Field/Frame Transform  VLC Codeword  VLC Size  Index
//  1-MV        1           Frame                   3             3       15
//  1-MV        1           Field                   45            6       16
//  1-MV        1           No CBP                  0             3       17
//  1-MV        0           Frame                   7             3       18
//  1-MV        0           Field                   23            5       19
//  2-MV(F)     N/A         Frame                   6             3       20
//  2-MV(F)     N/A         Field                   1             3       21
//  2-MV(F)     N/A         No CBP                  2             3       22
//  4-MV        N/A         Frame                   10            4       23
//  4-MV        N/A         Field                   39            6       24
//  4-MV        N/A         No CBP                  44            6       25
//  4-MV(F)     N/A         Frame                   8             4       26
//  4-MV(F)     N/A         Field                   18            5       27
//  4-MV(F)     N/A         No CBP                  77            7       28
//  INTRA       N/A         N/A                     76            7       29

extern const Ipp32s VC1_4MV_MB_Mode_PBPic_Table1[] =
{
    7, /* max bits */
    1,  /* total subtables */
    7,/* subtable sizes */

    0, /* 1-bit codes */
    0, /* 2-bit codes */
    6, /* 3-bit codes */
        3, 15,     0, 17,      7, 18,     6, 20,
        1, 21,     2, 22,
    2, /* 4-bit codes */
        10, 23,    8, 26,
    2, /* 5-bit codes */
        23, 19,    18, 27,
    3, /* 6-bit codes */
        45, 16,    39,  24,    44,  25,
    2, /* 7bit codes */
        77,  28,   76,  29,

    -1 /* end of table */
};

//Table 163: Interlace Frame 4-MV MB Mode Table 2
//MB Type   MV Present   Field/Frame Transform  VLC Codeword VLC Size Index
//  1-MV        1           Frame                  15             4   30
//  1-MV        1           Field                  6              3   31
//  1-MV        1           No CBP                 28             5   32
//  1-MV        0           Frame                  9              5   33
//  1-MV        0           Field                  41             7   34
//  2-MV(F)     N/A         Frame                  6              4   35
//  2-MV(F)     N/A         Field                  2              2   36
//  2-MV(F)     N/A         No CBP                 15             5   37
//  4-MV        N/A         Frame                  14             5   38
//  4-MV        N/A         Field                  8              5   39
//  4-MV        N/A         No CBP                 40             7   40
//  4-MV(F)     N/A         Frame                  29             5   41
//  4-MV(F)     N/A         Field                  0              2   42
//  4-MV(F)     N/A         No CBP                 21             6   43
//  INTRA       N/A         N/A                    11             5   44

extern const Ipp32s VC1_4MV_MB_Mode_PBPic_Table2[] =
{
    7, /* max bits */
    1,  /* total subtables */
    7,/* subtable sizes */

    0, /* 1-bit codes */
    2, /* 2-bit codes */
        2, 36,    0, 42,
    1, /* 3-bit codes */
        6, 31,
    2, /* 4-bit codes */
        15, 30,   6, 35,
    7, /* 5-bit codes */
        28,32,     9, 33,     15, 37,    14,  38,
        8, 39,     29, 41,    11, 44,
    1, /* 6-bit codes */
        21, 43,
    2, /* 7bit codes */
        41, 34,    40, 40,
    -1 /* end of table */
};

//Table 164: Interlace Frame 4-MV MB Mode Table 3
//MB Type   MV Present  Field/Frame Transform  VLC Codeword  VLC Size Index
//  1-MV        1          Frame                    7           4      45
//  1-MV        1          Field                    198         9      46
//  1-MV        1          No CBP                   1           1      47
//  1-MV        0          Frame                    2           3      48
//  1-MV        0          Field                    193         9      49
//  2-MV(F)     N/A        Frame                    13          5      50
//  2-MV(F)     N/A        Field                    25          6      51
//  2-MV(F)     N/A        No CBP                   0           2      52
//  4-MV        N/A        Frame                    97          8      53
//  4-MV        N/A        Field                    1599        12     54
//  4-MV        N/A        No CBP                   98          8      55
//  4-MV(F)     N/A        Frame                    398         10     56
//  4-MV(F)     N/A        Field                    798         11     57
//  4-MV(F)     N/A        No CBP                   192         9      58
//  INTRA       N/A        N/A                      1598        12     59

extern const Ipp32s VC1_4MV_MB_Mode_PBPic_Table3[] =
{
    12, /* max bits */
    2,  /* total subtables */
    6, 6,/* subtable sizes */

    1, /* 1-bit codes */
        1, 47,
    1, /* 2-bit codes */
        0, 52,
    1, /* 3-bit codes */
        2, 48,
    1, /* 4-bit codes */
        7, 45,
    1, /* 5-bit codes */
        13, 50,
    1, /* 6-bit codes */
        25, 51,
    0, /* 7bit codes */
    2, /* 8bit codes */
        97,  53,      98,  55,
    3, /* 9bit codes */
        198,  46,     193,  49,     192,  58,
    1, /* 10bit codes */
        398,  56,
    1, /* 11bit codes */
        798,  57,
    2, /* 12bit codes */
        1599, 54,      1598, 59,

    -1 /* end of table */
};

//Table 165: Interlace Frame Non 4-MV MB Mode Table 0
//MB Type   MV Present   Field/Frame Transform VLC Codeword   VLC Size Index
// 1-MV         1           Frame                  9            4       60
// 1-MV         1           Field                  22           5       61
// 1-MV         1           No CBP                 0            2       62
// 1-MV         0           Frame                  17           5       63
// 1-MV         0           Field                  16           5       64
// 2-MV(F)      N/A         Frame                  10           4       65
// 2-MV(F)      N/A         Field                  1            2       66
// 2-MV(F)      N/A         No CBP                 3            2       67
// INTRA        N/A         N/A                    23           5       68
extern const Ipp32s VC1_Non4MV_MB_Mode_PBPic_Table0[] =
{
    5, /* max bits */
    1,  /* total subtables */
    5,/* subtable sizes */

    0, /* 1-bit codes */
    3, /* 2-bit codes */
        0, 62,     1, 66,     3, 67,
    0, /* 3-bit codes */
    2, /* 4-bit codes */
        9, 60,    10, 65,
    4, /* 5-bit codes */
        22, 61,   17, 63,     16, 64,     23, 68,

    -1 /* end of table */
};


//Table 166: Interlace Frame Non 4-MV MB Mode Table 1
//MB Type   MV Present   Field/Frame Transform  VLC Codeword  VLC Size Index
// 1-MV         1           Frame                   7           3       69
// 1-MV         1           Field                   0           4       70
// 1-MV         1           No CBP                  5           6       71
// 1-MV         0           Frame                   2           2       72
// 1-MV         0           Field                   1           3       73
// 2-MV(F)      N/A         Frame                   1           2       74
// 2-MV(F)      N/A         Field                   6           3       75
// 2-MV(F)      N/A         No CBP                  3           5       76
// INTRA        N/A         N/A                     4           6       77
extern const Ipp32s VC1_Non4MV_MB_Mode_PBPic_Table1[] =
{
    6, /* max bits */
    1,  /* total subtables */
    6,/* subtable sizes */

    0, /* 1-bit codes */
    2, /* 2-bit codes */
        2, 72,     1, 74,

    3, /* 3-bit codes */
        7, 69,     1, 73,     6, 75,
    1, /* 4-bit codes */
        0,  70,
    1, /* 5-bit codes */
        3, 76,
    2, /* 6-bit codes */
        5, 71,     4, 77,

    -1 /* end of table */
};


//Table 167: Interlace Frame Non 4-MV MB Mode Table 2
//MB Type   MV Present   Field/Frame Transform VLC Codeword VLC Size Index
// 1-MV         1           Frame                   1           2      78
// 1-MV         1           Field                   0           2      79
// 1-MV         1           No CBP                  10          4      80
// 1-MV         0           Frame                   23          5      81
// 1-MV         0           Field                   44          6      82
// 2-MV(F)      N/A         Frame                   8           4      83
// 2-MV(F)      N/A         Field                   3           2      84
// 2-MV(F)      N/A         No CBP                  9           4      85
// INTRA        N/A         N/A                     45          6      86
extern const Ipp32s VC1_Non4MV_MB_Mode_PBPic_Table2[] =
{
    6, /* max bits */
    1,  /* total subtables */
    6,/* subtable sizes */

    0, /* 1-bit codes */
    3, /* 2-bit codes */
        1, 78,       0, 79,       3, 84,
    0, /* 3-bit codes */
    3, /* 4-bit codes */
        10, 80,      8, 83,       9, 85,
    1, /* 5-bit codes */
        23, 81,

    2, /* 6-bit codes */
        44, 82,      45, 86,

    -1 /* end of table */
};

//Table 168: Interlace Frame Non 4-MV MB Mode Table 3
//MB Type   MV Present   Field/Frame Transform  VLC Codeword VLC Size Index
// 1-MV         1               Frame               7            4     87
// 1-MV         1               Field               97           8     88
// 1-MV         1               No CBP              1            1     89
// 1-MV         0               Frame               2            3     90
// 1-MV         0               Field               49           7     91
// 2-MV(F)      N/A             Frame               13           5     92
// 2-MV(F)      N/A             Field               25           6     93
// 2-MV(F)      N/A             No CBP              0            2     94
// INTRA        N/A             N/A                 96           8     95
extern const Ipp32s VC1_Non4MV_MB_Mode_PBPic_Table3[] =
{
    8, /* max bits */
    1,  /* total subtables */
    8,/* subtable sizes */

    1, /* 1-bit codes */
        1, 89,
    1, /* 2-bit codes */
        0, 94,
    1, /* 3-bit codes */
        2, 90,
    1, /* 4-bit codes */
        7, 87,
    1, /* 5-bit codes */
        13, 92,
    1, /* 6-bit codes */
        25, 93,
    1, /* 7-bit codes */
        49,  91,
    2, /* 8-bit codes */
        97, 88,     96, 95,

    -1 /* end of table */
};

extern const Ipp8u VC1_MB_Mode_PBPic_Transform_Table[] =
{
    // 0..9
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    VC1_NO_CBP_TRANSFORM,   VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    // 10..19
    VC1_NO_CBP_TRANSFORM,   VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    VC1_NA_TRANSFORM,       VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    // 20.. 29
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    VC1_NO_CBP_TRANSFORM,   VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    VC1_NO_CBP_TRANSFORM,   VC1_NA_TRANSFORM,
    //30..39
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    VC1_NO_CBP_TRANSFORM,   VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    //40..49
    VC1_NO_CBP_TRANSFORM,   VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    VC1_NA_TRANSFORM,       VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    //50..59
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    VC1_NO_CBP_TRANSFORM,   VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    VC1_NO_CBP_TRANSFORM,   VC1_NA_TRANSFORM,
    //60..69
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    VC1_NO_CBP_TRANSFORM,   VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    VC1_NA_TRANSFORM,       VC1_FRAME_TRANSFORM,
    //70..79
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    VC1_NO_CBP_TRANSFORM,   VC1_NA_TRANSFORM,
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    //80..89
    VC1_NO_CBP_TRANSFORM,   VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    VC1_NA_TRANSFORM,       VC1_FRAME_TRANSFORM,
    VC1_FIELD_TRANSFORM,    VC1_NO_CBP_TRANSFORM,
    //90..95
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    VC1_FRAME_TRANSFORM,    VC1_FIELD_TRANSFORM,
    VC1_NO_CBP_TRANSFORM,   VC1_NA_TRANSFORM
};

extern const Ipp8u VC1_MB_Mode_PBPic_MBtype_Table[] =
{
    // 0..9
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_2MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_4MV_INTER,                   VC1_MB_4MV_INTER,
    //10..19
    VC1_MB_4MV_INTER,                   VC1_MB_4MV_FIELD_INTER,
    VC1_MB_4MV_FIELD_INTER,             VC1_MB_4MV_FIELD_INTER,
    VC1_MB_INTRA,                       VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    //20..29
    VC1_MB_2MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_2MV_INTER,                   VC1_MB_4MV_INTER,
    VC1_MB_4MV_INTER,                   VC1_MB_4MV_INTER,
    VC1_MB_4MV_FIELD_INTER,             VC1_MB_4MV_FIELD_INTER,
    VC1_MB_4MV_FIELD_INTER,             VC1_MB_INTRA,
    //30..39
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_2MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_4MV_INTER,                   VC1_MB_4MV_INTER,
    //40..49
    VC1_MB_4MV_INTER,                   VC1_MB_4MV_FIELD_INTER,
    VC1_MB_4MV_FIELD_INTER,             VC1_MB_4MV_FIELD_INTER,
    VC1_MB_INTRA,                       VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    //50..59
    VC1_MB_2MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_2MV_INTER,                   VC1_MB_4MV_INTER,
    VC1_MB_4MV_INTER,                   VC1_MB_4MV_INTER,
    VC1_MB_4MV_FIELD_INTER,             VC1_MB_4MV_FIELD_INTER,
    VC1_MB_4MV_FIELD_INTER,             VC1_MB_INTRA,
    //60..69
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_2MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_INTRA,                       VC1_MB_1MV_INTER,
    //70..79
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_2MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_2MV_INTER,                   VC1_MB_INTRA,
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    //80..89
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_2MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_INTRA,                       VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    //90..96
    VC1_MB_1MV_INTER,                   VC1_MB_1MV_INTER,
    VC1_MB_2MV_INTER,                   VC1_MB_2MV_INTER,
    VC1_MB_2MV_INTER,                   VC1_MB_INTRA
};

extern const Ipp8s VC1_MB_Mode_PBPic_MVPresent_Table[] =
{
    // 0..9
    1, 1, 1, 0, 0, -1, -1, -1, -1, -1,
    //10..19
    -1, -1, -1, -1, -1, 1, 1, 1, 0, 0,
    //20..29
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    //30..39
    1, 1, 1, 0, 0, -1, -1, -1, -1, -1,
    //40..49
    -1, -1, -1, -1, -1, 1, 1, 1, 0, 0,
    //50..59
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    //60..69
    1, 1, 1, 0, 0, -1, -1, -1, -1, 1,
    //70..79
    1, 1, 0, 0, -1, -1, -1, -1, 1, 1,
    //80..89
    1, 0, 0, -1, -1, -1, -1, 1, 1, 1,
    //90..95
    0, 0, -1, -1, -1, -1

};


extern const Ipp8u VC1_MB_Mode_PBPic_FIELDTX_Table[] =
{
    // 0..9
    0, 1, 0, 0, 1, 0, 1, 1, 0, 1,
    //10..19
    0, 0, 1, 1, 0, 0, 1, 0, 0, 1,
    //20..29
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0,
    //30..39
    0, 1, 0, 0, 1, 0, 1, 1, 0, 1,
    //40..49
    0, 0, 1, 1, 0, 0, 1, 0, 0, 1,
    //50..59
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0,
    //60..69
    0, 1, 0, 0, 1, 0, 1, 1, 0, 0,
    //7..79
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1,
    //80..89
    0, 0, 1, 0, 1, 1, 0, 0, 1, 0,
    //90..95
    0, 1, 0, 1, 1, 0
};

//table 111, 112
extern const Ipp8u VC1_MB_Mode_PBFieldPic_MBtype_Table[] =
{
    VC1_MB_INTRA,
    VC1_MB_INTRA,
    VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,
    VC1_MB_1MV_INTER,
    VC1_MB_4MV_INTER,
    VC1_MB_4MV_INTER
};

extern const Ipp8u VC1_MB_Mode_PBFieldPic_CBPPresent_Table[] =
{
    0, 1, 0, 0, 1, 1, 0, 1
};

extern const Ipp8s VC1_MB_Mode_PBFieldPic_MVData_Table[] =
{
    -1, -1, 0, 1, 0, 1, -1, -1
};

#endif  //UMC_ENABLE_VC1_VIDEO_DECODER
