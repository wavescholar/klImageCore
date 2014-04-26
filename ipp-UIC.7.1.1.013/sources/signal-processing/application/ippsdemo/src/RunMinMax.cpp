/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunMinMax.cpp : implementation of the CRunMinMax class.
// CRunMinMax class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "RunMinMax.h"
#include "ParmMinMaxDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunMinMax::CRunMinMax()
{

}

CRunMinMax::~CRunMinMax()
{

}

BOOL CRunMinMax::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   (m_value[0]).Init(func,argDST);
   (m_value[1]).Init(func,argDST);
   return TRUE;
}

void CRunMinMax::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmMinMaxDlg *pDlg = (CParmMinMaxDlg*)parmDlg;
   if (save) {
      CString sType = m_Func.TypeName();
      if (pDlg->m_IsIdx)
         m_Func = "ippsMinMaxIndx_" + sType;
      else
         m_Func = "ippsMinMax_" + sType;
      for (int i=0; i<2; i++)
         m_value[i].Init(m_Func,argDST);
   } else {
      pDlg->m_IsIdx = m_Func.Found("Indx");
      for (int i=0; i<2; i++) {
         pDlg->m_IdxStr[i].Format("%d", m_index[i]);
         m_value[i].Get(pDlg->m_ReStr[i], pDlg->m_ImStr[i]);
      }
   }
}

BOOL CRunMinMax::CallIpp(BOOL bMessage)
{
   CParmMinMaxDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunMinMax::SetValues(CParmMinMaxDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   CippsRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunMinMax::CallIppFunction()
{
   FUNC_CALL(ippsMinMax_64f,((Ipp64f*)pSrc, len, (Ipp64f*)(m_value[0]), (Ipp64f*)(m_value[1])))
   FUNC_CALL(ippsMinMax_32f,((Ipp32f*)pSrc, len, (Ipp32f*)(m_value[0]), (Ipp32f*)(m_value[1])))
   FUNC_CALL(ippsMinMax_32s,((Ipp32s*)pSrc, len, (Ipp32s*)(m_value[0]), (Ipp32s*)(m_value[1])))
   FUNC_CALL(ippsMinMax_32u,((Ipp32u*)pSrc, len, (Ipp32u*)(m_value[0]), (Ipp32u*)(m_value[1])))
   FUNC_CALL(ippsMinMax_16s,((Ipp16s*)pSrc, len, (Ipp16s*)(m_value[0]), (Ipp16s*)(m_value[1])))
   FUNC_CALL(ippsMinMax_16u,((Ipp16u*)pSrc, len, (Ipp16u*)(m_value[0]), (Ipp16u*)(m_value[1])))
   FUNC_CALL(ippsMinMax_8u, ((Ipp8u*) pSrc, len, (Ipp8u*) (m_value[0]), (Ipp8u*) (m_value[1])))


   FUNC_CALL(ippsMinMaxIndx_64f,((Ipp64f*)pSrc, len, (Ipp64f*)(m_value[0]), m_index + 0,
                                                                   (Ipp64f*)(m_value[1]), m_index + 1))
   FUNC_CALL(ippsMinMaxIndx_32f,((Ipp32f*)pSrc, len, (Ipp32f*)(m_value[0]), m_index + 0,
                                                                   (Ipp32f*)(m_value[1]), m_index + 1))
   FUNC_CALL(ippsMinMaxIndx_32s,((Ipp32s*)pSrc, len, (Ipp32s*)(m_value[0]), m_index + 0,
                                                                   (Ipp32s*)(m_value[1]), m_index + 1))
   FUNC_CALL(ippsMinMaxIndx_32u,((Ipp32u*)pSrc, len, (Ipp32u*)(m_value[0]), m_index + 0,
                                                                   (Ipp32u*)(m_value[1]), m_index + 1))
   FUNC_CALL(ippsMinMaxIndx_16s,((Ipp16s*)pSrc, len, (Ipp16s*)(m_value[0]), m_index + 0,
                                                                   (Ipp16s*)(m_value[1]), m_index + 1))
   FUNC_CALL(ippsMinMaxIndx_16u,((Ipp16u*)pSrc, len, (Ipp16u*)(m_value[0]), m_index + 0,
                                                                   (Ipp16u*)(m_value[1]), m_index + 1))
   FUNC_CALL(ippsMinMaxIndx_8u, ((Ipp8u*) pSrc, len, (Ipp8u*) (m_value[0]), m_index + 0,
                                                                   (Ipp8u*) (m_value[1]), m_index + 1))

   return stsNoFunction;
}
