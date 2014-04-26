/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, AC Inter(Cr, Cb) tables
//
*/

#include "umc_config.h"
#if defined UMC_ENABLE_VC1_VIDEO_DECODER || defined UMC_ENABLE_VC1_SPLITTER || defined UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_common_defs.h"
#include "umc_vc1_common_acinter.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////High Motion Inter///////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 188: High Motion Inter Delta Level
//Indexed by Run Table (Last = 0)
//Run    DeltaLevel    Run    DeltaLevel
//0            23       14        2
//1            11       15        2
//2            8        16        2
//3            7        17        1
//4            5        18        1
//5            5        19        1
//6            4        20        1
//7            4        21        1
//8            3        22        1
//9            3        23        1
//10           3        24        1
//11           3        25        1
//12           2        26        1
//13           2

extern const Ipp8s VC1_HighMotionInterDeltaLevelLast0[27] =
{
    23, 11, 8,  7,
    5,  5,  4,  4,
    3,  3,  3,  3,
    2,  2,  2,  2,

    2,  1,  1,  1,
    1,  1,  1,  1,
    1,  1,  1
};

//VC-1 Table 189: High Motion Inter Delta Level
//Indexed by Run Table (Last = 1)
//Run    DeltaLevel    Run    DeltaLevel
//0         9        19        1
//1         5        20        1
//2         4        21        1
//3         4        22        1
//4         3        23        1
//5         3        24        1
//6         3        25        1
//7         2        26        1
//8         2        27        1
//9         2        28        1
//10        2        29        1
//11        2        30        1
//12        2        31        1
//13        2        32        1
//14        2        33        1
//15        1        34        1
//16        1        35        1
//17        1        36        1
//18        1

extern const Ipp8s VC1_HighMotionInterDeltaLevelLast1[37] =
{
    9, 5, 4, 4,
    3, 3, 3, 2,
    2, 2, 2, 2,
    2, 2, 2, 1,

    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,

    1, 1, 1, 1,
    1
};

//VC-1 Table 190: High Motion Inter Delta Run
//Indexed by Level Table (Last = 0)
//Level    DeltaRun    Level    DeltaRun
//1        26          13            0
//2        16          14            0
//3        11          15            0
//4         7          16            0
//5         5          17            0
//6         3          18            0
//7         3          19            0
//8         2          20            0
//9         1          21            0
//10        1          22            0
//11        1          23            0
//12        0

extern const Ipp8s VC1_HighMotionInterDeltaRunLast0[24] =
{
    -1,

    26,16,11,7,
    5, 3, 3, 2,
    1, 1, 1, 0,
    0, 0, 0, 0,

    0, 0, 0, 0,
    0, 0, 0
};

//VC-1 Table 191: High Motion Inter Delta Run
//Indexed by Level Table (Last = 1)
//Level    DeltaRun
//1            36
//2            14
//3            6
//4            3
//5            1
//6            0
//7            0
//8            0
//9            0
extern const Ipp8s VC1_HighMotionInterDeltaRunLast1[10] =
{
    -1,

    36,14, 6, 3,
    1, 0,  0, 0,
    0
};

//////////////////////////////////////////////////////////////////////////
//////////////////////Low Motion Inter///////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 202: Low Motion Inter Delta Level
//Indexed by Run Table (Last = 0)
//Run    DeltaLevel    Run    DeltaLevel
//0        14            15        2
//1        9            16        1
//2        5            17        1
//3        4            18        1
//4        4            19        1
//5        4            20        1
//6        3            21        1
//7        3            22        1
//8        3            23        1
//9        3            24        1
//10       3            25        1
//11       3            26        1
//12       3            27        1
//13       2            28        1
//14       2            29        1

extern const Ipp8s VC1_LowMotionInterDeltaLevelLast0[30] =
{
    14,9, 5, 4,
    4, 4, 3, 3,
    3, 3, 3, 3,
    3, 2, 2, 2,

    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1
};

//VC-1 Table 203: Low Motion Inter Delta Level
//Indexed by Run Table (Last = 1)
//Run    DeltaLevel    Run    DeltaLevel
//0        5            22        1
//1        4            23        1
//2        3            24        1
//3        3            25        1
//4        2            26        1
//5        2            27        1
//6        2            28        1
//7        2            29        1
//8        2            30        1
//9        2            31        1
//10       2            32        1
//11       2            33        1
//12       2            34        1
//13       2            35        1
//14       2            36        1
//15       2            37        1
//16       1            38        1
//17       1            39        1
//18       1            40        1
//19       1            41        1
//20       1            42        1
//21       1            43        1
extern const Ipp8s VC1_LowMotionInterDeltaLevelLast1[44] =
{
    5,4,3,3,
    2,2,2,2,
    2,2,2,2,
    2,2,2,2,

    1,1,1,1,
    1,1,1,1,
    1,1,1,1,
    1,1,1,1,

    1,1,1,1,
    1,1,1,1,
    1,1,1,1

};

//VC-1 Table 204: Low Motion Inter Delta Run
//Indexed by Level Table (Last = 0)
//Level    Delta Run    Level    Delta Run
//1            29        8        1
//2            15        9        1
//3            12       10        0
//4            5        11        0
//5            2        12        0
//6            1        13        0
//7            1        14        0
extern const Ipp8s VC1_LowMotionInterDeltaRunLast0[15] =
{
    -1,

    29, 15, 12, 5,
    2,  1,  1,  1,
    1,  0,  0,  0,
    0,  0

};

//VC-1 Table 205: Low Motion Inter Delta Run
//Indexed by Level Table (Last = 1)
//Level    DeltaRun
//1            43
//2            15
//3            3
//4            1
//5            0
extern const Ipp8s VC1_LowMotionInterDeltaRunLast1[6] =
{
    -1,
    43, 15, 3, 1,
    0
};

//////////////////////////////////////////////////////////////////////////
//////////////////////Mid Rate Inter//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 216: Mid Rate Inter Delta Level
//Indexed by Run Table (Last = 0)
//Run    Delta Level    Run    Delta Level
//0           12        14        1
//1            6        15        1
//2            4        16        1
//3            3        17        1
//4            3        18        1
//5            3        19        1
//6            3        20        1
//7            2        21        1
//8            2        22        1
//9            2        23        1
//10           2        24        1
//11           1        25        1
//12           1        26        1
//13           1

extern const Ipp8s VC1_MidRateInterDeltaLevelLast0[27] =
{
    12, 6, 4, 3,
    3,  3, 3, 2,
    2,  2, 2, 1,
    1,  1, 1, 1,

    1,  1, 1, 1,
    1,  1, 1, 1,
    1,  1, 1
};

//VC-1 Table 217: Mid Rate Inter Delta Level
//Indexed by Run Table (Last = 1)
//Run    Delta Level    Run    Delta Level
//0            3        21            1
//1            2        22            1
//2            1        23            1
//3            1        24            1
//4            1        25            1
//5            1        26            1
//6            1        27            1
//7            1        28            1
//8            1        29            1
//9            1        30            1
//10           1        31            1
//11           1        32            1
//12           1        33            1
//13           1        34            1
//14           1        35            1
//15           1        36            1
//16           1        37            1
//17           1        38            1
//18           1        39            1
//19           1        40            1
//20           1

extern const Ipp8s VC1_MidRateInterDeltaLevelLast1[41] =
{
    3, 2, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,

    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,

    1, 1, 1, 1,
    1, 1, 1, 1,
    1,
};

//VC-1 Table 218: Mid Rate Inter Delta Run
//Indexed by Level Table (Last = 0)
//Level    Delta Run    Level    Delta Run
//1            26        7            0
//2            10        8            0
//3            6         9            0
//4            2        10            0
//5            1        11            0
//6            1        12            0
extern const Ipp8s VC1_MidRateInterDeltaRunLast0[13] =
{
    -1,
    26, 10, 6, 2,
    1,   1, 0, 0,
    0,   0, 0, 0
};


//VC-1 Table 219: Mid Rate Inter Delta Run
//Indexed by Level Table (Last = 1)
//Level    Delta Run
//1            40
//2            1
//3            0
extern const Ipp8s VC1_MidRateInterDeltaRunLast1[4] =
{
    -1,
    40, 1, 0
};

//////////////////////////////////////////////////////////////////////////
//////////////////////High Rate Inter/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 230: High Rate Inter Delta Level
//Indexed by Run Table (Last = 0)
//Run    Delta Level    Run    Delta Level
//0            32       13        2
//1            13       14        2
//2            8        15        2
//3            6        16        2
//4            5        17        2
//5            4        18        2
//6            4        19        2
//7            3        20        2
//8            3        21        2
//9            3        22        2
//10           2        23        1
//11           2        24        1
//12           2


extern const Ipp8s VC1_HighRateInterDeltaLevelLast0[25] =
{
    32, 13, 8, 6,
    5,  4,  4, 3,
    3,  3,  2, 2,
    2,  2,  2, 2,

    2,  2,  2, 2,
    2,  2,  2, 1,
    1

};

//VC-1 Table 231: High Rate Inter Delta Level
//Indexed by Run Table (Last = 1)
//Run    Delta Level    Run    Delta Level
//0            4        16        2
//1            3        17        2
//2            3        18        2
//3            3        19        2
//4            2        20        2
//5            2        21        2
//6            2        22        2
//7            2        23        2
//8            2        24        2
//9            2        25        2
//10           2        26        2
//11           2        27        2
//12           2        28        2
//13           2        29        1
//14           2        30        1
//15           2

extern const Ipp8s VC1_HighRateInterDeltaLevelLast1[31] =
{
    4, 3, 3, 3,
    2, 2, 2, 2,
    2, 2, 2, 2,
    2, 2, 2, 2,

    2, 2, 2, 2,
    2, 2, 2, 2,
    2, 2, 2, 2,
    2, 1, 1
};

//VC-1 Table 232: High Rate Inter Delta Run
//Indexed by Level Table (Last = 0)
//Level    Delta Run    Level    Delta Run
//1            24       17        0
//2            22       18        0
//3            9        19        0
//4            6        20        0
//5            4        21        0
//6            3        22        0
//7            2        23        0
//8            2        24        0
//9            1        25        0
//10           1        26        0
//11           1        27        0
//12           1        28        0
//13           1        29        0
//14           0        30        0
//15           0        31        0
//16           0        32        0

extern const Ipp8s VC1_HighRateInterDeltaRunLast0[33] =
{
    -1,
    24, 22, 9,  6,
    4,  3,  2,  2,
    1,  1,  1,  1,

    1, //I don't know 0 or 1

    0,  0,  0,  0,
    0,  0,  0,  0,
    0,  0,  0,  0,
    0,  0,  0,  0,
    0,  0,  0,
};

//VC-1 Table 233: High Rate Inter Delta Run
//Indexed by Level Table (Last = 1)
//Level    Delta Run
//1        30
//2        28
//3        3
//4        0

extern const Ipp8s VC1_HighRateInterDeltaRunLast1[5] =
{
    -1,
    30, 28, 3, 0
};

#endif //UMC_ENABLE_VC1_VIDEO_DECODER
