/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Parm3Dlg.h: interface for CParm3Dlg class.
// CParm3Dlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARM3DLG_H__507C0165_7365_4E2C_81AA_BFEAD1F82E77__INCLUDED_)
#define AFX_PARM3DLG_H__507C0165_7365_4E2C_81AA_BFEAD1F82E77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParm3Dlg dialog

class CParm3Dlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParm3Dlg)
// Construction
public:
   CParm3Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParm3Dlg)
   enum { IDD = IDD_PARM_3 };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParm3Dlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParm3Dlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARM3DLG_H__507C0165_7365_4E2C_81AA_BFEAD1F82E77__INCLUDED_)
