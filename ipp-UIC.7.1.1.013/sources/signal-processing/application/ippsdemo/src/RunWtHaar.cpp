/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunWtHaar.cpp : implementation of the CRunWtHaar class.
// CRunWtHaar class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "RunWT.h"
#include "Parm2Dlg.h"
#include "Parm3Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunWtHaar::CRunWtHaar()
{
   m_Fwd = TRUE;
}

CRunWtHaar::~CRunWtHaar()
{

}

BOOL CRunWtHaar::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_Fwd = func.BaseName().Found("Fwd");
   if (m_Fwd)
      m_UsedVectors = VEC_SRC|VEC_DST|VEC_DST2;
   else
      m_UsedVectors = VEC_SRC|VEC_SRC2|VEC_DST;
   return TRUE;
}

CParamDlg* CRunWtHaar::CreateDlg() 
{
   if (m_Fwd) return new CParm2Dlg;
   else       return new CParm3Dlg;
}

int CRunWtHaar::GetDstLength()
{
   int length = CippsRun::GetDstLength();
   if (m_Fwd) 
      return (length + 1) >> 1;
   else if (length < m_pDocSrc->GetVector()->Length())
      return (length << 1) + 1;
   else
      return length << 1;
}

int CRunWtHaar::GetDst2Length()
{
   int length = CippsRun::GetDstLength();
   return length >> 1;
}

BOOL CRunWtHaar::BeforeCall()
{
   if (m_Fwd)
      len = m_pDocSrc->GetVector()->Length();
   else
      len = GetDstLength();
   return TRUE;
}

IppStatus CRunWtHaar::CallIppFunction()
{
   FUNC_CALL(ippsWTHaarFwd_8s, ((Ipp8s*)pSrc, len, (Ipp8s*)pDst, (Ipp8s*)pDst2 ))
   FUNC_CALL(ippsWTHaarFwd_16s,((Ipp16s*)pSrc, len,(Ipp16s*)pDst, (Ipp16s*)pDst2 ))
   FUNC_CALL(ippsWTHaarFwd_32s,((Ipp32s*)pSrc, len,(Ipp32s*)pDst, (Ipp32s*)pDst2 ))
   FUNC_CALL(ippsWTHaarFwd_64s,((Ipp64s*)pSrc, len,(Ipp64s*)pDst, (Ipp64s*)pDst2 ))
   FUNC_CALL(ippsWTHaarFwd_32f,((Ipp32f*)pSrc, len,(Ipp32f*)pDst, (Ipp32f*)pDst2 ))
   FUNC_CALL(ippsWTHaarFwd_64f,((Ipp64f*)pSrc, len,(Ipp64f*)pDst, (Ipp64f*)pDst2 ))
   FUNC_CALL(ippsWTHaarFwd_8s_Sfs, ((Ipp8s*)pSrc, len, (Ipp8s*)pDst, (Ipp8s*)pDst2, scaleFactor))
   FUNC_CALL(ippsWTHaarFwd_16s_Sfs,((Ipp16s*)pSrc, len,(Ipp16s*)pDst,(Ipp16s*)pDst2,scaleFactor ))
   FUNC_CALL(ippsWTHaarFwd_32s_Sfs,((Ipp32s*)pSrc, len,(Ipp32s*)pDst,(Ipp32s*)pDst2,scaleFactor ))
   FUNC_CALL(ippsWTHaarFwd_64s_Sfs,((Ipp64s*)pSrc, len,(Ipp64s*)pDst,(Ipp64s*)pDst2,scaleFactor ))
   FUNC_CALL(ippsWTHaarInv_8s, ((Ipp8s* )pSrc,(Ipp8s* )pSrc2,(Ipp8s*)pDst,len ))
   FUNC_CALL(ippsWTHaarInv_16s,((Ipp16s*)pSrc,(Ipp16s*)pSrc2,(Ipp16s*)pDst,len ))
   FUNC_CALL(ippsWTHaarInv_32s,((Ipp32s*)pSrc,(Ipp32s*)pSrc2,(Ipp32s*)pDst,len ))
   FUNC_CALL(ippsWTHaarInv_64s,((Ipp64s*)pSrc,(Ipp64s*)pSrc2,(Ipp64s*)pDst,len ))
   FUNC_CALL(ippsWTHaarInv_32f,((Ipp32f*)pSrc,(Ipp32f*)pSrc2,(Ipp32f*)pDst,len ))
   FUNC_CALL(ippsWTHaarInv_64f,((Ipp64f*)pSrc,(Ipp64f*)pSrc2,(Ipp64f*)pDst,len ))
   FUNC_CALL(ippsWTHaarInv_8s_Sfs, ((Ipp8s*)pSrc,(Ipp8s*)pSrc2,  (Ipp8s*)pDst,len,scaleFactor ))
   FUNC_CALL(ippsWTHaarInv_16s_Sfs,((Ipp16s*)pSrc,(Ipp16s*)pSrc2,(Ipp16s*)pDst,len,scaleFactor ))
   FUNC_CALL(ippsWTHaarInv_32s_Sfs,((Ipp32s*)pSrc,(Ipp32s*)pSrc2,(Ipp32s*)pDst,len,scaleFactor ))
   FUNC_CALL(ippsWTHaarInv_64s_Sfs,((Ipp64s*)pSrc,(Ipp64s*)pSrc2,(Ipp64s*)pDst,len,scaleFactor ))

   return stsNoFunction;
}

CString CRunWtHaar::GetHistoryParms()
{
   CMyString parms;
   return parms << len;
}
