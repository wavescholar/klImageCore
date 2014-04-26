/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// CenterDlg.h : interface for the Set Center dialog.
// Command: Menu-Contour-Center&Shift-Set
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CENTERDLG_H__ECC74633_E0E8_11D2_8EE2_00AA00A03C3C__INCLUDED_)
#define AFX_CENTERDLG_H__ECC74633_E0E8_11D2_8EE2_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CCenterDlg dialog

class CCenterDlg : public CDialog
{
// Construction
public:
   CCenterDlg(CippiDemoDoc* pDoc);   // standard constructor

// Dialog Data
    CippiDemoDoc* m_pDoc;
   //{{AFX_DATA(CCenterDlg)
   enum { IDD = IDD_CENTER };
   //}}AFX_DATA
    double m_Center[2];
    double m_Shift[2];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CCenterDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CCenterDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CENTERDLG_H__ECC74633_E0E8_11D2_8EE2_00AA00A03C3C__INCLUDED_)
