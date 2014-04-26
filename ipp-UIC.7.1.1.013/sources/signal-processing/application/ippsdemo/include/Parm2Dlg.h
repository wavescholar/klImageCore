/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// Parm2Dlg.h: interface for CParm2Dlg class.
// CParm2Dlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARM2DLG_H__507C0165_7365_4E2C_81AA_BFEAD1F82E77__INCLUDED_)
#define AFX_PARM2DLG_H__507C0165_7365_4E2C_81AA_BFEAD1F82E77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParm2Dlg dialog

class CParm2Dlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParm2Dlg)
// Construction
public:
   CParm2Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParm2Dlg)
   enum { IDD = IDD_PARM_2 };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParm2Dlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParm2Dlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARM2DLG_H__507C0165_7365_4E2C_81AA_BFEAD1F82E77__INCLUDED_)
