/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, AC Intra(Y) tables
//
*/

#include "umc_config.h"
#if defined UMC_ENABLE_VC1_VIDEO_DECODER || defined UMC_ENABLE_VC1_SPLITTER || defined UMC_ENABLE_VC1_VIDEO_ENCODER

#include "umc_vc1_common_defs.h"
#include "umc_vc1_common_acintra.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////High Motion Intra///////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//VC-1 Table 181: High Motion Intra Delta Level
//Indexed by Run Table (Last = 0)
//Run    DeltaLevel    Run    DeltaLevel
//0            19       16        2
//1            15       17        2
//2            12       18        1
//3            11       19        1
//4            6        20        1
//5            5        21        1
//6            4        22        1
//7            4        23        1
//8            4        24        1
//9            4        25        1
//10           3        26        1
//11           3        27        1
//12           3        28        1
//13           3        29        1
//14           3        30        1
//15           3

extern const Ipp8s VC1_HighMotionIntraDeltaLevelLast0[31] =
{
    19, 15, 12, 11,
    6,  5,  4,  4,
    4,  4,  3,  3,
    3,  3,  3,  3,

    2,  2,  1,  1,
    1,  1,  1,  1,
    1,  1,  1,  1,
    1,  1,  1
};

//VC-1 Table 182: High Motion Intra Delta Level
//Indexed by Run Table (Last = 1)
//Run    DeltaLevel    Run    DeltaLevel
//0            6        19        1
//1            5        20        1
//2            4        21        1
//3            4        22        1
//4            3        23        1
//5            2        24        1
//6            2        25        1
//7            2        26        1
//8            2        27        1
//9            2        28        1
//10           2        29        1
//11           2        30        1
//12           2        31        1
//13           2        32        1
//14           2        33        1
//15           2        34        1
//16           1        35        1
//17           1        36        1
//18           1        37        1

extern const Ipp8s VC1_HighMotionIntraDeltaLevelLast1[38] =
{
    6, 5, 4, 4,
    3, 2, 2, 2,
    2, 2, 2, 2,
    2, 2, 2, 2,

    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,

    1, 1, 1, 1,
    1, 1,

};

//VC-1 Table 183: High Motion Intra Delta Run
//Indexed by Level Table (Last = 0)
//Level    DeltaRun    Level    DeltaRun
//1            30       11            3
//2            17       12            2
//3            15       13            1
//4            9        14            1
//5            5        15            1
//6            4        16            0
//7            3        17            0
//8            3        18            0
//9            3        19            0
//10           3

extern const Ipp8s VC1_HighMotionIntraDeltaRunLast0[20] =
{
    -1,
    30, 17, 15, 9,
    5,  4,  3,  3,
    3,  3,  3,  2,
    1,  1,  1,  0,

    0,  0,  0
};

//VC-1 Table 184: High Motion Intra Delta Run
//Indexed by Level Table (Last = 1)
//Level    Delta Run
//1            37
//2            15
//3            4
//4            3
//5            1
//6            0

extern const Ipp8s VC1_HighMotionIntraDeltaRunLast1[7] =
{
    -1,
    37, 15, 4, 3,
    1,  0
};

//////////////////////////////////////////////////////////////////////////
//////////////////////Low Motion Intra///////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 195: Low Motion Intra Delta Level
//Indexed by Run Table (Last = 0)
//Run    DeltaLevel    Run        DeltaLevel
//0            16       11            3
//1            11       12            3
//2            8        13            3
//3            7        14            2
//4            5        15            2
//5            4        16            1
//6            4        17            1
//7            3        18            1
//8            3        19            1
//9            3        20            1
//10           3
extern const Ipp8s VC1_LowMotionIntraDeltaLevelLast0[21] =
{
    16, 11, 8, 7,
    5, 4, 4, 3,
    3, 3, 3, 3,
    3, 3, 2, 2,
    1, 1, 1, 1,
    1
};

//VC-1 Table 196: Low Motion Intra Delta Level
//Indexed by Run Table (Last = 1)
//Run    DeltaLevel    Run    DeltaLevel
//0            4        14        1
//1            4        15        1
//2            3        16        1
//3            3        17        1
//4            2        18        1
//5            2        19        1
//6            2        20        1
//7            2        21        1
//8            2        22        1
//9            2        23        1
//10           2        24        1
//11           2        25        1
//12           2        26        1
//13           2
extern const Ipp8s VC1_LowMotionIntraDeltaLevelLast1[27] =
{
    4, 4, 3, 3,
    2, 2, 2, 2,
    2, 2, 2, 2,
    2, 2, 1, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,
    1, 1, 1
};

//VC-1 Table 197: Low Motion Intra Delta Run
//Indexed by Level Table (Last = 0)
//Level    DeltaRun       Level  DeltaRun
//1            20            9        1
//2            15           10        1
//3            13           11        1
//4            6            12        0
//5            4            13        0
//6            3            14        0
//7            3            15        0
//8            2            16        0
extern const Ipp8s VC1_LowMotionIntraDeltaRunLast0[16+1] =
{
    -1,
    20, 15, 13, 6,
    4, 3, 3, 2,
    1, 1, 1, 0,
    0, 0, 0, 0
};

//VC-1 Table 198: Low Motion Intra Delta Run Indexed by Level Table (Last = 1)
//Level    DeltaRun
//1            26
//2            13
//3            3
//4            1
extern const Ipp8s VC1_LowMotionIntraDeltaRunLast1[4+1] =
{
    -1,
    26, 13, 3, 1
};

//////////////////////////////////////////////////////////////////////////
//////////////////////Mid Rate Intra//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 209: Mid Rate Intra Delta Level Indexed by Run Table (Last = 0)
//Run    Delta Level    Run    Delta Level
//0            27        8        2
//1            10        9        2
//2            5        10        1
//3            4        11        1
//4            3        12        1
//5            3        13        1
//6            3        14        1
//7            3
extern const Ipp8s VC1_MidRateIntraDeltaLevelLast0[15] =
{
    27, 10, 5, 4,
    3,  3,  3, 3,
    2,  2,  1, 1,
    1,  1,  1
};

//VC-1 Table 210: Mid Rate Intra Delta Level Indexed by Run Table (Last = 1)
//Run    Delta Level    Run    Delta Level
//0            8        11        1
//1            3        12        1
//2            2        13        1
//3            2        14        1
//4            2        15        1
//5            2        16        1
//6            2        17        1
//7            1        18        1
//8            1        19        1
//9            1        20        1
//10        1

extern const Ipp8s VC1_MidRateIntraDeltaLevelLast1[21] =
{
    8, 3, 2, 2,
    2, 2, 2, 1,
    1, 1, 1, 1,
    1, 1, 1, 1,

    1, 1, 1, 1,
    1
};

//VC-1 Table 211: Mid Rate Intra Delta Run Indexed by Level Table (Last = 0)
//Level    Delta Run    Level    Delta Run
//1           14        15        0
//2            9        16        0
//3            7        17        0
//4            3        18        0
//5            2        19        0
//6            1        20        0
//7            1        21        0
//8            1        22        0
//9            1        23        0
//10           1        24        0
//11           0        25        0
//12           0        26        0
//13           0        27        0
//14           0
extern const Ipp8s VC1_MidRateIntraDeltaRunLast0[28] =
{
    -1,
    14, 9, 7, 3,
    2,  1, 1, 1,
    1,  1, 0, 0,
    0,  0, 0, 0,

    0,  0, 0, 0,
    0,  0, 0, 0,
    0,  0, 0
};

//VC-1 Table 212: Mid Rate Intra Delta Run Indexed by Level Table (Last = 1)
//Level    Delta Run
//1    20
//2    6
//3    1
//4    0
//5    0
//6    0
//7    0
//8    0
extern const Ipp8s VC1_MidRateIntraDeltaRunLast1[9] =
{
    -1,
    20, 6, 1, 0,
    0,  0, 0, 0
};


//////////////////////////////////////////////////////////////////////////
//////////////////////High Rate Intra//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 223: High Rate Intra Delta Level
//Indexed by Run Table (Last = 0)
//Run    Delta Level    Run    Delta Level
//0            56        8        3
//1            20        9        3
//2            10       10        2
//3            7        11        2
//4            6        12        2
//5            5        13        2
//6            4        14        1
//7            3


extern const Ipp8s VC1_HighRateIntraDeltaLevelLast0[15] =
{
    56, 20, 10, 7,
    6,  5,  4,  3,
    3,  3,  2,  2,
    2,  2,  1
};

//VC-1 Table 224: High Rate Intra Delta Level
//Indexed by Run Table (Last = 1)
//Run    Delta Level    Run    Delta Level
//0            4        9         2
//1            3        10        2
//2            3        11        2
//3            2        12        2
//4            2        13        2
//5            2        14        2
//6            2        15        1
//7            2        16        1
//8            2

extern const Ipp8s VC1_HighRateIntraDeltaLevelLast1[17] =
{
    4, 3, 3, 2,
    2, 2, 2, 2,
    2, 2, 2, 2,
    2, 2, 2, 1,

    1
};

//VC-1 Table 225: High Rate Intra Delta Run
//Indexed by Level Table (Last = 0)
//Level Delta Run   Level    Delta Run
//1        14        29        0
//2        13        30        0
//3         9        31        0
//4         6        32        0
//5         5        33        0
//6         4        34        0
//7         3        35        0
//8         2        36        0
//9         2        37        0
//10        2        38        0
//11        1        39        0
//12        1        40        0
//13        1        41        0
//14        1        42        0
//15        1        43        0
//16        1        44        0
//17        1        45        0
//18        1        46        0
//19        1        47        0
//20        1        48        0
//21        0        49        0
//22        0        50        0
//23        0        51        0
//24        0        52        0
//25        0        53        0
//26        0        54        0
//27        0        55        0
//28        0        56        0

extern const Ipp8s VC1_HighRateIntraDeltaRunLast0[57] =
{
    -1,
    14, 13, 9,  6,
    5,  4,  3,  2,
    2,  2,  1,  1,
    1,  1,  1,  1,

    1,  1,  1,  1,
    0,  0,  0,  0,
    0,  0,  0,  0,
    0,  0,  0,  0,

    0,  0,  0,  0,
    0,  0,  0,  0,
    0,  0,  0,  0,
    0,  0,  0,  0,

    0,  0,  0,  0,
    0,  0,  0,  0,

};

//VC-1 Table 226: High Rate Intra Delta Run
//Indexed by Level Table (Last = 1)
//Level    Delta Run
//1            16
//2            14
//3            2
//4            0

extern const Ipp8s VC1_HighRateIntraDeltaRunLast1[5] =
{
    -1,
    16, 14, 2, 0
};
#endif //UMC_ENABLE_VC1_VIDEO_DECODER
