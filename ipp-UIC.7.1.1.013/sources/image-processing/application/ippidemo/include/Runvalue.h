/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunValue.h: interface for the CRunValue class.
// CRunValue class processes image by ippIP functions listed in
// CallIppFunction member function.
// See CRun & CippiRun classes for more information.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNVALUE_H__6AF4C0A5_C1C8_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_RUNVALUE_H__6AF4C0A5_C1C8_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ippiRun.h"
#include "Value.h"

class CRunValue : public CippiRun
{
public:
   CRunValue();
   virtual ~CRunValue();
   virtual BOOL IsMovie(CFunc func);
protected:
   virtual BOOL Open(CFunc func);
   virtual void Close();
   virtual CParamDlg* CreateDlg();
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL BeforeCall();
   virtual CString GetHistoryParms();
   virtual int SetMovieArgs(int tim, CMyString srcInfo, CMyString& dstInfo,
                            int& numReps);
   virtual void ResetMovieArgs();

   CVector m_value ;

   CVectorUnit m_valShift;
   CVectorUnit m_valDiv;
   CVectorUnit m_valAdd;
   CVectorUnit m_valMul;
   CVectorUnit m_valMulScale;
   CVectorUnit m_valAnd;

   int m_numCoi;
};

#endif // !defined(AFX_RUNVALUE_H__6AF4C0A5_C1C8_11D1_AE6B_444553540000__INCLUDED_)
