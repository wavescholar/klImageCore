/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMirrorDlg.h: interface for CParmMirrorDlg class.
// CParmMirrorDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMMIRRORDLG_H__030C9F05_1E4F_11D3_8F2A_00AA00A03C3C__INCLUDED_)
#define AFX_PARMMIRRORDLG_H__030C9F05_1E4F_11D3_8F2A_00AA00A03C3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmMirrorDlg dialog

class CParmMirrorDlg : public CParamDlg
{
// Construction
public:
   CParmMirrorDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmMirrorDlg)
   enum { IDD = IDD_PARM_MIRROR };
   int      m_Flip;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmMirrorDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmMirrorDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMMIRRORDLG_H__030C9F05_1E4F_11D3_8F2A_00AA00A03C3C__INCLUDED_)
