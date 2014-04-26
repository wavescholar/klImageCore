/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmFilterCustomDlg.h: interface for CParmFilterCustomDlg class.
// CParmFilterCustomDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMFILTERCUSTOMDLG_H__48FC38C3_E093_4CBB_B58F_77BB1B5AE7A6__INCLUDED_)
#define AFX_PARMFILTERCUSTOMDLG_H__48FC38C3_E093_4CBB_B58F_77BB1B5AE7A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParmFilterBoxDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmFilterCustomDlg dialog

class CParmFilterCustomDlg : public CParmFilterBoxDlg
{
// Construction
public:
   CParmFilterCustomDlg(UINT nID = IDD, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   CVector m_kernel;
   enum {N_MAX = 7};
   //{{AFX_DATA(CParmFilterCustomDlg)
   enum { IDD = IDD_PARM_FILCUST };
   BOOL m_bDiv;
   BOOL m_bRound;
   CString  m_DivStr;
   int      m_DivType;
   int m_roundMode;
   //}}AFX_DATA
   CEdit m_KernelEdit[N_MAX][N_MAX];
   CString  m_KernelStr[N_MAX][N_MAX];
   CButton  m_DivStatic;
   CButton  m_DivRadio[2];
   CEdit    m_DivEdit;
   CSpinButtonCtrl   m_DivSpin;
   CButton  m_RoundStatic;
   CButton  m_RoundRadio[2];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmFilterCustomDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   int m_MaxSize[2];

   void ShowDivisor();
   void ShowRound();
   void ShowKernel();
   void UpdateKernel(BOOL bSave = TRUE);
   void UpdateDivisor();
   int GetStart(int i);
   int GetStop(int i);

   // Generated message map functions
   //{{AFX_MSG(CParmFilterCustomDlg)
   afx_msg void OnChangeSize();
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnChangeKernel();
   afx_msg void OnDivRadio();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMFILTERCUSTOMDLG_H__48FC38C3_E093_4CBB_B58F_77BB1B5AE7A6__INCLUDED_)
