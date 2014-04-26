/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCDlg.h: interface for CParmCDlg class.
// CParmCDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCDLG_H__758B739F_710A_41FF_A510_24DC327D5B45__INCLUDED_)
#define AFX_PARMCDLG_H__758B739F_710A_41FF_A510_24DC327D5B45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmCDlg dialog

class CParmCDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmCDlg)
// Construction
public:
   CParmCDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCDlg)
   enum { IDD = IDD_PARM_C };
   CEdit m_ReEdit;
   CEdit m_ImEdit;
   CButton  m_ValButton;
   CStatic  m_ImStatic;
   CStatic  m_ReStatic;
   CString  m_ImStr;
   CString  m_ReStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmCDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCDLG_H__758B739F_710A_41FF_A510_24DC327D5B45__INCLUDED_)
