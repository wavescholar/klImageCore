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
// CParmNormDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMNORMDLG_H__E293783A_DF9D_421E_AF23_5EE9B493503B__INCLUDED_)
#define AFX_PARMNORMDLG_H__E293783A_DF9D_421E_AF23_5EE9B493503B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CRunNorm;

/////////////////////////////////////////////////////////////////////////////
// CParmNormDlg dialog

class CParmNormDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmNormDlg)
// Construction
public:
   CParmNormDlg(CRunNorm* pRun, UINT nID = IDD);   // standard constructor

   enum {_Inf, _L1, _L2, _L2Sqr, FuncNum};
   enum {_32f, _32s, _64f, _64s, TypeNum};
// Dialog Data
   //{{AFX_DATA(CParmNormDlg)
   enum { IDD = IDD_PARM_NORM };
   CButton  m_TypeStatic;
   int      m_FuncIndex;
   int      m_TypeIndex;
   double   m_Value;
   //}}AFX_DATA
   CButton  m_FuncButton[FuncNum];
   CButton  m_TypeButton[TypeNum];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmNormDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunNorm* m_pRun;
   BOOL m_bDiff;

   void InitFunc() ;
   void SetFunc() ;
   int FuncIndex(CString funcName);
   CString FuncBase(int idx);
   int TypeIndex(CString funcName);
   CString TypeName(int idx);
   CFunc CreateFuncName(int funcIndex, int typeIndex);
   void EnableSrc2();
   void EnableFunc();
   void EnableType();

   // Generated message map functions
   //{{AFX_MSG(CParmNormDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnFunc();
   afx_msg void OnChangeScaleEdit();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMNORMDLG_H__E293783A_DF9D_421E_AF23_5EE9B493503B__INCLUDED_)
