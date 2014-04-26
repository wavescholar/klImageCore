
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFirSparse.h : interface for the CRunFirSparse class.
// CRunFirSparse class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined __RUNFIRSPARSE_H__
#define __RUNFIRSPARSE_H__

#pragma once

#include "RunFirD.h"

class CFirSparse {
public:
   CFirSparse() : m_pTapsVector(NULL), m_pNZTaps(NULL), m_pNZTapPos(NULL), m_NZTapsLen(0) {}
   void Init(CVector* pVector);
   int Sparse(double epsilon);
   Ipp32s* GetNZTapPos();
   int GetNZTapsLen();
   Ipp32f* GetNZTaps();
   CVector* GetSparsedVector();
protected:
   CVector* m_pTapsVector;
   CVector m_SparsedVector;
   Ipp32f* m_pNZTaps;
   Ipp32s* m_pNZTapPos;
   int m_NZTapsLen;

   void Create();
   void Delete();
};

class CRunFirSparse : public CRunFirD  
{
public:
   CRunFirSparse();
   virtual ~CRunFirSparse();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual IppStatus CallIppFunction();
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);
   virtual CString GetHistoryParms();

   CFirSparse m_FirSparse;
   double     m_epsilon;
   void* m_pState;
   Ipp8u* m_pBuffer;
};

#endif // !defined __RUNFIRSPARSE_H__
