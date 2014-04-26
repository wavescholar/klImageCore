/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright (c) 2011-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __ALS_DEC_H__
#define __ALS_DEC_H__

#include "aac_status.h"
#include "bstream.h"
#include "audio_codec_params.h"
#include "mp4cmn_config.h"

#define NUM_MCC_LAYERS 10
#define ALS_MLZ_DICT_SIZE 32769

typedef struct {
  Ipp16s letter;
  Ipp16s code;
  Ipp32s pos;
} alsMLZDict;

typedef struct {
  Ipp32s codeBits;
  Ipp32s freezeFlag;
  Ipp32s bumpCode;
  Ipp32s nextCode;
  alsMLZDict MLZDict[ALS_MLZ_DICT_SIZE];
  Ipp32s *tmpBuf;
  Ipp32s *acfMantissa;
  Ipp32s *shiftValue;
  Ipp32s *diffMantissa;
  Ipp32s *nBits;
  Ipp32s *alignedLen;
  Ipp32f *floatX;
} alsMLZState;

typedef struct {
  Ipp32s  WeightingFactor[NUM_MCC_LAYERS][6];
  Ipp32s  masterChannelIndex[NUM_MCC_LAYERS];
  Ipp32s  timeDifferenceFlag[NUM_MCC_LAYERS];
  Ipp32s  timeDifferenceIndex[NUM_MCC_LAYERS];
  Ipp32s  stopFlag[NUM_MCC_LAYERS];
} alsMCCState;

typedef struct {
  Ipp32s  filterWeight[1024];
  Ipp32s  buf[1024];
} alsLMSState;

typedef struct {
  alsLMSState LMSFilter[8];
  Ipp64s P[900];
  Ipp32s RLSFilterWeight[30];
  Ipp32s DPCMBuf[1];
  Ipp32s RLSBuf[30];
  Ipp32s wFinal[10];
} alsRLSLMSState;

typedef struct {
  Ipp32s *res;
  Ipp32s *x;
  Ipp32s *xSaved;
  Ipp32s *parcor;
  Ipp32s *lpc;
  Ipp32s raBlock;
  Ipp32s blockType;
  Ipp32s constBlock;
  Ipp32s jsBlock;
  Ipp32s constVal;
  Ipp32s subBlocks;
  Ipp32s blockLength;
  Ipp32s sbLength;
  Ipp32s shiftPos;
  Ipp32s optOrder;
  Ipp32s LTPenable;
  Ipp32s LTPgain[5];
  Ipp32s LTPlag;
  Ipp32s s[16];
  Ipp32s sx[16];
} alsBlockState;

typedef struct {
  Ipp32s LMSFilterMu[8];
  Ipp32s LMSFilterOrder[8];
  Ipp32s LMSStage;
  Ipp32s LMSStepSize;
  Ipp32s RLSFilterOrder;
  Ipp32s RLSLambda;
  Ipp32s RLSLambdaRA;
  Ipp32s RLSExtensionBits;
  Ipp32s RLSMonoBlock;
  Ipp32s lastRLSMonoBlock;
  Ipp32u sampFreq;
  Ipp32u numSamples;
  Ipp32u headerSize;
  Ipp32u trailerSize;
  Ipp32s timeNeededLen;
  Ipp32s lagNeededLen;
  Ipp32s numChannels;
  Ipp32s chNeededLen;
  Ipp32s fileType;
  Ipp32s resolution;
  Ipp32s floating;
  Ipp32s msbFirst;
  Ipp32s frameLength;
  Ipp32s ra;
  Ipp32s raFlag;
  Ipp32s raUnitSize;
  Ipp32s raUnitNum;
  Ipp32s raTotNumUnits;
  Ipp32s adaptOrder;
  Ipp32s coefTable;
  Ipp32s LongTermPrediction;
  Ipp32s maxOrder;
  Ipp32s blockSwitching;
  Ipp32s bgmcMode;
  Ipp32s sbPart;
  Ipp32s jointStereo;
  Ipp32s mcCoding;
  Ipp32s chanConfig;
  Ipp32s chanSort;
  Ipp32s crcEnabled;
  Ipp32s RLSLMS;
  Ipp32s auxDataEnabled;
  Ipp32s chanConfigInfo;
  Ipp32s numFrames;
  Ipp32s lastFrameLength;
  Ipp32s frameId;
  Ipp32s jsSwitch;
  Ipp32s isMCC;
  Ipp32s raFrame;
  Ipp32u crc;
  Ipp32u curCrc;
  Ipp8u  *parcorRiceParam;
  Ipp32s *parcorOffset;
  Ipp32u *raUnitSizeBuf;
  Ipp16u *chanPos;
  Ipp8u  *origHeader;
  Ipp8u  *origTrailer;
  Ipp32s *MCCreconstructed;
  Ipp32s **xDiff;
  Ipp32s **x;
  Ipp32s **orderedSamples;
  alsRLSLMSState **rlState;
  alsMCCState **mccState;
  alsBlockState **blockState;
  alsMLZState *MLZState;
} ALSDec;

#ifdef __cplusplus
extern "C" {
#endif

  void decALSGetDataFromSpecificConfig(ALSDec *state,
                                       sBitsreamBuffer *pBS);

  AACStatus alsdecInit(sALS_specific_config *inState,
                       ALSDec *state,
                       Ipp8u  *mem,
                       Ipp32s *size);

  AACStatus alsdecUpdateState(ALSDec *state,
                              Ipp32s mShift);

  AACStatus alsdecGetFrame(Ipp8u    *inPointer,
                           Ipp32s   *decodedBytes,
                           Ipp8u    *outPointer,
                           Ipp32s   *outFrameLen,
                           Ipp32s   inDataSize,
                           Ipp32s   outBufferSize,
                           Ipp32s   useHeaderTraler,
                           ALSDec   *state);

  AACStatus alsdecGetFrameSize(Ipp32s *frameSize,
                               ALSDec *state);

  AACStatus alsdecGetSampleFrequency(Ipp32s *freq,
                                     ALSDec *state);

  AACStatus alsdecGetChannels(Ipp32s *ch,
                              ALSDec *state);

  AACStatus alsdecGetBitsPerSample(Ipp32s *bits,
                                   ALSDec *state);

#ifdef __cplusplus
}
#endif

#endif // __ALS_DEC_H__
