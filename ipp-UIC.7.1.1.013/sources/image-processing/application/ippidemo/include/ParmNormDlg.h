/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmNormDlg.h: interface for CParmNormDlg class.
// CParmNormDlg dialog gets parameters for certain ippIP functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMNORMDLG_H__16A831EC_59B9_46CF_9E87_BE44B6BB35CE__INCLUDED_)
#define AFX_PARMNORMDLG_H__16A831EC_59B9_46CF_9E87_BE44B6BB35CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

class CRunNorm;

/////////////////////////////////////////////////////////////////////////////
// CParmNormDlg dialog

class CParmNormDlg : public CParamDlg
{
// Construction
public:
   CParmNormDlg(CRunNorm* pRun, UINT nID = IDD);   // standard constructor


// Dialog Data
   CRunNorm* m_pRun;
   //{{AFX_DATA(CParmNormDlg)
   enum { IDD = IDD_PARM_NORM };
   CButton  m_HintStatic;
   int      m_Hint;
   int      m_Norm;
   //}}AFX_DATA
   CButton  m_HintButton[3];
   CButton  m_NormButton[7];
   CEdit m_ValueEdit[4];
   double   m_Value[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmNormDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   virtual int NormIndex(CString funcName);
   virtual CString NormFunc(int idx);
   void InitNorm();
   void EnableValues();
   void EnableHint();
   void SetFunc();

   // Generated message map functions
   //{{AFX_MSG(CParmNormDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnHint();
   afx_msg void OnNorm();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMNORMDLG_H__16A831EC_59B9_46CF_9E87_BE44B6BB35CE__INCLUDED_)
