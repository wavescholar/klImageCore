/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmConvert1uDlg.h: interface for CParmConvert1uDlg class.
// CParmConvert1uDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCONVERT1UDLG_H__F66E15FB_D3FD_48DB_AEF4_BE81098A063D__INCLUDED_)
#define AFX_PARMCONVERT1UDLG_H__F66E15FB_D3FD_48DB_AEF4_BE81098A063D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmConvert1uDlg dialog

class CParmConvert1uDlg : public CParamDlg
{
// Construction
public:
   CParmConvert1uDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmConvert1uDlg)
   enum { IDD = IDD_PARM_CONVERT_1U };
   CEdit m_ThreshEdit;
   CStatic  m_ThreshStatic;
   CSpinButtonCtrl   m_ThreshSpin;
   CString  m_ThreshStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmConvert1uDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmConvert1uDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCONVERT1UDLG_H__F66E15FB_D3FD_48DB_AEF4_BE81098A063D__INCLUDED_)
