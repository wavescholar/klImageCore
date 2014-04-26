/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmMinDlg.h: interface for CParmMinDlg class.
// CParmMinDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMMINDLG_H__9A7FE7F2_6BDB_434A_861F_C7EA629B85FD__INCLUDED_)
#define AFX_PARMMINDLG_H__9A7FE7F2_6BDB_434A_861F_C7EA629B85FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CRunMin;

/////////////////////////////////////////////////////////////////////////////
// CParmMinDlg dialog

class CParmMinDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmMinDlg)
// Construction
public:
   CParmMinDlg(CRunMin* pRun);   // standard constructor

   enum { 
      Min, Max, MinIndx, MaxIndx, 
      MinAbs, MaxAbs, MinAbsIndx, MaxAbsIndx, 
      MaxOrder,
      FuncNum};
// Dialog Data
   //{{AFX_DATA(CParmMinDlg)
   enum { IDD = IDD_PARM_MIN };
   CStatic  m_IndexStatic;
   CEdit m_IndexEdit;
   int      m_FuncIndex;
   int      m_Index;
   double   m_Im;
   CEdit m_ImEdit;
   CStatic  m_ImStatic;
   double   m_Re;
   CEdit m_ReEdit;
   CStatic  m_ReStatic;
   //}}AFX_DATA
   CButton  m_FuncButton[FuncNum];

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmMinDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunMin* m_pRun;

   void InitFunc();
   void EnableFunc();
   void EnableValues();
   void EnableIndex();
   void SetFunc();
   CFunc GetFunc(int index) ;
   int FuncIndex(CMyString funcName);
   CMyString FuncBase(int idx);

   // Generated message map functions
   //{{AFX_MSG(CParmMinDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnFunc();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMMINDLG_H__9A7FE7F2_6BDB_434A_861F_C7EA629B85FD__INCLUDED_)
