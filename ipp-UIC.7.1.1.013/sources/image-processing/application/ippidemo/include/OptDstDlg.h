/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// OptDstDlg.h : interface for the New Dst Conception dialog.
// Command: Menu-Options-NewDst
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTDSTDLG_H__6081A455_279B_11D3_8F32_00AA00A03C3C__INCLUDED_)
#define AFX_OPTDSTDLG_H__6081A455_279B_11D3_8F32_00AA00A03C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COptDstDlg dialog

class COptDstDlg : public CDialog
{
// Construction
public:
   COptDstDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(COptDstDlg)
   enum { IDD = IDD_OPT_DST };
   int      m_PickDst;
   int      m_NewSize;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(COptDstDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(COptDstDlg)
   afx_msg void OnCreate();
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTDSTDLG_H__6081A455_279B_11D3_8F32_00AA00A03C3C__INCLUDED_)
