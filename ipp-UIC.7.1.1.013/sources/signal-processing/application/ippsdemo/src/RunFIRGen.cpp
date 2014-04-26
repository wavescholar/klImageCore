/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFIRGen.cpp : implementation of the CRunFIRGen class.
// CRunFIRGen class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsdemo.h"
#include "RunFIRGen.h"
#include "ParmFIRGenDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunFIRGen::CRunFIRGen()
{
   m_doNormal = ippTrue;
   m_winType = ippWinHamming;
   m_freq[0] = 0.1;
   m_freq[1] = 0.4;
}

CRunFIRGen::~CRunFIRGen()
{

}

BOOL CRunFIRGen::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_UsedVectors = VEC_SRC;
   return TRUE;
}

CParamDlg* CRunFIRGen::CreateDlg() { 
   return new CParmFIRGenDlg;
}

void CRunFIRGen::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmFIRGenDlg *pDlg = (CParmFIRGenDlg*)parmDlg;
   if (save) {
      m_doNormal = (IppBool)pDlg->m_doNormal;
      m_winType = (IppWinType)pDlg->m_winType;
      for (int i=0; i<2; i++) 
         m_freq[i] = atof(pDlg->m_FreqStr[i]);
   } else {
      pDlg->m_doNormal = (BOOL)m_doNormal;
      pDlg->m_winType = (int)m_winType;
      for (int i=0; i<2; i++) 
         pDlg->m_FreqStr[i].Format("%.6g", m_freq[i]);
   }
}

IppStatus CRunFIRGen::CallIppFunction()
{
   FUNC_CALL(ippsFIRGenLowpass_64f, (m_freq[0], (Ipp64f*) pSrc, len, m_winType, m_doNormal))
   FUNC_CALL(ippsFIRGenHighpass_64f, (m_freq[1], (Ipp64f*) pSrc, len, m_winType, m_doNormal))
   FUNC_CALL(ippsFIRGenBandpass_64f, (m_freq[0], m_freq[1], (Ipp64f*) pSrc, len, m_winType, m_doNormal))
   FUNC_CALL(ippsFIRGenBandstop_64f, (m_freq[0], m_freq[1], (Ipp64f*) pSrc, len, m_winType, m_doNormal))
   return stsNoFunction;
}

CString CRunFIRGen::GetHistoryParms()
{
   CMyString parms;
   if (!m_Func.Found("Highpass"))
      parms << m_freq[0] << ", ";
   if (!m_Func.Found("Lowpass"))
      parms << m_freq[1] << ", ";
   parms << ", .., " << m_winType << m_doNormal;
   return parms;
}
