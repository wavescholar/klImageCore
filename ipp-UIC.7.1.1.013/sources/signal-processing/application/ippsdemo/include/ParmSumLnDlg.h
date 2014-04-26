/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 1999-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmSumLnDlg.h: interface for CParmSumLnDlg class.
// CParmSumLnDlg dialog gets parameters for certain ippSP functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMSUMLNDLG_H__3E0D4030_ADB3_4BF1_8E24_67A4A2EF4845__INCLUDED_)
#define AFX_PARMSUMLNDLG_H__3E0D4030_ADB3_4BF1_8E24_67A4A2EF4845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ParamDlg.h"
class CRunSumLn;

/////////////////////////////////////////////////////////////////////////////
// CParmSumLnDlg dialog

class CParmSumLnDlg : public CParamDlg
{
   DECLARE_DYNAMIC(CParmSumLnDlg)
// Construction
public:
   CParmSumLnDlg(CRunSumLn* pRun);  

   enum {_32f, _32f64f, TypeNum};
// Dialog Data
   //{{AFX_DATA(CParmSumLnDlg)
   enum { IDD = IDD_PARM_SUMLN };
   CButton  m_TypeStatic;
   int      m_DstType;
   CString  m_ValStr;
   //}}AFX_DATA
   CButton  m_TypeButton[TypeNum];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmSumLnDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunSumLn* m_pRun;

   void InitType() ;
   void SetFunc() ;
   CString TypeName(int dstType) ;
   int TypeIndex(CFunc func);
   void ShowType();
   void ShowTypeButton(int i, CString text);

   // Generated message map functions
   //{{AFX_MSG(CParmSumLnDlg)
   virtual BOOL OnInitDialog();
   virtual void OnOK();
   afx_msg void OnType();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMSUMLNDLG_H__3E0D4030_ADB3_4BF1_8E24_67A4A2EF4845__INCLUDED_)
