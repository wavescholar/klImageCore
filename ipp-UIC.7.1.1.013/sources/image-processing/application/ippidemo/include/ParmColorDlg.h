/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmColorDlg.h: interface for CParmColorDlg class.
// CParmColorDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCOLORDLG_H__0EF91B29_D233_4B73_838F_2759D05C0D5A__INCLUDED_)
#define AFX_PARMCOLORDLG_H__0EF91B29_D233_4B73_838F_2759D05C0D5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmColorDlg dialog

class CParmColorDlg : public CParamDlg
{
// Construction
public:
   CParmColorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   float m_coeffs[3];
   //{{AFX_DATA(CParmColorDlg)
   enum { IDD = IDD_PARM_COLOR };
   //}}AFX_DATA
   CString  m_CoeffStr[3];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmColorDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmColorDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCOLORDLG_H__0EF91B29_D233_4B73_838F_2759D05C0D5A__INCLUDED_)
