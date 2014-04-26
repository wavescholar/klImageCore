/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCnvrtDlg.h: interface for CParmCnvrtDlg class.
// CParmCnvrtDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCNVRTDLG_H__2FB9B487_3772_4771_B95F_AB4B822214E7__INCLUDED_)
#define AFX_PARMCNVRTDLG_H__2FB9B487_3772_4771_B95F_AB4B822214E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmCnvrtDlg dialog

class CParmCnvrtDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmCnvrtDlg)
// Construction
public:
   CParmCnvrtDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCnvrtDlg)
   enum { IDD = IDD_PARM_CNVRT };
   int      m_Rnd;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCnvrtDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmCnvrtDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCNVRTDLG_H__2FB9B487_3772_4771_B95F_AB4B822214E7__INCLUDED_)
