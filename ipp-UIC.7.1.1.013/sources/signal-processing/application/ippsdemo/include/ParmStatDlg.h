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
// CParmStatDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMSTATDLG_H__9A7FE7F2_6BDB_434A_861F_C7EA629B85FD__INCLUDED_)
#define AFX_PARMSTATDLG_H__9A7FE7F2_6BDB_434A_861F_C7EA629B85FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CRunStat;

/////////////////////////////////////////////////////////////////////////////
// CParmStatDlg dialog

class CParmStatDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmStatDlg)
// Construction
public:
   CParmStatDlg(CRunStat* pRun);   // standard constructor

   enum { 
      Sum, Mean, StdDev,
      FuncNum};
   enum {_16s, _16s32s, TypeNum};
   enum {_16sc, _16sc32sc};
// Dialog Data
   //{{AFX_DATA(CParmStatDlg)
   enum { IDD = IDD_PARM_STAT };
   CButton  m_HintStatic;
   int      m_FuncIndex;
   CButton  m_TypeStatic;
   int      m_DstType;
   double   m_Im;
   CEdit m_ImEdit;
   CStatic  m_ImStatic;
   double   m_Re;
   CEdit m_ReEdit;
   CStatic  m_ReStatic;
   int      m_hint;
   //}}AFX_DATA
   CButton  m_FuncButton[FuncNum];
   CButton  m_TypeButton[TypeNum];
   CButton  m_HintButton[3];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmStatDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunStat* m_pRun;

   void InitFunc();
   void EnableFunc();
   void EnableValues();
   void EnableType();
   void EnableHint();
   void ShowHint();
   void ShowType();
   void ShowTypeButton(int i, CMyString text);
   void SetFunc();
   CFunc GetFunc(int index, int dstType) ;
   int FuncIndex(CMyString funcName);
   CMyString FuncBase(int idx);
   int TypeIndex(CMyString funcName);
   CMyString TypeName(int idx);
   BOOL TypeFlag(CMyString funcBase);

   // Generated message map functions
   //{{AFX_MSG(CParmStatDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnFunc();
   afx_msg void OnChangeScaleEdit();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMSTATDLG_H__9A7FE7F2_6BDB_434A_861F_C7EA629B85FD__INCLUDED_)
