/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmStatCountDlg.h: interface for CParmStatCountDlg class.
// CParmStatCountDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMSTATCOUNTDLG_H__DC932829_C004_46BD_970C_533E691F4EA8__INCLUDED_)
#define AFX_PARMSTATCOUNTDLG_H__DC932829_C004_46BD_970C_533E691F4EA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CRunStatCount;

/////////////////////////////////////////////////////////////////////////////
// CParmStatCountDlg dialog

class CParmStatCountDlg : public CParamDlg
{
// Construction
public:
   CParmStatCountDlg(CRunStatCount* pRun);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmStatCountDlg)
   enum { IDD = IDD_PARM_STAT_COUNT };
   //}}AFX_DATA
   CEdit m_valueEdit[3];
   CMyString m_valueStr[3];
   double m_value[3];
   CEdit m_xEdit[3];
   CEdit m_yEdit[3];
   CMyString m_xStr[3];
   CMyString m_yStr[3];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmStatCountDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   void ShowValues();
   CRunStatCount* m_pRun;

   // Generated message map functions
   //{{AFX_MSG(CParmStatCountDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnChangeXY();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMSTATCOUNTDLG_H__DC932829_C004_46BD_970C_533E691F4EA8__INCLUDED_)
