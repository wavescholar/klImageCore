
/*
//
//               INTEL CORPORATION PROPRIETARY INFORMATION
//  This software is supplied under the terms of a license agreement or
//  nondisclosure agreement with Intel Corporation and may not be copied
//  or disclosed except in accordance with the terms of that agreement.
//        Copyright(c) 2005-2012 Intel Corporation. All Rights Reserved.
//
*/

// ParmChFind.h: interface for CParmChFind class.
// CParmChFind dialog gets parameters for certain ippCH functions.
// 
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARMCHFINDDLG_H__C41F5F7A_17D8_4BCE_B4A4_AF2DD24F61A0__INCLUDED_)
#define AFX_PARMCHFINDDLG_H__C41F5F7A_17D8_4BCE_B4A4_AF2DD24F61A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ParamDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CParmChFindDlg dialog

class CParmChFindDlg : public CParamDlg
{
   DECLARE_DYNCREATE(CParmChFindDlg)
// Construction
public:
   CParmChFindDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
   //{{AFX_DATA(CParmChFindDlg)
   enum { IDD = IDD_PARMCH_FIND };
   int      m_index;
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CParmChFindDlg)
   protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(CParmChFindDlg)
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARMCHFINDDLG_H__C41F5F7A_17D8_4BCE_B4A4_AF2DD24F61A0__INCLUDED_)
