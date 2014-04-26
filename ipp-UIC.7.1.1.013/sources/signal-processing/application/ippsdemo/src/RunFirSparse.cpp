
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFirSparse.cpp : implementation of the CRunFirSparseclass.
// CRunFirSparseclass processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CRunFirD classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "RunFirSparse.h"
#include "ParmFirSparseDlg.h"
#include "Histo.h"

CRunFirSparse::CRunFirSparse()
{
   m_epsilon = 0.01;
   m_pState = NULL;
   m_pBuffer = NULL;
}

CRunFirSparse::~CRunFirSparse()
{

}

BOOL CRunFirSparse::Open(CFunc func) 
{
   if (!CRunFirD::Open(func)) return FALSE;   
   return TRUE;
}

CParamDlg* CRunFirSparse::CreateDlg()
{
   return new CParmFirSparseDlg;
}

void CRunFirSparse::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmFirSparseDlg *pDlg = (CParmFirSparseDlg*)parmDlg;
   if (save) {
      m_epsilon = pDlg->m_epsilon;
   } else {
      m_FirSparse.Init(m_pDocTaps->GetVector());
      pDlg->m_pFirSparse = &m_FirSparse;
      pDlg->m_TapTitle = m_pDocTaps->GetTitle();
      pDlg->m_epsilon = m_epsilon;
   }
}

BOOL CRunFirSparse::BeforeCall()
{
   CRunFirD::BeforeCall();

   int stateSize;
   CHECK_CALL(ippsFIRSparseGetStateSize_32f,(
       m_FirSparse.GetNZTapsLen(), m_pDocTaps->GetVector()->Length(), &stateSize));

   m_pBuffer = (Ipp8u*)ippMalloc(stateSize);

   CHECK_CALL(ippsFIRSparseInit_32f,((IppsFIRSparseState_32f**)&m_pState,
      m_FirSparse.GetNZTaps(), m_FirSparse.GetNZTapPos(), m_FirSparse.GetNZTapsLen(),
      (Ipp32f*)m_pDelay, m_pBuffer))
   return TRUE;
}

BOOL CRunFirSparse::AfterCall(BOOL bOk)
{
   ippFree(m_pBuffer);
   return TRUE;
}

IppStatus CRunFirSparse::CallIppFunction()
{
   FUNC_CALL(ippsFIRSparse_32f, ((Ipp32f*)pSrc, (Ipp32f*)pDst, len,
      (IppsFIRSparseState_32f*)m_pState))

   return stsNoFunction;
}

void CRunFirSparse::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CMyString initParms;
   CMyString initInfo;
   initParms << m_pDocTaps->GetTitle() 
         << ", " << m_FirSparse.GetNZTapsLen();
   initInfo << "eps=" << m_epsilon;
   pHisto->AddFuncString("ippsFIRSparseInit_32f", initParms, initInfo);
   CippsRun::AddHistoFunc(pHisto,vecPos);
}

CString CRunFirSparse::GetHistoryParms()
{
   CMyString parms;
   parms << len;
   return parms;
}


//////////////////////////////////////////////////////////////////////////////
//    class CFirSparse implementation
//

void CFirSparse::Init(CVector* pVector)
{
   if (pVector->Type() != pp32f) {
      Delete();
      return;
   }
   m_pTapsVector = pVector;
   Create();
   Sparse(0);
}

void CFirSparse::Create()
{
   if (m_pTapsVector == NULL) return;
   int len = m_pTapsVector->Length();
   m_SparsedVector.Init(pp32f, len);
   m_pNZTaps = (Ipp32f*)ippMalloc(len*sizeof(Ipp32f));
   m_pNZTapPos = (Ipp32s*)ippMalloc(len*sizeof(Ipp32s));
}

void CFirSparse::Delete()
{   
   m_pTapsVector = NULL;
   m_SparsedVector.Deallocate();
   ippFree(m_pNZTaps);  m_pNZTaps= NULL;
   ippFree(m_pNZTapPos);  m_pNZTapPos= NULL;
   m_NZTapsLen = 0;
}

int CFirSparse::Sparse(double epsilon)
{
   if (m_pTapsVector == NULL) return 0;
   Ipp32f* pSrcTaps = (Ipp32f*)(*m_pTapsVector);
   Ipp32f* pDstTaps = (Ipp32f*)m_SparsedVector;
   int len = m_pTapsVector->Length();
   int iNZ = 0;
   if (epsilon < 0) epsilon = -epsilon;
   for (int i=0; i<len; i++) {
      if (pSrcTaps[i] > epsilon || pSrcTaps[i] < -epsilon) {
         pDstTaps[i] = pSrcTaps[i];
         m_pNZTaps[iNZ] = pSrcTaps[i];
         m_pNZTapPos[iNZ] = i;
         iNZ++;
      } else {
         pDstTaps[i] = 0;
      }
   }
   m_NZTapsLen = iNZ;
   return iNZ;
}
      

Ipp32s* CFirSparse::GetNZTapPos() { return m_pNZTapPos;}

int CFirSparse::GetNZTapsLen() { return m_NZTapsLen;}

Ipp32f* CFirSparse::GetNZTaps() { return m_pNZTaps;}

CVector* CFirSparse::GetSparsedVector() { return &m_SparsedVector;}
