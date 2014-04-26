/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCopyManagedDlg.h: interface for CParmCopyManagedDlg class.
// CParmCopyManagedDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCOPYMANAGEDTDLG_H__3C73C49A_A787_4DCF_8166_CDF3C94DDC27__INCLUDED_)
#define AFX_PARMCOPYMANAGEDTDLG_H__3C73C49A_A787_4DCF_8166_CDF3C94DDC27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmCopyManagedDlg dialog

class CParmCopyManagedDlg : public CParamDlg
{
// Construction
public:
   CParmCopyManagedDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCopyManagedDlg)
   enum { IDD = IDD_PARM_COPYMANAG };
   int  m_flags;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCopyManagedDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   BOOL  m_Flag[2];

   // Generated message map functions
   //{{AFX_MSG(CParmCopyManagedDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCOPYMANAGEDTDLG_H__3C73C49A_A787_4DCF_8166_CDF3C94DDC27__INCLUDED_)
