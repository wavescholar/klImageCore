/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Parm2ValueDlg.h: interface for CParm2ValueDlg class.
// CParm2ValueDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARM2VALUEDLG_H__925BF9D7_462A_43B6_A1A0_DCFEE7821370__INCLUDED_)
#define AFX_PARM2VALUEDLG_H__925BF9D7_462A_43B6_A1A0_DCFEE7821370__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParmIMDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParm2ValueDlg dialog

class CParm2ValueDlg : public CParmIMDlg
{
// Construction
public:
   CParm2ValueDlg();   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParm2ValueDlg)
   enum { IDD = IDD_PARM_IM2VAL };
   CString  m_ValStr;
   CString  m_ValName;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParm2ValueDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParm2ValueDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARM2VALUEDLG_H__925BF9D7_462A_43B6_A1A0_DCFEE7821370__INCLUDED_)
