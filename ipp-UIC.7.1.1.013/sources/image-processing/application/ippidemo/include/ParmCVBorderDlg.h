/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVBorderDlg.h: interface for CParmCVBorderDlg class.
// CParmCVBorderDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCVBORDERDLG_H__8B05EDD7_809B_47D9_9365_702C1EF7ABEF__INCLUDED_)
#define AFX_PARMCVBORDERDLG_H__8B05EDD7_809B_47D9_9365_702C1EF7ABEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmCVBorderDlg dialog

class CParmCVBorderDlg : public CParamDlg
{
// Construction
public:
   CParmCVBorderDlg(UINT nID = IDD);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCVBorderDlg)
   enum { IDD = IDD_PARMCV_BORDER };
   CEdit m_ValueEdit;
   int      m_BorderType;
   CString  m_ValueStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCVBorderDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmCVBorderDlg)
   afx_msg void OnBorder();
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCVBORDERDLG_H__8B05EDD7_809B_47D9_9365_702C1EF7ABEF__INCLUDED_)
