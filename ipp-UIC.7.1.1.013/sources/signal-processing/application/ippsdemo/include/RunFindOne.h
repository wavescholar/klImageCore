/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// RunFindOne.h : interface for the CRunFindOne class.
// CRunFindOne class processes vectors by ippSP functions listed in
// CallIppFunction member function.
// See CRun & CippsRun classes for more information.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUNFINDONE_H__BEC14D06_B376_4853_BB9A_7B43025D6098__INCLUDED_)
#define AFX_RUNFINDONE_H__BEC14D06_B376_4853_BB9A_7B43025D6098__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ippsRun.h"

class CParmFindOneDlg;

class CRunFindOne : public CippsRun  
{
public:
   CRunFindOne();
   virtual ~CRunFindOne();
   BOOL SetValues(CParmFindOneDlg* pDlg);
   void CreateEvenTable(int tblLen = 0);
protected:   
   virtual void UpdateData(CParamDlg* pDlg, BOOL save = TRUE);
   virtual IppStatus CallIppFunction();
   virtual BOOL Open(CFunc funcName);
   virtual int CallIpp(BOOL bMessage = TRUE);
   virtual CString GetHistoryParms();

   int m_TableMode;
   CVector m_Table;

   Ipp16u m_inVal;
   Ipp16u m_outVal;
   int m_outIndex;

   BOOL IsIncreasingTable();
};

#endif // !defined(AFX_RUNFINDONE_H__BEC14D06_B376_4853_BB9A_7B43025D6098__INCLUDED_)
