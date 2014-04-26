/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Run3S.cpp : implementation of the CRun3S class.
// CRun3S class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "Run3S.h"
#include "Parm3SDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRun3S::CRun3S()
{

}

CRun3S::~CRun3S()
{

}

BOOL CRun3S::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_SRC3;
   return TRUE;
}

CParamDlg* CRun3S::CreateDlg() { 
   return new CParm3SDlg;
}

IppStatus CRun3S::CallIppFunction()
{
   FUNC_CALL(ippsAddProduct_16s_Sfs,   ((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (Ipp16s*)pSrc3, len, scaleFactor ))
   FUNC_CALL(ippsAddProduct_16s32s_Sfs,((Ipp16s*)pSrc, (Ipp16s*)pSrc2, (Ipp32s*)pSrc3, len, scaleFactor ))
   FUNC_CALL(ippsAddProduct_32s_Sfs,   ((Ipp32s*)pSrc, (Ipp32s*)pSrc2, (Ipp32s*)pSrc3, len, scaleFactor ))
   FUNC_CALL(ippsAddProduct_32f,       ((Ipp32f*)pSrc, (Ipp32f*)pSrc2, (Ipp32f*)pSrc3, len ))
   FUNC_CALL(ippsAddProduct_64f,       ((Ipp64f*)pSrc, (Ipp64f*)pSrc2, (Ipp64f*)pSrc3, len ))
   FUNC_CALL(ippsAddProduct_32fc,   ((Ipp32fc*)pSrc, (Ipp32fc*)pSrc2, (Ipp32fc*)pSrc3, len ))
   FUNC_CALL(ippsAddProduct_64fc,   ((Ipp64fc*)pSrc, (Ipp64fc*)pSrc2, (Ipp64fc*)pSrc3, len ))

   return stsNoFunction;
}

