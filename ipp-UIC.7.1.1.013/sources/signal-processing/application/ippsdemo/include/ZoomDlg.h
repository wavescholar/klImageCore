/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ZoomDlg.h : interface for the Zoom dialog.
// Command: Menu-Zoom-CustomZoom
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZOOMDLG_H__50FDECBC_29E9_42F6_9F20_8BAF73407AF0__INCLUDED_)
#define AFX_ZOOMDLG_H__50FDECBC_29E9_42F6_9F20_8BAF73407AF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZoomDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZoomDlg dialog

class CippsDemoDoc;

class CZoomDlg : public CDialog
{
// Construction
public:
   CZoomDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CippsDemoDoc* m_pDoc;
   double   m_ScaleH;
   double   m_ScaleW;
   //{{AFX_DATA(CZoomDlg)
   enum { IDD = IDD_ZOOM };
   CEdit m_ScaleWEdit;
   CEdit m_ScaleHEdit;
   int      m_PixelH;
   int      m_PixelW;
   CString  m_ScaleHStr;
   CString  m_ScaleWStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CZoomDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   double EtoD(CEdit& edit);
   void UpdatePixelW() ;
   void UpdatePixelH() ;

   // Generated message map functions
   //{{AFX_MSG(CZoomDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnChangeScaleH();
   afx_msg void OnChangeScaleW();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZOOMDLG_H__50FDECBC_29E9_42F6_9F20_8BAF73407AF0__INCLUDED_)
