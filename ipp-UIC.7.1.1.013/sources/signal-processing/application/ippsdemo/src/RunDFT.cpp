/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDFT.cpp : implementation of the CRunDFT class.
// CRunDFT class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "Histo.h"
#include "RunDFT.h"
#include "ParmFFTDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunDFT::CRunDFT()
{
   m_pSpec = NULL;
   m_buffer  = NULL;
   m_flag  = IPP_FFT_NODIV_BY_ANY;
   m_hint  = ippAlgHintFast;
}

CRunDFT::~CRunDFT()
{

}

BOOL CRunDFT::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;
   BOOL bCplx = func.Found("CToC");
   if (bCplx && !(func.DstType() & PP_CPLX))
      m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST | VEC_DST2;

   m_buffer = NULL;
   int type = (int)func.DstType() & ~PP_CPLX;
   m_InitName = "";
   m_FreeName = "";
   m_GetBufSizeName = "";

   CString sBase = "ipps" + func.BaseName().Mid(0, func.BaseName().Find("_") - 3);
   CString sType = (bCplx ? "_C_" : "_R_") + func.TypeName();
   m_InitName = sBase + "InitAlloc" + sType;
   m_FreeName = sBase + "Free" + sType;
   m_GetBufSizeName = sBase + "GetBufSize" + sType;
   return TRUE;
}

CParamDlg* CRunDFT::CreateDlg()
{
   return new CParmFFTDlg;
}

void CRunDFT::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmFFTDlg *pDlg = (CParmFFTDlg*)parmDlg;
   if (save) {
      m_flag  = pDlg->m_Flag ;
      m_hint  = (IppHintAlgorithm)pDlg->m_Hint ;
   } else {
      pDlg->m_Flag  = m_flag ;
      pDlg->m_Hint  = (int)m_hint ;
   }
}

BOOL CRunDFT::BeforeCall()
{
   IppStatus status = CallInit();
   IppErrorMessage(m_InitName, status);
   if (status < 0) return FALSE;
   int size;
   status = CallGetBufSize(size);
   IppErrorMessage(m_GetBufSizeName, status);
   if (status < 0) return FALSE;
   if (size) m_buffer = (Ipp8u*)ippMalloc(size);
   return TRUE;
}

BOOL CRunDFT::AfterCall(BOOL bOk)
{
   IppStatus status = CallFree(); m_pSpec = NULL;
   IppErrorMessage(m_FreeName, status);
   ippFree(m_buffer); m_buffer = NULL;
   return TRUE;
}

IppStatus CRunDFT::CallInit()
{
   MATE_CALL(m_InitName, ippsDFTInitAlloc_C_16sc,
                   ( (IppsDFTSpec_C_16sc**)&m_pSpec,
                     len, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsDFTInitAlloc_C_16s,
                   ( (IppsDFTSpec_C_16s**)&m_pSpec,
                     len, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsDFTInitAlloc_R_16s,
                   ( (IppsDFTSpec_R_16s**)&m_pSpec,
                     len, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsDFTInitAlloc_C_32fc,
                   ( (IppsDFTSpec_C_32fc**)&m_pSpec,
                     len, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsDFTInitAlloc_C_32f,
                   ( (IppsDFTSpec_C_32f**)&m_pSpec,
                     len, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsDFTInitAlloc_R_32f,
                   ( (IppsDFTSpec_R_32f**)&m_pSpec,
                     len, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsDFTInitAlloc_C_64fc,
                   ( (IppsDFTSpec_C_64fc**)&m_pSpec,
                     len, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsDFTInitAlloc_C_64f,
                   ( (IppsDFTSpec_C_64f**)&m_pSpec,
                     len, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsDFTInitAlloc_R_64f,
                   ( (IppsDFTSpec_R_64f**)&m_pSpec,
                     len, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsDFTOutOrdInitAlloc_C_32fc,
                   ( (IppsDFTOutOrdSpec_C_32fc**)&m_pSpec,
                     len, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsDFTOutOrdInitAlloc_C_64fc,
                   ( (IppsDFTOutOrdSpec_C_64fc**)&m_pSpec,
                     len, m_flag, m_hint))
   return stsNoFunction;
}

IppStatus CRunDFT::CallFree()
{
   MATE_CALL(m_FreeName, ippsDFTFree_C_16sc, ( (IppsDFTSpec_C_16sc*)m_pSpec ))
   MATE_CALL(m_FreeName, ippsDFTFree_C_16s,  ( (IppsDFTSpec_C_16s*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsDFTFree_R_16s,  ( (IppsDFTSpec_R_16s*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsDFTFree_C_32fc, ( (IppsDFTSpec_C_32fc*)m_pSpec ))
   MATE_CALL(m_FreeName, ippsDFTFree_C_32f,  ( (IppsDFTSpec_C_32f*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsDFTFree_R_32f,  ( (IppsDFTSpec_R_32f*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsDFTFree_C_64fc, ( (IppsDFTSpec_C_64fc*)m_pSpec ))
   MATE_CALL(m_FreeName, ippsDFTFree_C_64f,  ( (IppsDFTSpec_C_64f*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsDFTFree_R_64f,  ( (IppsDFTSpec_R_64f*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsDFTOutOrdFree_C_32fc, ( (IppsDFTOutOrdSpec_C_32fc*)m_pSpec ))
   MATE_CALL(m_FreeName, ippsDFTOutOrdFree_C_64fc, ( (IppsDFTOutOrdSpec_C_64fc*)m_pSpec ))
   return stsNoFunction;
}

IppStatus CRunDFT::CallGetBufSize(int& size)
{
   MATE_CALL(m_GetBufSizeName, ippsDFTGetBufSize_C_16sc,
                   ( (IppsDFTSpec_C_16sc*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDFTGetBufSize_C_16s,
                   ( (IppsDFTSpec_C_16s*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDFTGetBufSize_R_16s,
                   ( (IppsDFTSpec_R_16s*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDFTGetBufSize_C_32fc,
                   ( (IppsDFTSpec_C_32fc*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDFTGetBufSize_C_32f,
                   ( (IppsDFTSpec_C_32f*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDFTGetBufSize_R_32f,
                   ( (IppsDFTSpec_R_32f*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDFTGetBufSize_C_64fc,
                   ( (IppsDFTSpec_C_64fc*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDFTGetBufSize_C_64f,
                   ( (IppsDFTSpec_C_64f*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDFTGetBufSize_R_64f,
                   ( (IppsDFTSpec_R_64f*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDFTOutOrdGetBufSize_C_32fc,
                   ((IppsDFTOutOrdSpec_C_32fc*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsDFTOutOrdGetBufSize_C_64fc,
                   ((IppsDFTOutOrdSpec_C_64fc*)m_pSpec, &size ))
   return stsNoFunction;
}

IppStatus CRunDFT::CallIppFunction()
{
   FUNC_CALL(ippsDFTFwd_CToC_16sc_Sfs,
                   ( (Ipp16sc*)pSrc, (Ipp16sc*)pDst,
                     (IppsDFTSpec_C_16sc*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsDFTInv_CToC_16sc_Sfs,
                   ( (Ipp16sc*)pSrc, (Ipp16sc*)pDst,
                     (IppsDFTSpec_C_16sc*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsDFTFwd_CToC_16s_Sfs,
                   ( (Ipp16s*)pSrc, (Ipp16s*)pSrc2,
                     (Ipp16s*)pDst, (Ipp16s*)pDst2,
                     (IppsDFTSpec_C_16s*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsDFTInv_CToC_16s_Sfs,
                   ( (Ipp16s*)pSrc, (Ipp16s*)pSrc2,
                     (Ipp16s*)pDst, (Ipp16s*)pDst2,
                     (IppsDFTSpec_C_16s*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsDFTFwd_CToC_32fc,
                   ( (Ipp32fc*)pSrc, (Ipp32fc*)pDst,
                     (IppsDFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTInv_CToC_32fc,
                   ( (Ipp32fc*)pSrc, (Ipp32fc*)pDst,
                     (IppsDFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTFwd_CToC_32f,
                   ( (Ipp32f*)pSrc, (Ipp32f*)pSrc2,
                     (Ipp32f*)pDst, (Ipp32f*)pDst2,
                     (IppsDFTSpec_C_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTInv_CToC_32f,
                   ( (Ipp32f*)pSrc, (Ipp32f*)pSrc2,
                     (Ipp32f*)pDst, (Ipp32f*)pDst2,
                     (IppsDFTSpec_C_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTFwd_CToC_64fc,
                   ( (Ipp64fc*)pSrc, (Ipp64fc*)pDst,
                     (IppsDFTSpec_C_64fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTInv_CToC_64fc,
                   ( (Ipp64fc*)pSrc, (Ipp64fc*)pDst,
                     (IppsDFTSpec_C_64fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTFwd_CToC_64f,
                   ( (Ipp64f*)pSrc, (Ipp64f*)pSrc2,
                     (Ipp64f*)pDst, (Ipp64f*)pDst2,
                     (IppsDFTSpec_C_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTInv_CToC_64f,
                   ( (Ipp64f*)pSrc, (Ipp64f*)pSrc2,
                     (Ipp64f*)pDst, (Ipp64f*)pDst2,
                     (IppsDFTSpec_C_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTFwd_RToPerm_16s_Sfs,
                   ( (Ipp16s*)pSrc, (Ipp16s*)pDst,
                     (IppsDFTSpec_R_16s*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsDFTFwd_RToPack_16s_Sfs,
                   ( (Ipp16s*)pSrc, (Ipp16s*)pDst,
                     (IppsDFTSpec_R_16s*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsDFTFwd_RToCCS_16s_Sfs,
                   ( (Ipp16s*)pSrc, (Ipp16s*)pDst,
                     (IppsDFTSpec_R_16s*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsDFTInv_PermToR_16s_Sfs,
                   ( (Ipp16s*)pSrc, (Ipp16s*)pDst,
                     (IppsDFTSpec_R_16s*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsDFTInv_PackToR_16s_Sfs,
                   ( (Ipp16s*)pSrc, (Ipp16s*)pDst,
                     (IppsDFTSpec_R_16s*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsDFTInv_CCSToR_16s_Sfs,
                   ( (Ipp16s*)pSrc, (Ipp16s*)pDst,
                     (IppsDFTSpec_R_16s*)m_pSpec,
                     scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsDFTFwd_RToPerm_32f,
                   ( (Ipp32f*)pSrc, (Ipp32f*)pDst,
                     (IppsDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTFwd_RToPack_32f,
                   ( (Ipp32f*)pSrc, (Ipp32f*)pDst,
                     (IppsDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTFwd_RToCCS_32f,
                   ( (Ipp32f*)pSrc, (Ipp32f*)pDst,
                     (IppsDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTInv_PermToR_32f,
                   ( (Ipp32f*)pSrc, (Ipp32f*)pDst,
                     (IppsDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTInv_PackToR_32f,
                   ( (Ipp32f*)pSrc, (Ipp32f*)pDst,
                     (IppsDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTInv_CCSToR_32f,
                   ( (Ipp32f*)pSrc, (Ipp32f*)pDst,
                     (IppsDFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTFwd_RToPerm_64f,
                   ( (Ipp64f*)pSrc, (Ipp64f*)pDst,
                     (IppsDFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTFwd_RToPack_64f,
                   ( (Ipp64f*)pSrc, (Ipp64f*)pDst,
                     (IppsDFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTFwd_RToCCS_64f,
                   ( (Ipp64f*)pSrc, (Ipp64f*)pDst,
                     (IppsDFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTInv_PermToR_64f,
                   ( (Ipp64f*)pSrc, (Ipp64f*)pDst,
                     (IppsDFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTInv_PackToR_64f,
                   ( (Ipp64f*)pSrc, (Ipp64f*)pDst,
                     (IppsDFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTInv_CCSToR_64f,
                   ( (Ipp64f*)pSrc, (Ipp64f*)pDst,
                     (IppsDFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTOutOrdFwd_CToC_32fc,
                   ((Ipp32fc*)pSrc, (Ipp32fc*)pDst,
                    (IppsDFTOutOrdSpec_C_32fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTOutOrdInv_CToC_32fc,
                   ((Ipp32fc*)pSrc, (Ipp32fc*)pDst,
                    (IppsDFTOutOrdSpec_C_32fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTOutOrdFwd_CToC_64fc,
                   ((Ipp64fc*)pSrc, (Ipp64fc*)pDst,
                    (IppsDFTOutOrdSpec_C_64fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsDFTOutOrdInv_CToC_64fc,
                   ((Ipp64fc*)pSrc, (Ipp64fc*)pDst,
                    (IppsDFTOutOrdSpec_C_64fc*)m_pSpec, (Ipp8u*)m_buffer ))

   return stsNoFunction;
}

void CRunDFT::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CMyString initParms;
   initParms << len << ", " 
             <<(EIppFlag)m_flag << ", "  
             << m_hint ;
   pHisto->AddFuncString(m_InitName, initParms);
   CippsRun::AddHistoFunc(pHisto,vecPos);
}





