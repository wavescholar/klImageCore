/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmCVNormDlg.h: interface for CParmCVNormDlg class.
// CParmCVNormDlg dialog gets parameters for certain ippCV functions.
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCVNORMDLG_H__2CC6580C_3354_4BBC_B32F_7B0BBEF0CAC8__INCLUDED_)
#define AFX_PARMCVNORMDLG_H__2CC6580C_3354_4BBC_B32F_7B0BBEF0CAC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CRunCVNorm;

/////////////////////////////////////////////////////////////////////////////
// CParmCVNormDlg dialog

class CParmCVNormDlg : public CParamDlg
{
// Construction
public:
   CParmCVNormDlg(CRunCVNorm* pRun, UINT nID = IDD);   // standard constructor

// Dialog Data
   CRunCVNorm* m_pRun;
   //{{AFX_DATA(CParmCVNormDlg)
   enum { IDD = IDD_PARMCV_NORM };
   CSpinButtonCtrl   m_CoiSpin;
   CStatic  m_CoiStatic;
   CEdit m_CoiEdit;
   int      m_FuncIdx;
   CString  m_ValueStr;
   CString  m_CoiStr;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmCVNormDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   virtual CFunc GetFunc(int idx);
   virtual int GetFuncNumber() { return 3;}
   int GetFuncIndex();
   void EnableCoi();

   // Generated message map functions
   //{{AFX_MSG(CParmCVNormDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnFunc();
   afx_msg void OnChangeCoi();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCVNORMDLG_H__2CC6580C_3354_4BBC_B32F_7B0BBEF0CAC8__INCLUDED_)
