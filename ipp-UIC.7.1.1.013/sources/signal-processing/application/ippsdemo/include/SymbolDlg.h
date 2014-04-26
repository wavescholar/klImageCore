/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// SymbolDlg.h : interface for the Symbol Inserting dialog.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMBOLDLG_H__06D7C377_D26E_4CAD_ABA6_CEFC538004E5__INCLUDED_)
#define AFX_SYMBOLDLG_H__06D7C377_D26E_4CAD_ABA6_CEFC538004E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SymbolWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CSymbolDlg dialog

class CSymbolDlg : public CDialog
{
   DECLARE_DYNAMIC(CSymbolDlg)
// Construction
public:
   CSymbolDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor
   virtual void UpdateString() {}
   void UpdateCode(int code);
   static int StringToCode(CString str);
   static CString CodeToString(int code);
   static int CharToCode(CString str);
   static CString CodeToChar(int code);

// Dialog Data
   int  m_Code;
   //{{AFX_DATA(CSymbolDlg)
   enum { IDD = IDD_SET_SYMBOL };
   CSymbolWnd  m_SymbolWnd;
   CString  m_CharStr;
   CString  m_CodeStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CSymbolDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   BOOL m_bInit;
   void SetChar();

   // Generated message map functions
   //{{AFX_MSG(CSymbolDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnChangeCode();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLDLG_H__06D7C377_D26E_4CAD_ABA6_CEFC538004E5__INCLUDED_)
