/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __ALS_DEC_OWN_H__
#define __ALS_DEC_OWN_H__

#include "aac_status.h"

#ifdef __cplusplus
extern "C" {
#endif
void alsgetBlocks(Ipp32s bsInfo,
                  Ipp32s len,
                  Ipp32s *blockLen,
                  Ipp32s *numBlocks);

Ipp32s alsdecParseBlockData(sBitsreamBuffer *pBS,
                            ALSDec        *state,
                            alsBlockState   *blockState);

void alsdecParseChannelData(sBitsreamBuffer *pBS,
                            ALSDec          *state,
                            Ipp32s ch,
                            Ipp32s *pLayer);

void alsReconstructData(ALSDec      *state,
                        alsBlockState *blockState);

Ipp32s alsRLSSynthesize(ALSDec         *state,
                        alsBlockState  *blockState,
                        alsRLSLMSState *rlState);

Ipp32s alsRLSSynthesizeJoint(ALSDec         *state,
                             alsBlockState  **blockState,
                             alsRLSLMSState **rlState);

void aasdecDiffMantissa(sBitsreamBuffer *pBS,
                        ALSDec          *state);

void alsMLZflushDict(alsMLZState *MLZState);


#ifdef __cplusplus
}
#endif

#endif // __ALS_DEC_OWN_H__
