/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// FilterDlg.h : interface for the Filter dialog.
// Command: Menu-Process-Process by + Filter button
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILTERAPPLYDLG_H__354FA3E4_B294_4069_A520_1DE1F94BF805__INCLUDED_)
#define AFX_FILTERAPPLYDLG_H__354FA3E4_B294_4069_A520_1DE1F94BF805__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CFilter.h"

/////////////////////////////////////////////////////////////////////////////
// CFilterDlg dialog

class CFilterDlg : public CDialog
{
// Construction
public:
   CFilterDlg(UINT nID, CString libTitle);

// Dialog Data
   CFilter m_Filter;
   //{{AFX_DATA(CFilterDlg)
   //}}AFX_DATA
   CButton  m_CheckButton[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CFilterDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CString m_LibTitle;
   void OnEnable(int idx);

   // Generated message map functions
   //{{AFX_MSG(CFilterDlg)
   afx_msg void OnEnable0();
   afx_msg void OnEnable1();
   afx_msg void OnEnable2();
   afx_msg void OnEnable3();
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILTERAPPLYDLG_H__354FA3E4_B294_4069_A520_1DE1F94BF805__INCLUDED_)
