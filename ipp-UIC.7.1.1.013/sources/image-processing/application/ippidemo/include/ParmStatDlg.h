/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmStatDlg.h: interface for CParmStatDlg class.
// CParmStatDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMSTATDLG_H__73C220F8_E0A0_4ED6_8DAC_118953DEFB48__INCLUDED_)
#define AFX_PARMSTATDLG_H__73C220F8_E0A0_4ED6_8DAC_118953DEFB48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

class CRunStat;

/////////////////////////////////////////////////////////////////////////////
// CParmStatDlg dialog

class CParmStatDlg : public CParamDlg
{
// Construction
public:
   CParmStatDlg(CRunStat* pRun);   // standard constructor

   enum {
      Min, Max, MinMax, Sum, Mean,
      FuncNum};
// Dialog Data
   //{{AFX_DATA(CParmStatDlg)
   enum { IDD = IDD_PARM_STAT };
   CButton  m_valueStatic;
   CButton  m_value2Static;
   CButton  m_HintStatic;
   int      m_Hint;
   int      m_FuncIndex;
   //}}AFX_DATA
   CButton m_HintButton[3];
   CButton m_FuncButton[FuncNum];
   CEdit m_valueEdit[4];
   CEdit m_value2Edit[4];
   CMyString m_valueStr[4];
   CMyString m_value2Str[4];
   double m_value[4];
   double m_value2[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmStatDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunStat* m_pRun;

   int FuncIndex(CString funcName);
   CString FuncBase(int idx);
   void InitFunc();
   void ShowValues();
   void EnableHint();
   void SetFunc();

   // Generated message map functions
   //{{AFX_MSG(CParmStatDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnHint();
   afx_msg void OnFunc();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMSTATDLG_H__73C220F8_E0A0_4ED6_8DAC_118953DEFB48__INCLUDED_)
