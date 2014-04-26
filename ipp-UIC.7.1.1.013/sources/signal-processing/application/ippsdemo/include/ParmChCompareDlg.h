
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmChCompare.h: interface for CParmChCompare class.
// CParmChCompare dialog gets parameters for certain ippCH functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCHCOMPAREDLG_H__02EF9F08_DD08_4BD7_A31B_93C06BF8847F__INCLUDED_)
#define AFX_PARMCHCOMPAREDLG_H__02EF9F08_DD08_4BD7_A31B_93C06BF8847F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"
class CRunChCompare;
/////////////////////////////////////////////////////////////////////////////
// CParmChCompareDlg dialog

class CParmChCompareDlg : public CParamDlg
{
   DECLARE_DYNCREATE(CParmChCompareDlg)
// Construction
public:
   CParmChCompareDlg(CRunChCompare* pRun);   // standard constructor

// Dialog Data
   int m_Comp;
   //{{AFX_DATA(CParmChCompareDlg)
   enum { IDD = IDD_PARMCH_COMPARE };
   CString  m_CompStr;
   int      m_FuncIdx;
   //}}AFX_DATA
   CButton  m_FuncButton[4];


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmChCompareDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunChCompare* m_pRun;
   void  InitFuncButtons() ;
   CFunc GetFuncName(int idx) ;
   void  SetCompStr() ;

   // Generated message map functions
   //{{AFX_MSG(CParmChCompareDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnFunc();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCHCOMPAREDLG_H__02EF9F08_DD08_4BD7_A31B_93C06BF8847F__INCLUDED_)
