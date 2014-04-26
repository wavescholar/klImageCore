/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterColDlg.h: interface for CParmFilterColDlg class.
// CParmFilterColDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFILTERCOLDLG_H__E849C879_A5F0_4168_98DA_601ADF37649F__INCLUDED_)
#define AFX_PARMFILTERCOLDLG_H__E849C879_A5F0_4168_98DA_601ADF37649F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParmFilterCustomDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmFilterColDlg dialog

class CParmFilterColDlg : public CParmFilterCustomDlg
{
// Construction
public:
   CParmFilterColDlg(UINT nID = IDD);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmFilterColDlg)
   enum { IDD = IDD_PARM_FILCOL };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFilterColDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmFilterColDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFILTERCOLDLG_H__E849C879_A5F0_4168_98DA_601ADF37649F__INCLUDED_)
