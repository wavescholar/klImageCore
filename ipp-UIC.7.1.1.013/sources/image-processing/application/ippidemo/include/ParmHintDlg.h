/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmHintDlg.h: interface for CParmHintDlg class.
// CParmHintDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMHINTDLG_H__3C73C49A_A787_4DCF_8166_CDF3C94DDC27__INCLUDED_)
#define AFX_PARMHINTDLG_H__3C73C49A_A787_4DCF_8166_CDF3C94DDC27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmHintDlg dialog

class CParmHintDlg : public CParamDlg
{
// Construction
public:
   CParmHintDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmHintDlg)
   enum { IDD = IDD_PARM_HINT };
   int      m_Hint;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmHintDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmHintDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMHINTDLG_H__3C73C49A_A787_4DCF_8166_CDF3C94DDC27__INCLUDED_)
