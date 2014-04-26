/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

#if !defined(AFX_HISTEVENDLG_H__1AD4DA39_B594_48B6_AB98_CCB29E33F282__INCLUDED_)
#define AFX_HISTEVENDLG_H__1AD4DA39_B594_48B6_AB98_CCB29E33F282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistEvenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistEvenDlg dialog

class CHistEvenDlg : public CDialog
{
// Construction
public:
   CHistEvenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   int m_num;
   //{{AFX_DATA(CHistEvenDlg)
   enum { IDD = IDD_HIST_EVEN };
   //}}AFX_DATA
   CStatic  m_UpperStatic[4];
   CEdit m_UpperEdit[4];
   CStatic  m_NumStatic[4];
   CSpinButtonCtrl   m_NumSpin[4];
   CEdit m_NumEdit[4];
   CStatic  m_LowerStatic[4];
   CEdit m_LowerEdit[4];
   CButton  m_GroupStatic[4];
   CMyString   m_LowerStr[4];
   CMyString   m_NumStr[4];
   CMyString   m_UpperStr[4];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CHistEvenDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CHistEvenDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTEVENDLG_H__1AD4DA39_B594_48B6_AB98_CCB29E33F282__INCLUDED_)
