/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// AboutDlg.h : interface for the About dialog.
// Command: Menu-Help-About IPPSP Demo
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ABOUTDLG_H__449EE6AA_208C_4D56_9C03_29549DAD06F4__INCLUDED_)
#define AFX_ABOUTDLG_H__449EE6AA_208C_4D56_9C03_29549DAD06F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog

class CAboutDlg : public CDialog
{
// Construction
public:
   CAboutDlg();   // standard constructor

// Dialog Data
   //{{AFX_DATA(CAboutDlg)
   enum { IDD = IDD_ABOUTBOX };
   CString  m_Copyright;
   CString  m_DemoName;
   //}}AFX_DATA
   CString  m_LibTitle[3];
   CString  m_LibDate[3];
   CString  m_LibName[3];
   CString  m_LibVers[3];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CAboutDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CAboutDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDLG_H__449EE6AA_208C_4D56_9C03_29549DAD06F4__INCLUDED_)
