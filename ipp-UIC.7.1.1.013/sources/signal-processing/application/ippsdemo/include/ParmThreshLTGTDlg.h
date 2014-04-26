/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmThreshLTGTDlg.h: interface for CParmThreshLTGTDlg class.
// CParmThreshLTGTDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMTHRESHLTGTDLG_H__877A33B6_221B_4904_BA72_D168102058B9__INCLUDED_)
#define AFX_PARMTHRESHLTGTDLG_H__877A33B6_221B_4904_BA72_D168102058B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParamDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CParmThreshLTGTDlg dialog

class CParmThreshLTGTDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmThreshLTGTDlg)
// Construction
public:
   CParmThreshLTGTDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmThreshLTGTDlg)
   enum { IDD = IDD_PARM_THRESH_LTGT };
   //}}AFX_DATA
   CString  m_LevelStr[2];
   CString  m_ValueStr[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmThreshLTGTDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmThreshLTGTDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMTHRESHLTGTDLG_H__877A33B6_221B_4904_BA72_D168102058B9__INCLUDED_)
