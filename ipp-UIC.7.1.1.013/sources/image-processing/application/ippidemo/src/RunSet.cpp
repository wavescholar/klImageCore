/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunSet.cpp: implementation of the CRunSet class.
// CRunSet class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "ippiDemo.h"
#include "ippiDemoDoc.h"
#include "RunSet.h"
#include "ParmSetDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunSet::CRunSet()
{
   m_set8u .Init(pp8u ,4);
   m_set16s.Init(pp16s,4);
   m_set32s.Init(pp32s,4);
   m_set32f.Init(pp32f,4);
   for (int i=0; i<4; i++) {
      m_set8u .Set(i, 64);
      m_set16s.Set(i, 16000);
      m_set32s.Set(i, 130000);
      m_set32f.Set(i, 0.5f);
   }
   m_channel = 0;
}

CRunSet::~CRunSet()
{

}

BOOL CRunSet::Open(CFunc func)
{
   if (!CippiRun::Open(func)) return FALSE;

   if (func.DescrName().Found("M"))
      m_UsedVectors = VEC_SRC|VEC_MASK;
   else
      m_UsedVectors = VEC_SRC;

   m_value.Init(func.DstType(),
      func.DescrName().Found("A") ? 3 :
      func.DescrName().Found("CR") ? 1 :
      func.DstChannels());

   m_value.CopyData(m_set8u );
   m_value.CopyData(m_set16s);
   m_value.CopyData(m_set32s);
   m_value.CopyData(m_set32f);
   return TRUE;
}

void CRunSet::Close()
{
   m_set8u .CopyData(m_value);
   m_set16s.CopyData(m_value);
   m_set32s.CopyData(m_value);
   m_set32f.CopyData(m_value);
}

CParamDlg* CRunSet::CreateDlg() { return new CParmSetDlg;}

void CRunSet::UpdateData(CParamDlg* parmDlg, BOOL save)
{
    CippiRun::UpdateData(parmDlg, save);
    CParmSetDlg* pDlg = (CParmSetDlg*)parmDlg;
    if (save) {
        m_value = pDlg->m_value;
        m_channel = pDlg->m_channel;
    } else {
        pDlg->m_value = m_value;
        pDlg->m_channel = m_channel;
    }
}

IppStatus CRunSet::CallIppFunction()
{
   FUNC_CALL(ippiSet_8u_C1R,   (*(Ipp8u*) m_value, (Ipp8u*) pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_8u_C3CR,  (*(Ipp8u*) m_value, (Ipp8u*) pSrc + m_channel, srcStep, roiSize ))
   FUNC_CALL(ippiSet_8u_C4CR,  (*(Ipp8u*) m_value, (Ipp8u*) pSrc + m_channel, srcStep, roiSize ))
   FUNC_CALL(ippiSet_8u_C3R,   ( (Ipp8u*) m_value, (Ipp8u*) pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_8u_C4R,   ( (Ipp8u*) m_value, (Ipp8u*) pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_8u_AC4R,  ( (Ipp8u*) m_value, (Ipp8u*) pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16u_C1R,  (*(Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16u_C3CR, (*(Ipp16u*)m_value, (Ipp16u*)pSrc + m_channel, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16u_C4CR, (*(Ipp16u*)m_value, (Ipp16u*)pSrc + m_channel, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16u_C3R,  ( (Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16u_C4R,  ( (Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16u_AC4R, ( (Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16s_C1R,  (*(Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16s_C3CR, (*(Ipp16s*)m_value, (Ipp16s*)pSrc + m_channel, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16s_C4CR, (*(Ipp16s*)m_value, (Ipp16s*)pSrc + m_channel, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16s_C3R,  ( (Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16s_C4R,  ( (Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_16s_AC4R, ( (Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32s_C1R,  (*(Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32s_C3CR, (*(Ipp32s*)m_value, (Ipp32s*)pSrc + m_channel, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32s_C4CR, (*(Ipp32s*)m_value, (Ipp32s*)pSrc + m_channel, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32s_C3R,  ( (Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32s_C4R,  ( (Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32s_AC4R, ( (Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32f_C1R,  (*(Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32f_C3CR, (*(Ipp32f*)m_value, (Ipp32f*)pSrc + m_channel, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32f_C4CR, (*(Ipp32f*)m_value, (Ipp32f*)pSrc + m_channel, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32f_C3R,  ( (Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32f_C4R,  ( (Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_32f_AC4R, ( (Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize ))
   FUNC_CALL(ippiSet_8u_C1MR,  (*(Ipp8u*) m_value, (Ipp8u*) pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_8u_C3MR,  ( (Ipp8u*) m_value, (Ipp8u*) pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_8u_C4MR,  ( (Ipp8u*) m_value, (Ipp8u*) pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_8u_AC4MR, ( (Ipp8u*) m_value, (Ipp8u*) pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_16u_C1MR, (*(Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_16u_C3MR, ( (Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_16u_C4MR, ( (Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_16u_AC4MR,( (Ipp16u*)m_value, (Ipp16u*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_16s_C1MR, (*(Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_16s_C3MR, ( (Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_16s_C4MR, ( (Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_16s_AC4MR,( (Ipp16s*)m_value, (Ipp16s*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_32s_C1MR, (*(Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_32s_C3MR, ( (Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_32s_C4MR, ( (Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_32s_AC4MR,( (Ipp32s*)m_value, (Ipp32s*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_32f_C1MR, (*(Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_32f_C3MR, ( (Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_32f_C4MR, ( (Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))
   FUNC_CALL(ippiSet_32f_AC4MR,( (Ipp32f*)m_value, (Ipp32f*)pSrc, srcStep, roiSize, (Ipp8u*)pMask, maskStep ))

   return stsNoFunction;
}

BOOL CRunSet::BeforeCall()
{
   return TRUE;
}

CString CRunSet::GetHistoryParms()
{
   CMyString parms;
   m_value.Get(parms,", ");
   if (m_value.Length() > 1)
      parms = "{" + parms + "}";
   return parms + ", ..";
}

