/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmRampDlg.h: interface for CParmRampDlg class.
// CParmRampDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMRAMPDLG_H__9EEA0D98_161A_430B_ABBB_2F4E5563D4BA__INCLUDED_)
#define AFX_PARMRAMPDLG_H__9EEA0D98_161A_430B_ABBB_2F4E5563D4BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmRampDlg dialog

class CParmRampDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmRampDlg)
// Construction
public:
   CParmRampDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmRampDlg)
   enum { IDD = IDD_PARM_RAMP };
   CString  m_Val0Str;
   CString  m_Val0Name;
   CString  m_Val1Name;
   CString  m_Val1Str;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmRampDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmRampDlg)
      // NOTE: the ClassWizard will add member functions here
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMRAMPDLG_H__9EEA0D98_161A_430B_ABBB_2F4E5563D4BA__INCLUDED_)
