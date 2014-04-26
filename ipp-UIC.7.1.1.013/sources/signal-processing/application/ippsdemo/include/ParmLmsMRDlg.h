/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmLmsMRDlg.h: interface for CParmLmsMRDlg class.
// CParmLmsMRDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMLMSMRDLG_H__53E9166B_A9FB_4D9D_8D4E_44BF9F2E7AF8__INCLUDED_)
#define AFX_PARMLMSMRDLG_H__53E9166B_A9FB_4D9D_8D4E_44BF9F2E7AF8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParmLmsDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmLmsMRDlg dialog

class CParmLmsMRDlg : public CParmLmsDlg
{
   DECLARE_DYNAMIC(CParmLmsMRDlg)
// Construction
public:
   CParmLmsMRDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmLmsMRDlg)
   enum { IDD = IDD_PARM_LMSMR };
   CSpinButtonCtrl   m_UpdateSpin;
   CSpinButtonCtrl   m_StepSpin;
   CString  m_StepStr;
   CString  m_UpdateStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmLmsMRDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmLmsMRDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMLMSMRDLG_H__53E9166B_A9FB_4D9D_8D4E_44BF9F2E7AF8__INCLUDED_)
