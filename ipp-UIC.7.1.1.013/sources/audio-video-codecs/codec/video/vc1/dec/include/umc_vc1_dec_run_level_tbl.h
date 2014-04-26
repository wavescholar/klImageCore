/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2004-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) decoder, run level tables
//
*/

#ifndef __UMC_VC1_DEC_RUN_LEVEL_TBL_H__
#define __UMC_VC1_DEC_RUN_LEVEL_TBL_H__

#include "umc_vc1_common_defs.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////Intra Decoding Sets/////////////////////////////////
//////////////////////////////////////////////////////////////////////////
extern VC1ACDecodeSet LowMotionIntraACDecodeSet;

extern VC1ACDecodeSet HighMotionIntraACDecodeSet;

extern VC1ACDecodeSet MidRateIntraACDecodeSet;

extern VC1ACDecodeSet HighRateIntraACDecodeSet;


extern VC1ACDecodeSet* IntraACDecodeSetPQINDEXle7[3];

extern VC1ACDecodeSet* IntraACDecodeSetPQINDEXgt7[3];

//////////////////////////////////////////////////////////////////////////
//////////////////////Inter Decoding Sets/////////////////////////////////
//////////////////////////////////////////////////////////////////////////
extern VC1ACDecodeSet LowMotionInterACDecodeSet;

extern VC1ACDecodeSet HighMotionInterACDecodeSet;

extern VC1ACDecodeSet MidRateInterACDecodeSet;

extern VC1ACDecodeSet HighRateInterACDecodeSet;

extern VC1ACDecodeSet* InterACDecodeSetPQINDEXle7[3];

extern VC1ACDecodeSet* InterACDecodeSetPQINDEXgt7[3];

extern const Ipp32s VC1_DQScaleTbl[64];

extern const Ipp32s VC1_HighMotionIntraAC[];
extern const Ipp32s VC1_HighMotionInterAC[];
extern const Ipp32s VC1_LowMotionIntraAC[];
extern const Ipp32s VC1_LowMotionInterAC[];
extern const Ipp32s VC1_MidRateIntraAC[];
extern const Ipp32s VC1_MidRateInterAC[];
extern const Ipp32s VC1_HighRateIntraAC[];
extern const Ipp32s VC1_HighRateInterAC[];

#endif //__umc_vc1_dec_run_level_tbl_H__
