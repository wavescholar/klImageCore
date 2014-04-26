/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, coding tables
//
*/

#ifndef _ENCODER_VC1_COMMON_TABLES_H_
#define _ENCODER_VC1_COMMON_TABLES_H_

#include "ippdefs.h"
#include "umc_vc1_common_defs_enc.h"

namespace UMC_VC1_ENCODER
{

    extern const Ipp32u DCLumaLowMotionEncTableVLC   [120*2];
    extern const Ipp32u DCLumaHighMotionEncTableVLC  [120*2];
    extern const Ipp32u DCChromaLowMotionEncTableVLC [120*2];
    extern const Ipp32u DCCromaHighMotionEncTableVLC [120*2];

    extern const Ipp8u HighMotionIntraIndexLevelLast0[31];
    extern const Ipp8u HighMotionIntraIndexLevelLast1[38];
    extern const Ipp8u LowMotionIntraIndexLevelLast0 [21];
    extern const Ipp8u LowMotionIntraIndexLevelLast1 [27];
    extern const Ipp8u MidRateIntraIndexLevelLast0   [15];
    extern const Ipp8u MidRateIntraIndexLevelLast1   [21];
    extern const Ipp8u HighRateIntraIndexLevelLast0  [15];
    extern const Ipp8u HighRateIntraIndexLevelLast1  [17];


    extern const Ipp8u Mode3SizeConservativeVLC[12*2];
    extern const Ipp8u Mode3SizeEfficientVLC   [12*2];

    extern const Ipp8u  frameTypeCodesVLC[2][4*2];
    extern const Ipp8u  MVRangeCodesVLC     [4*2];
    extern const Ipp8u  ACTableCodesVLC     [3*2];

    extern const Ipp8u  DCQuantValues       [32];
    extern const Ipp8u  quantValue          [32];



    extern const Ipp32u* DCTables[2][2];
    extern const sACTablesSet ACTablesSet[8];

    extern const Ipp8u HorPrediction[VC1_ENC_BLOCK_SIZE];
    extern const Ipp8u VerPrediction[VC1_ENC_BLOCK_SIZE];
    extern const Ipp8u NonPrediction[VC1_ENC_BLOCK_SIZE];

    extern const Ipp8u MVModeP [4][4*2];


    extern const Ipp8u QuantProfileTableVLC[12*2];

    extern const Ipp16u*  MVDiffTablesVLC[4];

    extern const Ipp8u    MVSizeOffset[VC1_ENC_MV_LIMIT*3];
    extern const Ipp8u    longMVLength[4*2];
    extern const Ipp16s   MVRange[4*2];
    extern const Ipp16s   MVRangeFields[4*2];

    extern const Ipp16u   VLCTableCBPCY_I[64*2];

    extern const    eCodingSet  LumaCodingSetsIntra[2][3];
    extern const    eCodingSet  ChromaCodingSetsIntra[2][3];
    extern const    eCodingSet  CodingSetsInter[2][3];

    extern const Ipp16u VLCTableCBPCY_PB_0[64*2];
    extern const Ipp16u VLCTableCBPCY_PB_1[64*2];
    extern const Ipp16u VLCTableCBPCY_PB_2[64*2];
    extern const Ipp16u VLCTableCBPCY_PB_3[64*2];

    extern const Ipp16u* VLCTableCBPCY_PB[4];

    extern const Ipp8u    BMVTypeVLC[2][3*2];
    extern const Ipp8u    BFractionVLC[9][8*2];
    extern const Ipp8u    BFractionScaleFactor[9][8];

    extern const  Ipp16s TTMBVLC_HighRate[2][4][3*2];            /*PQUANT<5 */
    extern const  Ipp16s TTMBVLC_MediumRate[2][4][3*2];          /*5<=PQUANT<13 */
    extern const  Ipp16s TTMBVLC_LowRate[2][4][3*2];             /* PQUANT>=13 */

    extern const Ipp8u  TTBLKVLC_HighRate[4][3*2];
    extern const Ipp8u  TTBLKVLC_MediumRate[4][3*2];
    extern const Ipp8u  TTBLKVLC_LowRate[4][3*2];

    extern const Ipp8u  SubPattern4x4VLC_HighRate[16*2];           /*PQUANT<5 */
    extern const Ipp8u  SubPattern4x4VLC_MediumRate[16*2] ;        /*5<=PQUANT<13 */
    extern const Ipp8u  SubPattern4x4VLC_LowRate[16*2] ;           /* PQUANT>=13 */

    extern const Ipp8u SubPatternMask[4][2];
    extern const Ipp8u SubPattern8x4_4x8VLC[3*2];

    extern const Ipp8u* ZagTables_I[3];
    extern const Ipp8u* ZagTables[4];
    extern const Ipp8u* ZagTables_Adv[4];
    extern const Ipp8u VC1_Inter_8x8_Scan[64];
    extern const Ipp8u VC1_Inter_8x4_Scan[32];
    extern const Ipp8u VC1_Inter_8x4_Scan_Adv[32];
    extern const Ipp8u VC1_Inter_4x8_Scan[32];
    extern const Ipp8u VC1_Inter_4x8_Scan_Adv[32];
    extern const Ipp8u VC1_Inter_4x4_Scan[16];
    extern const Ipp8u VC1_Inter_Interlace_8x4_Scan_Adv[32];
    extern const Ipp8u VC1_Inter_Interlace_4x8_Scan_Adv[32];
    extern const Ipp8u VC1_Inter_Interlace_4x4_Scan_Adv[16];

    extern const eTransformType BlkTransformTypeTabl[4];

    extern const Ipp8u*  MBTypeFieldMixedTable_VLC[8] ;
    extern const Ipp8u*  MBTypeFieldTable_VLC[8] ;
    extern const Ipp32u* MVModeField2RefTable_VLC[8];
    extern const Ipp32u* MVModeField1RefTable_VLC[4];
    extern const Ipp32u* CBPCYFieldTable_VLC[8];

    extern const Ipp8u MVSizeOffsetFieldIndex [256];
    extern const Ipp8u MVSizeOffsetFieldExIndex [512];
    extern const Ipp16s MVSizeOffsetField [10];
    extern const Ipp16s MVSizeOffsetFieldEx [10];

    extern const Ipp8u VC1_Inter_InterlaceIntra_8x8_Scan_Adv[64];
    extern const Ipp8u* ZagTables_Fields[4];
    extern const Ipp8u MVModeBField [2][4*2];

}

#endif
