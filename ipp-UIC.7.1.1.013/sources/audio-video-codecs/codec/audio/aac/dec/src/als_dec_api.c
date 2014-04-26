/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives ALS Decode Sample for Windows*
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
//  MPEG-4 and ALC are international standards promoted by ISO, IEC, ITU, ETSI
//  and other organizations. Implementations of these standards, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

#include "als_dec.h"
#include "als_dec_own.h"
#include "als_dec_tabs.h"
#include "bstream.h"
#include "align.h"
#include "aac_dec_own_fp.h"
#include <stdlib.h>
#include <stdio.h>

#include "ippdc.h"

/********************************************************************/

void decALSGetDataFromSpecificConfig(ALSDec *state,
                                     sBitsreamBuffer *pBS)
{
  Ipp32s i, tmp, ch;
  Ipp32s size0, size1;
  Ipp8u  *ptr;

  if (state->chanSort) {
    for (ch = 0; ch < state->numChannels; ch++) {
      GET_BITS(pBS, state->chanPos[ch], state->chNeededLen, Ipp16u)
    }
  }
  Byte_alignment(pBS);

  GET_BITS(pBS, tmp, 32, Ipp32s)
  GET_BITS(pBS, tmp, 32, Ipp32s)

  if (state->headerSize > 0x7fffffff) {
    size0 = 0x7fffffff;
    size1 = (Ipp32s)(state->headerSize - 0x7fffffff);
  } else {
    size0 = (Ipp32s)state->headerSize;
    size1 = 0;
  }

  ptr = state->origHeader;

  for (i = 0; i < size0; i++) {
    GET_BITS(pBS, ptr[i], 8, Ipp8u)
  }

  ptr += size0;

  for (i = 0; i < size1; i++) {
    GET_BITS(pBS, ptr[i], 8, Ipp8u)
  }

  if (state->trailerSize > 0x7fffffff) {
    size0 = 0x7fffffff;
    size1 = (Ipp32s)(state->trailerSize - 0x7fffffff);
  } else {
    size0 = (Ipp32s)state->trailerSize;
    size1 = 0;
  }

  ptr = state->origTrailer;

  for (i = 0; i < size0; i++) {
    GET_BITS(pBS, ptr[i], 8, Ipp8u)
  }

  ptr += size0;

  for (i = 0; i < size1; i++) {
    GET_BITS(pBS, ptr[i], 8, Ipp8u)
  }

  if (state->crcEnabled) {
    GET_BITS(pBS, tmp, 32, Ipp32u)
  }

  if ((state->raFlag == 2) && (state->ra > 0)) {
    for (i = 0; i < state->raTotNumUnits; i++) {
      GET_BITS(pBS, state->raUnitSizeBuf[i], 32, Ipp32u)
    }
  }
}

/********************************************************************/

AACStatus alsdecInit(sALS_specific_config *inState,
                     ALSDec *state,
                     Ipp8u  *mem,
                     Ipp32s *size_all)
{
  Ipp8u *ptr;
  Ipp64s sizeMLZState, sizeMLZTmpBuf, sizeMLZDiffMantissa;
  Ipp64s sizeMLZNBits, sizeMLZSlignedLen, sizeMLZFloatX;
  Ipp64s sizeMLZAcfMantissa, sizeMLZShiftValue;
  Ipp64s sizeRaUnitSizeBuf, sizeChanPos, sizeOrigHeader;
  Ipp64s sizeOrigTrailer, sizeXDiffPtr, sizeXDiff, sizeXPtr;
  Ipp64s sizeX, sizeOrderedSamples, sizeRlStatePtr, sizeRlState;
  Ipp64s sizeBlockStatePtr, sizeBlockState, sizeMCCStatePtr, sizeMCCState;
  Ipp64s sizeRes, sizeResLTP, sizeXSaved, sizeParcor, sizeLpc, size;
  Ipp32s numBlockStates;
  Ipp32s i, lagNeededLen, maxOrder;

  if (!inState || !size_all)
    return AAC_NULL_PTR;

  sizeRaUnitSizeBuf = __ALIGNED(inState->raTotNumUnits * sizeof(Ipp32u));

  if (inState->chanSort) {
    sizeChanPos = __ALIGNED(inState->numChannels * sizeof(Ipp16u));
  } else {
    sizeChanPos = 0;
  }

  sizeOrigHeader =__ALIGNED((Ipp64s)inState->headerSize);
  sizeOrigTrailer =__ALIGNED((Ipp64s)inState->trailerSize);

  maxOrder = inState->maxOrder;

  if (inState->RLSLMS)
    maxOrder = 10;

  if (inState->jointStereo) {
    sizeXDiffPtr = __ALIGNED((inState->numChannels >> 1) * sizeof(Ipp32s*));
    sizeXDiff = (inState->numChannels >> 1) *
                 __ALIGNED((inState->frameLength + maxOrder) * sizeof(Ipp32s));
  } else {
    sizeXDiffPtr = sizeXDiff = 0;
  }

  sizeXPtr = __ALIGNED(inState->numChannels * sizeof(Ipp32s*));
  sizeX = inState->numChannels *
          __ALIGNED((inState->frameLength + maxOrder) * sizeof(Ipp32s));
  sizeOrderedSamples = sizeXPtr;

  sizeRlStatePtr = __ALIGNED(inState->numChannels * sizeof(alsRLSLMSState*));

  if (inState->RLSLMS) {
    sizeRlState = inState->numChannels * __ALIGNED(sizeof(alsRLSLMSState));
  } else {
    sizeRlState = 0;
  }

  sizeBlockState = __ALIGNED(sizeof(alsBlockState));

  sizeRes = __ALIGNED(inState->frameLength * sizeof(Ipp32s));

  lagNeededLen = 8;
  if (inState->sampFreq >= 192000)
    lagNeededLen = 10;
  else if (inState->sampFreq >= 96000)
    lagNeededLen = 9;

  if (inState->LongTermPrediction) {
    Ipp32s tmp = (1 << lagNeededLen) + maxOrder + 4;
    sizeResLTP = __ALIGNED(tmp * sizeof(Ipp32s));
  } else {
    sizeResLTP = 0;
  }

  sizeXSaved = __ALIGNED(maxOrder * sizeof(Ipp32s));
  sizeParcor = sizeLpc = sizeXSaved;

  if (inState->mcCoding) {
    numBlockStates = inState->numChannels;
    sizeMCCStatePtr = __ALIGNED(inState->numChannels * sizeof(alsMCCState*));
    sizeMCCState = inState->numChannels * __ALIGNED(sizeof(alsMCCState));
  } else {
    numBlockStates = 2;
    sizeMCCStatePtr = 0;
    sizeMCCState = 0;
  }

  sizeBlockStatePtr = __ALIGNED(numBlockStates * sizeof(alsBlockState*));

  size = sizeRaUnitSizeBuf + sizeChanPos + sizeOrigHeader + sizeOrigTrailer +
         sizeXDiffPtr + sizeXDiff + sizeXPtr + sizeX + sizeOrderedSamples + sizeRlStatePtr +
         sizeRlState + sizeBlockStatePtr + sizeMCCStatePtr + sizeMCCState +
         numBlockStates * (sizeBlockState + sizeRes + sizeResLTP + sizeXSaved + sizeParcor + sizeLpc);

  if (inState->floating) {
    sizeMLZState = __ALIGNED(sizeof(alsMLZState));
    sizeMLZTmpBuf = __ALIGNED(inState->frameLength * sizeof(Ipp32s));
    sizeMLZDiffMantissa = __ALIGNED(inState->frameLength * sizeof(Ipp32s));
    sizeMLZNBits = __ALIGNED(inState->frameLength * sizeof(Ipp32s));
    sizeMLZSlignedLen = __ALIGNED(inState->frameLength * sizeof(Ipp32s));
    sizeMLZFloatX = __ALIGNED(inState->frameLength * sizeof(Ipp32f));
    sizeMLZAcfMantissa = __ALIGNED(inState->numChannels * sizeof(Ipp32s));
    sizeMLZShiftValue = __ALIGNED(inState->numChannels * sizeof(Ipp32s));

    size += sizeMLZState + sizeMLZTmpBuf + sizeMLZDiffMantissa +
            sizeMLZNBits + sizeMLZSlignedLen + sizeMLZFloatX +
            sizeMLZAcfMantissa + sizeMLZShiftValue;
  } else {
    /* warnings */
    sizeMLZState = 0;
    sizeMLZTmpBuf = 0;
    sizeMLZDiffMantissa = 0;
    sizeMLZNBits = 0;
    sizeMLZSlignedLen = 0;
    sizeMLZFloatX = 0;
    sizeMLZAcfMantissa = 0;
    sizeMLZShiftValue = 0;
  }

  if (mem != NULL) {

    if (!state)
      return AAC_NULL_PTR;

    ptr = mem;

    state->raUnitSizeBuf = (Ipp32u*)ptr;
    ptr += sizeRaUnitSizeBuf;

    if (inState->chanSort) {
      state->chanPos = (Ipp16u*)ptr;
      ptr += sizeChanPos;
    }

    state->origHeader = (Ipp8u*)ptr;
    ptr += sizeOrigHeader;

    state->origTrailer = (Ipp8u*)ptr;
    ptr += sizeOrigTrailer;

    if (inState->jointStereo) {
      state->xDiff = (Ipp32s**)ptr;
      ptr += sizeXDiffPtr;

      size = __ALIGNED((inState->frameLength + maxOrder) * sizeof(Ipp32s));

      for (i = 0; i < inState->numChannels >> 1; i++) {
        state->xDiff[i] = (Ipp32s*)ptr;
        ippsZero_32s(state->xDiff[i], maxOrder);
        state->xDiff[i] += maxOrder;
        ptr += size;
      }
    }

    state->x = (Ipp32s**)ptr;
    ptr += sizeXPtr;

    size = __ALIGNED((inState->frameLength + maxOrder) * sizeof(Ipp32s));

    for (i = 0; i < inState->numChannels; i++) {
      state->x[i] = (Ipp32s*)ptr;
      ippsZero_32s(state->x[i], maxOrder);
      state->x[i] += maxOrder;
      ptr += size;
    }

    state->orderedSamples = (Ipp32s**)ptr;
    ptr += sizeOrderedSamples;

    state->rlState = (alsRLSLMSState**)ptr;
    ptr += sizeRlStatePtr;

    if (inState->RLSLMS) {
      size = __ALIGNED(sizeof(alsRLSLMSState));

      for (i = 0; i < inState->numChannels; i++) {
        state->rlState[i] = (alsRLSLMSState*)ptr;
        ippsZero_8u((Ipp8u*)state->rlState[i], (Ipp32s)(size));
        ptr += size;
      }
    } else {
      for (i = 0; i < inState->numChannels; i++) {
        state->rlState[i] = NULL;
      }
    }

    state->blockState = (alsBlockState**)ptr;
    ptr += sizeBlockStatePtr;

    for (i = 0; i < numBlockStates; i++) {
      size = __ALIGNED(sizeof(alsBlockState));

      state->blockState[i] = (alsBlockState*)ptr;
      ptr += size;

      state->blockState[i]->res = (Ipp32s*)ptr;
      ippsZero_8u((Ipp8u*)state->blockState[i]->res, (Ipp32s)sizeResLTP);
      state->blockState[i]->res += sizeResLTP/sizeof(Ipp32s);
      ptr += (sizeRes + sizeResLTP);

      state->blockState[i]->xSaved = (Ipp32s*)ptr;
      ptr += sizeXSaved;

      state->blockState[i]->parcor = (Ipp32s*)ptr;
      ptr += sizeParcor;

      state->blockState[i]->lpc = (Ipp32s*)ptr;
      ptr += sizeLpc;
    }

    if (inState->mcCoding) {
      state->mccState = (alsMCCState**)ptr;
      ptr += sizeMCCStatePtr;

      size = __ALIGNED(sizeof(alsMCCState));

      for (i = 0; i < inState->numChannels; i++) {
        state->mccState[i] = (alsMCCState*)ptr;
        ptr += size;
      }
    }

    if (inState->floating) {
      state->MLZState = (alsMLZState*)ptr;
      ptr += sizeMLZState;

      state->MLZState->tmpBuf = (Ipp32s*)ptr;
      ptr += sizeMLZTmpBuf;

      state->MLZState->diffMantissa = (Ipp32s*)ptr;
      ptr += sizeMLZDiffMantissa;

      state->MLZState->nBits = (Ipp32s*)ptr;
      ptr += sizeMLZNBits;

      state->MLZState->alignedLen = (Ipp32s*)ptr;
      ptr += sizeMLZSlignedLen;

      state->MLZState->floatX = (Ipp32f*)ptr;
      ptr += sizeMLZFloatX;

      state->MLZState->acfMantissa = (Ipp32s*)ptr;
      ptr += sizeMLZAcfMantissa;

      state->MLZState->shiftValue = (Ipp32s*)ptr;
      ptr += sizeMLZShiftValue;

      for (i = 0; i < 256; i++) {
        state->MLZState->MLZDict[i].pos = 0;
      }

      for (i = 0; i < inState->numChannels; i++) {
        state->MLZState->acfMantissa[i] = 0;
        state->MLZState->shiftValue[i] = 0;
      }
      alsMLZflushDict(state->MLZState);
    }

    state->sampFreq = inState->sampFreq;
    state->numSamples = inState->numSamples;
    state->headerSize = inState->headerSize;
    state->trailerSize = inState->trailerSize;
    state->numChannels = inState->numChannels;
    state->chNeededLen = inState->chNeededLen;
    state->fileType = inState->fileType;
    state->resolution = inState->resolution;
    state->floating = inState->floating;
    state->msbFirst = inState->msbFirst;
    state->frameLength = inState->frameLength;
    state->ra = inState->ra;
    state->raFlag = inState->raFlag;
    state->raTotNumUnits = inState->raTotNumUnits;
    state->adaptOrder = inState->adaptOrder;
    state->coefTable = inState->coefTable;
    state->LongTermPrediction = inState->LongTermPrediction;
    state->maxOrder = maxOrder;
    state->blockSwitching = inState->blockSwitching;
    state->bgmcMode = inState->bgmcMode;
    state->sbPart = inState->sbPart;
    state->jointStereo = inState->jointStereo;
    state->mcCoding = inState->mcCoding;
    state->chanConfig = inState->chanConfig;
    state->chanSort = inState->chanSort;
    state->crcEnabled = inState->crcEnabled;
    state->RLSLMS = inState->RLSLMS;
    state->auxDataEnabled = inState->auxDataEnabled;
    state->chanConfigInfo = inState->chanConfigInfo;
    state->numFrames = inState->numFrames;
    state->lastFrameLength = inState->lastFrameLength;
    state->crc = inState->crc;

    state->raUnitNum = 0;
    state->curCrc  = 0;

    state->timeNeededLen = 5;
    if (state->sampFreq > 96000)
      state->timeNeededLen = 7;
    else if (state->sampFreq > 48000)
      state->timeNeededLen = 6;

    state->lagNeededLen = lagNeededLen;

    if (state->coefTable != 3) {
      state->parcorRiceParam =
        alsParcorRiceParamTable[state->coefTable];

      state->parcorOffset =
        alsParcorOffsetTable[state->coefTable];
    }

    state->frameId = 0;

    if (state->numChannels == 1) {
      state->jointStereo = 0;
    }

    if (state->chanSort) {
      for (i = 0; i < inState->numChannels; i++) {
        state->orderedSamples[i] = state->x[state->chanPos[i]];
      }
    } else {
      for (i = 0; i < inState->numChannels; i++) {
        state->orderedSamples[i] = state->x[i];
      }
    }
  }

  *size_all = (Ipp32s)size;

  return AAC_OK;
}

/********************************************************************/

AACStatus alsdecUpdateState(ALSDec *state,
                            Ipp32s mShift)
{
  Ipp32s i, numBlockStates;

  AACDEC_UPDATE_PTR(Ipp32u, state->raUnitSizeBuf, mShift)

  if (state->chanSort) {
    AACDEC_UPDATE_PTR(Ipp16u, state->chanPos, mShift)
  }

  AACDEC_UPDATE_PTR(Ipp8u, state->origHeader, mShift)
  AACDEC_UPDATE_PTR(Ipp8u, state->origTrailer, mShift)

  if (state->jointStereo) {
    AACDEC_UPDATE_PPTR(Ipp32s, state->xDiff, mShift)

    for (i = 0; i < state->numChannels >> 1; i++) {
      AACDEC_UPDATE_PTR(Ipp32s, state->xDiff[i], mShift)
    }
  }

  AACDEC_UPDATE_PPTR(Ipp32s, state->x, mShift)

  for (i = 0; i < state->numChannels; i++) {
    AACDEC_UPDATE_PTR(Ipp32s, state->x[i], mShift)
  }

  AACDEC_UPDATE_PPTR(Ipp32s, state->orderedSamples, mShift)

  for (i = 0; i < state->numChannels; i++) {
    AACDEC_UPDATE_PTR(Ipp32s, state->orderedSamples[i], mShift)
  }

  AACDEC_UPDATE_PPTR(alsRLSLMSState, state->rlState, mShift)

  if (state->RLSLMS) {
    for (i = 0; i < state->numChannels; i++) {
      AACDEC_UPDATE_PTR(alsRLSLMSState, state->rlState[i], mShift)
    }
  }

  if (state->mcCoding) {
    numBlockStates = state->numChannels;

    AACDEC_UPDATE_PPTR(alsMCCState, state->mccState, mShift)

    for (i = 0; i < state->numChannels; i++) {
      AACDEC_UPDATE_PTR(alsMCCState, state->mccState[i], mShift)
    }
  } else {
    numBlockStates = 2;
  }

  AACDEC_UPDATE_PPTR(alsBlockState, state->blockState, mShift)

  for (i = 0; i < numBlockStates; i++) {
    AACDEC_UPDATE_PTR(alsBlockState, state->blockState[i], mShift)
    AACDEC_UPDATE_PTR(Ipp32s, state->blockState[i]->res, mShift)
    AACDEC_UPDATE_PTR(Ipp32s, state->blockState[i]->xSaved, mShift)
    AACDEC_UPDATE_PTR(Ipp32s, state->blockState[i]->parcor, mShift)
    AACDEC_UPDATE_PTR(Ipp32s, state->blockState[i]->lpc, mShift)
  }

  if (state->floating) {
    AACDEC_UPDATE_PTR(alsMLZState, state->MLZState, mShift)
    AACDEC_UPDATE_PTR(Ipp32s, state->MLZState->tmpBuf, mShift)
    AACDEC_UPDATE_PTR(Ipp32s, state->MLZState->diffMantissa, mShift)
    AACDEC_UPDATE_PTR(Ipp32s, state->MLZState->nBits, mShift)
    AACDEC_UPDATE_PTR(Ipp32s, state->MLZState->alignedLen, mShift)
    AACDEC_UPDATE_PTR(Ipp32f, state->MLZState->floatX, mShift)
    AACDEC_UPDATE_PTR(Ipp32s, state->MLZState->acfMantissa, mShift)
    AACDEC_UPDATE_PTR(Ipp32s, state->MLZState->shiftValue, mShift)
  }

  return AAC_OK;
}

/********************************************************************/

#define RETURN_AAC_BAD_STREAM                                     \
{                                                                 \
  GET_BITS_COUNT(pBS, (*decodedBytes))                            \
  *decodedBytes = (*decodedBytes + 7) >> 3;                       \
  return AAC_BAD_STREAM;                                          \
}

/********************************************************************/

AACStatus alsdecGetFrame(Ipp8u    *inPointer,
                         Ipp32s   *decodedBytes,
                         Ipp8u    *outPointer,
                         Ipp32s   *outFrameLen,
                         Ipp32s   inDataSize,
                         Ipp32s   outBufferSize,
                         Ipp32s   useHeaderTraler,
                         ALSDec   *state)
{
  sBitsreamBuffer BS;
  sBitsreamBuffer *pBS = &BS;

  Ipp32s blockLen[32];
  Ipp32s bsInfo, independentBS, numBlocks, raFrame;
  Ipp32s ch;
  Ipp32s neededSize, resolution, num;
  Ipp32s start, i, j;

  if (!inPointer || !outPointer)
    return AAC_NULL_PTR;

  if (state->frameId != state->numFrames - 1) {
    neededSize = state->numChannels * state->frameLength*(state->resolution >> 3);
  } else {
    neededSize = state->numChannels * state->lastFrameLength*(state->resolution >> 3);
  }

  if (useHeaderTraler) {
    if (state->frameId == 0) {
      neededSize += state->headerSize;
    }

    if (state->frameId == state->numFrames - 1) {
      neededSize += state->trailerSize;
    }
  }

  if (outBufferSize < neededSize) {
    return AAC_NOT_ENOUGH_BUFFER;
  }

  *outFrameLen = 0;

  if (useHeaderTraler) {
    if (state->frameId == 0) {
      ippsCopy_8u(state->origHeader, outPointer, state->headerSize);
      *outFrameLen += state->headerSize;
      outPointer += state->headerSize;
    }
  }

  GET_INIT_BITSTREAM(pBS, inPointer)
  pBS->nDataLen = inDataSize;

  raFrame = 0;
  if (state->ra != 0) {
    if ((state->frameId % state->ra) == 0) {
      raFrame = 1;
      if (state->raFlag == 1) {
        GET_BITS(pBS, state->raUnitSize, 32, Ipp32u)
      } else if (state->raFlag == 2) {
        state->raUnitSize = state->raUnitSizeBuf[state->raUnitNum];
        state->raUnitNum++;
      }
    }
  }

  state->raFrame = raFrame;

  state->jsSwitch = 0;
  if (state->mcCoding && state->jointStereo) {
    GET_BITS(pBS, state->jsSwitch, 1, Ipp32s)
    Byte_alignment(pBS);
  }

  state->frameId++;

  if (!state->mcCoding || state->jsSwitch) {
    state->isMCC = 0;
    for (ch = 0; ch < state->numChannels; ch++) {

      independentBS = 0;
      if ((state->jointStereo == 0) ||
          (ch == state->numChannels - 1) ||
          ((ch & 1) == 1)) {
        independentBS = 1;
      }

      if (!state->RLSLMS) {
        if (state->blockSwitching) {
          Ipp32s size = state->blockSwitching << 3;

          if (state->blockSwitching == 3) {
            size = 32;
          }
          GET_BITS(pBS, bsInfo, size, Ipp32u)

          if (size < 32) {
            bsInfo <<= (32 - size);
          }

          if (bsInfo & 0x80000000) {
            independentBS = 1;
          }

          alsgetBlocks(bsInfo, state->frameLength,
                      blockLen, &numBlocks);
        } else {
          numBlocks = 1;
          blockLen[0] = state->frameLength;
        }
      } else {
        numBlocks = 1;
        blockLen[0] = state->frameLength;
      }

      /* for the last frame num and sizes of blocks should be */
      /* recalculated because last frame can be shorter       */
      if ((state->frameId == state->numFrames) &&
          (state->lastFrameLength != state->frameLength)) {
        Ipp32s num = 0;

        numBlocks = 0;
        while (num < state->lastFrameLength) {
          num += blockLen[numBlocks];
          numBlocks++;
        }
        blockLen[numBlocks-1] -= (num - state->lastFrameLength);
      }

      start = 0;
      if (independentBS) {
        raFrame = state->raFrame;
        for (i = 0; i < numBlocks; i++) {

          state->blockState[0]->blockLength = blockLen[i];
          state->blockState[0]->raBlock = raFrame;
          state->blockState[0]->x = state->x[ch] + start;

          if (alsdecParseBlockData(pBS, state, state->blockState[0]) < 0) {
            RETURN_AAC_BAD_STREAM
          }

          alsReconstructData(state, state->blockState[0]);

          if (state->RLSLMS) {
            if (alsRLSSynthesize(state, state->blockState[0],
                                 state->rlState[ch]) < 0) {
              RETURN_AAC_BAD_STREAM;
            }
          }

          raFrame = 0;
          start += blockLen[i];
        }

        if ((state->jointStereo == 1) && ((ch & 1) == 1)) {
          ippsSub_32s_Sfs(state->x[ch-1], state->x[ch],
                          state->xDiff[ch>>1], state->frameLength, 0);
        }
      } else {
        raFrame = state->raFrame;
        for (i = 0; i < numBlocks; i++) {
          state->blockState[0]->blockLength = blockLen[i];
          state->blockState[1]->blockLength = blockLen[i];
          state->blockState[0]->raBlock = raFrame;
          state->blockState[1]->raBlock = raFrame;

          if (alsdecParseBlockData(pBS, state, state->blockState[0]) < 0) {
            RETURN_AAC_BAD_STREAM
          }

          if (alsdecParseBlockData(pBS, state, state->blockState[1]) < 0) {
            RETURN_AAC_BAD_STREAM
          }

          if (!state->RLSLMS) {
            if (state->blockState[0]->jsBlock == 1) {
              state->blockState[0]->x = state->xDiff[ch>>1]  + start;
              state->blockState[1]->x = state->x[ch+1]  + start;
            } else {
              state->blockState[0]->x = state->x[ch] + start;
              if (state->blockState[1]->jsBlock == 1) {
                state->blockState[1]->x = state->xDiff[ch>>1] + start;
              } else {
                state->blockState[1]->x = state->x[ch+1] + start;
              }
            }
          } else {
            state->blockState[0]->x = state->x[ch] + start;
            state->blockState[1]->x = state->x[ch+1] + start;
          }

          alsReconstructData(state, state->blockState[0]);
          alsReconstructData(state, state->blockState[1]);

          if (!state->RLSLMS) {
            if (state->blockState[0]->jsBlock == 1) {
              ippsSub_32s_Sfs(state->xDiff[ch>>1] + start, state->x[ch+1] + start,
                              state->x[ch] + start, blockLen[i], 0);
            } else {
              if (state->blockState[1]->jsBlock == 1) {
                ippsAdd_32s_Sfs(state->xDiff[ch>>1] + start, state->x[ch] + start,
                                state->x[ch+1] + start, blockLen[i], 0);
              } else {
                ippsSub_32s_Sfs(state->x[ch] + start, state->x[ch+1] + start,
                                state->xDiff[ch>>1] + start, blockLen[i], 0);
              }
            }
          } else {
            alsRLSLMSState *tmpRlState[2];

            tmpRlState[0] = state->rlState[ch];
            tmpRlState[1] = state->rlState[ch+1];

            if (alsRLSSynthesizeJoint(state, state->blockState, tmpRlState) < 0) {
              RETURN_AAC_BAD_STREAM
            }
          }

          raFrame = 0;
          start += blockLen[i];
        }
        ch++;
      }
    }
  } else {
    state->isMCC = 1;
    if (state->blockSwitching) {
      Ipp32s size = state->blockSwitching << 3;

      if (state->blockSwitching == 3) {
        size = 32;
      }
      GET_BITS(pBS, bsInfo, size, Ipp32u)

      alsgetBlocks(bsInfo, state->frameLength,
                   blockLen, &numBlocks);

    } else {
      numBlocks = 1;
      blockLen[0] = state->frameLength;
    }

    /* for the last frame num and sizes of blocks should be */
    /* recalculated because last frame can be shorter       */
    if ((state->frameId == state->numFrames) &&
        (state->lastFrameLength != state->frameLength)) {
      Ipp32s num = 0;

      numBlocks = 0;
      while (num < state->lastFrameLength) {
        num += blockLen[numBlocks];
        numBlocks++;
      }
      blockLen[numBlocks-1] -= (num - state->lastFrameLength);
    }

    start = 0;

    for (i = 0; i < numBlocks; i++) {
      Ipp32s maxLayer, layer;

      maxLayer = -1;
      for (ch = 0; ch < state->numChannels; ch++) {
        for (j = 0; j < NUM_MCC_LAYERS; j++) {
          state->mccState[ch]->stopFlag[j] = 1;
        }
        state->blockState[ch]->blockLength = blockLen[i];
        state->blockState[ch]->raBlock = raFrame;
        state->blockState[ch]->x = state->x[ch] + start;

        if (alsdecParseBlockData(pBS, state, state->blockState[ch]) < 0) {
          RETURN_AAC_BAD_STREAM
        }
        alsdecParseChannelData(pBS, state, ch, &layer);
        if (layer > maxLayer) {
          maxLayer = layer;
        }
      }

      for (ch = 0; ch < state->numChannels; ch++) {
        alsReconstructData(state, state->blockState[ch]);
      }

      if ((state->jointStereo == 1) && ((ch & 1) == 1)) {
        ippsSub_32s_Sfs(state->x[ch-1] + start, state->x[ch] + start,
                        state->xDiff[ch>>1] + start, blockLen[i], 0);
      }

      raFrame = 0;
      start += blockLen[i];
    }
  }

  for (ch = 0; ch < state->numChannels; ch++) {
    ippsCopy_32s(state->x[ch] + state->frameLength - state->maxOrder,
      state->x[ch] - state->maxOrder, state->maxOrder);
  }

  if (state->jointStereo) {
    for (ch = 0; ch < state->numChannels >> 1; ch++) {
      ippsCopy_32s(state->xDiff[ch] + state->frameLength - state->maxOrder,
        state->xDiff[ch] - state->maxOrder, state->maxOrder);
    }
  }

  if (state->floating) {
    aasdecDiffMantissa(pBS, state);
  }

  num = state->frameLength;

  if (state->frameId == state->numFrames) {
    num = state->lastFrameLength;
  }

  resolution = state->resolution;
  if (state->floating) {
    resolution = 32;
  }

  if (resolution == 8) {
    for (ch = 0; ch < state->numChannels; ch++) {
      Ipp8u  *tmpPtr = (Ipp8u*)outPointer + ch;
      Ipp32s *srcPrt = state->orderedSamples[ch];

      for (i = 0; i < num; i++) {
        *tmpPtr = (Ipp8u)(*srcPrt);
        tmpPtr += state->numChannels;
        srcPrt++;
      }
    }
  } else if (resolution == 16) {
    for (ch = 0; ch < state->numChannels; ch++) {
      Ipp16s *tmpPtr = (Ipp16s*)outPointer + ch;
      Ipp32s *srcPrt = state->orderedSamples[ch];

      if (state->msbFirst == 0) {
        for (i = 0; i < num; i++) {
          *tmpPtr = (Ipp16s)(*srcPrt);
          tmpPtr += state->numChannels;
          srcPrt++;
        }
      } else {
        for (i = 0; i < num; i++) {
          Ipp32s tmp = *srcPrt;
          *tmpPtr = (Ipp16s)((tmp << 8) || ((tmp >> 8) & 0xff));
          tmpPtr += state->numChannels;
          srcPrt++;
        }
      }
    }
  } else if (resolution == 24) {
    for (ch = 0; ch < state->numChannels; ch++) {
      Ipp8u  *tmpPtr = (Ipp8u*)outPointer + 3*ch;
      Ipp32s *srcPrt = state->orderedSamples[ch];

      if (state->msbFirst == 0) {
        for (i = 0; i < num; i++) {
          Ipp32s tmp = *srcPrt;
          tmpPtr[0] = (Ipp8u)(tmp & 0xff);
          tmpPtr[1] = (Ipp8u)((tmp >> 8) & 0xff);
          tmpPtr[2] = (Ipp8u)((tmp >> 16) & 0xff);
          tmpPtr += 3 * state->numChannels;
          srcPrt++;
        }
      } else {
        for (i = 0; i < num; i++) {
          Ipp32s tmp = *srcPrt;
          tmpPtr[0] = (Ipp8u)((tmp >> 16) & 0xff);
          tmpPtr[1] = (Ipp8u)((tmp >> 8) & 0xff);
          tmpPtr[2] = (Ipp8u)(tmp & 0xff);
          tmpPtr += 3 * state->numChannels;
          srcPrt++;
        }
      }
    }
  } else if (resolution == 32) {
    for (ch = 0; ch < state->numChannels; ch++) {
      Ipp32s *tmpPtr = (Ipp32s*)outPointer + ch;
      Ipp32s *srcPrt = state->orderedSamples[ch];

      if (state->msbFirst == 0) {
        for (i = 0; i < num; i++) {
          *tmpPtr = *srcPrt;
          tmpPtr += state->numChannels;
          srcPrt++;
        }
      } else {
        for (i = 0; i < num; i++) {
          Ipp32u tmp = (Ipp32u)(*srcPrt);
          *tmpPtr = (Ipp32s)((tmp << 24) + ((tmp & 0xff00) << 8) +
                            ((tmp >> 8) & 0xff00) + (tmp >> 24));
          *tmpPtr = *srcPrt;
          tmpPtr += state->numChannels;
          srcPrt++;
        }
      }
    }
  }

  if (state->crcEnabled) {
    ippsCRC32_8u((Ipp8u *)outPointer,
                  num * state->numChannels * (resolution >> 3),
                 (Ipp32u*)&(state->curCrc));
  }

  outPointer += num * state->numChannels * (resolution >> 3);
  *outFrameLen += num * state->numChannels * (resolution >> 3);

  if (state->frameId == state->numFrames) {
    if (useHeaderTraler) {
      ippsCopy_8u(state->origTrailer, outPointer, state->trailerSize);
      *outFrameLen += state->trailerSize;
    }
  }

  state->RLSExtensionBits = 0;

  GET_BITS_COUNT(pBS, (*decodedBytes))
  *decodedBytes >>= 3;

  return AAC_OK;
}

/********************************************************************/

AACStatus alsdecGetFrameSize(Ipp32s *frameSize,
                             ALSDec *state)
{
  if (!frameSize || !state)
    return AAC_NULL_PTR;

  *frameSize = state->frameLength;
  return AAC_OK;
}

/********************************************************************/

AACStatus alsdecGetSampleFrequency(Ipp32s *freq,
                                   ALSDec *state)
{
  if (!freq || !state)
    return AAC_NULL_PTR;

  *freq = state->sampFreq;
  return AAC_OK;
}

/********************************************************************/

AACStatus alsdecGetChannels(Ipp32s *ch,
                            ALSDec *state)
{
  if (!ch || !state)
    return AAC_NULL_PTR;

  *ch = state->numChannels;
  return AAC_OK;
}

/********************************************************************/

AACStatus alsdecGetBitsPerSample(Ipp32s *bits,
                                 ALSDec *state)
{
  if (!bits || !state)
    return AAC_NULL_PTR;

  *bits = state->resolution;
  return AAC_OK;
}

/********************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_DECODER
