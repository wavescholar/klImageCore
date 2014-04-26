/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCorr.cpp : implementation of the CRunCorr class.
// CRunCorr class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "RunCorr.h"
#include "ParmCorrDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunCorr::CRunCorr()
{
   m_dstLen = 512;
   m_loLag  = 0;
}

CRunCorr::~CRunCorr()
{

}

BOOL CRunCorr::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   if (m_Func.Found("Auto"))
      m_UsedVectors = VEC_SRC | VEC_DST;
   else
      m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   return TRUE;
}

CParamDlg* CRunCorr::CreateDlg() { 
   return new CParmCorrDlg;
}

void CRunCorr::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmCorrDlg *pDlg = (CParmCorrDlg*)parmDlg;
   if (save) {
      m_loLag  = atoi(pDlg->m_LolagStr );
      m_dstLen = atoi(pDlg->m_DstlenStr);
   } else {
      pDlg->m_LolagStr  << m_loLag ;
      pDlg->m_DstlenStr << m_dstLen;
   }
}

int CRunCorr::GetDstLength()
{
   return m_dstLen;
}

IppStatus CRunCorr::CallIppFunction()
{
   FUNC_CALL(ippsCrossCorr_32f, ((Ipp32f*)pSrc, m_lenSrc, 
      (Ipp32f*)pSrc2, m_lenSrc2, 
      (Ipp32f*)pDst, m_lenDst, m_loLag))
   FUNC_CALL(ippsCrossCorr_64f, ((Ipp64f*)pSrc, m_lenSrc, 
      (Ipp64f*)pSrc2, m_lenSrc2, 
      (Ipp64f*)pDst, m_lenDst, m_loLag))
   FUNC_CALL(ippsCrossCorr_32fc,((Ipp32fc*)pSrc, m_lenSrc, 
      (Ipp32fc*)pSrc2, m_lenSrc2, 
      (Ipp32fc*)pDst, m_lenDst, m_loLag))
   FUNC_CALL(ippsCrossCorr_64fc,((Ipp64fc*)pSrc, m_lenSrc, 
      (Ipp64fc*)pSrc2, m_lenSrc2, 
      (Ipp64fc*)pDst, m_lenDst, m_loLag))
   FUNC_CALL(ippsCrossCorr_16s_Sfs, ((Ipp16s*)pSrc, m_lenSrc, 
      (Ipp16s*)pSrc2, m_lenSrc2, 
      (Ipp16s*)pDst, m_lenDst, m_loLag, scaleFactor))
   FUNC_CALL(ippsAutoCorr_32f, ( (Ipp32f*)pSrc, m_lenSrc, 
      (Ipp32f*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_NormA_32f, ( (Ipp32f*)pSrc, m_lenSrc, 
      (Ipp32f*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_NormB_32f, ( (Ipp32f*)pSrc, m_lenSrc, 
      (Ipp32f*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_64f, ( (Ipp64f*)pSrc, m_lenSrc, 
      (Ipp64f*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_NormA_64f, ( (Ipp64f*)pSrc, m_lenSrc, 
      (Ipp64f*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_NormB_64f, ( (Ipp64f*)pSrc, m_lenSrc, 
      (Ipp64f*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_32fc,( (Ipp32fc*)pSrc, m_lenSrc, 
      (Ipp32fc*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_NormA_32fc,( (Ipp32fc*)pSrc, m_lenSrc, 
      (Ipp32fc*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_NormB_32fc,( (Ipp32fc*)pSrc, m_lenSrc, 
      (Ipp32fc*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_64fc,( (Ipp64fc*)pSrc, m_lenSrc, 
      (Ipp64fc*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_NormA_64fc,( (Ipp64fc*)pSrc, m_lenSrc, 
      (Ipp64fc*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_NormB_64fc,( (Ipp64fc*)pSrc, m_lenSrc, 
      (Ipp64fc*)pDst, m_lenDst ))
   FUNC_CALL(ippsAutoCorr_16s_Sfs,( (Ipp16s*)pSrc, m_lenSrc, 
      (Ipp16s*)pDst, m_lenDst, scaleFactor ))
   FUNC_CALL(ippsAutoCorr_NormA_16s_Sfs,( (Ipp16s*)pSrc, m_lenSrc, 
      (Ipp16s*)pDst, m_lenDst, scaleFactor ))
   FUNC_CALL(ippsAutoCorr_NormB_16s_Sfs,( (Ipp16s*)pSrc, m_lenSrc, 
      (Ipp16s*)pDst, m_lenDst, scaleFactor ))

   return stsNoFunction;
}

CString CRunCorr::GetHistoryParms()
{
   CMyString parms;
   return parms << m_loLag;
}

