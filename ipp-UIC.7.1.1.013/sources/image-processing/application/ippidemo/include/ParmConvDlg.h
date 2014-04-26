/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmConvDlg.h: interface for CParmConvDlg class.
// CParmConvDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCONVDLG_H__23E8DE42_060A_49A4_97A2_2AF2F1631154__INCLUDED_)
#define AFX_PARMCONVDLG_H__23E8DE42_060A_49A4_97A2_2AF2F1631154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmConvDlg dialog

class CParmConvDlg : public CParamDlg
{
// Construction
public:
   CParmConvDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmConvDlg)
   enum { IDD = IDD_PARM_CONVFULL };
   CStatic  m_DivStatic;
   CSpinButtonCtrl   m_DivSpin;
   CEdit m_DivEdit;
   CString  m_DivStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmConvDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmConvDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCONVDLG_H__23E8DE42_060A_49A4_97A2_2AF2F1631154__INCLUDED_)
