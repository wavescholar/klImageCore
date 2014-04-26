/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// StringDlg.h : interface for the String Inserting dialog.
// Command: Menu-Edit-Insert
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGDLG_H__94702645_B83D_4AE6_BD48_4DA47EF48DEB__INCLUDED_)
#define AFX_STRINGDLG_H__94702645_B83D_4AE6_BD48_4DA47EF48DEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StringDlg.h : header file
//

#include "SymbolDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CStringDlg dialog

class CStringDlg : public CSymbolDlg
{
   DECLARE_DYNAMIC(CStringDlg)
// Construction
public:
   CStringDlg(CWnd* pParent = NULL);   // standard constructor
   virtual void UpdateString();

// Dialog Data
   int m_start;
   int m_end;
   //{{AFX_DATA(CStringDlg)
   enum { IDD = IDD_SET_STRING };
   CEdit m_StringEdit;
   CString  m_String;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CStringDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   BOOL m_bSetFocus;

   // Generated message map functions
   //{{AFX_MSG(CStringDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnSetfocusString();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STRINGDLG_H__94702645_B83D_4AE6_BD48_4DA47EF48DEB__INCLUDED_)
