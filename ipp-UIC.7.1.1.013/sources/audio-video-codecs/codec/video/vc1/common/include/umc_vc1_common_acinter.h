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

#ifndef __UMC_VC1_COMMON_ACINTER_H__
#define __UMC_VC1_COMMON_ACINTER_H__

#include "umc_vc1_common_defs.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////High Motion Inter///////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 188: High Motion Inter Delta Level Indexed by Run Table (Last = 0)
extern const Ipp8s VC1_HighMotionInterDeltaLevelLast0[27];

//VC-1 Table 189: High Motion Inter Delta Level Indexed by Run Table (Last = 1)
extern const Ipp8s VC1_HighMotionInterDeltaLevelLast1[37];

//VC-1 Table 190: High Motion Inter Delta Run Indexed by Level Table (Last = 0)
extern const Ipp8s VC1_HighMotionInterDeltaRunLast0[24];

//VC-1 Table 191: High Motion Inter Delta Run Indexed by Level Table (Last = 1)
extern const Ipp8s VC1_HighMotionInterDeltaRunLast1[10];



//////////////////////////////////////////////////////////////////////////
//////////////////////Low Motion Inter///////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 202: Low Motion Inter Delta Level Indexed by Run Table (Last = 0)
extern const Ipp8s VC1_LowMotionInterDeltaLevelLast0[30];

//VC-1 Table 203: Low Motion Inter Delta Level Indexed by Run Table (Last = 1)
extern const Ipp8s VC1_LowMotionInterDeltaLevelLast1[44];

//VC-1 Table 204: Low Motion Inter Delta Run Indexed by Level Table (Last = 0)
extern const Ipp8s VC1_LowMotionInterDeltaRunLast0[15];

//VC-1 Table 205: Low Motion Inter Delta Run Indexed by Level Table (Last = 1)
extern const Ipp8s VC1_LowMotionInterDeltaRunLast1[6];

//////////////////////////////////////////////////////////////////////////
//////////////////////Mid Rate Inter//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 216: Mid Rate Inter Delta Level Indexed by Run Table (Last = 0)
extern const Ipp8s VC1_MidRateInterDeltaLevelLast0[27];

//VC-1 Table 217: Mid Rate Inter Delta Level Indexed by Run Table (Last = 1)
extern const Ipp8s VC1_MidRateInterDeltaLevelLast1[41];

//VC-1 Table 218: Mid Rate Inter Delta Run Indexed by Level Table (Last = 0)
extern const Ipp8s VC1_MidRateInterDeltaRunLast0[13];

//VC-1 Table 219: Mid Rate Inter Delta Run Indexed by Level Table (Last = 1)
extern const Ipp8s VC1_MidRateInterDeltaRunLast1[4];

//////////////////////////////////////////////////////////////////////////
//////////////////////High Rate Inter/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//VC-1 Table 230: High Rate Inter Delta Level Indexed by Run Table (Last = 0)
extern const Ipp8s VC1_HighRateInterDeltaLevelLast0[25];

//VC-1 Table 231: High Rate Inter Delta Level Indexed by Run Table (Last = 1)
extern const Ipp8s VC1_HighRateInterDeltaLevelLast1[31];

//VC-1 Table 232: High Rate Inter Delta Run Indexed by Level Table (Last = 0)
extern const Ipp8s VC1_HighRateInterDeltaRunLast0[33];

//VC-1 Table 233: High Rate Inter Delta Run Indexed by Level Table (Last = 1)
extern const Ipp8s VC1_HighRateInterDeltaRunLast1[5];

#endif //__umc_vc1_common_acinter_H__
