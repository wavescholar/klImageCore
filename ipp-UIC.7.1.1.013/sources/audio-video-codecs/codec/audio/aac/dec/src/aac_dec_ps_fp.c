/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_DECODER)

/* SYSTEM */
#include <math.h>
/* SBR/PS */
#include "sbr_huff_tabs.h"
#include "sbr_dec_tabs_fp.h"
#include "ps_dec_parser.h"
/* AAC */
#include "aac_dec_ps_fp.h"
#include "aac_status.h"
/* debug */
#include "vm_debug.h"

#include "ipps.h"

/********************************************************************/

#define PS_EPS_0              (1e-12f)

/********************************************************************/

#define  OWN_MULC(inData, outData, coef) \
  outData.re = inData.re;                \
  outData.im = inData.im * coef;


/********************************************************************/

#define IPDOPD_SF  ( (Ipp32f)IPP_PI / 4.f)

/* corrigendum contains only one factor */
static const Ipp32f DECAY_SLOPE       = 0.05f;




/********************************************************************/
#define PSDEC_UPDATE_PTR(type, ptr,inc)  \
  if (ptr) {                              \
  ptr = (type *)((Ipp8u *)(ptr) + inc); \
  }

/********************************************************************/

void   psdecUpdateMemMap(sPSDecState_32f* pPSState, Ipp32s mShift)
{
  Ipp32s m, k;

  PSDEC_UPDATE_PTR(Ipp32fc, pPSState->ppDelaySubQMF[0], mShift)
  PSDEC_UPDATE_PTR(Ipp32fc, pPSState->ppDelayQMF[0], mShift)

  for(k = 0; k < 14; k++){
    PSDEC_UPDATE_PTR(Ipp32fc, pPSState->ppDelaySubQMF[k], mShift)
    PSDEC_UPDATE_PTR(Ipp32fc, pPSState->ppDelayQMF[k], mShift)
  }

  /* problem code for update memory allocator, due to 3d matrix, will be fixed later */
  for(m=0; m < 3; m++){
    PSDEC_UPDATE_PTR(Ipp32fc*, pPSState->pppAllPassFilterMemSubQMF[m], mShift)
    for( k = 0; k < 5; k++){
      PSDEC_UPDATE_PTR(Ipp32fc, pPSState->pppAllPassFilterMemSubQMF[m][k], mShift)
    }
  }

  /* problem code for update memory allocator, due to 3d matrix */
  for(m=0; m < 3; m++){
    PSDEC_UPDATE_PTR(Ipp32fc*, pPSState->pppAllPassFilterMemQMF[m], mShift)
    for( k = 0; k < 5; k++){
      PSDEC_UPDATE_PTR(Ipp32fc, pPSState->pppAllPassFilterMemQMF[m][k], mShift)
    }
  }

  for(k = 0; k < 12; k++){
    PSDEC_UPDATE_PTR(Ipp32fc, pPSState->ppFracDelayLenTab1020[k], mShift)
  }

  for(k = 0; k < 32; k++){
    PSDEC_UPDATE_PTR(Ipp32fc, pPSState->ppFracDelayLenTab34[k], mShift)
  }

  for(k = 0; k < 64; k++){
    PSDEC_UPDATE_PTR(Ipp32fc, pPSState->ppFracDelayLenTabQMF[k], mShift)
  }

  for( k = 0; k < 32; k++ ){
    PSDEC_UPDATE_PTR(Ipp32fc, pPSState->ppHybridL[k], mShift)
    PSDEC_UPDATE_PTR(Ipp32fc, pPSState->ppHybridR[k] , mShift)
  }

  return;//OK
}

/********************************************************************/

void   psDecoderGetSize(Ipp32s *pSize)
{

  Ipp32s sum_size = 14*32 + 14*64 + 3*5*32 + (5*3) + 3*5*64 + (5*3) + (12+32+64)*3 + (32*32) + (32*32);

  *pSize = sum_size * sizeof(Ipp32fc);

  return;//OK
}

/********************************************************************/

static Ipp32s psDrawMem(sPSDecState_32f* pState, void* pMem)
{
  Ipp32fc* ptr = NULL;
  Ipp32s k = 0, m = 0, offset;

  ptr = (Ipp32fc*)pMem;

  pState->ppDelaySubQMF[0] = (Ipp32fc*)ptr;
  offset = 14*32;
  pState->ppDelayQMF[0]    = (Ipp32fc*)(ptr + offset);
  for(k = 0; k < 14; k++){
    pState->ppDelaySubQMF[k] = ptr + k * 32;
    pState->ppDelayQMF[k]    = ptr + offset + k*64;
  }

  offset += 14*64;
  for(m=0; m < 3; m++){
    pState->pppAllPassFilterMemSubQMF[m] = (Ipp32fc**)(ptr + offset);
    offset += 5;
    for( k = 0; k < 5; k++){
      pState->pppAllPassFilterMemSubQMF[m][k] = (Ipp32fc*)(ptr + offset + k*32);
    }
    offset += 5*32;
  }

  for(m=0; m < 3; m++){
    pState->pppAllPassFilterMemQMF[m] = (Ipp32fc**)(ptr + offset);
    offset += 5;
    for( k = 0; k < 5; k++){
      pState->pppAllPassFilterMemQMF[m][k] = ptr + offset + k*64;
    }
    offset += 5*64;
  }

  for(k = 0; k < 12; k++){
    pState->ppFracDelayLenTab1020[k] = ptr + offset + 3*k;
  }
  offset += 3*12;

  for(k = 0; k < 32; k++){
    pState->ppFracDelayLenTab34[k] = ptr + offset + 3*k;
  }
  offset += 3*32;

  for(k = 0; k < 64; k++){
    pState->ppFracDelayLenTabQMF[k] = ptr + offset + 3*k;
  }
  offset += 3*64;

  for( k = 0; k < 32; k++ ){
    pState->ppHybridL[k]  = ptr + offset + 32*k;
    pState->ppHybridR[k] = ptr + offset + 32*32 + 32*k;
  }

  return 0;//OK
}

/********************************************************************/

Ipp32s psInitDecoder_32f(sPSDecState_32f* pState, void* pMem)
{
  Ipp32f tabHCenterFreq20[12] = {0.5/4,    1.5/4,    2.5/4,  3.5/4,
                                 4.5/4*0,  5.5/4*0, -1.5/4, -0.5/4,
                                 3.5/2,    2.5/2,    4.5/2,  5.5/2};

  Ipp32f tabHCenterFreq34[32] = { 1.0f/12,    3.0f/12,    5.0f/12,   7.0f/12,
                                  9.0f/12,    11.0f/12,  13.0f/12,  15.0f/12,
                                  17.0f/12,   -5.0f/12,  -3.0f/12,  -1.0f/12,
                                  17.0f/8,    19.0f/8,    5.0f/8,    7.0f/8,
                                  9.0f/8,     11.0f/8,   13.0f/8,   15.0f/8,
                                  9.0f/4,     11.0f/4,   13.0f/4,    7.0f/4,
                                  17.0f/4,    11.0f/4,   13.0f/4,   15.0f/4,
                                  17.0f/4,    19.0f/4,   21.0f/4,   15.0f/4};

  Ipp32f tabFracDelayLen[] = {0.43f, 0.75f, 0.347f};

  Ipp32s k = 0, m = 0;
  Ipp32f arg = 0.f;
  Ipp32f fracParam = 0.39f;
  Ipp32s i;

  ps_header_fill_default( &(pState->comState) );

  // memory allocator
  psDrawMem(pState, pMem);

  /* ----------------------------------------------------- */

  /* TUNING TABLES */
  /* [tab1] - fractional delay vector 1020 */
  for( k = 0; k < 12; k++ ){
    arg = (Ipp32f)(- IPP_PI * tabHCenterFreq20[k] * fracParam);
    pState->pFracDelayLenTab1020[k].re = (Ipp32f)cos( arg );
    pState->pFracDelayLenTab1020[k].im = (Ipp32f)sin( arg );
  }

  /* [tab2] - fractional delay vector 34 */
  for( k = 0; k < 32; k++ ){
    arg = (Ipp32f)(- IPP_PI * tabHCenterFreq34[k] * fracParam);
    pState->pFracDelayLenTab34[k].re = (Ipp32f)cos( arg );
    pState->pFracDelayLenTab34[k].im = (Ipp32f)sin( arg );
  }

  /* [tab3] - fractional delay vector. QMF domain */
  for( k = 0; k < 64; k++ ){
    arg = (Ipp32f)(-IPP_PI * ( k + 0.5f ) * fracParam);
    pState->pFracDelayLenTabQMF[k].re = (Ipp32f)cos(arg);
    pState->pFracDelayLenTabQMF[k].im = (Ipp32f)sin(arg);
  }

  /* [tab4] - fractional delay matrix */
  for(m = 0; m < 3; m++){
    /* 1020 */
    for( k = 0; k < 12; k++){
      arg = (Ipp32f)(-IPP_PI * tabHCenterFreq20[k] * tabFracDelayLen[m]);
      pState->ppFracDelayLenTab1020[k][m].re = (Ipp32f)cos( arg );
      pState->ppFracDelayLenTab1020[k][m].im = (Ipp32f)sin( arg );
    }

    /* 34 */
    for( k = 0; k < 32; k++ ){
      arg = (Ipp32f)(-IPP_PI * tabHCenterFreq34[k] * tabFracDelayLen[m]);
      pState->ppFracDelayLenTab34[k][m].re = (Ipp32f)cos( arg );
      pState->ppFracDelayLenTab34[k][m].im = (Ipp32f)sin( arg );
    }

    /* QMF domain */
    for( k = 0; k < 64; k++ ){
      arg = (Ipp32f)(-IPP_PI * ( k + 0.5f ) * tabFracDelayLen[m]);
      pState->ppFracDelayLenTabQMF[k][m].re = (Ipp32f)cos( arg );
      pState->ppFracDelayLenTabQMF[k][m].im = (Ipp32f)sin( arg );
    }
  }

  /* init tables */
  ippsSet_32s(14, pState->bufNumSampleDelayQMF, 35);
  ippsSet_32s(1, pState->bufNumSampleDelayQMF + 35, 64 - 35);

  /* common part */
  pState->comState.delayLenIndx = 0;

/************************************************************************/
/*       mapping index                                                  */
/************************************************************************/
  for( i = 0; i < 34; i++ ){
    pState->h11Prev[i].re = 1.f;
    pState->h11Prev[i].im = 0.f;

    pState->h12Prev[i].re = 1.f;
    pState->h12Prev[i].im = 0.f;

    pState->h21Prev[i].re = 0.f;
    pState->h21Prev[i].im = 0.f;

    pState->h22Prev[i].re = 0.f;
    pState->h22Prev[i].im = 0.f;
  }

  return 0;//OK
}

/********************************************************************/

Ipp32s psFreeDecoder_32f(/* sPSDecState_32f* pState */)
{

  return 0;//OK
}

/********************************************************************/
#if 0
static
Ipp32s ownAnalysisFilter_PSDec_Kernel_32fc(const Ipp32fc* pSrc,
                                           Ipp32fc ppDst[32][12],
                                           const Ipp32f* pTab,
                                           Ipp32s nSubBands)
{
  Ipp32s band, n, q;
  Ipp32f arg = 0.f;
  Ipp32fc hRes;

  for(band = 0; band < NUM_SBR_BAND; band++) {
    for(q = 0; q < nSubBands; q++) {
      Ipp32fc res = {0.f, 0.f};

      for(n = 0; n < 13; n++) {
        arg = (Ipp32f)(2.f * IPP_PI * (n-6) / nSubBands);

        if ( 2 == nSubBands ) {
          arg *= q;

          hRes.re = (Ipp32f)cos(arg);
          hRes.im = 0.f;
        } else {
          arg *= (q + 0.5f);

          hRes.re =  (Ipp32f)cos( arg );
          hRes.im = -(Ipp32f)sin( arg );
        }
        res.re += pTab[n] * ( pSrc[n+band].re * hRes.re - pSrc[n+band].im * hRes.im );
        res.im += pTab[n] * ( pSrc[n+band].im * hRes.re + pSrc[n+band].re * hRes.im );
      }
      ppDst[band][q] = res;
    }
  }

  return 0; //OK
}
#endif
/********************************************************************/

static Ipp32s ownAnalysisUpdateMem_32fc(Ipp32fc** ppSrc, Ipp32fc* pMemBuf,
                                        Ipp32fc* pWorkBuf, int band)
{
  Ipp32fc* ptr     = NULL;
  Ipp32s   sbr_band = 0;

  ippsCopy_32fc(pMemBuf, pWorkBuf, LEN_PS_FILTER);
  ptr = pWorkBuf + LEN_PS_FILTER;
  for( sbr_band = 0; sbr_band < NUM_SBR_BAND; sbr_band++ ){
    ptr[sbr_band] = ppSrc[ sbr_band + DELAY_PS_FILTER ][band];
  }
  ippsCopy_32fc(pWorkBuf + NUM_SBR_BAND, pMemBuf, LEN_PS_FILTER);

  return 0;//OK
}

/********************************************************************/

static Ipp32s ownAnalysisFilter_PSDec_32fc(Ipp32fc** ppSrc,
                                           Ipp32fc** ppDst,
                                           sHybridAnalysis* pState,
                                           Ipp32s flag)
{

  IPP_ALIGNED_ARRAY(32, Ipp32fc, bufW, 44);
  Ipp32s   nBand = 0, band = 0, sbr_band = 0, q = 0;
  Ipp32s   offset_tab = 0, offset_channel = 0, hRes = 0;
  Ipp32s*  pResTab = NULL;
  int      flag34 = (flag == CONFIG_HA_BAND34) ? 1 : 0;

  /* [1] select config */
  pResTab = (flag == CONFIG_HA_BAND34) ? (Ipp32s*)tabResBand34 : (Ipp32s*)tabResBand1020;
  nBand   =  pResTab[0];
  offset_tab = (flag == CONFIG_HA_BAND34) ? 0 : 1;

  /* [2] core filtering */
  for( band = 0; band < nBand; band++ ){

    hRes = pResTab[ band + 1 ];// "+1" because [0] - len of vector

    ownAnalysisUpdateMem_32fc(ppSrc, pState->mMemBuf[band], bufW, band);
    /* filtering */
    ownAnalysisFilter_PSDec_Kernel_v2_32fc(bufW, pState->mTmpBuf, hRes, flag34);

    for(sbr_band = 0; sbr_band < NUM_SBR_BAND; sbr_band++) {
      for(q = 0; q < hRes; q++) {
        ppDst[sbr_band][offset_channel + q] = pState->mTmpBuf[sbr_band][q];
      }
    }
    offset_channel += hRes;
  }// END for( band = 0;


  /* [3] post-process of filtering */
  if( CONFIG_HA_BAND1020 == flag ){

    for( band = 3; band < 5; band++ ){
      for( sbr_band = 0; sbr_band < 12; sbr_band++ ){
        pState->mMemBuf[band][sbr_band] = ppSrc[ sbr_band + 26 ][band];
      }
    }

    for( band = 0; band < NUM_SBR_BAND; band++ ){
      ppDst[band][3].re += ppDst[band][4].re;
      ppDst[band][3].im += ppDst[band][4].im;
      ppDst[band][4].re  = ppDst[band][4].im = 0.f;

      ppDst[band][2].re += ppDst[band][5].re;
      ppDst[band][2].im += ppDst[band][5].im;
      ppDst[band][5].re  = ppDst[band][5].im = 0.f;
    }
  }

  return 0;//OK
}

/********************************************************************/

static Ipp32s ownCalcAttenuation_32f(Ipp32f* pPowLine,
                                      Ipp32f* pGAttenLine,
                                      sPSDecState_32f* pPSDecState,
                                      Ipp32s stopBand)
{
  const Ipp32f factorPeakDecay   = 0.76592833836465f;
  const Ipp32f coefSmooth        = 0.25f;
  const Ipp32f factorTransImpact = 1.5f;

  Ipp32f  nrgPeakDecayDiff, nrg;

  Ipp32s bin;

  for( bin = 0; bin < stopBand; bin++ ){

    /* [2.1] - peak decay */
    pPSDecState->bufPeakDecayNrg[bin] *= factorPeakDecay;
    if( pPSDecState->bufPeakDecayNrg[bin] <  pPowLine[bin]) {
      pPSDecState->bufPeakDecayNrg[bin] = pPowLine[bin];
    }

    /* [2.2] - filter power & peak decay */
    nrgPeakDecayDiff  = pPSDecState->bufPeakDecayDiffNrgPrev[bin];
    nrgPeakDecayDiff += coefSmooth * (pPSDecState->bufPeakDecayNrg[bin] - pPowLine[bin] - nrgPeakDecayDiff);
    pPSDecState->bufPeakDecayDiffNrgPrev[bin] = nrgPeakDecayDiff;

    /* [2.3] - transient attenuator */
    nrg  = pPSDecState->bufPeakDecayNrgPrev[bin];
    nrg += coefSmooth * (pPowLine[bin] - nrg);
    pPSDecState->bufPeakDecayNrgPrev[bin] = nrg;

    pGAttenLine[bin] = 1.f;
    if( factorTransImpact* nrgPeakDecayDiff > nrg ) {
      pGAttenLine[bin] = nrg / (factorTransImpact * nrgPeakDecayDiff);
    }

  }// end for(k = 0;


  return 0;//OK
}

/********************************************************************/

static Ipp32fc ownAllPassFilter_32fc(Ipp32fc*** pppMem, Ipp32fc* pTabQ, Ipp32s* pDelayIndx,
                                     Ipp32f coef, Ipp32s pos, Ipp32fc HS )
{

  Ipp32s ind0, ind1, ind2;
  Ipp32f coefDecay;
  Ipp32fc tmp0, tmp;
  Ipp32f tabExp[] = { 0.6514390707f,
                      0.5647181273f,
                      0.4895416498f };

  ind0 = pDelayIndx[0];
  ind1 = pDelayIndx[1];
  ind2 = pDelayIndx[2];

  //for( m = 0; m < 3; m++ ){

    // m = 0
    tmp0   = pppMem[0][ ind0 ][pos];

    tmp.re = tmp0.re * pTabQ[0].re - tmp0.im * pTabQ[0].im;
    tmp.im = tmp0.re * pTabQ[0].im + tmp0.im * pTabQ[0].re;

    coefDecay = tabExp[ 0 ];

    tmp.re -= coef * coefDecay * HS.re;
    tmp.im -= coef * coefDecay * HS.im;

    HS.re += coef * coefDecay * tmp.re;
    HS.im += coef * coefDecay * tmp.im;
    pppMem[0][ ind0 ][pos] = HS;

    HS = tmp;

    // m = 1
    tmp0   = pppMem[1][ ind1 ][pos];

    tmp.re = tmp0.re * pTabQ[1].re - tmp0.im * pTabQ[1].im;
    tmp.im = tmp0.re * pTabQ[1].im + tmp0.im * pTabQ[1].re;

    coefDecay = tabExp[1];

    tmp.re -= coef * coefDecay * HS.re;
    tmp.im -= coef * coefDecay * HS.im;

    HS.re += coef * coefDecay * tmp.re;
    HS.im += coef * coefDecay * tmp.im;
    pppMem[1][ ind1 ][pos] = HS;

    HS = tmp;

    // m = 2
    tmp0   = pppMem[2][ ind2 ][pos];

    tmp.re = tmp0.re * pTabQ[2].re - tmp0.im * pTabQ[2].im;
    tmp.im = tmp0.re * pTabQ[2].im + tmp0.im * pTabQ[2].re;

    coefDecay = tabExp[2];

    tmp.re -= coef * coefDecay * HS.re;
    tmp.im -= coef * coefDecay * HS.im;

    HS.re += coef * coefDecay * tmp.re;
    HS.im += coef * coefDecay * tmp.im;
    pppMem[2][ ind2 ][pos] = HS;

    HS = tmp;
  //}

  return HS;
}

/********************************************************************/
// strategy from ref code: clean state of decorrelation instead of remapping
static Ipp32s ownResetDecorrState( sPSDecState_32f* pState )
{
  int i, j;

  /* 1D */
  ippsZero_32f(pState->bufPeakDecayNrg, 34);
  ippsZero_32f(pState->bufPeakDecayDiffNrgPrev, 34);
  ippsZero_32f(pState->bufPeakDecayNrgPrev, 34);

  /* 2D */
  for( i = 0; i < 14; i++ ){
    ippsZero_32fc(pState->ppDelaySubQMF[i], 32);
    ippsZero_32fc(pState->ppDelayQMF[i], 64);
  }

  /* 3D */
  for( i = 0; i < 3; i++ ){
    for( j = 0; j < 5; j++ ){
      ippsZero_32fc(pState->pppAllPassFilterMemSubQMF[i][j], 32);
      ippsZero_32fc(pState->pppAllPassFilterMemQMF[i][j], 64);
    }
  }

  return 0;//OK
}

/********************************************************************/

static Ipp32s ownDecorrelation_PSDec_32fc(Ipp32fc** ppSrcL,
                                          Ipp32fc** ppSrcR,
                                          sTDSpec* pTDSpec,
                                          sPSDecState_32f* pPSDecState)
{
#if !defined(ANDROID)
  Ipp32f  mPower[32][34];
  Ipp32f  mGTransRatio[32][34];
#else
  static Ipp32f  mPower[32][34];
  static Ipp32f  mGTransRatio[32][34];
#endif
  Ipp32f  transRatio = 0.f;
  Ipp32fc val, inputLeft;

  Ipp32s  group = 0, bk = 0, k = 0, n = 0, bord = 0;
  Ipp32s  tmpDelayLenIndx = 0, tmpIndx2 = 0, condition = 0;

  Ipp32s  delayIndxMem[3];

  Ipp32s  eqThr = 0;

  /* matrix */
  Ipp32fc*** pppAllPassFilterMem = NULL;
  Ipp32fc**  ppDelayQMF          = NULL;
  Ipp32fc**  ppFracDelayLenTabQ  = NULL;
  Ipp32fc*   pFracDelayLenTabFi  = NULL;
  Ipp32fc**  ppTabQ              = NULL;
  Ipp32fc*   pTabFi              = NULL;

  Ipp32fc**  ppSrc               = NULL; // dynamic input for every case [ppSrcL OR ppHybL]
  Ipp32fc**  ppDst               = NULL; // dynamic output for every case[ppSrcR OR ppHybR]


  /* code */
  /* [y] tuning */
  if( CONFIG_HA_BAND1020 == pPSDecState->comState.flag_HAconfig ){
    ppTabQ = pPSDecState->ppFracDelayLenTab1020;
    pTabFi = pPSDecState->pFracDelayLenTab1020;
  } else {
    ppTabQ = pPSDecState->ppFracDelayLenTab34;
    pTabFi = pPSDecState->pFracDelayLenTab34;
  }

  /* [1] power matrix */
  ippsZero_32f( mPower[0], 32*34 );

  ppSrc = pPSDecState->ppHybridL;
  for( group = 0; group < pTDSpec->thres; group++ ){
    bk = pTDSpec->pBinTab[ group ];
    for( k = pTDSpec->pStartBordTab[group]; k < pTDSpec->pStopBordTab[group]; k++ ){
      for( n = 0; n < 32;  ){
        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;
      }
    }
  }

  ppSrc = ppSrcL;
  for( group = pTDSpec->thres; group < pTDSpec->nGroup; group++ ){
    bk = pTDSpec->pBinTab[ group ];
    for( k = pTDSpec->pStartBordTab[group]; k < pTDSpec->pStopBordTab[group]; k++ ){
      for( n = 0; n < 32;  ){
        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;

        val = ppSrc[n][k];
        mPower[n++][bk] += val.re*val.re + val.im*val.im;
      }
    }
  }

  /* [x] reset if switch 20<=>34 band detected */
  if( pPSDecState->comState.flag_HAconfig != pPSDecState->comState.flag_HAconfigPrev ){
    ownResetDecorrState( pPSDecState );
  }

  /* [2] transient attenuator */
  for( k = 0; k < 32; k++ ){
    ownCalcAttenuation_32f(mPower[k], mGTransRatio[k], pPSDecState, pTDSpec->nBin);
  }

  /* [3] calc decorrelated signal */
  for( group = 0; group < pTDSpec->nGroup; group++ ){

    /* common place to change */
    if( group < pTDSpec->thres ){
      pFracDelayLenTabFi = pTabFi; //vector fi
      ppFracDelayLenTabQ = ppTabQ;//matrix Q

      pppAllPassFilterMem= pPSDecState->pppAllPassFilterMemSubQMF;
      ppDelayQMF         = pPSDecState->ppDelaySubQMF;

      ppSrc = pPSDecState->ppHybridL;
      ppDst = pPSDecState->ppHybridR;

      eqThr = 0;

    } else {
      pFracDelayLenTabFi = pPSDecState->pFracDelayLenTabQMF; //vector fi
      ppFracDelayLenTabQ = pPSDecState->ppFracDelayLenTabQMF;//matrix Q

      pppAllPassFilterMem= pPSDecState->pppAllPassFilterMemQMF;
      ppDelayQMF         = pPSDecState->ppDelayQMF;

      ppSrc = ppSrcL;
      ppDst = ppSrcR;

      eqThr = 1;
    }

    bk = pTDSpec->pBinTab[ group ];

    for( bord = pTDSpec->pStartBordTab[group]; bord < pTDSpec->pStopBordTab[group]; bord++ ){

      Ipp32f factorDecaySlope = 1.0f;

      if ( eqThr && bord > pTDSpec->decay_cutoff ){
        factorDecaySlope += pTDSpec->decay_cutoff * DECAY_SLOPE - DECAY_SLOPE * bord;
        factorDecaySlope  = IPP_MAX(factorDecaySlope, 0.0f);
      }

      tmpDelayLenIndx = pPSDecState->comState.delayLenIndx;

      delayIndxMem[0] = pPSDecState->delayIndxMem[0];
      delayIndxMem[1] = pPSDecState->delayIndxMem[1];
      delayIndxMem[2] = pPSDecState->delayIndxMem[2];

      condition = (eqThr) && (bord  >= pTDSpec->firstDelaySb);

      if ( 0 == condition ){
        for( k = 0; k < 32; k++ ){
          Ipp32fc tmp, HS, tabD;

          inputLeft = ppSrc[k][bord];
          tabD      = pFracDelayLenTabFi[bord];

          tmpIndx2 = tmpDelayLenIndx;

          HS = ppDelayQMF[ tmpIndx2 ][bord];
          ppDelayQMF[ tmpIndx2 ][bord] = inputLeft;

          tmp.re = HS.re * tabD.re - HS.im * tabD.im;
          tmp.im = HS.re * tabD.im + HS.im * tabD.re;

          HS = ownAllPassFilter_32fc(pppAllPassFilterMem, ppFracDelayLenTabQ[bord], delayIndxMem,
                                     factorDecaySlope, bord, tmp);

          transRatio = mGTransRatio[k][bk];

          /* "right" channel, output data */
          ppDst[k][bord].re = transRatio * HS.re;
          ppDst[k][bord].im = transRatio * HS.im;

          /* update delay index buffer */
          tmpDelayLenIndx = (tmpDelayLenIndx + 1) & 1;

          //-------------------
          if ( ++delayIndxMem[0] == 3 ) delayIndxMem[0] = 0;
          if ( ++delayIndxMem[1] == 4 ) delayIndxMem[1] = 0;
          if ( ++delayIndxMem[2] == 5 ) delayIndxMem[2] = 0;
          //-------------------

        }//end for( k = 0;
      } else { // condition != 0

        for( k = 0; k < 32; k++ ){
          Ipp32fc HS;

          inputLeft = ppSrc[k][bord];

          tmpIndx2 = pPSDecState->bufIndexDelayQMF[bord];

          HS = ppDelayQMF[ tmpIndx2 ][bord];
          ppDelayQMF[ tmpIndx2 ][bord] = inputLeft;

          transRatio = mGTransRatio[k][bk];

          /* "right" channel, output data */
          ppDst[k][bord].re = transRatio * HS.re;
          ppDst[k][bord].im = transRatio * HS.im;

          /* update delay index buffer */
          tmpDelayLenIndx = (tmpDelayLenIndx + 1) & 1;

          //-------------------
          if( ++(pPSDecState->bufIndexDelayQMF[bord]) >= pPSDecState->bufNumSampleDelayQMF[bord] ){
            pPSDecState->bufIndexDelayQMF[bord] = 0;
          }
          if ( ++delayIndxMem[0] == 3 ) delayIndxMem[0] = 0;
          if ( ++delayIndxMem[1] == 4 ) delayIndxMem[1] = 0;
          if ( ++delayIndxMem[2] == 5 ) delayIndxMem[2] = 0;
          //-------------------

        }//end for( k = 0;
      }// condition
    }//for( bord =
  }// end for( group = 0;

  /* update */
  pPSDecState->comState.delayLenIndx = tmpDelayLenIndx;

  pPSDecState->delayIndxMem[0] = delayIndxMem[0];
  pPSDecState->delayIndxMem[1] = delayIndxMem[1];
  pPSDecState->delayIndxMem[2] = delayIndxMem[2];

  return 0;//OK
}

/********************************************************************/

static Ipp32s ownMap20To34_32fc(Ipp32fc* pSrcDst)
{
  Ipp32fc bufIDX[34];

  bufIDX[0]  = pSrcDst[0];

  bufIDX[1].re  = (pSrcDst[0].re + pSrcDst[1].re)/2.f;
  bufIDX[1].im  = (pSrcDst[0].im + pSrcDst[1].im)/2.f;

  bufIDX[2]  = pSrcDst[1];
  bufIDX[3]  = pSrcDst[2];

  bufIDX[4].re  = (pSrcDst[2].re + pSrcDst[3].re)/2.f;
  bufIDX[4].im  = (pSrcDst[2].im + pSrcDst[3].im)/2.f;

  bufIDX[5]  = pSrcDst[3];
  bufIDX[6]  = pSrcDst[4];
  bufIDX[7]  = pSrcDst[4];
  bufIDX[8]  = pSrcDst[5];
  bufIDX[9]  = pSrcDst[5];
  bufIDX[10] = pSrcDst[6];
  bufIDX[11] = pSrcDst[7];
  bufIDX[12] = pSrcDst[8];
  bufIDX[13] = pSrcDst[8];
  bufIDX[14] = pSrcDst[9];
  bufIDX[15] = pSrcDst[9];
  bufIDX[16] = pSrcDst[10];
  bufIDX[17] = pSrcDst[11];
  bufIDX[18] = pSrcDst[12];
  bufIDX[19] = pSrcDst[13];
  bufIDX[20] = pSrcDst[14];
  bufIDX[21] = pSrcDst[14];
  bufIDX[22] = pSrcDst[15];
  bufIDX[23] = pSrcDst[15];
  bufIDX[24] = pSrcDst[16];
  bufIDX[25] = pSrcDst[16];
  bufIDX[26] = pSrcDst[17];
  bufIDX[27] = pSrcDst[17];
  bufIDX[28] = pSrcDst[18];
  bufIDX[29] = pSrcDst[18];
  bufIDX[30] = pSrcDst[18];
  bufIDX[31] = pSrcDst[18];
  bufIDX[32] = pSrcDst[19];
  bufIDX[33] = pSrcDst[19];

  ippsCopy_32fc(bufIDX, pSrcDst, 34);

  return 0;//OK
}

/********************************************************************/

static Ipp32s ownMap34To20_32fc(Ipp32fc* pSrcDst)
{
  pSrcDst[0].re  = (2*pSrcDst[0].re+pSrcDst[1].re)/3.f;
  pSrcDst[0].im  = (2*pSrcDst[0].im+pSrcDst[1].im)/3.f;

  pSrcDst[1].re  = (pSrcDst[1].re+2*pSrcDst[2].re)/3.f;
  pSrcDst[1].im  = (pSrcDst[1].im+2*pSrcDst[2].im)/3.f;

  pSrcDst[2].re  = (2*pSrcDst[3].re+pSrcDst[4].re)/3.f;
  pSrcDst[2].im  = (2*pSrcDst[3].im+pSrcDst[4].im)/3.f;

  pSrcDst[3].re  = (pSrcDst[4].re+2*pSrcDst[5].re)/3.f;
  pSrcDst[3].im  = (pSrcDst[4].im+2*pSrcDst[5].im)/3.f;

  pSrcDst[4].re  = (pSrcDst[6].re+pSrcDst[7].re)/2.f;
  pSrcDst[4].im  = (pSrcDst[6].im+pSrcDst[7].im)/2.f;

  pSrcDst[5].re  = (pSrcDst[8].re+pSrcDst[9].re)/2.f;
  pSrcDst[5].im  = (pSrcDst[8].im+pSrcDst[9].im)/2.f;

  pSrcDst[6]  = pSrcDst[10];
  pSrcDst[7]  = pSrcDst[11];

  pSrcDst[8].re  = (pSrcDst[12].re+pSrcDst[13].re)/2.f;
  pSrcDst[8].im  = (pSrcDst[12].im+pSrcDst[13].im)/2.f;

  pSrcDst[9].re  = (pSrcDst[14].re+pSrcDst[15].re)/2.f;
  pSrcDst[9].im  = (pSrcDst[14].im+pSrcDst[15].im)/2.f;

  pSrcDst[10] = pSrcDst[16];
  pSrcDst[11] = pSrcDst[17];
  pSrcDst[12] = pSrcDst[18];
  pSrcDst[13] = pSrcDst[19];

  pSrcDst[14].re = (pSrcDst[20].re+pSrcDst[21].re)/2.f;
  pSrcDst[14].im = (pSrcDst[20].im+pSrcDst[21].im)/2.f;

  pSrcDst[15].re = (pSrcDst[22].re+pSrcDst[23].re)/2.f;
  pSrcDst[15].im = (pSrcDst[22].im+pSrcDst[23].im)/2.f;

  pSrcDst[16].re = (pSrcDst[24].re+pSrcDst[25].re)/2.f;
  pSrcDst[16].im = (pSrcDst[24].im+pSrcDst[25].im)/2.f;

  pSrcDst[17].re = (pSrcDst[26].re+pSrcDst[27].re)/2.f;
  pSrcDst[17].im = (pSrcDst[26].im+pSrcDst[27].im)/2.f;

  pSrcDst[18].re = (pSrcDst[28].re+pSrcDst[29].re+pSrcDst[30].re+pSrcDst[31].re)/4.f;
  pSrcDst[18].im = (pSrcDst[28].im+pSrcDst[29].im+pSrcDst[30].im+pSrcDst[31].im)/4.f;

  pSrcDst[19].re = (pSrcDst[32].re+pSrcDst[33].re)/2.f;
  pSrcDst[19].im = (pSrcDst[32].im+pSrcDst[33].im)/2.f;

  return 0;//OK
}

/********************************************************************/

static Ipp32s ownSmoothRemap_32fc( Ipp32fc* pCoef11, Ipp32fc* pCoef12,
                                   Ipp32fc* pCoef21, Ipp32fc* pCoef22,
                                   Ipp32s flag)
{
  if( flag ){ /* map 20->34 bands */
    ownMap20To34_32fc( pCoef11 );
    ownMap20To34_32fc( pCoef12 );
    ownMap20To34_32fc( pCoef21 );
    ownMap20To34_32fc( pCoef22 );
  } else {   /* map 34->20 bands */
    ownMap34To20_32fc( pCoef11 );
    ownMap34To20_32fc( pCoef12 );
    ownMap34To20_32fc( pCoef21 );
    ownMap34To20_32fc( pCoef22 );
  }

  return 0;//OK
}

/********************************************************************/

static Ipp32s ownStereoFilter_32fc(Ipp32fc* pSrcL, Ipp32fc* pSrcR,
                                   Ipp32fc* bufH,  int iStart, int iStop)
{

  int i;
  Ipp32fc tmpL, tmpR, srcL, srcR;
  Ipp32fc c0, c1, c2, c3;

  c0 = bufH[0];
  c1 = bufH[1];
  c2 = bufH[2];
  c3 = bufH[3];

  for( i = iStart; i < iStop; i++ ){

    srcL = pSrcL[i];
    srcR = pSrcR[i];

    tmpL.re =  c0.re * srcL.re -  c0.im * srcL.im +
               c2.re * srcR.re - c2.im * srcR.im;

    tmpL.im  =  c0.im * srcL.re +  c0.re * srcL.im +
                c2.im * srcR.re + c2.re * srcR.im;

    tmpR.re =  c1.re * srcL.re - c1.im * srcL.im +
               c3.re * srcR.re - c3.im * srcR.im;

    tmpR.im =  c1.im * srcL.re +  c1.re * srcL.im +
               c3.im * srcR.re + c3.re * srcR.im;

    pSrcL[i] = tmpL;
    pSrcR[i] = tmpR;
  }

  return 0;//OK
}

/********************************************************************/

static Ipp32s ownIpdOpdSmooth_32fc(Ipp32fc* pCoeffs, sPSDecComState* pCom, int env, int pos)
{
  Ipp32f ipd, ipd1, ipd2;
  Ipp32f opd, opd1, opd2;
  Ipp32fc tmpL, tmpR;

  ipd  = IPDOPD_SF * pCom->indxIpdMapped[env][pos];
  ipd1 = IPDOPD_SF * pCom->indxIpdMapped_1[pos];
  ipd2 = IPDOPD_SF * pCom->indxIpdMapped_2[pos];

  opd  = IPDOPD_SF * pCom->indxOpdMapped[env][pos];
  opd1 = IPDOPD_SF * pCom->indxOpdMapped_1[pos];
  opd2 = IPDOPD_SF * pCom->indxOpdMapped_2[pos];

  tmpL.re = (Ipp32f)(cos(ipd) + 0.5f*cos(ipd1) + 0.25f*cos(ipd2));
  tmpL.im = (Ipp32f)(sin(ipd) + 0.5f*sin(ipd1) + 0.25f*sin(ipd2));
  tmpR.re = (Ipp32f)(cos(opd) + 0.5f*cos(opd1) + 0.25f*cos(opd2));
  tmpR.im = (Ipp32f)(sin(opd) + 0.5f*sin(opd1) + 0.25f*sin(opd2));

  ipd = (Ipp32f)atan2( tmpL.im, tmpL.re );
  opd = (Ipp32f)atan2( tmpR.im, tmpR.re );

  /* phase rotation */
  tmpL.re  = (Ipp32f)cos( opd );
  tmpL.im  = (Ipp32f)sin( opd );
  opd -= ipd;
  tmpR.re = (Ipp32f)cos( opd );
  tmpR.im = (Ipp32f)sin( opd );

  pCoeffs[0].im = pCoeffs[0].re * tmpL.im;
  pCoeffs[1].im = pCoeffs[1].re * tmpR.im;
  pCoeffs[2].im = pCoeffs[2].re * tmpL.im;
  pCoeffs[3].im = pCoeffs[3].re * tmpR.im;

  pCoeffs[0].re *= tmpL.re;
  pCoeffs[1].re *= tmpR.re;
  pCoeffs[2].re *= tmpL.re;
  pCoeffs[3].re *= tmpR.re;

  return 0;//OK
}

/********************************************************************/

#define  UMC_INV_SQRT2 (0.7071067811865475244016887242097f)
#define  UMC_PI_4      (0.78539816339744830961566084581988f)
#define  UMC_PI_2      (1.5707963267948966192313216916398f)

static Ipp32s ownStereoProcessing_PSDec_32fc(Ipp32fc** ppSrcL,
                                             Ipp32fc** ppSrcR,
                                             sTDSpec*  pTDSpec,
                                             sPSDecState_32f* pPSDecState)
{
  /* internal tables */
  const Ipp32f tabAlphas[8] = {0.f, 0.17842767f, 0.2856673f, 0.46307236f,
                               0.59716314f, 0.78539819f, 1.1003085f, 1.5707964f};

  Ipp32fc bufCoeffs[4];
  Ipp32fc bufH[4];

  Ipp32f  scaleL, scaleR;
  Ipp32f  alpha,  beta, arg;

  Ipp32s  group = 0, i = 0, len = 0, indx = 0;
  Ipp32s  nIIDSteps, env = 0, bin = 0, typeMix = 0;

  sPSDecComState*  pCom       = NULL;
  Ipp32fc**        ppSuitSrcL = NULL;
  Ipp32fc**        ppSuitSrcR = NULL;
  Ipp32f*          pTabScale  = NULL;
  Ipp32f*          pTabQuantIID = NULL;

  pCom    = &(pPSDecState->comState);
  indx    = ( CONFIG_HA_BAND1020 == pPSDecState->comState.flag_HAconfig ) ? 0 : 1;
  typeMix = pCom->mix_strategy;

  /* [1]. tuning params */
  if( pCom->iid_quant ){
    nIIDSteps = NUM_IID_FINE_GRID_STEPS;
    pTabScale = (Ipp32f*)tabScaleIidFine;
    pTabQuantIID = (Ipp32f*)tabQuantIidFine;
  } else {
    nIIDSteps = NUM_IID_GRID_STEPS;
    pTabScale = (Ipp32f*)tabScaleIidStd;
    pTabQuantIID = (Ipp32f*)tabQuantIidStd;
  }

  /* [2]. smooth remapping */
  if( pCom->flag_HAconfig != pCom->flag_HAconfigPrev ){

    ownSmoothRemap_32fc( pPSDecState->h11Prev, pPSDecState->h12Prev,
                         pPSDecState->h21Prev, pPSDecState->h22Prev, indx);

    /* common part */
    ippsZero_32s(pCom->indxIpdMapped_1, 17);
    ippsZero_32s(pCom->indxIpdMapped_2, 17);
    ippsZero_32s(pCom->indxOpdMapped_1, 17);
    ippsZero_32s(pCom->indxOpdMapped_2, 17);
  }

  /* [3]. processing */
  for( env = 0; env < pCom->nEnv; env++ ){
    for( bin = 0; bin < pTDSpec->nBin; bin++ ){

      if( PS_MIX_RA == typeMix ){/* Mixing Ra */
        Ipp32s id = pCom->indxIidMapped[env][bin];

        scaleR = pTabScale[ nIIDSteps + id ];
        scaleL = pTabScale[ nIIDSteps - id ];

        alpha = tabAlphas[ pCom->indxIccMapped[env][bin] ];
        beta  = alpha * ( scaleR - scaleL ) * UMC_INV_SQRT2;

        bufCoeffs[0].re = ( Ipp32f )( scaleL * cos( beta + alpha ) );
        bufCoeffs[1].re = ( Ipp32f )( scaleR * cos( beta - alpha ) );
        bufCoeffs[2].re = ( Ipp32f )( scaleL * sin( beta + alpha ) );
        bufCoeffs[3].re = ( Ipp32f )( scaleR * sin( beta - alpha ) );

      } else {/* Mixing Rb */
        Ipp32f c, rho, mu, alpha, gamma;
        int i;

        i = pCom->indxIidMapped[env][bin];
        c = pTabQuantIID[ i + nIIDSteps ];

        rho = tabQuantRHO[pCom->indxIccMapped[env][bin]];
        rho = IPP_MAX(rho, 0.05f);

        if ( fabs(c - 1.f) < PS_EPS_0 ) { //if ( c == 1 )
          alpha = UMC_PI_4;
        } else {
          arg   = (2.0f*c*rho) / (c*c - 1.0f);
          alpha = (Ipp32f)( 0.5f * atan(arg) );

          if (alpha < 0.f ) alpha += UMC_PI_2; //not tested yet
        }

        arg = c + 1.0f/c;
        mu  = 1.0f + (4.0f*rho*rho-4.0f) / (arg*arg);
        mu = (Ipp32f)sqrt(mu);

        arg   = (Ipp32f)sqrt( (1.0f-mu) / (1.0f+mu) );
        gamma = (Ipp32f)atan( arg );

        bufCoeffs[0].re =  ( Ipp32f )( IPP_SQRT2 * cos(alpha) * cos(gamma) );
        bufCoeffs[1].re =  ( Ipp32f )( IPP_SQRT2 * sin(alpha) * cos(gamma) );
        bufCoeffs[2].re = -( Ipp32f )( IPP_SQRT2 * sin(alpha) * sin(gamma) );
        bufCoeffs[3].re =  ( Ipp32f )( IPP_SQRT2 * cos(alpha) * sin(gamma) );
      }

      /* first the ipd/opd are smoothed */
      if( bin < pCom->nIpdOpdBands ){
        ownIpdOpdSmooth_32fc(bufCoeffs, pCom, env, bin);
      } else {
        bufCoeffs[0].im = bufCoeffs[1].im = bufCoeffs[2].im = bufCoeffs[3].im = 0.f;
      }

      /* update */
      pPSDecState->h11Curr[bin] = bufCoeffs[0];
      pPSDecState->h12Curr[bin] = bufCoeffs[1];
      pPSDecState->h21Curr[bin] = bufCoeffs[2];
      pPSDecState->h22Curr[bin] = bufCoeffs[3];

    }//for( bin = 0; bin <

    // group < thres
    ppSuitSrcL = pPSDecState->ppHybridL;
    ppSuitSrcR = pPSDecState->ppHybridR;
    for( group = 0; group < pTDSpec->thres; group++ ){
      Ipp32s param;
      Ipp32fc deltaH[4];

      /* tuning */
      bin        = pTDSpec->pBinTab[ group ];
      len        = pCom->bufBordPos[env + 1] - pCom->bufBordPos[env];
      param      = pTDSpec->pPostBinTab[group];

      /* calc */
      // HXY
      OWN_MULC(pPSDecState->h11Prev[bin], bufH[0], param);
      OWN_MULC(pPSDecState->h12Prev[bin], bufH[1], param);
      OWN_MULC(pPSDecState->h21Prev[bin], bufH[2], param);
      OWN_MULC(pPSDecState->h22Prev[bin], bufH[3], param);

      // hxy
      OWN_MULC(pPSDecState->h11Curr[bin], bufCoeffs[0], param);
      OWN_MULC(pPSDecState->h12Curr[bin], bufCoeffs[1], param);
      OWN_MULC(pPSDecState->h21Curr[bin], bufCoeffs[2], param);
      OWN_MULC(pPSDecState->h22Curr[bin], bufCoeffs[3], param);

      /* delta */
      ippsSub_32fc(bufH, bufCoeffs, deltaH, 4);
      ippsMulC_32f_I(1.f/len, (Ipp32f*)deltaH, 4*2);

      for( i = pCom->bufBordPos[env]; i < pCom->bufBordPos[env+1]; i++ ){
        ippsAdd_32fc_I(deltaH, bufH, 4);
        ownStereoFilter_32fc(ppSuitSrcL[i], ppSuitSrcR[i], bufH,
                             pTDSpec->pStartBordTab[group], pTDSpec->pStopBordTab[group]);
      }
    }// for(group = 0; group < ...
    // group >= thres
    ppSuitSrcL = ppSrcL;
    ppSuitSrcR = ppSrcR;
    for( group = pTDSpec->thres; group < pTDSpec->nGroup; group++ ){
      Ipp32s param;
      Ipp32fc deltaH[4];

      /* tuning */
      bin        = pTDSpec->pBinTab[ group ];
      len        = pCom->bufBordPos[env + 1] - pCom->bufBordPos[env];
      param      = pTDSpec->pPostBinTab[group];

      /* calc */
      // HXY
      OWN_MULC(pPSDecState->h11Prev[bin], bufH[0], param);
      OWN_MULC(pPSDecState->h12Prev[bin], bufH[1], param);
      OWN_MULC(pPSDecState->h21Prev[bin], bufH[2], param);
      OWN_MULC(pPSDecState->h22Prev[bin], bufH[3], param);

      // hxy
      OWN_MULC(pPSDecState->h11Curr[bin], bufCoeffs[0], param);
      OWN_MULC(pPSDecState->h12Curr[bin], bufCoeffs[1], param);
      OWN_MULC(pPSDecState->h21Curr[bin], bufCoeffs[2], param);
      OWN_MULC(pPSDecState->h22Curr[bin], bufCoeffs[3], param);

      /* delta */
      ippsSub_32fc(bufH, bufCoeffs, deltaH, 4);
      ippsMulC_32f_I(1.f/len, (Ipp32f*)deltaH, 4*2);

      for( i = pCom->bufBordPos[env]; i < pCom->bufBordPos[env+1]; i++ ){
        ippsAdd_32fc_I(deltaH, bufH, 4);
        ownStereoFilter_32fc(ppSuitSrcL[i], ppSuitSrcR[i], bufH,
          pTDSpec->pStartBordTab[group], pTDSpec->pStopBordTab[group]);
      }
    }// for(group = thres; group < ...

    // end group

    //update
    for( bin = 0; bin < pTDSpec->nBin; bin++ ){
      pPSDecState->h11Prev[bin] = pPSDecState->h11Curr[bin];
      pPSDecState->h12Prev[bin] = pPSDecState->h12Curr[bin];
      pPSDecState->h21Prev[bin] = pPSDecState->h21Curr[bin];
      pPSDecState->h22Prev[bin] = pPSDecState->h22Curr[bin];
    }

    for( bin = 0; bin < pCom->nIpdOpdBands; bin++ ){
      pCom->indxIpdMapped_2[bin] = pCom->indxIpdMapped_1[bin];
      pCom->indxOpdMapped_2[bin] = pCom->indxOpdMapped_1[bin];
      pCom->indxIpdMapped_1[bin] = pCom->indxIpdMapped[env][bin];
      pCom->indxOpdMapped_1[bin] = pCom->indxOpdMapped[env][bin];
    }
  }//for( env = 0; env < ...

  return 0;//OK
}

/********************************************************************/

static Ipp32s ownSynthesisFilter_PSDec_32fc(Ipp32fc** ppSrc,
                                            Ipp32fc** pDst,
                                            Ipp32s* pResTab,
                                            Ipp32s frameSize)
{
  Ipp32s band, k, n, res, offset = 0, nBand;
  Ipp32s* pTab = NULL;

  nBand = pResTab[0];
  pTab  = pResTab + 1;

  for( band = 0; band < nBand; band++ ){
    res = pTab[band];

    for( n = 0; n < frameSize; n++ ){
      pDst[n][band].re = pDst[n][band].im = 0.f;

      for( k = 0; k < res; k++ ){
        pDst[n][band].re += ppSrc[n][offset + k].re;
        pDst[n][band].im += ppSrc[n][offset + k].im;
      }
    }
    offset += res;
  }

  return 0;//OK
}

/********************************************************************/

static Ipp32s ownUpdateBands_PSDec_32fc( sPSDecState_32f* pState, Ipp32s startBand )
{
  Ipp32s i = 0, k = 0, len = 64 - startBand;
  const Ipp32s tabNSD[3] = {3, 4, 5};

  for( i = 0; i < 3; i++ ){
    for( k = 0; k < tabNSD[i]; k++ ){
      ippsZero_32fc(pState->pppAllPassFilterMemQMF[i][k] + startBand, len);
    }
  }

  for( k = 0; k < 14; k++ ){
    ippsZero_32fc(pState->ppDelayQMF[k] + startBand, len);
  }

  return 0;//OK
}

/********************************************************************/
/*             High Level API of Parametric Stereo                  */
/********************************************************************/

Ipp32s psdecDecode_32fc(Ipp32fc** ppSrcL,
                        Ipp32fc** ppSrcR,
                        sPSDecState_32f* pPSDecState,
                        Ipp32s startBand)
{
  Ipp32fc** ppHybL = pPSDecState->ppHybridL;
  Ipp32fc** ppHybR = pPSDecState->ppHybridR;
  Ipp32s*   pTabRes[2] = {(Ipp32s*)tabResBand1020,  (Ipp32s*)tabResBand34};
  sTDSpec*  pConfSet[2]= {(sTDSpec*)&setConfBand20, (sTDSpec*)&setConfBand34};
  Ipp32s    indx;

  indx = ( CONFIG_HA_BAND1020 == pPSDecState->comState.flag_HAconfig ) ? 0 : 1;

  /* step [1] - HybridAnalysis */
  ownAnalysisFilter_PSDec_32fc(ppSrcL,
                               ppHybL,
                               &(pPSDecState->haState),
                               pPSDecState->comState.flag_HAconfig );

  /* step [2] - decorrelation */
  ownDecorrelation_PSDec_32fc(ppSrcL, ppSrcR, pConfSet[indx], pPSDecState);

  /* step [3] - stereo processing */
  ownStereoProcessing_PSDec_32fc(ppSrcL, ppSrcR, pConfSet[indx], pPSDecState);

  /* step [4] - */
  /* L channel */
  ownSynthesisFilter_PSDec_32fc(ppHybL, ppSrcL, pTabRes[indx], 32);
  /* R channel */
  ownSynthesisFilter_PSDec_32fc(ppHybR, ppSrcR, pTabRes[indx], 32);

  /* step [x] - update HA_CONFIG */
  pPSDecState->comState.flag_HAconfigPrev = pPSDecState->comState.flag_HAconfig;
  ownUpdateBands_PSDec_32fc( pPSDecState, startBand );

  return 0;//OK
}

/********************************************************************/
/* EOF */

#endif //UMC_ENABLE_AAC_AUDIO_DECODER

