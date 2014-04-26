
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmChFindC.h: interface for CParmChFindC class.
// CParmChFindC dialog gets parameters for certain ippCH functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCHFINDCDLG_H__363299AF_9CA6_40F7_BAC1_E596E818165D__INCLUDED_)
#define AFX_PARMCHFINDCDLG_H__363299AF_9CA6_40F7_BAC1_E596E818165D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParmChValueDlg.h"
class CRunChFindC;

/////////////////////////////////////////////////////////////////////////////
// CParmChFindCDlg dialog

class CParmChFindCDlg : public CParmChValueDlg
{
   DECLARE_DYNCREATE(CParmChFindCDlg)
// Construction
public:
   CParmChFindCDlg(CRunChFindC* pRun);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmChFindCDlg)
   enum { IDD = IDD_PARMCH_FINDC };
   int      m_index;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmChFindCDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:
   CRunChFindC* m_pRun;

   // Generated message map functions
   //{{AFX_MSG(CParmChFindCDlg)
   virtual BOOL OnInitDialog();
   afx_msg void OnChangeChar();
   afx_msg void OnChangeCode();
   afx_msg void OnSymbol();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCHFINDCDLG_H__363299AF_9CA6_40F7_BAC1_E596E818165D__INCLUDED_)
