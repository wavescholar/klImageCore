/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Run3.cpp : implementation of the CRun3 class.
// CRun3 class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "Run3.h"
#include "Parm3Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CRun3::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   if (func.BaseName() == "CartToPolar" ||
       func.BaseName() == "PolarToCart" && !(func.DstType() & PP_CPLX))
      m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST | VEC_DST2;
   return TRUE;
}

CParamDlg* CRun3::CreateDlg() { 
   return new CParm3Dlg;
}

int CRun3::GetDstLength()
{
   int length = CippsRun::GetDstLength();

   if (m_Func.BaseName() == "Conv")
      return ((CippsDemoDoc*)m_pDocSrc)->Length() + 
             ((CippsDemoDoc*)m_pDocSrc2)->Length() - 1;
   else
      return length;
}

IppStatus CRun3::CallIppFunction()
{
   FUNC_CALL(ippsCartToPolar_32f,((Ipp32f*)pSrc, (Ipp32f*)pSrc2, 
                        (Ipp32f*)pDst, (Ipp32f*)pDst2, len))
   FUNC_CALL(ippsCartToPolar_64f,((Ipp64f*)pSrc, (Ipp64f*)pSrc2, 
                        (Ipp64f*)pDst, (Ipp64f*)pDst2, len))
   FUNC_CALL(ippsPolarToCart_32fc,((Ipp32f*)pSrc,
                  (Ipp32f*)pSrc2, (Ipp32fc*)pDst, len))
   FUNC_CALL(ippsPolarToCart_64fc,((Ipp64f*)pSrc,
                  (Ipp64f*)pSrc2, (Ipp64fc*)pDst, len))
   FUNC_CALL(ippsPolarToCart_32f,((Ipp32f*)pSrc,
                  (Ipp32f*)pSrc2, (Ipp32f*)pDst, (Ipp32f*)pDst2, len))
   FUNC_CALL(ippsPolarToCart_64f,((Ipp64f*)pSrc,
                  (Ipp64f*)pSrc2, (Ipp64f*)pDst, (Ipp64f*)pDst2, len))
   FUNC_CALL( ippsRealToCplx_64f,( (Ipp64f*)pSrc, 
       (Ipp64f*)pSrc2, (Ipp64fc*)pDst, len ))
   FUNC_CALL( ippsRealToCplx_32f,( (Ipp32f*)pSrc, 
       (Ipp32f*)pSrc2, (Ipp32fc*)pDst, len ))
   FUNC_CALL( ippsRealToCplx_16s,( (Ipp16s*)pSrc, 
       (Ipp16s*)pSrc2, (Ipp16sc*)pDst, len ))
   FUNC_CALL(ippsMagnitude_16s32f,((Ipp16s*)pSrc,(Ipp16s*)pSrc2,(Ipp32f*)pDst,len))
   FUNC_CALL(ippsMagnitude_32f,((Ipp32f*)pSrc,(Ipp32f*)pSrc2,(Ipp32f*)pDst,len))
   FUNC_CALL(ippsMagnitude_64f,((Ipp64f*)pSrc,(Ipp64f*)pSrc2,(Ipp64f*)pDst,len))
   FUNC_CALL(ippsMagnitude_16s_Sfs,((Ipp16s*)pSrc,(Ipp16s*)pSrc2,
                               (Ipp16s*)pDst,len,scaleFactor))
   FUNC_CALL(ippsPhase_64f,((Ipp64f*)pSrc, (Ipp64f*)pSrc2,(Ipp64f*)pDst, len))
   FUNC_CALL(ippsPhase_32f,((Ipp32f*)pSrc, (Ipp32f*)pSrc2,(Ipp32f*)pDst, len))
   FUNC_CALL(ippsPhase_16s_Sfs,((Ipp16s*)pSrc, (Ipp16s*)pSrc2,
                                           (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsPhase_16s32f,((Ipp16s*)pSrc, (Ipp16s*)pSrc2,
                                           (Ipp32f*)pDst, len))

   FUNC_CALL(ippsAdd_16s_I,  ((Ipp16s*)pSrc,  (Ipp16s*)pSrc2,  len))
   FUNC_CALL(ippsSub_16s_I,  ((Ipp16s*)pSrc,  (Ipp16s*)pSrc2,  len))
   FUNC_CALL(ippsMul_16s_I,  ((Ipp16s*)pSrc,  (Ipp16s*)pSrc2,  len))
   FUNC_CALL(ippsAdd_32f_I,  ((Ipp32f*)pSrc,  (Ipp32f*)pSrc2,  len))
   FUNC_CALL(ippsSub_32f_I,  ((Ipp32f*)pSrc,  (Ipp32f*)pSrc2,  len))
   FUNC_CALL(ippsMul_32f_I,  ((Ipp32f*)pSrc,  (Ipp32f*)pSrc2,  len))
   FUNC_CALL(ippsAdd_64f_I,  ((Ipp64f*)pSrc,  (Ipp64f*)pSrc2,  len))
   FUNC_CALL(ippsSub_64f_I,  ((Ipp64f*)pSrc,  (Ipp64f*)pSrc2,  len))
   FUNC_CALL(ippsMul_64f_I,  ((Ipp64f*)pSrc,  (Ipp64f*)pSrc2,  len))
   FUNC_CALL(ippsAdd_32fc_I, ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, len))
   FUNC_CALL(ippsSub_32fc_I, ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, len))
   FUNC_CALL(ippsMul_32fc_I, ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, len))
   FUNC_CALL(ippsAdd_64fc_I, ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, len))
   FUNC_CALL(ippsSub_64fc_I, ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, len))
   FUNC_CALL(ippsMul_64fc_I, ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, len))
   FUNC_CALL(ippsAdd_16s_ISfs,  ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsSub_16s_ISfs,  ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsMul_16s_ISfs,  ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsAdd_16sc_ISfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsSub_16sc_ISfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsMul_16sc_ISfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsAdd_16s,    ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (Ipp16s*)pDst, len))
   FUNC_CALL(ippsSub_16s,    ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (Ipp16s*)pDst, len))
   FUNC_CALL(ippsMul_16s,    ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (Ipp16s*)pDst, len))
   FUNC_CALL(ippsAdd_32f,    ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsSub_32f,    ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsMul_32f,    ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsAdd_64f,    ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, (Ipp64f*)pDst, len))
   FUNC_CALL(ippsSub_64f,    ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, (Ipp64f*)pDst, len))
   FUNC_CALL(ippsMul_64f,    ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, (Ipp64f*)pDst, len))
   FUNC_CALL(ippsAdd_16s32f, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsSub_16s32f, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsMul_16s32f, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsAdd_32fc,   ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, (Ipp32fc*)pDst, len))
   FUNC_CALL(ippsSub_32fc,   ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, (Ipp32fc*)pDst, len))
   FUNC_CALL(ippsMul_32fc,   ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, (Ipp32fc*)pDst, len))
   FUNC_CALL(ippsAdd_64fc,   ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, (Ipp64fc*)pDst, len))
   FUNC_CALL(ippsSub_64fc,   ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, (Ipp64fc*)pDst, len))
   FUNC_CALL(ippsMul_64fc,   ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, (Ipp64fc*)pDst, len))
   FUNC_CALL(ippsAdd_16s_Sfs,  ((Ipp16s*)pSrc, (Ipp16s*)pSrc2,
                                 (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsSub_16s_Sfs,  ((Ipp16s*)pSrc, (Ipp16s*)pSrc2,
                                 (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsMul_16s_Sfs,  ((Ipp16s*)pSrc, (Ipp16s*)pSrc2,
                                 (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsAdd_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pSrc2,
                                 (Ipp16sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsSub_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pSrc2,
                                 (Ipp16sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsMul_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pSrc2,
                                 (Ipp16sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsMul_32f32fc_I, ((const Ipp32f*)pSrc, (Ipp32fc*)pSrc2, len))
   FUNC_CALL(ippsMul_32f32fc, ((const Ipp32f*)pSrc, (const Ipp32fc*)pSrc2, (Ipp32fc*)pDst, len))
   FUNC_CALL(ippsDiv_32f, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsDiv_32fc, ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, (Ipp32fc*)pDst, len))
   FUNC_CALL(ippsDiv_64f, ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, (Ipp64f*)pDst, len))
   FUNC_CALL(ippsDiv_64fc, ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, (Ipp64fc*)pDst, len))
   FUNC_CALL(ippsDiv_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsDiv_32s_Sfs, ((Ipp32s*)pSrc, (Ipp32s*)pSrc2, (Ipp32s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsDiv_32s16s_Sfs, ((Ipp16s*)pSrc, (Ipp32s*)pSrc2, (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsDiv_8u_Sfs, ((Ipp8u*)pSrc, (Ipp8u*)pSrc2, (Ipp8u*)pDst, len, scaleFactor))
   FUNC_CALL(ippsDiv_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pSrc2, (Ipp16sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsDiv_32f_I, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, len))
   FUNC_CALL(ippsDiv_32fc_I, ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, len))
   FUNC_CALL(ippsDiv_64f_I, ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, len))
   FUNC_CALL(ippsDiv_64fc_I, ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, len))
   FUNC_CALL(ippsDiv_16s_ISfs, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsDiv_32s_ISfs, ((Ipp32s*)pSrc, (Ipp32s*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsDiv_8u_ISfs, ((Ipp8u*)pSrc, (Ipp8u*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsDiv_16sc_ISfs, ((Ipp16sc*)pSrc, (Ipp16sc*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsConv_32f, ((Ipp32f*)pSrc, m_lenSrc, (Ipp32f*)pSrc2, m_lenSrc2, (Ipp32f*)pDst))
   FUNC_CALL(ippsConv_64f, ((Ipp64f*)pSrc, m_lenSrc, (Ipp64f*)pSrc2, m_lenSrc2, (Ipp64f*)pDst))
   FUNC_CALL(ippsConv_16s_Sfs, ((Ipp16s*)pSrc, m_lenSrc, (Ipp16s*)pSrc2, m_lenSrc2, (Ipp16s*)pDst, scaleFactor))
   FUNC_CALL(ippsMulPack_16s_ISfs, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsMulPerm_16s_ISfs, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, len, scaleFactor))
   FUNC_CALL(ippsMulPack_32f_I, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, len))
   FUNC_CALL(ippsMulPerm_32f_I, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, len))
   FUNC_CALL(ippsMulPack_64f_I, ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, len))
   FUNC_CALL(ippsMulPerm_64f_I, ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, len))
   FUNC_CALL(ippsMulPack_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsMulPerm_16s_Sfs, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsMulPack_32f, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsMulPerm_32f, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsMulPack_64f, ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, (Ipp64f*)pDst, len))
   FUNC_CALL(ippsMulPerm_64f, ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, (Ipp64f*)pDst, len))
   FUNC_CALL(ippsMulPackConj_32f_I, ((Ipp32f*) pSrc, (Ipp32f*) pSrc2, len))
   FUNC_CALL(ippsMulPackConj_64f_I, ((Ipp64f*) pSrc, (Ipp64f*) pSrc2, len))
   FUNC_CALL(ippsPowerSpectr_64f,( (Ipp64f*)pSrc,  (Ipp64f*)pSrc2,(Ipp64f*)pDst, len))
   FUNC_CALL(ippsPowerSpectr_32f,( (Ipp32f*)pSrc,  (Ipp32f*)pSrc2,(Ipp32f*)pDst, len))
   FUNC_CALL(ippsPowerSpectr_16s_Sfs,( (Ipp16s*)pSrc,  (Ipp16s*)pSrc2, (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsPowerSpectr_16s32f, ( (Ipp16s*)pSrc,  (Ipp16s*)pSrc2, (Ipp32f*)pDst, len))
   FUNC_CALL(ippsAnd_8u_I, ((Ipp8u*)pSrc, (Ipp8u*)pSrc2, len))
   FUNC_CALL(ippsAnd_8u, ((Ipp8u*)pSrc, (Ipp8u*)pSrc2, (Ipp8u*)pDst, len))
   FUNC_CALL(ippsAnd_16u_I, ((Ipp16u*)pSrc, (Ipp16u*)pSrc2, len))
   FUNC_CALL(ippsAnd_16u, ((Ipp16u*)pSrc, (Ipp16u*)pSrc2, (Ipp16u*)pDst, len))
   FUNC_CALL(ippsAnd_32u_I, ((Ipp32u*)pSrc, (Ipp32u*)pSrc2, len))
   FUNC_CALL(ippsAnd_32u, ((Ipp32u*)pSrc, (Ipp32u*)pSrc2, (Ipp32u*)pDst, len))
   FUNC_CALL(ippsOr_8u_I, ((Ipp8u*)pSrc, (Ipp8u*)pSrc2, len))
   FUNC_CALL(ippsOr_8u, ((Ipp8u*)pSrc, (Ipp8u*)pSrc2, (Ipp8u*)pDst, len))
   FUNC_CALL(ippsOr_16u_I, ((Ipp16u*)pSrc, (Ipp16u*)pSrc2, len))
   FUNC_CALL(ippsOr_16u, ((Ipp16u*)pSrc, (Ipp16u*)pSrc2, (Ipp16u*)pDst, len))
   FUNC_CALL(ippsOr_32u_I, ((Ipp32u*)pSrc, (Ipp32u*)pSrc2, len))
   FUNC_CALL(ippsOr_32u, ((Ipp32u*)pSrc, (Ipp32u*)pSrc2, (Ipp32u*)pDst, len))
   FUNC_CALL(ippsXor_8u_I, ((Ipp8u*)pSrc, (Ipp8u*)pSrc2, len))
   FUNC_CALL(ippsXor_8u, ((Ipp8u*)pSrc, (Ipp8u*)pSrc2, (Ipp8u*)pDst, len))
   FUNC_CALL(ippsXor_16u_I, ((Ipp16u*)pSrc, (Ipp16u*)pSrc2, len))
   FUNC_CALL(ippsXor_16u, ((Ipp16u*)pSrc, (Ipp16u*)pSrc2, (Ipp16u*)pDst, len))
   FUNC_CALL(ippsXor_32u_I, ((Ipp32u*)pSrc, (Ipp32u*)pSrc2, len))
   FUNC_CALL(ippsXor_32u, ((Ipp32u*)pSrc, (Ipp32u*)pSrc2, (Ipp32u*)pDst, len))
   FUNC_CALL(ippsMul_16u16s_Sfs, ((Ipp16u*)pSrc, (Ipp16s*)pSrc2,
       (Ipp16s*)pDst, len, scaleFactor))
   FUNC_CALL(ippsAdd_16u, ((Ipp16u*)pSrc, (Ipp16u*)pSrc2,
       (Ipp16u*)pDst, len))
   FUNC_CALL(ippsAdd_32u, ((Ipp32u*)pSrc, (Ipp32u*)pSrc2,
       (Ipp32u*)pDst, len))
   FUNC_CALL(ippsMinEvery_16s_I, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, len))
   FUNC_CALL(ippsMinEvery_32s_I, ((Ipp32s*)pSrc, (Ipp32s*)pSrc2, len))
   FUNC_CALL(ippsMinEvery_32f_I, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, len))
   FUNC_CALL(ippsMaxEvery_16s_I, ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, len))
   FUNC_CALL(ippsMaxEvery_32s_I, ((Ipp32s*)pSrc, (Ipp32s*)pSrc2, len))
   FUNC_CALL(ippsMaxEvery_32f_I, ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, len))

   FUNC_CALL(ippsAdd_8u_Sfs,     ((Ipp8u*)  pSrc, (Ipp8u*)  pSrc2,
       (Ipp8u*)  pDst, len, scaleFactor))
   FUNC_CALL(ippsSub_8u_Sfs,     ((Ipp8u*)  pSrc, (Ipp8u*)  pSrc2,
       (Ipp8u*)  pDst, len, scaleFactor))
   FUNC_CALL(ippsMul_8u_Sfs,     ((Ipp8u*)  pSrc, (Ipp8u*)  pSrc2,
       (Ipp8u*)  pDst, len, scaleFactor))
   FUNC_CALL(ippsAdd_32s_ISfs,   ((Ipp32s*) pSrc, (Ipp32s*) pSrc2,
       len, scaleFactor))
   FUNC_CALL(ippsAdd_32sc_ISfs,  ((Ipp32sc*)pSrc, (Ipp32sc*)pSrc2,
       len, scaleFactor))
   FUNC_CALL(ippsSub_32s_ISfs,   ((Ipp32s*) pSrc, (Ipp32s*) pSrc2,
       len, scaleFactor))
   FUNC_CALL(ippsSub_32sc_ISfs,  ((Ipp32sc*)pSrc, (Ipp32sc*)pSrc2,
       len, scaleFactor))
   FUNC_CALL(ippsMul_32s_ISfs,   ((Ipp32s*) pSrc, (Ipp32s*) pSrc2,
       len, scaleFactor))
   FUNC_CALL(ippsMul_32sc_ISfs,  ((Ipp32sc*)pSrc, (Ipp32sc*)pSrc2,
       len, scaleFactor))
   FUNC_CALL(ippsAdd_8u_ISfs,    ((Ipp8u*)  pSrc, (Ipp8u*)  pSrc2,
       len, scaleFactor))
   FUNC_CALL(ippsSub_8u_ISfs,    ((Ipp8u*)  pSrc, (Ipp8u*)  pSrc2,
       len, scaleFactor))
   FUNC_CALL(ippsMul_8u_ISfs,    ((Ipp8u*)  pSrc, (Ipp8u*)  pSrc2,
       len, scaleFactor))
   FUNC_CALL(ippsAdd_8u16u,  ((Ipp8u*)  pSrc, (Ipp8u*)  pSrc2,
       (Ipp16u*) pDst, len))
   FUNC_CALL(ippsMul_8u16u,  ((Ipp8u*)  pSrc, (Ipp8u*)  pSrc2,
       (Ipp16u*) pDst, len))
   FUNC_CALL(ippsAdd_32s_Sfs,    ((Ipp32s*) pSrc, (Ipp32s*) pSrc2,
       (Ipp32s*) pDst, len, scaleFactor))
   FUNC_CALL(ippsAdd_32sc_Sfs,   ((Ipp32sc*)pSrc, (Ipp32sc*)pSrc2,
       (Ipp32sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsSub_32s_Sfs,    ((Ipp32s*) pSrc, (Ipp32s*) pSrc2,
       (Ipp32s*) pDst, len, scaleFactor))
   FUNC_CALL(ippsSub_32sc_Sfs,   ((Ipp32sc*)pSrc, (Ipp32sc*)pSrc2,
       (Ipp32sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsMul_32s_Sfs,    ((Ipp32s*) pSrc, (Ipp32s*) pSrc2,
       (Ipp32s*) pDst, len, scaleFactor))
   FUNC_CALL(ippsMul_32sc_Sfs,   ((Ipp32sc*)pSrc, (Ipp32sc*)pSrc2,
       (Ipp32sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsMul_16s32s_Sfs, ((Ipp16s*)pSrc,(Ipp16s*)pSrc2,
       (Ipp32s*)pDst,len,scaleFactor))
   FUNC_CALL(ippsMul_32s32sc_ISfs, ((Ipp32s*)pSrc, (Ipp32sc*)pSrc2,
       len, scaleFactor))
   FUNC_CALL(ippsMul_32s32sc_Sfs,  ((Ipp32s*)pSrc, (Ipp32sc*)pSrc2,
       (Ipp32sc*)pDst, len, scaleFactor))
   FUNC_CALL(ippsAdd_16s32s_I, ((const Ipp16s*)pSrc, (Ipp32s*)pSrc2, len))

   return stsNoFunction;
}

