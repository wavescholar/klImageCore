/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterRowDlg.h: interface for CParmFilterRowDlg class.
// CParmFilterRowDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFILTERROWDLG_H__1FACACCA_A0D2_4F21_B642_C3351A88031D__INCLUDED_)
#define AFX_PARMFILTERROWDLG_H__1FACACCA_A0D2_4F21_B642_C3351A88031D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParmFilterCustomDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmFilterRowDlg dialog

class CParmFilterRowDlg : public CParmFilterCustomDlg
{
// Construction
public:
   CParmFilterRowDlg(UINT nID = IDD);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmFilterRowDlg)
   enum { IDD = IDD_PARM_FILROW };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFilterRowDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmFilterRowDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFILTERROWDLG_H__1FACACCA_A0D2_4F21_B642_C3351A88031D__INCLUDED_)
