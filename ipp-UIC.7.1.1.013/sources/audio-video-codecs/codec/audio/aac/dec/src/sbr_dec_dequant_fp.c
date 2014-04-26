/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"

#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

#include <math.h>
#include "sbr_settings.h"
#include "aac_dec_sbr_fp.h"

/********************************************************************/

#ifndef ID_SCE
#define ID_SCE    0x0
#endif

#ifndef ID_CPE
#define ID_CPE    0x1
#endif

#ifndef SBR_ENV_PROTECT
#define SBR_ENV_PROTECT( data )   IPP_MAX( IPP_MIN(data, 128), 0 )
#endif

/********************************************************************/

/* SBR_TABLE_DEQUANT_INVERT[i] = 2^1 / (1 + 2^(12 - i)), FP */
static const Ipp32f SBR_TABLE_DEQUANT_INVERT[25] = {
  0.000488162040710f,
  0.000976085662842f,
  0.001951219514012f,
  0.003898635506630f,
  0.007782101631165f,
  0.015503875911236f,
  0.030769230797887f,
  0.060606060549617f,
  0.117647059261799f,
  0.222222222015262f,
  0.400000000372529f,
  0.666666666977108f,
  1.000000000000000f,
  1.333333333022893f,
  1.599999999627471f,
  1.777777777984738f,
  1.882352940738201f,
  1.939393939450383f,
  1.969230769202113f,
  1.984496124088764f,
  1.992217898368835f,
  1.996101364493370f,
  1.998048780485988f,
  1.999023914337158f,
  1.999511837959290f
};

/********************************************************************/

static Ipp32s sbrDequantInvertCouple(Ipp32f *pSrcDstL, Ipp16s *pSrcR, Ipp32f *pDstR,
                              Ipp32s len, Ipp32s shift)
{
  Ipp16s  expR;
  Ipp32s  i;

  for (i = 0; i < len; i++) {

    expR = (Ipp16s)(pSrcR[i] >> shift);
/*
 * confine expR [0, 24]
 */
    if (expR < 0)
      expR = 0;
    if (expR > 24)
      expR = 24;

    pDstR[i] = pSrcDstL[i] * SBR_TABLE_DEQUANT_INVERT[24 - expR];
    pSrcDstL[i] = pSrcDstL[i] * SBR_TABLE_DEQUANT_INVERT[expR];
  }

  return 0;     // OK
}

/********************************************************************/

static Ipp32s sbrDequantNoiseUncouple(Ipp32f *pSrcDstL, Ipp16s *pSrcR, Ipp32f *pDstR,
                               Ipp32s len)
{

  sbrDequantInvertCouple(pSrcDstL, pSrcR, pDstR, len, 0);

  return 0;     // OK
}

/********************************************************************/

static Ipp32s sbrDequantEnvUncouple(Ipp32f *pSrcDstL, Ipp16s *pSrcR, Ipp32f *pDstR,
                             Ipp32s len, Ipp32s ampRes)
{

  Ipp32s  shift = 1 - ampRes;

  sbrDequantInvertCouple(pSrcDstL, pSrcR, pDstR, len, shift);

  return 0;     // OK
}

/********************************************************************/

static Ipp32s sbrDequantNoiseMono(Ipp16s *pSrc, Ipp32f *pDst, Ipp32s len)
{
  Ipp32s  l;

  for (l = 0; l < len; l++) {
    pDst[l] = (Ipp32f)pow(2.f, (Ipp32f)(SBR_DEQUANT_OFFSET - pSrc[l]));
  }

  return 0;
}

/********************************************************************/

static Ipp32s sbrDequantEnvMono(Ipp16s *pSrc, Ipp32f *pDst, Ipp32s len,
                                Ipp32s bs_amp_res)
{
  Ipp32s  l;
  Ipp32f  a = (bs_amp_res == 1) ? 1.0f : 0.5f;
  Ipp32f  deg = 0.f;

  if (len <= 0)
    return 0;

  for (l = 0; l < len; l++) {
    deg = SBR_ENV_PROTECT( pSrc[l] ) * a + SBR_DEQUANT_OFFSET;
    pDst[l] = (Ipp32f)pow(2.f, deg);
  }

  return 0;     // OK
}

/********************************************************************/

Ipp32s sbrDequantization(sSBRDecComState * pSbrCom, sSBRDecWorkState * pSbrWS,
                         Ipp32s ch, Ipp32s bs_amp_res)
{
  Ipp32s  error = 0;    // OK

  sSBREnvDataState* pEDState = &(pSbrCom->sbrEDState[ch]);

  sSBREnvDataState* pEDState1 = &(pSbrCom->sbrEDState[1]);
  sSBREnvDataState* pEDState0 = &(pSbrCom->sbrEDState[0]);

/* ORDINARY */
/*
 * envelope
 */
  sbrDequantEnvMono(pEDState->bufEnvQuant, pSbrWS->bufEnvOrig[ch],
                    pEDState->vSizeEnv[pSbrCom->sbrFIState[ch].nEnv], bs_amp_res);

/*
 * noise
 */
  sbrDequantNoiseMono(pEDState->bufNoiseQuant, pSbrWS->bufNoiseOrig[ch],
                      pEDState->vSizeNoise[pSbrCom->sbrFIState[ch].nNoiseEnv]);

/* COUPLE MODE */
  if (pSbrCom->bs_coupling == 1) {

/*
 * envelope
 */
    sbrDequantEnvUncouple(pSbrWS->bufEnvOrig[0], pEDState1->bufEnvQuant,
                          pSbrWS->bufEnvOrig[1],
                          pEDState0->vSizeEnv[pSbrCom->sbrFIState[1].nEnv], bs_amp_res);

/*
 * noise
 */
    sbrDequantNoiseUncouple(pSbrWS->bufNoiseOrig[0], pEDState1->bufNoiseQuant,
                            pSbrWS->bufNoiseOrig[1],
                            pEDState0->vSizeNoise[pSbrCom->sbrFIState[0].nNoiseEnv]);
  }

  return error;
}

#endif //UMC_ENABLE_AAC_AUDIO_DECODER

