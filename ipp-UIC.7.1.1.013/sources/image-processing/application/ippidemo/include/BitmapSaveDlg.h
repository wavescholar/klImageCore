/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// BitmapSaveDlg.h : interface for the BitmapSave dialog.
//
/////////////////////////////////////////////////////////////////////////////

#include "afxwin.h"
#if !defined(AFX_BITMAPSAVEDLG_H__CE5C6A66_4C85_4B1D_A7D8_A54089CF0F58__INCLUDED_)
#define AFX_BITMAPSAVEDLG_H__CE5C6A66_4C85_4B1D_A7D8_A54089CF0F58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBitmapSaveDlg dialog

class CBitmapSaveDlg : public CDialog
{
// Construction
public:
   CBitmapSaveDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   static BOOL m_HideDialog;
   //{{AFX_DATA(CBitmapSaveDlg)
   enum { IDD = IDD_BMP_SAVE };
   //}}AFX_DATA

   BOOL m_bConvert;


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CBitmapSaveDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CBitmapSaveDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnConvert();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
    CStatic m_Static1;
    CStatic m_Static2;
    CButton m_ConvertButton;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPSAVEDLG_H__CE5C6A66_4C85_4B1D_A7D8_A54089CF0F58__INCLUDED_)
