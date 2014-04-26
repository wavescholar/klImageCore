/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmGoerDlg.h: interface for CParmGoerDlg class.
// CParmGoerDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMGOERDLG_H__3B8BD0AD_9E59_4E41_A3B6_B0A1477E53C0__INCLUDED_)
#define AFX_PARMGOERDLG_H__3B8BD0AD_9E59_4E41_A3B6_B0A1477E53C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CRunGoer;
/////////////////////////////////////////////////////////////////////////////
// CParmGoerDlg dialog

class CParmGoerDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmGoerDlg)
// Construction
public:
   CParmGoerDlg(CRunGoer* pRun);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmGoerDlg)
   enum { IDD = IDD_PARM_GOER };
   CButton  m_ApplyButton;
   //}}AFX_DATA
   CButton  m_ValStatic[2];
   CEdit    m_ReEdit[2];
   CEdit    m_ImEdit[2];
   CEdit    m_FreqEdit[2];
   CStatic  m_ReStatic[2];
   CStatic  m_ImStatic[2];
   CStatic  m_FreqStatic[2];
   CString  m_FreqStr[2];
   double   m_Im[2];
   double   m_Re[2];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmGoerDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunGoer* m_pRun;

   // Generated message map functions
   //{{AFX_MSG(CParmGoerDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnChange();
   afx_msg void OnApply();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMGOERDLG_H__3B8BD0AD_9E59_4E41_A3B6_B0A1477E53C0__INCLUDED_)
