/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmGammaDlg.h: interface for CParmGammaDlg class.
// CParmGammaDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMGAMMADLG_H__F769CBD0_874D_4AE2_B35E_A860493F5650__INCLUDED_)
#define AFX_PARMGAMMADLG_H__F769CBD0_874D_4AE2_B35E_A860493F5650__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmGammaDlg dialog

class CParmGammaDlg : public CParamDlg
{
// Construction
public:
   CParmGammaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   BOOL m_is1;
   //{{AFX_DATA(CParmGammaDlg)
   enum { IDD = IDD_PARM_GAMMA };
   CStatic  m_Static1;
   CEdit m_Edit1;
   CString  m_Str0;
   CString  m_Str1;
   CString  m_Name0;
   CString  m_Name1;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmGammaDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmGammaDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMGAMMADLG_H__F769CBD0_874D_4AE2_B35E_A860493F5650__INCLUDED_)
