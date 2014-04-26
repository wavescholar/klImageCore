/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmAlphaCDlg.h: interface for CParmAlphaCDlg class.
// CParmAlphaCDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMALPHASDLG_H__4E085302_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_)
#define AFX_PARMALPHASDLG_H__4E085302_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ParmAlphaDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmAlphaCDlg dialog

class CParmAlphaCDlg : public CParmAlphaDlg
{
// Construction
public:
   CParmAlphaCDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmAlphaCDlg)
   enum { IDD = IDD_PARM_ALPHAS };
   CString  m_Alpha1Str;
   CString  m_Alpha2Str;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmAlphaCDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmAlphaCDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMALPHASDLG_H__4E085302_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_)
