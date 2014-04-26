/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) splitter tables
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_VC1_SPLITTER)

#include "umc_vc1_spl_tbl.h"

AspectRatio AspectRatioTable[16] =
{
    {0, 0},     {1, 1},    {12, 11},    {10, 11},
    {16, 11},   {40, 33},  {24, 11},    {20, 11},
    {32, 11},   {80, 33},  {18, 11},    {15, 11},
    {64, 33},   {169, 99}, {0, 0}
};

Ipp64f FrameRateNumerator[256] =
{
    0,       24000.0,    25000.0,    30000.0,
    50000.0, 60000.0,    48000.0,    72000.0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

Ipp64f FrameRateDenomerator[16] =
{
    0, 1000, 1001, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0
};


Ipp32u bMax_LevelLimits[4][5] =
{
     {   /* Simple Profile */
        20,     /* Low    Level */
        77,     /* Medium Level */
        0,      /* Hight Level */
        0,
        0
    },
    {   /* Main Profile */
        306,        /* Low    Level */
        611,        /* Medium Level */
        2442,       /* Hight Level */
        0,
        0
    },
    {   /* Reserved Profile */
        0,0,0,0,0
    },
    {   /* Advanced Profile */
        250,        /*L0 level*/
        1250,       /*L1 level*/
        2500,       /*L2 level*/
        5500,       /*L3 level*/
        16500
    }
};

#endif //UMC_ENABLE_VC1_SPLITTER
