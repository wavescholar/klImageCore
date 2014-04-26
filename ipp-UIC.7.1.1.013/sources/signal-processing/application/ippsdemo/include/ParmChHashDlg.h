
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmChHash.h: interface for CParmChHash class.
// CParmChHash dialog gets parameters for certain ippCH functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCHHASHDLG_H__2AAC164D_587D_4BEB_A7C7_79A5B2C1BCA9__INCLUDED_)
#define AFX_PARMCHHASHDLG_H__2AAC164D_587D_4BEB_A7C7_79A5B2C1BCA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CRunChHash;

/////////////////////////////////////////////////////////////////////////////
// CParmChHashDlg dialog

class CParmChHashDlg : public CParamDlg
{
   DECLARE_DYNCREATE(CParmChHashDlg)
// Construction
public:
   CParmChHashDlg(CRunChHash* pRun);   // standard constructor

// Dialog Data
   Ipp32u m_result;
   //{{AFX_DATA(CParmChHashDlg)
   enum { IDD = IDD_PARMCH_HASH };
   int      m_FuncIdx;
   //}}AFX_DATA
   CButton  m_FuncButton[3];
   CString  m_ValStr[3];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmChHashDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunChHash* m_pRun;

   void  InitFuncButtons() ;
   CFunc GetFuncName(int idx) ;
   void  SetFuncResult(int idx) ;

   // Generated message map functions
   //{{AFX_MSG(CParmChHashDlg)
   afx_msg void OnFunc();
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCHHASHDLG_H__2AAC164D_587D_4BEB_A7C7_79A5B2C1BCA9__INCLUDED_)
