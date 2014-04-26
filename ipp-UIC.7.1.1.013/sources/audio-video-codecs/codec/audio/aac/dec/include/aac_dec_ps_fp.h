/*//////////////////////////////////////////////////////////////////////////////
//
//                  INTEL CORPORATION PROPRIETARY INFORMATION
//     This software is supplied under the terms of a license agreement or
//     nondisclosure agreement with Intel Corporation and may not be copied
//     or disclosed except in accordance with the terms of that agreement.
//          Copyright(c) 2007-2012 Intel Corporation. All Rights Reserved.
//
*/

#ifndef __AACDEC_PS_FP_H__
#define __AACDEC_PS_FP_H__

#include "align.h"
/* PARAMETRIC STEREO */
#include "ps_dec_settings.h"
#include "ps_dec_struct.h"

/********************************************************************/

#ifdef  __cplusplus
extern  "C" {
#endif

  typedef struct{

    Ipp32fc mMemBuf[5][12];
    Ipp32fc mTmpBuf[32][12];

  } sHybridAnalysis;


  typedef struct{

    Ipp32fc* ppHybridL[32];
    Ipp32fc* ppHybridR[32];

    sHybridAnalysis haState;

    sPSDecComState comState;

    /* tables */
    /* vector fi_fract */
    Ipp32fc pFracDelayLenTab34[32];
    Ipp32fc pFracDelayLenTab1020[12];
    Ipp32fc pFracDelayLenTabQMF[64];

    /* matrix Q_fract_allpass */
    Ipp32fc* ppFracDelayLenTab34[32];
    Ipp32fc* ppFracDelayLenTab1020[32];
    //Ipp32fc* ppFracDelayLenTabQMF[32];
    Ipp32fc* ppFracDelayLenTabQMF[64];

    /* buffers */
    Ipp32f bufPeakDecayNrg[34];
    Ipp32f bufPeakDecayNrgPrev[34];
    Ipp32f bufPeakDecayDiffNrgPrev[34];

    /* decorrelate */
    Ipp32fc*  ppDelaySubQMF[14];
    Ipp32fc*  ppDelayQMF[14];
    Ipp32fc** pppAllPassFilterMemSubQMF[3];
    Ipp32fc** pppAllPassFilterMemQMF[3];

    /* spec index buffers */
    Ipp32s delayIndxMem[3];

    Ipp32s bufIndexDelayQMF[64];
    Ipp32s bufNumSampleDelayQMF[64];

    /* mapping params */
    Ipp32fc h11Prev[34];
    Ipp32fc h12Prev[34];
    Ipp32fc h21Prev[34];
    Ipp32fc h22Prev[34];

    /* stereo processing */
    Ipp32fc h11Curr[34];
    Ipp32fc h12Curr[34];
    Ipp32fc h21Curr[34];
    Ipp32fc h22Curr[34];

  } sPSDecState_32f;

void   psDecoderGetSize(Ipp32s *pSize);
void   psdecUpdateMemMap(sPSDecState_32f* pPSState, Ipp32s mShift);

Ipp32s psInitDecoder_32f(sPSDecState_32f* pState, void* pMem);
Ipp32s psdecDecode_32fc(Ipp32fc** ppSrcL, Ipp32fc** ppSrcR, sPSDecState_32f* pPSDecState, Ipp32s startBand);
Ipp32s psFreeDecoder_32f(/* sPSDecState_32f* pState*/);

/* AnalysisFilter */
Ipp32s ownAnalysisFilter_PSDec_Kernel_v2_32fc(const Ipp32fc* pSrc, Ipp32fc ppDst[32][12], int len, int flag34);

#ifdef  __cplusplus
}
#endif

/********************************************************************/
#endif             /* __AACDEC_PS_FP_H__ */
/* EOF */
