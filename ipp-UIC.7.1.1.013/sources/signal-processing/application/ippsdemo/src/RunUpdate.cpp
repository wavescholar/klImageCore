/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunUpdate.cpp : implementation of the CRunUpdate class.
// CRunUpdate class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ippsDemo.h"
#include "RunUpdate.h"
#include "ParmUpdateDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunUpdate::CRunUpdate()
{
   m_hint = ippAlgHintFast;
   m_alpha.Init(pp16s, 1);
   m_shift.Init(pp32s, 1);
   m_srcDst.Init(pp32s, 2);
   m_srcDstIn.Init(pp32s, 2);
}

CRunUpdate::~CRunUpdate()
{

}

BOOL CRunUpdate::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   m_alpha.Init(func,argSRC);
   m_srcDstIn.Init(func,argDST);
   m_srcDst.Init(func,argDST);
   m_shift.Init(pp32s);
   return TRUE;
}

void CRunUpdate::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmUpdateDlg *pDlg = (CParmUpdateDlg*)parmDlg;
   if (save) {
      m_alpha.Set(pDlg->m_AlphaStr);
      m_shift.Set(pDlg->m_ShiftStr);
      m_hint = (IppHintAlgorithm)pDlg->m_hint;
      m_srcDstIn.Set(pDlg->m_InValStr);
      m_srcDst = m_srcDstIn;
   } else {
      m_alpha.Get(pDlg->m_AlphaStr);
      m_shift.Get(pDlg->m_ShiftStr);
      m_srcDstIn.Get(pDlg->m_InValStr);
      m_srcDst.Get(pDlg->m_OutValStr);
      pDlg->m_hint = (int)m_hint;
   }
}

BOOL CRunUpdate::CallIpp(BOOL bMessage)
{
   CParmUpdateDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunUpdate::SetValue(CParmUpdateDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   CippsRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunUpdate::CallIppFunction()
{
   FUNC_CALL(ippsUpdateLinear_16s32s_I,((Ipp16s*)pSrc,len,
       (Ipp32s*)m_srcDst, (int)m_shift,(Ipp16s)m_alpha, m_hint))
   FUNC_CALL(ippsUpdatePower_16s32s_I,((Ipp16s*)pSrc,len,
       (Ipp32s*)m_srcDst, (int)m_shift,(Ipp16s)m_alpha, m_hint))

   return stsNoFunction;
}

CString CRunUpdate::GetHistoryParms()
{
   CMyString parm;
   parm << m_srcDstIn << ", " << m_shift << ", " << m_alpha << ", " << m_hint;
   return parm;
}
