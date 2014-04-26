
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChHash.cpp : implementation of the CRunChHashclass.
// CRunChHashclass processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunChHash.h"
#include "ParmChHashDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunChHash::CRunChHash()
{

}

CRunChHash::~CRunChHash()
{

}

BOOL CRunChHash::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   return TRUE;
}

void CRunChHash::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmChHashDlg *pDlg = (CParmChHashDlg*)parmDlg;
   if (save) {
      m_Func = pDlg->m_Func;
      m_result = pDlg->m_result;
   } else {
      pDlg->m_Func = m_Func;
      pDlg->m_result = m_result;
   }
}

BOOL CRunChHash::CallIpp(BOOL bMessage)
{
   CParmChHashDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunChHash::SetValues(CParmChHashDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   CippsRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunChHash::CallIppFunction()
{
   FUNC_CH_CALL(ippsHash_8u32u,  ((Ipp8u*)pSrc, len, &m_result ))
   FUNC_CH_CALL(ippsHash_16u32u, ((Ipp16u*)pSrc, len, &m_result ))
   FUNC_CH_CALL(ippsHashSJ2_8u32u, ((Ipp8u*)pSrc, len, &m_result))
   FUNC_CH_CALL(ippsHashSJ2_16u32u, ((Ipp16u*)pSrc, len, &m_result))
   FUNC_CH_CALL(ippsHashMSCS_8u32u, ((Ipp8u*)pSrc, len, &m_result))
   FUNC_CH_CALL(ippsHashMSCS_16u32u, ((Ipp16u*)pSrc, len, &m_result))

   return stsNoFunction;
}
