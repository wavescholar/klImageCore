/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// Run1S2.cpp : implementation of the CRun1S2 class.
// CRun1S2 class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "Run1S2.h"
#include "Parm1S2Dlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRun1S2::CRun1S2()
{
   magnScaleFactor = 0;
   phaseScaleFactor = 0;

}

CRun1S2::~CRun1S2()
{

}

BOOL CRun1S2::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC | VEC_DST | VEC_DST2;
   return TRUE;
}

CParamDlg* CRun1S2::CreateDlg() { 
   return new CParm1S2Dlg;
}

void CRun1S2::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParm1S2Dlg *pDlg = (CParm1S2Dlg*)parmDlg;
   if (save) {
      magnScaleFactor = pDlg->magnScaleFactor;
      phaseScaleFactor = pDlg->phaseScaleFactor;
   } else {
      pDlg->magnScaleFactor = magnScaleFactor;
      pDlg->phaseScaleFactor = phaseScaleFactor;
   }
}

IppStatus CRun1S2::CallIppFunction()
{
   FUNC_CALL( ippsCartToPolar_16sc_Sfs, ((Ipp16sc*)pSrc, (Ipp16s*)pDst, (Ipp16s*)pDst2, len, 
      magnScaleFactor, phaseScaleFactor))

   return stsNoFunction;
}

CString CRun1S2::GetHistoryParms()
{
   CMyString parms;
   parms << magnScaleFactor << phaseScaleFactor;
   return parms;
}
