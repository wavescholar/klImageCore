/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunNorm.cpp : implementation of the CRunNorm class.
// CRunNorm class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunNorm.h"
#include "ParmNormDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunNorm::CRunNorm()
{

}

CRunNorm::~CRunNorm()
{

}

BOOL CRunNorm::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   if (m_Func.Found("Diff"))
      m_UsedVectors = VEC_SRC | VEC_SRC2;
   else
      m_UsedVectors = VEC_SRC;
   m_value.Init(func,argDST);
   return TRUE;
}

void CRunNorm::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmNormDlg *pDlg = (CParmNormDlg*)parmDlg;
   if (save) {
      m_Func = pDlg->m_Func;
      m_value.Init(m_Func,argDST);
   } else {
      m_value.Get(pDlg->m_Value);
   }
}

BOOL CRunNorm::CallIpp(BOOL bMessage)
{
   CParmNormDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunNorm::SetValues(CParmNormDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   CippsRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunNorm::CallIppFunction()
{
   FUNC_CALL(ippsNorm_Inf_16s32f, ((Ipp16s*)pSrc, len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNorm_Inf_16s32s_Sfs, ((Ipp16s*)pSrc, len, (Ipp32s*)m_value,  scaleFactor))
   FUNC_CALL(ippsNorm_Inf_32f, ((Ipp32f*)pSrc, len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNorm_Inf_64f, ((Ipp64f*)pSrc, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNorm_Inf_32fc32f, ((Ipp32fc*)pSrc, len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNorm_Inf_64fc64f, ((Ipp64fc*)pSrc, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNorm_L1_16s32f, ((Ipp16s*)pSrc, len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNorm_L1_16s32s_Sfs, ((Ipp16s*)pSrc, len, (Ipp32s*)m_value,  scaleFactor))
   FUNC_CALL(ippsNorm_L1_32f, ((Ipp32f*)pSrc, len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNorm_L1_64f, ((Ipp64f*)pSrc, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNorm_L1_32fc64f, ((Ipp32fc*)pSrc, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNorm_L1_64fc64f, ((Ipp64fc*)pSrc, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNorm_L2_16s32f, ((Ipp16s*)pSrc, len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNorm_L2_16s32s_Sfs, ((Ipp16s*)pSrc, len, (Ipp32s*)m_value,  scaleFactor))
   FUNC_CALL(ippsNorm_L2_32f, ((Ipp32f*)pSrc, len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNorm_L2_64f, ((Ipp64f*)pSrc, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNorm_L2_32fc64f, ((Ipp32fc*)pSrc, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNorm_L2_64fc64f, ((Ipp64fc*)pSrc, len, (Ipp64f*)m_value))
   FUNC_CALL( ippsNorm_L1_16s64s_Sfs, ((const Ipp16s*)pSrc, len, (Ipp64s*)m_value, scaleFactor))
   FUNC_CALL( ippsNorm_L2Sqr_16s64s_Sfs, ((const Ipp16s*)pSrc, len, (Ipp64s*)m_value, scaleFactor))

   FUNC_CALL(ippsNormDiff_Inf_16s32f, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2,  len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNormDiff_Inf_16s32s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2,  len, (Ipp32s*)m_value,  scaleFactor))
   FUNC_CALL(ippsNormDiff_Inf_32f, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2,  len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNormDiff_Inf_64f, ((Ipp64f*)pSrc, (Ipp64f*)pSrc2,  len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNormDiff_Inf_32fc32f, ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNormDiff_Inf_64fc64f, ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNormDiff_L1_16s32f, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2,  len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNormDiff_L1_16s32s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2,  len, (Ipp32s*)m_value,  scaleFactor))
   FUNC_CALL(ippsNormDiff_L1_32f, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2,  len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNormDiff_L1_64f, ((Ipp64f*)pSrc, (Ipp64f*)pSrc2,  len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNormDiff_L1_32fc64f, ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNormDiff_L1_64fc64f, ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNormDiff_L2_16s32f, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2,  len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNormDiff_L2_16s32s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2,  len, (Ipp32s*)m_value,  scaleFactor))
   FUNC_CALL(ippsNormDiff_L2_32f, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2,  len, (Ipp32f*)m_value))
   FUNC_CALL(ippsNormDiff_L2_64f, ((Ipp64f*)pSrc, (Ipp64f*)pSrc2,  len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNormDiff_L2_32fc64f, ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsNormDiff_L2_64fc64f, ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, len, (Ipp64f*)m_value))
   FUNC_CALL( ippsNormDiff_L1_16s64s_Sfs, ((const Ipp16s*)pSrc, (const Ipp16s*)pSrc2, len, (Ipp64s*)m_value, scaleFactor))
   FUNC_CALL( ippsNormDiff_L2Sqr_16s64s_Sfs, ((const Ipp16s*)pSrc, (const Ipp16s*)pSrc2, len, (Ipp64s*)m_value, scaleFactor))

   return stsNoFunction;
}
