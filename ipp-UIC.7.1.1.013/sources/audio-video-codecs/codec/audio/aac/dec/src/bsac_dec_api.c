/*
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
//     Intel(R) Integrated Performance Primitives AAC Decode Sample for Windows*
//
//  By downloading and installing this sample, you hereby agree that the
//  accompanying Materials are being provided to you under the terms and
//  conditions of the End User License Agreement for the Intel(R) Integrated
//  Performance Primitives product previously accepted by you. Please refer
//  to the file ippEULA.rtf or ippEULA.txt located in the root directory of your Intel(R) IPP
//  product installation for more information.
//
//  MPEG-4 and AAC are international standards promoted by ISO, IEC, ITU, ETSI
//  and other organizations. Implementations of these standards, or the standard
//  enabled platforms may require licenses from various entities, including
//  Intel Corporation.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

/********************************************************************/

#include "bsac_dec.h"
#include "bsac_dec_tabs.h"
#include "bstream.h"
#include "aac_dec_own_fp.h"
#include "aac_dec_decoding_fp.h"
#include <math.h>

#define BSAC_MAX_CBAND0_SI_LEN 11

/********************************************************************/

static Ipp32s bsacDecodeSymbol(Ipp16s *freq,
                        Ipp32s *sym,
                        BSACDec *bsState,
                        sBitsreamBuffer *pBS)
{
  Ipp32s value = bsState->value;
  Ipp32s range = bsState->range;
  Ipp32s estCwLen = bsState->estCwLen;
  Ipp32s tmp, trh, symbol;

  if (estCwLen != 0) {
    range <<= estCwLen;
    GET_BITS(pBS, tmp, estCwLen, Ipp32s)
      value = (value << estCwLen) | tmp;
  }

  range >>= 14;
  trh = value/range;

  if (trh < 0) {
    return -1;
  }

  /* Find symbol */
  for (symbol = 0; freq[symbol] > trh; symbol++);

  value -= (range * freq[symbol]);

  if (symbol > 0) {
    range *= (freq[symbol - 1] - freq[symbol]);
  } else {
    range *= (16384 - freq[symbol]);
  }

  for (estCwLen = 0; range < bsacHalf[estCwLen]; estCwLen++);

  bsState->value = value;
  bsState->range = range;
  bsState->estCwLen = estCwLen;
  *sym = symbol;
  return 0;
}

/********************************************************************/

static void bsacDecodeSymbolBinary(Ipp16s p0,
                            Ipp32s *sym,
                            BSACDec *bsState,
                            sBitsreamBuffer *pBS)
{
  Ipp32s value = bsState->value;
  Ipp32s range = bsState->range;
  Ipp32s estCwLen = bsState->estCwLen;
  Ipp32s tmp;

  if (estCwLen != 0) {
    range <<= estCwLen;
    GET_BITS(pBS, tmp, estCwLen, Ipp32s)
      value = (value << estCwLen) | tmp;
  }

  range >>= 14;

  /* Find symbol */
  if ((p0 * range) <= value) {
    *sym = 1;

    value -= range * p0;
    range *= (16384 - p0);
  } else {
    *sym = 0;

    range *= p0;
  }

  for (estCwLen = 0; range < bsacHalf[estCwLen]; estCwLen++);

  bsState->value = value;
  bsState->range = range;
  bsState->estCwLen = estCwLen;
}

/********************************************************************/

static Ipp32s bsacDecodingCBandSideInfo(Ipp32s  layer,
                                 Ipp32s  numChannels,
                                 Ipp32s  *pDecLen,
                                 BSACDec *bsState,
                                 sBitsreamBuffer *pBS)
{
  Ipp16s *freqTab;
  Ipp32s group = bsState->layerGroup[layer];
  Ipp32s start = bsState->layerStartCBand[layer];
  Ipp32s end   = bsState->layerEndCBand[layer];
  Ipp32s startIndex = bsState->layerStartIndex[layer];
  Ipp32s endIndex   = bsState->layerEndIndex[layer];
  Ipp32s decLen;
  Ipp32s i, cband, maxCbandSI, ch;

  decLen = 0;

  for (ch = 0; ch < numChannels; ch++) {
    if (start == 0) {
      freqTab = bsacCbandModel7;
      maxCbandSI = bsacLargestCband0SiTbl[bsState->cbandSiType[ch]];
    } else {
      freqTab = bsacCbandModelTab[bsState->cbandSiType[ch]];
      maxCbandSI = bsacLargestCbandSiTbl[bsState->cbandSiType[ch]];
    }

    for (cband = start; cband < end; cband++) {
      if (bsacDecodeSymbol(freqTab, &bsState->cbandSI[ch][group][cband],
          bsState, pBS) < 0) {
        return -1;
      }
      decLen += bsState->estCwLen;

      if (bsState->cbandSI[ch][group][cband] > maxCbandSI) {
        bsState->cbandSI[ch][group][cband] = maxCbandSI;
      }

      if (bsState->cbandSI[ch][group][cband] >= 15) {
        bsState->MSBPlane[ch][group][cband] =
          bsState->cbandSI[ch][group][cband] - 7;
      } else {
        bsState->MSBPlane[ch][group][cband] =
          (bsState->cbandSI[ch][group][cband] + 1) >> 1;
      }
    }

    for (i = startIndex; i < endIndex; i++) {
      bsState->curSnf[ch][group][i] = bsState->MSBPlane[ch][group][i >> 5];
    }
  }

  *pDecLen = decLen;

  return 0;
}

/********************************************************************/

static Ipp32s bsacDecodingScalefactor(Ipp32s layer,
                               Ipp32s *pDecLen,
                               AACDec *state,
                               sBitsreamBuffer *pBS)
{
  sCpe_channel_element *cpe = &(state->com.m_cpe);
  BSACDec              *bsState = state->bsState;
  Ipp32s *sfbModel;
  Ipp32s numChannels = bsState->numChannels;
  Ipp32s group = bsState->layerGroup[layer];
  Ipp32s start = bsState->layerStartSfb[layer];
  Ipp32s end   = bsState->layerEndSfb[layer];
  Ipp32s decLen, tmp;
  Ipp32s i, ch;

  if (layer < bsState->sLayerSize) {
    sfbModel = bsState->bsaeScfModel;
  } else {
    sfbModel = bsState->enhScfModel;
  }

  decLen = 0;

  for (ch = 0; ch < numChannels; ch++) {
    for (i = start; i < end; i++) {
      if (numChannels == 1) {
        if (bsState->pnsDataPresent == 1) {
          if (i >= bsState->pnsDataSfb) {
            if (bsacDecodeSymbol(bsacNoiseFlagModelTab, &tmp, bsState, pBS) < 0) {
              return -1;
            }
            decLen += bsState->estCwLen;
            bsState->pnsFlag[0][group][i] = tmp;
          }
        }
      } else if (!bsState->alreadyDecoded[group][i]) {
        bsState->alreadyDecoded[group][i] = 1;
        if (cpe->ms_mask_present != 2) {
          tmp = 0;
          if (cpe->ms_mask_present == 1) {
            if (bsacDecodeSymbol(bsacMsUsedModelTab, &tmp, bsState, pBS) < 0) {
              return -1;
            }
            decLen += bsState->estCwLen;
          } else if (cpe->ms_mask_present == 3) {
            if (bsacDecodeSymbol(bsacStereoInfoModelTab, &tmp, bsState, pBS) < 0) {
              return -1;
            }
            decLen += bsState->estCwLen;
          }

          if (tmp == 1) {
            cpe->ms_used[group][i] = 1;
          } else if (tmp == 2) {
            cpe->streams[1].sfb_cb[group][i] = INTENSITY_HCB;
            bsState->intensityDataPresent = 1;
          } else if (tmp == 3) {
            cpe->streams[1].sfb_cb[group][i] = INTENSITY_HCB2;
            bsState->intensityDataPresent = 1;
          }

          if ((bsState->pnsDataPresent == 1) && (i >= bsState->pnsDataSfb)) {
            if (bsacDecodeSymbol(bsacNoiseFlagModelTab, &tmp, bsState, pBS) < 0) {
              return -1;
            }
            decLen += bsState->estCwLen;
            bsState->pnsFlag[0][group][i] = tmp;

            if (bsacDecodeSymbol(bsacNoiseFlagModelTab, &tmp, bsState, pBS) < 0) {
              return -1;
            }
            decLen += bsState->estCwLen;
            bsState->pnsFlag[1][group][i] = tmp;

            if ((cpe->streams[1].sfb_cb[group][i] == INTENSITY_HCB2) &&
                bsState->pnsFlag[0][group][i] &&
                bsState->pnsFlag[1][group][i]) {

              if (bsacDecodeSymbol(bsacNoiseModeModelTab, &tmp, bsState, pBS) < 0) {
                return -1;
              }
              decLen += bsState->estCwLen;
              bsState->pnsMode[group][i] = tmp;
            }
          }
        }
      }

      if (bsState->pnsFlag[ch][group][i]) {

        /* If noise power is not decoded yet */
        if (bsState->pnsEnergyFlag[ch] == 1) {
          if (bsacDecodeSymbol(bsacNoiseEnergyModelTab, &tmp, bsState, pBS) < 0) {
            return -1;
          }
          decLen += bsState->estCwLen;
          bsState->maxPnsPower[ch] = tmp;
          bsState->pnsEnergyFlag[ch] = 0; /*decoded */
        }

        if (bsacDecodeSymbol(bsacScfModelTab[sfbModel[ch]], &tmp, bsState, pBS) < 0) {
          return -1;
        }
        decLen += bsState->estCwLen;
        cpe->streams[ch].sf[group][i] = (Ipp16s)(bsState->maxPnsPower[ch] - tmp);
      } else if ((cpe->streams[1].sfb_cb[group][i] != 0) && (ch == 1)) {
        if (sfbModel[ch] == 0) {
          cpe->streams[ch].sf[group][i] = 0;
        } else {
          if (bsacDecodeSymbol(bsacScfModelTab[sfbModel[ch]], &tmp, bsState, pBS) < 0) {
            return -1;
          }
          decLen += bsState->estCwLen;

          if (tmp & 1) {
            cpe->streams[ch].sf[group][i] = (Ipp16s)(-((tmp + 1) >> 1));
          } else {
            cpe->streams[ch].sf[group][i] = (Ipp16s)(tmp >> 1);
          }
        }
      } else {
        if (sfbModel[ch] == 0) {
          cpe->streams[ch].sf[group][i] = (Ipp16s)bsState->maxScalefactor[ch];
        } else {
          if (bsacDecodeSymbol(bsacScfModelTab[sfbModel[ch]], &tmp, bsState, pBS) < 0) {
            return -1;
          }
          decLen += bsState->estCwLen;
          cpe->streams[ch].sf[group][i] = (Ipp16s)(bsState->maxScalefactor[ch] - tmp);
        }
      }
    }
  }

  *pDecLen = decLen;

  return 0;
}

/********************************************************************/

static void bsacDecodingSpectralData(Ipp32s startGroup,
                              Ipp32s endGroup,
                              Ipp32s *startIndex,
                              Ipp32s *endIndex,
                              Ipp32s thrSnf,
                              Ipp32s numChannels,
                              Ipp32s *availableLen,
                              AACDec *state,
                              sBitsreamBuffer *pBS)
{
  sCpe_channel_element *cpe = &(state->com.m_cpe);
  BSACDec *bsState = state->bsState;
  Ipp32s maxSnf;
  Ipp32s ch, group, i, snf;

  if (*availableLen <= 0)
    return;

  /* maxSnf calculation */
  maxSnf = 0;

  for (ch = 0; ch < numChannels; ch++) {
    for (group = startGroup; group < endGroup; group++) {
      for (i = startIndex[group]; i < endIndex[group]; i++) {
        if (maxSnf < bsState->curSnf[ch][group][i]) {
          maxSnf = bsState->curSnf[ch][group][i];
        }
      }
    }
  }

  for (snf = maxSnf; snf > thrSnf; snf--) {
    Ipp16s scaledBit = (Ipp16s)(1 << (snf - 1));
    for (group = startGroup; group < endGroup; group++) {
      for (i = startIndex[group]; i < endIndex[group]; i++) {
        for (ch = 0; ch < numChannels; ch++) {
          Ipp32s tmp;

          if (bsState->curSnf[ch][group][i] < snf)
            continue;

          /* Sample decoding */
          if ((cpe->streams[ch].p_spectrum[group][i] == 0) ||
              (bsState->signIsCoded[ch][group][i] != 0)) {
            Ipp32s offsetA = (i & 3) << 7;
            Ipp32s cband, deltaSnf, p0Pos;
            Ipp16s p0;

            cband = i >> 5;

            if (offsetA == 0) {
              bsState->offsetHGFE[ch][group][i >> 2] |=
                (bsState->offsetBCD[ch][group][i >> 2] << 3);
              bsState->offsetBCD[ch][group][i >> 2] = 0;
            }

            deltaSnf = bsState->MSBPlane[ch][group][cband] - snf;

            if (bsState->higherBitVector[ch][group][i] != 0) {
              Ipp32s addOffset = bsState->higherBitVector[ch][group][i] - 1;

              if (addOffset > 15) {
                addOffset = 15;
              }

              if (deltaSnf >= 4) {
                deltaSnf = 4;
              }

              p0Pos = (bsacOffsetsNonZero[bsState->cbandSI[ch][group][cband] - 3])[deltaSnf-1] +
                       addOffset;
            } else {
              if (deltaSnf >= 3) {
                deltaSnf = 3;
              }
              p0Pos = (bsacOffsetsZero[bsState->cbandSI[ch][group][cband] - 1])[deltaSnf] +
                      bsacAddOffsetZero[offsetA + bsState->offsetHGFE[ch][group][i >> 2] +
                                                  bsState->offsetBCD[ch][group][i >> 2]];
            }

            p0 = bsacSpectralProbabilityTab[p0Pos];

            if ((*availableLen < 14) && (bsState->sbaMode == 1)) {
              if (p0 < bsacMinP0[*availableLen]) {
                p0 = bsacMinP0[*availableLen];
              } else if (p0 > bsacMaxP0[*availableLen]) {
                p0 = bsacMaxP0[*availableLen];
              }
            }

            bsacDecodeSymbolBinary(p0, &tmp, bsState, pBS);
            *availableLen -= bsState->estCwLen;

            if (tmp != 0) {
              if (cpe->streams[ch].p_spectrum[group][i] < 0) {
                cpe->streams[ch].p_spectrum[group][i] = 
                    cpe->streams[ch].p_spectrum[group][i] - scaledBit;
              } else {
               cpe->streams[ch].p_spectrum[group][i] = 
                   cpe->streams[ch].p_spectrum[group][i] + scaledBit;
              }
            }

            bsState->offsetBCD[ch][group][i >> 2] =
              (bsState->offsetBCD[ch][group][i >> 2] << 1) + tmp;

            bsState->higherBitVector[ch][group][i] =
              (bsState->higherBitVector[ch][group][i] << 1) + tmp;
          }

          /* Decoding of the sign bit */
          if ((cpe->streams[ch].p_spectrum[group][i] != 0) &&
              (bsState->signIsCoded[ch][group][i] == 0)) {

            if (*availableLen <= 0) {
              return;
            }

            bsacDecodeSymbolBinary(8192, &tmp, bsState, pBS);
            *availableLen -= bsState->estCwLen;

            if (tmp != 0) {
              cpe->streams[ch].p_spectrum[group][i] =
                -cpe->streams[ch].p_spectrum[group][i];
            }

            bsState->signIsCoded[ch][group][i] = 1;
          }

          bsState->curSnf[ch][group][i] -= 1;
          if (*availableLen <= 0) {
            return;
          }
        }
      }
    }
  }
}

/********************************************************************/

static Ipp32s bsacdecLayerElement(Ipp32s layer,
                           Ipp32s *startIndexLowSpectra,
                           Ipp32s *endIndexLowSpectra,
                           Ipp32s *availableLen,
                           AACDec *state,
                           sBitsreamBuffer *pBS)
{
  sCpe_channel_element *cpe = &(state->com.m_cpe);
  BSACDec *bsState = state->bsState;
  Ipp32s numWindowGroups = cpe->streams[0].num_window_groups;
  Ipp32s numChannels = bsState->numChannels;
  Ipp32s startIndex[8], endIndex[8];
  Ipp32s decLen, group, thrSnf;
  Ipp32s i;

  /* layer_cband_si */
  if (bsacDecodingCBandSideInfo(layer, numChannels,
                                &decLen, bsState, pBS) < 0) {
    return -1;
  }
  *availableLen -= decLen;

  /* layer_sfb_si */
  if (bsacDecodingScalefactor(layer, &decLen, state, pBS) < 0) {
    return -1;
  }
  *availableLen -= decLen;

  /* bsac_layer_spectra */
  group = bsState->layerGroup[layer];
  startIndex[group] = bsState->layerStartIndex[layer];
  endIndex[group] = bsState->layerEndIndex[layer];

  if (layer < bsState->sLayerSize) {
    thrSnf = bsState->baseSnfThr;
  } else {
    thrSnf = 0;
    endIndexLowSpectra[group] = bsState->layerEndIndex[layer];
  }

  bsacDecodingSpectralData(group, group+1, startIndex, endIndex,
                           thrSnf, numChannels, availableLen, state, pBS);

  if (!bsState->sbaMode) {
    /* bsac_lower_spectra */

    bsacDecodingSpectralData(0, numWindowGroups, startIndexLowSpectra,
                             endIndexLowSpectra, 0, numChannels,
                             availableLen, state, pBS);

  } else if (bsState->terminalLayer[layer] == 1) {
    /* bsac_lower_spectra */

    bsacDecodingSpectralData(0, numWindowGroups, startIndexLowSpectra,
                             endIndexLowSpectra, 0, numChannels,
                             availableLen, state, pBS);

    /* bsac_higher_spectra */

    for (i = layer + 1; i < bsState->maxLayer; i++) {
      group = bsState->layerGroup[i];
      startIndex[group] = bsState->layerStartIndex[i];
      endIndex[group] = bsState->layerEndIndex[i];

      bsacDecodingSpectralData(group, group+1, startIndex, endIndex,
                               0, numChannels, availableLen, state, pBS);
    }
  }

  return 0;
}

/********************************************************************/

static Ipp32s bsacdecParseHeaders(sBitsreamBuffer *pBS,
                           Ipp32s          extended,
                           AACDec          *state)
{
  sCpe_channel_element *cpe = &(state->com.m_cpe);
  BSACDec              *bsState = state->bsState;
  Ipp32s numChannels = bsState->numChannels;
  Ipp32s i, tmp;

  GET_BITS(pBS, bsState->frameLength, 11, Ipp32s)

  if (bsState->frameLength == 0)
    return -1;

  if (extended == 1) { /* Extended element */
    GET_BITS(pBS, tmp, 3, Ipp32s)
    bsState->numChannels = (tmp & 1) + 1;
    GET_BITS(pBS, tmp, 1, Ipp32s)
  }

  /* bsac_header */
  GET_BITS(pBS, bsState->headerLength, 4, Ipp32s)
  GET_BITS(pBS, bsState->sbaMode, 1, Ipp32s)
  GET_BITS(pBS, bsState->topLayer, 6, Ipp32s)
  GET_BITS(pBS, bsState->baseSnfThr, 2, Ipp32s)

  for (i = 0; i < numChannels; i++) {
    GET_BITS(pBS, bsState->maxScalefactor[i], 8, Ipp32s)
  }

  GET_BITS(pBS, bsState->baseBand, 5, Ipp32s)

  bsState->maxSfbSiLen = 0;
  for (i = 0; i < numChannels; i++) {
    GET_BITS(pBS, bsState->cbandSiType[i], 5, Ipp32s)
    GET_BITS(pBS, bsState->bsaeScfModel[i], 3, Ipp32s)
    GET_BITS(pBS, bsState->enhScfModel[i], 3, Ipp32s)
    GET_BITS(pBS, tmp, 4, Ipp32s)
    bsState->maxSfbSiLen += (tmp + 5);
  }

  /* general_header */
  if (dec_ics_info(&(cpe->streams[0]), pBS, AOT_ER_BSAC) < 0) {
    return -1;
  }

  GET_BITS(pBS, bsState->pnsDataPresent, 1, Ipp32s)
  if (bsState->pnsDataPresent) {
    GET_BITS(pBS, bsState->pnsDataSfb, 6, Ipp32s)
  }

  if (numChannels == 2) {
    GET_BITS(pBS, cpe->ms_mask_present, 2, Ipp32s)
  } else {
    cpe->ms_mask_present = 0;
  }

  cpe->streams[1].num_windows = cpe->streams[0].num_windows;

  for (i = 0; i < numChannels; i++) {
    GET_BITS(pBS, cpe->streams[i].tns_data_present, 1, Ipp32s)
    if (cpe->streams[i].tns_data_present) {
      dec_tns_data(&(cpe->streams[i]), pBS);
    }

    GET_BITS(pBS, cpe->streams[i].ltp_data_present, 1, Ipp32s)
    if (cpe->streams[i].ltp_data_present) {
      dec_ltp_data(&(cpe->streams[i]), pBS, AOT_ER_BSAC);
    }
  }

  Byte_alignment(pBS);
  return 0;
}

/********************************************************************/

static void bsacdecInitHelpVariables(Ipp32s usedBits,
                              AACDec *state)
{
  BSACDec *bsState = state->bsState;
  s_SE_Individual_channel_stream *pData = &(state->com.m_cpe.streams[0]);
  Ipp32s endInd[8], endCBand[8], endSfb[8];
  Ipp32s layerSiMaxLen[BSAC_MAX_LAYER];
  Ipp32s *layerGroup = bsState->layerGroup;
  Ipp32s *layerStartCBand = bsState->layerStartCBand;
  Ipp32s *layerEndCBand = bsState->layerEndCBand;
  Ipp32s *layerStartIndex = bsState->layerStartIndex;
  Ipp32s *layerEndIndex = bsState->layerEndIndex;
  Ipp32s *layerStartSfb = bsState->layerStartSfb;
  Ipp32s *layerEndSfb = bsState->layerEndSfb;
  Ipp32s *layerBitOffset = bsState->layerBitOffset;
  Ipp32s *windowGroupLen = pData->len_window_group;
  Ipp32s numWindowGroups = pData->num_window_groups;
  Ipp32s windowSequence = pData->window_sequence;
  Ipp32s numChannels = bsState->numChannels;
  Ipp32s fs = state->com.m_sampling_frequency;
  Ipp32s maxSfb = pData->max_sfb;
  Ipp32s sLayerSize, layer, maxLayer;
  Ipp32s i, j;

  /* sLayerSize calculation */

  sLayerSize = 0;
  if (windowSequence == EIGHT_SHORT_SEQUENCE) {
    for (i = 0; i < numWindowGroups; i++) {
      endInd[i] = bsState->baseBand * 4 * windowGroupLen[i];
      if (fs >= 44100) {
        if ((endInd[i] & 0x1f) >= 16) {
          endInd[i] = (endInd[i] & ~0x1f) + 20;
        } else if ((endInd[i] & 0x1f) >= 4) {
          endInd[i] = (endInd[i] & ~0x1f) + 8;
        }
      } else if (fs >= 22050) {
        endInd[i] = (endInd[i] & ~0xf);
      } else if (fs >= 11025) {
        endInd[i] = (endInd[i] & ~0x1f);
      } else {
        endInd[i] = (endInd[i] & ~0x3f);
      }
      if (endInd[i] > bsState->sfbOffset[i][maxSfb]) {
        endInd[i] = bsState->sfbOffset[i][maxSfb];
      }
      endCBand[i] = (endInd[i] + 31) >> 5;
      sLayerSize += endCBand[i];
    }
  } else {
    endInd[0] = bsState->baseBand << 5;
    if (endInd[0] > bsState->sfbOffset[0][maxSfb]) {
      endInd[0] = bsState->sfbOffset[0][maxSfb];
    }
    endCBand[0] = (endInd[0] + 31) >> 5;
    sLayerSize = endCBand[0];
  }

  maxLayer = bsState->topLayer + sLayerSize;

  /* layerGroup calculation              */
  if (windowSequence == EIGHT_SHORT_SEQUENCE) {
    layer = sLayerSize;
    for (i = 0; i < numWindowGroups; i++) {
      for (j = 0; j < windowGroupLen[i]; j++) {
        layerGroup[layer] = i;
        layer++;
      }
    }

    for (layer = sLayerSize + 8; layer < maxLayer; layer++) {
      layerGroup[layer] = layerGroup[layer - 8];
    }
  } else {
    for (layer = sLayerSize; layer < maxLayer; layer++) {
      layerGroup[layer] = 0;
    }
  }

  /* layerStartCBand, layerEndCBand             */
  /* and layerEndIndex calculation              */
  layer = 0;
  for (i = 0; i < numWindowGroups; i++) {
    for (j = 0; j < endCBand[i]; j++) {
      layerGroup[layer] = i;
      layerStartCBand[layer] = j;
      layerEndCBand[layer] = j+1;
      layerStartIndex[layer] = j << 5;
      layerEndIndex[layer] = (j+1) << 5;
      if (layerEndIndex[layer] > endInd[i]) {
        layerEndIndex[layer] = endInd[i];
      }
      layer++;
    }

    if (layer > 0) {
      endCBand[i] = layerEndCBand[layer - 1];
      endInd[i] = layerEndIndex[layer - 1];
    } else {
      endCBand[i] = 0;
      endInd[i] = 0;
    }
  }

  for (layer = sLayerSize; layer < maxLayer; layer++) {
    i = layerGroup[layer];
    layerStartIndex[layer] = endInd[i];

    if (fs >= 44100) {
      if ((endInd[i] & 0x1f) == 0x18) {
        endInd[i] += 8;
      } else {
        endInd[i] += 12;
      }
    } else if (fs >= 22050) {
      endInd[i] += 16;
    } else if (fs >= 11025) {
      endInd[i] += 32;
    } else {
      endInd[i] += 64;
    }

    if (endInd[i] > bsState->sfbOffset[i][maxSfb]) {
      endInd[i] = bsState->sfbOffset[i][maxSfb];
    }

    layerEndIndex[layer] = endInd[i];
    layerStartCBand[layer] = endCBand[i];
    endCBand[i] = layerEndCBand[layer] = (endInd[i] + 31) >> 5;
  }

  for (layer = 0; layer < maxLayer - 1; layer++) {
    bsState->terminalLayer[layer] = 1;
    if ((layerGroup[layer] == layerGroup[layer+1]) &&
        (layerEndCBand[layer] == layerEndCBand[layer + 1])) {
      bsState->terminalLayer[layer] = 0;
    }
  }

  bsState->terminalLayer[maxLayer - 1] = 1;

  /* layerEndSfb calculation */

  for (i = 0; i < numWindowGroups; i++) {
    endSfb[i] = 0;
  }

  for (layer = 0; layer < maxLayer; layer++) {
    Ipp32s stopLayer = layer;
    while (bsState->terminalLayer[stopLayer] == 0) {
      stopLayer++;
    }
    i = layerGroup[layer];
    layerEndSfb[layer] = maxSfb;
    for (j = 0; j < maxSfb; j++) {
      if (layerEndIndex[stopLayer] <= bsState->sfbOffset[i][j + 1]) {
        layerEndSfb[layer] = j + 1;
        break;
      }
    }
    layerStartSfb[layer] = endSfb[i];
    endSfb[i] = layerEndSfb[layer];
  }

  if (!bsState->sbaMode) {
    for (layer = 0; layer < maxLayer; layer++) {
      bsState->terminalLayer[layer] = 0;
    }
  }

  /* availableLen[] calculation */

  for (layer = 0; layer < maxLayer; layer++) {
    layerSiMaxLen[layer] = bsState->maxSfbSiLen *
                          (layerEndSfb[layer] - layerStartSfb[layer]);

    for (i = 0; i < numChannels; i++) {
      Ipp32s maxCbandSiLen = bsacMaxCbandSiLenTbl[bsState->cbandSiType[i]];

      for (j = layerStartCBand[layer]; j < layerEndCBand[layer]; j++) {
        if (j == 0) {
          layerSiMaxLen[layer] += BSAC_MAX_CBAND0_SI_LEN;
        } else {
          layerSiMaxLen[layer] += maxCbandSiLen;
        }
      }
    }
  }

  for (layer = sLayerSize; layer <= maxLayer; layer++) {
    Ipp32s layerBitrate = ((layer - sLayerSize) * 1000 + 16000);
    layerBitOffset[layer] = (Ipp32s)((Ipp32f)layerBitrate * (Ipp32f)1024 /
                                     (Ipp32f)state->com.m_sampling_frequency);
    layerBitOffset[layer] = layerBitOffset[layer] & (~7);
    layerBitOffset[layer] *= numChannels;
  }

  if (layerBitOffset[maxLayer] < bsState->frameLength * 8) {
    layerBitOffset[maxLayer] = bsState->frameLength * 8;
  }

  for (layer = maxLayer - 1; layer >= sLayerSize; layer--) {
    Ipp32s bitOffset = layerBitOffset[layer + 1] - layerSiMaxLen[layer];
    if (bitOffset < layerBitOffset[layer]) {
      layerBitOffset[layer] = bitOffset;
    }
  }

  for (layer = sLayerSize - 1; layer >= 0; layer--) {
    layerBitOffset[layer] = layerBitOffset[layer + 1] - layerSiMaxLen[layer];
  }

  if (usedBits > layerBitOffset[0]) {
    Ipp32s bitOffset = usedBits - layerBitOffset[0];
    layerBitOffset[0] = usedBits;

    for (layer = maxLayer - 1; layer >= sLayerSize; layer--) {
      Ipp32s layerBitSize = layerBitOffset[layer + 1] - layerBitOffset[layer];
      layerBitSize -= layerSiMaxLen[layer];

      if (layerBitSize >= bitOffset) {
        layerBitSize = bitOffset;
        bitOffset = 0;
      } else {
        bitOffset -= layerBitSize;
      }

      for (i = 1; i <= layer; i++) {
        layerBitOffset[i] += layerBitSize;
      }

      if (bitOffset <= 0) {
        break;
      }
    }
  } else {
    Ipp32s bitOffset = layerBitOffset[0] - usedBits;
    Ipp32s sum, rem;

    if (sLayerSize != 0) {
      sum = bitOffset/sLayerSize;
      rem = bitOffset - sum * sLayerSize;
    } else {
      sum = rem = 0;
    }

    layerBitOffset[0] = usedBits;

    for (i = 1; i < sLayerSize; i++) {
      layerBitOffset[i] = layerBitOffset[i - 1] + layerSiMaxLen[i - 1] + sum;
      if (i <= rem) {
        layerBitOffset[i]++;
      }
    }
  }

  for (layer = 0; layer < maxLayer; layer++) {
    if (bsState->layerBitOffset[layer] > bsState->frameLength * 8) {
      bsState->layerBitOffset[layer] = bsState->frameLength * 8;
    }
  }

  bsState->sLayerSize = sLayerSize;
}

/********************************************************************/

static void bsacdeinterlieve(s_SE_Individual_channel_stream *pData,
                      Ipp32s *lenWindowGroupBuf,
                      Ipp32s numWindowGroups,
                      Ipp32s maxSfb)
{
  IPP_ALIGNED_ARRAY(32, Ipp16s, buf, 1024);
  Ipp16s *p_spectrum = pData->spectrum_data;
  Ipp16s *pBuf = buf;
  Ipp32s g, sfb, b, offset;

  ippsCopy_16s(p_spectrum, buf, 1024);

  for (g = 0; g < numWindowGroups; g++) {
    Ipp32s lenWindowGroup = lenWindowGroupBuf[g];
    Ipp32s offset = 0;
    for (sfb = 0; sfb < 32; sfb++) {
      for (b = 0; b < lenWindowGroup; b++) {
        ippsCopy_16s(pBuf, p_spectrum + b * 128 + offset, 4);
        pBuf += 4;
      }
      offset += 4;
    }
    p_spectrum += 128*lenWindowGroup;
  }

  offset = 7;
  for (g = numWindowGroups - 1; g >= 0; g--) {
    Ipp32s lenWindowGroup = lenWindowGroupBuf[g];
    for (b = 0; b < lenWindowGroup; b++) {
      ippsCopy_16s(pData->sf[g], pData->sf[offset], maxSfb);
      offset--;
    }
  }
}

/********************************************************************/

static void apply_bsacpns(Ipp32f *p_spectrumL,
                   Ipp32f *p_spectrumR,
                   AACDec *state)
{
  BSACDec *bsState = state->bsState;
  sCpe_channel_element *cpe = &(state->com.m_cpe);
  Ipp32f *p_spectrum = p_spectrumL;
  Ipp32s numChannels = bsState->numChannels;
  Ipp32s ch;

  for (ch = 0; ch < numChannels; ch++) {
    s_SE_Individual_channel_stream *pData = &(cpe->streams[ch]);
    Ipp32s *sfb_offset = pData->window_sequence == EIGHT_SHORT_SEQUENCE ?
                         pData->sfb_offset_short_window :
                         pData->sfb_offset_long_window;
    Ipp32s num_window_groups = pData->num_window_groups;
    Ipp32s max_sfb = pData->max_sfb;
    Ipp32f norm;
    Ipp32s i, g, sfb;

    for (g = 0; g < num_window_groups; g++) {
      Ipp32s len_window_group = pData->len_window_group[g];
      for (sfb = 0; sfb < max_sfb; sfb++) {
        if (bsState->pnsFlag[ch][g][sfb]) {
          if (bsState->pnsMode[g][sfb]) {
            norm = aacdec_noise_generator(p_spectrum + sfb_offset[sfb] * len_window_group,
                                         (sfb_offset[sfb + 1] - sfb_offset[sfb]) * len_window_group,
                                          &bsState->noiseSfbState[g][sfb]);
          } else {
            bsState->noiseSfbState[g][sfb] = state->com.noiseState;
            norm = aacdec_noise_generator(p_spectrum + sfb_offset[sfb] * len_window_group,
                                         (sfb_offset[sfb + 1] - sfb_offset[sfb]) * len_window_group,
                                          &state->com.noiseState);

          }

          norm *= (Ipp32f)pow(2.0, 0.25 * (pData->sf[g][sfb]));

          for (i = sfb_offset[sfb] * len_window_group;
                i < sfb_offset[sfb + 1]  * len_window_group; i++) {
            p_spectrum[i] *= norm;
          }
        }
        p_spectrum += sfb_offset[max_sfb] * len_window_group;
      }
    }

    p_spectrum = p_spectrumR;
  }
}

/********************************************************************/

#define COPY_TO_BUFFER                                                             \
{                                                                                  \
  if (endBitOffset > bsState->frameLength * 8) {                                   \
    Ipp32s bytesToZero = ((endBitOffset - bsState->frameLength * 8 + 7) >> 3) + 4; \
    if (bsState->frameLength > startByteOffset) {                                  \
      ippsCopy_8u(inPointer + startByteOffset, dstPtr,                             \
                bsState->frameLength - startByteOffset);                           \
      dstPtr += bsState->frameLength - startByteOffset;                            \
    }                                                                              \
    ippsZero_8u(dstPtr, bytesToZero);                                              \
    dstPtr += bytesToZero;                                                         \
  } else {                                                                         \
    Ipp32s bytesToCopy = (endBitOffset + 7) >> 3;                                  \
    Ipp8u mask = 8 - (Ipp8u)(endBitOffset & 7);                                    \
                                                                                   \
    if (mask == 8) {                                                               \
      mask = 0xff;                                                                 \
    } else {                                                                       \
      mask = (0xff >> mask) << mask;                                               \
    }                                                                              \
                                                                                   \
    if (bytesToCopy > startByteOffset) {                                           \
      ippsCopy_8u(inPointer + startByteOffset, dstPtr,                             \
                bytesToCopy - startByteOffset);                                    \
      dstPtr += bytesToCopy - startByteOffset;                                     \
    }                                                                              \
    dstPtr[-1] = dstPtr[-1] & mask;                                                \
    ippsZero_8u(dstPtr, 4);                                                        \
    dstPtr += 4;                                                                   \
  }                                                                                \
}

/********************************************************************/

static Ipp32s bsacdecRawDataBlock(Ipp8u  *inPointer,
                           Ipp32s inDataSize,
                           Ipp32s layersToDecode,
                           Ipp32s startCh,
                           Ipp32s extended,
                           Ipp32s *pUsedBits,
                           AACDec *state)
{
  BSACDec *bsState = state->bsState;
  sCpe_channel_element *cpe = &(state->com.m_cpe);
  s_SE_Individual_channel_stream *pData = &(cpe->streams[0]);
  sBitsreamBuffer BS;
  sBitsreamBuffer *pBS = &BS;
  Ipp8u *dstPtr;
  Ipp32s windowSequence;
  Ipp32s numWindowGroups;
  Ipp32s numChannels;
  Ipp32s startIndexLowSpectra[8];
  Ipp32s endIndexLowSpectra[8];
  Ipp32s layer, maxLayer, availableLen, usedBits;
  Ipp32s startByteOffset;
  Ipp32s startBitOffset, endBitOffset;
  Ipp32s startLayer;
  Ipp32s i, j, ch, offset, offsetData;

  GET_INIT_BITSTREAM(pBS, inPointer)

  if (bsacdecParseHeaders(pBS, extended, state) < 0) {
    return -1;
  }

  windowSequence = pData->window_sequence;
  numWindowGroups = pData->num_window_groups;
  numChannels = bsState->numChannels;

  if (extended == 0) { /* Not extended element */
    if (bsState->frameLength > inDataSize) {
      bsState->frameLength = inDataSize;
    }
  }

  if ((layersToDecode < 0) || (layersToDecode > bsState->topLayer)) {
    layersToDecode = bsState->topLayer;
  }

  /* SfbOffset recalculation */

  if (windowSequence == EIGHT_SHORT_SEQUENCE) {
    for (i = 0; i < numWindowGroups; i++) {
      for (j = 0; j <= pData->num_swb_short; j++) {
        bsState->sfbOffset[i][j] =
          pData->sfb_offset_short_window[j] *
          pData->len_window_group[i];
      }
    }
  } else {
    for (j = 0; j <= pData->num_swb_long; j++) {
      bsState->sfbOffset[0][j] =
        pData->sfb_offset_long_window[j];
    }
  }

  bsState->intensityDataPresent = 0;

  if (windowSequence == EIGHT_SHORT_SEQUENCE) {
    for (ch = 0; ch < numChannels; ch++) {
      for (i = 0; i < 8; i++) {
        ippsZero_16s(cpe->streams[ch].sf[i], pData->num_swb_short);
        ippsZero_32s(cpe->streams[ch].sfb_cb[i], pData->num_swb_short);
        ippsZero_32s(bsState->pnsFlag[ch][i], pData->num_swb_short);
      }
    }

    for (i = 0; i < 8; i++) {
      ippsZero_32s(cpe->ms_used[i], pData->num_swb_short);
      ippsZero_32s(bsState->pnsMode[i], pData->num_swb_short);
      ippsZero_32s(bsState->alreadyDecoded[i], pData->num_swb_short);
    }
  } else {
    for (ch = 0; ch < numChannels; ch++) {
      ippsZero_16s(cpe->streams[ch].sf[0], pData->max_sfb);
      ippsZero_32s(cpe->streams[ch].sfb_cb[0], pData->max_sfb);
      ippsZero_32s(bsState->pnsFlag[ch][0], pData->max_sfb);
    }

    ippsZero_32s(cpe->ms_used[0], pData->max_sfb);
    ippsZero_32s(bsState->pnsMode[0], pData->max_sfb);
    ippsZero_32s(bsState->alreadyDecoded[0], pData->max_sfb);
  }

  ippsZero_32s(bsState->curSnfBuf, numChannels * 1024);
  ippsZero_32s(bsState->signIsCodedBuf, numChannels * 1024);
  ippsZero_32s(bsState->higherBitVectorBuf, numChannels * 1024);
  ippsZero_32s(bsState->offsetHGFEBuf, numChannels * 256);
  ippsZero_32s(bsState->offsetBCDBuf, numChannels * 256);

  offset = 0;
  for (ch = 0; ch < numChannels; ch++) {
    ippsZero_16s(cpe->streams[ch].spectrum_data, 1024);
    offsetData = 0;
    for (i = 0; i < numWindowGroups; i++) {
      cpe->streams[ch].p_spectrum[i] = cpe->streams[ch].spectrum_data + offsetData;
      bsState->curSnf[ch][i] = bsState->curSnfBuf + offset;
      bsState->signIsCoded[ch][i] = bsState->signIsCodedBuf + offset;
      bsState->higherBitVector[ch][i] = bsState->higherBitVectorBuf + offset;
      bsState->offsetHGFE[ch][i] = bsState->offsetHGFEBuf + (offset >> 2);
      bsState->offsetBCD[ch][i] = bsState->offsetBCDBuf + (offset >> 2);
      offset += (pData->len_window_group[i]) << 7;
      offsetData += (pData->len_window_group[i]) << 7;
    }
    offset = 1024;
  }

  GET_BITS_COUNT(pBS, usedBits)

  bsacdecInitHelpVariables(usedBits, state);

  maxLayer = bsState->sLayerSize + layersToDecode;

  for (layer = maxLayer - 1; layer >= 0; layer--) {
    if (bsState->layerBitOffset[layer] > (inDataSize * 8)) {
      maxLayer--;
    } else {
      break;
    }
  }

  bsState->maxLayer = maxLayer;

  /* Copy input stream into internal buffer */

  if (!bsState->sbaMode) {
    startByteOffset = (usedBits>>3);
    /* bsState->topLayer + bsState->sLayerSize instead of maxLayer        */
    /* because there are more bits than bsState->layerBitOffset[maxLayer] */
    /* can be used                                                        */
    endBitOffset = bsState->layerBitOffset[bsState->topLayer + bsState->sLayerSize];
    dstPtr = (Ipp8u*)bsState->bitBuf;

    COPY_TO_BUFFER

  } else {
    startLayer = 0;
    startBitOffset = usedBits;
    layer = 0;
    dstPtr = (Ipp8u*)bsState->bitBuf;
    for (;;) {
      /* bsState->topLayer + bsState->sLayerSize instead of maxLayer        */
      /* because there are more bits than bsState->layerBitOffset[maxLayer] */
      /* can be used                                                        */

      for (layer = startLayer; layer < bsState->topLayer + bsState->sLayerSize; layer++) {
        if (bsState->terminalLayer[layer] == 1) {
          break;
        }
      }

      if (layer == bsState->topLayer + bsState->sLayerSize) {
        layer--;
      }

      endBitOffset = bsState->layerBitOffset[layer+1];

      startByteOffset = (startBitOffset >> 3);
      bsState->flashBits[startLayer] = startBitOffset & 7;

      bsState->layerBufferOffset[startLayer] = (Ipp32s)(dstPtr - (Ipp8u*)bsState->bitBuf);

      COPY_TO_BUFFER

      startLayer = layer+1;
      startBitOffset = endBitOffset;

      if (layer == (bsState->topLayer + bsState->sLayerSize - 1)) {
        break;
      }
    }
  }

  for (i = 0; i < numWindowGroups; i++) {
    startIndexLowSpectra[i] = 0;
  }

  if (windowSequence == EIGHT_SHORT_SEQUENCE) {
    for (i = 0; i < bsState->sLayerSize; i++) {
      endIndexLowSpectra[bsState->layerGroup[i]] =
        bsState->layerEndIndex[i];
    }
  } else {
    endIndexLowSpectra[0] =
      bsState->layerEndIndex[bsState->sLayerSize - 1];
  }

  /* Init */
  GET_INIT_BITSTREAM(pBS, (Ipp8u*)bsState->bitBuf)
  bsState->value = 0;
  bsState->range = 1;
  bsState->estCwLen = 30;
  availableLen = bsState->layerBitOffset[1] -
                 bsState->layerBitOffset[0];
  if (bsState->sbaMode == 1) {
    availableLen--;
  }

  if (bsacdecLayerElement(0, startIndexLowSpectra,
                          endIndexLowSpectra, &availableLen, state, pBS) < 0) {
    return -1;
  }

  for (layer = 1; layer < maxLayer; layer++) {
    if (bsState->terminalLayer[layer-1] == 1) {
      GET_INIT_BITSTREAM(pBS, (Ipp8u*)bsState->bitBuf +
                         bsState->layerBufferOffset[layer]);
      if (bsState->flashBits[layer] != 0) {
        Ipp32s tmp;
        GET_BITS(pBS, tmp, bsState->flashBits[layer], Ipp32s)
      }

      bsState->value = 0;
      bsState->range = 1;
      bsState->estCwLen = 30;
      availableLen = bsState->layerBitOffset[layer + 1] -
                     bsState->layerBitOffset[layer] - 1;

    } else {
      availableLen += bsState->layerBitOffset[layer + 1] -
                      bsState->layerBitOffset[layer];
    }

    if (bsacdecLayerElement(layer, startIndexLowSpectra,
                            endIndexLowSpectra, &availableLen, state, pBS) < 0) {
      return -1;
    }
  }

  GET_BITS_COUNT(pBS, usedBits)

  if ((windowSequence == EIGHT_SHORT_SEQUENCE) && (numWindowGroups != 8)) {

    for (ch = 0; ch < numChannels; ch++) {
      bsacdeinterlieve(&(cpe->streams[ch]), pData->len_window_group,
                         numWindowGroups, pData->max_sfb);
    }

    if ((cpe->ms_mask_present == 1) || (cpe->ms_mask_present == 3)) {
      /* ms_used deinterlieve */
      offset = 7;
      for (i = numWindowGroups - 1; i >= 0; i--) {
        Ipp32s lenWindowGroup = pData->len_window_group[i];
        for (j = 0; j < lenWindowGroup; j++) {
          ippsCopy_32s(cpe->ms_used[i], cpe->ms_used[offset], pData->max_sfb);
          offset--;
        }
      }
    }

    if (bsState->pnsDataPresent != 0) {
      /* pns deinterlieve */
      offset = 7;
      for (i = numWindowGroups - 1; i >= 0; i--) {
        Ipp32s lenWindowGroup = pData->len_window_group[i];
        for (j = 0; j < lenWindowGroup; j++) {
          ippsCopy_32s(bsState->pnsMode[i], bsState->pnsMode[offset],
                       pData->max_sfb);
          offset--;
        }
      }

      for (ch = 0; ch < numChannels; ch++) {
        offset = 7;
        for (i = numWindowGroups - 1; i >= 0; i--) {
          Ipp32s lenWindowGroup = pData->len_window_group[i];
          for (j = 0; j < lenWindowGroup; j++) {
            ippsCopy_32s(bsState->pnsFlag[ch][i], bsState->pnsFlag[ch][offset],
                         pData->max_sfb);
            offset--;
          }
        }
      }
    }

    if (bsState->intensityDataPresent != 0) {
      for (ch = 0; ch < numChannels; ch++) {
        offset = 7;
        for (i = numWindowGroups - 1; i >= 0; i--) {
          Ipp32s lenWindowGroup = pData->len_window_group[i];
          for (j = 0; j < lenWindowGroup; j++) {
            ippsCopy_32s(cpe->streams[ch].sfb_cb[i],
                         cpe->streams[ch].sfb_cb[offset], pData->max_sfb);
            offset--;
          }
        }
      }
    }

    cpe->streams[0].num_window_groups = 8;

    for (i = 0; i < 8; i++) {
      cpe->streams[0].len_window_group[i] = 1;
    }
  }

  if (windowSequence == EIGHT_SHORT_SEQUENCE) {
    cpe->streams[0].max_sfb = cpe->streams[0].num_swb_short;
  }

  cpe->streams[1].window_sequence = cpe->streams[0].window_sequence;
  cpe->streams[1].max_sfb = cpe->streams[0].max_sfb;

  cpe->streams[1].num_window_groups = cpe->streams[0].num_window_groups;

  for (i = 0; i < cpe->streams[0].num_window_groups; i++) {
    cpe->streams[1].len_window_group[i] =
      cpe->streams[0].len_window_group[i];
  }

  for (ch = 0; ch < numChannels; ch++) {
    ics_apply_scale_factors(&(cpe->streams[ch]),
                            state->m_spectrum_data[startCh + ch]);
  }

  if (cpe->ms_mask_present > 0) {
    cpe_apply_ms(cpe, state->m_spectrum_data[startCh + 0],
                 state->m_spectrum_data[startCh + 1]);
  }

  if (bsState->pnsDataPresent != 0) {
    apply_bsacpns(state->m_spectrum_data[startCh + 0],
                  state->m_spectrum_data[startCh + 1], state);
  }

  if (bsState->intensityDataPresent != 0) {
    cpe_apply_intensity(cpe, state->m_spectrum_data[startCh + 0],
                        state->m_spectrum_data[startCh + 1]);
  }

  for (ch = 0; ch < numChannels; ch++) {
    ics_calc_tns_data(&(cpe->streams[ch]), &(state->tns_data[startCh + ch]));

#ifdef USE_LTP_IN_BSAC

    state->m_ltp.p_samples_1st_part = state->m_ltp_buf[startCh + ch][state->com.m_index_1st];
    state->m_ltp.p_samples_2nd_part = state->m_ltp_buf[startCh + ch][state->com.m_index_2nd];
    state->m_ltp.p_samples_3rd_part = state->m_ltp_buf[startCh + ch][state->com.m_index_3rd];
    state->m_ltp.prev_windows_shape = state->com.m_prev_win_shape[startCh + ch];

    state->m_ltp.p_filterbank_data = &(state->m_filterbank);
    state->m_ltp.p_tns_data = &(state->tns_data[startCh + ch]);

    ics_apply_ltp_I(&(state->m_ltp), &(cpe->streams[ch]),
                    state->m_spectrum_data[startCh + ch]);
#endif
  }

  for (ch = 0; ch < numChannels; ch++) {
    state->com.m_curr_win_shape[startCh + ch] = cpe->streams[0].window_shape;
    state->com.m_curr_win_sequence[startCh + ch] = cpe->streams[0].window_sequence;
  }

  if (extended == 0) { /* Not extended element */
    /* Because I don't have multichannel's test vectors let ... */
    usedBits = bsState->frameLength * 8;
    *pUsedBits = usedBits;
  } else {
    *pUsedBits = bsState->frameLength * 8;
  }

  return 0;
}

/********************************************************************/

AACStatus bsacdecGetFrame(Ipp8u    *inPointer,
                          Ipp32s   inDataSize,
                          Ipp32s   layersToDecode,
                          AACDec   *state)
{
  BSACDec *bsState = state->bsState;
  Ipp32s  usedBits, availableFrameData;

  if (!inPointer)
    return AAC_NULL_PTR;

  /* for first non extended element */
  bsState->numChannels = state->com.m_channel_number;
  if (bsState->numChannels > 2) {
    bsState->numChannels = 2;
  }

  if (bsacdecRawDataBlock(inPointer, inDataSize, layersToDecode, 0,
                          0, &usedBits, state) < 0) {
    return AAC_BAD_STREAM;
  }

  state->com.m_channel_number = bsState->numChannels;

  if ((layersToDecode < 0) || (layersToDecode > bsState->topLayer)) {
    layersToDecode = bsState->topLayer;
  }

  availableFrameData = bsState->frameLength * 8;

  if (usedBits < availableFrameData) {
    Ipp32s zeroCode, syncWord;

    /* Synchronization */
    if (layersToDecode == bsState->topLayer) {
      zeroCode = 0;
      syncWord = 0;
    } else {
      zeroCode = 0xffff;
      syncWord = 0xf;
    }

    while (usedBits < availableFrameData) {
      zeroCode = (zeroCode << 8) + syncWord;
      syncWord = inPointer[usedBits >> 3];
      usedBits += 8;

      if ((zeroCode == 0) && (syncWord >= 0xfe)) {
        break;
      }
    }

    availableFrameData -= usedBits;

    while (availableFrameData > 0) {
      Ipp32s tmpUsedBits;

      if (bsacdecRawDataBlock(inPointer + (usedBits >> 3), inDataSize,
                              layersToDecode, state->com.m_channel_number,
                              1, &tmpUsedBits, state) < 0) {
          return AAC_BAD_STREAM;
      }

      availableFrameData -= tmpUsedBits;
      usedBits += tmpUsedBits;

      if (availableFrameData >= -32) {
        state->com.m_channel_number += bsState->numChannels;
      }
    }
  }

  return AAC_OK;
}
/********************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_DECODER


