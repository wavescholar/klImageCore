/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCmpDlg.h: interface for CParmCmpDlg class.
// CParmCmpDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCMPDLG_H__918FBD71_7691_4251_9B3A_FBA9EF4DB13A__INCLUDED_)
#define AFX_PARMCMPDLG_H__918FBD71_7691_4251_9B3A_FBA9EF4DB13A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CParmCmpDlg dialog

#include "ParamDlg.h"

class CParmCmpDlg : public CParamDlg
{
// Construction
public:
   CParmCmpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   int m_numCoi;
   //{{AFX_DATA(CParmCmpDlg)
   enum { IDD = IDD_PARM_CMP };
   CEdit m_EpsEdit;
   CButton  m_ValueStatic;
   CStatic  m_EpsStatic;
   CButton  m_CmpStatic;
   int      m_Cmp;
   CString  m_EpsStr;
   //}}AFX_DATA
   CMyString   m_ValueStr[4];
   CEdit m_ValueEdit[4];
   CButton  m_CmpButton[5];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCmpDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmCmpDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCMPDLG_H__918FBD71_7691_4251_9B3A_FBA9EF4DB13A__INCLUDED_)
