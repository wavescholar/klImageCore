/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSampDlg.h: interface for CParmSampDlg class.
// CParmSampDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMSAMPDLG_H__A2CFAF29_22DD_45B3_858C_C4FDA68ED119__INCLUDED_)
#define AFX_PARMSAMPDLG_H__A2CFAF29_22DD_45B3_858C_C4FDA68ED119__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmSampDlg dialog

class CParmSampDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmSampDlg)
// Construction
public:
   CParmSampDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmSampDlg)
   enum { IDD = IDD_PARM_SAMP };
   CSpinButtonCtrl   m_phaseSpin;
   CSpinButtonCtrl   m_factorSpin;
   int      m_phase;
   int      m_factor;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmSampDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmSampDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMSAMPDLG_H__A2CFAF29_22DD_45B3_858C_C4FDA68ED119__INCLUDED_)
