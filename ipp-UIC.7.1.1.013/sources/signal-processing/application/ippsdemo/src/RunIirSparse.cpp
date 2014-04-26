
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunIirSparse.cpp : implementation of the CRunIirSparseclass.
// CRunIirSparseclass processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ippsDemo.h"
#include "ippsDemoDoc.h"
#include "RunIirSparse.h"
#include "ParmIirSparseDlg.h"
#include "Histo.h"

CRunIirSparse::CRunIirSparse()
{
   m_epsilon = 0.01;
   m_pBuffer = NULL;
}

CRunIirSparse::~CRunIirSparse()
{

}

BOOL CRunIirSparse::Open(CFunc func) 
{
   if (!CRunIir::Open(func)) return FALSE;
   return TRUE;
}

CParamDlg* CRunIirSparse::CreateDlg()
{
   return new CParmIirSparseDlg;
}

void CRunIirSparse::UpdateData(CParamDlg* parmDlg, BOOL save)
{
   CippsRun::UpdateData(parmDlg,save);
   CParmIirSparseDlg *pDlg = (CParmIirSparseDlg*)parmDlg;
   if (save) {
      m_epsilon = pDlg->m_epsilon;
   } else {
      m_IirSparse.Init(m_pDocTaps->GetVector());
      pDlg->m_pIirSparse = &m_IirSparse;
      pDlg->m_TapTitle = m_pDocTaps->GetTitle();
      pDlg->m_epsilon = m_epsilon;
   }
}

BOOL CRunIirSparse::BeforeCall()
{
   int stateSize;
   CHECK_CALL(ippsIIRSparseGetStateSize_32f,(
       m_IirSparse.GetNZTapsLen1(), m_IirSparse.GetNZTapsLen2(), 
       m_pDocTaps->GetVector()->Length()>>1, m_pDocTaps->GetVector()->Length()>>1, 
       &stateSize));

   m_pBuffer = (Ipp8u*)ippMalloc(stateSize);

   CHECK_CALL(ippsIIRSparseInit_32f,((IppsIIRSparseState_32f**)&m_pState,
      m_IirSparse.GetNZTaps(), m_IirSparse.GetNZTapPos(), 
      m_IirSparse.GetNZTapsLen1(), m_IirSparse.GetNZTapsLen2(),
      (Ipp32f*)m_pDelay, m_pBuffer))
   return TRUE;
}

BOOL CRunIirSparse::AfterCall(BOOL bOk)
{
   ippFree(m_pBuffer);
   return TRUE;
}

IppStatus CRunIirSparse::CallIppFunction()
{
   FUNC_CALL(ippsIIRSparse_32f, ((Ipp32f*)pSrc, (Ipp32f*)pDst, len,
      (IppsIIRSparseState_32f*)m_pState))

   return stsNoFunction;
}

void CRunIirSparse::AddHistoFunc(CHisto* pHisto, int vecPos)
{
   CMyString initParms;
   CMyString initInfo;
   initParms << m_pDocTaps->GetTitle() 
         << ", " << m_IirSparse.GetNZTapsLen1()
         << ", " << m_IirSparse.GetNZTapsLen2()
         ;
   initInfo << "eps=" << m_epsilon;
   pHisto->AddFuncString("ippsIIRSparseInit_32f", initParms, initInfo);
   CippsRun::AddHistoFunc(pHisto,vecPos);
}

CString CRunIirSparse::GetHistoryParms()
{
   CMyString parm;
   parm << len;
   return parm;
}

//////////////////////////////////////////////////////////////////////////////
//    class CIirSparse implementation
//

void CIirSparse::Init(CVector* pVector)
{
   if (pVector->Type() != pp32f) {
      Delete();
      return;
   }
   m_pTapsVector = pVector;
   Create();
   Sparse(0);
}

void CIirSparse::Create()
{
   if (m_pTapsVector == NULL) return;
   int len = m_pTapsVector->Length();
   m_SparsedVector.Init(pp32f, len);
   m_pNZTaps = (Ipp32f*)ippMalloc(len*sizeof(Ipp32f));
   m_pNZTapPos = (Ipp32s*)ippMalloc(len*sizeof(Ipp32s));
}

void CIirSparse::Delete()
{   
   m_pTapsVector = NULL;
   m_SparsedVector.Deallocate();
   ippFree(m_pNZTaps);  m_pNZTaps= NULL;
   ippFree(m_pNZTapPos);  m_pNZTapPos= NULL;
   m_NZTapsLen1 = 0;
   m_NZTapsLen2 = 0;
}

void CIirSparse::Sparse(double epsilon)
{
   if (m_pTapsVector == NULL) return;
   Ipp32f* pSrcTaps = (Ipp32f*)(*m_pTapsVector);
   Ipp32f* pDstTaps = (Ipp32f*)m_SparsedVector;
   int len1 = m_pTapsVector->Length()>>1;
   int len2 = m_pTapsVector->Length()>>1;
   if (pSrcTaps[len1] == 0) return;
   Ipp32f A0 = 1/pSrcTaps[len1];
   if (epsilon < 0) epsilon = -epsilon;
   int iNZ = 0;
   int i = 0;
   for ( ; i<len1; i++) {
      Ipp32f nz = pSrcTaps[i]*A0;
      if (nz > epsilon || nz < -epsilon) {
         pDstTaps[i] = pSrcTaps[i];
         m_pNZTaps[iNZ] = nz;
         m_pNZTapPos[iNZ] = i;
         iNZ++;
      } else {
         pDstTaps[i] = 0;
      }
   }
   m_NZTapsLen1 = iNZ;
   pDstTaps[i] = pSrcTaps[i];
   for (i++; i<len1 + len2; i++) {
      Ipp32f nz = pSrcTaps[i]*A0;
      if (nz > epsilon || nz < -epsilon) {
         pDstTaps[i] = pSrcTaps[i];
         m_pNZTaps[iNZ] = -nz;
         m_pNZTapPos[iNZ] = i - len1;
         iNZ++;
      } else {
         pDstTaps[i] = 0;
      }
   }
   m_NZTapsLen2 = iNZ - m_NZTapsLen1;
}
      

Ipp32s* CIirSparse::GetNZTapPos() { return m_pNZTapPos;}

int CIirSparse::GetNZTapsLen1() { return m_NZTapsLen1;}
int CIirSparse::GetNZTapsLen2() { return m_NZTapsLen2;}

Ipp32f* CIirSparse::GetNZTaps() { return m_pNZTaps;}

CVector* CIirSparse::GetSparsedVector() { return &m_SparsedVector;}
