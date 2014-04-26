/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewTaps.cpp: implementation of the CNewTaps class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsRun.h"
#include "NewTaps.h"
#include "NewTapsDlg.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewTaps::CNewTaps()
{
   m_Type = pp32f;
   m_Filter = 0;
   m_length = 27;
   m_freqLow = 0.2;
   m_freqHigh = 0.3;
}

CNewTaps::~CNewTaps()
{

}

BOOL CNewTaps::Dialog()
{
   CNewTapsDlg dlg;
   dlg.m_Type     = m_Type    ;
   dlg.m_Filter   = m_Filter  ;
   dlg.m_length   = m_length  ;
   dlg.m_freqLow  = m_freqLow ;
   dlg.m_freqHigh = m_freqHigh;
   if (dlg.DoModal() != IDOK) return FALSE;
   m_Type     = dlg.m_Type    ;
   m_Filter   = dlg.m_Filter  ;
   m_length   = dlg.m_length  ;
   m_freqLow  = dlg.m_freqLow ;
   m_freqHigh = dlg.m_freqHigh;
   return TRUE;
}

static BOOL SetFIRTaps(Ipp64f* pTaps, int len, Ipp64f freqLow, Ipp64f freqHigh, int filter)
{
   IppWinType winType = ippWinHamming;
   switch (filter) {
   case CNewTapsDlg::FILTER_LOW:
      CHECK_CALL(ippsFIRGenLowpass_64f,(freqLow, pTaps, len, winType, ippTrue));
      break;
   case CNewTapsDlg::FILTER_HIGH:
      CHECK_CALL(ippsFIRGenHighpass_64f,(freqHigh, pTaps, len, winType, ippTrue));
      break;
   case CNewTapsDlg::FILTER_BANDPASS:
      CHECK_CALL(ippsFIRGenBandpass_64f,(freqLow, freqHigh, pTaps, len, winType, ippTrue));
      break;
   case CNewTapsDlg::FILTER_BANDSTOP:
      CHECK_CALL(ippsFIRGenBandstop_64f,(freqLow, freqHigh, pTaps, len, winType, ippTrue));
      break;
   }
   return TRUE;
}

static Ipp64f* SetFIRTaps_NoZero(Ipp64f* pTaps, int len, Ipp64f freqLow, Ipp64f freqHigh, int filter)
{
   Ipp64f eps = 1.e-12;
   int offset = 0;
   if (!SetFIRTaps(pTaps, len, freqLow, freqHigh, filter)) return NULL;
//   if (pTaps[0] > eps) 
      return pTaps;
   if (!SetFIRTaps(pTaps, len + 2, freqLow, freqHigh, filter)) return NULL;
   return pTaps + 1;
}


int scaleTapsTo32s(Ipp64f* pTaps, int len)
{
   double max, min;

   ippsMax_64f(pTaps, len, &max);
   ippsMin_64f(pTaps, len, &min);
   if (max < -min) max = -min;

   int tapsFactor = 0;
   double mpy = 1;
   if (max > IPP_MAX_32S) {
       while (max > IPP_MAX_32S) {
           tapsFactor++;
           max *= 0.5;
           mpy *= 0.5;
       }
   } else {
       while ((max < IPP_MAX_32S) && (tapsFactor > -17)) {
           tapsFactor--;
           max += max;
           mpy += mpy;
       }
       tapsFactor++;
       mpy *= 0.5;
   }
   ippsMulC_64f_I(mpy, pTaps, len);
   return tapsFactor;
}

BOOL CNewTaps::CreateVector(CVector* pVector) 
{
   const Ipp64f Q15 = IPP_MAX_16S;
   int len = (m_Filter == CNewTapsDlg::FILTER_NONE) ? 0 : m_length;
   pVector->Init(m_Type, len);
   if (len <= 0) return TRUE;
   Ipp64f* pBuffer = new Ipp64f[len + 2];
   Ipp64f* pTaps = SetFIRTaps_NoZero(pBuffer, len, m_freqLow, m_freqHigh, m_Filter);
   if (!pTaps) {
      delete[] pBuffer;
      return FALSE;
   }
   void* pData = pVector->GetData();
   void* pRe = NULL;
   void* pIm = NULL;
   switch (m_Type) {
   case pp64f:
      ippsCopy_64f(pTaps, (Ipp64f*)pData, len);
      break;
   case pp64fc:
      pIm = new Ipp64f[len];
      ippsZero_64f((Ipp64f*)pIm,len);
      ippsRealToCplx_64f(pTaps, (Ipp64f*)pIm, (Ipp64fc*)pData, len);
      break;
   case pp32f:
      ippsConvert_64f32f(pTaps, (Ipp32f*)pData, len);
      break;
   case pp32fc:
      pRe = new Ipp32f[len];
      pIm = new Ipp32f[len];
      ippsConvert_64f32f(pTaps, (Ipp32f*)pRe, len);
      ippsZero_32f((Ipp32f*)pIm,len);
      ippsRealToCplx_32f((Ipp32f*)pRe, (Ipp32f*)pIm, (Ipp32fc*)pData, len);
      break;
   case pp32s:
      pVector->SetTapsFactor(scaleTapsTo32s(pTaps, len));
      ippsConvert_64f32s_Sfs(pTaps, (Ipp32s*)pData, len, ippRndNear, 0);
      break;
   case pp32sc:
      pVector->SetTapsFactor(scaleTapsTo32s(pTaps, len));
      pIm = new Ipp32s[len];
      pRe = new Ipp32s[len];
      ippsConvert_64f32s_Sfs(pTaps, (Ipp32s*)pRe, len, ippRndNear, 0);
      ippsZero_32f((Ipp32f*)pIm,len);
      ippsRealToCplx_32f((Ipp32f*)pRe, (Ipp32f*)pIm, (Ipp32fc*)pData, len);
      break;
   case pp16s:
      ippsMulC_64f_I(Q15, pTaps, len);
      pIm = new Ipp32f[len];
      ippsConvert_64f32f(pTaps, (Ipp32f*)pIm, len);
      ippsConvert_32f16s_Sfs((Ipp32f*)pIm, (Ipp16s*)pData, len, ippRndNear, 0);
      break;
   case pp16sc:
      ippsMulC_64f_I(Q15, pTaps, len);
      pIm = new Ipp32f[len];
      pRe = new Ipp16s[len];
      ippsConvert_64f32f(pTaps, (Ipp32f*)pIm, len);
      ippsConvert_32f16s_Sfs((Ipp32f*)pIm, (Ipp16s*)pRe, len, ippRndNear, 0);
      delete[] pIm;
      pIm = new Ipp16s[len];
      ippsZero_16s((Ipp16s*)pIm,len);
      ippsRealToCplx_16s((Ipp16s*)pRe, (Ipp16s*)pIm, (Ipp16sc*)pData, len);
      break;
   }
   if (pRe) delete[] pRe;
   if (pIm) delete[] pIm;
   return TRUE;
}

