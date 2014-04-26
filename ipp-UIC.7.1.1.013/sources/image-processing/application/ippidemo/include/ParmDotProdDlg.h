/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmDotProdDlg.h: interface for CParmDotProdDlg class.
// CParmDotProdDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMDOTDROD_DLG_H__0EF91B29_D233_4B73_838F_2759D05C0D5A__INCLUDED_)
#define AFX_PARMDOTDROD_DLG_H__0EF91B29_D233_4B73_838F_2759D05C0D5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CParmDotProdDlg dialog

class CParmDotProdDlg : public CParamDlg
{
// Construction
public:
   CParmDotProdDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmDotProdDlg)
   enum { IDD = IDD_PARM_DOTPROD};
   //}}AFX_DATA
   int   m_Num;
   Ipp64f m_DotProd[4];
   CEdit m_DotProdEdit[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmDotProdDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmDotProdDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   void ShowDotProd();
   DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMDOTDROD_DLG_H__0EF91B29_D233_4B73_838F_2759D05C0D5A__INCLUDED_)
