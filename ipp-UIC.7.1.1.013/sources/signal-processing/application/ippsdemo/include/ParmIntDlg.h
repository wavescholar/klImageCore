/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmIntDlg.h: interface for CParmIntDlg class.
// CParmIntDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMINTDLG_H__744001D1_F04B_4DC1_8890_C34800856280__INCLUDED_)
#define AFX_PARMINTDLG_H__744001D1_F04B_4DC1_8890_C34800856280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmIntDlg dialog

class CParmIntDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmIntDlg)
// Construction
public:
   CParmIntDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   int m_MinVal;
   int m_MaxVal;
   //{{AFX_DATA(CParmIntDlg)
   enum { IDD = IDD_PARM_INT };
   CSpinButtonCtrl   m_ValSpin;
   CEdit m_ValEdit;
   int      m_Val;
   CString  m_ValName;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmIntDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmIntDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMINTDLG_H__744001D1_F04B_4DC1_8890_C34800856280__INCLUDED_)
