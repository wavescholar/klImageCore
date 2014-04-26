/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2003-2012 Intel Corporation. All Rights Reserved.
//
//     Intel Integrated Performance Primitives AAC Encode Sample for Windows*
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel IPP
//  product installation for more information.
//
//  MPEG-4 and AAC are international standards promoted by ISO, IEC, ITU, ETSI
//  and other organizations. Implementations of these standards, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.
//
*/

#ifndef __LTPENCODE_H
#define __LTPENCODE_H

#include "aac_filterbank_fp.h"
#include "aac_enc_own.h"

#include "ipps.h"

#ifdef  __cplusplus
extern "C" {
#endif

void ltpEncode(Ipp32f *inBuf,
               Ipp32f *ltpBuf,
               Ipp32f *predictedBuf,
               Ipp32s *ltpDelay,
               Ipp32s *ltpInd,
               IppsFFTSpec_R_32f* corrFft,
               Ipp8u* corrBuff);

void ltpBufferUpdate(Ipp32f **ltpBuffer,
                     Ipp32f **prevSamples,
                     Ipp32f **predictedSpectrum,
                     sEnc_channel_pair_element *pElement,
                     sFilterbank *filterbank,
                     Ipp32s *sfb_offset_short,
                     Ipp32s *prevWindowShape,
                     Ipp32s numCh);

#ifdef  __cplusplus
}
#endif

#endif//__LTPENCODE_H
