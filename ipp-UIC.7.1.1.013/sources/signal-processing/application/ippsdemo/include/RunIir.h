/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunIir.h : interface for the CRunIir class.
// CRunIir class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNIIR_H__22C6787F_16F2_49B9_A27C_54EDEFC130EE__INCLUDED_)
#define AFX_RUNIIR_H__22C6787F_16F2_49B9_A27C_54EDEFC130EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum {iirCUSTOM, iirGAUSS, iirLAPLAS, iirNUM};

#include "RunFir.h"

class CRunIir : public CRunFir  
{
public:
   CRunIir();
   virtual ~CRunIir();
   static int GetOrder(const CVector* pTaps);
   static int GetNumQuads(const CVector* pTaps);
protected:
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL BeforeCall();
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);

   BOOL m_BQ;
   int m_order;
   int m_numQuads;

   virtual void SetInitFreeNames();
   virtual IppStatus CallInit();
   virtual IppStatus CallFree();
};

#endif // !defined(AFX_RUNIIR_H__22C6787F_16F2_49B9_A27C_54EDEFC130EE__INCLUDED_)
