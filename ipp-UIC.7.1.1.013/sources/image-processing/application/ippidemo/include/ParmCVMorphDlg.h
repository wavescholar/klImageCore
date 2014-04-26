/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVMorphDlg.h: interface for CParmCVMorphDlg class.
// CParmCVMorphDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCVMORPHDLG_H__739F3960_FCF9_4EF0_ABDA_6658F6DBB880__INCLUDED_)
#define AFX_PARMCVMORPHDLG_H__739F3960_FCF9_4EF0_ABDA_6658F6DBB880__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParmMorphDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmCVMorphDlg dialog

class CParmCVMorphDlg : public CParmMorphDlg
{
// Construction
public:
   CParmCVMorphDlg();   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCVMorphDlg)
   enum { IDD = IDD_PARMCV_MORPH };
   int      m_BorderType;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCVMorphDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   // Generated message map functions
   //{{AFX_MSG(CParmCVMorphDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCVMORPHDLG_H__739F3960_FCF9_4EF0_ABDA_6658F6DBB880__INCLUDED_)
