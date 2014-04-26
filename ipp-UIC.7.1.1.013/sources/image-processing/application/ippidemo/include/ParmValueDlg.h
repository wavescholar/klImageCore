/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmValueDlg.h: interface for CParmValueDlg class.
// CParmValueDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMVALUEDLG_H__AE1CB7E2_B98D_11D1_AE6B_444553540000__INCLUDED_)
#define AFX_PARMVALUEDLG_H__AE1CB7E2_B98D_11D1_AE6B_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CParmValueDlg dialog

#include "ParamDlg.h"

class CParmValueDlg : public CParamDlg
{
// Construction
public:
   CParmValueDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CVector m_value;
   //{{AFX_DATA(CParmValueDlg)
   enum { IDD = IDD_PARM_VALUE };
   //}}AFX_DATA
   CEdit m_ValEdit[4];
   CString  m_ValStr[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmValueDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmValueDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMVALUEDLG_H__AE1CB7E2_B98D_11D1_AE6B_444553540000__INCLUDED_)
