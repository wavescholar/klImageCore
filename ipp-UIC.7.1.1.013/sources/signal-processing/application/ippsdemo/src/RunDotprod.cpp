/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDotprod.cpp : implementation of the CRunDotprod class.
// CRunDotprod class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunDotprod.h"
#include "ParmDotprodDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunDotprod::CRunDotprod()
{

}

CRunDotprod::~CRunDotprod()
{

}

BOOL CRunDotprod::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2;
   m_value.Init(func,argDST);
   return TRUE;
}

void CRunDotprod::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmDotprodDlg *pDlg = (CParmDotprodDlg*)parmDlg;
   if (save) {
      m_Func = pDlg->m_Func;
      m_value.Init(m_Func,argDST);
   } else {
      m_value.Get(pDlg->m_Re, pDlg->m_Im);
   }
}

BOOL CRunDotprod::CallIpp(BOOL bMessage)
{
   CParmDotprodDlg dlg(this);
   UpdateData(&dlg,FALSE);
   if (dlg.DoModal() != IDOK) return TRUE;
   UpdateData(&dlg);
   return TRUE;
}

void CRunDotprod::SetValues(CParmDotprodDlg* pDlg)
{
   UpdateData(pDlg);
   PrepareParameters();
   CippsRun::CallIpp();
   Timing();
   SetHistory();
   UpdateData(pDlg,FALSE);
}

IppStatus CRunDotprod::CallIppFunction()
{
   FUNC_CALL(ippsDotProd_32f, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, len, (Ipp32f*)m_value))
   FUNC_CALL(ippsDotProd_32fc,((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, len, (Ipp32fc*)m_value))
   FUNC_CALL(ippsDotProd_32f32fc,((Ipp32f*)pSrc, (Ipp32fc*)pSrc2, len, (Ipp32fc*)m_value))
   FUNC_CALL(ippsDotProd_64f, ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, len, (Ipp64f*)m_value))
   FUNC_CALL(ippsDotProd_64fc,((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, len, (Ipp64fc*)m_value))
   FUNC_CALL(ippsDotProd_64f64fc,((Ipp64f*)pSrc, (Ipp64fc*)pSrc2, len, (Ipp64fc*)m_value))
   FUNC_CALL(ippsDotProd_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, len, (Ipp16s*)m_value, scaleFactor))
   FUNC_CALL(ippsDotProd_16sc_Sfs,((Ipp16sc*)pSrc, (Ipp16sc*)pSrc2, len, (Ipp16sc*)m_value, scaleFactor))
   FUNC_CALL(ippsDotProd_16s16sc_Sfs, ((Ipp16s*)pSrc, (Ipp16sc*)pSrc2, len, (Ipp16sc*)m_value, scaleFactor))
   FUNC_CALL(ippsDotProd_16s64s, ((Ipp16s*) pSrc, (Ipp16s*) pSrc2, len, (Ipp64s*) m_value))
   FUNC_CALL(ippsDotProd_16sc64sc, ((Ipp16sc*)pSrc, (Ipp16sc*)pSrc2, len, (Ipp64sc*)m_value))
   FUNC_CALL(ippsDotProd_16s16sc64sc,((Ipp16s*) pSrc, (Ipp16sc*)pSrc2, len, (Ipp64sc*)m_value))
   FUNC_CALL(ippsDotProd_16s32f, ((Ipp16s*) pSrc, (Ipp16s*) pSrc2, len, (Ipp32f*) m_value))
   FUNC_CALL(ippsDotProd_16sc32fc, ((Ipp16sc*)pSrc, (Ipp16sc*)pSrc2, len, (Ipp32fc*)m_value))
   FUNC_CALL(ippsDotProd_16s16sc32fc,((Ipp16s*) pSrc, (Ipp16sc*)pSrc2, len, (Ipp32fc*)m_value))
   FUNC_CALL(ippsDotProd_32f64f, ( (Ipp32f*)pSrc, (Ipp32f*)pSrc2, len, (Ipp64f*)m_value )) 
   FUNC_CALL(ippsDotProd_32fc64fc, ( (Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, len, (Ipp64fc*)m_value )) 
   FUNC_CALL(ippsDotProd_32f32fc64fc, ( (Ipp32f*)pSrc, (Ipp32fc*)pSrc2, len, (Ipp64fc*)m_value ))

   FUNC_CALL(ippsDotProd_16s32s_Sfs, ((Ipp16s*)pSrc,(Ipp16s*)pSrc2, len, (Ipp32s*)m_value, scaleFactor ))
   FUNC_CALL(ippsDotProd_16sc32sc_Sfs, ((Ipp16sc*)pSrc,(Ipp16sc*)pSrc2, len, (Ipp32sc*)m_value, scaleFactor ))
   FUNC_CALL(ippsDotProd_16s16sc32sc_Sfs, ((Ipp16s*)pSrc,(Ipp16sc*)pSrc2, len, (Ipp32sc*)m_value, scaleFactor ))
   FUNC_CALL(ippsDotProd_32s_Sfs, ((Ipp32s*)pSrc,(Ipp32s*)pSrc2, len, (Ipp32s*)m_value, scaleFactor ))
   FUNC_CALL(ippsDotProd_32sc_Sfs, ((Ipp32sc*)pSrc,(Ipp32sc*)pSrc2, len, (Ipp32sc*)m_value, scaleFactor ))
   FUNC_CALL(ippsDotProd_32s32sc_Sfs, ((Ipp32s*)pSrc,(Ipp32sc*)pSrc2, len, (Ipp32sc*)m_value, scaleFactor ))
   FUNC_CALL(ippsDotProd_16s32s32s_Sfs, ((Ipp16s*)pSrc, (Ipp32s*)pSrc2, len, (Ipp32s*)m_value, scaleFactor ))

   return stsNoFunction;
}
