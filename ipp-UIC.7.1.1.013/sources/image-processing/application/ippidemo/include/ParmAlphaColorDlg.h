/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmAlphaColorDlg.h: interface for CParmAlphaColorDlg class.
// CParmAlphaColorDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMALPHACOLORDLG_H__4E085302_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_)
#define AFX_PARMALPHACOLORDLG_H__4E085302_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ParmCompColorDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmAlphaColorDlg dialog

class CParmAlphaColorDlg : public CParmCompColorDlg
{
// Construction
public:
   CParmAlphaColorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmAlphaColorDlg)
   enum { IDD = IDD_PARM_ALPHACOLOR };
   int      m_alphaType;
   CString  m_alpha1Str;
   CString  m_alpha2Str;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmAlphaColorDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmAlphaColorDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMALPHACOLORDLG_H__4E085302_E7ED_11D2_8EED_00AA00A03C3C__INCLUDED_)
