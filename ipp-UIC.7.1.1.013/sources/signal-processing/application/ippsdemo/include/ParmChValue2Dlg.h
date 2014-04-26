
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmChValue2.h: interface for CParmChValue2 class.
// CParmChValue2 dialog gets parameters for certain ippCH functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCHVALUE2DLG_H__C7264B0E_401A_42A4_B2D8_897E59DB5DBB__INCLUDED_)
#define AFX_PARMCHVALUE2DLG_H__C7264B0E_401A_42A4_B2D8_897E59DB5DBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CParmChValue2Dlg dialog

class CParmChValue2Dlg : public CParamDlg
{
   DECLARE_DYNCREATE(CParmChValue2Dlg)
// Construction
public:
   CParmChValue2Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   int GetCode(int idx);
   void SetCode(int code, int idx);
   void SetNames(CString name1, CString name2);
   //{{AFX_DATA(CParmChValue2Dlg)
   enum { IDD = IDD_PARMCH_VALUE_2 };
   //}}AFX_DATA
   CString  m_Name[2];
   CButton  m_NameButton[2];
   CString  m_CharStr[2];
   CString  m_CodeStr[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmChValue2Dlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void SetCodeStr(int code, int idx) ;
   void SetCharStr(int code, int idx) ;
   void OnChangeChar(int idx);
   void OnChangeCode(int idx);
   void OnSymbol(int idx);
   // Generated message map functions
   //{{AFX_MSG(CParmChValue2Dlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnChangeChar0();
   afx_msg void OnChangeCode0();
   afx_msg void OnSymbol0();
   afx_msg void OnChangeChar1();
   afx_msg void OnChangeCode1();
   afx_msg void OnSymbol1();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCHVALUE2DLG_H__C7264B0E_401A_42A4_B2D8_897E59DB5DBB__INCLUDED_)
