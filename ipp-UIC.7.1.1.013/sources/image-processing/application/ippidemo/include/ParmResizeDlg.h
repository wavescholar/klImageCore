/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmResizeDlg.h: interface for CParmResizeDlg class.
// CParmResizeDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMRESIZEDLG_H__44AAAF65_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
#define AFX_PARMRESIZEDLG_H__44AAAF65_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CParmResizeDlg dialog

#include "ParamDlg.h"

class CParmResizeDlg : public CParamDlg
{
// Construction
public:
   CParmResizeDlg(UINT nID = CParmResizeDlg::IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    double m_Fract[2];
   //{{AFX_DATA(CParmResizeDlg)
   enum { IDD = IDD_PARM_RESIZE };
   int      m_Inter;
   int      m_Type;
   //}}AFX_DATA
   CString  m_FractStr[2];
   CEdit m_FractEdit[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmResizeDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
    virtual void UpdateMyData(BOOL save = TRUE) ;
    void UpdateType() ;
    void EnableFract() ;
    void UpdateFract() ;
    void FormatFract();

   // Generated message map functions
   //{{AFX_MSG(CParmResizeDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnType();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMRESIZEDLG_H__44AAAF65_E1BA_11D2_8EE4_00AA00A03C3C__INCLUDED_)
