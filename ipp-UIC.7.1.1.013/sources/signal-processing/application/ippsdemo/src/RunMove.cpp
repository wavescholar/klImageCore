/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunMove.cpp : implementation of the CRunMove class.
// CRunMove class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "ippsdemodoc.h"
#include "RunMove.h"
#include "ParmMoveDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunMove::CRunMove() 
{
   m_len = 200;
   m_offsetSrc = 0;
   m_offsetDst = 100;
}

CRunMove::~CRunMove()
{

}

BOOL CRunMove::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   return TRUE;
}

CParamDlg* CRunMove::CreateDlg()
{
   return new CParmMoveDlg;
}

void CRunMove::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmMoveDlg *pDlg = (CParmMoveDlg*)parmDlg;
   if (save) {
      m_len        = atoi(pDlg->m_LenStr      );
      m_offsetSrc  = atoi(pDlg->m_OffsetSrcStr);
      m_offsetDst  = atoi(pDlg->m_OffsetDstStr);
   } else {
      pDlg->m_LenStr      .Format("%d", m_len      );
      pDlg->m_OffsetSrcStr.Format("%d", m_offsetSrc);
      pDlg->m_OffsetDstStr.Format("%d", m_offsetDst);
   }
}

BOOL CRunMove::BeforeCall()
{
   int item = m_pDocSrc->GetVector()->ItemSize();
   if (m_len > len - m_offsetSrc) m_len = len - m_offsetSrc;
   if (m_len > len - m_offsetDst) m_len = len - m_offsetDst;
   return TRUE;
}

IppStatus CRunMove::CallIppFunction()
{
   FUNC_CALL(ippsMove_8u, ((Ipp8u*)pSrc + m_offsetSrc, (Ipp8u*)pSrc + m_offsetDst, m_len ))
   FUNC_CALL(ippsMove_16s, ((Ipp16s*)pSrc + m_offsetSrc, (Ipp16s*)pSrc + m_offsetDst, m_len ))
   FUNC_CALL(ippsMove_16sc, ((Ipp16sc*)pSrc + m_offsetSrc, (Ipp16sc*)pSrc + m_offsetDst, m_len ))
   FUNC_CALL(ippsMove_32f, ((Ipp32f*)pSrc + m_offsetSrc, (Ipp32f*)pSrc + m_offsetDst, m_len ))
   FUNC_CALL(ippsMove_32fc, ((Ipp32fc*)pSrc + m_offsetSrc, (Ipp32fc*)pSrc + m_offsetDst, m_len ))
   FUNC_CALL(ippsMove_64f, ((Ipp64f*)pSrc + m_offsetSrc, (Ipp64f*)pSrc + m_offsetDst, m_len ))
   FUNC_CALL(ippsMove_64fc, ((Ipp64fc*)pSrc + m_offsetSrc, (Ipp64fc*)pSrc + m_offsetDst, m_len ))
   FUNC_CALL(ippsMove_32s, ((Ipp32s*)pSrc + m_offsetSrc, (Ipp32s*)pSrc + m_offsetDst, m_len ))
   FUNC_CALL(ippsMove_32sc, ((Ipp32sc*)pSrc + m_offsetSrc, (Ipp32sc*)pSrc + m_offsetDst, m_len ))
   FUNC_CALL(ippsMove_64s, ((Ipp64s*)pSrc + m_offsetSrc, (Ipp64s*)pSrc + m_offsetDst, m_len ))
   FUNC_CALL(ippsMove_64sc, ((Ipp64sc*)pSrc + m_offsetSrc, (Ipp64sc*)pSrc + m_offsetDst, m_len ))
   return stsNoFunction;
}

CString CRunMove::GetHistoryInfo() 
{
   CMyString info;
   return info << "[" << m_offsetSrc << " " << m_offsetSrc + m_len 
      << "]->[" << m_offsetDst << " " << m_offsetDst + m_len << "]";
}

