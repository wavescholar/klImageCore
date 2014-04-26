/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmAlphaDlg.h: interface for CParmAlphaDlg class.
// CParmAlphaDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMALPHADLG_H__B2667392_E72F_11D2_8EEC_00AA00A03C3C__INCLUDED_)
#define AFX_PARMALPHADLG_H__B2667392_E72F_11D2_8EEC_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CParmAlphaDlg dialog

#include "ParamDlg.h"

class CParmAlphaDlg : public CParamDlg
{
// Construction
public:
   CParmAlphaDlg(UINT nID = IDD_PARM_ALPHA, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmAlphaDlg)
   enum { IDD = IDD_PARM_ALPHA };
   CButton  m_ShowButton;
   int      m_AlphaType;
   BOOL  m_ShowPremul;
   BOOL  m_Inplace;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmAlphaDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
    void EnableType();
    void EnableShowPremul();

   // Generated message map functions
   //{{AFX_MSG(CParmAlphaDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnType();
   afx_msg void OnInplace();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMALPHADLG_H__B2667392_E72F_11D2_8EEC_00AA00A03C3C__INCLUDED_)
