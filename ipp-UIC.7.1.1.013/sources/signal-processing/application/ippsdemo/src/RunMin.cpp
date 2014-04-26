/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunMin.cpp : implementation of the CRunMin class.
// CRunMin class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunMin.h"
#include "ParmMinDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunMin::CRunMin()
{
}

CRunMin::~CRunMin()
{

}

BOOL CRunMin::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   if (m_Func.Found("Order"))
      m_value.Init(pp32s);
   else
      m_value.Init(m_Func,argDST);
   return TRUE;
}

void CRunMin::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmMinDlg *pDlg = (CParmMinDlg*)parmDlg;
   if (save) {
      m_Func = pDlg->m_Func;
      if (m_Func.Found("Order"))
         m_value.Init(pp32s);
      else
         m_value.Init(m_Func,argDST);
   } else {
      pDlg->m_Index = m_index;
      m_value.Get(pDlg->m_Re, pDlg->m_Im);
   }
}

BOOL CRunMin::CallIpp(BOOL bMessage)
{
   CParmMinDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunMin::SetValues(CParmMinDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   CippsRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunMin::CallIppFunction()
{
   FUNC_CALL(ippsMax_32f,((Ipp32f*)pSrc,len,(Ipp32f*)m_value))
   FUNC_CALL(ippsMax_64f,((Ipp64f*)pSrc,len,(Ipp64f*)m_value))
   FUNC_CALL(ippsMax_16s,((Ipp16s*)pSrc,len,(Ipp16s*)m_value))
   FUNC_CALL(ippsMax_32s,((Ipp32s*)pSrc,len,(Ipp32s*)m_value))
   FUNC_CALL(ippsMaxIndx_16s, 
         ( (Ipp16s*)pSrc, len, (Ipp16s*)m_value,&m_index )) 
   FUNC_CALL(ippsMaxIndx_32s, 
         ( (Ipp32s*)pSrc, len, (Ipp32s*)m_value,&m_index )) 
   FUNC_CALL(ippsMaxIndx_32f, 
         ( (Ipp32f*)pSrc, len, (Ipp32f*)m_value,&m_index )) 
   FUNC_CALL(ippsMaxIndx_64f, 
         ( (Ipp64f*)pSrc, len, (Ipp64f*)m_value,&m_index ))
   FUNC_CALL(ippsMin_32f,((Ipp32f*)pSrc,len,(Ipp32f*)m_value))
   FUNC_CALL(ippsMin_64f,((Ipp64f*)pSrc,len,(Ipp64f*)m_value))
   FUNC_CALL(ippsMin_16s,((Ipp16s*)pSrc,len,(Ipp16s*)m_value))
   FUNC_CALL(ippsMin_32s,((Ipp32s*)pSrc,len,(Ipp32s*)m_value))
   FUNC_CALL(ippsMinIndx_16s, 
         ( (Ipp16s*)pSrc, len, (Ipp16s*)m_value,&m_index )) 
   FUNC_CALL(ippsMinIndx_32s, 
         ( (Ipp32s*)pSrc, len, (Ipp32s*)m_value,&m_index )) 
   FUNC_CALL(ippsMinIndx_32f, 
         ( (Ipp32f*)pSrc, len, (Ipp32f*)m_value,&m_index )) 
   FUNC_CALL(ippsMinIndx_64f, 
         ( (Ipp64f*)pSrc, len, (Ipp64f*)m_value,&m_index )) 
   FUNC_CALL( ippsMinAbs_16s, ((const Ipp16s*)pSrc, len, (Ipp16s*)m_value))
   FUNC_CALL( ippsMaxAbs_16s, ((const Ipp16s*)pSrc, len, (Ipp16s*)m_value))
   FUNC_CALL( ippsMinAbsIndx_16s, ((const Ipp16s*)pSrc, len, (Ipp16s*)m_value, &m_index))
   FUNC_CALL( ippsMaxAbsIndx_16s, ((const Ipp16s*)pSrc, len, (Ipp16s*)m_value, &m_index))
   FUNC_CALL( ippsMinAbs_32s, ((const Ipp32s*)pSrc, len, (Ipp32s*)m_value))
   FUNC_CALL( ippsMaxAbs_32s, ((const Ipp32s*)pSrc, len, (Ipp32s*)m_value))
   FUNC_CALL( ippsMinAbsIndx_32s, ((const Ipp32s*)pSrc, len, (Ipp32s*)m_value, &m_index))
   FUNC_CALL( ippsMaxAbsIndx_32s, ((const Ipp32s*)pSrc, len, (Ipp32s*)m_value, &m_index))
   FUNC_CALL(ippsMaxOrder_64f,((Ipp64f*)pSrc, len, (Ipp32s*)m_value))
   FUNC_CALL(ippsMaxOrder_32f,((Ipp32f*)pSrc, len, (Ipp32s*)m_value))
   FUNC_CALL(ippsMaxOrder_32s,((Ipp32s*)pSrc, len, (Ipp32s*)m_value))
   FUNC_CALL(ippsMaxOrder_16s,((Ipp16s*)pSrc, len, (Ipp32s*)m_value))
   return stsNoFunction;
}
