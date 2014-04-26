/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCompColorDlg.h: interface for CParmCompColorDlg class.
// CParmCompColorDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCOMPCOLORDLG_H__B2667392_E72F_11D2_8EEC_00AA00A03C3C__INCLUDED_)
#define AFX_PARMCOMPCOLORDLG_H__B2667392_E72F_11D2_8EEC_00AA00A03C3C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CParmCompColorDlg dialog

#include "ParamDlg.h"

class CParmCompColorDlg : public CParamDlg
{
// Construction
public:
   CParmCompColorDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCompColorDlg)
   enum { IDD = IDD_PARM_COMPCOLOR };
   CButton  m_colorKeyEdit[4];
   CMyString  m_colorKeyStr[4];
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCompColorDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
    void ShowColorKey();

   // Generated message map functions
   //{{AFX_MSG(CParmCompColorDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCOMPCOLORDLG_H__B2667392_E72F_11D2_8EEC_00AA00A03C3C__INCLUDED_)
