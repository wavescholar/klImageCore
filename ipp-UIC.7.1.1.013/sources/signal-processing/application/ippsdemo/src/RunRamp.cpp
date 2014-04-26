/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunRamp.cpp : implementation of the CRunRamp class.
// CRunRamp class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunRamp.h"
#include "ParmRampDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunRamp::CRunRamp()
{
   m_offset.Init(pp32f, 0);
   m_slope.Init(pp32f, 0.1);

}

BOOL CRunRamp::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   return TRUE;
}

CParamDlg* CRunRamp::CreateDlg() { 
   return new CParmRampDlg;
}

void CRunRamp::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmRampDlg *pDlg = (CParmRampDlg*)parmDlg;
   if (save) {
      m_offset.Set(pDlg->m_Val0Str);      
      m_slope.Set(pDlg->m_Val1Str);      
   } else {
      m_offset.Get(pDlg->m_Val0Str);      
      m_slope.Get(pDlg->m_Val1Str);      
   }
}

IppStatus CRunRamp::CallIppFunction()
{
   FUNC_CALL(ippsVectorRamp_8u,  ((Ipp8u*) pSrc, len, m_offset, m_slope))
   FUNC_CALL(ippsVectorRamp_8s,  ((Ipp8s*) pSrc, len, m_offset, m_slope))
   FUNC_CALL(ippsVectorRamp_16u, ((Ipp16u*)pSrc, len, m_offset, m_slope))
   FUNC_CALL(ippsVectorRamp_16s, ((Ipp16s*)pSrc, len, m_offset, m_slope))
   FUNC_CALL(ippsVectorRamp_32u, ((Ipp32u*)pSrc, len, m_offset, m_slope))
   FUNC_CALL(ippsVectorRamp_32s, ((Ipp32s*)pSrc, len, m_offset, m_slope))
   FUNC_CALL(ippsVectorRamp_32f, ((Ipp32f*)pSrc, len, m_offset, m_slope))
   FUNC_CALL(ippsVectorRamp_64f, ((Ipp64f*)pSrc, len, m_offset, m_slope))

   return stsNoFunction;
}

CString CRunRamp::GetHistoryParms()
{
   CMyString parm;
   parm << m_offset.String() << ", " << m_slope.String();
   return parm;
}

