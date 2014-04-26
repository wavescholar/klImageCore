
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChInsert.cpp : implementation of the CRunChInsertclass.
// CRunChInsertclass processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "RunChInsert.h"
#include "Histo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunChInsert::CRunChInsert()
{
   m_len = 0;
   m_lenSave = 0;
   m_pos = 0;
   m_offset = 0;
   m_sublen = 0;
   m_bInsert = FALSE;
}

CRunChInsert::~CRunChInsert()
{

}

BOOL CRunChInsert::Open(CFunc func)
{
   if (!CippsRun::Open(func)) return FALSE;
   m_bInsert = func.Found(_T("Insert"));
   if (m_bInsert)
      m_UsedVectors = VEC_SRC | VEC_SRC2 | VEC_DST;
   return TRUE;
}

int CRunChInsert::GetDstLength()
{
   if (m_bInsert)
      return m_pDocSrc->GetVector()->Length() + m_pDocSrc2->GetVector()->Length();
   else
      return m_pDocSrc->GetVector()->Length();
}

CMyString CRunChInsert::GetVectorName(int vecPos)
{
   if (vecPos == VEC_SRC2)
      return _T("Insert");
   else
      return CippsRun::GetVectorName(vecPos);
}

BOOL CRunChInsert::GetPickID(int vecPos, UINT& pickID, UINT& grabID)
{
   if (vecPos == VEC_SRC2) {
      pickID = IDC_CURSOR_PI;
      grabID = IDC_CURSOR_I;
      return TRUE;
   } 
   return CippsRun::GetPickID(vecPos, pickID, grabID);
}

BOOL CRunChInsert::PrepareSrc()
{
   CVector* pSrcVector = m_pDocSrc->GetVector();
   m_len = pSrcVector->Length();
   m_lenSave = m_len;
   if (m_Func.Inplace() && m_bInsert) {
      pSrcVector->SetLength(
           pSrcVector->Length() + m_pDocSrc2->GetVector()->Length());
   }
   return TRUE;
}

void CRunChInsert::PrepareParameters()
{
   CippsRun::PrepareParameters();
   CVector* pSrcVector = m_pDocSrc->GetVector();
   m_pos = pSrcVector->GetPos();
   m_offset = m_pos;
   m_sublen = 0;
   if (pSrcVector->IsVoi())
      pSrcVector->GetVoi(m_offset, m_sublen);
   pSrc = pSrcVector->GetData();
}

void CRunChInsert::ActivateDst()
{
   if (m_Func.Inplace()) {
      CVector* pVector = m_pDocSrc->GetVector();
      pVector->SetLength(m_len);
      if (!m_bInsert) {
         pVector->SetPos(m_offset);
      }
      ActivateDoc((CippsDemoDoc*)m_pDocSrc2);
      ActivateDoc((CippsDemoDoc*)m_pDocSrc);
   } else {
      CVector* pVector = m_pDocDst->GetVector();
      if (m_bInsert) {
         pVector->SetPos(m_pos);
      } else {
         pVector->SetPos(m_offset);
         pVector->SetLength(m_len - m_sublen);
      }
      CippsRun::ActivateDst();
   }
}

void CRunChInsert::SaveInplaceVector()
{
   m_InplaceVector = *((CVector*)(CippsDemoDoc*)m_pDocSrc);
}

void CRunChInsert::SetInplaceParms()
{
   SetParams(&m_InplaceVector,pSrc,m_lenSrc);
}

void CRunChInsert::ResetInplaceParms()
{
   SetParams((CippsDemoDoc*)m_pDocSrc,pSrc,m_lenSrc);
}

void CRunChInsert::Loop(int num)
{
   while (num--) {
      m_len = m_lenSave;
      CallIppFunction();
   }
}

IppStatus CRunChInsert::CallIppFunction()
{
   FUNC_CH_CALL(ippsInsert_8u_I,  ((Ipp8u*)pSrc2, m_lenSrc2, (Ipp8u*)pSrc,
                                      &m_len, m_pos))
   FUNC_CH_CALL(ippsInsert_16u_I, ((Ipp16u*)pSrc2, m_lenSrc2, (Ipp16u*)pSrc,
                                      &m_len, m_pos))
   FUNC_CH_CALL(ippsInsert_8u,    ((Ipp8u*)pSrc, m_len, (Ipp8u*)pSrc2,
                                      m_lenSrc2, (Ipp8u*)pDst, m_pos))
   FUNC_CH_CALL(ippsInsert_16u,   ((Ipp16u*)pSrc, m_len, (Ipp16u*)pSrc2,
                                      m_lenSrc2, (Ipp16u*)pDst, m_pos))
   FUNC_CH_CALL(ippsRemove_8u_I,  ((Ipp8u*)pSrc, &m_len, m_offset, m_sublen))
   FUNC_CH_CALL(ippsRemove_16u_I, ((Ipp16u*)pSrc, &m_len, m_offset, m_sublen))
   FUNC_CH_CALL(ippsRemove_8u,    ((Ipp8u*)pSrc, m_len, (Ipp8u*)pDst, m_offset, m_sublen))
   FUNC_CH_CALL(ippsRemove_16u,   ((Ipp16u*)pSrc, m_len, (Ipp16u*)pDst, m_offset, m_sublen))

   return stsNoFunction;
}

void CRunChInsert::SetHistory()
{
   if (m_Func.Inplace() && m_bInsert) {
      CHisto* pHisto = m_pDocSrc->GetVector()->GetHisto();
      pHisto->AddTail(m_pDocSrc2->GetTitle());
      AddHistoFunc(pHisto);
   } else {
      CippsRun::SetHistory();
   }
}

CString CRunChInsert::GetHistoryParms()
{
   CMyString parms;
   if (m_Func.Inplace()) 
      parms << m_lenSave << _T(", ");
   if (m_bInsert)
      parms << m_pos;
   else
      parms << m_offset << _T(", ") << m_sublen;
   return parms;
}


