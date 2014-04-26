
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunChSplit.cpp : implementation of the CRunChSplitclass.
// CRunChSplitclass processes vectors by ippCH functions listed in
// CallIppFunction member function.
// See CRunCh & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "RunChSplit.h"
#include "ParmChSplitDlg.h"
#include "Histo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRunChSplit::CRunChSplit()
{
   m_bSplit = FALSE;
   m_bDelim = FALSE;
   m_iDoc = 0;
   m_numSrc = 3;
   m_numDst = 0;
   m_numDoc = 0;
   m_ppVec = NULL;
   m_ppDoc = NULL;
   m_pLen.Init(pp32s);
   m_delim.Init(pp8u, ',');
   m_delim.Init(pp16u, ',');
}

CRunChSplit::~CRunChSplit()
{
   DeleteD2L(); 
}

BOOL CRunChSplit::Open(CFunc func)
{
   if (!CippsRun::Open(func))
      return FALSE;
   m_bSplit = func.Found(_T("Split"));
   m_bDelim = func.Found(_T("C_"));
   m_delim.Init(func);

   if (m_bSplit) m_UsedVectors = VEC_SRC;

   int srcLen = m_pDocSrc->GetVector()->Length();
   if (m_bSplit) {
      m_numDoc = (srcLen + 1)>>1;
      m_numDst = m_numDoc;
      m_pLen.Allocate(m_numDoc);
      for (int i=0; i<m_numDoc; i++)
         m_pLen.Set(i, srcLen - (i<<1));
   } else {
      m_numDoc = m_numSrc;
   }
   if (!CippsRun::GetParms()) return FALSE;
   if (m_numDoc <= 0) {
      return FALSE;
   }
   if (m_bSplit) {
      if (m_numDst > m_numDoc)
         m_numDst = m_numDoc;
      m_numDoc = m_numDst;
   } else {
      m_numSrc = m_numDoc;
      m_pLen.Allocate(m_numDoc);
   }

   CreateD2L(m_numDoc);
   return TRUE;
}

void CRunChSplit::CreateD2L(int num)
{
   DeleteD2L();
   m_ppDoc   = new CippsDemoDoc*[num];
   m_ppVec   = new void*        [num];
   for (int i=0; i<num; i++) {
      m_ppDoc  [i] = NULL;
      m_ppVec  [i] = NULL;
   }
}

void CRunChSplit::DeleteD2L()
{
   if (m_ppDoc  ) delete[] m_ppDoc  ;  m_ppDoc   = NULL;
   if (m_ppVec  ) delete[] m_ppVec  ;  m_ppVec   = NULL;
}

void CRunChSplit::Close()
{
   CippsRun::Close();
}

BOOL CRunChSplit::PickStart()
{
   if (m_bSplit)
      return TRUE;
   CippsRun::PickStart();
   m_ppDoc[0] = (CippsDemoDoc*)m_pDocSrc;
   m_iDoc = 1;
   return TRUE;
}

BOOL CRunChSplit::PickNextPos()
{
   if (m_bSplit)
      return FALSE;
   if (m_iDoc < m_numDoc) return TRUE;
   m_PickVecPos = -1;
   return FALSE;
}

BOOL CRunChSplit::PickSetID(CMyString& vectorName, UINT& pickID, UINT& grabID)
{
   vectorName.Format(_T("Src%d"), m_iDoc + 1);
   pickID = IDC_CURSOR_PSI;
   grabID = IDC_CURSOR_SI;
   return TRUE;
}

void CRunChSplit::GrabDoc(CDemoDoc* pDoc)
{
   m_ppDoc[m_iDoc] = (CippsDemoDoc*)pDoc;
   m_iDoc++;
   pDoc->IsPicked(TRUE);
}

BOOL CRunChSplit::PrepareDst()
{
   if (m_bSplit) {
      for (int i=0; i<m_numDst; i++) {
         int item = (m_pDocSrc->GetVector()->Type() & PP_MASK)>>3;
         m_ppVec[i] = ippMalloc(m_pLen.GetInt(i)*item);
      }
      return TRUE;
   } else {
      return CippsRun::PrepareDst();
   }
}

void CRunChSplit::DeleteNewDst()
{
   if (m_bSplit) {
      for (int i=0; i<m_numDst; i++) {
         ippFree(m_ppVec[i]);
         m_ppVec[i] = NULL;
      }
   } else {
      CippsRun::DeleteNewDst();
   }
}

int CRunChSplit::GetDstLength()
{
   if (m_bSplit) return 0; 
   int len = m_bDelim ? m_numDoc - 1 : 0;
   for (int i=0; i<m_numDoc; i++)
      len += m_ppDoc[i]->Length();
   return len;
}

void CRunChSplit::PrepareParameters()
{
   if (m_bSplit) {
      SetParams((CippsDemoDoc*)m_pDocSrc, pSrc, m_lenSrc);
   } else {
      int length;
      SetParams((CippsDemoDoc*)m_pDocDst ,pDst , length);
      for (int i=0; i<m_numDoc; i++) { 
         SetParams(m_ppDoc[i] , m_ppVec[i] , length);
         m_pLen.Set(i, length);
      }
   }
}

void CRunChSplit::ActivateDst()
{
   int i;
   if (m_bSplit) {
      for (i=0; i < m_numDst; i++) {
         BOOL bNew;
         if (CreateNewDst(TRUE, m_pLen.GetInt(i), bNew, (CDemoDoc*&)(m_ppDoc[i])))
            ippsCopy_8u((Ipp8u*)m_ppVec[i],
                        (Ipp8u*)m_ppDoc[i]->GetVector()->GetData(),                          
                        m_pLen.GetInt(i)*((m_pDocSrc->GetVector()->Type() & PP_MASK)>>3));
      }
      for (i=0; i < m_numDoc; i++) {
         ippFree(m_ppVec[i]);
         m_ppVec[i] = NULL;
      }         
   }

   ActivateDoc((CippsDemoDoc*)m_pDocSrc);
   for (i=0; i<m_numDoc; i++) 
      ActivateDoc(m_ppDoc[i]);
   ActivateDoc((CippsDemoDoc*)m_pDocDst);
}

void CRunChSplit::SetHistory()
{
   if (m_bSplit) {
      for (m_iDoc=0; m_iDoc<m_numDoc; m_iDoc++)
         SetToHisto(m_ppDoc[m_iDoc], VEC_DST );
   } else {
      m_iDoc = -1;
      SetToHisto(m_pDocDst, VEC_DST );
   }
}

void CRunChSplit::SetToHistoSrc(CDemoDoc* pDoc, int vecPos)
{
   CHisto* pHisto = pDoc->GetVector()->GetHisto();
   CMyString title = pDoc->GetTitle();
   pHisto->RemoveAll();
   pHisto->AddTail(m_pDocSrc->GetVector()->GetHisto());
   if (!m_bSplit) {
      for (int i=1; i<m_numDoc; i++) 
         pHisto->AddTail(m_ppDoc[i]->GetTitle());
   }
}

CString CRunChSplit::GetVecName(int vecPos)
{
   if (!m_bSplit) return _T("");
   CMyString vecName;
   return vecName << _T("Dst[") << m_iDoc << _T("]");
}

CParamDlg* CRunChSplit::CreateDlg() { 
   return new CParmChSplitDlg;
}

void CRunChSplit::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmChSplitDlg *pDlg = (CParmChSplitDlg*)parmDlg;
   if (save) {
      m_numDoc = _ttoi(pDlg->m_NumStr);
      m_pLen.Set(pDlg->m_LenStr);
      m_delim.Set(pDlg->GetCode());
   } else {
      pDlg->m_NumStr.Format(_T("%d"), m_numDoc);
      m_pLen.Get(pDlg->m_LenStr);
      pDlg->SetCode(m_delim.GetInt());
   }
}

IppStatus CRunChSplit::CallIppFunction()
{
   FUNC_CH_CALL(ippsConcat_8u_D2L,  ((const Ipp8u** const)m_ppVec, m_pLen, m_numDoc, 
                                 (Ipp8u*)pDst))
   FUNC_CH_CALL(ippsConcat_16u_D2L, ((const Ipp16u** const)m_ppVec, m_pLen, m_numDoc, 
                                 (Ipp16u*)pDst))

   FUNC_CH_CALL(ippsConcatC_8u_D2L, ((const Ipp8u** const)m_ppVec, m_pLen, m_numDoc, (Ipp8u)m_delim, 
                                 (Ipp8u*)pDst))
   FUNC_CH_CALL(ippsConcatC_16u_D2L,((const Ipp16u** const)m_ppVec, m_pLen, m_numDoc, (Ipp16u)m_delim, 
                                 (Ipp16u*)pDst))

   FUNC_CH_CALL(ippsSplitC_8u_D2L,  ((const Ipp8u*)pSrc, m_lenSrc, (Ipp8u)m_delim, 
                                 (Ipp8u**)m_ppVec, m_pLen, &m_numDst))
   FUNC_CH_CALL(ippsSplitC_16u_D2L, ((const Ipp16u*)pSrc, m_lenSrc, (Ipp16u)m_delim, 
                                 (Ipp16u**)m_ppVec, m_pLen, &m_numDst))

   return stsNoFunction;
}

CString CRunChSplit::GetHistoryParms()
{
   CMyString parm;
   if (m_bSplit)
      parm << _T(".., ") << m_delim << _T(",.. , ") << m_numDoc;
   else if (m_bDelim)
      parm << _T(".., ") << m_numDoc << _T(", ") << m_delim << _T(",..");
   else
      parm << _T(".., ") << m_numDoc << _T(",..");
   return parm;
}
