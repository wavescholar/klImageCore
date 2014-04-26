/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// OptTimingDlg.h : interface for the Timing Options dialog.
// Command: Menu-Options-Timing
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTTIMINGDLG_H__98D7CAFC_727E_4FBF_A45F_AA9E67CB0B29__INCLUDED_)
#define AFX_OPTTIMINGDLG_H__98D7CAFC_727E_4FBF_A45F_AA9E67CB0B29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Timing.h"

/////////////////////////////////////////////////////////////////////////////
// COptTimingDlg dialog

class COptTimingDlg : public CDialog
{
// Construction
public:
   COptTimingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   int m_Number;
   //{{AFX_DATA(COptTimingDlg)
   enum { IDD = IDD_OPT_TIMING };
   BOOL m_ShowStatus;
   BOOL m_ShowHisto;
   int m_UnitType;
   CButton  m_unitRadio[3];
   CStatic  m_unitStatic2;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(COptTimingDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void EnableUnit();

   // Generated message map functions
   //{{AFX_MSG(COptTimingDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnBnClickedShow();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTTIMINGDLG_H__98D7CAFC_727E_4FBF_A45F_AA9E67CB0B29__INCLUDED_)
