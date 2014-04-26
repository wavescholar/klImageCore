
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunIirSparse.h : interface for the CRunIirSparse class.
// CRunIirSparse class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined __RUNIIRSPARSE_H__
#define __RUNIIRSPARSE_H__

#pragma once

#include "RunIir.h"

class CIirSparse {
public:
   CIirSparse() : m_pTapsVector(NULL), m_pNZTaps(NULL), m_pNZTapPos(NULL), m_NZTapsLen1(0), m_NZTapsLen2(0) {}
   void Init(CVector* pVector);
   void Sparse(double epsilon);
   Ipp32s* GetNZTapPos();
   int GetNZTapsLen1();
   int GetNZTapsLen2();
   Ipp32f* GetNZTaps();
   CVector* GetSparsedVector();
protected:
   CVector* m_pTapsVector;
   CVector m_SparsedVector;
   Ipp32f* m_pNZTaps;
   Ipp32s* m_pNZTapPos;
   int m_NZTapsLen1;
   int m_NZTapsLen2;

   void Create();
   void Delete();
};

class CRunIirSparse: public CRunIir  
{
public:
   CRunIirSparse();
   virtual ~CRunIirSparse();
protected:
   virtual BOOL Open(CFunc func);
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual IppStatus CallIppFunction();
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);
   virtual CString GetHistoryParms();

   CIirSparse m_IirSparse;
   double     m_epsilon;
   Ipp8u*     m_pBuffer;
};

#endif // !defined __RUNIIRSPARSE_H__
