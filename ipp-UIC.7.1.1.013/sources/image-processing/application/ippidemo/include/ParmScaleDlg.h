/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmScaleDlg.h: interface for CParmScaleDlg class.
// CParmScaleDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMSCALEDLG_H__D8CD3536_72DE_49DD_B50E_5C7F679AD1DE__INCLUDED_)
#define AFX_PARMSCALEDLG_H__D8CD3536_72DE_49DD_B50E_5C7F679AD1DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmScaleDlg dialog

class CParmScaleDlg : public CParamDlg
{
// Construction
public:
   CParmScaleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmScaleDlg)
   enum { IDD = IDD_PARM_SCALE };
   CString  m_MaxStr;
   CString  m_MinStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmScaleDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmScaleDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMSCALEDLG_H__D8CD3536_72DE_49DD_B50E_5C7F679AD1DE__INCLUDED_)
