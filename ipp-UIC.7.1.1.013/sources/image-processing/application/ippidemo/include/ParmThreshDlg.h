/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmThreshDlg.h: interface for CParmThreshDlg class.
// CParmThreshDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMTHRESHDLG_H__81571BCC_17AC_4C42_B060_A0B748360A19__INCLUDED_)
#define AFX_PARMTHRESHDLG_H__81571BCC_17AC_4C42_B060_A0B748360A19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmThreshDlg dialog

class CParmThreshDlg : public CParamDlg
{
// Construction
public:
   CParmThreshDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmThreshDlg)
   enum { IDD = IDD_PARM_THRESH };
   CButton  m_ValueGTStatic;
   CButton  m_ValueLTStatic;
   CButton  m_ThreshGTStatic;
   CButton  m_ThreshLTStatic;
   CButton  m_CmpStatic;
   int      m_cmpOp;
   //}}AFX_DATA
   CButton  m_CmpButton[5];
   CMyString   m_ThreshLTStr[4];
   CMyString   m_ThreshGTStr[4];
   CEdit m_ThreshLTEdit[4];
   CEdit m_ThreshGTEdit[4];
   CEdit m_ValueLTEdit[4];
   CEdit m_ValueGTEdit[4];
   CMyString   m_ValueLTStr[4];
   CMyString   m_ValueGTStr[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmThreshDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   BOOL m_bCmp;
   BOOL m_bLT;
   BOOL m_bGT;
   BOOL m_bVal;

   void EnableThreshLT();
   void EnableThreshGT();
   void EnableValueLT();
   void EnableValueGT();
   void EnableCmp();
   void EnableEdit(CEdit wnd[], CButton& frame, BOOL flag) ;

   // Generated message map functions
   //{{AFX_MSG(CParmThreshDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMTHRESHDLG_H__81571BCC_17AC_4C42_B060_A0B748360A19__INCLUDED_)
