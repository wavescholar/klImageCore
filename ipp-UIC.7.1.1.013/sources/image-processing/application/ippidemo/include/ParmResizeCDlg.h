/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmResizeCDlg.h: interface for CParmResizeCDlg class.
// CParmResizeCDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMRESIZECDLG_H__298F9F0D_FBEA_4171_B9C5_9A79E5F9CBAA__INCLUDED_)
#define AFX_PARMRESIZECDLG_H__298F9F0D_FBEA_4171_B9C5_9A79E5F9CBAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CParmResizeCDlg dialog

#include "ParmResizeDlg.h"

class CParmResizeCDlg : public CParmResizeDlg
{
// Construction
public:
   CParmResizeCDlg(UINT nID = CParmResizeCDlg::IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    double m_Center[2];
   //{{AFX_DATA(CParmResizeCDlg)
   enum { IDD = IDD_PARM_RESIZE_C };
   int      m_CenterType;
   //}}AFX_DATA
   CEdit m_CenterEdit[2];
   CString  m_CenterStr[2];
   int m_Mode;
   enum {CENTER, SHIFT, SQR};


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmResizeCDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
    virtual void UpdateMyData(BOOL save = TRUE) ;
    void UpdateCType() ;
    void EnableCenter() ;
    void UpdateCenter() ;
    void FormatCenter();

   // Generated message map functions
   //{{AFX_MSG(CParmResizeCDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnCtype();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnChangeFract();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMRESIZECDLG_H__298F9F0D_FBEA_4171_B9C5_9A79E5F9CBAA__INCLUDED_)
