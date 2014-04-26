
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunCopy1u.cpp : implementation of the CRunCopy1uclass.
// CRunCopy1uclass processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "RunCopy1u.h"
#include "ParmCopy1uDlg.h"

CRunCopy1u::CRunCopy1u()
{
   m_bitOffset[0]= 0;
   m_bitOffset[1]= 0;
   m_numBits = 1;
   m_maxBits = 1;
}

CRunCopy1u::~CRunCopy1u()
{

}

BOOL CRunCopy1u::Open(CFunc func) 
{
   if (!CippsRun::Open(func)) return FALSE;
   m_itemBits = m_pDocSrc->GetVector()->ItemSize() << 3;
   m_maxBits = m_pDocSrc->GetVector()->Length()*m_itemBits;
   m_numBits = m_maxBits - m_bitOffset[0]; 
   return TRUE;
}

int CRunCopy1u::GetDstLength()
{
   return (m_numBits + m_bitOffset[1] + m_itemBits - 1)/m_itemBits;
}

CParamDlg* CRunCopy1u::CreateDlg()
{
   return new CParmCopy1uDlg;
}

void CRunCopy1u::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   int i;
   CippsRun::UpdateData(parmDlg,save);
   CParmCopy1uDlg *pDlg = (CParmCopy1uDlg*)parmDlg;
   if (save) {
      for (i=0; i<2; i++) m_bitOffset[i] = pDlg->m_bitOffset[i];
      m_numBits = pDlg->m_numBits;
      if (m_numBits > m_maxBits - m_bitOffset[0])
         m_numBits = m_maxBits - m_bitOffset[0];
   } else {
      for (i=0; i<2; i++) pDlg->m_bitOffset[i] = m_bitOffset[i];
      pDlg->m_numBits = m_numBits;
      pDlg->m_maxBits = m_maxBits;
      pDlg->m_itemBits = m_itemBits;
   }
}

IppStatus CRunCopy1u::CallIppFunction()
{
   FUNC_CALL( ippsCopy_1u, ((const Ipp8u*)m_pDocSrc->GetVector()->GetData(), m_bitOffset[0], 
                            (Ipp8u*)m_pDocDst->GetVector()->GetData(), m_bitOffset[1], m_numBits))

   return stsNoFunction;
}

CString CRunCopy1u::GetHistoryParms()
{
   CMyString parm;
   parm << m_bitOffset[0] << m_bitOffset[1] << m_numBits;
   return parm;
}
