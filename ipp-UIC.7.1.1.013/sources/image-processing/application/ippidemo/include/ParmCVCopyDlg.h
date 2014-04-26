/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVCopyDlg.h: interface for CParmCVCopyDlg class.
// CParmCVCopyDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCVCOPYDLG_H__58838228_BB20_453E_A008_E5D4B4A3F4AB__INCLUDED_)
#define AFX_PARMCVCOPYDLG_H__58838228_BB20_453E_A008_E5D4B4A3F4AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmCVCopyDlg dialog

class CParmCVCopyDlg : public CParamDlg
{
// Construction
public:
   CParmCVCopyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmCVCopyDlg)
   enum { IDD = IDD_PARMCV_COPY };
   CString  m_dxStr;
   CString  m_dyStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCVCopyDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmCVCopyDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCVCOPYDLG_H__58838228_BB20_453E_A008_E5D4B4A3F4AB__INCLUDED_)
