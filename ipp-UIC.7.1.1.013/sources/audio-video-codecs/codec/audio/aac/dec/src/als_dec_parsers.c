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
#include <stdlib.h>
#include <stdio.h>

static void riceDecode(sBitsreamBuffer *pBS,
                       Ipp32s s,
                       Ipp32s *dst,
                       Ipp32s len);

static void ricePosDecode(sBitsreamBuffer *pBS,
                          Ipp32s s,
                          Ipp32s *dst,
                          Ipp32s len);

static Ipp32s bgmcDecode(sBitsreamBuffer *pBS,
                         alsBlockState   *blockState,
                         Ipp32s          start);

/********************************************************************/

static Ipp32s alsLMSPredictorOrderTable[] =
{
    2,    3,    4,    5,    6,    7,    8,    9,
   10,   12,   14,   16,   18,   20,   24,   28,
   32,   36,   48,   64,   80,   96,  128,  256,
  384,  448,  512,  640,  768,  896, 1024,    0
};

/********************************************************************/

static Ipp32s alsLMSPredictorStepsizeTable[] =
{
   1,   2,   3,   4,   5,   6,   7,   8,
   9,  10,  11,  12,  13,  14,  15,  16,
  18,  20,  22,  24,  26,  28,  30,  35,
  40,  45,  50,  55,  60,  70,  80, 100
};

/********************************************************************/

static Ipp32s alsLTPTable[] =
{
   0,   8,  16,  24,  32,  40,  48,  56,
  64,  70,  76,  82,  88,  92,  96, 100
};

/********************************************************************/

static Ipp32s alsMCCGainTable[] =
{
  204,  192,  179,  166,  153,  140,  128,  115,
  102,   89,   76,   64,   51,   38,   25,   12,
    0,  -12,  -25,  -38,  -51,  -64,  -76,  -89,
 -102, -115, -128, -140, -153, -166, -179, -192
};

/********************************************************************/

Ipp32s alsdecParseBlockData(sBitsreamBuffer *pBS,
                            ALSDec          *state,
                            alsBlockState   *blockState)
{
  Ipp32s tmpBuf[16];
  Ipp32s ecSub, shiftLsbs;
  Ipp32s start;
  Ipp32s i, b, tmp, len;

  GET_BITS(pBS, blockState->blockType, 1, Ipp32s)

  if (blockState->blockType == 0) {
    GET_BITS(pBS, tmp, 7, Ipp32s)
    blockState->constBlock = (tmp >> 6) & 1;
    blockState->jsBlock = (tmp >> 5) & 1;

    if (blockState->constBlock == 1) {
      if (state->floating == 0) {
        GET_BITS(pBS, blockState->constVal, state->resolution, Ipp32s)
        if (state->resolution != 32) {
          blockState->constVal =
            ((blockState->constVal << (32 - state->resolution)) >>
                                      (32 - state->resolution));
        }
      } else {
        GET_BITS(pBS, blockState->constVal, 24, Ipp32s)
          blockState->constVal = ((blockState->constVal << 8) >> 8);
      }
    }
  } else {
    GET_BITS(pBS, blockState->jsBlock, 1, Ipp32s)

    if ((state->bgmcMode == 0) && (state->sbPart == 0)) {
      blockState->subBlocks = 1;
    } else if ((state->bgmcMode == 1) && (state->sbPart ==1)) {
      GET_BITS(pBS, ecSub, 2, Ipp32s)
      blockState->subBlocks = 1 << ecSub;
    } else {
      GET_BITS(pBS, ecSub, 1, Ipp32s)
      if (ecSub == 0) {
        blockState->subBlocks = 1;
      } else {
        blockState->subBlocks = 4;
      }
    }

    if (state->bgmcMode == 0) {
      Ipp32s len = 4;
      if (state->resolution > 16)
        len = 5;

      GET_BITS(pBS, blockState->s[0], len, Ipp32s)

      if (blockState->subBlocks != 1) {
        riceDecode(pBS, 0, tmpBuf, 3);
        blockState->s[1] = blockState->s[0] + tmpBuf[0];
        blockState->s[2] = blockState->s[1] + tmpBuf[1];
        blockState->s[3] = blockState->s[2] + tmpBuf[2];
      }
    } else {
      Ipp32s len = 8;
      if (state->resolution > 16)
        len = 9;

      GET_BITS(pBS, tmpBuf[0], len, Ipp32s)

      riceDecode(pBS, 2, tmpBuf+1, blockState->subBlocks-1);

      for (i = 1; i < blockState->subBlocks; i++) {
        tmpBuf[i] += tmpBuf[i-1];
      }

      for (i = 0; i < blockState->subBlocks; i++) {
        blockState->s[i] = tmpBuf[i] >> 4;
        blockState->sx[i] = tmpBuf[i] & 0xf;
      }
    }

    blockState->sbLength = blockState->blockLength / blockState->subBlocks;

    GET_BITS(pBS, shiftLsbs, 1, Ipp32s)

    blockState->shiftPos = 0;
    if (shiftLsbs == 1) {
      GET_BITS(pBS, blockState->shiftPos, 4, Ipp32s)
      blockState->shiftPos += 1;
    }

    blockState->optOrder = 10;
    if (!state->RLSLMS) {
      blockState->optOrder = state->maxOrder;

      if (state->adaptOrder == 1) {
        tmp = (blockState->blockLength >> 3) - 1;
        if (tmp < 0)
          tmp = 0;
        if (tmp > state->maxOrder)
          tmp = state->maxOrder;
        if (tmp < 32) {
          len = alsLogTab[tmp];
        } else {
          len = alsLogTab[tmp >> 5] + 5;
        }

        GET_BITS(pBS, blockState->optOrder, len, Ipp32s)
        if (blockState->optOrder > state->maxOrder) {
          return -1;
        }
      }

      if (state->coefTable == 3) {
        for (i = 0; i < blockState->optOrder; i++) {
          GET_BITS(pBS, tmp, 7, Ipp32s)
          blockState->parcor[i] = tmp - 64;
        }
      } else {
        Ipp32s tmp0 = 20;
        Ipp32s tmp1 = 127;

        if (tmp0 > blockState->optOrder)
          tmp0 = blockState->optOrder;

        if (tmp1 > blockState->optOrder)
          tmp1 = blockState->optOrder;

        for (i = 0; i < tmp0; i++) {
          riceDecode(pBS, state->parcorRiceParam[i], blockState->parcor+i, 1);
          blockState->parcor[i] += state->parcorOffset[i];
        }

        riceDecode(pBS, 2, blockState->parcor+tmp0, tmp1 - tmp0);
        riceDecode(pBS, 1, blockState->parcor+tmp1, blockState->optOrder - tmp1);

        for (i = tmp0; i < tmp1; i++) {
          blockState->parcor[i] += (i & 1);
        }
      }
    }

    if (state->LongTermPrediction) {
      GET_BITS(pBS, blockState->LTPenable, 1, Ipp32s)

      if (blockState->LTPenable) {
        riceDecode(pBS, 1, blockState->LTPgain+0, 1);
        riceDecode(pBS, 2, blockState->LTPgain+1, 1);
        ricePosDecode(pBS, 2, blockState->LTPgain+2, 1);
        riceDecode(pBS, 2, blockState->LTPgain+3, 1);
        riceDecode(pBS, 1, blockState->LTPgain+4, 1);

        blockState->LTPgain[0] <<= 3;
        blockState->LTPgain[1] <<= 3;
        blockState->LTPgain[3] <<= 3;
        blockState->LTPgain[4] <<= 3;

        blockState->LTPgain[2] = alsLTPTable[blockState->LTPgain[2]];

        GET_BITS(pBS, blockState->LTPlag, state->lagNeededLen, Ipp32s)
      }
    }

    start = 0;
    if (blockState->raBlock) {
      Ipp32s riceParam;
      if (blockState->optOrder > 0) {
        riceDecode(pBS, state->resolution-4, &blockState->res[0], 1);
      }

      if (blockState->optOrder > 1) {
        riceParam = blockState->s[0] + 3;
        if (riceParam > 31)
          riceParam = 31;

        riceDecode(pBS, riceParam, &blockState->res[1], 1);
      }

      if (blockState->optOrder > 2) {
        riceParam = blockState->s[0] + 1;
        if (riceParam > 31)
          riceParam = 31;

        riceDecode(pBS, riceParam, &blockState->res[2], 1);
      }

      if (blockState->optOrder < 3) {
        start = blockState->optOrder;
      } else {
        start = 3;
      }
    }

    if (state->bgmcMode) {
      if (bgmcDecode(pBS, blockState, start) < 0) {
        return -1;
      }
    } else {
      riceDecode(pBS, blockState->s[0], blockState->res + start,
                 blockState->sbLength - start);
      for (b = 1; b < blockState->subBlocks; b++) {
        riceDecode(pBS, blockState->s[b], blockState->res + b*blockState->sbLength,
                   blockState->sbLength);
      }
    }
  }

  if (state->RLSLMS) {
    Ipp32s extMode;

    GET_BITS(pBS, tmp, 2, Ipp32s)
    state->RLSMonoBlock  = (tmp >> 1) & 1;
    extMode = tmp & 1;

    if (extMode) {
      GET_BITS(pBS, state->RLSExtensionBits, 3, Ipp32s)

      if (state->RLSExtensionBits & 1) {
        GET_BITS(pBS, tmp, 7, Ipp32s)
        state->RLSFilterOrder = (tmp >> 2) & 0x1e;
        state->LMSStage =  (tmp & 7);

        for (i = 0; i < state->LMSStage; i++) {
          GET_BITS(pBS, state->LMSFilterOrder[i], 5, Ipp32s)
          state->LMSFilterOrder[i] =
            alsLMSPredictorOrderTable[state->LMSFilterOrder[i]];
        }
      }

      if (state->RLSExtensionBits & 2) {
        if (state->RLSFilterOrder) {
#ifdef ALS_STANDARD
          GET_BITS(pBS, state->RLSLambda, 10, Ipp32s)
          if (state->ra) {
            GET_BITS(pBS, state->RLSLambdaRA, 10, Ipp32s)
          }
#else
          GET_BITS(pBS, state->RLSLambdaRA, 10, Ipp32s)
          GET_BITS(pBS, state->RLSLambda, 10, Ipp32s)
#endif
        }
      }

      if (state->RLSExtensionBits & 4) {
        for (i = 0; i < state->LMSStage; i++) {
          GET_BITS(pBS, tmp, 5, Ipp32s)
          state->LMSFilterMu[i] = alsLMSPredictorStepsizeTable[tmp];
        }
        GET_BITS(pBS, tmp, 3, Ipp32s)
        state->LMSStepSize = tmp * 16777; /* 0.001 in Q24 */
      }
    }
  }

  if (!state->isMCC) {
    Byte_alignment(pBS);
  }

  return 0;
}

/********************************************************************/

void alsdecParseChannelData(sBitsreamBuffer *pBS,
                            ALSDec          *state,
                            Ipp32s ch,
                            Ipp32s *pLayer)
{
  alsMCCState *mccState = state->mccState[ch];
  Ipp32s layer = -1;
  Ipp32s timeDifferenceSign;

  for(;;) {
    Ipp32s stopFlag;

    GET_BITS(pBS, stopFlag, 1, Ipp32s)

    if (stopFlag == 1) {
      break;
    }

    if (layer >= (NUM_MCC_LAYERS - 1)) {
      break;
    }

    layer++;

    mccState->stopFlag[layer] = 0;

    GET_BITS(pBS, mccState->masterChannelIndex[layer], state->chNeededLen, Ipp32s)

    if (ch != mccState->masterChannelIndex[layer]) {
      GET_BITS(pBS, mccState->timeDifferenceFlag[layer], 1, Ipp32s)

      riceDecode(pBS, 1, mccState->WeightingFactor[layer] + 0, 1);
      riceDecode(pBS, 2, mccState->WeightingFactor[layer] + 1, 1);
      mccState->WeightingFactor[layer][0] =
        alsMCCGainTable[mccState->WeightingFactor[layer][0] + 16];
      mccState->WeightingFactor[layer][1] =
        alsMCCGainTable[mccState->WeightingFactor[layer][1] + 14];

      if (mccState->timeDifferenceFlag[layer] == 0) {
        riceDecode(pBS, 1, mccState->WeightingFactor[layer] + 2, 1);

        mccState->WeightingFactor[layer][2] =
          alsMCCGainTable[mccState->WeightingFactor[layer][2] + 16];
      } else {
        riceDecode(pBS, 1, mccState->WeightingFactor[layer] + 2, 4);

        mccState->WeightingFactor[layer][2] =
          alsMCCGainTable[mccState->WeightingFactor[layer][2] + 16];
        mccState->WeightingFactor[layer][3] =
          alsMCCGainTable[mccState->WeightingFactor[layer][3] + 16];
        mccState->WeightingFactor[layer][4] =
          alsMCCGainTable[mccState->WeightingFactor[layer][4] + 16];
        mccState->WeightingFactor[layer][5] =
          alsMCCGainTable[mccState->WeightingFactor[layer][5] + 16];

        GET_BITS(pBS, timeDifferenceSign, 1, Ipp32s)
        GET_BITS(pBS, mccState->timeDifferenceIndex[layer], state->timeNeededLen, Ipp32s)
        mccState->timeDifferenceIndex[layer] += 3;
        if (timeDifferenceSign) {
          mccState->timeDifferenceIndex[layer] =
            -mccState->timeDifferenceIndex[layer];
        }
      }
    }
  }

  Byte_alignment(pBS);
  *pLayer = layer;
}

/********************************************************************/

#define ALS_MLZ_FLUSH_CODE  256
#define ALS_MLZ_FREEZE_CODE 257
#define ALS_MLZ_FIRST_CODE  258
#define ALS_MLZ_MAX_CODE    32767

/********************************************************************/

void alsMLZflushDict(alsMLZState *MLZState)
{
  MLZState->codeBits = 9;
  MLZState->freezeFlag = 0;
  MLZState->bumpCode = 511;
  MLZState->nextCode = ALS_MLZ_FIRST_CODE;
}

/********************************************************************/

void alsMLZSetNewEntryToDict(Ipp32s code,
                             Ipp32s letter,
                             alsMLZState *MLZState)
{
  MLZState->MLZDict[MLZState->nextCode].pos = MLZState->MLZDict[code].pos + 1;
  MLZState->MLZDict[MLZState->nextCode].letter = (Ipp16s)letter;
  MLZState->MLZDict[MLZState->nextCode].code = (Ipp16s)code;
}

/********************************************************************/

Ipp32s alsMLZDecodeString(Ipp8u* decBuf,
                          Ipp32s code,
                          Ipp32s bufLen,
                          alsMLZState *MLZState)
{
  Ipp32s maxPosLen, outBytes;

  maxPosLen = bufLen;
  if (ALS_MLZ_MAX_CODE < bufLen) {
    maxPosLen = ALS_MLZ_MAX_CODE;
  }

  outBytes = 0;
  for (;;) {
    if (outBytes >= bufLen) {
      return outBytes;
    }
    if ((code < 0) || (code > ALS_MLZ_MAX_CODE)) {
      return outBytes;
    } else if (code < ALS_MLZ_FIRST_CODE) {
      decBuf[0] = (Ipp8u)code;
      return (outBytes + 1);
    } else {
      Ipp32s pos = MLZState->MLZDict[code].pos;
      if ((pos < 0) || (pos > maxPosLen)) {
        return outBytes;
      }
      decBuf[pos] = (Ipp8u)(MLZState->MLZDict[code].letter);
      code = MLZState->MLZDict[code].code;
      outBytes++;
    }
  }
}

/********************************************************************/

void alsMasked_LZ_decompression(sBitsreamBuffer *pBS,
                                alsMLZState *MLZState,
                                Ipp32s nChars)
{
  Ipp8u *decBuf = (Ipp8u*)MLZState->tmpBuf;
  Ipp32s decChars;
  Ipp32s tmp;
  Ipp32s lastStringCode, stringCode;

  lastStringCode = -1;

  for (decChars = 0; decChars < nChars;) {
    GET_BITS(pBS, stringCode, MLZState->codeBits, Ipp32s);

    {
      int i;
      tmp = 0;
      for (i = 0; i < MLZState->codeBits; i++) {
        tmp = (tmp << 1) + ((stringCode >> i) & 1);
      }
      stringCode = tmp;
    }

    if ((stringCode == ALS_MLZ_FLUSH_CODE) || (stringCode == ALS_MLZ_MAX_CODE)) {
      alsMLZflushDict(MLZState);
      lastStringCode = -1;
    } else if (stringCode == ALS_MLZ_FREEZE_CODE) {
      MLZState->freezeFlag = 1;
    } else if (stringCode == MLZState->bumpCode) {
      MLZState->codeBits++;
      MLZState->bumpCode = MLZState->bumpCode * 2 + 1;
    } else {
      if (stringCode >= MLZState->nextCode) {
        if (lastStringCode != -1) {
          tmp = alsMLZDecodeString(decBuf+decChars, lastStringCode,
                                   nChars - decChars, MLZState);
          decBuf[decChars+tmp] = decBuf[decChars];
          alsMLZSetNewEntryToDict(lastStringCode, decBuf[decChars], MLZState);
          decChars += (tmp + 1);
          MLZState->nextCode++;
        }
      } else {
        tmp = alsMLZDecodeString(decBuf+decChars, stringCode,
                                 nChars - decChars, MLZState);
        if ((decChars <= nChars) && (lastStringCode != -1) && (MLZState->freezeFlag == 0)) {
          alsMLZSetNewEntryToDict(lastStringCode, decBuf[decChars], MLZState);
          MLZState->nextCode++;
        }
        decChars += tmp;
      }
      lastStringCode = stringCode;
    }
  }
}

/********************************************************************/

void aasdecDiffMantissa(sBitsreamBuffer *pBS,
                        ALSDec          *state)
{
  alsMLZState *MLZState = state->MLZState;
  Ipp8u* pTmp;
  Ipp32s useAcf, acfFlag, partAFlag, shiftAmp;
  Ipp32s nChars, highestByte, compressedFlag;
  Ipp32s numBytesDiffFloat, startByte, stopByte;
  Ipp32s i, j, ch, tmp, frameLength;

  if (state->frameId != state->numFrames - 1) {
    frameLength = state->frameLength;
  } else {
    frameLength = state->lastFrameLength;
  }


  GET_BITS(pBS, numBytesDiffFloat, 32, Ipp32s);

  GET_BITS_COUNT(pBS, startByte)
  startByte >>= 3;

  GET_BITS(pBS, useAcf, 1, Ipp32s);

  if (state->raFrame) {
    for (ch = 0; ch < state->numChannels; ch++) {
      MLZState->acfMantissa[ch] = 0;
      MLZState->shiftValue[ch] = 0;
    }
    alsMLZflushDict(MLZState);
  }

  for (ch = 0; ch < state->numChannels; ch++) {

    for (i = 0; i < frameLength; i++) {
      MLZState->floatX[i] = (Ipp32f)state->x[ch][i];
    }

    if (useAcf == 1) {
      GET_BITS(pBS, acfFlag, 1, Ipp32s);

      if (acfFlag == 1) {
        GET_BITS(pBS, MLZState->acfMantissa[ch], 23, Ipp32s);
      }
    } else {
      MLZState->acfMantissa[ch] = 0;
    }

    GET_BITS(pBS, tmp, 4, Ipp32s);

    highestByte = (tmp >> 2) & 3;
    partAFlag = (tmp >> 1) & 1;
    shiftAmp = tmp & 1;

    if (shiftAmp == 1) {
      GET_BITS(pBS, MLZState->shiftValue[ch], 8, Ipp32s);
    }

    if (highestByte == 0) {
      for (i = 0; i < frameLength; i++) {
        MLZState->diffMantissa[i] = 0;
      }
    }

    /* diff_mantissa() */
    if (partAFlag != 0) {
      GET_BITS(pBS, compressedFlag, 1, Ipp32s);

      if (compressedFlag == 0) {
        for (i = 0; i < frameLength; i++) {
          if (state->x[ch][i] == 0) {
            GET_BITS(pBS, MLZState->diffMantissa[i], 32, Ipp32s);
          }
        }
      } else {
        nChars = 0;
        for (i = 0; i < frameLength; i++) {
          if (state->x[ch][i] == 0) {
            nChars += 4;
          }
        }
        alsMasked_LZ_decompression(pBS, MLZState, nChars);

        j = 0;
        for (i = 0; i < frameLength; i++) {
          if (state->x[ch][i] == 0) {
            Ipp8u *pTmp0 = (Ipp8u *)(MLZState->diffMantissa + i);
            Ipp8u *pTmp1 = (Ipp8u *)(MLZState->tmpBuf + j);

            pTmp0[0] = pTmp1[3];
            pTmp0[1] = pTmp1[2];
            pTmp0[2] = pTmp1[1];
            pTmp0[3] = pTmp1[0];
            j++;
          }
        }
      }
    } else {
      for (i = 0; i < frameLength; i++) {
        if (state->x[ch][i] == 0) {
          MLZState->diffMantissa[i] = 0;
        }
      }
    }

    if (highestByte != 0) {
      GET_BITS(pBS, compressedFlag, 1, Ipp32s);

      if (compressedFlag == 0) {
        if (MLZState->acfMantissa[ch] == 0) {
          for (i = 0; i < frameLength; i++) {
            if (state->x[ch][i] != 0) {
              Ipp32u intX = *(Ipp32u*)&MLZState->floatX[i];
              MLZState->nBits[0] = 150 - ((intX >> 23) & 0xff);
              if (MLZState->nBits[0] > highestByte * 8) {
                MLZState->nBits[0] = highestByte * 8;
              }

              GET_BITS(pBS, MLZState->diffMantissa[i], MLZState->nBits[0], Ipp32s);
            }
          }
        } else {
          MLZState->nBits[0] = 23;
          if (MLZState->nBits[0] > highestByte * 8) {
            MLZState->nBits[0] = highestByte * 8;
          }

          for (i = 0; i < frameLength; i++) {
            if (state->x[ch][i] != 0) {
              GET_BITS(pBS, MLZState->diffMantissa[i], MLZState->nBits[0], Ipp32s);
            }
          }
        }
      } else {
        nChars = 0;
        if (MLZState->acfMantissa[ch] == 0) {
          for (i = 0; i < frameLength; i++) {
            if (state->x[ch][i] != 0) {
              Ipp32u intX = *(Ipp32u*)&MLZState->floatX[i];
              MLZState->nBits[i] = 150 - ((intX >> 23) & 0xff);
              if (MLZState->nBits[i] > highestByte * 8) {
                MLZState->nBits[i] = highestByte * 8;
              }

              MLZState->alignedLen[i] = (MLZState->nBits[i] + 7) >> 3;
              nChars += MLZState->alignedLen[i];
            }
          }

          alsMasked_LZ_decompression(pBS, MLZState, nChars);

          pTmp = (Ipp8u*)MLZState->tmpBuf;
          for (i = 0; i < frameLength; i++) {
            if (state->x[ch][i] != 0) {
              tmp = 0;
              for (j = 0; j < MLZState->alignedLen[i]; j++) {
                tmp = (tmp << 8) + pTmp[0];
                pTmp++;
              }
              MLZState->diffMantissa[i] =
                (tmp >> (MLZState->alignedLen[i] * 8 - MLZState->nBits[i]));
            }
          }

        } else {
          Ipp32s shift;
          MLZState->nBits[0] = 23;
          if (MLZState->nBits[0] > highestByte * 8) {
            MLZState->nBits[0] = highestByte * 8;
          }
          MLZState->alignedLen[0] = (MLZState->nBits[0] + 7) >> 3;

          shift = (MLZState->alignedLen[0] * 8 - MLZState->nBits[0]);

          for (i = 0; i < frameLength; i++) {
            if (state->x[ch][i] != 0) {
              nChars += MLZState->alignedLen[0];
            }
          }

          alsMasked_LZ_decompression(pBS, MLZState, nChars);

          pTmp = (Ipp8u*)MLZState->tmpBuf;
          for (i = 0; i < frameLength; i++) {
            if (state->x[ch][i] != 0) {
              tmp = 0;
              for (j = 0; j < MLZState->alignedLen[0]; j++) {
                tmp = (tmp << 8) + pTmp[0];
                pTmp++;
              }
              MLZState->diffMantissa[i] = tmp >> shift;
            }
          }
        }
      }
    }

    Byte_alignment(pBS);

    for (i = 0; i < frameLength; i++) {
      if (state->x[ch][i] == 0) {
        state->x[ch][i] = MLZState->diffMantissa[i];
      } else {
        Ipp64s tmp64;
        Ipp32s scaleFactor;
        Ipp32u intX = *(Ipp32u*)&(MLZState->floatX[i]);
        Ipp32s signX = intX &0x80000000;
        Ipp32s expX = ((intX >> 23) & 0xff) - 23;
        Ipp32s mantX = (intX & 0x7fffff) | 0x800000;

        /* multiplication */
        if (MLZState->acfMantissa[ch] != 0) {
          Ipp32s round;

          tmp64 = ((Ipp64s)(MLZState->acfMantissa[ch] | 0x800000) * (Ipp64s)mantX);
          scaleFactor = 23;

          if (tmp64 & ((Ipp64s)1 << 47)) {
            scaleFactor = 24;
            expX++;
          }

          round = ((Ipp64s)1 << (scaleFactor - 1)) - 1;
          mantX = (Ipp32s)((tmp64 + round + ((tmp64 >> scaleFactor) & 1)) >> scaleFactor);
          if ((mantX & 0x1000000) != 0) {
            mantX >>= 1;
            expX++;
          }
        }

        /* addition */

        mantX = (MLZState->diffMantissa[i] + mantX);
        if ((mantX & 0x1000000) != 0) {
          mantX >>= 1;
          expX++;
        }

        expX += (MLZState->shiftValue[ch] - 127);

        state->x[ch][i] = signX + (expX << 23) + (mantX & 0x7fffff);
      }
    }
  }

  GET_BITS_COUNT(pBS, stopByte)
  stopByte >>= 3;

  for (i = 0; i < numBytesDiffFloat - stopByte + startByte; i++) {
    GET_BITS(pBS, tmp, 8, Ipp32s);
  }
}

/********************************************************************/

void alsgetBlocks(Ipp32s bsInfo,
                  Ipp32s len,
                  Ipp32s *blockLen,
                  Ipp32s *numBlocks)
{
  Ipp32s i0, i1, i2, i3;
  Ipp32s num = 0;

  if ((bsInfo & 0x40000000) == 0) { /* no further division */
    blockLen[num] = len;
    num++;
  } else { /* further division */
    for (i0 = 0; i0 < 2; i0++) {
      if (((bsInfo << i0) & 0x20000000) == 0) { /* no further division */
        blockLen[num] = len >> 1;
        num++;
      } else { /* further division */
        for (i1 = 0; i1 < 2; i1++) {
          if ((bsInfo << (i0*2+i1) & 0x8000000) == 0) { /* no further division */
            blockLen[num] = len >> 2;
            num++;
          } else { /* further division */
            for (i2 = 0; i2 < 2; i2++) {
              if ((bsInfo << (i0*4+i1*2+i2) & 0x800000) == 0) { /* no further division */
                blockLen[num] = len >> 3;
                num++;
              } else { /* further division */
                for (i3 = 0; i3 < 2; i3++) {
                  if ((bsInfo << (i0*8+i1*4+i2*2+i3) & 0x8000) == 0) { /* no further division */
                    blockLen[num] = len >> 4;
                    num++;
                  } else {
                    blockLen[num] = len >> 5;
                    blockLen[num+1] = len >> 5;
                    num += 2;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  *numBlocks = num;
}

/********************************************************************/

static void riceDecode(sBitsreamBuffer *pBS,
                       Ipp32s s,
                       Ipp32s *dst,
                       Ipp32s len)
{
  Ipp32s i, p, tmp;

  if (s == 0) {
    for (i = 0; i < len; i++) {
      p = 0;
      for (;;) {
        GET_BITS(pBS, tmp, 1, Ipp32s);
        if (tmp == 0) {
          break;
        }
        p++;
      }

      if ((p & 1) == 0)
        dst[i] = p >> 1;
      else
        dst[i] = (-(p + 1)) >> 1;
    }
  } else {
    for (i = 0; i < len; i++) {
      p = 0;
      for (;;) {
        GET_BITS(pBS, tmp, 1, Ipp32s);
        if (tmp == 0) {
          break;
        }
        p++;
      }

      GET_BITS(pBS, tmp, s, Ipp32s);

      if (tmp & (1 << (s-1)))
        dst[i] = tmp + ((p-1) << (s-1));
      else
        dst[i] = -(tmp + (p << (s-1)) + 1);
    }
  }
}

/********************************************************************/

static void ricePosDecode(sBitsreamBuffer *pBS,
                          Ipp32s s,
                          Ipp32s *dst,
                          Ipp32s len)
{
  Ipp32s i, p, tmp;

  for (i = 0; i < len; i++) {
    p = 0;
    for (;;) {
      GET_BITS(pBS, tmp, 1, Ipp32s);
      if (tmp == 0) {
        break;
      }
      p++;
    }

    GET_BITS(pBS, tmp, s, Ipp32s);
    dst[i] = tmp + (p << s);
  }
}

/********************************************************************/

#define FREQ_BITS  14      /* num. bits used by frequency counters  */
#define VALUE_BITS 18      /* num. bits used to describe code range */
#define TOP_VALUE  0x3ffff /* lagest code value                     */
#define FIRST_QTR  0x10000 /* first quarter                         */
#define HALF       0x20000 /* first half                            */
#define THIRD_QTR  0x30000 /* third quarter                         */

typedef struct {
  Ipp32s high;
  Ipp32s low;
  Ipp32s value;
} alsBgmcState;

/********************************************************************/

static Ipp32s bgmcDecodeBlock(sBitsreamBuffer *pBS,
                              alsBgmcState *bgmcState,
                              Ipp32s delta,
                              Ipp16u *sFreq,
                              Ipp32s *res,
                              Ipp32s len)
{
  Ipp32u range, target, symbol;
  Ipp32u high, low, value, tmp;
  Ipp32s decodedBits, remainedBits;
  Ipp32s i, del;

  high = bgmcState->high;
  low = bgmcState->low;
  value = bgmcState->value;
  del = 1 << delta;

  GET_BITS_COUNT(pBS, decodedBits)
  remainedBits = pBS->nDataLen * 8 - decodedBits;

  for (i = 0; i < len; i++) {
    range = high - low + 1;
    target = (((value - low + 1) << FREQ_BITS) - 1) / range;

    symbol = del;
    while (sFreq[symbol] > target)
      symbol += del;

    high = low + ((range * sFreq[symbol - del] - (1 << FREQ_BITS)) >> FREQ_BITS);
    low  = low + ((range * sFreq[symbol]) >> FREQ_BITS);

    res[i] = (symbol - del) >> delta;

    for (;;) {
      if (high >= HALF) {
        if (low >= HALF) {
          value -= HALF;
          low -= HALF;
          high -= HALF;
        } else if ((low >= FIRST_QTR) && (high < THIRD_QTR)) {
          value -= FIRST_QTR;
          low -= FIRST_QTR;
          high -= FIRST_QTR;
        } else {
          break;
        }
      }
      if (remainedBits == 0) {
        return -1;
      }
      low = (low << 1);
      high = (high << 1) + 1;
      GET_BITS(pBS, tmp, 1, Ipp32u);
      value = (value << 1) + tmp;
      remainedBits--;
    }
  }

  bgmcState->high = high;
  bgmcState->low = low;
  bgmcState->value = value;

  return 0;
}

/********************************************************************/

static void bgmcReconstructBlock(sBitsreamBuffer *pBS,
                                 Ipp32s absMaxX,
                                 Ipp32s tailCode,
                                 Ipp32s s,
                                 Ipp32s k,
                                 Ipp32s *res,
                                 Ipp32s len)
{
  Ipp32s i, tail, lsb, msbi;

  for (i = 0; i < len; i++) {
    msbi = res[i];

    if (msbi == tailCode) {
      riceDecode(pBS, s, &tail, 1);

      if (tail >= 0) {
        res[i] = tail + ((absMaxX) << k);
      } else {
        res[i] = tail - ((absMaxX - 1) << k);
      }
    } else {

      if (msbi > tailCode) {
        msbi--;
      }

      if (msbi & 1) {
        msbi = (-msbi - 1) >> 1;
      } else {
        msbi >>= 1;
      }

      if (k != 0) {
        GET_BITS(pBS, lsb, k, Ipp32s);
        res[i] = (msbi << k) | lsb;
      } else {
        res[i] = msbi;
      }
    }
  }
}

/********************************************************************/

static Ipp32s bgmcDecode(sBitsreamBuffer *pBS,
                         alsBlockState   *blockState,
                         Ipp32s          start)
{

  alsBgmcState bgmcState;
  Ipp32s k[8], delta[8];
  Ipp32s i, b;

  /* bgmc start decoding */
  bgmcState.high = TOP_VALUE;
  bgmcState.low = 0;
  GET_BITS(pBS, bgmcState.value, VALUE_BITS, Ipp32s);

  if (blockState->blockLength < 8) {
    b = 0;
  } else if (blockState->blockLength <= 256) {
    b = (alsLogTab[blockState->blockLength - 1] - 3) >> 1;
  } else if (blockState->blockLength < 8192) {
    b = (alsLogTab[(blockState->blockLength - 1) >> 8] + 5) >> 1;
  } else {
    b = 5;
  }

  k[0] = 0;
  if (blockState->s[0] > b) {
    k[0] = blockState->s[0] - b;
  }

  delta[0] = 5 - blockState->s[0] + k[0];

  if (bgmcDecodeBlock(pBS, &bgmcState, delta[0], alsFreqTable[blockState->sx[0]],
      blockState->res + start, blockState->sbLength - start) < 0) {
     return -1;
  }

  for (i = 1; i < blockState->subBlocks; i++) {
    k[i] = 0;
    if (blockState->s[i] > b) {
      k[i] = blockState->s[i] - b;
    }

    delta[i] = 5 - blockState->s[i] + k[i];

    if (bgmcDecodeBlock(pBS, &bgmcState, delta[i], alsFreqTable[blockState->sx[i]],
                        blockState->res + i * blockState->sbLength,
                        blockState->sbLength) < 0) {
      return -1;
    }
  }

  /* bgmc finish decoding */
  pBS->nBit_offset += (VALUE_BITS - 2);

  if (pBS->nBit_offset > 32) {
    pBS->nBit_offset -= 32;
    pBS->pCurrent_dword--;
    pBS->dword = BSWAP(pBS->pCurrent_dword[0]);
  }

  bgmcReconstructBlock(pBS, (alsAbsMaxXTable[blockState->sx[0]] >> delta[0]),
                       alsTailCode[blockState->sx[0]][delta[0]],
                       blockState->s[0], k[0], blockState->res + start,
                       blockState->sbLength - start);

  for (i = 1; i < blockState->subBlocks; i++) {
    bgmcReconstructBlock(pBS, (alsAbsMaxXTable[blockState->sx[i]] >> delta[i]),
                         alsTailCode[blockState->sx[i]][delta[i]],
                         blockState->s[i], k[i],
                         blockState->res + i * blockState->sbLength,
                         blockState->sbLength);
  }

  return 0;
}

/********************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_DECODER

