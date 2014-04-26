/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmNormlzeDlg.h: interface for CParmNormlzeDlg class.
// CParmNormlzeDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMNORMLZEDLG_H__96A289F8_3BB4_4F88_BD81_487833A7861B__INCLUDED_)
#define AFX_PARMNORMLZEDLG_H__96A289F8_3BB4_4F88_BD81_487833A7861B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmNormlzeDlg dialog

class CParmNormlzeDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmNormlzeDlg)
// Construction
public:
   CParmNormlzeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmNormlzeDlg)
   enum { IDD = IDD_PARM_NORMZE };
   CString  m_DivStr;
   //}}AFX_DATA
   CStatic  m_SubStatic[2];
   CEdit m_SubEdit[2];
   CString  m_SubStr[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmNormlzeDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmNormlzeDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMNORMLZEDLG_H__96A289F8_3BB4_4F88_BD81_487833A7861B__INCLUDED_)
