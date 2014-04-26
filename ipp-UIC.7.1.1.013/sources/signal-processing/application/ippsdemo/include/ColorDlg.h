/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ColorDlg.h : interface for the Color dialog.
// Command: Menu-Options-Color
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLORDLG_H__3CAAFAB4_6BF4_44C3_B28C_3A95F1569105__INCLUDED_)
#define AFX_COLORDLG_H__3CAAFAB4_6BF4_44C3_B28C_3A95F1569105__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorDlg.h : header file
//

#include "SampleWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

class CColorDlg : public CDialog
{
// Construction
public:
   CColorDlg(CWnd* pParent = NULL);   // standard constructor

   static void SetCustomColor(int i, COLORREF clr) { m_CustomColors[i] = clr;}
   static COLORREF GetCustomColor(int i) { return m_CustomColors[i];}

// Dialog Data
   //{{AFX_DATA(CColorDlg)
   enum { IDD = IDD_COLOR };
   CSampleWnd m_Sample;
   //}}AFX_DATA
   CButton    m_ColorButton[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CColorDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   static COLORREF m_CustomColors[16];
   COLORREF m_Color[4];

   void OnColor(int i) ;
   void RedrawSample();

   // Generated message map functions
   //{{AFX_MSG(CColorDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnButtonCol0();
   afx_msg void OnButtonCol1();
   afx_msg void OnButtonCol2();
   afx_msg void OnButtonCol3();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORDLG_H__3CAAFAB4_6BF4_44C3_B28C_3A95F1569105__INCLUDED_)
