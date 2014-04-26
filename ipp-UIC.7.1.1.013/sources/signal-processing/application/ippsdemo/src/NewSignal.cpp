/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// NewSignal.cpp: implementation of the CNewSignal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "NewSignal.h"
#include "NewSignalDlg.h"
#include "Vector.h"
#include "ippsRun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define STS_CALL(Func, Args) { \
   IppStatus status = Func Args; \
   if (status != ippStsNoErr) { \
      CippsRun::IppErrorMessage(#Func, status); \
      if (status < 0) return FALSE; \
   } \
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewSignal::CNewSignal()
{
   m_Form = newJaehne;
   m_Type = pp16s;
   m_Length = 512;
   m_Offset = 0;

   InitTone();
   InitTrian();
   InitJaehne();
   InitRandU();
   InitRandN();
   InitSpike();
}

CNewSignal::~CNewSignal()
{

}


void CNewSignal::InitTone() {
   int form = newTone;
   CDlgItem item;
   CValue value;

   InitMagn(value);
   item.Set(CDlgItem::varReal,value,"Magnitude");
   m_FormItems[form].Add(item);

   value.Init(pp32f, 0.02);
   item.Set(value,"Frequency (0 : 1)");
   m_FormItems[form].Add(item);

   value.Init(pp32f, 0);
   item.Set(value,"Phase (0 : 2)","PI");
   m_FormItems[form].Add(item);
}

BOOL CNewSignal::SetTone(CVector* pVector) 
{
   int form = newTone;
   CValue magnValue = m_FormItems[form][0].m_value;
   double rfreq = m_FormItems[form][1].m_value.GetDouble();
   double phase64 = m_FormItems[form][2].m_value.GetDouble() * (double)IPP_PI;
   float  phase32 = (float)phase64;
   double magn;
   magnValue.Get(magn);

   void* pDst = pVector->GetData();
   switch (m_Type) {
   case pp8u:
   case pp8s:   
   case pp16u:   
      pDst = ippMalloc(m_Length<<2); break;
   case pp8sc:  
      pDst = ippMalloc(m_Length<<3); break;
   case pp32u:  
   case pp32s:  
   case pp64s:  
      pDst = ippMalloc(m_Length<<3); break;
   case pp32sc: 
   case pp64sc: 
      pDst = ippMalloc(m_Length<<4); break;
   }

   switch (m_Type) {
   case pp8u:
   case pp8s:
   case pp16u:   
   case pp32f:
      STS_CALL(ippsTone_Direct_32f,(
         (Ipp32f*)pDst, m_Length, (Ipp32f)magn,
         (float)rfreq, &phase32, ippAlgHintFast))
      break;
   case pp8sc:
   case pp32fc:
      STS_CALL(ippsTone_Direct_32fc,(
         (Ipp32fc*)pDst, m_Length, (Ipp32f)magn,
         (float)rfreq, &phase32, ippAlgHintFast))
      break;
   case pp32u:
   case pp32s:
   case pp64s:
   case pp64f:
      STS_CALL(ippsTone_Direct_64f,(
         (Ipp64f*)pDst, m_Length, (Ipp64f)magn,
         rfreq, &phase64, ippAlgHintFast))
      break;
   case pp32sc:
   case pp64sc:
   case pp64fc:
      STS_CALL(ippsTone_Direct_64fc,(
         (Ipp64fc*)pDst, m_Length, (Ipp64f)magn,
         rfreq, &phase64, ippAlgHintFast))
      break;
   case pp16s:
      STS_CALL(ippsTone_Direct_16s,(
         (Ipp16s*)pDst, m_Length, (Ipp16s)magn,
         (float)rfreq, &phase32, ippAlgHintFast))
      break;
   case pp16sc:
      STS_CALL(ippsTone_Direct_16sc,(
         (Ipp16sc*)pDst, m_Length, (Ipp16s)magn,
         (float)rfreq, &phase32, ippAlgHintFast))
      break;
   default:
      return FALSE;
   }
   int len = (m_Type & PP_CPLX) ? m_Length*2 : m_Length;
   switch (m_Type) {
   case pp8u:
      STS_CALL(ippsAddC_32f_I,(
         -IPP_MIN_8S, (Ipp32f*)pDst, m_Length))
      STS_CALL(ippsConvert_32f8u_Sfs,(
         (Ipp32f*)pDst, (Ipp8u*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp16u:   
      STS_CALL(ippsAddC_32f_I,(
         -IPP_MIN_16S, (Ipp32f*)pDst, m_Length))
      STS_CALL(ippsConvert_32f16u_Sfs,(
         (Ipp32f*)pDst, (Ipp16u*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp8s:   
   case pp8sc: 
      STS_CALL(ippsConvert_32f8s_Sfs,(
         (Ipp32f*)pDst, (Ipp8s*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp32s:  
   case pp32sc:
      STS_CALL(ippsConvert_64f32s_Sfs,(
         (Ipp64f*)pDst, (Ipp32s*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp32u:  
      {
         Ipp32u* p32u = (Ipp32u*)pVector->GetData();
         Ipp64f* p64f = (Ipp64f*)pDst;
         for (int i=0; i<len; i++)
            p32u[i] = (Ipp32u)(p64f[i] - IPP_MIN_32S);
      }
      break;
   case pp64s:  
   case pp64sc:  
      {
         Ipp64s* p64s = (Ipp64s*)pVector->GetData();
         Ipp64f* p64f = (Ipp64f*)pDst;
         for (int i=0; i<len; i++)
            p64s[i] = (Ipp64s)(p64f[i]);
      }
      break;
   }
   if (pDst != pVector->GetData()) ippFree(pDst);
   return TRUE;
}

void CNewSignal::InitTrian() {
   int form = newTrian;
   CDlgItem item;
   CValue value;

   InitMagn(value);
   item.Set(CDlgItem::varReal,value,"Magnitude");
   m_FormItems[form].Add(item);

   value.Init(pp32f, 0.02);
   item.Set(value,"Frequency (0 : 1)");
   m_FormItems[form].Add(item);

   value.Init(pp32f, 0);
   item.Set(value,"Phase (0 : 2)","PI");
   m_FormItems[form].Add(item);

   value.Init(pp32f, 0);
   item.Set(value,"Asymmetry (-1 : 1)","PI");
   m_FormItems[form].Add(item);
}

BOOL CNewSignal::SetTrian(CVector* pVector) 
{
   int form = newTrian;
   CValue magnValue = m_FormItems[form][0].m_value;
   double rfreq = m_FormItems[form][1].m_value.GetDouble();
   double phase64 = m_FormItems[form][2].m_value.GetDouble() * (double)IPP_PI;
   float  phase32 = (float)phase64;
   double asym  = m_FormItems[form][3].m_value.GetDouble() * (double)IPP_PI;
   double magn;
   magnValue.Get(magn);

   void* pDst = pVector->GetData();
   switch (m_Type) {
   case pp8u:
   case pp8s:   
   case pp16u:   
      pDst = ippMalloc(m_Length<<2); break;
   case pp8sc:  
      pDst = ippMalloc(m_Length<<3); break;
   case pp32u:  
   case pp32s:  
   case pp64s:  
      pDst = ippMalloc(m_Length<<3); break;
   case pp32sc: 
   case pp64sc: 
      pDst = ippMalloc(m_Length<<4); break;
   }

   switch (m_Type) {
   case pp8u:
   case pp8s:
   case pp16u:   
   case pp32f:
      STS_CALL(ippsTriangle_Direct_32f,(
         (Ipp32f*)pDst, m_Length, (Ipp32f)magn,
         (float)rfreq, (float)asym, &phase32))
      break;
   case pp8sc:
   case pp32fc:
      STS_CALL(ippsTriangle_Direct_32fc,(
         (Ipp32fc*)pDst, m_Length, (Ipp32f)magn,
         (float)rfreq, (float)asym, &phase32))
      break;
   case pp32u:
   case pp32s:
   case pp64s:
   case pp64f:
      STS_CALL(ippsTriangle_Direct_64f,(
         (Ipp64f*)pDst, m_Length, (Ipp64f)magn,
         (float)rfreq, (float)asym, &phase64))
      break;
   case pp32sc:
   case pp64sc:
   case pp64fc:
      STS_CALL(ippsTriangle_Direct_64fc,(
         (Ipp64fc*)pDst, m_Length, (Ipp64f)magn,
         (float)rfreq, (float)asym, &phase64))
      break;
   case pp16s:
      STS_CALL(ippsTriangle_Direct_16s,(
         (Ipp16s*)pDst, m_Length, (Ipp16s)magn,
         (float)rfreq, (float)asym, &phase32))
      break;
   case pp16sc:
      STS_CALL(ippsTriangle_Direct_16sc,(
         (Ipp16sc*)pDst, m_Length, (Ipp16s)magn,
         (float)rfreq, (float)asym, &phase32))
      break;
   default:
      return FALSE;
   }

   int len = (m_Type & PP_CPLX) ? m_Length*2 : m_Length;
   switch (m_Type) {
   case pp8u:
      STS_CALL(ippsAddC_32f_I,(
         -IPP_MIN_8S, (Ipp32f*)pDst, m_Length))
      STS_CALL(ippsConvert_32f8u_Sfs,(
         (Ipp32f*)pDst, (Ipp8u*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp16u:   
      STS_CALL(ippsAddC_32f_I,(
         -IPP_MIN_16S, (Ipp32f*)pDst, m_Length))
      STS_CALL(ippsConvert_32f16u_Sfs,(
         (Ipp32f*)pDst, (Ipp16u*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp8s:   
   case pp8sc: 
      STS_CALL(ippsConvert_32f8s_Sfs,(
         (Ipp32f*)pDst, (Ipp8s*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp32s:  
   case pp32sc:
      STS_CALL(ippsConvert_64f32s_Sfs,(
         (Ipp64f*)pDst, (Ipp32s*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp32u:  
      {
         Ipp32u* p32u = (Ipp32u*)pVector->GetData();
         Ipp64f* p64f = (Ipp64f*)pDst;
         for (int i=0; i<len; i++)
            p32u[i] = (Ipp32u)(p64f[i] - IPP_MIN_32S);
      }
      break;
   case pp64s:  
   case pp64sc:  
      {
         Ipp64s* p64s = (Ipp64s*)pVector->GetData();
         Ipp64f* p64f = (Ipp64f*)pDst;
         for (int i=0; i<len; i++)
            p64s[i] = (Ipp64s)(p64f[i]);
      }
      break;
   }
   if (pDst != pVector->GetData()) ippFree(pDst);
   return TRUE;
}

void CNewSignal::InitJaehne() {
   int form = newJaehne;
   CDlgItem item;
   CValue value;

   InitMagn(value);
   item.Set(CDlgItem::varReal,value,"Magnitude");
   m_FormItems[form].Add(item);
}

BOOL CNewSignal::SetJaehne(CVector* pVector) 
{
   int form = newJaehne;
   CValue magnValue = m_FormItems[form][0].m_value;
   double magn;
   magnValue.Get(magn);

   void* pDst = pVector->GetData();
   switch (m_Type) {
   case pp8sc : pDst = ippMalloc(m_Length); break;
   case pp16sc: pDst = ippMalloc(m_Length<<1); break;
   case pp32sc: pDst = ippMalloc(m_Length<<2); break;
   case pp32fc: pDst = ippMalloc(m_Length<<2); break;
   case pp64s:
   case pp64sc:
   case pp64fc: pDst = ippMalloc(m_Length<<3); break;
   }

   switch (m_Type) {
   case pp8u:
      STS_CALL(ippsVectorJaehne_8u,((Ipp8u*) pDst, m_Length, (Ipp8u)magn ))
      break;
   case pp8s:
   case pp8sc :
      STS_CALL(ippsVectorJaehne_8s,((Ipp8s*) pDst, m_Length, (Ipp8s)magn ))
      break;
   case pp16u:
      STS_CALL(ippsVectorJaehne_16u,((Ipp16u*)pDst, m_Length, (Ipp16u)magn ))
      break;
   case pp16s:
   case pp16sc:
      STS_CALL(ippsVectorJaehne_16s,((Ipp16s*)pDst, m_Length, (Ipp16s)magn ))
      break;
   case pp32u:
      STS_CALL(ippsVectorJaehne_32u,((Ipp32u*)pDst, m_Length, (Ipp32u)magn ))
      break;
   case pp32s:
   case pp32sc:
      STS_CALL(ippsVectorJaehne_32s,((Ipp32s*)pDst, m_Length, (Ipp32s)magn ))
      break;
   case pp32f:
   case pp32fc:
      STS_CALL(ippsVectorJaehne_32f,((Ipp32f*)pDst, m_Length, (Ipp32f)magn))
      break;
   case pp64f:
   case pp64fc:
   case pp64s:
   case pp64sc:
      STS_CALL(ippsVectorJaehne_64f,((Ipp64f*)pDst, m_Length, (Ipp64f)magn))
      break;
   default:
      return FALSE;
   }

   switch (m_Type) {
   case pp8sc :
      {
         Ipp8s * pReal = (Ipp8s*)pDst;
         Ipp8sc* pCplx = (Ipp8sc*)pVector->GetData();
         for (int i=0; i<m_Length; i++) {
            (pCplx[i]).re = pReal[i];
            (pCplx[i]).im = pReal[i];
         }
      }
      break;
   case pp16sc:
      STS_CALL(ippsRealToCplx_16s,( (Ipp16s*)pDst,
         (Ipp16s*)pDst, (Ipp16sc*)pVector->GetData(), m_Length ))
      break;
   case pp32sc:
   case pp32fc:
      STS_CALL(ippsRealToCplx_32f,( (Ipp32f*)pDst,
         (Ipp32f*)pDst, (Ipp32fc*)pVector->GetData(), m_Length ))
      break;
   case pp64fc:
      STS_CALL(ippsRealToCplx_64f,( (Ipp64f*)pDst,
         (Ipp64f*)pDst, (Ipp64fc*)pVector->GetData(), m_Length ))
      break;
   case pp64s:
      {
         Ipp64f* p64f = (Ipp64f*)pDst;
         Ipp64s* p64s = (Ipp64s*)pVector->GetData();
         for (int i=0; i<m_Length; i++)
            p64s[i] = (Ipp64s)p64f[i];
      }
      break;
   case pp64sc:
      {
         Ipp64f * pReal = (Ipp64f*)pDst;
         Ipp64sc* pCplx = (Ipp64sc*)pVector->GetData();
         for (int i=0; i<m_Length; i++) {
            (pCplx[i]).re = (Ipp64s)pReal[i];
            (pCplx[i]).im = (Ipp64s)pReal[i];
         }
      }
      break;
   }
   if (pDst != pVector->GetData()) ippFree(pDst);
   return TRUE;
}

void CNewSignal::InitRandU() {
   int form = newRandU;
   CDlgItem item;
   CValue value;

   InitMin(value);
   item.Set(CDlgItem::varReal,value,"Low");
   m_FormItems[form].Add(item);

   InitMax(value);
   item.Set(CDlgItem::varReal,value,"High");
   m_FormItems[form].Add(item);

   value.Init(pp32u, 0);
   item.Set(value,"Seed",0,INT_MAX);
   m_FormItems[form].Add(item);
}

void CNewSignal::InitRandN() {
   int form = newRandN;
   CDlgItem item;
   CValue value;

   InitMean(value);
   item.Set(CDlgItem::varReal,value,"Mean");
   m_FormItems[form].Add(item);

   InitMagn(value);
   item.Set(CDlgItem::varReal,value,"Deviation");
   m_FormItems[form].Add(item);

   value.Init(pp32u, 0);
   item.Set(value,"Seed",0,INT_MAX);
   m_FormItems[form].Add(item);
}

BOOL CNewSignal::SetRandU(CVector* pVector) 
{
   int form = newRandU;
   CValue lowValue = m_FormItems[form][0].m_value;
   CValue highValue = m_FormItems[form][1].m_value;
   Ipp32u seed = (Ipp32u)(m_FormItems[form][2].m_value);
   double low;
   double high;
   lowValue.Get(low);
   highValue.Get(high);

   void* pDst = pVector->GetData();
   void* pImag = NULL;
   switch (m_Type) {
   case pp8sc:  
   case pp32fc:  
      pImag = ippMalloc(m_Length<<2); 
   case pp8u:
   case pp8s:   
   case pp16u:   
      pDst = ippMalloc(m_Length<<2); 
      break;
   case pp16sc:  
      pImag = ippMalloc(m_Length<<1); 
      pDst = ippMalloc(m_Length<<1); 
      break;
   case pp32sc: 
   case pp64sc: 
   case pp64fc: 
      pImag = ippMalloc(m_Length<<3); 
   case pp32u:  
   case pp32s:  
   case pp64s:  
      pDst = ippMalloc(m_Length<<3); 
      break;
   }

   switch (m_Type) {
   case pp8u:
   case pp8s:
   case pp16u:   
   case pp32f:
   case pp8sc:
   case pp32fc:
      STS_CALL(ippsRandUniform_Direct_32f,(
         (Ipp32f*)pDst, m_Length, (Ipp32f)low, (Ipp32f)high, &seed))
      if (pImag) {
         STS_CALL(ippsRandUniform_Direct_32f,(
            (Ipp32f*)pImag, m_Length, (Ipp32f)low, (Ipp32f)high, &seed))
         Ipp32fc* pReal = (Ipp32fc*)pDst;
         pDst = (m_Type == pp32fc) ? 
            pVector->GetData() : ippMalloc(m_Length<<3);
         STS_CALL(ippsRealToCplx_32f,(
            (Ipp32f*)pReal,(Ipp32f*)pImag,(Ipp32fc*)pDst,m_Length))
         ippFree(pReal);
      }
      break;
   case pp32u:
   case pp32s:
   case pp64s:
   case pp64f:
   case pp32sc:
   case pp64sc:
   case pp64fc:
      STS_CALL(ippsRandUniform_Direct_64f,(
         (Ipp64f*)pDst, m_Length, (Ipp64f)low, (Ipp64f)high, &seed))
      if (pImag) {
         STS_CALL(ippsRandUniform_Direct_64f,(
            (Ipp64f*)pImag, m_Length, (Ipp64f)low, (Ipp64f)high, &seed))
         Ipp64fc* pReal = (Ipp64fc*)pDst;
         pDst = (m_Type == pp64fc) ? 
            pVector->GetData() : ippMalloc(m_Length<<4);
         STS_CALL(ippsRealToCplx_64f,(
            (Ipp64f*)pReal,(Ipp64f*)pImag,(Ipp64fc*)pDst,m_Length))
         ippFree(pReal);
      }
      break;
   case pp16s:
   case pp16sc:
      STS_CALL(ippsRandUniform_Direct_16s,(
         (Ipp16s*)pDst, m_Length, (Ipp16s)low, (Ipp16s)high, &seed))
      if (pImag) {
         STS_CALL(ippsRandUniform_Direct_16s,(
            (Ipp16s*)pImag, m_Length, (Ipp16s)low, (Ipp16s)high, &seed))
         Ipp16sc* pReal = (Ipp16sc*)pDst;
         pDst = pVector->GetData();
         STS_CALL(ippsRealToCplx_16s,(
            (Ipp16s*)pReal,(Ipp16s*)pImag,(Ipp16sc*)pDst,m_Length))
         ippFree(pReal);
      }
      break;
   default:
      return FALSE;
   }
   int len = (m_Type & PP_CPLX) ? m_Length*2 : m_Length;
   switch (m_Type) {
   case pp8u:
      STS_CALL(ippsConvert_32f8u_Sfs,(
         (Ipp32f*)pDst, (Ipp8u*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp16u:   
      STS_CALL(ippsConvert_32f16u_Sfs,(
         (Ipp32f*)pDst, (Ipp16u*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp8s:   
   case pp8sc: 
      STS_CALL(ippsConvert_32f8s_Sfs,(
         (Ipp32f*)pDst, (Ipp8s*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp32s:  
   case pp32sc:
      STS_CALL(ippsConvert_64f32s_Sfs,(
         (Ipp64f*)pDst, (Ipp32s*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp32u:  
      {
         Ipp32u* p32u = (Ipp32u*)pVector->GetData();
         Ipp64f* p64f = (Ipp64f*)pDst;
         for (int i=0; i<len; i++)
            p32u[i] = (Ipp32u)(p64f[i] - IPP_MIN_32S);
      }
      break;
   case pp64s:  
   case pp64sc:  
      {
         Ipp64s* p64s = (Ipp64s*)pVector->GetData();
         Ipp64f* p64f = (Ipp64f*)pDst;
         for (int i=0; i<len; i++)
            p64s[i] = (Ipp64s)(p64f[i]);
      }
      break;
   }
   if (pDst != pVector->GetData()) ippFree(pDst);
   if (pImag) ippFree(pImag);
   return TRUE;
}

BOOL CNewSignal::SetRandN(CVector* pVector) 
{
   int form = newRandN;
   CValue lowValue = m_FormItems[form][0].m_value;
   CValue highValue = m_FormItems[form][1].m_value;
   Ipp32u seed = (Ipp32u)(m_FormItems[form][2].m_value);
   double low;
   double high;
   lowValue.Get(low);
   highValue.Get(high);

   void* pDst = pVector->GetData();
   void* pImag = NULL;
   switch (m_Type) {
   case pp8sc:  
   case pp32fc:  
      pImag = ippMalloc(m_Length<<2); 
   case pp8u:
   case pp8s:   
   case pp16u:   
      pDst = ippMalloc(m_Length<<2); 
      break;
   case pp16sc:  
      pImag = ippMalloc(m_Length<<1); 
      pDst = ippMalloc(m_Length<<1); 
      break;
   case pp32sc: 
   case pp64sc: 
   case pp64fc: 
      pImag = ippMalloc(m_Length<<3); 
   case pp32u:  
   case pp32s:  
   case pp64s:  
      pDst = ippMalloc(m_Length<<3); 
      break;
   }

   switch (m_Type) {
   case pp8u:
   case pp8s:
   case pp16u:   
   case pp32f:
   case pp8sc:
   case pp32fc:
      STS_CALL(ippsRandGauss_Direct_32f,(
         (Ipp32f*)pDst, m_Length, (Ipp32f)low, (Ipp32f)high, &seed))
      if (pImag) {
         STS_CALL(ippsRandGauss_Direct_32f,(
            (Ipp32f*)pImag, m_Length, (Ipp32f)low, (Ipp32f)high, &seed))
         Ipp32fc* pReal = (Ipp32fc*)pDst;
         pDst = (m_Type == pp32fc) ? 
            pVector->GetData() : ippMalloc(m_Length<<3);
         STS_CALL(ippsRealToCplx_32f,(
            (Ipp32f*)pReal,(Ipp32f*)pImag,(Ipp32fc*)pDst,m_Length))
         ippFree(pReal);
      }
      break;
   case pp32u:
   case pp32s:
   case pp64s:
   case pp64f:
   case pp32sc:
   case pp64sc:
   case pp64fc:
      STS_CALL(ippsRandGauss_Direct_64f,(
         (Ipp64f*)pDst, m_Length, (Ipp64f)low, (Ipp64f)high, &seed))
      if (pImag) {
         STS_CALL(ippsRandGauss_Direct_64f,(
            (Ipp64f*)pImag, m_Length, (Ipp64f)low, (Ipp64f)high, &seed))
         Ipp64fc* pReal = (Ipp64fc*)pDst;
         pDst = (m_Type == pp64fc) ? 
            pVector->GetData() : ippMalloc(m_Length<<4);
         STS_CALL(ippsRealToCplx_64f,(
            (Ipp64f*)pReal,(Ipp64f*)pImag,(Ipp64fc*)pDst,m_Length))
         ippFree(pReal);
      }
      break;
   case pp16s:
   case pp16sc:
      STS_CALL(ippsRandGauss_Direct_16s,(
         (Ipp16s*)pDst, m_Length, (Ipp16s)low, (Ipp16s)high, &seed))
      if (pImag) {
         STS_CALL(ippsRandGauss_Direct_16s,(
            (Ipp16s*)pImag, m_Length, (Ipp16s)low, (Ipp16s)high, &seed))
         Ipp16sc* pReal = (Ipp16sc*)pDst;
         pDst = pVector->GetData();
         STS_CALL(ippsRealToCplx_16s,(
            (Ipp16s*)pReal,(Ipp16s*)pImag,(Ipp16sc*)pDst,m_Length))
         ippFree(pReal);
      }
      break;
   default:
      return FALSE;
   }
   int len = (m_Type & PP_CPLX) ? m_Length*2 : m_Length;
   switch (m_Type) {
   case pp8u:
      STS_CALL(ippsConvert_32f8u_Sfs,(
         (Ipp32f*)pDst, (Ipp8u*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp16u:   
      STS_CALL(ippsConvert_32f16u_Sfs,(
         (Ipp32f*)pDst, (Ipp16u*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp8s:   
   case pp8sc: 
      STS_CALL(ippsConvert_32f8s_Sfs,(
         (Ipp32f*)pDst, (Ipp8s*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp32s:  
   case pp32sc:
      STS_CALL(ippsConvert_64f32s_Sfs,(
         (Ipp64f*)pDst, (Ipp32s*)pVector->GetData(),
         len, ippRndNear, 0))
      break;
   case pp32u:  
      {
         Ipp32u* p32u = (Ipp32u*)pVector->GetData();
         Ipp64f* p64f = (Ipp64f*)pDst;
         for (int i=0; i<len; i++)
            p32u[i] = (Ipp32u)(p64f[i] - IPP_MIN_32S);
      }
      break;
   case pp64s:  
   case pp64sc:  
      {
         Ipp64s* p64s = (Ipp64s*)pVector->GetData();
         Ipp64f* p64f = (Ipp64f*)pDst;
         for (int i=0; i<len; i++)
            p64s[i] = (Ipp64s)(p64f[i]);
      }
      break;
   }
   if (pDst != pVector->GetData()) ippFree(pDst);
   if (pImag) ippFree(pImag);
   return TRUE;
}

void CNewSignal::InitSpike() {
   int form = newSpike;
   CDlgItem item;
   CValue value;

   InitMagn(value);
   item.Set(CDlgItem::varReal,value,"Magnitude");
   m_FormItems[form].Add(item);

   value.Init(pp32s, 5);
   item.Set(value,"Width",1,INT_MAX);
   m_FormItems[form].Add(item);

   value.Init(ppNONE);
   item.Set(value);
   m_FormItems[form].Add(item);
   m_FormItems[form].Add(item);

   value.Init(pp32s, 100);
   item.Set(value,"",0,INT_MAX);
   m_FormItems[form].Add(item);
}

BOOL CNewSignal::SetSpike(CVector* pVector) 
{
   int form = newSpike;
   CValue magnValue = m_FormItems[form][0].m_value;
   int width = (int)(m_FormItems[form][1].m_value);
   int offset = (int)(m_FormItems[form][4].m_value);
   double magn;
   magnValue.Get(magn);

   if (!m_Offset) offset = (m_Length - width) >> 1;
   for (int i=0; i < m_Length; i++) {
      if (offset <= i && i < offset + width) 
         pVector->Set(i, magn, magn);
      else
         pVector->Set(i, 0, 0);
   }
   return TRUE;
}

void CNewSignal::InitMagn(CValue& val) 
{
   val.Init(pp8u ,IPP_MAX_8S>>3);
   val.Init(pp8s ,IPP_MAX_8S>>3);
   val.Init(pp16u,IPP_MAX_16S>>3);
   val.Init(pp16s,IPP_MAX_16S>>3);
   val.Init(pp32u,IPP_MAX_32S>>3);
   val.Init(pp32s,IPP_MAX_32S>>3);
   val.Init64s(pp64s,IPP_MAX_64S>>3);
   val.Init(pp32f,1);
   val.Init(pp64f,1);
   val.Init(m_Type);
}

void CNewSignal::InitMin(CValue& val) 
{
   val.Init(pp8u ,0);
   val.Init(pp8s ,IPP_MIN_8S);
   val.Init(pp16u,0);
   val.Init(pp16s,IPP_MIN_16S);
   val.Init(pp32u,0);
   val.Init(pp32s,IPP_MIN_32S);
   val.Init64s(pp64s,IPP_MIN_64S);
   val.Init(pp32f,-1);
   val.Init(pp64f,-1);
   val.Init(m_Type);
}

void CNewSignal::InitMax(CValue& val) 
{
   val.Init(pp8u ,IPP_MAX_8U);
   val.Init(pp8s ,IPP_MAX_8S);
   val.Init(pp16u,IPP_MAX_16U);
   val.Init(pp16s,IPP_MAX_16S);
   val.Init(pp32u,IPP_MAX_32U);
   val.Init(pp32s,IPP_MAX_32S);
   val.Init64s(pp64s,IPP_MAX_64S);
   val.Init(pp32f,1);
   val.Init(pp64f,1);
   val.Init(m_Type);
}

void CNewSignal::InitMean(CValue& val) 
{
   val.Init(pp8u ,IPP_MAX_8S);
   val.Init(pp8s ,0);
   val.Init(pp16u,IPP_MAX_16S);
   val.Init(pp16s,0);
   val.Init(pp32u,IPP_MAX_32S);
   val.Init(pp32s,0);
   val.Init(pp64s,0);
   val.Init(pp32f,0);
   val.Init(pp64f,0);
   val.Init(m_Type);
}

BOOL CNewSignal::Dialog() 
{
   CNewSignalDlg dlg;
   dlg.m_Type = m_Type;
   dlg.m_Form = m_Form;
   dlg.m_LenStr.Format("%d", m_Length);
   dlg.m_Offset = m_Offset;
   for (int i=0; i<newNUM; i++)
      dlg.m_FormItems[i] = m_FormItems[i];

   if (dlg.DoModal() != IDOK) return FALSE;

   m_Type = dlg.m_Type;
   m_Form = dlg.m_Form;
   m_Length = atoi(dlg.m_LenStr);
   m_Offset = dlg.m_Offset;
   m_FormItems[m_Form] = dlg.m_FormItems[m_Form];
   return TRUE;
}

BOOL CNewSignal::CreateVector(CVector* pVector) 
{
   pVector->Init(m_Type, m_Length);
   switch (m_Form) {
   case newTone  : return SetTone  (pVector);
   case newTrian : return SetTrian (pVector);
   case newJaehne: return SetJaehne(pVector);
   case newRandU : return SetRandU (pVector);
   case newRandN : return SetRandN (pVector);
   case newSpike : return SetSpike (pVector);
   }
   return FALSE;
}

