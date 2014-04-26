/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmThreshDlg.h: interface for CParmThreshDlg class.
// CParmThreshDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMTHRESHDLG_H__C85F6B26_CC8F_47E0_80D6_5755B5CABB36__INCLUDED_)
#define AFX_PARMTHRESHDLG_H__C85F6B26_CC8F_47E0_80D6_5755B5CABB36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmThreshDlg dialog

class CParmThreshDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmThreshDlg)
// Construction
public:
   CParmThreshDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmThreshDlg)
   enum { IDD = IDD_PARM_THRESH };
   int      m_Cmp;
   CString  m_LevelStr;
   //}}AFX_DATA
   CString  m_ValueStr[2];
   CEdit    m_ValueEdit[2];
   CWnd     m_ValueStatic[3];
   CWnd     m_CmpWnd[3];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmThreshDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmThreshDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMTHRESHDLG_H__C85F6B26_CC8F_47E0_80D6_5755B5CABB36__INCLUDED_)
