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
#include "aac_status.h"

#include "ipps.h"

/********************************************************************/

static const Ipp32s alsParcorTable[] =
{
  -1048544, -1048288, -1047776, -1047008, -1045984, -1044704, -1043168, -1041376,
  -1039328, -1037024, -1034464, -1031648, -1028576, -1025248, -1021664, -1017824,
  -1013728, -1009376, -1004768,  -999904,  -994784,  -989408,  -983776,  -977888,
   -971744,  -965344,  -958688,  -951776,  -944608,  -937184,  -929504,  -921568,
   -913376,  -904928,  -896224,  -887264,  -878048,  -868576,  -858848,  -848864,
   -838624,  -828128,  -817376,  -806368,  -795104,  -783584,  -771808,  -759776,
   -747488,  -734944,  -722144,  -709088,  -695776,  -682208,  -668384,  -654304,
   -639968,  -625376,  -610528,  -595424,  -580064,  -564448,  -548576,  -532448,
   -516064,  -499424,  -482528,  -465376,  -447968,  -430304,  -412384,  -394208,
   -375776,  -357088,  -338144,  -318944,  -299488,  -279776,  -259808,  -239584,
   -219104,  -198368,  -177376,  -156128,  -134624,  -112864,   -90848,   -68576,
    -46048,   -23264,     -224,    23072,    46624,    70432,    94496,   118816,
    143392,   168224,   193312,   218656,   244256,   270112,   296224,   322592,
    349216,   376096,   403232,   430624,   458272,   486176,   514336,   542752,
    571424,   600352,   629536,   658976,   688672,   718624,   748832,   779296,
    810016,   840992,   872224,   903712,   935456,   967456,   999712,  1032224
};

/********************************************************************/

static void alsPredictionFilter(alsBlockState *state)
{
  Ipp32s *parcor = state->parcor;
  Ipp32s *lpc = state->lpc;
  Ipp32s *res = state->res;
  Ipp32s *x = state->x;
  Ipp64s tmp0, tmp1, y;
  Ipp32s order = state->optOrder;
  Ipp32s len = state->blockLength;
  Ipp32s ra = state->raBlock;
  Ipp32s m, i, j;

  if ((ra) && (order > len)) {
    order = len;
  }

  /* Reconstruction of the parcor coefficients */
  if (order > 0) {
    parcor[0] = alsParcorTable[parcor[0] + 64];
  }
  if (order > 1) {
    parcor[1] = -alsParcorTable[parcor[1] + 64];
  }
  for (i = 2; i < order; i++) {
    parcor[i] = (parcor[i] << 14) + 8192;
  }

  if (!ra) {
    for (m = 1; m <= order; m++) {
      for (j = 1; j <= m/2; j++) {
        tmp0 = lpc[j-1] +   ((((Ipp64s)parcor[m-1] * lpc[m-j-1]) + 0x80000) >> 20);
        tmp1 = lpc[m-j-1] + ((((Ipp64s)parcor[m-1] * lpc[j-1])   + 0x80000) >> 20);
        lpc[m-j-1] = (Ipp32s)tmp1;
        lpc[j-1]   = (Ipp32s)tmp0;

      }
      lpc[m-1] = parcor[m-1];
    }

    for (i = 0; i < len; i++) {
      y = 0x80000;
      for (j = 1; j <= order; j++) {
        y += (Ipp64s)lpc[j-1] * x[i-j];
      }
      x[i] = res[i] - (Ipp32s)(y >> 20);
    }
  } else {
    for (i = 0; i < order; i++) {
      y = 0x80000;
      for (j = 1; j <= i; j++) {
        y += (Ipp64s)lpc[j-1] * x[i-j];
      }

      x[i] = res[i] - (Ipp32s)(y >> 20);

      m = i+1;

      for (j = 1; j <= m/2; j++) {
        tmp0 = lpc[j-1] +   ((((Ipp64s)parcor[m-1] * lpc[m-j-1]) + 0x80000) >> 20);
        tmp1 = lpc[m-j-1] + ((((Ipp64s)parcor[m-1] * lpc[j-1])   + 0x80000) >> 20);
        lpc[m-j-1] = (Ipp32s)tmp1;
        lpc[j-1]   = (Ipp32s)tmp0;
      }
      lpc[m-1] = parcor[m-1];
    }

    for (i = order; i < len; i++) {
      y = 0x80000;
      for (j = 1; j <= order; j++) {
        y += (Ipp64s)lpc[j-1] * x[i-j];
      }
      x[i] = res[i] - (Ipp32s)(y >> 20);
    }
  }
}

/********************************************************************/

static void alsLTPReconstruct(alsBlockState *state)
{
  Ipp32s *gain = state->LTPgain;
  Ipp32s *res = state->res;
  Ipp32s len = state->blockLength;
  Ipp32s lag = state->LTPlag;
  Ipp64s y;
  Ipp32s *ptr;
  Ipp32s i, j;

  if (state->optOrder < 4) {
    lag += 4;
  } else {
    lag += state->optOrder + 1;
  }

  ptr = res - lag;

  for (i = 0; i < len; i++) {
    y = 64;
    for (j = -2; j < 3; j++) {
      y += (Ipp64s)gain[j + 2] * ptr[i + j];
    }
    res[i] += (Ipp32s)(y >> 7);
  }
}

/********************************************************************/
#ifndef LONG_MAX
#define LONG_MAX 0x7fffffff
#endif

#ifndef LONG_MIN
#define LONG_MIN 0x80000000
#endif

#define TOTAL_FILTER_LEN 1024 * 8
#define PFACTOR 115292150460684LL /* 0.0001 in Q60 */
#define ROUND1(x) (((Ipp32s)((x)+8))>>4)
#define ROUND2(x) ((Ipp64s)(((Ipp64s)(x)) + 1)>>1)

/********************************************************************/

static void alsReinitP(ALSDec         *state,
                alsRLSLMSState *rlState)
{
  Ipp32s i;

  for (i = 0; i < state->RLSFilterOrder*state->RLSFilterOrder; i++) {
    rlState->P[i] = 0;
  }

  for (i = 0; i < state->RLSFilterOrder; i++) {
    rlState->P[i * state->RLSFilterOrder + i] = (Ipp64s)(PFACTOR);
  }
}

/********************************************************************/

static void alsRLSLMSInit(ALSDec         *state,
                   alsRLSLMSState *rlState)
{
  Ipp32s j;

  ippsZero_32s(rlState->RLSFilterWeight, state->RLSFilterOrder);
  ippsZero_32s(rlState->RLSBuf, state->RLSFilterOrder);

  for (j = 0; j < state->LMSStage; j++) {
    ippsZero_32s(rlState->LMSFilter[j].filterWeight, state->LMSFilterOrder[j]);
    ippsZero_32s(rlState->LMSFilter[j].buf, state->LMSFilterOrder[j]);
  }

  ippsSet_32s((Ipp32s)1 << 24, rlState->wFinal, state->LMSStage + 2);

  rlState->DPCMBuf[0] = 0;

  alsReinitP(state, rlState);
}

/********************************************************************/

static Ipp32s alsGetPredictor(Ipp32s *w,
                       Ipp32s *x,
                       Ipp32s order,
                       Ipp32s scaleFac)
{
  Ipp64s y = 0;
  Ipp32s i;

  for (i = 0; i < order; i++) {
    y += (Ipp64s)w[i] * x[i];
  }

  y >>= scaleFac;
  if (scaleFac == 12) {
    if (y > 0x3fffffffLL)
      y = 0x3fffffffLL;

    if (y < -0x40000000LL)
      y = -0x40000000LL;
  } else {
    if (y > 0x7ffffffLL)
      y = 0x7ffffffLL;

    if (y < -0x7ffffffLL)
      y = -0x7ffffffLL;
  }

  return (Ipp32s)y;
}

/********************************************************************/

static Ipp32s alsTotPrediction(Ipp32s *wFinal,
                        Ipp32s *pred,
                        Ipp32s res,
                        Ipp32s order,
                        Ipp32s LMSStepsize)
{
  Ipp32s x, i;
  Ipp64s y, err;

  y = 0;

  for (i = 0; i < order; i++) {
    y += (Ipp64s)wFinal[i]* pred[i];
  }

  y >>= 24;
  x = res + ROUND1(y);
  err = (((Ipp64s)x) << 4) - y;

  if (err > 0) {
    for (i = 2; i < order; i++) {
      if (pred[i] >= 0) {
        if (wFinal[i] < LONG_MAX) {
          wFinal[i] = (Ipp32s)((Ipp64s)wFinal[i] + LMSStepsize);
        }
      } else {
        if (wFinal[i] > LONG_MIN) {
          wFinal[i] = (Ipp32s)((Ipp64s)wFinal[i] - LMSStepsize);
        }
      }
    }
  } else if (err < 0) {
    for (i = 2; i < order; i++) {
      if (pred[i] <= 0) {
        if (wFinal[i] < LONG_MAX) {
          wFinal[i] = (Ipp32s)((Ipp64s)wFinal[i] + LMSStepsize);
        }
      } else {
        if (wFinal[i] > LONG_MIN) {
          wFinal[i] = (Ipp32s)((Ipp64s)wFinal[i] - LMSStepsize);
        }
      }
    }
  }
  return x;
}

/********************************************************************/

static Ipp32s alsFastBitcount(Ipp64s n)
{
  Ipp32u k;
  Ipp32s i;

  if (n == 0 ) {
    i = 0;
  } else {
    if (n >= (Ipp64s)0x100000000LL) {
      k = (Ipp32u)(n >> 32);
      i = 32;
    } else {
      k = (Ipp32u)(n);
      i = 0;
    }

    if (k >= 0x10000) {
      if (k >= 0x1000000) {
        k >>= 24;
        i += 24;
      } else {
        k >>= 16;
        i += 16;
      }
    } else {
      if (k >= 0x100) {
        k >>= 8;
        i += 8;
      }
    }

    i += alsLogTab[k];
  }
  return (i);
}

/********************************************************************/

static void MulMtxVec(Ipp64s *P,
               Ipp32s *x,
               Ipp32s  M,
               Ipp32s *yi,
               Ipp32s *vscale)

{
  Ipp64s ya[256];
  Ipp64s *ptr;
  Ipp64s imax, htemp1, ttemp1;
  Ipp32s i, j, pscale, nscale;

  *vscale = 0;
  imax = 0;
  htemp1 = 0;

  for (i = 0; i < M; i++) {
    ptr = P;
    ptr += i*M;
    for (j = 0; j <= i; j++) {
      htemp1 |= (*ptr > 0 ? *ptr : -*ptr);
      ptr++;
    }
  }

  pscale = 63 - alsFastBitcount(htemp1);
  ttemp1 = 0;

  for (i = 0; i < M; i++) {
    ptr = P;
    ptr += i*M;
    ya[i]=0;

    for (j = 0; j < M; j++) {
      ya[i] += (Ipp64s)(((*ptr++ << pscale) + (Ipp64s)0x80000000LL) >> 32) * x[j];
    }
    ttemp1 |= (ya[i] > 0 ? ya[i]:-ya[i]);
  }

  nscale = alsFastBitcount(ttemp1);
  if (nscale > 28) {
    nscale -= 28;
    for (i = 0; i < M; i++) {
      ya[i] >>= nscale;
      yi[i] = (Ipp32s)ya[i];
    }
    *vscale = nscale-pscale;
  } else {
    nscale -=28;
    for (i = 0; i < M; i++) {
      yi[i] = (Ipp32s)ya[i];
    }
    *vscale = -pscale;
  }
}

/********************************************************************/

static Ipp64s MulVecVec(Ipp32s *x,
                 Ipp32s *y,
                 Ipp32s  M,
                 Ipp32s *scale)

{
  Ipp32s i;
  Ipp64s z, zh, temp;

  *scale = 0;
  zh = 0;
  for (i = 0; i < M; i++) {
    zh += (Ipp64s)y[i] * x[i];
  }

  temp = zh ;
  if (temp < 0)
    temp = -temp;

  *scale = alsFastBitcount(temp);

  if (*scale > 28) {
    *scale -= 28;
    z = (zh << (32 - (*scale - 1)));
    z = ROUND2(z);
  } else {
    z = (zh << 32);
  }

  return(z);
}

/********************************************************************/

static void alsUpdateRLSFilter(Ipp32s *x,
                        Ipp32s  y,
                        ALSDec         *state,
                        alsRLSLMSState *rlState,
                        Ipp32s *bufl,
                        Ipp32s lambda)
{
  Ipp64s k[256];
  Ipp32s vl[256];
  Ipp32s *w = rlState->RLSFilterWeight;
  Ipp64s *P = rlState->P;
  Ipp32s order = state->RLSFilterOrder;
  Ipp32s i, j, shift, vscale, dscale;
  Ipp32s lr, e, shifted_e;
  Ipp64s wtemp, wtemp2;
  Ipp64s htemp, ir, htemp1, htemp2;

  if (order == 0)
    return;

  e = (*x-y);

  MulMtxVec(P, bufl, order, vl, &vscale);
  wtemp = MulVecVec(bufl, vl, order, &dscale);

  i = 0;

  while (wtemp > LONG_MAX/4 && wtemp!=0) {
    wtemp >>= 1;
    i++;
  }

  i += vscale + dscale;

  if (i <= 60) {
    wtemp += ((Ipp64s)1 << (60-i));
  } else {
    alsReinitP(state, rlState);
  }

  wtemp2 = wtemp;

  if (wtemp == 0) {
    ir = 1 << 30;
  //} else if ((90 - i) > 62) {
  } else if (i < 28) {
    shift = 28 - i;
    ir = ((Ipp64s)1 << 62) / wtemp2;

    if (shift > 32) {
      ir = 1 << 30;
    } else if (shift >= 0) {
      ir <<= shift;
    }
  } else {
    //if ((90 - i) > 32) {
    //if (i < 58) {
      ir = ((Ipp64s)1 << (90 - i)) / wtemp2;
    //}
  }

  lr = (Ipp32s)ir;
  htemp1 = 0;

  for (i = 0; i < order; i++) {
    htemp = (Ipp64s)vl[i] * lr;

    if (vscale >= 12) {
      k[i] = htemp << (vscale - 12);
      k[i] = ROUND2(k[i]);
    } else {
      k[i] = htemp >> (11 - vscale);
      k[i] = ROUND2(k[i]) ;
    }
    htemp1 |= (k[i] > 0 ? k[i]: -k[i]);
  }

  dscale = alsFastBitcount(htemp1);

  if (dscale > 30) {
    dscale -= 30;
    for (i = 0; i < order; i++) {
      k[i] >>= dscale;
    }
  } else {
    dscale = 0;
  }

  shifted_e = e >> 3;
  for (i = 0; i < order; i++) {
    htemp1 = (Ipp64s)k[i] * shifted_e;
    htemp = (htemp1 >> (30 - dscale));
    wtemp = w[i] + ROUND2(htemp);
    w[i] = (Ipp32s)wtemp;
  }
  vscale += dscale;

  for (i = 0; i < order; i++) {
    for (j = 0; j <= i; j++) {
      htemp2 = (Ipp64s)k[i] * vl[j];
      wtemp = htemp2>>(14-vscale);
      P[i*order+j] -= wtemp;

      if (P[i*order+j] >= (Ipp64s)0x3fffffffffffffffLL) {
        alsReinitP(state, rlState);
        break;
      }

      if (P[i*order+j] <= (Ipp64s)0xC000000000000000LL) {
        alsReinitP(state, rlState);
        break;
      }

      wtemp = P[i*order+j]/lambda;
      P[i*order+j] += wtemp;
    }
  }

  for (i = 1; i < order; i++) {
    for (j = 0; j < i; j++) {
      P[j*order+i] = P[i*order+j];
    }
  }

  /* shift buffer */
  j = order - 1;
  while (j-- > 0) {
    *bufl = *(bufl + 1);
    bufl++;
  }
  *bufl = (*x >> 4);

  *x = (Ipp32s)e;
}

/********************************************************************/

static void alsUpdateLMSFilter(Ipp32s *x,
                        Ipp32s y,
                        Ipp32s order,
                        Ipp32s mu,
                        alsLMSState *lsmState)
{
  Ipp32s *buf = lsmState->buf;
  Ipp32s *w = lsmState->filterWeight;
  Ipp32s i, j;
  Ipp64s fact, pow;
  Ipp64s wtemp, e, wtemp1;
  Ipp32s temp;

  e = (*x - y);

  pow = 0;

  for (i = 0; i < order; i++) {
    pow += (Ipp64s)buf[i] * buf[i];
  }

  wtemp1 = wtemp = ((Ipp64s) mu * (pow >> 7));

  i = 0;
  while(wtemp > LONG_MAX) {
    wtemp >>= 1;
    i++;
  }

  fact = ((Ipp64s)e << (29 - i)) / ((Ipp64s)((wtemp1 + 1) >> i));

  for (j = 0; j < order; j++) {
    w[j] = w[j] + (Ipp32s)(((Ipp64s)buf[j] * fact + 0x8000) >> 16);
  }

  temp = (*x) >> 4;

  /* shift buffer */
  j = order - 1;
  while (j-- > 0) {
    *buf = *(buf + 1);
    buf++;
  }
  *buf = temp;

  *x = (Ipp32s)e;
}

/********************************************************************/

static Ipp32s alsRLSSynthesizeChannels(ALSDec         *state,
                                alsBlockState  **blockState,
                                alsRLSLMSState **rlState,
                                Ipp32s         numCh)
{
  Ipp32s err[1], pred[10];
  Ipp32s ra = blockState[0]->raBlock;
  Ipp32s i, j, x, ch, lambda;

  if (state->RLSExtensionBits == 7) {
    ra = 1;
  }

  lambda = state->RLSLambda;
  if (ra) {
    lambda = state->RLSLambdaRA;
  }

  for (i = 0; i < blockState[0]->blockLength; i++) {
    for (ch = 0; ch < numCh; ch++) {
      pred[0] = (rlState[ch]->DPCMBuf[0] << 4);
      pred[1] = alsGetPredictor(rlState[ch]->RLSFilterWeight,
                                rlState[0]->RLSBuf,
                                state->RLSFilterOrder, 12);

      for (j = 0; j < state->LMSStage; j++) {
        pred[j+2] = alsGetPredictor(rlState[ch]->LMSFilter[j].filterWeight,
                                    rlState[ch]->LMSFilter[j].buf,
                                    state->LMSFilterOrder[j], 20);
      }

      x = alsTotPrediction(rlState[ch]->wFinal, pred, blockState[ch]->res[i],
                           state->LMSStage+2, state->LMSStepSize);

      blockState[ch]->x[i] = rlState[ch]->DPCMBuf[0] = x;
      err[0] = (x << 4) - pred[0];

      if (i > 300) {
        lambda = state->RLSLambda;
      }

      if (lambda <= 0) {
        return -1;
      }

      alsUpdateRLSFilter(err, pred[1], state, rlState[ch],
                         rlState[0]->RLSBuf, lambda);

      if ((!ra) || (ra && (i > (blockState[ch]->blockLength >> 5)))) {
        for (j = 0; j < state->LMSStage; j++) {
          alsUpdateLMSFilter(err, pred[j+2], state->LMSFilterOrder[j],
                             state->LMSFilterMu[j], &(rlState[ch]->LMSFilter[j]));
        }
      }
    }
  }

  return 0;
}

/********************************************************************/

Ipp32s alsRLSSynthesize(ALSDec         *state,
                        alsBlockState  *blockState,
                        alsRLSLMSState *rlState)
{

  if ((blockState->raBlock == 1) || (state->RLSExtensionBits == 7)) {
    alsRLSLMSInit(state, rlState);
  }

  if (blockState->blockType == 0) {
    return 0;
  }

  if (alsRLSSynthesizeChannels(state, &blockState, &rlState, 1) < 0) {
    return -1;
  }

  return 0;
}

/********************************************************************/

Ipp32s alsRLSSynthesizeJoint(ALSDec         *state,
                             alsBlockState  **blockState,
                             alsRLSLMSState **rlState)
{
  if ((blockState[0]->raBlock == 1) || (state->RLSExtensionBits == 7)) {
    alsRLSLMSInit(state, rlState[0]);
    alsRLSLMSInit(state, rlState[1]);
  }

  if ((blockState[0]->blockType == 0) &&
      (blockState[1]->blockType == 0)) {
    alsReinitP(state, rlState[0]);
    alsReinitP(state, rlState[1]);
    return 0;
  }

  if (state->RLSMonoBlock != state->lastRLSMonoBlock) {
    alsReinitP(state, rlState[0]);
    alsReinitP(state, rlState[1]);
  }

  if (state->RLSMonoBlock == 1) {
    if (alsRLSSynthesizeChannels(state, blockState, rlState, 1) < 0) {
      return -1;
    }
    ippsAdd_32s_Sfs(blockState[0]->x, blockState[1]->res,
                    blockState[1]->x, blockState[0]->blockLength, 0);
  } else {
    if (alsRLSSynthesizeChannels(state, blockState, rlState, 2) < 0) {
      return -1;
    }
  }

  state->lastRLSMonoBlock = state->RLSMonoBlock;

  return 0;
}

/********************************************************************/

void alsReconstructData(ALSDec      *state,
                        alsBlockState *blockState)
{
  if (blockState->blockType == 0) {
    if (blockState->constBlock == 0) {
      ippsZero_32s(blockState->x, blockState->blockLength);
    } else {
      ippsSet_32s(blockState->constVal, blockState->x,
                  blockState->blockLength);
    }
  } else {

    if (!state->RLSLMS) {
      if (blockState->shiftPos != 0) {
        ippsCopy_32s(blockState->x - blockState->optOrder,
                     blockState->xSaved, blockState->optOrder);

        ippsRShiftC_32s_I(blockState->shiftPos,
                          blockState->x - blockState->optOrder,
                          blockState->optOrder);
      }
    }

    if (state->LongTermPrediction) {
      if (blockState->LTPenable) {
        alsLTPReconstruct(blockState);
      }
    }

    if (!state->RLSLMS) {
      alsPredictionFilter(blockState);

      if (blockState->shiftPos != 0) {
        ippsCopy_32s(blockState->xSaved,
                     blockState->x - blockState->optOrder,
                     blockState->optOrder);

        ippsLShiftC_32s_I(blockState->shiftPos, blockState->x,
                          blockState->blockLength);
      }
    } else {
      ippsCopy_32s(blockState->res, blockState->x,
                   blockState->blockLength);

      if (blockState->shiftPos != 0) {
        Ipp32s i;
        Ipp32s mask = (1 << blockState->shiftPos) - 1;

        for (i = -blockState->optOrder; i < 0; i++) {
          blockState->x[i] &= ~mask; /* RD ?????????? */
        }

        ippsLShiftC_32s_I(blockState->shiftPos, blockState->x,
                          blockState->blockLength);
      }
    }
  }
}

/********************************************************************/

#endif //UMC_ENABLE_AAC_AUDIO_DECODER




