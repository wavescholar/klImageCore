/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmNormDiffDlg.h: interface for CParmNormDiffDlg class.
// CParmNormDiffDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMNORMDIFFDLG_H__66421D37_F690_4A3F_98F6_210A6732E24F__INCLUDED_)
#define AFX_PARMNORMDIFFDLG_H__66421D37_F690_4A3F_98F6_210A6732E24F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParmNormDlg.h"
class CRunNorm;

/////////////////////////////////////////////////////////////////////////////
// CParmNormDiffDlg dialog

class CParmNormDiffDlg : public CParmNormDlg
{
// Construction
public:
   CParmNormDiffDlg(CRunNorm* pRun);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmNormDiffDlg)
   enum { IDD = IDD_PARM_NORM_DIFF };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmNormDiffDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   virtual int NormIndex(CString funcName);
   virtual CString NormFunc(int idx);

   // Generated message map functions
   //{{AFX_MSG(CParmNormDiffDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMNORMDIFFDLG_H__66421D37_F690_4A3F_98F6_210A6732E24F__INCLUDED_)
