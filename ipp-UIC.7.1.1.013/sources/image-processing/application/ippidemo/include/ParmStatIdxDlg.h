/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmStatIdxDlg.h: interface for CParmStatIdxDlg class.
// CParmStatIdxDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMSTATIDXDLG_H__C8A80884_DF8A_4E3B_AE43_DF69E7869B64__INCLUDED_)
#define AFX_PARMSTATIDXDLG_H__C8A80884_DF8A_4E3B_AE43_DF69E7869B64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CRunStatIdx;

/////////////////////////////////////////////////////////////////////////////
// CParmStatIdxDlg dialog

class CParmStatIdxDlg : public CParamDlg
{
// Construction
public:
   CParmStatIdxDlg(CRunStatIdx* pRun);   // standard constructor

   enum {
      MinIndx, MaxIndx,
      FuncNum};

// Dialog Data
   //{{AFX_DATA(CParmStatIdxDlg)
   enum { IDD = IDD_PARM_STAT_IDX };
   CButton  m_valueStatic;
   int      m_FuncIndex;
   //}}AFX_DATA
   CButton m_FuncButton[FuncNum];
   CEdit m_valueEdit[4];
   CMyString m_valueStr[4];
   double m_value[4];
   CEdit m_xEdit[4];
   CEdit m_yEdit[4];
   CMyString m_xStr[4];
   CMyString m_yStr[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmStatIdxDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunStatIdx* m_pRun;

   int FuncIndex(CString funcName);
   CString FuncBase(int idx);
   void InitFunc();
   void SetFunc();
   void ShowValues();

   // Generated message map functions
   //{{AFX_MSG(CParmStatIdxDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnFunc();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMSTATIDXDLG_H__C8A80884_DF8A_4E3B_AE43_DF69E7869B64__INCLUDED_)
