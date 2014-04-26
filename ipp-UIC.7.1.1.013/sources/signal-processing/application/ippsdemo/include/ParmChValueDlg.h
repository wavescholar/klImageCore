
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmChValue.h: interface for CParmChValue class.
// CParmChValue dialog gets parameters for certain ippCH functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMVALUEDLG_H__DB0A04C3_EDF0_4B58_8E89_784B126B3365__INCLUDED_)
#define AFX_PARMVALUEDLG_H__DB0A04C3_EDF0_4B58_8E89_784B126B3365__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmChValueDlg dialog

class CParmChValueDlg : public CParamDlg
{
   DECLARE_DYNCREATE(CParmChValueDlg)
// Construction
public:
   CParmChValueDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor
   int GetCode();
   void SetCode(int code);

// Dialog Data
   CString m_Name;
   //{{AFX_DATA(CParmChValueDlg)
   enum { IDD = IDD_PARMCH_VALUE };
   CButton  m_SymbolButton;
   CStatic  m_CodeStatic;
   CEdit m_CodeEdit;
   CStatic  m_CharStatic;
   CEdit m_CharEdit;
   CButton  m_NameButton;
   CString  m_CharStr;
   CString  m_CodeStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmChValueDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void EnableValue(BOOL flag);
   void SetCodeStr(int code) ;
   void SetCharStr(int code) ;
   // Generated message map functions
   //{{AFX_MSG(CParmChValueDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnChangeChar();
   afx_msg void OnChangeCode();
   afx_msg void OnSymbol();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMVALUEDLG_H__DB0A04C3_EDF0_4B58_8E89_784B126B3365__INCLUDED_)
