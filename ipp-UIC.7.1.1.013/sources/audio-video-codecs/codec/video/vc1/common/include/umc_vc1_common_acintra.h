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

#ifndef __UMC_VC1_COMMON_ACINTRA_H__
#define __UMC_VC1_COMMON_ACINTRA_H__

#include "umc_vc1_common_defs.h"
//Tables 178 - 184 High motion Intra
//Tables 192 - 198 Low  motion Intra
//Tables 206 - 212 Mid Rate Intra
//Tables 220 - 226 High Rate Intra

//////////////////////////////////////////////////////////////////////////
//////////////////////High Motion Intra///////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 181: High Motion Intra Delta Level Indexed by Run Table (Last = 0)
extern const Ipp8s VC1_HighMotionIntraDeltaLevelLast0[31];

//VC-1 Table 182: High Motion Intra Delta Level Indexed by Run Table (Last = 1)
extern const Ipp8s VC1_HighMotionIntraDeltaLevelLast1[38];

//VC-1 Table 183: High Motion Intra Delta Run Indexed by Level Table (Last = 0)
extern const Ipp8s VC1_HighMotionIntraDeltaRunLast0[20];

//VC-1 Table 184: High Motion Intra Delta Run Indexed by Level Table (Last = 1)
extern const Ipp8s VC1_HighMotionIntraDeltaRunLast1[7];

//////////////////////////////////////////////////////////////////////////
//////////////////////Low Motion Intra///////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 195: Low Motion Intra Delta Level Indexed by Run Table (Last = 0)
extern const Ipp8s VC1_LowMotionIntraDeltaLevelLast0[21];

//VC-1 Table 196: Low Motion Intra Delta Level Indexed by Run Table (Last = 1)
extern const Ipp8s VC1_LowMotionIntraDeltaLevelLast1[27];

//VC-1 Table 197: Low Motion Intra Delta Run Indexed by Level Table (Last = 0)
extern const Ipp8s VC1_LowMotionIntraDeltaRunLast0[16+1];

//VC-1 Table 198: Low Motion Intra Delta Run Indexed by Level Table (Last = 1)
extern const Ipp8s VC1_LowMotionIntraDeltaRunLast1[4+1];

//////////////////////////////////////////////////////////////////////////
//////////////////////Mid Rate Intra//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 209: Mid Rate Intra Delta Level Indexed by Run Table (Last = 0)
extern const Ipp8s VC1_MidRateIntraDeltaLevelLast0[15];

//VC-1 Table 210: Mid Rate Intra Delta Level Indexed by Run Table (Last = 1)
extern const Ipp8s VC1_MidRateIntraDeltaLevelLast1[21];

//VC-1 Table 211: Mid Rate Intra Delta Run Indexed by Level Table (Last = 0)
extern const Ipp8s VC1_MidRateIntraDeltaRunLast0[28];

//VC-1 Table 212: Mid Rate Intra Delta Run Indexed by Level Table (Last = 1)
extern const Ipp8s VC1_MidRateIntraDeltaRunLast1[9];

//////////////////////////////////////////////////////////////////////////
//////////////////////High Rate Intra//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 223: High Rate Intra Delta Level Indexed by Run Table (Last = 0)
extern const Ipp8s VC1_HighRateIntraDeltaLevelLast0[15];

//VC-1 Table 224: High Rate Intra Delta Level Indexed by Run Table (Last = 1)
extern const Ipp8s VC1_HighRateIntraDeltaLevelLast1[17];

//VC-1 Table 225: High Rate Intra Delta Run Indexed by Level Table (Last = 0)
extern const Ipp8s VC1_HighRateIntraDeltaRunLast0[57];

//VC-1 Table 226: High Rate Intra Delta Run Indexed by Level Table (Last = 1)
extern const Ipp8s VC1_HighRateIntraDeltaRunLast1[5];

#endif //__umc_vc1_common_acintra_H__
