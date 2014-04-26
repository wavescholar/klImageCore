/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFIRGenDlg.h: interface for CParmFIRGenDlg class.
// CParmFIRGenDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFIRGENDLG_H__763EE92B_99E0_4F2F_BB89_2AC3478C66B0__INCLUDED_)
#define AFX_PARMFIRGENDLG_H__763EE92B_99E0_4F2F_BB89_2AC3478C66B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParamDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CParmFIRGenDlg dialog

class CParmFIRGenDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmFIRGenDlg)
// Construction
public:
   CParmFIRGenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmFIRGenDlg)
   enum { IDD = IDD_PARM_FIRGEN };
   BOOL  m_doNormal;
   int      m_winType;
   //}}AFX_DATA
   CStatic  m_FreqStatic[2];
   CEdit m_FreqEdit[2];
   CString  m_FreqStr[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFIRGenDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmFIRGenDlg)
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFIRGENDLG_H__763EE92B_99E0_4F2F_BB89_2AC3478C66B0__INCLUDED_)
