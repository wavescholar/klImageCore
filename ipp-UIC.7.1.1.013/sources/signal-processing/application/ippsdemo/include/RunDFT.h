/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunDFT.h : interface for the CRunDFT class.
// CRunDFT class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNDFT_H__4DE5831F_DB89_4EE5_BDF9_2C59959F305C__INCLUDED_)
#define AFX_RUNDFT_H__4DE5831F_DB89_4EE5_BDF9_2C59959F305C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CRunDFT : public CippsRun  
{
public:
   CRunDFT();
   virtual ~CRunDFT();
protected:
   virtual CParamDlg* CreateDlg();
   virtual IppStatus CallIppFunction();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual BOOL Open(CFunc func);
   virtual BOOL BeforeCall();
   virtual BOOL AfterCall(BOOL bOk);
   virtual void AddHistoFunc(CHisto* pHisto, int vecPos);

   void* m_pSpec;
   Ipp8u* m_buffer;
   int m_flag;
   IppHintAlgorithm m_hint;

   CFunc m_InitName;
   CFunc m_FreeName;
   CFunc m_GetBufSizeName;

   IppStatus CallInit();
   IppStatus CallFree();
   IppStatus CallGetBufSize(int& sise);
};

#endif // !defined(AFX_RUNDFT_H__4DE5831F_DB89_4EE5_BDF9_2C59959F305C__INCLUDED_)
