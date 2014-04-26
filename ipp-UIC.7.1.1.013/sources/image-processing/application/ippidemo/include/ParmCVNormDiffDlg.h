/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVNormDiffDlg.h: interface for CParmCVNormDiffDlg class.
// CParmCVNormDiffDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCVNORMDIFFDLG_H__333B395B_77C7_4C1B_B08E_5916BB572E67__INCLUDED_)
#define AFX_PARMCVNORMDIFFDLG_H__333B395B_77C7_4C1B_B08E_5916BB572E67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParmCVNormDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CParmCVNormDiffDlg dialog

class CParmCVNormDiffDlg : public CParmCVNormDlg
{
// Construction
public:
   CParmCVNormDiffDlg(CRunCVNorm* pRun, UINT nID = IDD);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCVNormDiffDlg)
   enum { IDD = IDD_PARMCV_NORMDIFF };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA

protected:
   virtual CFunc GetFunc(int idx);
   virtual int GetFuncNumber() { return 6;}

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCVNormDiffDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmCVNormDiffDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCVNORMDIFFDLG_H__333B395B_77C7_4C1B_B08E_5916BB572E67__INCLUDED_)
