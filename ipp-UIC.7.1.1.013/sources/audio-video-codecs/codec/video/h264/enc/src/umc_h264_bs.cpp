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
#include <string.h>
#include "umc_h264_bs.h"
#include "umc_h264_video_encoder.h"
#include "umc_h264_core_enc.h"
#include "umc_h264_tables.h"

// Bit stream field sizes
typedef struct {
    Ipp8u code;
    Ipp8u len;
} struct_CodeEntry;

static const struct_CodeEntry EncTotalCoeff[5][17][4] =
{
    {   // Num-VLC0
        //   0       1       2       3  <-- Trailing Ones
        { { 1, 1},{ 0, 0},{ 0, 0},{ 0, 0} }, // 0 Total_Coeffs
        { { 5, 6},{ 1, 2},{ 0, 0},{ 0, 0} }, // 1 Total_Coeffs
        { { 7, 8},{ 4, 6},{ 1, 3},{ 0, 0} }, // 2 Total_Coeffs
        { { 7, 9},{ 6, 8},{ 5, 7},{ 3, 5} }, // 3 Total_Coeffs
        { { 7,10},{ 6, 9},{ 5, 8},{ 3, 6} }, // 4 Total_Coeffs
        { { 7,11},{ 6,10},{ 5, 9},{ 4, 7} }, // 5 Total_Coeffs
        { {15,13},{ 6,11},{ 5,10},{ 4, 8} }, // 6 Total_Coeffs
        { {11,13},{14,13},{ 5,11},{ 4, 9} }, // 7 Total_Coeffs
        { { 8,13},{10,13},{13,13},{ 4,10} }, // 8 Total_Coeffs
        { {15,14},{14,14},{ 9,13},{ 4,11} }, // 9 Total_Coeffs
        { {11,14},{10,14},{13,14},{12,13} }, // 10 Total_Coeffs
        { {15,15},{14,15},{ 9,14},{12,14} }, // 11 Total_Coeffs
        { {11,15},{10,15},{13,15},{ 8,14} }, // 12 Total_Coeffs
        { {15,16},{ 1,15},{ 9,15},{12,15} }, // 13 Total_Coeffs
        { {11,16},{14,16},{13,16},{ 8,15} }, // 14 Total_Coeffs
        { { 7,16},{10,16},{ 9,16},{12,16} }, // 15 Total_Coeffs
        { { 4,16},{ 6,16},{ 5,16},{ 8,16} }  // 16 Total_Coeffs

    },
    {   // Num-VLC1
        //   0       1       2       3  <-- Trailing Ones
        { { 3, 2},{ 0, 0},{ 0, 0},{ 0, 0} }, // 0 Total_Coeffs
        { {11, 6},{ 2, 2},{ 0, 0},{ 0, 0} }, // 1 Total_Coeffs
        { { 7, 6},{ 7, 5},{ 3, 3},{ 0, 0} }, // 2 Total_Coeffs
        { { 7, 7},{10, 6},{ 9, 6},{ 5, 4} }, // 3 Total_Coeffs
        { { 7, 8},{ 6, 6},{ 5, 6},{ 4, 4} }, // 4 Total_Coeffs
        { { 4, 8},{ 6, 7},{ 5, 7},{ 6, 5} }, // 5 Total_Coeffs
        { { 7, 9},{ 6, 8},{ 5, 8},{ 8, 6} }, // 6 Total_Coeffs
        { {15,11},{ 6, 9},{ 5, 9},{ 4, 6} }, // 7 Total_Coeffs
        { {11,11},{14,11},{13,11},{ 4, 7} }, // 8 Total_Coeffs
        { {15,12},{10,11},{ 9,11},{ 4, 9} }, // 9 Total_Coeffs
        { {11,12},{14,12},{13,12},{12,11} }, // 10 Total_Coeffs
        { { 8,12},{10,12},{ 9,12},{ 8,11} }, // 11 Total_Coeffs
        { {15,13},{14,13},{13,13},{12,12} }, // 12 Total_Coeffs
        { {11,13},{10,13},{ 9,13},{12,13} }, // 13 Total_Coeffs
        { { 7,13},{11,14},{ 6,13},{ 8,13} }, // 14 Total_Coeffs
        { { 9,14},{ 8,14},{10,14},{ 1,13} }, // 15 Total_Coeffs
        { { 7,14},{ 6,14},{ 5,14},{ 4,14} }  // 16 Total_Coeffs
    },
    {   // Num-VLC2
        //   0       1       2       3  <-- Trailing Ones
        { {15, 4},{ 0, 0},{ 0, 0},{ 0, 0} }, // 0 Total_Coeffs
        { {15, 6},{14, 4},{ 0, 0},{ 0, 0} }, // 1 Total_Coeffs
        { {11, 6},{15, 5},{13, 4},{ 0, 0} }, // 2 Total_Coeffs
        { { 8, 6},{12, 5},{14, 5},{12, 4} }, // 3 Total_Coeffs
        { {15, 7},{10, 5},{11, 5},{11, 4} }, // 4 Total_Coeffs
        { {11, 7},{ 8, 5},{ 9, 5},{10, 4} }, // 5 Total_Coeffs
        { { 9, 7},{14, 6},{13, 6},{ 9, 4} }, // 6 Total_Coeffs
        { { 8, 7},{10, 6},{ 9, 6},{ 8, 4} }, // 7 Total_Coeffs
        { {15, 8},{14, 7},{13, 7},{13, 5} }, // 8 Total_Coeffs
        { {11, 8},{14, 8},{10, 7},{12, 6} }, // 9 Total_Coeffs
        { {15, 9},{10, 8},{13, 8},{12, 7} }, // 10 Total_Coeffs
        { {11, 9},{14, 9},{ 9, 8},{12, 8} }, // 11 Total_Coeffs
        { { 8, 9},{10, 9},{13, 9},{ 8, 8} }, // 12 Total_Coeffs
        { {13,10},{ 7, 9},{ 9, 9},{12, 9} }, // 13 Total_Coeffs
        { { 9,10},{12,10},{11,10},{10,10} }, // 14 Total_Coeffs
        { { 5,10},{ 8,10},{ 7,10},{ 6,10} }, // 15 Total_Coeffs
        { { 1,10},{ 4,10},{ 3,10},{ 2,10} }  // 16 Total_Coeffs
    },
    {   // Num-VLC_ChromaDC 420
        //   0       1       2       3  <-- Trailing Ones
        { { 1, 2},{ 0, 0},{ 0, 0},{ 0, 0} }, // 0 Total_Coeffs
        { { 7, 6},{ 1, 1},{ 0, 0},{ 0, 0} }, // 1 Total_Coeffs
        { { 4, 6},{ 6, 6},{ 1, 3},{ 0, 0} }, // 2 Total_Coeffs
        { { 3, 6},{ 3, 7},{ 2, 7},{ 5, 6} }, // 3 Total_Coeffs
        { { 2, 6},{ 3, 8},{ 2, 8},{ 0, 7} }, // 4 Total_Coeffs
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} }, // Not used...
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} }
    },
    {   // Num-VLC_ChromaDC 422
        //   0       1       2       3  <-- Trailing Ones
        { { 1,  1},{ 0,  0},{ 0,  0},{ 0, 0} }, // 0 Total_Coeffs
        { { 15, 7},{ 1,  2},{ 0,  0},{ 0, 0} }, // 1 Total_Coeffs
        { { 14, 7},{ 13, 7},{ 1,  3},{ 0, 0} }, // 2 Total_Coeffs
        { {  7, 9},{ 12, 7},{ 11, 7},{ 1, 5} }, // 3 Total_Coeffs
        { {  6, 9},{ 5,  9},{ 10, 7},{ 1, 6}  }, // 4 Total_Coeffs
        { { 7, 10},{ 6, 10},{ 4,  9},{ 9, 7} }, // 5 Total_Coeffs
        { { 7, 11},{ 6, 11},{ 5, 10},{ 8, 7} }, // 6 Total_Coeffs
        { { 7, 12},{ 6, 12},{ 5, 11},{ 4, 10} }, // 7 Total_Coeffs
        { { 7, 13},{ 5, 12},{ 4, 12},{ 4, 11} }, // 8 Total_Coeffs
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} }, // Not used...
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} },
        { { 0, 0},{ 0, 0},{ 0, 0},{ 0, 0} }
    }
};

static const struct_CodeEntry EncTotalZerosChroma[3][16][16] = {
    {
    //  0     1     2    <- (TotalCoeff-1)
    { {1,1},{1,1},{1,1} },  // 0 TotalZeros
    { {1,2},{1,2},{0,1} },  // 1 TotalZeros
    { {1,3},{0,2},{0,0} },  // 2 TotalZeros
    { {0,3},{0,0},{0,0} }   // 3 TotalZeros
    },{
    //  0     1     2     3     4     5     6 <- (TotalCoeff-1)
    { {1,1},{0,3},{0,3},{6,3},{0,2},{0,2},{0,1} },   // 0 TotalZeros
    { {2,3},{1,2},{1,3},{0,2},{1,2},{1,2},{1,1} },   // 1 TotalZeros
    { {3,3},{1,3},{1,2},{1,2},{2,2},{1,1},{0,0} },   // 2 TotalZeros
    { {2,4},{4,3},{2,2},{2,2},{3,2},{0,0},{0,0} },   // 3 TotalZeros
    { {3,4},{5,3},{6,3},{7,3},{0,0},{0,0},{0,0} },   // 4 TotalZeros
    { {1,4},{6,3},{7,3},{0,0},{0,0},{0,0},{0,0} },   // 5 TotalZeros
    { {1,5},{7,3},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 6 TotalZeros
    { {0,5},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 7 TotalZeros
    },{
    //  0     1     2     3     4     5     6     7     8     9     10    11    12    13    14 <- (TotalCoeff-1)
    { {1,1},{7,3},{5,4},{3,5},{5,4},{1,6},{1,6},{1,6},{1,6},{1,5},{0,4},{0,4},{0,3},{0,2},{0,1} },   // 0 TotalZeros
    { {3,3},{6,3},{7,3},{7,3},{4,4},{1,5},{1,5},{1,4},{0,6},{0,5},{1,4},{1,4},{1,3},{1,2},{1,1} },   // 1 TotalZeros
    { {2,3},{5,3},{6,3},{5,4},{3,4},{7,3},{5,3},{1,5},{1,4},{1,3},{1,3},{1,2},{1,1},{1,1},{0,0} },   // 2 TotalZeros
    { {3,4},{4,3},{5,3},{4,4},{7,3},{6,3},{4,3},{3,3},{3,2},{3,2},{2,3},{1,1},{1,2},{0,0},{0,0} },   // 3 TotalZeros
    { {2,4},{3,3},{4,4},{6,3},{6,3},{5,3},{3,3},{3,2},{2,2},{2,2},{1,1},{1,3},{0,0},{0,0},{0,0} },   // 4 TotalZeros
    { {3,5},{5,4},{3,4},{5,3},{5,3},{4,3},{3,2},{2,2},{1,3},{1,2},{3,3},{0,0},{0,0},{0,0},{0,0} },   // 5 TotalZeros
    { {2,5},{4,4},{4,3},{4,3},{4,3},{3,3},{2,3},{2,3},{1,2},{1,4},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 6 TotalZeros
    { {3,6},{3,4},{3,3},{3,4},{3,3},{2,3},{1,4},{1,3},{1,5},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 7 TotalZeros
    { {2,6},{2,4},{2,4},{3,3},{2,4},{1,4},{1,3},{0,6},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 8 TotalZeros
    { {3,7},{3,5},{3,5},{2,4},{1,5},{1,3},{0,6},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 9 TotalZeros
    { {2,7},{2,5},{2,5},{2,5},{1,4},{0,6},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 10 TotalZeros
    { {3,8},{3,6},{1,6},{1,5},{0,5},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 11 TotalZeros
    { {2,8},{2,6},{1,5},{0,5},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 12 TotalZeros
    { {3,9},{1,6},{0,6},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 13 TotalZeros
    { {2,9},{0,6},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 14 TotalZeros
    { {1,9},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0} },   // 15 TotalZeros
    }
};

static const struct_CodeEntry EncTotalZeros4x4[16][16] = {
    //   0/7   1/8   2/9  3/10  4/11  5/12  6/13   14  <- (TotalCoeff-1)
    {   {1,1},{7,3},{5,4},{3,5},{5,4},{1,6},{1,6},
        {1,6},{1,6},{1,5},{0,4},{0,4},{0,3},{0,2},{0,1}, {0,0} },  // 0 TotalZeros
    {   {3,3},{6,3},{7,3},{7,3},{4,4},{1,5},{1,5},
        {1,4},{0,6},{0,5},{1,4},{1,4},{1,3},{1,2},{1,1}, {0,0} },  // 1 TotalZeros
    {   {2,3},{5,3},{6,3},{5,4},{3,4},{7,3},{5,3},
        {1,5},{1,4},{1,3},{1,3},{1,2},{1,1},{1,1},{0,0}, {0,0} },  // 2 TotalZeros
    {   {3,4},{4,3},{5,3},{4,4},{7,3},{6,3},{4,3},
        {3,3},{3,2},{3,2},{2,3},{1,1},{1,2},{0,0},{0,0}, {0,0} },  // 3 TotalZeros
    {   {2,4},{3,3},{4,4},{6,3},{6,3},{5,3},{3,3},
        {3,2},{2,2},{2,2},{1,1},{1,3},{0,0},{0,0},{0,0}, {0,0} },  // 4 TotalZeros
    {   {3,5},{5,4},{3,4},{5,3},{5,3},{4,3},{3,2},
        {2,2},{1,3},{1,2},{3,3},{0,0},{0,0},{0,0},{0,0}, {0,0} },  // 5 TotalZeros
    {   {2,5},{4,4},{4,3},{4,3},{4,3},{3,3},{2,3},
        {2,3},{1,2},{1,4},{0,0},{0,0},{0,0},{0,0},{0,0}, {0,0} },  // 6 TotalZeros
    {   {3,6},{3,4},{3,3},{3,4},{3,3},{2,3},{1,4},
        {1,3},{1,5},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, {0,0} },  // 7 TotalZeros
    {   {2,6},{2,4},{2,4},{3,3},{2,4},{1,4},{1,3},
        {0,6},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, {0,0} },  // 8 TotalZeros
    {   {3,7},{3,5},{3,5},{2,4},{1,5},{1,3},{0,6},
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, {0,0} },  // 9 TotalZeros
    {   {2,7},{2,5},{2,5},{2,5},{1,4},{0,6},{0,0},
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, {0,0} },  // 10 TotalZeros
    {   {3,8},{3,6},{1,6},{1,5},{0,5},{0,0},{0,0},
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, {0,0} },  // 11 TotalZeros
    {   {2,8},{2,6},{1,5},{0,5},{0,0},{0,0},{0,0},
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, {0,0} },  // 12 TotalZeros
    {   {3,9},{1,6},{0,6},{0,0},{0,0},{0,0},{0,0},
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, {0,0} },  // 13 TotalZeros
    {   {2,9},{0,6},{0,0},{0,0},{0,0},{0,0},{0,0},
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, {0,0} },  // 14 TotalZeros
    {   {1,9},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},
        {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{0,0}, {0,0} }   // 15 TotalZeros
};

static const struct_CodeEntry EncRuns[15][8] = {
    //    0     1     2     3     4     5     6+     <- (ZerosLeft-1)
    {   {1,1},{1,1},{3,2},{3,2},{3,2},{3,2},{7,3}, {0,0} }, // 0 RunBefore
    {   {0,1},{1,2},{2,2},{2,2},{2,2},{0,3},{6,3}, {0,0} }, // 1 RunBefore
    {   {0,0},{0,2},{1,2},{1,2},{3,3},{1,3},{5,3}, {0,0} }, // 2 RunBefore
    {   {0,0},{0,0},{0,2},{1,3},{2,3},{3,3},{4,3}, {0,0} }, // 3 RunBefore
    {   {0,0},{0,0},{0,0},{0,3},{1,3},{2,3},{3,3}, {0,0} }, // 4 RunBefore
    {   {0,0},{0,0},{0,0},{0,0},{0,3},{5,3},{2,3}, {0,0} }, // 5 RunBefore
    {   {0,0},{0,0},{0,0},{0,0},{0,0},{4,3},{1,3}, {0,0} }, // 6 RunBefore
    {   {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,4}, {0,0} }, // 7 RunBefore
    {   {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,5}, {0,0} }, // 8 RunBefore
    {   {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,6}, {0,0} }, // 9 RunBefore
    {   {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,7}, {0,0} }, // 10 RunBefore
    {   {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,8}, {0,0} }, // 11 RunBefore
    {   {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,9}, {0,0} }, // 12 RunBefore
    {   {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,10}, {0,0}}, // 13 RunBefore
    {   {0,0},{0,0},{0,0},{0,0},{0,0},{0,0},{1,11}, {0,0}}  // 14 RunBefore
};

static const Ipp32s ctx_neq1p1[8] = { 1,2,3,4,0,0,0,0};
static const Ipp32s ctx_ngt1[8] =   { 5,5,5,5,6,7,8,9};
static const Ipp32s ctx_id_trans0[8] = {1,2,3,3,4,5,6,7};
static const Ipp32s ctx_id_trans1[8] = {4,4,4,4,5,6,7,7};
static const Ipp32s ctx_id_trans13[7] = {4,4,4,4,5,6,6};



/////////////////////////////////////





// ---------------------------------------------------------------------------
//  CH264pBs::Reset()
//      reset bitstream; used in the encoder
// ---------------------------------------------------------------------------
void H264BsReal_Reset(
    void* state)
{
    H264BsReal* bs = (H264BsReal *)state;
    H264BsBase_Reset(&bs->m_base);
    bs->m_pbsRBSPBase = bs->m_base.m_pbsBase;
}

// ---------------------------------------------------------------------------
//  CH264pBs::ResetRBSP()
//      reset bitstream to beginning of current RBSP; used in the encoder
// ---------------------------------------------------------------------------
void H264BsReal_ResetRBSP(
    void* state)
{
    H264BsReal* bs = (H264BsReal *)state;
    bs->m_base.m_pbs = bs->m_pbsRBSPBase;
    bs->m_base.m_bitOffset = 0;
    bs->m_base.m_pbs[0] = 0;   // Zero the first byte, since subsequent bits written will be OR'd
                    // with this byte.  Subsequent bytes will be completely overwritten
                    // or zeroed, so no need to clear them out.

}

// ---------------------------------------------------------------------------
//  CH264pBs::EndOfNAL()
// ---------------------------------------------------------------------------
Ipp32u H264BsReal_EndOfNAL(
    void* state,
    Ipp8u* const pout,
    Ipp8u const uIDC,
    NAL_Unit_Type const uUnitType,
    bool& startPicture)
{
    H264BsReal* bs = (H264BsReal *)state;
    Ipp32u size, ExtraBytes;
    Ipp8u* curPtr, *endPtr, *outPtr;

    // get current RBSP compressed size
    size = (Ipp32u)(bs->m_base.m_pbs - bs->m_pbsRBSPBase);
    ExtraBytes = 0;

    // Set Pointers
    endPtr = bs->m_pbsRBSPBase + size - 1;  // Point at Last byte with data in it.
    curPtr = bs->m_pbsRBSPBase;
    outPtr = pout;

    // start access unit => should be zero_byte
    if (startPicture &&
        (((uUnitType >= NAL_UT_SLICE ) && (uUnitType <= NAL_UT_SEI)) ||
        (uUnitType == NAL_UT_AUD) ||
        (((uUnitType >= 0x0e) && (uUnitType <= 0x12)))) ) {
        *outPtr++ = 0;
        ExtraBytes = 1;
        startPicture = false;
    }

    // for SPS and PPS NAL units zero_byte should exist
    if( uUnitType == NAL_UT_SPS || uUnitType == NAL_UT_PPS ) {
        *outPtr++ = 0;
        ExtraBytes = 1;
    }

    *outPtr++ = 0;
    *outPtr++ = 0;
    *outPtr++ = 1;
    *outPtr++ = (Ipp8u) ((uIDC << 5) | uUnitType);  //nal_unit_type
    ExtraBytes += 4;

    if( size == 0 ) return ExtraBytes;

    while (curPtr < endPtr-1) { // Copy all but the last 2 bytes
        *outPtr++ = *curPtr;

        // Check for start code emulation
        if ((*curPtr++ == 0) && (*curPtr == 0) && (!(*(curPtr+1) & 0xfc))) {
            *outPtr++ = *curPtr++;
            *outPtr++ = 0x03;   // Emulation Prevention Byte
            ExtraBytes++;
        }
    }

    if (curPtr < endPtr) *outPtr++ = *curPtr++;
    // copy the last byte
    *outPtr = *curPtr;

    // Update RBSP Base Pointer
    bs->m_pbsRBSPBase = bs->m_base.m_pbs;

    // copy encoded frame to output
    return(size+ExtraBytes);

}

Status H264BsReal_PutSeqExParms(
    void* state,
    const H264SeqParamSet& seq_parms)
{
    H264BsReal_PutVLCCode(state, seq_parms.seq_parameter_set_id);
    H264BsReal_PutVLCCode(state, seq_parms.aux_format_idc);
    if(seq_parms.aux_format_idc != 0) {
        H264BsReal_PutVLCCode(state, seq_parms.bit_depth_aux - 8);
        H264BsReal_PutBit(state, seq_parms.alpha_incr_flag);
        H264BsReal_PutBits(state, seq_parms.alpha_opaque_value, seq_parms.bit_depth_aux + 1);
        H264BsReal_PutBits(state, seq_parms.alpha_transparent_value, seq_parms.bit_depth_aux + 1);
    }
    H264BsReal_PutBit(state, seq_parms.additional_extension_flag);
    return(UMC_OK);
}
// ---------------------------------------------------------------------------
//  CH264pBs::PutSeqParms()
// ---------------------------------------------------------------------------
Status H264BsReal_PutSeqParms(
    void* state,
    const H264SeqParamSet& seq_parms)
{
    Status ps = UMC_OK;

    // Write profile and level information

    H264BsReal_PutBits(state, seq_parms.profile_idc, 8);

    H264BsReal_PutBit(state, seq_parms.constraint_set0_flag);
    H264BsReal_PutBit(state, seq_parms.constraint_set1_flag);
    H264BsReal_PutBit(state, seq_parms.constraint_set2_flag);
    H264BsReal_PutBit(state, seq_parms.constraint_set3_flag);
    H264BsReal_PutBit(state, seq_parms.constraint_set4_flag);
    H264BsReal_PutBit(state, seq_parms.constraint_set5_flag);

    // 2 reserved zero bits
    H264BsReal_PutBits(state, 0, 2);

    H264BsReal_PutBits(state, seq_parms.level_idc, 8);

    // Write the sequence parameter set id
    H264BsReal_PutVLCCode(state, seq_parms.seq_parameter_set_id);

    if(seq_parms.profile_idc == H264_PROFILE_HIGH ||
        seq_parms.profile_idc == H264_PROFILE_HIGH10 ||
        seq_parms.profile_idc == H264_PROFILE_HIGH422 ||
        seq_parms.profile_idc == H264_PROFILE_HIGH444 ||
        seq_parms.profile_idc == H264_PROFILE_CAVLC444)
    {
        H264BsReal_PutVLCCode(state, seq_parms.chroma_format_idc);
        if(seq_parms.chroma_format_idc == 3) {
            H264BsReal_PutBit(state, seq_parms.residual_colour_transform_flag);
        }
        H264BsReal_PutVLCCode(state, seq_parms.bit_depth_luma - 8);
        H264BsReal_PutVLCCode(state, seq_parms.bit_depth_chroma - 8);
        H264BsReal_PutBit(state, seq_parms.qpprime_y_zero_transform_bypass_flag);
        H264BsReal_PutBit(state, seq_parms.seq_scaling_matrix_present_flag);
        if(seq_parms.seq_scaling_matrix_present_flag)
        {
            Ipp32s i;
            bool UseDefaultScalingMatrix;
            for( i=0; i<8 ; i++){
                //Put scaling list present flag
                H264BsReal_PutBit(state, seq_parms.seq_scaling_list_present_flag[i]);
                if( seq_parms.seq_scaling_list_present_flag[i] ){
                   if( i<6 )
                      H264BsReal_PutScalingList(state, &seq_parms.seq_scaling_list_4x4[i][0], 16, UseDefaultScalingMatrix);
                   else
                      H264BsReal_PutScalingList(state, &seq_parms.seq_scaling_list_8x8[i-6][0], 64, UseDefaultScalingMatrix);
                }
            }
        }
    }

    // Write log2_max_frame_num_minus4
    H264BsReal_PutVLCCode(state, seq_parms.log2_max_frame_num - 4);

    // Write pic_order_cnt_type and associated data
    H264BsReal_PutVLCCode(state, seq_parms.pic_order_cnt_type);

    // Write data specific to various pic order cnt types

    // pic_order_cnt_type == 1 is NOT currently supported
    if (seq_parms.pic_order_cnt_type == 0) {
        H264BsReal_PutVLCCode(state, seq_parms.log2_max_pic_order_cnt_lsb - 4);
    }

    // Write num_ref_frames
    H264BsReal_PutVLCCode(state, seq_parms.num_ref_frames);

    // Write required_frame_num_update_behaviour_flag
    H264BsReal_PutBit(state, seq_parms.gaps_in_frame_num_value_allowed_flag);

    // Write picture MB dimensions
    H264BsReal_PutVLCCode(state, seq_parms.frame_width_in_mbs - 1);
    H264BsReal_PutVLCCode(state, seq_parms.frame_height_in_mbs - 1);

    // Write other misc flags
    H264BsReal_PutBit(state, seq_parms.frame_mbs_only_flag);

    if (!seq_parms.frame_mbs_only_flag) {
        H264BsReal_PutBit(state, seq_parms.mb_adaptive_frame_field_flag);
    }

    // Right now, the decoder only supports this flag with
    // a value of zero.
    H264BsReal_PutBit(state, seq_parms.direct_8x8_inference_flag);

    H264BsReal_PutBit(state, seq_parms.frame_cropping_flag);

    if (seq_parms.frame_cropping_flag)  {
        H264BsReal_PutVLCCode(state, seq_parms.frame_crop_left_offset);
        H264BsReal_PutVLCCode(state, seq_parms.frame_crop_right_offset);
        H264BsReal_PutVLCCode(state, seq_parms.frame_crop_top_offset);
        H264BsReal_PutVLCCode(state, seq_parms.frame_crop_bottom_offset);
    }

    H264BsReal_PutBit(state, seq_parms.vui_parameters_present_flag);

    if (seq_parms.vui_parameters_present_flag) {

        H264BsReal_PutBit(state,  seq_parms.vui_parameters.aspect_ratio_info_present_flag );
        if( seq_parms.vui_parameters.aspect_ratio_info_present_flag ){
            H264BsReal_PutBits(state, seq_parms.vui_parameters.aspect_ratio_idc, 8);
            if( seq_parms.vui_parameters.aspect_ratio_idc == 255 ){ // == Extended_SAR
                H264BsReal_PutBits(state, seq_parms.vui_parameters.sar_width,16);
                H264BsReal_PutBits(state, seq_parms.vui_parameters.sar_height,16);
            }
        }

        H264BsReal_PutBit(state,  seq_parms.vui_parameters.overscan_info_present_flag );
        if(seq_parms.vui_parameters.overscan_info_present_flag){
            H264BsReal_PutBit(state,  seq_parms.vui_parameters.overscan_appropriate_flag );
        }

        H264BsReal_PutBit(state,  seq_parms.vui_parameters.video_signal_type_present_flag );
        if(seq_parms.vui_parameters.video_signal_type_present_flag){
            H264BsReal_PutBits(state, seq_parms.vui_parameters.video_format,3);
            H264BsReal_PutBit(state, seq_parms.vui_parameters.video_full_range_flag);
            H264BsReal_PutBit(state, seq_parms.vui_parameters.colour_description_present_flag);
            if(seq_parms.vui_parameters.colour_description_present_flag){
                H264BsReal_PutBits(state, seq_parms.vui_parameters.colour_primaries,8);
                H264BsReal_PutBits(state, seq_parms.vui_parameters.transfer_characteristics,8);
                H264BsReal_PutBits(state, seq_parms.vui_parameters.matrix_coefficients,8);
            }
        }

        H264BsReal_PutBit(state,  seq_parms.vui_parameters.chroma_loc_info_present_flag );
        if(seq_parms.vui_parameters.chroma_loc_info_present_flag){
            H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.chroma_sample_loc_type_top_field);
            H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.chroma_sample_loc_type_bottom_field);
        }

        H264BsReal_PutBit(state,  seq_parms.vui_parameters.timing_info_present_flag );
        if(seq_parms.vui_parameters.timing_info_present_flag){
            H264BsReal_PutBits(state, seq_parms.vui_parameters.num_units_in_tick>>24, 8);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.num_units_in_tick&0x00ffffff, 24); //Due to restrictions of BsTypeType::PutBits
            H264BsReal_PutBits(state, seq_parms.vui_parameters.time_scale>>24, 8);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.time_scale&0x00ffffff, 24);
            H264BsReal_PutBit(state, seq_parms.vui_parameters.fixed_frame_rate_flag);
        }

        H264BsReal_PutBit(state,  seq_parms.vui_parameters.nal_hrd_parameters_present_flag );
        if(seq_parms.vui_parameters.nal_hrd_parameters_present_flag){
            Ipp32s i;

            H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.hrd_params.cpb_cnt_minus1);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.hrd_params.bit_rate_scale, 4);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.hrd_params.cpb_size_scale, 4);
            for( i=0; i <= seq_parms.vui_parameters.hrd_params.cpb_cnt_minus1; i++ ){
                H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.hrd_params.bit_rate_value_minus1[i]);
                H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.hrd_params.cpb_size_value_minus1[i]);
                H264BsReal_PutBit(state, seq_parms.vui_parameters.hrd_params.cbr_flag[i]);
            }
            H264BsReal_PutBits(state, seq_parms.vui_parameters.hrd_params.initial_cpb_removal_delay_length_minus1, 5);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.hrd_params.cpb_removal_delay_length_minus1, 5);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.hrd_params.dpb_output_delay_length_minus1, 5);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.hrd_params.time_offset_length, 5);
        }

        H264BsReal_PutBit(state,  seq_parms.vui_parameters.vcl_hrd_parameters_present_flag );
        if(seq_parms.vui_parameters.vcl_hrd_parameters_present_flag){
            Ipp32s i;

            H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.vcl_hrd_params.cpb_cnt_minus1);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.bit_rate_scale, 4);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.cpb_size_scale, 4);
            for( i=0; i <= seq_parms.vui_parameters.hrd_params.cpb_cnt_minus1; i++ ){
                H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.vcl_hrd_params.bit_rate_value_minus1[i]);
                H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.vcl_hrd_params.cpb_size_value_minus1[i]);
                H264BsReal_PutBit(state, seq_parms.vui_parameters.vcl_hrd_params.cbr_flag[i]);
            }
            H264BsReal_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.initial_cpb_removal_delay_length_minus1, 5);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.cpb_removal_delay_length_minus1, 5);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.dpb_output_delay_length_minus1, 5);
            H264BsReal_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.time_offset_length, 5);
        }

        if( seq_parms.vui_parameters.nal_hrd_parameters_present_flag ||
            seq_parms.vui_parameters.vcl_hrd_parameters_present_flag){
            H264BsReal_PutBit(state, seq_parms.vui_parameters.low_delay_hrd_flag);
        }

        H264BsReal_PutBit(state,  seq_parms.vui_parameters.pic_struct_present_flag );
        H264BsReal_PutBit(state,  seq_parms.vui_parameters.bitstream_restriction_flag );
        if(seq_parms.vui_parameters.bitstream_restriction_flag){
            H264BsReal_PutBit(state, seq_parms.vui_parameters.motion_vectors_over_pic_boundaries_flag);
            H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.max_bytes_per_pic_denom);
            H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.max_bits_per_mb_denom);
            H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.log2_max_mv_length_horizontal);
            H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.log2_max_mv_length_vertical);
            H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.num_reorder_frames);
            H264BsReal_PutVLCCode(state, seq_parms.vui_parameters.max_dec_frame_buffering);
        }
    }

    return ps;
}

// ---------------------------------------------------------------------------
//  CH264pBs::PutPicParms()
// ---------------------------------------------------------------------------
Status H264BsReal_PutPicParms(void* state, const H264PicParamSet& pic_parms, const H264SeqParamSet& seq_parms)
{
    Status ps = UMC_OK;

    // Write IDs
    H264BsReal_PutVLCCode(state, pic_parms.pic_parameter_set_id);
    H264BsReal_PutVLCCode(state, pic_parms.seq_parameter_set_id);

    // Write Entropy coding mode
    H264BsReal_PutBit(state, pic_parms.entropy_coding_mode);
    H264BsReal_PutBit(state, pic_parms.pic_order_present_flag);

    // Only one slice group is currently supported
    // Write num_slice_groups_minus1
    H264BsReal_PutVLCCode(state, pic_parms.num_slice_groups - 1);

    // If multiple slice groups are ever supported, then add code here
    // to write the slice group map information needed to allocate MBs
    // to the defined slice groups.

    // Write num_ref_idx_active counters
    // Right now these are limited to one frame each...
    H264BsReal_PutVLCCode(state, pic_parms.num_ref_idx_l0_active - 1);
    H264BsReal_PutVLCCode(state, pic_parms.num_ref_idx_l1_active - 1);

    // Write some various flags

    // Weighted pred for P slices is not supported
    H264BsReal_PutBit(state, pic_parms.weighted_pred_flag);

    // Explicit weighted BiPred not supported
    // So 0 or 2 are the acceptable values
    H264BsReal_PutBits(state, pic_parms.weighted_bipred_idc, 2);

    // Write quantization values
    H264BsReal_PutVLCCode(state, SIGNED_VLC_CODE(pic_parms.pic_init_qp - 26));
    H264BsReal_PutVLCCode(state, SIGNED_VLC_CODE(pic_parms.pic_init_qs - 26));
    H264BsReal_PutVLCCode(state, SIGNED_VLC_CODE(pic_parms.chroma_qp_index_offset));

    // Write some more flags
    H264BsReal_PutBit(state, pic_parms.deblocking_filter_variables_present_flag);
    H264BsReal_PutBit(state, pic_parms.constrained_intra_pred_flag);
    H264BsReal_PutBit(state, pic_parms.redundant_pic_cnt_present_flag);
    if(seq_parms.profile_idc == H264_PROFILE_HIGH ||
       seq_parms.profile_idc == H264_PROFILE_HIGH10 ||
       seq_parms.profile_idc == H264_PROFILE_HIGH422 ||
       seq_parms.profile_idc == H264_PROFILE_HIGH444)
    {
        H264BsReal_PutBit(state, pic_parms.transform_8x8_mode_flag);
        H264BsReal_PutBit(state, pic_parms.pic_scaling_matrix_present_flag);
        if(pic_parms.pic_scaling_matrix_present_flag)
        { /* scaling matrices coding is not supported */
            VM_ASSERT(0);
            // TO DO: add scaling matrices coding.
        }
        H264BsReal_PutVLCCode(state, SIGNED_VLC_CODE(pic_parms.second_chroma_qp_index_offset));
    }

    return ps;
}

// ---------------------------------------------------------------------------
//  CH264pBs::PutPicDelimiter()
// ---------------------------------------------------------------------------
Status H264BsReal_PutPicDelimiter(
    void* state,
    EnumPicCodType PicCodType)
{
    Status ps = UMC_OK;

    // Write pic_type
    H264BsReal_PutBits(state, PicCodType, 3);

    return ps;
} // CH264pBs::PutPicDelimiter()

// ---------------------------------------------------------------------------
//  CH264pBs::PutDQUANT()
// ---------------------------------------------------------------------------
void H264BsReal_PutDQUANT(
    void* state,
    const Ipp32u quant,
    const Ipp32u quant_prev)
{
    Ipp32s dquant;

    // compute dquant
    dquant=quant-quant_prev;


    // Get dquant between (QP_RANGE-1) to (-QP_RANGE)  (25 to -26 for JVT)

    if (dquant >= H264_QP_RANGE)
        dquant = dquant - H264_QP_MAX;
    else if (dquant < -H264_QP_RANGE)
        dquant = dquant + H264_QP_MAX;

    H264BsReal_PutVLCCode(state, SIGNED_VLC_CODE(dquant));
}

// ---------------------------------------------------------------------------
//  CH264pBs::PutSliceHeader()
// ---------------------------------------------------------------------------
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264BsReal_PutSliceHeader(
    void* state,
    const H264SliceHeader& slice_hdr,
    const H264PicParamSet& pic_parms,
    const H264SeqParamSet& seq_parms,
    const EnumPicClass& ePictureClass,
    const H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    EnumSliceType slice_type = curr_slice->m_slice_type;
    Status ps = UMC_OK;

    // Write first_mb_in_slice
    H264BsReal_PutVLCCode(state, curr_slice->m_first_mb_in_slice);

    // Write slice_type_idc
    H264BsReal_PutVLCCode(state, slice_type);

    // Write pic_parameter_set_id
    H264BsReal_PutVLCCode(state, slice_hdr.pic_parameter_set_id);

    // Write frame_num (modulo MAX_FN) using log2_max_frame_num bits
    H264BsReal_PutBits(state, slice_hdr.frame_num, seq_parms.log2_max_frame_num);

    // Only write field values if not frame only...
    if (!seq_parms.frame_mbs_only_flag) {
        // Write field_pic_flag
        H264BsReal_PutBit(state, slice_hdr.field_pic_flag);

        // Write bottom_field_flag
        if (slice_hdr.field_pic_flag == 1) {
            H264BsReal_PutBit(state, slice_hdr.bottom_field_flag);
        }
    }

    if (ePictureClass == IDR_PIC) {
        H264BsReal_PutVLCCode(state, slice_hdr.idr_pic_id);
    }

    // Write pic_order_cnt info
    if (seq_parms.pic_order_cnt_type == 0) {
        H264BsReal_PutBits(state, slice_hdr.pic_order_cnt_lsb, seq_parms.log2_max_pic_order_cnt_lsb);
        if ((pic_parms.pic_order_present_flag == 1) && (!slice_hdr.field_pic_flag)) {
            H264BsReal_PutVLCCode(state, SIGNED_VLC_CODE(slice_hdr.delta_pic_order_cnt_bottom));
        }
    }

    // Handle Redundant Slice Flag
    if (pic_parms.redundant_pic_cnt_present_flag) {
        H264BsReal_PutVLCCode(state, slice_hdr.redundant_pic_cnt);
    }

    // Write direct_spatial_mv_pred_flag if this is a BPREDSLICE
    if (slice_type == BPREDSLICE) {
        H264BsReal_PutBit(state, slice_hdr.direct_spatial_mv_pred_flag);
    }

    if ((slice_type == BPREDSLICE) ||
        (slice_type == S_PREDSLICE) ||
        (slice_type == PREDSLICE)) {
        // Write num_ref_idx_active_override_flag
        H264BsReal_PutBit(state, curr_slice->num_ref_idx_active_override_flag);
        if (curr_slice->num_ref_idx_active_override_flag) {
            H264BsReal_PutVLCCode(state, curr_slice->num_ref_idx_l0_active - 1);
            if (slice_type == BPREDSLICE) {
                H264BsReal_PutVLCCode(state, curr_slice->num_ref_idx_l1_active - 1);
            }
        }
    }
    // ref_pic_list_reordering() default settings

    if ((slice_type != INTRASLICE) &&
        (slice_type != S_INTRASLICE)) {

        // ref_pic_list_reordering_flag_l0
        H264BsReal_PutBit(state, 0);

        // More data would be inserted here if the above flag is 1

        if (slice_type == BPREDSLICE) {
            // ref_pic_list_reordering_flag_l1
            H264BsReal_PutBit(state, 0);
            // More data would be inserted here if the above flag is 1
        }
    }

    if ((pic_parms.weighted_pred_flag &&
        (slice_type == PREDSLICE || slice_type == S_PREDSLICE)) ||
        ((pic_parms.weighted_bipred_idc == 1) && slice_type == BPREDSLICE)) {
        // Add support for pred_weight_table() ???
    }

    // Write appropriate bits for dec_ref_pic_marking()
    // Note!  Currently there are no structure members for these syntax elements,
    // so the default bits are just written directly.  This need to be updated
    // when appropriate structures are implemented.  TODO - VSI
    if (ePictureClass == IDR_PIC) {
            // no_output_of_prior_pics_flag
            H264BsReal_PutBit(state, 0);
            // long_term_reference_flag
            H264BsReal_PutBit(state, 0);
    } else if (ePictureClass == REFERENCE_PIC) {
            // adaptive_ref_pic_marking_mode_flag
            H264BsReal_PutBit(state, slice_hdr.adaptive_ref_pic_marking_mode_flag);

            // Other things would be written here if memory management control
            // were to be implemented, changing the value of the adaptive_ref_pic_marking_mode_flag
            // written above to 1.
    }

    if (pic_parms.entropy_coding_mode && slice_type != INTRASLICE &&
            slice_type != S_INTRASLICE) {
        H264BsReal_PutVLCCode(state, curr_slice->m_cabac_init_idc);
    }

    // Write slice_qp_delta
    H264BsReal_PutVLCCode(state, SIGNED_VLC_CODE(curr_slice->m_slice_qp_delta));

    // No sp_for_switch_flag or slice_qp_s_delta are written because we don't
    // support SI and SP slices
    if ((slice_type == S_PREDSLICE) || (slice_type == S_INTRASLICE)) {
        if (slice_type == S_PREDSLICE)
            H264BsReal_PutBit(state, slice_hdr.sp_for_switch_flag);
        // Write slice_qp_s_delta
        H264BsReal_PutVLCCode(state, SIGNED_VLC_CODE(slice_hdr.slice_qs_delta));
    }

    // Write Filter Parameters
    if (pic_parms.deblocking_filter_variables_present_flag) {

        // Write disable_deblocking_filter_idc
        H264BsReal_PutVLCCode(state, curr_slice->m_disable_deblocking_filter_idc);

        if (curr_slice->m_disable_deblocking_filter_idc != 1) {

            // Write slice_alpha_c0_offset_div2
            H264BsReal_PutVLCCode(state, SIGNED_VLC_CODE(curr_slice->m_slice_alpha_c0_offset>>1));

            // Write slice_beta_offset_div2
            H264BsReal_PutVLCCode(state, SIGNED_VLC_CODE(curr_slice->m_slice_beta_offset>>1));

        } else {    // If the filter is disabled, both offsets are -51
        }

    } else {    //  If no parms are written, then the filter is ON
    }

    if (pic_parms.num_slice_groups > 1) {
        // Write slice_group_change_cycle
    }

    return ps;

}

Status H264BsReal_PutNumCoeffAndTrailingOnes(
    void* state,
    Ipp32u N,                   // N, obtained from num coeffs of above/left blocks
    Ipp32s bChromaDC,           // True if this is a Chroma DC coeff block (2x2)
    Ipp32u uNumCoeff,           // Number of non-trailing one coeffs to follow (0-4 or 0-16)
    Ipp32u uNumTrailingOnes,    // Number of trailing ones (0-3)
    Ipp32u TrOneSigns)          // Signs of the trailing ones, packed into 3 LSBs, (1==neg)
{
    Ipp32s uVLCSelect = 0;
    Status ps = UMC_OK;

    if (bChromaDC) {
        switch( bChromaDC ){
            case 1:
                uVLCSelect = 3; //nC = -1
                break;
            case 2:
                uVLCSelect = 4; //nC = -2
                break;
            case 3:
                uVLCSelect = 0; //nC = 0
                break;
        }
    } else {

        if (N < 2) {
            uVLCSelect = 0; // 0<=nC<2
        } else if (N < 4) {
            uVLCSelect = 1; // 2<=nC<4
        } else if (N < 8) {
            uVLCSelect = 2; // 4<=nC<8
        } else {  // N > 7  // 8<=nC
            uVLCSelect = -1;    // escape to Fixed Length code
        }

    }
    if (uVLCSelect >= 0) {
        // Write coeff_token from Look-up table

        H264BsReal_PutBits(state, EncTotalCoeff[uVLCSelect][uNumCoeff][uNumTrailingOnes].code,
                EncTotalCoeff[uVLCSelect][uNumCoeff][uNumTrailingOnes].len);

    } else {
        if (uNumCoeff == 0) {
            H264BsReal_PutBits(state, 3, 6);
        } else {
            // //  xxxxyy -> xxxx = uNumCoeff-l , yy = uNumTrailingOnes
            H264BsReal_PutBits(state, (((uNumCoeff-1)<<2)|uNumTrailingOnes), 6);
        }
    }

    // Write signs of NumTrailingOnes
    if( uNumTrailingOnes )
      H264BsReal_PutBits(state, TrOneSigns, uNumTrailingOnes);

    return ps;
}
//#define TRACE_CAVLC

template<typename COEFFSTYPE>
Status H264BsReal_PutLevels(
    void* state,
    COEFFSTYPE* iLevels,      // Array of Levels to write
    Ipp32s      NumLevels,    // Total Number of coeffs to write (0-4 or 0-16)
    Ipp32s      TrailingOnes) // Trailing Ones
{
    Ipp32s VLCSelect = 0;
    Status ps = UMC_OK;
    static const Ipp32s vlc_inc_tab[7] = {0, 3, 6, 12, 24, 48, 0x8000};
    static const Ipp32s escape_tab[7] = {16, 16, 31, 61, 121, 241, 481};
    Ipp32s cnt;
    Ipp32s level_adj = 0;   // Used to flag that the next level
                            // is decreased in magnitude by 1 when coded.

    // Fixup first coeff level if Trailing Ones < 3
    if (TrailingOnes < 3)
    {
        level_adj = 1;  // Subtracted from the level when coded

        if ((TrailingOnes + NumLevels) > 10)
            VLCSelect = 1; // Start with different table
    }
#if defined (TRACE_CAVLC)
    printf("PutLevels: NumLevels = %d, TOnes = %d level_adj = %d\n",
        NumLevels, TrailingOnes, level_adj);
#endif // TRACE_CAVLC
    for (cnt = 0; cnt < NumLevels; cnt++) {
        Ipp32s L = ABS(iLevels[cnt]);
        Ipp32s sign = (L != iLevels[cnt]);

#if defined (TRACE_CAVLC)
        printf("iLevels[%d] = %d, L = %d, sign = %d, VLCSelect = %d, esc = %d\n",
            cnt, iLevels[cnt], L, sign, VLCSelect, escape_tab[VLCSelect]);
        if(L > 2063) {
            printf("PutLevels: L(%d) > 2063, iLevels[%d] = %d, VLCSelect = %d, esc = %d\n",
                L, cnt, iLevels[cnt], VLCSelect, escape_tab[VLCSelect]);
            ps = (Status)cnt;
        }
#endif // TRACE_CAVLC
        L -= level_adj;

        if (L >= escape_tab[VLCSelect]) {  // 28-bit escape
            // Catch cases where the level is too large to write to the BS
            Ipp32u num = L-escape_tab[VLCSelect];

            if( num & (0xffffffff<<11)){
                Ipp32s addbit = 1;
                while(((Ipp32s)num-(2048<<addbit)+2048) >= 0) addbit++;
                addbit--;
                H264BsReal_PutBits(state, 1, 16+addbit);
                H264BsReal_PutBits(state, ((num-(2048<<addbit)+2048)<<1)|sign, 12+addbit);
            }else{
                H264BsReal_PutBits(state, 1, 16); // BsTypeType::PutBits maxes out at 24 bits
                H264BsReal_PutBits(state, (num<<1)|sign, 12);
            }
        } else if (VLCSelect) {    // VLC Level 1-6

            Ipp32s N= VLCSelect - 1;
            H264BsReal_PutBits(state, 1, ((L-1)>>(N))+1);
//          H264BsReal_PutBits(state, (((L-1)%(1<<N))<<1)+sign, VLCSelect);
            H264BsReal_PutBits(state, (((L-1)&((1<<N)-1))<<1)+sign, VLCSelect);
        } else { // VLC Level 0
            if (L < 8) {
                H264BsReal_PutBits(state, 1,sign+((L-1)<<1)+1);  // Start with a 0 if negative
            } else { // L 8-15
                H264BsReal_PutBits(state, 16+((L&7)<<1)+sign,19); // 19 bit escape
            }
        }

        L += level_adj; // Restore the true level for the following calculations

        // Adjust the VLC table depending on the current table and
        // the Level just coded.
        if (!VLCSelect && (L > 3))
            VLCSelect = 2;
        else if (L > vlc_inc_tab[VLCSelect])
            VLCSelect++;

        level_adj = 0;  // Clear this now that it's served its purpose
    }

    return ps;
}

Status H264BsReal_PutTotalZeros(
    void* state,
    Ipp32s TotalZeros,
    Ipp32s TotalCoeffs,
    Ipp32s bChromaDC)
{
    Status ps = UMC_OK;

    TotalCoeffs -= 1;

    if (bChromaDC) {
        H264BsReal_PutBits(state, EncTotalZerosChroma[bChromaDC-1][TotalZeros][TotalCoeffs].code,
            EncTotalZerosChroma[bChromaDC-1][TotalZeros][TotalCoeffs].len);
    } else {
        H264BsReal_PutBits(state, EncTotalZeros4x4[TotalZeros][TotalCoeffs].code,
            EncTotalZeros4x4[TotalZeros][TotalCoeffs].len);
    }

    return ps;
}

Status H264BsReal_PutRuns(
    void* state,
    Ipp8u* uRuns,
    Ipp32s TotalZeros,
    Ipp32s TotalCoeffs)
{
    Status ps = UMC_OK;
    Ipp32s cnt = 0;

    TotalCoeffs--; // Don't write the last run, since it can be inferred.

    while (TotalZeros && (cnt != TotalCoeffs)) {
        Ipp32u zeros_idx = (TotalZeros > 6) ? 6 : TotalZeros-1;
        H264BsReal_PutBits(state, EncRuns[uRuns[cnt]][zeros_idx].code,
            EncRuns[uRuns[cnt]][zeros_idx].len);
        TotalZeros -= uRuns[cnt];
        cnt++;
    }

    return ps;
}

/**************************** cabac **********************************/
Status H264BsReal_MBFieldModeInfo_CABAC(
    void* state,
    Ipp32s mb_field,
    Ipp32s field_available_left,
    Ipp32s field_available_above)
{
    H264BsReal* bs = (H264BsReal *)state;
    H264BsReal_EncodeSingleBin_CABAC(
        state,
        bs->m_base.context_array +
            field_available_left +
            field_available_above +
            ctxIdxOffset[MB_FIELD_DECODING_FLAG],
        mb_field != 0);
    return UMC_OK;
}

Status H264BsReal_MBTypeInfo_CABAC(
    void* state,
    EnumSliceType SliceType,
    Ipp32s mb_type_cur,
    MB_Type type_cur,
    MB_Type type_left,
    MB_Type type_above)
{
    H264BsReal* bs = (H264BsReal *)state;
    Ipp32s a, b,c=0;
    Ipp32s type_temp;
    Ipp32s itype=mb_type_cur-1;
    Ipp8u* ctx;

    if(IS_INTRA_SLICE(SliceType)){
        b = (type_above >= NUMBER_OF_MBTYPES)?0:((type_above != MBTYPE_INTRA) ? 1 : 0 );
        a = (type_left >= NUMBER_OF_MBTYPES)?0:((type_left != MBTYPE_INTRA) ? 1 : 0 );
        ctx = bs->m_base.context_array + ctxIdxOffset[MB_TYPE_I];
encoding_intra:
        if (type_cur==MBTYPE_INTRA){ // 4x4 Intra
            H264BsReal_EncodeSingleBin_CABAC(state, ctx + a + b, 0);
        }else if( type_cur == MBTYPE_PCM){ // PCM-MODE
            H264BsReal_EncodeSingleBin_CABAC(state, ctx + a + b, 1);
            H264BsReal_EncodeFinalSingleBin_CABAC(state, 1);
        }else{ // 16x16 Intra
            H264BsReal_EncodeSingleBin_CABAC(state, ctx + a + b, 1);
            H264BsReal_EncodeFinalSingleBin_CABAC(state, 0);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx + 3 + c, itype / 12 != 0); // coding of AC/no AC
            itype %= 12;
            if (itype<4){
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 4 + c, 0);
            }else{
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 4 + c, 1);
                if (itype>=4 && itype<8){
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                }else{
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                }
            }
            H264BsReal_EncodeSingleBin_CABAC(state, ctx + 6,  (itype&2) != 0 );
            H264BsReal_EncodeSingleBin_CABAC(state, ctx + 7-c,  (itype&1) != 0 );
        }
    }else{ // INTER
        if (IS_B_SLICE(SliceType)){
            type_temp = mb_type_cur;
            b = (type_above >= NUMBER_OF_MBTYPES)?0:(IS_SKIP_MBTYPE(type_above) ? 0 : 1 );
            a = (type_left >= NUMBER_OF_MBTYPES)?0:(IS_SKIP_MBTYPE(type_left) ? 0 : 1 );
            if (mb_type_cur>=24) type_temp=24;
            ctx = bs->m_base.context_array + ctxIdxOffset[MB_TYPE_B];

            if (type_temp==0){
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + a+b,  0);
            }else if (type_temp<=2){
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + a+b,  1);
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 3,  0);
                if (type_temp-1) H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else      H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  0);
            }else if (type_temp<=10){
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + a+b,1);
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 3,  1);
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 4,  0);
                if (((type_temp-3)>>2)&0x01)
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                if (((type_temp-3)>>1)&0x01)
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                if ((type_temp-3)&0x01)
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  0);
            }else if (type_temp==11 || type_temp==11*2){
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + a+b,1);
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 3,  1);
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 4,  1);
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                if (type_temp!=11)
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  0);
            }else{
                if (type_temp > 22 ) type_temp--;

                H264BsReal_EncodeSingleBin_CABAC(state, ctx + a+b,1);
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 3,  1);
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 4,  1);

                if ((type_temp-12)&8)
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                if ((type_temp-12)&4)
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                if ((type_temp-12)&2)
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                if ((type_temp-12)&1)
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                //if (type_temp >12) type_temp++;
            }
        if( type_cur==MBTYPE_PCM ){
            H264BsReal_EncodeFinalSingleBin_CABAC(state, 1);
            return UMC_OK;
        }
        if(type_cur==MBTYPE_INTRA_16x16){
            H264BsReal_EncodeFinalSingleBin_CABAC(state, 0);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx + 6,  (mb_type_cur-24)/12 != 0);
            mb_type_cur %= 12;

            type_temp = mb_type_cur >> 2;
            if (type_temp==0){
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 7,  0);
            }else{
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 7,  1);
                if (type_temp==1){
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 7,  0);
                }else{
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 7,  1);
                }
            }

            H264BsReal_EncodeSingleBin_CABAC(state, ctx + 8,  (mb_type_cur&2) != 0);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx + 8,  (mb_type_cur&1) != 0);
        }

        }else{ // P slice
            ctx = bs->m_base.context_array + ctxIdxOffset[MB_TYPE_P_SP] ;
            if (IS_INTRA_MBTYPE(type_cur)){
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 0,  1);
                a=0;b=3;c=1;itype-=5;
                goto encoding_intra;
            }else{
                H264BsReal_EncodeSingleBin_CABAC(state, ctx + 0,  0);
                switch(mb_type_cur)
                {
                case 0:
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 1,  0);
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 2,  0);
                    break;
                case 1:
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 1,  1);
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 3,  1);
                    break;
                case 2:
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 1,  1);
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 3,  0);
                    break;
                case 3:
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 1,  0);
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx + 2,  1);
                    break;
                default:
                    return UMC_ERR_FAILED;
                }
            }
        }
     }
    return UMC_OK;
}

Status H264BsReal_SubTypeInfo_CABAC(
    void* state,
    EnumSliceType SliceType,
    Ipp32s type)
{
    H264BsReal* bs = (H264BsReal *)state;
    Ipp8u* ctx;
    if (!IS_B_SLICE(SliceType)){
        ctx = bs->m_base.context_array + ctxIdxOffset[SUB_MB_TYPE_P_SP];
        switch (type)
        {
        case 0:
            H264BsReal_EncodeSingleBin_CABAC(state, ctx,1);
            break;
        case 1:
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+0,0);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+1,0);
            break;
        case 2:
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+0,0);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+1,1);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+2,1);
            break;
        case 3:
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+0,0);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+1,1);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+2,0);
            break;
        }
    } else {
        ctx = bs->m_base.context_array + ctxIdxOffset[SUB_MB_TYPE_B];
        if (!type){
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+0,0);
            return UMC_OK;
        } else {
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+0,1);
            type--;
        }

        if (type<2){
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+1,0);
            if (!type)
                H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,0);
            else
                H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,1);
        }else if (type<6){
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+1,1);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+2,0);
            if (((type-2)>>1)&1)
                H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,1);
            else
                H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,0);
            if ((type-2)&1)
                H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,1);
            else
                H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,0);
        } else {
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+1,1);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+2,1);
            if (((type-6)>>2)&1){
                H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,1);
                if ((type-6)&1)
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,1);
                else
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,0);
            } else {
                H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,0);
                if (((type-6)>>1)&1)
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,1);
                else
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,0);
                if ((type-6)&1)
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,1);
                else
                    H264BsReal_EncodeSingleBin_CABAC(state, ctx+3,0);
            }
        }
    }
    return UMC_OK;
}

Status H264BsReal_IntraPredMode_CABAC(
    void* state,
    Ipp32s mode)
{
    H264BsReal* bs = (H264BsReal *)state;
    if (mode == -1)
        H264BsReal_EncodeSingleBin_CABAC(state, bs->m_base.context_array + ctxIdxOffset[PREV_INTRA4X4_PRED_MODE_FLAG],1);
    else{
        H264BsReal_EncodeSingleBin_CABAC(state, bs->m_base.context_array + ctxIdxOffset[PREV_INTRA4X4_PRED_MODE_FLAG],0);
        Ipp8u* ctx = bs->m_base.context_array + ctxIdxOffset[REM_INTRA4X4_PRED_MODE];
        H264BsReal_EncodeSingleBin_CABAC(state, ctx,(mode&1));
        H264BsReal_EncodeSingleBin_CABAC(state, ctx,(mode&2)>>1);
        H264BsReal_EncodeSingleBin_CABAC(state, ctx,(mode&4)>>2);
    }
    return UMC_OK;
}

Status H264BsReal_ChromaIntraPredMode_CABAC(
    void* state,
    Ipp32s mode,
    Ipp32s left_p,
    Ipp32s top_p)
{
    H264BsReal* bs = (H264BsReal *)state;
    Ipp8u* ctx = bs->m_base.context_array + ctxIdxOffset[INTRA_CHROMA_PRED_MODE];
    switch( mode ){
        case 0:
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+left_p+top_p,0);
            break;
        case 1:
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+left_p+top_p,1);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+3, 0);
            break;
        case 2:
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+left_p+top_p,1);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+3, 1);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+3, 0);
            break;
        case 3:
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+left_p+top_p,1);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+3, 1);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+3, 1);
            break;
    }

    return UMC_OK;
}

Status H264BsReal_MVD_CABAC(
    void* state,
    Ipp32s vector,
    Ipp32s left_p,
    Ipp32s top_p,
    Ipp32s contextbase)
{
    H264BsReal* bs = (H264BsReal *)state;
    Ipp32s ctx_mod;

    if (labs(left_p) + labs(top_p) < 3){
        ctx_mod = 0;
    } else {
        if (labs(left_p) + labs(top_p) <= 32)
            ctx_mod = 1;
        else
            ctx_mod = 2;
    }

   Ipp8u* ctx = bs->m_base.context_array + ctxIdxOffset[contextbase];
    if (vector== 0){
        H264BsReal_EncodeSingleBin_CABAC(state, ctx+ctx_mod, 0);
    } else {
        H264BsReal_EncodeSingleBin_CABAC(state, ctx+ctx_mod, 1);
        H264BsReal_EncodeExGRepresentedMVS_CABAC(state, ctx+3,labs(vector)-1);
        H264BsReal_EncodeBypass_CABAC(state, vector<0);
    }
    return UMC_OK;
}

/*!
 ****************************************************************************
 * \brief
 *    This function is used to arithmetically encode the coded
 *    block pattern of a given delta quant.
 ****************************************************************************
 */
Status H264BsReal_DQuant_CABAC(
    void* state,
    Ipp32s deltaQP,
    Ipp32s left_c)
{
    H264BsReal* bs = (H264BsReal *)state;
    Ipp32s value;
    Ipp8u* ctx = bs->m_base.context_array + ctxIdxOffset[MB_QP_DELTA];
    value = 2*labs(deltaQP)+(deltaQP<=0)-1;

    if (deltaQP==0){
      H264BsReal_EncodeSingleBin_CABAC(state, ctx+left_c,0);
    }else{
        H264BsReal_EncodeSingleBin_CABAC(state, ctx+left_c,1);
//      EncodeUnaryRepresentedSymbol_CABAC(ctxIdxOffset[MB_QP_DELTA]+2,1,value-1);
        if (value == 1){
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+2, 0);
        }else{
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+2, 1);
            Ipp32s temp=value-1;
            while ((--temp)>0)
                H264BsReal_EncodeSingleBin_CABAC(state, ctx+2+1, 1);
//          if ((value-1)<0x7fffffff)
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+2+1, 0);
        }
    }
    return UMC_OK;
}

template<typename COEFFSTYPE>
Status H264BsReal_ResidualBlock_CABAC(
    void* state,
    T_Block_CABAC_Data<COEFFSTYPE> *c_data,
    bool frame_block)
{
    H264BsReal* bs = (H264BsReal *)state;
    Ipp32s ctxIdxOffsetSig;
    Ipp32s ctxIdxOffsetLastSig;
    Ipp32s ctxIdxOffsetAbsLevelMinus1;
    Ipp32s firstCoeff = c_data->uFirstCoeff;
    Ipp32s numCoeffs = c_data->uNumSigCoeffs;
    Ipp8u *ctx, *ctx_last, *ctx_sig;
    const Ipp32s* ctx_trans0 = &ctx_id_trans0[0];
    const Ipp32s* ctx_trans1;

    if(c_data->CtxBlockCat == 5) {
        if(frame_block) {
            ctxIdxOffsetSig = ctxIdxOffsetFrameCoded_BlockCat_5[SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetLastSig = ctxIdxOffsetFrameCoded_BlockCat_5[LAST_SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[LAST_SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetAbsLevelMinus1 = ctxIdxOffsetFrameCoded_BlockCat_5[COEFF_ABS_LEVEL_MINUS1];
        }
        else {
            ctxIdxOffsetSig = ctxIdxOffsetFieldCoded_BlockCat_5[SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetLastSig = ctxIdxOffsetFieldCoded_BlockCat_5[LAST_SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[LAST_SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetAbsLevelMinus1 = ctxIdxOffsetFieldCoded_BlockCat_5[COEFF_ABS_LEVEL_MINUS1];
        }
        ctx_sig = bs->m_base.context_array + ctxIdxOffsetSig;
        ctx_last = bs->m_base.context_array + ctxIdxOffsetLastSig;
        ctx_trans1 = &ctx_id_trans1[0];

        Ipp32s i,j;
        for (i=0;i<numCoeffs;i++)
        {
            Ipp32s num=c_data->uSignificantMap[i];
            Ipp32s prevnum = i?c_data->uSignificantMap[i-1]:firstCoeff-1;
            if (num>prevnum+1){
               for (j=prevnum+1;j<num;j++) H264BsReal_EncodeSingleBin_CABAC(state, ctx_sig+Table_9_34[!frame_block][j-firstCoeff],0);
            }
            if (num>=c_data->uLastCoeff) break;
            H264BsReal_EncodeSingleBin_CABAC(state, ctx_sig + Table_9_34[!frame_block][num-firstCoeff],1);
            H264BsReal_EncodeSingleBin_CABAC(state, ctx_last + Table_9_34[2][num-firstCoeff],i==numCoeffs-1);
        }
    }else {
        if(frame_block) {
            ctxIdxOffsetSig = ctxIdxOffsetFrameCoded[SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetLastSig = ctxIdxOffsetFrameCoded[LAST_SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[LAST_SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetAbsLevelMinus1 = ctxIdxOffsetFrameCoded[COEFF_ABS_LEVEL_MINUS1];
        }
        else {
            ctxIdxOffsetSig = ctxIdxOffsetFieldCoded[SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetLastSig = ctxIdxOffsetFieldCoded[LAST_SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[LAST_SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetAbsLevelMinus1 = ctxIdxOffsetFieldCoded[COEFF_ABS_LEVEL_MINUS1];
        }
        ctx_sig = bs->m_base.context_array + ctxIdxOffsetSig;
        ctx_last = bs->m_base.context_array + ctxIdxOffsetLastSig;

        Ipp32s i,j;
        if( c_data->CtxBlockCat == 3 ){
            ctx_trans1 = &ctx_id_trans13[0];
            for (i=0;i<numCoeffs;i++){
                Ipp32s num=c_data->uSignificantMap[i];
                Ipp32s prevnum = i?c_data->uSignificantMap[i-1]:firstCoeff-1;
                if (num > prevnum + 1)
                    for (j = prevnum + 1; j < num; j++)
                        H264BsReal_EncodeSingleBin_CABAC(
                            state,
                            ctx_sig + MIN(((j - firstCoeff) >> bs->num8x8Cshift2), 2),
                            0);

                if (num >= c_data->uLastCoeff)
                    break;
                H264BsReal_EncodeSingleBin_CABAC(
                    state,
                    ctx_sig + MIN(((num - firstCoeff) >> bs->num8x8Cshift2), 2),
                    1);
                H264BsReal_EncodeSingleBin_CABAC(
                    state,
                    ctx_last + MIN(((num-firstCoeff) >> bs->num8x8Cshift2), 2),
                    i == numCoeffs - 1);
            }
        }else{
            ctx_trans1 = &ctx_id_trans1[0];
            for (i=0;i<numCoeffs;i++){
                Ipp32s num=c_data->uSignificantMap[i];
                Ipp32s prevnum = i?c_data->uSignificantMap[i-1]:firstCoeff-1;
                if (num>prevnum+1)
                    for (j=prevnum+1;j<num;j++) H264BsReal_EncodeSingleBin_CABAC(state, ctx_sig + j - firstCoeff,0);
                if ( num >= c_data->uLastCoeff ) break;
                H264BsReal_EncodeSingleBin_CABAC(state, ctx_sig + num - firstCoeff,1);
                H264BsReal_EncodeSingleBin_CABAC(state, ctx_last + num - firstCoeff,i==numCoeffs-1);
            }
        }
    }
    Ipp32s i;
    Ipp32s ctx_id = 0;
    Ipp32s CtxInc;
    COEFFSTYPE* levels = c_data->uSignificantLevels;
    ctx = bs->m_base.context_array + ctxIdxOffsetAbsLevelMinus1 + ctxIdxBlockCatOffset[COEFF_ABS_LEVEL_MINUS1][c_data->CtxBlockCat];
    for (i=numCoeffs-1; i>=0; i--){
        Ipp32s coef=levels[i];
        CtxInc = ctx_neq1p1[ctx_id];
        if (coef){
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+CtxInc, 1);
            H264BsReal_EncodeExGRepresentedLevels_CABAC(state, ctx+ctx_ngt1[ctx_id], coef-1);
            ctx_id = ctx_trans1[ctx_id];
        }else{
            H264BsReal_EncodeSingleBin_CABAC(state, ctx+CtxInc, 0);
            ctx_id = ctx_trans0[ctx_id];
        }
        H264BsReal_EncodeBypass_CABAC(state, c_data->uSignificantSigns[i] != 0);
    }
    return UMC_OK;
}

Status H264BsReal_PutScalingList(
    void* state,
    const Ipp8u* scalingList,
    Ipp32s sizeOfScalingList,
    bool& useDefaultScalingMatrixFlag)
{
    Ipp16s lastScale, nextScale;
    Ipp32s j;

    Ipp16s delta_scale;
    Ipp8s delta_code;
    const Ipp32s* scan;

    lastScale=nextScale=8;

    if( sizeOfScalingList == 16 )
        scan = dec_single_scan[0];
    else
        scan = dec_single_scan_8x8[0];

    for( j = 0; j<sizeOfScalingList; j++ ){
         if( nextScale != 0 ){
            delta_scale = scalingList[scan[j]]-lastScale;
            delta_code = (Ipp8s)(delta_scale);

            //Put delta_scale
            H264BsReal_PutVLCCode(state, SIGNED_VLC_CODE(delta_code));

            nextScale = scalingList[scan[j]];
            useDefaultScalingMatrixFlag = ( j==0 && nextScale == 0 ); //Signal that input matrix is incorrect
     }
         lastScale = (nextScale==0) ? lastScale:nextScale;
    //scalingList[scan[j]] = (nextScale==0) ? lastScale:nextScale; // Update the actual scalingList matrix with the correct values
    //lastScale = scalingList[scan[j]];
    }

    return UMC_OK;
}

Status H264BsReal_PutSEI_UserDataUnregistred(
    void* state,
    void* data,
    Ipp32s data_size )
{
    static const Ipp8u UUID[16] = {
        0x0d, 0xad, 0x3f, 0x34,
        0x8e, 0x9a, 0x48, 0xd7,
        0x8a, 0x7e, 0xc1, 0xe1,
        0xb4, 0x7b, 0xdf, 0x9a
    };

    H264BsReal* bs = (H264BsReal *)state;
    Ipp8u* user_data = (Ipp8u*)data;
    Ipp32s i;

    data_size += 16; //+ UUID size
    //payload type
    H264BsReal_PutBits(state, SEI_TYPE_USER_DATA_UNREGISTERED, 8);
    //payload size
    while( data_size > 255 ){
        H264BsReal_PutBits(state, 0xff, 8);
        data_size -= 255;
    }
    H264BsReal_PutBits(state, data_size, 8);

    //uuid
    for( i=0; i<16; i++ )
        H264BsReal_PutBits(state, UUID[i], 8);

    for( i=0; i<data_size-16; i++ )
        H264BsReal_PutBits(state, user_data[i], 8);

    if (H264BsBase_GetBsOffset(&(bs->m_base)) & 0x07)
        H264BsBase_WriteTrailingBits(&bs->m_base);

    return UMC_OK;
}

Status H264BsReal_PutSEI_BufferingPeriod(
    void* state,
    const H264SeqParamSet& seq_parms,
    const Ipp8u NalHrdBpPresentFlag,
    const Ipp8u VclHrdBpPresentFlag,
    Ipp32u * nal_initial_cpb_removal_delay,
    Ipp32u * nal_initial_cpb_removal_delay_offset,
    Ipp32u * vcl_initial_cpb_removal_delay,
    Ipp32u * vcl_initial_cpb_removal_delay_offset)
{
    Ipp32s SchedSelIdx;
    Ipp32s data_size, data_size_bits;

    Ipp8u seq_parameter_set_id, cpb_cnt_minus1, initial_cpb_removal_delay_length_minus1;

    seq_parameter_set_id = seq_parms.seq_parameter_set_id;

    if (seq_parms.vui_parameters_present_flag)
    {
        if (seq_parms.vui_parameters.nal_hrd_parameters_present_flag)
        {
            cpb_cnt_minus1 = seq_parms.vui_parameters.hrd_params.cpb_cnt_minus1;
            initial_cpb_removal_delay_length_minus1 = seq_parms.vui_parameters.hrd_params.initial_cpb_removal_delay_length_minus1;
        }
        else if (seq_parms.vui_parameters.vcl_hrd_parameters_present_flag)
        {
            cpb_cnt_minus1 = seq_parms.vui_parameters.vcl_hrd_params.cpb_cnt_minus1;
            initial_cpb_removal_delay_length_minus1 = seq_parms.vui_parameters.vcl_hrd_params.initial_cpb_removal_delay_length_minus1;
        }
        else
        {
            cpb_cnt_minus1 = 0;
            initial_cpb_removal_delay_length_minus1 = 23;
        }
    }
    else
    {
        cpb_cnt_minus1 = 0;
        initial_cpb_removal_delay_length_minus1 = 23;
    }

    H264BsReal* bs = (H264BsReal *)state;

    data_size_bits = (Ipp32s)(log(float(seq_parameter_set_id))/log(2.0)) * 2 + 1;
    data_size_bits += (NalHrdBpPresentFlag + VclHrdBpPresentFlag) * (cpb_cnt_minus1 + 1) * (initial_cpb_removal_delay_length_minus1 + 1) * 2;
    data_size = data_size_bits/8;
    data_size += (data_size_bits%8 != 0);

    //payload type
    H264BsReal_PutBits(state, SEI_TYPE_BUFFERING_PERIOD, 8);
    //payload size
    while( data_size > 255 ){
        H264BsReal_PutBits(state, 0xff, 8);
        data_size -= 255;
    }
    H264BsReal_PutBits(state, data_size, 8);

    H264BsReal_PutVLCCode(state, seq_parameter_set_id);

    if (NalHrdBpPresentFlag)
        for (SchedSelIdx = 0; SchedSelIdx <= cpb_cnt_minus1; SchedSelIdx++)
        {
            H264BsReal_PutBits(state, nal_initial_cpb_removal_delay[SchedSelIdx], initial_cpb_removal_delay_length_minus1 + 1);
            H264BsReal_PutBits(state, nal_initial_cpb_removal_delay_offset[SchedSelIdx], initial_cpb_removal_delay_length_minus1 + 1);
        }

    if (VclHrdBpPresentFlag)
        for (SchedSelIdx = 0; SchedSelIdx <= cpb_cnt_minus1; SchedSelIdx++)
        {
            H264BsReal_PutBits(state, vcl_initial_cpb_removal_delay[SchedSelIdx], initial_cpb_removal_delay_length_minus1 + 1);
            H264BsReal_PutBits(state, vcl_initial_cpb_removal_delay_offset[SchedSelIdx], initial_cpb_removal_delay_length_minus1 + 1);
        }

        if (H264BsBase_GetBsOffset(&(bs->m_base)) & 0x07)
            H264BsBase_WriteTrailingBits(&bs->m_base);

    return UMC_OK;
}

Status H264BsReal_PutSEI_PictureTiming(
    void* state,
    const H264SeqParamSet& seq_parms,
    const Ipp8u CpbDpbDelaysPresentFlag,
    const Ipp8u pic_struct_present_flag,
    Ipp32u cpb_removal_delay,
    Ipp32u dpb_output_delay,
    const PicStructTimingData& timing_data)
{
    Ipp32s i, NumClockTS=0;
    H264BsReal* bs = (H264BsReal *)state;
    Ipp32s data_size, data_size_bits = 0;

    Ipp8u cpb_removal_delay_length_minus1, dpb_output_delay_length_minus1, time_offset_length;

    if (seq_parms.vui_parameters_present_flag)
    {
        if (seq_parms.vui_parameters.nal_hrd_parameters_present_flag)
        {
            cpb_removal_delay_length_minus1 =  seq_parms.vui_parameters.hrd_params.cpb_removal_delay_length_minus1;
            dpb_output_delay_length_minus1 =  seq_parms.vui_parameters.hrd_params.dpb_output_delay_length_minus1;
            time_offset_length = seq_parms.vui_parameters.hrd_params.time_offset_length;
        }
        else if (seq_parms.vui_parameters.vcl_hrd_parameters_present_flag)
        {
            cpb_removal_delay_length_minus1 =  seq_parms.vui_parameters.vcl_hrd_params.cpb_removal_delay_length_minus1;
            dpb_output_delay_length_minus1 =  seq_parms.vui_parameters.vcl_hrd_params.dpb_output_delay_length_minus1;
            time_offset_length = seq_parms.vui_parameters.vcl_hrd_params.time_offset_length;
        }
        else
        {
            cpb_removal_delay_length_minus1 =  23;
            dpb_output_delay_length_minus1 =  23;
            time_offset_length = 24;
        }
    }
    else
    {
        cpb_removal_delay_length_minus1 =  23;
        dpb_output_delay_length_minus1 =  23;
        time_offset_length = 24;
    }

    if (CpbDpbDelaysPresentFlag)
    {
        data_size_bits += cpb_removal_delay_length_minus1 + dpb_output_delay_length_minus1 + 2;
    }

    if (pic_struct_present_flag)
    {
        if (timing_data.pic_struct > 8)
            return UMC_ERR_FAILED;
        NumClockTS = (timing_data.pic_struct <= 2) ? 1 : (timing_data.pic_struct <= 4 || timing_data.pic_struct == 7) ? 2 : 3;
        data_size_bits += (4 + NumClockTS); // pic_struct and clock_timestamp_flags

        for (i = 0; i < NumClockTS; i ++)
        {
            Ipp8u timestamp_data_size_bits = timing_data.full_timestamp_flag[i] ? 17 : (1 +
                timing_data.seconds_flag[i] * (7 + timing_data.minutes_flag[i]*(7 + timing_data.hours_flag[i]*5)));
            data_size_bits += timing_data.clock_timestamp_flag[i] * (19 +
            timestamp_data_size_bits + time_offset_length);
        }

    }

    data_size = data_size_bits/8;
    data_size += (data_size_bits%8 != 0);

    //payload type
    H264BsReal_PutBits(state, SEI_TYPE_PIC_TIMING, 8);
    //payload size
    while( data_size > 255 ){
        H264BsReal_PutBits(state, 0xff, 8);
        data_size -= 255;
    }
    H264BsReal_PutBits(state, data_size, 8);

    if (CpbDpbDelaysPresentFlag)
    {
        H264BsReal_PutBits(state, cpb_removal_delay, cpb_removal_delay_length_minus1 + 1);
        H264BsReal_PutBits(state, dpb_output_delay, dpb_output_delay_length_minus1 + 1);
    }

    if (pic_struct_present_flag)
    {
        H264BsReal_PutBits(state, timing_data.pic_struct, 4);
        for (i = 0; i < NumClockTS; i ++)
        {
            H264BsReal_PutBit(state, timing_data.clock_timestamp_flag[i]);
            if (timing_data.clock_timestamp_flag[i])
            {
                H264BsReal_PutBits(state, timing_data.ct_type[i], 2);
                H264BsReal_PutBit(state, timing_data.nuit_field_based_flag[i]);
                H264BsReal_PutBits(state, timing_data.counting_type[i], 5);
                H264BsReal_PutBit(state, timing_data.full_timestamp_flag[i]);
                H264BsReal_PutBit(state, timing_data.discontinuity_flag[i]);
                H264BsReal_PutBit(state, timing_data.cnt_dropped_flag[i]);
                H264BsReal_PutBits(state, timing_data.n_frames[i], 8);
                if (timing_data.full_timestamp_flag[i])
                {
                    H264BsReal_PutBits(state, timing_data.seconds_value[i], 6);
                    H264BsReal_PutBits(state, timing_data.minutes_value[i], 6);
                    H264BsReal_PutBits(state, timing_data.hours_value[i], 5);
                }
                else
                {
                    H264BsReal_PutBit(state, timing_data.seconds_flag[i]);
                    if (timing_data.seconds_flag[i])
                    {
                        H264BsReal_PutBits(state, timing_data.seconds_value[i], 6);
                        H264BsReal_PutBit(state, timing_data.minutes_flag[i]);
                        if (timing_data.minutes_flag[i])
                        {
                            H264BsReal_PutBits(state, timing_data.minutes_value[i], 6);
                            H264BsReal_PutBit(state, timing_data.hours_flag[i]);
                            if (timing_data.hours_flag[i])
                                H264BsReal_PutBits(state, timing_data.hours_value[i], 5);
                        }
                    }
                }
                H264BsReal_PutBits(state, timing_data.time_offset[i], time_offset_length);
            }
        }
    }

    if (H264BsBase_GetBsOffset(&(bs->m_base)) & 0x07)
        H264BsBase_WriteTrailingBits(&bs->m_base);

    return UMC_OK;
}

Status H264BsReal_PutSEI_RecoveryPoint(
    void* state,
    Ipp32s recovery_frame_cnt,
    Ipp8u exact_match_flag,
    Ipp8u broken_link_flag,
    Ipp8u changing_slice_group_idc)
{
    H264BsReal* bs = (H264BsReal *)state;
    Ipp32s data_size, data_size_bits = 0;

    data_size_bits = (Ipp32s)(log(float(recovery_frame_cnt))/log(2.0)) * 2 + 5;

    data_size = data_size_bits/8;
    data_size += (data_size_bits%8 != 0);

    //payload type
    H264BsReal_PutBits(state, SEI_TYPE_RECOVERY_POINT, 8);
    //payload size
    while( data_size > 255 ){
        H264BsReal_PutBits(state, 0xff, 8);
        data_size -= 255;
    }
    H264BsReal_PutBits(state, data_size, 8);

    H264BsReal_PutVLCCode(state, recovery_frame_cnt);
    H264BsReal_PutBit(state, exact_match_flag);
    H264BsReal_PutBit(state, broken_link_flag);
    H264BsReal_PutBits(state, changing_slice_group_idc,2);

    if (H264BsBase_GetBsOffset(&(bs->m_base)) & 0x07)
        H264BsBase_WriteTrailingBits(&bs->m_base);

    return UMC_OK;
}



//////////////////////




// ---------------------------------------------------------------------------
//  CH264pBs::ResetRBSP()
//      reset bitstream to beginning of current RBSP; used in the encoder
// ---------------------------------------------------------------------------
void H264BsFake_ResetRBSP(
    void* state)
{
    H264BsFake* bs = (H264BsFake *)state;
    bs->m_base.m_pbs = bs->m_pbsRBSPBase;
    bs->m_base.m_bitOffset = 0;
    bs->m_base.m_pbs[0] = 0;   // Zero the first byte, since subsequent bits written will be OR'd
                    // with this byte.  Subsequent bytes will be completely overwritten
                    // or zeroed, so no need to clear them out.

}

// ---------------------------------------------------------------------------
//  CH264pBs::EndOfNAL()
// ---------------------------------------------------------------------------
Ipp32u H264BsFake_EndOfNAL(
    void* state,
    Ipp8u* const pout,
    Ipp8u const uIDC,
    NAL_Unit_Type const uUnitType,
    bool& startPicture)
{
    H264BsFake* bs = (H264BsFake *)state;
    Ipp32u size, ExtraBytes;
    Ipp8u* curPtr, *endPtr, *outPtr;

    // get current RBSP compressed size
    size = (Ipp32u)(bs->m_base.m_pbs - bs->m_pbsRBSPBase);
    ExtraBytes = 0;

    // Set Pointers
    endPtr = bs->m_pbsRBSPBase + size - 1;  // Point at Last byte with data in it.
    curPtr = bs->m_pbsRBSPBase;
    outPtr = pout;

    // start access unit => should be zero_byte
    if (startPicture &&
        (((uUnitType >= NAL_UT_SLICE ) && (uUnitType <= NAL_UT_SEI)) ||
        (uUnitType == NAL_UT_AUD) ||
        (((uUnitType >= 0x0e) && (uUnitType <= 0x12)))) ) {
        *outPtr++ = 0;
        ExtraBytes = 1;
        startPicture = false;
    }

    // for SPS and PPS NAL units zero_byte should exist
    if( uUnitType == NAL_UT_SPS || uUnitType == NAL_UT_PPS ) {
        *outPtr++ = 0;
        ExtraBytes = 1;
    }

    *outPtr++ = 0;
    *outPtr++ = 0;
    *outPtr++ = 1;
    *outPtr++ = (Ipp8u) ((uIDC << 5) | uUnitType);  //nal_unit_type
    ExtraBytes += 4;

    if( size == 0 ) return ExtraBytes;

    while (curPtr < endPtr-1) { // Copy all but the last 2 bytes
        *outPtr++ = *curPtr;

        // Check for start code emulation
        if ((*curPtr++ == 0) && (*curPtr == 0) && (!(*(curPtr+1) & 0xfc))) {
            *outPtr++ = *curPtr++;
            *outPtr++ = 0x03;   // Emulation Prevention Byte
            ExtraBytes++;
        }
    }

    if (curPtr < endPtr) *outPtr++ = *curPtr++;
    // copy the last byte
    *outPtr = *curPtr;

    // Update RBSP Base Pointer
    bs->m_pbsRBSPBase = bs->m_base.m_pbs;

    // copy encoded frame to output
    return(size+ExtraBytes);

}

Status H264BsFake_PutSeqExParms(
    void* state,
    const H264SeqParamSet& seq_parms)
{
    H264BsFake_PutVLCCode(state, seq_parms.seq_parameter_set_id);
    H264BsFake_PutVLCCode(state, seq_parms.aux_format_idc);
    if(seq_parms.aux_format_idc != 0) {
        H264BsFake_PutVLCCode(state, seq_parms.bit_depth_aux - 8);
        H264BsFake_PutBit(state, seq_parms.alpha_incr_flag);
        H264BsFake_PutBits(state, seq_parms.alpha_opaque_value, seq_parms.bit_depth_aux + 1);
        H264BsFake_PutBits(state, seq_parms.alpha_transparent_value, seq_parms.bit_depth_aux + 1);
    }
    H264BsFake_PutBit(state, seq_parms.additional_extension_flag);
    return(UMC_OK);
}
// ---------------------------------------------------------------------------
//  CH264pBs::PutSeqParms()
// ---------------------------------------------------------------------------
Status H264BsFake_PutSeqParms(
    void* state,
    const H264SeqParamSet& seq_parms)
{
    Status ps = UMC_OK;

    // Write profile and level information

    H264BsFake_PutBits(state, seq_parms.profile_idc, 8);

    H264BsFake_PutBit(state, seq_parms.constraint_set0_flag);
    H264BsFake_PutBit(state, seq_parms.constraint_set1_flag);
    H264BsFake_PutBit(state, seq_parms.constraint_set2_flag);
    H264BsFake_PutBit(state, seq_parms.constraint_set3_flag);
    H264BsFake_PutBit(state, seq_parms.constraint_set4_flag);
    H264BsFake_PutBit(state, seq_parms.constraint_set5_flag);

    // 2 reserved zero bits
    H264BsFake_PutBits(state, 0, 2);

    H264BsFake_PutBits(state, seq_parms.level_idc, 8);

    // Write the sequence parameter set id
    H264BsFake_PutVLCCode(state, seq_parms.seq_parameter_set_id);

    if(seq_parms.profile_idc == H264_PROFILE_HIGH ||
        seq_parms.profile_idc == H264_PROFILE_HIGH10 ||
        seq_parms.profile_idc == H264_PROFILE_HIGH422 ||
        seq_parms.profile_idc == H264_PROFILE_HIGH444 ||
        seq_parms.profile_idc == H264_PROFILE_CAVLC444)
    {
        H264BsFake_PutVLCCode(state, seq_parms.chroma_format_idc);
        if(seq_parms.chroma_format_idc == 3) {
            H264BsFake_PutBit(state, seq_parms.residual_colour_transform_flag);
        }
        H264BsFake_PutVLCCode(state, seq_parms.bit_depth_luma - 8);
        H264BsFake_PutVLCCode(state, seq_parms.bit_depth_chroma - 8);
        H264BsFake_PutBit(state, seq_parms.qpprime_y_zero_transform_bypass_flag);
        H264BsFake_PutBit(state, seq_parms.seq_scaling_matrix_present_flag);
        if(seq_parms.seq_scaling_matrix_present_flag)
        {
            Ipp32s i;
            bool UseDefaultScalingMatrix;
            for( i=0; i<8 ; i++){
                //Put scaling list present flag
                H264BsFake_PutBit(state, seq_parms.seq_scaling_list_present_flag[i]);
                if( seq_parms.seq_scaling_list_present_flag[i] ){
                   if( i<6 )
                      H264BsFake_PutScalingList(state, &seq_parms.seq_scaling_list_4x4[i][0], 16, UseDefaultScalingMatrix);
                   else
                      H264BsFake_PutScalingList(state, &seq_parms.seq_scaling_list_8x8[i-6][0], 64, UseDefaultScalingMatrix);
                }
            }
        }
    }

    // Write log2_max_frame_num_minus4
    H264BsFake_PutVLCCode(state, seq_parms.log2_max_frame_num - 4);

    // Write pic_order_cnt_type and associated data
    H264BsFake_PutVLCCode(state, seq_parms.pic_order_cnt_type);

    // Write data specific to various pic order cnt types

    // pic_order_cnt_type == 1 is NOT currently supported
    if (seq_parms.pic_order_cnt_type == 0) {
        H264BsFake_PutVLCCode(state, seq_parms.log2_max_pic_order_cnt_lsb - 4);
    }

    // Write num_ref_frames
    H264BsFake_PutVLCCode(state, seq_parms.num_ref_frames);

    // Write required_frame_num_update_behaviour_flag
    H264BsFake_PutBit(state, seq_parms.gaps_in_frame_num_value_allowed_flag);

    // Write picture MB dimensions
    H264BsFake_PutVLCCode(state, seq_parms.frame_width_in_mbs - 1);
    H264BsFake_PutVLCCode(state, seq_parms.frame_height_in_mbs - 1);

    // Write other misc flags
    H264BsFake_PutBit(state, seq_parms.frame_mbs_only_flag);

    if (!seq_parms.frame_mbs_only_flag) {
        H264BsFake_PutBit(state, seq_parms.mb_adaptive_frame_field_flag);
    }

    // Right now, the decoder only supports this flag with
    // a value of zero.
    H264BsFake_PutBit(state, seq_parms.direct_8x8_inference_flag);

    H264BsFake_PutBit(state, seq_parms.frame_cropping_flag);

    if (seq_parms.frame_cropping_flag)  {
        H264BsFake_PutVLCCode(state, seq_parms.frame_crop_left_offset);
        H264BsFake_PutVLCCode(state, seq_parms.frame_crop_right_offset);
        H264BsFake_PutVLCCode(state, seq_parms.frame_crop_top_offset);
        H264BsFake_PutVLCCode(state, seq_parms.frame_crop_bottom_offset);
    }

    H264BsFake_PutBit(state, seq_parms.vui_parameters_present_flag);

    if (seq_parms.vui_parameters_present_flag) {

        H264BsFake_PutBit(state,  seq_parms.vui_parameters.aspect_ratio_info_present_flag );
        if( seq_parms.vui_parameters.aspect_ratio_info_present_flag ){
            H264BsFake_PutBits(state, seq_parms.vui_parameters.aspect_ratio_idc, 8);
            if( seq_parms.vui_parameters.aspect_ratio_idc == 255 ){ // == Extended_SAR
                H264BsFake_PutBits(state, seq_parms.vui_parameters.sar_width,16);
                H264BsFake_PutBits(state, seq_parms.vui_parameters.sar_height,16);
            }
        }

        H264BsFake_PutBit(state,  seq_parms.vui_parameters.overscan_info_present_flag );
        if(seq_parms.vui_parameters.overscan_info_present_flag){
            H264BsFake_PutBit(state,  seq_parms.vui_parameters.overscan_appropriate_flag );
        }

        H264BsFake_PutBit(state,  seq_parms.vui_parameters.video_signal_type_present_flag );
        if(seq_parms.vui_parameters.video_signal_type_present_flag){
            H264BsFake_PutBits(state, seq_parms.vui_parameters.video_format,3);
            H264BsFake_PutBit(state, seq_parms.vui_parameters.video_full_range_flag);
            H264BsFake_PutBit(state, seq_parms.vui_parameters.colour_description_present_flag);
            if(seq_parms.vui_parameters.colour_description_present_flag){
                H264BsFake_PutBits(state, seq_parms.vui_parameters.colour_primaries,8);
                H264BsFake_PutBits(state, seq_parms.vui_parameters.transfer_characteristics,8);
                H264BsFake_PutBits(state, seq_parms.vui_parameters.matrix_coefficients,8);
            }
        }

        H264BsFake_PutBit(state,  seq_parms.vui_parameters.chroma_loc_info_present_flag );
        if(seq_parms.vui_parameters.chroma_loc_info_present_flag){
            H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.chroma_sample_loc_type_top_field);
            H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.chroma_sample_loc_type_bottom_field);
        }

        H264BsFake_PutBit(state,  seq_parms.vui_parameters.timing_info_present_flag );
        if(seq_parms.vui_parameters.timing_info_present_flag){
            H264BsFake_PutBits(state, seq_parms.vui_parameters.num_units_in_tick>>24, 8);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.num_units_in_tick&0x00ffffff, 24); //Due to restrictions of BsTypeType::PutBits
            H264BsFake_PutBits(state, seq_parms.vui_parameters.time_scale>>24, 8);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.time_scale&0x00ffffff, 24);
            H264BsFake_PutBit(state, seq_parms.vui_parameters.fixed_frame_rate_flag);
        }

        H264BsFake_PutBit(state,  seq_parms.vui_parameters.nal_hrd_parameters_present_flag );
        if(seq_parms.vui_parameters.nal_hrd_parameters_present_flag){
            Ipp32s i;

            H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.hrd_params.cpb_cnt_minus1);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.hrd_params.bit_rate_scale, 4);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.hrd_params.cpb_size_scale, 4);
            for( i=0; i <= seq_parms.vui_parameters.hrd_params.cpb_cnt_minus1; i++ ){
                H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.hrd_params.bit_rate_value_minus1[i]);
                H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.hrd_params.cpb_size_value_minus1[i]);
                H264BsFake_PutBit(state, seq_parms.vui_parameters.hrd_params.cbr_flag[i]);
            }
            H264BsFake_PutBits(state, seq_parms.vui_parameters.hrd_params.initial_cpb_removal_delay_length_minus1, 5);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.hrd_params.cpb_removal_delay_length_minus1, 5);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.hrd_params.dpb_output_delay_length_minus1, 5);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.hrd_params.time_offset_length, 5);
        }

        H264BsFake_PutBit(state,  seq_parms.vui_parameters.vcl_hrd_parameters_present_flag );
        if(seq_parms.vui_parameters.vcl_hrd_parameters_present_flag){
            Ipp32s i;

            H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.vcl_hrd_params.cpb_cnt_minus1);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.bit_rate_scale, 4);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.cpb_size_scale, 4);
            for( i=0; i <= seq_parms.vui_parameters.hrd_params.cpb_cnt_minus1; i++ ){
                H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.vcl_hrd_params.bit_rate_value_minus1[i]);
                H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.vcl_hrd_params.cpb_size_value_minus1[i]);
                H264BsFake_PutBit(state, seq_parms.vui_parameters.vcl_hrd_params.cbr_flag[i]);
            }
            H264BsFake_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.initial_cpb_removal_delay_length_minus1, 5);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.cpb_removal_delay_length_minus1, 5);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.dpb_output_delay_length_minus1, 5);
            H264BsFake_PutBits(state, seq_parms.vui_parameters.vcl_hrd_params.time_offset_length, 5);
        }

        if( seq_parms.vui_parameters.nal_hrd_parameters_present_flag ||
            seq_parms.vui_parameters.vcl_hrd_parameters_present_flag){
            H264BsFake_PutBit(state, seq_parms.vui_parameters.low_delay_hrd_flag);
        }

        H264BsFake_PutBit(state,  seq_parms.vui_parameters.pic_struct_present_flag );
        H264BsFake_PutBit(state,  seq_parms.vui_parameters.bitstream_restriction_flag );
        if(seq_parms.vui_parameters.bitstream_restriction_flag){
            H264BsFake_PutBit(state, seq_parms.vui_parameters.motion_vectors_over_pic_boundaries_flag);
            H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.max_bytes_per_pic_denom);
            H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.max_bits_per_mb_denom);
            H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.log2_max_mv_length_horizontal);
            H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.log2_max_mv_length_vertical);
            H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.num_reorder_frames);
            H264BsFake_PutVLCCode(state, seq_parms.vui_parameters.max_dec_frame_buffering);
        }
    }

    return ps;
}

// ---------------------------------------------------------------------------
//  CH264pBs::PutPicParms()
// ---------------------------------------------------------------------------
Status H264BsFake_PutPicParms(void* state, const H264PicParamSet& pic_parms, const H264SeqParamSet& seq_parms)
{
    Status ps = UMC_OK;

    // Write IDs
    H264BsFake_PutVLCCode(state, pic_parms.pic_parameter_set_id);
    H264BsFake_PutVLCCode(state, pic_parms.seq_parameter_set_id);

    // Write Entropy coding mode
    H264BsFake_PutBit(state, pic_parms.entropy_coding_mode);
    H264BsFake_PutBit(state, pic_parms.pic_order_present_flag);

    // Only one slice group is currently supported
    // Write num_slice_groups_minus1
    H264BsFake_PutVLCCode(state, pic_parms.num_slice_groups - 1);

    // If multiple slice groups are ever supported, then add code here
    // to write the slice group map information needed to allocate MBs
    // to the defined slice groups.

    // Write num_ref_idx_active counters
    // Right now these are limited to one frame each...
    H264BsFake_PutVLCCode(state, pic_parms.num_ref_idx_l0_active - 1);
    H264BsFake_PutVLCCode(state, pic_parms.num_ref_idx_l1_active - 1);

    // Write some various flags

    // Weighted pred for P slices is not supported
    H264BsFake_PutBit(state, pic_parms.weighted_pred_flag);

    // Explicit weighted BiPred not supported
    // So 0 or 2 are the acceptable values
    H264BsFake_PutBits(state, pic_parms.weighted_bipred_idc, 2);

    // Write quantization values
    H264BsFake_PutVLCCode(state, SIGNED_VLC_CODE(pic_parms.pic_init_qp - 26));
    H264BsFake_PutVLCCode(state, SIGNED_VLC_CODE(pic_parms.pic_init_qs - 26));
    H264BsFake_PutVLCCode(state, SIGNED_VLC_CODE(pic_parms.chroma_qp_index_offset));

    // Write some more flags
    H264BsFake_PutBit(state, pic_parms.deblocking_filter_variables_present_flag);
    H264BsFake_PutBit(state, pic_parms.constrained_intra_pred_flag);
    H264BsFake_PutBit(state, pic_parms.redundant_pic_cnt_present_flag);
    if(seq_parms.profile_idc == H264_PROFILE_HIGH ||
       seq_parms.profile_idc == H264_PROFILE_HIGH10 ||
       seq_parms.profile_idc == H264_PROFILE_HIGH422 ||
       seq_parms.profile_idc == H264_PROFILE_HIGH444)
    {
        H264BsFake_PutBit(state, pic_parms.transform_8x8_mode_flag);
        H264BsFake_PutBit(state, pic_parms.pic_scaling_matrix_present_flag);
        if(pic_parms.pic_scaling_matrix_present_flag)
        { /* scaling matrices coding is not supported */
            VM_ASSERT(0);
            // TO DO: add scaling matrices coding.
        }
        H264BsFake_PutVLCCode(state, SIGNED_VLC_CODE(pic_parms.second_chroma_qp_index_offset));
    }

    return ps;
}

// ---------------------------------------------------------------------------
//  CH264pBs::PutPicDelimiter()
// ---------------------------------------------------------------------------
Status H264BsFake_PutPicDelimiter(
    void* state,
    EnumPicCodType PicCodType)
{
    Status ps = UMC_OK;

    // Write pic_type
    H264BsFake_PutBits(state, PicCodType, 3);

    return ps;
} // CH264pBs::PutPicDelimiter()

// ---------------------------------------------------------------------------
//  CH264pBs::PutDQUANT()
// ---------------------------------------------------------------------------
void H264BsFake_PutDQUANT(
    void* state,
    const Ipp32u quant,
    const Ipp32u quant_prev)
{
    Ipp32s dquant;

    // compute dquant
    dquant=quant-quant_prev;


    // Get dquant between (QP_RANGE-1) to (-QP_RANGE)  (25 to -26 for JVT)

    if (dquant >= H264_QP_RANGE)
        dquant = dquant - H264_QP_MAX;
    else if (dquant < -H264_QP_RANGE)
        dquant = dquant + H264_QP_MAX;

    H264BsFake_PutVLCCode(state, SIGNED_VLC_CODE(dquant));
}

// ---------------------------------------------------------------------------
//  CH264pBs::PutSliceHeader()
// ---------------------------------------------------------------------------
template<typename COEFFSTYPE, typename PIXTYPE>
Status H264BsFake_PutSliceHeader(
    void* state,
    const H264SliceHeader& slice_hdr,
    const H264PicParamSet& pic_parms,
    const H264SeqParamSet& seq_parms,
    const EnumPicClass& ePictureClass,
    const H264Slice<COEFFSTYPE, PIXTYPE> *curr_slice)
{
    EnumSliceType slice_type = curr_slice->m_slice_type;
    Status ps = UMC_OK;

    // Write first_mb_in_slice
    H264BsFake_PutVLCCode(state, curr_slice->m_first_mb_in_slice);

    // Write slice_type_idc
    H264BsFake_PutVLCCode(state, slice_type);

    // Write pic_parameter_set_id
    H264BsFake_PutVLCCode(state, slice_hdr.pic_parameter_set_id);

    // Write frame_num (modulo MAX_FN) using log2_max_frame_num bits
    H264BsFake_PutBits(state, slice_hdr.frame_num, seq_parms.log2_max_frame_num);

    // Only write field values if not frame only...
    if (!seq_parms.frame_mbs_only_flag) {
        // Write field_pic_flag
        H264BsFake_PutBit(state, slice_hdr.field_pic_flag);

        // Write bottom_field_flag
        if (slice_hdr.field_pic_flag == 1) {
            H264BsFake_PutBit(state, slice_hdr.bottom_field_flag);
        }
    }

    if (ePictureClass == IDR_PIC) {
        H264BsFake_PutVLCCode(state, slice_hdr.idr_pic_id);
    }

    // Write pic_order_cnt info
    if (seq_parms.pic_order_cnt_type == 0) {
        H264BsFake_PutBits(state, slice_hdr.pic_order_cnt_lsb, seq_parms.log2_max_pic_order_cnt_lsb);
        if ((pic_parms.pic_order_present_flag == 1) && (!slice_hdr.field_pic_flag)) {
            H264BsFake_PutVLCCode(state, SIGNED_VLC_CODE(slice_hdr.delta_pic_order_cnt_bottom));
        }
    }

    // Handle Redundant Slice Flag
    if (pic_parms.redundant_pic_cnt_present_flag) {
        H264BsFake_PutVLCCode(state, slice_hdr.redundant_pic_cnt);
    }

    // Write direct_spatial_mv_pred_flag if this is a BPREDSLICE
    if (slice_type == BPREDSLICE) {
        H264BsFake_PutBit(state, slice_hdr.direct_spatial_mv_pred_flag);
    }

    if ((slice_type == BPREDSLICE) ||
        (slice_type == S_PREDSLICE) ||
        (slice_type == PREDSLICE)) {
        // Write num_ref_idx_active_override_flag
        H264BsFake_PutBit(state, curr_slice->num_ref_idx_active_override_flag);
        if (curr_slice->num_ref_idx_active_override_flag) {
            H264BsFake_PutVLCCode(state, curr_slice->num_ref_idx_l0_active - 1);
            if (slice_type == BPREDSLICE) {
                H264BsFake_PutVLCCode(state, curr_slice->num_ref_idx_l1_active - 1);
            }
        }
    }
    // ref_pic_list_reordering() default settings

    if ((slice_type != INTRASLICE) &&
        (slice_type != S_INTRASLICE)) {

        // ref_pic_list_reordering_flag_l0
        H264BsFake_PutBit(state, 0);

        // More data would be inserted here if the above flag is 1

        if (slice_type == BPREDSLICE) {
            // ref_pic_list_reordering_flag_l1
            H264BsFake_PutBit(state, 0);
            // More data would be inserted here if the above flag is 1
        }
    }

    if ((pic_parms.weighted_pred_flag &&
        (slice_type == PREDSLICE || slice_type == S_PREDSLICE)) ||
        ((pic_parms.weighted_bipred_idc == 1) && slice_type == BPREDSLICE)) {
        // Add support for pred_weight_table() ???
    }

    // Write appropriate bits for dec_ref_pic_marking()
    // Note!  Currently there are no structure members for these syntax elements,
    // so the default bits are just written directly.  This need to be updated
    // when appropriate structures are implemented.  TODO - VSI
    if (ePictureClass == IDR_PIC) {
            // no_output_of_prior_pics_flag
            H264BsFake_PutBit(state, 0);
            // long_term_reference_flag
            H264BsFake_PutBit(state, 0);
    } else if (ePictureClass == REFERENCE_PIC) {
            // adaptive_ref_pic_marking_mode_flag
            H264BsFake_PutBit(state, slice_hdr.adaptive_ref_pic_marking_mode_flag);

            // Other things would be written here if memory management control
            // were to be implemented, changing the value of the adaptive_ref_pic_marking_mode_flag
            // written above to 1.
    }

    if (pic_parms.entropy_coding_mode && slice_type != INTRASLICE &&
            slice_type != S_INTRASLICE) {
        H264BsFake_PutVLCCode(state, curr_slice->m_cabac_init_idc);
    }

    // Write slice_qp_delta
    H264BsFake_PutVLCCode(state, SIGNED_VLC_CODE(curr_slice->m_slice_qp_delta));

    // No sp_for_switch_flag or slice_qp_s_delta are written because we don't
    // support SI and SP slices
    if ((slice_type == S_PREDSLICE) || (slice_type == S_INTRASLICE)) {
        if (slice_type == S_PREDSLICE)
            H264BsFake_PutBit(state, slice_hdr.sp_for_switch_flag);
        // Write slice_qp_s_delta
        H264BsFake_PutVLCCode(state, SIGNED_VLC_CODE(slice_hdr.slice_qs_delta));
    }

    // Write Filter Parameters
    if (pic_parms.deblocking_filter_variables_present_flag) {

        // Write disable_deblocking_filter_idc
        H264BsFake_PutVLCCode(state, curr_slice->m_disable_deblocking_filter_idc);

        if (curr_slice->m_disable_deblocking_filter_idc != 1) {

            // Write slice_alpha_c0_offset_div2
            H264BsFake_PutVLCCode(state, SIGNED_VLC_CODE(curr_slice->m_slice_alpha_c0_offset>>1));

            // Write slice_beta_offset_div2
            H264BsFake_PutVLCCode(state, SIGNED_VLC_CODE(curr_slice->m_slice_beta_offset>>1));

        } else {    // If the filter is disabled, both offsets are -51
        }

    } else {    //  If no parms are written, then the filter is ON
    }

    if (pic_parms.num_slice_groups > 1) {
        // Write slice_group_change_cycle
    }

    return ps;

}

Status H264BsFake_PutNumCoeffAndTrailingOnes(
    void* state,
    Ipp32u N,                   // N, obtained from num coeffs of above/left blocks
    Ipp32s bChromaDC,           // True if this is a Chroma DC coeff block (2x2)
    Ipp32u uNumCoeff,           // Number of non-trailing one coeffs to follow (0-4 or 0-16)
    Ipp32u uNumTrailingOnes,    // Number of trailing ones (0-3)
    Ipp32u TrOneSigns)          // Signs of the trailing ones, packed into 3 LSBs, (1==neg)
{
    Ipp32s uVLCSelect = 0;
    Status ps = UMC_OK;

    if (bChromaDC) {
        switch( bChromaDC ){
            case 1:
                uVLCSelect = 3; //nC = -1
                break;
            case 2:
                uVLCSelect = 4; //nC = -2
                break;
            case 3:
                uVLCSelect = 0; //nC = 0
                break;
        }
    } else {

        if (N < 2) {
            uVLCSelect = 0; // 0<=nC<2
        } else if (N < 4) {
            uVLCSelect = 1; // 2<=nC<4
        } else if (N < 8) {
            uVLCSelect = 2; // 4<=nC<8
        } else {  // N > 7  // 8<=nC
            uVLCSelect = -1;    // escape to Fixed Length code
        }

    }
    if (uVLCSelect >= 0) {
        // Write coeff_token from Look-up table

        H264BsFake_PutBits(state, EncTotalCoeff[uVLCSelect][uNumCoeff][uNumTrailingOnes].code,
                EncTotalCoeff[uVLCSelect][uNumCoeff][uNumTrailingOnes].len);

    } else {
        if (uNumCoeff == 0) {
            H264BsFake_PutBits(state, 3, 6);
        } else {
            // //  xxxxyy -> xxxx = uNumCoeff-l , yy = uNumTrailingOnes
            H264BsFake_PutBits(state, (((uNumCoeff-1)<<2)|uNumTrailingOnes), 6);
        }
    }

    // Write signs of NumTrailingOnes
    if( uNumTrailingOnes )
      H264BsFake_PutBits(state, TrOneSigns, uNumTrailingOnes);

    return ps;
}
//#define TRACE_CAVLC

template<typename COEFFSTYPE>
Status H264BsFake_PutLevels(
    void* state,
    COEFFSTYPE* iLevels,      // Array of Levels to write
    Ipp32s      NumLevels,    // Total Number of coeffs to write (0-4 or 0-16)
    Ipp32s      TrailingOnes) // Trailing Ones
{
    Ipp32s VLCSelect = 0;
    Status ps = UMC_OK;
    static const Ipp32s vlc_inc_tab[7] = {0, 3, 6, 12, 24, 48, 0x8000};
    static const Ipp32s escape_tab[7] = {16, 16, 31, 61, 121, 241, 481};
    Ipp32s cnt;
    Ipp32s level_adj = 0;   // Used to flag that the next level
                            // is decreased in magnitude by 1 when coded.

    // Fixup first coeff level if Trailing Ones < 3
    if (TrailingOnes < 3)
    {
        level_adj = 1;  // Subtracted from the level when coded

        if ((TrailingOnes + NumLevels) > 10)
            VLCSelect = 1; // Start with different table
    }
#if defined (TRACE_CAVLC)
    printf("PutLevels: NumLevels = %d, TOnes = %d level_adj = %d\n",
        NumLevels, TrailingOnes, level_adj);
#endif // TRACE_CAVLC
    for (cnt = 0; cnt < NumLevels; cnt++) {
        Ipp32s L = ABS(iLevels[cnt]);
        Ipp32s sign = (L != iLevels[cnt]);

#if defined (TRACE_CAVLC)
        printf("iLevels[%d] = %d, L = %d, sign = %d, VLCSelect = %d, esc = %d\n",
            cnt, iLevels[cnt], L, sign, VLCSelect, escape_tab[VLCSelect]);
        if(L > 2063) {
            printf("PutLevels: L(%d) > 2063, iLevels[%d] = %d, VLCSelect = %d, esc = %d\n",
                L, cnt, iLevels[cnt], VLCSelect, escape_tab[VLCSelect]);
            ps = (Status)cnt;
        }
#endif // TRACE_CAVLC
        L -= level_adj;

        if (L >= escape_tab[VLCSelect]) {  // 28-bit escape
            // Catch cases where the level is too large to write to the BS
            Ipp32u num = L-escape_tab[VLCSelect];

            if( num & (0xffffffff<<11)){
                Ipp32s addbit = 1;
                while(((Ipp32s)num-(2048<<addbit)+2048) >= 0) addbit++;
                addbit--;
                H264BsFake_PutBits(state, 1, 16+addbit);
                H264BsFake_PutBits(state, ((num-(2048<<addbit)+2048)<<1)|sign, 12+addbit);
            }else{
                H264BsFake_PutBits(state, 1, 16); // BsTypeType::PutBits maxes out at 24 bits
                H264BsFake_PutBits(state, (num<<1)|sign, 12);
            }
        } else if (VLCSelect) {    // VLC Level 1-6

            Ipp32s N= VLCSelect - 1;
            H264BsFake_PutBits(state, 1, ((L-1)>>(N))+1);
//          H264BsFake_PutBits(state, (((L-1)%(1<<N))<<1)+sign, VLCSelect);
            H264BsFake_PutBits(state, (((L-1)&((1<<N)-1))<<1)+sign, VLCSelect);
        } else { // VLC Level 0
            if (L < 8) {
                H264BsFake_PutBits(state, 1,sign+((L-1)<<1)+1);  // Start with a 0 if negative
            } else { // L 8-15
                H264BsFake_PutBits(state, 16+((L&7)<<1)+sign,19); // 19 bit escape
            }
        }

        L += level_adj; // Restore the true level for the following calculations

        // Adjust the VLC table depending on the current table and
        // the Level just coded.
        if (!VLCSelect && (L > 3))
            VLCSelect = 2;
        else if (L > vlc_inc_tab[VLCSelect])
            VLCSelect++;

        level_adj = 0;  // Clear this now that it's served its purpose
    }

    return ps;
}

Status H264BsFake_PutTotalZeros(
    void* state,
    Ipp32s TotalZeros,
    Ipp32s TotalCoeffs,
    Ipp32s bChromaDC)
{
    Status ps = UMC_OK;

    TotalCoeffs -= 1;

    if (bChromaDC) {
        H264BsFake_PutBits(state, EncTotalZerosChroma[bChromaDC-1][TotalZeros][TotalCoeffs].code,
            EncTotalZerosChroma[bChromaDC-1][TotalZeros][TotalCoeffs].len);
    } else {
        H264BsFake_PutBits(state, EncTotalZeros4x4[TotalZeros][TotalCoeffs].code,
            EncTotalZeros4x4[TotalZeros][TotalCoeffs].len);
    }

    return ps;
}

Status H264BsFake_PutRuns(
    void* state,
    Ipp8u* uRuns,
    Ipp32s TotalZeros,
    Ipp32s TotalCoeffs)
{
    Status ps = UMC_OK;
    Ipp32s cnt = 0;

    TotalCoeffs--; // Don't write the last run, since it can be inferred.

    while (TotalZeros && (cnt != TotalCoeffs)) {
        Ipp32u zeros_idx = (TotalZeros > 6) ? 6 : TotalZeros-1;
        H264BsFake_PutBits(state, EncRuns[uRuns[cnt]][zeros_idx].code,
            EncRuns[uRuns[cnt]][zeros_idx].len);
        TotalZeros -= uRuns[cnt];
        cnt++;
    }

    return ps;
}

/**************************** cabac **********************************/
Status H264BsFake_MBFieldModeInfo_CABAC(
    void* state,
    Ipp32s mb_field,
    Ipp32s field_available_left,
    Ipp32s field_available_above)
{
    H264BsFake* bs = (H264BsFake *)state;
    H264BsFake_EncodeSingleBin_CABAC(
        state,
        bs->m_base.context_array +
            field_available_left +
            field_available_above +
            ctxIdxOffset[MB_FIELD_DECODING_FLAG],
        mb_field != 0);
    return UMC_OK;
}

Status H264BsFake_MBTypeInfo_CABAC(
    void* state,
    EnumSliceType SliceType,
    Ipp32s mb_type_cur,
    MB_Type type_cur,
    MB_Type type_left,
    MB_Type type_above)
{
    H264BsFake* bs = (H264BsFake *)state;
    Ipp32s a, b,c=0;
    Ipp32s type_temp;
    Ipp32s itype=mb_type_cur-1;
    Ipp8u* ctx;

    if(IS_INTRA_SLICE(SliceType)){
        b = (type_above >= NUMBER_OF_MBTYPES)?0:((type_above != MBTYPE_INTRA) ? 1 : 0 );
        a = (type_left >= NUMBER_OF_MBTYPES)?0:((type_left != MBTYPE_INTRA) ? 1 : 0 );
        ctx = bs->m_base.context_array + ctxIdxOffset[MB_TYPE_I];
encoding_intra:
        if (type_cur==MBTYPE_INTRA){ // 4x4 Intra
            H264BsFake_EncodeSingleBin_CABAC(state, ctx + a + b, 0);
        }else if( type_cur == MBTYPE_PCM){ // PCM-MODE
            H264BsFake_EncodeSingleBin_CABAC(state, ctx + a + b, 1);
            H264BsFake_EncodeFinalSingleBin_CABAC(state, 1);
        }else{ // 16x16 Intra
            H264BsFake_EncodeSingleBin_CABAC(state, ctx + a + b, 1);
            H264BsFake_EncodeFinalSingleBin_CABAC(state, 0);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx + 3 + c, itype / 12 != 0); // coding of AC/no AC
            itype %= 12;
            if (itype<4){
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 4 + c, 0);
            }else{
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 4 + c, 1);
                if (itype>=4 && itype<8){
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                }else{
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                }
            }
            H264BsFake_EncodeSingleBin_CABAC(state, ctx + 6,  (itype&2) != 0 );
            H264BsFake_EncodeSingleBin_CABAC(state, ctx + 7-c,  (itype&1) != 0 );
        }
    }else{ // INTER
        if (IS_B_SLICE(SliceType)){
            type_temp = mb_type_cur;
            b = (type_above >= NUMBER_OF_MBTYPES)?0:(IS_SKIP_MBTYPE(type_above) ? 0 : 1 );
            a = (type_left >= NUMBER_OF_MBTYPES)?0:(IS_SKIP_MBTYPE(type_left) ? 0 : 1 );
            if (mb_type_cur>=24) type_temp=24;
            ctx = bs->m_base.context_array + ctxIdxOffset[MB_TYPE_B];

            if (type_temp==0){
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + a+b,  0);
            }else if (type_temp<=2){
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + a+b,  1);
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 3,  0);
                if (type_temp-1) H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else      H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  0);
            }else if (type_temp<=10){
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + a+b,1);
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 3,  1);
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 4,  0);
                if (((type_temp-3)>>2)&0x01)
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                if (((type_temp-3)>>1)&0x01)
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                if ((type_temp-3)&0x01)
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  0);
            }else if (type_temp==11 || type_temp==11*2){
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + a+b,1);
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 3,  1);
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 4,  1);
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                if (type_temp!=11)
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  0);
            }else{
                if (type_temp > 22 ) type_temp--;

                H264BsFake_EncodeSingleBin_CABAC(state, ctx + a+b,1);
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 3,  1);
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 4,  1);

                if ((type_temp-12)&8)
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                if ((type_temp-12)&4)
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                if ((type_temp-12)&2)
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                if ((type_temp-12)&1)
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  1);
                else
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 5,  0);
                //if (type_temp >12) type_temp++;
            }
        if( type_cur==MBTYPE_PCM ){
            H264BsFake_EncodeFinalSingleBin_CABAC(state, 1);
            return UMC_OK;
        }
        if(type_cur==MBTYPE_INTRA_16x16){
            H264BsFake_EncodeFinalSingleBin_CABAC(state, 0);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx + 6,  (mb_type_cur-24)/12 != 0);
            mb_type_cur %= 12;

            type_temp = mb_type_cur >> 2;
            if (type_temp==0){
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 7,  0);
            }else{
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 7,  1);
                if (type_temp==1){
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 7,  0);
                }else{
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 7,  1);
                }
            }

            H264BsFake_EncodeSingleBin_CABAC(state, ctx + 8,  (mb_type_cur&2) != 0);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx + 8,  (mb_type_cur&1) != 0);
        }

        }else{ // P slice
            ctx = bs->m_base.context_array + ctxIdxOffset[MB_TYPE_P_SP] ;
            if (IS_INTRA_MBTYPE(type_cur)){
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 0,  1);
                a=0;b=3;c=1;itype-=5;
                goto encoding_intra;
            }else{
                H264BsFake_EncodeSingleBin_CABAC(state, ctx + 0,  0);
                switch(mb_type_cur)
                {
                case 0:
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 1,  0);
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 2,  0);
                    break;
                case 1:
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 1,  1);
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 3,  1);
                    break;
                case 2:
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 1,  1);
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 3,  0);
                    break;
                case 3:
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 1,  0);
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx + 2,  1);
                    break;
                default:
                    return UMC_ERR_FAILED;
                }
            }
        }
     }
    return UMC_OK;
}

Status H264BsFake_SubTypeInfo_CABAC(
    void* state,
    EnumSliceType SliceType,
    Ipp32s type)
{
    H264BsFake* bs = (H264BsFake *)state;
    Ipp8u* ctx;
    if (!IS_B_SLICE(SliceType)){
        ctx = bs->m_base.context_array + ctxIdxOffset[SUB_MB_TYPE_P_SP];
        switch (type)
        {
        case 0:
            H264BsFake_EncodeSingleBin_CABAC(state, ctx,1);
            break;
        case 1:
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+0,0);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+1,0);
            break;
        case 2:
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+0,0);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+1,1);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+2,1);
            break;
        case 3:
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+0,0);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+1,1);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+2,0);
            break;
        }
    } else {
        ctx = bs->m_base.context_array + ctxIdxOffset[SUB_MB_TYPE_B];
        if (!type){
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+0,0);
            return UMC_OK;
        } else {
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+0,1);
            type--;
        }

        if (type<2){
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+1,0);
            if (!type)
                H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,0);
            else
                H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,1);
        }else if (type<6){
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+1,1);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+2,0);
            if (((type-2)>>1)&1)
                H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,1);
            else
                H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,0);
            if ((type-2)&1)
                H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,1);
            else
                H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,0);
        } else {
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+1,1);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+2,1);
            if (((type-6)>>2)&1){
                H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,1);
                if ((type-6)&1)
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,1);
                else
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,0);
            } else {
                H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,0);
                if (((type-6)>>1)&1)
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,1);
                else
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,0);
                if ((type-6)&1)
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,1);
                else
                    H264BsFake_EncodeSingleBin_CABAC(state, ctx+3,0);
            }
        }
    }
    return UMC_OK;
}

Status H264BsFake_IntraPredMode_CABAC(
    void* state,
    Ipp32s mode)
{
    H264BsFake* bs = (H264BsFake *)state;
    if (mode == -1)
        H264BsFake_EncodeSingleBin_CABAC(state, bs->m_base.context_array + ctxIdxOffset[PREV_INTRA4X4_PRED_MODE_FLAG],1);
    else{
        H264BsFake_EncodeSingleBin_CABAC(state, bs->m_base.context_array + ctxIdxOffset[PREV_INTRA4X4_PRED_MODE_FLAG],0);
        Ipp8u* ctx = bs->m_base.context_array + ctxIdxOffset[REM_INTRA4X4_PRED_MODE];
        H264BsFake_EncodeSingleBin_CABAC(state, ctx,(mode&1));
        H264BsFake_EncodeSingleBin_CABAC(state, ctx,(mode&2)>>1);
        H264BsFake_EncodeSingleBin_CABAC(state, ctx,(mode&4)>>2);
    }
    return UMC_OK;
}

Status H264BsFake_ChromaIntraPredMode_CABAC(
    void* state,
    Ipp32s mode,
    Ipp32s left_p,
    Ipp32s top_p)
{
    H264BsFake* bs = (H264BsFake *)state;
    Ipp8u* ctx = bs->m_base.context_array + ctxIdxOffset[INTRA_CHROMA_PRED_MODE];
    switch( mode ){
        case 0:
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+left_p+top_p,0);
            break;
        case 1:
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+left_p+top_p,1);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+3, 0);
            break;
        case 2:
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+left_p+top_p,1);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+3, 1);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+3, 0);
            break;
        case 3:
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+left_p+top_p,1);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+3, 1);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+3, 1);
            break;
    }

    return UMC_OK;
}

Status H264BsFake_MVD_CABAC(
    void* state,
    Ipp32s vector,
    Ipp32s left_p,
    Ipp32s top_p,
    Ipp32s contextbase)
{
    H264BsFake* bs = (H264BsFake *)state;
    Ipp32s ctx_mod;

    if (labs(left_p) + labs(top_p) < 3){
        ctx_mod = 0;
    } else {
        if (labs(left_p) + labs(top_p) <= 32)
            ctx_mod = 1;
        else
            ctx_mod = 2;
    }

   Ipp8u* ctx = bs->m_base.context_array + ctxIdxOffset[contextbase];
    if (vector== 0){
        H264BsFake_EncodeSingleBin_CABAC(state, ctx+ctx_mod, 0);
    } else {
        H264BsFake_EncodeSingleBin_CABAC(state, ctx+ctx_mod, 1);
        H264BsFake_EncodeExGRepresentedMVS_CABAC(state, ctx+3,labs(vector)-1);
        H264BsFake_EncodeBypass_CABAC(state, vector<0);
    }
    return UMC_OK;
}

/*!
 ****************************************************************************
 * \brief
 *    This function is used to arithmetically encode the coded
 *    block pattern of a given delta quant.
 ****************************************************************************
 */
Status H264BsFake_DQuant_CABAC(
    void* state,
    Ipp32s deltaQP,
    Ipp32s left_c)
{
    H264BsFake* bs = (H264BsFake *)state;
    Ipp32s value;
    Ipp8u* ctx = bs->m_base.context_array + ctxIdxOffset[MB_QP_DELTA];
    value = 2*labs(deltaQP)+(deltaQP<=0)-1;

    if (deltaQP==0){
      H264BsFake_EncodeSingleBin_CABAC(state, ctx+left_c,0);
    }else{
        H264BsFake_EncodeSingleBin_CABAC(state, ctx+left_c,1);
//      EncodeUnaryRepresentedSymbol_CABAC(ctxIdxOffset[MB_QP_DELTA]+2,1,value-1);
        if (value == 1){
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+2, 0);
        }else{
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+2, 1);
            Ipp32s temp=value-1;
            while ((--temp)>0)
                H264BsFake_EncodeSingleBin_CABAC(state, ctx+2+1, 1);
//          if ((value-1)<0x7fffffff)
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+2+1, 0);
        }
    }
    return UMC_OK;
}

template<typename COEFFSTYPE>
Status H264BsFake_ResidualBlock_CABAC(
    void* state,
    T_Block_CABAC_Data<COEFFSTYPE> *c_data,
    bool frame_block)
{
    H264BsFake* bs = (H264BsFake *)state;
    Ipp32s ctxIdxOffsetSig;
    Ipp32s ctxIdxOffsetLastSig;
    Ipp32s ctxIdxOffsetAbsLevelMinus1;
    Ipp32s firstCoeff = c_data->uFirstCoeff;
    Ipp32s numCoeffs = c_data->uNumSigCoeffs;
    Ipp8u *ctx, *ctx_last, *ctx_sig;
    const Ipp32s* ctx_trans0 = &ctx_id_trans0[0];
    const Ipp32s* ctx_trans1;

    if(c_data->CtxBlockCat == 5) {
        if(frame_block) {
            ctxIdxOffsetSig = ctxIdxOffsetFrameCoded_BlockCat_5[SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetLastSig = ctxIdxOffsetFrameCoded_BlockCat_5[LAST_SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[LAST_SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetAbsLevelMinus1 = ctxIdxOffsetFrameCoded_BlockCat_5[COEFF_ABS_LEVEL_MINUS1];
        }
        else {
            ctxIdxOffsetSig = ctxIdxOffsetFieldCoded_BlockCat_5[SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetLastSig = ctxIdxOffsetFieldCoded_BlockCat_5[LAST_SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[LAST_SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetAbsLevelMinus1 = ctxIdxOffsetFieldCoded_BlockCat_5[COEFF_ABS_LEVEL_MINUS1];
        }
        ctx_sig = bs->m_base.context_array + ctxIdxOffsetSig;
        ctx_last = bs->m_base.context_array + ctxIdxOffsetLastSig;
        ctx_trans1 = &ctx_id_trans1[0];

        Ipp32s i,j;
        for (i=0;i<numCoeffs;i++)
        {
            Ipp32s num=c_data->uSignificantMap[i];
            Ipp32s prevnum = i?c_data->uSignificantMap[i-1]:firstCoeff-1;
            if (num>prevnum+1){
               for (j=prevnum+1;j<num;j++) H264BsFake_EncodeSingleBin_CABAC(state, ctx_sig+Table_9_34[!frame_block][j-firstCoeff],0);
            }
            if (num>=c_data->uLastCoeff) break;
            H264BsFake_EncodeSingleBin_CABAC(state, ctx_sig + Table_9_34[!frame_block][num-firstCoeff],1);
            H264BsFake_EncodeSingleBin_CABAC(state, ctx_last + Table_9_34[2][num-firstCoeff],i==numCoeffs-1);
        }
    }else {
        if(frame_block) {
            ctxIdxOffsetSig = ctxIdxOffsetFrameCoded[SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetLastSig = ctxIdxOffsetFrameCoded[LAST_SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[LAST_SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetAbsLevelMinus1 = ctxIdxOffsetFrameCoded[COEFF_ABS_LEVEL_MINUS1];
        }
        else {
            ctxIdxOffsetSig = ctxIdxOffsetFieldCoded[SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetLastSig = ctxIdxOffsetFieldCoded[LAST_SIGNIFICANT_COEFF_FLAG] + ctxIdxBlockCatOffset[LAST_SIGNIFICANT_COEFF_FLAG][c_data->CtxBlockCat];
            ctxIdxOffsetAbsLevelMinus1 = ctxIdxOffsetFieldCoded[COEFF_ABS_LEVEL_MINUS1];
        }
        ctx_sig = bs->m_base.context_array + ctxIdxOffsetSig;
        ctx_last = bs->m_base.context_array + ctxIdxOffsetLastSig;

        Ipp32s i,j;
        if( c_data->CtxBlockCat == 3 ){
            ctx_trans1 = &ctx_id_trans13[0];
            for (i=0;i<numCoeffs;i++){
                Ipp32s num=c_data->uSignificantMap[i];
                Ipp32s prevnum = i?c_data->uSignificantMap[i-1]:firstCoeff-1;
                if (num > prevnum + 1)
                    for (j = prevnum + 1; j < num; j++)
                        H264BsFake_EncodeSingleBin_CABAC(
                            state,
                            ctx_sig + MIN(((j - firstCoeff) >> bs->num8x8Cshift2), 2),
                            0);

                if (num >= c_data->uLastCoeff)
                    break;
                H264BsFake_EncodeSingleBin_CABAC(
                    state,
                    ctx_sig + MIN(((num - firstCoeff) >> bs->num8x8Cshift2), 2),
                    1);
                H264BsFake_EncodeSingleBin_CABAC(
                    state,
                    ctx_last + MIN(((num-firstCoeff) >> bs->num8x8Cshift2), 2),
                    i == numCoeffs - 1);
            }
        }else{
            ctx_trans1 = &ctx_id_trans1[0];
            for (i=0;i<numCoeffs;i++){
                Ipp32s num=c_data->uSignificantMap[i];
                Ipp32s prevnum = i?c_data->uSignificantMap[i-1]:firstCoeff-1;
                if (num>prevnum+1)
                    for (j=prevnum+1;j<num;j++) H264BsFake_EncodeSingleBin_CABAC(state, ctx_sig + j - firstCoeff,0);
                if ( num >= c_data->uLastCoeff ) break;
                H264BsFake_EncodeSingleBin_CABAC(state, ctx_sig + num - firstCoeff,1);
                H264BsFake_EncodeSingleBin_CABAC(state, ctx_last + num - firstCoeff,i==numCoeffs-1);
            }
        }
    }
    Ipp32s i;
    Ipp32s ctx_id = 0;
    Ipp32s CtxInc;
    COEFFSTYPE* levels = c_data->uSignificantLevels;
    ctx = bs->m_base.context_array + ctxIdxOffsetAbsLevelMinus1 + ctxIdxBlockCatOffset[COEFF_ABS_LEVEL_MINUS1][c_data->CtxBlockCat];
    for (i=numCoeffs-1; i>=0; i--){
        Ipp32s coef=levels[i];
        CtxInc = ctx_neq1p1[ctx_id];
        if (coef){
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+CtxInc, 1);
            H264BsFake_EncodeExGRepresentedLevels_CABAC(state, ctx+ctx_ngt1[ctx_id], coef-1);
            ctx_id = ctx_trans1[ctx_id];
        }else{
            H264BsFake_EncodeSingleBin_CABAC(state, ctx+CtxInc, 0);
            ctx_id = ctx_trans0[ctx_id];
        }
        H264BsFake_EncodeBypass_CABAC(state, c_data->uSignificantSigns[i] != 0);
    }
    return UMC_OK;
}

Status H264BsFake_PutScalingList(
    void* state,
    const Ipp8u* scalingList,
    Ipp32s sizeOfScalingList,
    bool& useDefaultScalingMatrixFlag)
{
    Ipp16s lastScale, nextScale;
    Ipp32s j;

    Ipp16s delta_scale;
    Ipp8s delta_code;
    const Ipp32s* scan;

    lastScale=nextScale=8;

    if( sizeOfScalingList == 16 )
        scan = dec_single_scan[0];
    else
        scan = dec_single_scan_8x8[0];

    for( j = 0; j<sizeOfScalingList; j++ ){
         if( nextScale != 0 ){
            delta_scale = scalingList[scan[j]]-lastScale;
            delta_code = (Ipp8s)(delta_scale);

            //Put delta_scale
            H264BsFake_PutVLCCode(state, SIGNED_VLC_CODE(delta_code));

            nextScale = scalingList[scan[j]];
            useDefaultScalingMatrixFlag = ( j==0 && nextScale == 0 ); //Signal that input matrix is incorrect
     }
         lastScale = (nextScale==0) ? lastScale:nextScale;
    //scalingList[scan[j]] = (nextScale==0) ? lastScale:nextScale; // Update the actual scalingList matrix with the correct values
    //lastScale = scalingList[scan[j]];
    }

    return UMC_OK;
}

Status H264BsFake_PutSEI_UserDataUnregistred(
    void* state,
    void* data,
    Ipp32s data_size )
{
    static const Ipp8u UUID[16] = {
        0x0d, 0xad, 0x3f, 0x34,
        0x8e, 0x9a, 0x48, 0xd7,
        0x8a, 0x7e, 0xc1, 0xe1,
        0xb4, 0x7b, 0xdf, 0x9a
    };

    H264BsFake* bs = (H264BsFake *)state;
    Ipp8u* user_data = (Ipp8u*)data;
    Ipp32s i;

    data_size += 16; //+ UUID size
    //payload type
    H264BsFake_PutBits(state, SEI_TYPE_USER_DATA_UNREGISTERED, 8);
    //payload size
    while( data_size > 255 ){
        H264BsFake_PutBits(state, 0xff, 8);
        data_size -= 255;
    }
    H264BsFake_PutBits(state, data_size, 8);

    //uuid
    for( i=0; i<16; i++ )
        H264BsFake_PutBits(state, UUID[i], 8);

    for( i=0; i<data_size-16; i++ )
        H264BsFake_PutBits(state, user_data[i], 8);

    if (H264BsBase_GetBsOffset(&(bs->m_base)) & 0x07)
        H264BsBase_WriteTrailingBits(&bs->m_base);

    return UMC_OK;
}

Status H264BsFake_PutSEI_BufferingPeriod(
    void* state,
    const H264SeqParamSet& seq_parms,
    const Ipp8u NalHrdBpPresentFlag,
    const Ipp8u VclHrdBpPresentFlag,
    Ipp32u * nal_initial_cpb_removal_delay,
    Ipp32u * nal_initial_cpb_removal_delay_offset,
    Ipp32u * vcl_initial_cpb_removal_delay,
    Ipp32u * vcl_initial_cpb_removal_delay_offset)
{
    Ipp32s SchedSelIdx;
    Ipp32s data_size, data_size_bits;

    Ipp8u seq_parameter_set_id, cpb_cnt_minus1, initial_cpb_removal_delay_length_minus1;

    seq_parameter_set_id = seq_parms.seq_parameter_set_id;

    if (seq_parms.vui_parameters_present_flag)
    {
        if (seq_parms.vui_parameters.nal_hrd_parameters_present_flag)
        {
            cpb_cnt_minus1 = seq_parms.vui_parameters.hrd_params.cpb_cnt_minus1;
            initial_cpb_removal_delay_length_minus1 = seq_parms.vui_parameters.hrd_params.initial_cpb_removal_delay_length_minus1;
        }
        else if (seq_parms.vui_parameters.vcl_hrd_parameters_present_flag)
        {
            cpb_cnt_minus1 = seq_parms.vui_parameters.vcl_hrd_params.cpb_cnt_minus1;
            initial_cpb_removal_delay_length_minus1 = seq_parms.vui_parameters.vcl_hrd_params.initial_cpb_removal_delay_length_minus1;
        }
        else
        {
            cpb_cnt_minus1 = 0;
            initial_cpb_removal_delay_length_minus1 = 23;
        }
    }
    else
    {
        cpb_cnt_minus1 = 0;
        initial_cpb_removal_delay_length_minus1 = 23;
    }

    H264BsFake* bs = (H264BsFake *)state;

    data_size_bits = (Ipp32s)(log(float(seq_parameter_set_id))/log(2.0)) * 2 + 1;
    data_size_bits += (NalHrdBpPresentFlag + VclHrdBpPresentFlag) * (cpb_cnt_minus1 + 1) * (initial_cpb_removal_delay_length_minus1 + 1) * 2;
    data_size = data_size_bits/8;
    data_size += (data_size_bits%8 != 0);

    //payload type
    H264BsFake_PutBits(state, SEI_TYPE_BUFFERING_PERIOD, 8);
    //payload size
    while( data_size > 255 ){
        H264BsFake_PutBits(state, 0xff, 8);
        data_size -= 255;
    }
    H264BsFake_PutBits(state, data_size, 8);

    H264BsFake_PutVLCCode(state, seq_parameter_set_id);

    if (NalHrdBpPresentFlag)
        for (SchedSelIdx = 0; SchedSelIdx <= cpb_cnt_minus1; SchedSelIdx++)
        {
            H264BsFake_PutBits(state, nal_initial_cpb_removal_delay[SchedSelIdx], initial_cpb_removal_delay_length_minus1 + 1);
            H264BsFake_PutBits(state, nal_initial_cpb_removal_delay_offset[SchedSelIdx], initial_cpb_removal_delay_length_minus1 + 1);
        }

    if (VclHrdBpPresentFlag)
        for (SchedSelIdx = 0; SchedSelIdx <= cpb_cnt_minus1; SchedSelIdx++)
        {
            H264BsFake_PutBits(state, vcl_initial_cpb_removal_delay[SchedSelIdx], initial_cpb_removal_delay_length_minus1 + 1);
            H264BsFake_PutBits(state, vcl_initial_cpb_removal_delay_offset[SchedSelIdx], initial_cpb_removal_delay_length_minus1 + 1);
        }

        if (H264BsBase_GetBsOffset(&(bs->m_base)) & 0x07)
            H264BsBase_WriteTrailingBits(&bs->m_base);

    return UMC_OK;
}

Status H264BsFake_PutSEI_PictureTiming(
    void* state,
    const H264SeqParamSet& seq_parms,
    const Ipp8u CpbDpbDelaysPresentFlag,
    const Ipp8u pic_struct_present_flag,
    Ipp32u cpb_removal_delay,
    Ipp32u dpb_output_delay,
    const PicStructTimingData& timing_data)
{
    Ipp32s i, NumClockTS=0;
    H264BsFake* bs = (H264BsFake *)state;
    Ipp32s data_size, data_size_bits = 0;

    Ipp8u cpb_removal_delay_length_minus1, dpb_output_delay_length_minus1, time_offset_length;

    if (seq_parms.vui_parameters_present_flag)
    {
        if (seq_parms.vui_parameters.nal_hrd_parameters_present_flag)
        {
            cpb_removal_delay_length_minus1 =  seq_parms.vui_parameters.hrd_params.cpb_removal_delay_length_minus1;
            dpb_output_delay_length_minus1 =  seq_parms.vui_parameters.hrd_params.dpb_output_delay_length_minus1;
            time_offset_length = seq_parms.vui_parameters.hrd_params.time_offset_length;
        }
        else if (seq_parms.vui_parameters.vcl_hrd_parameters_present_flag)
        {
            cpb_removal_delay_length_minus1 =  seq_parms.vui_parameters.vcl_hrd_params.cpb_removal_delay_length_minus1;
            dpb_output_delay_length_minus1 =  seq_parms.vui_parameters.vcl_hrd_params.dpb_output_delay_length_minus1;
            time_offset_length = seq_parms.vui_parameters.vcl_hrd_params.time_offset_length;
        }
        else
        {
            cpb_removal_delay_length_minus1 =  23;
            dpb_output_delay_length_minus1 =  23;
            time_offset_length = 24;
        }
    }
    else
    {
        cpb_removal_delay_length_minus1 =  23;
        dpb_output_delay_length_minus1 =  23;
        time_offset_length = 24;
    }

    if (CpbDpbDelaysPresentFlag)
    {
        data_size_bits += cpb_removal_delay_length_minus1 + dpb_output_delay_length_minus1 + 2;
    }

    if (pic_struct_present_flag)
    {
        if (timing_data.pic_struct > 8)
            return UMC_ERR_FAILED;
        NumClockTS = (timing_data.pic_struct <= 2) ? 1 : (timing_data.pic_struct <= 4 || timing_data.pic_struct == 7) ? 2 : 3;
        data_size_bits += (4 + NumClockTS); // pic_struct and clock_timestamp_flags

        for (i = 0; i < NumClockTS; i ++)
        {
            Ipp8u timestamp_data_size_bits = timing_data.full_timestamp_flag[i] ? 17 : (1 +
                timing_data.seconds_flag[i] * (7 + timing_data.minutes_flag[i]*(7 + timing_data.hours_flag[i]*5)));
            data_size_bits += timing_data.clock_timestamp_flag[i] * (19 +
            timestamp_data_size_bits + time_offset_length);
        }

    }

    data_size = data_size_bits/8;
    data_size += (data_size_bits%8 != 0);

    //payload type
    H264BsFake_PutBits(state, SEI_TYPE_PIC_TIMING, 8);
    //payload size
    while( data_size > 255 ){
        H264BsFake_PutBits(state, 0xff, 8);
        data_size -= 255;
    }
    H264BsFake_PutBits(state, data_size, 8);

    if (CpbDpbDelaysPresentFlag)
    {
        H264BsFake_PutBits(state, cpb_removal_delay, cpb_removal_delay_length_minus1 + 1);
        H264BsFake_PutBits(state, dpb_output_delay, dpb_output_delay_length_minus1 + 1);
    }

    if (pic_struct_present_flag)
    {
        H264BsFake_PutBits(state, timing_data.pic_struct, 4);
        for (i = 0; i < NumClockTS; i ++)
        {
            H264BsFake_PutBit(state, timing_data.clock_timestamp_flag[i]);
            if (timing_data.clock_timestamp_flag[i])
            {
                H264BsFake_PutBits(state, timing_data.ct_type[i], 2);
                H264BsFake_PutBit(state, timing_data.nuit_field_based_flag[i]);
                H264BsFake_PutBits(state, timing_data.counting_type[i], 5);
                H264BsFake_PutBit(state, timing_data.full_timestamp_flag[i]);
                H264BsFake_PutBit(state, timing_data.discontinuity_flag[i]);
                H264BsFake_PutBit(state, timing_data.cnt_dropped_flag[i]);
                H264BsFake_PutBits(state, timing_data.n_frames[i], 8);
                if (timing_data.full_timestamp_flag[i])
                {
                    H264BsFake_PutBits(state, timing_data.seconds_value[i], 6);
                    H264BsFake_PutBits(state, timing_data.minutes_value[i], 6);
                    H264BsFake_PutBits(state, timing_data.hours_value[i], 5);
                }
                else
                {
                    H264BsFake_PutBit(state, timing_data.seconds_flag[i]);
                    if (timing_data.seconds_flag[i])
                    {
                        H264BsFake_PutBits(state, timing_data.seconds_value[i], 6);
                        H264BsFake_PutBit(state, timing_data.minutes_flag[i]);
                        if (timing_data.minutes_flag[i])
                        {
                            H264BsFake_PutBits(state, timing_data.minutes_value[i], 6);
                            H264BsFake_PutBit(state, timing_data.hours_flag[i]);
                            if (timing_data.hours_flag[i])
                                H264BsFake_PutBits(state, timing_data.hours_value[i], 5);
                        }
                    }
                }
                H264BsFake_PutBits(state, timing_data.time_offset[i], time_offset_length);
            }
        }
    }

    if (H264BsBase_GetBsOffset(&(bs->m_base)) & 0x07)
        H264BsBase_WriteTrailingBits(&bs->m_base);

    return UMC_OK;
}

Status H264BsFake_PutSEI_RecoveryPoint(
    void* state,
    Ipp32s recovery_frame_cnt,
    Ipp8u exact_match_flag,
    Ipp8u broken_link_flag,
    Ipp8u changing_slice_group_idc)
{
    H264BsFake* bs = (H264BsFake *)state;
    Ipp32s data_size, data_size_bits = 0;

    data_size_bits = (Ipp32s)(log(float(recovery_frame_cnt))/log(2.0)) * 2 + 5;

    data_size = data_size_bits/8;
    data_size += (data_size_bits%8 != 0);

    //payload type
    H264BsFake_PutBits(state, SEI_TYPE_RECOVERY_POINT, 8);
    //payload size
    while( data_size > 255 ){
        H264BsFake_PutBits(state, 0xff, 8);
        data_size -= 255;
    }
    H264BsFake_PutBits(state, data_size, 8);

    H264BsFake_PutVLCCode(state, recovery_frame_cnt);
    H264BsFake_PutBit(state, exact_match_flag);
    H264BsFake_PutBit(state, broken_link_flag);
    H264BsFake_PutBits(state, changing_slice_group_idc,2);

    if (H264BsBase_GetBsOffset(&(bs->m_base)) & 0x07)
        H264BsBase_WriteTrailingBits(&bs->m_base);

    return UMC_OK;
}


// forced instantiation
#ifdef BITDEPTH_9_12
#define COEFFSTYPE Ipp32s
#define PIXTYPE Ipp16u
template Status H264BsReal_PutSliceHeader<COEFFSTYPE, PIXTYPE>(void*, const H264SliceHeader&, const H264PicParamSet&,  const H264SeqParamSet&, const EnumPicClass&, const H264Slice<COEFFSTYPE, PIXTYPE>*);
template Status H264BsReal_PutLevels<COEFFSTYPE>(void*, COEFFSTYPE*, Ipp32s, Ipp32s);
template Status H264BsReal_ResidualBlock_CABAC<COEFFSTYPE>(void*, T_Block_CABAC_Data<COEFFSTYPE>*, bool);

template Status H264BsFake_PutLevels<COEFFSTYPE>(void*, COEFFSTYPE*, Ipp32s, Ipp32s);
template Status H264BsFake_ResidualBlock_CABAC<COEFFSTYPE>(void*, T_Block_CABAC_Data<COEFFSTYPE>*, bool);

#undef COEFFSTYPE
#undef PIXTYPE
#endif

#define COEFFSTYPE Ipp16s
#define PIXTYPE Ipp8u
template Status H264BsReal_PutSliceHeader<COEFFSTYPE, PIXTYPE>(void*, const H264SliceHeader&, const H264PicParamSet&,  const H264SeqParamSet&, const EnumPicClass&, const H264Slice<COEFFSTYPE, PIXTYPE>*);
template Status H264BsReal_PutLevels<COEFFSTYPE>(void*, COEFFSTYPE*, Ipp32s, Ipp32s);
template Status H264BsReal_ResidualBlock_CABAC<COEFFSTYPE>(void*, T_Block_CABAC_Data<COEFFSTYPE>*, bool);

template Status H264BsFake_PutLevels<COEFFSTYPE>(void*, COEFFSTYPE*, Ipp32s, Ipp32s);
template Status H264BsFake_ResidualBlock_CABAC<COEFFSTYPE>(void*, T_Block_CABAC_Data<COEFFSTYPE>*, bool);

#endif //UMC_ENABLE_H264_VIDEO_ENCODER

