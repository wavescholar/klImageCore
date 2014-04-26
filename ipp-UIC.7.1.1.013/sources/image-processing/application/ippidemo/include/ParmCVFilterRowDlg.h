/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVFilterRowDlg.h: interface for CParmCVFilterRowDlg class.
// CParmCVFilterRowDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCVFILTERROWDLG_H__1AEF54AF_5A78_45DD_8CEB_B69322C2000E__INCLUDED_)
#define AFX_PARMCVFILTERROWDLG_H__1AEF54AF_5A78_45DD_8CEB_B69322C2000E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParmFilterRowDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmCVFilterRowDlg dialog

class CParmCVFilterRowDlg : public CParmFilterRowDlg
{
// Construction
public:
   CParmCVFilterRowDlg(UINT nID = IDD);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCVFilterRowDlg)
   enum { IDD = IDD_PARMCV_FITERROW };
   int      m_BorderType;
   //}}AFX_DATA
   CMyString  m_ValueStr[3];
   CEdit m_ValueEdit[3];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCVFilterRowDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmCVFilterRowDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCVFILTERROWDLG_H__1AEF54AF_5A78_45DD_8CEB_B69322C2000E__INCLUDED_)
