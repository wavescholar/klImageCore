/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmC13Dlg.h: interface for CParmC13Dlg class.
// CParmC13Dlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMC13DLG_H__E37B6927_A01C_4805_BAEC_6EE759ACE234__INCLUDED_)
#define AFX_PARMC13DLG_H__E37B6927_A01C_4805_BAEC_6EE759ACE234__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmC13Dlg dialog

class CParmC13Dlg : public CParamDlg
{
// Construction
public:
   CParmC13Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmC13Dlg)
   enum { IDD = IDD_PARM_C13 };
   CButton  m_srcChannelStatic;
   CButton  m_dstChannelStatic;
   int      m_srcChannel;
   int      m_dstChannel;
   //}}AFX_DATA
   CButton  m_dstChannelButton[4];
   CButton  m_srcChannelButton[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmC13Dlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmC13Dlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMC13DLG_H__E37B6927_A01C_4805_BAEC_6EE759ACE234__INCLUDED_)
