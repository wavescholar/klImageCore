/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmImg3Dlg.h: interface for CParmImg3Dlg class.
// CParmImg3Dlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMIMG3DLG_H__0E223F41_BD6F_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_PARMIMG3DLG_H__0E223F41_BD6F_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmImg3Dlg dialog

class CParmImg3Dlg : public CParamDlg
{
// Construction
public:
   CParmImg3Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmImg3Dlg)
   enum { IDD = IDD_PARM_IMG3 };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmImg3Dlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   // Generated message map functions
   //{{AFX_MSG(CParmImg3Dlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMIMG3DLG_H__0E223F41_BD6F_11D1_AE6B_444553540000__INCLUDED_)
