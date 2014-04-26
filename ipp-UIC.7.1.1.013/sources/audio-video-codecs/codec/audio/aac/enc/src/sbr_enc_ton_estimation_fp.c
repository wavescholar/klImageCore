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

#include "sbr_enc_api_fp.h"
#include "sbr_enc_own_fp.h"
#include "sbr_freq_tabs.h"
#include "sbr_enc_settings.h"

#ifdef SBR_NEED_LOG
#include "sbr_enc_dbg.h"
#endif

#include "aac_status.h"

/********************************************************************/

#define IS_ZERO(val)  !(val*val > 0.f)

/********************************************************************/

/* Auto correlation coefficients */

typedef struct {
  Ipp32f  f00;
  Ipp32f  f11;
  Ipp32fc f01;
  Ipp32fc f02;
  Ipp32fc f12;
  Ipp32f  f22;

  Ipp32f  det;

} sAutoCorrCoefs;

/********************************************************************/

static
Ipp32s ownGetPredictCoefs(sAutoCorrCoefs* acc, Ipp32fc* pCoefs)
{
#if !defined(ANDROID)
  const Ipp32fc zeroCoef = {0.0f, 0.0f};
#else
  static const Ipp32fc zeroCoef = {0.0f, 0.0f};
#endif

  //if ( acc->det == 0 ) {
  if ( IS_ZERO(acc->det) ) {

    pCoefs[0] = pCoefs[1] = zeroCoef;
    return 0;//OK
  }

  pCoefs[1].re = ( acc->f01.re * acc->f12.re - acc->f01.im * acc->f12.im - acc->f02.re * acc->f11 ) / acc->det;
  pCoefs[1].im = ( acc->f01.im * acc->f12.re + acc->f01.re * acc->f12.im - acc->f02.im * acc->f11 ) / acc->det;

  //if ( acc->f11 == 0 ) {
  if ( IS_ZERO(acc->f11) ) {

    pCoefs[0] = zeroCoef;
  } else {
    pCoefs[0].re = - ( acc->f01.re + pCoefs[1].re * acc->f12.re + pCoefs[1].im * acc->f12.im ) / acc->f11;
    pCoefs[0].im = - ( acc->f01.im + pCoefs[1].im * acc->f12.re - pCoefs[1].re * acc->f12.im ) / acc->f11;
  }

  return 0; //OK
}

/********************************************************************/

static
Ipp32s ownCalcCovMatrixElements_32f(const Ipp32fc* pSrc, sAutoCorrCoefs* acc)
{
  Ipp32s n;
  Ipp32s nStart = 2;
  Ipp32s nEnd = 16;
  Ipp32f rel = 1.0f / (1.0f + EPS);

  ippsZero_8u((Ipp8u*)acc, sizeof(sAutoCorrCoefs));

  for ( n = nStart; n < nEnd; n++ ) {


    acc->f00    += pSrc[n].re * pSrc[n].re + pSrc[n].im * pSrc[n].im;

    acc->f11    += pSrc[n-1].re * pSrc[n-1].re + pSrc[n-1].im * pSrc[n-1].im;

    acc->f01.re += pSrc[n].re * pSrc[n-1].re + pSrc[n].im * pSrc[n-1].im;

    acc->f01.im += pSrc[n].im * pSrc[n-1].re - pSrc[n].re * pSrc[n-1].im;

    acc->f02.re += pSrc[n].re * pSrc[n-2].re + pSrc[n].im * pSrc[n-2].im;

    acc->f02.im += pSrc[n].im * pSrc[n-2].re - pSrc[n].re * pSrc[n-2].im;

    acc->f22    += pSrc[n-2].re * pSrc[n-2].re + pSrc[n-2].im * pSrc[n-2].im;

    acc->f12.re += pSrc[n-1].re * pSrc[n-2].re + pSrc[n-1].im * pSrc[n-2].im;

    acc->f12.im += pSrc[n-1].im * pSrc[n-2].re - pSrc[n-1].re * pSrc[n-2].im;
  }

  acc->det = acc->f11 * acc->f22 - rel * (acc->f12.re * acc->f12.re + acc->f12.im * acc->f12.im);

  return 0;
}

/********************************************************************/

IppStatus ownEstimateTNR_SBR_32f(const Ipp32fc* pSrc, Ipp32f* pTNR0, Ipp32f* pTNR1,
       Ipp32f* pMeanNrg )
{
  sAutoCorrCoefs acc;
#if !defined(ANDROID)
  Ipp32fc coefsPredict[2];

  Ipp32f* pBufTNR[2];
  Ipp32f coefs[2] = {0.f, 0.f};
#else
  static Ipp32fc coefsPredict[2];

  static Ipp32f* pBufTNR[2];
  static Ipp32f coefs[2] = {0.f, 0.f};
#endif
  Ipp32s l;

  pBufTNR[0] = pTNR0;
  pBufTNR[1] = pTNR1;

  for(l = 0; l <= 1; l++){

    ownCalcCovMatrixElements_32f( pSrc + 16*l, &acc);

    ownGetPredictCoefs( &acc, coefsPredict );

    pBufTNR[l][0]   = 0.0f;//-1.f;

    if( !IS_ZERO(acc.f00) ){

      Ipp32f tmp;

      tmp = coefsPredict[0].re * acc.f01.re + coefsPredict[0].im * acc.f01.im +
        coefsPredict[1].re * acc.f02.re + coefsPredict[1].im * acc.f02.im;

      tmp /= -acc.f00;
      pBufTNR[l][0] = tmp / ((Ipp32f)1.0 - tmp +(Ipp32f)EPS);

    }
    coefs[l] = acc.f00;
  }

  pMeanNrg[0] = 0.5f*(coefs[0] + coefs[1]);

  return ippStsNoErr; //OK
}

/********************************************************************/
/* EOF */

#endif //UMC_ENABLE_AAC_AUDIO_ENCODER

