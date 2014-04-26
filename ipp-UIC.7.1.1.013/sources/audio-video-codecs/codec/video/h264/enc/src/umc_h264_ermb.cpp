//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2004 - 2012 Intel Corporation. All Rights Reserved.
//

#include "umc_config.h"
#ifdef UMC_ENABLE_H264_VIDEO_ENCODER

#include <math.h>

#include "umc_h264_video_encoder.h"
#include "umc_h264_tables.h"
#include "umc_h264_to_ipp.h"
#include "umc_h264_bme.h"

#include "ippdefs.h"

//#define TRACE_INTRA 50
//#define TRACE_INTER 5
//#define TRACE_INTRA_16X16 185

#ifdef INTRINSIC_OPT
#define TRANS4x4_OPT
#endif

#define MAX_CAVLC_LEVEL 2064
#define MIN_CAVLC_LEVEL -2063

#define LUMA_MB_MAX_COST 6
#define CHROMA_COEFF_MAX_COST 7
#define LUMA_8X8_MAX_COST 4

#define LUMA_COEFF_8X8_MAX_COST 4
#define LUMA_COEFF_MB_8X8_MAX_COST 6

static const __ALIGN16 Ipp16s FwdQuantTable_16s[6][16] = {
    {13107, 8066, 13107, 8066, 8066, 5243,  8066, 5243, 13107, 8066, 13107, 8066, 8066, 5243,  8066, 5243},
    {11916, 7490, 11916, 7490, 7490, 4660,  7490, 4660, 11916, 7490, 11916, 7490, 7490, 4660,  7490, 4660},
    {10082, 6554, 10082, 6554, 6554, 4194,  6554, 4194, 10082, 6554, 10082, 6554, 6554, 4194,  6554, 4194},
    {9362, 5825, 9362, 5825, 5825, 3647, 5825, 3647, 9362, 5825, 9362, 5825, 5825, 3647, 5825, 3647},
    {8192, 5243, 8192, 5243, 5243, 3355, 5243, 3355, 8192, 5243, 8192, 5243, 5243, 3355, 5243, 3355},
    {7282, 4559, 7282, 4559, 4559, 2893, 4559, 2893, 7282, 4559, 7282, 4559, 4559, 2893, 4559, 2893}
};

static const __ALIGN16 Ipp16s InvLevelScale_4x4_default[6][16] = {
    {160, 208, 160, 208, 208, 256, 208, 256, 160, 208, 160, 208, 208, 256, 208, 256},
    {176, 224, 176, 224, 224, 288, 224, 288, 176, 224, 176, 224, 224, 288, 224, 288},
    {208, 256, 208, 256, 256, 320, 256, 320, 208, 256, 208, 256, 256, 320, 256, 320},
    {224, 288, 224, 288, 288, 368, 288, 368, 224, 288, 224, 288, 288, 368, 288, 368},
    {256, 320, 256, 320, 320, 400, 320, 400, 256, 320, 256, 320, 320, 400, 320, 400},
    {288, 368, 288, 368, 368, 464, 368, 464, 288, 368, 288, 368, 368, 464, 368, 464}
};

static const __ALIGN16 Ipp16s InvScale4x4[6][16] = {
    { 2560, 4160, 2560, 4160, 4160, 6400, 4160, 6400, 2560, 4160, 2560, 4160, 4160, 6400, 4160, 6400, },
    { 2816, 4480, 2816, 4480, 4480, 7200, 4480, 7200, 2816, 4480, 2816, 4480, 4480, 7200, 4480, 7200, },
    { 3328, 5120, 3328, 5120, 5120, 8000, 5120, 8000, 3328, 5120, 3328, 5120, 5120, 8000, 5120, 8000, },
    { 3584, 5760, 3584, 5760, 5760, 9200, 5760, 9200, 3584, 5760, 3584, 5760, 5760, 9200, 5760, 9200, },
    { 4096, 6400, 4096, 6400, 6400, 10000, 6400, 10000, 4096, 6400, 4096, 6400, 6400, 10000, 6400, 10000, },
    { 4608, 7360, 4608, 7360, 7360, 11600, 7360, 11600, 4608, 7360, 4608, 7360, 7360, 11600, 7360, 11600 }
};

static const __ALIGN16 Ipp8u h264_qp_rem[90]= {
    0,   1,   2,   3,   4,   5,   0,   1,   2,   3,   4,   5,   0,   1,   2,  3,   4,   5,
    0,   1,   2,   3,   4,   5,   0,   1,   2,   3,   4,   5,   0,   1,   2,  3,   4,   5,
    0,   1,   2,   3,   4,   5,   0,   1,   2,   3,   4,   5,   0,   1,   2,  3,   4,   5,
    0,   1,   2,   3,   4,   5,   0,   1,   2,   3,   4,   5,   0,   1,   2,  3,   4,   5,
    0,   1,   2,   3,   4,   5,   0,   1,   2,   3,   4,   5,   0,   1,   2,  3,   4,   5
};

static const __ALIGN16 Ipp8u h264_qp6[90] = {
    0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,   2,   2,   2,  2,   2,   2,
    3,   3,   3,   3,   3,   3,   4,   4,   4,   4,   4,   4,   5,   5,   5,  5,   5,   5,
    6,   6,   6,   6,   6,   6,   7,   7,   7,   7,   7,   7,   8,   8,   8,  8,   8,   8,
    9,   9,   9,   9,   9,   9,  10,  10,  10,  10,  10,  10,  11,  11,  11, 11,  11,  11,
    12,  12,  12,  12,  12,  12,  13,  13,  13,  13,  13,  13,  14,  14,  14, 14,  14,  14
};

static const __ALIGN16 Ipp8u CorrMatrix[16] = {
    16, 20, 16, 20,
    20, 25, 20, 25,
    16, 20, 16, 20,
    20, 25, 20, 25
};

static const Ipp32s ctx_neq1p1[8] = { 1,2,3,4,0,0,0,0};
static const Ipp32s ctx_ngt1[8] =   { 5,5,5,5,6,7,8,9};
static const Ipp32s ctx_id_trans0[8] = {1,2,3,3,4,5,6,7};
static const Ipp32s ctx_id_trans1[8] = {4,4,4,4,5,6,7,7};
static const Ipp32s ctx_id_trans13[7] = {4,4,4,4,5,6,6};

void transform4x4(Ipp16s* pSrc, Ipp16s* pDst)
{
    const Ipp16s *psrcrow[4];
    Ipp16s *pdstrow[4];
    Ipp16s a[4];
    Ipp16s i;

    psrcrow[0] = pSrc;
    psrcrow[1] = pSrc + 4;
    psrcrow[2] = pSrc + 8;
    psrcrow[3] = pSrc + 12;
    pdstrow[0] = pDst;
    pdstrow[1] = pDst + 4;
    pdstrow[2] = pDst + 8;
    pdstrow[3] = pDst + 12;

    for (i = 0; i < 4; i ++)
    {
        a[0] = (Ipp16s) (psrcrow[i][0] + psrcrow[i][3]);
        a[3] = (Ipp16s) (psrcrow[i][0] - psrcrow[i][3]);
        a[1] = (Ipp16s) (psrcrow[i][1] + psrcrow[i][2]);
        a[2] = (Ipp16s) (psrcrow[i][1] - psrcrow[i][2]);

        pdstrow[i][0] = (Ipp16s) (a[0] + a[1]);
        pdstrow[i][1] = (Ipp16s) (2 * a[3] + a[2]);
        pdstrow[i][2] = (Ipp16s) (a[0] - a[1]);
        pdstrow[i][3] = (Ipp16s) (a[3] - 2 * a[2]);
    }

    /* vertical */
    for (i = 0; i < 4; i ++)
    {
        a[0] = (Ipp16s) (pdstrow[0][i] + pdstrow[3][i]);
        a[3] = (Ipp16s) (pdstrow[0][i] - pdstrow[3][i]);
        a[1] = (Ipp16s) (pdstrow[1][i] + pdstrow[2][i]);
        a[2] = (Ipp16s) (pdstrow[1][i] - pdstrow[2][i]);

        pdstrow[0][i] = (Ipp16s) (a[0] + a[1]);
        pdstrow[1][i] = (Ipp16s) (2 * a[3] + a[2]);
        pdstrow[2][i] = (Ipp16s) (a[0] - a[1]);
        pdstrow[3][i] = (Ipp16s) (a[3] - 2 * a[2]);
    }
}

Ipp16s quant(Ipp16s coeff, Ipp32s pos, Ipp32s QP, Ipp32s)
{
    Ipp32s  qp_rem, qp6;
    const   Ipp16s*  quantTbl;
    Ipp16s sign;
    Ipp32s scale;
    Ipp32s scaleOffset;

    qp_rem = h264_qp_rem[QP];
    qp6 = h264_qp6[QP];

    quantTbl = FwdQuantTable_16s[qp_rem];
    scale = 15+qp6;
//    scaleOffset = intra ? (1<<scale)/3 : (1<<scale)/6;
//    scaleOffset = (1<<(scale-1)); //intra ? (1<<scale)/3 : (1<<scale)/6;
    scaleOffset = 1<<scale; //intra ? (1<<scale)/3 : (1<<scale)/6;

    sign = (Ipp16s)(coeff < 0 ? -1:1);
    return (Ipp16s)(sign*((sign*coeff*quantTbl[pos]+scaleOffset)>>scale));
}

//pos - non zig-zag scan pos (normal pos)
Ipp16s dequant(Ipp16s coeff,Ipp32s pos, Ipp32s QP)
{
    Ipp32s  qp_rem, qp6;
    const   Ipp16s*  invQuantTbl;

    qp_rem = h264_qp_rem[QP];
    qp6 = h264_qp6[QP];

    //invQuantTbl = (pScaleLevelsInv == NULL)? InvLevelScale_4x4_default[qp_rem]: pScaleLevelsInv;
    invQuantTbl = InvLevelScale_4x4_default[qp_rem];

    if( QP >= 24 ){
        Ipp32s shift = qp6 - 4;
        return  (coeff*invQuantTbl[pos])<<shift;
    }else{
        Ipp32s shift = 4 - qp6;
        Ipp32s offset = 1<<(shift - 1);
        return (coeff*invQuantTbl[pos]+offset)>>shift;
    }
}

IppStatus TransformQuantOptFwd4x4_H264_16s32s_C1(
    Ipp16s*,
    Ipp32s*,
    Ipp32s,
    Ipp32s*,
    Ipp32s,
    const Ipp16s*,
    Ipp32s*,
    const Ipp16s*,
    const H264Slice<Ipp32s, Ipp16u>*,
    Ipp32s,
    CabacStates*)
{
    return ippStsNoErr;
}

static const Ipp8u bitcountLPS[128][4] = {
{ 1, 1, 1, 1,  },
{ 1, 1, 1, 1,  },
{ 1, 1, 1, 1,  },
{ 2, 1, 1, 1,  },
{ 2, 1, 1, 1,  },
{ 2, 1, 1, 1,  },
{ 2, 1, 1, 1,  },
{ 2, 2, 1, 1,  },
{ 2, 2, 1, 1,  },
{ 2, 2, 1, 1,  },
{ 2, 2, 2, 1,  },
{ 2, 2, 2, 1,  },
{ 2, 2, 2, 1,  },
{ 2, 2, 2, 2,  },
{ 2, 2, 2, 2,  },
{ 2, 2, 2, 2,  },
{ 3, 2, 2, 2,  },
{ 3, 2, 2, 2,  },
{ 3, 2, 2, 2,  },
{ 3, 2, 2, 2,  },
{ 3, 3, 2, 2,  },
{ 3, 3, 2, 2,  },
{ 3, 3, 2, 2,  },
{ 3, 3, 3, 2,  },
{ 3, 3, 3, 2,  },
{ 3, 3, 3, 2,  },
{ 3, 3, 3, 3,  },
{ 3, 3, 3, 3,  },
{ 3, 3, 3, 3,  },
{ 3, 3, 3, 3,  },
{ 4, 3, 3, 3,  },
{ 4, 3, 3, 3,  },
{ 4, 3, 3, 3,  },
{ 4, 4, 3, 3,  },
{ 4, 4, 3, 3,  },
{ 4, 4, 3, 3,  },
{ 4, 4, 3, 3,  },
{ 4, 4, 4, 3,  },
{ 4, 4, 4, 3,  },
{ 4, 4, 4, 4,  },
{ 4, 4, 4, 4,  },
{ 4, 4, 4, 4,  },
{ 4, 4, 4, 4,  },
{ 5, 4, 4, 4,  },
{ 5, 4, 4, 4,  },
{ 5, 4, 4, 4,  },
{ 5, 4, 4, 4,  },
{ 5, 5, 4, 4,  },
{ 5, 5, 4, 4,  },
{ 5, 5, 4, 4,  },
{ 5, 5, 5, 4,  },
{ 5, 5, 5, 4,  },
{ 5, 5, 5, 4,  },
{ 5, 5, 5, 5,  },
{ 5, 5, 5, 5,  },
{ 5, 5, 5, 5,  },
{ 5, 5, 5, 5,  },
{ 6, 5, 5, 5,  },
{ 6, 5, 5, 5,  },
{ 6, 5, 5, 5,  },
{ 6, 5, 5, 5,  },
{ 6, 6, 5, 5,  },
{ 6, 6, 5, 5,  },
{ 6, 6, 6, 6,  },
{ 1, 1, 1, 1,  },
{ 1, 1, 1, 1,  },
{ 1, 1, 1, 1,  },
{ 2, 1, 1, 1,  },
{ 2, 1, 1, 1,  },
{ 2, 1, 1, 1,  },
{ 2, 1, 1, 1,  },
{ 2, 2, 1, 1,  },
{ 2, 2, 1, 1,  },
{ 2, 2, 1, 1,  },
{ 2, 2, 2, 1,  },
{ 2, 2, 2, 1,  },
{ 2, 2, 2, 1,  },
{ 2, 2, 2, 2,  },
{ 2, 2, 2, 2,  },
{ 2, 2, 2, 2,  },
{ 3, 2, 2, 2,  },
{ 3, 2, 2, 2,  },
{ 3, 2, 2, 2,  },
{ 3, 2, 2, 2,  },
{ 3, 3, 2, 2,  },
{ 3, 3, 2, 2,  },
{ 3, 3, 2, 2,  },
{ 3, 3, 3, 2,  },
{ 3, 3, 3, 2,  },
{ 3, 3, 3, 2,  },
{ 3, 3, 3, 3,  },
{ 3, 3, 3, 3,  },
{ 3, 3, 3, 3,  },
{ 3, 3, 3, 3,  },
{ 4, 3, 3, 3,  },
{ 4, 3, 3, 3,  },
{ 4, 3, 3, 3,  },
{ 4, 4, 3, 3,  },
{ 4, 4, 3, 3,  },
{ 4, 4, 3, 3,  },
{ 4, 4, 3, 3,  },
{ 4, 4, 4, 3,  },
{ 4, 4, 4, 3,  },
{ 4, 4, 4, 4,  },
{ 4, 4, 4, 4,  },
{ 4, 4, 4, 4,  },
{ 4, 4, 4, 4,  },
{ 5, 4, 4, 4,  },
{ 5, 4, 4, 4,  },
{ 5, 4, 4, 4,  },
{ 5, 4, 4, 4,  },
{ 5, 5, 4, 4,  },
{ 5, 5, 4, 4,  },
{ 5, 5, 4, 4,  },
{ 5, 5, 5, 4,  },
{ 5, 5, 5, 4,  },
{ 5, 5, 5, 4,  },
{ 5, 5, 5, 5,  },
{ 5, 5, 5, 5,  },
{ 5, 5, 5, 5,  },
{ 5, 5, 5, 5,  },
{ 6, 5, 5, 5,  },
{ 6, 5, 5, 5,  },
{ 6, 5, 5, 5,  },
{ 6, 5, 5, 5,  },
{ 6, 6, 5, 5,  },
{ 6, 6, 5, 5,  },
{ 6, 6, 6, 6,  },
};

static const Ipp16u rangeLPS[128][4] = {
{ 256, 352, 416, 480,  },
{ 256, 334, 394, 454,  },
{ 256, 316, 374, 432,  },
{ 492, 300, 356, 410,  },
{ 464, 284, 338, 390,  },
{ 444, 270, 320, 370,  },
{ 420, 256, 304, 350,  },
{ 400, 488, 288, 332,  },
{ 380, 464, 274, 316,  },
{ 360, 440, 260, 300,  },
{ 340, 416, 492, 284,  },
{ 324, 396, 468, 270,  },
{ 308, 376, 444, 256,  },
{ 292, 356, 420, 488,  },
{ 276, 340, 400, 464,  },
{ 264, 320, 380, 440,  },
{ 496, 304, 360, 416,  },
{ 472, 288, 344, 396,  },
{ 448, 276, 324, 376,  },
{ 424, 260, 308, 356,  },
{ 408, 496, 292, 340,  },
{ 384, 472, 276, 320,  },
{ 368, 448, 264, 304,  },
{ 344, 424, 504, 288,  },
{ 328, 400, 472, 276,  },
{ 312, 384, 448, 260,  },
{ 296, 360, 432, 496,  },
{ 280, 344, 408, 472,  },
{ 264, 328, 384, 448,  },
{ 256, 312, 368, 424,  },
{ 480, 296, 344, 400,  },
{ 464, 280, 328, 384,  },
{ 432, 264, 312, 360,  },
{ 416, 496, 296, 344,  },
{ 384, 480, 280, 328,  },
{ 368, 448, 264, 312,  },
{ 352, 432, 256, 296,  },
{ 336, 416, 480, 280,  },
{ 320, 384, 464, 264,  },
{ 304, 368, 432, 496,  },
{ 288, 352, 416, 480,  },
{ 272, 336, 400, 448,  },
{ 256, 320, 368, 432,  },
{ 480, 304, 352, 400,  },
{ 448, 288, 336, 384,  },
{ 448, 272, 320, 368,  },
{ 416, 256, 304, 352,  },
{ 384, 480, 288, 336,  },
{ 384, 448, 272, 320,  },
{ 352, 448, 256, 304,  },
{ 352, 416, 480, 288,  },
{ 320, 384, 480, 272,  },
{ 320, 384, 448, 256,  },
{ 288, 352, 416, 480,  },
{ 288, 352, 384, 448,  },
{ 256, 320, 384, 448,  },
{ 256, 288, 352, 416,  },
{ 448, 288, 352, 384,  },
{ 448, 288, 320, 384,  },
{ 448, 256, 320, 352,  },
{ 384, 256, 288, 352,  },
{ 384, 448, 288, 320,  },
{ 384, 448, 256, 288,  },
{ 128, 128, 128, 128,  },

{ 256, 352, 416, 480,  },
{ 256, 334, 394, 454,  },
{ 256, 316, 374, 432,  },
{ 492, 300, 356, 410,  },
{ 464, 284, 338, 390,  },
{ 444, 270, 320, 370,  },
{ 420, 256, 304, 350,  },
{ 400, 488, 288, 332,  },
{ 380, 464, 274, 316,  },
{ 360, 440, 260, 300,  },
{ 340, 416, 492, 284,  },
{ 324, 396, 468, 270,  },
{ 308, 376, 444, 256,  },
{ 292, 356, 420, 488,  },
{ 276, 340, 400, 464,  },
{ 264, 320, 380, 440,  },
{ 496, 304, 360, 416,  },
{ 472, 288, 344, 396,  },
{ 448, 276, 324, 376,  },
{ 424, 260, 308, 356,  },
{ 408, 496, 292, 340,  },
{ 384, 472, 276, 320,  },
{ 368, 448, 264, 304,  },
{ 344, 424, 504, 288,  },
{ 328, 400, 472, 276,  },
{ 312, 384, 448, 260,  },
{ 296, 360, 432, 496,  },
{ 280, 344, 408, 472,  },
{ 264, 328, 384, 448,  },
{ 256, 312, 368, 424,  },
{ 480, 296, 344, 400,  },
{ 464, 280, 328, 384,  },
{ 432, 264, 312, 360,  },
{ 416, 496, 296, 344,  },
{ 384, 480, 280, 328,  },
{ 368, 448, 264, 312,  },
{ 352, 432, 256, 296,  },
{ 336, 416, 480, 280,  },
{ 320, 384, 464, 264,  },
{ 304, 368, 432, 496,  },
{ 288, 352, 416, 480,  },
{ 272, 336, 400, 448,  },
{ 256, 320, 368, 432,  },
{ 480, 304, 352, 400,  },
{ 448, 288, 336, 384,  },
{ 448, 272, 320, 368,  },
{ 416, 256, 304, 352,  },
{ 384, 480, 288, 336,  },
{ 384, 448, 272, 320,  },
{ 352, 448, 256, 304,  },
{ 352, 416, 480, 288,  },
{ 320, 384, 480, 272,  },
{ 320, 384, 448, 256,  },
{ 288, 352, 416, 480,  },
{ 288, 352, 384, 448,  },
{ 256, 320, 384, 448,  },
{ 256, 288, 352, 416,  },
{ 448, 288, 352, 384,  },
{ 448, 288, 320, 384,  },
{ 448, 256, 320, 352,  },
{ 384, 256, 288, 352,  },
{ 384, 448, 288, 320,  },
{ 384, 448, 256, 288,  },
{ 128, 128, 128, 128,  },
};

Ipp32s w[16] = {
    25,10,25,10,
    10, 4,10, 4,
    25,10,25,10,
    10, 4,10, 4
};

#if 0
/* int(-log2(p)*256 + 0.5)
   Based on original CABAC probabilities from paper
   H.Schwarz, T.Wiegand "Context-Based Adaptive Binary Arithmetic Coding in the H.264/AVC Video Compression Standard" */
const Ipp32s p_bits_lps[64] = {
 256,  275,  294,  314,
 333,  352,  371,  391,
 410,  429,  448,  468,
 487,  506,  525,  545,
 564,  583,  602,  622,
 641,  660,  679,  699,
 718,  737,  756,  776,
 795,  814,  833,  853,
 872,  891,  910,  930,
 949,  968,  987, 1007,
1026, 1045, 1064, 1084,
1103, 1122, 1141, 1161,
1180, 1199, 1218, 1238,
1257, 1276, 1295, 1315,
1334, 1353, 1372, 1392,
1411, 1430, 1449, 1469
};

const Ipp32s p_bits_mps[64]={
 256,  238,  221,  206,
 192,  180,  168,  157,
 148,  139,  130,  122,
 115,  108,  102,   96,
  90,   85,   80,   76,
  72,   68,   64,   60,
  57,   54,   51,   48,
  46,   43,   41,   39,
  37,   35,   33,   31,
  29,   28,   26,   25,
  24,   22,   21,   20,
  19,   18,   17,   16,
  15,   15,   14,   13,
  12,   12,   11,   11,
  10,   10,    9,    9,
   8,    8,    7,    7,
};
#endif

/* Average from H.264 standard range table */
//extern const Ipp32s p_bits[128];
#if 0
static const Ipp32s p_bits[128]={
//MPS bits
 246,  232,  220,  206,
 192,  179,  167,  157,
 148,  138,  129,  122,
 115,  108,  102,   96,
  90,   85,   80,   75,
  72,   67,   64,   60,
  57,   54,   51,   48,
  45,   43,   41,   39,
  36,   35,   33,   31,
  30,   28,   26,   25,
  24,   23,   21,   20,
  19,   18,   17,   16,
  15,   15,   14,   13,
  13,   12,   11,   11,
  10,    9,    9,    9,
   8,    8,    7,    2,
//LPS bits
 266,  282,  296,  314,
 334,  353,  373,  391,
 410,  430,  450,  468,
 488,  507,  526,  545,
 566,  584,  603,  624,
 641,  662,  679,  700,
 720,  738,  757,  776,
 796,  813,  835,  852,
 874,  893,  912,  933,
 948,  967,  987, 1008,
1024, 1044, 1065, 1087,
1106, 1120, 1141, 1164,
1180, 1198, 1216, 1237,
1249, 1280, 1294, 1313,
1338, 1358, 1366, 1385,
1409, 1430, 1451, 1928
};
#endif
#if 0
/* Average overall input range */
const Ipp32s p_bits_mps[64]={
 247,  233,  221,  207,
 193,  180,  168,  158,
 148,  139,  130,  123,
 115,  108,  102,   96,
  90,   85,   81,   76,
  72,   68,   64,   60,
  57,   54,   51,   48,
  46,   44,   41,   39,
  37,   35,   33,   31,
  30,   28,   27,   25,
  24,   23,   21,   20,
  19,   18,   17,   16,
  16,   15,   14,   13,
  13,   12,   11,   11,
  10,   10,    9,    9,
   8,    8,    7,    2
};
#endif

#define MAX_COST ((((Ipp64u)1)<<63)-1)

typedef struct _node{
     Ipp64u cost;
     Ipp16s coeff_id;
     Ipp8u cabac_states_alevelm1[10];
} node;

typedef struct _node8x8{
     Ipp64u cost;
     Ipp16s coeff_id;
     Ipp8u cabac_states_alevelm1[10];
} node8x8;

typedef struct _coeff_n{
    Ipp16s coeff;
    Ipp16s prev_coeff;
} coeff_n;

#define P_BITS


#if 1
const Ipp32s lambda_t[2][52] = {
{
   19,    24,    30,    38,    48,    60,    76,    96,
  121,   152,   192,   242,   306,   386,   486,   614,
  774,   976,  1232,  1554,  1962,  2475,  3124,  3943,
 4977,  6283,  7932, 10015, 12645, 15968, 20165, 25466,
32164, 40627, 51320, 64831, 81906, 103485, 130757, 165229,
208802, 263884, 333519, 421557, 532869, 673615, 851590, 1076656,
1361289, 1721276, 2176591, 2752512,
},
{
/*   11,    14,    18,    22,    28,    35,    45,    56,
   71,    89,   113,   142,   180,   227,   286,   361,
  455,   574,   725,   914,  1154,  1456,  1838,  2319,
 2928,  3696,  4666,  5891,  7438,  9393, 11862, 14980,
18920, 23898, 30188, 38136, 48180, 60874, 76916, 97194,
122825, 155226, 196188, 247975, 313452, 396244, 500935, 633327,
800758, 1012515, 1280348, 1619125,
*/

   12,    16,    19,    24,    31,    39,    49,    62,
   78,    99,   124,   157,   198,   250,   315,   398,
  502,   634,   801,  1011,  1277,  1613,  2037,  2574,
 3252,  4109,  5193,  6563,  8296, 10487, 13259, 16764,
21199, 26809, 33908, 42890, 54258, 68646, 86856, 109908,
139091, 176039, 222823, 282064, 357088, 452105, 572454, 724900,
918019, 1162679, 1472662, 1865436,

/*   13,    17,    21,    26,    33,    42,    53,    67,
   84,   106,   134,   169,   213,   269,   339,   428,
  541,   683,   862,  1089,  1375,  1737,  2194,  2772,
 3502,  4425,  5592,  7068,  8934, 11294, 14278, 18053,
22829, 28871, 36516, 46189, 58432, 73926, 93537, 118362,
149791, 189581, 239963, 303762, 384556, 486882, 616488, 780662,
988635, 1252116, 1585944, 2008931,
*/
/*   17,    22,    27,    34,    43,    55,    69,    87,
  109,   138,   174,   220,   277,   350,   441,   557,
  703,   888,  1121,  1416,  1788,  2258,  2852,  3603,
 4553,  5753,  7270,  9188, 11614, 14682, 18562, 23469,
29678, 37532, 47471, 60046, 75961, 96104, 121598, 153871,
194728, 246455, 311952, 394890, 499923, 632947, 801435, 1014860,
1285226, 1627751, 2061727, 2611610,
*/
/*   15,    20,    25,    31,    39,    50,    63,    79,
   99,   125,   158,   200,   252,   318,   401,   506,
  639,   807,  1019,  1287,  1625,  2053,  2593,  3275,
 4139,  5230,  6609,  8353, 10558, 13347, 16875, 21335,
26980, 34120, 43155, 54587, 69055, 87367, 110544, 139883,
177025, 224050, 283593, 358991, 454475, 575406, 728577, 922600,
1168387, 1479774, 1874297, 2374191,
*/
}
};
#endif

static __ALIGN16 const Ipp8u enc_scan[2][16] = {
    {0,1,5,6,2,4,7,12,3,8,11,13,9,10,14,15},
    {0,2,8,12,1,5,9,13,3,6,10,14,4,7,11,15}
};

static __ALIGN16 const Ipp16s offset_dequant[4][16]={
    { 128, 160, 128, 160, 160, 200, 160, 200, 128, 160, 128, 160, 160, 200, 160, 200, },
    { 64, 80, 64, 80, 80, 100, 80, 100, 64, 80, 64, 80, 80, 100, 80, 100, },
    { 32, 40, 32, 40, 40, 50, 40, 50, 32, 40, 32, 40, 40, 50, 40, 50, },
    { 16, 20, 16, 20, 20, 25, 20, 25, 16, 20, 16, 20, 20, 25, 20, 25, }
};

IppStatus TransformQuantOptFwd4x4_H264_16s_C1(
    Ipp16s* pSrc,
    Ipp16s* pDst,
    Ipp32s  Qp,
    Ipp32s* pNumLevels,
    Ipp32s  Intra,
    Ipp16s* pScanMatrix,
    Ipp32s* pLastCoeff,
    Ipp16s* pScaleLevels,
    const H264Slice<Ipp16s, Ipp8u>* curr_slice,
    Ipp32s sCoeff,
    CabacStates* cbSt)
{
    pScaleLevels = pScaleLevels;
    pScanMatrix = pScanMatrix;
    Ipp32s i;
    __ALIGN16 Ipp16s t[16];
    const Ipp32s isField = curr_slice->m_is_cur_mb_field;
    const Ipp32u lambda = lambda_t[Intra][Qp];
//    if( Intra ) lambda = lambda_t[Intra][Qp]*1.2;
//    else lambda = lambda_t[Intra][Qp];

    //Ipp64u lambda = 400*curr_slice->m_cur_mb.lambda;
    const Ipp32s* dec_scan = dec_single_scan[isField];
#ifdef TRANS4x4_OPT
    const Ipp8u*  escan = enc_scan[isField];
#endif
    const Ipp32s qp_rem = h264_qp_rem[Qp];
    const Ipp32s qp6 = h264_qp6[Qp];
    const Ipp16s* quantTbl = FwdQuantTable_16s[qp_rem];
    const Ipp16s* invQuantTbl = InvScale4x4[qp_rem];
    const Ipp32s scale = 15+qp6;
    const Ipp32s scaleOffset = 1<<scale;
    Ipp32s shift_dq;
    Ipp32s offset_d = 0;
    __ALIGN16 Ipp16s sign[16];

    if( Qp >= 24 ){
        shift_dq = qp6 - 4;
    }else{
        shift_dq = 4 - qp6;
        offset_d = (1<<(shift_dq-1));
    }

#ifdef TRANS4x4_OPT
#if 0
    __ALIGN16 __m128i _p_0, _p_1, _p_2, _p_3, _p_4, _p_5, _p_6, _p_7;

    _p_0 = _mm_loadl_epi64((__m128i*)(&pSrc[0]));
    _p_1 = _mm_loadl_epi64((__m128i*)(&pSrc[4]));
    _p_2 = _mm_loadl_epi64((__m128i*)(&pSrc[8]));
    _p_3 = _mm_loadl_epi64((__m128i*)(&pSrc[12]));
//Vertical
    _p_0 = _mm_unpacklo_epi16( _p_0, _p_1 );
    _p_2 = _mm_unpacklo_epi16( _p_2, _p_3 );
    _p_1 = _p_0;
    _p_0 = _mm_unpacklo_epi32( _p_0, _p_2 );
    _p_1 = _mm_unpackhi_epi32( _p_1, _p_2 );

    _p_1 = _mm_shuffle_epi32( _p_1, 0x4e );
    _p_5 = _p_0;
    _p_0 = _mm_adds_epi16( _p_0, _p_1 );
    _p_5 = _mm_subs_epi16( _p_5, _p_1 );
    _p_6 = _p_0;
    _p_1 = _p_0;
    _p_0 = _mm_srli_si128( _p_0, 8);
    _p_1 = _mm_subs_epi16( _p_1, _p_0 );
    _p_0 = _mm_adds_epi16( _p_0, _p_6 );
    _p_2 = _p_5;
    _p_2 = _mm_slli_epi16( _p_2, 1 );
    _p_2 = _mm_shuffle_epi32( _p_2, 0x4e );
    _p_3 = _p_2;
    _p_2 = _mm_adds_epi16( _p_2, _p_5 );
    _p_2 = _mm_srli_si128( _p_2, 8);
    _p_5 = _mm_subs_epi16( _p_5, _p_3 );
//Horizontal
    _p_0 = _mm_unpacklo_epi16( _p_0, _p_2 );
    _p_1 = _mm_unpacklo_epi16( _p_1, _p_5 );
    _p_2 = _p_0;
    _p_0 = _mm_unpacklo_epi32( _p_0, _p_1 );
    _p_2 = _mm_unpackhi_epi32( _p_2, _p_1 );

    _p_2 = _mm_shuffle_epi32( _p_2, 0x4e );
    _p_5 = _p_0;
    _p_0 = _mm_adds_epi16( _p_0, _p_2 );
    _p_5 = _mm_subs_epi16( _p_5, _p_2 );
    _p_6 = _p_0;
    _p_1 = _p_0;
    _p_0 = _mm_srli_si128( _p_0, 8);
    _p_1 = _mm_subs_epi16( _p_1, _p_0 ); //result 2 _p_1
    _p_0 = _mm_adds_epi16( _p_0, _p_6 ); //result 0 _p_0
    _p_2 = _p_5;
    _p_2 = _mm_slli_epi16( _p_2, 1 );
    _p_2 = _mm_shuffle_epi32( _p_2, 0x4e );
    _p_3 = _p_2;
    _p_2 = _mm_adds_epi16( _p_2, _p_5 );
    _p_2 = _mm_srli_si128( _p_2, 8);      //result 1 _p_2
    _p_5 = _mm_subs_epi16( _p_5, _p_3 );     //result 3 _p_5
    _p_0 = _mm_unpacklo_epi64(_p_0,_p_2);
    _p_1 = _mm_unpacklo_epi64(_p_1,_p_5);
    _mm_store_si128((__m128i*)&t[0], _p_0 );
    _mm_store_si128((__m128i*)&t[8], _p_1 );
#else
//Alternative transform
//Matrix pre mult
    __ALIGN16 __m128i p0, p1, p2, p3, p4, p5, p6, p7;

    p0 = _mm_load_si128((__m128i*)(&pSrc[0]));
    p1 = _mm_load_si128((__m128i*)(&pSrc[8]));
    p1 = _mm_shuffle_epi32( p1, 0x4e );
    p2 = p0;
    p0 = _mm_adds_epi16( p0, p1 );
    p2 = _mm_subs_epi16( p2, p1 );
    p1 = _mm_shuffle_epi32( p0, 0x4e );
    p3 = _mm_shuffle_epi32( p2, 0x4e );

    p4 = _mm_adds_epi16( p0, p1 );
    p5 = _mm_adds_epi16( p3, p2 );
    p6 = _mm_subs_epi16( p0, p1 );
    p7 = _mm_subs_epi16( p2, p3 );
    p5 = _mm_adds_epi16( p5, p2 );
    p7 = _mm_subs_epi16( p7, p3 );
//Transpose matrix
    p4 = _mm_unpacklo_epi16( p4, p5 );
    p6 = _mm_unpacklo_epi16( p6, p7 );
    p0 = p4;
    p4 = _mm_unpacklo_epi32( p4, p6 );
    p0 = _mm_unpackhi_epi32( p0, p6 );
//Matrix post mult
    p0 = _mm_shuffle_epi32( p0, 0x4e );
    p2 = p4;
    p4 = _mm_adds_epi16( p4, p0 );
    p2 = _mm_subs_epi16( p2, p0 );
    p0 = _mm_shuffle_epi32( p4, 0x4e );
    p3 = _mm_shuffle_epi32( p2, 0x4e );

    p6 = _mm_subs_epi16( p4, p0 );
    p5 = _mm_adds_epi16( p3, p2 );
    p4 = _mm_adds_epi16( p4, p0 );
    p7 = _mm_subs_epi16( p2, p3 );
    p5 = _mm_adds_epi16( p5, p2 );
    p7 = _mm_subs_epi16( p7, p3 );
//Transpose matrix
    p4 = _mm_unpacklo_epi16( p4, p5 );
    p6 = _mm_unpacklo_epi16( p6, p7 );
    p0 = p4;
    p4 = _mm_unpacklo_epi32( p4, p6 );
    p0 = _mm_unpackhi_epi32( p0, p6 );

//    _mm_store_si128((__m128i*)&t[0], p4 );
//    _mm_store_si128((__m128i*)&t[8], p0 );
/*
//Count number non-zeros
    p3 = _mm_xor_si128(p3,p3);
    p6 = _mm_xor_si128(p6,p6);
    p5 = p4;
    p5 = _mm_packs_epi16(p5,p0);
    p3 = _mm_cmpeq_epi8(p3,p5);
    p6 = _mm_sad_epu8( p6, p3 );
    p5 = _mm_shuffle_epi32( p6, 0x0e );
    p6 = _mm_add_epi32( p6,p5 );
*/

//Store signs
    p1 = _mm_xor_si128(p1,p1);
    p2 = _mm_xor_si128(p2,p2);
    p1 = _mm_cmpgt_epi16(p1,p4);
    p2 = _mm_cmpgt_epi16(p2,p0);
    _mm_store_si128((__m128i*)&sign[0], p1 );
    _mm_store_si128((__m128i*)&sign[8], p2 );
//Make abs tranform
    p4 = _mm_adds_epi16(p4,p1);
    p4 = _mm_xor_si128(p4,p1);
    p0 = _mm_adds_epi16(p0,p2);
    p0 = _mm_xor_si128(p0,p2);
    _mm_store_si128((__m128i*)&t[0], p4 );
    _mm_store_si128((__m128i*)&t[8], p0 );
//Quant
    p6 = _mm_load_si128((__m128i*)&quantTbl[0]);
    p7 = _mm_load_si128((__m128i*)&quantTbl[8]);
    p3 = p4;
    p5 = p0;

    p3 = _mm_mullo_epi16(p3,p6);
    p5 = _mm_mullo_epi16(p5,p7);
    p4 = _mm_mulhi_epu16(p4,p6);
    p0 = _mm_mulhi_epu16(p0,p7);

    p6 = _mm_cvtsi32_si128(scaleOffset);
    p7 = _mm_cvtsi32_si128(scale);
    p2 = p3;
    p1 = p5;
    p2 = _mm_unpacklo_epi16( p2, p4 );
    p3 = _mm_unpackhi_epi16( p3, p4 );
    p1 = _mm_unpacklo_epi16( p1, p0 );
    p5 = _mm_unpackhi_epi16( p5, p0 );
//Add scale
    p6 = _mm_shuffle_epi32(p6, 0);
    p2 = _mm_add_epi32(p2,p6);
    p3 = _mm_add_epi32(p3,p6);
    p1 = _mm_add_epi32(p1,p6);
    p5 = _mm_add_epi32(p5,p6);

    p2 = _mm_sra_epi32(p2,p7);
    p3 = _mm_sra_epi32(p3,p7);
    p1 = _mm_sra_epi32(p1,p7);
    p5 = _mm_sra_epi32(p5,p7);

    p2 = _mm_packs_epi32(p2,p3);
    p1 = _mm_packs_epi32(p1,p5);

    _mm_store_si128((__m128i*)&pDst[0], p2 );
    _mm_store_si128((__m128i*)&pDst[8], p1 );
//Get last
    p3 = _mm_xor_si128(p3,p3);
    p5 = p2;
    p5 = _mm_packs_epi16(p5,p1);
    p3 = _mm_cmpeq_epi8(p3,p5);
    //load scan matrix
    p5 = _mm_load_si128((__m128i*)escan);
    p3 = _mm_andnot_si128(p3,p5);
    p5 = _mm_shuffle_epi32( p3, 0x4e ); //could be shift
    p3 = _mm_max_epu8(p3,p5);
    p5 = _mm_shufflelo_epi16( p3, 0x4e ); //could be shift
    p3 = _mm_max_epu8(p3,p5);
    p5 = p3;
    p5 = _mm_srli_epi32( p5, 16);
    p3 = _mm_max_epu8(p3,p5);
    p5 = p3;
    p5 = _mm_srli_epi16( p5, 8);
    p3 = _mm_max_epu8(p3,p5);
    *pLastCoeff =  _mm_cvtsi128_si32(p3) & 0xff;
//Dequant
/*
            Ipp32s d = (tcoeff<<6) + 32; *64
            if( Qp >= 24 )
                d = (d - (c*invQuantTbl[pos]<<shift_dq)*CorrMatrix[pos])>>6;
            else
                d = (d - ((c*invQuantTbl[pos]+offset_dq)>>shift_dq)*CorrMatrix[pos])>>6;
*/
/*
    if( Qp >= 24 ){
         p6 = _mm_load_si128((__m128i*)&quantTbl[0]);
         p7 = _mm_load_si128((__m128i*)&quantTbl[8]);
    p3 = p4;
    p5 = p0;

    p3 = _mm_mullo_epi16(p3,p6);
    p5 = _mm_mullo_epi16(p5,p7);
    p4 = _mm_mulhi_epu16(p4,p6);
    p0 = _mm_mulhi_epu16(p0,p7);

    p6 = _mm_cvtsi32_si128(scaleOffset);
    p7 = _mm_cvtsi32_si128(scale);
    p2 = p3;
    p1 = p5;
    p2 = _mm_unpacklo_epi16( p2, p4 );
    p3 = _mm_unpackhi_epi16( p3, p4 );
    p1 = _mm_unpacklo_epi16( p1, p0 );
    p5 = _mm_unpackhi_epi16( p5, p0 );
//Add scale
    p6 = _mm_shuffle_epi32(p6, 0);
    p2 = _mm_add_epi32(p2,p6);
    p3 = _mm_add_epi32(p3,p6);
    p1 = _mm_add_epi32(p1,p6);
    p5 = _mm_add_epi32(p5,p6);

    p2 = _mm_sra_epi32(p2,p7);
    p3 = _mm_sra_epi32(p3,p7);
    p1 = _mm_sra_epi32(p1,p7);
    p5 = _mm_sra_epi32(p5,p7);

    p2 = _mm_packs_epi32(p2,p3);
    p1 = _mm_packs_epi32(p1,p5);

    _mm_store_si128((__m128i*)&pDst[0], p2 );
    _mm_store_si128((__m128i*)&pDst[8], p1 );

    }else{
    }
*/
//    *pLastCoeff = -1;
//    for( i = sCoeff; i<16; i++ ){
//        if(pDst[i] != 0 && *pLastCoeff < pScanMatrix[i] ) *pLastCoeff = pScanMatrix[i];
//    }
#endif
#else
    transform4x4(pSrc, t);
    *pLastCoeff = -1;
    for( i = sCoeff; i<16; i++ ){
        sign[i] = t[i] < 0 ? -1:1;
        pDst[i] = (Ipp16s)((sign[i]*t[i]*quantTbl[i]+scaleOffset)>>scale);
        if(pDst[i] != 0 && *pLastCoeff < pScanMatrix[i] ) *pLastCoeff = pScanMatrix[i];
    }
#endif
    if( *pLastCoeff < sCoeff ){
        *pNumLevels = *pLastCoeff = 0;
        return ippStsNoErr;
    }

    Ipp32s ctx_id,c;

    __ALIGN16 node  layer[2][8];
    node* layer_next = &layer[0][0];
    node* layer_cur = &layer[1][0];

    coeff_n coeffs[16*8*2];
    Ipp32s  last_coeff=0;

    //Init first layer
    layer_next[0].cost = 0;
    layer_next[0].coeff_id = 0;
    coeffs[0].coeff = 0;
    coeffs[0].prev_coeff = 0; //loop to itself
    last_coeff = 1;
    for(ctx_id=1;ctx_id<8; ctx_id++) layer_next[ctx_id].cost = MAX_COST;

    /* Copy CABAC contexts to 0 ctx_id (start ctx) */
    Ipp8u* cabac_sig, *cabac_last;
    //I4x4 contexts
    memcpy( layer_next[0].cabac_states_alevelm1, cbSt->absLevelM1, 10*sizeof(CABAC_CONTEXT));

//Precalculate cost of last and significant
    cabac_sig = cbSt->sig;
    cabac_last = cbSt->last;

    for (i=*pLastCoeff; i>=sCoeff; i--){
        Ipp32s pos = dec_scan[i];
        Ipp32s coeff = pDst[pos];
        Ipp32s state_sig = cabac_sig[i];
        Ipp32s state_last = cabac_last[i];

        if( coeff == 0 ){  //Take into account only sig_flag
            Ipp64u cost = (lambda*p_bits[state_sig]+128)>>8;
            for( ctx_id = 1; ctx_id<8; ctx_id++ ){
                register node* n = &layer_next[ctx_id];
                if( n->cost != MAX_COST ){
                    n->cost += cost;
                    coeffs[last_coeff].coeff = 0;
                    coeffs[last_coeff].prev_coeff = n->coeff_id;
                    n->coeff_id = last_coeff;
                    last_coeff++;
                }
            }
            continue;
        }

#ifdef TRANS4x4_OPT
        Ipp32u tcoeff = t[pos];
#else
        Ipp32u tcoeff = t[pos];
        if( sign[pos] < 0 ) tcoeff = IPP_MAX_32U - tcoeff + 1;
#endif

        //Exchange layers
        node* layer_tmp = layer_cur;
        layer_cur = layer_next;
        layer_next = layer_tmp;
        //Set cost to MAX on next layer
        //for(ctx_id = 0; ctx_id<8; ctx_id++) layer_next[ctx_id].cost = MAX_COST;
        for(; layer_tmp<layer_next+8; layer_tmp++) layer_tmp->cost = MAX_COST;

        for( c = coeff; c >= coeff-1; c-- ){
            Ipp64s d = (tcoeff<<6) + 32; //*64
            if( Qp >= 24 )
                d = (d - (c*invQuantTbl[pos]<<shift_dq))>>6;
            else
                d = (d - ((c*invQuantTbl[pos]+offset_d)>>shift_dq))>>6;
            Ipp64u ssd = d*d*w[pos];

            if( c == 0 ){
              Ipp64u b=0;
              if( i != 15 ) b = ((lambda*p_bits[state_sig]+128)>>8);
              for( ctx_id = 0; ctx_id<8; ctx_id++ ){
                if( layer_cur[ctx_id].cost == MAX_COST ) continue;
                Ipp64u cost = layer_cur[ctx_id].cost+ssd;
                if( ctx_id != 0) cost += b;
                if( cost < layer_next[ctx_id].cost ){
                    Ipp32s id = layer_next[ctx_id].coeff_id;
                    if( layer_next[ctx_id].cost == MAX_COST ){
                        id = last_coeff;
                        last_coeff++;
                    }
                    layer_next[ctx_id] = layer_cur[ctx_id]; //copy states
                    layer_next[ctx_id].cost = cost;
                    layer_next[ctx_id].coeff_id = id;
                    coeffs[id].coeff = c;
                    coeffs[id].prev_coeff = layer_cur[ctx_id].coeff_id;
                }
              }
            }else if( c == 1){
                Ipp32u b[2]={0,0},bb=0;
                if( i != 15 ){
                    bb   =  p_bits[state_sig^64];
                    b[0] =  p_bits[state_last];
                    b[1] =  p_bits[state_last^64];
                }
                for( ctx_id = 0; ctx_id<8; ctx_id++ ){
                    if( layer_cur[ctx_id].cost == MAX_COST ) continue;
                    Ipp32s next_ctx_id = ctx_id_trans0[ctx_id];
                    Ipp32s ctx = ctx_neq1p1[ctx_id];
                    Ipp32u bits = bb+b[ctx_id==0];
                    Ipp8u ctx_val = *(layer_cur[ctx_id].cabac_states_alevelm1+ctx);
                    bits += 256 + p_bits[ctx_val]; //+1 bit for sign
                    ctx_val = transTbl[0][ctx_val];
                    Ipp64u cost = layer_cur[ctx_id].cost + ssd + ((lambda*bits+128)>>8);
                    if( cost < layer_next[next_ctx_id].cost ){
                       Ipp32s id = layer_next[next_ctx_id].coeff_id;
                       if( layer_next[next_ctx_id].cost == MAX_COST ){
                            id = last_coeff;
                            last_coeff++;
                        }
                        layer_next[next_ctx_id] = layer_cur[ctx_id]; //copy states
                        layer_next[next_ctx_id].cost = cost;
                        layer_next[next_ctx_id].coeff_id = id;
                        coeffs[id].coeff = c;
                        coeffs[id].prev_coeff = layer_cur[ctx_id].coeff_id;
                        *(layer_next[next_ctx_id].cabac_states_alevelm1 + ctx) = ctx_val;
                    }
                }
            }else{
                Ipp32u b[2]={0,0},bb=0;
                if( i != 15 ){
                    bb   =  p_bits[state_sig^64];
                    b[0] =  p_bits[state_last];
                    b[1] =  p_bits[state_last^64];
                }
                for( ctx_id = 0; ctx_id<8; ctx_id++ ){
                    if( layer_cur[ctx_id].cost == MAX_COST ) continue;
                    Ipp32s next_ctx_id = ctx_id_trans1[ctx_id];
                    Ipp8u st[4];
                    st[0] = ctx_neq1p1[ctx_id];
                    Ipp32u bits = bb+b[ctx_id==0];
                    st[1] = *(layer_cur[ctx_id].cabac_states_alevelm1+st[0]);
                    bits += 256 + p_bits[(st[1])^64]; //+1 bit for sign
                    st[1] = transTbl[1][st[1]];
                    st[2] = ctx_ngt1[ctx_id];
                    st[3] = *(layer_cur[ctx_id].cabac_states_alevelm1+st[2]);
                    Ipp32s code = c - 2;
                    if( code < 13 ){
                         bits +=  pref_bits[st[3]][code];
                         st[3] = pref_state[st[3]][code];
                    }else{
                         bits += pref_bits[st[3]][13];
                         st[3] = pref_state[st[3]][13];
                         if( code >= 65536-1+13 ){ bits += 32<<8; code >>= 16; }
                         if( code >= 256-1+13 ){ bits += 16<<8; code >>= 8; }
                         bits += bitcount_EG0[code];
                    }
                    Ipp64u cost = layer_cur[ctx_id].cost + ssd + ((lambda*bits+128)>>8);
                    if( cost < layer_next[next_ctx_id].cost ){
                       Ipp32s id = layer_next[next_ctx_id].coeff_id;
                       if( layer_next[next_ctx_id].cost == MAX_COST ){
                            id = last_coeff;
                            last_coeff++;
                        }
                        layer_next[next_ctx_id] = layer_cur[ctx_id]; //copy states
                        layer_next[next_ctx_id].cost = cost;
                        layer_next[next_ctx_id].coeff_id = id;
                        coeffs[id].coeff = c;
                        coeffs[id].prev_coeff = layer_cur[ctx_id].coeff_id;
                        *(layer_next[next_ctx_id].cabac_states_alevelm1 + st[0]) = st[1];
                        *(layer_next[next_ctx_id].cabac_states_alevelm1 + st[2]) = st[3];
                    }
                }
            }

        }
    }

    //Store new coeffs
    node *best=layer_next;
    node *cur;
    for( cur = layer_next+1; cur<layer_next+8; cur++ ){
        if( cur->cost < best->cost ) best=cur;
    }
    c = best->coeff_id;
    memcpy( cbSt->absLevelM1, best->cabac_states_alevelm1, 10*sizeof(CABAC_CONTEXT));
/*
    Ipp32s best_ctx = 0;
    Ipp64u best_cost = layer_next[0].cost;
    for( ctx_id = 1; ctx_id<8 ; ctx_id++ ){
        if( layer_next[ctx_id].cost < best_cost ){
            best_ctx = ctx_id;
            best_cost = layer_next[ctx_id].cost;
        }
    }
    c = layer_next[best_ctx].coeff_id;
    memcpy( cbSt->absLevelM1, layer_next[best_ctx].cabac_states_alevelm1, 10*sizeof(CABAC_CONTEXT));
*/
    Ipp32s last = *pLastCoeff;
    *pNumLevels = *pLastCoeff = 0;
    for (i=sCoeff; i<=last; i++){
        Ipp16s cf = coeffs[c].coeff;
        Ipp32s pos = dec_scan[i];
        if( cf != 0 ){
#ifndef TRANS4x4_OPT
            pDst[pos] = cf * sign[pos];
#else
            pDst[pos] = cf;
#endif
            (*pNumLevels)++;
            *pLastCoeff = i;
        }else{
            pDst[pos] = 0;
        }
        c = coeffs[c].prev_coeff;
    }

#ifdef TRANS4x4_OPT
//restore sign
    p0 = _mm_load_si128((__m128i*)&pDst[0]);
    p1 = _mm_load_si128((__m128i*)&pDst[8]);
    p2 = _mm_load_si128((__m128i*)&sign[0]);
    p3 = _mm_load_si128((__m128i*)&sign[8]);
    p0 = _mm_adds_epi16(p0,p2); //sign
    p0 = _mm_xor_si128(p0,p2);
    p1 = _mm_adds_epi16(p1,p3); //sign
    p1 = _mm_xor_si128(p1,p3);
    _mm_store_si128((__m128i*)&pDst[0], p0 );
    _mm_store_si128((__m128i*)&pDst[8], p1 );
#endif
#if 0
    for( i=sCoeff; i<=*pLastCoeff && i<15; i++ ){
            if( pDst[dec_scan[i]] != 0 ){
                cabac_sig[i] = transTbl[1][cabac_sig[i]];
                cabac_last[i] = transTbl[ i == *pLastCoeff ][cabac_last[i]];
            }else{
                cabac_sig[i] = transTbl[0][cabac_sig[i]];
            }
    }
#endif

    if (pDst[0] != 0){
        if( sCoeff ) (*pNumLevels)++;
        *pNumLevels = -*pNumLevels;
     }

    return (IppStatus)0;
}

Ipp16s quant8x8(Ipp16s coeff,Ipp32s pos, Ipp32s Qp6, Ipp32s intra, const Ipp16s* pScaleLevels)
{
    intra = intra;
    Ipp16s sign;
    Ipp32s scale;
    Ipp32s scaleOffset;

    scale = 12+Qp6;
//    scaleOffset = intra ? (682<<(1 + Qp6)) : (342<<(1 + Qp6));
//    scaleOffset = (1<<(scale-1)); //intra ? (1<<scale)/3 : (1<<scale)/6;
    scaleOffset = 1<<scale; //intra ? (1<<scale)/3 : (1<<scale)/6;

    sign = (Ipp16s)(coeff < 0 ? -1:1);
    return (Ipp16s)(sign*((sign*coeff*pScaleLevels[pos]+scaleOffset)>>scale));
}

//pos - non zig-zag scan pos (normal pos)
Ipp16s dequant8x8(Ipp16s coeff,Ipp32s pos, Ipp32s Qp6, const Ipp16s* pInvLevelScale)
{
    if( Qp6 >= 6 ){
        Ipp32s shift = Qp6 - 6;
        return  (coeff*pInvLevelScale[pos])<<shift;
    }else{
        Ipp32s shift = 6 - Qp6;
        Ipp32s offset = 1<<(shift - 1);
        return (coeff*pInvLevelScale[pos]+offset)>>shift;
    }
}

const Ipp32s CorrMatrix8x8[64] = {
       65536, 73984, 40960, 73984, 65536, 73984, 40960, 73984,
       73984, 83521, 46240, 83521, 73984, 83521, 46240, 83521,
       40960, 46240, 25600, 46240, 40960, 46240, 25600, 46240,
       73984, 83521, 46240, 83521, 73984, 83521, 46240, 83521,
       65536, 73984, 40960, 73984, 65536, 73984, 40960, 73984,
       73984, 83521, 46240, 83521, 73984, 83521, 46240, 83521,
       40960, 46240, 25600, 46240, 40960, 46240, 25600, 46240,
       73984, 83521, 46240, 83521, 73984, 83521, 46240, 83521
};

const Ipp32s w8x8[64] = {
    400, 354, 640, 354, 400, 354, 640, 354,
    354, 314, 567, 314, 354, 314, 567, 314,
    640, 567,1024, 567, 640, 567,1024, 567,
    354, 314, 567, 314, 354, 314, 567, 314,
    400, 354, 640, 354, 400, 354, 640, 354,
    354, 314, 567, 314, 354, 314, 567, 314,
    640, 567,1024, 567, 640, 567,1024, 567,
    354, 314, 567, 314, 354, 314, 567, 314
};

const Ipp32u lambda_t8x8[2][52] = {
{
   19,    24,    30,    38,    48,    61,    76,    96,
  121,   153,   193,   243,   306,   386,   486,   612,
  771,   971,  1224,  1542,  1943,  2448,  3084,  3982,
 5068,  6154,  7964,  9774, 12308, 15566, 19548, 24978,
31132, 39458, 49594, 62626, 78916, 99188, 124890, 157470,
198376, 250142, 314940, 397114, 500284, 630242, 793866, 1000206,
1260484, 1588094, 2000774, 2520606,
},{
   14,    17,    21,    27,    34,    43,    54,    68,
   86,   108,   136,   171,   216,   271,   342,   431,
  543,   684,   862,  1086,  1369,  1724,  2172,  2805,
 3570,  4335,  5610,  6885,  8670, 10965, 13770, 17595,
21930, 27795, 34935, 44115, 55590, 69870, 87975, 110925,
139740, 176205, 221850, 279735, 352410, 443955, 559215, 704565,
887910, 1118685, 1409385, 1775565,

/*   15,    18,    23,    29,    37,    46,    58,    73,
   92,   116,   146,   184,   232,   292,   368,   464,
  585,   737,   928,  1170,  1474,  1857,  2339,  3021,
 3845,  4668,  6042,  7415,  9337, 11808, 14829, 18948,
23617, 29933, 37622, 47508, 59866, 75245, 94742, 119458,
150489, 189759, 238915, 301253, 379518, 478105, 602232, 758762,
956211, 1204738, 1517799, 1912147,
*/
/*
   19,    24,    30,    38,    48,    60,    75,    95,
  120,   151,   190,   239,   302,   380,   479,   603,
  760,   958,  1207,  1521,  1916,  2414,  3041,  3927,
 4998,  6069,  7854,  9639, 12138, 15351, 19278, 24633,
30702, 38913, 48909, 61761, 77826, 97818, 123165, 155295,
195636, 246687, 310590, 391629, 493374, 621537, 782901, 986391,
1243074, 1566159, 1973139, 2485791,
*/
}
};

static __ALIGN16 const Ipp32s InvScale8x8[6][64] = {
{ 20971520, 22491136, 16384000, 22491136, 20971520, 22491136, 16384000, 22491136, 22491136, 24054048, 17756160, 24054048, 22491136, 24054048, 17756160, 24054048, 16384000, 17756160, 13107200, 17756160, 16384000, 17756160, 13107200, 17756160, 22491136, 24054048, 17756160, 24054048, 22491136, 24054048, 17756160, 24054048, 20971520, 22491136, 16384000, 22491136, 20971520, 22491136, 16384000, 22491136, 22491136, 24054048, 17756160, 24054048, 22491136, 24054048, 17756160, 24054048, 16384000, 17756160, 13107200, 17756160, 16384000, 17756160, 13107200, 17756160, 22491136, 24054048, 17756160, 24054048, 22491136, 24054048, 17756160, 24054048, },
{ 23068672, 24858624, 18350080, 24858624, 23068672, 24858624, 18350080, 24858624, 24858624, 25390384, 19235840, 25390384, 24858624, 25390384, 19235840, 25390384, 18350080, 19235840, 14336000, 19235840, 18350080, 19235840, 14336000, 19235840, 24858624, 25390384, 19235840, 25390384, 24858624, 25390384, 19235840, 25390384, 23068672, 24858624, 18350080, 24858624, 23068672, 24858624, 18350080, 24858624, 24858624, 25390384, 19235840, 25390384, 24858624, 25390384, 19235840, 25390384, 18350080, 19235840, 14336000, 19235840, 18350080, 19235840, 14336000, 19235840, 24858624, 25390384, 19235840, 25390384, 24858624, 25390384, 19235840, 25390384, },
{ 27262976, 28409856, 21626880, 28409856, 27262976, 28409856, 21626880, 28409856, 28409856, 30735728, 22935040, 30735728, 28409856, 30735728, 22935040, 30735728, 21626880, 22935040, 17203200, 22935040, 21626880, 22935040, 17203200, 22935040, 28409856, 30735728, 22935040, 30735728, 28409856, 30735728, 22935040, 30735728, 27262976, 28409856, 21626880, 28409856, 27262976, 28409856, 21626880, 28409856, 28409856, 30735728, 22935040, 30735728, 28409856, 30735728, 22935040, 30735728, 21626880, 22935040, 17203200, 22935040, 21626880, 22935040, 17203200, 22935040, 28409856, 30735728, 22935040, 30735728, 28409856, 30735728, 22935040, 30735728, },
{ 29360128, 30777344, 22937600, 30777344, 29360128, 30777344, 22937600, 30777344, 30777344, 33408400, 24414720, 33408400, 30777344, 33408400, 24414720, 33408400, 22937600, 24414720, 18432000, 24414720, 22937600, 24414720, 18432000, 24414720, 30777344, 33408400, 24414720, 33408400, 30777344, 33408400, 24414720, 33408400, 29360128, 30777344, 22937600, 30777344, 29360128, 30777344, 22937600, 30777344, 30777344, 33408400, 24414720, 33408400, 30777344, 33408400, 24414720, 33408400, 22937600, 24414720, 18432000, 24414720, 22937600, 24414720, 18432000, 24414720, 30777344, 33408400, 24414720, 33408400, 30777344, 33408400, 24414720, 33408400, },
{ 33554432, 35512320, 26214400, 35512320, 33554432, 35512320, 26214400, 35512320, 35512320, 37417408, 28113920, 37417408, 35512320, 37417408, 28113920, 37417408, 26214400, 28113920, 20889600, 28113920, 26214400, 28113920, 20889600, 28113920, 35512320, 37417408, 28113920, 37417408, 35512320, 37417408, 28113920, 37417408, 33554432, 35512320, 26214400, 35512320, 33554432, 35512320, 26214400, 35512320, 35512320, 37417408, 28113920, 37417408, 35512320, 37417408, 28113920, 37417408, 26214400, 28113920, 20889600, 28113920, 26214400, 28113920, 20889600, 28113920, 35512320, 37417408, 28113920, 37417408, 35512320, 37417408, 28113920, 37417408, },
{ 37748736, 40247296, 30146560, 40247296, 37748736, 40247296, 30146560, 40247296, 40247296, 42762752, 31813120, 42762752, 40247296, 42762752, 31813120, 42762752, 30146560, 31813120, 23756800, 31813120, 30146560, 31813120, 23756800, 31813120, 40247296, 42762752, 31813120, 42762752, 40247296, 42762752, 31813120, 42762752, 37748736, 40247296, 30146560, 40247296, 37748736, 40247296, 30146560, 40247296, 40247296, 42762752, 31813120, 42762752, 40247296, 42762752, 31813120, 42762752, 30146560, 31813120, 23756800, 31813120, 30146560, 31813120, 23756800, 31813120, 40247296, 42762752, 31813120, 42762752, 40247296, 42762752, 31813120, 42762752, },
};

static __ALIGN16 const Ipp32s offset_dequant8x8[6][64]={
{ 2097152, 2367488, 1310720, 2367488, 2097152, 2367488, 1310720, 2367488, 2367488, 2672672, 1479680, 2672672, 2367488, 2672672, 1479680, 2672672, 1310720, 1479680, 819200, 1479680, 1310720, 1479680, 819200, 1479680, 2367488, 2672672, 1479680, 2672672, 2367488, 2672672, 1479680, 2672672, 2097152, 2367488, 1310720, 2367488, 2097152, 2367488, 1310720, 2367488, 2367488, 2672672, 1479680, 2672672, 2367488, 2672672, 1479680, 2672672, 1310720, 1479680, 819200, 1479680, 1310720, 1479680, 819200, 1479680, 2367488, 2672672, 1479680, 2672672, 2367488, 2672672, 1479680, 2672672, },
{ 1048576, 1183744, 655360, 1183744, 1048576, 1183744, 655360, 1183744, 1183744, 1336336, 739840, 1336336, 1183744, 1336336, 739840, 1336336, 655360, 739840, 409600, 739840, 655360, 739840, 409600, 739840, 1183744, 1336336, 739840, 1336336, 1183744, 1336336, 739840, 1336336, 1048576, 1183744, 655360, 1183744, 1048576, 1183744, 655360, 1183744, 1183744, 1336336, 739840, 1336336, 1183744, 1336336, 739840, 1336336, 655360, 739840, 409600, 739840, 655360, 739840, 409600, 739840, 1183744, 1336336, 739840, 1336336, 1183744, 1336336, 739840, 1336336, },
{ 524288, 591872, 327680, 591872, 524288, 591872, 327680, 591872, 591872, 668168, 369920, 668168, 591872, 668168, 369920, 668168, 327680, 369920, 204800, 369920, 327680, 369920, 204800, 369920, 591872, 668168, 369920, 668168, 591872, 668168, 369920, 668168, 524288, 591872, 327680, 591872, 524288, 591872, 327680, 591872, 591872, 668168, 369920, 668168, 591872, 668168, 369920, 668168, 327680, 369920, 204800, 369920, 327680, 369920, 204800, 369920, 591872, 668168, 369920, 668168, 591872, 668168, 369920, 668168, },
{ 262144, 295936, 163840, 295936, 262144, 295936, 163840, 295936, 295936, 334084, 184960, 334084, 295936, 334084, 184960, 334084, 163840, 184960, 102400, 184960, 163840, 184960, 102400, 184960, 295936, 334084, 184960, 334084, 295936, 334084, 184960, 334084, 262144, 295936, 163840, 295936, 262144, 295936, 163840, 295936, 295936, 334084, 184960, 334084, 295936, 334084, 184960, 334084, 163840, 184960, 102400, 184960, 163840, 184960, 102400, 184960, 295936, 334084, 184960, 334084, 295936, 334084, 184960, 334084, },
{ 131072, 147968, 81920, 147968, 131072, 147968, 81920, 147968, 147968, 167042, 92480, 167042, 147968, 167042, 92480, 167042, 81920, 92480, 51200, 92480, 81920, 92480, 51200, 92480, 147968, 167042, 92480, 167042, 147968, 167042, 92480, 167042, 131072, 147968, 81920, 147968, 131072, 147968, 81920, 147968, 147968, 167042, 92480, 167042, 147968, 167042, 92480, 167042, 81920, 92480, 51200, 92480, 81920, 92480, 51200, 92480, 147968, 167042, 92480, 167042, 147968, 167042, 92480, 167042, },
{ 65536, 73984, 40960, 73984, 65536, 73984, 40960, 73984, 73984, 83521, 46240, 83521, 73984, 83521, 46240, 83521, 40960, 46240, 25600, 46240, 40960, 46240, 25600, 46240, 73984, 83521, 46240, 83521, 73984, 83521, 46240, 83521, 65536, 73984, 40960, 73984, 65536, 73984, 40960, 73984, 73984, 83521, 46240, 83521, 73984, 83521, 46240, 83521, 40960, 46240, 25600, 46240, 40960, 46240, 25600, 46240, 73984, 83521, 46240, 83521, 73984, 83521, 46240, 83521, },
};

void QuantOptLuma8x8_H264_16s_C1_8u16s(
    const Ipp16s* pSrc,
    Ipp16s* pDst,
    Ipp32s  Qp,
    Ipp32s  Intra,
    const Ipp16s* pScanMatrix,
    const Ipp16s* pScaleLevels,
    Ipp32s* pNumLevels,
    Ipp32s* pLastCoeff,
    const H264Slice<Ipp16s, Ipp8u>* curr_slice,
    CabacStates* cbSt,
    const Ipp16s* pInvLevelScale)
{
    pInvLevelScale = pInvLevelScale;
    __ALIGN16 Ipp16s qcoeff[64];
    Ipp32s i;
    Ipp32s isField = curr_slice->m_is_cur_mb_field;

    const Ipp32s Qp6 = h264_qp6[Qp];
//    Ipp64u lambda = 362*curr_slice->m_cur_mb.lambda;
    Ipp32u lambda = lambda_t8x8[Intra][Qp];
//    if(Intra) lambda = lambda_t8x8[0][Qp]/3;
//    else lambda = lambda_t8x8[Intra][Qp];
    const Ipp32s* dec_scan = dec_single_scan_8x8[isField];
    const Ipp32s scale = 12+Qp6;
    const Ipp32s scaleOffset = 1<<scale;
    Ipp16s sign[64];
    Ipp32s shift_dq;
    Ipp32s offset_d = 0;
    const Ipp32s* invQuantTbl = InvScale8x8[ h264_qp_rem[Qp] ];

    if( Qp6 >= 6 ){
        shift_dq = Qp6 - 6;
    }else{
        shift_dq = 6 - Qp6;
        offset_d = 1<<(shift_dq - 1);
     }

    *pLastCoeff = -1;
    //Save original data
    for( i = 0; i<64; i++ ){
        sign[i] = pSrc[i] < 0 ? -1:1;
        qcoeff[i] = (Ipp16s)((sign[i]*pSrc[i]*pScaleLevels[i]+scaleOffset)>>scale);
        if(qcoeff[i] != 0 && (*pLastCoeff < pScanMatrix[i]) ) *pLastCoeff = pScanMatrix[i];
    }

    if( *pLastCoeff < 0 ){
        *pNumLevels = *pLastCoeff = 0;
        memset( pDst, 0, 64*sizeof(Ipp16s) );
        return ;
    }

    Ipp32s ctx_id,c;

    node8x8 layer[2][8];
    node8x8* layer_next = &layer[0][0];
    node8x8* layer_cur = &layer[1][0];

    coeff_n coeffs[64*8*2];
    Ipp32s  last_coeff=0;

    //Init first layer
    layer_next[0].cost = 0;
    layer_next[0].coeff_id = 0;
    coeffs[0].coeff = 0;
    coeffs[0].prev_coeff = 0; //loop to itself
    last_coeff = 1;
    for(ctx_id=1;ctx_id<8; ctx_id++) layer_next[ctx_id].cost = MAX_COST;

    /* Copy CABAC contexts to 0 ctx_id (start ctx) */
    //I8x8 contexts
    memcpy( layer_next[0].cabac_states_alevelm1, cbSt->absLevelM1, 10*sizeof(CABAC_CONTEXT));
    //Copy contexts for significant and last

    Ipp8u cabac_sig[64],cabac_last[64];
    for( i = 0; i<=*pLastCoeff; i++ ){
        cabac_sig[i] = cbSt->sig[Table_9_34[isField][i]];
        cabac_last[i] = cbSt->last[Table_9_34[2][i]];
    }

    for (i=*pLastCoeff; i>=0 ; i--){
        Ipp32s pos = dec_scan[i];
        Ipp32s coeff = qcoeff[pos];
        Ipp32s state_sig = cabac_sig[i];
        Ipp32s state_last = cabac_last[i];

        if( coeff == 0 ){  //Take into account only sig_flag
            Ipp64u cost = (lambda*p_bits[state_sig]+128)>>8;
            for( ctx_id = 1; ctx_id<8; ctx_id++ ){
                register node8x8* n = &layer_next[ctx_id];
                if( n->cost != MAX_COST ){
                    n->cost += cost;
                    coeffs[last_coeff].coeff = 0;
                    coeffs[last_coeff].prev_coeff = n->coeff_id;
                    n->coeff_id = last_coeff;
                    last_coeff++;
                }
            }
            continue;
        }

        Ipp32u tcoeff;
        if( sign[pos] < 0 )
            tcoeff = -pSrc[pos];
        else
            tcoeff = pSrc[pos];

        //Exchange layers
        node8x8* layer_tmp = layer_cur;
        layer_cur = layer_next;
        layer_next = layer_tmp;
        //Set cost to MAX on next layer
        for(; layer_tmp<layer_next+8; layer_tmp++) layer_tmp->cost = MAX_COST;

        for( c = coeff; c >= coeff-1; c-- ){
            Ipp64s d = (tcoeff<<6) + 32;
            if( Qp6 >= 6 )
                d =  ( d - ((((c*invQuantTbl[pos])<<shift_dq) + 512)>>10) )>>6;
            else
                d =  ( d - ((((c*invQuantTbl[pos]+offset_d)>>shift_dq) + 512)>>10) )>>6;

            Ipp64u ssd = (d*d*w8x8[pos] + 32)>>6;

            if( c == 0 ){
              Ipp64u b=0;
              if( i != 63 ) b = ((lambda*p_bits[state_sig]+128)>>8);
              for( ctx_id = 0; ctx_id<8; ctx_id++ ){
                if( layer_cur[ctx_id].cost == MAX_COST ) continue;
                Ipp64u cost = layer_cur[ctx_id].cost+ssd;
                if( ctx_id != 0) cost += b;
                if( cost < layer_next[ctx_id].cost ){
                    Ipp32s id = layer_next[ctx_id].coeff_id;
                    if( layer_next[ctx_id].cost == MAX_COST ){
                        id = last_coeff;
                        last_coeff++;
                    }
                    layer_next[ctx_id] = layer_cur[ctx_id]; //copy states
                    layer_next[ctx_id].cost = cost;
                    layer_next[ctx_id].coeff_id = id;
                    coeffs[id].coeff = c;
                    coeffs[id].prev_coeff = layer_cur[ctx_id].coeff_id;
                }
              }
            }else if( c == 1){
                Ipp32u b[2]={0,0},bb=0;
                if( i != 63 ){
                    bb   =  p_bits[state_sig^64];
                    b[0] =  p_bits[state_last];
                    b[1] =  p_bits[state_last^64];
                }
                for( ctx_id = 0; ctx_id<8; ctx_id++ ){
                    if( layer_cur[ctx_id].cost == MAX_COST ) continue;
                    Ipp32s next_ctx_id = ctx_id_trans0[ctx_id];
                    Ipp32u bits = bb+b[ctx_id==0];

                    Ipp32s ctx = ctx_neq1p1[ctx_id];
                    Ipp8u ctx_val = *(layer_cur[ctx_id].cabac_states_alevelm1+ctx);
                    bits += 256 + p_bits[ctx_val]; //+1 bit for sign
                    ctx_val = transTbl[0][ctx_val];
                    Ipp64u cost = layer_cur[ctx_id].cost + ssd + ((lambda*bits+128)>>8);
                    if( cost < layer_next[next_ctx_id].cost ){
                       Ipp32s id = layer_next[next_ctx_id].coeff_id;
                       if( layer_next[next_ctx_id].cost == MAX_COST ){
                            id = last_coeff;
                            last_coeff++;
                        }
                        layer_next[next_ctx_id] = layer_cur[ctx_id]; //copy states
                        layer_next[next_ctx_id].cost = cost;
                        layer_next[next_ctx_id].coeff_id = id;
                        coeffs[id].coeff = c;
                        coeffs[id].prev_coeff = layer_cur[ctx_id].coeff_id;
                        *(layer_next[next_ctx_id].cabac_states_alevelm1 + ctx) = ctx_val;
                    }
                }
            }else{
                Ipp32u b[2]={0,0},bb=0;
                if( i != 63 ){
                    bb   =  p_bits[state_sig^64];
                    b[0] =  p_bits[state_last];
                    b[1] =  p_bits[state_last^64];
                }
                for( ctx_id = 0; ctx_id<8; ctx_id++ ){
                    if( layer_cur[ctx_id].cost == MAX_COST ) continue;
                    Ipp32s next_ctx_id = ctx_id_trans1[ctx_id];
                    Ipp32u bits = bb+b[ctx_id==0];
                    Ipp8u st[4];

                    st[0] = ctx_neq1p1[ctx_id];
                    st[1] = *(layer_cur[ctx_id].cabac_states_alevelm1+st[0]);
                    bits += 256 + p_bits[(st[1])^64]; //+1 bit for sign
                    st[1] = transTbl[1][st[1]];
                    st[2] = ctx_ngt1[ctx_id];
                    st[3] = *(layer_cur[ctx_id].cabac_states_alevelm1+st[2]);
                    Ipp32s code = c - 2;
                    if( code < 13 ){
                         bits +=  pref_bits[st[3]][code];
                         st[3] = pref_state[st[3]][code];
                    }else{
                         bits += pref_bits[st[3]][13];
                         st[3] = pref_state[st[3]][13];
                         if( code >= 65536-1+13 ){ bits += 32<<8; code >>= 16; }
                         if( code >= 256-1+13 ){ bits += 16<<8; code >>= 8; }
                         bits += bitcount_EG0[code];
                    }
                    Ipp64u cost = layer_cur[ctx_id].cost + ssd + ((lambda*bits+128)>>8);
                    if( cost < layer_next[next_ctx_id].cost ){
                       Ipp32s id = layer_next[next_ctx_id].coeff_id;
                       if( layer_next[next_ctx_id].cost == MAX_COST ){
                            id = last_coeff;
                            last_coeff++;
                        }
                        layer_next[next_ctx_id] = layer_cur[ctx_id]; //copy states
                        layer_next[next_ctx_id].cost = cost;
                        layer_next[next_ctx_id].coeff_id = id;
                        coeffs[id].coeff = c;
                        coeffs[id].prev_coeff = layer_cur[ctx_id].coeff_id;
                        *(layer_next[next_ctx_id].cabac_states_alevelm1 + st[0]) = st[1];
                        *(layer_next[next_ctx_id].cabac_states_alevelm1 + st[2]) = st[3];
                    }
                }
            }

        }
    }

    //Store new coeffs
    node8x8 *best=layer_next;
    node8x8 *cur;
    for( cur = layer_next+1; cur<layer_next+8; cur++ ){
        if( cur->cost < best->cost ) best=cur;
    }
    c = best->coeff_id;

#if 0
    //cbSt->codRange = layer_next[best_ctx].codRange;
    memcpy( cbSt->absLevelM1, layer_next[best_ctx].cabac_states_alevelm1, 10*sizeof(CABAC_CONTEXT));
    memcpy( cbSt->sig, cabac_sig, 15*sizeof(CABAC_CONTEXT));
    memcpy( cbSt->last, cabac_last, 15*sizeof(CABAC_CONTEXT));
#endif

    Ipp32s last = *pLastCoeff;
    *pNumLevels = *pLastCoeff = 0;
    for (i=0; i<=last; i++){
        Ipp16s cf = coeffs[c].coeff;
        Ipp32s pos = dec_scan[i];
        if( cf != 0 ){
            pDst[pos] = cf * sign[pos];
//            pDst[pos] = cf;
            (*pNumLevels)++;
            *pLastCoeff = i;
        }else{
            pDst[pos] = 0;
        }
        c = coeffs[c].prev_coeff;
    }

    if (pDst[0] != 0)
        *pNumLevels = -*pNumLevels;
}

#define PRINT 0

static Ipp32s chromaPredInc[3][16] = {
     { 4, 60, 4,   0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }, //pitch 16
     { 4, 60, 4,  60,  4, 60, 4,  0,  0,  0,  0,  0,  0,  0,  0,  0  }, //pitch 16
     { 4, 60, 4, -60,  4, 60, 4, 52,  4, 60,  4,-60,  4, 60,  4,  0  } //pitch 16
};

static Ipp32s chromaDCOffset[3][16] = {
    { 0, 1, 2, 3, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0 },
    { 0, 1, 2, 3, 4, 5, 6, 7, 0, 0, 0 , 0,  0,  0,  0,  0 },
    { 0, 1, 4, 5, 2, 3, 6, 7, 8, 9, 12, 13, 10, 11, 14, 15}
};

inline void ownCalcNonZero(const Ipp16s *coeffs, Ipp32s  m, Ipp32s *numCoeffs)
{
    Ipp32s n = 0;
    for (Ipp32s i = 0; i < m; i ++) {
        if (coeffs[i] != 0)
            n ++;
    }
    *numCoeffs = (coeffs[0] != 0)? -n: n;
}

inline void ownCalcNonZero(const Ipp32s *coeffs, Ipp32s  m, Ipp32s *numCoeffs)
{
    Ipp32s n = 0;
    for (Ipp32s i = 0; i < m; i ++) {
        if (coeffs[i] != 0)
            n ++;
    }
    *numCoeffs = (coeffs[0] != 0)? -n: n;
}

inline void ownCountCoeffs(const Ipp32s *coeffs, Ipp32s *numCoeffs, const Ipp16s *enc_scan, Ipp32s *lastCoeff, Ipp32s  m)
{
    // m can take the following values: 16, 64.
    Ipp32s l = 0;
    Ipp32s n = 0;
    for(Ipp32s i = 0; i < m; i++) {
        if (coeffs[i] != 0) {
            n ++;
            if (l < enc_scan[i])
                l = enc_scan[i];
        }
    }

    *numCoeffs = (coeffs[0] != 0)? -n: n;
    *lastCoeff = l;
}

inline void ownCountCoeffs(const Ipp16s *coeffs, Ipp32s *numCoeffs, const Ipp16s *enc_scan, Ipp32s *lastCoeff, Ipp32s  m)
{
    // m can take the following values: 16, 64.
    Ipp32s l = 0;
    Ipp32s n = 0;
    for(Ipp32s i = 0; i < m; i++) {
        if (coeffs[i] != 0) {
            n ++;
            if (l < enc_scan[i])
                l = enc_scan[i];
        }
    }
    *numCoeffs = (coeffs[0] != 0)? -n: n;
    *lastCoeff = l;
}






//////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// CEncAndRec4x4IntraBlock
//
// Encode and Reconstruct one blocks in an Intra macroblock with 4x4 prediction
//
////////////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_Encode4x4IntraBlock(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, Ipp32s block)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s      iNumCoeffs = 0;
    Ipp32s      iLastCoeff = 0;
    __ALIGN16 Ipp16s pDiffBuf[16];
    COEFFSTYPE*  pTransformResult;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32u uMBQP       = cur_mb.lumaQP;
    Ipp32s pitchPixels = cur_mb.mbPitchPixels;
    Ipp32u uCBPLuma     = cur_mb.m_uIntraCBP4x4;
    PIXTYPE* pBlockData = cur_mb.mbPtr + xoff[block] + yoff[block]*pitchPixels;
    PIXTYPE* pPredBuf = cur_mb.mb4x4.prediction + xoff[block] + yoff[block]*16;
    PIXTYPE* pReconBuf = cur_mb.mb4x4.reconstruct + xoff[block] + yoff[block]*16;
    __ALIGN16 COEFFSTYPE pTransRes[16];
    bool transform_bypass = (core_enc->m_SeqParamSet.qpprime_y_zero_transform_bypass_flag && uMBQP == 0);

    pTransformResult = &cur_mb.mb4x4.transform[block*16];
    Diff4x4(pPredBuf, pBlockData, pitchPixels, pDiffBuf);

    if(!transform_bypass)
    {
        ownTransformQuantResidual_H264(pDiffBuf, pTransformResult, uMBQP, &iNumCoeffs, 1,
            enc_single_scan[curr_slice->m_is_cur_mb_field], &iLastCoeff, NULL, NULL, 0, NULL); //Always use f for INTRA
        if (!iNumCoeffs)
        {
            Copy4x4(pPredBuf, 16, pReconBuf, 16);
            uCBPLuma &= ~CBP4x4Mask[block];
        }
        else
        {
            memcpy( pTransRes, pTransformResult, 16*sizeof( COEFFSTYPE ));
            ownDequantTransformResidualAndAdd_H264(pPredBuf, pTransRes, NULL, pReconBuf, 16, 16, uMBQP,
                ((iNumCoeffs < -1) || (iNumCoeffs > 0)), core_enc->m_PicParamSet.bit_depth_luma, NULL);
        }
    }
    else
    {
        Copy4x4(pBlockData, pitchPixels, pReconBuf, 16);
        for( Ipp32s i = 0; i < 16; i++)
            pTransformResult[i] = pDiffBuf[i];
        ownCountCoeffs(pTransformResult, &iNumCoeffs, enc_single_scan[curr_slice->m_is_cur_mb_field], &iLastCoeff, 16);
        if (iNumCoeffs == 0)
        {
            uCBPLuma &= ~CBP4x4Mask[block];
            Copy4x4(pBlockData, pitchPixels, pPredBuf, 16);
        }
    }
    cur_mb.m_iNumCoeffs4x4[ block ] = iNumCoeffs;
    cur_mb.m_iLastCoeff4x4[ block ] = iLastCoeff;
    cur_mb.m_uIntraCBP4x4 = uCBPLuma;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_Encode8x8IntraBlock(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice, Ipp32s block)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32s     iNumCoeffs;
    Ipp32s     iLastCoeff;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32u uMBQP       = cur_mb.lumaQP;
    Ipp32s pitchPixels = cur_mb.mbPitchPixels;

    PIXTYPE* pBlockData = cur_mb.mbPtr + xoff[4*block] + yoff[4*block]*pitchPixels;
    // loop over all 8x8 blocks in Y plane for the MB
    PIXTYPE* pPredBuf = cur_mb.mb8x8.prediction + xoff[block<<2] + yoff[block<<2]*16;
    PIXTYPE* pReconBuf = cur_mb.mb8x8.reconstruct + xoff[block<<2] + yoff[block<<2]*16;

    Ipp32u uCBPLuma     = cur_mb.m_uIntraCBP8x8;
    COEFFSTYPE* pTransformResult = &cur_mb.mb8x8.transform[block*64];
    __ALIGN16 Ipp16s pDiffBuf[64];
    __ALIGN16 COEFFSTYPE pTransRes[64];
    bool transform_bypass = (core_enc->m_SeqParamSet.qpprime_y_zero_transform_bypass_flag && uMBQP == 0);

    Diff8x8(pPredBuf, pBlockData, pitchPixels, pDiffBuf);

    if(!transform_bypass)
    {
        ownTransformLuma8x8Fwd_H264(pDiffBuf, pTransformResult);
        ownQuantLuma8x8_H264(pTransformResult, pTransformResult, QP_DIV_6[uMBQP], 1, enc_single_scan_8x8[curr_slice->m_is_cur_mb_field],
            core_enc->m_SeqParamSet.seq_scaling_matrix_8x8[0][QP_MOD_6[uMBQP]], &iNumCoeffs, &iLastCoeff, NULL, NULL, NULL);
        if (!iNumCoeffs)
        {
            Copy8x8(pPredBuf, 16, pReconBuf, 16);
            uCBPLuma &= ~CBP8x8Mask[block];
        }
        else
        {
            memcpy( pTransRes, pTransformResult, 64*sizeof( COEFFSTYPE ));
            ownQuantLuma8x8Inv_H264(pTransRes, QP_DIV_6[uMBQP], core_enc->m_SeqParamSet.seq_scaling_inv_matrix_8x8[0][QP_MOD_6[uMBQP]]);
            ownTransformLuma8x8InvAddPred_H264(pPredBuf, 16, pTransRes, pReconBuf, 16, core_enc->m_PicParamSet.bit_depth_luma);
        }
    }
    else
    {
        Copy8x8(pBlockData, pitchPixels, pReconBuf, 16);
        for (Ipp32s i = 0; i < 64; i++)
            pTransformResult[i] = pDiffBuf[i];
        ownCountCoeffs(pTransformResult, &iNumCoeffs, enc_single_scan_8x8[curr_slice->m_is_cur_mb_field], &iLastCoeff, 64);
        if (iNumCoeffs == 0)
        {
            uCBPLuma &= ~CBP8x8Mask[block];
            Copy8x8(pBlockData, pitchPixels, pPredBuf, 16);
        }
    }
    cur_mb.m_iNumCoeffs8x8[ block ] = iNumCoeffs;
    cur_mb.m_iLastCoeff8x8[ block ] = iLastCoeff;
    cur_mb.m_uIntraCBP8x8 = uCBPLuma;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_TransQuantIntra16x16_RD(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u  uBlock;     // block number, 0 to 23
    Ipp32u  uMBQP;          // QP of current MB
    Ipp32u  uMB;
    Ipp32u  uCBPLuma;        // coded flags for all 4x4 blocks
    COEFFSTYPE* pDCBuf;     // chroma & luma dc coeffs pointer
    PIXTYPE*  pPredBuf;       // prediction block pointer
    PIXTYPE*  pReconBuf;       // prediction block pointer
    Ipp16s* pDiffBuf;       // difference block pointer
    Ipp16s* pTempDiffBuf;       // difference block pointer
    COEFFSTYPE *pTransformResult; // for transform results.
    Ipp16s* pMassDiffBuf;   // difference block pointer
    COEFFSTYPE* pQBuf;          // quantized block pointer
    Ipp32s  pitchPixels;     // buffer pitch in pixels
    Ipp8u   bCoded; // coded block flag
    Ipp32s  iNumCoeffs; // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s  iLastCoeff; // Number of nonzero coeffs after quant (negative if DC is nonzero)
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;

    pitchPixels = cur_mb.mbPitchPixels;
    uCBPLuma    = cur_mb.LocalMacroblockInfo->cbp_luma;
    uMBQP       = cur_mb.lumaQP;
    pDiffBuf    = (Ipp16s*) (curr_slice->m_pMBEncodeBuffer + 512);
    pTransformResult = (COEFFSTYPE*)(pDiffBuf + 16);
    pQBuf       = (COEFFSTYPE*) (pTransformResult + 16);
    pDCBuf      = (COEFFSTYPE*) (pQBuf + 16);   // Used for both luma and chroma DC blocks
    pMassDiffBuf = (Ipp16s*) (pDCBuf + 16);
    uMB = cur_mb.uMB;

    //--------------------------------------------------------------------------
    // encode Y plane blocks (0-15)
    //--------------------------------------------------------------------------

    // initialize pointers and offset
    pPredBuf    = cur_mb.mb16x16.prediction; // 16-byte aligned work buffer
    pReconBuf    = cur_mb.mb16x16.reconstruct; // 16-byte aligned work buffer
    Ipp32s pitchPix = 16;

    cur_mb.MacroblockCoeffsInfo->lumaAC = 0;
    ownSumsDiff16x16Blocks4x4(cur_mb.mbPtr, pitchPixels, pPredBuf, 16, pDCBuf, pMassDiffBuf); // compute the 4x4 luma DC transform coeffs

    // apply second transform on the luma DC transform coeffs
    ownTransformQuantLumaDC_H264(
        pDCBuf,
        pQBuf,
        uMBQP,
        &iNumCoeffs,
        1,
        enc_single_scan[is_cur_mb_field],&iLastCoeff,
        NULL);

    if (core_enc->m_PicParamSet.entropy_coding_mode)
    {
        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[Y_DC_RLE];
        bCoded = c_data->uNumSigCoeffs /* = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock]*/ = ABS(iNumCoeffs);
        c_data->uLastSignificant = iLastCoeff;
        c_data->CtxBlockCat = BLOCK_LUMA_DC_LEVELS;
        c_data->uFirstCoeff = 0;
        c_data->uLastCoeff = 15;
        H264CoreEncoder_MakeSignificantLists_CABAC(pDCBuf,dec_single_scan[is_cur_mb_field], c_data);
    }
    else
    {
        ownEncodeCoeffsCAVLC_H264(pDCBuf,0, dec_single_scan[is_cur_mb_field],iLastCoeff,
                                   &curr_slice->Block_RLE[Y_DC_RLE].uTrailing_Ones,
                                   &curr_slice->Block_RLE[Y_DC_RLE].uTrailing_One_Signs,
                                   &curr_slice->Block_RLE[Y_DC_RLE].uNumCoeffs,
                                   &curr_slice->Block_RLE[Y_DC_RLE].uTotalZeros,
                                   curr_slice->Block_RLE[Y_DC_RLE].iLevels,
                                   curr_slice->Block_RLE[Y_DC_RLE].uRuns);
        bCoded = curr_slice->Block_RLE[Y_DC_RLE].uNumCoeffs;
    }

    ownTransformDequantLumaDC_H264(
        pDCBuf,
        uMBQP,
        NULL);

    // loop over all 4x4 blocks in Y plane for the MB
    for (uBlock = 0; uBlock < 16; uBlock++ ){
        pPredBuf = cur_mb.mb16x16.prediction + xoff[uBlock] + yoff[uBlock]*16;
        pReconBuf = cur_mb.mb16x16.reconstruct + xoff[uBlock] + yoff[uBlock]*16;

        cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;        // This will be updated if the block is coded
        if (core_enc->m_PicParamSet.entropy_coding_mode) {
            cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
        } else {
            curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
            curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
            curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
            curr_slice->Block_RLE[uBlock].uTotalZeros = 15;
        }

        bCoded = ((uCBPLuma & CBP4x4Mask[uBlock])?(1):(0)); // check if block is coded

        if (!bCoded){
            Copy4x4(pPredBuf, 16, pReconBuf, pitchPix); // update reconstruct frame for the empty block
        }else{   // block not declared empty, encode
            pTempDiffBuf = pMassDiffBuf+ xoff[uBlock]*4 + yoff[uBlock]*16;
            ownTransformQuantResidual_H264(
                pTempDiffBuf,
                pTransformResult,
                uMBQP,
                &iNumCoeffs,
                1,
                enc_single_scan[is_cur_mb_field],
                &iLastCoeff,
                NULL,
                NULL,
                0,
                NULL); //Always use f for INTRA

            cur_mb.MacroblockCoeffsInfo->lumaAC |= ((iNumCoeffs < -1) || (iNumCoeffs > 0));

            if (!iNumCoeffs){
                bCoded = 0;
            } else {
                if (core_enc->m_PicParamSet.entropy_coding_mode)
                {
                    T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
                    c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                    c_data->uLastSignificant = iLastCoeff;
                    c_data->CtxBlockCat = BLOCK_LUMA_AC_LEVELS;
                    c_data->uFirstCoeff = 1;
                    c_data->uLastCoeff = 15;
                    H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult, dec_single_scan[is_cur_mb_field], c_data);
                    bCoded = c_data->uNumSigCoeffs;
                }
                else
                {
                    ownEncodeCoeffsCAVLC_H264(pTransformResult, 1, dec_single_scan[is_cur_mb_field], iLastCoeff,
                                               &curr_slice->Block_RLE[uBlock].uTrailing_Ones,
                                               &curr_slice->Block_RLE[uBlock].uTrailing_One_Signs,
                                               &curr_slice->Block_RLE[uBlock].uNumCoeffs,
                                               &curr_slice->Block_RLE[uBlock].uTotalZeros,
                                               curr_slice->Block_RLE[uBlock].iLevels,
                                               curr_slice->Block_RLE[uBlock].uRuns);
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = bCoded = curr_slice->Block_RLE[uBlock].uNumCoeffs;
                }
            }

            if (!bCoded) uCBPLuma &= ~CBP4x4Mask[uBlock];

            // If the block wasn't coded and the DC coefficient is zero
            if (!bCoded && !pDCBuf[block_subblock_mapping_[uBlock]]){
                Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
            } else {
                ownDequantTransformResidualAndAdd_H264(
                    pPredBuf,
                    pTransformResult,
                    &pDCBuf[block_subblock_mapping_[uBlock]],
                    pReconBuf,
                    16,
                    pitchPix,
                    uMBQP,
                    ((iNumCoeffs < -1) || (iNumCoeffs > 0)),
                    core_enc->m_PicParamSet.bit_depth_luma,
                    NULL);
            }
        }
    }

    cur_mb.LocalMacroblockInfo->cbp_luma = uCBPLuma;
    if (cur_mb.MacroblockCoeffsInfo->lumaAC > 1)
        cur_mb.MacroblockCoeffsInfo->lumaAC = 1;

}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_EncodeChroma(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u  uBlock;         // block number, 0 to 23
    Ipp32u  uOffset;        // to upper left corner of block from start of plane
    Ipp32u  uMBQP;          // QP of current MB
    PIXTYPE*  pSrcPlane;    // start of plane to encode
    Ipp32s    pitchPixels;  // buffer pitch
    COEFFSTYPE *pDCBuf;     // chroma & luma dc coeffs pointer
    PIXTYPE*  pPredBuf;     // prediction block pointer
    PIXTYPE*  pReconBuf;     // prediction block pointer
    PIXTYPE*  pPredBuf_copy;     // prediction block pointer
    PIXTYPE*  pReconBuf_copy;     // prediction block pointer
    COEFFSTYPE* pQBuf;      // quantized block pointer
    Ipp16s* pMassDiffBuf;   // difference block pointer
    Ipp32u   uCBPChroma;    // coded flags for all chroma blocks
    Ipp32s   bCoded;        // coded block flag
    Ipp32s   iNumCoeffs = 0;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s   iLastCoeff;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s   RLE_Offset;    // Index into BlockRLE array

    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;
    EnumSliceType slice_type = curr_slice->m_slice_type;
    COEFFSTYPE *pTransformResult;
    COEFFSTYPE *pTransform;
    Ipp32s QPy = cur_mb.lumaQP;
    Ipp32s pitchPix;

    pitchPix = pitchPixels = cur_mb.mbPitchPixels;
    uMBQP       = cur_mb.chromaQP;
    pTransform = (COEFFSTYPE*)curr_slice->m_pMBEncodeBuffer;
    pQBuf       = (COEFFSTYPE*) (pTransform + 64*2);
    pDCBuf      = (COEFFSTYPE*) (pQBuf + 16);   // Used for both luma and chroma DC blocks
    pMassDiffBuf= (Ipp16s*) (pDCBuf+ 16);
    Ipp16s*  pTempDiffBuf;
    Ipp32u  uMB = cur_mb.uMB;
    Ipp32u  mbOffset;

    // initialize pointers and offset
    mbOffset = uOffset = core_enc->m_pMBOffsets[uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
    bool transform_bypass = (core_enc->m_SeqParamSet.qpprime_y_zero_transform_bypass_flag && QPy == 0);
    uCBPChroma  = cur_mb.LocalMacroblockInfo->cbp_chroma;
    bool intra = (cur_mb.GlobalMacroblockInfo->mbtype == MBTYPE_INTRA) || (cur_mb.GlobalMacroblockInfo->mbtype == MBTYPE_INTRA_16x16);

    if (intra) {
        pPredBuf = cur_mb.mbChromaIntra.prediction;
        pReconBuf = cur_mb.mbChromaIntra.reconstruct;
    } else {
        cur_mb.MacroblockCoeffsInfo->chromaNC = 0;
        pPredBuf = cur_mb.mbChromaInter.prediction;
        pReconBuf = cur_mb.mbChromaInter.reconstruct;
    }

    // initialize pointers for the U plane blocks
    Ipp32s num_blocks = 2 << core_enc->m_PicParamSet.chroma_format_idc;
    Ipp32s startBlock;
    startBlock = uBlock = 16;
    Ipp32u uLastBlock = uBlock+num_blocks;
    Ipp32u uFinalBlock = uBlock+2*num_blocks;
    PIXTYPE* pPredBufV = pPredBuf+8;

    do
    {
        if (uBlock == uLastBlock)
        {
            startBlock = uBlock;
            uOffset = mbOffset;
            pSrcPlane = core_enc->m_pCurrentFrame->m_pVPlane;
            pPredBuf = pPredBufV;
            pReconBuf = core_enc->m_pReconstructFrame->m_pVPlane+uOffset;
            RLE_Offset = V_DC_RLE;
            uLastBlock += num_blocks;
        }
        else
        {
            RLE_Offset = U_DC_RLE;
            pSrcPlane = core_enc->m_pCurrentFrame->m_pUPlane;
            pReconBuf = core_enc->m_pReconstructFrame->m_pUPlane+uOffset;
        }

        ownSumsDiff8x8Blocks4x4(pSrcPlane + uOffset, pitchPixels, pPredBuf, 16, pDCBuf, pMassDiffBuf);
        if (core_enc->m_PicParamSet.chroma_format_idc == 2)
             ownSumsDiff8x8Blocks4x4(pSrcPlane + uOffset+8*pitchPixels, pitchPixels, pPredBuf+8*16, 16, pDCBuf+4, pMassDiffBuf+64);
        // Code chromaDC
        if (!transform_bypass)  {
            switch (core_enc->m_PicParamSet.chroma_format_idc) {
                case 1:
                    ownTransformQuantChromaDC_H264(pDCBuf, pQBuf, uMBQP, &iNumCoeffs, (slice_type == INTRASLICE), 1, NULL);
                    break;
                case 2:
                    ownTransformQuantChroma422DC_H264(pDCBuf, pQBuf, uMBQP, &iNumCoeffs, (slice_type == INTRASLICE), 1, NULL);
                    break;
                default:
                    break;
            }
        } else {
            Ipp32s i,j;
            Ipp32s num_rows, num_cols;
            Ipp32s bPitch;
            num_cols = ((core_enc->m_PicParamSet.chroma_format_idc - 1) & 0x2) ? 4 : 2;
            num_rows = (core_enc->m_PicParamSet.chroma_format_idc & 0x2) ? 4 : 2;
            bPitch = num_cols * 16;
            for(i = 0; i < num_rows; i++) {
                for(j = 0; j < num_cols; j++) {
                    pDCBuf[i*num_cols+j] = pMassDiffBuf[i*bPitch + j*16];
                }
            }
            ownCalcNonZero(pDCBuf, num_blocks, &iNumCoeffs);
        }
        // DC values in this block if iNonEmpty is 1.
        cur_mb.MacroblockCoeffsInfo->chromaNC |= (iNumCoeffs != 0);
        // record RLE info
        if (core_enc->m_PicParamSet.entropy_coding_mode){
            Ipp32s ctxIdxBlockCat = BLOCK_CHROMA_DC_LEVELS;
            switch (core_enc->m_PicParamSet.chroma_format_idc) {
                case 1:
                    H264CoreEncoder_ScanSignificant_CABAC(pDCBuf,ctxIdxBlockCat,4,dec_single_scan_p, &cur_mb.cabac_data[RLE_Offset]);
                    break;
                case 2:
                    H264CoreEncoder_ScanSignificant_CABAC(pDCBuf,ctxIdxBlockCat,8,dec_single_scan_p422, &cur_mb.cabac_data[RLE_Offset]);
                    break;
                default:
                    break;
            }
       }else{
            switch( core_enc->m_PicParamSet.chroma_format_idc ){
                case 1:
                   ownEncodeChromaDcCoeffsCAVLC_H264(
                       pDCBuf,
                       &curr_slice->Block_RLE[RLE_Offset].uTrailing_Ones,
                       &curr_slice->Block_RLE[RLE_Offset].uTrailing_One_Signs,
                       &curr_slice->Block_RLE[RLE_Offset].uNumCoeffs,
                       &curr_slice->Block_RLE[RLE_Offset].uTotalZeros,
                       curr_slice->Block_RLE[RLE_Offset].iLevels,
                       curr_slice->Block_RLE[RLE_Offset].uRuns);
                    break;
                case 2:
                    ownEncodeChroma422DC_CoeffsCAVLC_H264(
                        pDCBuf,
                        &curr_slice->Block_RLE[RLE_Offset].uTrailing_Ones,
                        &curr_slice->Block_RLE[RLE_Offset].uTrailing_One_Signs,
                        &curr_slice->Block_RLE[RLE_Offset].uNumCoeffs,
                        &curr_slice->Block_RLE[RLE_Offset].uTotalZeros,
                        curr_slice->Block_RLE[RLE_Offset].iLevels,
                        curr_slice->Block_RLE[RLE_Offset].uRuns);
                    break;
            }
        }
        // Inverse transform and dequantize for chroma DC
        if (!transform_bypass ){
            switch( core_enc->m_PicParamSet.chroma_format_idc ){
             case 1:
                 ownTransformDequantChromaDC_H264(pDCBuf, uMBQP, NULL);
                 break;
             case 2:
                 ownTransformDequantChromaDC422_H264(pDCBuf, uMBQP, NULL);
                 break;
            default:
                break;
            }
        }
//Encode croma AC
        Ipp32s coeffsCost = 0;
        pPredBuf_copy = pPredBuf;
        pReconBuf_copy = pReconBuf;
        for (uBlock = startBlock; uBlock < uLastBlock; uBlock ++) {
            cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;     // This will be updated if the block is coded
            if (core_enc->m_PicParamSet.entropy_coding_mode){
                cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
            } else {
                curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
                curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
                curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
                curr_slice->Block_RLE[uBlock].uTotalZeros = 15;
            }
            // check if block is coded
            bCoded = ((uCBPChroma & CBP4x4Mask[uBlock-16])?(1):(0));
            if (!bCoded){ // update reconstruct frame for the empty block
                Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
            } else {   // block not declared empty, encode
                pTempDiffBuf = pMassDiffBuf + (uBlock-startBlock)*16;
                pTransformResult = pTransform + (uBlock-startBlock)*16;
                if(!transform_bypass) {
                    ownTransformQuantResidual_H264(
                         pTempDiffBuf,
                         pTransformResult,
                         uMBQP,
                         &iNumCoeffs,
                         0,
                         enc_single_scan[is_cur_mb_field],
                         &iLastCoeff,
                         NULL,
                         NULL,
                         9,
                         NULL);//,NULL, curr_slice, 1, &cbSt);
                    coeffsCost += CalculateCoeffsCost(pTransformResult, 15, &dec_single_scan[is_cur_mb_field][1]);
                 }else {
                    for(Ipp32s i = 0; i < 16; i++) {
                        pTransformResult[i] = pTempDiffBuf[i];
                    }
                    ownCountCoeffs(pTempDiffBuf, &iNumCoeffs, enc_single_scan[is_cur_mb_field], &iLastCoeff, 16);
                }
                // if everything quantized to zero, skip RLE
                cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                if (cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock]){  // the block is empty so it is not coded
                   if (core_enc->m_PicParamSet.entropy_coding_mode){
                        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
                        c_data->uLastSignificant = iLastCoeff;
                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock];
                        c_data->CtxBlockCat = BLOCK_CHROMA_AC_LEVELS;
                        c_data->uFirstCoeff = 1;
                        c_data->uLastCoeff = 15;
                        H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan[is_cur_mb_field], c_data);
                        cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = c_data->uNumSigCoeffs;
                    } else {
                        ownEncodeCoeffsCAVLC_H264(
                            pTransformResult,// pDiffBuf,
                            1,
                            dec_single_scan[is_cur_mb_field],
                            iLastCoeff,
                            &curr_slice->Block_RLE[uBlock].uTrailing_Ones,
                            &curr_slice->Block_RLE[uBlock].uTrailing_One_Signs,
                            &curr_slice->Block_RLE[uBlock].uNumCoeffs,
                            &curr_slice->Block_RLE[uBlock].uTotalZeros,
                            curr_slice->Block_RLE[uBlock].iLevels,
                            curr_slice->Block_RLE[uBlock].uRuns);

                        cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = curr_slice->Block_RLE[uBlock].uNumCoeffs;
                    }
                }
            }
            pPredBuf += chromaPredInc[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock]; //!!!
            pReconBuf += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock];
        }

        if(!transform_bypass && coeffsCost <= (CHROMA_COEFF_MAX_COST<<(core_enc->m_PicParamSet.chroma_format_idc-1)) ){ //Reset all ac coeffs
//           if(cur_mb.MacroblockCoeffsInfo->chromaNC&1) //if we have DC coeffs
//           memset( pTransform, 0, (64*sizeof(COEFFSTYPE))<<(core_enc->m_PicParamSet.chroma_format_idc-1));
           for(uBlock = startBlock; uBlock < uLastBlock; uBlock++){
                cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;
                if (core_enc->m_PicParamSet.entropy_coding_mode){
                   cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
                } else {
                    curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
                    curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
                    curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
                    curr_slice->Block_RLE[uBlock].uTotalZeros = 15;
                }
            }
        }

        pPredBuf = pPredBuf_copy;
        pReconBuf = pReconBuf_copy;
        for (uBlock = startBlock; uBlock < uLastBlock; uBlock ++) {
            cur_mb.MacroblockCoeffsInfo->chromaNC |= 2*(cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock]!=0);
            if (!cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] && !pDCBuf[ chromaDCOffset[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock] ]){
                uCBPChroma &= ~CBP4x4Mask[uBlock-16];
                Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
            }else if(!transform_bypass){
                    ownDequantTransformResidualAndAdd_H264(
                        pPredBuf,
                        pTransform + (uBlock-startBlock)*16,
                        &pDCBuf[ chromaDCOffset[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock] ],
                        pReconBuf,
                        16,
                        pitchPix,
                        uMBQP,
                        (cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock]!=0),
                        core_enc->m_SeqParamSet.bit_depth_chroma,
                        NULL);
            }
            pPredBuf += chromaPredInc[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock]; //!!!
            pReconBuf += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock];
        }   // for uBlock in chroma plane
    } while (uBlock < uFinalBlock);
    //Reset other chroma
    uCBPChroma &= ~(0xffffffff<<(uBlock-16));
    cur_mb.LocalMacroblockInfo->cbp_chroma = uCBPChroma;
    if (cur_mb.MacroblockCoeffsInfo->chromaNC == 3)
        cur_mb.MacroblockCoeffsInfo->chromaNC = 2;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_CEncAndRec4x4IntraMB(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u  uBlock;     // block number, 0 to 23
    Ipp32u  uOffset;        // to upper left corner of block from start of plane
    Ipp32u  uMBQP;          // QP of current MB
    Ipp32u  uMBType;        // current MB type
    Ipp32u  uMB;
    Ipp32u  uCBPLuma;        // coded flags for all 4x4 blocks
    Ipp32u  uIntraSAD;      // intra MB SAD
    Ipp16s* pMassDiffBuf;   // difference block pointer

    COEFFSTYPE* pDCBuf;     // chroma & luma dc coeffs pointer
    PIXTYPE*  pPredBuf;       // prediction block pointer
    PIXTYPE*  pReconBuf;       // prediction block pointer
    Ipp16s*   pDiffBuf;       // difference block pointer
    COEFFSTYPE* pTransformResult; // Result of the transformation.
    COEFFSTYPE* pQBuf;          // quantized block pointer
    PIXTYPE*  pSrcPlane;      // start of plane to encode
    Ipp32s    pitchPixels;     // buffer pitch
    Ipp32s    iMBCost;        // recode MB cost counter
    Ipp32s    iBlkCost[2];    // coef removal counter for left/right 8x8 luma blocks
    Ipp8u     bCoded; // coded block flag
    Ipp32s    iNumCoeffs;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s    iLastCoeff;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32u    uTotalCoeffs = 0;    // Used to detect single expensive coeffs.

    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;
    __ALIGN16 CabacStates cbSt;

    uMB = cur_mb.uMB;

    pitchPixels = cur_mb.mbPitchPixels;
    uCBPLuma     = cur_mb.LocalMacroblockInfo->cbp_luma;
    uMBQP       = cur_mb.lumaQP;
    uMBType     = cur_mb.GlobalMacroblockInfo->mbtype;
    pDiffBuf    = (Ipp16s*) (curr_slice->m_pMBEncodeBuffer + 512);
    pTransformResult = (COEFFSTYPE*)(pDiffBuf + 64);
    pQBuf       = (COEFFSTYPE*) (pTransformResult + 64);
    pDCBuf      = (COEFFSTYPE*) (pQBuf + 16);   // Used for both luma and chroma DC blocks
    pMassDiffBuf= (Ipp16s*) (pDCBuf+ 16);
//  uIntraSAD   = rd_quant_intra[uMBQP] * 24;   // TODO ADB 'handicap' using reconstructed data
    uIntraSAD   = 0;
    iMBCost     = 0;
    iBlkCost[0] = 0;
    iBlkCost[1] = 0;

    //--------------------------------------------------------------------------
    // encode Y plane blocks (0-15)
    //--------------------------------------------------------------------------

    // initialize pointers and offset
    pSrcPlane = core_enc->m_pCurrentFrame->m_pYPlane;
    uOffset = core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
    bool transform_bypass = (core_enc->m_SeqParamSet.qpprime_y_zero_transform_bypass_flag && uMBQP == 0);

    Ipp32s pitchPix;
    pitchPix = pitchPixels;

    if(pGetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo))
    {
        if( core_enc->m_params.quant_opt_level > OPT_QUANT_INTRA8x8 )
        {
            uCBPLuma = 0xffff;
            memcpy( cbSt.absLevelM1, &curr_slice->m_pbitstream->context_array[426], 10*sizeof(CABAC_CONTEXT));
            if( !is_cur_mb_field )
            {
                //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[402], 15*sizeof(CABAC_CONTEXT));
                //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[417], 9*sizeof(CABAC_CONTEXT));
                cbSt.sig = &curr_slice->m_pbitstream->context_array[402];
                cbSt.last = &curr_slice->m_pbitstream->context_array[417];
            }
            else
            {
                //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[436], 15*sizeof(CABAC_CONTEXT));
                //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[451], 9*sizeof(CABAC_CONTEXT));
                cbSt.sig = &curr_slice->m_pbitstream->context_array[436];
                cbSt.last = &curr_slice->m_pbitstream->context_array[451];
            }
        }
        pSetMB8x8TSFlag(cur_mb.GlobalMacroblockInfo, 1);
        //loop over all 8x8 blocks in Y plane for the MB
        for (uBlock = 0; uBlock < 4; uBlock ++)
        {
            Ipp32s idxb, idx, idxe;

            idxb = uBlock<<2;
            idxe = idxb+4;
            pPredBuf = cur_mb.mb8x8.prediction + xoff[4*uBlock] + yoff[4*uBlock]*16;
            pReconBuf = core_enc->m_pReconstructFrame->m_pYPlane + uOffset;

            if(core_enc->m_PicParamSet.entropy_coding_mode)
            {
                cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;        // These will be updated if the block is coded
                cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
            }
            else
            {
                for( idx = idxb; idx<idxe; idx++ )
                {
                    curr_slice->Block_RLE[idx].uNumCoeffs = 0;
                    curr_slice->Block_RLE[idx].uTrailing_Ones = 0;
                    curr_slice->Block_RLE[idx].uTrailing_One_Signs = 0;
                    curr_slice->Block_RLE[idx].uTotalZeros = 16;
                    cur_mb.MacroblockCoeffsInfo->numCoeff[idx] = 0;
                }
            }

            if (!curr_slice->m_use_transform_for_intra_decision)
            {
                uIntraSAD += H264CoreEncoder_AIModeSelectOneMB_8x8(state, curr_slice, pSrcPlane + uOffset, pReconBuf, uBlock, cur_mb.intra_types, pPredBuf);
            }

            // check if block is coded
            bCoded = ((uCBPLuma & CBP8x8Mask[uBlock])?(1):(0));

            if (!bCoded)
            {  // update reconstruct frame for the empty block
                Copy8x8(pPredBuf, 16, pReconBuf, pitchPix);
            }
            else
            {
                // block not declared empty, encode
                // compute difference of predictor and source pels
                // note: asm version does not use pDiffBuf
                //       output is being passed in the mmx registers
                if (!curr_slice->m_use_transform_for_intra_decision ||  core_enc->m_params.quant_opt_level > OPT_QUANT_INTRA8x8)
                {
                    Diff8x8(pPredBuf, pSrcPlane + uOffset, pitchPixels, pDiffBuf);
                    if(!transform_bypass)
                    {
                        if(core_enc->m_params.quant_opt_level > OPT_QUANT_INTRA8x8)
                        {
                            __ALIGN16 PIXTYPE pred[64];
                            PIXTYPE pred_pels[25]; //Sources for prediction
                            Ipp32u pred_pels_mask = 0;
                            Ipp32s i;
                            bool top_avbl;
                            bool left_avbl;
                            bool left_above_avbl = curr_slice->m_cur_mb.BlockNeighbours.mb_above_left.mb_num >= 0;
                            bool right_above_avbl = curr_slice->m_cur_mb.BlockNeighbours.mb_above_right.mb_num >= 0;

                            if( uBlock & 0x2)
                                top_avbl = true;
                            else
                                top_avbl = curr_slice->m_cur_mb.BlockNeighbours.mb_above.mb_num >= 0;

                            if( uBlock & 0x1 )
                                left_avbl = true;
                            else
                                left_avbl = curr_slice->m_cur_mb.BlockNeighbours.mbs_left[0].mb_num >= 0;

                            //Copy pels
                            //TOP
                            if( top_avbl )
                            {
                                for( i=0; i<8; i++ )
                                    pred_pels[1+i] = *(pReconBuf-pitchPixels+i);
                                pred_pels_mask |= 0x000001fe;
                            }

                            //LEFT
                            if( left_avbl )
                            {
                                for( i=0; i<8; i++ )
                                    pred_pels[17+i] = *(pReconBuf+i*pitchPixels-1);
                                pred_pels_mask |= 0x1fe0000;
                            }

                            //LEFT_ABOVE
                            if((uBlock == 0 && left_above_avbl) || uBlock == 3 || (uBlock == 1 && top_avbl) || ( uBlock == 2 && left_avbl))
                            {
                                pred_pels[0] = *(pReconBuf-pitchPixels-1);
                                pred_pels_mask |= 0x01;
                            }

                            //RIGHT_ABOVE
                            if( (uBlock == 2) || (uBlock == 0 && top_avbl) || (uBlock == 1 && right_above_avbl) )
                            {
                                for( i=0; i<8; i++ )
                                    pred_pels[9+i] = *(pReconBuf-pitchPixels+i+8);
                                pred_pels_mask |= 0x0001fe00;
                            }

                            if(!((pred_pels_mask & 0x0001FE00)==0x0001FE00) && (pred_pels_mask & 0x00000100))
                            {
                                pred_pels_mask |= 0x0001FE00;
                                for( i=0; i<8; i++ )
                                    pred_pels[9+i] = pred_pels[1+7];
                            }

                            H264CoreEncoder_Filter8x8Pels(pred_pels, pred_pels_mask);
                            H264CoreEncoder_GetPrediction8x8<COEFFSTYPE, PIXTYPE>(state, cur_mb.intra_types[uBlock<<2], pred_pels, pred_pels_mask, pred );
                            PIXTYPE* p = pPredBuf;
                            for( i=0; i<8; i++)
                            {
                                memcpy(p, &pred[i*8], 8*sizeof(PIXTYPE));
                                p += 16; //pitch = 16
                            }
                            Diff8x8(pPredBuf, pSrcPlane + uOffset, pitchPixels, pDiffBuf);
                            ownTransformLuma8x8Fwd_H264(pDiffBuf, pTransformResult);
                            ownQuantLuma8x8_H264(pTransformResult, pTransformResult, uMBQP, 1, enc_single_scan_8x8[is_cur_mb_field],
                                core_enc->m_SeqParamSet.seq_scaling_matrix_8x8[0][QP_MOD_6[uMBQP]], &iNumCoeffs, &iLastCoeff, curr_slice,
                                &cbSt, core_enc->m_SeqParamSet.seq_scaling_inv_matrix_8x8[0][QP_MOD_6[uMBQP]]); //Use scaling matrix for INTRA
                        }
                        else
                        {
                            // forward transform and quantization, in place in pDiffBuf
                            ownTransformLuma8x8Fwd_H264(pDiffBuf, pTransformResult);
                            ownQuantLuma8x8_H264(pTransformResult, pTransformResult, QP_DIV_6[uMBQP], 1, enc_single_scan_8x8[is_cur_mb_field],
                                core_enc->m_SeqParamSet.seq_scaling_matrix_8x8[0][QP_MOD_6[uMBQP]], &iNumCoeffs, &iLastCoeff, NULL, NULL, NULL); //Use scaling matrix for INTRA
                        }
                    }
                    else
                    {
                        for(Ipp32s i = 0; i < 64; i++)
                            pTransformResult[i] = pDiffBuf[i];
                        ownCountCoeffs(pTransformResult, &iNumCoeffs, enc_single_scan_8x8[is_cur_mb_field], &iLastCoeff, 64);
                    }
                }
                else
                {
                        iNumCoeffs = cur_mb.m_iNumCoeffs8x8[ uBlock ];
                        iLastCoeff = cur_mb.m_iLastCoeff8x8[ uBlock ];
                        pTransformResult = &cur_mb.mb8x8.transform[ uBlock*64 ];
                }

                // if everything quantized to zero, skip RLE
                if (!iNumCoeffs ) // the block is empty so it is not coded
                    bCoded = 0;
                else
                {
                    uTotalCoeffs += ((iNumCoeffs < 0) ? -(iNumCoeffs*2) : iNumCoeffs);

                    // record RLE info
                    if (core_enc->m_PicParamSet.entropy_coding_mode)
                    {
                        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
                        c_data->uLastSignificant = iLastCoeff;
                        c_data->CtxBlockCat = BLOCK_LUMA_64_LEVELS;
//                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs);
                        c_data->uFirstCoeff = 0;
                        c_data->uLastCoeff = 63;
                        H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan_8x8[is_cur_mb_field], c_data);
                        bCoded = c_data->uNumSigCoeffs;
                    }
                    else
                    {
                        COEFFSTYPE buf4x4[4][16];
                        Ipp32s i4x4;
                        Ipp32s i;

                        //Reorder 8x8 block for coding with CAVLC
                        for(i4x4=0; i4x4<4; i4x4++ )
                        {
                            for(i = 0; i<16; i++ )
                                buf4x4[i4x4][dec_single_scan[is_cur_mb_field][i]] = pTransformResult[dec_single_scan_8x8[is_cur_mb_field][4*i+i4x4]];
                        }

                        bCoded = 0;
                        //Encode each block with CAVLC 4x4
                        for(i4x4 = 0; i4x4<4; i4x4++ )
                        {
                            Ipp32s i;
                            iLastCoeff = 0;
                            idx = idxb + i4x4;

                            //Check for last coeff
                            for(i = 0; i<16; i++ )
                            {
                                if( buf4x4[i4x4][dec_single_scan[is_cur_mb_field][i]] != 0 )
                                    iLastCoeff=i;
                            }

                            ownEncodeCoeffsCAVLC_H264(buf4x4[i4x4], 0, dec_single_scan[is_cur_mb_field], iLastCoeff,
                                &curr_slice->Block_RLE[idx].uTrailing_Ones, &curr_slice->Block_RLE[idx].uTrailing_One_Signs, &curr_slice->Block_RLE[idx].uNumCoeffs,
                                &curr_slice->Block_RLE[idx].uTotalZeros, curr_slice->Block_RLE[idx].iLevels, curr_slice->Block_RLE[idx].uRuns);

                            bCoded += curr_slice->Block_RLE[idx].uNumCoeffs;
                            cur_mb.MacroblockCoeffsInfo->numCoeff[idx] = curr_slice->Block_RLE[idx].uNumCoeffs;
                        }
                    }
                }

                // update flags if block quantized to empty
                if (curr_slice->m_use_transform_for_intra_decision && core_enc->m_params.quant_opt_level < OPT_QUANT_INTRA8x8 + 1 )
                {
                    if (!bCoded)
                    {
                        uCBPLuma &= ~CBP8x8Mask[uBlock];
                        //Copy  prediction
                        Copy8x8(pPredBuf, 16, pReconBuf, pitchPix);
                    }
                    else //Copy reconstruct
                        Copy8x8(pPredBuf + 256, 16, pReconBuf, pitchPix);
                }
                else
                {
                    // update flags if block quantized to empty
                    if (!bCoded)
                    {
                        uCBPLuma &= ~CBP8x8Mask[uBlock];
                        // update reconstruct frame with prediction
                        Copy8x8(pPredBuf, 16, pReconBuf, pitchPix);
                    }
                    else if(!transform_bypass)
                    {
                        // inverse transform for reconstruct AND...
                        // add inverse transformed coefficients to original predictor
                        // to obtain reconstructed block, store in reconstruct frame
                        // buffer
                        if(iNumCoeffs != 0)
                        {
                            ownQuantLuma8x8Inv_H264(pTransformResult, QP_DIV_6[uMBQP], core_enc->m_SeqParamSet.seq_scaling_inv_matrix_8x8[0][QP_MOD_6[uMBQP]]);
                            ownTransformLuma8x8InvAddPred_H264(pPredBuf, 16, pTransformResult, pReconBuf, pitchPix, core_enc->m_PicParamSet.bit_depth_luma);
                        }
                    }/*
                    else
                    {
                        // Transform bypass => lossless
                        // RecPlane == SrcPlane => there is no need to copy.
                    }*/
                }   // block not declared empty
            } //curr_slice->m_use_transform_for_intra_decision
            uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock] * 2;
        }  // for uBlock in luma plane
    }
    else
    {
        // loop over all 4x4 blocks in Y plane for the MB
        if(core_enc->m_params.quant_opt_level > OPT_QUANT_INTRA4x4)
        {
            uCBPLuma = 0xffff;
            memcpy( cbSt.absLevelM1, &curr_slice->m_pbitstream->context_array[227+20], 10*sizeof(CABAC_CONTEXT));
            if( !is_cur_mb_field )
            {
                //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[105+29], 15*sizeof(CABAC_CONTEXT));
                //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[166+29], 15*sizeof(CABAC_CONTEXT));
                cbSt.sig = &curr_slice->m_pbitstream->context_array[105+29];
                cbSt.last = &curr_slice->m_pbitstream->context_array[166+29];
            }
            else
            {
                //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[105+172+29], 15*sizeof(CABAC_CONTEXT));
                //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[166+172+29], 15*sizeof(CABAC_CONTEXT));
                cbSt.sig = &curr_slice->m_pbitstream->context_array[105+172+29];
                cbSt.last = &curr_slice->m_pbitstream->context_array[166+172+29];
            }
        }

        for (uBlock = 0; uBlock < 16; uBlock++ )
        {
            pPredBuf = cur_mb.mb4x4.prediction + xoff[uBlock] + yoff[uBlock]*16;
            pReconBuf = core_enc->m_pReconstructFrame->m_pYPlane + uOffset;

            cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0; // These will be updated if the block is coded
            if (core_enc->m_PicParamSet.entropy_coding_mode)
                cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
            else
            {
                curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
                curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
                curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
                curr_slice->Block_RLE[uBlock].uTotalZeros = 16;
            }

            // find advanced intra prediction block, store in PredBuf
            // Select best AI mode for the block, using reconstructed
            // predictor pels. This function also stores the block
            // predictor pels at pPredBuf.
            if (!curr_slice->m_use_transform_for_intra_decision)
            {
                uIntraSAD += H264CoreEncoder_AIModeSelectOneBlock(state, curr_slice, pSrcPlane + uOffset, pReconBuf, uBlock, cur_mb.intra_types, pPredBuf);
            }

            // check if block is coded
            bCoded = ((uCBPLuma & CBP4x4Mask[uBlock])?(1):(0));

            if(!bCoded)
            {
                // update reconstruct frame for the empty block
                Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
            }
            else
            {
                // block not declared empty, encode
                // compute difference of predictor and source pels
                // note: asm version does not use pDiffBuf
                //       output is being passed in the mmx registers
                if (!curr_slice->m_use_transform_for_intra_decision || core_enc->m_params.quant_opt_level > OPT_QUANT_INTRA4x4 )
                {
                    Diff4x4(pPredBuf, pSrcPlane + uOffset, pitchPixels, pDiffBuf);

                    if(!transform_bypass)
                    {
                        if(core_enc->m_params.quant_opt_level > OPT_QUANT_INTRA4x4)
                        {
                            PIXTYPE PredPel[13];
                            //We need to calculate new prediction
                            H264CoreEncoder_GetBlockPredPels(state, curr_slice, pReconBuf, pitchPixels, pReconBuf, pitchPixels, pReconBuf, pitchPixels, uBlock, PredPel);
                            H264CoreEncoder_GetPredBlock(cur_mb.intra_types[uBlock], pPredBuf, PredPel);
                            Diff4x4(pPredBuf, pSrcPlane + uOffset, pitchPixels, pDiffBuf);

                            ownTransformQuantResidual_H264(pDiffBuf, pTransformResult, uMBQP, &iNumCoeffs, 1,
                                enc_single_scan[is_cur_mb_field], &iLastCoeff, NULL, curr_slice, 0, &cbSt); //Always use f for INTRA 
                        }
                        else
                        {
                            ownTransformQuantResidual_H264(pDiffBuf, pTransformResult, uMBQP, &iNumCoeffs, 1,
                                enc_single_scan[is_cur_mb_field], &iLastCoeff, NULL, NULL, 0, NULL); //Always use f for INTRA
                        }
                    }
                    else
                    {
                        for(Ipp32s i = 0; i < 16; i++)
                            pTransformResult[i] = pDiffBuf[i];
                        ownCountCoeffs(pTransformResult, &iNumCoeffs, enc_single_scan[is_cur_mb_field],&iLastCoeff, 16);
                    }
                }
                else
                {
                    iNumCoeffs = cur_mb.m_iNumCoeffs4x4[ uBlock ];
                    iLastCoeff = cur_mb.m_iLastCoeff4x4[ uBlock ];
                    pTransformResult = &cur_mb.mb4x4.transform[ uBlock*16 ];
                }

                // if everything quantized to zero, skip RLE
                if(!iNumCoeffs)
                {
                    // the block is empty so it is not coded
                    bCoded = 0;
                }
                else
                {
                    // Preserve the absolute number of coeffs.
                    if(core_enc->m_PicParamSet.entropy_coding_mode)
                    {
                        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
//                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs);
                        c_data->uLastSignificant = iLastCoeff;
                        c_data->CtxBlockCat = BLOCK_LUMA_LEVELS;
                        c_data->uFirstCoeff = 0;
                        c_data->uLastCoeff = 15;
                        H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan[is_cur_mb_field], c_data);
                        bCoded = c_data->uNumSigCoeffs;
                    }
                    else
                    {
                        // record RLE info
                        cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs);
                        ownEncodeCoeffsCAVLC_H264(pTransformResult, 0, dec_single_scan[is_cur_mb_field], iLastCoeff,
                            &curr_slice->Block_RLE[uBlock].uTrailing_Ones, &curr_slice->Block_RLE[uBlock].uTrailing_One_Signs, &curr_slice->Block_RLE[uBlock].uNumCoeffs,
                            &curr_slice->Block_RLE[uBlock].uTotalZeros, curr_slice->Block_RLE[uBlock].iLevels, curr_slice->Block_RLE[uBlock].uRuns);
                        cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = bCoded = curr_slice->Block_RLE[uBlock].uNumCoeffs;
                    }
                }

                // update flags if block quantized to empty
                if (curr_slice->m_use_transform_for_intra_decision && core_enc->m_params.quant_opt_level < OPT_QUANT_INTRA4x4+1 )
                {
                    if (!bCoded)
                    {
                        uCBPLuma &= ~CBP4x4Mask[uBlock]; //Copy predition
                        Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
                    }
                    else //Copy reconstruct
                        Copy4x4(pPredBuf + 256, 16, pReconBuf, pitchPix);
                }
                else
                {
                    if (!bCoded)
                    {
                        uCBPLuma &= ~CBP4x4Mask[uBlock];
                        // update reconstruct frame for the empty block
                        Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
                    }
                    else if(!transform_bypass)
                    {
                        // inverse transform for reconstruct AND...
                        // add inverse transformed coefficients to original predictor
                        // to obtain reconstructed block, store in reconstruct frame
                        // buffer
                        ownDequantTransformResidualAndAdd_H264(pPredBuf, pTransformResult, NULL, pReconBuf, 16, pitchPix, uMBQP,
                            ((iNumCoeffs < -1) || (iNumCoeffs > 0)), core_enc->m_PicParamSet.bit_depth_luma, NULL);
                    }
                }
            }   // block not declared empty

            // proceed to the next block
            uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock];
        }  // for uBlock in luma plane
    }

    // update coded block flags
    cur_mb.LocalMacroblockInfo->cbp_luma = uCBPLuma;

    // for each block of the MB initialize the AI mode (for INTER MB)
    // or motion vectors (for INTRA MB) to values which will be
    // correct predictors of that type. MV and AI mode prediction
    // depend upon this instead of checking MB type.

    return 1;
}   // CEncAndRec4x4IntraMB

////////////////////////////////////////////////////////////////////////////////
// CEncAndRec16x16IntraMB
//
// Encode and Reconstruct all blocks in one 16x16 Intra macroblock
//
////////////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_CEncAndRec16x16IntraMB(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u  uBlock;     // block number, 0 to 23
    Ipp32u  uOffset;        // to upper left corner of block from start of plane
    Ipp32u  uMBQP;          // QP of current MB
    Ipp32u  uMB;
    Ipp32u  uCBPLuma;        // coded flags for all 4x4 blocks
    Ipp32u  uCBPChroma;        // coded flags for all chroma blocks
    Ipp32u  uIntraSAD;      // intra MB SAD
    COEFFSTYPE* pDCBuf;     // chroma & luma dc coeffs pointer
    PIXTYPE*  pPredBuf;       // prediction block pointer
    PIXTYPE*  pReconBuf;       // prediction block pointer
    Ipp16s* pDiffBuf;       // difference block pointer
    Ipp16s* pTempDiffBuf;       // difference block pointer
    COEFFSTYPE *pTransformResult; // for transform results.
    Ipp16s* pMassDiffBuf;   // difference block pointer
    COEFFSTYPE* pQBuf;          // quantized block pointer
    PIXTYPE*  pSrcPlane;      // start of plane to encode
    Ipp32s  pitchPixels;     // buffer pitch in pixels
    Ipp8u   bCoded; // coded block flag
    Ipp32s  iNumCoeffs; // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s  iLastCoeff; // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32u  RLE_Offset;    // Index into BlockRLE array
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;

    pitchPixels = cur_mb.mbPitchPixels;
    uCBPLuma    = cur_mb.LocalMacroblockInfo->cbp_luma;
    uCBPChroma  = cur_mb.LocalMacroblockInfo->cbp_chroma;
    uMBQP       = cur_mb.lumaQP;
    pDiffBuf    = (Ipp16s*) (curr_slice->m_pMBEncodeBuffer + 512);
    pTransformResult = (COEFFSTYPE*)(pDiffBuf + 16);
    pQBuf       = (COEFFSTYPE*) (pTransformResult + 16);
    pDCBuf      = (COEFFSTYPE*) (pQBuf + 16);   // Used for both luma and chroma DC blocks
    pMassDiffBuf = (Ipp16s*) (pDCBuf + 16);
//  uIntraSAD   = rd_quant_intra[uMBQP] * 24;   // 'handicap' using reconstructed data
    uIntraSAD   = 0;
    uMB = cur_mb.uMB;

    bool transform_bypass = (core_enc->m_SeqParamSet.qpprime_y_zero_transform_bypass_flag && uMBQP == 0);
    //--------------------------------------------------------------------------
    // encode Y plane blocks (0-15)
    //--------------------------------------------------------------------------

    // initialize pointers and offset
    pSrcPlane = core_enc->m_pCurrentFrame->m_pYPlane;
    uOffset = core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
    RLE_Offset = Y_DC_RLE;  // Used in 16x16 Intra mode only
    pPredBuf    = cur_mb.mb16x16.prediction; // 16-byte aligned work buffer
    Ipp32s pitchPix;
    pReconBuf    = core_enc->m_pReconstructFrame->m_pYPlane; // 16-byte aligned work buffer
    pitchPix = pitchPixels;

    // for INTRA 16x16 MBs computation of luma prediction was done as
    // a byproduct of sad calculation prior to this function being
    // called; the predictor blocks are already at pPredBuf.

    // Initialize the AC coeff flag value
    cur_mb.MacroblockCoeffsInfo->lumaAC = 0;
    // compute the 4x4 luma DC transform coeffs
    ownSumsDiff16x16Blocks4x4(pSrcPlane + uOffset, pitchPixels, pPredBuf, 16, pDCBuf, pMassDiffBuf);

    if(!transform_bypass)
    {
         // apply second transform on the luma DC transform coeffs
        // check for CAVLC "level_prefix" overflow for baseline, main and extended profiles (see A.2.1, A.2.2, A.2.3)
        if((core_enc->m_params.profile_idc == H264_PROFILE_BASELINE ||
            core_enc->m_params.profile_idc == H264_PROFILE_MAIN ||
            core_enc->m_params.profile_idc == H264_PROFILE_EXTENDED) &&
            !core_enc->m_params.entropy_coding_mode_flag )
        {
            bool CAVLC_overflow;
            COEFFSTYPE tmpLumaBuf[16];
            COEFFSTYPE tmpChromaBuf[16];
            Ipp32s i;

            // chroma DC values for level check
            PIXTYPE*    pSrcPlaneU    = core_enc->m_pCurrentFrame->m_pUPlane;      // start of plane to encode
            PIXTYPE*    pSrcPlaneV    = core_enc->m_pCurrentFrame->m_pVPlane;      // start of plane to encode
            PIXTYPE*    pPredBufU     = cur_mb.mbChromaIntra.prediction;            // prediction block pointer
            PIXTYPE*    pPredBufV     = cur_mb.mbChromaIntra.prediction + 8;        // prediction block pointer
            Ipp32u      uOffsetChroma = core_enc->m_pMBOffsets[uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
            COEFFSTYPE *pQBufChroma   = (COEFFSTYPE*)(((COEFFSTYPE*)curr_slice->m_pMBEncodeBuffer) + 64*2);
            COEFFSTYPE *pDCBufU       = (COEFFSTYPE*)(pQBufChroma + 16);          // Used for both luma and chroma DC blocks
            COEFFSTYPE *pDCBufV       = pDCBufU + 8;
            Ipp16s*     pMassDiffBufChroma = (Ipp16s*)(pDCBufU + 16);
            Ipp32s      iNumCoeffsChroma;

            ippsCopy_8u((Ipp8u*)pDCBuf, (Ipp8u*)&tmpLumaBuf[0], 16*sizeof(COEFFSTYPE));

            if( core_enc->m_PicParamSet.chroma_format_idc != 0 )
            {
                ownSumsDiff8x8Blocks4x4(pSrcPlaneU + uOffsetChroma, pitchPixels, pPredBufU, 16, pDCBufU, pMassDiffBufChroma);
                ownSumsDiff8x8Blocks4x4(pSrcPlaneV + uOffsetChroma, pitchPixels, pPredBufV, 16, pDCBufV, pMassDiffBufChroma);
                if (core_enc->m_PicParamSet.chroma_format_idc == 2)
                {
                    ownSumsDiff8x8Blocks4x4(pSrcPlaneU + uOffsetChroma+8*pitchPixels, pitchPixels, pPredBufU+8*16, 16, pDCBufU+4, pMassDiffBufChroma+64);
                    ownSumsDiff8x8Blocks4x4(pSrcPlaneV + uOffsetChroma+8*pitchPixels, pitchPixels, pPredBufV+8*16, 16, pDCBufV+4, pMassDiffBufChroma+64);
                }
                ippsCopy_8u((Ipp8u*)pDCBufU, (Ipp8u*)&tmpChromaBuf[0], 16*sizeof(COEFFSTYPE));
            }

            do
            {
                ownTransformQuantLumaDC_H264(pDCBuf, pQBuf, uMBQP, &iNumCoeffs, 1, enc_single_scan[is_cur_mb_field], &iLastCoeff, NULL);
                CAVLC_overflow = false;

                for(i = 0; i < 16; i++)
                {
                    if( pDCBuf[i] > MAX_CAVLC_LEVEL || pDCBuf[i] < MIN_CAVLC_LEVEL )
                    {
                        CAVLC_overflow = true;
                        break;
                    }
                }

                // also check for chroma overflow
                if( core_enc->m_PicParamSet.chroma_format_idc == 1 && !CAVLC_overflow )
                {
                    ownTransformQuantChromaDC_H264(pDCBufU, pQBufChroma, cur_mb.chromaQP, &iNumCoeffsChroma, 1, 1, NULL);
                    for(i = 0; i < 4; i++)
                    {
                        if( pDCBufU[i] > MAX_CAVLC_LEVEL || pDCBufU[i] < MIN_CAVLC_LEVEL )
                        {
                            CAVLC_overflow = true;
                            break;
                        }
                    }

                    if(!CAVLC_overflow)
                    {
                        ownTransformQuantChromaDC_H264(pDCBufV, pQBufChroma, cur_mb.chromaQP, &iNumCoeffsChroma, 1, 1, NULL);
                        for(i = 0; i < 4; i++)
                        {
                            if( pDCBufV[i] > MAX_CAVLC_LEVEL || pDCBufV[i] < MIN_CAVLC_LEVEL )
                            {
                                CAVLC_overflow = true;
                                break;
                            }
                        }
                    }

                }
                else if(core_enc->m_PicParamSet.chroma_format_idc == 2 && !CAVLC_overflow)
                {
                    ownTransformQuantChroma422DC_H264(pDCBufU, pQBufChroma, cur_mb.chromaQP, &iNumCoeffsChroma, 1, 1, NULL);
                    for(i = 0; i < 8; i++)
                    {
                        if( pDCBufU[i] > MAX_CAVLC_LEVEL || pDCBufU[i] < MIN_CAVLC_LEVEL )
                        {
                            CAVLC_overflow = true;
                            break;
                        }
                    }

                    if(!CAVLC_overflow)
                    {
                        ownTransformQuantChroma422DC_H264(pDCBufV, pQBufChroma, cur_mb.chromaQP, &iNumCoeffsChroma, 1, 1, NULL);
                        for(i = 0; i < 8; i++)
                        {
                            if( pDCBufV[i] > MAX_CAVLC_LEVEL || pDCBufV[i] < MIN_CAVLC_LEVEL )
                            {
                                CAVLC_overflow = true;
                                break;
                            }
                        }
                    }
                }
                if( CAVLC_overflow )
                {
                    ippsCopy_8u((Ipp8u*)&tmpLumaBuf[0], (Ipp8u*)pDCBuf, 16*sizeof(COEFFSTYPE));
                    if( core_enc->m_PicParamSet.chroma_format_idc != 0 )
                        ippsCopy_8u((Ipp8u*)&tmpChromaBuf[0], (Ipp8u*)pDCBufU, 16*sizeof(COEFFSTYPE));
                    cur_mb.LocalMacroblockInfo->QP++;
                    uMBQP = cur_mb.lumaQP = getLumaQP(cur_mb.LocalMacroblockInfo->QP, core_enc->m_PicParamSet.bit_depth_luma);
                    cur_mb.lumaQP51 = getLumaQP51(cur_mb.LocalMacroblockInfo->QP, core_enc->m_PicParamSet.bit_depth_luma);
                    cur_mb.chromaQP = getChromaQP(cur_mb.LocalMacroblockInfo->QP, core_enc->m_PicParamSet.chroma_qp_index_offset, core_enc->m_SeqParamSet.bit_depth_chroma);
                }
            } while(CAVLC_overflow);
        }
        else
        {
            ownTransformQuantLumaDC_H264(pDCBuf, pQBuf, uMBQP, &iNumCoeffs, 1, enc_single_scan[is_cur_mb_field], &iLastCoeff, NULL);
        }
    }
    else
    {
       for(Ipp32s i = 0; i < 4; i++)
       {
            for(Ipp32s j = 0; j < 4; j++)
            {
                Ipp32s x, y;
                x = j*16;
                y = i*64;
                pDCBuf[i*4 + j] = pMassDiffBuf[x+y];
            }
        }
        ownCountCoeffs(pDCBuf, &iNumCoeffs, enc_single_scan[is_cur_mb_field], &iLastCoeff, 16);
    }

    // insert the quantized luma Ipp64f transform DC coeffs into
    // RLE buffer

    // record RLE info
    if (core_enc->m_PicParamSet.entropy_coding_mode)
    {
        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[Y_DC_RLE];
        bCoded = c_data->uNumSigCoeffs /* = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock]*/ = ABS(iNumCoeffs);
        c_data->uLastSignificant = iLastCoeff;
        c_data->CtxBlockCat = BLOCK_LUMA_DC_LEVELS;
        c_data->uFirstCoeff = 0;
        c_data->uLastCoeff = 15;
        H264CoreEncoder_MakeSignificantLists_CABAC(pDCBuf,dec_single_scan[is_cur_mb_field], c_data);
    }
    else
    {
        ownEncodeCoeffsCAVLC_H264(pDCBuf, 0, dec_single_scan[is_cur_mb_field], iLastCoeff,
            &curr_slice->Block_RLE[RLE_Offset].uTrailing_Ones,
            &curr_slice->Block_RLE[RLE_Offset].uTrailing_One_Signs,
            &curr_slice->Block_RLE[RLE_Offset].uNumCoeffs,
            &curr_slice->Block_RLE[RLE_Offset].uTotalZeros,
            curr_slice->Block_RLE[RLE_Offset].iLevels,
            curr_slice->Block_RLE[RLE_Offset].uRuns);

        bCoded = curr_slice->Block_RLE[RLE_Offset].uNumCoeffs;
    }

    if(!transform_bypass)
    {
        ownTransformDequantLumaDC_H264(pDCBuf, uMBQP, NULL);
    }

    CabacStates cbSt;
    if( core_enc->m_params.quant_opt_level > OPT_QUANT_INTRA16x16 )
    {
        memcpy( cbSt.absLevelM1, &curr_slice->m_pbitstream->context_array[227+10], 10*sizeof(CABAC_CONTEXT));
        if(!is_cur_mb_field )
        {
            //memcpy( cbSt.sig+1, &curr_slice->m_pbitstream->context_array[105+15], 14*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last+1, &curr_slice->m_pbitstream->context_array[166+15], 14*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[105+14];
            cbSt.last = &curr_slice->m_pbitstream->context_array[166+14];
        }
        else
        {
            //memcpy( cbSt.sig+1, &curr_slice->m_pbitstream->context_array[105+172+15], 14*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last+1, &curr_slice->m_pbitstream->context_array[166+172+15], 14*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[105+172+14];
            cbSt.last = &curr_slice->m_pbitstream->context_array[166+172+14];
        }
    }

    // loop over all 4x4 blocks in Y plane for the MB
    for (uBlock = 0; uBlock < 16; uBlock++ )
    {
        pPredBuf = cur_mb.mb16x16.prediction + xoff[uBlock] + yoff[uBlock]*16;
        pReconBuf = core_enc->m_pReconstructFrame->m_pYPlane+uOffset;
        cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;        // This will be updated if the block is coded
        if (core_enc->m_PicParamSet.entropy_coding_mode)
            cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
        else
        {
            curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
            curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
            curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
            curr_slice->Block_RLE[uBlock].uTotalZeros = 15;
        }

        // check if block is coded
        bCoded = ((uCBPLuma & CBP4x4Mask[uBlock])?(1):(0));

        if (!bCoded)
        {
            // update reconstruct frame for the empty block
            Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
        }
        else
        {   // block not declared empty, encode
            pTempDiffBuf = pMassDiffBuf+ xoff[uBlock]*4 + yoff[uBlock]*16;
            if(!transform_bypass)
            {
                if( core_enc->m_params.quant_opt_level > OPT_QUANT_INTRA16x16)
                {
                    ownTransformQuantResidual_H264(
                        pTempDiffBuf,
                        pTransformResult,
                        uMBQP,
                        &iNumCoeffs,
                        1, //Always use f for INTRA
                        enc_single_scan[is_cur_mb_field],
                        &iLastCoeff,
                        NULL,
                        curr_slice,
                        1,
                        &cbSt);
                }
                else
                {
                    ownTransformQuantResidual_H264(
                        pTempDiffBuf,
                        pTransformResult,
                        uMBQP,
                        &iNumCoeffs,
                        1, //Always use f for INTRA
                        enc_single_scan[is_cur_mb_field],
                        &iLastCoeff,
                        NULL,
                        NULL,
                        0,
                        NULL);
                }
            }
            else
            {
                for(Ipp32s i = 0; i < 16; i++)
                    pTransformResult[i] = pTempDiffBuf[i];
                ownCountCoeffs(pTransformResult, &iNumCoeffs, enc_single_scan[is_cur_mb_field], &iLastCoeff, 16);
            }

            if( ((iNumCoeffs < -1) || (iNumCoeffs > 0)) )
            {
                cur_mb.MacroblockCoeffsInfo->lumaAC |= 1;
                // Preserve the absolute number of coeffs.
                if (core_enc->m_PicParamSet.entropy_coding_mode)
                {
                    T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
                    c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                    c_data->uLastSignificant = iLastCoeff;
                    c_data->CtxBlockCat = BLOCK_LUMA_AC_LEVELS;
                    c_data->uFirstCoeff = 1;
                    c_data->uLastCoeff = 15;
                    H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan[is_cur_mb_field], c_data);
                    bCoded = c_data->uNumSigCoeffs;
                }
                else
                {
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                    ownEncodeCoeffsCAVLC_H264(
                        pTransformResult,//pDiffBuf,
                        1,
                        dec_single_scan[is_cur_mb_field],
                        iLastCoeff,
                        &curr_slice->Block_RLE[uBlock].uTrailing_Ones,
                        &curr_slice->Block_RLE[uBlock].uTrailing_One_Signs,
                        &curr_slice->Block_RLE[uBlock].uNumCoeffs,
                        &curr_slice->Block_RLE[uBlock].uTotalZeros,
                        curr_slice->Block_RLE[uBlock].iLevels,
                        curr_slice->Block_RLE[uBlock].uRuns);

                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = bCoded = curr_slice->Block_RLE[uBlock].uNumCoeffs;
                }
            }
            else
            {
               bCoded = 0;
               uCBPLuma &= ~CBP4x4Mask[uBlock];
            }

            // If the block wasn't coded and the DC coefficient is zero
            if (!bCoded && !pDCBuf[block_subblock_mapping_[uBlock]])
            {
                // update reconstruct frame for the empty block
                Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
            }
            else if(!transform_bypass)
            {
                // inverse transform for reconstruct AND...
                // add inverse transformed coefficients to original predictor
                // to obtain reconstructed block, store in reconstruct frame
                // buffer
                ownDequantTransformResidualAndAdd_H264(
                    pPredBuf,
                    pTransformResult,
                    &pDCBuf[block_subblock_mapping_[uBlock]],
                    pReconBuf,
                    16,
                    pitchPix,
                    uMBQP,
                    ((iNumCoeffs < -1) || (iNumCoeffs > 0)),
                    core_enc->m_PicParamSet.bit_depth_luma,
                    NULL);
            }
        }   // block not declared empty

        // proceed to the next block
        uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock];
    }  // for uBlock in luma plane

    // In JVT, Chroma is Intra if any part of luma is intra.
    // update coded block flags
    cur_mb.LocalMacroblockInfo->cbp_luma = uCBPLuma;

    // Correct the value of nc if both chroma DC and AC coeffs will be coded.
    if (cur_mb.MacroblockCoeffsInfo->lumaAC > 1)
        cur_mb.MacroblockCoeffsInfo->lumaAC = 1;

    // for each block of the MB initialize the AI mode (for INTER MB)
    // or motion vectors (for INTRA MB) to values which will be
    // correct predictors of that type. MV and AI mode prediction
    // depend upon this instead of checking MB type.
    cur_mb.intra_types[0] = cur_mb.LocalMacroblockInfo->intra_16x16_mode;
    for (Ipp32s i=1; i<16; i++)
        cur_mb.intra_types[i] = 2;

    return 1;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_CEncAndRecInterMB(
    void* state,
    H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u  uBlock;     // block number, 0 to 23
    Ipp32u  uOffset;        // to upper left corner of block from start of plane
    Ipp32u  uMBQP;          // QP of current MB
    Ipp32u  uMBType;        // current MB type
    Ipp32u  uMB;
    Ipp32u  uCBPLuma;        // coded flags for all 4x4 blocks

    COEFFSTYPE* pDCBuf;     // chroma & luma dc coeffs pointer
    PIXTYPE*  pPredBuf;       // prediction block pointer
    PIXTYPE*  pReconBuf;       // prediction block pointer
    Ipp16s* pDiffBuf;       // difference block pointer
    COEFFSTYPE *pTransform; // result of the transform.
    COEFFSTYPE *pTransformResult; // result of the transform.
    COEFFSTYPE* pQBuf;          // quantized block pointer
    Ipp16s* pMassDiffBuf;   // difference block pointer
    PIXTYPE*  pSrcPlane;      // start of plane to encode
    Ipp32s    pitchPixels;     // buffer pitch in pixels
    Ipp8u     bCoded;        // coded block flag
    Ipp32s    iNumCoeffs;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s    iLastCoeff;  // Number of nonzero coeffs after quant (negative if DC is nonzero)
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;

    uMBQP       = cur_mb.lumaQP;
    bool transform_bypass = core_enc->m_SeqParamSet.qpprime_y_zero_transform_bypass_flag && uMBQP == 0;
    __ALIGN16 CabacStates cbSt;

    uCBPLuma    = cur_mb.LocalMacroblockInfo->cbp_luma;
    pitchPixels = core_enc->m_pCurrentFrame->m_pitchPixels<<is_cur_mb_field;
    uMBType     = cur_mb.GlobalMacroblockInfo->mbtype;
    pTransform  = (COEFFSTYPE*)curr_slice->m_pMBEncodeBuffer;
    pDiffBuf    = (Ipp16s*) (curr_slice->m_pMBEncodeBuffer + 512);
    pQBuf       = (COEFFSTYPE*) (pDiffBuf+64);
    pDCBuf      = (COEFFSTYPE*) (pQBuf + 16);   // Used for both luma and chroma DC blocks
    pMassDiffBuf= (Ipp16s*) (pDCBuf+ 16);
    uMB=cur_mb.uMB;

    //--------------------------------------------------------------------------
    // encode Y plane blocks (0-15)
    //--------------------------------------------------------------------------

    Ipp32s pitchPix;
    pitchPix = pitchPixels;

    // initialize pointers and offset
    pSrcPlane = core_enc->m_pCurrentFrame->m_pYPlane;
    uOffset = core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
    pPredBuf = cur_mb.mbInter.prediction;

    // Motion Compensate this MB
    H264CoreEncoder_MCOneMBLuma(state, curr_slice, cur_mb.MVs[LIST_0]->MotionVectors, cur_mb.MVs[LIST_1]->MotionVectors, pPredBuf);

    if (core_enc->m_PicParamSet.entropy_coding_mode){
        for( uBlock = 0; uBlock<16; uBlock++ ){
              cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;        // These will be updated if the block is coded
              cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
        }
    } else {
        for( uBlock = 0; uBlock<16; uBlock++ ){
            cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;        // These will be updated if the block is coded
            curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
            curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
            curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
            curr_slice->Block_RLE[uBlock].uTotalZeros = 16;
        }
    }

    if(pGetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo)) {
        Ipp32s mbCost=0;

    if( core_enc->m_params.quant_opt_level > OPT_QUANT_INTER ){
        memcpy( cbSt.absLevelM1, &curr_slice->m_pbitstream->context_array[426], 10*sizeof(CABAC_CONTEXT));
        if( !is_cur_mb_field ){
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[402], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[417], 9*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[402];
            cbSt.last = &curr_slice->m_pbitstream->context_array[417];
        }else{
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[436], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[451], 9*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[436];
            cbSt.last = &curr_slice->m_pbitstream->context_array[451];
        }
    }
        pSetMB8x8TSFlag(cur_mb.GlobalMacroblockInfo, 1);
        //loop over all 8x8 blocks in Y plane for the MB
        Ipp32s coeffCost = 0;
        for (uBlock = 0; uBlock < 4; uBlock++){
            pPredBuf = cur_mb.mbInter.prediction + xoff[uBlock*4] + yoff[uBlock*4]*16;
            // check if block is coded
            bCoded = ((uCBPLuma & CBP8x8Mask[uBlock])?(1):(0));

            if (bCoded){
                Diff8x8(pPredBuf, pSrcPlane + uOffset, pitchPixels, pDiffBuf);
                pTransformResult = pTransform + uBlock*64;
                if(!transform_bypass) {
                    // forward transform and quantization, in place in pDiffBuf
                    ownTransformLuma8x8Fwd_H264(pDiffBuf, pTransformResult);
                    if( core_enc->m_params.quant_opt_level > OPT_QUANT_INTER ){
                        ownQuantLuma8x8_H264(pTransformResult, pTransformResult, uMBQP, 0,
                            enc_single_scan_8x8[is_cur_mb_field], core_enc->m_SeqParamSet.seq_scaling_matrix_8x8[1][QP_MOD_6[uMBQP]], // INTER scaling matrix
                            &iNumCoeffs, &iLastCoeff,curr_slice,&cbSt,core_enc->m_SeqParamSet.seq_scaling_inv_matrix_8x8[1][QP_MOD_6[uMBQP]]);
                    }else{
                    ownQuantLuma8x8_H264(
                        pTransformResult,
                        pTransformResult,
                        QP_DIV_6[uMBQP],
                        0,
                        enc_single_scan_8x8[is_cur_mb_field],
                        core_enc->m_SeqParamSet.seq_scaling_matrix_8x8[1][QP_MOD_6[uMBQP]], // INTER scaling matrix
                        &iNumCoeffs,
                        &iLastCoeff,
                        NULL,
                        NULL,
                        NULL);
                    }
                    coeffCost = CalculateCoeffsCost(pTransformResult, 64, dec_single_scan_8x8[is_cur_mb_field]);
                    mbCost += coeffCost;
                } else {
                    for(Ipp32s i = 0; i < 64; i++) {
                        pTransformResult[i] = pDiffBuf[i];
                    }
                    ownCountCoeffs(pTransformResult, &iNumCoeffs, enc_single_scan_8x8[is_cur_mb_field], &iLastCoeff, 64);
                }

                // if everything quantized to zero, skip RLE
            if (!iNumCoeffs || (!transform_bypass && coeffCost < LUMA_COEFF_8X8_MAX_COST && core_enc->m_params.quant_opt_level < OPT_QUANT_INTER+1 ) ){
                    uCBPLuma &= ~CBP8x8Mask[uBlock];
                } else {
                    // record RLE info
                    if (core_enc->m_PicParamSet.entropy_coding_mode){
                        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
                        c_data->uLastSignificant = iLastCoeff;
                        c_data->CtxBlockCat = BLOCK_LUMA_64_LEVELS;
                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs);
//                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                        c_data->uFirstCoeff = 0;
                        c_data->uLastCoeff = 63;
                        H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan_8x8[is_cur_mb_field], c_data);
                    }else{
                        COEFFSTYPE buf4x4[4][16];
                        Ipp8u iLastCoeff;
                        Ipp32s i4x4;

                        //Reorder 8x8 block for coding with CAVLC
                        for(i4x4=0; i4x4<4; i4x4++ ) {
                            Ipp32s i;
                            for(i = 0; i<16; i++ )
                                buf4x4[i4x4][dec_single_scan[is_cur_mb_field][i]] =
                                    pTransformResult[dec_single_scan_8x8[is_cur_mb_field][4*i+i4x4]];
                        }

                        Ipp32s idx = uBlock*4;
                        //Encode each block with CAVLC 4x4
                        for(i4x4 = 0; i4x4<4; i4x4++, idx++ ) {
                            Ipp32s i;
                            iLastCoeff = 0;

                            //Check for last coeff
                            for(i = 0; i<16; i++ ) if( buf4x4[i4x4][dec_single_scan[is_cur_mb_field][i]] != 0 ) iLastCoeff=i;

                            ownEncodeCoeffsCAVLC_H264(
                                buf4x4[i4x4],
                                0, //Luma
                                dec_single_scan[is_cur_mb_field],
                                iLastCoeff,
                                &curr_slice->Block_RLE[idx].uTrailing_Ones,
                                &curr_slice->Block_RLE[idx].uTrailing_One_Signs,
                                &curr_slice->Block_RLE[idx].uNumCoeffs,
                                &curr_slice->Block_RLE[idx].uTotalZeros,
                                curr_slice->Block_RLE[idx].iLevels,
                                curr_slice->Block_RLE[idx].uRuns);

                            cur_mb.MacroblockCoeffsInfo->numCoeff[idx] = curr_slice->Block_RLE[idx].uNumCoeffs;
                         }
                    }
                }
            }
                uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock] * 2;
        }

        if( !transform_bypass && mbCost < LUMA_COEFF_MB_8X8_MAX_COST ){
                uCBPLuma = 0;
        }

       uOffset = core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
       for (uBlock = 0; uBlock < 4; uBlock++){
            pPredBuf = cur_mb.mbInter.prediction + xoff[uBlock*4] + yoff[uBlock*4]*16;
            pReconBuf = core_enc->m_pReconstructFrame->m_pYPlane + uOffset;
            bCoded = ((uCBPLuma & CBP8x8Mask[uBlock])?(1):(0));
            if (!bCoded){
                Copy8x8(pPredBuf, 16, pReconBuf, pitchPix);
                if (core_enc->m_PicParamSet.entropy_coding_mode)
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;
                else
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock*4+0] =
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock*4+1] =
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock*4+2] =
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock*4+3] = 0;
            } else if(!transform_bypass){
                    ownQuantLuma8x8Inv_H264(pTransform + uBlock*64, QP_DIV_6[uMBQP], core_enc->m_SeqParamSet.seq_scaling_inv_matrix_8x8[1][QP_MOD_6[uMBQP]]); //scaling matrix for INTER slice
                    ownTransformLuma8x8InvAddPred_H264(pPredBuf, 16, pTransform + uBlock*64, pReconBuf, pitchPix, core_enc->m_PicParamSet.bit_depth_luma);
            }
            uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock] * 2;
        }
    } else {
        //loop over all 4x4 blocks in Y plane for the MB
        //first make transform for all blocks
      if( core_enc->m_params.quant_opt_level > OPT_QUANT_INTER ){
        memcpy( cbSt.absLevelM1, &curr_slice->m_pbitstream->context_array[227+20], 10*sizeof(CABAC_CONTEXT));
        if( !is_cur_mb_field ){
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[105+29], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[166+29], 15*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[105+29];
            cbSt.last = &curr_slice->m_pbitstream->context_array[166+29];
        }else{
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[105+172+29], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[166+172+29], 15*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[105+172+29];
            cbSt.last = &curr_slice->m_pbitstream->context_array[166+172+29];
        }
      }

        Ipp32s iNumCoeffs[16], CoeffsCost[16] = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};
        for (uBlock = 0; uBlock < 16; uBlock++ ){
            pPredBuf = cur_mb.mbInter.prediction + xoff[uBlock] + yoff[uBlock]*16;

            // check if block is coded
            bCoded = ((uCBPLuma & CBP4x4Mask[uBlock])?(1):(0));

            if( bCoded ){   // block not declared empty, encode
                Diff4x4(pPredBuf, pSrcPlane + uOffset, pitchPixels, pDiffBuf);
                pTransformResult = pTransform + uBlock*16;
                if(!transform_bypass) {
                    // forward transform and quantization, in place in pDiffBuf
                    if( core_enc->m_params.quant_opt_level > OPT_QUANT_INTER ){
                        ownTransformQuantResidual_H264(
                            pDiffBuf,
                            pTransformResult,
                            uMBQP,
                            &iNumCoeffs[uBlock],
                            0,
                            enc_single_scan[is_cur_mb_field],
                            &iLastCoeff,
                            NULL,
                            curr_slice,
                            0,
                            &cbSt);
                    }else{
                        ownTransformQuantResidual_H264(
                            pDiffBuf,
                            pTransformResult,
                            uMBQP,
                            &iNumCoeffs[uBlock],
                            0,
                            enc_single_scan[is_cur_mb_field],
                            &iLastCoeff,
                            NULL,
                            NULL,
                            0,
                            NULL);
                    }
                    CoeffsCost[uBlock] = CalculateCoeffsCost(pTransformResult, 16, dec_single_scan[is_cur_mb_field]);
                } else {
                    for(Ipp32s i = 0; i < 16; i++) {
                        pTransformResult[i] = pDiffBuf[i];
                    }
                    ownCountCoeffs(pTransformResult, &iNumCoeffs[uBlock], enc_single_scan[is_cur_mb_field], &iLastCoeff, 16);
                }

                if (!iNumCoeffs[uBlock]) { // if everything quantized to zero, skip RLE
                    uCBPLuma &= ~CBP4x4Mask[uBlock];
                }else{
                    // Preserve the absolute number of coeffs.
                    if (core_enc->m_PicParamSet.entropy_coding_mode) {
                        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
//                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs[uBlock] < 0) ? -(iNumCoeffs[uBlock]+1) : iNumCoeffs[uBlock]);
                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs[uBlock]);
                        c_data->uLastSignificant = iLastCoeff;
                        c_data->CtxBlockCat = BLOCK_LUMA_LEVELS;
                        c_data->uFirstCoeff = 0;
                        c_data->uLastCoeff = 15;
                        H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan[is_cur_mb_field], c_data);
                    } else {
                        cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs[uBlock]);
                        ownEncodeCoeffsCAVLC_H264(
                            pTransformResult,
                            0,
                            dec_single_scan[is_cur_mb_field],
                            iLastCoeff,
                            &curr_slice->Block_RLE[uBlock].uTrailing_Ones,
                            &curr_slice->Block_RLE[uBlock].uTrailing_One_Signs,
                            &curr_slice->Block_RLE[uBlock].uNumCoeffs,
                            &curr_slice->Block_RLE[uBlock].uTotalZeros,
                            curr_slice->Block_RLE[uBlock].iLevels,
                            curr_slice->Block_RLE[uBlock].uRuns);
                    }
                }
            }

            // proceed to the next block
            uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock];
        }  // for 4x4 uBlock in luma plane

        //Skip subblock 8x8 if it cost is < 4 or skip MB if it's cost is < 5
        if( !transform_bypass ){
            Ipp32s mbCost=0;
            for( uBlock = 0; uBlock < 4; uBlock++ ){
                Ipp32s sb = uBlock*4;
                Ipp32s block8x8cost = CoeffsCost[sb] + CoeffsCost[sb+1] + CoeffsCost[sb+2] + CoeffsCost[sb+3];

                mbCost += block8x8cost;
                if( block8x8cost <= LUMA_8X8_MAX_COST && core_enc->m_params.quant_opt_level < OPT_QUANT_INTER+1 )
                    uCBPLuma &= ~CBP8x8Mask[uBlock];
            }
                if( mbCost <= LUMA_MB_MAX_COST )
                    uCBPLuma = 0;
        }

        //Make inverse quantization and transform for non zero blocks
        uOffset = core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
        for( uBlock=0; uBlock < 16; uBlock++ ){
            pPredBuf = cur_mb.mbInter.prediction + xoff[uBlock] + yoff[uBlock]*16;
            pReconBuf = core_enc->m_pReconstructFrame->m_pYPlane + uOffset;

            bCoded = ((uCBPLuma & CBP4x4Mask[uBlock])?(1):(0));
            if (!bCoded) {
                // update reconstruct frame for the empty block
                Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
                cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;
            } else if(!transform_bypass) {
                 ownDequantTransformResidualAndAdd_H264(
                     pPredBuf,
                     pTransform + uBlock*16,
                     NULL,
                     pReconBuf,
                     16,
                     pitchPix,
                     uMBQP,
                     ((iNumCoeffs[uBlock] < -1) || (iNumCoeffs[uBlock] > 0)),
                     core_enc->m_PicParamSet.bit_depth_luma,
                     NULL);
              }
            uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock];
           }
   }

    //--------------------------------------------------------------------------
    // encode U plane blocks then V plane blocks
    //--------------------------------------------------------------------------

    // update coded block flags
    cur_mb.LocalMacroblockInfo->cbp_luma = uCBPLuma;

    return 1;
}

////////////////////////////////////////////////////////////////////////////////
// CEncAndRecMB
//
// Main function to drive encode and reconstruct for all blocks
// of one macroblock.
////////////////////////////////////////////////////////////////////////////////
template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_CEncAndRecMB(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    curr_slice->m_cur_mb.MacroblockCoeffsInfo->chromaNC = 0;
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;
    Ipp32s pitchPix;
    PIXTYPE *pDst, *pSrc;

    if(curr_slice->m_cur_mb.GlobalMacroblockInfo->mbtype != MBTYPE_SKIPPED && core_enc->m_PicParamSet.chroma_format_idc != 0)
    {
        PIXTYPE*  pPredBuf;     // prediction block pointer
        Ipp32s    pitchPixels = cur_mb.mbPitchPixels;  // buffer pitch
        Ipp32u    uMB     = cur_mb.uMB;
        Ipp32u    uOffset = core_enc->m_pMBOffsets[uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
        bool      intra   = (cur_mb.GlobalMacroblockInfo->mbtype == MBTYPE_INTRA) || (cur_mb.GlobalMacroblockInfo->mbtype == MBTYPE_INTRA_16x16);
        if(intra)
        {
            pPredBuf = cur_mb.mbChromaIntra.prediction;
            if(!((core_enc->m_Analyse & ANALYSE_RD_OPT) || (core_enc->m_Analyse & ANALYSE_RD_MODE)))
            {
                cur_mb.MacroblockCoeffsInfo->chromaNC = 0;
                H264CoreEncoder_AIModeSelectChromaMBs_8x8(state, curr_slice,
                    core_enc->m_pCurrentFrame->m_pUPlane + uOffset,
                    core_enc->m_pReconstructFrame->m_pUPlane + uOffset,
                    core_enc->m_pCurrentFrame->m_pVPlane + uOffset,
                    core_enc->m_pReconstructFrame->m_pVPlane + uOffset,
                    pitchPixels, &cur_mb.LocalMacroblockInfo->intra_chroma_mode, pPredBuf, pPredBuf+8);  //up to 422 only
            }
        }
        else
        {
            cur_mb.MacroblockCoeffsInfo->chromaNC = 0;
            pPredBuf = cur_mb.mbChromaInter.prediction;
            H264CoreEncoder_MCOneMBChroma(state, curr_slice, pPredBuf);
        }
    }

    switch (curr_slice->m_cur_mb.GlobalMacroblockInfo->mbtype)
    {
        case MBTYPE_INTRA:
            H264CoreEncoder_CEncAndRec4x4IntraMB(state, curr_slice);
            if( core_enc->m_PicParamSet.chroma_format_idc != 0 )
                H264CoreEncoder_EncodeChroma(state, curr_slice);
            break;

        case MBTYPE_INTRA_16x16:
            H264CoreEncoder_CEncAndRec16x16IntraMB(state, curr_slice);
            if( core_enc->m_PicParamSet.chroma_format_idc != 0 )
                H264CoreEncoder_EncodeChroma(state, curr_slice);
            break;

        case MBTYPE_INTER:
        case MBTYPE_INTER_8x8:
        case MBTYPE_INTER_8x8_REF0:
        case MBTYPE_INTER_8x16:
        case MBTYPE_INTER_16x8:
        case MBTYPE_FORWARD:
        case MBTYPE_BACKWARD:
        case MBTYPE_FWD_FWD_16x8:
        case MBTYPE_FWD_BWD_16x8:
        case MBTYPE_BWD_FWD_16x8:
        case MBTYPE_BWD_BWD_16x8:
        case MBTYPE_FWD_FWD_8x16:
        case MBTYPE_FWD_BWD_8x16:
        case MBTYPE_BWD_FWD_8x16:
        case MBTYPE_BWD_BWD_8x16:
        case MBTYPE_BIDIR_FWD_16x8:
        case MBTYPE_FWD_BIDIR_16x8:
        case MBTYPE_BIDIR_BWD_16x8:
        case MBTYPE_BWD_BIDIR_16x8:
        case MBTYPE_BIDIR_BIDIR_16x8:
        case MBTYPE_BIDIR_FWD_8x16:
        case MBTYPE_FWD_BIDIR_8x16:
        case MBTYPE_BIDIR_BWD_8x16:
        case MBTYPE_BWD_BIDIR_8x16:
        case MBTYPE_BIDIR_BIDIR_8x16:
        case MBTYPE_B_8x8:
        case MBTYPE_DIRECT:
        case MBTYPE_BIDIR:
            H264CoreEncoder_CEncAndRecInterMB(state, curr_slice);
            if( core_enc->m_PicParamSet.chroma_format_idc != 0 )
                H264CoreEncoder_EncodeChroma(state, curr_slice);
            //Check for possible skips after cbp reset for MBTYPE_DIRECT & MBTYPE_INTER
            {
                if( cur_mb.LocalMacroblockInfo->cbp_luma != 0 || cur_mb.LocalMacroblockInfo->cbp_chroma != 0 ) break;
                if( cur_mb.GlobalMacroblockInfo->mbtype == MBTYPE_INTER && cur_mb.RefIdxs[LIST_0]->RefIdxs[0] == 0 ){
                    H264MotionVector skip_vec;
                    H264CoreEncoder_Skip_MV_Predicted(state, curr_slice, NULL, &skip_vec);
                    if( cur_mb.MVs[LIST_0]->MotionVectors[0] != skip_vec ) break;
                }else if( cur_mb.GlobalMacroblockInfo->mbtype != MBTYPE_DIRECT ) break;
            }

        case MBTYPE_SKIPPED: //copy prediction to recostruct
            {
                Ipp32s i;
//                for( i = 0; i<4; i++ )  cur_mb.GlobalMacroblockInfo->sbtype[i] = (MBTypeValue)NUMBER_OF_MBTYPES;
                pitchPix = cur_mb.mbPitchPixels;
                pDst = core_enc->m_pReconstructFrame->m_pYPlane + core_enc->m_pMBOffsets[cur_mb.uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
                pSrc = cur_mb.mbInter.prediction;
                if( curr_slice->m_slice_type == BPREDSLICE ){
                    curr_slice->m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_DIRECT; //for correct MC
                }
                H264CoreEncoder_MCOneMBLuma(state, curr_slice, cur_mb.MVs[LIST_0]->MotionVectors, cur_mb.MVs[LIST_1]->MotionVectors, pSrc);
                curr_slice->m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_SKIPPED;
                for( i = 0; i<16; i++ ){
                    memcpy( pDst, pSrc, 16*sizeof(PIXTYPE));
                    pDst += pitchPix;
                    pSrc += 16;
                }
                memset( cur_mb.MacroblockCoeffsInfo->numCoeff, 0, 16 );  //Reset this number for skips
                for (i=0; i<16; i++) cur_mb.intra_types[i] = 2;

                if( core_enc->m_PicParamSet.chroma_format_idc != 0 ){
                    pDst = core_enc->m_pReconstructFrame->m_pUPlane + core_enc->m_pMBOffsets[cur_mb.uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
                    pSrc = cur_mb.mbChromaInter.prediction;
                    if( curr_slice->m_slice_type == BPREDSLICE )
                         curr_slice->m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_DIRECT; //for correct MC
                    H264CoreEncoder_MCOneMBChroma(state, curr_slice, pSrc);
                    curr_slice->m_cur_mb.GlobalMacroblockInfo->mbtype = MBTYPE_SKIPPED;
                    Ipp32s vsize = 8;
                    if( core_enc->m_params.chroma_format_idc == 2 ) vsize = 16;
                    for( i = 0; i < vsize; i++ ){
                        memcpy( pDst, pSrc, 8*sizeof(PIXTYPE));
                        pDst += pitchPix;
                        pSrc += 16;
                    }
                    pDst = core_enc->m_pReconstructFrame->m_pVPlane + core_enc->m_pMBOffsets[cur_mb.uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
                    pSrc = cur_mb.mbChromaInter.prediction+8;
                    for( i = 0; i < vsize; i++ ){
                         memcpy( pDst, pSrc, 8*sizeof(PIXTYPE));
                         pDst += pitchPix;
                         pSrc += 16;
                    }
                    memset( &cur_mb.MacroblockCoeffsInfo->numCoeff[16], 0, 32 );  //Reset this number for skips
                    cur_mb.LocalMacroblockInfo->cbp_chroma = 0;
                }
            }
            break;
        default:
            break;
    }

   if( IS_INTRA_MBTYPE( curr_slice->m_cur_mb.GlobalMacroblockInfo->mbtype ) ){
        Ipp32s k;
        for (k = 0; k < 16; k ++){
            curr_slice->m_cur_mb.MVs[LIST_0]->MotionVectors[k] = null_mv;
            curr_slice->m_cur_mb.MVs[LIST_1]->MotionVectors[k] = null_mv;
            curr_slice->m_cur_mb.MVs[LIST_0 + 2]->MotionVectors[k] = null_mv;
            curr_slice->m_cur_mb.MVs[LIST_1 + 2]->MotionVectors[k] = null_mv;
            curr_slice->m_cur_mb.RefIdxs[LIST_0]->RefIdxs[k] = -1;
            curr_slice->m_cur_mb.RefIdxs[LIST_1]->RefIdxs[k] = -1;
         }
   }else{
      Ipp32s k;
      for (k = 0; k < 16; k++)
           curr_slice->m_cur_mb.intra_types[k] = (T_AIMode) 2;
   }

    if (curr_slice->m_cur_mb.LocalMacroblockInfo->cbp_chroma == 0)
        curr_slice->m_cur_mb.MacroblockCoeffsInfo->chromaNC = 0;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_TransQuantIntra_RD(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u  uBlock;     // block number, 0 to 23
    Ipp32u  uOffset;        // to upper left corner of block from start of plane
    Ipp32u  uMBQP;          // QP of current MB
    Ipp32u  uMBType;        // current MB type
    Ipp32u  uMB;
    Ipp32u  uCBPLuma;        // coded flags for all 4x4 blocks
    Ipp32u  uIntraSAD;      // intra MB SAD
    Ipp16s* pMassDiffBuf;   // difference block pointer

    COEFFSTYPE* pDCBuf;     // chroma & luma dc coeffs pointer
    PIXTYPE*  pPredBuf;       // prediction block pointer
    PIXTYPE*  pReconBuf;       // prediction block pointer
    Ipp16s*   pDiffBuf;       // difference block pointer
    COEFFSTYPE* pTransformResult; // Result of the transformation.
    COEFFSTYPE* pQBuf;          // quantized block pointer
    PIXTYPE*  pSrcPlane;      // start of plane to encode
    Ipp32s    pitchPixels;     // buffer pitch
    Ipp32s    iMBCost;        // recode MB cost counter
    Ipp32s    iBlkCost[2];    // coef removal counter for left/right 8x8 luma blocks
    Ipp8u     bCoded; // coded block flag
    Ipp32s    iNumCoeffs;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s    iLastCoeff;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32u    uTotalCoeffs = 0;    // Used to detect single expensive coeffs.

    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;
    uMB = cur_mb.uMB;

    pitchPixels = cur_mb.mbPitchPixels;
    uCBPLuma     = cur_mb.LocalMacroblockInfo->cbp_luma;
    uMBQP       = cur_mb.lumaQP;
    uMBType     = cur_mb.GlobalMacroblockInfo->mbtype;
    pDiffBuf    = (Ipp16s*) (curr_slice->m_pMBEncodeBuffer + 512);
    pTransformResult = (COEFFSTYPE*)(pDiffBuf + 64);
    pQBuf       = (COEFFSTYPE*) (pTransformResult + 64);
    pDCBuf      = (COEFFSTYPE*) (pQBuf + 16);   // Used for both luma and chroma DC blocks
    pMassDiffBuf= (Ipp16s*) (pDCBuf+ 16);
    uIntraSAD   = 0;
    iMBCost     = 0;
    iBlkCost[0] = 0;
    iBlkCost[1] = 0;

    //--------------------------------------------------------------------------
    // encode Y plane blocks (0-15)
    //--------------------------------------------------------------------------

    // initialize pointers and offset
    pSrcPlane = core_enc->m_pCurrentFrame->m_pYPlane;
    uOffset = core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
    CabacStates cbSt;

    Ipp32s pitchPix = 16;
//    pitchPix = pitchPixels;

    if(pGetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo)) {

      if( core_enc->m_params.quant_opt_level > 1){
        memcpy( cbSt.absLevelM1, &curr_slice->m_pbitstream->context_array[426], 10*sizeof(CABAC_CONTEXT));
        if( !is_cur_mb_field ){
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[402], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[417], 9*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[402];
            cbSt.last = &curr_slice->m_pbitstream->context_array[417];
        }else{
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[436], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[451], 9*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[436];
            cbSt.last = &curr_slice->m_pbitstream->context_array[451];
        }
     }
        pSetMB8x8TSFlag(cur_mb.GlobalMacroblockInfo, 1);

        for (uBlock = 0; uBlock < 4; uBlock ++){
            Ipp32s idxb, idx, idxe;

            idxb = uBlock<<2;
            idxe = idxb+4;
            pPredBuf = cur_mb.mb8x8.prediction + xoff[4*uBlock] + yoff[4*uBlock]*16;
            pReconBuf = cur_mb.mb8x8.reconstruct + xoff[4*uBlock] + yoff[4*uBlock]*16;
            //pReconBuf = core_enc->m_pReconstructFrame->m_pYPlane + uOffset;

            if (core_enc->m_PicParamSet.entropy_coding_mode)
            {
                cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;        // These will be updated if the block is coded
                cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
            } else {
                for( idx = idxb; idx<idxe; idx++ ){
                    curr_slice->Block_RLE[idx].uNumCoeffs = 0;
                    curr_slice->Block_RLE[idx].uTrailing_Ones = 0;
                    curr_slice->Block_RLE[idx].uTrailing_One_Signs = 0;
                    curr_slice->Block_RLE[idx].uTotalZeros = 16;
                    cur_mb.MacroblockCoeffsInfo->numCoeff[idx] = 0;
               }
            }

            if (!curr_slice->m_use_transform_for_intra_decision){
                uIntraSAD += H264CoreEncoder_AIModeSelectOneMB_8x8(
                    state,
                    curr_slice,
                    pSrcPlane + uOffset,
                    pReconBuf,
                    uBlock,
                    cur_mb.intra_types,
                    pPredBuf);
            }

            // check if block is coded
            bCoded = ((uCBPLuma & CBP8x8Mask[uBlock])?(1):(0));

            if (!bCoded){  // update reconstruct frame for the empty block
                Copy8x8(pPredBuf, 16, pReconBuf, pitchPix);
            } else {   // block not declared empty, encode
                // compute difference of predictor and source pels
                // note: asm version does not use pDiffBuf
                //       output is being passed in the mmx registers
            if (!curr_slice->m_use_transform_for_intra_decision /*|| core_enc->m_params.quant_opt_level > 1*/){
                Diff8x8(pPredBuf, pSrcPlane + uOffset, pitchPixels, pDiffBuf);
                    // forward transform and quantization, in place in pDiffBuf
                    ownTransformLuma8x8Fwd_H264(pDiffBuf, pTransformResult);
                    if( core_enc->m_params.quant_opt_level > 1 ){
                        ownQuantLuma8x8_H264(pTransformResult,pTransformResult,uMBQP, 1,
                            enc_single_scan_8x8[is_cur_mb_field], core_enc->m_SeqParamSet.seq_scaling_matrix_8x8[0][QP_MOD_6[uMBQP]], //Use scaling matrix for INTRA
                            &iNumCoeffs, &iLastCoeff,curr_slice,&cbSt,core_enc->m_SeqParamSet.seq_scaling_inv_matrix_8x8[0][QP_MOD_6[uMBQP]]);
                    }else{
                        ownQuantLuma8x8_H264(
                            pTransformResult,
                            pTransformResult,
                            QP_DIV_6[uMBQP],
                            1,
                            enc_single_scan_8x8[is_cur_mb_field],
                            core_enc->m_SeqParamSet.seq_scaling_matrix_8x8[0][QP_MOD_6[uMBQP]], //Use scaling matrix for INTRA
                            &iNumCoeffs,
                            &iLastCoeff,
                            NULL,
                            NULL,
                            NULL);
                    }
            }else{
                    iNumCoeffs = cur_mb.m_iNumCoeffs8x8[ uBlock ];
                    iLastCoeff = cur_mb.m_iLastCoeff8x8[ uBlock ];
                    pTransformResult = &cur_mb.mb8x8.transform[ uBlock*64 ];
            }

                // if everything quantized to zero, skip RLE
                if (!iNumCoeffs ){ // the block is empty so it is not coded
                    bCoded = 0;
                } else {
                    uTotalCoeffs += ((iNumCoeffs < 0) ? -(iNumCoeffs*2) : iNumCoeffs);

                    // record RLE info
                    if (core_enc->m_PicParamSet.entropy_coding_mode){
                        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
                        c_data->uLastSignificant = iLastCoeff;
                        c_data->CtxBlockCat = BLOCK_LUMA_64_LEVELS;
//                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs);
                        c_data->uFirstCoeff = 0;
                        c_data->uLastCoeff = 63;
                        H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan_8x8[is_cur_mb_field], c_data);
                        bCoded = c_data->uNumSigCoeffs;
                    }else{
                        COEFFSTYPE buf4x4[4][16];
                        Ipp32s i4x4;

                        //Reorder 8x8 block for coding with CAVLC
                        for(i4x4=0; i4x4<4; i4x4++ ) {
                            Ipp32s i;
                            for(i = 0; i<16; i++ )
                                buf4x4[i4x4][dec_single_scan[is_cur_mb_field][i]] =
                                    pTransformResult[dec_single_scan_8x8[is_cur_mb_field][4*i+i4x4]];
                        }

                        bCoded = 0;
                        //Encode each block with CAVLC 4x4
                        for(i4x4 = 0; i4x4<4; i4x4++ ) {
                            Ipp32s i;
                            iLastCoeff = 0;
                            idx = idxb + i4x4;

                            //Check for last coeff
                            for(i = 0; i<16; i++ ) if( buf4x4[i4x4][dec_single_scan[is_cur_mb_field][i]] != 0 ) iLastCoeff=i;

                            ownEncodeCoeffsCAVLC_H264(
                                buf4x4[i4x4],
                                0, //Luma
                                dec_single_scan[is_cur_mb_field],
                                iLastCoeff,
                                &curr_slice->Block_RLE[idx].uTrailing_Ones,
                                &curr_slice->Block_RLE[idx].uTrailing_One_Signs,
                                &curr_slice->Block_RLE[idx].uNumCoeffs,
                                &curr_slice->Block_RLE[idx].uTotalZeros,
                                curr_slice->Block_RLE[idx].iLevels,
                                curr_slice->Block_RLE[idx].uRuns);

                            bCoded += curr_slice->Block_RLE[idx].uNumCoeffs;
                            cur_mb.MacroblockCoeffsInfo->numCoeff[idx] = curr_slice->Block_RLE[idx].uNumCoeffs;
                         }
                    }
                }

            // update flags if block quantized to empty
            if (curr_slice->m_use_transform_for_intra_decision){
                if (!bCoded){
                    uCBPLuma &= ~CBP8x8Mask[uBlock];
                    //Copy  prediction
                    Copy8x8(pPredBuf, 16, pReconBuf, pitchPix);
                }else //Copy reconstruct
                    Copy8x8(pPredBuf + 256, 16, pReconBuf, pitchPix);
            }else{
                // update flags if block quantized to empty
                if (!bCoded){
                    uCBPLuma &= ~CBP8x8Mask[uBlock];
                    // update reconstruct frame with prediction
                    Copy8x8(pPredBuf, 16, pReconBuf, pitchPix);
                }else {
                    // inverse transform for reconstruct AND...
                    // add inverse transformed coefficients to original predictor
                    // to obtain reconstructed block, store in reconstruct frame
                    // buffer
                    if(iNumCoeffs != 0) {
                        ownQuantLuma8x8Inv_H264(pTransformResult, QP_DIV_6[uMBQP], core_enc->m_SeqParamSet.seq_scaling_inv_matrix_8x8[0][QP_MOD_6[uMBQP]]);
                        ownTransformLuma8x8InvAddPred_H264(pPredBuf, 16, pTransformResult, pReconBuf, pitchPix, core_enc->m_PicParamSet.bit_depth_luma);
                    }
                }
            }   // block not declared empty
            } //curr_slice->m_use_transform_for_intra_decision
            uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock] * 2;
        }  // for uBlock in luma plane
    }else{
      if( core_enc->m_params.quant_opt_level > 1 ){
        memcpy( cbSt.absLevelM1, &curr_slice->m_pbitstream->context_array[227+20], 10*sizeof(CABAC_CONTEXT));
        if( !is_cur_mb_field ){
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[105+29], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[166+29], 15*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[105+29];
            cbSt.last = &curr_slice->m_pbitstream->context_array[166+29];
        }else{
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[105+172+29], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[166+172+29], 15*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[105+172+29];
            cbSt.last = &curr_slice->m_pbitstream->context_array[166+172+29];
        }
      }

    for (uBlock = 0; uBlock < 16; uBlock++ ){
        pPredBuf = cur_mb.mb4x4.prediction + xoff[uBlock] + yoff[uBlock]*16;
        pReconBuf = cur_mb.mb4x4.reconstruct + xoff[uBlock] + yoff[uBlock]*16;
        //pReconBuf = core_enc->m_pReconstructFrame->m_pYPlane + uOffset;

        cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0; // These will be updated if the block is coded
        if (core_enc->m_PicParamSet.entropy_coding_mode){
            cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
        }else{
            curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
            curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
            curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
            curr_slice->Block_RLE[uBlock].uTotalZeros = 16;
        }

        // find advanced intra prediction block, store in PredBuf
        // Select best AI mode for the block, using reconstructed
        // predictor pels. This function also stores the block
        // predictor pels at pPredBuf.
        if (!curr_slice->m_use_transform_for_intra_decision){
            uIntraSAD += H264CoreEncoder_AIModeSelectOneBlock(
                state,
                curr_slice,
                pSrcPlane + uOffset,
                pReconBuf,
                uBlock,
                cur_mb.intra_types,
                pPredBuf);
        }

        // check if block is coded
        bCoded = ((uCBPLuma & CBP4x4Mask[uBlock])?(1):(0));

        if (!bCoded){
            // update reconstruct frame for the empty block
            Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
        } else {   // block not declared empty, encode
            // compute difference of predictor and source pels
            // note: asm version does not use pDiffBuf
            //       output is being passed in the mmx registers
          if (!curr_slice->m_use_transform_for_intra_decision /*|| core_enc->m_params.quant_opt_level > 1*/){
            Diff4x4(pPredBuf, pSrcPlane + uOffset, pitchPixels, pDiffBuf);
                if( core_enc->m_params.quant_opt_level > 1 ){
                    ownTransformQuantResidual_H264(
                        pDiffBuf,
                        pTransformResult,
                        uMBQP,
                        &iNumCoeffs,
                        1, //Always use f for INTRA
                        enc_single_scan[is_cur_mb_field],
                        &iLastCoeff,
                        NULL,
                        curr_slice,
                        0,
                        &cbSt);
                }else{
                    ownTransformQuantResidual_H264(
                        pDiffBuf,
                        pTransformResult,
                        uMBQP,
                        &iNumCoeffs,
                        1, //Always use f for INTRA
                        enc_single_scan[is_cur_mb_field],
                        &iLastCoeff,
                        NULL,
                        NULL,
                        0,
                        NULL);
                 }
          }else{
              iNumCoeffs = cur_mb.m_iNumCoeffs4x4[ uBlock ];
              iLastCoeff = cur_mb.m_iLastCoeff4x4[ uBlock ];
              pTransformResult = &cur_mb.mb4x4.transform[ uBlock*16 ];
          }
            // if everything quantized to zero, skip RLE
            if (!iNumCoeffs){
                // the block is empty so it is not coded
                bCoded = 0;
            } else {
                // Preserve the absolute number of coeffs.
                if (core_enc->m_PicParamSet.entropy_coding_mode){
                    T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
//                    c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                    c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs);
                    c_data->uLastSignificant = iLastCoeff;
                    c_data->CtxBlockCat = BLOCK_LUMA_LEVELS;
                    c_data->uFirstCoeff = 0;
                    c_data->uLastCoeff = 15;
                    H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan[is_cur_mb_field], c_data);
                    bCoded = c_data->uNumSigCoeffs;
                } else {
                // record RLE info
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs);
                    ownEncodeCoeffsCAVLC_H264(
                        pTransformResult,
                        0,
                        dec_single_scan[is_cur_mb_field],
                        iLastCoeff,
                        &curr_slice->Block_RLE[uBlock].uTrailing_Ones,
                        &curr_slice->Block_RLE[uBlock].uTrailing_One_Signs,
                        &curr_slice->Block_RLE[uBlock].uNumCoeffs,
                        &curr_slice->Block_RLE[uBlock].uTotalZeros,
                        curr_slice->Block_RLE[uBlock].iLevels,
                        curr_slice->Block_RLE[uBlock].uRuns);
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = bCoded = curr_slice->Block_RLE[uBlock].uNumCoeffs;
                }
            }

            // update flags if block quantized to empty
            if (curr_slice->m_use_transform_for_intra_decision) {
                if (!bCoded) {
                    uCBPLuma &= ~CBP4x4Mask[uBlock]; //Copy predition
                    Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
                }else //Copy reconstruct
                    Copy4x4(pPredBuf + 256, 16, pReconBuf, pitchPix);
            } else {
                if (!bCoded){
                    uCBPLuma &= ~CBP4x4Mask[uBlock];
                    Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
                } else {
                    ownDequantTransformResidualAndAdd_H264(
                        pPredBuf,
                        pTransformResult,
                        NULL,
                        pReconBuf,
                        16,
                        pitchPix,
                        uMBQP,
                        ((iNumCoeffs < -1) || (iNumCoeffs > 0)),
                        core_enc->m_PicParamSet.bit_depth_luma,
                        NULL);
                }
            }
        }   // block not declared empty

        // proceed to the next block
        uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock];
    }  // for uBlock in luma plane
    }

    cur_mb.LocalMacroblockInfo->cbp_luma = uCBPLuma;

    return 1;
}

template<typename COEFFSTYPE, typename PIXTYPE>
Ipp32u H264CoreEncoder_TransQuantInter_RD(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u  uBlock;     // block number, 0 to 23
    Ipp32u  uOffset;        // to upper left corner of block from start of plane
    Ipp32u  uMBQP;          // QP of current MB
    Ipp32u  uMBType;        // current MB type
    Ipp32u  uMB;
    Ipp32u  uCBPLuma;        // coded flags for all 4x4 blocks

    COEFFSTYPE* pDCBuf;     // chroma & luma dc coeffs pointer
    PIXTYPE*  pPredBuf;       // prediction block pointer
    PIXTYPE*  pReconBuf;       // prediction block pointer
    Ipp16s* pDiffBuf;       // difference block pointer
    COEFFSTYPE *pTransform; // result of the transform.
    COEFFSTYPE *pTransformResult; // result of the transform.
    COEFFSTYPE* pQBuf;          // quantized block pointer
    Ipp16s* pMassDiffBuf;   // difference block pointer
    PIXTYPE*  pSrcPlane;      // start of plane to encode
    Ipp32s    pitchPixels;     // buffer pitch in pixels
    Ipp8u     bCoded;        // coded block flag
    Ipp32s    iNumCoeffs;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s    iLastCoeff;  // Number of nonzero coeffs after quant (negative if DC is nonzero)
    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;

    uMBQP       = cur_mb.lumaQP;
    CabacStates cbSt;

    uCBPLuma    = cur_mb.LocalMacroblockInfo->cbp_luma;
    pitchPixels = core_enc->m_pCurrentFrame->m_pitchPixels << is_cur_mb_field;
    uMBType     = cur_mb.GlobalMacroblockInfo->mbtype;
    pTransform  = (COEFFSTYPE*)curr_slice->m_pMBEncodeBuffer;
    pDiffBuf    = (Ipp16s*) (curr_slice->m_pMBEncodeBuffer + 512);
    pQBuf       = (COEFFSTYPE*) (pDiffBuf+64);
    pDCBuf      = (COEFFSTYPE*) (pQBuf + 16);   // Used for both luma and chroma DC blocks
    pMassDiffBuf= (Ipp16s*) (pDCBuf+ 16);
    uMB=cur_mb.uMB;

    //--------------------------------------------------------------------------
    // encode Y plane blocks (0-15)
    //--------------------------------------------------------------------------

    Ipp32s pitchPix = 16;
//    pitchPix = pitchPixels;

    // initialize pointers and offset
    pSrcPlane = core_enc->m_pCurrentFrame->m_pYPlane;
    uOffset = core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
    pPredBuf = cur_mb.mbInter.prediction;

    // Motion Compensate this MB
    H264CoreEncoder_MCOneMBLuma(state, curr_slice, cur_mb.MVs[LIST_0]->MotionVectors, cur_mb.MVs[LIST_1]->MotionVectors, pPredBuf);

    if (core_enc->m_PicParamSet.entropy_coding_mode){
        for( uBlock = 0; uBlock<16; uBlock++ ){
              cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;        // These will be updated if the block is coded
              cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
        }
    } else {
        for( uBlock = 0; uBlock<16; uBlock++ ){
            cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;        // These will be updated if the block is coded
            curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
            curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
            curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
            curr_slice->Block_RLE[uBlock].uTotalZeros = 16;
        }
    }

    if(pGetMB8x8TSPackFlag(cur_mb.GlobalMacroblockInfo)) {
        Ipp32s mbCost=0;

    if( core_enc->m_params.quant_opt_level > OPT_QUANT_INTER_RD ){
//Save current cabac state
        memcpy( cbSt.absLevelM1, &curr_slice->m_pbitstream->context_array[426], 10*sizeof(CABAC_CONTEXT));
        if( !is_cur_mb_field ){
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[402], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[417], 9*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[402];
            cbSt.last = &curr_slice->m_pbitstream->context_array[417];
        }else{
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[436], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[451], 9*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[436];
            cbSt.last = &curr_slice->m_pbitstream->context_array[451];
        }
     }
        pSetMB8x8TSFlag(cur_mb.GlobalMacroblockInfo, 1);
        //loop over all 8x8 blocks in Y plane for the MB
        Ipp32s coeffCost;
        for (uBlock = 0; uBlock < 4; uBlock++){
            pPredBuf = cur_mb.mbInter.prediction + xoff[uBlock*4] + yoff[uBlock*4]*16;
            // check if block is coded
            bCoded = ((uCBPLuma & CBP8x8Mask[uBlock])?(1):(0));

            if (bCoded){
                Diff8x8(pPredBuf, pSrcPlane + uOffset, pitchPixels, pDiffBuf);
                pTransformResult = pTransform + uBlock*64;
                    // forward transform and quantization, in place in pDiffBuf
                    ownTransformLuma8x8Fwd_H264(pDiffBuf, pTransformResult);
                    if( core_enc->m_params.quant_opt_level > OPT_QUANT_INTER_RD ){
                        ownQuantLuma8x8_H264(
                            pTransformResult,
                            pTransformResult,
                            uMBQP,
                            0,
                            enc_single_scan_8x8[is_cur_mb_field],
                            core_enc->m_SeqParamSet.seq_scaling_matrix_8x8[1][QP_MOD_6[uMBQP]], // INTER scaling matrix
                            &iNumCoeffs,
                            &iLastCoeff,
                            curr_slice,
                            &cbSt,
                            core_enc->m_SeqParamSet.seq_scaling_inv_matrix_8x8[1][QP_MOD_6[uMBQP]]);
                    }else{
                        ownQuantLuma8x8_H264(
                            pTransformResult,
                            pTransformResult,
                            QP_DIV_6[uMBQP],
                            0,
                            enc_single_scan_8x8[is_cur_mb_field],
                            core_enc->m_SeqParamSet.seq_scaling_matrix_8x8[1][QP_MOD_6[uMBQP]], // INTER scaling matrix
                            &iNumCoeffs,
                            &iLastCoeff,
                            NULL,
                            NULL,
                            NULL);
                    }
                    coeffCost = CalculateCoeffsCost(pTransformResult, 64, dec_single_scan_8x8[is_cur_mb_field]);
                    mbCost += coeffCost;

                // if everything quantized to zero, skip RLE
                if (!iNumCoeffs || (coeffCost < LUMA_COEFF_8X8_MAX_COST && core_enc->m_params.quant_opt_level < OPT_QUANT_INTER_RD+1)){
                    uCBPLuma &= ~CBP8x8Mask[uBlock];
                } else {
                    // record RLE info
                    if (core_enc->m_PicParamSet.entropy_coding_mode){
                        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
                        c_data->uLastSignificant = iLastCoeff;
                        c_data->CtxBlockCat = BLOCK_LUMA_64_LEVELS;
                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs);
//                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                        c_data->uFirstCoeff = 0;
                        c_data->uLastCoeff = 63;
                        H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan_8x8[is_cur_mb_field], c_data);
                    }else{
                        COEFFSTYPE buf4x4[4][16];
                        Ipp8u iLastCoeff;
                        Ipp32s i4x4;

                        //Reorder 8x8 block for coding with CAVLC
                        for(i4x4=0; i4x4<4; i4x4++ ) {
                            Ipp32s i;
                            for(i = 0; i<16; i++ )
                                buf4x4[i4x4][dec_single_scan[is_cur_mb_field][i]] =
                                    pTransformResult[dec_single_scan_8x8[is_cur_mb_field][4*i+i4x4]];
                        }

                        Ipp32s idx = uBlock*4;
                        //Encode each block with CAVLC 4x4
                        for(i4x4 = 0; i4x4<4; i4x4++, idx++ ) {
                            Ipp32s i;
                            iLastCoeff = 0;

                            //Check for last coeff
                            for(i = 0; i<16; i++ ) if( buf4x4[i4x4][dec_single_scan[is_cur_mb_field][i]] != 0 ) iLastCoeff=i;

                            ownEncodeCoeffsCAVLC_H264(
                                buf4x4[i4x4],
                                0, //Luma
                                dec_single_scan[is_cur_mb_field],
                                iLastCoeff,
                                &curr_slice->Block_RLE[idx].uTrailing_Ones,
                                &curr_slice->Block_RLE[idx].uTrailing_One_Signs,
                                &curr_slice->Block_RLE[idx].uNumCoeffs,
                                &curr_slice->Block_RLE[idx].uTotalZeros,
                                curr_slice->Block_RLE[idx].iLevels,
                                curr_slice->Block_RLE[idx].uRuns);

                            cur_mb.MacroblockCoeffsInfo->numCoeff[idx] = curr_slice->Block_RLE[idx].uNumCoeffs;
                         }
                    }
                }
            }
                uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock] * 2;
        }

        if( mbCost < LUMA_COEFF_MB_8X8_MAX_COST ){
                uCBPLuma = 0;
        }

       uOffset = core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
       for (uBlock = 0; uBlock < 4; uBlock++){
            pPredBuf = cur_mb.mbInter.prediction + xoff[uBlock*4] + yoff[uBlock*4]*16;
            pReconBuf = cur_mb.mbInter.reconstruct + xoff[uBlock*4] + yoff[uBlock*4]*16;
            //pReconBuf = core_enc->m_pReconstructFrame->m_pYPlane + uOffset;

            bCoded = ((uCBPLuma & CBP8x8Mask[uBlock])?(1):(0));
            if (!bCoded){
                Copy8x8(pPredBuf, 16, pReconBuf, pitchPix);
                if (core_enc->m_PicParamSet.entropy_coding_mode)
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;
                else
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock*4+0] =
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock*4+1] =
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock*4+2] =
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock*4+3] = 0;
            } else {
                    ownQuantLuma8x8Inv_H264(pTransform + uBlock*64, QP_DIV_6[uMBQP], core_enc->m_SeqParamSet.seq_scaling_inv_matrix_8x8[1][QP_MOD_6[uMBQP]]); //scaling matrix for INTER slice
                    ownTransformLuma8x8InvAddPred_H264(pPredBuf, 16, pTransform + uBlock*64, pReconBuf, pitchPix, core_enc->m_PicParamSet.bit_depth_luma);
            }
            uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock] * 2;
        }
    } else {
      if( core_enc->m_params.quant_opt_level > OPT_QUANT_INTER_RD ){
//Save current cabac state
        memcpy( cbSt.absLevelM1, &curr_slice->m_pbitstream->context_array[227+20], 10*sizeof(CABAC_CONTEXT));
        if( !is_cur_mb_field ){
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[105+29], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[166+29], 15*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[105+29];
            cbSt.last = &curr_slice->m_pbitstream->context_array[166+29];
        }else{
            //memcpy( cbSt.sig, &curr_slice->m_pbitstream->context_array[105+172+29], 15*sizeof(CABAC_CONTEXT));
            //memcpy( cbSt.last, &curr_slice->m_pbitstream->context_array[166+172+29], 15*sizeof(CABAC_CONTEXT));
            cbSt.sig = &curr_slice->m_pbitstream->context_array[105+172+29];
            cbSt.last = &curr_slice->m_pbitstream->context_array[166+172+29];
        }
     }

        Ipp32s iNumCoeffs[16], CoeffsCost[16] = {9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9};
        for (uBlock = 0; uBlock < 16; uBlock++ ){
            pPredBuf = cur_mb.mbInter.prediction + xoff[uBlock] + yoff[uBlock]*16;

            // check if block is coded
            bCoded = ((uCBPLuma & CBP4x4Mask[uBlock])?(1):(0));

            if( bCoded ){   // block not declared empty, encode
                Diff4x4(pPredBuf, pSrcPlane + uOffset, pitchPixels, pDiffBuf);
                pTransformResult = pTransform + uBlock*16;
                if( core_enc->m_params.quant_opt_level > OPT_QUANT_INTER_RD ){
                    ownTransformQuantResidual_H264(
                        pDiffBuf,
                        pTransformResult,
                        uMBQP,
                        &iNumCoeffs[uBlock],
                        0,
                        enc_single_scan[is_cur_mb_field],
                        &iLastCoeff,
                        NULL,
                        curr_slice,
                        0,
                        &cbSt);
                }else{
                    ownTransformQuantResidual_H264(
                        pDiffBuf,
                        pTransformResult,
                        uMBQP,
                        &iNumCoeffs[uBlock],
                        0,
                        enc_single_scan[is_cur_mb_field],
                        &iLastCoeff,
                        NULL,
                        NULL,
                        0,
                        NULL);
                }
                CoeffsCost[uBlock] = CalculateCoeffsCost(pTransformResult, 16, dec_single_scan[is_cur_mb_field]);

                if (!iNumCoeffs[uBlock]) { // if everything quantized to zero, skip RLE
                    uCBPLuma &= ~CBP4x4Mask[uBlock];
                }else{
                    // Preserve the absolute number of coeffs.
                    if (core_enc->m_PicParamSet.entropy_coding_mode) {
                        T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
//                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs[uBlock] < 0) ? -(iNumCoeffs[uBlock]+1) : iNumCoeffs[uBlock]);
                        c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs[uBlock]);
                        c_data->uLastSignificant = iLastCoeff;
                        c_data->CtxBlockCat = BLOCK_LUMA_LEVELS;
                        c_data->uFirstCoeff = 0;
                        c_data->uLastCoeff = 15;
                        H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan[is_cur_mb_field], c_data);
                    } else {
                        cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)ABS(iNumCoeffs[uBlock]);
                        ownEncodeCoeffsCAVLC_H264(
                            pTransformResult,
                            0,
                            dec_single_scan[is_cur_mb_field],
                            iLastCoeff,
                            &curr_slice->Block_RLE[uBlock].uTrailing_Ones,
                            &curr_slice->Block_RLE[uBlock].uTrailing_One_Signs,
                            &curr_slice->Block_RLE[uBlock].uNumCoeffs,
                            &curr_slice->Block_RLE[uBlock].uTotalZeros,
                            curr_slice->Block_RLE[uBlock].iLevels,
                            curr_slice->Block_RLE[uBlock].uRuns);
                    }
                }
            }

            // proceed to the next block
            uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock];
        }  // for 4x4 uBlock in luma plane

        //Skip subblock 8x8 if it cost is < 4 or skip MB if it's cost is < 5
            Ipp32s mbCost=0;
            for( uBlock = 0; uBlock < 4; uBlock++ ){
                Ipp32s sb = uBlock*4;
                Ipp32s block8x8cost = CoeffsCost[sb] + CoeffsCost[sb+1] + CoeffsCost[sb+2] + CoeffsCost[sb+3];

                mbCost += block8x8cost;
                if( block8x8cost <= LUMA_8X8_MAX_COST && core_enc->m_params.quant_opt_level < OPT_QUANT_INTER_RD+1)
                    uCBPLuma &= ~CBP8x8Mask[uBlock];
            }
                if( mbCost <= LUMA_MB_MAX_COST )
                    uCBPLuma = 0;

        //Make inverse quantization and transform for non zero blocks
        uOffset = core_enc->m_pMBOffsets[uMB].uLumaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
        for( uBlock=0; uBlock < 16; uBlock++ ){
            pPredBuf = cur_mb.mbInter.prediction + xoff[uBlock] + yoff[uBlock]*16;
            pReconBuf = cur_mb.mbInter.reconstruct + xoff[uBlock] + yoff[uBlock]*16;
            //pReconBuf = core_enc->m_pReconstructFrame->m_pYPlane + uOffset;

            bCoded = ((uCBPLuma & CBP4x4Mask[uBlock])?(1):(0));
            if (!bCoded) {
                // update reconstruct frame for the empty block
                Copy4x4(pPredBuf, 16, pReconBuf, pitchPix);
                cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;
            } else {
                 ownDequantTransformResidualAndAdd_H264(
                     pPredBuf,
                     pTransform + uBlock*16,
                     NULL,
                     pReconBuf,
                     16,
                     pitchPix,
                     uMBQP,
                     ((iNumCoeffs[uBlock] < -1) || (iNumCoeffs[uBlock] > 0)),
                     core_enc->m_PicParamSet.bit_depth_luma,
                     NULL);
              }
            uOffset += core_enc->m_EncBlockOffsetInc[is_cur_mb_field][uBlock];
           }
   }
    cur_mb.LocalMacroblockInfo->cbp_luma = uCBPLuma;
    return 1;
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_TransQuantChromaIntra_RD(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u  uBlock;         // block number, 0 to 23
    Ipp32u  uOffset;        // to upper left corner of block from start of plane
    Ipp32u  uMBQP;          // QP of current MB
    Ipp32u  uMB;
    PIXTYPE*  pSrcPlane;    // start of plane to encode
    Ipp32s    pitchPixels;  // buffer pitch
    COEFFSTYPE *pDCBuf;     // chroma & luma dc coeffs pointer
    PIXTYPE*  pPredBuf;     // prediction block pointer
    PIXTYPE*  pReconBuf;     // prediction block pointer
    PIXTYPE*  pPredBuf_copy;     // prediction block pointer
    COEFFSTYPE* pQBuf;      // quantized block pointer
    Ipp16s* pMassDiffBuf;   // difference block pointer

    Ipp32u   uCBPChroma;    // coded flags for all chroma blocks
    Ipp32s   iNumCoeffs;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s   iLastCoeff;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s   RLE_Offset;    // Index into BlockRLE array

    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;
    EnumSliceType slice_type = curr_slice->m_slice_type;
    COEFFSTYPE *pTransformResult;
    COEFFSTYPE *pTransform;

    pitchPixels = cur_mb.mbPitchPixels;
    uMBQP       = cur_mb.chromaQP;
    pTransform = (COEFFSTYPE*)curr_slice->m_pMBEncodeBuffer;
    pQBuf       = (COEFFSTYPE*) (pTransform + 64*2);
    pDCBuf      = (COEFFSTYPE*) (pQBuf + 16);   // Used for both luma and chroma DC blocks
    pMassDiffBuf= (Ipp16s*) (pDCBuf+ 16);
    Ipp16s*  pTempDiffBuf;
    uMB = cur_mb.uMB;

    // initialize pointers and offset
    uOffset = core_enc->m_pMBOffsets[uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
//    uCBPChroma  = cur_mb.LocalMacroblockInfo->cbp_chroma;
    uCBPChroma  = cur_mb.LocalMacroblockInfo->cbp_chroma = 0xffffffff;
    cur_mb.MacroblockCoeffsInfo->chromaNC = 0;

    pPredBuf = cur_mb.mbChromaIntra.prediction;
        // initialize pointers for the U plane blocks
        Ipp32s num_blocks = 2<<core_enc->m_PicParamSet.chroma_format_idc;
        Ipp32s startBlock;
        startBlock = uBlock = 16;
        Ipp32u uLastBlock = uBlock+num_blocks;
        Ipp32u uFinalBlock = uBlock+2*num_blocks;

        // encode first chroma U plane then V plane
        do{
            // Adjust the pPredBuf to point at the V plane predictor when appropriate:
            // (blocks and an INTRA Y plane mode...)
            if (uBlock == uLastBlock) {
                startBlock = uBlock;
                uOffset = core_enc->m_pMBOffsets[uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
                pSrcPlane = core_enc->m_pCurrentFrame->m_pVPlane;
                pPredBuf = cur_mb.mbChromaIntra.prediction+8;
                pReconBuf = cur_mb.mbChromaIntra.reconstruct+8;
                RLE_Offset = V_DC_RLE;
                uLastBlock += num_blocks;
            } else {
                RLE_Offset = U_DC_RLE;
                pSrcPlane = core_enc->m_pCurrentFrame->m_pUPlane;
                pPredBuf = cur_mb.mbChromaIntra.prediction;
                pReconBuf = cur_mb.mbChromaIntra.reconstruct;
            }

            if( core_enc->m_PicParamSet.chroma_format_idc == 2 ){
                ownSumsDiff8x8Blocks4x4(
                    pSrcPlane + uOffset,    // source pels
                    pitchPixels,                 // source pitch
                    pPredBuf,               // predictor pels
                    16,
                    pDCBuf,                 // result buffer
                    pMassDiffBuf);
                // Process second part of 2x4 block for DC coeffs
                ownSumsDiff8x8Blocks4x4(
                    pSrcPlane + uOffset+8*pitchPixels,    // source pels
                    pitchPixels,                 // source pitch
                    pPredBuf+8*16,               // predictor pels
                    16,
                    pDCBuf+4,                 // result buffer
                    pMassDiffBuf+64);   //+Offset for second path
                ownTransformQuantChroma422DC_H264(
                    pDCBuf,
                    pQBuf,
                    uMBQP,
                    &iNumCoeffs,
                    (slice_type == INTRASLICE),
                    1,
                    NULL);
                 // DC values in this block if iNonEmpty is 1.
                cur_mb.MacroblockCoeffsInfo->chromaNC |= (iNumCoeffs != 0);
                if (core_enc->m_PicParamSet.entropy_coding_mode){
                    Ipp32s ctxIdxBlockCat = BLOCK_CHROMA_DC_LEVELS;
                    H264CoreEncoder_ScanSignificant_CABAC(pDCBuf,ctxIdxBlockCat,8,dec_single_scan_p422, &cur_mb.cabac_data[RLE_Offset]);
                }else{
                        ownEncodeChroma422DC_CoeffsCAVLC_H264(
                            pDCBuf,
                            &curr_slice->Block_RLE[RLE_Offset].uTrailing_Ones,
                            &curr_slice->Block_RLE[RLE_Offset].uTrailing_One_Signs,
                            &curr_slice->Block_RLE[RLE_Offset].uNumCoeffs,
                            &curr_slice->Block_RLE[RLE_Offset].uTotalZeros,
                            curr_slice->Block_RLE[RLE_Offset].iLevels,
                            curr_slice->Block_RLE[RLE_Offset].uRuns);
                }
                ownTransformDequantChromaDC422_H264(pDCBuf, uMBQP, NULL);
           }else{
                ownSumsDiff8x8Blocks4x4(
                    pSrcPlane + uOffset,    // source pels
                    pitchPixels,                 // source pitch
                    pPredBuf,               // predictor pels
                    16,
                    pDCBuf,                 // result buffer
                    pMassDiffBuf);
                ownTransformQuantChromaDC_H264(
                    pDCBuf,
                    pQBuf,
                    uMBQP,
                    &iNumCoeffs,
                    (slice_type == INTRASLICE),
                    1,
                    NULL);
                // DC values in this block if iNonEmpty is 1.
                cur_mb.MacroblockCoeffsInfo->chromaNC |= (iNumCoeffs != 0);
                if (core_enc->m_PicParamSet.entropy_coding_mode){
                    Ipp32s ctxIdxBlockCat = BLOCK_CHROMA_DC_LEVELS;
                        H264CoreEncoder_ScanSignificant_CABAC(pDCBuf,ctxIdxBlockCat,4,dec_single_scan_p, &cur_mb.cabac_data[RLE_Offset]);
                }else{
                       ownEncodeChromaDcCoeffsCAVLC_H264(
                           pDCBuf,
                           &curr_slice->Block_RLE[RLE_Offset].uTrailing_Ones,
                           &curr_slice->Block_RLE[RLE_Offset].uTrailing_One_Signs,
                           &curr_slice->Block_RLE[RLE_Offset].uNumCoeffs,
                           &curr_slice->Block_RLE[RLE_Offset].uTotalZeros,
                           curr_slice->Block_RLE[RLE_Offset].iLevels,
                           curr_slice->Block_RLE[RLE_Offset].uRuns);
                }
                ownTransformDequantChromaDC_H264(pDCBuf, uMBQP, NULL);
           }

//Encode croma AC
       Ipp32s coeffsCost = 0;
       pPredBuf_copy = pPredBuf;
       for (uBlock = startBlock; uBlock < uLastBlock; uBlock ++) {
            cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;     // This will be updated if the block is coded
            if (core_enc->m_PicParamSet.entropy_coding_mode){
                cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
            } else {
                curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
                curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
                curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
                curr_slice->Block_RLE[uBlock].uTotalZeros = 15;
            }
                 pTempDiffBuf = pMassDiffBuf + (uBlock-startBlock)*16;
                 pTransformResult = pTransform + (uBlock-startBlock)*16;
                 ownTransformQuantResidual_H264(
                     pTempDiffBuf,
                     pTransformResult,
                     uMBQP,
                     &iNumCoeffs,
                     0,
                     enc_single_scan[is_cur_mb_field],
                     &iLastCoeff,
                     NULL,
                     NULL,
                     0,
                     NULL);
                 coeffsCost += CalculateCoeffsCost(pTransformResult, 15, &dec_single_scan[is_cur_mb_field][1]);

                    // if everything quantized to zero, skip RLE
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                    if (cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock]){  // the block is empty so it is not coded
                       if (core_enc->m_PicParamSet.entropy_coding_mode)
                       {
                            T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
                            c_data->uLastSignificant = iLastCoeff;
                            c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock];
                            c_data->CtxBlockCat = BLOCK_CHROMA_AC_LEVELS;
                            c_data->uFirstCoeff = 1;
                            c_data->uLastCoeff = 15;
                            H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan[is_cur_mb_field], c_data);
                            cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = c_data->uNumSigCoeffs;
                       }
                       else
                       {
                            ownEncodeCoeffsCAVLC_H264(pTransformResult,// pDiffBuf,
                                                        1,
                                                        dec_single_scan[is_cur_mb_field],
                                                        iLastCoeff,
                                                        &curr_slice->Block_RLE[uBlock].uTrailing_Ones,
                                                        &curr_slice->Block_RLE[uBlock].uTrailing_One_Signs,
                                                        &curr_slice->Block_RLE[uBlock].uNumCoeffs,
                                                        &curr_slice->Block_RLE[uBlock].uTotalZeros,
                                                        curr_slice->Block_RLE[uBlock].iLevels,
                                                        curr_slice->Block_RLE[uBlock].uRuns);

                            cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = curr_slice->Block_RLE[uBlock].uNumCoeffs;
                        }
                    }
                pPredBuf += chromaPredInc[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock]; //!!!
       }

       if(coeffsCost <= (CHROMA_COEFF_MAX_COST<<(core_enc->m_PicParamSet.chroma_format_idc-1)) ){ //Reset all ac coeffs
//           memset( pTransform, 0, (64*sizeof(COEFFSTYPE))<<(core_enc->m_PicParamSet.chroma_format_idc-1));
           for(uBlock = startBlock; uBlock < uLastBlock; uBlock++){
                cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;
                if (core_enc->m_PicParamSet.entropy_coding_mode){
                   cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
                } else {
                    curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
                    curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
                    curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
                    curr_slice->Block_RLE[uBlock].uTotalZeros = 15;
                }
           }
       }


       pPredBuf = pPredBuf_copy;
       for (uBlock = startBlock; uBlock < uLastBlock; uBlock ++) {
                   cur_mb.MacroblockCoeffsInfo->chromaNC |= 2*(cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock]!=0);
                    if (!cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] && !pDCBuf[ chromaDCOffset[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock] ]){
                        uCBPChroma &= ~CBP4x4Mask[uBlock-16];
                        Copy4x4(pPredBuf, 16, pReconBuf, 16);
                    }else {
                            ownDequantTransformResidualAndAdd_H264(
                                pPredBuf,
                                pTransform + (uBlock-startBlock)*16,
                                &pDCBuf[ chromaDCOffset[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock] ],
                                pReconBuf,
                                16,
                                16,
                                uMBQP,
                                (cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock]!=0),
                                core_enc->m_SeqParamSet.bit_depth_chroma,
                                NULL);
                    }
                Ipp32s inc = chromaPredInc[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock];
                pPredBuf += inc; //!!!
                pReconBuf += inc;
            }   // for uBlock in chroma plane
    } while (uBlock < uFinalBlock);
    uCBPChroma &= ~(0xffffffff<<(uBlock-16));

    cur_mb.LocalMacroblockInfo->cbp_chroma = uCBPChroma;

    if (cur_mb.MacroblockCoeffsInfo->chromaNC == 3)
        cur_mb.MacroblockCoeffsInfo->chromaNC = 2;

    if ((cur_mb.GlobalMacroblockInfo->mbtype != MBTYPE_INTRA_16x16) && (cur_mb.GlobalMacroblockInfo->mbtype!= MBTYPE_PCM)){
        cur_mb.LocalMacroblockInfo->cbp = (cur_mb.MacroblockCoeffsInfo->chromaNC << 4);
    } else  {
        cur_mb.LocalMacroblockInfo->cbp = 0;
    }
}

template<typename COEFFSTYPE, typename PIXTYPE>
void H264CoreEncoder_TransQuantChromaInter_RD(void* state, H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    H264CoreEncoder<COEFFSTYPE, PIXTYPE>* core_enc = (H264CoreEncoder<COEFFSTYPE, PIXTYPE>*)state;
    Ipp32u  uBlock;         // block number, 0 to 23
    Ipp32u  uOffset;        // to upper left corner of block from start of plane
    Ipp32u  uMBQP;          // QP of current MB
    Ipp32u  uMB;
    PIXTYPE*  pSrcPlane;    // start of plane to encode
    Ipp32s    pitchPixels;  // buffer pitch
    COEFFSTYPE *pDCBuf;     // chroma & luma dc coeffs pointer
    PIXTYPE*  pPredBuf;     // prediction block pointer
    PIXTYPE*  pReconBuf;     // prediction block pointer
    PIXTYPE*  pPredBuf_copy;     // prediction block pointer
    COEFFSTYPE* pQBuf;      // quantized block pointer
    Ipp16s* pMassDiffBuf;   // difference block pointer

    Ipp32u   uCBPChroma;    // coded flags for all chroma blocks
    Ipp32s   iNumCoeffs;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s   iLastCoeff;    // Number of nonzero coeffs after quant (negative if DC is nonzero)
    Ipp32s   RLE_Offset;    // Index into BlockRLE array

    H264CurrentMacroblockDescriptor<COEFFSTYPE, PIXTYPE> &cur_mb = curr_slice->m_cur_mb;
    Ipp32s is_cur_mb_field = curr_slice->m_is_cur_mb_field;
    EnumSliceType slice_type = curr_slice->m_slice_type;
    COEFFSTYPE *pTransformResult;
    COEFFSTYPE *pTransform;
    bool  VPlane;

    pitchPixels = cur_mb.mbPitchPixels;
    uMBQP       = cur_mb.chromaQP;
    pTransform = (COEFFSTYPE*)curr_slice->m_pMBEncodeBuffer;
    pQBuf       = (COEFFSTYPE*) (pTransform + 64*2);
    pDCBuf      = (COEFFSTYPE*) (pQBuf + 16);   // Used for both luma and chroma DC blocks
    pMassDiffBuf= (Ipp16s*) (pDCBuf+ 16);
    Ipp16s*  pTempDiffBuf;
    uMB = cur_mb.uMB;

    // initialize pointers and offset
    uOffset = core_enc->m_pMBOffsets[uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
    uCBPChroma  = cur_mb.LocalMacroblockInfo->cbp_chroma;

    cur_mb.MacroblockCoeffsInfo->chromaNC = 0;

    // initialize pointers for the U plane blocks
    Ipp32s num_blocks = 2<<core_enc->m_PicParamSet.chroma_format_idc;
    Ipp32s startBlock;
    startBlock = uBlock = 16;
    Ipp32u uLastBlock = uBlock+num_blocks;
    Ipp32u uFinalBlock = uBlock+2*num_blocks;

    pPredBuf = cur_mb.mbChromaInter.prediction;
    H264CoreEncoder_MCOneMBChroma(state, curr_slice, pPredBuf);
    // encode first chroma U plane then V plane
    do
    {
        // Adjust the pPredBuf to point at the V plane predictor when appropriate:
        // (blocks and an INTRA Y plane mode...)
        if (uBlock == uLastBlock) {
            startBlock = uBlock;
            uOffset = core_enc->m_pMBOffsets[uMB].uChromaOffset[core_enc->m_is_cur_pic_afrm][is_cur_mb_field];
            pSrcPlane = core_enc->m_pCurrentFrame->m_pVPlane;
            pPredBuf = cur_mb.mbChromaInter.prediction+8;
            pReconBuf = cur_mb.mbChromaInter.reconstruct+8;
            RLE_Offset = V_DC_RLE;
            // initialize pointers for the V plane blocks
            uLastBlock += num_blocks;
            VPlane = true;
        } else {
            RLE_Offset = U_DC_RLE;
            pSrcPlane = core_enc->m_pCurrentFrame->m_pUPlane;
            pPredBuf = cur_mb.mbChromaInter.prediction;
            pReconBuf = cur_mb.mbChromaInter.reconstruct;
            VPlane = false;
        }
        if( core_enc->m_PicParamSet.chroma_format_idc == 2 ){
            ownSumsDiff8x8Blocks4x4(
                pSrcPlane + uOffset,    // source pels
                pitchPixels,                 // source pitch
                pPredBuf,               // predictor pels
                16,
                pDCBuf,                 // result buffer
                pMassDiffBuf);
            // Process second part of 2x4 block for DC coeffs
            ownSumsDiff8x8Blocks4x4(
                pSrcPlane + uOffset+8*pitchPixels,    // source pels
                pitchPixels,                 // source pitch
                pPredBuf+8*16,               // predictor pels
                16,
                pDCBuf+4,                 // result buffer
                pMassDiffBuf+64);   //+Offset for second path
            ownTransformQuantChroma422DC_H264(
                pDCBuf,
                pQBuf,
                uMBQP,
                &iNumCoeffs,
                (slice_type == INTRASLICE),
                1,
                NULL);
             // DC values in this block if iNonEmpty is 1.
             cur_mb.MacroblockCoeffsInfo->chromaNC |= (iNumCoeffs != 0);
            if (core_enc->m_PicParamSet.entropy_coding_mode){
                Ipp32s ctxIdxBlockCat = BLOCK_CHROMA_DC_LEVELS;
                H264CoreEncoder_ScanSignificant_CABAC(pDCBuf,ctxIdxBlockCat,8,dec_single_scan_p422, &cur_mb.cabac_data[RLE_Offset]);
            }else{
                    ownEncodeChroma422DC_CoeffsCAVLC_H264(
                        pDCBuf,
                        &curr_slice->Block_RLE[RLE_Offset].uTrailing_Ones,
                        &curr_slice->Block_RLE[RLE_Offset].uTrailing_One_Signs,
                        &curr_slice->Block_RLE[RLE_Offset].uNumCoeffs,
                        &curr_slice->Block_RLE[RLE_Offset].uTotalZeros,
                        curr_slice->Block_RLE[RLE_Offset].iLevels,
                        curr_slice->Block_RLE[RLE_Offset].uRuns);
            }
            ownTransformDequantChromaDC422_H264(pDCBuf, uMBQP, NULL);
       }else{
            ownSumsDiff8x8Blocks4x4(
                pSrcPlane + uOffset,    // source pels
                pitchPixels,                 // source pitch
                pPredBuf,               // predictor pels
                16,
                pDCBuf,                 // result buffer
                pMassDiffBuf);
            ownTransformQuantChromaDC_H264(
                pDCBuf,
                pQBuf,
                uMBQP,
                &iNumCoeffs,
                (slice_type == INTRASLICE),
                1,
                NULL);
            // DC values in this block if iNonEmpty is 1.
            cur_mb.MacroblockCoeffsInfo->chromaNC |= (iNumCoeffs != 0);
            if (core_enc->m_PicParamSet.entropy_coding_mode){
                Ipp32s ctxIdxBlockCat = BLOCK_CHROMA_DC_LEVELS;
                    H264CoreEncoder_ScanSignificant_CABAC(pDCBuf,ctxIdxBlockCat,4,dec_single_scan_p, &cur_mb.cabac_data[RLE_Offset]);
            }else{
                   ownEncodeChromaDcCoeffsCAVLC_H264(
                       pDCBuf,
                       &curr_slice->Block_RLE[RLE_Offset].uTrailing_Ones,
                       &curr_slice->Block_RLE[RLE_Offset].uTrailing_One_Signs,
                       &curr_slice->Block_RLE[RLE_Offset].uNumCoeffs,
                       &curr_slice->Block_RLE[RLE_Offset].uTotalZeros,
                       curr_slice->Block_RLE[RLE_Offset].iLevels,
                       curr_slice->Block_RLE[RLE_Offset].uRuns);
            }
            ownTransformDequantChromaDC_H264(pDCBuf, uMBQP, NULL);
       }

//Encode croma AC
#ifdef H264_RD_TRELLIS
//Save current cabac state
/*      CabacStates cbSt;
        memcpy( cbSt.absLevelM1, &curr_slice->m_pbitstream->context_array[227+39], 10*sizeof(CABAC_CONTEXT));
        if( !is_cur_mb_field ){
            memcpy( cbSt.sig+1, &curr_slice->m_pbitstream->context_array[105+47], 14*sizeof(CABAC_CONTEXT));
            memcpy( cbSt.last+1, &curr_slice->m_pbitstream->context_array[166+47], 14*sizeof(CABAC_CONTEXT));
        }else{
            memcpy( cbSt.sig+1, &curr_slice->m_pbitstream->context_array[105+172+47], 14*sizeof(CABAC_CONTEXT));
            memcpy( cbSt.last+1, &curr_slice->m_pbitstream->context_array[166+172+47], 14*sizeof(CABAC_CONTEXT));
        }
*/
#endif
       Ipp32s coeffsCost = 0;
       pPredBuf_copy = pPredBuf;
       for (uBlock = startBlock; uBlock < uLastBlock; uBlock ++) {
            cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;     // This will be updated if the block is coded
            if (core_enc->m_PicParamSet.entropy_coding_mode){
                cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
            } else {
                curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
                curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
                curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
                curr_slice->Block_RLE[uBlock].uTotalZeros = 15;
            }

            // check if block is coded
                 pTempDiffBuf = pMassDiffBuf + (uBlock-startBlock)*16;
                 pTransformResult = pTransform + (uBlock-startBlock)*16;
                 ownTransformQuantResidual_H264(
                     pTempDiffBuf,
                     pTransformResult,
                     uMBQP,
                     &iNumCoeffs,
                     0,
                     enc_single_scan[is_cur_mb_field],
                     &iLastCoeff,
                     NULL,
                     NULL,
                     0,
                     NULL);
                 coeffsCost += CalculateCoeffsCost(pTransformResult, 15, &dec_single_scan[is_cur_mb_field][1]);

                    // if everything quantized to zero, skip RLE
                    cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = (T_NumCoeffs)((iNumCoeffs < 0) ? -(iNumCoeffs+1) : iNumCoeffs);
                    if (cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock]){  // the block is empty so it is not coded
                       if (core_enc->m_PicParamSet.entropy_coding_mode){
                            T_Block_CABAC_Data<COEFFSTYPE>* c_data = &cur_mb.cabac_data[uBlock];
                            c_data->uLastSignificant = iLastCoeff;
                            c_data->uNumSigCoeffs = cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock];
                            c_data->CtxBlockCat = BLOCK_CHROMA_AC_LEVELS;
                            c_data->uFirstCoeff = 1;
                            c_data->uLastCoeff = 15;
                            H264CoreEncoder_MakeSignificantLists_CABAC(pTransformResult,dec_single_scan[is_cur_mb_field], c_data);
                            cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = c_data->uNumSigCoeffs;
                        } else {
                            ownEncodeCoeffsCAVLC_H264(pTransformResult,// pDiffBuf,
                                                        1,
                                                        dec_single_scan[is_cur_mb_field],
                                                        iLastCoeff,
                                                        &curr_slice->Block_RLE[uBlock].uTrailing_Ones,
                                                        &curr_slice->Block_RLE[uBlock].uTrailing_One_Signs,
                                                        &curr_slice->Block_RLE[uBlock].uNumCoeffs,
                                                        &curr_slice->Block_RLE[uBlock].uTotalZeros,
                                                        curr_slice->Block_RLE[uBlock].iLevels,
                                                        curr_slice->Block_RLE[uBlock].uRuns);

                            cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = curr_slice->Block_RLE[uBlock].uNumCoeffs;
                        }
                    }
                pPredBuf += chromaPredInc[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock]; //!!!
       }

       if(coeffsCost <= (CHROMA_COEFF_MAX_COST<<(core_enc->m_PicParamSet.chroma_format_idc-1)) ){ //Reset all ac coeffs
//           if(cur_mb.MacroblockCoeffsInfo->chromaNC&1) //if we have DC coeffs
//           memset( pTransform, 0, (64*sizeof(COEFFSTYPE))<<(core_enc->m_PicParamSet.chroma_format_idc-1));
           for(uBlock = startBlock; uBlock < uLastBlock; uBlock++){
                cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] = 0;
                if (core_enc->m_PicParamSet.entropy_coding_mode){
                   cur_mb.cabac_data[uBlock].uNumSigCoeffs = 0;
                } else {
                    curr_slice->Block_RLE[uBlock].uNumCoeffs = 0;
                    curr_slice->Block_RLE[uBlock].uTrailing_Ones = 0;
                    curr_slice->Block_RLE[uBlock].uTrailing_One_Signs = 0;
                    curr_slice->Block_RLE[uBlock].uTotalZeros = 15;
                }
           }
       }
//#endif

       pPredBuf = pPredBuf_copy;
       for (uBlock = startBlock; uBlock < uLastBlock; uBlock ++) {
                    cur_mb.MacroblockCoeffsInfo->chromaNC |= 2*(cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock]!=0);

                    if (!cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock] && !pDCBuf[ chromaDCOffset[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock] ]){
                        uCBPChroma &= ~CBP4x4Mask[uBlock-16];
                        Copy4x4(pPredBuf, 16, pReconBuf, 16);
                    }else{
                            ownDequantTransformResidualAndAdd_H264(
                                pPredBuf,
                                pTransform + (uBlock-startBlock)*16,
                                &pDCBuf[ chromaDCOffset[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock] ],
                                pReconBuf,
                                16,
                                16,
                                uMBQP,
                                (cur_mb.MacroblockCoeffsInfo->numCoeff[uBlock]!=0),
                                core_enc->m_SeqParamSet.bit_depth_chroma,
                                NULL);
                    }
                Ipp32s inc = chromaPredInc[core_enc->m_PicParamSet.chroma_format_idc-1][uBlock-startBlock];
                pPredBuf += inc; //!!!
                pReconBuf += inc; //!!!
            }   // for uBlock in chroma plane
    } while (uBlock < uFinalBlock);

    //Reset other chroma
    uCBPChroma &= ~(0xffffffff<<(uBlock-16));

    cur_mb.LocalMacroblockInfo->cbp_chroma = uCBPChroma;

    if (cur_mb.MacroblockCoeffsInfo->chromaNC == 3)
        cur_mb.MacroblockCoeffsInfo->chromaNC = 2;
}


// forced instantiation
#ifdef BITDEPTH_9_12
#define COEFFSTYPE Ipp32s
#define PIXTYPE Ipp16u
template void   H264CoreEncoder_CEncAndRecMB<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template void   H264CoreEncoder_TransQuantIntra16x16_RD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template void   H264CoreEncoder_TransQuantChromaInter_RD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template Ipp32u H264CoreEncoder_TransQuantInter_RD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template Ipp32u H264CoreEncoder_TransQuantIntra_RD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template void   H264CoreEncoder_TransQuantChromaIntra_RD(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template void   H264CoreEncoder_Encode4x4IntraBlock<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32s);
template void   H264CoreEncoder_Encode8x8IntraBlock<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32s);

#undef COEFFSTYPE
#undef PIXTYPE
#endif

#define COEFFSTYPE Ipp16s
#define PIXTYPE Ipp8u
template void   H264CoreEncoder_CEncAndRecMB<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template void   H264CoreEncoder_TransQuantIntra16x16_RD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template void   H264CoreEncoder_TransQuantChromaInter_RD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template Ipp32u H264CoreEncoder_TransQuantInter_RD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template Ipp32u H264CoreEncoder_TransQuantIntra_RD<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template void   H264CoreEncoder_TransQuantChromaIntra_RD(void*, H264Slice<COEFFSTYPE, PIXTYPE>*);
template void   H264CoreEncoder_Encode4x4IntraBlock<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32s);
template void   H264CoreEncoder_Encode8x8IntraBlock<COEFFSTYPE, PIXTYPE>(void*, H264Slice<COEFFSTYPE, PIXTYPE>*, Ipp32s);

#endif //UMC_ENABLE_H264_VIDEO_ENCODER


