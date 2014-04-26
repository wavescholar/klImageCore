
/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2006-2012 Intel Corporation. All Rights Reserved.
//
*/

#include "umc_config.h"
#if defined (UMC_ENABLE_AAC_AUDIO_ENCODER)

#include <math.h>
/* HEAAC profile*/
#include "aaccmn_const.h"
#include "sbr_enc_api_fp.h"
#include "sbr_enc_own_fp.h"
#include "aac_status.h"

/* AYAlog */
#ifdef SBR_NEED_LOG
#include "sbr_enc_dbg.h"
#endif

/********************************************************************/

typedef enum eId eID_CH_EL;

/********************************************************************/

static const Ipp32f MAX_THRESHOLD = 128000.0f;

/********************************************************************/

void sbrencCalcNrg(Ipp32fc bufX[][64], Ipp32fc bufXFlip[][32], Ipp32f bufNrg[][64], Ipp32f bufNrgFlip[][64])
{
  Ipp32s i = 0, j = 0;


  ippsCopy_32f(bufNrg[32], bufNrg[0], 32*64);
  ippsPowerSpectr_32fc(bufX[0], bufNrg[32], 32*64);

  /* transpose */
  for (i=0; i<32; i++) {
    for(j=0; j<64; j++){
      bufNrgFlip[j][i] = bufNrg[i][j];
      bufXFlip[j][i] = bufX[i][j];
    }
  }

  for (i=32; i<64; i++) {
    for(j=0; j<64; j++){
      //bufXFlip[j][i] = bufX[i][j];
      bufNrgFlip[j][i] = bufNrg[i][j];
    }
  }

  return;
}

/********************************************************************/
/*                  TRANSIENT DETECTOR                              */
/********************************************************************/


static
void ownTransientDetect_SBR_32f(const Ipp32f* pSrc, Ipp32f* pThr, Ipp32f* pTran)
{
  Ipp32s n,d;
  Ipp32f invThr;
  Ipp32f delta_1, delta_2, delta_3;
  Ipp32f tmp_thr = 0.f;
  Ipp32f a = 0.f, b = 0.f;
  /* speed optimization */
  Ipp32s posL1, posL2, posL3, posR1, posR2, posR3;
  Ipp32f sumL1, sumL2, sumL3, sumR1, sumR2, sumR3;


  ippsStdDev_32f( pSrc+16, 48, &tmp_thr, ippAlgHintFast);
  /* threshold */
  a = MAX_THRESHOLD;
  b = 0.66f * pThr[0] + 0.34f * tmp_thr;

  pThr[0] = IPP_MAX( a,  b );

  invThr = 1.f / pThr[0];

  /* ***************************************************** */
  /* step1: n = 16 */
  n = 16;
  posL1 = 14; posR1 = 16;
  posL2 = 14; posR2 = 18;
  posL3 = 12; posR3 = 18;

  sumL1 = 0.5f * ( pSrc[posL1] + pSrc[posL1+1] );
  sumR1 = 0.5f * ( pSrc[posR1] + pSrc[posR1+1] );
  delta_1 = sumR1 - sumL1;

  if( delta_1 > pThr[0] ){
    pTran[n] += (delta_1 * invThr - 1.0f);
  }

  sumL2 = 0.5f * ( pSrc[posL2] + pSrc[posL2+1] );
  sumR2 = 0.5f * ( pSrc[posR2] + pSrc[posR2+1] );

  delta_2 = sumR2 - sumL2 + delta_1;
  if(delta_2 > pThr[0]) {
    pTran[n] += (delta_2 * invThr - 1.0f);
  }

  sumL3 = 0.5f * ( pSrc[posL3] + pSrc[posL3+1] );
  sumR3 = 0.5f * ( pSrc[posR3] + pSrc[posR3+1] );

  delta_3 = sumR3 - sumL3 + delta_2;
  if(delta_3 > pThr[0]) {
    pTran[n] += (delta_3 * invThr - 1.0f);
  }

  /* ***************************************************** */

  for(n=17; n<61; n += 2){

    /* [n] */
    d = 1;
    posL1 += 2;
    posR1 += 2;

    sumL1 = 0.5f * ( pSrc[posL1] + pSrc[posL1+1] );
    sumR1 = 0.5f * ( pSrc[posR1] + pSrc[posR1+1] );
    delta_1 = sumR1 - sumL1;

    if( delta_1 > pThr[0] ){
      Ipp32f tmp = (delta_1 * invThr - 1.0f);

      pTran[n]   += tmp;
      pTran[n+1] += tmp;
    }

    delta_2 = sumR2 - sumL2 + delta_1;

    if(delta_2 > pThr[0]) {
      pTran[n] += (delta_2 * invThr - 1.0f);
    }

    d = 3;
    posL3 += 2;
    posR3 += 2;

    sumL3 = 0.5f * ( pSrc[posL3] + pSrc[posL3+1] );
    sumR3 = 0.5f * ( pSrc[posR3] + pSrc[posR3+1] );

    delta_3 = sumR3 - sumL3 + delta_2;

    if(delta_3 > pThr[0]) {
      pTran[n] += (delta_3 * invThr - 1.0f);
    }

    /* *********** [n+1] *************** */
    d = 2;

    posL2 += 2;
    posR2 += 2;

    sumL2 = 0.5f * ( pSrc[posL2] + pSrc[posL2+1] );
    sumR2 = 0.5f * ( pSrc[posR2] + pSrc[posR2+1] );

    delta_2 = sumR2 - sumL2 + delta_1;
    if(delta_2 > pThr[0]) {
      pTran[n+1] += (delta_2 * invThr - 1.0f);
    }

    delta_3 = sumR3 - sumL3 + delta_2;
    if(delta_3 > pThr[0]) {
      pTran[n+1] += (delta_3 * invThr - 1.0f);
    }
  }

  return;
}

/********************************************************************/

static void sbrencCalcTranParams(Ipp32f* pTransient, Ipp32s* tranPos, Ipp32s* tranFlag)
{
  Ipp32s n;
  Ipp32f* a = pTransient;

  for(n=8; n<40; n++){

    if( (a[n] < 0.9f * a[n-1]) && (a[n-1] > 203.125f) ){
      /* if YES then up-date params & quit */
      *tranPos = ((n-8)>>1);
      *tranFlag = 1;
      break;
    }

  }
}

/********************************************************************/

void sbrencTransientDetector(Ipp32f bufNrgFlip[][64], Ipp32f* pThr,
                             Ipp32f* pTransient,  Ipp32s* tranPos, Ipp32s* tranFlag)
{
  /* ********************** *
   * 3GPP SBR Enc Spec -
   * array <t> - pThr
   * array <a> - pTransient
   * ********************** */
  int k;

  //sbrencCalcThreshold( bufNrg, pThr );

  /* AYAlog */
#ifdef SBR_NEED_LOG
  {
    Ipp32s myI;

    fprintf(logFile, "t = \n");
    for(myI=0; myI<64; myI++){
      fprintf(logFile, " %15.10f ", pThr[myI]);
    }

    fprintf(logFile, "\n");
  }
#endif

  /* up-date buffer */
  ippsCopy_32f(pTransient + 32, pTransient, 16);
  ippsZero_32f(pTransient + 16, 32);

  for(k=0; k<64; k++){
    ownTransientDetect_SBR_32f(bufNrgFlip[k], pThr + k, pTransient);
  }

  /* AYAlog */
#ifdef SBR_NEED_LOG
  {
    Ipp32s myI;

    fprintf(logFile, "a = \n");
    for(myI=8; myI<40; myI++){
      fprintf(logFile, " %15.10f ", pTransient[myI]);
    }

    fprintf(logFile, "\n");
  }
#endif

  *tranPos = *tranFlag = 0;
  sbrencCalcTranParams( pTransient, tranPos, tranFlag );

  /* AYAlog */
#ifdef SBR_NEED_LOG
  fprintf(logFile, "%i %i\n", *tranPos, *tranFlag);
#endif

}

/********************************************************************/
/*                   FRAME SPLITTER                                 */
/********************************************************************/

static Ipp32f sbrencCalcNrgLowBand(Ipp32f bufNrg[][64], Ipp32s sbrStartBand)
{
  Ipp32f nrgLow = 0.0f;
  Ipp32s i, n;

  for(i=0; i<sbrStartBand; i++){
    for(n=16; n<48; n++){
      nrgLow += bufNrg[n][i];
    }
  }

  return nrgLow;
}

/********************************************************************/

static Ipp32f sbrencCalcNrgHighBand(Ipp32f bufNrg[][64], Ipp32s* pTabF, Ipp32s nBand,
                                    Ipp32f* pNrgHighBand0, Ipp32f* pNrgHighBand1)
{
  Ipp32f nrg = 0.0f;
  Ipp32s p, iStart, iEnd, i, j;

  /* "durty" ref code in 3GPP SBR Enc spec */

  /* zero */
  for(i=0; i<nBand; i++){
    pNrgHighBand0[i] = 0.0f;
    pNrgHighBand1[i] = 0.0f;
  }

  /* e_high(p,l): l=0 (j=0-7), l=1(j=8-15) */
  for(j=0; j<16; j++){
    for(p=0; p<nBand; p++){
      iStart = pTabF[p];
      iEnd   = pTabF[p + 1];

      for(i=iStart; i<iEnd; i++){
        pNrgHighBand0[p] += bufNrg[j][i];
        pNrgHighBand1[p] += bufNrg[16+j][i];
      }
    }
  }

  /* e_curr_high : [0, nBand] or [0, (nBand-1)] */
  nrg = 0.0f;
  for(p=0; p<nBand; p++){
    nrg += (pNrgHighBand0[p] + pNrgHighBand1[p]);
  }

  return nrg;

}

/********************************************************************/

Ipp32s sbrencDecisionSplitFrame(Ipp32f* nrgHighBand0, Ipp32f* nrgHighBand1,
                                Ipp32s nBand, Ipp32f nrgTotal, const Ipp32f splitThr)
{
  Ipp32s p;
  Ipp32f dvec, delta = 0.0f;
  Ipp32s splitFlag = 0;
  Ipp32f tmp = 0.f;

  for(p=0; p<nBand; p++){

    dvec = (nrgHighBand1[p] + 8.0e6f) / (nrgHighBand0[p] + 8.0e6f);
    dvec = (Ipp32f)fabs( log(dvec) );

    /* debug */
    tmp = nrgHighBand1[p] + nrgHighBand0[p] + 16.0e6f;
    tmp /= nrgTotal;

    //delta += dvec * sqrt( nrgHighBand1[p] + nrgHighBand0[p] + 16.0e6f );
    delta += dvec * (Ipp32f)sqrt( tmp );
  }

  //delta = delta * (1.0f / nrgTotal);

  /* decision */
  if ( delta > splitThr ){
    splitFlag = 1;
  }

  return splitFlag;
}

/********************************************************************/

Ipp32s sbrencFrameSplitter(Ipp32f bufNrg[][64], Ipp32s* pTabF, Ipp32s nBand, Ipp32f* nrgPrevLow, const Ipp32f splitThr)
{
#if !defined(ANDROID)
  Ipp32f nrgHighBand0[30], nrgHighBand1[30];
#else
  static Ipp32f nrgHighBand0[30];
  static Ipp32f nrgHighBand1[30];
#endif
  Ipp32f nrgCurrLow  = 0.f;
  Ipp32f nrgCurrHigh = 0.f;
  Ipp32f nrgTotal;
  Ipp32s splitFlag = 0;

  Ipp32s sbrStartBand = pTabF[0];

  /* energy LowBand */
  nrgCurrLow = sbrencCalcNrgLowBand(bufNrg, sbrStartBand);

#ifdef SBR_NEED_LOG
  fprintf(logFile, "newLowbandEnergy =  %15.10f\n", nrgCurrLow);
#endif

  /* energy HighBand */
  nrgCurrHigh = sbrencCalcNrgHighBand(bufNrg, pTabF, nBand, nrgHighBand0, nrgHighBand1);

#ifdef SBR_NEED_LOG
  fprintf(logFile, "HighBandEnergy =  %15.10f\n", nrgCurrHigh);
#endif

  /* average */
  nrgTotal = 2.0f + nrgCurrHigh + 0.5f * ( nrgCurrLow + *nrgPrevLow );

  /* decision */
  splitFlag = sbrencDecisionSplitFrame( nrgHighBand0, nrgHighBand1, nBand, nrgTotal, splitThr );

  /* re-place */
  *nrgPrevLow = nrgCurrLow;

  return splitFlag;
}

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

