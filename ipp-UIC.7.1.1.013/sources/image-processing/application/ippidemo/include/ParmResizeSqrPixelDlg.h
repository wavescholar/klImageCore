/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmResizeSqrPixelDlg.h: interface for CParmResizeSqrPixelDlg class.
// CParmResizeSqrPixelDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMRESIZECSQRPIXELDLG_H__298F9F0D_FBEA_4171_B9C5_9A79E5F9CBAA__INCLUDED_)
#define AFX_PARMRESIZECSQRPIXELDLG_H__298F9F0D_FBEA_4171_B9C5_9A79E5F9CBAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CParmResizeSqrPixelDlg dialog

#include "ParmResizeCDlg.h"
#include "afxwin.h"

class CParmResizeSqrPixelDlg : public CParmResizeCDlg
{
// Construction
public:
   CParmResizeSqrPixelDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   int m_interpolate;

   //{{AFX_DATA(CParmResizeSqrPixelDlg)
   enum { IDD = IDD_PARM_RESIZE_SQR };

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmResizeSqrPixelDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
    virtual void UpdateMyData(BOOL save = TRUE) ;
    void UpdateInterpolation(BOOL save = TRUE) ;

   // Generated message map functions
   //{{AFX_MSG(CParmResizeSqrPixelDlg)
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
public:
   BOOL m_interSmooth;
   BOOL m_interSubpixel;
   BOOL m_interAntiAliasing;
   CButton m_interAntiAliasing_Check;
   afx_msg void OnBnClickedInter();
   afx_msg void OnBnClickedInterSmooth();
   afx_msg void OnBnClickedInterSubpixel();
   afx_msg void OnBnClickedInterAntiAliasing();
   virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMRESIZECSQRPIXELDLG_H__298F9F0D_FBEA_4171_B9C5_9A79E5F9CBAA__INCLUDED_)
