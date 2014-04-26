/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRandDlg.h: interface for CParmRandDlg class.
// CParmRandDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMRANDDLG_H__ADA656C2_7C79_4075_8311_26BE14561816__INCLUDED_)
#define AFX_PARMRANDDLG_H__ADA656C2_7C79_4075_8311_26BE14561816__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmRandDlg dialog

class CParmRandDlg : public CParamDlg
{
// Construction
public:
   CParmRandDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmRandDlg)
   enum { IDD = IDD_PARM_RAND };
   CSpinButtonCtrl   m_SeedSpin;
   CString  m_SeedStr;
   //}}AFX_DATA
   CString  m_ParmStr[2];
   CString  m_Name[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmRandDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmRandDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMRANDDLG_H__ADA656C2_7C79_4075_8311_26BE14561816__INCLUDED_)
