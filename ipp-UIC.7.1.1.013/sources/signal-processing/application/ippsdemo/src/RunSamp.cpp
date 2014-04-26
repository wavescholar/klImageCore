/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunSamp.cpp : implementation of the CRunSamp class.
// CRunSamp class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunSamp.h"
#include "ParmSampDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunSamp::CRunSamp()
{
   m_factor = 2;
   m_phase  = 0;
   m_dstLen = 0;

   m_phaseSave = 0;
}

CRunSamp::~CRunSamp()
{

}

CParamDlg* CRunSamp::CreateDlg()
{
   return new CParmSampDlg;
}

void CRunSamp::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmSampDlg *pDlg = (CParmSampDlg*)parmDlg;
   if (save) {
      m_factor    = pDlg->m_factor   ;
      m_phase     = pDlg->m_phase    ;
      m_phaseSave = m_phase;
   } else {
      pDlg->m_factor    = m_factor   ;
      pDlg->m_phase     = m_phase    ;
   }
}

int CRunSamp::GetDstLength()
{
   int length = CippsRun::GetDstLength();
   if (m_Func.BaseName() == "SampleUp")
      length = length*m_factor;
   else
      length = (length - m_phase + 1) / m_factor;
   if (length < 1) length = 1;
   return length;
}

IppStatus CRunSamp::CallIppFunction()
{
   FUNC_CALL(ippsSampleUp_32f, ((Ipp32f*)pSrc,  len,
                                             (Ipp32f*)pDst, &m_dstLen,
                                             m_factor,   &m_phase))
   FUNC_CALL(ippsSampleUp_32fc, ((Ipp32fc*)pSrc,  len,
                                              (Ipp32fc*)pDst, &m_dstLen,
                                              m_factor,   &m_phase))
   FUNC_CALL(ippsSampleUp_64f, ((Ipp64f*)pSrc,  len,
                                             (Ipp64f*)pDst, &m_dstLen,
                                             m_factor,   &m_phase))
   FUNC_CALL(ippsSampleUp_64fc, ((Ipp64fc*)pSrc,  len,
                                              (Ipp64fc*)pDst, &m_dstLen,
                                              m_factor,   &m_phase))
   FUNC_CALL(ippsSampleUp_16s, ((Ipp16s*)pSrc,  len,
                                             (Ipp16s*)pDst, &m_dstLen,
                                             m_factor,   &m_phase))
   FUNC_CALL(ippsSampleUp_16sc, ((Ipp16sc*)pSrc,  len,
                                              (Ipp16sc*)pDst, &m_dstLen,
                                              m_factor,   &m_phase))
   FUNC_CALL(ippsSampleDown_32f, ((Ipp32f*)pSrc,  len,
                                               (Ipp32f*)pDst, &m_dstLen,
                                               m_factor,   &m_phase))
   FUNC_CALL(ippsSampleDown_32fc, ((Ipp32fc*)pSrc,  len,
                                                (Ipp32fc*)pDst, &m_dstLen,
                                                m_factor,   &m_phase))
   FUNC_CALL(ippsSampleDown_64f, ((Ipp64f*)pSrc,  len,
                                               (Ipp64f*)pDst, &m_dstLen,
                                               m_factor,   &m_phase))
   FUNC_CALL(ippsSampleDown_64fc, ((Ipp64fc*)pSrc,  len,
                                                (Ipp64fc*)pDst, &m_dstLen,
                                                m_factor,   &m_phase))
   FUNC_CALL(ippsSampleDown_16s, ((Ipp16s*)pSrc,  len,
                                               (Ipp16s*)pDst, &m_dstLen,
                                               m_factor,   &m_phase))
   FUNC_CALL(ippsSampleDown_16sc, ((Ipp16sc*)pSrc,  len,
                                                (Ipp16sc*)pDst, &m_dstLen,
                                                m_factor,   &m_phase))
   return stsNoFunction;
}

CString CRunSamp::GetHistoryParms()
{
   CMyString parm;
   parm << m_dstLen << ", "
        << m_factor << ", "
        << "(" << m_phaseSave << ") " << m_phase;
   return parm;
}
