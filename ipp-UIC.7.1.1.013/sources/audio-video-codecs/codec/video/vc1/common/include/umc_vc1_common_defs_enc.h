/* /////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2011-2012 Intel Corporation. All Rights Reserved.
//
//
//          VC-1 (VC1) encoder, common declarations
//
*/

#ifndef _ENCODER_VC1_COMMON_ENC
#define _ENCODER_VC1_COMMON_ENC

#include "ippdefs.h"

namespace UMC_VC1_ENCODER
{
    #define VC1_ENC_BLOCK_SIZE          64
    #define VC1_ENC_MV_LIMIT            159
    typedef enum
    {
        VC1_ENC_HIGH_MOTION_INTRA   = 0,
        VC1_ENC_LOW_MOTION_INTRA    = 1,
        VC1_ENC_MID_RATE_INTRA      = 2,
        VC1_ENC_HIGH_RATE_INTRA     = 3,
        VC1_ENC_HIGH_MOTION_INTER   = 4,
        VC1_ENC_LOW_MOTION_INTER    = 5,
        VC1_ENC_MID_RATE_INTER      = 6,
        VC1_ENC_HIGH_RATE_INTER     = 7,
    }eCodingSet;

    typedef struct
    {
        const Ipp8u * pTableDL;
        const Ipp8u * pTableDR;
        const Ipp8u * pTableInd;
        const Ipp8u * pTableDLLast;
        const Ipp8u * pTableDRLast;
        const Ipp8u * pTableIndLast;
        const Ipp32u* pEncTable;
    } sACTablesSet;

    typedef enum
    {
        VC1_ENC_8x8_TRANSFORM = 0,
        VC1_ENC_8x4_TRANSFORM = 1,
        VC1_ENC_4x8_TRANSFORM = 2,
        VC1_ENC_4x4_TRANSFORM = 3,
    }eTransformType;
}

#endif
