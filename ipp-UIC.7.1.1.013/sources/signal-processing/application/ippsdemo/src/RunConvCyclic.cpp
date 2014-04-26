/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunConvCyclic.cpp : implementation of the CRunConvCyclic class.
// CRunConvCyclic class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "RunConvCyclic.h"
#include "Parm3Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunConvCyclic::CRunConvCyclic()
{
   m_SrcLen = 0;
   m_DstLen = 0;
   m_iSrc = 0;
   m_iDst = 0;
}

CRunConvCyclic::~CRunConvCyclic()
{

}

BOOL CRunConvCyclic::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   m_SrcLen = 1;
   if (func.Found("4x4")) m_SrcLen = 4;
   if (func.Found("8x8")) m_SrcLen = 8;
//   m_DstLen = m_SrcLen*2 - 1;
   m_DstLen = m_SrcLen;
   return TRUE;
}

CParamDlg* CRunConvCyclic::CreateDlg() { 
   return new CParm3Dlg;
}

int CRunConvCyclic::GetDstLength()
{
   int length = CippsRun::GetDstLength();
   return (length / m_SrcLen)*m_DstLen;
}

BOOL CRunConvCyclic::CallIpp(BOOL bMessage)
{
   IppStatus funcError = ippStsNoErr;
   IppStatus funcWarning = ippStsNoErr;
   try {
      for (m_iSrc=0, m_iDst = 0; m_iSrc<len; m_iSrc+=m_SrcLen, m_iDst+=m_DstLen) {
         IppStatus result = CallIppFunction();
         if (result < funcError) funcError = result;
         if (result > funcWarning) funcWarning = result;
      }
   } catch (CSExcept except) {
      ExcIppMessage(except);
      return FALSE;
   }
   if (funcError == ippStsNoErr) funcError = funcWarning;
   IppErrorMessage(funcError);
   return funcError >= 0 ? TRUE : FALSE;
}

void CRunConvCyclic::Loop(int num)
{
   while (num--)
      for (m_iSrc=0, m_iDst = 0; m_iSrc<len; m_iSrc+=m_SrcLen, m_iDst+=m_DstLen)
         CallIppFunction();
}

IppStatus CRunConvCyclic::CallIppFunction()
{
   FUNC_CALL(ippsConvCyclic8x8_32f,( (Ipp32f*)pSrc + m_iSrc,
       (Ipp32f*)pSrc2 + m_iSrc, (Ipp32f*)pDst + m_iDst ))
   FUNC_CALL(ippsConvCyclic8x8_16s_Sfs,( (Ipp16s*)pSrc + m_iSrc,
       (Ipp16s*)pSrc2 + m_iSrc, (Ipp16s*)pDst + m_iDst, scaleFactor ))
   FUNC_CALL(ippsConvCyclic4x4_32f32fc,( (Ipp32f*)pSrc + m_iSrc,
       (Ipp32fc*)pSrc2 + m_iSrc, (Ipp32fc*)pDst + m_iDst ))

   return stsNoFunction;
}

