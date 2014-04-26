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

#ifndef __QUANTIZATION_H
#define __QUANTIZATION_H

#include "aac_enc_psychoacoustic_fp.h"
#include "aac_enc_own.h"

#include "ipps.h"

#define  MAX_QUANT     8191
#define  MAGIC_NUMBER  0.4054

#ifndef  SIGN
#define  SIGN(x) ((x) > 0 ? (1): (-1))
#endif

typedef struct {
  Ipp32f* mdct_line;
  Ipp32f* noiseThr;
  Ipp32f* minSNR;
  Ipp32f* bitsToPeCoeff;
  Ipp32f* energy;
  Ipp32f* logEnergy;
  Ipp32f* sfbPE;
  Ipp32f* scalefactorDataBits;
  Ipp32f* PEtoNeededPECoeff;
  Ipp32f  outPe;
  Ipp32s  predAttackWindow;
  Ipp32s  allowHolesSfb;
} sQuantizationData;

#ifdef  __cplusplus
extern "C" {
#endif

  void Quantization(sQuantizationBlock* pBlock,
                    sEnc_individual_channel_stream* pStream,
                    sQuantizationData* qData);

#ifdef  __cplusplus
}
#endif

#endif//__QUANTIZATION_H
