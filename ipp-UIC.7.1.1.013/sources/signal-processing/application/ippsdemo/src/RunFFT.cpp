/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFFT.cpp : implementation of the CRunFFT class.
// CRunFFT class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "Histo.h"
#include "RunFFT.h"
#include "ParmFFTDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFFT::CRunFFT()
{
   m_pSpec = NULL;
   m_buffer  = NULL;
   m_order = 0;
   m_flag = 0;
   m_flagFwd  = IPP_FFT_DIV_FWD_BY_N;
   m_flagInv  = IPP_FFT_NODIV_BY_ANY;
   m_hint  = ippAlgHintFast;
}

CRunFFT::~CRunFFT()
{

}

BOOL CRunFFT::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;
   BOOL bCplx = func.Found("CToC");
   if (bCplx && !(func.DstType() & PP_CPLX))
      m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST | VEC_DST2;
   if (m_Func.Found("Fwd"))
      m_flag = m_flagFwd;
   else
      m_flag = m_flagInv;
   m_buffer = NULL;
   int type = (int)func.DstType() & ~PP_CPLX;
   m_InitName = "";
   m_FreeName = "";
   m_GetBufSizeName = "";

   CString sType = func.TypeName();
   if (sType == "32s16s") sType = "16s32s";
   sType = (bCplx ? "_C_" : "_R_") + sType;

   m_InitName = "ippsFFTInitAlloc" + sType;
   m_FreeName = "ippsFFTFree" + sType;
   m_GetBufSizeName = "ippsFFTGetBufSize" + sType;
   return TRUE;
}

void CRunFFT::Close() 
{
   if (m_Func.Found("Fwd"))
      m_flagFwd = m_flag;
   else
      m_flagInv = m_flag;
}

CParamDlg* CRunFFT::CreateDlg()
{
   return new CParmFFTDlg;
}

void CRunFFT::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmFFTDlg *pDlg = (CParmFFTDlg*)parmDlg;
   if (save) {
      m_order = pDlg->m_Order;
      m_flag = pDlg->m_Flag ;
      m_hint  = (IppHintAlgorithm)pDlg->m_Hint ;
   } else {
      pDlg->m_Order = m_order;
      pDlg->m_Flag = m_flag;
      pDlg->m_Hint  = (int)m_hint ;
   }
}

int CRunFFT::GetDstLength()
{
   int length = 1;
   for (int i = 0; i < m_order; i++) length <<= 1;
   return length;
}

BOOL CRunFFT::BeforeCall()
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

BOOL CRunFFT::AfterCall(BOOL bOk)
{
   IppStatus status = CallFree(); m_pSpec = NULL;
   IppErrorMessage(m_FreeName, status);
   ippFree(m_buffer); m_buffer = NULL;
   return TRUE;
}

IppStatus CRunFFT::CallInit()
{
   MATE_CALL(m_InitName, ippsFFTInitAlloc_C_16sc,
                   ( (IppsFFTSpec_C_16sc**)&m_pSpec,
                     m_order, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsFFTInitAlloc_C_16s,
                   ( (IppsFFTSpec_C_16s**)&m_pSpec,
                     m_order, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsFFTInitAlloc_R_16s,
                   ( (IppsFFTSpec_R_16s**)&m_pSpec,
                     m_order, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsFFTInitAlloc_C_32fc,
                   ( (IppsFFTSpec_C_32fc**)&m_pSpec,
                     m_order, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsFFTInitAlloc_C_32f,
                   ( (IppsFFTSpec_C_32f**)&m_pSpec,
                     m_order, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsFFTInitAlloc_R_32f,
                   ( (IppsFFTSpec_R_32f**)&m_pSpec,
                     m_order, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsFFTInitAlloc_C_64fc,
                   ( (IppsFFTSpec_C_64fc**)&m_pSpec,
                     m_order, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsFFTInitAlloc_C_64f,
                   ( (IppsFFTSpec_C_64f**)&m_pSpec,
                     m_order, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsFFTInitAlloc_R_64f,
                   ( (IppsFFTSpec_R_64f**)&m_pSpec,
                     m_order, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsFFTInitAlloc_C_32sc, ((IppsFFTSpec_C_32sc**)&m_pSpec, m_order, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsFFTInitAlloc_R_32s, ((IppsFFTSpec_R_32s**)&m_pSpec, m_order, m_flag, m_hint))
   MATE_CALL(m_InitName, ippsFFTInitAlloc_R_16s32s, ((IppsFFTSpec_R_16s32s**)&m_pSpec, m_order, m_flag, m_hint))
   return stsNoFunction;
}

IppStatus CRunFFT::CallFree()
{
   MATE_CALL(m_FreeName, ippsFFTFree_C_16sc, ( (IppsFFTSpec_C_16sc*)m_pSpec ))
   MATE_CALL(m_FreeName, ippsFFTFree_C_16s,  ( (IppsFFTSpec_C_16s*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsFFTFree_R_16s,  ( (IppsFFTSpec_R_16s*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsFFTFree_C_32fc, ( (IppsFFTSpec_C_32fc*)m_pSpec ))
   MATE_CALL(m_FreeName, ippsFFTFree_C_32f,  ( (IppsFFTSpec_C_32f*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsFFTFree_R_32f,  ( (IppsFFTSpec_R_32f*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsFFTFree_C_64fc, ( (IppsFFTSpec_C_64fc*)m_pSpec ))
   MATE_CALL(m_FreeName, ippsFFTFree_C_64f,  ( (IppsFFTSpec_C_64f*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsFFTFree_R_64f,  ( (IppsFFTSpec_R_64f*) m_pSpec ))
   MATE_CALL(m_FreeName, ippsFFTFree_C_32sc,  ((IppsFFTSpec_C_32sc*)m_pSpec))
   MATE_CALL(m_FreeName, ippsFFTFree_R_32s,   ((IppsFFTSpec_R_32s*)m_pSpec))
   MATE_CALL(m_FreeName, ippsFFTFree_R_16s32s,((IppsFFTSpec_R_16s32s*)m_pSpec))
   return stsNoFunction;
}

IppStatus CRunFFT::CallGetBufSize(int& size)
{
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_C_16sc,
                   ( (IppsFFTSpec_C_16sc*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_C_16s,
                   ( (IppsFFTSpec_C_16s*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_R_16s,
                   ( (IppsFFTSpec_R_16s*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_C_32fc,
                   ( (IppsFFTSpec_C_32fc*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_C_32f,
                   ( (IppsFFTSpec_C_32f*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_R_32f,
                   ( (IppsFFTSpec_R_32f*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_C_64fc,
                   ( (IppsFFTSpec_C_64fc*)m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_C_64f,
                   ( (IppsFFTSpec_C_64f*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_R_64f,
                   ( (IppsFFTSpec_R_64f*) m_pSpec, &size ))
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_C_32sc, ((IppsFFTSpec_C_32sc*)m_pSpec, &size))
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_R_32s, ((IppsFFTSpec_R_32s*)m_pSpec, &size))
   MATE_CALL(m_GetBufSizeName, ippsFFTGetBufSize_R_16s32s, ((IppsFFTSpec_R_16s32s*)m_pSpec, &size))
   return stsNoFunction;
}

IppStatus CRunFFT::CallIppFunction()
{
   FUNC_CALL(ippsFFTFwd_CToC_16sc_Sfs,
      ((Ipp16sc*)pSrc, (Ipp16sc*)pDst,(IppsFFTSpec_C_16sc*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_16sc_Sfs,
      ((Ipp16sc*)pSrc, (Ipp16sc*)pDst,(IppsFFTSpec_C_16sc*)m_pSpec,scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_16s_Sfs,
      ((Ipp16s*)pSrc, (Ipp16s*)pSrc2,(Ipp16s*)pDst, (Ipp16s*)pDst2,(IppsFFTSpec_C_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_16s_Sfs,
      ((Ipp16s*)pSrc, (Ipp16s*)pSrc2,(Ipp16s*)pDst, (Ipp16s*)pDst2,(IppsFFTSpec_C_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_32fc,
      ((Ipp32fc*)pSrc, (Ipp32fc*)pDst,(IppsFFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_32fc,
      ((Ipp32fc*)pSrc, (Ipp32fc*)pDst, (IppsFFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_32f,
      ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, (Ipp32f*)pDst, (Ipp32f*)pDst2, (IppsFFTSpec_C_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_32f,
      ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, (Ipp32f*)pDst, (Ipp32f*)pDst2, (IppsFFTSpec_C_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_64fc,
      ((Ipp64fc*)pSrc, (Ipp64fc*)pDst, (IppsFFTSpec_C_64fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_64fc,
      ((Ipp64fc*)pSrc, (Ipp64fc*)pDst, (IppsFFTSpec_C_64fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_64f,
      ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, (Ipp64f*)pDst, (Ipp64f*)pDst2, (IppsFFTSpec_C_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_64f,
      ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, (Ipp64f*)pDst, (Ipp64f*)pDst2, (IppsFFTSpec_C_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToPerm_16s_Sfs,
      ((Ipp16s*)pSrc, (Ipp16s*)pDst, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToPack_16s_Sfs,
      ((Ipp16s*)pSrc, (Ipp16s*)pDst, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToCCS_16s_Sfs,
      ((Ipp16s*)pSrc, (Ipp16s*)pDst, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PermToR_16s_Sfs,
      ((Ipp16s*)pSrc, (Ipp16s*)pDst, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PackToR_16s_Sfs,
      ((Ipp16s*)pSrc, (Ipp16s*)pDst, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CCSToR_16s_Sfs,
      ((Ipp16s*)pSrc, (Ipp16s*)pDst, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToPerm_32f,
      ((Ipp32f*)pSrc, (Ipp32f*)pDst, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToPack_32f,
      ((Ipp32f*)pSrc, (Ipp32f*)pDst, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToCCS_32f,
      ((Ipp32f*)pSrc, (Ipp32f*)pDst, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PermToR_32f,
      ((Ipp32f*)pSrc, (Ipp32f*)pDst, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PackToR_32f,
      ((Ipp32f*)pSrc, (Ipp32f*)pDst, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CCSToR_32f,
      ((Ipp32f*)pSrc, (Ipp32f*)pDst, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToPerm_64f,
      ((Ipp64f*)pSrc, (Ipp64f*)pDst, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToPack_64f,
      ((Ipp64f*)pSrc, (Ipp64f*)pDst, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToCCS_64f,
      ((Ipp64f*)pSrc, (Ipp64f*)pDst, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PermToR_64f,
      ((Ipp64f*)pSrc, (Ipp64f*)pDst, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PackToR_64f,
      ((Ipp64f*)pSrc, (Ipp64f*)pDst, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CCSToR_64f,
      ((Ipp64f*)pSrc, (Ipp64f*)pDst, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_32sc_Sfs, 
      ((Ipp32sc*)pSrc, (Ipp32sc*)pDst, (IppsFFTSpec_C_32sc*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsFFTInv_CToC_32sc_Sfs, 
      ((Ipp32sc*)pSrc, (Ipp32sc*)pDst, (IppsFFTSpec_C_32sc*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsFFTFwd_RToCCS_32s_Sfs, 
      ((Ipp32s*)pSrc, (Ipp32s*)pDst, (IppsFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsFFTInv_CCSToR_32s_Sfs, 
      ((Ipp32s*)pSrc, (Ipp32s*)pDst, (IppsFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsFFTFwd_RToCCS_16s32s_Sfs, 
      ((Ipp16s*)pSrc, (Ipp32s*)pDst, (IppsFFTSpec_R_16s32s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsFFTInv_CCSToR_32s16s_Sfs, 
      ((Ipp32s*)pSrc, (Ipp16s*)pDst, (IppsFFTSpec_R_16s32s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer))


   FUNC_CALL(ippsFFTFwd_CToC_16sc_ISfs,
      ((Ipp16sc*)pSrc, (IppsFFTSpec_C_16sc*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_16sc_ISfs,
      ((Ipp16sc*)pSrc, (IppsFFTSpec_C_16sc*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_16s_ISfs,
      ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (IppsFFTSpec_C_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_16s_ISfs,
      ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (IppsFFTSpec_C_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_32fc_I,
      ((Ipp32fc*)pSrc, (IppsFFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_32fc_I,
      ((Ipp32fc*)pSrc, (IppsFFTSpec_C_32fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_32f_I,
      ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, (IppsFFTSpec_C_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_32f_I,
      ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, (IppsFFTSpec_C_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_64fc_I,
      ((Ipp64fc*)pSrc, (IppsFFTSpec_C_64fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_64fc_I,
      ((Ipp64fc*)pSrc, (IppsFFTSpec_C_64fc*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_64f_I,
      ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, (IppsFFTSpec_C_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CToC_64f_I,
      ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, (IppsFFTSpec_C_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_CToC_32sc_ISfs,
      ((Ipp32sc*)pSrc, (IppsFFTSpec_C_32sc*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsFFTInv_CToC_32sc_ISfs,
      ((Ipp32sc*)pSrc, (IppsFFTSpec_C_32sc*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsFFTFwd_RToPerm_16s_ISfs,
      ((Ipp16s*)pSrc, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToPack_16s_ISfs,
      ((Ipp16s*)pSrc, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToCCS_16s_ISfs,
      ((Ipp16s*)pSrc, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PermToR_16s_ISfs,
      ((Ipp16s*)pSrc, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PackToR_16s_ISfs,
      ((Ipp16s*)pSrc, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CCSToR_16s_ISfs,
      ((Ipp16s*)pSrc, (IppsFFTSpec_R_16s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToPerm_32f_I,
      ((Ipp32f*)pSrc, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToPack_32f_I,
      ((Ipp32f*)pSrc, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToCCS_32f_I,
      ((Ipp32f*)pSrc, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PermToR_32f_I,
      ((Ipp32f*)pSrc, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PackToR_32f_I,
      ((Ipp32f*)pSrc, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CCSToR_32f_I, ((Ipp32f*)pSrc, (IppsFFTSpec_R_32f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToPerm_64f_I, ((Ipp64f*)pSrc, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToPack_64f_I, ((Ipp64f*)pSrc, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToCCS_64f_I, ((Ipp64f*)pSrc, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PermToR_64f_I, ((Ipp64f*)pSrc, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_PackToR_64f_I, ((Ipp64f*)pSrc, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTInv_CCSToR_64f_I, ((Ipp64f*)pSrc, (IppsFFTSpec_R_64f*)m_pSpec, (Ipp8u*)m_buffer ))
   FUNC_CALL(ippsFFTFwd_RToCCS_32s_ISfs, ((Ipp32s*)pSrc, (IppsFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer))
   FUNC_CALL(ippsFFTInv_CCSToR_32s_ISfs, ((Ipp32s*)pSrc, (IppsFFTSpec_R_32s*)m_pSpec, scaleFactor, (Ipp8u*)m_buffer))

   return stsNoFunction;
}

void CRunFFT::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CMyString initParms;
   initParms << m_order << ", " 
             <<(EIppFlag)m_flag << ", "  
             << m_hint << ",..";
   pHisto->AddFuncString(m_InitName, initParms);
   CippsRun::AddHistoFunc(pHisto,vecPos);
}






